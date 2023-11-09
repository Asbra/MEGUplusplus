#ifndef GettingUp_h
#define GettingUp_h
#pragma once

#include "GCore.h"     // GCore.dll
#include "GScript.h"   // GScript_Rhapsody.dll
#include "GRhapsody.h" // g_Rhapsody.sgl

class GettingUp
{
private:
    HMODULE hModule = NULL;

public:
    GCore gcore = GCore();
    GScript gscript = GScript();
    GRhapsody rhapsody = GRhapsody();
    GCore::TEInterface* gameInterface = nullptr;
    
    bool Initialized = false;

    class D3DRasterizer //static address 00A8F078 (GettingUp+0068F078)
    {
    public:
        char pad_0000[4]; //0x0000
        bool N0000118A; //0x0004
        bool N000011F2; //0x0005
        bool N000011F5; //0x0006
        bool N000011F3; //0x0007
        int32_t N0000118B; //0x0008
        int32_t N0000118C; //0x000C
        int32_t N000010CC; //0x0010
        int32_t N000010D0; //0x0014
        class DisplayAdapter* N000010D1; //0x0018
        class DisplayAdapter* N00001006; //0x001C
        bool N000010DE; //0x0020
        char pad_0021[3]; //0x0021
        int32_t N000010DF; //0x0024
        char pad_0028[12]; //0x0028
        void* N000010E3; //0x0034
        void* N000010E4; //0x0038
        char pad_003C[72]; //0x003C
        class IDirect3D8* d3d8; //0x0084 //00A8F0FC
        class IDirect3DDevice8* d3d8device; //0x0088 //00A8F100
        char pad_008C[20]; //0x008C
    }; //Size: 0x00A0

    // GettingUp(HMODULE hMod) : hModule(hMod) {}
    GettingUp() {}

    bool Initialize() {
        if (Initialized) return true;

        if (hModule == NULL) {
            if (!GetModuleHandleP("GettingUp.exe", &hModule) || hModule == NULL)
                return false;
            DBGPRINT("-- GettingUp.exe %p", hModule);
        }

        if (!gcore.Initialize()) return false;
        if (!gscript.Initialize()) return false;
        if (!rhapsody.Initialize()) return false;

        Initialized = true;
        return true;
    }

    bool Update() {
        if (!gcore.Update()) { DBGPRINT("-- GCore::Update FAILED"); return false; }
        if (!gscript.Update()) { DBGPRINT("-- GScript::Update FAILED"); return false; }
        if (!rhapsody.Update()) { DBGPRINT("-- GRhapsody::Update FAILED"); return false; }
        return true;
    }

