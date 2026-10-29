#include "Contents/Effects/ChargingPierceEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00703060
CChargingPierceEffect::CChargingPierceEffect() : CEffect(-1), offsets{} {
}

// OFFSET: 0x007030F0
CChargingPierceNewEffect::CChargingPierceNewEffect() : CEffect(-1) {
}

// OFFSET: 0x00703970
// FUNCTION: fn_07025
int CChargingPierceEffect::ActiveCount() {
  int count = 0;
  for (int index = 0; index < 10; ++index) {
    if (this->sparks[index] || this->emitters[index]) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00703A40
// FUNCTION: fn_07026
void CChargingPierceEffect::Slot01() {
  this->retained = 1;
  for (int index = 0; index < 10; ++index) {
    this->sparks[index] = nullptr;
    this->emitters[index] = nullptr;
  }

  this->offsets[0] = {-5.0f, -2.5f, 0.0f};
  this->offsets[1] = {-2.5f, -2.5f, 0.0f};
  this->offsets[2] = {-0.0f, -2.5f, 0.0f};
  this->offsets[3] = {2.5f, -2.5f, 0.0f};
  this->offsets[4] = {5.0f, -2.5f, 0.0f};
  this->offsets[5] = {-5.0f, 0.0f, 0.0f};
  this->offsets[6] = {-2.5f, 0.0f, 0.0f};
  this->offsets[7] = {0.0f, 0.0f, 0.0f};
  this->offsets[8] = {2.5f, 0.0f, 0.0f};
  this->offsets[9] = {5.0f, 0.0f, 0.0f};
}

// OFFSET: 0x00703C40
// FUNCTION: fn_07027
void CChargingPierceNewEffect::Slot01() {
  this->retained = 1;
}

// OFFSET: 0x00703C50
// FUNCTION: fn_07028
bool CChargingPierceEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00703C60
// FUNCTION: fn_07029
bool CChargingPierceNewEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00704360
void CChargingPierceNewEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    const float x = this->position[0];
    const float y = this->position[1];
    const float z = this->position[2];
    CEZ2STRParticle* particle = *it;

    const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
    const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
    const float viewX = view[0] * x + view[3] * y + view[6] * z + view[9] + particle->unknown_00D0;
    const float viewY = view[1] * x + view[4] * y + view[7] * z + view[10] + particle->unknown_00D4;
    const float viewZ = view[2] * x + view[5] * y + view[8] * z + view[11] + particle->unknown_00D8;
    particle->position.x = viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9];
    particle->position.y = viewY * inverse[4] + viewX * inverse[1] + viewZ * inverse[7] + inverse[10];
    particle->position.z = viewY * inverse[5] + viewX * inverse[2] + viewZ * inverse[8] + inverse[11];
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x00704A30
void CChargingPierceEffect::Slot04() {
  for (int index = 0; index < 10; ++index) {
    CParticle* spark = this->sparks[index];
    if (spark) {
      CTexture* texture = spark->texture;
      const float left = g_textureOffsetX / texture->TextureWidth();
      const float right = (texture->Width() + g_textureOffsetX) / texture->TextureWidth();
      const float top = g_textureOffsetY / texture->TextureHeight();
      const float bottom = (texture->Height() + g_textureOffsetY) / spark->texture->TextureHeight();
      spark->Render(true, bottom, top, left, right, false, true);
    }

    if (this->emitters[index]) {
      this->emitters[index]->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00704BB0
void CChargingPierceNewEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}
