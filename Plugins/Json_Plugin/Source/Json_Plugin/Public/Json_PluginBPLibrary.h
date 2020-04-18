// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Json_PluginBPLibrary.generated.h"

/* 

*/

typedef TSharedPtr<FJsonObject> JsonObjectPtr;

UCLASS()
class UJson_PluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(Blueprintcallable, Category = "Json Utilities")
		static bool WriteInventoryData(FString FileName, FString CharName, TArray<AActor*> InventoryItems, TArray<int> InventoryCount);

	UFUNCTION(BlueprintCallable, Category = "Json Utilities")
		static bool ReadInventoryData(FDateTime& SaveDate, FString& CharName, TArray<FString>& InventoryItems, TArray<int>& InventoryCount);

};
