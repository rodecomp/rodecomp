#include "UI/Chat/UINewChatWnd.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Stove/StoveMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Item.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/ClientState.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/Effect.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/MapPropertyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/NavigationManager.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/RodexMailStore.h"
#include "Contents/RouletteMgr.h"
#include "Contents/Schedule/CaptchaUploadSchedule.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "File/ShortcutFilename.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/NoticeEnums.h"
#include "Render/Renderer.h"
#include "Resource/World/TerrainResource.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/CashShop/UICashShopWnd.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Guild/UIGuild_Storage_Log.h"
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
#include "UI/Status/UIStatusWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIRodexContentsWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Math/AffineMatrix.h"
#include "UI/UIBroadcastBalloon.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/UITextViewer.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <xtimec.h>
#include "Resource/MSGSTRINGID.h"


// GLOBAL: gl_25441
static const std::string g_whisperItemLinkTag = "<ITEML>";

#include "UI/Rodex/UIRodexInboxWnd.h"
#include "Network/Packets/PacketId.h"
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

struct StatusChangeEffectPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::int16_t type;
  /* 0008 */ std::uint32_t remaining;
  /* 000C */ int value1;
  /* 0010 */ int value2;
  /* 0014 */ int value3;
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
  /* 0004 */ int serial;
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
  int* bonusStats = reinterpret_cast<int*>(g_session.combatStats);
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

  *reinterpret_cast<int*>(g_session.statusPoints) = packet->statusPoint;
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

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.statusWnd)) {
    window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D45970
// FUNCTION: fn_21840
void CGameMode::ReceiveStatusChange(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const StatusChangePacket*>(data);
  switch (packet->type) {
  case SP_USTR:
    reinterpret_cast<int*>(g_session.combatStats)[0] = packet->value;
    break;
  case SP_UAGI:
    reinterpret_cast<int*>(g_session.combatStats)[1] = packet->value;
    break;
  case SP_UVIT:
    reinterpret_cast<int*>(g_session.combatStats)[2] = packet->value;
    break;
  case SP_UINT:
    reinterpret_cast<int*>(g_session.combatStats)[3] = packet->value;
    break;
  case SP_UDEX:
    reinterpret_cast<int*>(g_session.combatStats)[4] = packet->value;
    break;
  case SP_ULUK:
    reinterpret_cast<int*>(g_session.combatStats)[5] = packet->value;
    break;
  case SP_UPOW:
    g_session.m_traits.points[0] = packet->value;
    break;
  case SP_USTA:
    g_session.m_traits.points[1] = packet->value;
    break;
  case SP_UWIS:
    g_session.m_traits.points[2] = packet->value;
    break;
  case SP_USPL:
    g_session.m_traits.points[3] = packet->value;
    break;
  case SP_UCON:
    g_session.m_traits.points[4] = packet->value;
    break;
  case SP_UCRT:
    g_session.m_traits.points[5] = packet->value;
    break;
  default:
    if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.statusWnd)) {
      window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    return;
  }

  g_session.InvalidateWindowFrames();

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.statusWnd)) {
    window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6EB40
// FUNCTION: fn_22115
void CGameMode::ReceiveStatusChangeEffect(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const StatusChangeEffectPacket*>(data);
  this->ApplyStatusChange(packet->aid, 1, packet->type, packet->remaining, packet->value1, packet->value2, packet->value3, 1, 0);
}

// OFFSET: 0x00D45B90
// FUNCTION: fn_21841
void CGameMode::ReceiveStatusChangeAck(const unsigned char* data) {
  this->waitingReqStatusAck = 0;
  UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.statusWnd);
  const auto* packet = reinterpret_cast<const StatusChangeAckPacket*>(data);
  if (!packet->result) {
    return;
  }

  int* bonusStats = reinterpret_cast<int*>(g_session.combatStats);
  switch (packet->type) {
  case SP_STR:
    g_session.m_standardStats[0] = packet->value;
    g_session.InvalidateWindowFrames();
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case SP_AGI:
    g_session.m_standardStats[1] = packet->value;
    g_session.InvalidateWindowFrames();
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case SP_VIT:
    g_session.m_standardStats[2] = packet->value;
    g_session.InvalidateWindowFrames();
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case SP_INT:
    g_session.m_standardStats[3] = packet->value;
    g_session.InvalidateWindowFrames();
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case SP_DEX:
    g_session.m_standardStats[4] = packet->value;
    g_session.InvalidateWindowFrames();
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case SP_LUK:
    g_session.m_standardStats[5] = packet->value;
    g_session.InvalidateWindowFrames();
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case SP_ATK1:
    bonusStats[7] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_ATK2:
    bonusStats[12] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MATK1:
    bonusStats[13] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MATK2:
    bonusStats[14] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_DEF1:
    bonusStats[10] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_DEF2:
    bonusStats[11] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MDEF1:
    bonusStats[8] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MDEF2:
    bonusStats[15] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_HIT:
    bonusStats[16] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_FLEE1:
    bonusStats[17] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_FLEE2:
    bonusStats[19] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_CRITICAL:
    bonusStats[18] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_ASPD:
    bonusStats[6] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_36:
    bonusStats[9] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_JOBLEVEL:
    g_session.m_jobLevel = packet->value;
    g_session.InvalidateWindowFrames();
    g_windowMgr.EnsureWindow(WID_NOTIFYJOBLEVELUPWND);
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

  actor->SendMsg(nullptr, AM_STOPMOVE, 0, static_cast<int>(static_cast<std::int64_t>(x)), static_cast<int>(static_cast<std::int64_t>(x) >> 32),
    static_cast<int>(static_cast<std::int64_t>(y)), static_cast<int>(static_cast<std::int64_t>(y) >> 32), 0, 0,
    static_cast<int>(static_cast<std::int64_t>(flag)), static_cast<int>(static_cast<std::int64_t>(flag) >> 32), 0, 0, 0, 0);
}

// OFFSET: 0x00D69140
// FUNCTION: fn_22044
void CGameMode::NotifyCaptchaRegistrationResult(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_COMPLETE_REGISTER_CAPTCHA)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_FAILED_REGISTER_CAPTCHA)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_FAILED_REGISTER_ANSWER)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D69700
// FUNCTION: fn_22051
void CGameMode::ReceiveRodexMailDeleted(const unsigned char* packet) {
  const auto* deleted = reinterpret_cast<const RodexMailDeletedPacket*>(packet);
  RodexMailMap::iterator mail;
  RodexMailMap::iterator end;

  switch (deleted->type) {
  case MAIL_INBOX_NORMAL:
    mail = g_rodexMailStore->m_mails0.find(deleted->mailId);
    end = g_rodexMailStore->m_mails0.end();
    break;
  case MAIL_INBOX_ACCOUNT:
    mail = g_rodexMailStore->m_mails1.find(deleted->mailId);
    end = g_rodexMailStore->m_mails1.end();
    break;
  case MAIL_INBOX_RETURNED:
    mail = g_rodexMailStore->m_mails2.find(deleted->mailId);
    end = g_rodexMailStore->m_mails2.end();
    break;
  default:
    goto refresh;
  }

  if (mail != end) {
    mail->second.isReturned = 1;
    if (!mail->second.isRead) {
      --g_rodexMailStore->unreadCount;
      if (g_rodexMailStore->unreadCount == 0) {
        UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND);
        if (window) {
          window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 476, 0, 0);
        }
      }
    }
  }

refresh:
  if (g_windowMgr.rodexInboxWnd) {
    reinterpret_cast<UIWindow*>(g_windowMgr.rodexInboxWnd)->Invalidate();
  }
}

// OFFSET: 0x00D69850
// FUNCTION: fn_22052
void CGameMode::NotifyInventoryExpansionResult(const unsigned char* packet) {
  switch (reinterpret_cast<const ByteValuePacket*>(packet)->value) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUY_ONE_AT_A_TIME)), 0xFFFF00, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVENTORY_EXTEND_FAILED_MAX_OVER)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVENTORY_EXTEND_SUCCESS)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LIMIT_BUY_ITEM3)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXCEED_LIMITED_ITEMNUM)), 0xFF, 0, 0, 0);
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
  case GENERATE_ROULETTE_SUCCESS:
    g_rouletteMgr->m_stage = result->stage;
    g_rouletteMgr->SetAdditionItem(result->additionItemId);
    bonus = 0;
    break;
  case GENERATE_ROULETTE_LOSING:
    g_rouletteMgr->m_stage = result->stage;
    g_rouletteMgr->SetAdditionItem(result->additionItemId);
    bonus = 1;
    break;
  case GENERATE_ROULETTE_FAILED:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_RECV_ITEM_OVERCOUNT)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.roulletteWnd) {
      g_windowMgr.EnableRouletteWnd();
    }
    return;
  case GENERATE_ROULETTE_NO_ENOUGH_POINT:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_RECV_ITEM_OVERWEIGHT)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.roulletteWnd) {
      g_windowMgr.EnableRouletteWnd();
    }
    return;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_JPN_PERSONALINFO_DEATHMSG)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.roulletteWnd) {
      g_windowMgr.EnableRouletteWnd();
    }
    return;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RODEX_DELETE)), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.roulletteWnd) {
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
void CGameMode::NotifyMacroBlacklistCheckResult(unsigned char result, unsigned short value) {
  char text[256];

  switch (result) {
  case 0:
    std::memset(text, 0, sizeof(text));
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_RESULT_FAIL_PARAM), value);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_RESULT_FAIL_QUESTION)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_RESULT_FAIL_MAPNAME_COUNT)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_REMAIN_TIME)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_NOTICE1)), 0xFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_NOTICE2)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6AE70
// FUNCTION: fn_22065
void CGameMode::NotifyPrivateAirplaneResult(int result) {
  UIWindow* window = g_windowMgr.privateAirplaneWnd;
  if (!window) {
    return;
  }

  switch (result) {
  case 0:
    if (!g_windowMgr.modalWindows.empty() && g_windowMgr.modalWindows.back()) {
      g_windowMgr.modalResponse = 188;
    }
    break;
  case 1:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PRIVATE_AIRPLANE_FAIL_LEVEL)), 0, 0, 0);
    break;
  case 2:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CREATE_ACCOUNT)), 0, 0, 0);
    break;
  case 3:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_REGISTER_NAME_THAT_CONTAINS_BADWORDS)), 0, 0, 0);
    break;
  case 4:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ENTER_ONE_LINE_WITHOUT_LINE_BREAKS)), 0, 0, 0);
    break;
  case 5:
    window->SendMsg(nullptr, 154, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MAKECHARWND_TITLE)), 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUSY)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6B950
// FUNCTION: fn_22071
void CGameMode::ReceiveNamedResult(std::uint32_t, std::string name, int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_USER_REPORT_EFST_ACTIVATED)), 0x64FFFF, 0, 0, 0);
    break;
  case 1: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_MACRO_USER_REPORT_PENALTY_COUNTMAX), name.c_str()).c_str()), 0x64FFFF, 0, 0, 0);

    CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    if (scene) {
      scene->player->LaunchEffect(41, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  }
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_USER_REPORT_REPORTED)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DECOM_NUM)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RESET_CASH_EMOTION_INFO)), 0x64FFFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_ALREADY_EXIST)), 0x64FFFF, 0, 0, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ENCHANT_MARK_1)), 0x64FFFF, 0, 0, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_DETECTOR_NOTICE4)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6BBC0
// FUNCTION: fn_22073
void CGameMode::ReceiveBasicInfoResult(unsigned char result) {
  switch (result) {
  case 1:
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_USESKILL_FAIL_NEED_CHARMS)), 0xFF, 0, 0, 0);
    if (g_windowMgr.equipWnd) {
      g_windowMgr.equipWnd->SendMsg(nullptr, UM_SET_DISABLE, 575, 0, 0, 0);
    }
    break;
  }

  if (g_windowMgr.equipWnd) {
    g_windowMgr.equipWnd->SendMsg(nullptr, UM_SET_DISABLE, 575, 0, 0, 0);
  }
}

// OFFSET: 0x00D6BD40
// FUNCTION: fn_22075
void CGameMode::ReceiveRodexComposeResult(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ByteValuePacket*>(data);

  switch (packet->value) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GET_ITEMS_FROM_MAIL_OVERWEIGHT)), 0x64FFFF, 0, 0, 0);
    break;
  case 1:
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_WRITE_RODEX, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EA6)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_WRITE_RODEX, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GOLDPCCAFE_MILEAGE)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_WRITE_RODEX, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PARTY_BOOKING_ERR5R2)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_WRITE_RODEX, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_WND_OVERFULL)), 0x64FFFF, 0, 0, 0);
    break;
  }

  if (g_windowMgr.rodexInboxWnd) {
    reinterpret_cast<UIWindow*>(g_windowMgr.rodexInboxWnd)->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6BFA0
// FUNCTION: fn_22077
void CGameMode::ReceiveWindow154Result(std::int16_t value) {
  switch (value) {
  case 0: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_ACCOUNTLIMITEDTOOLWND);
    if (window) {
      window->SendMsg(nullptr, UM_PARAM, 1, 0, 0, 0);
    }
    break;
  }
  case 1: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_ACCOUNTLIMITEDTOOLWND);
    if (window) {
      window->SendMsg(nullptr, UM_PARAM, 1, 1, 0, 0);
    }
    break;
  }
  case 3: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_ACCOUNTLIMITEDTOOLWND);
    if (window) {
      window->SendMsg(nullptr, UM_PARAM, 1, 3, 0, 0);
    }
    break;
  }
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BARGAINSALE_CASHSHOP_TAB_NAME)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6D200
// FUNCTION: fn_22087
void CGameMode::ReceiveResultNotice(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ETC_PROBABILITY_WEB_CONNECT)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GENDER_CHANGE_FAILED_CAUSE_JOB)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GENDER_CHANGE_FAILED_CAUSE_GROUP)), 0xFF, 0, 0, 0);
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
      first->render.colorLocked = 1;
      first->SetColor(-1, 64, 64, 64);
      first->render.instant.Set(3, 1);
      second->render.instant.Set(3, 1);
      second->render.isMotionFreezed = true;
      second->render.colorLocked = 1;
      second->SetColor(-1, 64, 64, 64);
    }
  } else {
    CRenderObject* first = packet->firstAid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(packet->firstAid);
    CRenderObject* second = packet->secondAid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(packet->secondAid);
    if (first) {
      first->SetFrameSequence(96, 4, 4, 96, 4, 1, 1, 0);
      first->render.colorLocked = 0;
      first->SetColor(-1, 255, 255, 255);
      first->render.instant.Set(3, 0);
    }

    if (second) {
      second->render.isMotionFreezed = false;
      second->render.colorLocked = 0;
      second->SetColor(-1, 255, 255, 255);
      second->render.instant.Set(3, 0);
    }
  }
}

// OFFSET: 0x00D6E100
// FUNCTION: fn_22096
void CGameMode::RefreshWindow16(const unsigned char*) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_SAYDIALOGWND);
  if (window) {
    window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6E130
// FUNCTION: fn_22097
void CGameMode::ReceiveEffectRemove(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorEffectPacket*>(data);

  CRenderObject* actor = this->scene->GetGameActorByAidIncludeMe(packet->aid);
  if (actor) {
    while (static_cast<EffectActor*>(actor)->HasId(packet->effectId)) {
      const std::int64_t effectId = packet->effectId;
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, static_cast<int>(effectId), static_cast<int>(effectId >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  while (g_effectMgr->Find(packet->aid, packet->effectId)) {
    g_effectMgr->Retire(packet->aid, packet->effectId);
  }
}

// OFFSET: 0x00D6E930
// FUNCTION: fn_22107
void CGameMode::ReceiveToggle128(int enabled) {
  SetPetInfoAlert(enabled != 0);
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
void CGameMode::ReceiveCoupleName(const unsigned char* data) {
  std::sprintf(this->coupleName, reinterpret_cast<const char*>(data + 2));
}

// OFFSET: 0x00D6E9F0
// FUNCTION: fn_22110
void CGameMode::ReceiveWindow154Close(std::int16_t value) {
  if (value == 0) {
    UIWindow* window = g_windowMgr.LookupWindow(WID_ACCOUNTLIMITEDTOOLWND);
    if (window) {
      window->SendMsg(nullptr, UM_PARAM, 2, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D6EA30
// FUNCTION: fn_22111
void CGameMode::SetWindow16Align(unsigned char align) {
  UISayDialogWnd* window = static_cast<UISayDialogWnd*>(g_windowMgr.LookupWindow(WID_SAYDIALOGWND));
  if (window) {
    static_cast<UITagTextViewer*>(window->m_viewer)->SetAlign(align);
  }
}

// OFFSET: 0x00D6EA60
// FUNCTION: fn_22112
void CGameMode::PlaceWindow16(unsigned int xPercent, unsigned int yPercent) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_SAYDIALOGWND);
  if (window) {
    window->Move(static_cast<int>((g_renderer->viewport.width - window->geometry.width) * (xPercent * 0.01)),
      static_cast<int>((g_renderer->viewport.height - window->geometry.height) * (yPercent * 0.01)));
  }
}

// OFFSET: 0x00D6EB00
// FUNCTION: fn_22113
void CGameMode::MoveWindow16(int x, int y) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_SAYDIALOGWND);
  if (window) {
    window->Move(x, y);
  }
}

// OFFSET: 0x00D6EB20
// FUNCTION: fn_22114
void CGameMode::ResizeWindow16(int width, int height) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_SAYDIALOGWND);
  if (window) {
    window->Resize(width, height);
  }
}

// OFFSET: 0x00D6EBD0
// FUNCTION: fn_22117
void CGameMode::ReceiveExtraInventorySlots(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ShortValuePacket*>(data);

  if (100 >= packet->value) {
    g_session.extraInventorySlots = packet->value;
    if (g_windowMgr.itemWnd) {
      reinterpret_cast<UIWindow*>(g_windowMgr.itemWnd)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D6EC10
// FUNCTION: fn_22118
void CGameMode::ReceiveZoneItemRetrievalFailure(const unsigned char*) {
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_GET_ITEM_FROM_ZONEDA)), 0xFF, 0, 0, 0);
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
    const std::int64_t x = packet->x;
    const std::int64_t y = packet->y;
    actor->SendMsg(nullptr, AM_FALLEN_ANGEL, 0, static_cast<int>(x), static_cast<int>(x >> 32), static_cast<int>(y), static_cast<int>(y >> 32), 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6EF60
// FUNCTION: fn_22121
void CGameMode::NotifyGmCheckerResult(int result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GAME_SETTINGS_TAB_BASIC)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GAME_SETTINGS_TAB_EFFECT)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GAME_SETTINGS_TAB_CONTROL)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHECK_CHAR_DELETE_TEXT)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GAME_SETTINGS_TAB_GRAPHICS)), 0xFF, 0, 0, 0);
    break;
  }
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
  if (-1 == packet->index) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHANGE_MATERIAL_MESSAGE)), 0x62B0FF, 0, 0, 0);
    return;
  }

  auto* window = static_cast<UIGradeEnchantWnd*>(g_windowMgr.LookupWindow(WID_GRADEENCHANTWND));
  if (window) {
    window->SetEnchantInfo(packet->index, packet->value, count, packet->info, packet->value1A, packet->value1E,
      reinterpret_cast<const GradeEnchantMaterial*>(data + sizeof(GradeEnchantInfoPacket)));
  }
}

// OFFSET: 0x00D6FCD0
// FUNCTION: fn_22127
void CGameMode::ReceivePartyItemPickup(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const PartyItemPickupPacket*>(data);
  MessengerMember member = g_session.PartyByAccount(packet->aid);
  if (packet->aid != g_session.m_aid && member.valid) {
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->itemId;
    item.identity[3] = 0;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    item.grade = packet->grade;

    std::string name = item.DisplayName(0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ITEM_PICKUP_PARTY), member.name.c_str(), name.c_str()).c_str()), 0xFFFF00, 9, 0, 0);
  }
}

// OFFSET: 0x00D71660
// FUNCTION: fn_22135
void CGameMode::NotifyMacroCheckRemovalResult(unsigned char result) {
  switch (result) {
  case 0:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_CHECK_REMOVE_OFF_SUCCESS)), 0xFFFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_CHECK_REMOVE_TIMESUP)), 0xFFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_CHECK_REMOVE_INVALID_AID)), 0xFFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NEWSEIGEMODE_SKILL)), 0xFFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DYNAMICNPC_CREATE_RESULT_FAIL_MAXCOUNT)), 0xFFFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_DETECTOR_NOTICE4)), 0xFFFF, 0, 0, 0);
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
  if (StateFlag01(gameActor->GetStateFlags()) == true || StateFlag02(gameActor->GetStateFlags()) == true) {
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  if (packet->state == 0) {
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else if (packet->state != 1 || static_cast<EffectActor*>(actor)->HasId(228)) {
    return;
  }

  CRagEffect* effect = static_cast<EffectActor*>(actor)->LaunchEffect(228, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (effect) {
    vector3d position{static_cast<float>(packet->value), 0.0f, 0.0f};
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D6FAC0
// FUNCTION: fn_22125
void CGameMode::ReceiveGuildAgitList(const unsigned char* packet) {
  const auto* header = reinterpret_cast<const VariablePacketHeader*>(packet);
  GuildInfo* guildInfo = &g_guildInfo;

  guildInfo->ClearAgitIds();
  const int count = header->length - 4;
  for (int i = 0; i < count; ++i) {
    unsigned char value;
    memcpy_s(&value, sizeof(value), packet + sizeof(VariablePacketHeader) + i, sizeof(value));
    guildInfo->AddAgitId(value);
  }
}

#pragma pack(push, 1)
struct ShortcutPagePayload {
  /* 0000 */ NormalItemSlot slots[36];
  /* 00FC */ NormalItemSlot quickSlot;
  /* 0103 */ NormalItemSlot alternateQuickSlot;
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
  /* 0008 */ std::uint32_t purchaseParameter;
  /* 000C */ std::uint32_t price;
  /* 0010 */ std::uint32_t saleParameter1;
  /* 0014 */ std::uint32_t saleParameter2;
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


struct AchievementStatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t type;
};

// OFFSET: 0x00D18BD0
// FUNCTION: fn_21601
__declspec(noinline) void ReceiveShortcutPage(int position, const ShortcutPagePayload* payload, std::int16_t bank) {
  int base = 0;
  if (0 == bank) {
    g_session.ClearShortcutSlots();
    memcpy_s(g_session.m_shortcutSlots, sizeof(g_session.m_shortcutSlots), payload, sizeof(payload->slots));
    g_session.ClearQuickSlots();
    g_session.quickSlot = payload->quickSlot;
    g_session.alternateQuickSlot = payload->alternateQuickSlot;
    base = g_session.mainShortcutRow;
  } else if (1 == bank) {
    g_session.ClearSecondaryShortcutSlots();
    memcpy_s(g_session.m_secondaryShortcutSlots, sizeof(g_session.m_secondaryShortcutSlots), payload, sizeof(payload->slots));
    base = g_session.alternateShortcutRow;
  }

  if (g_windowMgr.shortCutWnd) {
    g_windowMgr.shortCutWnd->SendMsg(nullptr, UM_ROTATE_SHORTCUT_ROWS, position - base + 4, bank, 0, 0);
  }
}

// OFFSET: 0x00D18F60
// FUNCTION: fn_21603
void CGameMode::ResendMinimapMarkers() {
  if (g_windowMgr.miniMapWnd) {
    for (auto it = this->minimapMarkers.begin(); it != this->minimapMarkers.end(); ++it) {
      if (0 == it->second.questIconId) {
        continue;
      }

      g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_ADD_QUEST_MARK, it->second.effectType, it->second.questIconId, it->second.x, it->second.y);
    }
  }
}

// OFFSET: 0x00D69260
// FUNCTION: fn_22046
void CGameMode::ReceiveJobTitle(unsigned char result, int job) {
  if (result != 0) {
    return;
  }

  g_session.jobTitle = job;
  std::string title = GetJobTitleString(job);
  CActorNameCard card = this->LookupNameCard(g_session.m_aid);
  this->nameCard.SetValue4(g_session.m_aid, title);

  UIWindow* window = g_windowMgr.LookupWindow(WID_EQUIPWND);
  if (window) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D694C0
// FUNCTION: fn_22049
void CGameMode::ReceiveQuestStates(int count, const unsigned char* data) {
  for (int i = 0; i < count; ++i) {
    const auto* entry = reinterpret_cast<const QuestStateEntry*>(data) + i;
    const unsigned int questId = entry->questId;
    if (entry->state == 1) {
      g_renewQuestMgr->AddNewQuest(questId);
      UIWindow* window = g_windowMgr.LookupWindow(WID_ADVENTUREGUIDE);
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

  g_windowMgr.Confirm(text.c_str(), 0, true, false, 0, caption.c_str(), 280, 120, 0);
}

// OFFSET: 0x00D6E5F0
// FUNCTION: fn_22102
void CGameMode::NotifyCaptchaRegistered() {
  UIWindow* window = g_windowMgr.LookupWindow(WID_CAPTCHAREGISTERWND);
  if (window) {
    static_cast<UICaptchaRegisterWnd*>(window)->SetRegisterDisabled(0);
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_DETECTOR_NOTICE1), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

// OFFSET: 0x00D6ECB0
// FUNCTION: fn_22120
void CGameMode::ReceiveItem1784List(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const Item1784ListPacket*>(data);
  if (g_session.characterRenameEnabled || packet->flag != 0) {
    return;
  }

  const unsigned int count = (packet->length - 6) / sizeof(Item1784Entry);
  g_session.ClearInventoryCategoryItems(9);

  char name[256];
  std::memset(name, 0, sizeof(name));
  int capacity = sizeof(name);
  for (unsigned int i = 0; i < count; ++i) {
    Item1784Entry entry;
    memcpy_s(&entry, sizeof(entry), &packet->entries[i], sizeof(entry));

    ITEM_INFO item;
    item.identity[1] = -1;
    item.SetItemId(entry.itemId);
    item.saleData[3] = entry.purchaseParameter;
    item.quantity = entry.quantity;
    item.prices[0] = entry.price;
    item.saleData[1] = entry.saleParameter1;
    item.saleData[2] = entry.saleParameter2;
    item.bindOnEquip = 8;
    item.identified = 1;
    item.GetIdDisplayName(name, capacity, 0);
    item.SetName(name);
    g_session.inventoryCategories[9].push_back(item);
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_CASHSHOP_WND);
  if (window) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 1, 0, 0, 0);
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
  case ITEMCOMPOSITON_SUCCESS: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_COMPOUNDING_SUCCEESS)), 0xFFFF00, 0, 0, 0);
    ITEM_INFO card = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->cardIndex);
    ITEM_INFO equip = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->equipIndex);
    if (equip.identity[1] != 0 && card.identity[1] != 0) {
      int* cards = reinterpret_cast<int*>(equip.cards);
      for (int i = 0; i < 4; ++i) {
        if (cards[i] == 0) {
          cards[i] = std::atoi(card.itemId.c_str());
          break;
        }
      }

      g_session.RemoveInventoryItem(packet->cardIndex, 1);
      g_session.UpdateInventoryItem(equip, 1);
    }
    break;
  }
  case ITEMCOMPOSITION_FAILURE:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_COMPOUNDING_FAIL)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D499B0
// FUNCTION: fn_21859
void CGameMode::ReceiveRefineResult(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RefineResultPacket*>(data);
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
  if (item.identity[1] != 0) {
    item.refiningLevel = packet->refine;
    g_session.UpdateInventoryItem(item, 1);
  }

  UIRefiningWnd* window = static_cast<UIRefiningWnd*>(g_windowMgr.LookupWindow(WID_REFININGWND));
  if (window) {
    switch (packet->result) {
    case ITEMREFINING_SUCCESS:
      window->SetState(2, 3);
      break;
    case ITEMREFINING_FAILURE:
      window->SetState(2, 4);
      break;
    case ITEMREFINING_DOWNGRADE:
      window->SetState(2, 5);
      break;
    case ITEMREFINING_FAILURE2:
      window->SetState(2, 6);
      break;
    }
    return;
  }

  switch (packet->result) {
  case ITEMREFINING_SUCCESS:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(154, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFINING_SUCCEESS)), 0xFFFF00, 0, 0, 0);
    break;
  case ITEMREFINING_FAILURE:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFINING_FAIL)), 0xFFFF00, 0, 0, 0);
    break;
  case ITEMREFINING_DOWNGRADE:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFINING_DOWNGRADE)), 0xFFFF00, 0, 0, 0);
    break;
  }

  ITEM_INFO refined = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
  refined.refiningLevel = packet->refine;
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

  switch (packet->type) {
  case 0:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(154, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_SUCCEESS_MSG), item.DisplayName(0).c_str()));
    break;
  case 1:
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_FAIL_MSG), item.DisplayName(0).c_str()));
    break;
  case 2:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->HasId(305)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 305, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(305, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_SUCCEESS_MSG), item.DisplayName(0).c_str()).c_str();
    break;
  case 3:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->HasId(306)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 306, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(306, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_FAIL_MSG), item.DisplayName(0).c_str()).c_str();
    break;
  case 4:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->HasId(1015)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1015, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1015, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_SUCCEESS_MSG), item.DisplayName(0).c_str()).c_str();
    break;
  case 5:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->HasId(1016)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1016, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1016, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_FAIL_MSG), item.DisplayName(0).c_str()).c_str();
    break;
  case 6: {
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->HasId(1017)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1017, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1017, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_FAIL_MSG), item.DisplayName(0).c_str()).c_str();
    UIWindow* window = g_windowMgr.LookupWindow(WID_CHANGEMATERIALWND);
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, 415, 0, 0, 0);
    }
    break;
  }
  case 7: {
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->HasId(1018)) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1018, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(1018, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    text = FormatOwnedString(LookupMessage(MSI_MAKE_TARGET_FAIL_MSG), item.DisplayName(0).c_str()).c_str();
    UIWindow* window = g_windowMgr.LookupWindow(WID_CHANGEMATERIALWND);
    if (window) {
      window->SendMsg(nullptr, UM_COMMAND, 415, 0, 0, 0);
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
    request.id = ID_CZ_CLOSE_UI_STYLINGSHOP;
    GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(request.id), &request);
    g_windowMgr.Confirm(LookupMessage(MSI_NOT_USING_SPECIALLETTERS_ON_PARTYNAME), 0, true, false, 0, "Message", 280, 120, 0);
    g_windowMgr.CloseWindow(WID_STYLINGSHOPWND);
    break;
  }
  case 1:
  case 2:
    g_windowMgr.Confirm(LookupMessage(MSI_SKILL_FAIL), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  }
}

