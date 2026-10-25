#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVESTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVESTATE_H

#include "Render/Primitives/RenderPrimitive.h"

#include <cstdint>
#include <vector>

struct RenderObjectState;
struct RagEffectState;
class CTexture;

struct EffectVector3 {
  float x;
  float y;
  float z;
};

struct EffectPrimitiveRecord {
  EffectVector3 position;
  EffectVector3 corners[4];
  std::uint32_t unknown_003C;
  float scale;
  float angle;
  float matrix[12];
  std::int32_t red;
  std::int32_t green;
  std::int32_t blue;
  float alpha;
  std::uint32_t color;
};

struct EffectPrimitiveState {
  explicit EffectPrimitiveState(RenderObjectState& render);
  ~EffectPrimitiveState();
  void Initialize(RenderObjectState& render, RagEffectState& parent, const RenderObjectState& parentRender, int type, float x, float y, float z);
  void UpdateColor();
  void InitializeRecords(const RenderObjectState& render);

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
  /* 0290 */ unsigned char unknown_0290[12];
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
  /* 02EC */ std::uint32_t unknown_02EC;
  /* 02F0 */ std::uint32_t unknown_02F0;
  /* 02F4 */ std::uint32_t unknown_02F4;
  /* 02F8 */ std::uint32_t unknown_02F8;
  /* 02FC */ std::uint32_t unknown_02FC;
  /* 0300 */ std::uint32_t unknown_0300;
  /* 0304 */ std::uint32_t unknown_0304;
  /* 0308 */ std::uint32_t unknown_0308;
  /* 030C */ float unknown_030C;
  /* 0310 */ float unknown_0310;
  /* 0314 */ float unknown_0314;
  /* 0318 */ float scale;
  /* 031C */ float unknown_031C;
  /* 0320 */ std::uint32_t unknown_0320;
  /* 0324 */ float unknown_0324;
  /* 0328 */ float unknown_0328;
  /* 032C */ float unknown_032C;
  /* 0330 */ float unknown_0330;
  /* 0334 */ float unknown_0334;
  /* 0338 */ std::uint32_t unknown_0338;
  /* 033C */ float unknown_033C;
  /* 0340 */ float unknown_0340;
  /* 0344 */ std::uint32_t unknown_0344;
  /* 0348 */ std::uint32_t unknown_0348;
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
  /* 03E8 */ unsigned char unknown_03E8[720];
  /* 06B8 */ EffectPrimitiveRecord records[50];
  /* 2210 */ RenderTriangle triangles[512];
  /* FA10 */ std::uint32_t unknown_FA10;
  /* FA14 */ std::uint32_t unknown_FA14;
};

static_assert(sizeof(EffectVector3) == 12);
static_assert(sizeof(EffectPrimitiveRecord) == 140);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectPrimitiveState) == 0xF8E0);
#endif

#endif
