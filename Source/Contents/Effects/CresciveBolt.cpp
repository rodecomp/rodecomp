#include "Contents/Effects/CresciveBolt.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Render/Renderer.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x0070AAD0
// FUNCTION: fn_07106
CCresciveBolt::CCresciveBolt() : CEffect(1951), particle(nullptr) {
}

// OFFSET: 0x0070AC00
// FUNCTION: fn_07108
void CCresciveBolt::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }
}

// OFFSET: 0x0070AC30
// FUNCTION: fn_07109
void CCresciveBolt::Slot01() {
  float fromReciprocalDepth = 0.0f;
  float toReciprocalDepth = 0.0f;
  float fromX;
  float fromY;
  float toX;
  float toY;
  g_renderer->ProjectViewport(this->position, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
  g_renderer->ProjectViewport(this->sourcePosition, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);

  const float dx = toX - fromX;
  const float dy = toY - fromY;
  const float length = SquareRoot(dy * dy + dx * dx);
  double angle = static_cast<float>(std::acos(static_cast<double>(-dx / length))) / 3.141592f * 180.0f;
  if (toY > fromY) {
    angle = 360.0 - angle;
  }

  CEZ2STRParticle* bolt = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (bolt) {
    bolt->Load("crescivebolt\\crescivebolt\\crescivebolt.str");
    bolt->m_angle = static_cast<float>(angle);
    this->particle = bolt;
  }

  this->AddEvent(0, "effect\\WH_CRESCIVE_BOLT.wav", 0, 0);
}

// OFFSET: 0x0070AE90
// FUNCTION: fn_07110
bool CCresciveBolt::Slot06() {
  if (!this->particle) {
    return false;
  }

  if (this->particle->life == 0.0f) {
    return true;
  }

  return static_cast<float>(this->particle->unknown_00E8) < this->particle->life * 60.0f;
}

// OFFSET: 0x0070AED0
// FUNCTION: fn_07111
void CCresciveBolt::Slot03() {
  if (!this->particle) {
    return;
  }

  this->particle->position.x = this->position[0] + 0.0f;
  this->particle->position.y = this->position[1] - 3.0f;
  this->particle->position.z = this->position[2] + 0.0f;

  const float angle = static_cast<float>(static_cast<double>(this->particle->m_angle));
  if (angle > 90.0f && angle < 270.0f) {
    this->particle->Process(0.0f, 0.0f, 0.0f);
    return;
  }

  this->particle->Process(0.0f, -40.0f, 0.0f);
}

// OFFSET: 0x0070AF80
// FUNCTION: fn_07112
void CCresciveBolt::Slot04() {
  if (this->particle) {
    this->particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}
