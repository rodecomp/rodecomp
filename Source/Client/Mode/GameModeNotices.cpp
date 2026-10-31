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
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/RodexMailStore.h"
#include "Contents/RouletteMgr.h"
#include "Contents/Schedule/CaptchaUploadSchedule.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/World/World.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Text/UITagTextViewer.h"
#include "UI/UICaptchaPreviewWnd.h"
#include "UI/UICaptchaRegisterWnd.h"
#include "UI/UIGradeEnchantWnd.h"
#include "UI/UILapineScrollBar.h"
#include "UI/UILapineUpgradeBoxWnd.h"
#include "UI/UIMacroBlackListCheckWnd.h"
#include "UI/UIMacroDetectorWnd.h"
#include "UI/UIMacroUserInfoWnd.h"
#include "UI/UIRefiningWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIRodexContentsWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <xtimec.h>

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

struct StatusPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t statusPoint;
  /* 0004 */ unsigned char stats[12];
  /* 0010 */ std::int16_t values[14];
};

struct StatusChangePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ unsigned char value;
};

struct StatusChangeAckPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ unsigned char result;
  /* 0005 */ unsigned char value;
};

struct ItemSlotOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};

struct ItemSlotAddOptionsPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ int slot;
  /* 000B */ unsigned char identified;
  /* 000C */ unsigned char damaged;
  /* 000D */ unsigned char refine;
  /* 000E */ unsigned char cards[16];
  /* 001E */ ItemSlotOption options[5];
  /* 0037 */ std::uint32_t expireTime;
  /* 003B */ std::uint16_t bindType;
};

struct ItemSlotAddGradePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ int slot;
  /* 000B */ unsigned char identified;
  /* 000C */ unsigned char damaged;
  /* 000D */ unsigned char cards[16];
  /* 001D */ ItemSlotOption options[5];
  /* 0036 */ std::uint32_t expireTime;
  /* 003A */ std::uint16_t bindType;
  /* 003C */ unsigned char refine;
  /* 003D */ unsigned char grade;
};

struct ItemOptionsRefreshPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t index;
  /* 0004 */ std::uint16_t refine;
  /* 0006 */ ItemSlotOption options[5];
};

struct PacketIdOnly {
  /* 0000 */ std::int16_t id;
};

struct AccountRequestPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
};

struct Window227ResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t first;
  /* 0004 */ std::uint16_t second;
  /* 0006 */ unsigned char result;
};

struct RouletteOpenPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ int serial;
  /* 0007 */ unsigned char stage;
  /* 0008 */ unsigned char index;
  /* 0009 */ int additionItemId;
  /* 000D */ int goldPoint;
  /* 0011 */ int silverPoint;
  /* 0015 */ int bronzePoint;
};

struct RodexOpenPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char text[24];
  /* 001A */ unsigned char result;
};

struct StackedItemReturnPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ std::int16_t index;
  /* 0005 */ std::uint16_t count;
  /* 0007 */ std::uint16_t value;
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

// INLINED
static int CountItemSlotOptions(const ItemSlotOption* options) {
  int count = 0;
  for (int index = 0; index < 5; ++index) {
    if (options[index].id != 0) {
      ++count;
    }
  }
  return count;
}

