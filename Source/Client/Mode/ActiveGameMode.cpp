#include "Client/Mode/ActiveGameMode.h"

// FUNCTION: fn_17831
CGameMode* CWorld::ActiveGameMode() {
  return this->state == 1 ? this->gameMode : nullptr;
}
