#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/ClientState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/RodexMailStore.h"
#include "Contents/RouletteMgr.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

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

struct ActorPointPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::int16_t x;
  /* 0008 */ std::int16_t y;
};

struct VariablePacketHeader {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
};

struct RodexMailDeletedPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char type;
  /* 0003 */ std::int64_t mailId;
};

struct RouletteResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ std::uint16_t stage;
  /* 0005 */ std::uint16_t index;
  /* 0007 */ int additionItemId;
  /* 000B */ int goldPoint;
  /* 000F */ int silverPoint;
  /* 0013 */ int bronzePoint;
};

struct ByteValuePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char value;
};
#pragma pack(pop)

// OFFSET: 0x00D45E30
// FUNCTION: fn_21842
void CGameMode::StopActorMove(std::uint32_t aid, std::int16_t x, std::int16_t y, unsigned char flag) {
  CRenderObject* actor;
  if (aid == g_session.m_aid) {
    actor = this->scene->player;
  } else {
    actor = this->scene->GetGameActorByAID(aid);
    if (!actor) {
      return;
    }
  }

  const int posX = x;
  const int posY = y;
  const int value = flag;
  actor->SendMsg(nullptr, 19, 0, posX, posX >> 31, posY, posY >> 31, 0, 0, value, value >> 31, 0, 0, 0, 0);
}

// OFFSET: 0x00D69140
// FUNCTION: fn_22044
void CGameMode::NotifyResult2877(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2877)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2878)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2879)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D69700

// OFFSET: 0x00D69850
// FUNCTION: fn_22052
void CGameMode::NotifyResult3562(const unsigned char* packet) {
  switch (reinterpret_cast<const ByteValuePacket*>(packet)->value) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3566)), 0xFFFF00, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3562)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3563)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3564)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3565)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D698F0
// FUNCTION: fn_22053
void CGameMode::IgnoreRemovedPacket6() {
}

// OFFSET: 0x00D69900
// FUNCTION: fn_22054
void CGameMode::ReceiveRouletteResult(const unsigned char* packet) {
  const auto* result = reinterpret_cast<const RouletteResultPacket*>(packet);
  unsigned char bonus;

  switch (result->result) {
  case 0:
    g_rouletteMgr->m_stage = result->stage;
    g_rouletteMgr->SetAdditionItem(result->additionItemId);
    bonus = 0;
    break;
  case 3:
    g_rouletteMgr->m_stage = result->stage;
    g_rouletteMgr->SetAdditionItem(result->additionItemId);
    bonus = 1;
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2634)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.unknown_0464) {
      g_windowMgr.EnableRouletteWnd();
    }
    return;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2635)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.unknown_0464) {
      g_windowMgr.EnableRouletteWnd();
    }
    return;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2693)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.unknown_0464) {
      g_windowMgr.EnableRouletteWnd();
    }
    return;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2700)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.unknown_0464) {
      g_windowMgr.EnableRouletteWnd();
    }
    return;
  default:
    return;
  }

  g_rouletteMgr->m_goldPoint = result->goldPoint;
  g_rouletteMgr->m_silverPoint = result->silverPoint;
  g_rouletteMgr->m_bronzePoint = result->bronzePoint;
  g_rouletteMgr->SetResult(result->index, 0, bonus);
  g_rouletteMgr->StartSpin(461, 341 - result->stage * 40, static_cast<int>(g_renderer->viewport.width * 0.5 - 320.0),
    static_cast<int>(g_renderer->viewport.height * 0.5 - 240.0));
}

// OFFSET: 0x00D69FC0
// FUNCTION: fn_22057
void CGameMode::NotifyResult4013(unsigned char result, unsigned short value) {
  char text[256];

  switch (result) {
  case 0:
    std::memset(text, 0, sizeof(text));
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(4013), value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4014)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4015)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4016)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4017)), 0xFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4018)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6AE70
// FUNCTION: fn_22065
void CGameMode::NotifyWindow4F258(int result) {
  UIWindow* window = g_windowMgr.window_4F258;
  if (!window) {
    return;
  }

  switch (result) {
  case 0:
    if (!g_windowMgr.modalWindows.empty() && g_windowMgr.modalWindows.back()) {
      g_windowMgr.unknown_0474 = 188;
    }
    break;
  case 1:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(2825)), 0, 0, 0);
    break;
  case 2:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(3333)), 0, 0, 0);
    break;
  case 3:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(3332)), 0, 0, 0);
    break;
  case 4:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(3331)), 0, 0, 0);
    break;
  case 5:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(3352)), 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1923)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6B950