// OFFSET: 0x00D6B010
// FUNCTION: fn_22066
void CGameMode::ReceiveLapineUpgradeResult(std::int16_t result) {
  if (0 != result) {
    UIWindow* window = g_windowMgr.LookupWindow(WID_LAPINEBOXWND);
    if (window) {
      static_cast<UILapineUpgradeBoxWnd*>(window)->SetWaiting(0);
    }
  } else if (g_windowMgr.LookupWindow(WID_LAPINEUPGRADEBOXWND)) {
    g_windowMgr.CloseWindow(WID_LAPINEUPGRADEBOXWND);
  }
}

// OFFSET: 0x00D6B070
// FUNCTION: fn_22067
void CGameMode::ReceiveLapineBoxResult(std::uint16_t result) {
  if (0 != result) {
    UIWindow* window = g_windowMgr.LookupWindow(WID_LAPINEBOXWND);
    if (window) {
      static_cast<UILapineBoxWnd*>(window)->SetWaiting(0);
    }
  }

  switch (result) {
  case 0:
    if (g_windowMgr.LookupWindow(WID_LAPINEBOXWND)) {
      g_windowMgr.CloseWindow(WID_LAPINEBOXWND);
    }
    break;
  case 5:
  case 7: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_LAPINEBOXWND);
    if (window) {
      window->SendMsg(nullptr, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REGISTER_PARTY_TO_BOOKINGSYSTEM)), 0, 0, 0);
    }
    break;
  }
  }
}

// OFFSET: 0x00D6BB10
// FUNCTION: fn_22072
void CGameMode::ReceiveRouletteInfo(const unsigned char* packet) {
  const auto* info = reinterpret_cast<const RouletteInfoPacket*>(packet);
  const RouletteInfoSlot* s = info->slots;
  for (int i = 0; i < 42; ++i, ++s) {
    RouletteSlot slot;
    slot.row = s->row;
    slot.column = s->column;
    slot.itemId = s->itemId;
    slot.count = s->count;
    g_rouletteMgr->AddItem(slot);
  }

  g_rouletteMgr->SetAdditionItem(g_rouletteMgr->m_additionItemId);
  if (g_rouletteMgr->m_resultShown == 1) {
    g_rouletteMgr->SetResult(static_cast<unsigned short>(g_rouletteMgr->m_index), 1, 0);
  }

  if (!g_windowMgr.roulletteWnd) {
    g_windowMgr.EnsureWindow(WID_ROULLETTEWND);
  }
}

// OFFSET: 0x00D6D080
// FUNCTION: fn_22084
void CGameMode::BeginCaptchaImage(unsigned short size, const char* key) {
  if (g_windowMgr.LookupWindow(WID_MACRODETECTORWND)) {
    return;
  }

  UIMacroDetectorWnd* window = static_cast<UIMacroDetectorWnd*>(g_windowMgr.EnsureWindow(WID_MACRODETECTORWND));
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
  UIMacroDetectorWnd* window = static_cast<UIMacroDetectorWnd*>(g_windowMgr.LookupWindow(WID_MACRODETECTORWND));
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
    UIWindow* window = g_windowMgr.EnsureWindow(WID_SPELLLISTWND);
    if (window) {
      for (int i = 0; i < count; ++i) {
        if (values[i] > 0) {
          window->SendMsg(nullptr, UM_ADDITEM, values[i], 0, 0, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00D6E440
// FUNCTION: fn_22099
void CloseAccountLimitedTool() {
  g_windowMgr.CloseWindow(WID_ACCOUNTLIMITEDTOOLWND);
}

// OFFSET: 0x00D6E450
// FUNCTION: fn_22100
void CGameMode::ReceiveMacroBlackListResult(int result) {
  switch (result) {
  case MCD_TIMEOUT:
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_INPUT_ANSWER), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case MCD_INCORRECT:
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_CHECK_ANSWER), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case MCD_GOOD:
    g_windowMgr.CloseWindow(WID_MACROBLACKLISTCHECKWND);
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_SELECT_CAPTCHA), 0, true, false, 0, "Message", 280, 120, 0);
    return;
  default:
    return;
  }

  GetRagConnection().Close();
  g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
}

// OFFSET: 0x00D6E520
// FUNCTION: fn_22101
void CGameMode::ReceiveMacroDetectorResult(int result) {
  switch (result) {
  case MCD_TIMEOUT:
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_INPUT_ANSWER), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case MCD_INCORRECT:
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_CHECK_ANSWER), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case MCD_GOOD:
    g_windowMgr.CloseWindow(WID_MACRODETECTORWND);
    g_windowMgr.Confirm(LookupMessage(MSI_MACRO_SELECT_CAPTCHA), 0, true, false, 0, "Message", 280, 120, 0);
    return;
  default:
    return;
  }

  GetRagConnection().Close();
  g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
}

// OFFSET: 0x00D71600
// FUNCTION: fn_22134
void CGameMode::ReceiveMacroBlackListCode(unsigned char state, const char* code) {
  UIMacroBlackListCheckWnd* window = static_cast<UIMacroBlackListCheckWnd*>(g_windowMgr.EnsureWindow(WID_MACROBLACKLISTCHECKWND));
  if (window) {
    window->SetCode(state, code);

    PacketIdOnly request;
    request.id = ID_CZ_START_MACROBLUSER_HUMAN_CHECK;
    GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(request.id), &request);
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
void CGameMode::ReceiveMacroReporterList(int count, const int* aids) {
  if (count <= 0) {
    return;
  }

  UIMacroReporterWnd* window = static_cast<UIMacroReporterWnd*>(g_windowMgr.LookupWindow(WID_MACROREPORTERWND));
  if (!window) {
    return;
  }

  for (int i = 0; i < count; ++i) {
    window->AddActorByAID(aids[i]);
  }
}

// GLOBAL: gl_25111
int g_unknown_01738268;

// GLOBAL: gl_25112
int g_unknown_0173826C;

// GLOBAL: gl_25113
int g_unknown_01738270;




// OFFSET: 0x00D1C050
// FUNCTION: fn_21624
void CGameMode::ReceiveStorageClose(const AchievementStatePacket* packet) {
  UIWindow* window = nullptr;
  bool notify = true;

  if (packet->type == 2) {
    g_windowMgr.Confirm(LookupMessage(MSI_USESKILL_FAIL_COINTS), 0, true, true, 0, "Message", 280, 120, 0);
    window = g_windowMgr.LookupWindow(WID_ITEMSTOREWND);
  } else {
    window = g_windowMgr.LookupWindow(WID_ITEMSTOREWND);
    notify = window != nullptr;
  }

  if (notify) {
    window->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_ITEMSTOREWND);
  }

  g_session.ClearCartItems();
  g_unknown_01738268 = 0;
  g_unknown_0173826C = 0;
  g_unknown_01738270 = 0;
}

// OFFSET: 0x00D6C460
// FUNCTION: fn_22081
void CGameMode::ReceiveAchievementList(int count, int score, short achievementLevel, int completed, int total, const AchievementRecordPacket* packets) {
  g_session.achievementScore = score;
  g_session.achievementLevel = achievementLevel;
  g_session.achievementCompleted = completed;
  g_session.achievementTotal = total;

  for (int i = 0; i < count; ++i) {
    g_session.UpdateAchievementRecord(packets[i]);
    if (packets[i].rewarded == 1) {
      g_session.AddAchievementJobTitle(packets[i].id);
    }
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_ACHIEVEMENTWND);
  if (window != nullptr) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D1A0A0
// FUNCTION: fn_21615
void CGameMode::OpenServiceWindow(unsigned char type) {
  switch (type) {
  case 0:
    if (!g_windowMgr.LookupWindow(251)) {
      AccountRequestPacket request;
      request.id = ID_CZ_REQ_BANKING_CHECK;
      request.aid = g_session.m_aid;
      GetRagConnection().Send(6, &request);
    }
    break;
  case 1:
    if (g_windowMgr.LookupWindow(WID_SAYDIALOGWND)) {
      this->SendMsg(MM_CLOSE_DIALOG, 0, 0, 0, 0);
    }
    if (!g_windowMgr.LookupWindow(WID_STYLINGSHOPWND)) {
      g_windowMgr.EnsureWindow(WID_STYLINGSHOPWND);
    }
    break;
  case 2:
    g_windowMgr.EnsureWindow(WID_CAPTCHAREGISTERWND);
    break;
  case 3:
    g_windowMgr.EnsureWindow(WID_MACROREPORTERWND);
    break;
  case 9:
    g_windowMgr.EnsureWindow(WID_CHANGEMATERIALWND);
    break;
  }
}

// OFFSET: 0x00D693E0
// FUNCTION: fn_22047
void CGameMode::ReceiveWindow328Result(int result, int value) {
  if (result == 0) {
    UIWindow* window = g_windowMgr.LookupWindow(WID_CHECKATTENDANCEWND);
    if (window) {
      window->SendMsg(nullptr, UM_SETINFO, value * 10 + 1, 0, 0, 0);
    }
  } else if (result == 1) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CAN_NOT_OFFER_EXP_TO_GUILD_MAX_LV)), 0xFF, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_CHECKATTENDANCEWND);
  }
}

// OFFSET: 0x00D69460
// FUNCTION: fn_22048
void CGameMode::ReceiveRouletteClose(const unsigned char* packet) {
  switch (reinterpret_cast<const ByteValuePacket*>(packet)->value) {
  case CLOSE_ROULETTE_SUCCESS:
    g_rouletteMgr->Reset();
    if (g_windowMgr.roulletteWnd) {
      g_windowMgr.CloseWindow(WID_ROULLETTEWND);
    }
    break;
  case CLOSE_ROULETTE_FAILED:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_RECV_ITEM)), 0x6464FF, 0, 0, 0);
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
  case MERGE_ITEM_SUCCESS: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_LEFT_ROLL)), 0x64FFFF, 0, 0, 0);
    UIWindow* window = g_windowMgr.LookupWindow(WID_MERGEITEMWND);
    if (window) {
      window->SendMsg(nullptr, UM_REFRESH, first, second, 0, 0);
    }
    break;
  }
  case MERGE_ITEM_FAILED_NOT_MERGE:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_RIGHT_ROLL)), 0xFF, 0, 0, 0);
    break;
  case MERGE_ITEM_FAILED_MAX_COUNT:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_VIEW_BEFORE_WORLDINFO)), 0xFF, 0, 0, 0);
    break;
  }

  g_windowMgr.CloseWindow(WID_MERGEITEMWND);
}

// OFFSET: 0x00D6AB60
// FUNCTION: fn_22061
void CGameMode::ReceiveRouletteOpen(const unsigned char* packet) {
  const auto* open = reinterpret_cast<const RouletteOpenPacket*>(packet);
  switch (open->result) {
  case OPEN_ROULETTE_SUCCESS:
    g_rouletteMgr->m_serial = open->serial;
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
      request.id = ID_CZ_REQ_ROULETTE_INFO;
      GetRagConnection().Send(g_packetLengths.FixedLength(request.id), &request);
    } else if (!g_windowMgr.roulletteWnd) {
      g_windowMgr.EnsureWindow(WID_ROULLETTEWND);
    }
    break;
  case OPEN_ROULETTE_FAILED:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOT_ENOUGH_ROULLETTE_POINT)), 0x6464FF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6ACE0
// FUNCTION: fn_22062
void CGameMode::ReceiveRodexOpen(const unsigned char* packet) {
  const auto* open = reinterpret_cast<const RodexOpenPacket*>(packet);
  if (open->result == 1) {
    if (!g_windowMgr.rodexComposeWnd) {
      g_session.RestoreSelectionItems();
      g_session.ClearItemQuantities();
      g_windowMgr.EnsureWindow(WID_RODEXCOMPOSEWND);
      const char* text = open->text;
      if (text) {
        reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.rodexComposeWnd)->SetContentsText(text);
      }
    }
  } else {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_EVOLUTION_FAIL_PET_ACC_OFF)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D6ADA0
// FUNCTION: fn_22064
void CGameMode::ReceiveCaptchaPreview(int result, unsigned short size, const char* key) {
  switch (result) {
  case 0: {
    UICaptchaPreviewWnd* window = static_cast<UICaptchaPreviewWnd*>(g_windowMgr.LookupWindow(WID_CAPTCHAPREVIEWWND));
    if (!window) {
      window = static_cast<UICaptchaPreviewWnd*>(g_windowMgr.EnsureWindow(WID_CAPTCHAPREVIEWWND));
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
    break;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("wrong captcha ID."), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6B8F0
// FUNCTION: fn_22070
void CGameMode::ReceiveStackedItemReturn(const unsigned char* packet) {
  const auto* result = reinterpret_cast<const StackedItemReturnPacket*>(packet);
  if (result->result == 1) {
    g_session.ReturnStackedItem(result->index, result->count);
    g_session.achievementResult = result->value;
  } else {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_JOINED_IN_CLAN)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D6E0B0
// FUNCTION: fn_22095
void CGameMode::ReceiveRodexRecipient(int id, unsigned short job, unsigned short level, const char* name) {
  if (id) {
    if (g_windowMgr.rodexComposeWnd) {
      reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->SetRecipient(id, job, level, name);
    }
  } else {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TAIWAN_PERSONALINFO_DEATHMSG)), 0x64FFFF, 0, 0, 0);
  }
}

struct BuyingStoreResultPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::int16_t result;
  /* 0004 */ std::int16_t value;
};

// OFFSET: 0x00D1D460
// FUNCTION: fn_21629
void CGameMode::ReceiveBuyingStoreResult(const BuyingStoreResultPacket* packet) {
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
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D18BA0
// FUNCTION: fn_21600
void CGameMode::SetActorGroupMemberVisible(std::uint32_t aid, int value) {
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (actor) {
    actor->SetGroupMemberVisible(value);
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
    g_modeMgr.gameMode->SendMsg(MM_CANCEL_EXCHANGE_ITEM, 0, 0, 0, 0);
  }

  if (packet->itemId == 0) {
    g_session.SetOtherExchangeMoney(packet->slot);
    return;
  }

  ITEM_INFO item;
  item.identity[0] = packet->type;
  item.SetItemId(packet->itemId);
  item.quantity = packet->slot;
  item.damaged = packet->damaged;
  item.identified = packet->identified;
  item.refiningLevel = packet->refine;
  std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
  item.identity[1] = 0xFFFFFFFF;
  item.optionCount = CountItemSlotOptions(packet->options);
  for (int index = 0; index < item.optionCount; ++index) {
    reinterpret_cast<ItemSlotOption*>(item.options)[index] = packet->options[index];
  }
  item.identity[2] = packet->expireTime;
  item.sprite = packet->bindType;
  g_session.AddItemSlot(item);
}

// OFFSET: 0x00D6C240
// FUNCTION: fn_22079
void CGameMode::ReceiveItemSlotAddGrade(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemSlotAddGradePacket*>(data);
  if (packet->slot < 0) {
    g_modeMgr.gameMode->SendMsg(MM_CANCEL_EXCHANGE_ITEM, 0, 0, 0, 0);
  }

  if (packet->itemId == 0) {
    g_session.SetOtherExchangeMoney(packet->slot);
    return;
  }

  ITEM_INFO item;
  item.identity[0] = packet->type;
  item.SetItemId(packet->itemId);
  item.quantity = packet->slot;
  item.damaged = packet->damaged;
  item.identified = packet->identified;
  item.refiningLevel = packet->refine;
  std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
  item.identity[1] = 0xFFFFFFFF;
  item.optionCount = CountItemSlotOptions(packet->options);
  for (int index = 0; index < item.optionCount; ++index) {
    reinterpret_cast<ItemSlotOption*>(item.options)[index] = packet->options[index];
  }
  item.identity[2] = packet->expireTime;
  item.sprite = packet->bindType;
  item.grade = packet->grade;
  g_session.AddItemSlot(item);
}

// OFFSET: 0x00D6D7E0
// FUNCTION: fn_22090
void CGameMode::ReceiveGuildAgitOwnershipChange(unsigned char value, std::uint32_t removeAid, std::uint32_t addAid) {
  GuildInfo* guildInfo = &g_guildInfo;
  if (g_session.IsLocalPlayerAid(removeAid) == 1) {
    guildInfo->RemoveAgitId(value);
  } else if (g_session.IsLocalPlayerAid(addAid) == 0) {
    guildInfo->AddAgitId(value);
  }

  if (g_windowMgr.guildInfoManageWnd) {
    g_windowMgr.guildInfoManageWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
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
  if (item.identity[1] != 0) {
    item.refiningLevel = packet->refine;
    item.optionCount = CountItemSlotOptions(packet->options);
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<ItemSlotOption*>(item.options)[index] = packet->options[index];
    }
    g_session.UpdateInventoryItem(item, 1);
  }
}

// OFFSET: 0x00D06410
// FUNCTION: fn_21575
int GetBaseJob(int job) {
  switch (job) {
  case JT_NOVICE:
  case JT_NOVICE_H:
  case JT_NOVICE_B:
    return JT_NOVICE;
  case JT_SWORDMAN:
  case JT_SWORDMAN_H:
  case JT_SWORDMAN_B:
    return JT_SWORDMAN;
  case JT_MAGICIAN:
  case JT_MAGICIAN_H:
  case JT_MAGICIAN_B:
    return JT_MAGICIAN;
  case JT_ARCHER:
  case JT_ARCHER_H:
  case JT_ARCHER_B:
    return JT_ARCHER;
  case JT_ACOLYTE:
  case JT_ACOLYTE_H:
  case JT_ACOLYTE_B:
    return JT_ACOLYTE;
  case JT_MERCHANT:
  case JT_MERCHANT_H:
  case JT_MERCHANT_B:
    return JT_MERCHANT;
  case JT_THIEF:
  case JT_THIEF_H:
  case JT_THIEF_B:
    return JT_THIEF;
  case JT_KNIGHT:
  case JT_KNIGHT_H:
  case JT_KNIGHT_B:
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_KNIGHT_B:
  case JT_DRAGON_KNIGHT:
    return JT_KNIGHT;
  case JT_PRIEST:
  case JT_PRIEST_H:
  case JT_PRIEST_B:
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
  case JT_CARDINAL:
    return JT_PRIEST;
  case JT_WIZARD:
  case JT_WIZARD_H:
  case JT_WIZARD_B:
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
  case JT_ARCH_MAGE:
    return JT_WIZARD;
  case JT_BLACKSMITH:
  case JT_BLACKSMITH_H:
  case JT_BLACKSMITH_B:
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MECHANIC_B:
  case JT_MEISTER:
    return JT_BLACKSMITH;
  case JT_HUNTER:
  case JT_HUNTER_H:
  case JT_HUNTER_B:
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_RANGER_B:
  case JT_WINDHAWK:
    return JT_HUNTER;
  case JT_ASSASSIN:
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CROSS:
    return JT_ASSASSIN;
  case JT_CRUSADER:
  case JT_CRUSADER_H:
  case JT_CRUSADER_B:
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_ROYAL_GUARD_B:
  case JT_IMPERIAL_GUARD:
    return JT_CRUSADER;
  case JT_MONK:
  case JT_MONK_H:
  case JT_MONK_B:
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
  case JT_INQUISITOR:
    return JT_MONK;
  case JT_SAGE:
  case JT_SAGE_H:
  case JT_SAGE_B:
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
  case JT_ELEMENTAL_MASTER:
    return JT_SAGE;
  case JT_ROGUE:
  case JT_ROGUE_H:
  case JT_ROGUE_B:
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
  case JT_ABYSS_CHASER:
    return JT_ROGUE;
  case JT_ALCHEMIST:
  case JT_ALCHEMIST_H:
  case JT_ALCHEMIST_B:
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
  case JT_BIOLO:
    return JT_ALCHEMIST;
  case JT_BARD:
  case JT_BARD_H:
  case JT_BARD_B:
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
  case JT_TROUBADOUR:
    return JT_BARD;
  case JT_DANCER:
  case JT_DANCER_H:
  case JT_DANCER_B:
  case JT_WANDERER:
  case JT_WANDERER_H:
  case JT_WANDERER_B:
  case JT_TROUVERE:
    return JT_DANCER;
  case JT_SUPERNOVICE:
  case JT_SUPERNOVICE_B:
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_HYPER_NOVICE:
    return JT_SUPERNOVICE;
  case JT_NINJA:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_NINJA_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
    return JT_NINJA;
  case JT_GUNSLINGER:
  case JT_REBELLION:
  case JT_GUNSLINGER_B:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    return JT_GUNSLINGER;
  case JT_TAEKWON:
  case JT_TAEKWON_B:
    return JT_TAEKWON;
  case JT_STAR:
  case JT_STAR_B:
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
  case JT_SKY_EMPEROR:
    return JT_STAR;
  case JT_LINKER:
  case JT_LINKER_B:
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
  case JT_SOUL_ASCETIC:
    return JT_LINKER;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
    return JT_DO_SUMMONER;
  case JT_DRUID:
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
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_BLACKSMITH_POINT), packet->first, packet->second).c_str()), 0xCDCDCD, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ALCHEMIST_POINT), packet->first, packet->second).c_str()), 0xCDFFCD, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_TAEKWON_POINT), packet->first, packet->second).c_str()), 0xFFCDCD, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_KILLER_POINT), packet->first, packet->second).c_str()), 0xCDCDCD, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D6BE80
// FUNCTION: fn_22076
void CGameMode::ReceiveRodexMailReturn(const unsigned char* data) {
  if (g_windowMgr.rodexContentsWnd) {
    reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.rodexContentsWnd)->SetZenyButtonDisabled(0);
  }

  const auto* packet = reinterpret_cast<const RodexMailReturnPacket*>(data);
  switch (packet->result) {
  case 0: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SET_ITEMS_TO_MAIL_SUCCEED)), 0x64FFFF, 0, 0, 0);

    RodexMailMap::iterator mail;
    switch (packet->type) {
    case MAIL_INBOX_NORMAL:
      mail = g_rodexMailStore->m_mails0.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails0.end()) {
        mail->second.type -= 2;
      }
      break;
    case MAIL_INBOX_ACCOUNT:
      mail = g_rodexMailStore->m_mails1.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails1.end()) {
        mail->second.type -= 2;
      }
      break;
    case MAIL_INBOX_RETURNED:
      mail = g_rodexMailStore->m_mails2.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails2.end()) {
        mail->second.type -= 2;
      }
      break;
    }

    if (g_windowMgr.rodexContentsWnd) {
      reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.rodexContentsWnd)->DisableZenyButton();
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVALID_MAIL_RECIEVER_INFO)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVALID_MAIL_TITLE_INFO)), 0x64FFFF, 0, 0, 0);
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
  case LEVELUP_EFFECT:
    actor->LaunchEffect(371, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case JOBLEVELUP_EFFECT:
    actor->LaunchEffect(158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case LEVELUP_EFFECT2:
    actor->LaunchEffect(338, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case LEVELUP_EFFECT3:
    actor->LaunchEffect(582, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case JOBLEVELUP_EFFECT2:
    actor->LaunchEffect(337, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case REFINING_FAIL_EFFECT:
    actor->LaunchEffect(155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case REFINING_SUCCESS_EFFECT:
    actor->LaunchEffect(154, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case MAKEITEM_AM_SUCCESS_EFFECT:
    actor->LaunchEffect(305, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case MAKEITEM_AM_FAIL_EFFECT:
    actor->LaunchEffect(306, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case GAME_OVER_EFFECT:
    g_partyEffectNoticePending = 1;
    break;
  }
}

// OFFSET: 0x00D73960
// FUNCTION: fn_22141
void CGameMode::ShowFormattedNotice(unsigned short messageId, std::uint32_t color, int argument) {
  std::string text = FormatOwnedString(LookupMessage(messageId), argument);
  if (!text.empty()) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), color, messageId == 2962 ? 22 : 0, 0, 0);
  }
}

// OFFSET: 0x00D6BC40
// FUNCTION: fn_22074
void CGameMode::ReceiveCaptchaUploadRequest(const std::uint32_t* sessionId, int result) {
  switch (result) {
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_DETECTOR_NOTICE3)), 0xFF, 0, 0, 0);
    break;
  case 0: {
    auto* window = static_cast<UICaptchaRegisterWnd*>(g_windowMgr.LookupWindow(WID_CAPTCHAREGISTERWND));
    if (!window) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_DETECTOR_NOTICE2)), 0xFF, 0, 0, 0);
      break;
    }

    g_timeScheduler->AddSchedule(new CCaptchaUploadSchedule(_Xtime_get_ticks(), sessionId, window->m_decompressedSize, window->m_decompressedData));
    break;
  }
  }
}

// OFFSET: 0x00D6A9F0
// FUNCTION: fn_22060
void CGameMode::ReceiveExtraInventoryResult(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ExtraInventoryResultPacket*>(data);
  int message;
  switch (packet->result) {
  case 0: {
    const int nextSlots = g_session.extraInventorySlots + 110;
    const int slots = g_session.extraInventorySlots + 100;
    const char* name = FindItemDescription(packet->itemId).identifiedName;
    const int answer = g_windowMgr.Confirm(
      FormatOwnedString(LookupMessage(MSI_INVENTORY_EXTEND_FAILED_NO_ITEM), name, slots, nextSlots).c_str(), 2, false, false, 0, "Message", 280, 120, 0);
    if (answer == 187) {
      this->SendMsg(MM_REQ_EXTEND_BODYITEM_SIZE, 0, 0, 0, 0);
    } else {
      this->SendMsg(MM_CLOSE_MSGBOX_EXTEND_BODYITEM_SIZE, 0, 0, 0, 0);
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

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D6FEA0
// FUNCTION: fn_22128
void CGameMode::ReceiveWindow348Result(unsigned char result, std::int16_t index) {
  switch (result) {
  case 0: {
    g_windowMgr.CloseWindow(WID_ITEMREFORMWND);
    g_effectMgr->Spawn(g_session.m_aid, 1608, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    if (item.identity[1] <= 0) {
      return;
    }
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    if (window) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
    }
    break;
  }
  case 3: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMREFORMWND);
    if (window) {
      window->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFORM_WARNNING_ENCHANT_MAYBE_LOST)), 0, 0, 0);
    }
    break;
  }
  }
}

// OFFSET: 0x00D69E60
// FUNCTION: fn_22056
void CGameMode::ReceiveRodexItemReturn(const unsigned char* data) {
  if (g_windowMgr.rodexContentsWnd) {
    reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.rodexContentsWnd)->SetItemButtonDisabled(0);
  }

  const auto* packet = reinterpret_cast<const RodexMailReturnPacket*>(data);
  switch (packet->result) {
  case 0: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GET_ZENY_FROM_MAIL_SUCCEED)), 0x64FFFF, 0, 0, 0);
    g_session.ClearItemQuantities();

    RodexMailMap::iterator mail;
    switch (packet->type) {
    case MAIL_INBOX_NORMAL:
      mail = g_rodexMailStore->m_mails0.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails0.end()) {
        mail->second.type -= 4;
      }
      break;
    case MAIL_INBOX_ACCOUNT:
      mail = g_rodexMailStore->m_mails1.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails1.end()) {
        mail->second.type -= 4;
      }
      break;
    case MAIL_INBOX_RETURNED:
      mail = g_rodexMailStore->m_mails2.find(packet->mailId);
      if (mail != g_rodexMailStore->m_mails2.end()) {
        mail->second.type -= 4;
      }
      break;
    }

    if (g_windowMgr.rodexContentsWnd) {
      auto* contents = reinterpret_cast<UIRodexContentsWnd*>(g_windowMgr.rodexContentsWnd);
      contents->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      contents->DisableItemButton();
    }
    if (g_windowMgr.rodexInboxWnd) {
      reinterpret_cast<UIWindow*>(g_windowMgr.rodexInboxWnd)->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GET_ZENY_FROM_MAIL_FAILED)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GET_ZENY_FROM_MAIL_OVERAMOUNT)), 0x64FFFF, 0, 0, 0);
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
    std::sprintf(buffer, LookupMessage(MSI_WARNING_METCHANT_SHOP_SELL), packet->amount);
    text += buffer;
  } else if (packet->type == 2) {
    std::sprintf(buffer, LookupMessage(MSI_SWAP_EQUIPITEM_WAITING), packet->amount);
    text += buffer;
  }

  if (packet->amount >= 0) {
    text += LookupMessage(MSI_GET);
  } else {
    text += LookupMessage(MSI_LOSS);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), color, 18, 0, 0);
}

#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/UIBarGraph.h"
#include "UI/Window/UIBasicInfoWnd.h"
#include "Contents/Effects/EmitterEffect.h"

#pragma pack(push, 1)
struct BankDepositAckPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t reason;
  /* 0004 */ std::int64_t money;
  /* 000C */ int zeny;
};

struct BankCheckPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int64_t money;
  /* 000A */ std::int16_t reason;
};

struct ExchangeItemAddAckPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t index;
  /* 0004 */ unsigned char result;
};

struct ExchangeRequestAckPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char result;
  /* 0003 */ std::uint32_t charId;
  /* 0007 */ std::int16_t level;
};

