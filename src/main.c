#include "HideWindow.h"

int main(int argc, char *argv[])
{
    DWORD ProcessId;

    printf("Hide process id : ");
    scanf("%d", &ProcessId);

    HideWindowFromProcessId(ProcessId);
    Sleep(5000);
    ShowWindowFromProcessId(ProcessId);

    system("pause");
}