// FUNCTION: fn_22071
void CGameMode::ReceiveNamedResult(std::uint32_t, std::string name, int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4068)), 0x64FFFF, 0, 0, 0);
    break;
  case 1: {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(4069), name.c_str()).c_str()), 0x64FFFF, 0, 0, 0);

    CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    if (scene) {
      scene->player->LaunchEffect(41, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  }
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4070)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4071)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4072)), 0x64FFFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4073)), 0x64FFFF, 0, 0, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4096)), 0x64FFFF, 0, 0, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4000)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6BBC0
// FUNCTION: fn_22073
void CGameMode::ReceiveBasicInfoResult(unsigned char result) {
  switch (result) {
  case 1:
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3956)), 0xFF, 0, 0, 0);
    if (g_windowMgr.unknown_01E0) {
      g_windowMgr.unknown_01E0->SendMsg(nullptr, 106, 575, 0, 0, 0);
    }
    break;
  }

  if (g_windowMgr.unknown_01E0) {
    g_windowMgr.unknown_01E0->SendMsg(nullptr, 106, 575, 0, 0, 0);
  }
}

// OFFSET: 0x00D6BD40
// FUNCTION: fn_22075
void CGameMode::ReceiveUnknown0454Result(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ByteValuePacket*>(data);

  switch (packet->value) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2587)), 0x64FFFF, 0, 0, 0);
    break;
  case 1:
    g_modeMgr.GetGameMode()->SendMsg(293, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2597)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_modeMgr.GetGameMode()->SendMsg(293, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2603)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_modeMgr.GetGameMode()->SendMsg(293, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2598)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_modeMgr.GetGameMode()->SendMsg(293, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2599)), 0x64FFFF, 0, 0, 0);
    break;
  }

  if (g_windowMgr.unknown_0454) {
    reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0454)->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6BFA0
// FUNCTION: fn_22077
void CGameMode::ReceiveWindow154Result(std::int16_t value) {
  switch (value) {
  case 0: {
    UIWindow* window = g_windowMgr.LookupWindow(340);
    if (window) {
      window->SendMsg(nullptr, 64, 1, 0, 0, 0);
    }
    break;
  }
  case 1: {
    UIWindow* window = g_windowMgr.LookupWindow(340);
    if (window) {
      window->SendMsg(nullptr, 64, 1, 1, 0, 0);
    }
    break;
  }
  case 3: {
    UIWindow* window = g_windowMgr.LookupWindow(340);
    if (window) {
      window->SendMsg(nullptr, 64, 1, 3, 0, 0);
    }
    break;
  }
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2517)), 0xFF, 0, 0, 0);
    break;
  }
}

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

// OFFSET: 0x00D6E930
// FUNCTION: fn_22107
void CGameMode::ReceiveToggle128(int enabled) {
  sub_E489C0(enabled != 0);
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

// OFFSET: 0x00D6EC40
// FUNCTION: fn_22119
void CGameMode::ReceiveActorMessage144(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorPointPacket*>(data);
  CRenderObject* actor;
  if (packet->aid == g_session.m_aid) {
    actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  } else {
    actor = g_session.FindGameActor(packet->aid);
  }

  if (actor) {
    const int x = packet->x;
    const int y = packet->y;
    actor->SendMsg(nullptr, 144, 0, x, x >> 31, y, y >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6EF60
// FUNCTION: fn_22121
void CGameMode::NotifyResult4138(int result) {
  int message;
  switch (result) {
  case 0:
    message = 4138;
    break;
  case 1:
    message = 4139;
    break;
  case 2:
    message = 4140;
    break;
  case 3:
    message = 4187;
    break;
  case 4:
    message = 4141;
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D6FAC0
// FUNCTION: fn_22125
void CGameMode::ReceiveGuildUnknown0114List(const unsigned char* packet) {
  const auto* header = reinterpret_cast<const VariablePacketHeader*>(packet);
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));

  guildInfo->ClearUnknown0114();
  const int count = header->length - 4;
  for (int i = 0; i < count; ++i) {
    unsigned char value;
    memcpy_s(&value, sizeof(value), packet + sizeof(VariablePacketHeader) + i, sizeof(value));
    guildInfo->AddUnknown0114(value);
  }
}
