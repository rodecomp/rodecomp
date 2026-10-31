#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/ClientState.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/RodexMailStore.h"
#include "Contents/RouletteMgr.h"
#include "Contents/World/World.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/UICaptchaRegisterWnd.h"
#include "UI/UIGradeEnchantWnd.h"
#include "UI/UILapineScrollBar.h"
#include "UI/UILapineUpgradeBoxWnd.h"
#include "UI/UIMacroBlackListCheckWnd.h"
#include "UI/UIMacroDetectorWnd.h"
#include "UI/UIRefiningWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Text/UITagTextViewer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
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

struct PacketIdOnly {
  /* 0000 */ std::int16_t id;
};

struct RouletteInfoSlot {
  /* 0000 */ std::int16_t row;
  /* 0002 */ std::int16_t column;
  /* 0004 */ int itemId;
  /* 0008 */ std::int16_t count;
  /* 000A */ std::int16_t unknown_000A;
};

struct RouletteInfoPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ int unknown_0004;
  /* 0008 */ RouletteInfoSlot slots[42];
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
// FUNCTION: fn_22051
void CGameMode::ReceiveRodexMailDeleted(const unsigned char* packet) {
  const auto* deleted = reinterpret_cast<const RodexMailDeletedPacket*>(packet);
  std::map<std::int64_t, RodexMail>::iterator mail;
  std::map<std::int64_t, RodexMail>::iterator end;

  switch (deleted->type) {
  case 0:
    mail = g_rodexMailStore->m_mails0.find(deleted->mailId);
    end = g_rodexMailStore->m_mails0.end();
    break;
  case 1:
    mail = g_rodexMailStore->m_mails1.find(deleted->mailId);
    end = g_rodexMailStore->m_mails1.end();
    break;
  case 2:
    mail = g_rodexMailStore->m_mails2.find(deleted->mailId);
    end = g_rodexMailStore->m_mails2.end();
    break;
  default:
    goto refresh;
  }

  if (mail != end) {
    mail->second.unknown_0190 = 1;
    if (!mail->second.isRead) {
      --g_rodexMailStore->unknown_0018;
      if (g_rodexMailStore->unknown_0018 == 0) {
        UIWindow* window = g_windowMgr.LookupWindow(307);
        if (window) {
          window->SendMsg(nullptr, 6, 286, 476, 0, 0);
        }
      }
    }
  }

refresh:
  if (g_windowMgr.unknown_0454) {
    reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0454)->Invalidate();
  }
}

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

// OFFSET: 0x00D6EA30
// FUNCTION: fn_22111
void CGameMode::SetWindow16Align(unsigned char align) {
  UISayDialogWnd* window = static_cast<UISayDialogWnd*>(g_windowMgr.LookupWindow(16));
  if (window) {
    static_cast<UITagTextViewer*>(window->m_viewer)->SetAlign(align);
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

#pragma pack(push, 1)
struct GradeEnchantInfoPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::int16_t index;
  /* 0006 */ int value;
  /* 000A */ GradeEnchantInfo info;
  /* 001A */ int value1A;
  /* 001E */ int value1E;
};

struct PartyItemPickupPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int itemId;
  /* 000A */ unsigned char identified;
  /* 000B */ unsigned char damaged;
  /* 000C */ std::uint32_t cards[4];
  /* 001C */ std::uint16_t location;
  /* 001E */ unsigned char type;
  /* 001F */ unsigned char refine;
  /* 0020 */ unsigned char grade;
};

struct ActorEffect228Packet {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::int16_t value;
  /* 0008 */ std::uint16_t state;
};
#pragma pack(pop)