struct SearchStoreOpenPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t openType;
  /* 0004 */ unsigned char count;
};

struct StorageListHeader {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
};

struct NamedStorageListHeader {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ char title[24];
};

struct StorageEquipRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ unsigned char identified;
  /* 0008 */ std::uint16_t location;
  /* 000A */ std::uint16_t wearState;
  /* 000C */ unsigned char damaged;
  /* 000D */ unsigned char refine;
  /* 000E */ std::uint32_t cards[4];
  /* 001E */ int expireTime;
};

struct StorageEquipRecord2 {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ unsigned char identified;
  /* 0008 */ std::uint16_t location;
  /* 000A */ std::uint16_t wearState;
  /* 000C */ unsigned char damaged;
  /* 000D */ unsigned char refine;
  /* 000E */ std::uint32_t cards[4];
  /* 001E */ int expireTime;
  /* 0022 */ std::uint16_t bindOnEquip;
  /* 0024 */ std::uint16_t sprite;
};

struct StorageEquipRecord3 {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::uint16_t location;
  /* 0009 */ std::uint16_t wearState;
  /* 000B */ unsigned char refine;
  /* 000C */ std::uint32_t cards[4];
  /* 001C */ int expireTime;
  /* 0020 */ std::uint16_t bindOnEquip;
  /* 0022 */ std::uint16_t sprite;
  /* 0024 */ unsigned char identified : 1;
  /* 0024 */ unsigned char damaged : 1;
  /* 0024 */ unsigned char etcTab : 1;
};

struct StorageEquipRecord4 {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ int location;
  /* 000B */ int wearState;
  /* 000F */ unsigned char refine;
  /* 0010 */ std::uint32_t cards[4];
  /* 0020 */ int expireTime;
  /* 0024 */ std::uint16_t bindOnEquip;
  /* 0026 */ std::uint16_t sprite;
  /* 0028 */ unsigned char flags;
};

struct ItemSlotOptions {
  /* 0000 */ ItemSlotOption options[5];
};

struct StorageEquipRecord5 {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ int location;
  /* 000B */ int wearState;
  /* 000F */ unsigned char refine;
  /* 0010 */ std::uint32_t cards[4];
  /* 0020 */ int expireTime;
  /* 0024 */ std::uint16_t bindOnEquip;
  /* 0026 */ std::uint16_t sprite;
  /* 0028 */ unsigned char optionCount;
  /* 0029 */ ItemSlotOptions options;
  /* 0042 */ unsigned char flags;
};

struct StorageNormalRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::int16_t amount;
  /* 0009 */ std::uint16_t wearState;
  /* 000B */ std::uint32_t cards[4];
  /* 001B */ int expireTime;
  /* 001F */ unsigned char flags;
};

struct StorageNormalRecord2 {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::int16_t amount;
  /* 0009 */ int wearState;
  /* 000D */ std::uint32_t cards[4];
  /* 001D */ int expireTime;
  /* 0021 */ unsigned char flags;
};

struct ItemCardSlots {
  /* 0000 */ std::uint32_t cards[4];
};

struct LongParChangePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::int64_t value;
};
#pragma pack(pop)


// OFFSET: 0x00D195D0
// FUNCTION: fn_21606
void CGameMode::ReceiveBankDepositAck(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const BankDepositAckPacket*>(data);
  if (packet->reason == 0) {
    g_session.m_bankBalance = packet->money;
    g_session.m_zeny = packet->zeny;
    g_session.RefreshHpSpWindows();
    g_session.InvalidateWindowFrames();
    return;
  }

  char text[256];
  if (packet->reason == 2) {
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_BANK_OVER_INT_MAX));
  } else if (packet->reason == 3) {
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_NOT_VALID_AUTH_TICKET));
  } else if (packet->reason == 4) {
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_WRONG_GUN));
  } else {
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_BANK_DEPOSIT_ZERO));
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D196E0
// FUNCTION: fn_21607
void CGameMode::ReceiveBankWithdrawAck(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const BankDepositAckPacket*>(data);
  if (packet->reason == 0) {
    g_session.m_bankBalance = packet->money;
    g_session.m_zeny = packet->zeny;
    g_session.RefreshHpSpWindows();
    g_session.InvalidateWindowFrames();
    return;
  }

  char text[256];
  if (packet->reason == 1) {
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_BANK_WITHDRAW_ZERO));
  } else if (packet->reason == 3) {
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_WRONG_GUN));
  } else {
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_BANK_DEPOSIT_ZERO));
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D197F0
// FUNCTION: fn_21610
void CGameMode::ReceiveBankCheck(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const BankCheckPacket*>(data);
  if (packet->reason == 0) {
    g_session.m_bankBalance = packet->money;
    if (!g_windowMgr.CloseWindow(WID_BANK_NEWWND)) {
      g_windowMgr.EnsureWindow(WID_BANK_NEWWND);
    }
    return;
  }

  if (packet->reason == 2) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DAILYSENDRODEXCNT_LIMIT)), 0xFF, 0, 0, 0);
  } else {
    char text[256];
    std::memset(text, 0, 250);
    std::sprintf(text, LookupMessage(MSI_BANK_DEPOSIT_ZERO));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
  }
}

#pragma pack(push, 1)
struct TradeItemFilterPacket {
  /* 0000 */ std::uint16_t opcode;
  /* 0002 */ int type;
};
#pragma pack(pop)


// OFFSET: 0x00D198F0
// FUNCTION: fn_21612
void CGameMode::ReceiveTradeItemFilter(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const TradeItemFilterPacket*>(data);
  g_session.ClearTradeItems();
  const int count = g_session.GetList0Size();
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = g_session.GetInventoryItemAt(i);
    if (item.identity[1] == 0) {
      continue;
    }
    if (packet->type == 1 && !this->IsEffectId994Group(std::atoi(item.itemId.c_str()))) {
      continue;
    }
    if (packet->type == 2 && !this->IsEffectId990Group(std::atoi(item.itemId.c_str()))) {
      continue;
    }
    if (item.identity[1] == g_session.equippedArrowIndex) {
      continue;
    }
    RawItemBasic raw = {};
    raw.id = item.identity[1];
    strcpy_s(raw.itemId, sizeof(raw.itemId), item.itemId.c_str());
    raw.type = item.identity[0];
    raw.identified = item.identified;
    raw.price = 1;
    raw.realPrice = 2;
    g_session.AddShopItem(raw, 1);
  }
  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND2);
  window->SendMsg(nullptr, UM_SETTITLE, 1, 0, 0, 0);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  g_windowMgr.EnsureWindow(WID_ITEMSELLWND2)->SendMsg(nullptr, UM_REFRESHITEM, packet->type, 0, 0, 0);
}

// OFFSET: 0x00D1BE10
// FUNCTION: fn_21623
void CGameMode::ReceiveExchangeItemAddAck(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ExchangeItemAddAckPacket*>(data);
  int& exchangeAmount = *reinterpret_cast<int*>(&this->equipmentRequestState[0x10]);
  *reinterpret_cast<int*>(&this->equipmentRequestState[0x00]) = 0;
  this->inventoryChanged = 1;
  switch (packet->result) {
  case EXITEM_ADD_SUCCEED:
    if (packet->index != 0) {
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
      item.quantity = exchangeAmount;
      g_session.AddExchangeItem(item);
    } else {
      g_session.m_zeny -= exchangeAmount;
      g_session.RefreshHpSpWindows();
      g_session.InvalidateWindowFrames();
      g_session.SetExchangeMoney(g_session.exchangeMoney + exchangeAmount);
    }
    break;
  case EXITEM_ADD_FAILED_OVERWEIGHT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_OVER_WEIGHT)), 0xFF, 0, 0, 0);
    break;
  case EXITEM_ADD_FAILED_OVERCOUNT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_OVER_COUNT)), 0xFF, 0, 0, 0);
    break;
  case EXITEM_ADD_FAILED_EACHITEM_OVERCOUNT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_EACHITEM_OVER_COUNT)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D1C2B0
// FUNCTION: fn_21625
void CGameMode::ReceiveExchangeRequestAck2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ExchangeRequestAckPacket*>(data);
  const std::string& exchangeName = this->exchangeName;
  switch (packet->result) {
  case TRADE_ACK_ACCEPT: {
    this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
    *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 1;
    g_session.ResetSlots();
    g_session.ClearItemSlotQuantities();
    g_windowMgr.CloseWindow(WID_EXCHANGEUI);
    UIWindow* window = g_windowMgr.EnsureWindow(WID_EXCHANGEUI);
    if (window) {
      window->SendMsg(nullptr, UM_SET_EXCHANGE_CHAR_NAME, reinterpret_cast<std::intptr_t>(exchangeName.c_str()), packet->charId, packet->level, 0);
    }
    break;
  }
  case TRADE_ACK_CANCEL:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REJECT_DEAL)), 0xFF, 0, 0, 0);
    break;
  case TRADE_ACK_TOOFAR:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_FAR_TO_DEAL)), 0xFF, 0, 0, 0);
    break;
  case TRADE_ACK_CHARNOTEXIST:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_NOT_EXIST)), 0xFF, 0, 0, 0);
    break;
  case TRADE_ACK_FAILED:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_DEALING)), 0xFF, 0, 0, 0);
    break;
  case TRADE_ACK_BUSY: {
    std::string text = FormatOwnedString(LookupMessage(MSI_CHARACTER_IS_BUSY), exchangeName.c_str());
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
    break;
  }
  }
}

// OFFSET: 0x00D1C4E0
// FUNCTION: fn_21626
void CGameMode::ReceiveExchangeRequestAck(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ExchangeRequestAckPacket*>(data);
  switch (packet->result) {
  case TRADE_ACK_ACCEPT: {
    this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
    *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 1;
    g_session.ResetSlots();
    g_session.ClearItemSlotQuantities();
    g_windowMgr.CloseWindow(WID_EXCHANGEUI);
    UIWindow* window = g_windowMgr.EnsureWindow(WID_EXCHANGEUI);
    if (window) {
      const std::string& exchangeName = this->exchangeName;
      window->SendMsg(nullptr, UM_SET_EXCHANGE_CHAR_NAME, reinterpret_cast<std::intptr_t>(exchangeName.c_str()), 0, 0, 0);
    }
    break;
  }
  case TRADE_ACK_CANCEL:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REJECT_DEAL)), 0xFF, 0, 0, 0);
    break;
  case TRADE_ACK_TOOFAR:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_FAR_TO_DEAL)), 0xFF, 0, 0, 0);
    break;
  case TRADE_ACK_CHARNOTEXIST:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_NOT_EXIST)), 0xFF, 0, 0, 0);
    break;
  case TRADE_ACK_FAILED:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_DEALING)), 0xFF, 0, 0, 0);
    break;
  }
}

#pragma pack(push, 1)
struct GuildStorageLogPacketEntry {
  /* 0000 */ int logId;
  /* 0004 */ int itemId;
  /* 0008 */ int quantity;
  /* 000C */ unsigned char operation;
  /* 000D */ int refine;
  /* 0011 */ unsigned char uniqueId[8];
  /* 0019 */ unsigned char identified;
  /* 001A */ std::int16_t count;
  /* 001C */ unsigned char cards[16];
  /* 002C */ char name[24];
  /* 0044 */ char date[24];
  /* 005C */ unsigned char highlighted;
};

struct GuildStorageLogPacket {
  /* 0000 */ std::uint16_t opcode;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ std::int16_t result;
  /* 0006 */ std::int16_t count;
  /* 0008 */ GuildStorageLogPacketEntry entries[1];
};
#pragma pack(pop)

// OFFSET: 0x00D1C640
// FUNCTION: fn_21627
void CGameMode::ReceiveGuildStorageLog(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildStorageLogPacket*>(data);
  if (packet->result == 0) {
    for (int i = 0; i < packet->count; ++i) {
      const GuildStorageLogPacketEntry& source = packet->entries[i];
      ITEM_INFO item;
      item.recordIds[0] = source.logId;
      item.recordIds[1] = source.quantity;
      item.recordType = source.operation;
      std::strcpy(reinterpret_cast<char*>(item.recordName), source.name);
      std::strcpy(reinterpret_cast<char*>(item.recordDate), source.date);
      char number[16] = {};
      item.itemId = _itoa(source.itemId, number, 10);
      item.identified = source.identified;
      item.identity[0] = source.count;
      item.refiningLevel = source.refine;
      std::memcpy(item.cards, source.cards, sizeof(source.cards));
      GuildStorageLogEntry entry;
      entry.logId = item.recordIds[0];
      entry.quantity = item.recordIds[1];
      entry.operation = item.recordType;
      std::strcpy(entry.characterName, reinterpret_cast<const char*>(item.recordName));
      std::strcpy(entry.timestamp, reinterpret_cast<const char*>(item.recordDate));
      entry.itemName = item.DisplayName(0);
      entry.identified = item.identified;
      entry.itemType = item.identity[0];
      entry.refineLevel = item.refiningLevel;
      std::memcpy(entry.cards, item.cards, sizeof(entry.cards));
      entry.category = source.highlighted;
      g_guildStorageLogs[0].push_back(entry);
    }
  } else if (packet->result == 1) {
    for (int i = 0; i < packet->count; ++i) {
      const GuildStorageLogPacketEntry& source = packet->entries[i];
      ITEM_INFO item;
      item.recordIds[0] = source.logId;
      item.recordIds[1] = source.quantity;
      item.recordType = source.operation;
      std::strcpy(reinterpret_cast<char*>(item.recordName), source.name);
      std::strcpy(reinterpret_cast<char*>(item.recordDate), source.date);
      char number[16] = {};
      item.itemId = _itoa(source.itemId, number, 10);
      item.identified = source.identified;
      item.identity[0] = source.count;
      item.refiningLevel = source.refine;
      std::memcpy(item.cards, source.cards, sizeof(source.cards));
      GuildStorageLogEntry entry;
      entry.logId = item.recordIds[0];
      entry.quantity = item.recordIds[1];
      entry.operation = item.recordType;
      std::strcpy(entry.characterName, reinterpret_cast<const char*>(item.recordName));
      std::strcpy(entry.timestamp, reinterpret_cast<const char*>(item.recordDate));
      entry.itemName = item.DisplayName(0);
      entry.identified = item.identified;
      entry.itemType = item.identity[0];
      entry.refineLevel = item.refiningLevel;
      std::memcpy(entry.cards, item.cards, sizeof(entry.cards));
      entry.category = source.highlighted;
      g_guildStorageLogs[0].push_back(entry);
    }
    GuildStorageLogEntry pivot;
    std::sort(g_guildStorageLogs[0].begin(), g_guildStorageLogs[0].end(), [&pivot](const GuildStorageLogEntry& left, const GuildStorageLogEntry& right) {
      return std::strcmp(left.timestamp, right.timestamp) > 0;
    });
    for (auto it = g_guildStorageLogs[0].begin(); it != g_guildStorageLogs[0].end(); ++it) {
      if (it->operation == 1) {
        g_guildStorageLogs[1].push_back(*it);
      } else {
        g_guildStorageLogs[2].push_back(*it);
      }
    }
    g_windowMgr.EnsureWindow(WID_GUILD_STORAGE_LOG);
  } else {
    char text[252];
    if (packet->result == 2) {
      std::memset(text, 0, 250);
      std::sprintf(text, LookupMessage(MSI_GUILD_STORAGE_LOG_OUT));
    } else {
      std::memset(text, 0, 250);
      std::sprintf(text, LookupMessage(MSI_RESULT_FAIL_UNKNOWN));
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    g_guildStorageLogs[0].clear();
    g_guildStorageLogs[1].clear();
    g_guildStorageLogs[2].clear();
  }
}

// OFFSET: 0x00D45E90
// FUNCTION: fn_21843
void CGameMode::ReceiveStorageEquipList(const unsigned char* data) {
  const auto* header = reinterpret_cast<const StorageListHeader*>(data);
  g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
  const int count = (header->length - sizeof(StorageListHeader)) / sizeof(StorageEquipRecord);
  const auto* records = reinterpret_cast<const StorageEquipRecord*>(data + sizeof(StorageListHeader));
  for (int i = 0; i < count; ++i) {
    const StorageEquipRecord* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[2] = record->location;
    item.identity[1] = record->index;
    item.identity[3] = record->wearState;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.quantity = 1;
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.damaged = record->damaged;
    item.refiningLevel = record->refine;
    item.expireTime = record->expireTime;
    g_session.AddCartItem(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D46000
// FUNCTION: fn_21844
void CGameMode::ReceiveStorageEquipList2(const unsigned char* data) {
  const auto* header = reinterpret_cast<const StorageListHeader*>(data);
  g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
  const int count = (header->length - sizeof(StorageListHeader)) / sizeof(StorageEquipRecord2);
  const auto* records = reinterpret_cast<const StorageEquipRecord2*>(data + sizeof(StorageListHeader));
  for (int i = 0; i < count; ++i) {
    const StorageEquipRecord2* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[2] = record->location;
    item.identity[1] = record->index;
    item.identity[3] = record->wearState;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.quantity = 1;
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.damaged = record->damaged;
    item.refiningLevel = record->refine;
    item.expireTime = record->expireTime;
    item.bindOnEquip = record->bindOnEquip;
    item.sprite = record->sprite;
    g_session.AddCartItem(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D46460
// FUNCTION: fn_21845
void CGameMode::ReceiveStorageEquipList3(const unsigned char* data) {
  const auto* header = reinterpret_cast<const NamedStorageListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (header->title[0] != '\0') {
    std::string title(header->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (header->length - sizeof(NamedStorageListHeader)) / sizeof(StorageEquipRecord3);
  const auto* records = reinterpret_cast<const StorageEquipRecord3*>(data + sizeof(NamedStorageListHeader));
  for (int i = 0; i < count; ++i) {
    const StorageEquipRecord3* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[2] = record->location;
    item.identity[1] = record->index;
    item.identity[3] = record->wearState;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.quantity = 1;
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.damaged = record->damaged;
    item.refiningLevel = record->refine;
    item.expireTime = record->expireTime;
    item.bindOnEquip = record->bindOnEquip;
    item.sprite = record->sprite;
    item.flags[0] = record->etcTab;
    g_session.AddCartItem(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// INLINED
static __forceinline void AddStorageEquipItem(
  unsigned char type, int location, std::int16_t index, int wearState, std::uint32_t card0, std::uint32_t card1, std::uint32_t card2,
  std::uint32_t card3, int itemId, unsigned char identified, unsigned char damaged, unsigned char refine, int expireTime,
  std::uint16_t bindOnEquip, std::uint16_t sprite, unsigned char etcTab, ItemSlotOptions options
) {
  if (location == 0x8000) {
    return;
  }

  ITEM_INFO item;
  item.identity[0] = type;
  item.identity[2] = location;
  item.identity[1] = index;
  item.identity[3] = wearState;
  std::uint32_t* cards = reinterpret_cast<std::uint32_t*>(item.cards);
  cards[0] = card0;
  cards[1] = card1;
  cards[2] = card2;
  cards[3] = card3;
  item.quantity = 1;
  item.SetItemId(itemId);
  item.identified = identified;
  item.damaged = damaged;
  item.refiningLevel = refine;
  item.expireTime = expireTime;
  item.bindOnEquip = bindOnEquip;
  item.sprite = sprite;
  item.flags[0] = etcTab;
  item.optionCount = CountItemSlotOptions(options.options);
  item.grade = 0;
  for (int i = 0; i < item.optionCount; ++i) {
    reinterpret_cast<ItemSlotOption*>(item.options)[i] = options.options[i];
  }
  g_session.AddCartItem(item, 0);
}

// OFFSET: 0x00D46680
// FUNCTION: fn_21846
void CGameMode::ReceiveStorageEquipList4(const unsigned char* data) {
  const auto* header = reinterpret_cast<const NamedStorageListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (header->title[0] != '\0') {
    std::string title(header->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (header->length - sizeof(NamedStorageListHeader)) / sizeof(StorageEquipRecord4);
  ItemSlotOptions options = {};
  const auto* entries = reinterpret_cast<const StorageEquipRecord4*>(data + sizeof(NamedStorageListHeader));
  for (int i = 0; i < count; ++i) {
    const StorageEquipRecord4* entry = &entries[i];
    AddStorageEquipItem(
      entry->type, entry->location, entry->index, entry->wearState, entry->cards[0], entry->cards[1], entry->cards[2], entry->cards[3],
      entry->itemId, entry->flags & 1, (entry->flags >> 1) & 1, entry->refine, entry->expireTime, entry->bindOnEquip, entry->sprite,
      (entry->flags >> 2) & 1, options
    );
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D46A20
// FUNCTION: fn_21847
void CGameMode::ReceiveStorageEquipList5(const unsigned char* data) {
  const auto* header = reinterpret_cast<const NamedStorageListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (header->title[0] != '\0') {
    std::string title(header->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (static_cast<std::uint16_t>(header->length) - sizeof(NamedStorageListHeader)) / sizeof(StorageEquipRecord5);
  const auto* records = reinterpret_cast<const StorageEquipRecord5*>(data + sizeof(NamedStorageListHeader));
  for (int i = 0; i < count; ++i) {
    const StorageEquipRecord5* record = &records[i];
    AddStorageEquipItem(
      record->type, record->location, record->index, record->wearState, record->cards[0], record->cards[1], record->cards[2],
      record->cards[3], record->itemId, record->flags & 1, (record->flags >> 1) & 1, record->refine, record->expireTime,
      record->bindOnEquip, record->sprite, (record->flags >> 2) & 1, record->options
    );
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D46EE0
// FUNCTION: fn_21848
void CGameMode::ReceiveStorageNormalList(const unsigned char* data) {
  const auto* header = reinterpret_cast<const NamedStorageListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (header->title[0] != '\0') {
    std::string title(header->title);
    window->title->SetTextFitted(title.c_str());
  }

  const unsigned int count = (header->length - sizeof(NamedStorageListHeader)) / sizeof(StorageNormalRecord);
  const auto* records = reinterpret_cast<const StorageNormalRecord*>(data + sizeof(NamedStorageListHeader));
  for (unsigned int i = 0; i < count; ++i) {
    const StorageNormalRecord* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[3] = record->wearState;
    item.identity[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.damaged = 0;
    item.identified = record->flags & 1;
    item.flags[0] = (record->flags >> 1) & 1;
    item.expireTime = record->expireTime;
    item.refiningLevel = 0;
    g_session.AddCartItem(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D470E0
// FUNCTION: fn_21849
void CGameMode::ReceiveStorageNormalList2(const unsigned char* data) {
  const auto* header = reinterpret_cast<const NamedStorageListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (header->title[0] != '\0') {
    std::string title(header->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (header->length - sizeof(NamedStorageListHeader)) / sizeof(StorageNormalRecord2);
  const auto* records = reinterpret_cast<const StorageNormalRecord2*>(data + sizeof(NamedStorageListHeader));
  for (int i = 0; i < count; ++i) {
    const StorageNormalRecord2* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[3] = record->wearState;
    item.identity[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.damaged = 0;
    item.identified = record->flags & 1;
    item.flags[0] = (record->flags >> 1) & 1;
    item.expireTime = record->expireTime;
    item.refiningLevel = 0;
    g_session.AddCartItem(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D6DEF0
// FUNCTION: fn_22094
void CGameMode::ReceiveItemUpgrade(std::uint16_t index, std::uint16_t refine, ItemCardSlots cards, ItemSlotOptions options) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
  item.refiningLevel = refine;
  std::memcpy(item.cards, cards.cards, sizeof(cards.cards));
  item.optionCount = CountItemSlotOptions(options.options);
  for (int i = 0; i < item.optionCount; ++i) {
    reinterpret_cast<ItemSlotOption*>(item.options)[i] = options.options[i];
  }
  g_session.UpdateInventoryItem(item, 1);
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
  }
}

// OFFSET: 0x00D6E640
// FUNCTION: fn_22103
void CGameMode::ReceiveFeedConfig(unsigned char state, unsigned char option) {
  *reinterpret_cast<int*>(g_session.equipmentViewState) = state;
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(state + 0x54E)), 0x64FFFF, 0, 0, 0);
  if (g_windowMgr.equipWnd) {
    g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  g_serviceOptions.Set(231, option);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(option + 0xBA2)), 0x64FFFF, 0, 0, 0);
  g_windowMgr.RefreshGameSettingOption(231);
}

// OFFSET: 0x00D6E6F0
// FUNCTION: fn_22104
void CGameMode::ReceiveFeedConfig2(unsigned char state, unsigned char option, bool petAutoFeed) {
  *reinterpret_cast<int*>(g_session.equipmentViewState) = state;
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(state + 0x54E)), 0x64FFFF, 0, 0, 0);
  if (g_windowMgr.equipWnd) {
    g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  g_serviceOptions.Set(231, option);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(option + 0xBA2)), 0x64FFFF, 0, 0, 0);
  g_windowMgr.RefreshGameSettingOption(231);
  g_session.SetPetAutoFeed(petAutoFeed);
}

// OFFSET: 0x00D6E7A0
// FUNCTION: fn_22105
void CGameMode::ReceiveFeedConfig3(unsigned char state, unsigned char option, bool petAutoFeed, bool homunculusAutoFeed) {
  *reinterpret_cast<int*>(g_session.equipmentViewState) = state;
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(state + 0x54E)), 0x64FFFF, 0, 0, 0);
  if (g_windowMgr.equipWnd) {
    g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  g_serviceOptions.Set(231, option);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(option + 0xBA2)), 0x64FFFF, 0, 0, 0);
  g_windowMgr.RefreshGameSettingOption(231);
  g_session.SetPetAutoFeed(petAutoFeed);
  g_session.SetHomunculusAutoFeed(homunculusAutoFeed);
}

// OFFSET: 0x00D6E860
// FUNCTION: fn_22106
void CGameMode::ReceiveFeedConfig4(unsigned char state, unsigned char option, bool petAutoFeed, bool homunculusAutoFeed, int value) {
  *reinterpret_cast<int*>(g_session.equipmentViewState) = state;
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(state + 0x54E)), 0x64FFFF, 0, 0, 0);
  if (g_windowMgr.equipWnd) {
    g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  g_serviceOptions.Set(231, option);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(option + 0xBA2)), 0x64FFFF, 0, 0, 0);
  g_windowMgr.RefreshGameSettingOption(231);
  g_session.SetPetAutoFeed(petAutoFeed);
  g_session.SetHomunculusAutoFeed(homunculusAutoFeed);
  *reinterpret_cast<int*>(g_session.feedOptions) = value;
}

// OFFSET: 0x00D713E0
// FUNCTION: fn_22133
void CGameMode::ReceiveLongParChange(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const LongParChangePacket*>(data);
  std::int64_t& exp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x04]);
  std::int64_t& nextExp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x0C]);
  std::int64_t& nextJobExp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x14]);
  std::int64_t& jobExp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x1C]);
  bool refresh = true;
  switch (packet->type) {
  case SP_BASEEXP:
    if (g_session.IsAdminSubMode(2)) {
      const std::int64_t value = packet->value;
      const int gained = static_cast<int>(value) - static_cast<int>(exp);
      if (value != 0 && gained != 0) {
        char text[256];
        StringFormat(
          text, sizeof(text), static_cast<std::size_t>(-1), "%lld : %lld/%lld More : %d", value - exp, value, nextExp,
          static_cast<int>((nextExp - exp) / gained)
        );
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xF51EF5, 0, 0, 0);
      }
    }
    exp = packet->value;
    break;
  case SP_JOBEXP:
    jobExp = packet->value;
    break;
  case SP_ZENY:
    if (*reinterpret_cast<int*>(&this->equipmentRequestState[0x14])) {
      *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.m_zeny = static_cast<int>(packet->value);
    g_session.RefreshHpSpWindows();
    break;
  case SP_NEXTBASEEXP:
    nextExp = packet->value;
    break;
  case SP_NEXTJOBEXP:
    nextJobExp = packet->value;
    break;
  default:
    refresh = false;
    break;
  }
  if (refresh) {
    g_session.InvalidateWindowFrames();
  }

  static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->baseExpBar)->SetValues(exp, nextExp);
  static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->jobExpBar)->SetValues(jobExp, nextJobExp);
}

// OFFSET: 0x00D73A10
// FUNCTION: fn_22142
void ReceiveNavigation(const unsigned char* packet) {
  int group = packet[3] >= 100 ? 1 : 0;
  int code = packet[3] >= 100 ? packet[3] - 100 : packet[3];
  if (code >= 10) {
    group += 2;
    code -= 10;
  }

  const unsigned char mode = static_cast<unsigned char>(group + (code < 1 ? 0 : 4));
  const int x = *reinterpret_cast<const std::int16_t*>(packet + 0x15);
  const int y = *reinterpret_cast<const std::int16_t*>(packet + 0x17);
  const int id = *reinterpret_cast<const std::int16_t*>(packet + 0x19);
  g_navigationManager.StartNavigation(std::string(reinterpret_cast<const char*>(packet + 5)), packet[2], mode, packet[4] != 0, x, y, id);
}

// OFFSET: 0x00D73AF0
// FUNCTION: fn_22143
void CGameMode::ReceiveNotifyEffect(const unsigned char* packet) {
  CRenderObject* actor = this->scene->GetGameActorByAidIncludeMe(*reinterpret_cast<const std::uint32_t*>(packet + 2));
  if (!actor) {
    return;
  }

  int effectId = *reinterpret_cast<const int*>(packet + 6);
  if (effectId == EF_DUSTSTORM) {
    CEmitterEffect* emitter = static_cast<CEmitterEffect*>(g_effectMgr->Spawn(
      0, 0x3CE, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, -1, 0, 0
    ));
    emitter->velocityMin.x = -2.0f;
    emitter->velocityMin.y = 2.0f;
    emitter->velocityMin.z = -2.0f;
    emitter->velocityMax.x = 0.0f;
    emitter->velocityMax.y = -1.0f;
    emitter->velocityMax.z = 0.0f;
    emitter->acceleration.x = 0.0f;
    emitter->acceleration.y = -7.5f;
    emitter->acceleration.z = 0.0f;
    emitter->spread.x = -1.0f;
    emitter->spread.y = 2.0f;
    emitter->spread.z = -1.0f;
    emitter->red = 255;
    emitter->green = 255;
    emitter->blue = 100;
    emitter->alpha = 100;
    emitter->rateMin = 20.0f;
    emitter->rateMax = 100.0f;
    emitter->scaleMin = 30.0f;
    emitter->scaleMax = 30.0f;
    emitter->lifeMin = 5.0f;
    emitter->lifeMax = 10.0f;
    emitter->sourceBlend = 5;
    emitter->destinationBlend = 2;
    emitter->SetParticleSpeed(3.0f);
    emitter->SetCount(30);
    emitter->SetTexture("smoke2.bmp");
    emitter->duration = 1250;
    effectId = *reinterpret_cast<const int*>(packet + 6);
  }

  if (g_serviceOptions.Get(0xE5)) {
    switch (*reinterpret_cast<const int*>(packet + 6)) {
    case EF_LORD:
      effectId = 0x48A;
      break;
    case EF_METEORSTORM:
      effectId = 0x48B;
      break;
    case EF_QUAGMIRE:
      effectId = 0x489;
      break;
    }
  }

  CGameActor* gameActor = static_cast<CGameActor*>(actor);
  if (!g_effectMgr->Spawn(
        actor->render.accountId, effectId, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, -1, 0, 0
      )) {
    gameActor->LaunchEffect(*reinterpret_cast<const int*>(packet + 6), vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }

  if (g_session.IsMercenaryJob(gameActor->GetEffectiveJob(-1)) && *reinterpret_cast<const int*>(packet + 6) == EF_HO_UP) {
    ++gameActor->characterLevel;
    gameActor->ResetJobEffects();
    gameActor->UpdateMaxLevelAura();
  }
}

#include "Client/Application/Launcher.h"

#pragma pack(push, 1)
struct TypedItemEntry {
  /* 0000 */ int itemId;
  /* 0004 */ std::uint32_t price;
  /* 0008 */ std::uint16_t viewId;
  /* 000A */ std::uint32_t equipLocation;
};

struct TypedItemListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::int16_t count;
  /* 0006 */ std::int16_t type;
  /* 0008 */ TypedItemEntry entries[1];
};
#pragma pack(pop)

// OFFSET: 0x00D1B690
// FUNCTION: fn_21619
void CGameMode::ReceiveTypedItemList(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const TypedItemListPacket*>(data);
  if (g_launchPassCommandLine) {
    return;
  }

  for (int i = 0; i < packet->count; ++i) {
    const TypedItemEntry& entry = packet->entries[i];
    const int itemId = entry.itemId;
    char name[256];
    ITEM_INFO item;
    item.identity[1] = -1;
    item.prices[0] = entry.price;
    item.prices[1] = entry.price;
    item.identity[0] = 4;
    item.identified = 1;
    item.damaged = 0;
    item.refiningLevel = 0;
    std::uint16_t type = packet->type;
    if (type == 8) {
      type = 0xFFFF;
    }
    item.sprite = entry.viewId;
    item.identity[2] = entry.equipLocation;
    item.bindOnEquip = type;
    item.SetItemId(itemId);
    int capacity = sizeof(name);
    item.GetIdDisplayName(name, capacity, 0);
    item.SetName(name);
    if (static_cast<unsigned int>(packet->type) <= 10) {
      g_session.inventoryCategories[packet->type].push_back(item);
    }
  }
}

#include "UI/Shop/UIMerchantItemMyShopWnd.h"
#include "UI/Shop/UIMerchantItemShopWnd.h"

#pragma pack(push, 1)
struct VendingItemRecord {
  /* 0000 */ int price;
  /* 0004 */ std::int16_t amount;
  /* 0006 */ unsigned char type;
  /* 0007 */ int itemId;
};
#pragma pack(pop)

// OFFSET: 0x00D70010
// FUNCTION: fn_22129
void CGameMode::ReceiveVendingItemList(
  int aid, int storeId, char own, int count, std::uint32_t duration, const unsigned char* data, int value, int
) {
  const auto* records = reinterpret_cast<const VendingItemRecord*>(data);
  if (own == 1) {
    g_session.ClearMerchantPurchaseItems();
    for (int i = 0; i < count;) {
      const VendingItemRecord record = records[i];
      ITEM_INFO item;
      ++i;
      item.prices[0] = record.price;
      item.prices[1] = record.price;
      item.quantity = record.amount;
      item.identity[0] = record.type;
      item.identity[1] = i;
      item.identified = 1;
      item.SetItemId(record.itemId);
      g_session.AddMerchantPurchaseItem(item);
    }

    UIMerchantItemMyShopWnd* window = dynamic_cast<UIMerchantItemMyShopWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_MERCHANTITEMMYSHOPWND_MODE_1)));
    if (window) {
      window->OpenShop();
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_SET_BUYING_SHOP_ZENY, value, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETNAID, aid, 0, 0, 0);
      window->SetDuration(duration);
    }
  } else {
    g_session.ClearMerchantMirrorItems();
    g_session.ClearMerchantHopeToSellItems();
    for (int i = 0; i < count;) {
      const VendingItemRecord record = records[i];
      ITEM_INFO item;
      ++i;
      item.prices[0] = record.price;
      item.prices[1] = record.price;
      item.quantity = record.amount;
      item.identity[0] = record.type;
      item.identity[1] = i;
      item.identified = 1;
      item.SetItemId(record.itemId);
      g_session.AddMerchantMirrorItem(item);
    }

    UIMerchantItemShopWnd* shop = static_cast<UIMerchantItemShopWnd*>(g_windowMgr.EnsureWindow(WID_MERCHANTITEMSHOPWND_MODE_1));
    shop->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    shop->SendMsg(nullptr, UM_SETNAID, aid, 0, 0, 0);
    shop->SendMsg(nullptr, UM_SET_BUYING_SHOP_ZENY, value, 0, 0, 0);
    shop->StartTimer(duration);
    UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMPURCHASEWND_MODE_1);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETNAID, aid, storeId, 0, 0);
    UIWindow* list = reinterpret_cast<UIWindow*>(g_windowMgr.purchaseMirrorItemWnd);
    if (list) {
      g_session.RefreshMerchantMyShopItems();
    } else {
      list = g_windowMgr.EnsureWindow(WID_MERCHANTMIRRORITEMWND_MODE_2);
    }
    if (list) {
      list->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
  }

  this->isOnQuest = 1;
}

#pragma pack(push, 1)
struct VendingItemRecord2 {
  /* 0000 */ int price;
  /* 0004 */ std::int16_t amount;
  /* 0006 */ std::int16_t index;
  /* 0008 */ unsigned char type;
  /* 0009 */ int itemId;
  /* 000D */ unsigned char identified;
  /* 000E */ unsigned char damaged;
  /* 000F */ unsigned char refine;
  /* 0010 */ std::uint32_t cards[4];
  /* 0020 */ ItemSlotOption options[5];
  /* 0039 */ int location;
  /* 003D */ std::uint16_t sprite;
};
#pragma pack(pop)

// INLINED
static __forceinline void FillVendingItem(ITEM_INFO& item, const VendingItemRecord2& record) {
  item.identity[1] = record.index;
  item.prices[0] = record.price;
  item.prices[1] = record.price;
  item.identity[0] = record.type;
  item.quantity = record.amount;
  item.identified = record.identified;
  item.damaged = record.damaged;
  item.refiningLevel = record.refine;
  item.SetItemId(record.itemId);
  std::memcpy(item.cards, record.cards, sizeof(record.cards));
  item.optionCount = CountItemSlotOptions(record.options);
  for (int index = 0; index < item.optionCount; ++index) {
    reinterpret_cast<ItemSlotOption*>(item.options)[index] = record.options[index];
  }
  item.identity[2] = record.location;
  item.sprite = record.sprite;
}

// OFFSET: 0x00D47720
// FUNCTION: fn_21850
void CGameMode::ReceiveVendingItemSoldNow(const unsigned char* data) {
  ProcessBuyingStorePurchase(static_cast<int>(_time32(nullptr)), *reinterpret_cast<const int*>(data + 2),
             *reinterpret_cast<const short*>(data + 6), *reinterpret_cast<const int*>(data + 8), 0, 0);
}

// OFFSET: 0x00D6C410
// FUNCTION: fn_22080
void CGameMode::ReceiveTraitStatusChangeAck(unsigned char result, unsigned short statusId, unsigned char value) {
  this->waitingReqStatusAck = 0;
  if (result == 1) {
    std::map<int, int>& traitStats = g_session.m_traits.stats;
    traitStats[statusId] = value;
    if (UIStatusWnd* window = reinterpret_cast<UIStatusWnd*>(g_windowMgr.statusWnd)) {
      window->ValidatePlacement();
    }
  }
}

// OFFSET: 0x00D70790
// FUNCTION: fn_22131
void CGameMode::ReceiveVendingItemList2(int aid, int storeId, char own, int count, std::uint32_t duration, const unsigned char* data) {
  const auto* records = reinterpret_cast<const VendingItemRecord2*>(data);
  if (own == 1) {
    g_session.ClearMerchantPurchaseItems();
    for (int i = 0; i < count; ++i) {
      const VendingItemRecord2 record = records[i];
      ITEM_INFO item;
      FillVendingItem(item, record);
      g_session.AddMerchantPurchaseItem(item);
    }

    UIMerchantItemMyShopWnd* window = dynamic_cast<UIMerchantItemMyShopWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_MERCHANTITEMMYSHOPWND)));
    if (window) {
      window->OpenShop();
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETNAID, aid, 0, 0, 0);
      window->SetDuration(duration);
    }
    this->isOnQuest = 1;
  } else {
    g_session.ClearMerchantMirrorItems();
    g_session.ClearMerchantHopeToSellItems();
    for (int i = 0; i < count; ++i) {
      const VendingItemRecord2 record = records[i];
      ITEM_INFO item;
      FillVendingItem(item, record);
      g_session.AddMerchantMirrorItem(item);
    }

    UIMerchantItemShopWnd* shop = static_cast<UIMerchantItemShopWnd*>(g_windowMgr.EnsureWindow(WID_MERCHANTITEMSHOPWND));
    shop->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    shop->SendMsg(nullptr, UM_SETNAID, aid, 0, 0, 0);
    shop->StartTimer(duration);
    UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMPURCHASEWND);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETNAID, aid, storeId, 0, 0);
    this->isOnQuest = 1;
  }
}

