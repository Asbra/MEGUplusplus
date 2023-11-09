#ifndef GCore_h
#define GCore_h
#pragma once

/* GCore.dll
 * [[[[0x07FCC054]+0x48]]+0x58] is IsWindowOpen
E8 9A 54 01 00    call    Interface_GetGameInterface
8B 48 48          mov     ecx, [eax+48h]
8B 11             mov     edx, [ecx]
FF 52 58          call    dword ptr [edx+58h]           ; cWindow::IsVisible
84 C0             test    al, al
74 3F             jz      short loc_7B37831
*/

class GCore
{
public:
    HMODULE hModule = NULL;
    DWORD GCoreBase = 0;
private:
    bool Initialized = false;
    bool bWasFrozen = false;

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

    // dword_4CBED4
    class TimeManager
    {
    public:
    };

    class IObject
    {
    public:
        virtual void Function0();
    private:
    };

    class ICharacter : public IObject
    {
    public:
        char pad_0004[8]; //0x0004
        char *CharacterName; //0x000C
        char pad_0010[1072]; //0x0010
    private:
    };

    class TCharScriptData
    {
    public:
        char pad_0000[100]; //0x0000
        class Matrix* mWorld; // 0x0064 IObject::Mtx_GetWorld(void)
        class ICharacter* character; // 0x0068
        char pad_006C[24]; //0x006C
        bool state; // 0x0084
        char pad_0085[79]; //0x0085
        void* NavigationData; // 0x00D4
    private:
    };

    // GCore(HMODULE hMod) : hModule(hMod) {}

    TEInterface* GameInterface = nullptr;

    bool Initialize() {
        if (Initialized) return true;

        if (hModule == NULL) {
            if (!GetModuleHandleP("GCore.dll", &hModule))
                return false;
            GCoreBase = (DWORD)hModule;
            DBGPRINT("-- GCore.dll %p", GCoreBase);
        }

        /*
        void __cdecl TGO::EnableRender(bool a1) { *(_BYTE *)dword_4CC6E0 = a1; }
        8A 44 24 04       mov     al, [esp+arg_0]
        8B 0D E0 C6 4C 00 mov     ecx, dword_4CC6E0
        88 01             mov     [ecx], al
        C3                retn
        */
        uintptr_t* Renderer = *(uintptr_t**)(GCoreBase + 0x004CC6E0);
        if (!IsValidPtr(Renderer)) return false;
        // DBGPRINT("-- Renderer %p", Renderer);

        if (!IsHudEnabled()) return false;
        // DBGPRINT("-- IsHudEnabled %d", IsHudEnabled());

        /*
        DWORD addr = (DWORD)GCoreBase + 0x004CBED4;
        if (*(DWORD*)(addr + 0x50) == NULL)
            return false;
        */
        
        GameInterface = GetGameInterface();
        if (!IsValidPtr(GameInterface)) return false;

        /*
        char __cdecl TGO::InShellMode()
        {
          struct TEInterface *v0; // eax

          v0 = Interface_GetGameInterface();
          if ( !*((_DWORD *)GameInterface + 0x14) )
            return 1;
          return sub_2F0470(*((_DWORD *)GameInterface + 0x14));
        }
        */

        /* TEInterface* GameInterface
        * 0004
        * 000C
        * 0012
        * 001A
        * 0050
        *
        if (*(DWORD*)(GameInterface + 0x12) == 0 ||
            *(DWORD*)(GameInterface + 0x04) == 0 ||
            // *(INT32*)(GameInterface + 0x0C) != -1 ||
            *(DWORD*)(GameInterface + 0x50) == 0 ||
            *(DWORD*)(GameInterface + 0x1A) == 0)
            return false;
        */

        DBGPRINT("-- Renderer %p", Renderer);
        DBGPRINT("-- IsHudEnabled %d", IsHudEnabled());
        DBGPRINT("-- GameInterface %p", GameInterface);

        Initialized = true;
        return true;
    }

    bool Update() {
        if (hModule == NULL) return false;
        return true;
    }

    DebugWindow* GetConsole() {
        if (!IsValidPtr(GameInterface) || !IsValidPtr(GameInterface->ConsoleWindow)) return nullptr;
        // DBGPRINT("-- GameInterface->ConsoleWindow %p", GameInterface->ConsoleWindow);
        DebugWindow* console = (DebugWindow*)((uintptr_t)GameInterface->ConsoleWindow - 0x54);
        // DBGPRINT("-- console %p", console);
        return IsValidPtr(console) ? console : nullptr;
    }

