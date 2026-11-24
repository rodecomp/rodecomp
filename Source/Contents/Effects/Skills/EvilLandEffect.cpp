#include "Contents/Effects/Skills/EvilLandEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Skills/NyanggrassEffect.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

// OFFSET: 0x006FE300
// FUNCTION: fn_06944
CEvilLandEffect::CEvilLandEffect() : CEffect(1555), particles() {
}

// OFFSET: 0x006FE410
// FUNCTION: fn_46238
__declspec(noinline) CEvilLandEffect::~CEvilLandEffect() {
}

// OFFSET: 0x006FE4F0
// FUNCTION: fn_06947
void CEvilLandEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x006FE530
// FUNCTION: fn_06948
void CEvilLandEffect::Open() {
  CTexture* texture = g_texMgr.Load("effect\\EVILLAND\\demonground.tga", 0, false, true, false);
  const float textureEffectLevel = static_cast<float>(this->effectLevel);
  const float textureStepU = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth()) / textureEffectLevel;
  const float textureStepV = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight()) / textureEffectLevel;

  CTexture* alpha = g_texMgr.Load("effect\\EVILLAND\\demonground_alpha_translucent.tga", 0, false, true, false);
  const int effectLevel = this->effectLevel;
  const float alphaEffectLevel = static_cast<float>(effectLevel);
  const float alphaStepU = static_cast<float>(alpha->ImageWidth()) / static_cast<float>(alpha->TextureWidth()) / alphaEffectLevel;
  const float alphaStepV = static_cast<float>(alpha->ImageHeight()) / static_cast<float>(alpha->TextureHeight()) / alphaEffectLevel;

  for (int row = 0; row < effectLevel; ++row) {
    for (int column = 0; column < effectLevel; ++column) {
      CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(CreateMultiTexParticle(&this->positionVector, -1));
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->rowIndex = row;
      particle->columnIndex = column;
      particle->groupIndex = 1;
      particle->texture1 = texture;
      particle->texture2 = alpha;

      const float left = static_cast<float>(column);
      const float top = static_cast<float>(row);
      const float right = static_cast<float>(column + 1);
      const float bottom = static_cast<float>(row + 1);

      particle->uv1[0][0] = left * textureStepU;
      particle->uv1[0][1] = top * textureStepV;
      particle->uv2[0][0] = left * alphaStepU;
      particle->uv2[0][1] = top * alphaStepV;

      particle->uv1[1][0] = right * textureStepU;
      particle->uv1[1][1] = top * textureStepV;
      particle->uv2[1][0] = right * alphaStepU;
      particle->uv2[1][1] = top * alphaStepV;

      particle->uv1[2][0] = left * textureStepU;
      particle->uv1[2][1] = bottom * textureStepV;
      particle->uv2[2][0] = left * alphaStepU;
      particle->uv2[2][1] = bottom * alphaStepV;

      particle->uv1[3][0] = right * textureStepU;
      particle->uv1[3][1] = bottom * textureStepV;
      particle->uv2[3][0] = right * alphaStepU;
      particle->uv2[3][1] = bottom * alphaStepV;

      particle->AllocateVertices(4);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x006FE830
// FUNCTION: fn_06949
bool CEvilLandEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x006FE880
// FUNCTION: fn_46239
__forceinline void CEvilLandEffect::ScrollParticleTexture(CMultiTexParticle* particle) {
  if (particle) {
    particle->offsetV += 0.001f;
  }
}

// OFFSET: 0x006FE840
// FUNCTION: fn_06950
void CEvilLandEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->ScrollParticleTexture(*it);
  }
}

// OFFSET: 0x006FE950
// FUNCTION: fn_46240
__forceinline void CEvilLandEffect::RenderParticle(CMultiTexParticle* particle) {
  if (particle) {
    CGameMode* gameMode = g_modeMgr.GetGameMode();
    if (gameMode) {
      int column = 0;
      int row = 0;
      gameMode->WorldCell(this->position[0], this->position[2], column, row);
      const int half = this->effectLevel / 2;
      particle->Render(particle->columnIndex - half + column, particle->rowIndex - half + row);
    }
  }
}

// OFFSET: 0x006FE8B0
// FUNCTION: fn_06951
void CEvilLandEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderParticle(*it);
  }
}