// OFFSET: 0x00D742B0
// FUNCTION: fn_22147
void CGameMode::ReceiveNotifyGroundSkill(const unsigned char* packet) {
  struct GroundSkillMessage {
    /* 0000 */ int skillId;
    /* 0004 */ int startTime;
    /* 0008 */ int x;
    /* 000C */ int y;
    /* 0010 */ int level;
  };

  GroundSkillMessage message;
  message.skillId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  message.startTime = *reinterpret_cast<const std::uint32_t*>(packet + 0xE);
  message.x = *reinterpret_cast<const std::int16_t*>(packet + 0xA);
  message.y = *reinterpret_cast<const std::int16_t*>(packet + 0xC);
  message.level = *reinterpret_cast<const std::int16_t*>(packet + 8);

  CGameActor* actor = nullptr;
  int job = -1;

  if (*reinterpret_cast<const std::uint32_t*>(packet + 4) == g_session.m_aid) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
    this->scene->player->SendMsg(nullptr, AM_GROUNDSKILL, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor = this->scene->player;
    if (actor != nullptr) {
      job = actor->GetEffectiveJob(-1);
    }
  } else {
    CGameActor* gameActor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
    if (gameActor != nullptr) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      gameActor->SendMsg(nullptr, AM_GROUNDSKILL, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor = gameActor;
      job = actor->GetEffectiveJob(-1);
    }
  }

  if (!g_session.IsMonsterActor(actor)) {
    this->SendMsg(MM_MAKE_SKILL_USED_MSG, *reinterpret_cast<const std::uint16_t*>(packet + 2), reinterpret_cast<std::intptr_t>(actor), *reinterpret_cast<const std::int16_t*>(packet + 8), 0);

    switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
    case PA_GOSPEL:
      g_soundMgr->PlayWave("effect\\가스펠.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case HP_BASILICA:
      g_soundMgr->PlayWave("effect\\바실리카.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case AC_SHOWER: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (actor == nullptr) {
        return;
      }
      vector3d* source = &actor->render.position;
      mode->SpawnArrowEffect64(source, source, 18.0f);
      const int offsets[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
      const int cellX = *reinterpret_cast<const std::int16_t*>(packet + 0xA);
      const int cellY = *reinterpret_cast<const std::int16_t*>(packet + 0xC);
      for (int index = 0; index < 8; ++index) {
        float x;
        float z;
        mode->CellCenter(offsets[index][0] + cellX, offsets[index][1] + cellY, x, z);
        vector3d target;
        target.x = x;
        target.y = mode->scene->terrain->Height(x, z);
        target.z = z;
        mode->SpawnArrowEffect64(source, &target, 18.0f);
      }
      break;
    }
    case NJ_RAIGEKISAI: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      this->LaunchDummyEffect(&position, 0x26E, 200, 0);
      break;
    }
    case NPC_CLOUD_KILL:
    case SO_CLOUD_KILL:
      actor->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\S클라우드킬.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case WL_FROSTMISTY: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      this->LaunchDummyEffect(&position, 0x2D6, EffectLifetime(0x2D6) + 10, 0);
      break;
    }
    case NPC_PSYCHIC_WAVE:
    case SO_PSYCHIC_WAVE: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      g_soundMgr->PlayWave("effect\\S사이킥웨이브.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      actor->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      int count = message.level + 2;
      if (count > 7) {
        count = 7;
      } else if (count <= 0) {
        return;
      }
      for (int delay = 0; count != 0; --count, delay += 500) {
        CEffect* effect;
        if (g_serviceOptions.Get(0xE5)) {
          effect = g_effectMgr->Spawn(0, 0x5AE, position.x, position.y, position.z, 0, -1, 0, 0);
          if (effect) {
            effect->SetDelay(delay);
          }
          effect = g_effectMgr->Spawn(0, 0x5AF, position.x, position.y, position.z, 0, -1, 0, 0);
        } else {
          effect = g_effectMgr->Spawn(0, 0x508, position.x, position.y, position.z, 0, -1, 0, 0);
          if (effect) {
            effect->SetDelay(delay);
          }
          effect = g_effectMgr->Spawn(0, 0x509, position.x, position.y, position.z, 0, -1, 0, 0);
        }
        if (effect) {
          effect->SetDelay(delay);
        }
      }
      break;
    }
    case SO_EARTHGRAVE: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      g_soundMgr->PlayWave("effect\\G가시나무덫.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      actor->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      const float y = mode->scene->terrain->Height(x, z);
      int delay = 0;
      for (int degrees = 0; degrees < 1080; degrees += 72) {
        const float radius = static_cast<float>(std::max(std::rand() % 20, 0)) + 5.0f;
        matrix rotation;
        rotation.MakeYRotation(static_cast<float>(degrees) / 180.0f * 3.141592f);
        vector3d position;
        position.x = rotation.values[0] * radius + rotation.values[3] * 0.0f + rotation.values[6] * 0.0f + rotation.values[9] + x;
        position.y = rotation.values[1] * radius + rotation.values[4] * 0.0f + rotation.values[7] * 0.0f + rotation.values[10] + y;
        position.z = rotation.values[2] * radius + rotation.values[5] * 0.0f + rotation.values[8] * 0.0f + rotation.values[11] + z;
        CEffect* effect = g_effectMgr->Spawn(0, 0x5B9, position.x, position.y, position.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetScale(0.5f);
          effect->SetDelay(delay);
        }
        effect = g_effectMgr->Spawn(0, 0x656, position.x, position.y, position.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetScale(0.5f);
          effect->SetDelay(delay);
        }
        delay += 70;
      }
      break;
    }
    case GN_CRAZYWEED: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      g_soundMgr->PlayWave("effect\\G가시나무덫.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      this->LaunchDummyEffect(&position, 0x393, 150, 0);
      break;
    }
    case LG_OVERBRAND: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (actor == nullptr) {
        return;
      }
      float angle = mode->CameraDesiredYaw() + actor->render.angle;
      if (angle >= 360.0f) {
        angle -= 360.0f;
      }
      int effectId = 0;
      if (angle >= 0.0f && angle < 23.0f) {
        effectId = 0x692;
      } else if (angle >= 23.0f && angle < 67.0f) {
        effectId = 0x691;
      } else if (angle >= 67.0f && angle < 112.0f) {
        effectId = 0x690;
      } else if (angle >= 112.0f && angle < 157.0f) {
        effectId = 0x68F;
      } else if (angle >= 157.0f && angle < 202.0f) {
        effectId = 0x696;
      } else if (angle >= 202.0f && angle < 247.0f) {
        effectId = 0x695;
      } else if (angle >= 247.0f && angle < 292.0f) {
        effectId = 0x694;
      } else if (angle >= 292.0f && angle < 337.0f) {
        effectId = 0x693;
      } else if (angle >= 337.0f && angle <= 360.0f) {
        effectId = 0x692;
      }
      g_effectMgr->Spawn(actor->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\LG_OVERBRAND.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case SR_RIDEINLIGHTNING: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      actor->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      this->LaunchDummyEffect(&position, 0x376, 300, 0);
      this->LaunchDummyEffect(&position, 0x375, 200, 0);
      this->LaunchDummyEffect(&position, 0x383, 999, 0);
      g_soundMgr->PlayWave("effect\\SR_RIDEINLIGHTNING.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case WM_LULLABY_DEEPSLEEP: {
      g_soundMgr->PlayWave("effect\\안식의자장가.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      CRagEffect* effect = actor->LaunchEffect(0x35A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect == nullptr) {
        return;
      }
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      break;
    }
    case WM_DOMINION_IMPULSE: {
      CRagEffect* effect = actor->LaunchEffect(0x35F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect == nullptr) {
        return;
      }
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\invisibility.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case SC_CHAOSPANIC:
      g_soundMgr->PlayWave("effect\\chaospanic.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case SC_MAELSTROM:
      g_soundMgr->PlayWave("effect\\maelstrom.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case SC_BLOODYLUST:
      g_soundMgr->PlayWave("effect\\bloodylust.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case SJ_BOOKOFCREATINGSTAR:
      if (actor == nullptr) {
        return;
      }
      g_effectMgr->Spawn(actor->render.accountId, 0x4D0, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, -1, 0, 0);
      break;
    default:
      CSkillActionMgr::GetInstance()->OnGroundSkillEffect(*reinterpret_cast<const std::uint16_t*>(packet + 2), *reinterpret_cast<const std::int16_t*>(packet + 0xA),
                                                   *reinterpret_cast<const std::int16_t*>(packet + 0xC), *reinterpret_cast<const std::int16_t*>(packet + 8),
                                                   reinterpret_cast<int>(actor));
      break;
    case EM_TERRA_DRIVE: {
      vector3d position = {0.0f, 0.0f, 0.0f};
      this->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), position.x, position.z);
      position.y = this->scene->terrain->Height(position.x, position.z);
      g_effectMgr->Spawn(0, 0x856, position.x, position.y, position.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(0, 0x855, position.x, position.y, position.z, 0, -1, 0, 0);
      break;
    }
    case EM_DIAMOND_STORM: {
      vector3d position = {0.0f, 0.0f, 0.0f};
      this->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), position.x, position.z);
      position.y = this->scene->terrain->Height(position.x, position.z);
      g_effectMgr->Spawn(0, 0x7EB, position.x, position.y, position.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(0, 0x7EA, position.x, position.y, position.z, 0, -1, 0, 0);
      break;
    }
    }
  } else {
    int action;
    int loopType;
    g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), *reinterpret_cast<const std::uint16_t*>(packet + 2), &action, &loopType);
    if (*reinterpret_cast<const std::uint16_t*>(packet + 2) == 0x2C8) {
      actor->LaunchEffect(0x199, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (*reinterpret_cast<const std::uint16_t*>(packet + 2) == 0x2E0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      g_soundMgr->PlayWave("effect\\S사이킥웨이브.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      actor->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      float x;
      float z;
      mode->CellCenter(*reinterpret_cast<const std::int16_t*>(packet + 0xA), *reinterpret_cast<const std::int16_t*>(packet + 0xC), x, z);
      vector3d position;
      position.x = x;
      position.y = mode->scene->terrain->Height(x, z);
      position.z = z;
      int count = message.level + 2;
      if (count > 7) {
        count = 7;
      } else if (count <= 0) {
        return;
      }
      for (int delay = 0; count != 0; --count, delay += 500) {
        CEffect* effect;
        if (g_serviceOptions.Get(0xE5)) {
          effect = g_effectMgr->Spawn(0, 0x5AE, position.x, position.y, position.z, 0, -1, 0, 0);
          if (effect) {
            effect->SetDelay(delay);
          }
          effect = g_effectMgr->Spawn(0, 0x5AF, position.x, position.y, position.z, 0, -1, 0, 0);
        } else {
          effect = g_effectMgr->Spawn(0, 0x508, position.x, position.y, position.z, 0, -1, 0, 0);
          if (effect) {
            effect->SetDelay(delay);
          }
          effect = g_effectMgr->Spawn(0, 0x509, position.x, position.y, position.z, 0, -1, 0, 0);
        }
        if (effect) {
          effect->SetDelay(delay);
        }
      }
    }
  }
}

#pragma pack(push, 1)
struct GameAcceptEnterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t startTime;
  /* 0006 */ unsigned char posDir[3];
  /* 0009 */ unsigned char xSize;
  /* 000A */ unsigned char ySize;
  /* 000B */ unsigned char font;
};
#pragma pack(pop)

// OFFSET: 0x00D1B890
// FUNCTION: fn_21620
void CGameMode::ReceiveAcceptEnter(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GameAcceptEnterPacket*>(data);
  if (Replay::Instance().state == 2) {
    Replay::Instance().SetState(3);
    g_windowMgr.replayRecordControlWnd->SendMsg(nullptr, UM_SETPOS, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_REPLAYRECCONTROLWND);
  } else if (Replay::Instance().state == 7) {
    Replay::Instance().SetState(8);
  }

  g_session.SetServerTime(packet->startTime);
  g_session.SetPlayerPosition((packet->posDir[0] << 2) | (packet->posDir[1] >> 6), ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4),
    packet->posDir[2] & 0xF);
  g_session.chatFont[0] = packet->font;

  char mapName[36];
  sprintf(mapName, "%s.rsw", g_session.m_navigationPlayerMap);
  g_modeMgr.Switch(1, mapName);
  g_session.SetOption(17, 0, 0);
  GetRagConnection().SetBlock(0);

  if (g_pingLogEnabled) {
    FILE* file = fopen("PingLog.txt", "at");
    if (file) {
      __time32_t now;
      _time32(&now);
      fprintf(file, "\n");
      fprintf(file, "server:%s\n", g_session.m_accountName.c_str());
      fprintf(file, "ip:%s\n", g_zoneServerEndpoint.address);
      fprintf(file, "map:%s.gat\n", g_session.m_navigationPlayerMap);
      fprintf(file, "time:%s\n\n", _ctime32(&now));
      fclose(file);
    }
    g_pingLogTicks.clear();
    g_pingLogFirstTick = 0;
  }
}

#pragma pack(push, 1)
struct RodexMailListEntry {
  /* 0000 */ std::uint8_t openType;
  /* 0001 */ std::int64_t mailId;
  /* 0009 */ std::uint8_t isRead;
  /* 000A */ std::uint8_t type;
  /* 000B */ char sender[24];
  /* 0023 */ int expireTime;
  /* 0027 */ std::int16_t titleLength;
  /* 0029 */ char title[1];
};
#pragma pack(pop)

// OFFSET: 0x00D68730
// FUNCTION: fn_22043
void CGameMode::ReceiveRodexMailList(const unsigned char* packet) {
  for (int offset = 5; offset < *reinterpret_cast<const std::int16_t*>(packet + 2);) {
    const RodexMailListEntry* entry = reinterpret_cast<const RodexMailListEntry*>(packet + offset);
    RodexMailMap* mails = nullptr;
    switch (entry->openType) {
    case MAIL_INBOX_NORMAL:
      mails = &g_rodexMailStore->m_mails0;
      break;
    case MAIL_INBOX_ACCOUNT:
      mails = &g_rodexMailStore->m_mails1;
      break;
    case MAIL_INBOX_RETURNED:
      mails = &g_rodexMailStore->m_mails2;
      break;
    }

    if (mails) {
      RodexMail mail;
      mail.isRead = entry->isRead;
      mail.type = entry->type;
      mail.title.assign(entry->title, entry->titleLength);
      mail.sender.assign(entry->sender, std::min<std::size_t>(std::strlen(entry->sender), 24));
      mail.expiresAt = _time32(nullptr) + entry->expireTime;
      if (entry->openType == MAIL_INBOX_NORMAL && !(mail.type & 8) && !g_session.HasCharacterName(mail.sender)) {
        mail.isKnownSender = g_session.IsKnownName(mail.sender);
      } else {
        mail.isKnownSender = 1;
      }

      if (mails->find(entry->mailId) == mails->end() && !mail.isRead) {
        ++g_rodexMailStore->unreadCount;
      }
      mails->insert(std::pair<const std::int64_t, RodexMail>(entry->mailId, mail));
    }

    offset += entry->titleLength + 0x29;
  }

  if (packet[4] == 1) {
    g_rodexMailStore->isPageComplete = true;
    UIWindow* window = g_windowMgr.LookupWindow(WID_RODEXINBOXWND);
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
  }
}

#pragma pack(push, 1)
struct StatusChangeListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ unsigned char on;
  /* 0009 */ std::uint16_t values[1];
};

struct StatusChangeExtendedPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t type;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ unsigned char on;
  /* 0009 */ std::uint32_t remaining;
  /* 000D */ int value1;
  /* 0011 */ int value2;
  /* 0015 */ int value3;
};

struct Window227EntriesPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint16_t entries[1];
};
#pragma pack(pop)

// OFFSET: 0x00D6EB70
// FUNCTION: fn_22116
void CGameMode::ReceiveStatusChangeList(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const StatusChangeListPacket*>(data);
  const unsigned int count = (packet->length - 9u) >> 1;
  for (int index = 0; index < static_cast<int>(count); ++index) {
    this->ApplyStatusChange(packet->aid, packet->on, 0x37F, 0, packet->values[index], 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D71A30
// FUNCTION: fn_22139
void CGameMode::ReceiveStatusChangeExtended(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const StatusChangeExtendedPacket*>(data);
  switch (packet->type) {
  case EFST_WHISTLE:
  case EFST_ASSASSINCROSS:
  case EFST_POEMBRAGI:
  case EFST_APPLEIDUN:
  case EFST_HUMMING:
  case EFST_DONTFORGETME:
  case EFST_FORTUNEKISS:
  case EFST_SERVICEFORYOU:
  case EFST_RICHMANKIM:
  case EFST_ETERNALCHAOS:
  case EFST_DRUMBATTLEFIELD:
  case EFST_RINGNIBELUNGEN:
  case EFST_ROKISWEIL:
  case EFST_INTOABYSS:
  case EFST_SIEGFRIED:
  case EFST_UNLIMITED_HUMMING_VOICE:
  case EFST_MUSICAL_INTERLUDE:
  case EFST_JAWAII_SERENADE:
  case EFST_PRON_MARCH:
    return;
  }

  this->ApplyStatusChange(packet->aid, packet->on, packet->type, packet->remaining, packet->value1, packet->value2, packet->value3, 0, 0);
}

// OFFSET: 0x00D71790
// FUNCTION: fn_22137
void CGameMode::ReceiveWindow227Entries(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const Window227EntriesPacket*>(data);
  const unsigned int count = (packet->length - 4u) >> 1;
  if (count == 0) {
    PacketIdOnly request;
    request.id = ID_CZ_CANCEL_MERGE_ITEM;
    GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(request.id), &request);
    return;
  }

  std::vector<std::uint16_t> entries;
  entries.insert(entries.end(), packet->entries, packet->entries + count);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_MERGEITEMWND);
  if (window) {
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(entries.data()), count, 0, 0);
  }
}

#pragma pack(push, 1)
struct ItemDropPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int itemId;
  /* 000A */ std::uint16_t type;
  /* 000C */ unsigned char identified;
  /* 000D */ std::int16_t column;
  /* 000F */ std::int16_t row;
  /* 0011 */ unsigned char subColumn;
  /* 0012 */ unsigned char subRow;
  /* 0013 */ std::int16_t count;
  /* 0015 */ unsigned char showEffect;
  /* 0016 */ std::uint16_t grade;
};
#pragma pack(pop)

struct ItemFallEntry {
  /* 0000 */ char itemId[0x20];
  /* 0020 */ std::uint32_t aid;
  /* 0024 */ int column;
  /* 0028 */ int row;
  /* 002C */ int subColumn;
  /* 0030 */ int subRow;
  /* 0034 */ int falling;
  /* 0038 */ int count;
  /* 003C */ unsigned char identified;
  /* 003E */ std::uint16_t type;
  /* 0040 */ unsigned char showEffect;
  /* 0042 */ std::uint16_t grade;
};

