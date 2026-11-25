#include "Contents/Effects/Skills/StormCannonEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <string>

// OFFSET: 0x0072CDC0
// FUNCTION: fn_07484
CStormCannonEffect::CStormCannonEffect() : CEffect(1755), direction({0.0f, 0.0f, 0.0f}), origin({0.0f, 0.0f, 0.0f}) {
}

// OFFSET: 0x0072CE80
// FUNCTION: fn_46079
__declspec(noinline) CStormCannonEffect::~CStormCannonEffect() {
}

// OFFSET: 0x0072CF60
// FUNCTION: fn_07486
void CStormCannonEffect::Close() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    FreeParticle(*it);
  }
  this->emitters.clear();
}

// INLINED
static vector3d Subtract(const vector3d& left, const vector3d& right) {
  return vector3d{left.x - right.x, left.y - right.y, left.z - right.z};
}

// INLINED
static void Normalize(vector3d& vector) {
  const float scale = 1.0f / SquareRoot(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
  vector.x = scale * vector.x;
  vector.y = scale * vector.y;
  vector.z = scale * vector.z;
}

// INLINED
static float Distance(const vector3d& left, const vector3d& right) {
  const float dx = left.x - right.x;
  const float dy = left.y - right.y;
  const float dz = left.z - right.z;
  return SquareRoot(dy * dy + dx * dx + dz * dz);
}

// OFFSET: 0x0072CFA0
// FUNCTION: fn_07487
void CStormCannonEffect::Open() {
  CEZ2STRParticle* bottom = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));
  if (bottom) {
    bottom->Load(std::string("stormcannon\\stormcannon_bottom\\stormcannon_bottom.str"));
    bottom->m_loop = true;
    this->emitters.push_back(bottom);
  }

  CEZ2STRParticle* cannon = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (cannon) {
    cannon->Load(std::string("stormcannon\\stormcannon\\stormcannon.str"));
    cannon->m_loop = true;
    this->emitters.push_back(cannon);
  }

  auto& sourcePositionVectorRef = this->sourcePositionVector;
  this->direction = Subtract(sourcePositionVectorRef, this->positionVector);
  Normalize(this->direction);

  this->origin = this->positionVector;
  this->speed = 1.7;

  this->distance = Distance(sourcePositionVectorRef, this->positionVector);
  this->Alpha() = 255;

  this->AddEvent(EFFECT_EVENT_SOUND, "effect\\AG_STORM_CANNON.wav", 0, 0);
}

// OFFSET: 0x0072D350
// FUNCTION: fn_07488
bool CStormCannonEffect::IsAlive() {
  return this->Alpha() > 0;
}

// OFFSET: 0x0072D4B0
// FUNCTION: fn_46080
__forceinline void CStormCannonEffect::UpdateEmitter(CEZ2STRParticle* emitter) {
  if (emitter) {
    emitter->m_alpha = static_cast<float>(this->Alpha() / 255.0);
    const vector3d position = {this->position[0] - 1.5f, this->position[1] - 17.0f, this->position[2] + 0.0f};
    emitter->position = position;
    emitter->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x0072D360
// FUNCTION: fn_07489
void CStormCannonEffect::OnProcess() {
  const float speed = static_cast<float>(this->speed);
  const vector3d step = {speed * this->direction.x, speed * this->direction.y, speed * this->direction.z};
  this->positionVector += step;

  const float travelled = Distance(this->positionVector, this->origin);
  if (travelled > this->distance) {
    this->Alpha() = std::max(this->Alpha() - 10, 0);
  }

  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    this->UpdateEmitter(*it);
  }
}

// OFFSET: 0x0072D590
// FUNCTION: fn_46081
__forceinline void CStormCannonEffect::RenderEmitter(CEZ2STRParticle* emitter) {
  if (emitter) {
    emitter->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x0072D540
// FUNCTION: fn_07490
void CStormCannonEffect::Render() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    this->RenderEmitter(*it);
  }
}
