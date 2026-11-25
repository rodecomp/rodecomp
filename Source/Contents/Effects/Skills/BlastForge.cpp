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
static void MatrixMult(vector3d& result, const vector3d& v, const float* m) {
  const float y = v.y;
  const float x = v.x;
  const float z = v.z;

  result.x = y * m[3];
  result.x += x * m[0];
  result.x += z * m[6];
  result.x += m[9];
  result.y = y * m[4];
  result.y += x * m[1];
  result.y += z * m[7];
  result.y += m[10];
  result.z = y * m[5];
  result.z += x * m[2];
  result.z += z * m[8];
  result.z += m[11];
}

// INLINED
static __forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return static_cast<float>(particle->elapsedFrames) < particle->life * 60.0f;
}

// OFFSET: 0x006F7360
// FUNCTION: fn_06858
CBlastForge::CBlastForge() : CEffect(-1), openToggle(0), startTime(timeGetTime()) {
  this->loopParticle = nullptr;
  this->loopBottomParticle = nullptr;
  this->blastParticle = nullptr;
  this->blastBottomParticle = nullptr;
}

// OFFSET: 0x006F7400
// FUNCTION: fn_46145
__declspec(noinline) CBlastForge::~CBlastForge() {
}

// OFFSET: 0x006F74C0
// FUNCTION: fn_46146
__forceinline void CBlastForge::CreateLoopParticles() {
  this->openToggle = (this->openToggle == 0);
  this->loopParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  this->loopBottomParticle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));

  if (this->loopParticle && this->loopBottomParticle) {
    this->loopParticle->Open("blast_forge\\blast_forge\\blast_forge_5s.str");
    this->loopBottomParticle->Open("blast_forge\\blast_forge\\blast_forge_5s_bottom.str");
    this->loopParticle->m_loop = true;
    this->loopBottomParticle->m_loop = true;
  }
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
  this->CreateLoopParticles();
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
  this->UpdateParticle(this->loopParticle);
  this->UpdateParticle(this->loopBottomParticle);
  this->ProcessBlastParticles();
}

// OFFSET: 0x006F7D20
// FUNCTION: fn_46147
__forceinline void CBlastForge::UpdateParticle(CEZ2STRParticle* particle) {
  if (!particle) {
    return;
  }

  particle->position.x = this->positionVector.x;
  particle->position.y = this->positionVector.y;
  particle->position.z = this->positionVector.z;

  vector3d position = particle->position;
  const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
  const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
  vector3d viewPosition;
  MatrixMult(viewPosition, position, view);
  viewPosition.y -= 8.0f;
  MatrixMult(position, viewPosition, inverse);
  particle->position = position;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x006F7EA0
// FUNCTION: fn_06865
void CBlastForge::Render() {
  if (!this->blastParticle) {
    this->RenderParticle(this->loopParticle);
    this->RenderParticle(this->loopBottomParticle);
  }

  this->RenderParticle(this->blastParticle);
  this->RenderParticle(this->blastBottomParticle);
}

// OFFSET: 0x006F7F50
// FUNCTION: fn_46148
__forceinline void CBlastForge::RenderParticle(CEZ2STRParticle* particle) {
  if (particle) {
    particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x006F7F80
// FUNCTION: fn_06866
void CBlastForge::ProcessBlastParticles() {
  if (this->blastParticle && this->blastBottomParticle) {
    if (IsParticleActive(this->blastParticle) == true) {
      this->UpdateParticle(this->blastParticle);
      this->UpdateParticle(this->blastBottomParticle);
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