// OFFSET: 0x00D6F870
// FUNCTION: fn_22123
void CGameMode::ReceiveGradeEnchantInfo(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GradeEnchantInfoPacket*>(data);
  const int count = static_cast<unsigned int>(packet->length - sizeof(GradeEnchantInfoPacket)) / sizeof(GradeEnchantMaterial);
  if (packet->index == -1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3720)), 0x62B0FF, 0, 0, 0);
    return;
  }

  auto* window = static_cast<UIGradeEnchantWnd*>(g_windowMgr.LookupWindow(343));
  if (window) {
    window->SetEnchantInfo(packet->index, packet->value, count, packet->info, packet->value1A, packet->value1E,
      reinterpret_cast<const GradeEnchantMaterial*>(data + sizeof(GradeEnchantInfoPacket)));
  }
}

// OFFSET: 0x00D6FCD0
// FUNCTION: fn_22127
void CGameMode::ReceivePartyItemPickup(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const PartyItemPickupPacket*>(data);
  MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyByAccount(packet->aid);
  if (packet->aid != g_session.m_aid && member.valid) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->itemId;
    item.unknown_0000[3] = 0;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0088 = packet->grade;

    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    std::string name = naming.DisplayName(item, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1276), member.name.c_str(), name.c_str()).c_str()), 0xFFFF00, 9, 0, 0);
  }
}

// OFFSET: 0x00D71660
// FUNCTION: fn_22135
void CGameMode::NotifyResult3995(unsigned char result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3995)), 0xFFFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3996)), 0xFFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3997)), 0xFFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3998)), 0xFFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3999)), 0xFFFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4000)), 0xFFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D718D0
// FUNCTION: fn_22138
void CGameMode::ReceiveActorEffect228(const unsigned char* data) {
  if (!g_serviceOptions.Get(8)) {
    return;
  }

  const auto* packet = reinterpret_cast<const ActorEffect228Packet*>(data);
  CRenderObject* actor = this->scene->GetGameActorByAID(packet->aid);
  if (g_session.m_aid == packet->aid) {
    actor = this->scene->player;
  }

  if (!actor) {
    return;
  }

  auto* gameActor = static_cast<CGameActor*>(actor);
  if (StateFlag01(gameActor->Slot13()) == true || StateFlag02(gameActor->Slot13()) == true) {
    actor->SendMsg(nullptr, 109, 0, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  if (packet->state == 0) {
    actor->SendMsg(nullptr, 109, 0, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else if (packet->state != 1 || static_cast<EffectActor*>(actor)->state.Attachments().HasId(228)) {
    return;
  }

  CRagEffect* effect = static_cast<EffectActor*>(actor)->LaunchEffect(228, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (effect) {
    vector3d position{static_cast<float>(packet->value), 0.0f, 0.0f};
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
  }
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

#pragma pack(push, 1)
struct ShortcutPagePayload {
  /* 0000 */ NormalItemSlot slots[36];
  /* 00FC */ NormalItemSlot slot06EB;
  /* 0103 */ NormalItemSlot slot06F2;
};

struct QuestStateEntry {
  /* 0000 */ unsigned int questId;
  /* 0004 */ unsigned char state;
};

struct MessageBoxPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t captionId;
  /* 0004 */ int captionArgument;
  /* 0008 */ std::uint16_t textId;
  /* 000A */ int textArgument;
};

struct Item1784Entry {
  /* 0000 */ int itemId;
  /* 0004 */ int quantity;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::uint32_t unknown_0010;
  /* 0014 */ std::uint32_t unknown_0014;
};

struct Item1784ListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::int16_t flag;
  /* 0006 */ Item1784Entry entries[1];
};

struct ShortcutPagePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char position;
  /* 0003 */ std::uint16_t bank;
  /* 0005 */ ShortcutPagePayload payload;
};

struct CardCompositionPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t equipIndex;
  /* 0004 */ std::int16_t cardIndex;
  /* 0006 */ unsigned char result;
};

struct RefineResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t result;
  /* 0004 */ std::int16_t index;
  /* 0006 */ std::int16_t refine;
};

struct ItemEffectResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t type;
  /* 0004 */ int itemId;
};
#pragma pack(pop)

