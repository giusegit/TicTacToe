// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TTT_unigeGameMode.generated.h"


/**
 * 
 */
UCLASS()
class TTT_UNIGE_API ATTT_unigeGameMode : public AGameMode
{
	GENERATED_BODY()



	ATTT_unigeGameMode();

	~ATTT_unigeGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called at the start of the game
	void ChoosePlayerAndStartGame();
	
};
