#include "Client/Mode/ActiveGameMode.h"

// STATIC: 0x013495b8
// GLOBAL: gl_24475
CWorld g_world = {1};

// FUNCTION: fn_17831
CGameMode* CWorld::ActiveGameMode() {
  return this->state == 1 ? this->gameMode : nullptr;
}
