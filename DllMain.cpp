#include "MEGU.h"

HMODULE pluginModule = NULL;

bool openConsole = false;

DWORD __stdcall ThreadMain(LPVOID lpThreadParameter)
{
    DBGPRINT("++ MEGU++ Loading ..");

    DBGPRINT("++ Module address %p", (void*)pluginModule);

    GettingUp game = GettingUp();

    while (!quit)
    {
        Sleep(10);

        if (!game.Initialize() || !game.Update())
            continue;

        GCore gcore = game.gcore;

        if (GetAsyncKeyState(VK_F1) & 1) // 0x8000
            openConsole = !openConsole;

        GCore::DebugWindow* console = gcore.GetConsole();
        if (IsValidPtr(console))
            console->IsOpen = openConsole;
    }

    DBGPRINT("++ MEGU++ Unloading");

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  fdwReason, LPVOID lpReserved)
{
    pluginModule = hModule;
    if (fdwReason == DLL_PROCESS_ATTACH) { CreateThread(0, 0, ThreadMain, 0, 0, 0); }
    else if (fdwReason == DLL_PROCESS_DETACH) {}
    else if (fdwReason == DLL_THREAD_ATTACH) {}
    else if (fdwReason == DLL_THREAD_DETACH) {}
    return TRUE;
}
