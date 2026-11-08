#include "Contents/Effects/BlastForge.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <windows.h>
#include <timeapi.h>

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

  this->blastParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->blastBottomParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));

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
  this->loopParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->loopBottomParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));

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
    this->loopParticle->position.x = this->position[0];
    this->loopParticle->position.y = this->position[1];
    this->loopParticle->position.z = this->position[2];

    const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
    const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

    float px = this->loopParticle->position.x;
    float py = this->loopParticle->position.y;
    float pz = this->loopParticle->position.z;

    float vx = py * view.values[3] + px * view.values[0] + pz * view.values[6] + view.values[9];
    float vy = (py * view.values[4] + px * view.values[1] + pz * view.values[7] + view.values[10]) - 8.0f;
    float vz = py * view.values[5] + px * view.values[2] + pz * view.values[8] + view.values[11];

    this->loopParticle->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
    this->loopParticle->position.y = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
    this->loopParticle->position.z = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];

    this->loopParticle->Process(0.0f, 0.0f, 0.0f);
  }

  if (this->loopBottomParticle) {
    this->loopBottomParticle->position.x = this->position[0];
    this->loopBottomParticle->position.y = this->position[1];
    this->loopBottomParticle->position.z = this->position[2];

    const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
    const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

    float px = this->loopBottomParticle->position.x;
    float py = this->loopBottomParticle->position.y;
    float pz = this->loopBottomParticle->position.z;

    float vx = py * view.values[3] + px * view.values[0] + pz * view.values[6] + view.values[9];
    float vy = (py * view.values[4] + px * view.values[1] + pz * view.values[7] + view.values[10]) - 8.0f;
    float vz = py * view.values[5] + px * view.values[2] + pz * view.values[8] + view.values[11];

    this->loopBottomParticle->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
    this->loopBottomParticle->position.y = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
    this->loopBottomParticle->position.z = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];

    this->loopBottomParticle->Process(0.0f, 0.0f, 0.0f);
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
      this->blastParticle->position.x = this->position[0];
      this->blastParticle->position.y = this->position[1];
      this->blastParticle->position.z = this->position[2];

      const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
      const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

      float px = this->blastParticle->position.x;
      float py = this->blastParticle->position.y;
      float pz = this->blastParticle->position.z;

      float vx = py * view.values[3] + px * view.values[0] + pz * view.values[6] + view.values[9];
      float vy = (py * view.values[4] + px * view.values[1] + pz * view.values[7] + view.values[10]) - 8.0f;
      float vz = py * view.values[5] + px * view.values[2] + pz * view.values[8] + view.values[11];

      this->blastParticle->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
      this->blastParticle->position.y = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
      this->blastParticle->position.z = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];

      this->blastParticle->Process(0.0f, 0.0f, 0.0f);

      if (this->blastBottomParticle) {
        this->blastBottomParticle->position.x = this->position[0];
        this->blastBottomParticle->position.y = this->position[1];
        this->blastBottomParticle->position.z = this->position[2];

        const matrix& view2 = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
        const matrix& inverse2 = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

        float px2 = this->blastBottomParticle->position.x;
        float py2 = this->blastBottomParticle->position.y;
        float pz2 = this->blastBottomParticle->position.z;

        float vx2 = py2 * view2.values[3] + px2 * view2.values[0] + pz2 * view2.values[6] + view2.values[9];
        float vy2 = (py2 * view2.values[4] + px2 * view2.values[1] + pz2 * view2.values[7] + view2.values[10]) - 8.0f;
        float vz2 = py2 * view2.values[5] + px2 * view2.values[2] + pz2 * view2.values[8] + view2.values[11];

        this->blastBottomParticle->position.x = vy2 * inverse2.values[3] + vx2 * inverse2.values[0] + vz2 * inverse2.values[6] + inverse2.values[9];
        this->blastBottomParticle->position.y = vy2 * inverse2.values[4] + vx2 * inverse2.values[1] + vz2 * inverse2.values[7] + inverse2.values[10];
        this->blastBottomParticle->position.z = vy2 * inverse2.values[5] + vx2 * inverse2.values[2] + vz2 * inverse2.values[8] + inverse2.values[11];

        this->blastBottomParticle->Process(0.0f, 0.0f, 0.0f);
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