// OFFSET: 0x00D6FB20
// FUNCTION: fn_22126
void CGameMode::ReceiveItemDropEntry(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemDropPacket*>(data);
  ItemFallEntry entry = {};
  _itoa(packet->itemId, entry.itemId, 10);
  entry.column = packet->column;
  entry.aid = packet->aid;
  entry.row = packet->row;
  entry.subColumn = packet->subColumn;
  entry.subRow = packet->subRow;
  entry.count = packet->count;
  entry.identified = packet->identified;
  entry.type = packet->type;
  entry.showEffect = packet->showEffect;
  entry.grade = packet->grade;
  entry.falling = 1;

  CItem* item = this->scene->GetItemByAID(packet->aid);
  if (!item) {
    item = new CItem;
    this->scene->objects.push_back(item);
    this->scene->items.push_back(item);
  }

  const std::int64_t entryAddress = reinterpret_cast<int>(&entry);
  item->SendMsg(nullptr, AM_INITSTANDENTRY, 0, static_cast<int>(entryAddress), static_cast<int>(entryAddress >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
#pragma pack(push, 1)
struct SpriteChangePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ unsigned char type;
  /* 0007 */ unsigned char value;
};

struct SpriteChange2Packet {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::int64_t value;
};
#pragma pack(pop)

// OFFSET: 0x00D44C90
// FUNCTION: fn_21837
void CGameMode::ReceiveSpriteChange(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const SpriteChangePacket*>(data);
  const int bodyCostume = g_session.m_bodyCostume;
  const int bodyStyle = g_session.m_bodyStyle;
  CGameActor* actor;
  if (packet->aid == g_session.m_aid) {
    actor = this->scene->player;
    switch (packet->type) {
    case LOOK_BASE:
      g_session.UpdateBodyStyle(g_session.GetDisplayJob(), bodyStyle, bodyCostume);
      g_session.SetJob(packet->value);
      break;
    case LOOK_HAIR:
      g_session.m_head = packet->value;
      break;
    case LOOK_WEAPON:
      *reinterpret_cast<int*>(&g_session.weaponAppearance[0x4]) = packet->value;
      *reinterpret_cast<int*>(&g_session.weaponAppearance[0x8]) = 0;
      break;
    case LOOK_HEAD_BOTTOM:
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x0]) = packet->value;
      break;
    case LOOK_HEAD_TOP:
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x4]) = packet->value;
      break;
    case LOOK_HEAD_MID:
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x8]) = packet->value;
      break;
    case LOOK_HAIR_COLOR:
      g_session.m_headPalette = packet->value;
      break;
    case LOOK_CLOTHES_COLOR: {
      const unsigned char style = packet->value;
      g_session.UpdateBodyStyle(g_session.GetDisplayJob(), style, bodyCostume);
      break;
    }
    case LOOK_SHIELD:
      *reinterpret_cast<int*>(&g_session.weaponAppearance[0xC]) = packet->value;
      break;
    }
    if (!actor) {
      return;
    }
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
    if (!actor) {
      CRenderObject* skill = this->scene->GetSkillByAID(packet->aid);
      if (!skill) {
        return;
      }
      if (packet->type == LOOK_BASE) {
        skill->SendMsg(nullptr, AM_SETJOB, 0, packet->value, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      return;
    }
  }

  switch (packet->type) {
  case LOOK_BASE:
    actor->SetActorJob(packet->value);
    actor->SetImfFileName();
    actor->SetHeadPaletteName();
    actor->SetBodyPaletteName();
    break;
  case LOOK_HAIR:
    actor->SetSprHead(packet->value);
    actor->SetImfFileName();
    actor->SetHeadPaletteName();
    actor->RefreshAccessory3();
    break;
  case LOOK_WEAPON:
    actor->SetSprWeapon(packet->value, 0);
    break;
  case LOOK_HEAD_BOTTOM:
    actor->SetSprAccessory(packet->value);
    break;
  case LOOK_HEAD_TOP:
    actor->SetSprAccessory2(packet->value);
    break;
  case LOOK_HEAD_MID:
    actor->SetSprAccessory3(packet->value);
    break;
  case LOOK_HAIR_COLOR:
    actor->SetHeadPalette(packet->value);
    break;
  case LOOK_CLOTHES_COLOR:
    actor->SetBodyPalette(packet->value);
    break;
  case LOOK_SHIELD:
    actor->SetSprShield(packet->value);
    break;
  }
}

// OFFSET: 0x00D44760
// FUNCTION: fn_21836
void CGameMode::ReceiveSpriteChange2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const SpriteChange2Packet*>(data);
  const int bodyCostume = g_session.m_bodyCostume;
  const int bodyStyle = g_session.m_bodyStyle;
  const int value = static_cast<int>(packet->value);
  CGameActor* actor;
  if (packet->aid == g_session.m_aid) {
    actor = this->scene->player;
    switch (packet->type) {
    case LOOK_BASE:
      g_session.UpdateBodyStyle(g_session.GetDisplayJob(), bodyStyle, bodyCostume);
      if (g_session.m_job != packet->value) {
        g_session.SetJob(value);
      }
      break;
    case LOOK_HAIR:
      g_session.m_head = value;
      break;
    case LOOK_WEAPON: {
      *reinterpret_cast<int*>(&g_session.weaponAppearance[0x4]) = value;
      const int shield = static_cast<int>(packet->value >> 32);
      *reinterpret_cast<int*>(&g_session.weaponAppearance[0x8]) = shield;
      if (value != 0 && GetWeaponViewValue(shield) == -1) {
        *reinterpret_cast<int*>(&g_session.weaponAppearance[0xC]) = shield;
      }
      break;
    }
    case LOOK_HEAD_BOTTOM:
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x0]) = value;
      break;
    case LOOK_HEAD_TOP:
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x4]) = value;
      break;
    case LOOK_HEAD_MID:
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x8]) = value;
      break;
    case LOOK_HAIR_COLOR:
      g_session.m_headPalette = value;
      break;
    case LOOK_CLOTHES_COLOR:
      g_session.UpdateBodyStyle(g_session.GetDisplayJob(), value, bodyCostume);
      break;
    case LOOK_RESET_COSTUMES:
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x0]) = 0;
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x4]) = 0;
      *reinterpret_cast<int*>(&g_session.accessoryAppearance[0x8]) = 0;
      g_session.m_robe = 0;
      break;
    case LOOK_ROBE:
      g_session.m_robe = value;
      break;
    case LOOK_BODY2:
      g_session.m_bodyCostume = value;
      break;
    case 14:
      g_session.SetFlag(static_cast<char>(value));
      actor->SetSex(value);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MACRO_CHECKER_MAP_SUCCESS)), 0, 0, 0, 0);
      break;
    }
    if (!actor) {
      return;
    }
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
    if (!actor) {
      CRenderObject* skill = this->scene->GetSkillByAID(packet->aid);
      if (!skill) {
        return;
      }
      if (packet->type == LOOK_BASE) {
        skill->SendMsg(nullptr, AM_SETJOB, 0, value, static_cast<int>(packet->value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      return;
    }
  }

  switch (packet->type) {
  case LOOK_BASE:
    actor->SetActorJob(value);
    if (packet->aid == g_session.m_aid) {
      actor->SetBodyPalette(g_session.m_bodyStyle);
    }
    actor->SetImfFileName();
    actor->SetHeadPaletteName();
    actor->SetBodyPaletteName();
    break;
  case LOOK_HAIR:
    actor->SetSprHead(value);
    actor->SetImfFileName();
    actor->SetHeadPaletteName();
    actor->RefreshAccessory3();
    break;
  case LOOK_WEAPON: {
    const int shield = static_cast<int>(packet->value >> 32);
    const int weapon = value;
    const int view = GetWeaponViewValue(shield);
    switch (actor->GetEffectiveJob(-1)) {
    case JT_ASSASSIN:
    case JT_ASSASSIN_H:
    case JT_ASSASSIN_B:
    case JT_GUILLOTINE_CROSS:
    case JT_GUILLOTINE_CROSS_H:
    case JT_GUILLOTINE_CROSS_B:
    case JT_SHADOW_CROSS:
      actor->SetSprWeapon2(value, shield);
      break;
    default:
      actor->SetSprWeapon2(weapon, static_cast<int>(static_cast<std::int64_t>(weapon) >> 32));
      break;
    }
    if (view == -1 || shield == 0) {
      actor->SetSprShield(shield);
    }
    break;
  }
  case LOOK_HEAD_BOTTOM:
    actor->SetSprAccessory(value);
    break;
  case LOOK_HEAD_TOP:
    actor->SetSprAccessory2(value);
    break;
  case LOOK_HEAD_MID:
    actor->SetSprAccessory3(value);
    break;
  case LOOK_HAIR_COLOR:
    actor->SetHeadPalette(value);
    break;
  case LOOK_CLOTHES_COLOR:
    actor->SetBodyPalette(value);
    break;
  case LOOK_SHOES:
    actor->SetSprShoe(value);
    break;
  case LOOK_RESET_COSTUMES:
    actor->SetSprAccessory(0);
    actor->SetSprAccessory2(0);
    actor->SetSprAccessory3(0);
    actor->SetSprRobe(0, 0);
    break;
  case LOOK_ROBE:
    actor->SetSprRobe(value, 0);
    break;
  case LOOK_BODY2:
    actor->SetBodyCostume(value, 1);
    if (StateFlag04(actor->GetStateFlags())) {
      actor->SendMsg(nullptr, AM_DELETE_BIRD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, AM_MAKE_BIRD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (StateFlag20(actor->GetStateFlags())) {
      actor->SendMsg(nullptr, AM_DELETE_WOLF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, AM_MAKE_WOLF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 14:
    actor->SetSex(value);
    break;
  }
}

#pragma pack(push, 1)
struct MapPropertyPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t mapId;
  /* 0004 */ std::uint32_t flags;
};
#pragma pack(pop)

// OFFSET: 0x00D19B30
// FUNCTION: fn_21613
void CGameMode::ReceiveMapProperty(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const MapPropertyPacket*>(data);
  g_mapPropertyMgr->SetValue(packet->mapId, 7, packet->flags & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 8, (packet->flags >> 1) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 9, (packet->flags >> 2) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 15, (packet->flags >> 3) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 16, (packet->flags >> 4) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 17, (packet->flags >> 5) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 18, (packet->flags >> 6) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 19, (packet->flags >> 7) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 20, (packet->flags >> 8) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 40, (packet->flags >> 9) & 1);
  g_mapPropertyMgr->SetValue(packet->mapId, 41, (packet->flags >> 10) & 1);
  g_session.mapPropertyId = packet->mapId;

  g_serviceOptions.Set(229, *reinterpret_cast<int*>(&g_session.interactionFlags[0x0]) != 0);
  *reinterpret_cast<int*>(&g_session.interactionFlags[0x4]) = 0;
  *reinterpret_cast<int*>(&g_session.interactionFlags[0x8]) = 0;
  g_session.partyBlocked = 0;
  if (g_session.GetMapProperty0F()) {
    g_serviceOptions.Set(229, 1);
  }
  if (g_session.GetMapProperty10()) {
    *reinterpret_cast<int*>(&g_session.interactionFlags[0x4]) = 1;
  }
  if (g_session.GetMapProperty11()) {
    *reinterpret_cast<int*>(&g_session.interactionFlags[0x8]) = 1;
  }
  if (g_session.GetMapProperty12()) {
    g_session.partyBlocked = 1;
  }
  if (g_session.GetMapProperty09()) {
    this->scene->isSiegeMode = 1;
  }
  if (g_session.GetMapProperty13()) {
    this->scene->isBattleFieldMode = 1;
  }

  if (GetBaseJob(g_session.GetDisplayJob()) == 0xFCF && !g_mapPropertyMgr->GetValue(g_session.mapPropertyId, 41)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RANKING_NAME)), 0xFFFF64, 0, 0, 0);
  }
  if (GetBaseJob(g_session.GetDisplayJob()) == 0x1082 && !g_mapPropertyMgr->GetValue(g_session.mapPropertyId, 41)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RANKING_NAME)), 0xFFFF64, 0, 0, 0);
  }
  if ((g_session.GetDisplayJob() == 0x108F || g_session.GetDisplayJob() == 0x1091 || g_session.GetDisplayJob() == 0x10CE) &&
      !g_mapPropertyMgr->GetValue(g_session.mapPropertyId, 41)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RANKING_NAME)), 0xFFFF64, 0, 0, 0);
  }
  if (g_mapPropertyMgr->GetValue(packet->mapId, 20)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DISABLE_COSTUMEITEM_MAP)), 0xFFFF64, 0, 0, 0);
  }

  PacketIdOnly request;
  request.id = ID_CZ_BLOCKING_PLAY_CANCEL;
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(request.id), &request);
}

#pragma pack(push, 1)
struct StovePurchaseResultPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int result;
  /* 0006 */ int itemId;
  /* 000A */ int count;
};
#pragma pack(pop)

// OFFSET: 0x00D6B5C0
// FUNCTION: fn_22069
void CGameMode::ReceiveStovePurchaseResult(const unsigned char* data) {
  if (!g_launchPassCommandLine) {
    return;
  }

  const auto* packet = reinterpret_cast<const StovePurchaseResultPacket*>(data);
  std::string message;
  switch (packet->result) {
  case 0: {
    char name[256];
    std::memset(name, 0, sizeof(name));
    int capacity = sizeof(name);
    ITEM_INFO item;
    item.SetItemId(packet->itemId);
    item.GetIdDisplayName(name, capacity, 0);
    message = static_cast<const std::string&>(FormatOwnedString("%s아이템 %d개 지급 완료", name, packet->count));
  }
  case 2:
    if (g_stoveMgr->m_pendingOperation == 2) {
      FetchStoveTermsAgreement();
    } else if (g_stoveMgr->m_pendingOperation == 3) {
      g_stoveMgr->m_pendingOperation = 0;
      UICashShopWnd* window = static_cast<UICashShopWnd*>(g_windowMgr.LookupWindow(WID_CASHSHOP_WND));
      g_stoveMgr->StartPurchase(window->m_cart->m_cartItems);
    }
    break;
  case 1:
    message = static_cast<const std::string&>(FormatOwnedString("Stove 유저가 아닙니다."));
    break;
  case 3:
  case 4:
    message = LookupMessage(MSI_ANOTHER_WORKING);
    break;
  case 5:
  case 6:
  case 7:
    message = static_cast<const std::string&>(FormatOwnedString("아이템 수령 실패"));
    break;
  default:
    message = static_cast<const std::string&>(FormatOwnedString("Unknown Error (Stove:%d)", packet->result));
    break;
  }

  g_stoveMgr->m_pendingOperation = 0;
  if (message.size() != 0) {
    g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }
}

#pragma pack(push, 1)
struct OngoingQuestListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ int count;
};

struct OngoingQuestEntry {
  /* 0000 */ unsigned int questId;
  /* 0004 */ unsigned char state;
  /* 0005 */ int startTime;
  /* 0009 */ int endTime;
  /* 000D */ std::int16_t huntCount;
};

struct OngoingQuestHunt {
  /* 0000 */ int huntId;
  /* 0004 */ int conditionFlags;
  /* 0008 */ int mobId;
  /* 000C */ std::int16_t minLevel;
  /* 000E */ std::int16_t maxLevel;
  /* 0010 */ std::int16_t count;
  /* 0012 */ std::int16_t maxCount;
  /* 0014 */ char name[24];
};

struct OngoingQuestHunt2 {
  /* 0000 */ std::int64_t huntId;
  /* 0008 */ int conditionFlags;
  /* 000C */ int mobId;
  /* 0010 */ std::int16_t minLevel;
  /* 0012 */ std::int16_t maxLevel;
  /* 0014 */ std::int16_t count;
  /* 0016 */ std::int16_t maxCount;
  /* 0018 */ char name[24];
};
#pragma pack(pop)

// OFFSET: 0x00D6C510
// FUNCTION: fn_22082
void CGameMode::ReceiveOngoingQuestList(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const OngoingQuestListPacket*>(data);
  const unsigned char* cursor = data + sizeof(OngoingQuestListPacket);
  for (int index = 0; index < packet->count; ++index) {
    const auto* entry = reinterpret_cast<const OngoingQuestEntry*>(cursor);
    cursor += sizeof(OngoingQuestEntry);
    if (g_renewQuestMgr->SetOngoingQuestState(entry->questId, entry->state)) {
      cursor += entry->huntCount * sizeof(OngoingQuestHunt);
      continue;
    }

    RenewQuestOngoingInfo info;
    info.m_state = entry->state;
    __time32_t endTime = 0;
    if (entry->endTime > 0) {
      endTime = entry->endTime;
    }
    const tm time = *_localtime32(&endTime);
    info.m_year = time.tm_year + 1900;
    info.m_month = time.tm_mon + 1;
    info.m_day = time.tm_mday;
    info.m_hour = entry->endTime != 0 ? time.tm_hour : 0;
    info.m_minute = entry->endTime != 0 ? time.tm_min : 0;
    info.m_huntCount = static_cast<char>(entry->huntCount);

    for (int i = 0; i < entry->huntCount; ++i) {
      const auto* hunt = reinterpret_cast<const OngoingQuestHunt*>(cursor);
      cursor += sizeof(OngoingQuestHunt);
      info.m_huntId[i] = hunt->huntId;
      info.m_huntUnknown01A0[i] = hunt->conditionFlags;
      info.m_huntUnknown00D4[i] = hunt->mobId;
      info.m_huntUnknown00E0[i] = hunt->count;
      info.m_huntUnknown0180[i] = hunt->maxCount;
      info.m_huntUnknown01AC[i] = hunt->minLevel;
      info.m_huntUnknown01B2[i] = hunt->maxLevel;
      const int mobId = hunt->mobId;
      if ((mobId < 1001 || mobId > 3998) && static_cast<unsigned int>(mobId - 20021) > 11978) {
        if (mobId == 0x1099) {
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND));
        } else if (mobId == 0x109A) {
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND_FAILED));
        } else if (mobId == 0x287B) {
          const std::string mapName = FormatOwnedString("%s.rsw", hunt->name);
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetMapDisplayName(mapName.c_str()));
        } else {
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetJobName(mobId, 99));
        }
      } else {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", hunt->name);
      }
    }

    g_renewQuestMgr->AddOngoingQuest(entry->questId, info);
  }

  g_renewQuestMgr->UpdateQuestWindows();
}

// OFFSET: 0x00D6CAD0
// FUNCTION: fn_22083
void CGameMode::ReceiveOngoingQuestList2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const OngoingQuestListPacket*>(data);
  const unsigned char* cursor = data + sizeof(OngoingQuestListPacket);
  for (int index = 0; index < packet->count; ++index) {
    const auto* entry = reinterpret_cast<const OngoingQuestEntry*>(cursor);
    cursor += sizeof(OngoingQuestEntry);
    if (g_renewQuestMgr->SetOngoingQuestState(entry->questId, entry->state)) {
      cursor += entry->huntCount * sizeof(OngoingQuestHunt2);
      continue;
    }

    RenewQuestOngoingInfo info;
    info.m_state = entry->state;
    __time32_t endTime = 0;
    if (entry->endTime > 0) {
      endTime = entry->endTime;
    }
    const tm time = *_localtime32(&endTime);
    info.m_year = time.tm_year + 1900;
    info.m_month = time.tm_mon + 1;
    info.m_day = time.tm_mday;
    info.m_hour = entry->endTime != 0 ? time.tm_hour : 0;
    info.m_minute = entry->endTime != 0 ? time.tm_min : 0;
    info.m_huntCount = static_cast<char>(entry->huntCount);

    for (int i = 0; i < entry->huntCount; ++i) {
      const auto* hunt = reinterpret_cast<const OngoingQuestHunt2*>(cursor);
      cursor += sizeof(OngoingQuestHunt2);
      info.m_huntId[i] = hunt->huntId;
      info.m_huntUnknown01A0[i] = hunt->conditionFlags;
      info.m_huntUnknown00D4[i] = hunt->mobId;
      info.m_huntUnknown00E0[i] = hunt->count;
      info.m_huntUnknown0180[i] = hunt->maxCount;
      info.m_huntUnknown01AC[i] = hunt->minLevel;
      info.m_huntUnknown01B2[i] = hunt->maxLevel;
      const int mobId = hunt->mobId;
      if ((mobId < 1001 || mobId > 3998) && static_cast<unsigned int>(mobId - 20021) > 11978) {
        if (mobId == 0x1099) {
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND));
        } else if (mobId == 0x109A) {
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND_FAILED));
        } else if (mobId == 0x287B) {
          const std::string mapName = FormatOwnedString("%s.rsw", hunt->name);
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetMapDisplayName(mapName.c_str()));
        } else {
          StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetJobName(mobId, 99));
        }
      } else {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", hunt->name);
      }
    }

    g_renewQuestMgr->AddOngoingQuest(entry->questId, info);
  }

  g_renewQuestMgr->UpdateQuestWindows();
}

#pragma pack(push, 1)
struct ShopItemListEntry {
  /* 0000 */ int price;
  /* 0004 */ std::int16_t amount;
  /* 0006 */ std::int16_t index;
  /* 0008 */ unsigned char type;
  /* 0009 */ int itemId;
  /* 000D */ unsigned char identified;
  /* 000E */ unsigned char damaged;
  /* 000F */ std::uint32_t cards[4];
  /* 001F */ ItemSlotOption options[5];
  /* 0038 */ std::uint32_t location;
  /* 003C */ std::uint16_t sprite;
  /* 003E */ unsigned char refine;
  /* 003F */ unsigned char grade;
};

struct ShopItemListHeader {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ int value;
  /* 0008 */ int argument;
  /* 000C */ unsigned char myShop;
  /* 000D */ std::uint32_t duration;
};
#pragma pack(pop)

// OFFSET: 0x00D703A0
// FUNCTION: fn_22130
void CGameMode::ReceiveShopItemListPacket(const unsigned char* data) {
  const auto* header = reinterpret_cast<const ShopItemListHeader*>(data);
  const auto* entries = reinterpret_cast<const ShopItemListEntry*>(data + sizeof(ShopItemListHeader));
  const unsigned int count = (header->length - sizeof(ShopItemListHeader)) / sizeof(ShopItemListEntry);
  std::vector<ITEM_INFO> items;
  if (count != 0) {
    items.reserve(count);
  }

  for (long long i = 0; i < count; ++i) {
    const ShopItemListEntry& entry = entries[i];
    ITEM_INFO item;
    item.identity[1] = entry.index;
    item.prices[0] = entry.price;
    item.identity[0] = entry.type;
    item.quantity = entry.amount;
    item.identified = entry.identified;
    item.damaged = entry.damaged;
    item.refiningLevel = entry.refine;
    item.prices[1] = entry.price;
    item.SetItemId(entry.itemId);
    std::memcpy(item.cards, entry.cards, sizeof(entry.cards));
    item.optionCount = CountItemSlotOptions(entry.options);
    for (long long option = 0; option < static_cast<unsigned int>(item.optionCount); ++option) {
      reinterpret_cast<ItemSlotOption*>(item.options)[option] = entry.options[option];
    }
    item.identity[2] = entry.location;
    item.sprite = entry.sprite;
    item.grade = entry.grade;
    items.push_back(item);
  }

  this->ReceiveShopItemList(items, header->value, 0, header->argument, 0, header->duration, header->myShop);
}

// OFFSET: 0x00D71720
// FUNCTION: fn_22136
void CGameMode::RequestLocalMapMove(const char* mapName, std::uint16_t x, std::uint16_t y) {
  struct MapMovePacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ char mapName[16];
    /* 0012 */ std::uint16_t x;
    /* 0014 */ std::uint16_t y;
  } packet;

  g_session.m_localMapMove = 1;
  packet.id = 0x91;
  strcpy_s(packet.mapName, sizeof(packet.mapName), mapName);
  packet.x = x;
  packet.y = y;
  this->ReceiveMapMove(reinterpret_cast<const unsigned char*>(&packet));
}

#pragma pack(push, 1)
struct GradeEnchantBroadcastPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
  /* 001A */ int itemId;
  /* 001E */ std::int16_t grade;
  /* 0020 */ unsigned char success;
};
#pragma pack(pop)

// OFFSET: 0x00D6D400
// FUNCTION: fn_22089
void CGameMode::ReceiveRefineStatus(const char* playerName, int itemId, unsigned char refine, bool success) {
  std::string name(playerName, 24);
  std::string itemName;
  ITEM_INFO item;
  item.SetItemId(itemId);
  item.identified = 1;
  {
    itemName = item.DisplayName(0);
  }

  std::string text = FormatOwnedString(LookupMessage(!success + 3271), name.c_str(), refine, itemName.c_str());
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    message.lines.push_back(BroadcastBalloonLine{text, 0x62B0FF, 0});
    balloon->AddMessage(0, message);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x62B0FF, 0, 0, 0);
}

// OFFSET: 0x00D6F000
// FUNCTION: fn_22122
void CGameMode::ReceiveGradeEnchantBroadcast(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GradeEnchantBroadcastPacket*>(data);
  std::string name(packet->name, 24);
  std::string itemName;
  ITEM_INFO item;
  item.SetItemId(packet->itemId);
  item.identified = 1;
  {
    itemName = item.DisplayName(0);
  }

  std::string grade;
  if (packet->grade == 1) {
    grade = "D";
  } else if (packet->grade == 2) {
    grade = "C";
  } else if (packet->grade == 3) {
    grade = "B";
  } else if (packet->grade == 4) {
    grade = "A";
  } else if (packet->grade == 5) {
    grade = "R";
  } else if (packet->grade == 6) {
    grade = "S";
  } else if (packet->grade == 7) {
    grade = "SS";
  }

  std::string text;
  if (packet->success == 1) {
    text = static_cast<const std::string&>(
      FormatOwnedString(LookupMessage(MSI_CHANGE_MATERIAL), name.c_str(), grade.c_str(), itemName.c_str())
    );
  } else {
    text = static_cast<const std::string&>(
      FormatOwnedString(LookupMessage(MSI_CHANGE_MATERIAL_SORT), name.c_str(), grade.c_str(), itemName.c_str())
    );
  }

  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    message.lines.emplace_back(text, 0x62B0FF, 0);
    balloon->AddMessage(0, message);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x62B0FF, 0, 0, 0);
}

#pragma pack(push, 1)
struct RodexMailItem {
  /* 0000 */ std::uint16_t quantity;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char identified;
  /* 0007 */ unsigned char damaged;
  /* 0008 */ std::uint32_t cards[4];
  /* 0018 */ std::uint32_t expireTime;
  /* 001C */ unsigned char type;
  /* 001D */ std::uint16_t bindType;
  /* 001F */ std::uint16_t wearState;
  /* 0021 */ ItemSlotOption options[5];
  /* 003A */ unsigned char refine;
  /* 003B */ unsigned char grade;
};

struct RodexReadMailPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ unsigned char type;
  /* 0005 */ std::int64_t mailId;
  /* 000D */ std::int16_t textLength;
  /* 000F */ std::int64_t zeny;
  /* 0017 */ unsigned char itemCount;
  /* 0018 */ char text[1];
};
#pragma pack(pop)

// OFFSET: 0x00D6B120
// FUNCTION: fn_22068
void CGameMode::ReceiveRodexReadMail(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RodexReadMailPacket*>(data);
  g_session.ClearItemQuantities();
  g_rodexMailStore->currentMailId = packet->mailId;
  g_rodexMailStore->inboxType = packet->type;

  RodexMailMap* mails;
  switch (packet->type) {
  case MAIL_INBOX_NORMAL:
    mails = &g_rodexMailStore->m_mails0;
    break;
  case MAIL_INBOX_ACCOUNT:
    mails = &g_rodexMailStore->m_mails1;
    break;
  case MAIL_INBOX_RETURNED:
    mails = &g_rodexMailStore->m_mails2;
    break;
  default:
    return;
  }

  RodexMailMap::iterator found = mails->find(packet->mailId);
  if (found == mails->end()) {
    return;
  }

  RodexMail& mail = found->second;
  if (!mail.isRead && --g_rodexMailStore->unreadCount == 0) {
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 476, 0, 0);
    }
  }

  mail.body.append(packet->text, packet->textLength);
  mail.hasItem = packet->itemCount;
  mail.zeny = packet->zeny;
  mail.isRead = 1;

  auto* mailItems = reinterpret_cast<RodexMailItem*>(mail.attachments.data());
  const auto* packetItems = reinterpret_cast<const RodexMailItem*>(packet->text + packet->textLength);
  for (long long i = 0; i < packet->itemCount; ++i) {
    RodexMailItem& record = mailItems[i];
    memcpy_s(&record, sizeof(RodexMailItem), &packetItems[i], sizeof(RodexMailItem));

    ITEM_INFO item;
    item.identity[0] = record.type;
    item.identity[2] = record.expireTime;
    item.quantity = record.quantity;
    std::memcpy(item.cards, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.identified;
    item.damaged = record.damaged;
    item.refiningLevel = record.refine;
    item.bindOnEquip = record.wearState;
    item.sprite = record.bindType;
    item.optionCount = CountItemSlotOptions(record.options);
    for (long long option = 0; option < item.optionCount; ++option) {
      reinterpret_cast<ItemSlotOption*>(item.options)[option] = record.options[option];
    }
    item.grade = record.grade;
    g_session.AddItemToFreeSlot(item);
  }

  if (!g_windowMgr.CloseWindow(WID_RODEXCONTENTSWND)) {
    g_windowMgr.EnsureWindow(WID_RODEXCONTENTSWND);
    g_windowMgr.rodexContentsWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

#pragma pack(push, 1)
struct RodexMailPageEntry {
  /* 0000 */ std::int64_t mailId;
  /* 0008 */ unsigned char isRead;
  /* 0009 */ unsigned char type;
  /* 000A */ char sender[24];
  /* 0022 */ int sentAt;
  /* 0026 */ int expireTime;
  /* 002A */ std::int16_t titleLength;
  /* 002C */ char title[1];
};

struct RodexMailPagePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ unsigned char type;
  /* 0005 */ unsigned char count;
  /* 0006 */ bool isEnd;
  /* 0007 */ RodexMailPageEntry entries[1];
};
#pragma pack(pop)

// OFFSET: 0x00D70BF0
// FUNCTION: fn_22132
void CGameMode::ReceiveRodexMailListRefresh(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RodexMailPagePacket*>(data);
  const RodexMailPageEntry* entry = packet->entries;
  g_rodexMailStore->inboxType = packet->type;
  g_rodexMailStore->isPageComplete = packet->isEnd;

  RodexMailMap* mails;
  if (g_rodexMailStore->inboxType == 0) {
    mails = &g_rodexMailStore->m_mails0;
  } else if (g_rodexMailStore->inboxType == 1) {
    mails = &g_rodexMailStore->m_mails1;
  } else if (g_rodexMailStore->inboxType == 2) {
    mails = &g_rodexMailStore->m_mails2;
  }
  mails->clear();

  for (int i = 0; i < packet->count; ++i) {
    RodexMail mail;
    mail.isRead = entry->isRead;
    mail.type = entry->type;
    mail.expiresAt = _time32(nullptr) + entry->expireTime;
    mail.sender.assign(entry->sender, (std::min)(std::strlen(entry->sender), static_cast<std::size_t>(24)));
    mail.title.assign(entry->title, entry->titleLength);
    if (mails->find(entry->mailId) == mails->end()) {
      mails->insert(std::make_pair(entry->mailId, mail));
    }
    entry = reinterpret_cast<const RodexMailPageEntry*>(reinterpret_cast<const char*>(entry) + 0x2C + entry->titleLength);
  }

  if (g_windowMgr.rodexInboxWnd) {
    reinterpret_cast<UIRodexInboxWnd*>(g_windowMgr.rodexInboxWnd)->InitializeMailActions();
    reinterpret_cast<UIRodexInboxWnd*>(g_windowMgr.rodexInboxWnd)->SetButtonsDisabled(0);
  }
}

// OFFSET: 0x00D19000
// FUNCTION: fn_21604
void CRenewQuestMgr::SetOngoingQuest(unsigned int id, const RenewQuestOngoingInfo& info) {
  this->m_ongoingQuests[id] = info;
}

#pragma pack(push, 1)
struct SpecialBroadcastPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char name[24];
  /* 001A */ int value;
  /* 001E */ unsigned char index;
  /* 001F */ unsigned char type;
};
#pragma pack(pop)

