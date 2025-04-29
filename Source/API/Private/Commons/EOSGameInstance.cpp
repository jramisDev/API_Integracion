#include "Commons/EOSGameInstance.h"

#include "EOSShared.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"

void UEOSGameInstance::Init()
{
    Super::Init();

    OnlineSubsystem = IOnlineSubsystem::Get();
    
    if (OnlineSubsystem)
    {
        IdentityPtr = OnlineSubsystem->GetIdentityInterface();
        IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &ThisClass::OnLoginComplete);
    }
}

void UEOSGameInstance::Login()
{
    if(IdentityPtr)
    {
        FOnlineAccountCredentials AccountCredentials;
        AccountCredentials.Type = TEXT("AccountPortal");
        AccountCredentials.Id = TEXT("");
        AccountCredentials.Token = TEXT("");
        
        IdentityPtr->Login(0, AccountCredentials);
    }
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("Login successful!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Login failed: %s"), *Error);
    }
}