    DebugWindow* GetStaticManager() {
    }

    DebugWindow* GetOctTree() {
    }

    bool ToggleConsole() {
        DebugWindow* console = GetConsole();
        if (!IsValidPtr(console)) return false;
        console->IsOpen = !console->IsOpen;
        return console->IsOpen;
    }

    bool SetConsoleOpen(bool state) {
        DebugWindow* console = GetConsole();
        if (!IsValidPtr(console)) return false;
        console->IsOpen = state;
        return console->IsOpen;
    }

    /* TEInterface* __cdecl Interface::GetGameInterface() { return dword_4CC054; }
    public ?Interface_GetGameInterface@@YAPAUTEInterface@@XZ
    A1 54 C0 4C 00    mov     eax, GameInterface
    C3                retn
    07B4CC80 GameInterface * Interface_GetGameInterface()
    A1 54 C0 FC 07    mov     eax, [7FCC054]
    C3                retn
    */
private:
    typedef TEInterface* (*tGetGameInterface)(void);
public:
    TEInterface* GetGameInterface()
    {
        void* pGetGameInterface = GetProcAddress(hModule, "?Interface_GetGameInterface@@YAPAUTEInterface@@XZ");
        TEInterface* GameInterface = ((tGetGameInterface)pGetGameInterface)();
        return IsValidPtr(GameInterface) ? GameInterface : nullptr;
    }

    /*
     * GCore.dll + 0x00051BE
    BOOL __cdecl TGO::IsGameFrozen()
      return *(_BYTE *)(dword_4CBED4 + 16) || *(_BYTE *)(*(_DWORD *)(dword_4CBED4 + 80) + 8);
    */
private:
    typedef BOOL(*tIsGameFrozen)(void);
public:
    BOOL __cdecl IsGameFrozen(void) {
        DWORD addr = GCoreBase + 0x004CBED4;
        return *(BYTE*)(addr + 16) || (*(DWORD*)(addr + 80)  && *(BYTE*)(*(DWORD*)(addr + 80) + 8));
        /*
        tIsGameFrozen func = (tIsGameFrozen)(GCoreBase + 0x00051BE0);
        func();
        */
    }

    /* TGO::FreezeGame(0, 1, 1);
    public ?FreezeGame@TGO@@SAX_N00@Z
    */
private:
    typedef void (*tFreezeGame)(bool state, bool, bool);
public:
    void FreezeGame(bool state, bool a2 = true, bool a3 = true) {
        tFreezeGame _FreezeGame = (tFreezeGame)(GCoreBase + 0x0005A110);
        _FreezeGame(state, a2, a3);
        bWasFrozen = state;
    }

    /* bool __cdecl TGO::IsHUDEnabled() { return byte_4C8CFC; }
    public ?IsHUDEnabled@TGO@@SA_NXZ
    A0 FC 8C 4C 00    mov     al, byte_4C8CFC
    C3                retn
    */
    bool IsHudEnabled() {
        return *(BYTE*)(GCoreBase + 0x004C8CFC);
    }

    /* 00050BE0 float __cdecl TGO::GetTotalElapsedGameTime()
    public ?GetTotalElapsedGameTime@TGO@@SAMXZ
    .text:00050BE0 A1 D4 BE 4C 00    mov     eax, dword_4CBED4
    .text:00050BE5 85 C0             test    eax, eax
    .text:00050BE7 74 07             jz      short loc_50BF0
    return *(float *)(*(_DWORD *)(dword_4CBED4 + 80) + 12)
    */
private:
    typedef double(*tGetTotalElapsedGameTime)(void);
public:
    float GetTotalElapsedGameTime()
    {
        DWORD addr = GCoreBase + 0x004CBED4;
        return (*(DWORD*)(addr + 80) && *(BYTE*)(*(DWORD*)(addr + 80) + 12));
        /*
        return *(float*)(*(DWORD*)(GCoreBase + 0x004CBED4 + 0x50) + 0x0C);
        tGetTotalElapsedGameTime GetTotalElapsedGameTime = (tGetTotalElapsedGameTime)(GCoreBase + 0x00050BE0);
        return GetTotalElapsedGameTime();
        */
    }

private:
    // ?HandleWallBumpingPositions@ICharacter@@QAEXAAU?$TArray@UTPushBoneData@@@@JM@Z
public:
    void __thiscall ICharacter__HandleWallBumpingPositions(ICharacter* a1, int a2, int a3, int a4)
    {
    }

