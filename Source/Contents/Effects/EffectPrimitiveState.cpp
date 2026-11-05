#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffect.h"
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
EffectPrimitiveState::~EffectPrimitiveState() {
  if (this->master) {
    auto& related = this->master->state.primList;

    for (auto entry = related.begin(); entry != related.end(); ++entry) {
      if (*entry == this) {
        related.erase(entry);
        break;
      }
    }
  }

  std::free(this->textures);
}

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
    if (normalX >= g_cosTable[index]) {
      *angle = normalY < 0.0f ? 360 - index : index;
      return;
    }
  }

  *angle = 180;
}

// OFFSET: 0x00BAE5D0
// FUNCTION: fn_19767
void EffectPrimitiveState::Initialize(
  RenderObjectState& render, CRagEffect& master, const RenderObjectState& masterRender, int type, float x, float y, float z
) {
  const float first = x + this->deltaPos.x;
  this->master = &master;
  this->count = type;
  this->deltaPos.x = first;
  this->deltaPos.y = y + this->deltaPos.y;
  this->deltaPos.z = z + this->deltaPos.z;
  this->orgPos = {masterRender.position.x, masterRender.position.y, masterRender.position.z};
  render.position.x = this->orgPos.x;
  render.position.y = this->orgPos.y;
  render.position.z = this->orgPos.z;
  this->isChangeProcessed = 0;
}
