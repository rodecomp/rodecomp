#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVESTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVESTATE_H

#include "Render/Primitives/RenderPrimitive.h"

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Contents/Actors/RenderObjectState.h"
struct RagEffectState;
class CTexture;

struct EffectVector3 {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
};

struct EffectPrimitiveRecord {
  /* 0000 */ EffectVector3 position;
  /* 000C */ EffectVector3 corners[4];
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
  /* 0000 */ unsigned char active;
  /* 0001 */ unsigned char unknown_0001;
  /* 0002 */ std::int16_t unknown_0002;
  /* 0004 */ std::int16_t unknown_0004;
  /* 0006 */ unsigned char unknown_0006[2];
  /* 0008 */ float unknown_0008;
  /* 000C */ int frame;
  /* 0010 */ std::int16_t angle;
  /* 0012 */ unsigned char unknown_0012[2];
  /* 0014 */ float values[21];
  /* 0068 */ unsigned char finished[21];
  /* 007D */ unsigned char unknown_007D;
  /* 007E */ std::int16_t unknown_007E;
  /* 0080 */ float unknown_0080;
  /* 0084 */ EffectVector3 unknown_0084[4];
};

struct EffectPrimitiveState {
  explicit EffectPrimitiveState(RenderObjectState& render);
  ~EffectPrimitiveState();
  void Initialize(RenderObjectState& render, RagEffectState& parent, const RenderObjectState& parentRender, int type, float x, float y, float z);
  void UpdateColor();
  void InitializeRecords(const RenderObjectState& render);
  void GetColorOverride(unsigned char* red, unsigned char* green, unsigned char* blue);

  /* 0138 */ std::uint32_t unknown_0138;
  /* 013C */ std::uint32_t unknown_013C;
  /* 0140 */ RagEffectState* parent;
  /* 0144 */ CTexture** textures;
  /* 0148 */ std::uint32_t unknown_0148;
  /* 014C */ EffectVector3 unknown_014C;
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
  /* 02A4 */ EffectVector3 unknown_02A4;
  /* 02B0 */ EffectVector3 unknown_02B0;
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
  /* 0324 */ float unknown_0324;
  /* 0328 */ float unknown_0328;
  /* 032C */ float unknown_032C;
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
  /* 0370 */ EffectVector3 unknown_0370;
  /* 037C */ EffectVector3 origin;
  /* 0388 */ EffectVector3 offset;
  /* 0394 */ EffectVector3 velocity;
  /* 03A0 */ EffectVector3 unknown_03A0;
  /* 03AC */ EffectVector3 unknown_03AC;
  /* 03B8 */ EffectVector3 corners[4];
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

void __stdcall CalculateVectorAngle(float x, float y, std::int16_t* angle);

static_assert(sizeof(EffectVector3) == 12);
static_assert(sizeof(EffectPrimitiveRecord) == 140);
static_assert(sizeof(EffectPrimitiveSlot) == 0xB4);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(EffectVector3, z) == 0x8);
static_assert(offsetof(EffectPrimitiveRecord, corners) == 0xC);
static_assert(offsetof(EffectPrimitiveRecord, matrix) == 0x48);
static_assert(offsetof(EffectPrimitiveRecord, color) == 0x88);
static_assert(offsetof(EffectPrimitiveSlot, values) == 0x14);
static_assert(offsetof(EffectPrimitiveSlot, finished) == 0x68);
static_assert(offsetof(EffectPrimitiveSlot, unknown_0084) == 0x84);
static_assert(sizeof(EffectPrimitiveState) == 0xF8E0);
#endif

#endif