// OFFSET: 0x00D45740
// FUNCTION: fn_21839
void CGameMode::ReceiveStatus(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const StatusPacket*>(data);
  int* bonusStats = reinterpret_cast<int*>(g_session.unknown_169C);
  g_session.m_standardStats[0] = packet->stats[0];
  g_session.InvalidateWindowFrames();
  g_session.m_standardStats[1] = packet->stats[2];
  g_session.InvalidateWindowFrames();
  g_session.m_standardStats[2] = packet->stats[4];
  g_session.InvalidateWindowFrames();
  g_session.m_standardStats[3] = packet->stats[6];
  g_session.InvalidateWindowFrames();
  g_session.m_standardStats[4] = packet->stats[8];
  g_session.InvalidateWindowFrames();
  g_session.m_standardStats[5] = packet->stats[10];
  g_session.InvalidateWindowFrames();
  bonusStats[0] = packet->stats[1];
  g_session.InvalidateWindowFrames();
  bonusStats[1] = packet->stats[3];
  g_session.InvalidateWindowFrames();
  bonusStats[2] = packet->stats[5];
  g_session.InvalidateWindowFrames();
  bonusStats[3] = packet->stats[7];
  g_session.InvalidateWindowFrames();
  bonusStats[4] = packet->stats[9];
  g_session.InvalidateWindowFrames();
  bonusStats[5] = packet->stats[11];
  g_session.InvalidateWindowFrames();

  *reinterpret_cast<int*>(g_session.unknown_1654) = packet->statusPoint;
  g_session.InvalidateWindowFrames();

  bonusStats[7] = packet->values[0];
  g_session.InvalidateWindowFrames();
  bonusStats[12] = packet->values[1];
  g_session.InvalidateWindowFrames();
  bonusStats[13] = packet->values[2];
  g_session.InvalidateWindowFrames();
  bonusStats[14] = packet->values[3];
  g_session.InvalidateWindowFrames();
  bonusStats[10] = packet->values[4];
  g_session.InvalidateWindowFrames();
  bonusStats[11] = packet->values[5];
  g_session.InvalidateWindowFrames();
  bonusStats[8] = packet->values[6];
  g_session.InvalidateWindowFrames();
  bonusStats[15] = packet->values[7];
  g_session.InvalidateWindowFrames();
  bonusStats[16] = packet->values[8];
  g_session.InvalidateWindowFrames();
  bonusStats[17] = packet->values[9];
  g_session.InvalidateWindowFrames();
  bonusStats[19] = packet->values[10];
  g_session.InvalidateWindowFrames();
  bonusStats[18] = packet->values[11];
  g_session.InvalidateWindowFrames();
  bonusStats[6] = packet->values[12];
  g_session.InvalidateWindowFrames();
  bonusStats[9] = packet->values[13];
  g_session.InvalidateWindowFrames();

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01C4)) {
    window->SendMsg(nullptr, 35, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D45970
// FUNCTION: fn_21840
void CGameMode::ReceiveStatusChange(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const StatusChangePacket*>(data);
  const unsigned int type = packet->type;
  if (type >= 32 && type <= 37) {
    reinterpret_cast<int*>(g_session.unknown_169C)[type - 32] = packet->value;
    g_session.InvalidateWindowFrames();
  } else if (type >= 247 && type <= 252) {
    reinterpret_cast<std::int64_t*>(&g_session.unknown_7FC0[0x20])[type - 247] = packet->value;
    g_session.InvalidateWindowFrames();
  }

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01C4)) {
    window->SendMsg(nullptr, 35, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D45B90
// FUNCTION: fn_21841
void CGameMode::ReceiveStatusChangeAck(const unsigned char* data) {
  this->unknown_02D4 = 0;
  UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01C4);
  const auto* packet = reinterpret_cast<const StatusChangeAckPacket*>(data);
  if (!packet->result) {
    return;
  }

  int* bonusStats = reinterpret_cast<int*>(g_session.unknown_169C);
  switch (packet->type) {
  case 13:
  case 14:
  case 15:
  case 16:
  case 17:
  case 18:
    g_session.m_standardStats[packet->type - 13] = packet->value;
    g_session.InvalidateWindowFrames();
    if (window) {
      window->SendMsg(nullptr, 35, 0, 0, 0, 0);
    }
    break;
  case 41:
    bonusStats[7] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 42:
    bonusStats[12] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 43:
    bonusStats[13] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 44:
    bonusStats[14] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 45:
    bonusStats[10] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 46:
    bonusStats[11] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 47:
    bonusStats[8] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 48:
    bonusStats[15] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 49:
    bonusStats[16] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 50:
    bonusStats[17] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 51:
    bonusStats[19] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 52:
    bonusStats[18] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 53:
    bonusStats[6] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 54:
    bonusStats[9] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 55:
    g_session.m_jobLevel = packet->value;
    g_session.InvalidateWindowFrames();
    g_windowMgr.EnsureWindow(49);
    break;
  }
}

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
  void ClearItems1738();
  void ClearItems1784(int index);
  void RemoveItem1710(std::uint32_t id, int quantity);
};

struct AchievementStatePacket {
  /* 0000 */ std::int16_t unknown_0000;
  /* 0002 */ std::int16_t type;
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

// OFFSET: 0x00D1BC20
// FUNCTION: fn_21621
void __stdcall nullsub_467(int) {
}

// OFFSET: 0x00D1BE00
// FUNCTION: fn_21622
void __stdcall nullsub_468(int) {
}

// OFFSET: 0x00D6AD60
// FUNCTION: fn_22063
void __stdcall sub_D6AD60(int count, const int* aids) {
  if (count <= 0) {
    return;
  }

  UIMacroReporterWnd* window = static_cast<UIMacroReporterWnd*>(g_windowMgr.LookupWindow(286));
  if (!window) {
    return;
  }

  for (int i = 0; i < count; ++i) {
    window->sub_7EF2E0(aids[i]);
  }
}

// GLOBAL: gl_25111
int g_unknown_01738268;

// GLOBAL: gl_25112
int g_unknown_0173826C;

// GLOBAL: gl_25113
int g_unknown_01738270;

// GLOBAL: gl_25114
int g_unknown_01738784;

// GLOBAL: gl_25115
short g_unknown_01738788;

// GLOBAL: gl_24957
int g_achievementCompleted;

// GLOBAL: gl_24958
int g_achievementTotal;

// OFFSET: 0x00D1C050
// FUNCTION: fn_21624
void __stdcall sub_D1C050(const AchievementStatePacket* packet) {
  UIWindow* window = nullptr;
  bool notify = true;

  if (packet->type == 2) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x9F5), 0, true, true, 0, "Message", 280, 120, 0);
    window = g_windowMgr.LookupWindow(0x21);
  } else {
    window = g_windowMgr.LookupWindow(0x21);
    notify = window != nullptr;
  }

  if (notify) {
    window->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
    g_windowMgr.CloseWindow(0x21);
  }

  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1738();
  g_unknown_01738268 = 0;
  g_unknown_0173826C = 0;
  g_unknown_01738270 = 0;
}

