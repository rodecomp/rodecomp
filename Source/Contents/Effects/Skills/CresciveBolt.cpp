#include "Contents/Effects/Skills/CresciveBolt.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Render/Renderer.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x0070AAD0
// FUNCTION: fn_07106
CCresciveBolt::CCresciveBolt() : CEffect(1951), particle(nullptr) {
}

// OFFSET: 0x0070AB40
// FUNCTION: fn_46196
__declspec(noinline) CCresciveBolt::~CCresciveBolt() {
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
void CCresciveBolt::Open() {
  float fromReciprocalDepth = 0.0f;
  float toReciprocalDepth = 0.0f;
  float fromScreen[2];
  float toScreen[2];
  g_renderer->ProjectViewport(this->position, g_modeMgr.GetGameMode()->camera->frame.view.values, &fromScreen[0], &fromScreen[1], &fromReciprocalDepth);
  const float* const toMatrix = g_modeMgr.GetGameMode()->camera->frame.view.values;
  const float* const toPosition = this->sourcePosition;
  g_renderer->ProjectViewport(toPosition, toMatrix, &toScreen[0], &toScreen[1], &toReciprocalDepth);

  const float dx = toScreen[0] - fromScreen[0];
  const float dy = toScreen[1] - fromScreen[1];
  const float length = SquareRoot(dy * dy + dx * dx);
  double angle = static_cast<float>(std::acos(static_cast<double>(-dx / length))) / 3.141592f * 180.0f;
  if (toScreen[1] > fromScreen[1]) {
    angle = 360.0 - angle;
  }

  CEZ2STRParticle* bolt = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (bolt) {
    bolt->Load("crescivebolt\\crescivebolt\\crescivebolt.str");
    bolt->m_angle = static_cast<float>(angle);
    this->particle = bolt;
  }

  this->AddEvent(EFFECT_EVENT_SOUND, "effect\\WH_CRESCIVE_BOLT.wav", 0, 0);
}

// OFFSET: 0x0070AE90
// FUNCTION: fn_07110
bool CCresciveBolt::IsAlive() {
  if (this->particle) {
    if (0.0f == this->particle->life) {
      return true;
    }

    return static_cast<float>(this->particle->elapsedFrames) < this->particle->life * 60.0f;
  }

  return false;
}

// OFFSET: 0x0070AED0
// FUNCTION: fn_07111
void CCresciveBolt::OnProcess() {
  CEZ2STRParticle* particle = this->particle;
  if (!particle) {
    return;
  }

  const float x = this->position[0];
  const float y = this->position[1];
  const float z = this->position[2];
  particle->position.x = x + 0.0f;
  particle->position.y = y - 3.0f;
  particle->position.z = z + 0.0f;

  particle = this->particle;
  const float angle = static_cast<float>(static_cast<double>(particle->m_angle));
  if (angle <= 90.0f || angle >= 270.0f) {
    particle->Process(0.0f, -40.0f, 0.0f);
    return;
  }

  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0070AF80
// FUNCTION: fn_07112
void CCresciveBolt::Render() {
  if (this->particle) {
    this->particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}