struct ClientItemLists {
  void ClearItems1784(int index);
  void RemoveItem1710(std::uint32_t id, int quantity);
};

// OFFSET: 0x00D18BD0
// FUNCTION: fn_21601
__declspec(noinline) void ReceiveShortcutPage(int position, const ShortcutPagePayload* payload, std::int16_t bank) {
  int base = 0;
  if (bank == 0) {
    g_session.ClearShortcutSlots();
    memcpy_s(g_session.m_shortcutSlots, sizeof(g_session.m_shortcutSlots), payload, sizeof(payload->slots));
    g_session.ClearSlots06EB();
    g_session.unknown_06EB = payload->slot06EB;
    g_session.unknown_06F2 = payload->slot06F2;
    base = g_session.unknown_06FC;
  } else if (bank == 1) {
    g_session.ClearSecondaryShortcutSlots();
    memcpy_s(g_session.m_secondaryShortcutSlots, sizeof(g_session.m_secondaryShortcutSlots), payload, sizeof(payload->slots));
    base = g_session.unknown_06A8;
  }

  if (g_windowMgr.window_01E8) {
    g_windowMgr.window_01E8->SendMsg(nullptr, 143, position - base + 4, bank, 0, 0);
  }
}

// OFFSET: 0x00D18F60
// FUNCTION: fn_21603
void CGameMode::ResendMinimapMarkers() {
  if (g_windowMgr.window_4F1CC) {
    for (auto it = this->minimapMarkers.begin(); it != this->minimapMarkers.end(); ++it) {
      if (it->second.unknown_0006 != 0) {
        g_windowMgr.window_4F1CC->SendMsg(nullptr, 485, it->second.unknown_0004, it->second.unknown_0006, it->second.unknown_0000, it->second.unknown_0002);
      }
    }
  }
}

// OFFSET: 0x00D69260
// FUNCTION: fn_22046
void CGameMode::ReceiveJobTitle(unsigned char result, int job) {
  if (result != 0) {
    return;
  }

  g_session.unknown_615C = job;
  std::string title = GetJobTitleString(job);
  CActorNameCard card = this->LookupNameCard(g_session.m_aid);
  this->nameCard.SetValue4(g_session.m_aid, title);

  UIWindow* window = g_windowMgr.LookupWindow(10);
  if (window) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D694C0
// FUNCTION: fn_22049
void CGameMode::ReceiveQuestStates(int count, const unsigned char* data) {
  const auto* entry = reinterpret_cast<const QuestStateEntry*>(data);
  for (int i = 0; i < count; ++i, ++entry) {
    const unsigned int questId = entry->questId;
    if (entry->state == 1) {
      g_renewQuestMgr->AddNewQuest(questId);
      UIWindow* window = g_windowMgr.LookupWindow(10002);
      if (window) {
        static_cast<CUIAdventureGuide*>(window)->MarkQuest(questId);
      }
    } else if (entry->state == 2) {
      g_renewQuestMgr->RemoveStatusRequest(questId);
    }
  }
}

// OFFSET: 0x00D6E1E0
// FUNCTION: fn_22098
void CGameMode::ReceiveMessageBox(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const MessageBoxPacket*>(data);

  std::string caption = LookupMessage(packet->captionId);
  if (packet->captionArgument) {
    const std::string formatted = FormatOwnedString(caption.c_str(), packet->captionArgument);
    caption = formatted;
  }

  std::string text = LookupMessage(packet->textId);
  if (packet->textArgument) {
    const std::string formatted = FormatOwnedString(text.c_str(), packet->textArgument);
    text = formatted;
  }

  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 0, true, false, 0, caption.c_str(), 280, 120, 0);
}

