#include "MEGU.h"

#include <MinHook.h>

HMODULE pluginModule = NULL;

bool quit = false;
DWORD d3d8dev = 0;

bool openConsole = false;
bool noClip = false;

GettingUp game = GettingUp();

/*
Hook here to get on the main thread, it's a renderer call
char __thiscall sub_418980(int this)

here's the referene call to it
.text:00403101                 mov     ecx, dword_663BD4
.text:00403107                 mov     edx, [ecx]
.text:00403109                 call    dword ptr [edx+14h]
*/

static int ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
    // GetExceptionCode()
    DBGPRINT("-- Exception filter hit 0x%p", (void*)code);
    // return code == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
    return EXCEPTION_EXECUTE_HANDLER;
}

DWORD __stdcall ThreadMain(LPVOID lpThreadParameter)
{
    DBGPRINT("-- MEGU++ ThreadMain ..");

    while (!quit)
    {
        Sleep(10);

        __try {
            if (!game.Initialize() || !game.Update())
                continue;

            // bool different = GetModuleHandleP("d3d8.dll", &hmD3D8);

            GCore gcore = game.gcore;
            GRhapsody rhapsody = game.rhapsody;

            if (gcore.hModule != NULL) {
                // TGCoreInterface* __cdecl TGCoreInterface::GetGCoreInterface()
                // void* pfn1 = GetProcAddress(gcore.hModule, "?GetGCoreInterface@TGCoreInterface@@SAPAU1@XZ");
                // DBGPRINT("pGetGCoreInterface #1\t%p", pfn1);
            }

            /*
            * 10000000 g_rhapsody.sgl
            * ?IsInitialized@TTaggingAreaManager@@QAE_NXZ
            10003070 | 8A81 95010000            | mov al,byte ptr ds:[ecx+195]
            10003076 | C3                       | ret
            */

            // gcore.FreezeGame(GetKeyState(VK_OEM_3) ? true : false);

            /*
            08763758 | E8 4311F9FF              | call <gcore.?GetSingleton@TCommandManager@@SAPAUIInstance@@XZ>                  |
            0876375D | 8BC8                     | mov ecx,eax                                                                     | ecx:&"�G", eax:"�G"
            0876375F | E8 7C50F9FF              | call <gcore.?Tick@TCommandManager@@QAEXXZ>                                      |

            0876378B | E8 A0DBFDFF              | call <gcore.?GetPlayer@TGO@@SAPAUIObject@@XZ>                                   |
            08763790 | 8BC8                     | mov ecx,eax                                                                     | ecx:&"�G", eax:"�G"
            08763792 | E8 596E0500              | call <gcore.?TickOrFixedTick@IObject@@QAEXKABUTGTickParams@@@Z>                 |
            08763797 | 8D4C24 10                | lea ecx,dword ptr ss:[esp+10]                                                   |
            0876379B | 51                       | push ecx                                                                        | ecx:&"�G"
            0876379C | 56                       | push esi                                                                        |
            0876379D | E8 4EDBFDFF              | call <gcore.?GetCamera@TGO@@SAPAUIObject@@XZ>                                   |
            087637A2 | 8BC8                     | mov ecx,eax                                                                     | ecx:&"�G", eax:"�G"
            087637A4 | E8 476E0500              | call <gcore.?TickOrFixedTick@IObject@@QAEXKABUTGTickParams@@@Z>                 |
            */

            /*
            static bool showLogo = false;
            if (GetAsyncKeyState(VK_F4) & 1)
                showLogo = !showLogo;
            if (showLogo)
                game.DrawString("HugePackets", 0xFF00FF00);
            */

            ///////////////////////////////////////////////////////////////
            // Handle hotkeys
            //
            if (GetAsyncKeyState(VK_F1) & 1) // 0x8000
            {
                openConsole = !openConsole;
                
                GCore::DebugWindow* console = gcore.GetConsole();
                if (IsValidPtr(console)) {
                    // if (openConsole && !console->IsOpen)
                    //     console->IsOpen = true;
                    // if (!openConsole && console->IsOpen)
                    //     console->IsOpen = false;
                    console->IsOpen = openConsole;
                }
            }

            if (GetAsyncKeyState(VK_F2) & 1)
                noClip = !noClip;

            if (noClip)
            {
            }
            
            if (GetAsyncKeyState(VK_F3) & 1) {
                GCore::ICharacter* player = reinterpret_cast<GCore::ICharacter*>(gcore.TGO__GetPlayer());
                DBGPRINT("-- Player\t%p", player);

                player = reinterpret_cast<GCore::ICharacter*>(gcore.TGO__GetPlayerImp());
                DBGPRINT("-- Player\t%p", player);

                GCore::TGCoreInterface* gcoreInterface = gcore.TGCoreInterface__GetGCoreInterface();
                DBGPRINT("-- GCoreInterface\t%p", gcoreInterface);

                GRhapsody::TGRhapsodyInterface* rhapsodyInterface = rhapsody.GetInterface();
                DBGPRINT("-- GRhapsodyInterface\t%p", rhapsodyInterface);
                // DBGPRINT("-- GameName\t%s", rhapsody.GetGameName(rhapsodyInterface));

                GCore::IObjectiveManager* objectiveMgr = gcore.IObjectiveManager__GetSingleton();
                DBGPRINT("-- ObjectiveManager\t%p", objectiveMgr);
                DBGPRINT("-- ScriptName\t%s", gcore.IObjectiveManager__GetScriptName(objectiveMgr));
            }

            /*
            D3D8 CreateTexture call in GettingUp.exe
            004FCD76 | A1 00F1A800              | mov eax,dword ptr ds:[A8F100] ; Direct3D8 Device
            004FCD7B | 8B10                     | mov edx,dword ptr ds:[eax]    ; d3d8dev vtable
            004FCD7D | 8D6F 14                  | lea ebp,dword ptr ds:[edi+14]
            004FCD80 | 55                       | push ebp
            004FCD81 | 6A 01                    | push 1
            004FCD83 | 53                       | push ebx
            004FCD84 | 6A 00                    | push 0
            004FCD86 | 51                       | push ecx
            004FCD87 | 8B4E 08                  | mov ecx,dword ptr ds:[esi+8]
            004FCD8A | 51                       | push ecx
            004FCD8B | 8B4E 04                  | mov ecx,dword ptr ds:[esi+4]
            004FCD8E | 51                       | push ecx
            004FCD8F | 50                       | push eax
            004FCD90 | FF52 50                  | call dword ptr ds:[edx+50]    ; Direct3D8Device::CreateTexture call
            */
            /*
            if (*((DWORD*)((DWORD)hmGettingUp + 0x004FCD77)) != d3d8dev) {
                d3d8dev = *((DWORD*)((DWORD)hmGettingUp + 0x004FCD77)); // 0x00A8F100
                if (d3d8dev != 0) {
                    DBGPRINT("-- Direct3D8 Device %p", (void*)d3d8dev);
                }
            }
            */
        }
        __except (ExceptionFilter(GetExceptionCode(), GetExceptionInformation())) {
            DBGPRINT("-- ACCESS VIOLATION in ThreadMain");
        }
    }

    DBGPRINT("-- MEGU++ Unloading");

    return 0;
}

