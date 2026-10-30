#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"

#include <cstdint>

// OFFSET: 0x00E1D6B0
// FUNCTION: fn_23850
int CSession::IsState291To303(int state) const {
  switch (state) {
  case 291:
  case 292:
  case 293:
  case 294:
  case 295:
  case 297:
  case 298:
  case 299:
  case 300:
  case 301:
  case 302:
  case 303:
    return 1;
  default:
    return 0;
  }
}

// OFFSET: 0x00E1D850
// FUNCTION: fn_23852
int CSession::IsType0Or4(int type) const {
  if (type == 0 || type == 4) {
    return 1;
  }
  return 0;
}

// OFFSET: 0x00E1DD50
// FUNCTION: fn_23879
int CSession::IsMonsterActor(CGameActor* actor) const {
  if (!actor) {
    return 0;
  }

  const int job = actor->GetEffectiveJob(-1);
  if ((job > 1000 && job < 4000) || (job >= 20021 && job <= 31999)) {
    return 1;
  }

  if (actor->GetEffectiveJob(-1) >= 2114) {
    actor->GetEffectiveJob(-1);
  }
  return 0;
}

// OFFSET: 0x00E1DDB0
// FUNCTION: fn_23880
int CSession::IsLocalPlayerAid(std::uint32_t aid) const {
  if (aid == 0) {
    return 0;
  }

  CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  if (player && aid == static_cast<std::uint32_t>(player->Slot50())) {
    return 1;
  }
  return 0;
}

// OFFSET: 0x00E1DDF0
// FUNCTION: fn_23881
bool CSession::IsHomunAid(std::uint32_t aid) const {
  return aid != 0xFFFFFFFF && aid == *reinterpret_cast<const std::uint32_t*>(&this->unknown_5570[8]);
}

// OFFSET: 0x00E1DE10
// FUNCTION: fn_23882
bool CSession::IsMercenaryAid(std::uint32_t aid) const {
  return aid != 0xFFFFFFFF && aid == *reinterpret_cast<const std::uint32_t*>(&this->unknown_5620[8]);
}

// OFFSET: 0x00E1DF00
// FUNCTION: fn_23885
bool CSession::IsPetAid(std::uint32_t aid) const {
  return aid != 0xFFFFFFFF && aid == this->unknown_1010;
}
