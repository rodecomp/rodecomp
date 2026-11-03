#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"

#include <shellapi.h>

// OFFSET: 0x00AD7500
// FUNCTION: fn_17849
int CMode::SendMsg(int event, int argument1, int, int, int) {
  if (event == 2) {
    g_accountLinkedUserDataMgr->Save();
    if (g_modeMgr.state != 1) {
      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->running = 0;
      }
    } else if (g_modeMgr.gameMode) {
      g_characterLinkedUserDataMgr->Save();
      if (g_modeMgr.state != 1) {
        if (g_modeMgr.gameMode) {
          g_modeMgr.gameMode->running = 0;
        }
      } else if (g_modeMgr.gameMode) {
        g_adventurerAgencyMgr->RequestStop();
        if (g_modeMgr.gameMode) {
          g_modeMgr.gameMode->running = 0;
        }
      }
    }

    if (g_serviceType == 11) {
      ShellExecuteA(hWnd, "open", "http://www.ragnarok.co.in/index.php", nullptr, nullptr, SW_SHOWNORMAL);
    }
    g_modeMgr.running = 0;
  } else if (event == 68) {
    this->screenshotRequested = 1;
    if (const char* text = reinterpret_cast<const char*>(argument1)) {
      this->screenshotName = text;
    }
  }
  return 0;
}
