#include "Contents/Effects/Competentia.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x007088B0
// FUNCTION: fn_07076
void CCompetentia::Slot04() {
  for (CParticle* particle : this->particles) {
    if (particle != nullptr) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      static_cast<CEZ2STRParticle*>(particle)->DrawBillboards(mode->camera->frame.view);
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
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00708AD0
// FUNCTION: fn_07080
void CCompetentia::Slot01() {
  for (int i = 0; i < 10; ++i) {
    const vector2d point = RandomDiscPoint(this->position[0], this->position[2], 50);
    vector3d pos;
    pos.x = point.x;
    pos.z = point.y;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    pos.y = mode->scene->terrain->Grid().Height(pos.x, pos.z);

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
bool CCompetentia::Slot06() {
  return !this->particles.empty();
}

// OFFSET: 0x00708E30
// FUNCTION: fn_07084
void CCompetentia::Slot14(int argument) {
  for (CParticle* particle : this->particles) {
    if (particle) {
      static_cast<CEZ2STRParticle*>(particle)->m_lifeTime = argument;
    }
  }
}
