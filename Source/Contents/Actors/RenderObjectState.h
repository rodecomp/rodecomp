#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECTSTATE_H

#include "Contents/InstantVar.h"

#include <cstddef>
#include <cstdint>
#include <string>

struct JobNames;
class ResourceMgr;
class CSprRes;
class CActRes;

struct RenderObjectState {
  RenderObjectState(ResourceMgr& resources, JobNames& names);
  ~RenderObjectState();
  void Reset(ResourceMgr& resources, JobNames& names);
  RenderObjectState(const RenderObjectState&) = delete;
  RenderObjectState& operator=(const RenderObjectState&) = delete;

  /* 0010 */ float position[3];
  /* 001C */ std::string name;
  /* 0034 */ int animation;
  /* 0038 */ int previousAnimation;
  /* 003C */ int curMotion;
  /* 0040 */ std::uint32_t unknown_0040;
  /* 0044 */ std::uint32_t unknown_0044;
  /* 0048 */ std::uint32_t unknown_0048;
  /* 004C */ float angle;
  /* 0050 */ float scale;
  /* 0054 */ float unknown_0054;
  /* 0058 */ float speed;
  /* 005C */ float unknown_005C;
  /* 0060 */ float unknown_0060;
  /* 0064 */ float unknown_0064;
  /* 0068 */ std::uint32_t unknown_0068;
  /* 006C */ std::uint32_t unknown_006C;
  /* 0070 */ std::uint32_t unknown_0070;
  /* 0074 */ std::uint32_t unknown_0074;
  /* 0078 */ std::uint32_t unknown_0078;
  /* 007C */ std::uint32_t unknown_007C;
  /* 0080 */ std::uint32_t unknown_0080;
  /* 0084 */ std::uint32_t overrideColor;
  /* 0088 */ std::uint32_t started;
  /* 008C */ std::uint32_t unknown_008C;
  /* 0090 */ std::uint32_t unknown_0090;
  /* 0094 */ std::uint32_t unknown_0094;
  /* 0098 */ std::uint32_t color;
  /* 009C */ bool isLieOnGround;
  /* 009D */ bool isMotionFinished;
  /* 009E */ bool isMotionFreezed;
  /* 009F */ bool isSingleColor;
  /* 00A0 */ unsigned char visible;
  /* 00A1 */ unsigned char enabled;
  /* 00A2 */ unsigned char unknown_00A2;
  /* 00A3 */ unsigned char unknown_00A3;
  union {
    /* 00A4 */ std::uint16_t unknown_00A4;
    /* 00A4 */ unsigned char unknown_00A4Bytes[2];
  };
  /* 00A6 */ unsigned char unknown_00A6[2];
  /* 00A8 */ std::uint32_t unknown_00A8;
  /* 00AC */ int screenX;
  /* 00B0 */ int screenY;
  /* 00B4 */ unsigned char unknown_00B4;
  /* 00B5 */ unsigned char sequenceAnimation;
  /* 00B6 */ unsigned char sequenceFrame;
  /* 00B7 */ unsigned char sequenceAnimations[5];
  /* 00BC */ unsigned char sequenceFrames[5];
  /* 00C1 */ unsigned char sequenceLast;
  /* 00C2 */ unsigned char unknown_00C2;
  /* 00C3 */ unsigned char sequenceFinishAnimation;
  /* 00C4 */ unsigned char sequenceFinishFrame;
  /* 00C5 */ unsigned char sequenceIndex;
  /* 00C6 */ unsigned char unknown_00C6[2];
  /* 00C8 */ std::uint32_t unknown_00C8;
  /* 00CC */ int sequenceLoop;
  /* 00D0 */ int sequenceTimed;
  /* 00D4 */ int sequenceFinished;
  /* 00D8 */ std::uint32_t unknown_00D8;
  /* 00DC */ std::uint32_t unknown_00DC;
  /* 00E0 */ std::uint32_t sequenceTick;
  /* 00E4 */ std::uint32_t sequenceDeadline;
  /* 00E8 */ std::uint32_t flags;
  /* 00EC */ std::uint32_t unknown_00EC;
  union {
    /* 00F0 */ std::uint16_t unknown_00F0;
    /* 00F0 */ unsigned char pulsePhases[2];
  };
  /* 00F2 */ unsigned char unknown_00F2;
  /* 00F3 */ unsigned char unknown_00F3;
  /* 00F4 */ short unknown_00F4;
  /* 00F6 */ unsigned char unknown_00F6;
  /* 00F7 */ unsigned char unknown_00F7;
  /* 00F8 */ short unknown_00F8;
  /* 00FA */ unsigned char unknown_00FA;
  /* 00FB */ unsigned char unknown_00FB;
  /* 00FC */ std::uint32_t unknown_00FC;
  /* 0100 */ unsigned char unknown_0100;
  /* 0101 */ unsigned char unknown_0101[3];
  /* 0104 */ CSprRes* sprite;
  /* 0108 */ CActRes* action;
  /* 010C */ std::uint32_t unknown_010C;
  /* 0110 */ std::uint32_t accountId;
  /* 0114 */ unsigned char unknown_0114;
  /* 0115 */ unsigned char unknown_0115[3];
  /* 0118 */ CInstantVar instant;
  /* 0124 */ std::uint32_t unknown_0124;
  /* 0128 */ std::uint32_t unknown_0128;
  /* 012C */ std::uint32_t unknown_012C;
  /* 0130 */ unsigned char unknown_0130[8];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderObjectState) == 296);
static_assert(offsetof(RenderObjectState, sequenceAnimation) == 0xA5);
static_assert(offsetof(RenderObjectState, sequenceAnimations) == 0xA7);
static_assert(offsetof(RenderObjectState, sequenceFrames) == 0xAC);
static_assert(offsetof(RenderObjectState, sequenceFinishAnimation) == 0xB3);
static_assert(offsetof(RenderObjectState, sequenceLoop) == 0xBC);
static_assert(offsetof(RenderObjectState, sequenceDeadline) == 0xD4);
#endif

#endif
