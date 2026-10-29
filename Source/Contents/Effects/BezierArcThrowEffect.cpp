#include "Contents/Effects/BezierArcThrowEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"

// OFFSET: 0x007AAEA0
// FUNCTION: fn_09102
CThunderingOrb::CThunderingOrb(const std::string& name) : CBezierArcThrowEffect(name) {}

// OFFSET: 0x007AAF40
// FUNCTION: fn_09103
CWindBomb::CWindBomb(const std::string& name) : CBezierArcThrowEffect(name) {}

// OFFSET: 0x007AB800
// FUNCTION: fn_09111
void CBezierArcThrowEffect::Slot04() {
  if (this->m_particle) {
    this->m_particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

// OFFSET: 0x007AB830
// FUNCTION: fn_09112
void CBezierArcThrowEffect::SetTarget(const vector3d& target) {
  this->m_target.x = target.x;
  this->m_target.y = target.y;
  this->m_target.z = target.z;
}
