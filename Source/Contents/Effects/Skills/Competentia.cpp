#include "Contents/Effects/Skills/Competentia.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Skills/NyanggrassEffect.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"

namespace {
// INLINED
__forceinline bool IsParticleActive(const CParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// OFFSET: 0x007088B0
// FUNCTION: fn_07076
void CNyanggrassEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle != nullptr) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      static_cast<CEZ2STRParticle*>(particle)->DrawScreen(mode->camera->frame.view);
    }
  }
}

// OFFSET: 0x00708930
// FUNCTION: fn_07077
CCompetentia::CCompetentia() : CEffect(1851) {
}

// OFFSET: 0x00708A90
// FUNCTION: fn_07079
void CCompetentia::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00708AD0
// FUNCTION: fn_07080
void CCompetentia::Open() {
  for (__int64 i = 0; i < 10; ++i) {
    vector3d pos = this->positionVector;
    const vector2d point = RandomDiscPoint(pos.x, pos.z, 50);
    pos.x = point.x;
    pos.z = point.y;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    pos.y = mode->scene->terrain->Height(pos.x, pos.z);

    CParticle* particle = CreateEZ2STRParticle(&pos, 5);
    if (particle != nullptr) {
      static_cast<CEZ2STRParticle*>(particle)->Open("competentia\\competentia_cast_particle\\competentia_cast_particle.str");
      static_cast<CEZ2STRParticle*>(particle)->m_loop = true;
      this->particles.push_back(particle);
    }
  }

  this->AddEvent(0, "effect\\EF_BeginSpell.wav", 0, 0);
}

// OFFSET: 0x00708CB0
// FUNCTION: fn_07081
bool CCompetentia::IsAlive() {
  return !this->particles.empty();
}

// OFFSET: 0x00708CC0
// FUNCTION: fn_07082
void CCompetentia::OnProcess() {
  for (auto current = this->particles.begin(); current != this->particles.end();) {
    if (IsParticleActive(*current)) {
      CParticle* particle = *current;
      if (particle) {
        static_cast<CEZ2STRParticle*>(particle)->Process(0.0f, -80.0f, 0.0f);
      }
      ++current;
    } else {
      FreeParticle(*current);
      current = this->particles.erase(current);
    }
  }
}

// OFFSET: 0x00708DB0
// FUNCTION: fn_07083
void CCompetentia::Render() {
  for (auto current = this->particles.begin(); current != this->particles.end(); ++current) {
    CParticle* particle = *current;
    if (particle) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      static_cast<CEZ2STRParticle*>(particle)->DrawScreen(mode->camera->frame.view);
    }
  }
}

// OFFSET: 0x00708E30
// FUNCTION: fn_07084
void CCompetentia::SetLifeTime(int argument) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      static_cast<CEZ2STRParticle*>(particle)->m_lifeTime = argument;
    }
  }
}
