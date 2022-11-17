// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTGameInstance.h"

void UTTTGameInstance::IncrementScoreHumanPlayer()
{
	ScoreHumanPlayer += 1;
}

void UTTTGameInstance::IncrementScoreAiPlayer()
{
	ScoreAiPlayer += 1;
}

int32 UTTTGameInstance::GetScoreHumanPlayer()
{
	return ScoreHumanPlayer;
}

int32 UTTTGameInstance::GetScoreAiPlayer()
{
	return ScoreAiPlayer;
}

void UTTTGameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

FString UTTTGameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}
