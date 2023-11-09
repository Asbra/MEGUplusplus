#include <Memory/Utils.h>

BYTE WriteByte(LPVOID lpAddress, BYTE bValue)
{
    DWORD dwOldProtection = 0;
    BYTE error = 0;

    if (*((BYTE*)lpAddress) == bValue)
        return 1;

    if (!VirtualProtect(lpAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtection))
        return 2;

    *((BYTE*)lpAddress) = bValue;

    if (*((BYTE*)lpAddress) != bValue)
        error = 3;

    if (!VirtualProtect(lpAddress, 1, dwOldProtection, &dwOldProtection))
        return 4;

    return error;
}

BOOL GetModuleHandleP(LPCSTR lpModuleName, HMODULE* hModule)
{
    HMODULE newModule = GetModuleHandleA(lpModuleName);
    if (newModule != NULL && *hModule != newModule) {
        *hModule = newModule;
        return TRUE;
    }
    return (newModule != NULL);
}