// OFFSET: 0x00D6C460
// FUNCTION: fn_22081
void __stdcall sub_D6C460(int count, int unknown01738784, short unknown01738788, int completed, int total, const AchievementRecordPacket* packets) {
  g_unknown_01738784 = unknown01738784;
  g_unknown_01738788 = unknown01738788;
  g_achievementCompleted = completed;
  g_achievementTotal = total;

  for (int i = 0; i < count; ++i) {
    g_session.UpdateRecord6144(packets[i]);
    if (packets[i].rewarded == 1) {
      g_session.AddUnknown6160(packets[i].id);
    }
  }

  UIWindow* window = g_windowMgr.LookupWindow(0x10E);
  if (window != nullptr) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D1A0A0
// FUNCTION: fn_21615
void CGameMode::OpenServiceWindow(unsigned char type) {
  switch (type) {
  case 0:
    if (!g_windowMgr.LookupWindow(251)) {
      AccountRequestPacket request;
      request.id = 0x9AB;
      request.aid = g_session.m_aid;
      GetRagConnection().Send(6, &request);
    }
    break;
  case 1:
    if (g_windowMgr.LookupWindow(16)) {
      this->SendMsg(89, 0, 0, 0, 0);
    }
    if (!g_windowMgr.LookupWindow(281)) {
      g_windowMgr.EnsureWindow(281);
    }
    break;
  case 2:
    g_windowMgr.EnsureWindow(284);
    break;
  case 3:
    g_windowMgr.EnsureWindow(286);
    break;
  case 9:
    g_windowMgr.EnsureWindow(344);
    break;
  }
}

// OFFSET: 0x00D693E0
// FUNCTION: fn_22047
void CGameMode::ReceiveWindow328Result(int result, int value) {
  if (result == 0) {
    UIWindow* window = g_windowMgr.LookupWindow(328);
    if (window) {
      window->SendMsg(nullptr, 34, value * 10 + 1, 0, 0, 0);
    }
  } else if (result == 1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3472)), 0xFF, 0, 0, 0);
    g_windowMgr.CloseWindow(328);
  }
}

