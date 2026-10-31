#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODEMGR_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODEMGR_H

#include "Client/Mode/CMode.h"

struct CModeMgr {
  /* 0000 */ int running;
  /* 0004 */ CMode* gameMode;
  /* 0008 */ char modeName[0x28];
  /* 0030 */ char nextModeName[0x28];
  /* 0058 */ int state;
  /* 005C */ int nextState;

  CMode* GetGameMode();
  CMode* GetLoginMode();
  void Quit();
  void Run(int mode, const char* name);
  void Switch(int mode, const char* name);
};

extern CModeMgr g_modeMgr;

#endif
