#ifndef RAGNAROK_SOURCE_CLIENT_MODE_ACTIVEGAMEMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_ACTIVEGAMEMODE_H

class CGameMode;

struct CWorld {
  unsigned char unknown_0000[4];
  CGameMode* gameMode;
  unsigned char unknown_0008[0x50];
  int state;

  CGameMode* ActiveGameMode();
};

// STATIC: 0x013495b8
// GLOBAL: gl_24475
extern CWorld g_world;

#endif
