// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM()
enum ETileStatus
{
	EMPTY     UMETA(DisplayName = "Empty"),
	OCCUPIED      UMETA(DisplayName = "Occupied"),
};

UCLASS()
class TTT_UNIGE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// set the player owner and the status of a tile
	void SetTileStatus(const int32 TileOwner, const TEnumAsByte<ETileStatus> TileStatus);

	// get the tile status
	TEnumAsByte<ETileStatus> GetTileStatus();

	// get the tile owner
	int32 GetOwner();

	// set the (x, y) position
	void SetGridPosition(const double InX, const double InY);

	// get the (x, y) position
	FVector2D GetGridPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// EditAnywhere: Indicates that this property can be edited by property windows
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	// BlueprintReadWrite: This property can be read or written from a Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETileStatus> Status;

	UPROPERTY(EditAnywhere)
	int32 PlayerOwner;

	// (x, y) position of the tile
	UPROPERTY(EditAnywhere)
	FVector2D TileGridPosition;


};