    /* 004234E0
        (*(void (__thiscall **)(int (__stdcall *)(char), _DWORD, _DWORD, const char *, unsigned int, int, int, _DWORD))(*(_DWORD *)off_4CA954[14][13] + 0x184))(
          off_4CA954[14][13],
          LODWORD(v37),
          LODWORD(v41),
          "SPIKES",
          0xFF00FF00,
          0x3F000000,
          0x3F000000,
          0);
        BOOL __stdcall sub_4234E0(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
        {
          int v7; // ecx

          v7 = *(_DWORD *)(dword_66BA90 + 0x18);
          return v7
              && (*(unsigned __int8 (__thiscall **)(int, int, int, int, int, int, int, int, int))(*(_DWORD *)v7 + 0x48))(
                   v7,
                   a1,
                   a2,
                   a3,
                   a4,
                   a5,
                   a6,
                   -1,
                   a7);
        }

        004234E0 | A1 90BA6600              | mov eax,dword ptr ds:[66BA90]                                            | Draw Debug String?
        004234E5 | 8B48 18                  | mov ecx,dword ptr ds:[eax+18]                                            | ecx:"0�T", eax+18:"P�B"
        004234E8 | 85C9                     | test ecx,ecx                                                             | ecx:"0�T"
        004234EA | 74 3A                    | je gettingup.423526                                                      |
        004234EC | 8B5424 1C                | mov edx,dword ptr ss:[esp+1C]                                            |
        004234F0 | 8B4424 18                | mov eax,dword ptr ss:[esp+18]                                            |
        004234F4 | 52                       | push edx                                                                 |
        004234F5 | 8B5424 18                | mov edx,dword ptr ss:[esp+18]                                            |
        004234F9 | 6A FF                    | push FFFFFFFF                                                            |
        004234FB | 50                       | push eax                                                                 |
        004234FC | 52                       | push edx                                                                 |
        004234FD | 8B5424 20                | mov edx,dword ptr ss:[esp+20]                                            | [esp+20]:sub_8804BC0+121
        00423501 | 51                       | push ecx                                                                 | ecx:"0�T"
        00423502 | 8BC4                     | mov eax,esp                                                              |
        00423504 | 8910                     | mov dword ptr ds:[eax],edx                                               |
        00423506 | 8B5424 20                | mov edx,dword ptr ss:[esp+20]                                            | [esp+20]:sub_8804BC0+121
        0042350A | 8B01                     | mov eax,dword ptr ds:[ecx]                                               | ecx:"0�T"
        0042350C | 52                       | push edx                                                                 |
        0042350D | 8B5424 20                | mov edx,dword ptr ss:[esp+20]                                            | [esp+20]:sub_8804BC0+121
        00423511 | 52                       | push edx                                                                 |
        00423512 | 8B5424 20                | mov edx,dword ptr ss:[esp+20]                                            | [esp+20]:sub_8804BC0+121
        00423516 | 52                       | push edx                                                                 |
        00423517 | FF50 48                  | call dword ptr ds:[eax+48]                                               | [eax+48]:"��\fV��\r��f"
        0042351A | 84C0                     | test al,al                                                               |
        0042351C | 74 08                    | je gettingup.423526                                                      |
        0042351E | B8 01000000              | mov eax,1                                                                |
        00423523 | C2 1C00                  | ret 1C                                                                   |
        00423526 | 33C0                     | xor eax,eax                                                              |
        00423528 | C2 1C00                  | ret 1C                                                                   |
    */
private:
    typedef BOOL(__fastcall* pGettingUp__DrawDebugString)(uintptr_t, uint32_t x, uint32_t y, const char* text, uint32_t color, float scaleX, float scaleY, uint32_t a6);
public:
    /* This is called inside of void __cdecl TGORender::Text3D(int a1, const char **a2, const struct TVector *a3, const struct TRGBA *a4) */
    bool DrawString(const char* string, int color = 0xFFFFFFFF)
    {
        /*
        08709B28 | 8B0D 54A9B808            | mov ecx,dword ptr ds:[8B8A954]
        08709B2E | 8B51 38                  | mov edx,dword ptr ds:[ecx+38]
        08709B31 | 8B4A 34                  | mov ecx,dword ptr ds:[edx+34]
        08709B34 | 8B01                     | mov eax,dword ptr ds:[ecx]
        */
        DWORD off_4CA954 = *(DWORD*)((DWORD)gcore.hModule + 0x004CA954);
        DWORD ptr1 = *(DWORD*)(off_4CA954 + 0x38);
        DWORD klass = *(DWORD*)(off_4CA954 + 0x34);
        DWORD vtable = *(DWORD*)(klass);
        /*
        BOOL __stdcall sub_4234E0(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
        {
          int v7; // ecx

          v7 = *(_DWORD *)(dword_66BA90 + 0x18);
          return v7
              && (*(unsigned __int8 (__thiscall **)(int, int, int, int, int, int, int, int, int))(*(_DWORD *)v7 + 0x48))(
                   v7,
                   a1,
                   a2,
                   a3,
                   a4,
                   a5,
                   a6,
                   -1,
                   a7);
        }
        */
        pGettingUp__DrawDebugString GettingUp__DrawDebugString = (pGettingUp__DrawDebugString)0x004234E0;
        return GettingUp__DrawDebugString(vtable, 100, 100, string, color, 0.5f, 0.5f, 0);
        /*
        (*(void (__thiscall **)(int (__stdcall *)(char), _DWORD, _DWORD, const char *, unsigned int, int, int, _DWORD))(*(_DWORD *)off_4CA954[14][13] + 0x184))(
          off_4CA954[14][13],
          LODWORD(v37),
          LODWORD(v41),
          "SPIKES",
          0xFF00FF00,
          0x3F000000,
          0x3F000000,
          0);

        (*(void (__thiscall **)(int (__stdcall *)(char), _DWORD, _DWORD, const char *, int, float, float, _DWORD))(*(_DWORD *)off_4CA954[14][13] + 0x184))(
          off_4CA954[14][13],
          LODWORD(v9),
          LODWORD(v10),
          a2[v5++],
          v11,
          COERCE_FLOAT(LODWORD(v13)),
          COERCE_FLOAT(LODWORD(v13)),
          0);
        */
    }

private:
    typedef void(__cdecl* pGettingUp__LogString)(LPCSTR lpOutputString);
public:
    void LogString(LPCSTR lpOutputString)
    {
        pGettingUp__LogString GettingUp__LogString = (pGettingUp__LogString)0x00402390;
        GettingUp__LogString(lpOutputString);
    }

private:
public:
    const char* GetRootPath()
    {
        /*
        .text:00408450 ; const char *__stdcall GetRootPath()
        .text:00408450                 mov     eax, offset byte_666278
        .text:00408455                 retn
        */
        void* pGetRootPath = (void*)((DWORD)hModule + 0x00008450);
        return (*(const char* (__cdecl*)(void))pGetRootPath)();
    }
};

