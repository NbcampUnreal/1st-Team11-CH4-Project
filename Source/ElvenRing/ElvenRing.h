// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#define LOG_NETMODEINFO ((GetNetMode() == NM_Client) ? *FString::Printf(TEXT("Client%d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDLALONE") : TEXT("SERVER")))
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define LOG(Format, ...) UE_LOG(LOG_ELDENRING, Warning, TEXT("[%s] %s %s"), LOG_NETMODEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__));

DECLARE_LOG_CATEGORY_EXTERN(LOG_ELDENRING, Log, All);

#include "CoreMinimal.h"