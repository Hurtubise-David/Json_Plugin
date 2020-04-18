// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Json_PluginBPLibrary.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include "Serialization/JsonReader.h"
#include "Json_Plugin.h"

static const FString RootName("inventoryObject");

UJson_PluginBPLibrary::UJson_PluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UJson_PluginBPLibrary::WriteInventoryData(FString FileName, FString CharName, TArray<AActor*> InventoryItems, TArray<int> InventoryCount)
{
	JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject);

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

	FString Path;
	Path = FPaths::ProjectSavedDir() / "Json";
	Path += "/";
	Path += FileName;

	return FFileHelper::SaveStringToFile(OutputString, *Path);
}

bool UJson_PluginBPLibrary::ReadInventoryData(FString & FileName, FString & CharName, TArray<FString>& InventoryItems, TArray<int>& InventoryCount)
{
	FString RawData;
	FString Path;
	Path = FPaths::ProjectSavedDir() / "Json";
	Path += "/";
	Path += FileName;

	bool bloadedfile = FFileHelper::LoadFileToString(RawData, *Path);

	if (bloadedfile)
	{
		JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject());

		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawData);

		if (FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
		{

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
