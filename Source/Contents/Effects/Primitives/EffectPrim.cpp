#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/World/World.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"

#include <cstring>
#include <windows.h>

// OFFSET: 0x00BAE170
// FUNCTION: fn_19762
EffectPrimitiveSlot::EffectPrimitiveSlot() {
  for (int i = 0; i < 4; ++i) {
    this->vectors[i].x = 0.0f;
    this->vectors[i].y = 0.0f;
    this->vectors[i].z = 0.0f;
  }
}

// OFFSET: 0x00BAD940
// FUNCTION: fn_19761
CEffectPrim::CEffectPrim() {
  this->state.playWaveType = 0;
  this->state.playWaveName = 0;
  this->state.type = 0;
  this->state.count = 0;
  this->state.pattern = 0;
  this->state.renderFlag = 1;
  this->state.masterReached = 0;
  this->state.alphaSpeedCycle = 0;
  this->state.alphaCycle = 0;
  this->state.chRadiSpeed = 0;
  this->state.isDisappear = 0;
  this->state.changePointReached = 0;
  this->state.master = nullptr;
  this->state.chCnt = 0;
  this->state.colorCycleIndex = 0;
  this->state.stateCnt = 0;
  this->state.radius = 0;
  this->state.minRadius = 0;
  this->state.arcAngle = 36.0f;
  this->render.angle = 0;
  this->state.validAngle = 180;
  this->state.size = 1.0f;
  this->state.innerSize = 0;
  this->state.outerSize = 0;
  this->state.widthSize = 0.0f;
  this->state.heightSize = 0.0f;
  this->state.maxHeightSize = 100.0f;
  this->state.speed = 0.0f;
  this->state.sizeSpeed = 0.0f;
  this->state.radiusSpeed = 0.0f;
  this->state.outerSpeed = 0;
  this->state.innerSpeed = 0;
  this->state.longSpeed = 0;
  this->state.latiSpeed = 0;
  this->state.rollSpeed = 0;
  this->state.widthSpeed = 0.0f;
  this->state.heightSpeed = 0.0f;
  this->state.alphaSpeed = 0.0f;
  this->state.gravSpeed = 0;
  this->state.accel = 0.0f;
  this->state.sizeAccel = 0;
  this->state.radiusAccel = 0.0f;
  this->state.gravAccel = 0;
  this->state.longAccel = 0;
  this->state.latiAccel = 0;
  this->state.rollAccel = 0;
  this->state.outerAccel = 0;
  this->state.innerAccel = 0;
  this->state.widthAccel = 0;
  this->state.heightAccel = 0;
  this->state.red = 255;
  this->state.green = 255;
  this->state.blue = 255;
  this->state.redSpeed = 0;
  this->state.greenSpeed = 0;
  this->state.blueSpeed = 0;
  this->state.changePoint = -1;
  this->state.changeWidthSpeed = 0.0001f;
  this->state.changeHeightSpeed = 0.0001f;
  this->state.changeSpeed = 0.0001f;
  this->state.changeSpeed3d.x = 0;
  this->state.changeSpeed3d.y = 0;
  this->state.changeSpeed3d.z = 0;
  this->state.changeAccel3d.x = 0;
  this->state.changeAccel3d.y = 0;
  this->state.changeAccel3d.z = 0;
  this->state.changeWidthAccel = 0.0001f;
  this->state.changeHeightAccel = 0.0001f;
  this->state.changeAccel = 0.0001f;
  this->state.RGBCycleCnt = 0;
  this->state.RGBCycleDelay = 0;
  this->state.commonPattern = 0;
  this->state.speed3d.Set(0, 0, 0);
  this->state.deltaPos = {0, 0, 0};
  this->state.deltaPos2 = {0, 0, 0};
  this->state.deltaPos3.Set(0, 0, 0);
  this->state.deltaPosAccel.Set(0, 0, 0);
  this->state.longitude = 0;
  this->state.latitude = 0;
  this->state.roll = 0.0f;
  this->state.duration = 100;
  this->state.fadeInCnt = -1;
  this->state.fadeOutCnt = -1;
  this->state.argb = -1;
  this->state.alpha = 254.0f;
  this->state.alphaDelta = 1.0f;
  this->state.maxAlpha = 254.0f;
  this->state.minAlpha = 0;
  this->state.repeatAnim = 1;
  this->state.numSegment = 1;
  this->state.animSpeed = 4;
  this->render.previousAnimation = 0;
  this->render.curMotion = 0;
  this->state.totalTexture = 1;
  this->state.v1 = 0;
  this->state.u1Bits = 0;
  this->state.v2 = 1.0f;
  this->state.u2 = 1.0f;

  std::memcpy(&this->state.transform, g_identityMatrix, sizeof(g_identityMatrix));
  std::memcpy(&this->state.rotTransform, g_identityMatrix, sizeof(g_identityMatrix));
  this->state.useRenderFlag[0] = 0;
  SecureZeroMemory(this->state.slots, sizeof(this->state.slots));
  this->state.spriteLift = 0;
}

// OFFSET: 0x00BE6FC0
// FUNCTION: fn_19930
void CEffectPrim::HandleMasterMessage(CGameObject*, int message, int a, int, int, int, int, int, int) {
  if (message == 0x18 && a == 0) {
    this->state.master = nullptr;
  }
}

// OFFSET: 0x00C1DE70
// FUNCTION: fn_20076
CEffectPrim* CRagEffect::LaunchEffectPrim(int type, vector3d deltaPos) {
  auto* particle = new CEffectPrim();
  if (!particle) {
    return nullptr;
  }

  particle->Initialize(*this, type, deltaPos);
  this->state.primList.push_back(particle);
  CGameMode* mode = g_modeMgr.GetGameMode();
  mode->scene->objects.push_back(particle);

  return particle;
}
