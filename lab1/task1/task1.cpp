// task1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

void causeError();
void printErrorMessage(HRESULT hResult = NULL);
void printSystemInformation();

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Rus");
	if (argc != 2)
	{
		printf("Неправильные аргументы\n");
	}
	else if (!strcmp(argv[1], "-e"))
	{
		causeError();
		printErrorMessage();
	}
	else if (!strcmp(argv[1], "-s"))
	{
		printSystemInformation();
	}
	else
	{
		printf("Неправильные аргументы\n");
	}
	return 0;
}

void causeError()
{
	/*CreateFile(L"", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);*/
	LocalAlloc(LPTR, ULONG_MAX);
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

void printSystemInformation()
{
	printf("****Информация о процессоре****\n");
	SYSTEM_INFO systeminfo;
	GetNativeSystemInfo(&systeminfo);

	CHAR szProcArchitecture[19];
	switch (systeminfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		strcpy(szProcArchitecture, "x64");
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		strcpy(szProcArchitecture, "ARM");
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		strcpy(szProcArchitecture, "Intel Itanium-based");
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		strcpy(szProcArchitecture, "x86");
		break;
	default:
		strcpy(szProcArchitecture, "Unknown");
		break;
	}
	printf("Архитектура процессора: %s\n", szProcArchitecture);
	printf("Уровень процессора: %u\n", systeminfo.wProcessorLevel);
	printf("Размер страницы: %u\n", systeminfo.dwPageSize);
	printf("Количество процессоров: %u\n", systeminfo.dwNumberOfProcessors);

	printf("\n");
	printf("****Состояние памяти*****\n");
	MEMORYSTATUSEX lpMemStat;
	lpMemStat.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&lpMemStat);
	printf("Насколько загружена память: %u%%\n", lpMemStat.dwMemoryLoad);
	printf("Всего занято: %llu bytes\n", lpMemStat.ullTotalPhys);
	printf("Свободное место: %llu bytes\n", lpMemStat.ullAvailPhys);
}