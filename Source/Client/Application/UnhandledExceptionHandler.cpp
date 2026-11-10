#include "Client/Application/LaunchInfo.h"
#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/UnhandledExceptionHandler.h"
#include "Client/Session.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <dbghelp.h>
#include <psapi.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// STATIC: 0x01389c78
// GLOBAL: gl_24469
CUnhandledExceptionHandler* g_unhandledExceptionHandler = nullptr;

// STATIC: 0x01389c7c
// GLOBAL: gl_24470
int (*g_unhandledExceptionCallback)() = nullptr;

// STATIC: 0x01389c80
// GLOBAL: gl_24359
char g_filename[264] = {};

// STATIC: 0x01389d88
// GLOBAL: gl_24471
char g_moduleBaseName[260] = {};

// STATIC: 0x01389e90
// GLOBAL: gl_24360
char g_crashString[8096] = {};

extern "C" long long __cdecl _Xtime_get_ticks();

// OFFSET: 0x00635A70
// FUNCTION: fn_04701
CUnhandledExceptionHandler::CUnhandledExceptionHandler() {
}

CUnhandledExceptionHandler::~CUnhandledExceptionHandler() = default;

// INLINED
CUnhandledExceptionHandler::CUnhandledExceptionHandler(int (*callback)()) {
  g_unhandledExceptionCallback = callback;
  GetModuleFileNameA(nullptr, g_filename, MAX_PATH);

  StringFormat(g_moduleBaseName, sizeof(g_moduleBaseName), static_cast<size_t>(-1), "%s", strrchr(g_filename, '\\') ? strrchr(g_filename, '\\') + 1 : g_filename);

  if (strrchr(g_filename, '\\')) {
    *strrchr(g_filename, '\\') = '\0';
  }

  if (strrchr(g_moduleBaseName, '.')) {
    *strrchr(g_moduleBaseName, '.') = '\0';
  }

  SetUnhandledExceptionFilter(TopLevelExceptionFilter);
  _set_invalid_parameter_handler(InvalidParameterHandler);
  _set_purecall_handler(PurecallHandler);
}

// OFFSET: 0x00635AB0
// FUNCTION: fn_04703
bool InitUnhandledExceptionHandler(int (*callback)()) {
  if (g_unhandledExceptionHandler == nullptr) {
    g_unhandledExceptionHandler = new CUnhandledExceptionHandler(callback);
    if (g_unhandledExceptionHandler != nullptr) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00635C10
// FUNCTION: fn_04704
void CleanupUnhandledExceptionHandler() {
  if (g_unhandledExceptionHandler != nullptr) {
    delete g_unhandledExceptionHandler;
    g_unhandledExceptionHandler = nullptr;
  }
}

// OFFSET: 0x00635C30
// FUNCTION: fn_04705
INT_PTR CALLBACK DialogFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_INITDIALOG: {
    HWND item = GetDlgItem(hDlg, 1006);
    RECT rect = {};
    SetWindowTextA(item, g_crashString);
    GetWindowRect(hDlg, &rect);

    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - (rect.bottom - rect.top) / 2;
    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - (rect.right - rect.left) / 2;

    MoveWindow(hDlg, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    break;
  }
  case WM_COMMAND:
    if (LOWORD(wParam) == 2) {
      HGLOBAL memory = GlobalAlloc(GMEM_DDESHARE, strlen(g_crashString) + 1);
      if (memory != nullptr) {
        char* locked = static_cast<char*>(GlobalLock(memory));
        if (locked == nullptr) {
          GlobalFree(memory);
        } else {
          strcpy(locked, g_crashString);
          GlobalUnWire(memory);

          if (OpenClipboard(hDlg)) {
            EmptyClipboard();
            SetClipboardData(CF_TEXT, memory);
            CloseClipboard();
          }

          HWND button = GetDlgItem(hDlg, 7);
          SetFocus(button);
        }
      }
    } else if (LOWORD(wParam) == 7) {
      EndDialog(hDlg, 7);
    }
    break;
  default:
    return FALSE;
  }

  return TRUE;
}

// OFFSET: 0x00635DB0
// FUNCTION: fn_04706
BOOL CALLBACK EnumerateLoadedModulesCallback(PCSTR moduleName, ULONG moduleBase, ULONG moduleSize, PVOID userContext) {
  auto* address = static_cast<const DWORD*>(userContext);
  if (*address < moduleBase || *address > moduleBase + moduleSize) {
    return TRUE;
  }

  sprintf(&g_crashString[strlen(g_crashString)], "%s", moduleName);
  return FALSE;
}

// OFFSET: 0x00635E00
// FUNCTION: fn_04707
void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved) {
  EXCEPTION_RECORD record = {};
  CONTEXT context = {};
  EXCEPTION_POINTERS exceptionInfo = {&record, &context};

  RtlCaptureContext(&context);

  record.ExceptionCode = 0xC000000D;
  record.NumberParameters = 3;
  record.ExceptionInformation[0] = reinterpret_cast<ULONG_PTR>(expression);
  record.ExceptionInformation[1] = reinterpret_cast<ULONG_PTR>(file);
  record.ExceptionInformation[2] = static_cast<ULONG_PTR>(line);

  TopLevelExceptionFilter(&exceptionInfo);
  abort();
}

