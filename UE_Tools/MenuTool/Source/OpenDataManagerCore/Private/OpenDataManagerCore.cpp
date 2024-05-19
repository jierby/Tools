// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenDataManagerCore.h"

static const FName OpenDataManagerTabName("OpenDataManagerCore");

#define LOCTEXT_NAMESPACE "FOpenDataManagerCoreModule"

void FOpenDataManagerCoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FOpenDataManagerCoreModule::ShutdownModule()
{
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOpenDataManagerCoreModule, OpenDataManagerCore)