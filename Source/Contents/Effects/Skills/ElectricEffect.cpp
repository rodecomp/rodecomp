#include "Contents/Effects/Skills/ElectricEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B4E890
// FUNCTION: fn_19002
void CElectricEffect::AddBolt(int tilt) {
  for (int row = 0; row < 4; ++row) {
    const int roll = rand() % 11;
    const float spin = static_cast<float>(roll >= 0 ? roll : 0) * 0.01f;

    for (int segment = 0; segment < 20; ++segment) {
      for (int strand = 0; strand < 4; ++strand) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        particle->effectId = this->id;
        particle->columnIndex = segment;
        particle->rowIndex = row;
        particle->groupIndex = strand;
        particle->SetTexture("effect\\elec1.tga");
        particle->speedParameter = 0.1f;
        particle->alpha = 0;
        particle->phaseParameter = tilt;
        particle->scale = 0.0f;
        *reinterpret_cast<float*>(&particle->angleBits) = spin;
        particle->angleParameter = 0;
        particle->InitParticleQuad(particle->vertices);
        this->bolts.push_back(particle);

        float* arc = &particle->animationParameter;
        arc[20] = 9.0f;
        for (int index = 20; index > 0; --index) {
          if (arc[index] + 9.0f > 180.0f) {
            arc[index - 1] = 180.0f;
          } else {
            arc[index - 1] = arc[index] + 9.0f;
          }
        }
      }
    }

    tilt += 45;
  }
}

// OFFSET: 0x00B4EDB0
// FUNCTION: fn_19003
void CElectricEffect::Close() {
  for (auto it = this->bolts.begin(); it != this->bolts.end(); ++it) {
    FreeParticle(*it);
  }

  this->bolts.clear();
}

// OFFSET: 0x00B4EE80
// FUNCTION: fn_19004
void CElectricEffect::Open() {
  this->effectLifetime = 300;
}

// OFFSET: 0x00B4EF40
// FUNCTION: fn_19005
bool CElectricEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B4EF50
// FUNCTION: fn_19006
void CElectricEffect::OnProcess() {
  if (this->tick >= 20 && this->tick < 24) {
    this->AddBolt(0);
    this->AddBolt(180);
  }

  if (this->tick == 25) {
    CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    const vector3d& player = reinterpret_cast<const CRenderObject*>(world->player)->render.position;
    g_soundMgr->PlayWave("effect\\T_전기.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);
  }

  ++this->tick;

  for (auto it = this->bolts.begin(); it != this->bolts.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    const float scale = particle->scale + 0.4f;
    ++particle->elapsedFrames;
    int alpha = scale > 15.0f ? particle->alpha - 2 : particle->alpha;
    if (particle->elapsedFrames <= 25) {
      const int flicker = rand() % 4;
      alpha += 5 + (flicker >= 0 ? flicker : 0);
    }
    particle->scale = scale;
    particle->alpha = alpha >= 0 ? alpha : 0;

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;

    const int segment = particle->columnIndex;
    const int strand = particle->groupIndex;
    vector3d unused[2] = {};

    if (strand == 0) {
      const float* arc = &particle->animationParameter;
      for (int index = 0; index < 2; ++index) {
        const float bend = arc[segment + index];
        const float radius = bend / 90.0f * particle->scale;
        this->path[index].x = radius * g_cosTable[particle->phaseParameter];
        this->path[index].y = -particle->scale * g_sinTable[static_cast<int>(bend)] * *reinterpret_cast<const float*>(&particle->angleBits);
        this->path[index].z = radius * g_sinTable[particle->phaseParameter];

        const int jitterZ = rand() % 7;
        const int jitterY = rand() % 7;
        const int jitterX = rand() % 7;
        this->path[index].x += static_cast<float>(jitterX >= 0 ? jitterX : 0) * 0.1f - 2.3f;
        this->path[index].y += static_cast<float>(jitterY >= 0 ? jitterY : 0) * 0.1f - 0.3f;
        this->path[index].z += static_cast<float>(jitterZ >= 0 ? jitterZ : 0) * 0.1f - 0.3f;
      }

      const float dx = this->path[0].x - this->path[1].x;
      const float dz = this->path[0].z - this->path[1].z;
      const float length = SquareRoot(dx * dx + dz * dz);
      const float nx = dx / length;
      const float nz = dz / length;

      short heading = 180;
      for (short step = 0; step <= 180; ++step) {
        if (nx >= g_cosTable[step]) {
          heading = nz < 0.0f ? static_cast<short>(360 - step) : step;
          break;
        }
      }

      this->angle = static_cast<short>((heading + 90) % 360);
    }

    if (segment >= 1 && segment <= 18) {
      const float radius = particle->speedParameter;
      const float horizontal = radius * g_cosTable[strand * 120];
      const float vertical = radius * g_sinTable[strand * 120];
      const float offsetX = horizontal * g_cosTable[this->angle];
      const float offsetZ = horizontal * g_sinTable[this->angle];

      const vector3d first = {offsetX + this->path[0].x, vertical + this->path[0].y, offsetZ + this->path[0].z};
      const vector3d second = {offsetX + this->path[1].x, vertical + this->path[1].y, offsetZ + this->path[1].z};

      particle->vertices[1].x = first.x;
      particle->vertices[1].y = first.y;
      particle->vertices[1].z = first.z;
      particle->vertices[3].x = second.x;
      particle->vertices[3].y = second.y;
      particle->vertices[3].z = second.z;

      if (strand != 0) {
        particle->vertices[0].x = this->lastEdge[0].x;
        particle->vertices[0].y = this->lastEdge[0].y;
        particle->vertices[0].z = this->lastEdge[0].z;
        particle->vertices[2].x = this->lastEdge[1].x;
        particle->vertices[2].y = this->lastEdge[1].y;
        particle->vertices[2].z = this->lastEdge[1].z;
      }

      this->lastEdge[0] = first;
      this->lastEdge[1] = second;
    }
  }
}

// OFFSET: 0x00B4F930
// FUNCTION: fn_19007
void CElectricEffect::Render() {
  for (auto it = this->bolts.begin(); it != this->bolts.end(); ++it) {
    CParticle* bolt = *it;
    if (bolt && static_cast<unsigned>(bolt->columnIndex - 1) <= 17 && bolt->groupIndex > 0) {
      const int frame = bolt->columnIndex;
      const int alpha = bolt->alpha;
      const int faded = alpha - frame * 5;
      bolt->alpha = faded >= 0 ? faded : 0;
      bolt->SetColor(static_cast<short>(frame * 6), static_cast<short>(frame * 6), 255);
      ParticleQuad quad;
      quad.billboard = false;
      quad.unused = false;
      quad.depth = true;
      quad.left = 0.0f;
      quad.right = 1.0f;
      quad.top = static_cast<float>(bolt->columnIndex - 1) / 21.0f;
      quad.bottom = static_cast<float>(bolt->columnIndex) / 21.0f;
      bolt->Render(quad);
      bolt->alpha = alpha;
    }
  }
}
