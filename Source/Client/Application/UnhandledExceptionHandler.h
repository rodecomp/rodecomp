#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_UNHANDLEDEXCEPTIONHANDLER_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_UNHANDLEDEXCEPTIONHANDLER_H

#include <windows.h>
#include <cstdint>

// STATIC: 0x0110c2b4
// VTABLE: vt_00282
class CUnhandledExceptionHandler {
public:
  CUnhandledExceptionHandler();

  virtual ~CUnhandledExceptionHandler();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUnhandledExceptionHandler) == 4);
#endif

bool InitUnhandledExceptionHandler(int (*callback)());

int CleanupUnhandledExceptionHandler();

INT_PTR CALLBACK DialogFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK EnumerateLoadedModulesCallback(PCSTR moduleName, ULONG moduleBase, ULONG moduleSize, PVOID userContext);

void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);

void __cdecl PurecallHandler();

LONG WINAPI TopLevelExceptionFilter(EXCEPTION_POINTERS* exceptionInfo);

#endif
