#include <windows.h>
#include <stdio.h>

int main(void) {
    HKEY hKey;
    LONG result;
    DWORD index = 0;
    char subKeyName[256];
    DWORD subKeyNameSize;

    result = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software",
        0,
        KEY_READ,
        &hKey
    );

    if (result != ERROR_SUCCESS) {
        printf("Eroare la deschiderea cheii din Registry. Cod: %ld\n", result);
        return 1;
    }

    printf("Subcheile din HKEY_CURRENT_USER\\Software sunt:\n\n");

    while (1) {
        subKeyNameSize = sizeof(subKeyName);

        result = RegEnumKeyExA(
            hKey,
            index,
            subKeyName,
            &subKeyNameSize,
            NULL,
            NULL,
            NULL,
            NULL
        );

        if (result == ERROR_NO_MORE_ITEMS) {
            break;
        }

        if (result == ERROR_SUCCESS) {
            printf("%lu. %s\n", index + 1, subKeyName);
        } else {
            printf("Eroare la citirea subcheii %lu. Cod: %ld\n", index, result);
        }

        index++;
    }

    RegCloseKey(hKey);
    return 0;
}