#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <cstring>

// INLINED
CEffectPrim::~CEffectPrim() {
  if (this->state.master) {
    this->state.master->SendMsg(this, AM_CHILD_DEAD, 0, 0, 0, 0, 0, 0);
  }

  if (this->state.textures) {
    delete[] this->state.textures;
  }
}

// OFFSET: 0x00BAE460
// FUNCTION: fn_19765
void CEffectPrim::GetColorOverride(unsigned char* red, unsigned char* green, unsigned char* blue) {
  if (this->state.master->state.colorChannels[0] != -1) {
    *red = static_cast<unsigned char>(this->state.master->state.colorChannels[0]);
  }

  if (this->state.master->state.colorChannels[1] != -1) {
    *green = static_cast<unsigned char>(this->state.master->state.colorChannels[1]);
  }

  if (this->state.master->state.colorChannels[2] != -1) {
    *blue = static_cast<unsigned char>(this->state.master->state.colorChannels[2]);
  }
}

// OFFSET: 0x00BAE4B0
// FUNCTION: fn_19766
void CEffectPrim::Get_Angle(float x, float y, std::int16_t* angle) {
  const float length = SquareRoot(x * x + y * y);
  const float normalY = y / length;
  const float normalX = x / length;

  for (std::int16_t index = 0; index <= 180; ++index) {
    if (normalX >= g_cosTable[index]) {
      if (normalY >= 0.0f) {
        *angle = index;
      } else {
        *angle = 360 - index;
      }
      return;
    }
  }

  *angle = 180;
}

// OFFSET: 0x00BAE5D0
// FUNCTION: fn_19767
void CEffectPrim::Initialize(CRagEffect& master, int type, vector3d deltaPos) {
  this->state.master = &master;
  this->state.count = type;
  this->state.deltaPos.x = deltaPos.x + this->state.deltaPos.x;
  this->state.deltaPos.y = deltaPos.y + this->state.deltaPos.y;
  this->state.deltaPos.z = deltaPos.z + this->state.deltaPos.z;
  this->state.orgPos = master.render.position;
  this->render.position = this->state.orgPos;
  this->state.isChangeProcessed = 0;
}
