#include "Widgets/MainMenu.h"

#include "Commons/EOSGameInstance.h"
#include "Components/Button.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = GetGameInstance<UEOSGameInstance>();

	LoginBtn->OnClicked.AddDynamic(this, &ThisClass::LoginBtnClicked);
	FriendsBtn->OnClicked.AddDynamic(this, &ThisClass::FriendsBtnClicked);
}

void UMainMenu::LoginBtnClicked()
{
	if(GameInstance)
	{
		GameInstance->Login();
	}
}

void UMainMenu::FriendsBtnClicked()
{
	if(GameInstance)
	{
		//GameInstance->GetAllFriends();
	}
}
