#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "EOSGameInstance.generated.h"

class IOnlineSubsystem;

UCLASS()
class API_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

	IOnlineSubsystem* OnlineSubsystem;
	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> IdentityPtr;

public:
	virtual void Init() override;
	void Login();

private:
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
};
