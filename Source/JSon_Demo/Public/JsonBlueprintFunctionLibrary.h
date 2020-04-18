// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"
#include "JsonBlueprintFunctionLibrary.generated.h"


typedef TSharedPtr<FJsonObject> JsonObjectPtr;

/**
 * 
 */

UCLASS()
class JSON_DEMO_API UJsonBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

		UFUNCTION(Blueprintcallable, Category = "Json")
		static bool WriteInventoryData(FString CharName, TArray<AActor*> InventoryItems, TArray<int> InventoryCount);

		UFUNCTION(BlueprintCallable, Category = "Json")
		static bool ReadInventoryData(FDateTime& SaveDate, FString& CharName, TArray<FString>& InventoryItems, TArray<int>& InventoryCount);

private:
};
