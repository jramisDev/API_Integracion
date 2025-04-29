#include "Commons/EOSGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

void UEOSGameInstance::Init()
{
    Super::Init();

    OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Subsistema cargado: %s"), *OnlineSubsystem->GetSubsystemName().ToString());

        IdentityPtr = OnlineSubsystem->GetIdentityInterface();
        if (IdentityPtr.IsValid())
        {
            IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::OnLoginComplete);
        }

        SessionPtr = OnlineSubsystem->GetSessionInterface();
        SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::CreateSessionCompleted);
        SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::FindSessionCompleted);
        SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::JoinSessionCompleted);
    }
    // Llamar login automáticamente al iniciar
    Login();
}

void UEOSGameInstance::Login()
{
    if (IdentityPtr.IsValid())
    {
        FOnlineAccountCredentials Credentials;
        Credentials.Type = TEXT("AccountPortal"); // Usar el portal de cuentas de Epic
        Credentials.Id = TEXT("");
        Credentials.Token = TEXT("");

        IdentityPtr->Login(0, Credentials);
    }
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    if (bWasSuccessful)
    {
        FString IdStr = UserId.ToString();
        FString TypeStr = UserId.GetType().ToString();
        UE_LOG(LogTemp, Warning, TEXT("Login exitoso. ID: %s - Tipo: %s"), *IdStr, *TypeStr);

        GetAllFriends();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Error al iniciar sesión: %s"), *Error);
    }
}

void UEOSGameInstance::CreateSession()
{
    if(SessionPtr)
    {
        FOnlineSessionSettings NewSessionSettings;
        NewSessionSettings.bAllowInvites = true;
        NewSessionSettings.bIsDedicated = false;
        NewSessionSettings.bIsLANMatch = false;
        NewSessionSettings.bShouldAdvertise = true;
        NewSessionSettings.bUseLobbiesIfAvailable = true;
        NewSessionSettings.bUsesPresence = true;
        NewSessionSettings.bAllowJoinInProgress = true;
        NewSessionSettings.bAllowJoinViaPresence = true;
        NewSessionSettings.NumPublicConnections = 10;

        NewSessionSettings.Set(FName("LobbyName"), FString("MyFunLobby"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
        
        SessionPtr->CreateSession(0, "FunSession", NewSessionSettings);
    }
}

void UEOSGameInstance::CreateSessionCompleted(FName NameSession, bool bWasSuccessful)
{
    if(bWasSuccessful)
    {
        UE_LOG(LogTemp, Error, TEXT("Session creada correctamente: %s"), *NameSession.ToString());
    }else
    {
        UE_LOG(LogTemp, Error, TEXT("Sesion no creada"));
    }

    if(!GameLevel.IsValid())
    {
        GameLevel.LoadSynchronous();
    }
    if(GameLevel.IsValid())
    {
        const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(GameLevel.ToString()));
        GetWorld()->ServerTravel(LevelName.ToString() + "?listen");
    }
}

void UEOSGameInstance::FindSession()
{
    if(SessionPtr)
    {
        SessionSearch = MakeShareable(new FOnlineSessionSearch);
        
        SessionSearch->bIsLanQuery = false;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
        
        SessionPtr->FindSessions(0,SessionSearch.ToSharedRef());
    }
}

void UEOSGameInstance::FindSessionCompleted(bool bWasSuccessful)
{
    if(bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
    {
        for (const FOnlineSessionSearchResult& LobbyFound : SessionSearch->SearchResults)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found session withd id: %s"), *LobbyFound.GetSessionIdStr());
        }

        const FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[0];
        SessionPtr->JoinSession(0, "", SearchResult);
        
    }
}

void UEOSGameInstance::JoinSessionCompleted(FName NameSession, EOnJoinSessionCompleteResult::Type Result)
{
    if(Result == EOnJoinSessionCompleteResult::Success)
    {
        FString TravelUrl;
        SessionPtr->GetResolvedConnectString("", TravelUrl);
        GetFirstLocalPlayerController(GetWorld())->ClientTravel(TravelUrl, TRAVEL_Absolute);
    }
}

void UEOSGameInstance::GetAllFriends() const
{
    if (!OnlineSubsystem)
        return;

    IOnlineFriendsPtr FriendsInterface = OnlineSubsystem->GetFriendsInterface();
    if (FriendsInterface.IsValid())
    {
        FriendsInterface->ReadFriendsList(0, TEXT("Default"),
            FOnReadFriendsListComplete::CreateLambda([](int32 LocalPlayerNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
            {
                if (bWasSuccessful)
                {
                    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
                    IOnlineFriendsPtr Friends = Subsystem->GetFriendsInterface();

                    TArray<TSharedRef<FOnlineFriend>> FriendList;
                    if (Friends->GetFriendsList(LocalPlayerNum, ListName, FriendList))
                    {
                        for (const TSharedRef<FOnlineFriend>& Friend : FriendList)
                        {
                            FString FriendId = Friend->GetUserId()->ToString();
                            FString DisplayName = Friend->GetDisplayName();
                            UE_LOG(LogTemp, Log, TEXT("Amigo encontrado: %s (%s)"), *DisplayName, *FriendId);
                        }
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("No se pudo obtener la lista de amigos: %s"), *ErrorStr);
                }
            }));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No se pudo obtener la interfaz de amigos."));
    }
}

void UEOSGameInstance::GetAllAchievements() const
{
    if (OnlineSubsystem)
    {
        const IOnlineAchievementsPtr AchievementsPtr = OnlineSubsystem->GetAchievementsInterface();

        TArray<FOnlineAchievement> OutAchievements;
        AchievementsPtr->GetCachedAchievements(*IdentityPtr->GetUniquePlayerId(0), OutAchievements);

        for (const auto Achievement : OutAchievements)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, FString::Printf(TEXT("Nombre del jugador: %s"), *Achievement.Id));
        }
    }
}