// OFFSET: 0x00D6E5F0
// FUNCTION: fn_22102
void CGameMode::NotifyCaptchaRegistered() {
  UIWindow* window = g_windowMgr.LookupWindow(284);
  if (window) {
    static_cast<UICaptchaRegisterWnd*>(window)->SetRegisterDisabled(0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2880), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00D6ECB0
// FUNCTION: fn_22120
void CGameMode::ReceiveItem1784List(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const Item1784ListPacket*>(data);
  if (g_session.unknown_5912 || packet->flag != 0) {
    return;
  }

  const unsigned int count = (packet->length - 6) / sizeof(Item1784Entry);
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1784(9);

  char name[256];
  std::memset(name, 0, sizeof(name));
  int capacity = sizeof(name);
  for (unsigned int i = 0; i < count; ++i) {
    Item1784Entry entry;
    memcpy_s(&entry, sizeof(entry), &packet->entries[i], sizeof(entry));

    ITEM_INFO item;
    item.unknown_0000[1] = -1;
    item.SetItemId(entry.itemId);
    item.unknown_0078[3] = entry.unknown_0008;
    item.quantity = entry.quantity;
    item.unknown_0014[0] = entry.unknown_000C;
    item.unknown_0078[1] = entry.unknown_0010;
    item.unknown_0078[2] = entry.unknown_0014;
    item.unknown_0064 = 8;
    item.identified = 1;
    ItemDescriptions descriptions{g_itemDescriptions};
    descriptions.Name(item, name, capacity, 0);
    item.SetName(name);
    g_session.unknown_1784[9].push_back(item);
  }

  UIWindow* window = g_windowMgr.LookupWindow(318);
  if (window) {
    window->SendMsg(nullptr, 23, 1, 0, 0, 0);
  }
}

// OFFSET: 0x00D48800
// FUNCTION: fn_21854
void CGameMode::ReceiveShortcutPagePacket(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ShortcutPagePacket*>(data);
  ReceiveShortcutPage(packet->position, &packet->payload, packet->bank);
}

// OFFSET: 0x00D496D0
// FUNCTION: fn_21858
void CGameMode::ReceiveCardCompositionResult(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const CardCompositionPacket*>(data);
  switch (packet->result) {
  case 0: {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(493)), 0xFFFF00, 0, 0, 0);
    ITEM_INFO card = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->cardIndex);
    ITEM_INFO equip = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->equipIndex);
    if (equip.unknown_0000[1] != 0 && card.unknown_0000[1] != 0) {
      int* cards = reinterpret_cast<int*>(equip.unknown_001C);
      for (int i = 0; i < 4; ++i) {
        if (cards[i] == 0) {
          cards[i] = std::atoi(card.itemId.c_str());
          break;
        }
      }

      reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1710(packet->cardIndex, 1);
      g_session.UpdateInventoryItem(equip, 1);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(494)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D499B0
// FUNCTION: fn_21859
void CGameMode::ReceiveRefineResult(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RefineResultPacket*>(data);
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
  if (item.unknown_0000[1] != 0) {
    item.unknown_0060 = packet->refine;
    g_session.UpdateInventoryItem(item, 1);
  }

  UIRefiningWnd* window = static_cast<UIRefiningWnd*>(g_windowMgr.LookupWindow(295));
  if (window) {
    switch (packet->result) {
    case 0:
      window->SetState(2, 3);
      break;
    case 1:
      window->SetState(2, 4);
      break;
    case 2:
      window->SetState(2, 5);
      break;
    case 3:
      window->SetState(2, 6);
      break;
    }
    return;
  }

  switch (packet->result) {
  case 0:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(154, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(498)), 0xFFFF00, 0, 0, 0);
    break;
  case 1:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(499)), 0xFFFF00, 0, 0, 0);
    break;
  case 2:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1537)), 0xFFFF00, 0, 0, 0);
    break;
  }

  ITEM_INFO refined = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
  refined.unknown_0060 = packet->refine;
  g_session.UpdateInventoryItem(refined, 1);
}

