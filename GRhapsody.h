#pragma once

/* IRhapsodyHudManager* IRhapsodyHudManager::GetSingleton()
100E9560 | A1 F8002F10              | mov eax,dword ptr ds:[102F00F8]
100E9565 | 85C0                     | test eax,eax
100E9567 | 74 09                    | je g_rhapsody.100E9572
100E9569 | F740 10 00000400         | test dword ptr ds:[eax+10],40000
100E9570 | 75 02                    | jne g_rhapsody.100E9574
100E9572 | 33C0                     | xor eax,eax
100E9574 | C3                       | ret
if ( !dword_102F00F8 || (*(_DWORD *)(dword_102F00F8 + 16) & 0x40000) == 0 ) return 0;
does this check on all singletons
*/

class GRhapsody
{
private:
    HMODULE hModule = NULL;
    bool Initialized = false;
    bool bWasFrozen = false;

    GCore gcore = GCore();

public:
    GRhapsody() {}

    bool Initialize() {
        if (Initialized) return true;

        if (hModule == NULL)
            if (!GetModuleHandleP("g_Rhapsody.sgl", &hModule) || hModule == NULL)
                return false;
        DBGPRINT("++ g_Rhapsody.sgl %p", hModule);

        if (!ExpandGraffitiDecalLimit()) return false;

        Initialized = true;
        return true;
    }

    bool Update() {
        if (hModule == NULL) return false;

        return true;
    }

    // Increase graffiti decal (spray tag,marker tag,poster,sticker,stencil,...) limit from 25 to 127
    bool ExpandGraffitiDecalLimit()
    {
        /*
        * g_Rhapsody.sgl 10000000
        * Patch 0x19 to a larger number (max 0x7F or signed overflow)
        1019BEDB | 56                       | push esi
        1019BEDC | 8DB40A 24020000          | lea esi,dword ptr ds:[edx+ecx+224]
        1019BEE3 | 8D0480                   | lea eax,dword ptr ds:[eax+eax*4]
        1019BEE6 | 8B1485 30EC2D10          | mov edx,dword ptr ds:[eax*4+102DEC30]
        1019BEED | 8B46 78                  | mov eax,dword ptr ds:[esi+78]
        1019BEF0 | 6BD2 1C                  | imul edx,edx,1C
        1019BEF3 | 83F8 19                  | cmp eax,19
        1019BEF6 | 57                       | push edi
        1019BEF7 | 8D7E 74                  | lea edi,dword ptr ds:[esi+74]
        * Also here
        1019BF9F | 66:FF43 14               | inc word ptr ds:[ebx+14]
        1019BFA3 | 8B8E 80000000            | mov ecx,dword ptr ds:[esi+80]
        1019BFA9 | 41                       | inc ecx
        1019BFAA | 8BC1                     | mov eax,ecx
        1019BFAC | 83F8 19                  | cmp eax,19
        1019BFAF | 898E 80000000            | mov dword ptr ds:[esi+80],ecx
        * Also here
        1019C30D | 8BD5                     | mov edx,ebp
        1019C30F | 69D2 84000000            | imul edx,edx,84
        1019C315 | 6A 19                    | push 19
        1019C317 | 8D8C1A 98020000          | lea ecx,dword ptr ds:[edx+ebx+298]
        1019C31E | E8 7DF5FFFF              | call g_rhapsody.1019B8A0
        1019C323 | 45                       | inc ebp
        1019C324 | 83FD 05                  | cmp ebp,5
        *
        * esi is the Graffiti object
        * CGraffitiList
        * 0074 MaxSize
        * 0078 Size
        * 007C pList
        * 0080 Index
        */
        LPVOID Offset1 = (LPVOID)((DWORD)hModule + 0x0019BEF5);
        LPVOID Offset2 = (LPVOID)((DWORD)hModule + 0x0019BFAE);
        LPVOID Offset3 = (LPVOID)((DWORD)hModule + 0x0019C316);

        if (*((BYTE*)Offset1) == 0x19) {
            if (WriteByte(Offset1, 0x7f) == 0) {
                DBGPRINT("++ Decal limit #1 patched @ %p [19 -> %02X]", Offset1, *((BYTE*)Offset1));
            }
            else {
                DBGPRINT("++ FAILED to patch decal limit #1 @ %p [%02X]", Offset1, *((BYTE*)Offset1));
            }
        }
        if (*((BYTE*)Offset2) == 0x19) {
            if (WriteByte(Offset2, 0x7f) == 0) {
                DBGPRINT("++ Decal limit #2 patched @ %p [19 -> %02X]", Offset2, *((BYTE*)Offset2));
            }
            else {
                DBGPRINT("++ FAILED to patch decal limit #2 @ %p [%02X]", Offset2, *((BYTE*)Offset2));
            }
        }
        if (*((BYTE*)Offset3) == 0x19) {
            if (WriteByte(Offset3, 0x7f) == 0) {
                DBGPRINT("++ Decal limit #3 patched @ %p [19 -> %02X]", Offset3, *((BYTE*)Offset3));
            }
            else {
                DBGPRINT("++ FAILED to patch decal limit #3 @ %p [%02X]", Offset3, *((BYTE*)Offset3));
            }
        }

        return true;
    }
};
