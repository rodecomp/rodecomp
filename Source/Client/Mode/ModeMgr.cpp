#include "Client/Mode/ModeMgr.h"

// STATIC: 0x013495b8
// GLOBAL: gl_24475
CModeMgr g_modeMgr = {1};

// OFFSET: 0x00AD6450
// FUNCTION: fn_17831
CMode* CModeMgr::GetGameMode() {
  return this->state == 1 ? this->gameMode : nullptr;
}

// OFFSET: 0x00AD6460
// FUNCTION: fn_17832
CMode* CModeMgr::GetLoginMode() {
  return this->state == 0 ? this->gameMode : nullptr;
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

// OFFSET: 0x00AD6710
// FUNCTION: fn_17839
int CMode::Run() {
  return 0;
}
