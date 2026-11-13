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
  const float textureStepU = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth()) / static_cast<float>(this->effectLevel);
  const float textureStepV = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight()) / static_cast<float>(this->effectLevel);

  CTexture* alpha = g_texMgr.Load("effect\\EVILLAND\\demonground_alpha_translucent.tga", 0, false, true, false);
  const float alphaStepU = static_cast<float>(alpha->ImageWidth()) / static_cast<float>(alpha->TextureWidth()) / static_cast<float>(this->effectLevel);
  const float alphaStepV = static_cast<float>(alpha->ImageHeight()) / static_cast<float>(alpha->TextureHeight()) / static_cast<float>(this->effectLevel);

  for (int row = 0; row < this->effectLevel; ++row) {
    for (int column = 0; column < this->effectLevel; ++column) {
      CParticle* created = CreateMultiTexParticle(&this->positionVector, -1);
      if (!created) {
        continue;
      }

      CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(created);

      particle->effectId = this->id;
      particle->rowIndex = row;
      particle->columnIndex = column;
      particle->groupIndex = 1;
      particle->texture1 = texture;
      particle->texture2 = alpha;

      particle->uv1[0][0] = static_cast<float>(column) * textureStepU;
      particle->uv1[0][1] = static_cast<float>(row) * textureStepV;
      particle->uv2[0][0] = static_cast<float>(column) * alphaStepU;
      particle->uv2[0][1] = static_cast<float>(row) * alphaStepV;

      particle->uv1[1][0] = static_cast<float>(column + 1) * textureStepU;
      particle->uv1[1][1] = static_cast<float>(row) * textureStepV;
      particle->uv2[1][0] = static_cast<float>(column + 1) * alphaStepU;
      particle->uv2[1][1] = static_cast<float>(row) * alphaStepV;

      particle->uv1[2][0] = static_cast<float>(column) * textureStepU;
      particle->uv1[2][1] = static_cast<float>(row + 1) * textureStepV;
      particle->uv2[2][0] = static_cast<float>(column) * alphaStepU;
      particle->uv2[2][1] = static_cast<float>(row + 1) * alphaStepV;

      particle->uv1[3][0] = static_cast<float>(column + 1) * textureStepU;
      particle->uv1[3][1] = static_cast<float>(row + 1) * textureStepV;
      particle->uv2[3][0] = static_cast<float>(column + 1) * alphaStepU;
      particle->uv2[3][1] = static_cast<float>(row + 1) * alphaStepV;

      particle->AllocateVertices(4);
      this->particles.push_back(created);
    }
  }
}

// OFFSET: 0x006FE830
// FUNCTION: fn_06949
bool CEvilLandEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x006FE840
// FUNCTION: fn_06950
void CEvilLandEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(*it)) {
      particle->offsetV += 0.001f;
    }
  }
}

// OFFSET: 0x006FE8B0
// FUNCTION: fn_06951
void CEvilLandEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(*it);
    if (particle) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (gameMode) {
        int column = 0;
        int row = 0;
        gameMode->WorldCell(this->position[0], this->position[2], column, row);
        const int half = this->effectLevel / 2;
        particle->Render(particle->columnIndex - half + column, particle->rowIndex - half + row);
      }
    }
  }
}
