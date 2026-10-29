#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_ITEM_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_ITEM_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CItem : public CRenderObject {
public:
  bool OnProcess() override;
  void Render(int argument) override;

  /* 0138 */ unsigned char unknown_0138[0x20];
  /* 0158 */ std::string name;
  /* 0170 */ unsigned char unknown_0170[0x10];
  /* 0180 */ std::uint32_t isFalling;
  /* 0184 */ float fallVelocity;
  /* 0188 */ float fallOrigin;
  /* 018C */ std::uint32_t unknown_018C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CItem, name) == 0x158);
static_assert(offsetof(CItem, isFalling) == 0x180);
static_assert(sizeof(CItem) == 0x190);
#endif

#endif