// OFFSET: 0x00D44F50
// FUNCTION: fn_21838
void CGameMode::ReceiveSpecialBroadcast(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const SpecialBroadcastPacket*>(data);
  CPlayer* player = this->scene->player;
  UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon();
  if (!balloon) {
    return;
  }

  std::string text;
  switch (packet->type) {
  case 0:
    text.assign(FormatOwnedString(LookupMessage(packet->index + 837), g_session.DecodePlayerName(), packet->name));
    break;
  case 1:
    text.assign(FormatOwnedString(LookupMessage(packet->index + 840), g_session.DecodePlayerName(), packet->name));
    player->LaunchEffect(543, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\T_피링.wav", 0.0f, -250.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 10:
    text.assign(FormatOwnedString(LookupMessage(packet->index + 843), g_session.DecodePlayerName(), g_session.GetJobName(packet->value, -1)));
    break;
  case 11:
    text.assign(FormatOwnedString(LookupMessage(packet->index + 846), g_session.DecodePlayerName(), g_session.GetJobName(packet->value, -1)));
    player->LaunchEffect(543, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\T_피링.wav", 0.0f, -250.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 20:
    text.assign(FormatOwnedString(LookupMessage(MSI_TAEKWON_MISSION), packet->name, packet->index));
    player->LaunchEffect(543, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 21:
    text.assign(FormatOwnedString(LookupMessage(MSI_GANGSI_MISSION), packet->name));
    player->LaunchEffect(543, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 22: {
    ITEM_INFO item;
    item.SetItemId(packet->value);
    char itemName[256];
    int capacity = sizeof(itemName);
    item.GetIdDisplayName(itemName, capacity, 0);
    text.assign(FormatOwnedString(LookupMessage(MSI_COLLECTOR_MISSION), itemName));
    player->LaunchEffect(543, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  case 30:
    text = LookupMessage(MSI_STAR_ANGEL);
    player->LaunchEffect(432, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 40:
    text = std::format("Target HP : {}", packet->value);
    break;
  }

  BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
  message.start = std::chrono::system_clock::now();
  message.fontHeight = 14;
  std::uint32_t color = 0xFFC8C8;
  message.lines.emplace_back(text, color);
  balloon->AddMessage(0, message);
}

#pragma pack(push, 1)
struct QuestHuntUpdateEntry {
  /* 0000 */ unsigned int questId;
  /* 0004 */ __int64 huntId;
  /* 000C */ short maxCount;
  /* 000E */ short count;
};

struct QuestHuntUpdatePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::int16_t count;
  /* 0006 */ QuestHuntUpdateEntry entries[1];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct QuestHuntUpdateEntry2 {
  /* 0000 */ unsigned int questId;
  /* 0004 */ unsigned int huntId;
  /* 0008 */ short maxCount;
  /* 000A */ short count;
};

struct QuestHuntUpdatePacket2 {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::int16_t count;
  /* 0006 */ QuestHuntUpdateEntry2 entries[1];
};
#pragma pack(pop)

// OFFSET: 0x00D47760
// FUNCTION: fn_21851
void CGameMode::ReceiveQuestHuntUpdate(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const QuestHuntUpdatePacket*>(data);
  std::vector<std::string> notices;
  const int count = (packet->length - 6) / sizeof(QuestHuntUpdateEntry);
  for (int i = 0; i < count; ++i) {
    const QuestHuntUpdateEntry& entry = packet->entries[i];
    const RenewQuestOngoingInfo& quest = g_renewQuestMgr->GetOngoingQuest(entry.questId);
    if (quest.m_title.empty()) {
      continue;
    }

    RenewQuestOngoingInfo info = quest;
    int index = 0;
    for (; index < 3; ++index) {
      if (info.m_huntId[index] == entry.huntId) {
        break;
      }
    }
    if (index < 3) {
      info.m_huntUnknown00E0[index] = entry.count;
      info.m_huntUnknown0180[index] = entry.maxCount;
      notices.push_back(this->HuntTargetText(info, index));

      std::string name;
      if (info.m_huntUnknown00D4[index] == 0x7CFF) {
        name.assign(g_renewQuestMgr->GetMonsterConditionText(info.m_huntUnknown01A0[index], info.m_huntUnknown01AC[index], info.m_huntUnknown01B2[index]));
      } else {
        name.assign(info.m_huntName[index]);
      }
      name = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_CLEAR_QUEST_MONSTER), info.m_title.c_str(), name.c_str(), info.m_huntUnknown00E0[index], entry.maxCount));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(name.c_str()), 0xFFFF, 19, 0, 0);
      g_renewQuestMgr->SetOngoingQuest(entry.questId, info);
    }
  }

  if (!notices.empty()) {
    this->ShowNoticeBalloon(&notices);
  }
  if (g_windowMgr.questDisplayWnd != nullptr) {
    g_windowMgr.questDisplayWnd->Invalidate();
  }
  g_renewQuestMgr->UpdateOngoingQuestInfo();
}

// OFFSET: 0x00D47C20
// FUNCTION: fn_21852
void CGameMode::ReceiveQuestHuntUpdate2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const QuestHuntUpdatePacket2*>(data);
  std::vector<std::string> notices;
  const int count = (packet->length - 6) / sizeof(QuestHuntUpdateEntry2);
  for (int i = 0; i < count; ++i) {
    const QuestHuntUpdateEntry2& entry = packet->entries[i];
    const RenewQuestOngoingInfo& quest = g_renewQuestMgr->GetOngoingQuest(entry.questId);
    if (quest.m_title.empty()) {
      continue;
    }

    RenewQuestOngoingInfo info = quest;
    int index = 0;
    for (; index < 3; ++index) {
      if (info.m_huntId[index] == entry.huntId) {
        break;
      }
    }
    if (index < 3) {
      info.m_huntUnknown00E0[index] = entry.count;
      info.m_huntUnknown0180[index] = entry.maxCount;
      notices.push_back(this->HuntTargetText(info, index));

      std::string name;
      if (info.m_huntUnknown00D4[index] == 0x7CFF) {
        name.assign(g_renewQuestMgr->GetMonsterConditionText(info.m_huntUnknown01A0[index], info.m_huntUnknown01AC[index], info.m_huntUnknown01B2[index]));
      } else {
        name.assign(info.m_huntName[index]);
      }
      name = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_CLEAR_QUEST_MONSTER), info.m_title.c_str(), name.c_str(), info.m_huntUnknown00E0[index], entry.maxCount));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(name.c_str()), 0xFFFF, 19, 0, 0);
      g_renewQuestMgr->SetOngoingQuest(entry.questId, info);
    }
  }

  if (!notices.empty()) {
    this->ShowNoticeBalloon(&notices);
  }
  if (g_windowMgr.questDisplayWnd != nullptr) {
    g_windowMgr.questDisplayWnd->Invalidate();
  }
  g_renewQuestMgr->UpdateOngoingQuestInfo();
}

// OFFSET: 0x00D6A0D0
// FUNCTION: fn_22058
void CGameMode::ReceiveRodexMailListAppend(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RodexMailPagePacket*>(data);
  const RodexMailPageEntry* entry = packet->entries;
  g_rodexMailStore->inboxType = packet->type;
  g_rodexMailStore->isPageComplete = packet->isEnd;

  if (packet->count != 0) {
    for (int i = 1; i < packet->count + 1; ++i) {
      RodexMail mail;
      mail.isRead = entry->isRead;
      mail.type = entry->type;
      mail.expiresAt = _time32(nullptr) + entry->expireTime;
      mail.sender.assign(entry->sender, (std::min)(std::strlen(entry->sender), static_cast<std::size_t>(24)));
      if (packet->type == MAIL_INBOX_NORMAL && (mail.type & 8) == MAIL_INBOX_NORMAL && !g_session.HasCharacterName(mail.sender)) {
        mail.isKnownSender = g_session.IsKnownName(mail.sender);
      } else {
        mail.isKnownSender = 1;
      }
      mail.title.append(entry->title, entry->titleLength);

      switch (packet->type) {
      case MAIL_INBOX_NORMAL:
        if (g_rodexMailStore->m_mails0.find(entry->mailId) == g_rodexMailStore->m_mails0.end()) {
          g_rodexMailStore->m_mails0.insert(std::make_pair(entry->mailId, mail));
        }
        break;
      case MAIL_INBOX_ACCOUNT:
        if (g_rodexMailStore->m_mails1.find(entry->mailId) == g_rodexMailStore->m_mails1.end()) {
          g_rodexMailStore->m_mails1.insert(std::make_pair(entry->mailId, mail));
        }
        break;
      case MAIL_INBOX_RETURNED:
        if (g_rodexMailStore->m_mails2.find(entry->mailId) == g_rodexMailStore->m_mails2.end()) {
          g_rodexMailStore->m_mails2.insert(std::make_pair(entry->mailId, mail));
        }
        break;
      }
      entry = reinterpret_cast<const RodexMailPageEntry*>(reinterpret_cast<const char*>(entry) + 0x2C + entry->titleLength);
    }
  }

  if (g_windowMgr.rodexInboxWnd) {
    static_cast<UIRodexInboxWnd*>(g_windowMgr.rodexInboxWnd)->RefreshMailLists();
    static_cast<UIRodexInboxWnd*>(g_windowMgr.rodexInboxWnd)->SetButtonsDisabled(0);
    static_cast<UIRodexInboxWnd*>(g_windowMgr.rodexInboxWnd)->ClearChecks();
  }
}

#include "Contents/Skills/ClientSkills.h"

#pragma pack(push, 1)
struct StatusChangeNoticePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t type;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ unsigned char state;
};
#pragma pack(pop)