// OFFSET: 0x00D4A650
// FUNCTION: fn_21862
void CGameMode::ReceiveItemEffectResult(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemEffectResultPacket*>(data);
  ITEM_INFO item;
  item.SetItemId(packet->itemId);
  item.identified = 1;
  std::string text;

  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};

  switch (packet->type) {
  case 0:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(154, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(431), naming.DisplayName(item, 0).c_str()));
    break;
  case 1:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(430), naming.DisplayName(item, 0).c_str()));
    break;
  case 2:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->state.Attachments().HasId(305)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, 109, 0, 305, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(305, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(431), naming.DisplayName(item, 0).c_str()).c_str();
    break;
  case 3:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->state.Attachments().HasId(306)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, 109, 0, 306, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(306, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(430), naming.DisplayName(item, 0).c_str()).c_str();
    break;
  case 4:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->state.Attachments().HasId(1015)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, 109, 0, 1015, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1015, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(431), naming.DisplayName(item, 0).c_str()).c_str();
    break;
  case 5:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->state.Attachments().HasId(1016)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, 109, 0, 1016, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1016, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(430), naming.DisplayName(item, 0).c_str()).c_str();
    break;
  case 6: {
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->state.Attachments().HasId(1017)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, 109, 0, 1017, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1017, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(430), naming.DisplayName(item, 0).c_str()).c_str();
    UIWindow* window = g_windowMgr.LookupWindow(344);
    if (window) {
      window->SendMsg(nullptr, 6, 415, 0, 0, 0);
    }
    break;
  }
  case 7: {
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->state.Attachments().HasId(1018)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, 109, 0, 1018, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1018, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(430), naming.DisplayName(item, 0).c_str()).c_str();
    UIWindow* window = g_windowMgr.LookupWindow(344);
    if (window) {
      window->SendMsg(nullptr, 6, 415, 0, 0, 0);
    }
    break;
  }
  }
}

// OFFSET: 0x00D691A0
// FUNCTION: fn_22045
void CGameMode::ReceiveCharacterRenameResult(const unsigned char* packet) {
  switch (reinterpret_cast<const ByteValuePacket*>(packet)->value) {
  case 0: {
    PacketIdOnly request;
    request.id = 0xA48;
    GetRagConnection().Send(g_packetLengths.FixedLength(request.id), &request);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2836), 0, true, false, 0, "Message", 280, 120, 0);
    g_windowMgr.CloseWindow(281);
    break;
  }
  case 1:
  case 2:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1575), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  }
}

// OFFSET: 0x00D6B010
// FUNCTION: fn_22066
void CGameMode::ReceiveLapineUpgradeResult(std::int16_t result) {
  if (result != 0) {
    UIWindow* window = g_windowMgr.LookupWindow(290);
    if (window) {
      static_cast<UILapineUpgradeBoxWnd*>(window)->SetWaiting(0);
    }
  } else if (g_windowMgr.LookupWindow(302)) {
    g_windowMgr.CloseWindow(302);
  }
}

// OFFSET: 0x00D6B070
// FUNCTION: fn_22067
void CGameMode::ReceiveLapineBoxResult(std::uint16_t result) {
  if (result != 0) {
    UIWindow* window = g_windowMgr.LookupWindow(290);
    if (window) {
      static_cast<UILapineBoxWnd*>(window)->SetWaiting(0);
    }
  }

  switch (result) {
  case 0:
    if (g_windowMgr.LookupWindow(290)) {
      g_windowMgr.CloseWindow(290);
    }
    break;
  case 5:
  case 7: {
    UIWindow* window = g_windowMgr.LookupWindow(290);
    if (window) {
      window->SendMsg(nullptr, 111, reinterpret_cast<std::intptr_t>(LookupMessage(2897)), 0, 0, 0);
    }
    break;
  }
  }
}

