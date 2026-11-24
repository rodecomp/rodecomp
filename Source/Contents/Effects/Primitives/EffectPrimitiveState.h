#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PRIMITIVES_EFFECTPRIMITIVESTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PRIMITIVES_EFFECTPRIMITIVESTATE_H

#include "Render/Primitives/RenderPrimitive.h"

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"
struct CRagEffect;
class CTexture;

extern float g_fullCircleAngle;
extern float g_fireflyNormalDrift;
extern float g_fireflyModeFive;

struct ZeroVector3d : vector3d {
  // INLINED
  ZeroVector3d() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
  }

  using vector3d::operator=;

  // INLINED
  void Set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

struct EffectPrimitiveRecord {
  /* 0000 */ ZeroVector3d position;
  /* 000C */ ZeroVector3d corners[4];
  /* 003C */ float radius;
  /* 0040 */ float scale;
  /* 0044 */ float angle;
  /* 0048 */ float matrix[12];
  /* 0078 */ std::int32_t red;
  /* 007C */ std::int32_t green;
  /* 0080 */ std::int32_t blue;
  /* 0084 */ float alpha;
  /* 0088 */ std::uint32_t color;
};

struct EffectPrimitiveSlot {
  EffectPrimitiveSlot();
  /* 0000 */ char life;
  /* 0001 */ unsigned char padding_0001;
  /* 0002 */ std::int16_t alphaB;
  /* 0004 */ std::int16_t alphaT;
  /* 0006 */ std::int16_t fullDisplayAngle;
  /* 0008 */ float maxHeight;
  /* 000C */ int process;
  /* 0010 */ std::int16_t rotStart;
  /* 0012 */ unsigned char padding_0012[2];
  /* 0014 */ float height[21];
  /* 0068 */ char flag1[21];
  /* 007D */ unsigned char padding_007D;
  /* 007E */ std::int16_t riseAngle;
  /* 0080 */ float distance;
  union {
    /* 0084 */ vector3d vectors[4];
    struct {
      /* 0084 */ vector3d vecBNow;
      /* 0090 */ vector3d vecTNow;
      /* 009C */ vector3d vecBPre;
      /* 00A8 */ vector3d vecTPre;
    };
  };
};

struct EffectPrimitiveState {
  // INLINED
  __forceinline EffectPrimitiveState() {}

