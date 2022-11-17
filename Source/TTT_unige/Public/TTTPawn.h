// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TTTPlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "TTTGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TTTPawn.generated.h"

UCLASS()
class TTT_UNIGE_API ATTTPawn : public APawn, public ITTTPlayerInterface
{
	GENERATED_BODY()

public:
	// camera component attacched to player pawn
	UCameraComponent* Camera;
	// game instance reference
	UTTTGameInstance* GameInstance;

	// Sets default values for this pawn's properties
	ATTTPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// keeps track of turn
	bool IsMyTurn = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	// called on left mouse click (binding)
	UFUNCTION()
	void OnClick();

};
