#include "Client/ModeBaseEvents.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"

#include <shellapi.h>

// OFFSET: 0x00AD7500
// FUNCTION: fn_17849
int ModeBaseEvents::Send(int event, const char* text, int, int, int) {
  if (event == 2) {
    g_accountLinkedUserDataMgr->Save(this->emotions, this->script, this->persistenceAccountId, this->renderSystem, this->token16, this->work);
    if (g_modeMgr.state != 1) {
      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->running = 0;
      }
    } else if (g_modeMgr.gameMode) {
      g_characterLinkedUserDataMgr->Save(this->characterSave, this->token16, this->work);
      if (g_modeMgr.state != 1) {
        if (g_modeMgr.gameMode) {
          g_modeMgr.gameMode->running = 0;
        }
      } else if (g_modeMgr.gameMode) {
        g_adventurerAgencyMgr->RequestStop(this->agency, this->stopRecruiting);
        if (g_modeMgr.gameMode) {
          g_modeMgr.gameMode->running = 0;
        }
      }
    }

    if (g_serviceType == 11) {
      ShellExecuteA(this->window, "open", "http://www.ragnarok.co.in/index.php", nullptr, nullptr, SW_SHOWNORMAL);
    }
    g_modeMgr.running = 0;
  } else if (event == 68) {
    this->transitionRequested = 1;
    if (text) {
      this->transitionName = text;
    }
  }
  return 0;
}
