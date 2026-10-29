#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/MessageTable.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstdio>

#pragma pack(push, 1)
struct ActorPairFreezePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t firstAid;
  /* 0006 */ std::uint32_t secondAid;
  /* 000A */ int frozen;
};

struct ActorEffectPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int effectId;
};

struct ActorPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
};

struct ShortValuePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
};
#pragma pack(pop)

// OFFSET: 0x00D6D200
// FUNCTION: fn_22087
void CGameMode::ReceiveResultNotice(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4125)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4126)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4127)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6D260
// FUNCTION: fn_22088
void CGameMode::ReceiveActorPairFreeze(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorPairFreezePacket*>(data);

  if (packet->frozen) {
    CRenderObject* first = packet->firstAid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(packet->firstAid);
    CRenderObject* second = packet->secondAid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(packet->secondAid);
    if (first && second) {
      first->SetFrameSequence(96, 4, 4, 96, 4, 1, 99999999, 0);
      first->render.unknown_00A3 = 1;
      first->SetColor(-1, 64, 64, 64);
      first->render.instant.Set(3, 1);
      second->render.instant.Set(3, 1);
      second->render.isMotionFreezed = true;
      second->render.unknown_00A3 = 1;
      second->SetColor(-1, 64, 64, 64);
    }
  } else {
    CRenderObject* first = packet->firstAid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(packet->firstAid);
    CRenderObject* second = packet->secondAid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(packet->secondAid);
    if (first) {
      first->SetFrameSequence(96, 4, 4, 96, 4, 1, 1, 0);
      first->render.unknown_00A3 = 0;
      first->SetColor(-1, 255, 255, 255);
      first->render.instant.Set(3, 0);
    }

    if (second) {
      second->render.isMotionFreezed = false;
      second->render.unknown_00A3 = 0;
      second->SetColor(-1, 255, 255, 255);
      second->render.instant.Set(3, 0);
    }
  }
}

// OFFSET: 0x00D6E100
// FUNCTION: fn_22096
void CGameMode::RefreshWindow16(const unsigned char*) {
  UIWindow* window = g_windowMgr.LookupWindow(16);
  if (window) {
    window->SendMsg(nullptr, 75, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6E130
// FUNCTION: fn_22097
void CGameMode::ReceiveEffectRemove(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorEffectPacket*>(data);

  CRenderObject* actor = this->scene->GetGameActorByAidIncludeMe(packet->aid);
  if (actor) {
    while (static_cast<EffectActor*>(actor)->state.Attachments().HasId(packet->effectId)) {
      actor->SendMsg(nullptr, 109, 0, packet->effectId, packet->effectId >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  while (g_effectMgr->Find(packet->aid, packet->effectId)) {
    g_effectMgr->Retire(packet->aid, packet->effectId);
  }
}

// OFFSET: 0x00D6E950
// FUNCTION: fn_22108
void CGameMode::ReceiveActorEffect347(const unsigned char* data) {
  if (g_serviceOptions.Get(8)) {
    const auto* packet = reinterpret_cast<const ActorPacket*>(data);
    CRenderObject* actor = packet->aid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(packet->aid);
    if (actor && packet->aid) {
      static_cast<EffectActor*>(actor)->LaunchEffect(347, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }
}

// OFFSET: 0x00D6E9D0
// FUNCTION: fn_22109
void CGameMode::ReceiveUnknown0294Text(const unsigned char* data) {
  std::sprintf(this->unknown_0294, reinterpret_cast<const char*>(data + 2));
}

// OFFSET: 0x00D6E9F0
// FUNCTION: fn_22110
void CGameMode::ReceiveWindow154Close(std::int16_t value) {
  if (value == 0) {
    UIWindow* window = g_windowMgr.LookupWindow(340);
    if (window) {
      window->SendMsg(nullptr, 64, 2, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D6EA60
// FUNCTION: fn_22112
void CGameMode::PlaceWindow16(unsigned int xPercent, unsigned int yPercent) {
  UIWindow* window = g_windowMgr.LookupWindow(16);
  if (window) {
    window->Move(static_cast<int>((g_renderer->viewport.width - window->geometry.width) * (xPercent * 0.01)),
      static_cast<int>((g_renderer->viewport.height - window->geometry.height) * (yPercent * 0.01)));
  }
}

// OFFSET: 0x00D6EB00
// FUNCTION: fn_22113
void CGameMode::MoveWindow16(int x, int y) {
  UIWindow* window = g_windowMgr.LookupWindow(16);
  if (window) {
    window->Move(x, y);
  }
}

// OFFSET: 0x00D6EB20
// FUNCTION: fn_22114
void CGameMode::ResizeWindow16(int width, int height) {
  UIWindow* window = g_windowMgr.LookupWindow(16);
  if (window) {
    window->Resize(width, height);
  }
}

// OFFSET: 0x00D6EBD0
// FUNCTION: fn_22117
void CGameMode::ReceiveUnknown7FBC(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ShortValuePacket*>(data);

  if (packet->value <= 100) {
    g_session.unknown_7FBC = packet->value;
    if (g_windowMgr.unknown_01D4) {
      reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01D4)->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D6EC10
// FUNCTION: fn_22118
void CGameMode::ReceiveNotice1950(const unsigned char*) {
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1950)), 0xFF, 0, 0, 0);
}
