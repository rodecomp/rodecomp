#include "Contents/Effects/Skills/MapChainEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B4A9E0
// FUNCTION: fn_18945
CMapChainEffect::CMapChainEffect()
    : CEffect(1024), particles(), chainModeA(2), chainModeB(2), chainModeC(2), chainModeD(2), chainModeE(2) {
}

// OFFSET: 0x00B4AB90
// FUNCTION: fn_18947
void CMapChainEffect::CreateChainParticles(int param) {
  int local8 = 0;
  for (int localC = 0; localC < 360; localC += 60, ++local8) {
    for (int i = 0; i < 21; ++i) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle != nullptr) {
        particle->rowIndex = local8;
        particle->effectId = this->id;
        particle->columnIndex = i;
        particle->groupIndex = 1;
        particle->speedParameter = 10.0f;
        particle->scale = 14.0f;
        particle->angle = static_cast<float>(localC);
        particle->phaseParameter = param;
        particle->alpha = 255;
        particle->angleParameter = 60;
        particle->animationParameter = -14.0f;
        particle->animationParameters[0] = 1.0f;
        particle->destinationBlend = 6;
        particle->InitParticleQuad(particle->vertices);
        particle->SetTexture("effect\\hero_005.tga");
        this->particles.push_back(particle);
      }
    }
  }

  int localC2 = 6;
  for (int local82 = 0; local82 < 360; local82 += 60, ++localC2) {
    for (int i = 0; i < 21; ++i) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle != nullptr) {
        particle->rowIndex = localC2;
        particle->effectId = this->id;
        particle->columnIndex = i;
        particle->groupIndex = 1;
        particle->speedParameter = 10.0f;
        particle->scale = 14.0f;
        particle->angle = static_cast<float>(local82);
        particle->phaseParameter = param;
        particle->alpha = 255;
        particle->angleParameter = 60;
        particle->animationParameter = -14.0f;
        particle->animationParameters[0] = 1.0f;
        particle->sourceBlend = 5;
        particle->destinationBlend = 7;
        particle->InitParticleQuad(particle->vertices);
        particle->SetTexture("effect\\hero_005.tga");
        this->particles.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B4AFA0
// FUNCTION: fn_18948
void CMapChainEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x00B4AFE0
// FUNCTION: fn_18949
void CMapChainEffect::Open() {
  this->CreateChainParticles(25);
  this->CreateChainParticles(50);
  this->CreateChainParticles(95);
  this->CreateChainParticles(120);
  this->CreateChainParticles(165);
}

// OFFSET: 0x00B4B0A0
// FUNCTION: fn_18950
void CMapChainEffect::ResetParticles() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->flags = 5;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    particle->speedParameter = this->areaSize;
    particle->scale = this->scaleParameter;
    particle->alpha = this->baseAlpha;
    particle->animationParameter = this->heights[0];
    particle->animationParameters[0] = this->heights[1];
    particle->sourceBlend = 5;
    particle->destinationBlend = 7;
    particle->InitParticleQuad(particle->vertices);
    particle->elapsedFrames = 0;
  }
}

// OFFSET: 0x00B4B130
// FUNCTION: fn_18951
bool CMapChainEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B4B140
// FUNCTION: fn_18952
void CMapChainEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      float angle = *static_cast<const float*>(particle->animationParameters) + particle->angle;
      if (angle >= 360.0f) {
        angle -= 360.0f;
      }
      particle->angle = angle;

      if (particle->rowIndex > 5) {
        int delta = -3;
        if (particle->elapsedFrames % 600 <= 250) {
          delta = 3;
        }
        int alpha = particle->alpha + delta;
        if (alpha >= 255) {
          alpha = 255;
        }
        particle->alpha = alpha > 0 ? alpha : 0;
      }

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
      particle->transform = rotation;

      const int segment = particle->columnIndex;
      const int index = static_cast<int>(static_cast<float>(static_cast<int>(particle->angleParameter / 20.0f) * segment) + particle->angle) % 360;

      MapChainPoint points[4];

      const int tilt = particle->phaseParameter;
      const float sinIndex = g_sinTable[index];
      const float cosIndex = g_cosTable[index];
      const float sinTilt = g_sinTable[tilt];
      const float cosTilt = g_cosTable[tilt];

      const float centerX = (sinTilt * particle->scale) * cosIndex;
      const float centerY = particle->scale * sinIndex;
      const float centerZ = (particle->scale * cosTilt) * cosIndex;
      const float sideX = particle->speedParameter * cosTilt;
      const float sideZ = -sinTilt * particle->speedParameter;

      const float v1x = (centerX - sideX) + 0.0f;
      const float v1y = (centerY - 0.0f) + particle->animationParameter;
      const float v1z = (centerZ - sideZ) + 0.0f;
      const float v3x = (sideX + centerX) + 0.0f;
      const float v3y = (centerY + 0.0f) + particle->animationParameter;
      const float v3z = (sideZ + centerZ) + 0.0f;

      particle->vertices[1].x = v1x;
      particle->vertices[1].y = v1y;
      particle->vertices[1].z = v1z;

      particle->vertices[3].x = v3x;
      particle->vertices[3].y = v3y;
      particle->vertices[3].z = v3z;

      if (segment != 0) {
        particle->vertices[0].x = this->corners[1].x;
        particle->vertices[0].y = this->corners[1].y;
        particle->vertices[0].z = this->corners[1].z;
        particle->vertices[2].x = this->corners[0].x;
        particle->vertices[2].y = this->corners[0].y;
        particle->vertices[2].z = this->corners[0].z;
      }

      this->corners[1].x = v1x;
      this->corners[1].y = v1y;
      this->corners[1].z = v1z;
      this->corners[0].x = v3x;
      this->corners[0].y = v3y;
      this->corners[0].z = v3z;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B4B740
// FUNCTION: fn_18953
void CMapChainEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->columnIndex > 0) {
      if (particle->rowIndex < 6) {
        particle->destinationBlend = 6;
      }

      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(particle->columnIndex - 1) / 21.0f;
      quad.right = static_cast<float>(particle->columnIndex) / 21.0f;
      quad.unused = true;
      quad.depth = true;
      particle->Render(quad);
    }
  }
}
