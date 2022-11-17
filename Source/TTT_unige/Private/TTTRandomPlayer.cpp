// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTRandomPlayer.h"
#include "TTT_unige/TTT_unige.h"
#include "MyGameMode.h"

// Sets default values
ATTTRandomPlayer::ATTTRandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UTTTGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void ATTTRandomPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATTTRandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATTTRandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATTTRandomPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Random Turn"));
	GameInstance->SetTurnMessage(TEXT("AI Turn"));

	FTimerHandle TimerHandle;
	 
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			TArray<ATile*> FreeCells;
			AMyGameMode* GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
			for (auto& CurrTile : GameMode->GField->GetTileArray())
			{
				if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
				{
					FreeCells.Add(CurrTile);
				}
			}

			if (FreeCells.Num() > 0)
			{
				int32 RandIdx = FMath::Rand() % FreeCells.Num();
				FVector Location = GameMode->GField->GetRelativeLocationByXYPosition((FreeCells[RandIdx])->GetGridPosition()[0], (FreeCells[RandIdx])->GetGridPosition()[1]);
				FreeCells[RandIdx]->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);

				GameMode->SetCellSign(PlayerNumber, Location);

			}
		}, 3, false);	
}

void ATTTRandomPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Random Win!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void ATTTRandomPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Random Lose!"));
	// GameInstance->SetTurnMessage(TEXT("AI Lose!"));
}