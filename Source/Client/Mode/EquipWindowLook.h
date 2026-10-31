#ifndef RAGNAROK_SOURCE_CLIENT_MODE_EQUIPWINDOWLOOK_H
#define RAGNAROK_SOURCE_CLIENT_MODE_EQUIPWINDOWLOOK_H

#include <cstddef>

struct EquipWindowLook {
  /* 0000 */ int job;
  /* 0004 */ int sex;
  /* 0008 */ int head;
  /* 000C */ int accessory;
  /* 0010 */ int accessory2;
  /* 0014 */ int accessory3;
  /* 0018 */ int bodyPalette;
  /* 001C */ int headPalette;
  /* 0020 */ int robe;
  /* 0024 */ int unknown_0024;
  /* 0028 */ char name[24];
};

extern EquipWindowLook g_equipWindowLook;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipWindowLook) == 0x40);
static_assert(offsetof(EquipWindowLook, name) == 0x28);
#endif

#endif
