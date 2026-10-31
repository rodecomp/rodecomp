#ifndef RAGNAROK_SOURCE_CONTENTS_SOUND_SOUNDMAKER_H
#define RAGNAROK_SOURCE_CONTENTS_SOUND_SOUNDMAKER_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>

class CSoundMaker : public CRenderObject {
public:
  CSoundMaker();
  ~CSoundMaker() override;
  bool OnProcess() override;
  void SendMsg(
    CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m
  ) override;
  void Render(int argument) override;

  /* 0138 */ std::string file;
  /* 0150 */ float volume;
  /* 0154 */ int width;
  /* 0158 */ int height;
  /* 015C */ float range;
  /* 0160 */ float radius;
  /* 0164 */ std::uint32_t lastTick;
  /* 0168 */ std::uint32_t cycle;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSoundMaker, file) == 0x138);
static_assert(offsetof(CSoundMaker, volume) == 0x150);
static_assert(offsetof(CSoundMaker, width) == 0x154);
static_assert(offsetof(CSoundMaker, height) == 0x158);
static_assert(offsetof(CSoundMaker, range) == 0x15C);
static_assert(offsetof(CSoundMaker, radius) == 0x160);
static_assert(offsetof(CSoundMaker, lastTick) == 0x164);
static_assert(offsetof(CSoundMaker, cycle) == 0x168);
static_assert(sizeof(CSoundMaker) == 0x170);
#endif

extern std::uint32_t g_groundMarkAltEnabled;

#endif
