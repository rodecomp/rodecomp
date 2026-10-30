#include "Contents/Effects/EvilLandEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/NyanggrassEffect.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

// OFFSET: 0x006FE300
// FUNCTION: fn_06944
CEvilLandEffect::CEvilLandEffect() : CEffect(1555), particles() {
}

// OFFSET: 0x006FE4F0
// FUNCTION: fn_06947
void CEvilLandEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x006FE530
// FUNCTION: fn_06948
void CEvilLandEffect::Slot01() {
  CTexture* texture = g_texMgr.Load("effect\\EVILLAND\\demonground.tga", 0, false, true, false);
  const float textureStepU = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth()) / static_cast<float>(this->unknown_00B0);
  const float textureStepV = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight()) / static_cast<float>(this->unknown_00B0);

  CTexture* alpha = g_texMgr.Load("effect\\EVILLAND\\demonground_alpha_translucent.tga", 0, false, true, false);
  const float alphaStepU = static_cast<float>(alpha->ImageWidth()) / static_cast<float>(alpha->TextureWidth()) / static_cast<float>(this->unknown_00B0);
  const float alphaStepV = static_cast<float>(alpha->ImageHeight()) / static_cast<float>(alpha->TextureHeight()) / static_cast<float>(this->unknown_00B0);

  for (int row = 0; row < this->unknown_00B0; ++row) {
    for (int column = 0; column < this->unknown_00B0; ++column) {
      CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(CreateMultiTexParticle(reinterpret_cast<const vector3d*>(this->position), -1));
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_0010 = row;
      particle->unknown_000C = column;
      particle->unknown_0008 = 1;
      particle->texture1 = texture;
      particle->texture2 = alpha;

      particle->uv1[0][0] = static_cast<float>(column) * textureStepU;
      particle->uv1[0][1] = static_cast<float>(row) * textureStepV;
      particle->uv1[1][0] = static_cast<float>(column + 1) * textureStepU;
      particle->uv1[1][1] = static_cast<float>(row) * textureStepV;
      particle->uv1[2][0] = static_cast<float>(column) * textureStepU;
      particle->uv1[2][1] = static_cast<float>(row + 1) * textureStepV;
      particle->uv1[3][0] = static_cast<float>(column + 1) * textureStepU;
      particle->uv1[3][1] = static_cast<float>(row + 1) * textureStepV;

      particle->uv2[0][0] = static_cast<float>(column) * alphaStepU;
      particle->uv2[0][1] = static_cast<float>(row) * alphaStepV;
      particle->uv2[1][0] = static_cast<float>(column + 1) * alphaStepU;
      particle->uv2[1][1] = static_cast<float>(row) * alphaStepV;
      particle->uv2[2][0] = static_cast<float>(column) * alphaStepU;
      particle->uv2[2][1] = static_cast<float>(row + 1) * alphaStepV;
      particle->uv2[3][0] = static_cast<float>(column + 1) * alphaStepU;
      particle->uv2[3][1] = static_cast<float>(row + 1) * alphaStepV;

      particle->AllocateVertices(4);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x006FE830
// FUNCTION: fn_06949
bool CEvilLandEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006FE840
// FUNCTION: fn_06950
void CEvilLandEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(*it)) {
      particle->offsetV += 0.001f;
    }
  }
}

// OFFSET: 0x006FE8B0
// FUNCTION: fn_06951
void CEvilLandEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CMultiTexParticle* particle = static_cast<CMultiTexParticle*>(*it);
    if (particle) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (gameMode) {
        int column = 0;
        int row = 0;
        gameMode->WorldCell(this->position[0], this->position[2], column, row);
        const int half = this->unknown_00B0 / 2;
        particle->Render(particle->unknown_000C - half + column, particle->unknown_0010 - half + row);
      }
    }
  }
}