// OFFSET: 0x00D69460
// FUNCTION: fn_22048
void CGameMode::ReceiveRouletteClose(const unsigned char* packet) {
  switch (reinterpret_cast<const ByteValuePacket*>(packet)->value) {
  case 0:
    g_rouletteMgr->Reset();
    if (g_windowMgr.unknown_0464) {
      g_windowMgr.CloseWindow(269);
    }
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2633)), 0x6464FF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6A920
// FUNCTION: fn_22059
void CGameMode::ReceiveWindow227Result(const unsigned char* packet) {
  const auto* result = reinterpret_cast<const Window227ResultPacket*>(packet);
  const unsigned short first = result->first;
  const unsigned short second = result->second;

  switch (result->result) {
  case 0: {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2171)), 0x64FFFF, 0, 0, 0);
    UIWindow* window = g_windowMgr.LookupWindow(227);
    if (window) {
      window->SendMsg(nullptr, 60, first, second, 0, 0);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2172)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2173)), 0xFF, 0, 0, 0);
    break;
  }

  g_windowMgr.CloseWindow(227);
}

// OFFSET: 0x00D6AB60
// FUNCTION: fn_22061
void CGameMode::ReceiveRouletteOpen(const unsigned char* packet) {
  const auto* open = reinterpret_cast<const RouletteOpenPacket*>(packet);
  switch (open->result) {
  case 0:
    g_rouletteMgr->unknown_0004 = open->serial;
    g_rouletteMgr->m_stage = open->stage;
    g_rouletteMgr->SetAdditionItem(open->additionItemId);
    g_rouletteMgr->m_goldPoint = open->goldPoint;
    g_rouletteMgr->m_silverPoint = open->silverPoint;
    g_rouletteMgr->m_bronzePoint = open->bronzePoint;
    g_rouletteMgr->SetResult(open->index, open->index != 0xFF, 0);
    if (open->index != 0xFF) {
      g_rouletteMgr->SetQuad((open->index + open->stage) * 36 + 173, 341 - open->stage * 40,
        static_cast<int>(g_renderer->viewport.width * 0.5 - 320.0), static_cast<int>(g_renderer->viewport.height * 0.5 - 240.0));
    }

    if (g_rouletteMgr->m_items.size() == 0) {
      PacketIdOnly request;
      request.id = 0xA1B;
      GetRagConnection().Send(g_packetLengths.FixedLength(request.id), &request);
    } else if (!g_windowMgr.unknown_0464) {
      g_windowMgr.EnsureWindow(269);
    }
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2632)), 0x6464FF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6ACE0
// FUNCTION: fn_22062
void CGameMode::ReceiveRodexOpen(const unsigned char* packet) {
  const auto* open = reinterpret_cast<const RodexOpenPacket*>(packet);
  if (open->result != 1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2604)), 0x64FFFF, 0, 0, 0);
    return;
  }

  if (g_windowMgr.unknown_0458) {
    return;
  }

  g_session.RestoreItems5C68();
  g_session.ClearItemQuantities();
  g_windowMgr.EnsureWindow(264);
  const char* text = open->text;
  if (text) {
    reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.unknown_0458)->SetContentsText(text);
  }
}

// OFFSET: 0x00D6ADA0
// FUNCTION: fn_22064
void CGameMode::ReceiveCaptchaPreview(int result, unsigned short size, const char* key) {
  if (result == 0) {
    UICaptchaPreviewWnd* window = static_cast<UICaptchaPreviewWnd*>(g_windowMgr.LookupWindow(288));
    if (!window) {
      window = static_cast<UICaptchaPreviewWnd*>(g_windowMgr.EnsureWindow(288));
      if (!window) {
        return;
      }
    }

    if (std::strncmp(window->m_captchaKey, key, 4) != 0) {
      window->ClearImageData();
    }
    window->Show(0);
    memcpy_s(window->m_captchaKey, 4, key, 4);
    window->InitImageData(size);
  } else if (result == 1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("wrong captcha ID."), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D6B8F0
// FUNCTION: fn_22070
void CGameMode::ReceiveStackedItemReturn(const unsigned char* packet) {
  const auto* result = reinterpret_cast<const StackedItemReturnPacket*>(packet);
  if (result->result == 1) {
    g_session.ReturnStackedItem(result->index, result->count);
    g_session.unknown_6140 = result->value;
  } else {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2602)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D6E0B0
// FUNCTION: fn_22095
void CGameMode::ReceiveRodexRecipient(int id, unsigned short job, unsigned short level, const char* name) {
  if (id == 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2615)), 0x64FFFF, 0, 0, 0);
  } else if (g_windowMgr.unknown_0458) {
    reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->SetRecipient(id, job, level, name);
  }
}

