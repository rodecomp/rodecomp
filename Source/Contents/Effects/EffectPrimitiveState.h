#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVESTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVESTATE_H

#include "Render/Primitives/RenderPrimitive.h"

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/Vector3d.h"
struct AttachedEffect;
class CTexture;

struct EffectPrimitiveRecord {
  /* 0000 */ vector3d position;
  /* 000C */ vector3d corners[4];
  /* 003C */ float unknown_003C;
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
  /* 0001 */ unsigned char unknown_0001;
  /* 0002 */ std::int16_t alphaB;
  /* 0004 */ std::int16_t alphaT;
  /* 0006 */ std::int16_t fullDisplayAngle;
  /* 0008 */ float maxHeight;
  /* 000C */ int process;
  /* 0010 */ std::int16_t rotStart;
  /* 0012 */ unsigned char unknown_0012[2];
  /* 0014 */ float height[21];
  /* 0068 */ char flag1[21];
  /* 007D */ unsigned char unknown_007D;
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
  explicit EffectPrimitiveState(RenderObjectState& render);
  ~EffectPrimitiveState();
  void Initialize(RenderObjectState& render, AttachedEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z);
  void ProcessAlpha();
  void GetColorOverride(unsigned char* red, unsigned char* green, unsigned char* blue);

  /* 0138 */ std::uint32_t unknown_0138;
  /* 013C */ std::uint32_t unknown_013C;
  /* 0140 */ AttachedEffect* master;
  /* 0144 */ CTexture** textures;
  /* 0148 */ std::uint32_t unknown_0148;
  /* 014C */ vector3d unknown_014C;
  /* 0158 */ std::uint32_t unknown_0158;
  /* 015C */ std::uint32_t unknown_015C;
  /* 0160 */ float matrix1[12];
  /* 0190 */ float matrix2[12];
  /* 01C0 */ std::uint32_t unknown_01C0;
  /* 01C4 */ std::uint32_t unknown_01C4;
  /* 01C8 */ unsigned char unknown_01C8[4];
  /* 01CC */ std::uint32_t unknown_01CC;
  /* 01D0 */ int type;
  /* 01D4 */ std::uint32_t unknown_01D4;
  /* 01D8 */ int unknown_01D8;
  /* 01DC */ std::uint32_t flags;
  /* 01E0 */ int unknown_01E0;
  /* 01E4 */ int lifetime;
  /* 01E8 */ int unknown_01E8;
  /* 01EC */ int unknown_01EC;
  /* 01F0 */ int unknown_01F0;
  /* 01F4 */ int unknown_01F4;
  /* 01F8 */ int unknown_01F8;
  /* 01FC */ std::uint32_t unknown_01FC;
  /* 0200 */ int unknown_0200;
  /* 0204 */ unsigned textureCount;
  /* 0208 */ std::uint32_t unknown_0208;
  /* 020C */ unsigned char unknown_020C[40];
  /* 0234 */ std::uint32_t unknown_0234;
  /* 0238 */ std::uint32_t unknown_0238;
  /* 023C */ int red;
  /* 0240 */ int green;
  /* 0244 */ int blue;
  /* 0248 */ int unknown_0248;
  /* 024C */ int unknown_024C;
  /* 0250 */ int unknown_0250;
  /* 0254 */ unsigned char unknown_0254;
  /* 0255 */ unsigned char unknown_0255[3];
  /* 0258 */ std::vector<float> unknown_0258[4];
  /* 0288 */ float unknown_0288;
  /* 028C */ std::uint32_t unknown_028C;
  /* 0290 */ float unknown_0290[3];
  /* 029C */ float unknown_029C;
  /* 02A0 */ float unknown_02A0;
  /* 02A4 */ vector3d unknown_02A4;
  /* 02B0 */ vector3d unknown_02B0;
  /* 02BC */ float unknown_02BC;
  /* 02C0 */ float unknown_02C0;
  /* 02C4 */ float unknown_02C4;
  /* 02C8 */ float unknown_02C8;
  /* 02CC */ float unknown_02CC;
  /* 02D0 */ float unknown_02D0;
  /* 02D4 */ float unknown_02D4;
  /* 02D8 */ float unknown_02D8;
  /* 02DC */ std::uint32_t unknown_02DC;
  /* 02E0 */ float unknown_02E0;
  /* 02E4 */ float unknown_02E4;
  /* 02E8 */ float unknown_02E8;
  /* 02EC */ float unknown_02EC;
  /* 02F0 */ float unknown_02F0;
  /* 02F4 */ float unknown_02F4;
  /* 02F8 */ float unknown_02F8;
  /* 02FC */ float unknown_02FC;
  /* 0300 */ float unknown_0300;
  /* 0304 */ float unknown_0304;
  /* 0308 */ float unknown_0308;
  /* 030C */ float unknown_030C;
  /* 0310 */ float unknown_0310;
  /* 0314 */ float unknown_0314;
  /* 0318 */ float scale;
  /* 031C */ float unknown_031C;
  /* 0320 */ float unknown_0320;
  /* 0324 */ float alpha;
  /* 0328 */ float alphaSpeed;
  /* 032C */ float alphaDelta;
  /* 0330 */ float unknown_0330;
  /* 0334 */ float unknown_0334;
  /* 0338 */ float unknown_0338;
  /* 033C */ float unknown_033C;
  /* 0340 */ float unknown_0340;
  /* 0344 */ float unknown_0344;
  /* 0348 */ float unknown_0348;
  /* 034C */ float unknown_034C;
  /* 0350 */ float unknown_0350;
  /* 0354 */ float unknown_0354;
  /* 0358 */ float unknown_0358;
  /* 035C */ float unknown_035C;
  /* 0360 */ float unknown_0360;
  /* 0364 */ float unknown_0364;
  /* 0368 */ float unknown_0368;
  /* 036C */ float unknown_036C;
  /* 0370 */ vector3d unknown_0370;
  /* 037C */ vector3d origin;
  /* 0388 */ vector3d offset;
  /* 0394 */ vector3d velocity;
  /* 03A0 */ vector3d unknown_03A0;
  /* 03AC */ vector3d unknown_03AC;
  /* 03B8 */ vector3d corners[4];
  /* 03E8 */ EffectPrimitiveSlot slots[4];
  /* 06B8 */ EffectPrimitiveRecord records[50];
  /* 2210 */ RenderTriangle triangles[512];
  /* FA10 */ std::uint32_t unknown_FA10;
  /* FA14 */ std::uint32_t unknown_FA14;
};

class CActRes;

struct EffectPrimitiveObject {
  /* 0000 */ RenderObjectState render;
  /* 0138 */ EffectPrimitiveState state;

  CActRes* ActionResource();
  bool AdvanceFrame();
};

void __stdcall Get_Angle(float x, float y, std::int16_t* angle);

static_assert(sizeof(EffectPrimitiveRecord) == 140);
static_assert(sizeof(EffectPrimitiveSlot) == 0xB4);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(EffectPrimitiveRecord, corners) == 0xC);
static_assert(offsetof(EffectPrimitiveRecord, matrix) == 0x48);
static_assert(offsetof(EffectPrimitiveRecord, color) == 0x88);
static_assert(offsetof(EffectPrimitiveSlot, height) == 0x14);
static_assert(offsetof(EffectPrimitiveSlot, flag1) == 0x68);
static_assert(offsetof(EffectPrimitiveSlot, vecBNow) == 0x84);
static_assert(sizeof(EffectPrimitiveState) == 0xF8E0);
#endif

#endif