  /* 0138 */ std::uint32_t changePointReached;
  /* 013C */ std::uint32_t isChangeProcessed;
  /* 0140 */ CRagEffect* master;
  /* 0144 */ CTexture** textures = nullptr;
  /* 0148 */ std::uint32_t playWaveName;
  /* 014C */ std::uint32_t masterReached;
  /* 0150 */ std::uint32_t alphaSpeedCycle;
  /* 0154 */ std::uint32_t alphaCycle;
  /* 0158 */ std::uint32_t chRadiSpeed;
  /* 015C */ std::uint32_t isDisappear;
  /* 0160 */ matrix transform;
  /* 0190 */ matrix rotTransform;
  /* 01C0 */ std::uint32_t argb;
  /* 01C4 */ std::uint32_t renderFlag;
  /* 01C8 */ unsigned char useRenderFlag[4];
  /* 01CC */ std::uint32_t type;
  /* 01D0 */ int count;
  /* 01D4 */ std::uint32_t chCnt;
  /* 01D8 */ int colorCycleIndex;
  /* 01DC */ std::uint32_t pattern;
  /* 01E0 */ int stateCnt;
  /* 01E4 */ int duration;
  /* 01E8 */ int animSpeed;
  /* 01EC */ int fadeInCnt;
  /* 01F0 */ int fadeOutCnt;
  /* 01F4 */ int numSegment;
  /* 01F8 */ int validAngle;
  /* 01FC */ std::uint32_t RGBCycleCnt;
  /* 0200 */ int changePoint;
  /* 0204 */ unsigned totalTexture;
  /* 0208 */ std::uint32_t playWaveType;
  /* 020C */ unsigned char notifyCounts[40];
  /* 0234 */ std::uint32_t RGBCycleDelay;
  /* 0238 */ std::uint32_t commonPattern;
  /* 023C */ int red;
  /* 0240 */ int green;
  /* 0244 */ int blue;
  /* 0248 */ int redSpeed;
  /* 024C */ int greenSpeed;
  /* 0250 */ int blueSpeed;
  /* 0254 */ unsigned char repeatAnim;
  /* 0255 */ unsigned char padding_0255[3];
  /* 0258 */ std::vector<float> chPoint;
  /* 0264 */ std::vector<float> chVal1;
  /* 0270 */ std::vector<float> chVal2;
  /* 027C */ std::vector<float> chVal3;
  /* 0288 */ float arcAngle;
  /* 028C */ std::uint32_t minRadius;
  /* 0290 */ float rotAxis[3];
  /* 029C */ float changeSpeed;
  /* 02A0 */ float changeAccel;
  /* 02A4 */ vector3d changeSpeed3d;
  /* 02B0 */ vector3d changeAccel3d;
  /* 02BC */ float speed;
  /* 02C0 */ float accel;
  /* 02C4 */ float changeWidthAccel;
  /* 02C8 */ float changeWidthSpeed;
  /* 02CC */ float changeHeightSpeed;
  /* 02D0 */ float changeHeightAccel;
  /* 02D4 */ float maxAlpha;
  /* 02D8 */ float minAlpha;
  /* 02DC */ std::uint32_t u1Bits;
  /* 02E0 */ float u2;
  /* 02E4 */ float v1;
  /* 02E8 */ float v2;
  /* 02EC */ float gravSpeed;
  /* 02F0 */ float gravAccel;
  /* 02F4 */ float innerSize;
  /* 02F8 */ float outerSize;
  /* 02FC */ float innerSpeed;
  /* 0300 */ float outerSpeed;
  /* 0304 */ float innerAccel;
  /* 0308 */ float outerAccel;
  /* 030C */ float longitude;
  /* 0310 */ float latitude;
  /* 0314 */ float roll;
  /* 0318 */ float size;
  /* 031C */ float sizeSpeed;
  /* 0320 */ float sizeAccel;
  /* 0324 */ float alpha;
  /* 0328 */ float alphaSpeed;
  /* 032C */ float alphaDelta;
  /* 0330 */ float longSpeed;
  /* 0334 */ float latiSpeed;
  /* 0338 */ float rollSpeed;
  /* 033C */ float longAccel;
  /* 0340 */ float latiAccel;
  /* 0344 */ float rollAccel;
  /* 0348 */ float radius;
  /* 034C */ float radiusSpeed;
  /* 0350 */ float radiusAccel;
  /* 0354 */ float widthSize;
  /* 0358 */ float widthSpeed;
  /* 035C */ float widthAccel;
  /* 0360 */ float heightSize;
  /* 0364 */ float heightSpeed;
  /* 0368 */ float heightAccel;
  /* 036C */ float maxHeightSize;
  /* 0370 */ ZeroVector3d speed3d;
  /* 037C */ ZeroVector3d orgPos;
  /* 0388 */ vector3d deltaPos;
  /* 0394 */ vector3d deltaPos2;
  /* 03A0 */ ZeroVector3d deltaPos3;
  /* 03AC */ ZeroVector3d deltaPosAccel;
  /* 03B8 */ ZeroVector3d segPos[4];
  /* 03E8 */ EffectPrimitiveSlot slots[4];
  /* 06B8 */ EffectPrimitiveRecord records[50];
  /* 2210 */ RenderTriangle triangles[512];
  union {
    /* FA10 */ std::int64_t spriteLift;
    struct {
      /* FA10 */ std::uint32_t spriteLiftLow;
      /* FA14 */ std::uint32_t spriteLiftHigh;
    };
  };
};

class CActRes;

struct EffectPrimitiveObject {
  /* 0000 */ RenderObjectState render;
  /* 0138 */ EffectPrimitiveState state;

  CActRes* ActionResource();
  bool AdvanceFrame();
};

static_assert(sizeof(EffectPrimitiveRecord) == 140);
static_assert(sizeof(EffectPrimitiveSlot) == 0xB4);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(EffectPrimitiveRecord, corners) == 0xC);
static_assert(offsetof(EffectPrimitiveRecord, matrix) == 0x48);
static_assert(offsetof(EffectPrimitiveRecord, color) == 0x88);
static_assert(offsetof(EffectPrimitiveSlot, height) == 0x14);
static_assert(offsetof(EffectPrimitiveSlot, flag1) == 0x68);
static_assert(offsetof(EffectPrimitiveSlot, vecBNow) == 0x84);
static_assert(offsetof(EffectPrimitiveState, masterReached) == 0x14);
static_assert(offsetof(EffectPrimitiveState, alphaCycle) == 0x1C);
static_assert(sizeof(EffectPrimitiveState) == 0xF8E0);
#endif

#endif
