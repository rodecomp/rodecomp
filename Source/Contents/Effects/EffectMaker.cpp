#include "Contents/Effects/EffectMaker.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/World/World.h"

// OFFSET: 0x00E4A2B0
// FUNCTION: fn_24251
CEffectMaker::CEffectMaker() {
  this->unknown_0160 = -1;
  this->unknown_015C = 360;
  this->unknown_0158 = 360;
  this->unknown_0164 = 0;
  this->unknown_0168 = 0;
  this->unknown_016C = 0;
  this->unknown_0170 = 0;
}

// OFFSET: 0x00E4BA80
// FUNCTION: fn_24278
void CEffectMaker::Reset() {
  this->unknown_0160 = -1;
  this->unknown_015C = 360;
  this->unknown_0158 = 360;
  this->unknown_0164 = 0;
  this->unknown_0168 = 0;
  this->unknown_016C = 0;
  this->unknown_0170 = 0;
}

// OFFSET: 0x00E4C210
// FUNCTION: fn_24282
bool CEffectMaker::OnProcess() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CPlayer* player = mode->scene->player;
  if (this->CalcDist(player->render.position.x, player->render.position.z) < 130.0f && this->unknown_0158 >= this->unknown_015C) {
    this->unknown_0158 = 0;
    CRagEffect* effect = this->LaunchEffect(this->unknown_0160, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(nullptr, 46, 0, reinterpret_cast<int>(&this->unknown_0164), 0, 0, 0, 0);
    }
  }

  ++this->unknown_0158;
  return true;
}
