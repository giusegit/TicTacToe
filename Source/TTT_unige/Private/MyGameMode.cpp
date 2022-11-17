// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "TTTPlayerController.h"
#include "TTTRandomPlayer.h"
#include "EngineUtils.h"
#include "TTTPawn.h" 
#include "GameField.h"

AMyGameMode::AMyGameMode()
{
	PlayerControllerClass = ATTTPlayerController::StaticClass();
	DefaultPawnClass = ATTTPawn::StaticClass();
	FieldSize = 3;
}

AMyGameMode::~AMyGameMode()
{
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	MoveCounter = 0;

	//ATTTPawn* Pawn = Cast<ATTTPawn>(*GetWorld()->GetPawnIterator());
	ATTTPawn* Pawn = Cast<ATTTPawn>(*TActorIterator<ATTTPawn>(GetWorld()));


	if (GameFieldClass != nullptr)
	{
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	Pawn->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());


	Players.Add(Pawn);

	// Random Player
	auto* AI = GetWorld()->SpawnActor<ATTTRandomPlayer>(FVector(), FRotator());

	Players.Add(AI);

	this->ChoosePlayerAndStartGame();

}

void AMyGameMode::SetCellSign(const int32 PlayerNumber, const FVector& SpawnPosition)
{
	if (IsGameOver || PlayerNumber != CurrentPlayer)
	{
		return;
	}

	UClass* SignActor = Players[CurrentPlayer]->Sign == ESign::X ? SignXActor : SignOActor;
	FVector Location = GField->GetActorLocation() + SpawnPosition + FVector(0, 0, 10);
	GetWorld()->SpawnActor(SignActor, &Location);

	if (GField->IsWinPosition(GField->GetXYPositionByRelativeLocation(SpawnPosition)))
	{
		IsGameOver = true;
		Players[CurrentPlayer]->OnWin();
		for (int32 i = 0; i < Players.Num(); i++)
			if (i != CurrentPlayer)
				Players[i]->OnLose();
	}
	else if (MoveCounter == (FieldSize * FieldSize))
	{
		// add a timer (3 seconds)
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				// function to delay
				GField->ResetField();
			}, 3, false);
	}
	else
	{
		TurnNextPlayer();
	}

}

int32 AMyGameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
		Player = 0;
	return Player;
}

void AMyGameMode::TurnNextPlayer()
{
	MoveCounter += 1;
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
}


void AMyGameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayer = FMath::RandRange(0, Players.Num() - 1);

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
		Players[i]->Sign = i == CurrentPlayer ? ESign::X : ESign::O;
	}
	MoveCounter += 1;
	Players[CurrentPlayer]->OnTurn();
}
