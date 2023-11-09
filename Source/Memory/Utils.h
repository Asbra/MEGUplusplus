#ifndef MemoryUtils_h
#define MemoryUtils_h
#pragma once

#include <Windows.h>

BYTE WriteByte(LPVOID lpAddress, BYTE bValue);

BOOL GetModuleHandleP(LPCSTR lpModuleName, HMODULE* hModule);

#endif // MemoryUtils_h
