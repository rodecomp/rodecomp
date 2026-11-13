#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_RENDEROBJECTSTATE_H

#include "Contents/InstantVar.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <string>

class ResourceMgr;
class CSprRes;
class CActRes;

struct RenderObjectState {
  RenderObjectState();
  ~RenderObjectState();
  void Reset();
  RenderObjectState(const RenderObjectState&) = delete;
  RenderObjectState& operator=(const RenderObjectState&) = delete;

  /* 0010 */ vector3d position;
  /* 001C */ std::string name;
  /* 0034 */ int animation;
  /* 0038 */ int previousAnimation;
  /* 003C */ int curMotion;
  /* 0040 */ std::uint32_t lastSoundAnimation;
  /* 0044 */ std::uint32_t lastSoundMotion;
  /* 0048 */ std::uint32_t bodyPalette;
  /* 004C */ float angle;
  /* 0050 */ float scale;
  /* 0054 */ float shadowScale;
  /* 0058 */ float speed;
  /* 005C */ float distanceScale;
  /* 0060 */ float motionFinishThreshold;
  /* 0064 */ float motionSpeedScale;
  /* 0068 */ float motionSpeed;
  /* 006C */ std::uint32_t motionType;
  /* 0070 */ std::uint32_t state;
  /* 0074 */ std::uint32_t previousState;
  /* 0078 */ std::uint32_t spriteOffsetX;
  /* 007C */ std::uint32_t spriteAngle;
  /* 0080 */ std::uint32_t depthLayer;
  /* 0084 */ std::uint32_t overrideColor;
  /* 0088 */ std::uint32_t started;
  /* 008C */ std::uint32_t motionStartTime;
  /* 0090 */ std::uint32_t savedTintColor;
  /* 0094 */ std::uint32_t targetTintColor;
  /* 0098 */ std::uint32_t color;
  /* 009C */ bool isLieOnGround;
  /* 009D */ bool isMotionFinished;
  /* 009E */ bool isMotionFreezed;
  /* 009F */ bool isSingleColor;
  /* 00A0 */ unsigned char visible;
  /* 00A1 */ unsigned char enabled;
  /* 00A2 */ unsigned char flatDepth;
  /* 00A3 */ unsigned char colorLocked;
  union {
    /* 00A4 */ std::uint16_t visibilityState;
    /* 00A4 */ unsigned char visibilityOptions[2];
  };
  /* 00A6 */ unsigned char padding_00A6[2];
  /* 00A8 */ std::uint32_t isCharacter;
  /* 00AC */ int screenX;
  /* 00B0 */ int screenY;
  /* 00B4 */ unsigned char useGroundLighting;
  /* 00B5 */ unsigned char sequenceAnimation;
  /* 00B6 */ unsigned char sequenceFrame;
  /* 00B7 */ unsigned char sequenceAnimations[5];
  /* 00BC */ unsigned char sequenceFrames[5];
  /* 00C1 */ unsigned char sequenceLast;
  /* 00C2 */ unsigned char sequenceInterval;
  /* 00C3 */ unsigned char sequenceFinishAnimation;
  /* 00C4 */ unsigned char sequenceFinishFrame;
  /* 00C5 */ unsigned char sequenceIndex;
  /* 00C6 */ unsigned char padding_00C6[2];
  /* 00C8 */ std::uint32_t frameSequenceActive;
  /* 00CC */ int sequenceLoop;
  /* 00D0 */ int sequenceTimed;
  /* 00D4 */ int sequenceFinished;
  /* 00D8 */ std::uint32_t animationSequenceActive;
  /* 00DC */ std::uint32_t timedSequenceActive;
  /* 00E0 */ std::uint32_t sequenceTick;
  /* 00E4 */ std::uint32_t sequenceDeadline;
  /* 00E8 */ std::uint32_t flags;
  union {
    /* 00EC */ std::uint32_t effectPhaseState;
    /* 00EC */ unsigned char effectPhaseBytes[4];
  };
  union {
    /* 00F0 */ std::uint16_t pulsePhaseState;
    /* 00F0 */ unsigned char pulsePhases[2];
  };
  /* 00F2 */ unsigned char outlineAlpha;
  /* 00F3 */ unsigned char padding_00F3;
  /* 00F4 */ short effectPhase;
  /* 00F6 */ unsigned char tintPhase;
  /* 00F7 */ unsigned char runStartTick;
  /* 00F8 */ short hoverAngle;
  /* 00FA */ unsigned char hoverHeight;
  /* 00FB */ unsigned char battleTeam;
  union {
    /* 00FC */ std::uint32_t fontSequenceState;
    struct {
      /* 00FC */ std::uint16_t font;
      /* 00FE */ unsigned char sequenceMode;
      /* 00FF */ unsigned char padding_00FF;
    };
  };
  /* 0100 */ unsigned char padding_0100;
  /* 0101 */ unsigned char padding_0101[3];
  /* 0104 */ CSprRes* sprite;
  /* 0108 */ CActRes* action;
  /* 010C */ std::uint32_t spriteJob;
  /* 0110 */ std::uint32_t accountId;
  /* 0114 */ unsigned char specialMotionLocked;
  /* 0115 */ unsigned char padding_0115[3];
  /* 0118 */ CInstantVar instant;
  /* 0124 */ std::uint32_t blendFlags;
  /* 0128 */ std::uint32_t hoverOffsetLow;
  /* 012C */ std::uint32_t hoverOffsetHigh;
  /* 0130 */ unsigned char translucentOther[8];
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
