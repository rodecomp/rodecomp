#ifndef RAGNAROK_SOURCE_CLIENT_UNHANDLEDEXCEPTIONHANDLER_H
#define RAGNAROK_SOURCE_CLIENT_UNHANDLEDEXCEPTIONHANDLER_H

#include <windows.h>
#include <cstdint>

// STATIC: 0x0110c2b4
// VTABLE: vt_00282
class CUnhandledExceptionHandler {
public:
  // OFFSET: 0x00635A70
  // FUNCTION: fn_04701
  CUnhandledExceptionHandler();

  // OFFSET: 0x00635A80
  // FUNCTION: fn_04702
  virtual ~CUnhandledExceptionHandler();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUnhandledExceptionHandler) == 4);
#endif

// OFFSET: 0x00635AB0
// FUNCTION: fn_04703
bool InitUnhandledExceptionHandler(int (*callback)());

// OFFSET: 0x00635C10
// FUNCTION: fn_04704
int CleanupUnhandledExceptionHandler();

// OFFSET: 0x00635C30
// FUNCTION: fn_04705
INT_PTR CALLBACK DialogFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// OFFSET: 0x00635DB0
// FUNCTION: fn_04706
BOOL CALLBACK EnumerateLoadedModulesCallback(PCSTR moduleName, ULONG moduleBase, ULONG moduleSize, PVOID userContext);

// OFFSET: 0x00635E00
// FUNCTION: fn_04707
void __cdecl InvalidParameterHandler(
  const wchar_t* expression,
  const wchar_t* function,
  const wchar_t* file,
  unsigned int line,
  uintptr_t pReserved
);

// OFFSET: 0x00635E90
// FUNCTION: fn_04708
void __cdecl PurecallHandler();

// OFFSET: 0x00635F00
// FUNCTION: fn_04709
LONG WINAPI TopLevelExceptionFilter(EXCEPTION_POINTERS* exceptionInfo);

#endif // RAGNAROK_SOURCE_CLIENT_UNHANDLEDEXCEPTIONHANDLER_H