/* TWindow
0018 Show (boolean)
001C *Name ("Console"/"Scene OctTree"/"TStaticManager")
*/

/* Static pointers to console window
[[["GettingUp.exe"+00168160]+70]+2C]+10
[[["GettingUp.exe"+0026BA90]+70]+2C]+10
[[["GCore.dll"+004CA998]+70]+2C]+10
[[["GCore.dll"+004CC054]+70]+2C]+10
[[["GCore.dll"+004E3CA4]+70]+2C]+10
[[["GScript_rhapsody.DLL"+001F4268]+70]+2C]+10
[[["G_rhapsody.sgl"+002E52C0]+70]+2C]+10
[[["GettingUp.exe"+0026BCAC]+38]+70]+2C]+10
[[["GettingUp.exe"+0068F0B0]+38]+70]+2C]+10
[[["GCore.dll"+004CC61C]+38]+70]+2C]+10
[[["GScript_rhapsody.dll"+00233758]+38]+70]+2C]+10
*/

/*
                  TWindow::IsVisible 0047A240
8A 41 18          mov     al, [ecx+18h]
C3                retn

char __thiscall IsWindowOpen(_BYTE *this)
{
  return this[24];
}
*/

/*
alloc 1000, 03DE0000

mov al, byte ptr ds:[ecx+0x18]
jmp 0x03DE0000
ret

push ebx
mov ebx, [ecx+0x1C]
mov ebx, [ebx]
cmp ebx, 0x736E6F43
je 0x03DE0020
cmp ebx, 0x6E656353
je 0x03DE0020
cmp ebx, 0x61745354
je 0x03DE0020
mov al, 1
jmp 0x0047A248
53 8B 59 1C 8B 1B 81 FB 43 6F 6E 73 74 12 81 FB 53 63 65 6E 74 0A 81 FB 54 53 74 61 74 02 B0 01 5B E9 22 A2 69 FC
*/

/*
* Developer console state [ecx+18] 0/1
8A 41 18                      mov     al, [ecx+18h]
C3                            retn
*/

// NAKED_HOOK_CONSOLE_OPEN_HOTKEY
/* IsWindowOpen
.text:0047A240 8A 41 18                                      mov     al, [ecx+18h]
.text:0047A243 C3                                            retn
0047A240 | 8A41 18                  | mov al,byte ptr ds:[ecx+18]
0047A243 | C3                       | ret
char __thiscall IsWindowOpen(_BYTE *this)
{
  return this[24];
}
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
    char *Name; //0x001C
    char pad_0020[24]; //0x0020
}; //Size: 0x0038

called from
088377E1 | E8 9A540100              | call <gcore.?Interface_GetGameInterface@@YAPAUTEInterface@@XZ>           |
088377E6 | 8B48 48                  | mov ecx,dword ptr ds:[eax+48]                                            | eax+48:&"X�T"
088377E9 | 8B11                     | mov edx,dword ptr ds:[ecx]                                               | edx:"@wG", [ecx]:"@wG"
088377EB | FF52 58                  | call dword ptr ds:[edx+58]                                               |
*/
/*
char __fastcall hk_console(int ecx)
{
    char result; // al
    SHORT keystate = 0;
    DWORD name = **(DWORD**)(ecx + 0x1C);

    if (name == "Console")
    {
        keystate = GetKeyState(VK_NUMPAD0);
        *(BYTE*)(ecx + 0x18) = keystate ? 1 : 0;
    }
    else if (name == "Scene OctTree")
    {
        keystate = GetKeyState(VK_NUMPAD1);
        *(BYTE*)(ecx + 0x18) = keystate ? 1 : 0;
    }
    else if (name == "TStaticManager")
    {
        keystate = GetKeyState(VK_NUMPAD2);
        *(BYTE*)(ecx + 0x18) = keystate ? 1 : 0;
    }

    result = *(BYTE*)(ecx + 0x18);

    return result;
}

BOOL Patches_GettingUp(HMODULE hmGettingUp)
{
    DBGPRINT("-- Patching GettingUp.exe");

    return TRUE;
}
*/
//--------------------------------------
#endif // GettingUp_h
