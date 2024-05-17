//UE_LogToFile:
  UFUNCTION(BlueprintCallable)
  static void LogFile(FString _filePath, FString _Text, bool& sussess)
  {
      IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
      //bool bSuccess = DesktopPlatform->OpenFileDialog(nullptr,TEXT("打开文件"))
      sussess = FFileHelper::SaveStringToFile(_Text, *_filePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
  }
