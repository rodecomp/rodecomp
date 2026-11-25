// Stand-in for the nProtect GameGuard SDK, linked only into the standalone exe
// built by scripts/exe/build_exe.py. Clank never compiles this file.
//
// The original statically links the SDK (0x00E8A000-0x00E96000). The client
// reaches it through four entry points, called by CNProtectGameGuardMgr in
// Source/Security/NProtectGameGuardMgr.cpp. This file provides those four with
// the behaviour of a successful start-up, so the client follows the same path it
// takes when GameGuard initialises normally. No anti-cheat checks are performed
// and server GameGuard authentication requests are not answered.

namespace {
  // NPGAMEMON_SUCCESS: CNProtectGameGuardMgr treats any other status as a
  // start-up failure.
  constexpr int kGameGuardSuccess = 0x755;

  int(__stdcall* g_callback)(int, const int*) = nullptr;
}

// fn_25538 (0x00E929B0): SDK initialisation.
void PreInitNPGameMonA(const char*) {
}

// fn_25536 (0x00E92820): status of the last initialisation.
int InitNPGameMon() {
  return kGameGuardSuccess;
}

// fn_25543 (0x00E92E90): registers the client's GameGuard message handler.
void SetCallbackToGameMon(int(__stdcall* callback)(int, const int*)) {
  g_callback = callback;
}

// fn_25533 (0x00E92420): SDK shutdown; nothing was started.
void CloseNPGameMon() {
}

// OFFSET: 0x00E92D90
// STUB: fn_25541
int Send_CSAuth3ToGameMon(const unsigned char*, unsigned long, unsigned long) {
  return 0;
}

// OFFSET: 0x00E92DE0
// STUB: fn_25542
int SendUserIDToGameMon(const char*) {
  return 1;
}

// OFFSET: 0x00E92F30
// STUB: fn_25544
void SetHwndToGameMon(unsigned long) {
}
