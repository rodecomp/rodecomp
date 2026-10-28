#ifndef RAGNAROK_SOURCE_CLIENT_MODE_ACTIVEGAMEMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_ACTIVEGAMEMODE_H

#include "Client/Mode/CMode.h"

struct CWorld {
  /* 0000 */ int running;
  /* 0004 */ CMode* gameMode;
  /* 0008 */ unsigned char unknown_0008[0x28];
  /* 0030 */ char nextModeName[0x28];
  /* 0058 */ int state;
  /* 005C */ int nextState;

  CMode* ActiveGameMode();
  void Quit();
  void Switch(int mode, const char* name);
};

extern CWorld g_world;

#endif
