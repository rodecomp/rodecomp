#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/AttachedEffect.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00BAE170
// FUNCTION: fn_19762
EffectPrimitiveSlot::EffectPrimitiveSlot() {
  for (int i = 0; i < 4; ++i) {
    this->vectors[i] = {0.0f, 0.0f, 0.0f};
  }
}

// INLINED
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
  this->master = nullptr;
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
  this->alphaSpeed = 0.0f;
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
  this->alpha = 254.0f;
  this->alphaDelta = 1.0f;
  this->unknown_02D4 = 254.0f;
  this->unknown_02D8 = 0;
  this->unknown_0254 = 1;
  this->unknown_01F4 = 1;
  this->unknown_01E8 = 4;
  render.previousAnimation = 0;
  render.curMotion = 0;
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

  for (vector3d& corner : this->corners) {
    corner = {};
  }

  for (EffectPrimitiveRecord& record : this->records) {
    record.position = {};

    for (vector3d& corner : record.corners) {
      corner = {};
    }
  }

  for (RenderTriangle& triangle : this->triangles) {
    for (RenderVertex& vertex : triangle.vertices) {
      vertex = {0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
    }
  }

  std::memcpy(this->matrix1, g_identityMatrix, sizeof(g_identityMatrix));
  std::memcpy(this->matrix2, g_identityMatrix, sizeof(g_identityMatrix));
  std::memset(this->slots, 0, sizeof(this->slots));
}

// INLINED
EffectPrimitiveState::~EffectPrimitiveState() {
  if (this->master) {
    auto& related = this->master->state.related;

    for (auto entry = related.begin(); entry != related.end(); ++entry) {
      if (*entry == this) {
        related.erase(entry);
        break;
      }
    }
  }

  std::free(this->textures);
}

// OFFSET: 0x00BAE3A0

// OFFSET: 0x00BAE460
// FUNCTION: fn_19765
void EffectPrimitiveState::GetColorOverride(unsigned char* red, unsigned char* green, unsigned char* blue) {
  if (this->master->state.unknown_11C88[0] != -1) {
    *red = static_cast<unsigned char>(this->master->state.unknown_11C88[0]);
  }

  if (this->master->state.unknown_11C88[1] != -1) {
    *green = static_cast<unsigned char>(this->master->state.unknown_11C88[1]);
  }

  if (this->master->state.unknown_11C88[2] != -1) {
    *blue = static_cast<unsigned char>(this->master->state.unknown_11C88[2]);
  }
}

// OFFSET: 0x00BAE4B0
// FUNCTION: fn_19766
void __stdcall Get_Angle(float x, float y, std::int16_t* angle) {
  const float length = SquareRoot(x * x + y * y);
  const float normalY = y / length;
  const float normalX = x / length;

  for (std::int16_t index = 0; index <= 180; ++index) {
    if (normalX >= ClientCosine(index)) {
      *angle = normalY < 0.0f ? 360 - index : index;
      return;
    }
  }

  *angle = 180;
}

// OFFSET: 0x00BAE5D0
// FUNCTION: fn_19767
void EffectPrimitiveState::Initialize(
  RenderObjectState& render, AttachedEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z
) {
  const float first = x + this->offset.x;
  this->master = &master;
  this->type = type;
  this->offset.x = first;
  this->offset.y = y + this->offset.y;
  this->offset.z = z + this->offset.z;
  this->origin = {masterRender.position.x, masterRender.position.y, masterRender.position.z};
  render.position.x = this->origin.x;
  render.position.y = this->origin.y;
  render.position.z = this->origin.z;
  this->unknown_013C = 0;
}
