#include "stdafx.h"
#include "dbman.h"

HANDLE fileOpen(LPCTSTR lpFileName, PHeader pHeader)
{
	HANDLE hFile;
	if ((hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
	{
		readHeader(hFile, pHeader);
	}
	return hFile;
}

HANDLE fileNew(LPCTSTR szFileName, DWORD dwNumberOfNotes, PHeader pHeader)
{
	HANDLE hFile;
	if ((hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
	{
		pHeader->dwCountNotes = 0;
		pHeader->dwFileSize = dwNumberOfNotes * sizeof(Note);
		writeHeader(hFile, pHeader);
		SetFilePointer(hFile, dwNumberOfNotes * sizeof(Note) + sizeof(Header), 0, FILE_BEGIN);
		SetEndOfFile(hFile);
	}
	return hFile;
}

BOOL readHeader(HANDLE hFile, PHeader pHeader)
{
	if (SetFilePointer(hFile, 0, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return FALSE;
	}
	if (!ReadFile(hFile, &(pHeader->dwFileSize), sizeof(DWORD), NULL, NULL))
	{
		return FALSE;
	}
	return ReadFile(hFile, &(pHeader->dwCountNotes), sizeof(DWORD), NULL, NULL);
}

BOOL writeHeader(HANDLE hFile, PHeader pHeader)
{
	if (SetFilePointer(hFile, 0, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return FALSE;
	}
	if (!WriteFile(hFile, &pHeader->dwFileSize, sizeof(DWORD), NULL, NULL))
	{
		return FALSE;
	}
	return WriteFile(hFile, &pHeader->dwCountNotes, sizeof(DWORD), NULL, NULL);
}

BOOL readNote(HANDLE hFile, PHeader pHeader, DWORD dwId, PNote pNote)
{
	if (dwId * sizeof(Note) > pHeader->dwFileSize)
	{
		return FALSE;
	}

	if (SetFilePointer(hFile, sizeof(Header) + sizeof(Note) * (dwId - 1), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return FALSE;
	}

	return ReadFile(hFile, pNote, sizeof(Note), NULL, NULL);
}

BOOL writeNote(HANDLE hFile, PHeader pHeader, DWORD dwId, CHAR* szString)
{
	if (dwId * sizeof(Note) > pHeader->dwFileSize)
	{
		return FALSE;
	}

	PNote pNote;
	if (!(pNote = (PNote)LocalAlloc(LMEM_FIXED, sizeof(Note))))
	{
		return FALSE;
	}
	if (!readNote(hFile, pHeader, dwId, pNote))
	{
		return FALSE;
	}

	if (pNote->dwId == 0)
	{
		pNote->dwId = dwId;
		pHeader->dwCountNotes++;
	}

	FileTimeToCurrentSystemTime(&pNote->lpLastChangeTime);
	pNote->dwCountChanges++;
	strcpy(pNote->szNote, szString);

	if (SetFilePointer(hFile, sizeof(Header) + sizeof(Note) * (dwId - 1), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return FALSE;
	}
	if (!WriteFile(hFile, pNote, sizeof(Note), NULL, NULL))
	{
		return FALSE;
	}

	LocalFree(pNote);
	return writeHeader(hFile, pHeader);
}

BOOL removeNote(HANDLE hFile, PHeader pHeader, DWORD dwId)
{
	if (dwId * sizeof(Note) > pHeader->dwFileSize)
	{
		return FALSE;
	}

	PNote pNote;
	if (!(pNote = (PNote)LocalAlloc(LMEM_FIXED, sizeof(Note))))
	{
		return FALSE;
	}

	ZeroMemory(pNote, sizeof(Note));
	if (SetFilePointer(hFile, sizeof(Header) + sizeof(Note) * (dwId - 1), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return FALSE;
	}
	if (!WriteFile(hFile, pNote, sizeof(Note), NULL, NULL))
	{
		return FALSE;
	}

	LocalFree(pNote);
	return writeHeader(hFile, pHeader);
}

void printNoteInfo(HANDLE hFile, PHeader pHeader, DWORD dwId)
{
	PNote pNote;
	if (!(pNote = (PNote)LocalAlloc(LMEM_FIXED, sizeof(Note))))
	{
		printf("Unable to allocale memory\n");
		return;
	}
	if (!readNote(hFile, pHeader, dwId, pNote))
	{
		printf("Unable to read note\n");
		return;
	}

	if (pNote->dwId == 0)
	{
		printf("Note #%u is empty\n", dwId);
		return;
	}

	printf("ID: %u\n", pNote->dwId);
	printf("Changes: %u\n", pNote->dwCountChanges);

	SYSTEMTIME st;
	FileTimeToSystemTime(&pNote->lpLastChangeTime, &st);
	printf("Last change: %d-%02d-%02d %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	printf("Note: %s\n", pNote->szNote);

	LocalFree(pNote);
}

void FileTimeToCurrentSystemTime(PFILETIME ft)
{
	//FILETIME ft;
	SYSTEMTIME st;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, ft);
}

BOOL FileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}