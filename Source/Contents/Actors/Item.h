#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_ITEM_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_ITEM_H

#include "Contents/Effects/EffectActor.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CItem : public EffectActor {
public:
  CItem();
  void SubD8B5A0();
  void SubD8B5B0();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  void Render(int argument) override;

  /* 0158 */ std::string name;
  /* 0170 */ std::uint32_t itemId;
  /* 0174 */ unsigned char identified;
  /* 0178 */ int itemIndex;
  /* 017C */ std::uint32_t itemAid;
  /* 0180 */ std::uint32_t isFalling;
  /* 0184 */ float fallVelocity;
  /* 0188 */ float fallOrigin;
  /* 018C */ std::uint16_t unknown_018C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CItem, name) == 0x158);
static_assert(offsetof(CItem, itemAid) == 0x17C);
static_assert(offsetof(CItem, isFalling) == 0x180);
static_assert(sizeof(CItem) == 0x190);
#endif

#endif
