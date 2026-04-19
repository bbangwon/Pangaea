// Fill out your copyright notice in the Description page of Project Settings.


#include "PangaeaGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPangaeaGameInstance::StartListenServer()
{	
	auto World = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(World, "TopDownMap", true, "?listen");
}

void UPangaeaGameInstance::JoinAsClient(const FString & IpAddress)
{
	auto World = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(World, *IpAddress, true, "?join");
}

void UPangaeaGameInstance::LeaveGame()
{
	auto World = GEngine->GetCurrentPlayWorld();
	UGameplayStatics::OpenLevel(World, "LobbyMap");
}
