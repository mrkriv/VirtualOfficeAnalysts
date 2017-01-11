#include "VR.h"
#include "Directory.h"

#define FM IFileManager::Get()

UDirectory::UDirectory()
{
	this->path = "";
}

FString UDirectory::GetName()
{
	return FPaths::GetCleanFilename(path);
}

FString UDirectory::GetPath()
{
	return path;
}

TArray<UFile*> UDirectory::GetFiles(FString mask)
{
	TArray<UFile*> files;

	if (path.Len() > 0)
	{
		TArray<FString> files_name;

		if (mask == "")
			mask = "*.*";
		else
			mask = (mask.Left(1) == ".") ? "*" + mask : "*." + mask;

		FString fullPath = path + "/" + mask;
		FM.FindFiles(files_name, *fullPath, true, false);

		for (size_t i = 0; i < files_name.Num(); i++)
			files.Add(UFile::GetFileInfo(path + "/" + files_name[i]));
	}

	return files;
}

TArray<UDirectory*> UDirectory::GetDirectories(FString mask)
{
	TArray<UDirectory*> dirs;

	if (path.Len() > 0)
	{
		TArray<FString> dir_names;

		if (mask == "")
			mask = "*.*";
		else
			mask = (mask.Left(1) == ".") ? "*" + mask : "*." + mask;

		FString fullPath = path + "/" + mask;
		FM.FindFiles(dir_names, *fullPath, false, true);

		for (size_t i = 0; i < dir_names.Num(); i++)
			dirs.Add(UDirectory::GetDirectory(fullPath + "/" + dir_names[i]));
	}

	return dirs;
}

UDirectory* UDirectory::GetDirectory(FString path)
{
	auto obj = NewObject<UDirectory>(UDirectory::StaticClass());
	obj->path = path;

	FPaths::NormalizeDirectoryName(obj->path);

	return obj;
}