#include "Contents/Effects/Skills/BlastForge.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <windows.h>
#include <timeapi.h>

// INLINED
static __forceinline vector3d TransformPoint(const matrix& transform, const vector3d& point) {
  vector3d result;
  result.x = point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9];
  result.y = point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10];
  result.z = point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11];
  return result;
}

// INLINED
static __forceinline void PlaceParticle(CEZ2STRParticle* particle, const float* position) {
  particle->position.x = position[0];
  particle->position.y = position[1];
  particle->position.z = position[2];

  const vector3d pos = particle->position;
  const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
  const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

  vector3d viewPos = TransformPoint(view, pos);
  viewPos.y -= 8.0f;
  const vector3d worldPos = TransformPoint(inverse, viewPos);
  particle->position.x = worldPos.x;
  particle->position.y = worldPos.y;
  particle->position.z = worldPos.z;

  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x006F7360
// FUNCTION: fn_06858
CBlastForge::CBlastForge() : CEffect(-1), openToggle(0), startTime(timeGetTime()) {
  this->loopParticle = nullptr;
  this->loopBottomParticle = nullptr;
  this->blastParticle = nullptr;
  this->blastBottomParticle = nullptr;
}

// OFFSET: 0x006F7660
// FUNCTION: fn_06860
void CBlastForge::RestartBlastParticles() {
  if (this->blastParticle) {
    FreeParticle(this->blastParticle);
    this->blastParticle = nullptr;
  }
  if (this->blastBottomParticle) {
    FreeParticle(this->blastBottomParticle);
    this->blastBottomParticle = nullptr;
  }

  this->blastParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  this->blastBottomParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));

  if (this->blastParticle && this->blastBottomParticle) {
    this->blastParticle->Open("blast_forge\\blast_forge\\blast_forge.str");
    this->blastBottomParticle->Open("blast_forge\\blast_forge\\blast_forge_bottom.str");
  }
}

// OFFSET: 0x006F7820
// FUNCTION: fn_06861
void CBlastForge::Close() {
  FreeParticle(this->loopParticle);
  FreeParticle(this->loopBottomParticle);
  if (this->blastParticle) {
    FreeParticle(this->blastParticle);
  }
  if (this->blastBottomParticle) {
    FreeParticle(this->blastBottomParticle);
  }
}

// OFFSET: 0x006F7870
// FUNCTION: fn_06862
void CBlastForge::Open() {
  this->retained = 1;
  this->openToggle = (this->openToggle == 0);
  this->loopParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  this->loopBottomParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));

  if (this->loopParticle && this->loopBottomParticle) {
    this->loopParticle->Open("blast_forge\\blast_forge\\blast_forge_5s.str");
    this->loopBottomParticle->Open("blast_forge\\blast_forge\\blast_forge_5s_bottom.str");
    this->loopParticle->m_loop = true;
    this->loopBottomParticle->m_loop = true;
  }

  this->RestartBlastParticles();
}

// OFFSET: 0x006F7A20
// FUNCTION: fn_06863
bool CBlastForge::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x006F7A30
// FUNCTION: fn_06864
void CBlastForge::OnProcess() {
  if (this->loopParticle) {
    PlaceParticle(this->loopParticle, this->position);
  }
  if (this->loopBottomParticle) {
    PlaceParticle(this->loopBottomParticle, this->position);
  }

  this->ProcessBlastParticles();
}

// OFFSET: 0x006F7EA0
// FUNCTION: fn_06865
void CBlastForge::Render() {
  if (!this->blastParticle) {
    if (this->loopParticle) {
      this->loopParticle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
    if (this->loopBottomParticle) {
      this->loopBottomParticle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }

  if (this->blastParticle) {
    this->blastParticle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
  if (this->blastBottomParticle) {
    this->blastBottomParticle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x006F7F80
// FUNCTION: fn_06866
void CBlastForge::ProcessBlastParticles() {
  if (this->blastParticle && this->blastBottomParticle) {
    if (this->blastParticle->life == 0.0f || static_cast<float>(this->blastParticle->elapsedFrames) < this->blastParticle->life * 60.0f) {
      PlaceParticle(this->blastParticle, this->position);
      if (this->blastBottomParticle) {
        PlaceParticle(this->blastBottomParticle, this->position);
      }
    } else {
      FreeParticle(this->blastParticle);
      FreeParticle(this->blastBottomParticle);
      this->blastParticle = nullptr;
      this->blastBottomParticle = nullptr;
    }
  }

  const DWORD now = timeGetTime();
  if (static_cast<std::int64_t>(now) - this->startTime >= 1000) {
    this->startTime = timeGetTime();
    this->RestartBlastParticles();
  }
}
