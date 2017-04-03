#pragma once

#define STR_MAX 80

struct Header
{
	DWORD dwFileSize;
	DWORD dwCountNotes;
};
typedef Header *PHeader;

struct Note
{
	DWORD dwId;
	FILETIME lpLastChangeTime;
	CHAR szNote[STR_MAX];
	DWORD dwCountChanges;
};
typedef Note *PNote;

void FileTimeToCurrentSystemTime(PFILETIME ft);

BOOL FileExists(LPCTSTR szPath);

BOOL readHeader(HANDLE hFile, PHeader pHeader);

BOOL writeHeader(HANDLE hFile, PHeader pHeader);

HANDLE fileNew(LPCTSTR szFileName, DWORD dwNumberOfNotes, PHeader pHeader);

HANDLE fileOpen(LPCTSTR szFileName, PHeader pHeader);

BOOL readNote(HANDLE hFile, PHeader pHeader, DWORD dwId, PNote pNote);

BOOL writeNote(HANDLE hFile, PHeader pHeader, DWORD dwId, CHAR* szString);

BOOL removeNote(HANDLE hFile, PHeader pHeader, DWORD dwId);

void printNoteInfo(HANDLE hFile, PHeader pHeader, DWORD dwId);