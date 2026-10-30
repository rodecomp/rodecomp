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
CBlastForge::CBlastForge() : CEffect(-1), unknown_00DC(0), startTime(timeGetTime()) {
  this->unknown_00CC = nullptr;
  this->unknown_00D0 = nullptr;
  this->unknown_00D4 = nullptr;
  this->unknown_00D8 = nullptr;
}

// OFFSET: 0x006F7660
// FUNCTION: fn_06860
void CBlastForge::sub_6F7660() {
  if (this->unknown_00D4) {
    FreeParticle(this->unknown_00D4);
    this->unknown_00D4 = nullptr;
  }
  if (this->unknown_00D8) {
    FreeParticle(this->unknown_00D8);
    this->unknown_00D8 = nullptr;
  }

  this->unknown_00D4 = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->unknown_00D8 = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));

  if (this->unknown_00D4 && this->unknown_00D8) {
    this->unknown_00D4->Open("blast_forge\\blast_forge\\blast_forge.str");
    this->unknown_00D8->Open("blast_forge\\blast_forge\\blast_forge_bottom.str");
  }
}

// OFFSET: 0x006F7820
// FUNCTION: fn_06861
void CBlastForge::Close() {
  FreeParticle(this->unknown_00CC);
  FreeParticle(this->unknown_00D0);
  if (this->unknown_00D4) {
    FreeParticle(this->unknown_00D4);
  }
  if (this->unknown_00D8) {
    FreeParticle(this->unknown_00D8);
  }
}

// OFFSET: 0x006F7870
// FUNCTION: fn_06862
void CBlastForge::Slot01() {
  this->retained = 1;
  this->unknown_00DC = (this->unknown_00DC == 0);
  this->unknown_00CC = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->unknown_00D0 = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));

  if (this->unknown_00CC && this->unknown_00D0) {
    this->unknown_00CC->Open("blast_forge\\blast_forge\\blast_forge_5s.str");
    this->unknown_00D0->Open("blast_forge\\blast_forge\\blast_forge_5s_bottom.str");
    this->unknown_00CC->m_loop = true;
    this->unknown_00D0->m_loop = true;
  }

  this->sub_6F7660();
}

// OFFSET: 0x006F7A20
// FUNCTION: fn_06863
bool CBlastForge::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006F7A30
// FUNCTION: fn_06864
void CBlastForge::Slot03() {
  if (this->unknown_00CC) {
    this->unknown_00CC->position.x = this->position[0];
    this->unknown_00CC->position.y = this->position[1];
    this->unknown_00CC->position.z = this->position[2];

    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const matrix& view = gameMode->camera->frame.view;
    const matrix& inverse = gameMode->camera->frame.inverse;

    float px = this->unknown_00CC->position.x;
    float py = this->unknown_00CC->position.y;
    float pz = this->unknown_00CC->position.z;

    float vx = py * view.values[3] + px * view.values[0] + pz * view.values[6] + view.values[9];
    float vy = (py * view.values[4] + px * view.values[1] + pz * view.values[7] + view.values[10]) - 8.0f;
    float vz = py * view.values[5] + px * view.values[2] + pz * view.values[8] + view.values[11];

    this->unknown_00CC->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
    this->unknown_00CC->position.y = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
    this->unknown_00CC->position.z = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];

    this->unknown_00CC->Process(0.0f, 0.0f, 0.0f);
  }

  if (this->unknown_00D0) {
    this->unknown_00D0->position.x = this->position[0];
    this->unknown_00D0->position.y = this->position[1];
    this->unknown_00D0->position.z = this->position[2];

    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const matrix& view = gameMode->camera->frame.view;
    const matrix& inverse = gameMode->camera->frame.inverse;

    float px = this->unknown_00D0->position.x;
    float py = this->unknown_00D0->position.y;
    float pz = this->unknown_00D0->position.z;

    float vx = py * view.values[3] + px * view.values[0] + pz * view.values[6] + view.values[9];
    float vy = (py * view.values[4] + px * view.values[1] + pz * view.values[7] + view.values[10]) - 8.0f;
    float vz = py * view.values[5] + px * view.values[2] + pz * view.values[8] + view.values[11];

    this->unknown_00D0->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
    this->unknown_00D0->position.y = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
    this->unknown_00D0->position.z = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];

    this->unknown_00D0->Process(0.0f, 0.0f, 0.0f);
  }

  this->sub_6F7F80();
}

