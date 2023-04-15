// Copyright 2010-2020 Deduktiva GmbH and contributors.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define ECLIPSE_EXE TEXT("Magnolia-IDE-Eclipse.exe")
#define MSGBOX_TITLE TEXT("Magnolia IDE Launcher")

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);

    TCHAR szPath[MAX_PATH];

    if(SUCCEEDED(SHGetFolderPath(NULL, 
        CSIDL_APPDATA|CSIDL_FLAG_CREATE, 
        NULL, 
        0, 
        szPath))) 
    {
        PathAppend(szPath, TEXT("\\Eclipse"));
    } else {
        MessageBox(NULL, TEXT("Getting AppData path failed."), MSGBOX_TITLE, MB_ICONERROR|MB_OK);
        exit(EXIT_FAILURE);
    }

    size_t bufChars = _tcslen(lpCmdLine) + _tcslen(szPath) + 50 + _tcslen(ECLIPSE_EXE);
    LPTSTR commandLine = (TCHAR *)calloc(bufChars+1, sizeof(TCHAR));
    _tcscpy_s(commandLine, bufChars, ECLIPSE_EXE);
    _tcscat_s(commandLine, bufChars, lpCmdLine);
    _tcscat_s(commandLine, bufChars, TEXT(" -configuration \""));
    _tcscat_s(commandLine, bufChars, szPath);
    _tcscat_s(commandLine, bufChars, TEXT("\""));

    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.wShowWindow = nCmdShow;
    startupInfo.dwFlags = STARTF_FORCEONFEEDBACK;

    TCHAR subprocess[] = ECLIPSE_EXE;
    PROCESS_INFORMATION processInformation;
    if(CreateProcess(subprocess,
        commandLine,
        0,
        0,
        FALSE,
        0,
        NULL,
        NULL,
        &startupInfo,
        &processInformation))
    {
        exit(EXIT_SUCCESS);
    } else {
        DWORD lastError = GetLastError();

        const size_t charsErrorMessage = 512;
        TCHAR errorMessage[charsErrorMessage];
        _tcscpy_s(errorMessage, charsErrorMessage, TEXT(""));
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, lastError, 0, errorMessage, charsErrorMessage, NULL);

        const size_t charsMessage = 1024;
        TCHAR message[charsMessage];
        _tcscpy_s(message, charsMessage, TEXT("Launching ") ECLIPSE_EXE TEXT(" failed: "));
        _tcscat_s(message, charsMessage, errorMessage);

        MessageBox(NULL, message, MSGBOX_TITLE, MB_ICONERROR|MB_OK);

        exit(EXIT_FAILURE);
    }
}