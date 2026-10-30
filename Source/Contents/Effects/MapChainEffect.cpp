#include "Contents/Effects/MapChainEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B4A9E0
// FUNCTION: fn_18945
CMapChainEffect::CMapChainEffect()
    : CEffect(1024), particles(), unknown_00F0(2), unknown_00F4(2), unknown_00F8(2), unknown_00FC(2), unknown_0100(2) {
}

// OFFSET: 0x00B4B130
// FUNCTION: fn_18951
bool CMapChainEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B4B140
// FUNCTION: fn_18952
void CMapChainEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      float& angle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      const float speed = *reinterpret_cast<const float*>(particle->unknown_0058);
      angle += speed;
      if (angle >= 360.0f) {
        angle -= 360.0f;
      }

      if (particle->unknown_0010 > 5) {
        int delta = -3;
        if (particle->unknown_00E8 % 600 <= 250) {
          delta = 3;
        }
        particle->alpha += delta;
        if (particle->alpha > 255) {
          particle->alpha = 255;
        } else if (particle->alpha < 0) {
          particle->alpha = 0;
        }
      }

      const CView* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(view->camera.yaw / -180.0f * 3.141592f);
      particle->transform = rotation;

      const int segment = particle->unknown_000C;
      const int index = static_cast<int>(static_cast<float>(static_cast<int>(particle->unknown_0050 / 20.0f) * segment) + angle) % 360;

      vector3d points[4] = {};

      const int tilt = particle->unknown_004C;
      const float sinIndex = g_sinTable[index];
      const float cosIndex = g_cosTable[index];
      const float sinTilt = g_sinTable[tilt];
      const float cosTilt = g_cosTable[tilt];

      const float termX = (sinTilt * particle->scale) * cosIndex;
      const float termZ = (particle->scale * cosTilt) * cosIndex;
      const float widthCos = particle->unknown_00C0 * cosTilt;
      const float widthSin = particle->unknown_00C0 * sinTilt;

      const float v1x = (termX - widthCos) + 0.0f;
      const float v3x = (termX + widthCos) + 0.0f;
      const float v1y = (particle->scale * sinIndex - 0.0f) + particle->unknown_0054;
      const float v3y = (particle->scale * sinIndex + 0.0f) + particle->unknown_0054;
      const float v1z = (termZ + widthSin) + 0.0f;
      const float v3z = (termZ - widthSin) + 0.0f;

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

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B4B740
// FUNCTION: fn_18953
void CMapChainEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->unknown_000C > 0) {
      if (particle->unknown_0010 < 6) {
        particle->destinationBlend = 6;
      }

      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(particle->unknown_000C - 1) / 21.0f;
      quad.right = static_cast<float>(particle->unknown_000C) / 21.0f;
      quad.unused = true;
      quad.depth = true;
      particle->Render(quad);
    }
  }
}
