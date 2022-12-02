#include "memmap.h"
#include <windows.h>

memmap::memmap() : basePtr(nullptr), hFile(nullptr), hMap(nullptr), fileSize(0)
{ }

memmap::memmap(const memmap& other) : basePtr(other.basePtr), hFile(other.hFile), hMap(other.hMap), fileSize(other.fileSize)
{ }

memmap& memmap::operator=(memmap&& other) noexcept
{
	if (this == &other)
		return *this;

	basePtr = other.basePtr;
	hFile = other.hFile;
	hMap = other.hMap;
	fileSize = other.fileSize;

	other.basePtr = nullptr;
	other.hFile = nullptr;
	other.hMap = nullptr;
	other.fileSize = 0;

	return *this;
}

memmap::memmap(std::string path, bool overwriteExisting) : memmap()
{
	LARGE_INTEGER liFileSize;
	hFile = CreateFile(path.c_str(),
		GENERIC_READ | GENERIC_WRITE,						// dwDesiredAccess
		FILE_SHARE_READ | FILE_SHARE_WRITE,					// dwShareMode
		NULL,												// lpSecurityAttributes
		overwriteExisting ? CREATE_ALWAYS : OPEN_ALWAYS,	// dwCreationDisposition
		FILE_ATTRIBUTE_NORMAL,								// dwFlagsAndAttributes
		NULL);												// hTemplateFile

	if (hFile == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "CreateFile failed with error %d\n", GetLastError());
		throw;
	}
	/*
	LARGE_INTEGER nfileSize;
	nfileSize.QuadPart = 1_GB;
	fileSize = nfileSize.QuadPart;

	SetFilePointerEx(hFile, nfileSize, NULL, FILE_BEGIN);
	SetEndOfFile(hFile);
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	*/
	if (!GetFileSizeEx(hFile, &liFileSize)) {
		fprintf(stderr, "SetFilePointer failed with error %d\n", GetLastError());
		CloseHandle(hFile);
		throw;
	}
	fileSize = liFileSize.QuadPart;

	hMap = CreateFileMapping(
		hFile,
		NULL,                          // Mapping attributes
		PAGE_READWRITE,                // Protection flags
		0,                             // MaximumSizeHigh
		0,                             // MaximumSizeLow
		NULL);                         // Name
	if (hMap == 0) {
		fprintf(stderr, "CreateFileMapping failed with error %d\n", GetLastError());
		CloseHandle(hFile);
		throw;
	}

	basePtr = MapViewOfFile(
		hMap,
		FILE_MAP_ALL_ACCESS,   // dwDesiredAccess
		0,                     // dwFileOffsetHigh
		0,                     // dwFileOffsetLow
		0);                    // dwNumberOfBytesToMap

	if (basePtr == NULL) {
		fprintf(stderr, "MapViewOfFile failed with error %d\n", GetLastError());
		CloseHandle(hMap);
		CloseHandle(hFile);
		throw;
	}
}


memmap::~memmap()
{
	if (basePtr) UnmapViewOfFile(basePtr);
	if (hMap) CloseHandle(hMap);
	if (hFile) CloseHandle(hFile);
}
