#pragma once

#include "GCore.h"     // GCore.dll
#include "GScript.h"   // GScript_Rhapsody.dll
#include "GRhapsody.h" // g_Rhapsody.sgl

class GettingUp
{
private:
    HMODULE hModule = NULL;
    bool Initialized = false;

public:
    GCore gcore = GCore();
    GScript gscript = GScript();
    GRhapsody rhapsody = GRhapsody();
    GCore::TEInterface* gameInterface = nullptr;

    GettingUp() {}

    bool Initialize() {
        if (Initialized) return true;

        if (hModule == NULL)
            if (!GetModuleHandleP("GettingUp.exe", &hModule) || hModule == NULL)
                return false;
        DBGPRINT("++ GettingUp.exe %p", hModule);

        if (!gcore.Initialize()) return false;
        if (!gscript.Initialize()) return false;
        if (!rhapsody.Initialize()) return false;

        Initialized = true;
        return true;
    }

    bool Update() {
        if (!gcore.Update()) { DBGPRINT("++ GCore::Update FAILED"); return false; }
        if (!gscript.Update()) { DBGPRINT("++ GScript::Update FAILED"); return false; }
        if (!rhapsody.Update()) { DBGPRINT("++ GRhapsody::Update FAILED"); return false; }
        return true;
    }
};
