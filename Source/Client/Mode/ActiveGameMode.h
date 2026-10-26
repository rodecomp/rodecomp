#ifndef RAGNAROK_SOURCE_CLIENT_MODE_ACTIVEGAMEMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_ACTIVEGAMEMODE_H

class CGameMode;

struct CWorld {
  /* 0000 */ int running;
  /* 0004 */ CGameMode* gameMode;
  /* 0008 */ unsigned char unknown_0008[0x50];
  /* 0058 */ int state;

  CGameMode* ActiveGameMode();
};

extern CWorld g_world;

#endif
