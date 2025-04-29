#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"


class UButton;
class UEOSGameInstance;

UCLASS()
class API_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UEOSGameInstance* GameInstance;

private:
	
	UPROPERTY(meta = (BindWidget))
	UButton* LoginBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreateSessionBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* FindSessionBtn;
	
	// UPROPERTY(meta = (BindWidget))
	// UButton* JoinSessionBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* FriendsBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AchievementsBtn;

	UFUNCTION()
	void LoginBtnClicked();

	UFUNCTION()
	void CreateSessionBtnClicked();
	
	UFUNCTION()
	void FindSessionBtnClicked();
	
	// UFUNCTION()
	// void JoinSessionBtnClicked();

	UFUNCTION()
	void FriendsBtnClicked();

	UFUNCTION()
	void AchievementsBtnClicked();
};
