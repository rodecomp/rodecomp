#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"

#include <shellapi.h>

// OFFSET: 0x00AD7500
// FUNCTION: fn_17849
int CMode::SendMsg(int event, int argument1, int, int, int) {
  switch (event) {
  case MM_QUIT:
    g_accountLinkedUserDataMgr->Save();
    if (g_modeMgr.state == 1 && g_modeMgr.gameMode) {
      g_characterLinkedUserDataMgr->Save();
      if (g_modeMgr.state == 1 && g_modeMgr.gameMode) {
        g_adventurerAgencyMgr->RequestStop();
      }
    }

    if (g_modeMgr.gameMode) {
      g_modeMgr.gameMode->running = 0;
    }

    if (g_serviceType == ServiceIndia) {
      ShellExecuteA(hWnd, "open", "http://www.ragnarok.co.in/index.php", nullptr, nullptr, SW_SHOWNORMAL);
    }
    g_modeMgr.running = 0;
    break;
  case MM_SCREENSHOT:
    this->screenshotRequested = 1;
    if (const char* text = reinterpret_cast<const char*>(argument1)) {
      this->screenshotName = text;
    }
    break;
  }
  return 0;
}