// OFFSET: 0x006F7EA0
// FUNCTION: fn_06865
void CBlastForge::Slot04() {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const matrix& view = gameMode->camera->frame.view;

  if (!this->unknown_00D4) {
    if (this->unknown_00CC) {
      this->unknown_00CC->DrawScreen(view);
    }
    if (this->unknown_00D0) {
      this->unknown_00D0->DrawScreen(view);
    }
  }

  if (this->unknown_00D4) {
    this->unknown_00D4->DrawScreen(view);
  }
  if (this->unknown_00D8) {
    this->unknown_00D8->DrawScreen(view);
  }
}

// OFFSET: 0x006F7F80
// FUNCTION: fn_06866
void CBlastForge::sub_6F7F80() {
  if (this->unknown_00D4 && this->unknown_00D8) {
    if (this->unknown_00D4->life == 0.0f || static_cast<float>(this->unknown_00D4->unknown_00E8) < this->unknown_00D4->life * 60.0f) {
      this->unknown_00D4->position.x = this->position[0];
      this->unknown_00D4->position.y = this->position[1];
      this->unknown_00D4->position.z = this->position[2];

      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const matrix& view = gameMode->camera->frame.view;
      const matrix& inverse = gameMode->camera->frame.inverse;

      float px = this->unknown_00D4->position.x;
      float py = this->unknown_00D4->position.y;
      float pz = this->unknown_00D4->position.z;

      float vx = py * view.values[3] + px * view.values[0] + pz * view.values[6] + view.values[9];
      float vy = (py * view.values[4] + px * view.values[1] + pz * view.values[7] + view.values[10]) - 8.0f;
      float vz = py * view.values[5] + px * view.values[2] + pz * view.values[8] + view.values[11];

      this->unknown_00D4->position.x = vy * inverse.values[3] + vx * inverse.values[0] + vz * inverse.values[6] + inverse.values[9];
      this->unknown_00D4->position.y = vy * inverse.values[4] + vx * inverse.values[1] + vz * inverse.values[7] + inverse.values[10];
      this->unknown_00D4->position.z = vy * inverse.values[5] + vx * inverse.values[2] + vz * inverse.values[8] + inverse.values[11];

      this->unknown_00D4->Process(0.0f, 0.0f, 0.0f);

      if (this->unknown_00D8) {
        this->unknown_00D8->position.x = this->position[0];
        this->unknown_00D8->position.y = this->position[1];
        this->unknown_00D8->position.z = this->position[2];

        float px2 = this->unknown_00D8->position.x;
        float py2 = this->unknown_00D8->position.y;
        float pz2 = this->unknown_00D8->position.z;

        float vx2 = py2 * view.values[3] + px2 * view.values[0] + pz2 * view.values[6] + view.values[9];
        float vy2 = (py2 * view.values[4] + px2 * view.values[1] + pz2 * view.values[7] + view.values[10]) - 8.0f;
        float vz2 = py2 * view.values[5] + px2 * view.values[2] + pz2 * view.values[8] + view.values[11];

        this->unknown_00D8->position.x = vy2 * inverse.values[3] + vx2 * inverse.values[0] + vz2 * inverse.values[6] + inverse.values[9];
        this->unknown_00D8->position.y = vy2 * inverse.values[4] + vx2 * inverse.values[1] + vz2 * inverse.values[7] + inverse.values[10];
        this->unknown_00D8->position.z = vy2 * inverse.values[5] + vx2 * inverse.values[2] + vz2 * inverse.values[8] + inverse.values[11];

        this->unknown_00D8->Process(0.0f, 0.0f, 0.0f);
      }
    } else {
      FreeParticle(this->unknown_00D4);
      FreeParticle(this->unknown_00D8);
      this->unknown_00D4 = nullptr;
      this->unknown_00D8 = nullptr;
    }
  }

  const DWORD now = timeGetTime();
  if (static_cast<std::int64_t>(now) - this->startTime >= 1000) {
    this->startTime = timeGetTime();
    this->sub_6F7660();
  }
}
