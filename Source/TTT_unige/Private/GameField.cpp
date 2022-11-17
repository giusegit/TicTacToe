// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"
#include "Kismet/GameplayStatics.h"
#include "BaseSign.h"
#include "MyGameMode.h"
#include "TTT_unige/TTT_unige.h"


// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// size of winnning line
	WinSize = 3;
	// size of the field (3x3)
	Size = 3;
	// tile dimension
	TileSize = 120;
	// tile padding dimension
	CellPadding = 20; 
	//normalized tilepadding
	NormalizedCellPadding = round(((TileSize + CellPadding) / TileSize)*100)/100;
 
}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform); 
	
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}

void AGameField::ResetField()
{

	for (ATile* Obj : TileArray)
	{
		Obj->SetTileStatus(NOT_ASSIGNED, EMPTY);
	}

	// send broadcast event to registered objects 
	OnResetEvent.Broadcast();

	AMyGameMode* GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	GameMode->IsGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();
}

void AGameField::GenerateField()
{
	 
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location =AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = Cast<ATile>(GetWorld()->SpawnActor(TileClass, &Location));
			float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)
{
	return  TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location)
{
	double x = Location[0] / (TileSize * NormalizedCellPadding);
	double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}

bool AGameField::IsWinPosition(const FVector2D Position) const
{
	int32 offset = WinSize - 1;
	// vertical lines
	for (int32 i = Position[0] - offset; i <= Position[0]; i++)
	{
		if (IsWinLine(FVector2D(i, Position[1]), FVector2D(i + offset, Position[1])))
		{
			return true;
		}		
	}
		
	// horizontal lines
	for (int32 i = Position[1] - offset; i <= Position[1]; i++)
	{
		if (IsWinLine(FVector2D(Position[0], i), FVector2D(Position[0], i + offset)))
		{
			return true;
		}
	}
		
	// diagonal lines
	for (int32 i = -offset; i <= 0; i++)
	{
		if (IsWinLine(FVector2D(Position[0] + i, Position[1] + i), FVector2D(Position[0] + offset + i, Position[1] + offset + i)))
			return true;
		if (IsWinLine(FVector2D(Position[0] + i, Position[1] - i), FVector2D(Position[0] + offset + i, Position[1] - offset - i)))
			return true;
	}

	return false;
}


inline bool AGameField::IsWinLine(const FVector2D Begin, const FVector2D End) const
{
	return IsValidPosition(Begin) && IsValidPosition(End) && AllEqual(GetLine(Begin, End));
}

inline bool AGameField::IsValidPosition(const FVector2D Position) const
{
	return 0 <= Position[0] && Position[0] < Size && 0 <= Position[1] && Position[1] < Size;
}
 
TArray<int32> AGameField::GetLine(const FVector2D Begin, const FVector2D End) const
{
	int32 xSign;
	if (Begin[0] == End[0])
		xSign = 0;
	else
		xSign = Begin[0] < End[0] ? 1 : -1;

	int32 ySign;
	if (Begin[1] == End[1])
		ySign = 0;
	else
		ySign = Begin[1] < End[1] ? 1 : -1;

	TArray<int32> line;
	int32 x = Begin[0] - xSign;
	int32 y = Begin[1] - ySign;
	do {
		x += xSign;
		y += ySign;
		line.Add((TileMap[FVector2D(x, y)])->GetOwner());
	} while (x != End[0] || y != End[1]);
	
	return line;
}

bool AGameField::AllEqual(const TArray<int32>& Array) const
{
	if (Array.Num() == 0)
	{
		return false;
	}
	int32 Value = Array[0];
	
	if (Value == NOT_ASSIGNED)
	{
		return false;
	}

	for (int32 i = 1; i < Array.Num(); i++)
	{
		if (Value != Array[i])
			return false;
	}
		
	return true;
};
