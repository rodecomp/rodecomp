#include "Contents/Effects/Effect.h"

#include "Client/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/World/World.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Sound/SoundMgr.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <bit>

// OFFSET: 0x00B4DAE0
// FUNCTION: fn_18979
CEffect::CEffect(int value)
    : id(value), position{}, sourcePosition{}, actor(0), sourceActor(0), frame(0), unknown_002C(60), unknown_0030(1), outsideRange(0), range(130),
      unknown_00B0(-1), retained(0), forceRender(0), unknown_00C4(0), unknown_00C8(0) {
}

// OFFSET: 0x00B4DD10
// OFFSET: 0x00B4DDD0
// FUNCTION: fn_18981
CEffect::~CEffect() = default;

// OFFSET: 0x005A2150
// FUNCTION: fn_03087
void CEffect::Close() {
}

// OFFSET: 0x00B4DE40
// FUNCTION: fn_18984
void CEffect::AddEvent(int kind, const char* text, int frame, int argument) {
  EffectEvent event{kind, frame, argument, text ? text : ""};
  this->events.push_back(event);
}

// OFFSET: 0x00B4DF90
// FUNCTION: fn_18985
bool CEffect::RetireIfInactive() {
  if (!this->Slot06()) {
    g_effectMgr->Retire(this, 0);
    return true;
  }

  return false;
}

// OFFSET: 0x00B4E0E0
// FUNCTION: fn_18986
bool CEffect::Slot33() {
  EffectActor* actor = static_cast<EffectActor*>(FindRenderObject(this->actor));
  if (!actor || actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (this->unknown_00C8 == 0) {
    if (this->actor == g_session.m_aid) {
      return true;
    }

    if (g_session.IsPvpMap() && g_session.IsPartyMember(actor->render.accountId, true)) {
      return true;
    }
  }

  return actor->EffectVisible();
}

// OFFSET: 0x00B4E150
// FUNCTION: fn_18987
bool CEffect::Slot06() {
  if (this->actor != 0 && !FindRenderObject(this->actor)) {
    return false;
  }

  return true;
}

// OFFSET: 0x00B4E170
// FUNCTION: fn_18988
void CEffect::SetEventFrame(int kind, int frame) {
  for (EffectEvent& event : this->events) {
    if (event.kind == kind) {
      event.frame = frame;
      return;
    }
  }
}

// OFFSET: 0x00B4E290
// FUNCTION: fn_18989
void CEffect::UpdateVisibility() {
  if ((g_serviceOptions.Get(8) || this->forceRender) && this->Slot06() && !this->outsideRange) {
    this->Slot04();
  }
}

// OFFSET: 0x00B4E390
// FUNCTION: fn_18991
void CEffect::SetActor(std::uint32_t aid) {
  this->actor = aid;
}

// OFFSET: 0x00B4E3A0
// FUNCTION: fn_18992
void CEffect::SetSourceActor(std::uint32_t aid) {
  if (CRenderObject* source = FindRenderObject(aid)) {
    this->sourceActor = aid;
    this->sourcePosition[0] = source->render.position.x;
    this->sourcePosition[1] = source->render.position.y;
    this->sourcePosition[2] = source->render.position.z;
  }
}

// OFFSET: 0x00B4E3E0
// FUNCTION: fn_18993
void CEffect::Update(std::uint32_t aid) {
  if (CRenderObject* actor = FindRenderObject(aid)) {
    this->position[0] = actor->render.position.x;
    this->position[1] = actor->render.position.y;
    this->position[2] = actor->render.position.z;
  }

  this->Slot02();
  if (this->outsideRange) {
    return;
  }

  const vector3d player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
  for (auto event = this->events.begin(); event != this->events.end(); ++event) {
    if (this->frame != event->frame) {
      continue;
    }

    switch (event->kind) {
    case 0:
      if (!event->text.empty()) {
        g_soundMgr->PlayWave(event->text.c_str(), this->position[0] - player.x, std::bit_cast<float>(event->argument),
                             this->position[2] - player.z, 250, 40, 1.0f, 0);
      }
      break;
    case 1:
      g_modeMgr.GetGameMode()->SendMsg(122, 0, 0, 0, 0);
      break;
    }
  }

  this->Slot03();
  ++this->frame;
}

// OFFSET: 0x00B3C390
// FUNCTION: fn_18808
vector3d PlayerPosition() {
  vector3d position;
  position.z = 0.0f;
  position.x = 0.0f;
  position.y = 0.0f;
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (CPlayer* player = mode->scene->player) {
    position = player->render.position;
    return position;
  }

  mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  mode->scene->terrain->Grid().CellCenter(g_session.m_playerX, g_session.m_playerY, position.x, position.z);
  mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  position.y = mode->scene->terrain->Grid().Height(position.x, position.z);
  return position;
}

// OFFSET: 0x00B4E510
// FUNCTION: fn_18994
void CEffect::Slot02() {
  const vector3d player = PlayerPosition();
  const float y = player.y - this->position[1];
  const float x = player.x - this->position[0];
  const float z = player.z - this->position[2];
  this->outsideRange = static_cast<float>(this->range) <= SquareRoot(y * y + x * x + z * z);
}

// OFFSET: 0x00B4E630
// FUNCTION: fn_18998
bool CEffect::InRange() {
  const vector3d player = PlayerPosition();
  const float y = player.y - this->position[1];
  const float x = player.x - this->position[0];
  const float z = player.z - this->position[2];
  return SquareRoot(y * y + x * x + z * z) < static_cast<float>(this->range);
}

// OFFSET: 0x00B4E6A0
// FUNCTION: fn_18999
bool CEffect::ActorExists() const {
  if (this->actor != 0 && !FindRenderObject(this->actor)) {
    return false;
  }

  return true;
}

// OFFSET: 0x00B4E6C0
// FUNCTION: fn_19000
bool CEffect::IsFinished() const {
  return this->frame >= this->unknown_002C;
}

// OFFSET: 0x00B249A0
// FUNCTION: fn_18773
void EffectRetirement::Clear() {
  for (auto effect = this->effects.begin(); effect != this->effects.end(); ++effect) {
    (*effect)->Close();
    delete *effect;
  }

  this->effects.clear();
}
