#ifndef RAGNAROK_SOURCE_CLIENT_MODE_CGAMEMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_CGAMEMODE_H

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Contents/ActorNameCard.h"

#include <cstddef>
#include <string>

class CView;
struct ActorNameAllPacket;
struct WorldScene;

class CGameMode : public CMode {
public:
  /* 0058 */ unsigned char unknown_0058[0x10];
  /* 0068 */ char mapName[16];
  /* 0078 */ unsigned char unknown_0078[0x54];
  /* 00CC */ WorldScene* scene;
  /* 00D0 */ CView* camera;
  /* 00D4 */ unsigned char unknown_00D4[0x8C];
  /* 0160 */ CActorNameCardHolder nameCard;
  /* 0194 */ unsigned char unknown_0194[0x120];
  /* 02B4 */ UIWindow* itemWindow;
  /* 02B8 */ unsigned char unknown_02B8[0x50];
  /* 0308 */ ModeDragState drag;
  /* 03C8 */ unsigned char unknown_03C8[0x2A8];

  std::string MapName();
  void WorldCell(float x, float z, int& column, int& row) const;
  void PlayerCell(int* column, int* row) const;
  void ReceiveActorNameAll(const ActorNameAllPacket* packet);
  void NotifyItemBounds();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGameMode, mapName) == 0x68);
static_assert(offsetof(CGameMode, scene) == 0xCC);
static_assert(offsetof(CGameMode, camera) == 0xD0);
static_assert(offsetof(CGameMode, nameCard) == 0x160);
static_assert(offsetof(CGameMode, drag) == 0x308);
static_assert(sizeof(CGameMode) == 0x670);
#endif

#endif
