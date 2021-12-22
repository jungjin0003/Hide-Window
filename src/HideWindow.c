#include "HideWindow.h"

DECLSPEC_IMPORT HANDLE WINAPI GetProcessHandleFromHwnd(HWND hwnd);

void HideWindow(HWND hWnd, BOOL Hide)
{
    ShowWindow(hWnd, Hide == TRUE ? 0 : 5);
    EnableWindow(hWnd, ~Hide);
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, HIDDEN *lParam)
{
    switch (lParam->Type)
    {
    case 0:
    {
        HANDLE hProcess = GetProcessHandleFromHwnd(hWnd);
        PROCESS_BASIC_INFORMATION ProcessInformation;
        PVOID RemotePointer;
        WCHAR BaseName[MAX_PATH] = { 0, };

        if (hProcess == NULL)
            break;

        NtQueryInformationProcess(hProcess, ProcessBasicInformation, &ProcessInformation, sizeof(PROCESS_BASIC_INFORMATION), NULL);
        // Read PEB_LDR_DATA address
        ReadProcessMemory(hProcess, (ULONGLONG)ProcessInformation.PebBaseAddress + 0x18, &RemotePointer, 8, NULL);
        // Read LDR_DATA_TABLE_ENTRY address
        ReadProcessMemory(hProcess, (ULONGLONG)RemotePointer + 0x10, &RemotePointer, 8, NULL);
        // Read process BaseName buffer address
        ReadProcessMemory(hProcess, (ULONGLONG)RemotePointer + 0x60, &RemotePointer, 8, NULL);
        // Read process BaseName
        ReadProcessMemory(hProcess, RemotePointer, BaseName, 256, NULL);

        if (wcsicmp(lParam->Buffer, BaseName) == 0)
            HideWindow(hWnd, lParam->Hide);

        CloseHandle(hProcess);
        break;
    }
    case 1:
    {
        DWORD ProcessId;
        GetWindowThreadProcessId(hWnd, &ProcessId);
        if (ProcessId == *(DWORD *)lParam->Buffer)
            HideWindow(hWnd, lParam->Hide);
        break;
    }
    }


    return TRUE;
}

VOID HideWindowFromProcessNameA(LPCSTR lpProcessName)
{
    DWORD Length = strlen(lpProcessName);
    LPWSTR ProcessName = malloc(Length * 2 + 2);

    for (int i = 0; i < Length; i++)
    {
        ProcessName[i] = lpProcessName[i];
    }

    HideWindowFromProcessNameW(ProcessName);
}

VOID HideWindowFromProcessNameW(LPCWSTR lpProcessName)
{
    HIDDEN hidden;

    hidden.Hide = TRUE;
    hidden.Type = 0;
    hidden.Buffer = lpProcessName;

    EnumWindows(EnumWindowsProc, &hidden);
}

VOID HideWindowFromProcessId(DWORD dwProcessId)
{
    HIDDEN hidden;

    hidden.Hide = TRUE;
    hidden.Type = 1;
    hidden.Buffer = &dwProcessId;

    EnumWindows(EnumWindowsProc, &hidden);
}

VOID ShowWindowFromProcessNameA(LPCSTR lpProcessName)
{
    DWORD Length = strlen(lpProcessName);
    LPWSTR ProcessName = malloc(Length * 2 + 2);

    for (int i = 0; i < Length; i++)
    {
        ProcessName[i] = lpProcessName[i];
    }

    ShowWindowFromProcessNameW(ProcessName);
}

VOID ShowWindowFromProcessNameW(LPCWSTR lpProcessName)
{
    HIDDEN hidden;

    hidden.Hide = FALSE;
    hidden.Type = 0;
    hidden.Buffer = lpProcessName;

    EnumWindows(EnumWindowsProc, &hidden);
}

VOID ShowWindowFromProcessId(DWORD dwProcessId)
{
    HIDDEN hidden;

    hidden.Hide = FALSE;
    hidden.Type = 1;
    hidden.Buffer = &dwProcessId;

    EnumWindows(EnumWindowsProc, &hidden);
}