// OFFSET: 0x00D71AB0
// FUNCTION: fn_22140
void CGameMode::ReceiveStatusChangeNotice(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const StatusChangeNoticePacket*>(data);
  if (packet->aid != g_session.m_aid) {
    if (CRenderObject* actor = this->scene->GetGameActorByAID(packet->aid)) {
      const std::int64_t state = packet->state;
      const std::int64_t type = packet->type;
      actor->SendMsg(nullptr, AM_STATUS_CHANGE, 0, static_cast<int>(type), static_cast<int>(type >> 32), static_cast<int>(state), static_cast<int>(state >> 32), 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  MessengerMember member = g_session.PartyByAccount(packet->aid);
  if (packet->aid != g_session.m_aid && (!member.valid || member.offline)) {
    return;
  }

  char name[256];
  std::sprintf(name, "");
  if (packet->aid != g_session.m_aid) {
    std::sprintf(name, LookupMessage(MSI_CHARACTER_NAME), member.name.c_str());
  }

  char message[256];
  std::memset(message, 0, sizeof(message));

  std::uint32_t color;
  if (packet->state) {
    if (packet->aid == g_session.m_aid) {
      ActiveSkillEffect effect;
      effect.id = packet->type;
      effect.expiration = 999999;
      if (!this->IsExcludedSkillEffectId(packet->type)) {
        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == packet->type) {
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }
      }
      g_activeSkillEffects.push_back(effect);
    }

    color = 0xFFFF00;
    switch (packet->type) {
    case EFST_TWOHANDQUICKEN:
    case EFST_ATTHASTE_POTION1:
    case EFST_ATTHASTE_POTION2:
    case EFST_ATTHASTE_POTION3:
    case EFST_ATTHASTE_INFINITY:
    case EFST_SPEARQUICKEN:
    case EFST_ONEHANDQUICKEN:
      std::sprintf(message, LookupMessage(MSI_INCATTACKSPEED));
      break;
    case EFST_SV_ROOTTWIST:
    case EFST_BITESCAR:
    case EFST_TUNAPARTY:
    case EFST_SPRITEMABLE:
    case EFST_SOULATTACK:
      break;
    case EFST_IMPOSITIO:
      std::sprintf(message, LookupMessage(MSI_CHECK_ATTENDANCE_DATE_INFO));
      break;
    case EFST_SUFFRAGIUM:
      std::sprintf(message, LookupMessage(MSI_FASTPREDELAY));
      break;
    case EFST_ENCHANTPOISON:
      std::sprintf(message, LookupMessage(MSI_WEAPONPROPERTYPOISON));
      break;
    case EFST_ASPERSIO:
    case EFST_WEAPONPROPERTY:
      std::sprintf(message, LookupMessage(MSI_WEAPONPROPERTYSAINT));
      break;
    case EFST_BENEDICTIO:
      std::sprintf(message, LookupMessage(MSI_ARMORPROPERTYSAINT));
      break;
    case EFST_KYRIE:
      std::sprintf(message, LookupMessage(MSI_BARRIOR));
      break;
    case EFST_WEAPONPERFECT:
      std::sprintf(message, LookupMessage(MSI_PERFECTDAMAGE));
      break;
    case EFST_OVERTHRUST:
    case EFST_OVERTHRUSTMAX:
      std::sprintf(message, LookupMessage(MSI_INCWEAPONATTACK));
      break;
    case EFST_MAXIMIZE:
      std::sprintf(message, LookupMessage(MSI_MAXIMIZE));
      break;
    case EFST_INC_AGI:
    case EFST_MOVHASTE_POTION:
    case EFST_MOVHASTE_INFINITY:
      std::sprintf(message, LookupMessage(MSI_SPEEDUP));
      break;
    case EFST_DEC_AGI:
      std::sprintf(message, LookupMessage(MSI_SPEEDDOWN));
      break;
    case EFST_POISONREACT:
      std::sprintf(message, LookupMessage(MSI_ENST_POISONREACT));
      break;
    case EFST_QUAGMIRE:
      std::sprintf(message, LookupMessage(MSI_ENST_QUAGMIRE));
      break;
    case EFST_ANGELUS:
      std::sprintf(message, LookupMessage(MSI_CASHSHOP_TAB_PREMIUM_SERVICE));
      break;
    case EFST_BLESSING:
      std::sprintf(message, LookupMessage(MSI_CASHSHOP_TAB_EFST_AID));
      break;
    case EFST_SLOWPOISON:
      std::sprintf(message, LookupMessage(MSI_ENST_SLOWPOISON));
      break;
    case EFST_MAGNIFICAT:
      std::sprintf(message, LookupMessage(MSI_ENST_MAGNIFICAT));
      break;
    case EFST_GLORIA:
      std::sprintf(message, LookupMessage(MSI_ENST_GLORIA));
      break;
    case EFST_LEXAETERNA:
      std::sprintf(message, LookupMessage(MSI_ENST_LEXAETERNA));
      break;
    case EFST_ADRENALINE:
    case EFST_ADRENALINE2:
      std::sprintf(message, LookupMessage(MSI_ENST_ADRENALINE));
      break;
    case EFST_TRICKDEAD:
      std::sprintf(message, "");
      if (packet->aid == g_session.m_aid) {
        this->scene->player->SetAction(64, 4, 1);
        this->scene->player->render.isMotionFreezed = true;
      }
      break;
    case EFST_SHOUT:
      std::sprintf(message, LookupMessage(MSI_CASHSHOP_TAB_JELLSTAR));
      break;
    case EFST_ENERGYCOAT:
      std::sprintf(message, LookupMessage(MSI_ENST_ENERGYCOAT));
      break;
    case EFST_BROKENARMOR:
      std::sprintf(message, LookupMessage(MSI_ENST_BROKENAMOR));
      break;
    case EFST_BROKENWEAPON:
      std::sprintf(message, LookupMessage(MSI_ENST_BROKENWEAPON));
      break;
    case EFST_CHASEWALK2:
      if (packet->aid == g_session.m_aid) {
        g_soundMgr->PlayWave("lava_golem_move.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
      std::sprintf(message, "");
      break;
    case EFST_PROVOKE:
    case EFST_ENDURE:
    case EFST_CONCENTRATION:
    case EFST_HIDING:
    case EFST_CLOAKING:
    case EFST_CRUCIS:
    case EFST_RIDING:
    case EFST_FALCON:
    case EFST_ILLUSION:
    case EFST_PROTECTWEAPON:
    case EFST_PROTECTSHIELD:
    case EFST_PROTECTARMOR:
    case EFST_PROTECTHELM:
    case EFST_PROVIDENCE:
    case EFST_EXPLOSIONSPIRITS:
    case EFST_STEELBODY:
    case EFST_STOP:
    case EFST_PROPERTYUNDEAD:
    case EFST_POWERUP:
    case EFST_AURABLADE:
    case EFST_PARRYING:
    case EFST_LKCONCENTRATION:
    case EFST_BERSERK:
    case EFST_ASSUMPTIO:
    case EFST_GROUNDMAGIC:
    case EFST_MAGICPOWER:
    case EFST_EDP:
    case EFST_TRUESIGHT:
    case EFST_WINDWALK:
    case EFST_MELTDOWN:
    case EFST_CARTBOOST:
    case EFST_SWORDREJECT:
    case EFST_MARIONETTE_MASTER:
    case EFST_MARIONETTE:
    case EFST_MOON:
    case EFST_BLOODING:
    case EFST_JOINTBEAT:
    case EFST_PROTECTEXP:
    case EFST_AUTOBERSERK:
    case EFST_RUN:
    case EFST_TING:
    case EFST_STORMKICK_ON:
    case EFST_DOWNKICK_ON:
    case EFST_TURNKICK_ON:
    case EFST_COUNTER_ON:
    case EFST_DODGE_ON:
    case EFST_STRUP:
    case EFST_SOULLINK:
    case EFST_DEVIL1:
    case EFST_KAITE:
    case EFST_KAIZEL:
    case EFST_KAAHI:
    case EFST_KAUPE:
    case EFST_SMA_READY:
    case EFST_SKE:
    case EFST_SG_SUN_WARM:
    case EFST_SG_MOON_WARM:
    case EFST_SG_STAR_WARM:
    case EFST_SUN_COMFORT:
    case EFST_MOON_COMFORT:
    case EFST_STAR_COMFORT:
    case EFST_PRESERVE:
    case EFST_CLAIRVOYANCE:
    case EFST_DOUBLECASTING:
    case EFST_TAROTCARD:
    case EFST_CR_SHRINK:
    case EFST_WZ_SIGHTBLASTER:
    case EFST_DC_WINKCHARM:
    case EFST_RG_CCONFINE_M:
    case EFST_RG_CCONFINE_S:
    case EFST_GS_MADNESSCANCEL:
    case EFST_GS_GATLINGFEVER:
    case EFST_EARTHSCROLL:
    case EFST_NJ_UTSUSEMI:
    case EFST_NJ_BUNSINJYUTSU:
    case EFST_NJ_NEN:
    case EFST_GS_ADJUSTMENT:
    case EFST_GS_ACCURACY:
    case EFST_NJ_SUITON:
    case EFST_PET:
    case EFST_MENTAL:
    case EFST_EXPMEMORY:
    case EFST_PERFORMANCE:
    case EFST_GAIN:
    case EFST_GRIFFON:
    case EFST_DRIFT:
    case EFST_WALLSHIFT:
    case EFST_REINCARNATION:
    case EFST_PATTACK:
    case EFST_PSPEED:
    case EFST_PDEFENSE:
    case EFST_PCRITICAL:
    case EFST_RANKING:
    case EFST_PTRIPLE:
    case EFST_DENERGY:
    case EFST_WAVE1:
    case EFST_WAVE2:
    case EFST_WAVE3:
    case EFST_WAVE4:
    case EFST_DAURA:
    case EFST_DFREEZER:
    case EFST_DPUNISH:
    case EFST_DBARRIER:
    case EFST_DWARNING:
    case EFST_DGAUGE:
    case EFST_DACCEL:
    case EFST_DBLOCK:
    case EFST_CASH_PLUSEXP:
    case EFST_DA_ENERGY:
    case EFST_DA_FIRSTSLOT:
    case EFST_DA_HEADDEF:
    case EFST_DA_SPACE:
    case EFST_DA_TRANSFORM:
    case EFST_DA_ITEMREBUILD:
    case EFST_DA_DARKPOWER:
    case EFST_DA_EARPLUG:
    case EFST_DA_CONTRACT:
    case EFST_DA_BLACK:
    case EFST_DA_MAGICCART:
    case EFST_CRYSTAL:
    case EFST_DA_REBUILD:
    case EFST_DA_EDARKNESS:
    case EFST_DA_EGUARDIAN:
    case EFST_DA_TIMEOUT:
    case EFST_SLOWCAST:
    case EFST_CRITICALWOUND:
    case EFST_MOVHASTE_HORSE:
    case EFST_PROTECT_DEF:
    case EFST_PROTECT_MDEF:
    case EFST_HEALPLUS:
    case EFST_S_LIFEPOTION:
    case EFST_L_LIFEPOTION:
    case EFST_CRITICALPERCENT:
    case EFST_PLUSAVOIDVALUE:
    case EFST_TARGET_ASPD:
    case EFST_ATKER_BLOOD:
    case EFST_TARGET_BLOOD:
    case EFST_ARMOR_PROPERTY:
    case EFST_PARTYFLEE:
    case EFST_ANGEL_PROTECT:
    case EFST_ENCHANTBLADE:
    case EFST_REFRESH:
    case EFST_GIANTGROWTH:
    case EFST_STONEHARDSKIN:
    case EFST_VITALITYACTIVATION:
    case EFST_FIGHTINGSPIRIT:
    case EFST_ABUNDANCE:
    case EFST_VENOMIMPRESS:
    case EFST_ORATIO:
    case EFST_CLOAKINGEXCEED:
    case EFST_HALLUCINATIONWALK:
    case EFST_RENOVATIO:
    case EFST_WEAPONBLOCKING:
    case EFST_ROLLINGCUTTER:
    case EFST_POISONINGWEAPON:
    case EFST_TOXIN:
    case EFST_PARALYSE:
    case EFST_VENOMBLEED:
    case EFST_MAGICMUSHROOM:
    case EFST_DEATHHURT:
    case EFST_PYREXIA:
    case EFST_OBLIVIONCURSE:
    case EFST_LEECHESEND:
    case EFST_DUPLELIGHT:
    case EFST_FROSTMISTY:
    case EFST_FEARBREEZE:
    case EFST_ELECTRICSHOCKER:
    case EFST_MARSHOFABYSS:
    case EFST_RECOGNIZEDSPELL:
    case EFST_WUGRIDER:
    case EFST_CAMOUFLAGE:
    case EFST_ACCELERATION:
    case EFST_HOVERING:
    case EFST_SUMMON1:
    case EFST_SUMMON2:
    case EFST_SUMMON3:
    case EFST_SUMMON4:
    case EFST_SUMMON5:
    case EFST_OVERHEAT:
    case EFST_SHAPESHIFT:
    case EFST_INFRAREDSCAN:
    case EFST_NEUTRALBARRIER:
    case EFST_STEALTHFIELD:
    case EFST_MANU_ATK:
    case EFST_MANU_DEF:
    case EFST_SPL_ATK:
    case EFST_SPL_DEF:
    case EFST_REPRODUCE:
    case EFST_MANU_MATK:
    case EFST_SPL_MATK:
    case EFST_STR_SCROLL:
    case EFST_INT_SCROLL:
    case EFST_LG_REFLECTDAMAGE:
    case EFST_AUTOSHADOWSPELL:
    case EFST_SHADOWFORM:
    case EFST_SHIELDSPELL_DEF:
    case EFST_SHIELDSPELL_MDEF:
    case EFST_SHIELDSPELL_REF:
    case EFST_BODYPAINT:
    case EFST_ADORAMUS:
    case EFST_PRESTIGE:
    case EFST_INVISIBILITY:
    case EFST_DEADLYINFECT:
    case EFST_BANDING:
    case EFST_INSPIRATION:
    case EFST_ENERVATION:
    case EFST_GROOMY:
    case EFST_RAISINGDRAGON:
    case EFST_IGNORANCE:
    case EFST_LAZINESS:
    case EFST_LIGHTNINGWALK:
    case EFST_ACARAJE:
    case EFST_UNLUCKY:
    case EFST_CURSEDCIRCLE_ATKER:
    case EFST_CURSEDCIRCLE_TARGET:
    case EFST_WEAKNESS:
    case EFST_CRESCENTELBOW:
    case EFST_STRIPACCESSARY:
    case EFST_MANHOLE:
    case EFST_POPECOOKIE:
    case EFST_GENTLETOUCH_ENERGYGAIN:
    case EFST_GENTLETOUCH_CHANGE:
    case EFST_GENTLETOUCH_REVITALIZE:
    case EFST_SWING:
    case EFST_SYMPHONY_LOVE:
    case EFST_NETHERWORLD:
    case EFST_SIREN:
    case EFST_DEEP_SLEEP:
    case EFST_SIRCLEOFNATURE:
    case EFST_COLD:
    case EFST_GLOOMYDAY:
    case EFST_SONG_OF_MANA:
    case EFST_CLOUD_KILL:
    case EFST_DANCE_WITH_WUG:
    case EFST_RUSH_WINDMILL:
    case EFST_ECHOSONG:
    case EFST_HARMONIZE:
    case EFST_STRIKING:
    case EFST_WARMER:
    case EFST_MOONLIT_SERENADE:
    case EFST_SATURDAY_NIGHT_FEVER:
    case EFST_ANALYZE:
    case EFST_LERADS_DEW:
    case EFST_MELODYOFSINK:
    case EFST_BEYOND_OF_WARCRY:
    case EFST_UNLIMITED_HUMMING_VOICE:
    case EFST_GN_CARTBOOST:
    case EFST_THORNS_TRAP:
    case EFST_BLOOD_SUCKER:
    case EFST_AB_SECRAMENT:
    case EFST_ASSUMPTIO2:
    case EFST_STOMACHACHE:
    case EFST_SAVAGE_STEAK:
    case EFST_COCKTAIL_WARG_BLOOD:
    case EFST_MINOR_BBQ:
    case EFST_SIROMA_ICE_TEA:
    case EFST_DROCERA_HERB_STEAMED:
    case EFST_PUTTI_TAILS_NOODLES:
    case EFST_EQUIPED_FLOOR:
    case EFST_PAIN_KILLER:
    case EFST_LIGHT_OF_REGENE:
    case EFST_OVERED_BOOST:
    case EFST_STYLE_CHANGE:
    case EFST_MAGMA_FLOW:
    case EFST_GRANITIC_ARMOR:
    case EFST_PYROCLASTIC:
    case EFST_VOLCANIC_ASH:
    case EFST_MAGIC_CANDY:
    case EFST_EQUIPPED_DIVINE_ARMOR:
    case EFST_EQUIPPED_HOLY_ARMOR:
    case EFST_M_LIFEPOTION:
    case EFST_NPC_HALLUCINATIONWALK:
    case EFST_SOULSHADOW:
    case EFST_SOULFAIRY:
    case EFST_SOULFALCON:
    case EFST_SOULGOLEM:
    case EFST_ASSUMPTIO_BUFF:
      std::sprintf(message, "");
      break;
    case EFST_DA_ILLUSION:
      if (packet->aid == g_session.m_aid) {
        g_session.guildLeader = 1;
      }
      std::sprintf(message, "");
      break;
    case EFST_FOOD_STR:
    case EFST_FOOD_STR_CASH:
      std::sprintf(message, LookupMessage(MSI_ENST_STR));
      break;
    case EFST_FOOD_AGI:
    case EFST_FOOD_AGI_CASH:
      std::sprintf(message, LookupMessage(MSI_ENST_AGI));
      break;
    case EFST_FOOD_VIT:
    case EFST_FOOD_VIT_CASH:
      std::sprintf(message, LookupMessage(MSI_ENST_VIT));
      break;
    case EFST_FOOD_DEX:
    case EFST_FOOD_DEX_CASH:
      std::sprintf(message, LookupMessage(MSI_ENST_DEX));
      break;
    case EFST_FOOD_INT:
    case EFST_FOOD_INT_CASH:
      std::sprintf(message, LookupMessage(MSI_ENST_INT));
      break;
    case EFST_FOOD_LUK:
    case EFST_FOOD_LUK_CASH:
      std::sprintf(message, LookupMessage(MSI_ENST_LUK));
      break;
    case EFST_FOOD_BASICAVOIDANCE:
      std::sprintf(message, LookupMessage(MSI_ENST_AVOIDANCE));
      break;
    case EFST_FOOD_BASICHIT:
      std::sprintf(message, LookupMessage(MSI_ENST_HIT));
      break;
    case EFST_FOOD_CRITICALSUCCESSVALUE:
      std::sprintf(message, LookupMessage(MSI_ENST_CRITICALSUCCESSVALUE));
      break;
    case EFST_CASH_DEATHPENALTY:
      std::sprintf(message, LookupMessage(MSI_DDEATHPENALTY));
      break;
    case EFST_CASH_RECEIVEITEM:
      std::sprintf(message, LookupMessage(MSI_RECEIVEITEM));
      break;
    case EFST_CASH_BOSS_ALARM:
      if (packet->aid == g_session.m_aid) {
        this->bossAlarmAndCopyNameFlags[0] = 1;
      }
      std::sprintf(message, LookupMessage(MSI_BOSS_ALARM));
      break;
    case EFST_CASH_PLUSONLYJOBEXP:
      std::sprintf(message, LookupMessage(MSI_PLUSONLYJOBEXP));
      break;
    case EFST_SUHIDE:
      if (packet->aid == g_session.m_aid) {
        this->scene->player->SetState(48, 1);
        this->scene->player->render.isMotionFreezed = true;
      }
      break;
    case EFST_SU_STOOP:
      if (packet->aid == g_session.m_aid) {
        this->scene->GetGameActorByAID(packet->aid);
        this->scene->player->SetState(47, 1);
      }
      break;
    case EFST_ALL_STAT_DOWN:
      color = 0xFF;
      std::sprintf(message, LookupMessage(MSI_ACCOUNTCLASS_INTEGRATION));
      break;
    default:
      this->RefreshStatusStateEffect(packet->aid);
      return;
    }
  } else {
    if (packet->aid == g_session.m_aid) {
      for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
        if (it->id == packet->type) {
          g_activeSkillEffects.erase(it);
          it = g_activeSkillEffects.begin();
        } else {
          ++it;
        }
      }
    }

    color = 0xFF;
    switch (packet->type) {
    case EFST_TWOHANDQUICKEN:
    case EFST_ATTHASTE_POTION1:
    case EFST_ATTHASTE_POTION2:
    case EFST_ATTHASTE_POTION3:
    case EFST_SPEARQUICKEN:
    case EFST_ONEHANDQUICKEN:
      std::sprintf(message, LookupMessage(MSI_DECATTACKSPEED));
      break;
    case EFST_IMPOSITIO:
      std::sprintf(message, LookupMessage(MSI_CHECK_ATTENDANCE_DATE_COUNT));
      break;
    case EFST_SUFFRAGIUM:
      std::sprintf(message, LookupMessage(MSI_NORMALPREDELAY));
      break;
    case EFST_WEAPONPROPERTY:
      if (packet->aid == g_session.m_aid) {
        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id - 897U <= 9) {
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }
      }
      std::sprintf(message, LookupMessage(MSI_WEAPONPROPERTYORIGINAL));
      break;
    case EFST_ENCHANTPOISON:
    case EFST_ASPERSIO:
      std::sprintf(message, LookupMessage(MSI_WEAPONPROPERTYORIGINAL));
      break;
    case EFST_BENEDICTIO:
      std::sprintf(message, LookupMessage(MSI_ARMORPROPERTYORIGINAL));
      break;
    case EFST_KYRIE:
      std::sprintf(message, LookupMessage(MSI_DISAPPEARBARRIOR));
      break;
    case EFST_WEAPONPERFECT:
      std::sprintf(message, LookupMessage(MSI_DISAPPEARPERFECTDAMAGE));
      break;
    case EFST_OVERTHRUST:
    case EFST_OVERTHRUSTMAX:
      std::sprintf(message, LookupMessage(MSI_DECWEAPONATTACK));
      break;
    case EFST_MAXIMIZE:
      std::sprintf(message, LookupMessage(MSI_DISAPPEARMAXIMIZE));
      break;
    case EFST_DEC_AGI:
      std::sprintf(message, LookupMessage(MSI_SPEEDUP));
      break;
    case EFST_POISONREACT:
      std::sprintf(message, LookupMessage(MSI_DSST_POISONREACT));
      break;
    case EFST_QUAGMIRE:
      std::sprintf(message, LookupMessage(MSI_DSST_QUAGMIRE));
      break;
    case EFST_ANGELUS:
      std::sprintf(message, LookupMessage(MSI_DSST_ANGELUS));
      break;
    case EFST_BLESSING:
      std::sprintf(message, LookupMessage(MSI_DSST_BLESSING));
      break;
    case EFST_SLOWPOISON:
      std::sprintf(message, LookupMessage(MSI_DSST_SLOWPOISON));
      break;
    case EFST_MAGNIFICAT:
      std::sprintf(message, LookupMessage(MSI_DSST_MAGNIFICAT));
      break;
    case EFST_GLORIA:
      std::sprintf(message, LookupMessage(MSI_DSST_GLORIA));
      break;
    case EFST_LEXAETERNA:
      std::sprintf(message, LookupMessage(MSI_DSST_LEXAETERNA));
      break;
    case EFST_ADRENALINE:
    case EFST_ADRENALINE2:
      std::sprintf(message, LookupMessage(MSI_DSST_ADRENALINE));
      break;
    case EFST_TRICKDEAD:
      std::sprintf(message, "");
      if (packet->aid == g_session.m_aid) {
        this->scene->player->SetState(0, 1);
        this->scene->player->render.isMotionFreezed = false;
      }
      break;
    case EFST_WHISTLE:
    case EFST_ASSASSINCROSS:
    case EFST_POEMBRAGI:
    case EFST_APPLEIDUN:
    case EFST_HUMMING:
    case EFST_DONTFORGETME:
    case EFST_FORTUNEKISS:
    case EFST_SERVICEFORYOU:
    case EFST_RICHMANKIM:
    case EFST_ETERNALCHAOS:
    case EFST_DRUMBATTLEFIELD:
    case EFST_RINGNIBELUNGEN:
    case EFST_ROKISWEIL:
    case EFST_INTOABYSS:
    case EFST_SIEGFRIED:
    case EFST_MONSTER_TRANSFORM:
    case EFST_KO_JYUMONJIKIRI:
    case EFST_MEIKYOUSISUI:
    case EFST_KYOUGAKU:
    case EFST_KYOMU:
    case EFST_KAGEMUSYA:
    case EFST_ZANGETSU:
    case EFST_GENSOU:
    case EFST_AKAITSUKI:
    case EFST_ACTIVE_MONSTER_TRANSFORM:
    case EFST_HAT_EFFECT:
    case EFST_OFFERTORIUM:
    case EFST_TELEKINESIS_INTENSE:
    case EFST_MOONSTAR:
    case EFST_STRANGELIGHTS:
    case EFST_FULL_THROTTLE:
    case EFST_REBOUND:
    case EFST_SUPER_STAR:
    case EFST_ILLUSIONDOPING:
    case EFST_WIDEWEB:
    case EFST_CHILL:
    case EFST_BURNT:
    case EFST_ANTI_M_BLAST:
    case EFST_DECORATION_OF_MUSIC:
    case EFST_FLOWERSMOKE:
    case EFST_FSTONE:
    case EFST_QSCARABA:
    case EFST_LJOSALFAR:
    case EFST_HAPPINESS_STAR:
    case EFST_DRESS_UP:
    case EFST_MAPLE_FALLS:
    case EFST_MARKING_USE_CHANGEMONSTER:
    case EFST_MERMAID_LONGING:
    case EFST_MAGICAL_FEATHER:
    case EFST_TIME_ACCESSORY:
    case EFST_HEAD_EQUIPMENT_EFFECT:
    case EFST_SV_ROOTTWIST:
    case EFST_BITESCAR:
    case EFST_TUNAPARTY:
    case EFST_SPRITEMABLE:
    case EFST_SOULATTACK:
    case EFST_IMMUNE_PROPERTY_NOTHING:
    case EFST_IMMUNE_PROPERTY_WATER:
    case EFST_IMMUNE_PROPERTY_GROUND:
    case EFST_IMMUNE_PROPERTY_FIRE:
    case EFST_IMMUNE_PROPERTY_WIND:
    case EFST_IMMUNE_PROPERTY_POISON:
    case EFST_IMMUNE_PROPERTY_SAINT:
    case EFST_IMMUNE_PROPERTY_DARKNESS:
    case EFST_IMMUNE_PROPERTY_TELEKINESIS:
    case EFST_IMMUNE_PROPERTY_UNDEAD:
    case EFST_LUNARSTANCE:
    case EFST_UNIVERSESTANCE:
    case EFST_SUNSTANCE:
    case EFST_FLASHKICK:
    case EFST_STARSTANCE:
    case EFST_GRAVITYCONTROL:
    case EFST_SOULUNITY:
    case EFST_EARTHSHAKER:
    case EFST_MISTY_FROST:
    case EFST_MAGIC_POISON:
    case EFST_PRISON:
    case EFST_DEADLY_DEFEASANCE:
    case EFST_CLIMAX_CRYIMP:
    case EFST_HOLY_OIL:
    case EFST_E_SLASH_COUNT:
    case EFST_MEDIALE:
    case EFST_HANDICAPSTATE_DEEPBLIND:
    case EFST_HANDICAPSTATE_DEEPSILENCE:
    case EFST_HANDICAPSTATE_LASSITUDE:
    case EFST_HANDICAPSTATE_FROSTBITE:
    case EFST_HANDICAPSTATE_SWOONING:
    case EFST_HANDICAPSTATE_LIGHTNINGSTRIKE:
    case EFST_HANDICAPSTATE_CRYSTALLIZATION:
    case EFST_HANDICAPSTATE_CONFLAGRATION:
    case EFST_HANDICAPSTATE_MISFORTUNE:
    case EFST_HANDICAPSTATE_DEADLYPOISON:
    case EFST_HANDICAPSTATE_DEPRESSION:
    case EFST_HANDICAPSTATE_HOLYFLAME:
    case EFST_FIRST_BRAND:
    case EFST_SECOND_BRAND:
    case EFST_SECOND_JUDGE:
    case EFST_THIRD_EXOR_FLAME:
    case EFST_FIRST_FAITH_POWER:
    case EFST_WINDSIGN:
    case EFST_SOUNDBLEND:
    case EFST_CLOUD_POISON:
    case EFST_SPORE_EXPLOSION_DEBUFF:
    case EFST_CURSE_R_CUBE:
    case EFST_CURSE_B_CUBE:
    case EFST_KILLING_AURA:
    case EFST_BLOCK:
      break;
    case EFST_SHOUT:
      std::sprintf(message, LookupMessage(MSI_DSST_SHOUT));
      break;
    case EFST_ENERGYCOAT:
      std::sprintf(message, LookupMessage(MSI_DSST_ENERGYCOAT));
      break;
    case EFST_ATTHASTE_INFINITY:
      if (packet->aid == g_session.m_aid) {
        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == packet->type) {
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }
      }
      std::sprintf(message, LookupMessage(MSI_SPEEDDOWN));
      break;
    case EFST_MOVHASTE_POTION:
      std::sprintf(message, LookupMessage(MSI_SPEEDDOWN));
      break;
    case EFST_INC_AGI:
    case EFST_MOVHASTE_INFINITY:
      if (packet->aid == g_session.m_aid) {
        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == packet->type) {
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }
      }
      std::sprintf(message, LookupMessage(MSI_SPEEDDOWN));
      break;
    case EFST_ROLLINGCUTTER:
      if (packet->aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        for (int i = 0; i < 10; ++i) {
          g_effectMgr->Retire(player->render.accountId, 1300 + i);
          g_effectMgr->Retire(player->render.accountId, 1467 + i);
        }
      }
      std::sprintf(message, "");
      break;
    case EFST_PROVOKE:
    case EFST_ENDURE:
    case EFST_CONCENTRATION:
    case EFST_HIDING:
    case EFST_CLOAKING:
    case EFST_CRUCIS:
    case EFST_RIDING:
    case EFST_FALCON:
    case EFST_BROKENARMOR:
    case EFST_BROKENWEAPON:
    case EFST_ILLUSION:
    case EFST_PROTECTWEAPON:
    case EFST_PROTECTSHIELD:
    case EFST_PROTECTARMOR:
    case EFST_PROTECTHELM:
    case EFST_EXPLOSIONSPIRITS:
    case EFST_STEELBODY:
    case EFST_STOP:
    case EFST_PROPERTYUNDEAD:
    case EFST_POWERUP:
    case EFST_AURABLADE:
    case EFST_PARRYING:
    case EFST_LKCONCENTRATION:
    case EFST_BERSERK:
    case EFST_ASSUMPTIO:
    case EFST_GROUNDMAGIC:
    case EFST_MAGICPOWER:
    case EFST_EDP:
    case EFST_TRUESIGHT:
    case EFST_WINDWALK:
    case EFST_MELTDOWN:
    case EFST_CARTBOOST:
    case EFST_SWORDREJECT:
    case EFST_MARIONETTE_MASTER:
    case EFST_MARIONETTE:
    case EFST_MOON:
    case EFST_BLOODING:
    case EFST_JOINTBEAT:
    case EFST_PROTECTEXP:
    case EFST_AUTOBERSERK:
    case EFST_RUN:
    case EFST_TING:
    case EFST_STORMKICK_ON:
    case EFST_DOWNKICK_ON:
    case EFST_TURNKICK_ON:
    case EFST_COUNTER_ON:
    case EFST_DODGE_ON:
    case EFST_STRUP:
    case EFST_SOULLINK:
    case EFST_PLUSATTACKPOWER:
    case EFST_PLUSMAGICPOWER:
    case EFST_DEVIL1:
    case EFST_KAITE:
    case EFST_KAIZEL:
    case EFST_KAAHI:
    case EFST_KAUPE:
    case EFST_SMA_READY:
    case EFST_SKE:
    case EFST_SG_SUN_WARM:
    case EFST_SG_MOON_WARM:
    case EFST_SG_STAR_WARM:
    case EFST_SUN_COMFORT:
    case EFST_MOON_COMFORT:
    case EFST_STAR_COMFORT:
    case EFST_PRESERVE:
    case EFST_CHASEWALK2:
    case EFST_CLAIRVOYANCE:
    case EFST_DOUBLECASTING:
    case EFST_TAROTCARD:
    case EFST_CR_SHRINK:
    case EFST_WZ_SIGHTBLASTER:
    case EFST_DC_WINKCHARM:
    case EFST_RG_CCONFINE_M:
    case EFST_RG_CCONFINE_S:
    case EFST_GS_MADNESSCANCEL:
    case EFST_GS_GATLINGFEVER:
    case EFST_EARTHSCROLL:
    case EFST_NJ_UTSUSEMI:
    case EFST_NJ_BUNSINJYUTSU:
    case EFST_NJ_NEN:
    case EFST_GS_ADJUSTMENT:
    case EFST_GS_ACCURACY:
    case EFST_NJ_SUITON:
    case EFST_PET:
    case EFST_MENTAL:
    case EFST_EXPMEMORY:
    case EFST_PERFORMANCE:
    case EFST_GAIN:
    case EFST_GRIFFON:
    case EFST_DRIFT:
    case EFST_WALLSHIFT:
    case EFST_REINCARNATION:
    case EFST_PATTACK:
    case EFST_PSPEED:
    case EFST_PDEFENSE:
    case EFST_PCRITICAL:
    case EFST_RANKING:
    case EFST_PTRIPLE:
    case EFST_DENERGY:
    case EFST_WAVE1:
    case EFST_WAVE2:
    case EFST_WAVE3:
    case EFST_WAVE4:
    case EFST_DAURA:
    case EFST_DFREEZER:
    case EFST_DPUNISH:
    case EFST_DBARRIER:
    case EFST_DWARNING:
    case EFST_DGAUGE:
    case EFST_DACCEL:
    case EFST_DBLOCK:
    case EFST_CASH_PLUSEXP:
    case EFST_CASH_DEATHPENALTY:
    case EFST_CASH_RECEIVEITEM:
    case EFST_DA_ENERGY:
    case EFST_DA_FIRSTSLOT:
    case EFST_DA_HEADDEF:
    case EFST_DA_SPACE:
    case EFST_DA_TRANSFORM:
    case EFST_DA_ITEMREBUILD:
    case EFST_DA_DARKPOWER:
    case EFST_DA_EARPLUG:
    case EFST_DA_CONTRACT:
    case EFST_DA_BLACK:
    case EFST_DA_MAGICCART:
    case EFST_CRYSTAL:
    case EFST_DA_REBUILD:
    case EFST_DA_EDARKNESS:
    case EFST_DA_EGUARDIAN:
    case EFST_DA_TIMEOUT:
    case EFST_SLOWCAST:
    case EFST_CRITICALWOUND:
    case EFST_MOVHASTE_HORSE:
    case EFST_PROTECT_DEF:
    case EFST_PROTECT_MDEF:
    case EFST_HEALPLUS:
    case EFST_S_LIFEPOTION:
    case EFST_L_LIFEPOTION:
    case EFST_CRITICALPERCENT:
    case EFST_PLUSAVOIDVALUE:
    case EFST_ATKER_ASPD:
    case EFST_TARGET_ASPD:
    case EFST_ATKER_MOVESPEED:
    case EFST_ATKER_BLOOD:
    case EFST_TARGET_BLOOD:
    case EFST_ARMOR_PROPERTY:
    case EFST_CASH_PLUSONLYJOBEXP:
    case EFST_PARTYFLEE:
    case EFST_ANGEL_PROTECT:
    case EFST_ENCHANTBLADE:
    case EFST_REFRESH:
    case EFST_GIANTGROWTH:
    case EFST_STONEHARDSKIN:
    case EFST_VITALITYACTIVATION:
    case EFST_FIGHTINGSPIRIT:
    case EFST_ABUNDANCE:
    case EFST_VENOMIMPRESS:
    case EFST_ORATIO:
    case EFST_CLOAKINGEXCEED:
    case EFST_HALLUCINATIONWALK:
    case EFST_RENOVATIO:
    case EFST_WEAPONBLOCKING:
    case EFST_POISONINGWEAPON:
    case EFST_TOXIN:
    case EFST_PARALYSE:
    case EFST_VENOMBLEED:
    case EFST_MAGICMUSHROOM:
    case EFST_DEATHHURT:
    case EFST_PYREXIA:
    case EFST_OBLIVIONCURSE:
    case EFST_LEECHESEND:
    case EFST_DUPLELIGHT:
    case EFST_FROSTMISTY:
    case EFST_FEARBREEZE:
    case EFST_ELECTRICSHOCKER:
    case EFST_MARSHOFABYSS:
    case EFST_RECOGNIZEDSPELL:
    case EFST_WUGRIDER:
    case EFST_CAMOUFLAGE:
    case EFST_ACCELERATION:
    case EFST_HOVERING:
    case EFST_SUMMON1:
    case EFST_SUMMON2:
    case EFST_SUMMON3:
    case EFST_SUMMON4:
    case EFST_SUMMON5:
    case EFST_OVERHEAT:
    case EFST_SHAPESHIFT:
    case EFST_INFRAREDSCAN:
    case EFST_NEUTRALBARRIER:
    case EFST_STEALTHFIELD:
    case EFST_MANU_ATK:
    case EFST_MANU_DEF:
    case EFST_SPL_ATK:
    case EFST_SPL_DEF:
    case EFST_REPRODUCE:
    case EFST_MANU_MATK:
    case EFST_SPL_MATK:
    case EFST_STR_SCROLL:
    case EFST_INT_SCROLL:
    case EFST_LG_REFLECTDAMAGE:
    case EFST_AUTOSHADOWSPELL:
    case EFST_SHADOWFORM:
    case EFST_SHIELDSPELL_DEF:
    case EFST_SHIELDSPELL_MDEF:
    case EFST_SHIELDSPELL_REF:
    case EFST_BODYPAINT:
    case EFST_ADORAMUS:
    case EFST_PRESTIGE:
    case EFST_INVISIBILITY:
    case EFST_DEADLYINFECT:
    case EFST_BANDING:
    case EFST_INSPIRATION:
    case EFST_ENERVATION:
    case EFST_GROOMY:
    case EFST_RAISINGDRAGON:
    case EFST_IGNORANCE:
    case EFST_LAZINESS:
    case EFST_LIGHTNINGWALK:
    case EFST_ACARAJE:
    case EFST_UNLUCKY:
    case EFST_CURSEDCIRCLE_ATKER:
    case EFST_CURSEDCIRCLE_TARGET:
    case EFST_WEAKNESS:
    case EFST_CRESCENTELBOW:
    case EFST_STRIPACCESSARY:
    case EFST_MANHOLE:
    case EFST_POPECOOKIE:
    case EFST_GENTLETOUCH_ENERGYGAIN:
    case EFST_GENTLETOUCH_CHANGE:
    case EFST_GENTLETOUCH_REVITALIZE:
    case EFST_SWING:
    case EFST_SYMPHONY_LOVE:
    case EFST_SIRCLEOFNATURE:
    case EFST_COLD:
    case EFST_GLOOMYDAY:
    case EFST_SONG_OF_MANA:
    case EFST_CLOUD_KILL:
    case EFST_DANCE_WITH_WUG:
    case EFST_RUSH_WINDMILL:
    case EFST_ECHOSONG:
    case EFST_HARMONIZE:
    case EFST_STRIKING:
    case EFST_WARMER:
    case EFST_MOONLIT_SERENADE:
    case EFST_SATURDAY_NIGHT_FEVER:
    case EFST_ANALYZE:
    case EFST_LERADS_DEW:
    case EFST_MELODYOFSINK:
    case EFST_BEYOND_OF_WARCRY:
    case EFST_UNLIMITED_HUMMING_VOICE:
    case EFST_GN_CARTBOOST:
    case EFST_THORNS_TRAP:
    case EFST_BLOOD_SUCKER:
    case EFST_AB_SECRAMENT:
    case EFST_ASSUMPTIO2:
    case EFST_STOMACHACHE:
    case EFST_SAVAGE_STEAK:
    case EFST_COCKTAIL_WARG_BLOOD:
    case EFST_MINOR_BBQ:
    case EFST_SIROMA_ICE_TEA:
    case EFST_DROCERA_HERB_STEAMED:
    case EFST_PUTTI_TAILS_NOODLES:
    case EFST_EQUIPED_FLOOR:
    case EFST_PAIN_KILLER:
    case EFST_LIGHT_OF_REGENE:
    case EFST_OVERED_BOOST:
    case EFST_STYLE_CHANGE:
    case EFST_MAGMA_FLOW:
    case EFST_GRANITIC_ARMOR:
    case EFST_PYROCLASTIC:
    case EFST_VOLCANIC_ASH:
    case EFST_MAGIC_CANDY:
    case EFST_ALL_RIDING:
    case EFST_OVERLAPEXPUP:
    case EFST_REWARD_PLUSONLYJOBEXP:
    case EFST_EQUIPPED_DIVINE_ARMOR:
    case EFST_EQUIPPED_HOLY_ARMOR:
    case EFST_M_LIFEPOTION:
    case EFST_NPC_HALLUCINATIONWALK:
    case EFST_SOULSHADOW:
    case EFST_SOULFAIRY:
    case EFST_SOULFALCON:
    case EFST_SOULGOLEM:
    case EFST_ASSUMPTIO_BUFF:
      std::sprintf(message, "");
      break;
    case EFST_SIREN:
      if (packet->aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 879, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case EFST_DEEP_SLEEP:
      if (packet->aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_DELETE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case EFST_NETHERWORLD:
      if (packet->aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 453, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case EFST_WUGDASH:
      if (packet->aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        while (player->HasId(752)) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 752, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
      }
      std::sprintf(message, "");
      break;
    case EFST_DA_ILLUSION:
      if (packet->aid == g_session.m_aid) {
        g_session.guildLeader = 0;
      }
      std::sprintf(message, "");
      break;
    case EFST_FOOD_STR:
    case EFST_FOOD_STR_CASH:
      std::sprintf(message, LookupMessage(MSI_DSST_STR));
      break;
    case EFST_FOOD_AGI:
    case EFST_FOOD_AGI_CASH:
      std::sprintf(message, LookupMessage(MSI_DSST_AGI));
      break;
    case EFST_FOOD_VIT:
    case EFST_FOOD_VIT_CASH:
      std::sprintf(message, LookupMessage(MSI_DSST_VIT));
      break;
    case EFST_FOOD_DEX:
    case EFST_FOOD_DEX_CASH:
      std::sprintf(message, LookupMessage(MSI_DSST_DEX));
      break;
    case EFST_FOOD_INT:
    case EFST_FOOD_INT_CASH:
      std::sprintf(message, LookupMessage(MSI_DSST_INT));
      break;
    case EFST_FOOD_LUK:
    case EFST_FOOD_LUK_CASH:
      std::sprintf(message, LookupMessage(MSI_DSST_LUK));
      break;
    case EFST_FOOD_BASICAVOIDANCE:
      std::sprintf(message, LookupMessage(MSI_DSST_AVOIDANCE));
      break;
    case EFST_FOOD_BASICHIT:
      std::sprintf(message, LookupMessage(MSI_DSST_HIT));
      break;
    case EFST_FOOD_CRITICALSUCCESSVALUE:
      std::sprintf(message, LookupMessage(MSI_DSST_CRITICALSUCCESSVALUE));
      break;
    case EFST_CASH_BOSS_ALARM:
      if (packet->aid == g_session.m_aid) {
        this->bossAlarmAndCopyNameFlags[0] = 0;
      }
      break;
    case EFST_ON_PUSH_CART:
      if (packet->aid == g_session.m_aid) {
        g_session.SetReplayPacketMode(0);
      }
      break;
    case EFST_HANDICAPSTATE_ICEEXPLO:
      if (packet->aid == g_session.m_aid && this->scene->player) {
        this->scene->player->SendMsg(nullptr, AM_RETURN_COLOR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case EFST_DARKCROW:
      if (packet->aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        while (player->HasId(1040)) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
      }
      break;
    case EFST_C_MARKER:
      if (packet->aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        while (player->HasId(1061)) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1061, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
      }
      break;
    case EFST_SUHIDE:
      if (packet->aid == g_session.m_aid) {
        this->scene->player->SetState(0, 1);
        this->scene->player->render.isMotionFreezed = false;
      }
      break;
    case EFST_SU_STOOP:
      if (packet->aid == g_session.m_aid && !this->scene->player->render.specialMotionLocked) {
        this->scene->player->SetState(0, 1);
      }
      break;
    case EFST_ALL_STAT_DOWN:
      color = 0xFFFF00;
      std::sprintf(message, LookupMessage(MSI_ACCOUNTCLASS_RAGNAROK));
      break;
    case EFST_SOULCURSE:
      if (CEffect* effect = g_effectMgr->Find(packet->aid, 1362)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_MADOGEAR_TYPE:
    {
      CGameActor* actor;
      if (packet->aid == g_session.m_aid) {
        actor = this->scene->player;
      } else {
        actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
      }
      if (!actor) {
        break;
      }
      actor->SetMadogearType(0);
      if (packet->aid == g_session.m_aid) {
        g_session.madogearType = 0;
      }
      actor->SendMsg(nullptr, AM_DELETE_CHICKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case EFST_RELIEVE_DAMAGE:
      if (CEffect* effect = g_effectMgr->Find(packet->aid, 1599)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_LOCKON_LASER:
    {
      CRenderObject* actor;
      if (packet->aid == g_session.m_aid) {
        actor = this->scene->player;
      } else {
        actor = this->scene->GetGameActorByAID(packet->aid);
      }
      if (actor) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1601, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case EFST_HELLS_PLANT_ARMOR:
    {
      CRenderObject* actor;
      if (packet->aid == g_session.m_aid) {
        actor = this->scene->player;
      } else {
        actor = this->scene->GetGameActorByAID(packet->aid);
      }
      if (actor) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1610, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case EFST_DRAGONIC_AURA:
      if (CEffect* effect = g_effectMgr->Find(packet->aid, 1726)) {
        g_effectMgr->Retire(effect, 1);
      }
      if (CEffect* effect = g_effectMgr->Find(packet->aid, 1727)) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    case EFST_ABYSS_SLAYER:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_UNBLOCK_CHAT));
      break;
    case EFST_ULTIMATE_S:
      _snprintf_s(message, 256, LookupMessage(MSI_SHC_SHADOW_EXCEED_ON));
      break;
    case EFST_POWERFUL_FAITH:
      _snprintf_s(message, 256, LookupMessage(MSI_IQ_SINCERE_FAITH_ON));
      break;
    case EFST_FIRM_FAITH:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_BENEDICTUM_ON));
      break;
    case EFST_SINCERE_FAITH:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_RELIGIO_ON));
      break;
    case EFST_BENEDICTUM:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_COMPETENTIA_ON));
      break;
    case EFST_RELIGIO:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_PRESENS_ACIES_ON));
      break;
    case EFST_COMPETENTIA:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_ARGUTUS_TELUM_ON));
      break;
    case EFST_PRE_ACIES:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_ARGUTUS_VITA_ON));
      break;
    case EFST_A_TELUM:
      _snprintf_s(message, 256, LookupMessage(MSI_ITEMNUM_AND_WEIGHT));
      break;
    case EFST_A_VITA:
      _snprintf_s(message, 256, LookupMessage(MSI_DK_CHARGINGPIERCE_OFF));
      break;
    case EFST_CLIMAX:
      if (CEffect* effect = g_effectMgr->Find(packet->aid, 1800)) {
        g_effectMgr->Retire(effect, 1);
      }
      _snprintf_s(message, 256, LookupMessage(MSI_SHC_POTENT_VENOM_ON));
      break;
    case EFST_POTENT_VENOM:
      _snprintf_s(message, 256, LookupMessage(MSI_HUNTING_QUEST_INFO_ON));
      break;
    case EFST_PROTECTSHADOWEQUIP:
      _snprintf_s(message, 256, LookupMessage(MSI_MT_A_MACHINE_ON));
      break;
    case EFST_RESEARCHREPORT:
      _snprintf_s(message, 256, LookupMessage(MSI_BO_ADVANCE_PROTECTION_ON));
      break;
    case EFST_BO_HELL_DUSTY:
      _snprintf_s(message, 256, LookupMessage(MSI_MT_D_MACHINE_ON));
      break;
    case EFST_D_MACHINE:
      if (CEffect* effect = g_effectMgr->Find(packet->aid, 1817)) {
        g_effectMgr->Retire(effect, 1);
        g_effectMgr->Spawn(packet->aid, 1821, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_TR_KVASIR_SONATA_OFF));
      break;
    case EFST_A_MACHINE:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_CASHSHOP_TAB_SEARCH));
      break;
    case EFST_MUSICAL_INTERLUDE:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_TR_PRON_MARCH_ON));
      break;
    case EFST_SPELL_ENCHANTING:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_ABC_ABYSS_SLAYER_OFF));
      break;
    case EFST_ABYSSFORCEWEAPON:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_CHARACTER_MAKING_RESTRICT));
      break;
    default:
      if (CSkillActionMgr::GetInstance()->HasEfstAction(packet->type) == true) {
        const EfstActionMessage action = CSkillActionMgr::GetInstance()->GetEfstEndMessage(packet->type);
        if (action.text) {
          color = action.color;
          StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", action.text);
        }
        break;
      }
      this->RefreshStatusStateEffect(packet->aid);
      return;
    }
  }

  this->RefreshStatusStateEffect(packet->aid);
  if (std::strlen(message) > 1) {
    char text[512];
    std::sprintf(text, "%s%s", name, message);
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), color, std::strlen(name) > 1 ? 10 : 8, 0, 0);
  }

  if (packet->aid == g_session.m_aid) {
    const std::int64_t state = packet->state;
    const std::int64_t type = packet->type;
    this->scene->player->SendMsg(nullptr, AM_STATUS_CHANGE, 0, static_cast<int>(type), static_cast<int>(type >> 32), static_cast<int>(state), static_cast<int>(state >> 32), 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

#pragma pack(push, 1)
struct GuildMemberRemovedPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char name[24];
  /* 001A */ char reason[40];
};
#pragma pack(pop)

// OFFSET: 0x00D48820
// FUNCTION: fn_21855
void CGameMode::ReceiveGuildMemberExpelled(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildMemberRemovedPacket*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  char reason[41];
  std::memcpy(reason, packet->reason, sizeof(packet->reason));
  reason[40] = '\0';
  char name[25];
  std::memcpy(name, packet->name, sizeof(packet->name));
  name[24] = '\0';

  if (std::string(name) == g_session.DecodePlayerName()) {
    g_guildInfo.m_guildId = 0;
    g_windowMgr.CloseWindow(WID_GUILDWND);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BAN_FROM_GUILD)), 0xFF, 4, 0, 0);
    this->SendMsg(MM_DELETEGUILDPOSALL, 0, 0, 0, 0);
    return;
  }

  if (guildInfo->membersRequested) {
    guildInfo->DeleteMember(name);
  }

  char message[1024];
  std::memset(message, 0, sizeof(message));
  std::sprintf(message, LookupMessage(MSI_BAN_GUILD), name);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);
  std::memset(message, 0, sizeof(message));
  sprintf_s(message, sizeof(message), LookupMessage(MSI_REASON_BAN_GUILD), reason);
  if (g_forbiddenWords.Contains(message) == true) {
    std::string masked;
    g_forbiddenWords.Mask(message, &masked);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);

  GuildInfo::Unknown00ECEntry entry;
  entry.name = name;
  entry.reason = reason;
  guildInfo->AddBannedMember(entry);
}

