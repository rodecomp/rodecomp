#include "Contents/Effects/ElectricEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
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
    const float spin = static_cast<float>((std::max)(rand() % 11, 0)) * 0.01f;

    for (int segment = 0; segment < 20; ++segment) {
      for (int strand = 0; strand < 4; ++strand) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        particle->unknown_0004 = this->id;
        particle->unknown_000C = segment;
        particle->unknown_0010 = row;
        particle->unknown_0008 = strand;
        particle->SetTexture("effect\\elec1.tga");
        particle->unknown_00C0 = 0.1f;
        particle->alpha = 0;
        particle->unknown_004C = tilt;
        particle->scale = 0.0f;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = spin;
        particle->unknown_0050 = 0;
        InitParticleQuad(particle->vertices);
        this->bolts.push_back(particle);

        float* arc = &particle->unknown_0054;
        arc[20] = 9.0f;
        for (int index = 20; index > 0; --index) {
          float value = arc[index] + 9.0f;
          if (value > 180.0f) {
            value = 180.0f;
          }
          arc[index - 1] = value;
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
void CElectricEffect::Slot01() {
  this->unknown_002C = 300;
}

// OFFSET: 0x00B4EF40
// FUNCTION: fn_19005
bool CElectricEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B4EF50
// FUNCTION: fn_19006
void CElectricEffect::Slot03() {
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
    ++particle->unknown_00E8;
    int alpha = scale > 15.0f ? particle->alpha - 2 : particle->alpha;
    if (particle->unknown_00E8 <= 25) {
      alpha += 5 + (std::max)(rand() % 4, 0);
    }
    particle->scale = scale;
    particle->alpha = (std::max)(alpha, 0);

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);
    particle->transform = rotation;

    const int segment = particle->unknown_000C;
    const int strand = particle->unknown_0008;
    vector3d unused[2] = {};

    if (strand == 0) {
      const float* arc = &particle->unknown_0054;
      for (int index = 0; index < 2; ++index) {
        const float bend = arc[segment + index];
        const float radius = bend / 90.0f * particle->scale;
        this->path[index].x = radius * g_cosTable[particle->unknown_004C];
        this->path[index].y = -particle->scale * g_sinTable[static_cast<int>(bend)] * *reinterpret_cast<const float*>(&particle->unknown_00C4);
        this->path[index].z = radius * g_sinTable[particle->unknown_004C];

        const int jitterZ = rand() % 7;
        const int jitterY = rand() % 7;
        const int jitterX = rand() % 7;
        this->path[index].x += static_cast<float>((std::max)(jitterX, 0)) * 0.1f - 2.3f;
        this->path[index].y += static_cast<float>((std::max)(jitterY, 0)) * 0.1f - 0.3f;
        this->path[index].z += static_cast<float>((std::max)(jitterZ, 0)) * 0.1f - 0.3f;
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
      const float radius = particle->unknown_00C0;
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
void CElectricEffect::Slot04() {
  for (CParticle* bolt : this->bolts) {
    if (bolt && static_cast<unsigned>(bolt->unknown_000C - 1) <= 17 && bolt->unknown_0008 > 0) {
      const int frame = bolt->unknown_000C;
      const int alpha = bolt->alpha;
      const int faded = alpha - frame * 5;
      bolt->alpha = faded < 0 ? 0 : faded;
      bolt->SetColor(static_cast<short>(frame * 6), static_cast<short>(frame * 6), 255);
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = static_cast<float>(bolt->unknown_000C - 1) / 21.0f;
      quad.bottom = static_cast<float>(bolt->unknown_000C) / 21.0f;
      quad.left = 0.0f;
      quad.right = 1.0f;
      quad.unused = false;
      quad.depth = true;
      bolt->Render(quad);
      bolt->alpha = alpha;
    }
  }
}
