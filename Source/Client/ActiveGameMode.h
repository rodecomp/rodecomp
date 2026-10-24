#ifndef RAGNAROK_SOURCE_CLIENT_ACTIVEGAMEMODE_H
#define RAGNAROK_SOURCE_CLIENT_ACTIVEGAMEMODE_H

class CGameMode;

struct CWorld {
  unsigned char unknown_0000[4];
  CGameMode* gameMode;
  unsigned char unknown_0008[0x50];
  int state;

  CGameMode* ActiveGameMode();
};

extern CWorld g_world;

#endif
