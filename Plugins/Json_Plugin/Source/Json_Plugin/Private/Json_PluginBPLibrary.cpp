// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Json_PluginBPLibrary.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include "Serialization/JsonReader.h"
#include "misc/DateTime.h"
#include "Json_Plugin.h"

static const FString RootName("inventoryObject");
static const FString WritePath(FPaths::ProjectSavedDir() / "Json");
static const FString InventoryFileName("InventoryData.json");
static const FString FilepathFull(WritePath / InventoryFileName);

UJson_PluginBPLibrary::UJson_PluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UJsonBlueprintFunctionLibrary::WriteInventoryData(FString CharName, TArray<AActor*> InventoryItems, TArray<int> InventoryCount)
{
	JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject);

	const FString SaveDate = FDateTime::UtcNow().ToString();
	JsonRootObject->SetStringField("SaveDate", SaveDate);

	JsonRootObject->SetStringField("Character", CharName);

	TArray<TSharedPtr<FJsonValue>> InventoryItemObjects;
	for (AActor* obj : InventoryItems)
	{
		InventoryItemObjects.Add(MakeShareable(new FJsonValueString(obj->GetFullName())));
	}

	JsonRootObject->SetArrayField("Items", InventoryItemObjects);

	TArray<TSharedPtr<FJsonValue>> InventoryItemCountArray;
	for (int i : InventoryCount)
	{
		InventoryItemCountArray.Add(MakeShareable(new FJsonValueNumber(i)));
	}

	JsonRootObject->SetArrayField("ItemCount", InventoryItemCountArray);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonRootObject.ToSharedRef(), Writer);

	return FFileHelper::SaveStringToFile(OutputString, *FilepathFull);
}

bool UJsonBlueprintFunctionLibrary::ReadInventoryData(FDateTime & SaveDate, FString & CharName, TArray<FString>& InventoryItems, TArray<int>& InventoryCount)
{
	FString RawData;

	bool bloadedfile = FFileHelper::LoadFileToString(RawData, *FilepathFull);

	if (bloadedfile)
	{
		JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject());

		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawData);

		if (FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
		{
			FDateTime::Parse(JsonRootObject->GetStringField("SaveDate"), SaveDate);

			CharName = JsonRootObject->GetStringField("Character");

			for (TSharedPtr<FJsonValue> V : JsonRootObject->GetArrayField("Items"))
			{
				InventoryItems.Add(V->AsString());
			}

			for (TSharedPtr<FJsonValue> V : JsonRootObject->GetArrayField("ItemCount"))
			{
				InventoryCount.Add(V->AsNumber());
			}
		}
	}

	return false;
}
