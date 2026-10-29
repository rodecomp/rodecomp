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
EffectPrimitiveState::EffectPrimitiveState(RenderObjectState& render) {
  this->textures = nullptr;
  this->playWaveType = 0;
  this->playWaveName = 0;
  this->type = 0;
  this->count = 0;
  this->pattern = 0;
  this->renderFlag = 1;
  this->unknown_014C.x = 0;
  this->unknown_014C.y = 0;
  this->unknown_014C.z = 0;
  this->chRadiSpeed = 0;
  this->isDisappear = 0;
  this->unknown_0138 = 0;
  this->master = nullptr;
  this->chCnt = 0;
  this->unknown_01D8 = 0;
  this->stateCnt = 0;
  this->radius = 0;
  this->minRadius = 0;
  this->arcAngle = 36.0f;
  render.angle = 0;
  this->validAngle = 180;
  this->size = 1.0f;
  this->innerSize = 0;
  this->outerSize = 0;
  this->widthSize = 0.0f;
  this->heightSize = 0.0f;
  this->maxHeightSize = 100.0f;
  this->speed = 0.0f;
  this->sizeSpeed = 0.0f;
  this->radiusSpeed = 0.0f;
  this->outerSpeed = 0;
  this->innerSpeed = 0;
  this->longSpeed = 0;
  this->latiSpeed = 0;
  this->rollSpeed = 0;
  this->widthSpeed = 0.0f;
  this->heightSpeed = 0.0f;
  this->alphaSpeed = 0.0f;
  this->gravSpeed = 0;
  this->accel = 0.0f;
  this->sizeAccel = 0;
  this->radiusAccel = 0.0f;
  this->gravAccel = 0;
  this->longAccel = 0;
  this->latiAccel = 0;
  this->rollAccel = 0;
  this->outerAccel = 0;
  this->innerAccel = 0;
  this->widthAccel = 0;
  this->heightAccel = 0;
  this->red = 255;
  this->green = 255;
  this->blue = 255;
  this->redSpeed = 0;
  this->greenSpeed = 0;
  this->blueSpeed = 0;
  this->changePoint = -1;
  this->changeWidthSpeed = 0.0001f;
  this->changeHeightSpeed = 0.0001f;
  this->changeSpeed = 0.0001f;
  this->changeSpeed3d.x = 0;
  this->changeSpeed3d.y = 0;
  this->changeSpeed3d.z = 0;
  this->changeAccel3d.x = 0;
  this->changeAccel3d.y = 0;
  this->changeAccel3d.z = 0;
  this->changeWidthAccel = 0.0001f;
  this->changeHeightAccel = 0.0001f;
  this->changeAccel = 0.0001f;
  this->RGBCycleCnt = 0;
  this->RGBCycleDelay = 0;
  this->commonPattern = 0;
  this->longitude = 0;
  this->latitude = 0;
  this->roll = 0.0f;
  this->duration = 100;
  this->fadeInCnt = -1;
  this->fadeOutCnt = -1;
  this->argb = -1;
  this->alpha = 254.0f;
  this->alphaDelta = 1.0f;
  this->maxAlpha = 254.0f;
  this->minAlpha = 0;
  this->repeatAnim = 1;
  this->numSegment = 1;
  this->animSpeed = 4;
  render.previousAnimation = 0;
  render.curMotion = 0;
  this->totalTexture = 1;
  this->v1 = 0;
  this->unknown_02DC = 0;
  this->v2 = 1.0f;
  this->u2 = 1.0f;
  this->unknown_01C8[0] = 0;
  this->unknown_FA10 = 0;
  this->unknown_FA14 = 0;

  this->speed3d = {};
  this->orgPos = {};
  this->deltaPos = {};
  this->deltaPos2 = {};
  this->deltaPos3 = {};
  this->deltaPosAccel = {};

  for (vector3d& corner : this->segPos) {
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

  std::memcpy(this->transform, g_identityMatrix, sizeof(g_identityMatrix));
  std::memcpy(this->rotTransform, g_identityMatrix, sizeof(g_identityMatrix));
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
