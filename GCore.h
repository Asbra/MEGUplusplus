#pragma once

class GCore
{
public:
    HMODULE hModule = NULL;
private:
    bool Initialized = false;
    bool bWasFrozen = false;

    // Typedefs
    typedef void (*tFreezeGame)(bool state, bool, bool);

public:
    class Window
    {
    public:
        char pad_0000[12]; //0x0000
        uint32_t Id; //0x000C
        char pad_0010[1]; //0x0010
        bool Active; //0x0011
        char Name[32]; //0x0012
        char pad_0032[98]; //0x0032
    }; //Size: 0x0094

    class Windows
    {
    public:
        class Window WindowArray[21]; //0x0000
        char pad_0C24[76]; //0x0C24
        void* N000011F8; //0x0C70
        uint8_t N000011F9; //0x0C74
        char pad_0C75[15]; //0x0C75
        float N000011FD; //0x0C84 00412661
        char pad_0C88[352]; //0x0C88
        bool IsConsoleOpen; //0x0DE8
    };

    class DebugWindow
    {
    public:
        char pad_0000[4]; //0x0000
        int32_t pos_x; //0x0004
        int32_t pos_y; //0x0008
        int32_t size_x; //0x000C
        int32_t size_y; //0x0010
        char pad_0014[4]; //0x0014
        bool IsOpen; //0x0018
        char pad_0019[3]; //0x0019
        char* Name; //0x001C
        char pad_0020[24]; //0x0020
    }; //Size: 0x0038

    class N00001866 // addr-0x54 = console DebugWindow
    {
    public:
        char pad_0004[64]; //0x0004
        virtual void Function0();
        virtual void Function1();
        virtual void Function2();
        virtual void Function3();
        virtual void Function4();
        virtual void Function5();
        virtual void Function6();
        virtual void Function7();
        virtual void Function8();
        virtual void Function9();
        virtual void Function10();
        virtual void Function11();
        virtual void Function12();
        virtual void Function13();
        virtual void Function14();
        virtual void Function15();
        virtual void Function16();
        virtual void Function17();
        virtual void Function18();
        virtual void Function19();
        virtual void Function20();
        virtual void Function21();
        virtual void IsWindowOpen_trampoline();
    }; //Size: 0x0044

    class TEInterface // GetGameInterface
    {
    public:
        char pad_0000[40]; //0x0000
        class Windows* Windows; //0x0028
        char pad_002C[8]; //0x002C
        class N00000F73* N00000E48; //0x0034
        char pad_0038[16]; //0x0038
        class N00001866* ConsoleWindow; //0x0048
    };

    TEInterface* GameInterface = nullptr;

    bool Initialize() {
        if (Initialized) return true;

        if (hModule == NULL)
            if (!GetModuleHandleP("GCore.dll", &hModule) || hModule == NULL)
                return false;
        DBGPRINT("++ GCore.dll %p", hModule);

        DBGPRINT("++ IsHudEnabled %d", IsHudEnabled());
        if (!IsHudEnabled())
            return false;

        /* void __cdecl TGO::EnableRender(bool a1) { *(_BYTE *)dword_4CC6E0 = a1; }
        8A 44 24 04       mov     al, [esp+arg_0]
        8B 0D E0 C6 4C 00 mov     ecx, dword_4CC6E0
        88 01             mov     [ecx], al
        C3                retn
        */
        uintptr_t* Renderer = *(uintptr_t**)((DWORD)hModule + 0x004CC6E0);
        DBGPRINT("++ Renderer %p", Renderer);
        if (!IsValidPtr(Renderer)) return false;

        GameInterface = GetGameInterface();
        DBGPRINT("++ GameInterface %p", GameInterface);
        if (!IsValidPtr(GameInterface)) return false;

        Initialized = true;
        return true;
    }

    bool Update() {
        if (hModule == NULL) return false;
        return true;
    }

    DebugWindow* GetConsole() {
        if (!IsValidPtr(GameInterface) || !IsValidPtr(GameInterface->ConsoleWindow)) return nullptr;
        DebugWindow* console = (DebugWindow*)((uintptr_t)GameInterface->ConsoleWindow - 0x54);
        return IsValidPtr(console) ? console : nullptr;
    }

    /* TEInterface* __cdecl Interface::GetGameInterface() { return dword_4CC054; }
    public ?Interface_GetGameInterface@@YAPAUTEInterface@@XZ
    A1 54 C0 4C 00    mov     eax, GameInterface
    C3                retn
    07B4CC80 GameInterface * Interface_GetGameInterface()
    A1 54 C0 FC 07    mov     eax, [7FCC054]
    C3                retn
    */
    TEInterface* GetGameInterface()
    {
        TEInterface* GameInterface = *(TEInterface**)((uintptr_t)hModule + 0x004CC054);
        DBGPRINT("++ GameInterface* %p", GameInterface);
        return IsValidPtr(GameInterface) ? GameInterface : nullptr;
    }

    /* GCore.dll + 0x00051BE
    BOOL __cdecl TGO::IsGameFrozen()
      return *(_BYTE *)(dword_4CBED4 + 16) || *(_BYTE *)(*(_DWORD *)(dword_4CBED4 + 80) + 8);
    */
    BOOL __cdecl IsGameFrozen(void) {
        DWORD addr = (DWORD)hModule + 0x004CBED4;
        return *(BYTE*)(addr + 16) || (*(DWORD*)(addr + 80)  && *(BYTE*)(*(DWORD*)(addr + 80) + 8));
    }

    /* TGO::FreezeGame(0, 1, 1);
    public ?FreezeGame@TGO@@SAX_N00@Z
    */
    void FreezeGame(bool state, bool a2 = true, bool a3 = true) {
        tFreezeGame _FreezeGame = (tFreezeGame)((DWORD)hModule + 0x0005A110);
        _FreezeGame(state, a2, a3);
        bWasFrozen = state;
    }

    /* bool __cdecl TGO::IsHUDEnabled() { return byte_4C8CFC; }
    public ?IsHUDEnabled@TGO@@SA_NXZ
    A0 FC 8C 4C 00    mov     al, byte_4C8CFC
    C3                retn
    */
    bool IsHudEnabled() {
        return *(BYTE*)((DWORD)hModule + 0x004C8CFC);
    }
};
