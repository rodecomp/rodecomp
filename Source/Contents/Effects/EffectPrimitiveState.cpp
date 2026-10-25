#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffectState.h"
#include "Contents/Actors/RenderObjectState.h"

#include <bit>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00BAD940
// FUNCTION: fn_19761
EffectPrimitiveState::EffectPrimitiveState(RenderObjectState& render) {
  this->textures = nullptr;
  this->unknown_0208 = 0;
  this->unknown_0148 = 0;
  this->unknown_01CC = 0;
  this->type = 0;
  this->flags = 0;
  this->unknown_01C4 = 1;
  this->unknown_014C.x = 0;
  this->unknown_014C.y = 0;
  this->unknown_014C.z = 0;
  this->unknown_0158 = 0;
  this->unknown_015C = 0;
  this->unknown_0138 = 0;
  this->parent = nullptr;
  this->unknown_01D4 = 0;
  this->unknown_01D8 = 0;
  this->unknown_01E0 = 0;
  this->unknown_0348 = 0;
  this->unknown_028C = 0;
  this->unknown_0288 = 36.0f;
  render.angle = 0;
  this->unknown_01F8 = 180;
  this->scale = 1.0f;
  this->unknown_02F4 = 0;
  this->unknown_02F8 = 0;
  this->unknown_0354 = 0.0f;
  this->unknown_0360 = 0.0f;
  this->unknown_036C = 100.0f;
  this->unknown_02BC = 0.0f;
  this->unknown_031C = 0.0f;
  this->unknown_034C = 0.0f;
  this->unknown_0300 = 0;
  this->unknown_02FC = 0;
  this->unknown_0330 = 0;
  this->unknown_0334 = 0;
  this->unknown_0338 = 0;
  this->unknown_0358 = 0.0f;
  this->unknown_0364 = 0.0f;
  this->unknown_0328 = 0.0f;
  this->unknown_02EC = 0;
  this->unknown_02C0 = 0.0f;
  this->unknown_0320 = 0;
  this->unknown_0350 = 0.0f;
  this->unknown_02F0 = 0;
  this->unknown_033C = 0;
  this->unknown_0340 = 0;
  this->unknown_0344 = 0;
  this->unknown_0308 = 0;
  this->unknown_0304 = 0;
  this->unknown_035C = 0;
  this->unknown_0368 = 0;
  this->red = 255;
  this->green = 255;
  this->blue = 255;
  this->unknown_0248 = 0;
  this->unknown_024C = 0;
  this->unknown_0250 = 0;
  this->unknown_0200 = -1;
  this->unknown_02C8 = 0.0001f;
  this->unknown_02CC = 0.0001f;
  this->unknown_029C = 0.0001f;
  this->unknown_02A4.x = 0;
  this->unknown_02A4.y = 0;
  this->unknown_02A4.z = 0;
  this->unknown_02B0.x = 0;
  this->unknown_02B0.y = 0;
  this->unknown_02B0.z = 0;
  this->unknown_02C4 = 0.0001f;
  this->unknown_02D0 = 0.0001f;
  this->unknown_02A0 = 0.0001f;
  this->unknown_01FC = 0;
  this->unknown_0234 = 0;
  this->unknown_0238 = 0;
  this->unknown_030C = 0;
  this->unknown_0310 = 0;
  this->unknown_0314 = 0.0f;
  this->lifetime = 100;
  this->unknown_01EC = -1;
  this->unknown_01F0 = -1;
  this->unknown_01C0 = -1;
  this->unknown_0324 = 254.0f;
  this->unknown_032C = 1.0f;
  this->unknown_02D4 = 254.0f;
  this->unknown_02D8 = 0;
  this->unknown_0254 = 1;
  this->unknown_01F4 = 1;
  this->unknown_01E8 = 4;
  render.previousAnimation = 0;
  render.unknown_003C = 0;
  this->textureCount = 1;
  this->unknown_02E4 = 0;
  this->unknown_02DC = 0;
  this->unknown_02E8 = 1.0f;
  this->unknown_02E0 = 1.0f;
  this->unknown_01C8[0] = 0;
  this->unknown_FA10 = 0;
  this->unknown_FA14 = 0;

  this->unknown_0370 = {};
  this->origin = {};
  this->offset = {};
  this->velocity = {};
  this->unknown_03A0 = {};
  this->unknown_03AC = {};

  for (EffectVector3& corner : this->corners) {
    corner = {};
  }

  for (EffectPrimitiveRecord& record : this->records) {
    record.position = {};

    for (EffectVector3& corner : record.corners) {
      corner = {};
    }
  }

  for (RenderTriangle& triangle : this->triangles) {
    for (RenderVertex& vertex : triangle.vertices) {
      vertex = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    }
  }

  // STATIC: 0x01312438
  // GLOBAL: gl_21913
  constexpr float identity[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
  std::memcpy(this->matrix1, identity, sizeof(identity));
  std::memcpy(this->matrix2, identity, sizeof(identity));
  std::memset(this->unknown_03E8, 0, sizeof(this->unknown_03E8));
}

// OFFSET: 0x00BAE2C0
// FUNCTION: fn_19763
EffectPrimitiveState::~EffectPrimitiveState() {
  if (this->parent) {
    auto& related = this->parent->related;

    for (auto entry = related.begin(); entry != related.end(); ++entry) {
      if (*entry == this) {
        related.erase(entry);
        break;
      }
    }
  }

  std::free(this->textures);
}

// OFFSET: 0x00BAE5D0
// FUNCTION: fn_19767
void EffectPrimitiveState::Initialize(
  RenderObjectState& render, RagEffectState& parent, const RenderObjectState& parentRender, int type, float x, float y, float z
) {
  const float first = x + this->offset.x;
  this->parent = &parent;
  this->type = type;
  this->offset.x = first;
  this->offset.y = y + this->offset.y;
  this->offset.z = z + this->offset.z;
  this->origin = {parentRender.position[0], parentRender.position[1], parentRender.position[2]};
  render.position[0] = this->origin.x;
  render.position[1] = this->origin.y;
  render.position[2] = this->origin.z;
  this->unknown_013C = 0;
}