struct BuyingStoreResultPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::int16_t result;
  /* 0004 */ std::int16_t value;
};

// OFFSET: 0x00D1D460
// FUNCTION: fn_21629
void __stdcall ReceiveBuyingStoreResult(const BuyingStoreResultPacket* packet) {
  char text[256];
  int message;
  const std::int16_t result = packet->result;
  if (result == 0) {
    std::memset(text, 0, 250);
    message = 0x9C0;
  } else if (result == 2) {
    std::memset(text, 0, 250);
    message = 0x9C1;
  } else if (result == 3) {
    g_unknown_01738270 = 1;
    std::memset(text, 0, 250);
    message = 0x9C2;
  } else if (result == 1) {
    g_unknown_01738268 = 1;
    g_unknown_0173826C = packet->value;
    if (packet->value != 0) {
      return;
    }
    std::memset(text, 0, 250);
    message = 0x9C5;
  } else {
    return;
  }

  std::sprintf(text, LookupMessage(message));
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D18BA0
// FUNCTION: fn_21600
void CGameMode::SetActorUnknown02E4(std::uint32_t aid, int value) {
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (actor) {
    actor->SetUnknown02E4(value);
  }
}

// OFFSET: 0x00D197D0
// FUNCTION: fn_21608
void CGameMode::IgnoreRemovedPacket13(const unsigned char*) {
}

// OFFSET: 0x00D197E0
// FUNCTION: fn_21609
void CGameMode::IgnoreRemovedPacket14(const unsigned char*) {
}

// OFFSET: 0x00D198E0
// FUNCTION: fn_21611
void CGameMode::IgnoreRemovedPacket15(const unsigned char*) {
}

// OFFSET: 0x00D19EE0
// FUNCTION: fn_21614
void CGameMode::IgnoreRemovedPacket16(const unsigned char*) {
}

// OFFSET: 0x00D6C080
// FUNCTION: fn_22078
void CGameMode::ReceiveItemSlotAddOptions(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemSlotAddOptionsPacket*>(data);
  if (packet->slot < 0) {
    g_modeMgr.gameMode->SendMsg(0x35, 0, 0, 0, 0);
  }

  if (packet->itemId == 0) {
    g_session.SetUnknown5214(packet->slot);
    return;
  }

  ITEM_INFO item;
  item.unknown_0000[0] = packet->type;
  item.SetItemId(packet->itemId);
  item.quantity = packet->slot;
  item.unknown_005D = packet->damaged;
  item.identified = packet->identified;
  item.unknown_0060 = packet->refine;
  std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
  item.unknown_0000[1] = 0xFFFFFFFF;
  item.unknown_0098 = CountItemSlotOptions(packet->options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemSlotOption*>(item.unknown_009C)[index] = packet->options[index];
  }
  item.unknown_0000[2] = packet->expireTime;
  item.unknown_0070 = packet->bindType;
  g_session.AddItemSlot(item);
}

// OFFSET: 0x00D6C240
// FUNCTION: fn_22079
void CGameMode::ReceiveItemSlotAddGrade(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemSlotAddGradePacket*>(data);
  if (packet->slot < 0) {
    g_modeMgr.gameMode->SendMsg(0x35, 0, 0, 0, 0);
  }

  if (packet->itemId == 0) {
    g_session.SetUnknown5214(packet->slot);
    return;
  }

  ITEM_INFO item;
  item.unknown_0000[0] = packet->type;
  item.SetItemId(packet->itemId);
  item.quantity = packet->slot;
  item.unknown_005D = packet->damaged;
  item.identified = packet->identified;
  item.unknown_0060 = packet->refine;
  std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
  item.unknown_0000[1] = 0xFFFFFFFF;
  item.unknown_0098 = CountItemSlotOptions(packet->options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemSlotOption*>(item.unknown_009C)[index] = packet->options[index];
  }
  item.unknown_0000[2] = packet->expireTime;
  item.unknown_0070 = packet->bindType;
  item.unknown_0088 = packet->grade;
  g_session.AddItemSlot(item);
}

// OFFSET: 0x00D6D7E0
// FUNCTION: fn_22090
void CGameMode::ReceiveGuildUnknown0114Change(unsigned char value, std::uint32_t removeAid, std::uint32_t addAid) {
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
  if (g_session.IsLocalPlayerAid(removeAid) == 1) {
    guildInfo->RemoveUnknown0114(value);
  } else if (g_session.IsLocalPlayerAid(addAid) == 0) {
    guildInfo->AddUnknown0114(value);
  }

  if (g_windowMgr.window_0320) {
    g_windowMgr.window_0320->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6DDD0
// FUNCTION: fn_22093
void CGameMode::ReceiveItemOptionsRefresh(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemOptionsRefreshPacket*>(data);
  if (!packet) {
    return;
  }

  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
  if (item.unknown_0000[1] != 0) {
    item.unknown_0060 = packet->refine;
    item.unknown_0098 = CountItemSlotOptions(packet->options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<ItemSlotOption*>(item.unknown_009C)[index] = packet->options[index];
    }
    g_session.UpdateInventoryItem(item, 1);
  }
}

// OFFSET: 0x00D06410
// FUNCTION: fn_21575
int GetBaseJob(int job) {
  switch (job) {
  case JT_NOVICE_H:
  case JT_NOVICE_B:
    return JT_NOVICE;
  case JT_SWORDMAN_H:
  case JT_SWORDMAN_B:
    return JT_SWORDMAN;
  case JT_MAGICIAN_H:
  case JT_MAGICIAN_B:
    return JT_MAGICIAN;
  case JT_ARCHER_H:
  case JT_ARCHER_B:
    return JT_ARCHER;
  case JT_ACOLYTE_H:
  case JT_ACOLYTE_B:
    return JT_ACOLYTE;
  case JT_MERCHANT_H:
  case JT_MERCHANT_B:
    return JT_MERCHANT;
  case JT_THIEF_H:
  case JT_THIEF_B:
    return JT_THIEF;
  case JT_KNIGHT_H:
  case JT_KNIGHT_B:
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_KNIGHT_B:
  case JT_DRAGON_KNIGHT:
    return JT_KNIGHT;
  case JT_PRIEST_H:
  case JT_PRIEST_B:
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
  case JT_CARDINAL:
    return JT_PRIEST;
  case JT_WIZARD_H:
  case JT_WIZARD_B:
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
  case JT_ARCH_MAGE:
    return JT_WIZARD;
  case JT_BLACKSMITH_H:
  case JT_BLACKSMITH_B:
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MECHANIC_B:
  case JT_MEISTER:
    return JT_BLACKSMITH;
  case JT_HUNTER_H:
  case JT_HUNTER_B:
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_RANGER_B:
  case JT_WINDHAWK:
    return JT_HUNTER;
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CROSS:
    return JT_ASSASSIN;
  case JT_CRUSADER_H:
  case JT_CRUSADER_B:
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_ROYAL_GUARD_B:
  case JT_IMPERIAL_GUARD:
    return JT_CRUSADER;
  case JT_MONK_H:
  case JT_MONK_B:
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
  case JT_INQUISITOR:
    return JT_MONK;
  case JT_SAGE_H:
  case JT_SAGE_B:
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
  case JT_ELEMENTAL_MASTER:
    return JT_SAGE;
  case JT_ROGUE_H:
  case JT_ROGUE_B:
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
  case JT_ABYSS_CHASER:
    return JT_ROGUE;
  case JT_ALCHEMIST_H:
  case JT_ALCHEMIST_B:
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
  case JT_BIOLO:
    return JT_ALCHEMIST;
  case JT_BARD_H:
  case JT_BARD_B:
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
  case JT_TROUBADOUR:
    return JT_BARD;
  case JT_DANCER_H:
  case JT_DANCER_B:
  case JT_WANDERER:
  case JT_WANDERER_H:
  case JT_WANDERER_B:
  case JT_TROUVERE:
    return JT_DANCER;
  case JT_SUPERNOVICE_B:
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_HYPER_NOVICE:
    return JT_SUPERNOVICE;
  case JT_REBELLION:
  case JT_GUNSLINGER_B:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    return JT_GUNSLINGER;
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_NINJA_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
    return JT_NINJA;
  case JT_TAEKWON_B:
    return JT_TAEKWON;
  case JT_STAR_B:
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
  case JT_SKY_EMPEROR:
    return JT_STAR;
  case JT_LINKER_B:
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
  case JT_SOUL_ASCETIC:
    return JT_LINKER;
  case JT_DO_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
    return JT_DO_SUMMONER;
  case JT_DRUID_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
  case JT_ALITEA:
    return JT_DRUID;
  }
  return job;
}

#pragma pack(push, 1)
struct TypedNoticePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t type;
  /* 0004 */ int first;
  /* 0008 */ int second;
};

struct RodexMailReturnPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int64_t mailId;
  /* 000A */ unsigned char type;
  /* 000B */ unsigned char result;
};

struct ActorStatusValuePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int status;
  /* 000A */ int value;
};

