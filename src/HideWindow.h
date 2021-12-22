#pragma once

#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <psapi.h>

void HideWindow(HWND hWnd, BOOL Hide);
VOID HideWindowFromProcessNameA(LPCSTR lpProcessName);
VOID HideWindowFromProcessNameW(LPCWSTR lpProcessName);
VOID HideWindowFromProcessId(DWORD dwProcessId);
VOID ShowWindowFromProcessNameA(LPCSTR lpProcessName);
VOID ShowWindowFromProcessNameW(LPCWSTR lpProcessName);
VOID ShowWindowFromProcessId(DWORD dwProcessId);

typedef struct _HIDDEN
{
    BOOL    Hide;
    DWORD   Type;
    PVOID   Buffer;
} HIDDEN;