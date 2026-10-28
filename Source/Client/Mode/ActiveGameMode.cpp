#include "Client/Mode/ActiveGameMode.h"

// STATIC: 0x013495b8
// GLOBAL: gl_24475
CWorld g_world = {1};

// OFFSET: 0x00AD6450
// FUNCTION: fn_17831
CMode* CWorld::ActiveGameMode() {
  return this->state == 1 ? this->gameMode : nullptr;
}

// OFFSET: 0x00AD66D0
// FUNCTION: fn_17835
void CMode::Slot7(int) {
}

// OFFSET: 0x00AD66E0
// FUNCTION: fn_17836
void CMode::End() {
}

// OFFSET: 0x00AD66F0
// FUNCTION: fn_17837
void CMode::Initialize(const char*) {
}

// OFFSET: 0x00AD6700
// FUNCTION: fn_17838
void CMode::Slot5(int) {
}
