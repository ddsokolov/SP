// task2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

void printErrorMessage();
LPSTR UnicodeToANSI(LPCWSTR src);
LPWSTR ANSIToUnicode(LPCSTR src);
void ANSIToUnicodeFile(LPCTSTR szSourceFile, LPCTSTR szDestFile);
void UnicodeToANSIFile(LPCTSTR szSourceFile, LPCTSTR szDestFile);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Rus");
	HANDLE hFileSrc = NULL;
	if (argc != 4)
	{
		printf("Неправильные аргументы \n");
	}
                                
	else if (!lstrcmp(argv[1], TEXT("-a")))
	{
		ANSIToUnicodeFile(argv[2], argv[3]);
	}
	else if (!lstrcmp(argv[1], TEXT("-u")))
	{
		UnicodeToANSIFile(argv[2], argv[3]);
	}
	else
	{
		printf("Неправильные аргументы\n");
	}
	return 0;
}

void ANSIToUnicodeFile(LPCTSTR szSourceFile, LPCTSTR szDestFile)
{
	HANDLE hFile;
	if (!(hFile = CreateFile(szSourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL)))
	{
		printErrorMessage();
		return;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwBytesRead;
	CHAR* szBuf = new CHAR[dwFileSize + 1]; szBuf[dwFileSize] = '\0';
	ReadFile(hFile, szBuf, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!(hFile = CreateFile(szDestFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL)))
	{
		printErrorMessage();
		return;
	}
	WCHAR* szBufW = ANSIToUnicode(szBuf);
	DWORD dwBytesToWrite = lstrlenW(szBufW) * sizeof(WCHAR);
	DWORD dwBytesWritten;
	WriteFile(hFile, szBufW, dwBytesToWrite, &dwBytesWritten, NULL);
	CloseHandle(hFile);
	delete[] szBuf;
}

void UnicodeToANSIFile(LPCTSTR szSourceFile, LPCTSTR szDestFile)
{
	HANDLE hFile;
	if (!(hFile = CreateFile(szSourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		printErrorMessage();
		return;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwBytesRead;
	WCHAR* szBufW = new WCHAR[dwFileSize / sizeof(WCHAR) + 1]; szBufW[dwFileSize / sizeof(WCHAR)] = 0;
	ReadFile(hFile, szBufW, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!(hFile = CreateFile(szDestFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		printErrorMessage();
		return;
	}
	CHAR* szBuf = UnicodeToANSI(szBufW);
	DWORD dwBytesToWrite = strlen(szBuf);
	DWORD dwBytesWritten;
	WriteFile(hFile, szBuf, dwBytesToWrite, &dwBytesWritten, NULL);
	CloseHandle(hFile);
	delete[] szBuf;
}

LPWSTR ANSIToUnicode(LPCSTR src)
{
	if (!src) return 0;
	int srcLen = strlen(src);
	if (!srcLen)
	{
		wchar_t *w = new wchar_t[1];
		w[0] = 0;
		return w;
	}

	int requiredSize = MultiByteToWideChar(CP_ACP, 0, src, srcLen, 0, 0);

	if (!requiredSize)
	{
		return 0;
	}

	wchar_t* w = new wchar_t[requiredSize + 1];
	w[requiredSize] = 0;

	int retval = MultiByteToWideChar(CP_ACP, 0, src, srcLen, w, requiredSize);
	if (!retval)
	{
		delete[] w;
		return 0;
	}

	return w;
}

LPSTR UnicodeToANSI(LPCWSTR src)
{
	if (!src) return 0;
	int srcLen = wcslen(src);
	if (!srcLen)
	{
		char *x = new char[1];
		x[0] = '\0';
		return x;
	}

	int requiredSize = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, src, srcLen, 0, 0, 0, 0);

	if (!requiredSize)
	{
		return 0;
	}

	char *x = new char[requiredSize + 1];
	x[requiredSize] = 0;

	int retval = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, src, srcLen, x, requiredSize, 0, 0);

	if (!retval)
	{
		delete[] x;
		return 0;
	}

	return x;
}

void printErrorMessage()
{
	HRESULT hResult = GetLastError();

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
		wprintf(L"%0s", errorText);
		LocalFree(errorText);
	}
}