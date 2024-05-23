//UE_LogToFile:
  UFUNCTION(BlueprintCallable)
  static void LogToFile(FString _filePath, FString _Text, bool& sussess)
  {
      IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
      //bool bSuccess = DesktopPlatform->OpenFileDialog(nullptr,TEXT("打开文件"))
      sussess = FFileHelper::SaveStringToFile(_Text, *_filePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
  }
//读取文件为String,可以使用蓝图节点ParseIntoArray进行分割
UFUNCTION(BlueprintCallable, Category = "GWorld|Flib|AssetManagerEx")
	static bool LoadFileToString(const FString& InFile, FString& OutString)
  {
      return FFileHelper::LoadFileToString(OutString, *InFile);
  }
