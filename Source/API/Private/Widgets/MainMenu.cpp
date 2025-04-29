#include "Widgets/MainMenu.h"

#include "Commons/EOSGameInstance.h"
#include "Components/Button.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = GetGameInstance<UEOSGameInstance>();

	LoginBtn->OnClicked.AddDynamic(this, &ThisClass::LoginBtnClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &ThisClass::CreateSessionBtnClicked);
	FindSessionBtn->OnClicked.AddDynamic(this, &ThisClass::FindSessionBtnClicked);
	// JoinSessionBtn->OnClicked.AddDynamic(this, &ThisClass::JoinSessionBtnClicked);
	FriendsBtn->OnClicked.AddDynamic(this, &ThisClass::FriendsBtnClicked);
	AchievementsBtn->OnClicked.AddDynamic(this, &ThisClass::AchievementsBtnClicked);
}

void UMainMenu::LoginBtnClicked()
{
	if(GameInstance)
	{
		GameInstance->Login();
	}
}

void UMainMenu::CreateSessionBtnClicked()
{
	if(GameInstance)
	{
		GameInstance->CreateSession();
	}
}

void UMainMenu::FindSessionBtnClicked()
{
	if(GameInstance)
	{
		GameInstance->FindSession();
	}
}

// void UMainMenu::JoinSessionBtnClicked()
// {
// 	if(GameInstance)
// 	{
// 		//GameInstance->JoinSession();
// 	}
// }

void UMainMenu::FriendsBtnClicked()
{
	if(GameInstance)
	{
		GameInstance->GetAllFriends();
	}
}

void UMainMenu::AchievementsBtnClicked()
{
	if(GameInstance)
	{
		GameInstance->GetAllAchievements();
	}
}