struct PartyMemberEffectPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int type;
};

struct ExtraInventoryResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ int itemId;
};
#pragma pack(pop)

// GLOBAL: gl_25117
int g_partyEffectNoticePending;

// OFFSET: 0x00D1A1C0
// FUNCTION: fn_21616
void CGameMode::ReceiveTypedNotice(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const TypedNoticePacket*>(data);
  switch (packet->type) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(901), packet->first, packet->second).c_str()), 0xCDCDCD, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(902), packet->first, packet->second).c_str()), 0xCDFFCD, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(926), packet->first, packet->second).c_str()), 0xFFCDCD, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(953), packet->first, packet->second).c_str()), 0xCDCDCD, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6BE80
// FUNCTION: fn_22076
void CGameMode::ReceiveRodexMailReturn(const unsigned char* data) {
  if (g_windowMgr.unknown_045C) {
    reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.unknown_045C)->SetZenyButtonDisabled(0);
  }

  const auto* packet = reinterpret_cast<const RodexMailReturnPacket*>(data);
  switch (packet->result) {
  case 0: {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2591)), 0x64FFFF, 0, 0, 0);

    std::map<std::int64_t, RodexMail>::iterator mail;
    switch (packet->type) {
    case 0:
      mail = g_rodexMailStore->m_mails0.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails0.end()) {
        mail->second.unknown_0001 -= 2;
      }
      break;
    case 1:
      mail = g_rodexMailStore->m_mails1.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails1.end()) {
        mail->second.unknown_0001 -= 2;
      }
      break;
    case 2:
      mail = g_rodexMailStore->m_mails2.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails2.end()) {
        mail->second.unknown_0001 -= 2;
      }
      break;
    }

    if (g_windowMgr.unknown_045C) {
      reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.unknown_045C)->DisableZenyButton();
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2592)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2593)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D73E00
// FUNCTION: fn_22144
void CGameMode::ReceiveActorStatusValuePacket(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorStatusValuePacket*>(data);
  this->ReceiveActorStatusValue(packet->aid, packet->status, packet->value, packet->value >> 31);
}