void Initialize()
{
    DBGPRINT(" ");
    DBGPRINT(" ");
    DBGPRINT("--------------------------------------------------------------------------------");
    DBGPRINT(" ");
    DBGPRINT(" ");

    DBGPRINT("-- MEGU++ Loading ..");
    
    DBGPRINT("-- Module address %p", (void*)pluginModule);

    DBGPRINT("-- Hooking resource manager ..");

    if (MH_Initialize() != MH_OK) {
        DBGPRINT("!! Failed to initialize MinHook");
        return;
    }
    else {
        // Hook file system to load mod files instead
        ModOverrides__Initialize();
    }

    /*
    HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook
    NTSTATUS result = LhInstallHook(pFOpen,  HookFOpen, NULL, &hHook);
    if (FAILED(result))
    {
        DBGPRINT("-- FAILED TO HOOK FOPEN");
        return 0;
    }
    */
    
    DBGPRINT("-- Initializing Lua scripting interface ..");
    lua_State* L = luaL_newstate();
    DBGPRINT("-- Lua state %p", (void*)L);
    luaL_dostring(L, " x = 42"); // Execute lua code.
    lua_getglobal(L, "x"); // Get global variable called 'x' and push it to the start of the Lua stack.
    lua_Number x = lua_tonumber(L, 1); // Get the value at the top of the stack.
    DBGPRINT("x = %f", x);
    lua_close(L);

    CreateThread(0, 0, ThreadMain, 0, 0, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  fdwReason, LPVOID lpReserved)
{
    pluginModule = hModule;
    if (fdwReason == DLL_PROCESS_ATTACH) { Initialize(); }
    else if (fdwReason == DLL_PROCESS_DETACH) {}
    else if (fdwReason == DLL_THREAD_ATTACH) {}
    else if (fdwReason == DLL_THREAD_DETACH) {}
    return TRUE;
}
