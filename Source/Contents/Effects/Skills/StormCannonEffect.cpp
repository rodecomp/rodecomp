#include "Contents/Effects/Skills/StormCannonEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <string>

// OFFSET: 0x0072CDC0
// FUNCTION: fn_07484
CStormCannonEffect::CStormCannonEffect() : CEffect(1755) {
}

// OFFSET: 0x0072CF60
// FUNCTION: fn_07486
void CStormCannonEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(*it);
  }
  this->emitters.clear();
}

// OFFSET: 0x0072CFA0
// FUNCTION: fn_07487
void CStormCannonEffect::Open() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  if (bottom) {
    bottom->Load(std::string("stormcannon\\stormcannon_bottom\\stormcannon_bottom.str"));
    bottom->m_loop = true;
    this->emitters.push_back(bottom);
  }

  CEZ2STRParticle* cannon = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (cannon) {
    cannon->Load(std::string("stormcannon\\stormcannon\\stormcannon.str"));
    cannon->m_loop = true;
    this->emitters.push_back(cannon);
  }

  this->direction.x = this->sourcePosition[0] - this->position[0];
  this->direction.y = this->sourcePosition[1] - this->position[1];
  this->direction.z = this->sourcePosition[2] - this->position[2];
  const float scale =
    1.0f / SquareRoot(this->direction.x * this->direction.x + this->direction.y * this->direction.y + this->direction.z * this->direction.z);
  this->direction.x = scale * this->direction.x;
  this->direction.y = scale * this->direction.y;
  this->direction.z = scale * this->direction.z;

  this->origin = *reinterpret_cast<const vector3d*>(this->position);
  this->speed = 1.7;

  const float dx = this->sourcePosition[0] - this->position[0];
  const float dy = this->sourcePosition[1] - this->position[1];
  const float dz = this->sourcePosition[2] - this->position[2];
  this->distance = SquareRoot(dy * dy + dx * dx + dz * dz);
  this->Alpha() = 255;

  this->AddEvent(0, "effect\\AG_STORM_CANNON.wav", 0, 0);
}

// OFFSET: 0x0072D350
// FUNCTION: fn_07488
bool CStormCannonEffect::IsAlive() {
  return this->Alpha() > 0;
}

// OFFSET: 0x0072D360
// FUNCTION: fn_07489
void CStormCannonEffect::OnProcess() {
  const float speed = static_cast<float>(this->speed);
  const float x = speed * this->direction.x + this->position[0];
  const float y = speed * this->direction.y + this->position[1];
  const float z = speed * this->direction.z + this->position[2];
  this->position[1] = y;
  this->position[0] = x;
  this->position[2] = z;

  const float dx = this->position[0] - this->origin.x;
  const float dy = this->position[1] - this->origin.y;
  const float dz = this->position[2] - this->origin.z;
  const float travelled = SquareRoot(dy * dy + dx * dx + dz * dz);
  if (travelled > this->distance) {
    const int alpha = this->Alpha() - 10;
    this->Alpha() = alpha < 0 ? 0 : alpha;
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    CEZ2STRParticle* emitter = *it;
    if (!emitter) {
      continue;
    }

    emitter->m_alpha = static_cast<float>(this->Alpha() / 255.0);
    const float emitterX = this->position[0] - 1.5f;
    const float emitterY = this->position[1] - 17.0f;
    const float emitterZ = this->position[2] + 0.0f;
    emitter->position.x = emitterX;
    emitter->position.y = emitterY;
    emitter->position.z = emitterZ;
    emitter->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x0072D540
// FUNCTION: fn_07490
void CStormCannonEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
