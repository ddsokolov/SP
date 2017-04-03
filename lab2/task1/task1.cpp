// task1.cpp: определяет точку входа для консольного приложения.
//

/*	TODO:
*/

#include "stdafx.h"
#include "dbman.h"

#define FILEPATH TEXT("notes.bin")

void readline(char str[], int str_len);

int _tmain(int argc, _TCHAR** argv)
{
	HANDLE hFile;
	PHeader pHeader = (PHeader)LocalAlloc(LMEM_FIXED, sizeof(Header));
	if (!(hFile = fileOpen(FILEPATH, pHeader)))
	{
		printf("Unable to open file. Creating new...\n");
		printf("Input a number of notes: ");
		DWORD num;
		scanf_s("%u", &num);
		if (!(hFile = fileNew(FILEPATH, num, pHeader)))
		{
			printf("Unable to create file\n");
			return 1;
		}
	}

	INT command;

	for (;;)
	{
		DWORD dwId;
		printf("Choose operation(9 - for help): ");
		scanf("%i", &command);
		switch (command)
		{
		case 0:
			LocalFree(pHeader);
			CloseHandle(hFile);
			return 0;
		case 1:
			printf("Input note ID: ");
			scanf("%u", &dwId);
			printNoteInfo(hFile, pHeader, dwId);
			system("pause");
			break;
		case 2:
			printf("Input note ID: ");
			scanf("%u", &dwId);

			printf("Input note text: ");
			char szStr[STR_MAX];
			readline(szStr, STR_MAX);
			if (writeNote(hFile, pHeader, dwId, szStr))
			{
				printf("Note is written successfully\n");
			}
			else
			{
				printf("Unable to write note\n");
			}
			system("pause");
			break;
		case 3:
			printf("Input note ID: ");
			DWORD dwId;
			scanf("%u", &dwId);

			if (removeNote(hFile, pHeader, dwId))
			{
				printf("Note removed successfully\n");
			}
			else
			{
				printf("Unable to remove note\n");
			}
			system("pause");
			break;
		case 9:
			printf("1 - print note\n");
			printf("2 - write note\n");
			printf("3 - delete note\n");
			printf("4 - print help\n");
			printf("5 - exit\n");
			system("pause");
			break;
		default:
			printf("Unknown command\n");
			system("pause");
		}
		system("cls");
	}

	CloseHandle(hFile);

	return 0;
}

void readline(char str[], int str_len)
{

	int i = 0, ch;
	while (isspace(ch = getchar()));
	while (ch != '\n')
	{
		if (i < str_len)
			str[i++] = ch;
		ch = getchar();
	}
	str[i] = '\0';	
}