    int sub_1006ADF0()
    {
        /*
        0966ADF0 | A1 B4C6AC09              | mov eax,dword ptr ds:[GCore+004CC6B4]
        0966ADF5 | C3                       | ret
        */
        return *(DWORD*)(GCoreBase + 0x004CC6B4); // dword_104CC6B4;
    }

public:
    IObject* __cdecl TGO__GetPlayerImp()
    {
        // typedef IObject* (*tTGO__GetPlayer)(void);
        // return ((tTGO__GetPlayer)GetProcAddress(hModule, "?GetPlayer@TGO@@SAPAUIObject@@XZ"))();

        void* pTGO__GetPlayer = GetProcAddress(hModule, "?GetPlayer@TGO@@SAPAUIObject@@XZ");
        return (*(IObject* (__cdecl*)(void))pTGO__GetPlayer)();
    }
    IObject* __cdecl TGO__GetPlayer() // TGO::GetPlayer
    {
        int v0 = 0;
        IObject* result = NULL;

        DWORD off_104CA954 = *(DWORD*)(GCoreBase + 0x004CA954); // [14][1]
        DWORD dword_104CBED4 = *(DWORD*)(GCoreBase + 0x004CBED4); // + 0x3D

        if (*(DWORD*)((*(DWORD*)(off_104CA954 + 0x38)) + 4) && !*(BYTE*)(dword_104CBED4 + 0x3D) && (v0 = sub_1006ADF0()) != 0)
            result = *(IObject**)(v0 + 0xC);

        return result;
    }

private:
public:
    class TGCoreInterface {};

    TGCoreInterface* __cdecl TGCoreInterface__GetGCoreInterface(void) {
        return (*(TGCoreInterface* (__cdecl*)(void))GetProcAddress(hModule, "?GetGCoreInterface@TGCoreInterface@@SAPAU1@XZ"))();
    }
    
    class IInstance {};
    class IObjectiveManager : public IInstance {};

    IObjectiveManager* __cdecl IObjectiveManager__GetSingleton(void) {
        return (*(IObjectiveManager* (__cdecl*)(void))GetProcAddress(hModule, "?GetSingleton@IObjectiveManager@@SAPAUIInstance@@XZ"))();
    }
    const char* __cdecl IObjectiveManager__GetScriptName(IObjectiveManager* self) {
        return (*(const char* (__cdecl*)(IObjectiveManager*))GetProcAddress(hModule, "?GetScriptName@IObjectiveManager@@UBEPBDXZ"))(self);
    }
};

/* GCore
  sub_7B484A0(*a2);
  v4 = Interface_GetGameInterface();
  if ( (*(unsigned __int8 (__thiscall **)(_DWORD))(**((_DWORD **)v4 + 18) + 88))(*((_DWORD *)v4 + 18)) )
  {
    if ( !TGO::IsGameFrozen() )
    {
      *(_BYTE *)(this + 281) = 0;
      v5 = TGO::Interface_MasterCamera();
      if ( v5 )
        *(_BYTE *)(this + 281) = (*(int (__thiscall **)(struct IMasterCamera *))(*(_DWORD *)v5 + 776))(v5);
      *(_BYTE *)(this + 280) = 1;
      TGO::FreezeGame(1, 1, 1);
    }
  }
  else if ( *(_BYTE *)(this + 280) )
  {
    *(_BYTE *)(this + 280) = 0;
    TGO::FreezeGame(0, 1, 1);
    if ( *(_BYTE *)(this + 281) )
    {
      v6 = TGO::Interface_MasterCamera();
      if ( v6 )
        (*(void (__thiscall **)(struct IMasterCamera *, int))(*(_DWORD *)v6 + 772))(v6, 1);
    }
  }

Interesting functions
  TGO::Cine_IsCutsceneRunning()

  v6 = TGO::Interface_MasterCamera();
  if ( v6 )
    (*(void (__thiscall **)(struct IMasterCamera *, int))(*(_DWORD *)v6 + 772))(v6, 1);

  v46 = 0;
  v49[4] = 1;
  v10 = IStreamedCinematicManager::GetClassScriptDef();
  TGO::List_GetNearObject(&v46, v49, v10, 1, -1, 0);

  v19 = TGO::GetCamera();
  TGO::InShellMode()
*/
#endif // GCore_h
