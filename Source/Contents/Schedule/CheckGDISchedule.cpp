#include "Contents/Schedule/CheckGDISchedule.h"

#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Contents/ShowRankingScheduler.h"
#include "Network/Connection/Connection.h"
#include "Network/Connection/ConnectionRuntime.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"

#include <cstdlib>
#include <windows.h>

// GLOBAL: gl_24852
char* g_timeScheduler;

// OFFSET: 0x00BA7FE0
// FUNCTION: fn_19716
CCheckGDISchedule::CCheckGDISchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 4) {
}

// OFFSET: 0x00BA81B0
// FUNCTION: fn_19718
void CCheckGDISchedule::Process(const std::uint64_t& now) {
  this->CSchedule::Process(now);
  if (CheckConnectionRuntime()) {
    HMODULE gdi32 = LoadLibraryA("gdi32.dll");
    if (gdi32 != nullptr) {
      FARPROC proc = GetProcAddress(gdi32, "TextOutW");
      if (proc != reinterpret_cast<FARPROC>(TextOutW)) {
        FreeLibrary(gdi32);
        goto perform_check;
      }
      FreeLibrary(gdi32);
    }
    if (!IsDebuggerPresent()) {
      return;
    }
  }

perform_check:
  if (g_serviceType != 0) {
    return;
  }

  const int roll = static_cast<int>((static_cast<double>(std::rand()) / 32767.0) * 5.0 + 1.0);
  if (roll != 2 && roll != 4 && roll != 5) {
    return;
  }

  g_timeScheduler[13] = 1;

  if (roll == 2) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(857), 0, true, false, 0, "Message", 280, 120, 0);
  } else if (roll == 4) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(800), 0, true, false, 0, "Message", 280, 120, 0);
  }

  GetRagConnection().Close();
  g_modeMgr.Quit();
}
