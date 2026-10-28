#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Contents/Configuration/ServiceConfiguration.h"

#include <cstring>
#include <shellapi.h>

// OFFSET: 0x00AD67E0
// FUNCTION: fn_17845
void CWorld::Quit() {
  if (this->gameMode) {
    this->gameMode->running = 0;
  }

  if (g_serviceType == 11) {
    ShellExecuteA(hWnd, "open", "http://www.ragnarok.co.in/index.php", nullptr, nullptr, SW_SHOWNORMAL);
  }
  this->running = 0;
}

// OFFSET: 0x00AD7620
// FUNCTION: fn_17851
void CWorld::Switch(int mode, const char* name) {
  this->gameMode->running = 0;
  std::strcpy(this->nextModeName, name);
  this->nextState = mode;
}
