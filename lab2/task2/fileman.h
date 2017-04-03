#pragma once

void printCurrentDirectory();
void setCurrentDirectory(LPCTSTR szPathName);
void printFiles();
void copyFile(LPCTSTR szExistingFileName, LPCTSTR szNewFileName);
void createDirectory(LPCTSTR szPathName);
void removeFile(LPCTSTR lpPathName);
void printFileInfo(LPCTSTR lpPathName);