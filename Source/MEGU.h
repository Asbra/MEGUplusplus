#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable: 4477) // %p requires void*

#include <Windows.h>

#include <stdint.h>
#include <stdio.h>

// #include <MinHook.h>

// Lua
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

// OutputDebugString wrapper
#ifdef _DEBUG
#define DBGPRINT(...) { char strout[512] = {0}; sprintf(strout, __VA_ARGS__);  OutputDebugStringA(strout); }
#else
#define DBGPRINT(...) ((void*)0)
#endif

#define _PTR_MAX_VALUE ((void*)0xFFE00000)
inline bool IsValidPtr(void* p) { return p != nullptr && (p >= (void*)0x10000) && (p < _PTR_MAX_VALUE); }

#include "Memory\Utils.h"

extern bool quit;

#include "Modules\GettingUp.h"

#include "ModOverrider\ModOverrides.h"
