#pragma once

class GScript
{
private:
    HMODULE hModule = NULL;
    bool Initialized = false;
    bool bWasFrozen = false;

    GCore gcore = GCore();

    typedef void (*tFreezeGame)(bool state, bool, bool);
    typedef BOOL(*tIsGameFrozen)(void);

public:
    GScript() {}

    bool Initialize() {
        if (Initialized) return true;

        if (hModule == NULL)
            if (!GetModuleHandleP("GScript_Rhapsody.dll", &hModule) || hModule == NULL)
                return false;
        DBGPRINT("++ GScript_Rhapsody.dll %p", hModule);

        Initialized = true;
        return true;
    }

    bool Update() {
        if (hModule == NULL) return false;
        return true;
    }
};
