#include "stdafx.h"
#include "fileman.h"

int FileTimeToString(FILETIME FileTime, DWORD nBufferLength, LPTSTR lpBuffer);
void printErrorMessage(HRESULT hResult = NULL);

void printCurrentDirectory()
{
	TCHAR szBuffer[MAX_PATH];
	DWORD dwRet;
	dwRet = GetCurrentDirectory(MAX_PATH, szBuffer);
	if (dwRet == 0)
	{
		printf("Unable to get current directory\n");
		return;
	}
	if (dwRet > MAX_PATH)
	{
		printf("Buffer too small; need %d characters\n", dwRet);
		return;
	}
	_tprintf(TEXT("Current directory: %s\n"), szBuffer);
}

void setCurrentDirectory(LPCTSTR lpPathName)
{
	if (!SetCurrentDirectory(lpPathName))
	{
		printf("Unable to set current directory (0x%08x)\n", GetLastError());
		return;
	}
	_tprintf(TEXT("Set current directory to %s\n"), lpPathName);
}

void copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
{
	if (!CopyFileEx(lpExistingFileName, lpNewFileName, NULL, NULL, NULL, NULL))
	{
		printf("File copied\n");
	}
	else
	{
		printf("Copy error\n");
	}
}

void createDirectory(LPCTSTR lpPathName)
{
	if (!CreateDirectory(lpPathName, NULL))
	{
		printf("Unable to create directory (0x%08x)\n", GetLastError());
		return;
	}
}

void removeFile(LPCTSTR lpPathName)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(lpPathName, &ffd);

	if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		if (!RemoveDirectory(lpPathName))
		{
			printf("Unable to remove the directory (0x%08x)\n", GetLastError());
		}
	}
	else
	{
		if (!DeleteFile(lpPathName))
		{
			printErrorMessage();
			//printf("Unable to delete the file (0x%08x)\n", GetLastError());
		}
	}
}

void printFiles()
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR szDir[MAX_PATH];
	DWORD dwError = 0;

	GetCurrentDirectory(MAX_PATH, szDir);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &ffd);

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s    <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s    %I64d bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
}

void printFileInfo(LPCTSTR lpPathName)
{
	HANDLE hFile = NULL;
	if ((hFile = CreateFile(lpPathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("Unable to open the file (0x%08x)\n", GetLastError());
		return;
	}

	BY_HANDLE_FILE_INFORMATION fileInfo;
	if (!GetFileInformationByHandle(hFile, &fileInfo))
	{
		printf("Unable to get file info (0x%08x)\n", GetLastError());
		return;
	}

	TCHAR szBuffer[MAX_PATH];
	GetFullPathName(lpPathName, MAX_PATH, szBuffer, NULL);
	_tprintf(TEXT("Full path: %s\n"), szBuffer);

	FileTimeToString(fileInfo.ftCreationTime, MAX_PATH, szBuffer);
	_tprintf(TEXT("Creation time: %s\n"), szBuffer);

	FileTimeToString(fileInfo.ftLastAccessTime, MAX_PATH, szBuffer);
	_tprintf(TEXT("Last access time: %s\n"), szBuffer);

	FileTimeToString(fileInfo.ftLastWriteTime, MAX_PATH, szBuffer);
	_tprintf(TEXT("Last write time: %s\n"), szBuffer);

	printf("File size: %d bytes\n", fileInfo.nFileSizeLow);

	CloseHandle(hFile);
}


int FileTimeToString(FILETIME FileTime, DWORD nBufferLength, LPTSTR lpBuffer)
{
	SYSTEMTIME st;
	FileTimeToSystemTime(&FileTime, &st);
	return _stprintf_s(lpBuffer, nBufferLength, TEXT("%04d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void printErrorMessage(HRESULT hResult)
{
	if (hResult == NULL)
	{
		hResult = GetLastError();
	}
	LPTSTR errorText = NULL;
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,
		0,
		NULL);
	if (NULL != errorText)
	{
		wprintf(L"%s", errorText);
		LocalFree(errorText);
	}
}