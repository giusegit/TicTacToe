// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTPawn.h"
#include "GameField.h"
#include "MyGameMode.h"

// Sets default values
ATTTPawn::ATTTPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//set the camera as RootComponent
	RootComponent = Camera; 
	// get the game instance reference
	GameInstance = Cast<UTTTGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PlayerNumber = -1;
	Sign = ESign::E;
}

// Called when the game starts or when spawned
void ATTTPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATTTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATTTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// bind the OnClick function to InputComponent
	InputComponent->BindAction("Click", IE_Pressed, this, &ATTTPawn::OnClick);
}

void ATTTPawn::OnClick()
{
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	
	if (Hit.bBlockingHit && IsMyTurn)
	{
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
		{
			if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked"));
				CurrTile->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);
				FVector SpawnPosition = CurrTile->GetActorLocation();
				AMyGameMode* GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
				GameMode->SetCellSign(PlayerNumber, SpawnPosition);
				IsMyTurn = false;
			} 
		}
	}
}

void ATTTPawn::OnTurn()
{
	IsMyTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
	GameInstance->SetTurnMessage(TEXT("Human Turn"));
}

void ATTTPawn::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
}

void ATTTPawn::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	// GameInstance->SetTurnMessage(TEXT("Human Lose!"));
}

