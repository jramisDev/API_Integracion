#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "EOSGameInstance.generated.h"

class FOnlineSessionSearch;
class IOnlineSession;
class IOnlineSubsystem;

UCLASS()
class API_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameLevel;

	IOnlineSubsystem* OnlineSubsystem;
	TSharedPtr<IOnlineIdentity, ESPMode::ThreadSafe> IdentityPtr;
	TSharedPtr<IOnlineSession, ESPMode::ThreadSafe> SessionPtr;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void CreateSessionCompleted(FName NameSession, bool bWasSuccessful);
	void FindSessionCompleted(bool bWasSuccessful);

public:
	
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void Login();

	UFUNCTION(BlueprintCallable)
	void CreateSession();

	UFUNCTION(BlueprintCallable)
	void FindSession();

	UFUNCTION(BlueprintCallable)
	void GetAllFriends() const;

	UFUNCTION(BlueprintCallable)
	void GetAllAchievements() const;
};