// OFFSET: 0x00635E90
// FUNCTION: fn_04708
void __cdecl PurecallHandler() {
  EXCEPTION_RECORD record = {};
  CONTEXT context = {};
  EXCEPTION_POINTERS exceptionInfo = {&record, &context};

  RtlCaptureContext(&context);

  record.ExceptionCode = 0xC0000025;

  TopLevelExceptionFilter(&exceptionInfo);
  abort();
}

// OFFSET: 0x00635F00
// FUNCTION: fn_04709
LONG WINAPI TopLevelExceptionFilter(EXCEPTION_POINTERS* exceptionInfo) {
  PCONTEXT context = exceptionInfo->ContextRecord;
  HMODULE dbgHelp = LoadLibraryA("dbghelp.dll");

  if (dbgHelp != nullptr) {
    auto pStackWalk = reinterpret_cast<BOOL(WINAPI*)(
      DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, PREAD_PROCESS_MEMORY_ROUTINE, PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE,
      PTRANSLATE_ADDRESS_ROUTINE
    )>(GetProcAddress(dbgHelp, "StackWalk"));

    auto pGetTimestamp = reinterpret_cast<DWORD(WINAPI*)(HMODULE)>(GetProcAddress(dbgHelp, "GetTimestampForLoadedLibrary"));

    auto pEnumerateLoadedModules =
      reinterpret_cast<BOOL(WINAPI*)(HANDLE, PENUMLOADED_MODULES_CALLBACK, PVOID)>(GetProcAddress(dbgHelp, "EnumerateLoadedModules"));

    HMODULE module = GetModuleHandleA(nullptr);
    CHAR filename[256];
    GetModuleFileNameA(module, filename, sizeof(filename));

    __time32_t timestamp = pGetTimestamp(module);
    sprintf(&g_crashString[strlen(g_crashString)], "Module Name: %s\r\n", filename);
    sprintf(&g_crashString[strlen(g_crashString)], "Time Stamp: 0x%08x - %s\r\n", timestamp, _ctime32(&timestamp));
    sprintf(&g_crashString[strlen(g_crashString)], "Version: 0x%08x\r\n", g_clientVersion);
    sprintf(&g_crashString[strlen(g_crashString)], "\r\n");
    sprintf(&g_crashString[strlen(g_crashString)], "Exception Type: 0x%08x\r\n", static_cast<unsigned int>(exceptionInfo->ExceptionRecord->ExceptionCode));
    sprintf(&g_crashString[strlen(g_crashString)], "\r\n");

    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    STACKFRAME frame = {};
    frame.AddrPC.Offset = context->Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context->Esp;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;

    sprintf(&g_crashString[strlen(g_crashString)], "\r\n");
    sprintf(
      &g_crashString[strlen(g_crashString)], "eax: 0x%08x\tebx: 0x%08x\r\n", static_cast<unsigned int>(context->Eax), static_cast<unsigned int>(context->Ebx)
    );
    sprintf(
      &g_crashString[strlen(g_crashString)], "ecx: 0x%08x\tedx: 0x%08x\r\n", static_cast<unsigned int>(context->Ecx), static_cast<unsigned int>(context->Edx)
    );
    sprintf(
      &g_crashString[strlen(g_crashString)], "esi: 0x%08x\tedi: 0x%08x\r\n", static_cast<unsigned int>(context->Esi), static_cast<unsigned int>(context->Edi)
    );
    sprintf(
      &g_crashString[strlen(g_crashString)], "ebp: 0x%08x\tesp: 0x%08x\r\n", static_cast<unsigned int>(context->Ebp), static_cast<unsigned int>(context->Esp)
    );
    sprintf(&g_crashString[strlen(g_crashString)], "\r\n");

    const auto* stack = reinterpret_cast<const unsigned char*>(context->Esp);
    for (int i = 0; i < 512; ++i) {
      if (frame.AddrPC.Offset == 0) {
        break;
      }

      if (!pStackWalk(IMAGE_FILE_MACHINE_I386, process, thread, &frame, context, nullptr, nullptr, nullptr, nullptr)) {
        break;
      }

      sprintf(&g_crashString[strlen(g_crashString)], "0x%08x\t", static_cast<unsigned int>(frame.AddrPC.Offset));
      pEnumerateLoadedModules(GetCurrentProcess(), EnumerateLoadedModulesCallback, &frame.AddrPC.Offset);
      sprintf(&g_crashString[strlen(g_crashString)], "\r\n");
    }

    sprintf(&g_crashString[strlen(g_crashString)], "\r\n");
    sprintf(
      &g_crashString[strlen(g_crashString)], "stack %08x - %08x\r\n", static_cast<unsigned int>(reinterpret_cast<uintptr_t>(stack)),
      static_cast<unsigned int>(reinterpret_cast<uintptr_t>(stack + 1024))
    );

    for (int i = 0; i < 16; ++i) {
      sprintf(&g_crashString[strlen(g_crashString)], "%08X : ", static_cast<unsigned int>(reinterpret_cast<uintptr_t>(stack)));

      for (int j = 0; j < 16; ++j) {
        sprintf(&g_crashString[strlen(g_crashString)], "%02X ", *stack);
        ++stack;
      }

      sprintf(&g_crashString[strlen(g_crashString)], "\r\n");
    }

    sprintf(&g_crashString[strlen(g_crashString)], "\r\nLaunch Info \r\n");

    int launchIndex = 0;

    for (int k = 0; k < 4; ++k) {
      for (int m = 0; m < 8; ++m) {
        sprintf(&g_crashString[strlen(g_crashString)], "%04X ", g_launchInfo[launchIndex]);
        ++launchIndex;
      }

      sprintf(&g_crashString[strlen(g_crashString)], "\r\n");
    }

    sprintf(&g_crashString[strlen(g_crashString)], "\r\nJob : %s\r\n", g_session.GetJobName(g_session.GetDisplayJob(), -1));

    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    gameMode != nullptr && sprintf(&g_crashString[strlen(g_crashString)], "%s %d %d\r\n", std::string(gameMode->mapName).c_str(), g_session.m_playerX, g_session.m_playerY);

    sprintf(&g_crashString[strlen(g_crashString)], "\r\n%I64d - %I64d", g_session.m_startTimeTicks / 10000000LL, g_session.m_loginTimeTicks / 10000000LL);

    int64_t reportTime = _Xtime_get_ticks() / 10000000LL;
    PROCESS_MEMORY_COUNTERS_EX memCounters;
    double workingSet = 0.0;
    double privateUsage = 0.0;

    if (K32GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memCounters), sizeof(memCounters))) {
      workingSet = static_cast<double>(static_cast<uint32_t>(memCounters.WorkingSetSize >> 10));
      privateUsage = static_cast<double>(static_cast<uint32_t>(memCounters.PrivateUsage >> 10));
    }

    sprintf(&g_crashString[strlen(g_crashString)], "\r\nReport Time: %I64d(%.0lf/%.0lf)\r\n", reportTime, workingSet, privateUsage);
    FreeLibrary(dbgHelp);
  } else {
    sprintf(&g_crashString[strlen(g_crashString)], "Can't find DbgHelp.DLL !!!");
  }

  DialogBoxParamA(GetModuleHandleA(nullptr), MAKEINTRESOURCEA(117), nullptr, DialogFunc, 0);

  if (g_unhandledExceptionCallback != nullptr) {
    g_unhandledExceptionCallback();
  }

  return EXCEPTION_EXECUTE_HANDLER;
}