// OFFSET: 0x00D6BB10
// FUNCTION: fn_22072
void CGameMode::ReceiveRouletteInfo(const unsigned char* packet) {
  const auto* info = reinterpret_cast<const RouletteInfoPacket*>(packet);
  for (int i = 0; i < 42; ++i) {
    RouletteSlot slot;
    slot.row = info->slots[i].row;
    slot.column = info->slots[i].column;
    slot.itemId = info->slots[i].itemId;
    slot.count = info->slots[i].count;
    g_rouletteMgr->AddItem(slot);
  }

  g_rouletteMgr->SetAdditionItem(g_rouletteMgr->m_additionItemId);
  if (g_rouletteMgr->m_resultShown == 1) {
    g_rouletteMgr->SetResult(static_cast<unsigned short>(g_rouletteMgr->m_index), 1, 0);
  }

  if (!g_windowMgr.unknown_0464) {
    g_windowMgr.EnsureWindow(269);
  }
}

// OFFSET: 0x00D6D080
// FUNCTION: fn_22084
void CGameMode::BeginCaptchaImage(unsigned short size, const char* key) {
  if (g_windowMgr.LookupWindow(285)) {
    return;
  }

  UIMacroDetectorWnd* window = static_cast<UIMacroDetectorWnd*>(g_windowMgr.EnsureWindow(285));
  if (window) {
    if (std::strncmp(window->m_captchaKey, key, 4) != 0) {
      window->ClearImageData();
    }
    window->Show(0);
    memcpy_s(window->m_captchaKey, 4, key, 4);
    window->InitImageData(size);
  }
}

// OFFSET: 0x00D6D130
// FUNCTION: fn_22085
void CGameMode::ReceiveCaptchaImageData(const char* key, const void* data, unsigned short size) {
  UIMacroDetectorWnd* window = static_cast<UIMacroDetectorWnd*>(g_windowMgr.LookupWindow(285));
  if (window && std::strncmp(window->m_captchaKey, key, 4) == 0) {
    window->AddImageData(data, size);
  }
}

// OFFSET: 0x00D6D180
// FUNCTION: fn_22086
void CGameMode::ReceiveWindow99Values(int count, const int* values) {
  if (count <= 0 || !values) {
    return;
  }

  char positive = 0;
  for (int i = 0; i < count; ++i) {
    if (values[i] > 0) {
      ++positive;
    }
  }

  if (positive > 0) {
    UIWindow* window = g_windowMgr.EnsureWindow(99);
    if (window) {
      for (int i = 0; i < count; ++i) {
        if (values[i] > 0) {
          window->SendMsg(nullptr, 31, values[i], 0, 0, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00D6E440
// FUNCTION: fn_22099
void sub_D6E440() {
  reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A8CF20)(&g_windowMgr, 0x154);
}

// OFFSET: 0x00D6E450
// FUNCTION: fn_22100
void CGameMode::ReceiveMacroBlackListResult(int result) {
  switch (result) {
  case 0:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2869), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 1:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2870), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 2:
    g_windowMgr.CloseWindow(359);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2871), 0, true, false, 0, "Message", 280, 120, 0);
    return;
  default:
    return;
  }

  GetRagConnection().Close();
  static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
}

// OFFSET: 0x00D6E520
// FUNCTION: fn_22101
void CGameMode::ReceiveMacroDetectorResult(int result) {
  switch (result) {
  case 0:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2869), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 1:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2870), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 2:
    g_windowMgr.CloseWindow(285);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(2871), 0, true, false, 0, "Message", 280, 120, 0);
    return;
  default:
    return;
  }

  GetRagConnection().Close();
  static_cast<CMode*>(g_charViewMode)->SendMsg(2, 0, 0, 0, 0);
}

// OFFSET: 0x00D71600
// FUNCTION: fn_22134
void CGameMode::ReceiveMacroBlackListCode(unsigned char state, const char* code) {
  UIMacroBlackListCheckWnd* window = static_cast<UIMacroBlackListCheckWnd*>(g_windowMgr.EnsureWindow(359));
  if (window) {
    window->SetCode(state, code);

    PacketIdOnly request;
    request.id = 0xBC4;
    GetRagConnection().Send(g_packetLengths.FixedLength(request.id), &request);
  }
}
