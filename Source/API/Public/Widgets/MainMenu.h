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
	UButton* FriendsBtn;

	UFUNCTION()
	void LoginBtnClicked();

	UFUNCTION()
	void FriendsBtnClicked();
};
