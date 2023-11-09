#include "ModOverrides.h"

#include <MEGU.h>
#include <MinHook.h>

#include <Modules/GettingUp.h>

const char* root_path = (const char*)0x00666278;

typedef FILE* (_cdecl* DEF_FOPEN)(const char*, const char*);
DEF_FOPEN oFOpen = NULL;

FILE* HookFOpen(const char* filename, const char* mode)
{
    if (root_path == nullptr ||
        root_path[0] == 0) {
        DBGPRINT("-- No root directory specified");
        return oFOpen(filename, mode);
    }

    // Only check files if they're being opened for reading
    if (strcmp(mode, "rb") != 0) {
        return oFOpen(filename, mode);
    }

    // Only check files that are in the game folder
    if (strncmp(filename, root_path, strlen(root_path)) != 0) {
        DBGPRINT("-- File outside of game file system %s", filename);
        return oFOpen(filename, mode);
    }

    // Find "\engine\" in the path
    char* engine = strstr((char*)filename, "\\engine\\");
    if (engine == NULL) {
        DBGPRINT("-- Engine not in path %s", filename);
        return oFOpen(filename, mode);
    }

    // Replace "\engine\" with "\modded\" to redirect the file
    strncpy(engine, "\\modded\\", 8);

    // Check if we have a override for the current file
    FILE* fpOverride = oFOpen(filename, mode);
    if (fpOverride != NULL) {
        // Give the game the override instead of original asset
        DBGPRINT("-- Mod override %s", filename);
        return fpOverride;
    }

    // There's no override, return original file
    strncpy(engine, "\\engine\\", 8);

    DBGPRINT("-- fopen %s", filename);

    return oFOpen(filename, mode);
}

bool ModOverrides__Initialize()
{
    HMODULE hMSVCR = GetModuleHandle(L"msvcr71.dll");
    DBGPRINT("-- msvcr71\t%p", hMSVCR);
    if (hMSVCR == NULL) return false;

    void* pFOpen = GetProcAddress(hMSVCR, "fopen");
    DBGPRINT("-- fopen\t%p", pFOpen);
    if (pFOpen == NULL) return false;

    if (MH_CreateHook(pFOpen, &HookFOpen, reinterpret_cast<LPVOID*>(&oFOpen)) != MH_OK)
    {
        DBGPRINT("!! Failed to hook msvcr71!fopen");
        return false;
    }
    else
    {
        if (MH_EnableHook(pFOpen) != MH_OK)
        {
            DBGPRINT("!! Failed to enable msvcr71!fopen hook");
            return false;
        }
        else
        {
            DBGPRINT("-- msvcr71!fopen hooked!");
            return false;
        }
    }

    return true;
}
