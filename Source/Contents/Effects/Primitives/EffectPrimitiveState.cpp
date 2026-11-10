#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Core/RagEffect.h"
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
  if (this->master->state.colorChannels[0] != -1) {
    *red = static_cast<unsigned char>(this->master->state.colorChannels[0]);
  }

  if (this->master->state.colorChannels[1] != -1) {
    *green = static_cast<unsigned char>(this->master->state.colorChannels[1]);
  }

  if (this->master->state.colorChannels[2] != -1) {
    *blue = static_cast<unsigned char>(this->master->state.colorChannels[2]);
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
void CEffectPrim::Initialize(CRagEffect& master, int type, float x, float y, float z) {
  this->state.master = &master;
  this->state.count = type;
  this->state.deltaPos.x = x + this->state.deltaPos.x;
  this->state.deltaPos.y = y + this->state.deltaPos.y;
  this->state.deltaPos.z = z + this->state.deltaPos.z;
  this->state.orgPos.x = master.render.position.x;
  this->state.orgPos.y = master.render.position.y;
  this->state.orgPos.z = master.render.position.z;
  this->render.position.x = this->state.orgPos.x;
  this->render.position.y = this->state.orgPos.y;
  this->render.position.z = this->state.orgPos.z;
  this->state.isChangeProcessed = 0;
}
