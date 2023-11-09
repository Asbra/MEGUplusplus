#ifndef ModOverrides_h
#define ModOverrides_h

#include <Windows.h>
#include <stdio.h>

FILE* HookFOpen(const char* filename, const char* mode);

bool ModOverrides__Initialize();

#endif // ModOverrides_h