// OFFSET: 0x00D73E20
// FUNCTION: fn_22145
void CGameMode::ReceivePartyMemberEffect(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const PartyMemberEffectPacket*>(data);
  auto* actor = static_cast<EffectActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (!actor) {
    return;
  }

  switch (packet->type) {
  case 0:
    actor->LaunchEffect(371, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 1:
    actor->LaunchEffect(158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 2:
    actor->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 3:
    actor->LaunchEffect(154, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 4:
    g_partyEffectNoticePending = 1;
    break;
  case 5:
    actor->LaunchEffect(305, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 6:
    actor->LaunchEffect(306, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 7:
    actor->LaunchEffect(338, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 8:
    actor->LaunchEffect(337, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 9:
    actor->LaunchEffect(582, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
}

// OFFSET: 0x00D73960
// FUNCTION: fn_22141
void CGameMode::ShowFormattedNotice(unsigned short messageId, std::uint32_t color, int argument) {
  std::string text = FormatOwnedString(LookupMessage(messageId), argument);
  if (!text.empty()) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), color, messageId == 2962 ? 22 : 0, 0, 0);
  }
}

// OFFSET: 0x00D6BC40
// FUNCTION: fn_22074
void CGameMode::ReceiveCaptchaUploadRequest(const std::uint32_t* sessionId, int result) {
  int message;
  switch (result) {
  case 0: {
    auto* window = static_cast<UICaptchaRegisterWnd*>(g_windowMgr.LookupWindow(284));
    if (window) {
      std::uint64_t now = _Xtime_get_ticks();
      CCaptchaUploadSchedule* schedule = new CCaptchaUploadSchedule(now, sessionId, window->m_decompressedSize, window->m_decompressedData);
      reinterpret_cast<CTimeScheduler*>(g_timeScheduler)->AddSchedule(schedule);
      return;
    }
    message = 2881;
    break;
  }
  case 1:
    message = 2882;
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D6A9F0
// FUNCTION: fn_22060
void CGameMode::ReceiveExtraInventoryResult(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ExtraInventoryResultPacket*>(data);
  int message;
  switch (packet->result) {
  case 0: {
    const int nextSlots = g_session.unknown_7FBC + 110;
    const int slots = g_session.unknown_7FBC + 100;
    const char* name = FindItemDescription(packet->itemId).identifiedName;
    const int answer = reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(
      FormatOwnedString(LookupMessage(3561), name, slots, nextSlots).c_str(), 2, false, false, 0, "Message", 280, 120, 0);
    if (answer == 187) {
      this->SendMsg(311, 0, 0, 0, 0);
    } else {
      this->SendMsg(312, 0, 0, 0, 0);
    }
    return;
  }
  case 1:
    message = 3562;
    break;
  case 2:
    message = 3563;
    break;
  case 3:
    message = 3564;
    break;
  case 4:
    message = 3565;
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D6FEA0
// FUNCTION: fn_22128
void CGameMode::ReceiveWindow348Result(unsigned char result, std::int16_t index) {
  if (result == 0) {
    g_windowMgr.CloseWindow(348);
    g_effectMgr->Spawn(g_session.m_aid, 1608, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    if (item.unknown_0000[1] != 0) {
      UIWindow* window = g_windowMgr.EnsureWindow(12);
      if (window) {
        window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
      }
    }
  } else if (result == 3) {
    UIWindow* window = g_windowMgr.LookupWindow(348);
    if (window) {
      window->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(3848)), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D69E60
// FUNCTION: fn_22056
void CGameMode::ReceiveRodexItemReturn(const unsigned char* data) {
  if (g_windowMgr.unknown_045C) {
    reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.unknown_045C)->SetItemButtonDisabled(0);
  }

  const auto* packet = reinterpret_cast<const RodexMailReturnPacket*>(data);
  switch (packet->result) {
  case 0: {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2588)), 0x64FFFF, 0, 0, 0);
    g_session.ClearItemQuantities();

    std::map<std::int64_t, RodexMail>::iterator mail;
    switch (packet->type) {
    case 0:
      mail = g_rodexMailStore->m_mails0.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails0.end()) {
        mail->second.unknown_0001 -= 4;
      }
      break;
    case 1:
      mail = g_rodexMailStore->m_mails1.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails1.end()) {
        mail->second.unknown_0001 -= 4;
      }
      break;
    case 2:
      mail = g_rodexMailStore->m_mails2.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails2.end()) {
        mail->second.unknown_0001 -= 4;
      }
      break;
    }

    if (g_windowMgr.unknown_045C) {
      auto* contents = reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.unknown_045C);
      contents->SendMsg(nullptr, 23, 0, 0, 0, 0);
      contents->DisableItemButton();
    }
    if (g_windowMgr.unknown_0454) {
      reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0454)->SendMsg(nullptr, 60, 0, 0, 0, 0);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2589)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2590)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

#pragma pack(push, 1)
struct ExpGainNoticePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::int64_t amount;
  /* 000E */ std::int16_t type;
  /* 0010 */ std::int16_t quest;
};
#pragma pack(pop)

// OFFSET: 0x00D74080
// FUNCTION: fn_22146
void CGameMode::ReceiveExpGainNotice(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ExpGainNoticePacket*>(data);
  std::string text;
  if (g_session.m_aid != packet->aid) {
    return;
  }

  const std::uint32_t color = packet->quest == 1 ? 0xBE58BE : 0xFFFF;
  if (packet->amount == 0) {
    return;
  }

  char buffer[256];
  if (packet->type == 1) {
    std::sprintf(buffer, LookupMessage(3220), packet->amount);
    text += buffer;
  } else if (packet->type == 2) {
    std::sprintf(buffer, LookupMessage(3227), packet->amount);
    text += buffer;
  }

  if (packet->amount < 0) {
    text += LookupMessage(1616);
  } else {
    text += LookupMessage(1615);
  }
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), color, 18, 0, 0);
}