#pragma pack(push, 1)
struct GuildPositionUpdateEntry {
  /* 0000 */ int positionId;
  /* 0004 */ unsigned int mode;
  /* 0008 */ int ranking;
  /* 000C */ int payRate;
  /* 0010 */ char name[24];
};

struct GuildPositionUpdatePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t length;
  /* 0004 */ GuildPositionUpdateEntry entries[1];
};
#pragma pack(pop)

// OFFSET: 0x00D49020
// FUNCTION: fn_21857
void CGameMode::ReceiveGuildPositionUpdate(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildPositionUpdatePacket*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  if (!guildInfo->positionsRequested) {
    return;
  }

  const int count = (packet->length - 4) / sizeof(GuildPositionUpdateEntry);
  for (int i = 0; i < count; ++i) {
    GuildPositionUpdateEntry position = packet->entries[i];
    char name[25];
    std::memcpy(name, position.name, sizeof(position.name));
    name[24] = '\0';
    if (g_forbiddenWords.Contains(name)) {
      std::string masked;
      g_forbiddenWords.Mask(name, &masked);
    }

    guildInfo->m_positions[position.positionId] = name;

    GuildInfo::Unknown00D4Entry entry;
    entry.positionId = position.positionId;
    entry.name = guildInfo->m_positions[position.positionId];
    entry.canInvite = position.mode & 1;
    entry.canExpel = (position.mode >> 4) & 1;
    entry.tax = position.payRate;
    entry.canStorage = (position.mode >> 8) & 1;
    entry.ranking = position.ranking;
    guildInfo->UpdatePosition(entry);
  }

  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D49CC0
// FUNCTION: fn_21860
void CGameMode::ReceiveGuildMemberLeft(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildMemberRemovedPacket*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  char reason[41];
  std::memcpy(reason, packet->reason, sizeof(packet->reason));
  reason[40] = '\0';
  char name[25];
  std::memcpy(name, packet->name, sizeof(packet->name));
  name[24] = '\0';

  if (std::string(name) == g_session.DecodePlayerName()) {
    guildInfo->Init();
    g_windowMgr.CloseWindow(WID_GUILDWND);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LEFT_GUILD)), 0xFF, 4, 0, 0);
    this->scene->player->SetGuildInfo(0, 0);
    this->SendMsg(MM_DELETEGUILDPOSALL, 0, 0, 0, 0);
    return;
  }

  if (guildInfo->membersRequested) {
    guildInfo->DeleteMember(name);
  }

  char message[1024];
  std::sprintf(message, LookupMessage(MSI_USER_LEFT_GUILD), name);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);
  sprintf_s(message, sizeof(message), LookupMessage(MSI_REASON_LEAVE_GUILD), reason);
  if (g_forbiddenWords.Contains(message) == true) {
    std::string masked;
    g_forbiddenWords.Mask(message, &masked);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);
}

#pragma pack(push, 1)
struct GuildPacketText {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ char text[1];
};

#pragma pack(push, 1)
struct GuildMemberKickedPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char text[40];
  /* 002A */ std::uint32_t characterId;
};

struct GuildMemberLeftPacket2 {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t characterId;
  /* 0006 */ char name[30];
};
#pragma pack(pop)

// INLINED
void ClearGuildShortcutSkills() {
  g_session.ClearAdditionalShortcutSkills();
}

// OFFSET: 0x00D48C60
// FUNCTION: fn_21856
void CGameMode::ReceiveGuildExpulsion2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildMemberKickedPacket*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  char reason[41];
  std::memcpy(reason, packet->text, sizeof(packet->text));
  reason[40] = '\0';

  if (packet->characterId == g_session.m_gid) {
    ClearGuildShortcutSkills();
    g_guildInfo.m_guildId = 0;
    g_windowMgr.CloseWindow(WID_GUILDWND);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BAN_FROM_GUILD)), 0xFF, 4, 0, 0);
    this->SendMsg(MM_DELETEGUILDPOSALL, 0, 0, 0, 0);
    if (this->scene != nullptr && this->scene->player != nullptr) {
      this->scene->player->SetGdid(0);
    }
    guildInfo->ClearAgitIds();
    return;
  }

  if (guildInfo->membersRequested) {
    guildInfo->DeleteMemberByCharacterId(static_cast<int>(packet->characterId));
  }

  std::string name = LookupPlayerName(packet->characterId);
  if (name.empty()) {
    name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
  }

  char message[1024];
  std::memset(message, 0, sizeof(message));
  std::sprintf(message, LookupMessage(MSI_BAN_GUILD), name.c_str());
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);
  std::memset(message, 0, sizeof(message));
  sprintf_s(message, sizeof(message), LookupMessage(MSI_REASON_BAN_GUILD), reason);
  if (g_forbiddenWords.Contains(message) == true) {
    std::string masked;
    g_forbiddenWords.Mask(message, &masked);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);

  GuildInfo::Unknown00ECEntry entry;
  entry.name = name.c_str();
  entry.reason = reason;
  guildInfo->AddBannedMember(entry);
}

// OFFSET: 0x00D4A060
// FUNCTION: fn_21861
void CGameMode::ReceiveGuildLeave2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildMemberLeftPacket2*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  char reason[40];
  strcpy_s(reason, sizeof(reason), packet->name);

  if (packet->characterId == g_session.m_gid) {
    ClearGuildShortcutSkills();
    guildInfo->Init();
    g_windowMgr.CloseWindow(WID_GUILDWND);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LEFT_GUILD)), 0xFF, 4, 0, 0);
    this->scene->player->SetGuildInfo(0, 0);
    this->SendMsg(MM_DELETEGUILDPOSALL, 0, 0, 0, 0);
    return;
  }

  if (guildInfo->membersRequested) {
    guildInfo->DeleteMemberByCharacterId(static_cast<int>(packet->characterId));
  }

  std::string name;
  if (packet->characterId == g_session.m_gid) {
    name = g_session.DecodePlayerName();
  } else {
    name = LookupPlayerName(packet->characterId);
  }
  if (name.empty()) {
    name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
  }

  char message[1024];
  std::sprintf(message, LookupMessage(MSI_USER_LEFT_GUILD), name.c_str());
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);
  sprintf_s(message, sizeof(message), LookupMessage(MSI_REASON_LEAVE_GUILD), reason);
  if (g_forbiddenWords.Contains(message) == true) {
    std::string masked;
    g_forbiddenWords.Mask(message, &masked);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 4, 0, 0);

  if (CRenderObject* actor = this->scene->GetGameActorByAID(packet->characterId)) {
    static_cast<CGameActor*>(actor)->SetGuildInfo(0, 0);
  }
}

#pragma pack(push, 1)
struct PartyRosterRecord50 {
  /* 0000 */ std::uint32_t accountId;
  /* 0004 */ char name[24];
  /* 001C */ char mapName[16];
  /* 002C */ unsigned char member;
  /* 002D */ unsigned char offline;
  /* 002E */ std::int16_t x;
  /* 0030 */ std::int16_t y;
};
#pragma pack(pop)

struct PartyRosterListPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t length;
  /* 0004 */ char partyName[24];
  /* 001C */ PartyRosterRecord50 entries[1];
};

// OFFSET: 0x00D6F900
// FUNCTION: fn_22124
void CGameMode::ReceivePartyInfo2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const PartyRosterListPacket*>(data);
  std::vector<PartyRosterEntry> members;
  const int count = (packet->length - 0x1C) / sizeof(PartyRosterRecord50);
  for (int index = 0; index < count; ++index) {
    const PartyRosterRecord50& record = packet->entries[index];
    PartyRosterEntry entry = {};
    entry.accountId = record.accountId;
    std::memcpy(entry.name, record.name, sizeof(entry.name));
    std::memcpy(entry.mapName, record.mapName, sizeof(entry.mapName));
    entry.member = record.member;
    entry.offline = record.offline;
    entry.x = record.x;
    entry.y = record.y;
    members.push_back(entry);
  }

  this->SetPartyMemberList(reinterpret_cast<const char*>(packet->partyName), &members);
}

#pragma pack(push, 1)
struct EquipItemUpdatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t itemId;
  /* 0004 */ unsigned char damaged;
  /* 0005 */ std::uint32_t cards[4];
  /* 0015 */ ItemSlotOption options[5];
  /* 002E */ unsigned char refine;
  /* 002F */ unsigned char grade;
};
#pragma pack(pop)

// OFFSET: 0x00D6D850
// FUNCTION: fn_22091
void CGameMode::RefreshInventoryOrEquipItem(std::uint16_t itemId, unsigned char unknownValue, std::uint16_t refine, ItemCardSlots cards, ItemSlotOptions options) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, itemId);
  int lookup = 0;
  int slot = 0;
  if (item.identity[1] == 0) {
    item = g_session.FindEquippedItem(itemId, &slot);
    lookup = 1;
    if (item.identity[1] == 0) {
      item = g_session.FindCostumeItem(itemId, &slot);
      lookup = 2;
    }
  }

  item.damaged = unknownValue;
  item.refiningLevel = refine;
  std::memcpy(item.cards, cards.cards, sizeof(cards.cards));
  item.optionCount = CountItemSlotOptions(options.options);
  for (int index = 0; index < item.optionCount; ++index) {
    reinterpret_cast<ItemSlotOption*>(item.options)[index] = options.options[index];
  }

  if (lookup == 0) {
    g_session.UpdateInventoryItem(item, 1);
  } else if (lookup == 1) {
    g_session.SetEquippedItem(slot, item);
  } else if (lookup == 2) {
    g_session.SetCostumeItem(slot, item);
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMCOLLECTIONWND)) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
  }
}

// OFFSET: 0x00D6DAE0
// FUNCTION: fn_22092
void CGameMode::RefreshInventoryOrEquipItem2(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const EquipItemUpdatePacket*>(data);
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->itemId);
  long long lookup = 0;
  int slot = 0;
  if (item.identity[1] == 0) {
    item = g_session.FindEquippedItem(packet->itemId, &slot);
    lookup = 1;
    if (item.identity[1] == 0) {
      item = g_session.FindCostumeItem(packet->itemId, &slot);
      lookup = 2;
    }
  }

  item.damaged = packet->damaged;
  item.refiningLevel = packet->refine;
  for (long long index = 0; index < 4; ++index) {
    reinterpret_cast<std::uint32_t*>(item.cards)[index] = packet->cards[index];
  }
  item.optionCount = CountItemSlotOptions(packet->options);
  for (long long index = 0; index < item.optionCount; ++index) {
    reinterpret_cast<ItemSlotOption*>(item.options)[index] = packet->options[index];
  }
  item.grade = packet->grade;

  if (lookup == 0) {
    g_session.UpdateInventoryItem(item, 1);
  } else if (lookup == 1) {
    g_session.SetEquippedItem(slot, item);
  } else if (lookup == 2) {
    g_session.SetCostumeItem(slot, item);
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMCOLLECTIONWND)) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
  }
}

// OFFSET: 0x00D48530
// FUNCTION: fn_21853
void CGameMode::ReceiveWhisper(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildPacketText*>(data);
  char nameBuffer[24] = {};
  StringFormat(nameBuffer, sizeof(nameBuffer), static_cast<std::size_t>(-1), "%s", reinterpret_cast<const char*>(data) + 4);

  std::string text(reinterpret_cast<const char*>(data) + 0x20, reinterpret_cast<const char*>(data) + packet->length);
  this->ProcessWhisper(0, std::string(nameBuffer), text, *reinterpret_cast<const int*>(data + 0x1C) != 0);
}

// OFFSET: 0x00D1A330
// FUNCTION: fn_21617
void CGameMode::ReceiveWhisper2(const unsigned char* data) {
  char nameBuffer[24];
  std::memset(nameBuffer, 0, sizeof(nameBuffer));
  StringFormat(nameBuffer, sizeof(nameBuffer), static_cast<std::size_t>(-1), "%s", reinterpret_cast<const char*>(data) + 8);

  const int length = *reinterpret_cast<const std::uint16_t*>(data + 2) - 0x21;
  std::string text(reinterpret_cast<const char*>(data) + 0x21, length > 0 ? static_cast<std::size_t>(length) : 0);

  this->ProcessWhisper(static_cast<int>(*reinterpret_cast<const std::uint32_t*>(data + 4)), std::string(nameBuffer), text, data[0x20] != 0);
}

#include "Contents/ShowRankingScheduler.h"

// OFFSET: 0x00D06760
// FUNCTION: fn_21576
static std::map<int, RankingBoardStyle> RankNoticeStyles() {
  std::map<int, RankingBoardStyle> styles;
  styles.insert({0, {"========== BLACKSMITH RANK ============", 0x9B9B9B, 0xCDCDCD}});
  styles.insert({1, {"=========== ALCHEMIST RANK =============", 0x9BCD9B, 0xCDFFCD}});
  styles.insert({2, {"============ TAEKWON RANK =============", 0xCD9B9B, 0xFFCDCD}});
  styles.insert({3, {"=========== KILLER RANK ==============", 0x9B9B9B, 0xCDCDCD}});
  return styles;
}

#pragma pack(push, 1)
struct RankingListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t type;
  /* 0004 */ char names[10][24];
  /* 00F4 */ int points[10];
  /* 011C */ int myPoint;
};
#pragma pack(pop)

// OFFSET: 0x00D19390
// FUNCTION: fn_21605
void CGameMode::ReceiveRankingList(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RankingListPacket*>(data);
  const int kind = packet->type;
  // GLOBAL: gl_25258
  static const std::map<int, RankingBoardStyle> styles = RankNoticeStyles();

  const char* title;
  std::uint32_t color;
  std::uint32_t background;
  auto style = styles.find(kind);
  if (style != styles.end()) {
    background = style->second.background;
    title = style->second.title;
    color = style->second.color;
  } else {
    title = "===========================================";
    background = 0;
    color = 0;
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(title), color, 0, 0, 0);

  for (int index = 0; index < 10; ++index) {
    const std::string line = FormatOwnedString("[ %2d ] %-18s  :  %4d POINT", index + 1, packet->names[index], packet->points[index]);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line.c_str()), background, 0, 0, 0);
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("==========================================="), color, 0, 0, 0);

  const std::string mine = FormatOwnedString("MY POINT    :  %4d POINT ", packet->myPoint);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(mine.c_str()), background, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("==========================================="), color, 0, 0, 0);
}

struct GuildStorageLogItem {
  /* 0000 */ ITEM_INFO item;
  /* 00F8 */ unsigned char highlighted;
};

#pragma pack(push, 1)
struct GuildStorageLogItemRecord {
  /* 0000 */ std::uint32_t logId;
  /* 0004 */ std::uint32_t itemId;
  /* 0008 */ std::uint32_t quantity;
  /* 000C */ unsigned char operation;
  /* 000D */ unsigned char uniqueId[8];
  /* 0015 */ unsigned char identified;
  /* 0016 */ std::int16_t type;
  /* 0018 */ std::uint32_t cards[4];
  /* 0028 */ char name[24];
  /* 0040 */ char characterName[24];
  /* 0058 */ unsigned char highlighted;
  /* 0059 */ unsigned char refine;
  /* 005A */ unsigned char grade;
};

struct GuildStorageLogItemsPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint16_t result;
  /* 0006 */ std::int16_t count;
  /* 0008 */ GuildStorageLogItemRecord records[1];
};
#pragma pack(pop)

// OFFSET: 0x00D69A90
// FUNCTION: fn_22055
void CGameMode::ReceiveGuildStorageLogItems(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildStorageLogItemsPacket*>(data);
  std::vector<GuildStorageLogItem> items;
  items.reserve(packet->count);
  if (packet->result == 0 || packet->result == 1) {
    for (long long index = 0; index < packet->count; ++index) {
      const GuildStorageLogItemRecord& record = packet->records[index];
      ITEM_INFO item;
      item.recordIds[0] = record.logId;
      item.recordIds[1] = record.quantity;
      item.recordType = record.operation;
      strncpy_s(reinterpret_cast<char*>(item.recordName), sizeof(item.recordName), record.name, _TRUNCATE);
      strncpy_s(reinterpret_cast<char*>(item.recordDate), sizeof(item.recordDate), record.characterName, _TRUNCATE);
      char number[16] = {};
      item.itemId = _itoa(record.itemId, number, 10);
      item.identified = record.identified;
      item.identity[0] = record.type;
      item.refiningLevel = record.refine;
      std::memcpy(item.cards, record.cards, sizeof(record.cards));
      item.grade = record.grade;
      items.push_back({item, record.highlighted});
    }
  }

  this->ApplyGuildStorageLog(packet->result, items);
}

static std::map<int, int> BuildStatusFlagEffects() {
  std::map<int, int> effects;
  effects[0x1] = 0x373;
  effects[0x2] = 0x374;
  effects[0x4] = 0x375;
  effects[0x8] = 0x376;
  effects[0x10] = 0x377;
  effects[0x20] = 0x378;
  effects[0x40] = 0x379;
  effects[0x80] = 0x37A;
  effects[0x100] = 0x37B;
  return effects;
}

static std::map<int, int> BuildStatusLevelEffects() {
  std::map<int, int> effects;
  effects[0] = 0;
  effects[1] = 0x36B;
  effects[2] = 0x36C;
  effects[3] = 0x36D;
  effects[4] = 0x36E;
  effects[5] = 0;
  effects[6] = 0x370;
  effects[7] = 0x371;
  effects[8] = 0x372;
  return effects;
}

// OFFSET: 0x00D18D10
// FUNCTION: fn_21602
void CGameMode::ApplyStatusIcons(int level, unsigned int flags) {
  // GLOBAL: gl_25274
  static const std::map<int, int> statusFlagEffects = BuildStatusFlagEffects();
  // GLOBAL: gl_25275
  static const std::map<int, int> statusLevelEffects = BuildStatusLevelEffects();

  const std::uint32_t aid = g_session.m_aid;
  for (const auto& effect : statusFlagEffects) {
    if (effect.second != 0x377 && effect.second != 0x378 && effect.second != 0x379) {
      this->ApplyStatusChange(aid, (effect.first & flags) != 0, effect.second, 9999, 0, 0, 0, 0, 9999);
    }
  }

  auto previous = statusLevelEffects.find(*reinterpret_cast<const int*>(&g_session.statusLevelEffects[0]));
  if (previous != statusLevelEffects.end()) {
    this->ApplyStatusChange(aid, 0, previous->second, 9999, 0, 0, 0, 0, 9999);
  }

  if (level != 0) {
    auto next = statusLevelEffects.find(level);
    if (next != statusLevelEffects.end()) {
      this->ApplyStatusChange(aid, 1, next->second, 9999, 0, 0, 0, 0, 9999);
    }
  }
}

// OFFSET: 0x00D1A520
// FUNCTION: fn_21618
void CGameMode::ProcessWhisper(int unknown_0004, std::string name, std::string text, bool flag) {
  const std::string sender(name);
  std::vector<char> message(text.begin(), text.end());
  message.push_back('\0');

  std::string line(name);
  line.append(" : ");
  line.append(message.data());

  if (g_windowMgr.ShowWhisperFrom(name.c_str(), line.c_str(), 0x222222, static_cast<std::uint32_t>(unknown_0004)) != 0) {
    if (g_session.autoReplyState[0] != 0) {
      char notice[24];
      std::sprintf(notice, LookupMessage(MSI_AUTO_MSG2));
      const char length = static_cast<char>(std::strlen(notice));
      char head[24];
      std::memcpy(head, message.data(), length);
      head[length] = '\0';
      if (std::strcmp(head, notice) != 0) {
        std::string reply(LookupMessage(MSI_AUTO_MSG2));
        reply.append(reinterpret_cast<const std::string&>(g_session.autoReplyState[4]).c_str());
        g_windowMgr.SetChatInputText(reply.c_str());
        g_modeMgr.gameMode->SendMsg(MM_WHISPERMSG, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
      }
    }

    if (flag) {
      if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
        std::string header = "[ ";
        header.append(sender);
        std::string body(message.data());
        {
          std::string linked;
          DisplayNavigationLink(linked, body);
          body = std::move(linked);
        }
        if (std::strstr(message.data(), g_whisperItemLinkTag.c_str()) != nullptr) {
          body = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(body);
          header = header + " ] : " + body;
        } else {
          header = header + " ] : " + message.data();
        }

        BroadcastBalloonMessage balloonMessage{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
        balloonMessage.start = std::chrono::system_clock::now();
        std::uint32_t color = 0xFFFF;
        balloonMessage.lines.emplace_back(header, color);
        balloon->AddMessage(1, balloonMessage);
      }
    }
    return;
  }

  std::string prefix;
  if (g_windowMgr.IsFriend(name.c_str())) {
    prefix = "[ Friend ";
    prefix.append(sender);
    if (unknown_0004 != 0) {
      prefix.append(" (" + ShortcutFilename(static_cast<std::uint32_t>(unknown_0004)) + ")");
    }
    prefix = prefix + " ] : " + message.data();
  } else if (flag) {
    prefix = "[ ";
    prefix.append(sender);
    if (unknown_0004 != 0) {
      prefix.append(" (" + ShortcutFilename(static_cast<std::uint32_t>(unknown_0004)) + ")");
    }
    prefix = prefix + " ] : " + message.data();
  } else if (g_session.IsGuildMemberName(name.c_str())) {
    prefix = "[ Member ";
    prefix.append(sender);
    if (unknown_0004 != 0) {
      prefix.append(" (" + ShortcutFilename(static_cast<std::uint32_t>(unknown_0004)) + ")");
    }
    prefix = prefix + " ] : " + message.data();
  } else {
    CSession& lists = g_session;
    if (lists.IsFriendNameDifferent(name.c_str())) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ID_WARNING)), 0xFFFF, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ID_WARNING)), 0xFFFF, 0, 0, 0);
    } else if (lists.IsGuildMemberNameDifferent(name.c_str())) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ID_WARNING_GUILD)), 0xFFFF, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ID_WARNING_GUILD)), 0xFFFF, 0, 0, 0);
    }
    prefix = "( From ";
    prefix.append(sender);
    if (unknown_0004 != 0) {
      prefix.append(" [" + ShortcutFilename(static_cast<std::uint32_t>(unknown_0004)) + "]");
    }
    prefix = prefix + " ) : " + message.data();
  }

  if (std::strcmp(name.c_str(), g_session.DecodePlayerName()) != 0) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(prefix.c_str()), 0xFFFF, flag ? 25 : 2, reinterpret_cast<std::intptr_t>(sender.c_str()), 0);
    g_windowMgr.AddWhisperName(name.c_str());
    if (flag) {
      if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
        if (prefix.find(g_whisperItemLinkTag) != std::string::npos) {
          prefix = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(prefix);
        }
        {
          std::string linked;
          DisplayNavigationLink(linked, prefix);
          prefix = std::move(linked);
        }

        BroadcastBalloonMessage balloonMessage{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
        balloonMessage.start = std::chrono::system_clock::now();
        std::uint32_t color = 0xFFFF;
        const char* lineText = prefix.c_str();
        balloonMessage.lines.emplace_back(lineText, color);
        balloon->AddMessage(1, balloonMessage);
      }
    }
    if (g_session.m_whisperRecord != 0) {
      this->WriteChatLog(prefix.c_str());
    }
  }

  if (g_session.autoReplyState[0] != 0) {
    char notice[24];
    std::sprintf(notice, LookupMessage(MSI_AUTO_MSG2));
    const char length = static_cast<char>(std::strlen(notice));
    char head[24];
    std::memcpy(head, message.data(), length);
    head[length] = '\0';
    if (std::strcmp(head, notice) != 0) {
      std::string reply(LookupMessage(MSI_AUTO_MSG2));
      reply.append(reinterpret_cast<const std::string&>(g_session.autoReplyState[4]).c_str());
      g_windowMgr.SetChatInputText(reply.c_str());
      g_modeMgr.gameMode->SendMsg(MM_WHISPERMSG, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D69530
// FUNCTION: fn_22050
void CGameMode::ReceiveStoreResult(unsigned char kind, unsigned char result) {
  if (kind == 100) {
    switch (result) {
      case 0:
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
        g_session.ClearMerchantPurchaseSavedItems();
        break;
      case 1:
      case 2:
      case 3: {
        UIWindow* window = g_windowMgr.LookupWindow(WID_MERCHANTSHOPMAKEWND);
        if (window) {
          static_cast<UIMerchantShopMakeWnd*>(window)->SetSuspended(0);
        }
        break;
      }
      case 4:
      case 6:
      case 7:
        g_session.ClearMerchantPurchaseSavedItems();
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
        break;
    }
  } else if (kind == 101) {
    switch (result) {
      case 0:
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
        g_session.ClearMerchantPurchaseSavedItems();
        break;
      case 1:
      case 2:
      case 3: {
        UIWindow* window = g_windowMgr.LookupWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
        if (window) {
          static_cast<UIMerchantShopMakeWnd*>(window)->SetSuspended(0);
        }
        break;
      }
      case 4:
      case 6:
      case 7:
      case 8:
        g_session.ClearMerchantPurchaseSavedItems();
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
        g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_1);
        break;
    }
  }

  switch (result) {
    case 2:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_EQUIPMENTSET)), 0xFF, 0, 0, 0);
      break;
    case 3:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_USESKILL_FAIL_THERE_ARE_NPC_AROUND)), 0xFF, 0, 0, 0);
      break;
    case 8:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OVER_WEIGHT)), 0xFF, 0, 0, 0);
      break;
  }

  if (result == 200) {
    g_session.ClearMerchantPurchaseSavedItems();
    g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
    g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
    g_windowMgr.CloseWindow(WID_ITEMWND);
    g_windowMgr.ShowWaitDBSaveWindow();
    return;
  }

  g_windowMgr.CloseWindow(WID_WAITDBSAVEWND);
}
