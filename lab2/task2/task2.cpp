// task2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "fileman.h"

void readline(TCHAR szBuffer[], DWORD nBufferSize);

int main()
{
	setlocale(LC_ALL, "rus");

	TCHAR szBuffer[MAX_PATH];
	TCHAR szBuffer2[MAX_PATH];
	int command = 0;

	while (true)
	{
		printf("Choose operation (9 for help): ");
		scanf_s("%d", &command);

		switch (command)
		{
		case 0:
			return 0;
		case 1:
			printf("Change directory to: ");
			readline(szBuffer, MAX_PATH);
			setCurrentDirectory(szBuffer);
			break;
		case 2:
			printFiles();
			break;
		case 3:
			printf("Enter existing file name: ");
			readline(szBuffer, MAX_PATH);
			printf("Enter new file name: ");
			readline(szBuffer2, MAX_PATH);
			copyFile(szBuffer, szBuffer2);
			break;
		case 4:
			printf("Enter dir name: ");
			readline(szBuffer, MAX_PATH);
			createDirectory(szBuffer);
			break;
		case 5:
			printf("Enter file name: ");
			readline(szBuffer, MAX_PATH);
			removeFile(szBuffer);
			break;
		case 6:
			printf("Enter file name: ");
			readline(szBuffer, MAX_PATH);
			printFileInfo(szBuffer);
			break;
		case 7:
			printCurrentDirectory();
			break;
		case 9:
			printf("1. Change directory\n");
			printf("2. Print files\n");
			printf("3. Copy a file\n");
			printf("4. Create directory\n");
			printf("5. Delete file\n");
			printf("6. Print file info\n");
			printf("7. Print current directory\n");
			printf("9. Print this help\n");
			printf("0. Exit\n");
			break;
		default:
			printf("Unknown command\n");
			break;
		}
		printf("\n");
	}

    return 0;
}

void readline(TCHAR szBuffer[], DWORD nBufferSize)
{
	//setlocale(LC_ALL, ".866");
	DWORD i = 0, ch;
	while (isspace(ch = getchar()));
	while (ch != '\n')
	{
		if (i < nBufferSize)
			szBuffer[i++] = ch;
		ch = getchar();
	}
	szBuffer[i] = '\0';
	//setlocale(LC_ALL, "");
}