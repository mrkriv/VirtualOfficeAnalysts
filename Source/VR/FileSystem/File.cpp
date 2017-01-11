#include "VR.h"
#include "File.h"

UFile::UFile()
{
	this->path = "";
}

FString UFile::GetName()
{
	return FPaths::GetCleanFilename(path);;
}

FString UFile::GetPath()
{
	return path;
}

int UFile::GetSize()
{
	return 0;
}

UFile* UFile::GetFileInfo(FString path)
{
	auto obj = NewObject<UFile>(UFile::StaticClass());
	obj->path = path;

	//FPatch::NormalizeFilename(obj->path);

	return obj;
}