#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#define SERVICES_KEY "SYSTEM\\CurrentControlSet\\Services"

int main(void)
{
    HKEY hServices;
    DWORD i = 0;
    char szName[256];
    DWORD cbName;

    LONG openResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, SERVICES_KEY, 0, KEY_READ, &hServices);
    if (openResult != ERROR_SUCCESS) {
        fprintf(stderr, "Cannot open registry.\n");
        return 1;
    }

    printf("%-40s %-10s\n", "Service", "Start");

    for (;;) {
        HKEY hSvc;
        DWORD dwType = 0, dwStart = 0;
        DWORD cbVal = sizeof(DWORD);
        const char *startStr;

        cbName = sizeof(szName);
        LONG rc = RegEnumKeyExA(hServices, i++, szName, &cbName, NULL, NULL, NULL, NULL);

        if (rc == ERROR_NO_MORE_ITEMS)
            break;
        if (rc != ERROR_SUCCESS)
            continue;
        if (RegOpenKeyExA(hServices, szName, 0, KEY_READ, &hSvc) != ERROR_SUCCESS)
            continue;

        RegQueryValueExA(hSvc, "Type",  NULL, NULL, (LPBYTE)&dwType,  &cbVal);
        cbVal = sizeof(DWORD);
        RegQueryValueExA(hSvc, "Start", NULL, NULL, (LPBYTE)&dwStart, &cbVal);

        if (dwType == 0x10 || dwType == 0x20) {
            if      (dwStart == 2) startStr = "Automatic";
            else if (dwStart == 3) startStr = "Manual";
            else if (dwStart == 4) startStr = "Disabled";
            else                   startStr = "Boot/System";

            printf("%-40s  %-10s\n", szName, startStr);
        }

        RegCloseKey(hSvc);
    }

    RegCloseKey(hServices);
    return 0;
}