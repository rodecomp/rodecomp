#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/GuildMgr.h"
#include "File/File.h"

#include <cstdio>
#include <algorithm>
#include <cstring>

// GLOBAL: gl_24759
std::vector<int> g_monsterJobs;

// OFFSET: 0x00AD1C10
// FUNCTION: fn_17813
__declspec(noinline) int MonsterJobColor(int) {
  return 0;
}

// OFFSET: 0x00AD1C20

// OFFSET: 0x00DDC6F0

// OFFSET: 0x00E3D290

// OFFSET: 0x00CD3400

// OFFSET: 0x00CD3470

// OFFSET: 0x00CD34D0

// OFFSET: 0x00CD3570

// OFFSET: 0x00CD3580
// FUNCTION: fn_21267
const char* CSession::Text8104() const {
  return this->unknown_8104.c_str();
}

// OFFSET: 0x00CD35A0
// FUNCTION: fn_21268
const char* CSession::Text80EC() const {
  return this->unknown_80EC.c_str();
}

// OFFSET: 0x00CD3A30

// OFFSET: 0x00CD3B10
