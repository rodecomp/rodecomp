#include "UI/Chat/UINewChatWnd.h"
#include "Resource/World/TerrainResource.h"
#include "Client/Application/Launcher.h"
#include "File/ShortcutFilename.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/EquipWindowLook.h"
#include "UI/UIMacroDetectorWnd.h"
#include "UI/UIMacroBlackListCheckWnd.h"
#include "UI/UILapineScrollBar.h"
#include "UI/UICaptchaPreviewWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/ModeFadeOut.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/MousePointer.h"
#include "Contents/GuildInfo.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/MonsterSkillEffects.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RodexMailStore.h"
#include "Contents/RouletteMgr.h"
#include "Contents/World/World.h"
#include "Contents/Camera/Camera.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/RenderSystemBase.h"
#include "Render/Renderer.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/ResourceMgr.h"
#include "Network/TextEncoding.h"
#include "Contents/SpecialPopupMgr.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "UI/UIBarGraph.h"
#include "UI/UIBroadcastBalloon.h"
#include "UI/Window/UIHomunInfoWnd.h"
#include "Contents/Sound/SoundMgr.h"
#include "UI/Status/UIStatusWnd.h"
#include "UI/Window/UIBasicInfoWnd.h"
#include "UI/Shop/UIMerchantItemMyShopWnd.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Shop/UIMerchantItemShopWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/UIRefiningWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/GuildMgr.h"
#include "Contents/EnchantMgr.h"
#include "Contents/Items/ItemReformMgr.h"
#include "Contents/ReputeMgr.h"
#include "Contents/RenewQuestMgr.h"
#include "Contents/Items/BarterMarketItemMgr.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/EmotionSchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Text/UITextViewer.h"
#include "Utility/Cipher/XorBytes.h"
#include <algorithm>
#include <set>
#include <direct.h>
#include <io.h>
#include <timeapi.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <xtimec.h>
#include "Contents/ChampionMonsterMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Actors/GraffiSkill.h"
#include "UI/Party/UIRequestJoinPartyMgr.h"
#include "Contents/Items/RuneSystem.h"
#include "Contents/SimpleStringMgr.h"
#include "Contents/Party/PartyJoinResult.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Contents/EntryQueueMgr.h"
#include "UI/Window/UIBalloonOpenBtn.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Client/Application/ErrorDialog.h"
#include "UI/Items/UIItemCollectionWnd.h"
#include "Contents/MapPropertyMgr.h"
#include <ctime>
#include "UI/Window/UIChatRoomWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "Contents/ClanInfo.h"
#include "Contents/NavigationManager.h"
#include "Utility/Text/LanguageCharset.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Mode/CLoginMode.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Item/UIParaItemShopWnd.h"
#include "Script/LuaScript.h"
#include "Contents/Actors/Item.h"
#include "Contents/ClientState.h"
#include "Contents/ClientItemLists.h"
#include "Contents/Sound/SoundMaker.h"
#include "Contents/PetEvolutionMgr.h"
#include "UI/Window/UIPetInfoWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "Network/ActorNamePacket.h"
#include "UI/Guild/UIGuild_Storage_Log.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Ground/TerrainHeight.h"
#include <new>

// OFFSET: 0x00D7AF70
// FUNCTION: fn_22198
void CGameMode::ReceiveItemDisappear(std::uint32_t itemAid) {
  if (this->scene->RemoveItem(itemAid)) {
    this->SendMsg(13, static_cast<int>(itemAid), 0, 0, 0);
  }
}

#pragma pack(push, 1)
struct TradeItemRecord {
  /* 0000 */ int itemId;
  /* 0004 */ unsigned char unknown_0004;
  /* 0005 */ int unknown_0005;
  /* 0009 */ int quantity;
  /* 000D */ std::uint16_t unknown_000D;
  /* 000F */ unsigned char unknown_000F;
  /* 0010 */ std::uint32_t cards[4];
  /* 0020 */ std::uint16_t unknown_0020;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TradeItemRecord) == 0x22);
#endif

void __fastcall sub_DFF150(CSession* session);

// GLOBAL: gl_24992
char g_pendingWhisperCount;

// GLOBAL: gl_25102
int g_whisperRecord;

namespace {
#pragma pack(push, 1)
  struct SimulationPacketHeader {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t length;
    /* 0004 */ std::uint32_t unknown_0004;
    /* 0008 */ int value;
    /* 000C */ unsigned char unknown_000C[0x18];
  };

  struct EnchantSimulationRecord {
    /* 0000 */ int itemId;
    /* 0004 */ int resultItemId;
    /* 0008 */ std::uint32_t unknown_0008[2];
    /* 0010 */ int rate;
    /* 0014 */ std::uint32_t unknown_0014;
    /* 0018 */ int count;
  };

  struct PackageSimulationRecord {
    /* 0000 */ int itemId;
    /* 0004 */ unsigned char refine;
    /* 0005 */ unsigned char packageType;
    /* 0006 */ int unknown_0006;
    /* 000A */ int unknown_000A;
    /* 000E */ char text[24];
    /* 0026 */ int unknown_0026;
    /* 002A */ int unknown_002A;
    /* 002E */ int count;
  };

  struct NamedProgressPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ unsigned char result;
    /* 0003 */ int first;
    /* 0007 */ int second;
    /* 000B */ std::uint16_t current;
    /* 000D */ std::uint16_t total;
    /* 000F */ unsigned char unknown_000F[4];
    /* 0013 */ char name[1];
  };

  struct PacketLengthHeader {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::int16_t length;
  };

  struct Item1740Record {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ unsigned char identified;
    /* 0008 */ std::uint16_t location;
    /* 000A */ std::uint16_t wearState;
    /* 000C */ unsigned char unknown_000C;
    /* 000D */ unsigned char refine;
    /* 000E */ std::uint32_t cards[4];
    /* 001E */ std::uint32_t expireTime;
  };

  struct Item1740EquipRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ unsigned char identified;
    /* 0008 */ std::uint16_t location;
    /* 000A */ std::uint16_t wearState;
    /* 000C */ unsigned char unknown_000C;
    /* 000D */ unsigned char refine;
    /* 000E */ std::uint32_t cards[4];
    /* 001E */ std::uint32_t expireTime;
    /* 0022 */ std::uint16_t bindOnEquip;
    /* 0024 */ std::uint16_t sprite;
  };

  struct ItemStoreListHeader {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t length;
    /* 0004 */ char title[24];
  };

  struct StoreItemOption {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t value;
    /* 0004 */ unsigned char param;
  };

  struct StoreEquipRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::uint32_t location;
    /* 000B */ std::uint32_t wearState;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ std::uint32_t expireTime;
    /* 0024 */ std::uint16_t bindOnEquip;
    /* 0026 */ std::uint16_t sprite;
    /* 0028 */ unsigned char flags;
  };

  struct StoreEquipOptionRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::uint32_t location;
    /* 000B */ std::uint32_t wearState;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ std::uint32_t expireTime;
    /* 0024 */ std::uint16_t bindOnEquip;
    /* 0026 */ std::uint16_t sprite;
    /* 0028 */ unsigned char optionCount;
    /* 0029 */ StoreItemOption options[5];
    /* 0042 */ unsigned char flags;
  };

  struct StoreNormalRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::int16_t amount;
    /* 0009 */ std::uint32_t location;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ std::uint32_t expireTime;
    /* 0021 */ unsigned char flags;
  };

  struct HomunculusProperty64Packet {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t type;
    /* 0004 */ std::int64_t value;
  };

  struct HomunculusPropertyPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t type;
    /* 0004 */ int value;
  };

  struct ValuePacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ int value;
  };

  struct ChatTextEntry {
    /* 0000 */ char text[16];
  };

  struct ChatEntryListPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::int16_t length;
    /* 0004 */ std::uint16_t unknown_0004;
    /* 0006 */ ChatTextEntry entries[1];
  };

  struct ActorValuePacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint32_t accountId;
    /* 0006 */ short value;
  };

  struct ActorSkillMessage {
    /* 0000 */ int unknown_0000;
    /* 0004 */ int unknown_0004;
    /* 0008 */ int unknown_0008;
    /* 000C */ int unknown_000C;
    /* 0010 */ int unknown_0010;
    /* 0014 */ int unknown_0014;
    /* 0018 */ int unknown_0018;
    /* 001C */ int unknown_001C;
    /* 0020 */ int unknown_0020;
    /* 0024 */ int unknown_0024;
    /* 0028 */ int unknown_0028;
  };

  struct DispelPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::uint32_t aid;
  };

  struct DevotionListPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::uint32_t aid;
    /* 0006 */ std::uint32_t devotees[5];
    /* 001A */ std::int16_t range;
  };

  struct FormatStringMessagePacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t length;
    /* 0004 */ std::uint16_t messageId;
  };

  struct NotifyExpPacket {
    /* 0000 */ std::int16_t packetType;
    /* 0002 */ std::uint32_t accountId;
    /* 0006 */ int amount;
    /* 000A */ std::uint16_t varId;
    /* 000C */ std::int16_t expType;
  };

  struct Item1758Record {
    /* 0000 */ std::int32_t price;
    /* 0004 */ std::int16_t quantity;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::int32_t itemId;
  };

  struct Item1758StoreRecord {
    /* 0000 */ std::uint32_t unknown_0000;
    /* 0004 */ std::int16_t index;
    /* 0006 */ std::int16_t amount;
    /* 0008 */ unsigned char type;
    /* 0009 */ int itemId;
    /* 000D */ unsigned char identified;
    /* 000E */ unsigned char damaged;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ StoreItemOption options[5];
  };

  struct Item1718Record {
    /* 0000 */ std::int32_t itemId;
    /* 0004 */ unsigned char type;
    /* 0005 */ std::uint32_t unknown_0005;
    /* 0009 */ std::int32_t quantity;
    /* 000D */ std::uint16_t unknown_000D;
    /* 000F */ std::uint32_t unknown_000F;
  };

  struct Item1748Record {
    /* 0000 */ std::uint32_t unknown_0000;
    /* 0004 */ std::int16_t amount;
    /* 0006 */ std::int16_t index;
    /* 0008 */ unsigned char type;
    /* 0009 */ int itemId;
    /* 000D */ unsigned char identified;
    /* 000E */ unsigned char damaged;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ StoreItemOption options[5];
    /* 0039 */ std::int32_t unknown_0039;
    /* 003D */ std::uint16_t sprite;
  };
#pragma pack(pop)

  static_assert(sizeof(ItemStoreListHeader) == 0x1C);
  static_assert(sizeof(StoreEquipRecord) == 0x29);
  static_assert(sizeof(StoreEquipOptionRecord) == 0x43);
  static_assert(sizeof(StoreNormalRecord) == 0x22);
  static_assert(sizeof(PacketLengthHeader) == 0x04);
  static_assert(sizeof(Item1740Record) == 0x22);
  static_assert(sizeof(Item1740EquipRecord) == 0x26);
  static_assert(sizeof(ValuePacket) == 0x06);
  static_assert(sizeof(ChatTextEntry) == 0x10);
  static_assert(sizeof(ChatEntryListPacket) == 0x16);
  static_assert(sizeof(ActorValuePacket) == 0x08);

  // INLINED
  int CountStoreItemOptions(const StoreItemOption* options) {
    int count = 0;
    for (int index = 0; index < 5; ++index) {
      if (options[index].id != 0) {
        ++count;
      }
    }
    return count;
  }

  static_assert(sizeof(DispelPacket) == 6);
  static_assert(sizeof(DevotionListPacket) == 0x1C);
  static_assert(sizeof(FormatStringMessagePacket) == 6);
  static_assert(sizeof(NotifyExpPacket) == 0xE);
  static_assert(sizeof(Item1758Record) == 0xB);
  static_assert(sizeof(Item1758StoreRecord) == 0x39);
  static_assert(sizeof(Item1718Record) == 0x13);
  static_assert(sizeof(Item1748Record) == 0x3F);
  static_assert(sizeof(SimulationPacketHeader) == 0x24);
  static_assert(sizeof(EnchantSimulationRecord) == 0x1C);
  static_assert(sizeof(PackageSimulationRecord) == 0x32);
}

// OFFSET: 0x00D1F540
// FUNCTION: fn_21638
void CGameMode::IgnoreRemovedPacket3(const unsigned char*) {
}

// OFFSET: 0x00D208F0
// FUNCTION: fn_21649
void CGameMode::IgnoreRemovedPacket4(const unsigned char*) {
}

// OFFSET: 0x00D21210
// FUNCTION: fn_21653
void CGameMode::IgnoreRemovedPacket5(const unsigned char*) {
}

// OFFSET: 0x00D21D00
// FUNCTION: fn_21657
void CGameMode::ReceiveUnknown05C4Result(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const NamedProgressPacket*>(data);
  char message[256];
  char name[51] = {};
  std::sprintf(name, "%s", packet->name);
  this->unknown_05C4 = -1;
  this->unknown_05C8 = -1;

  switch (packet->result) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1223)), 0xFFFFFF, 0, 0, 0);
    g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(1223)), 0xFFFFFF, 0, 0, 0);
    return;
  case 1:
  case 2:
    this->unknown_05C4 = packet->first;
    this->unknown_05C8 = packet->second;
    if (packet->result != 2) {
      return;
    }
    std::sprintf(message, LookupMessage(1225), name);
    break;
  case 3:
    if (packet->current == 0 && packet->total == 1) {
      std::sprintf(message, LookupMessage(1261), name);
    } else {
      std::sprintf(message, LookupMessage(1224), name, packet->current, packet->total);
    }
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
}

// OFFSET: 0x00D230E0
// FUNCTION: fn_21660
void CGameMode::ReceiveColoredNotice(const unsigned char* packet) {
  std::string text;
  text.assign(reinterpret_cast<const char*>(packet + 9), *reinterpret_cast<const short*>(packet + 2) - 9);
  if (!text.empty()) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), *reinterpret_cast<const std::uint32_t*>(packet + 5), packet[4], 0, 0);
  }
}

// OFFSET: 0x00D1D690
// FUNCTION: fn_21630
void CGameMode::ReceiveNamedActionResult(const unsigned char* packet) {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  const std::string name = reinterpret_cast<const char*>(packet + 2);
  const int result = *reinterpret_cast<const int*>(packet + 0x1A);

  switch (result) {
  case 0:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x50)), 0xFF, 0, 0, 0);
    break;
  case 1: {
    const std::string text = name + LookupMessage(0x80C);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
    break;
  }
  case 2: {
    const std::string text = name + LookupMessage(0x80D);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF00, 0, 0, 0);
    break;
  }
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x53)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x260)), 0xFF, 0, 0, 0);
    break;
  }

  switch (result) {
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x52C)), 0xFF, 0, 0, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x47)), 0xFF, 0, 0, 0);
    break;
  case 8: {
    char text[256];
    std::sprintf(text, LookupMessage(0x56C), reinterpret_cast<const char*>(packet + 2));
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 9:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x74F)), 0xFF, 0, 0, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBD3)), 0xFF, 0, 0, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xC9A)), 0xFF, 0, 0, 0);
    break;
  case 18:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xE32)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D1E610
// FUNCTION: fn_21634
void CGameMode::ReceiveStatDump(const unsigned char* packet) {
  char text[256];
  const short* values = reinterpret_cast<const short*>(packet);

  std::sprintf(
    text, "             STR=%3d      AGI=%3d      VIT=%3d      INT=%3d      DEX=%3d      LUK=%d", packet[2], packet[4], packet[6], packet[8], packet[10], packet[12]
  );
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  std::sprintf(
    text, "standard STR=%3d      AGI=%3d      VIT=%3d      INT=%3d      DEX=%3d      LUK=%2d", packet[3], packet[5], packet[7], packet[9], packet[11], packet[13]
  );
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF37, 0, 0, 0);
  std::sprintf(
    text, "    attPower=%3d    refiningPow=%3d    MAXmatPow=%3d    MINmatPower=%3d       ASPD=%3d", values[7], values[8], values[9], values[10], values[19]
  );
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  std::sprintf(
    text, "itemdefPow=%3d     plusdefPow=%3d    mdefPower=%3d    plusmdefPow=%3d    plusASPD=%3d", values[11], values[12], values[13], values[14], values[20]
  );
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF37, 0, 0, 0);
  std::sprintf(text, "hitSuccessVal=%3d    avoidSuccessVal=%3d    plusAvoidSuccessValue=%3d", values[15], values[16], values[17]);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
}

// OFFSET: 0x00D24430
// FUNCTION: fn_21666
void CGameMode::ReceiveActorText(const unsigned char* packet) {
  char text[81];
  std::strncpy(text, reinterpret_cast<const char*>(packet + 6), 80);
  text[80] = '\0';
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(text);
    actor->SendMsg(nullptr, 141, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D25920
// FUNCTION: fn_21674
void CGameMode::ReceiveRentalItemTime(const unsigned char* packet) {
  char number[12];
  char text[260];
  _itoa(*reinterpret_cast<const int*>(packet + 2), number, 10);
  const ITEM_DESC& description = FindItemDescription(std::atoi(number));
  const std::uint32_t seconds = *reinterpret_cast<const std::uint32_t*>(packet + 6);
  const char* name = description.identifiedName;
  const int minutes = static_cast<int>(seconds / 60);
  if (minutes > 0) {
    std::sprintf(text, LookupMessage(1256), name, minutes);
  } else {
    std::sprintf(text, LookupMessage(1270), name, seconds);
  }
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  if (minutes == 1) {
    std::sprintf(text, LookupMessage(1257), name);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D264E0
// FUNCTION: fn_21679
void CGameMode::ReceiveUnknownD264E0(const unsigned char* packet) {
  if (UIWindow* window = g_windowMgr.unknown_01F0) {
    window->SendMsg(nullptr, 32, 0, 0, 0, 0);
    window->SendMsg(nullptr, 63, 0, 0, 0, 0);
  } else {
    this->unknown_024C = 0;
    this->unknown_02DC = *reinterpret_cast<const int*>(packet + 2);
    g_windowMgr.CloseWindow(16);
    g_windowMgr.CloseWindow(17);
    g_windowMgr.CloseWindow(226);
  }

  if (UIWindow* window = g_windowMgr.unknown_01F4) {
    window->SendMsg(nullptr, 32, 0, 0, 0, 0);
    window->SendMsg(nullptr, 63, 0, 0, 0, 0);
  } else {
    this->unknown_024C = 0;
    this->unknown_02DC = *reinterpret_cast<const int*>(packet + 2);
    g_windowMgr.CloseWindow(226);
    g_windowMgr.CloseWindow(17);
    g_windowMgr.CloseWindow(226);
  }
}

// OFFSET: 0x00D26600
// FUNCTION: fn_21680
void CGameMode::ReceiveActorSequenceDeadline(const unsigned char* packet) {
  g_session.InsertPair(*reinterpret_cast<const int*>(packet + 2), *reinterpret_cast<const int*>(packet + 6));
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  if (aid == g_session.m_aid) {
    this->scene->player->SetSequenceDeadline(*reinterpret_cast<const std::uint32_t*>(packet + 6));
    return;
  }
  if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
    actor->SetSequenceDeadline(*reinterpret_cast<const std::uint32_t*>(packet + 6));
  }
}

// OFFSET: 0x00D26E50
// FUNCTION: fn_21685
void CGameMode::ReceiveDevotionList(const unsigned char* packet) {
  const auto* list = reinterpret_cast<const DevotionListPacket*>(packet);

  if (!g_serviceOptions.Get(8)) {
    return;
  }

  CGameActor* actor;
  if (list->aid == g_session.m_aid) {
    actor = this->scene->player;
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(list->aid));
  }

  if (actor != nullptr) {
    for (int index = 0; index < 5; ++index) {
      actor->SendMsg(nullptr, 109, 0, 232, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  for (int index = 0; index < 5; ++index) {
    if (list->devotees[index] == 0) {
      continue;
    }

    vector3d position{static_cast<float>(list->aid), static_cast<float>(list->devotees[index]), 0.0f};
    if (position.y == position.y && position.x == position.x) {
      if (actor != nullptr) {
        CRagEffect* effect = actor->LaunchEffect(232, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect != nullptr) {
          effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
        }
      }
    } else {
      char message[256];
      sprintf(message, "DEVOTION OVERFLOW : %d -- %d , %d , %d , %d , %d", list->aid, list->devotees[0], list->devotees[1], list->devotees[2],
        list->devotees[3], list->devotees[4]);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0x3232FA, 0, 0, 0);
      sprintf(message, "DEVOTION OVERFLOW : 그라비티에 메일로 문의해주십시요");
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0x3232FA, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D27060
// FUNCTION: fn_21686
void CGameMode::ReceiveActorSignal(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    actor->SendMsg(nullptr, 142, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D2A990
// FUNCTION: fn_21705
void CGameMode::ReceiveFormatStringMessage(const unsigned char* packet) {
  const auto* message = reinterpret_cast<const FormatStringMessagePacket*>(packet);
  const char* format = LookupMessage(message->messageId);

  int count = 0;
  const char* cursor = format;
  while (*cursor != '\0') {
    if (IsDBCSLeadByte(*cursor)) {
      cursor += 2;
    } else if (cursor[0] == '%' && cursor[1] == 's') {
      ++count;
      cursor += 2;
    } else {
      ++cursor;
    }
  }

  char text[261];
  std::memset(text, 0, sizeof(text));

  const char* first = reinterpret_cast<const char*>(message + 1);
  switch (count) {
  case 0:
    _snprintf(text, 260, format);
    break;
  case 1:
    _snprintf(text, 260, format, first);
    break;
  case 2: {
    const char* second = first + std::strlen(first) + 1;
    _snprintf(text, 260, format, first, second);
    break;
  }
  case 3: {
    const char* second = first + std::strlen(first) + 1;
    const char* third = second + std::strlen(second) + 1;
    _snprintf(text, 260, format, first, second, third);
    break;
  }
  default:
    sprintf(text, "unknown argment list");
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFFFF, 0, 0, 0);
}

// OFFSET: 0x00D2AF90
// FUNCTION: fn_21707
void CGameMode::IgnorePacket(const unsigned char*) {
}

// OFFSET: 0x00D2B710
// FUNCTION: fn_21710
void CGameMode::ReceiveItem1738EquipList(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(33));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreEquipRecord);
  const StoreEquipRecord* record = reinterpret_cast<const StoreEquipRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i, ++record) {
    StoreItemOption options[5] = {};
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = (record->flags >> 1) & 1;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    *reinterpret_cast<std::uint32_t*>(item.unknown_0074) = record->sprite;
    *reinterpret_cast<unsigned char*>(item.unknown_0078) = (record->flags >> 2) & 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
    g_session.AddItem1738(item, 1);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2BA50
// FUNCTION: fn_21711
void CGameMode::ReceiveItem1738EquipListOptions(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(33));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreEquipOptionRecord);
  const StoreEquipOptionRecord* record = reinterpret_cast<const StoreEquipOptionRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = (record->flags >> 1) & 1;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    *reinterpret_cast<std::uint32_t*>(item.unknown_0074) = record->sprite;
    *reinterpret_cast<unsigned char*>(item.unknown_0078) = (record->flags >> 2) & 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
    g_session.AddItem1738(item, 1);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2BDF0
// FUNCTION: fn_21712
void CGameMode::ReceiveItem1738NormalList(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(33));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreNormalRecord);
  const StoreNormalRecord* record = reinterpret_cast<const StoreNormalRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[3] = record->location;
    item.unknown_0000[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = 0;
    item.unknown_0060 = 0;
    item.unknown_0068 = record->expireTime;
    item.unknown_0074[0] = (record->flags >> 1) & 1;
    g_session.AddItem1738(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2C190
// FUNCTION: fn_21713
void CGameMode::ReceiveHomunculusProperty64(const unsigned char* data) {
  if (!data) {
    return;
  }

  const HomunculusProperty64Packet* packet = reinterpret_cast<const HomunculusProperty64Packet*>(data);
  switch (packet->type) {
  case 1:
    g_session.m_homunExp = packet->value;
    g_session.RefreshHomunculusInfo();
    if (UIHomunInfoWnd* window = static_cast<UIHomunInfoWnd*>(g_windowMgr.window_03C0)) {
      window->expBar->SetValues(g_session.m_homunExp, g_session.m_homunMaxExp);
    }
    break;
  case 5:
    g_session.m_homunHp = static_cast<int>(packet->value);
    g_session.RefreshHomunculusInfo();
    break;
  case 7:
    g_session.m_homunSp = static_cast<int>(packet->value);
    g_session.RefreshHomunculusInfo();
    break;
  }

  g_session.RefreshHomunculusInfo();
}

// OFFSET: 0x00D2C250
// FUNCTION: fn_21714
void CGameMode::ReceiveHomunculusProperty(const unsigned char* data) {
  const HomunculusPropertyPacket* packet = reinterpret_cast<const HomunculusPropertyPacket*>(data);
  switch (packet->type) {
  case 5:
    g_session.m_homunHp = packet->value;
    g_session.RefreshHomunculusInfo();
    break;
  case 7:
    g_session.m_homunSp = packet->value;
    g_session.RefreshHomunculusInfo();
    break;
  case 1:
    g_session.m_homunExp = packet->value;
    g_session.RefreshHomunculusInfo();
    break;
  }

  g_session.RefreshHomunculusInfo();
}

// OFFSET: 0x00D2C2B0
// FUNCTION: fn_21715
void CGameMode::ReceiveHuntingList(const unsigned char* packet) {
  char text[256];
  int size = *reinterpret_cast<const short*>(packet + 2) - 4;
  if (size > 5000) {
    size = 5000;
  }
  char* list = new char[size + 1];
  std::memcpy(list, packet + 4, size);
  list[size] = '\0';
  std::sprintf(text, "=========== HUNTING LIST =============");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(list), 0xCDFFCD, 0, 0, 0);
  std::sprintf(text, "========================================");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
}

// OFFSET: 0x00D2F150
// FUNCTION: fn_21728
bool CGameMode::NotifyItemResult(int result) {
  int message;
  switch (result) {
  case 1:
  case 6:
    message = 53;
    break;
  case 2:
    message = 52;
    break;
  case 4:
    message = 220;
    break;
  case 5:
    message = 279;
    break;
  case 7:
    message = 1418;
    break;
  case 9:
    message = 3984;
    break;
  default:
    return false;
  }
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
  return true;
}

// OFFSET: 0x00D4B2C0
// FUNCTION: fn_21864
void CGameMode::ReceiveEquipAck(const unsigned char* packet) {
  switch (*reinterpret_cast<const short*>(packet + 2)) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(242)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(243)), 0xFFFF64, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(244)), 0xFFFF64, 0, 0, 0);
    break;
  case 3:
    switch (g_session.GetDisplayJob()) {
    case 12:
    case 4013:
    case 4035:
    case 4059:
    case 4065:
    case 4101:
    case 4254:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1040)), 0xFFFF64, 0, 0, 0);
      break;
    case 24:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1175)), 0xFFFF64, 0, 0, 0);
      break;
    case 25:
    case 4222:
      break;
    default:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(245)), 0xFFFF64, 0, 0, 0);
      break;
    }
    break;
  }
}

// OFFSET: 0x00D58970
// FUNCTION: fn_21955
std::string CGameMode::MakeSimulatorFolder(const std::string& folder) {
  std::string path = g_applicationDirectory + folder;
  if (_access(path.c_str(), 0) != 0 && _mkdir(path.c_str()) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("Create Folder Fail!!! %s", path.c_str()).c_str()), 0xFF, 0, 0, 0);
    return std::string();
  }
  return path;
}

// OFFSET: 0x00D58B20
// FUNCTION: fn_21956
std::FILE* CGameMode::OpenPackageSimulatorFile(const std::string& folder, int itemId, std::string& itemName) {
  std::string directory = this->MakeSimulatorFolder(folder);
  if (directory.size() == 0) {
    return nullptr;
  }

  itemName = FindItemDescription(itemId).identifiedName;
  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string name = FormatOwnedString("%s_%d(%d_%d_%d)", itemName.c_str(), itemId, time.wYear, time.wMonth, time.wDay);
  std::string path = FormatOwnedString("%s\\%s.txt", directory.c_str(), name.c_str());
  int index = 0;
  while (_access(path.c_str(), 0) == 0) {
    ++index;
    path = FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index);
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("Simulator Result File Create %s %d!!!!!!!!!!!!", file ? "success" : "Fail", itemId).c_str()),
                         file ? 0xFFFF : 0xFF, 0, 0, 0);
  return file;
}

// OFFSET: 0x00D58D80
// FUNCTION: fn_21957
std::FILE* CGameMode::OpenSimulatorFile(const std::string& folder, const std::string& name) {
  std::string directory = this->MakeSimulatorFolder(folder);
  if (directory.size() == 0) {
    return nullptr;
  }

  std::string path = FormatOwnedString("%s\\%s.txt", directory.c_str(), name.c_str());
  int index = 0;
  while (_access(path.c_str(), 0) == 0) {
    ++index;
    path = FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index);
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s Simulator File Create %s !!!!!!!!!!!!", name.c_str(), file ? "success" : "Fail").c_str()),
                         file ? 0xFFFF : 0xFF, 0, 0, 0);
  return file;
}

// OFFSET: 0x00D5F830
// FUNCTION: fn_21971
void CGameMode::ReceiveRandomUpgradeSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string name = FormatOwnedString("%d_EnchantUpgradeTest_Slot_%d(%d_%d_%d)", header->value / 10, header->value % 10, time.wYear, time.wMonth, time.wDay);
  std::FILE* file = this->OpenSimulatorFile("\\PackageItemTest", name);
  if (!file) {
    return;
  }

  const EnchantSimulationRecord* record = reinterpret_cast<const EnchantSimulationRecord*>(packet + sizeof(SimulationPacketHeader));
  int total = 0;
  const int count = static_cast<int>((header->length - sizeof(SimulationPacketHeader)) / sizeof(EnchantSimulationRecord));
  std::fprintf(file, "\n- %s\n\n", ToUtf8(name, g_textCodePage).c_str());
  std::fprintf(file, "============================\tSTART\t============================\n");
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.SetItemId(record->itemId);
    item.identified = 1;
    item.unknown_0000[0] = 0x15;
    ITEM_INFO result;
    result.SetItemId(record->resultItemId);
    result.identified = 1;
    result.unknown_0000[0] = 0x15;
    std::fprintf(file, "\t\t%s\t%s\t%d\t\t%.5f%%\n", ToUtf8(item.DisplayName(0), g_textCodePage).c_str(),
                 ToUtf8(result.DisplayName(0), g_textCodePage).c_str(), record->count, static_cast<float>(record->rate) / 1000.0f);
    total += record->count;
    ++record;
  }
  std::fprintf(file, "Try\t%d\tRandomUpgrade\n", total);
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D5FC60
// FUNCTION: fn_21972
void CGameMode::ReceiveEnchantSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string name = FormatOwnedString("%d_EnchantTest_Slot_%d(%d_%d_%d)", header->value / 100, header->value / 10 % 10, time.wYear, time.wMonth, time.wDay);
  std::FILE* file = this->OpenSimulatorFile("\\PackageItemTest", name);
  if (!file) {
    return;
  }

  const int grade = header->value % 10;
  const EnchantSimulationRecord* record = reinterpret_cast<const EnchantSimulationRecord*>(packet + sizeof(SimulationPacketHeader));
  int total = 0;
  const int count = static_cast<int>((header->length - sizeof(SimulationPacketHeader)) / sizeof(EnchantSimulationRecord));
  std::fprintf(file, "\n- %s\n\n", ToUtf8(name, g_textCodePage).c_str());
  std::fprintf(file, "============================\tSTART\t============================\n");
  std::fprintf(file, "\nGrade %d\n", grade);
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.SetItemId(record->itemId);
    item.identified = 1;
    item.unknown_0000[0] = 6;
    std::fprintf(file, "\t\t%s\t%d\t%.5f%%\n", ToUtf8(item.DisplayName(0), g_textCodePage).c_str(), record->count,
                 static_cast<float>(record->rate) / 1000.0f);
    total += record->count;
    ++record;
  }
  std::fprintf(file, "Try\t%d\tSetEnchant\n", total);
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D60030
// FUNCTION: fn_21973
void CGameMode::ReceivePackageSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  std::FILE* file = this->OpenPackageSimulatorFile("\\PackageItemTest", header->value, itemName);
  if (!file) {
    return;
  }

  const char* typeNames[22] = {
    "PACKAGETYPE_NONE",      "PACKAGETYPE_MUST",      "PACKAGETYPE_RANDOM_0",  "PACKAGETYPE_RANDOM_1",  "PACKAGETYPE_RANDOM_2",  "PACKAGETYPE_RANDOM_3",
    "PACKAGETYPE_RANDOM_4",  "PACKAGETYPE_RANDOM_5",  "PACKAGETYPE_RANDOM_6",  "PACKAGETYPE_RANDOM_7",  "PACKAGETYPE_RANDOM_8",  "PACKAGETYPE_RANDOM_9",
    "PACKAGETYPE_NATURAL_0", "PACKAGETYPE_NATURAL_1", "PACKAGETYPE_NATURAL_2", "PACKAGETYPE_NATURAL_3", "PACKAGETYPE_NATURAL_4", "PACKAGETYPE_NATURAL_5",
    "PACKAGETYPE_NATURAL_6", "PACKAGETYPE_NATURAL_7", "PACKAGETYPE_NATURAL_8", "PACKAGETYPE_NATURAL_9",
  };
  const PackageSimulationRecord* record = reinterpret_cast<const PackageSimulationRecord*>(packet + sizeof(SimulationPacketHeader));
  const int count = static_cast<int>((header->length - sizeof(SimulationPacketHeader)) / sizeof(PackageSimulationRecord));
  std::vector<const PackageSimulationRecord*> packages[22];
  for (int i = 0; i < count; ++i) {
    packages[record->packageType].push_back(record);
    ++record;
  }

  std::fprintf(file, "\n--Item Name : %s (%d)\n\n", ToUtf8(itemName, g_textCodePage).c_str(), header->value);
  std::fprintf(file, "============================\tSTART\t============================\n");
  for (int type = 0; type < 22; ++type) {
    if (packages[type].empty()) {
      continue;
    }

    std::fprintf(file, "\n--\t%s\n", typeNames[type]);
    int unpacked = 0;
    for (const PackageSimulationRecord* package : packages[type]) {
      ITEM_INFO item;
      item.SetItemId(package->itemId);
      item.unknown_0088 = package->refine;
      item.identified = 1;
      item.unknown_0060 = package->unknown_0006;
      if (package->refine != 0 || package->unknown_0006 != 0) {
        item.unknown_0000[0] = 0x15;
      }
      std::fprintf(file, "\t%-24s\t\t\t\t\t\t\t%d\t(%d)\t%d/%d\t%s\n", ToUtf8(item.DisplayName(0), g_textCodePage).c_str(), package->count,
                   package->unknown_000A, package->unknown_0026, package->unknown_002A, ToUtf8(std::string(package->text), g_textCodePage).c_str());
      unpacked += package->count;
    }
    std::fprintf(file, "\n\t\t\t\t\tUnpacked\t%d\tPackages\n", unpacked);
  }
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D612A0
// FUNCTION: fn_21979
void CGameMode::ReceiveUnknownD612A0(const unsigned char* packet) {
  UIWindow* window = g_windowMgr.EnsureWindow(267);
  if (!window) {
    return;
  }

  window->SendMsg(
    nullptr, 34, packet[2] != 0, *reinterpret_cast<const int*>(packet + 4), *reinterpret_cast<const int*>(packet + 8), static_cast<signed char>(packet[3])
  );
}

// OFFSET: 0x00D77BF0
// FUNCTION: fn_22152
void CGameMode::ReceiveUnknownD77BF0(const unsigned char* packet) {
  if (packet[2] == 1) {
    if (!g_windowMgr.unknown_0450) {
      g_windowMgr.EnsureWindow(262);
    }

    if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
      window->SendMsg(nullptr, 6, 286, 476, 1, 0);
    }
    return;
  }

  g_windowMgr.CloseWindow(262);
}

// OFFSET: 0x00D8A300
// FUNCTION: fn_22218
void CGameMode::ReceiveHpSpRecovery(const unsigned char* packet, int variant) {
  CPlayer* player = this->scene->player;
  int amount;
  std::int16_t type;
  switch (variant) {
  case 0:
    amount = *reinterpret_cast<const std::int16_t*>(packet + 4);
    type = *reinterpret_cast<const std::int16_t*>(packet + 2);
    break;
  case 1:
    amount = *reinterpret_cast<const int*>(packet + 4);
    type = *reinterpret_cast<const std::int16_t*>(packet + 2);
    break;
  }

  switch (type) {
  case 5: {
    g_session.UpdateRandomStats();
    const int hp = g_session.unknown_5568 + amount;
    if (g_session.unknown_5568 != hp) {
      g_session.unknown_5568 = hp;
      g_session.RefreshHpSpWindows();
    }
    player->SendMsg(nullptr, 88, 0, amount, amount >> 31, static_cast<int>(0xFF00FF00), 0, 114, 0, 0, 0, 0, 0, 0, 0);
    player->LaunchEffect(331, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  case 7: {
    g_session.UpdateRandomStats();
    int& sp = *reinterpret_cast<int*>(&g_session.unknown_5570[0]);
    const int value = sp + amount;
    if (sp != value) {
      sp = value;
      g_session.RefreshHpSpWindows();
    }
    player->SendMsg(nullptr, 88, 0, amount, amount >> 31, static_cast<int>(0xFF0000FF), 0, 114, 0, 0, 0, 0, 0, 0, 0);
    player->LaunchEffect(332, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  }
}

// GLOBAL: gl_24965
int g_unknown173A5BC;

// OFFSET: 0x00D50580
// FUNCTION: fn_21884
void CGameMode::IgnoreRemovedPacket7(const unsigned char*) {
}

// OFFSET: 0x00D79420
// FUNCTION: fn_22167
void CGameMode::ReceiveUnknown173A5BC(const unsigned char* packet) {
  const int value = *reinterpret_cast<const int*>(packet + 2);
  if (value > 0) {
    g_unknown173A5BC = value;
  }
}

// GLOBAL: gl_24967
int g_unknown16FC588;

// OFFSET: 0x00D7A0B0
// FUNCTION: fn_22186
void CGameMode::ReceiveUnknown16FC588(const unsigned char* packet) {
  const signed char value = *reinterpret_cast<const signed char*>(packet + 2);
  if (value != 1 && value != 2) {
    g_unknown16FC588 = 0;
  }
}

// OFFSET: 0x00D08370
// FUNCTION: fn_21586
void CGameMode::IgnoreRemovedPacket8() {
}

// OFFSET: 0x00D08380
// FUNCTION: fn_21587
void CGameMode::IgnoreRemovedPacket9() {
}

// OFFSET: 0x00D088F0
// FUNCTION: fn_21593
void CGameMode::IgnoreRemovedPacket10(const unsigned char*) {
}

// OFFSET: 0x00D08900
// FUNCTION: fn_21594
void CGameMode::IgnoreRemovedPacket11(const unsigned char*) {
}

// OFFSET: 0x00D89BD0
// FUNCTION: fn_22211
void CGameMode::ReceiveCharViewResult(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
  case 0: {
    CMode* mode = g_modeMgr.gameMode;
    CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    if (player->render.unknown_0070 != 0 && mode != nullptr) {
      mode->SendMsg(137, 3, player->unknown_051C, 0, 0);
    }
    return;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3009)), 0xFF, 0, 0, 0);
    return;
  default:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3040)), 0xFF, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x00D89CA0
// FUNCTION: fn_22213
void CGameMode::ResetItemLists(const unsigned char*) {
  g_session.ResetItems61B8();
  g_session.ResetItems6B68();
}

// OFFSET: 0x00D8A1F0
// FUNCTION: fn_22216
void CGameMode::ClearItemLists(const unsigned char* packet) {
  const std::uint16_t slot = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  g_session.ClearUnknown1710(slot);
  this->unknown_05CE = 1;
  RefreshItemWindows(&g_windowMgr);
  g_session.ClearUnknown1740(slot);
  g_session.RefreshItem1740Window();
  g_session.ClearUnknown1738(slot);
  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D31E50
// FUNCTION: fn_21736
void CGameMode::ReceiveMemorialDungeonInfo(const unsigned char* packet) {
  g_session.unknown_5950 = reinterpret_cast<const char*>(packet + 2);
  g_session.unknown_596C = *reinterpret_cast<const int*>(packet + 63);
  g_session.unknown_5970 = *reinterpret_cast<const int*>(packet + 67);
  const int previous = g_session.unknown_5948;
  const int state = *reinterpret_cast<const int*>(packet + 67) != 0 ? 1 : 2;
  g_session.unknown_5948 = state;
  if (previous != state || state != 2 || g_session.unknown_593C != 0) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(137)) {
      window->SendMsg(nullptr, 60, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D31EE0
// FUNCTION: fn_21737
void CGameMode::ReceiveMemorialDungeonNotify(const unsigned char* packet) {
  const int previous = g_session.unknown_5948;
  g_session.unknown_5970 = *reinterpret_cast<const int*>(packet + 6);
  const int time = *reinterpret_cast<const int*>(packet + 6);
  g_session.unknown_5948 = time != 0 ? 1 : 2;
  if (*reinterpret_cast<const int*>(packet + 2) == 0) {
    if (previous != (time != 0 ? 1 : 2) || time != 0 || g_session.unknown_593C != 0) {
      if (UIWindow* window = g_windowMgr.EnsureWindow(137)) {
        window->SendMsg(nullptr, 60, 0, 0, 0, 0);
      }
    }
    return;
  }

  g_windowMgr.CloseWindow(137);
  const int type = *reinterpret_cast<const int*>(packet + 2);
  if (type == 1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1346)), 0x64FFFF, 0, 0, 0);
  } else if (type == 2) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1347)), 0x64FFFF, 0, 0, 0);
  } else if (type == 3) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1348)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D31FD0
// FUNCTION: fn_21738
void CGameMode::ReceiveMemorialDungeonSubscriptionInfo(const unsigned char* packet) {
  g_session.unknown_5950 = reinterpret_cast<const char*>(packet + 2);
  g_session.unknown_5968 = *reinterpret_cast<const short*>(packet + 63);
  g_session.unknown_5948 = 0;
  if (UIWindow* window = g_windowMgr.EnsureWindow(137)) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D32040
// FUNCTION: fn_21739
void CGameMode::ReceiveMemorialDungeonSubscriptionNotify(const unsigned char* packet) {
  g_session.unknown_5968 = *reinterpret_cast<const short*>(packet + 2);
  UIWindow* window = g_windowMgr.EnsureWindow(137);
  if (!window) {
    return;
  }

  if (*reinterpret_cast<const short*>(packet + 2) >= 0) {
    g_session.unknown_5948 = 0;
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.CloseWindow(137);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1345)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D320D0
// FUNCTION: fn_21740
void CGameMode::ReceiveWindow17Entries(const unsigned char* packet) {
  this->unknown_024C = 1;
  g_windowMgr.CloseWindow(17);
  if (UIWindow* window = g_windowMgr.EnsureWindow(17)) {
    std::string text;
    text.assign(reinterpret_cast<const char*>(packet + 8), *reinterpret_cast<const unsigned short*>(packet + 2) - 9);
    const std::vector<std::string> entries = Split(text, ":");
    for (const std::string& entry : entries) {
      if (!entry.empty()) {
        window->SendMsg(nullptr, 27, reinterpret_cast<std::intptr_t>(entry.c_str()), 0, 0, 0);
      }
    }
    window->SendMsg(nullptr, 28, *reinterpret_cast<const int*>(packet + 4), 0, 0, 0);
  }

  if (g_windowMgr.unknown_01F0) {
    g_windowMgr.unknown_01F0->SendMsg(nullptr, 32, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D32820
// FUNCTION: fn_21744
void CGameMode::IgnorePacket473(const unsigned char*) {
}

// OFFSET: 0x00D33970
// FUNCTION: fn_21750
void CGameMode::IgnorePacket474(const unsigned char*) {
}

// OFFSET: 0x00D34040
// FUNCTION: fn_21753
void CGameMode::Zc_Notify_Exp(const unsigned char* packet) {
  const NotifyExpPacket* data = reinterpret_cast<const NotifyExpPacket*>(packet);
  std::string text;

  if (g_session.m_aid != data->accountId) {
    return;
  }

  const std::uint32_t color = data->expType != 1 ? 0xFFFF : 0xBE58BE;
  const int amount = data->amount;
  if (amount == 0) {
    return;
  }

  char buffer[256];
  if (data->varId == 1) {
    std::sprintf(buffer, LookupMessage(1613), amount);
    text += buffer;
  } else if (data->varId == 2) {
    std::sprintf(buffer, LookupMessage(1614), amount);
    text += buffer;
  }

  if (data->amount >= 0) {
    text += LookupMessage(1615);
  } else {
    text += LookupMessage(1616);
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), color, 18, 0, 0);
}

// OFFSET: 0x00D61290
// FUNCTION: fn_21978
void CGameMode::IgnorePacket478(const unsigned char*) {
}

// OFFSET: 0x00D3A1D0
// FUNCTION: fn_21775
void CGameMode::ClearUnknown0244(const unsigned char*) {
  this->unknown_0244 = 0;
}

#pragma pack(push, 1)
struct ZoneServerChangePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char mapName[16];
  /* 0012 */ std::int16_t x;
  /* 0014 */ std::int16_t y;
  /* 0016 */ std::uint32_t ip;
  /* 001A */ std::int16_t port;
};
#pragma pack(pop)

// OFFSET: 0x00D3A4C0
// FUNCTION: fn_21777
void CGameMode::ReceiveZoneServerChange(const unsigned char* packet) {
  const auto* change = reinterpret_cast<const ZoneServerChangePacket*>(packet);
  const std::string mapName(change->mapName, change->mapName + sizeof(change->mapName));
  this->ChangeZoneServer(mapName.c_str(), change->x, change->y, static_cast<int>(change->ip), change->port, std::string());
}

// OFFSET: 0x00D79F00
// FUNCTION: fn_22184
void CGameMode::ReceiveZoneServerChange(const char* mapName, std::int16_t x, std::int16_t y, int ip, std::int16_t port) {
  g_session.m_localMapMove = 1;
  this->ChangeZoneServer(mapName, x, y, ip, port, std::string());
}

// OFFSET: 0x00D272A0
// FUNCTION: fn_21688
void CGameMode::ReceiveActorStatusBlock(const unsigned char* packet) {
  g_session.SetUnknown5BC0(*reinterpret_cast<const int*>(packet + 2));
  g_session.unknown_5BC4 = *reinterpret_cast<const int*>(packet + 6);
  g_session.unknown_5BC8 = *reinterpret_cast<const int*>(packet + 0xE);
  g_session.unknown_5BCC = *reinterpret_cast<const int*>(packet + 0xA);
  g_session.unknown_5BD0 = *reinterpret_cast<const int*>(packet + 0x12);
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    actor->SendMsg(nullptr, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D27320
// FUNCTION: fn_21689
void CGameMode::ReceiveActorStatusUpdate(const unsigned char* packet) {
  if (g_session.unknown_5BC0 != -1) {
    const unsigned int field = *reinterpret_cast<const std::uint16_t*>(packet + 2) - 5;
    switch (field) {
    case 0:
      g_session.unknown_5BC4 = *reinterpret_cast<const int*>(packet + 4);
      break;
    case 1:
      g_session.unknown_5BCC = *reinterpret_cast<const int*>(packet + 4);
      break;
    case 2:
      g_session.unknown_5BC8 = *reinterpret_cast<const int*>(packet + 4);
      break;
    case 3:
      g_session.unknown_5BD0 = *reinterpret_cast<const int*>(packet + 4);
      break;
    }
    if (CRenderObject* actor = this->scene->GetGameActorByAID(g_session.unknown_5BC0)) {
      actor->SendMsg(nullptr, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D51A90
// FUNCTION: fn_21890
void CGameMode::ReceiveActorTitle(const unsigned char* packet) {
  char text[81];
  std::strncpy(text, reinterpret_cast<const char*>(packet + 6), 80);
  text[80] = '\0';
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(text);
    actor->SendMsg(nullptr, 0x37, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D61340
// FUNCTION: fn_21981
void CGameMode::ReceiveActorInt8(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t value = *reinterpret_cast<const signed char*>(packet + 6);
    actor->SendMsg(nullptr, 0x91, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0x14, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D612F0
// FUNCTION: fn_21980
void CGameMode::ReceiveActorInt64Pair(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t first = *reinterpret_cast<const int*>(packet + 6);
    const std::int64_t second = *reinterpret_cast<const int*>(packet + 0xA);
    actor->SendMsg(
      nullptr, 0x91, 0, static_cast<int>(first), static_cast<int>(first >> 32), static_cast<int>(second), static_cast<int>(second >> 32), 0, 0, 0, 0, 0, 0, 0, 0
    );
  }
}

// OFFSET: 0x00D24C40
// FUNCTION: fn_21670
void CGameMode::ReceiveItem1740EquipList(const unsigned char* packet) {
  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(StoreEquipRecord);
  const StoreEquipRecord* record = reinterpret_cast<const StoreEquipRecord*>(packet + 4);
  for (int i = 0; i < count; ++i, ++record) {
    StoreItemOption options[5] = {};
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = (record->flags >> 1) & 1;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    item.unknown_0070 = record->sprite;
    item.unknown_0074[0] = (record->flags >> 2) & 1;
    item.unknown_0088 = 0;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
    g_session.AddItem1740(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D24EF0
// FUNCTION: fn_21671
void CGameMode::ReceiveItem1740EquipListOptions(const unsigned char* packet) {
  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(StoreEquipOptionRecord);
  const StoreEquipOptionRecord* record = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 4);
  for (int i = 0; i < count; ++i, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = (record->flags >> 1) & 1;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    item.unknown_0070 = record->sprite;
    item.unknown_0074[0] = (record->flags >> 2) & 1;
    item.unknown_0088 = 0;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
    g_session.AddItem1740(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D26800
// FUNCTION: fn_21682
void CGameMode::ReceiveUnknownD26800(const unsigned char* packet) {
  const unsigned char value = packet[2];
  *reinterpret_cast<int*>(g_session.unknown_5974) = value;
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(value != 0 ? 0x54F : 0x54E)), 0x64FFFF, 0, 0, 0);
  if (g_windowMgr.unknown_01E0) {
    g_windowMgr.unknown_01E0->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D79660
// FUNCTION: fn_22172
void CGameMode::ReceiveUnknownD79660(const unsigned char* packet) {
  UIWindow* window = g_windowMgr.LookupWindow(10032);
  if (!window) {
    return;
  }

  switch (*reinterpret_cast<const std::int16_t*>(packet + 6) - 1) {
  case 0:
    window->Show(0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x10E8)), 0xFF, 0, 0, 0);
    return;
  case 1:
    g_windowMgr.CloseWindow(10032);
    return;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x79B)), 0xFF, 0, 0, 0);
    return;
  case 3:
    window->Show(1);
    return;
  }
}

// OFFSET: 0x00D79E90
// FUNCTION: fn_22183
void CGameMode::ReceiveUnknownD79E90(std::int16_t result, std::intptr_t value) {
  switch (result) {
  case 0:
    if (UIWindow* window = g_windowMgr.LookupWindow(340)) {
      window->SendMsg(nullptr, 0x40, 0, value, 0, 0);
    }
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x9D1)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D79440
// FUNCTION: fn_22168
void CGameMode::ReceiveRouletteSetResult(const unsigned char* packet) {
  switch (packet[2]) {
  case 0:
    g_rouletteMgr->SetAdditionItem(*reinterpret_cast<const int*>(packet + 3));
    g_rouletteMgr->SetResult(0, 0, 0);
    g_rouletteMgr->Reset();
    if (g_windowMgr.unknown_0464) {
      g_windowMgr.RefreshRouletteWnd();
    }
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA4C)), 0x6464FF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA4D)), 0x6464FF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xA4E)), 0x6464FF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D611B0
// FUNCTION: fn_21977
void CGameMode::ReceiveDynamicNpcNotice(const unsigned char* packet) {
  char text[0x400];
  const char* message;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 1:
    std::memset(text, 0, sizeof(text));
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "dynamic npc create error found(unknown) ");
    message = text;
    break;
  case 2:
    std::memset(text, 0, sizeof(text));
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "dynamic npc not found ");
    message = text;
    break;
  case 3:
    message = LookupMessage(0xA47);
    break;
  case 4:
    message = LookupMessage(0xFA2);
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D079A0
// FUNCTION: fn_21584
void CGameMode::sub_D079A0(unsigned char kind, int argument) {
  std::string& speaker = this->unknown_0214;
  std::string& target = this->unknown_022C;

  switch (kind) {
  case 0: {
    std::string line(g_session.DecodePlayerName());
    line.append(" : ", 3);
    line.append(speaker.data(), speaker.size());
    if (g_windowMgr.ShowWhisperFrom(target.c_str(), line.c_str(), 0x7800, static_cast<std::uint32_t>(argument)) != 0) {
      return;
    }

    const std::string copied = target;
    const std::string identity = ShortcutFilename(static_cast<std::uint32_t>(argument));
    std::string message;
    if (g_windowMgr.IsFriend(target.c_str()) || g_session.IsGuildMemberName(target.c_str())) {
      message = std::string("[ To ") + copied + " (" + identity + ") ] : " + speaker;
    } else {
      message = std::string("( To ") + copied + " (" + identity + ") : " + speaker;
    }

    if (g_pendingWhisperCount <= 0) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF, 2, reinterpret_cast<std::intptr_t>(copied.c_str()), 0);
    } else {
      --g_pendingWhisperCount;
    }
    if (g_whisperRecord != 0) {
      this->WriteChatLog(message.c_str());
    }
    return;
  }
  case 1:
  case 2:
  case 3: {
    const int messageId = kind == 1 ? 0x94 : kind == 2 ? 0x95 : 0x96;
    const std::string text = std::string("(") + target + ")" + LookupMessage(messageId);
    if (kind == 1 && g_pendingWhisperCount > 0) {
      --g_pendingWhisperCount;
      return;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 2, 0, 0);
    return;
  }
  default: {
    const std::string text = std::string("(") + target + ")" + LookupMessage(0x94);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 2, 0, 0);
    if (g_whisperRecord != 0) {
      this->WriteChatLog(text.c_str());
    }
    return;
  }
  }
}

// OFFSET: 0x00D08390
// FUNCTION: fn_21588
void CGameMode::sub_D08390(const unsigned char* packet) {
  const int x = *reinterpret_cast<const std::int16_t*>(packet + 0x12);
  const int y = *reinterpret_cast<const std::int16_t*>(packet + 0x14);
  const int ip = *reinterpret_cast<const int*>(packet + 0x16);
  const int port = *reinterpret_cast<const std::int16_t*>(packet + 0x1A);
  const std::string host(reinterpret_cast<const char*>(packet + 0x1C), reinterpret_cast<const char*>(packet + 0x9C));
  const std::string mapName(reinterpret_cast<const char*>(packet + 2), reinterpret_cast<const char*>(packet + 0x12));
  this->ChangeZoneServer(mapName.c_str(), x, y, ip, port, host.c_str());
}

// OFFSET: 0x00D08110
// FUNCTION: fn_21585
void __stdcall sub_D08110(int field8, int itemIdWord, int quantityWord, int field14, int field18) {
  const int itemId = static_cast<short>(itemIdWord);
  const int quantity = static_cast<short>(quantityWord);
  ITEM_INFO item = g_session.FindItem1758(static_cast<unsigned int>(itemId));
  item.unknown_0000[1] = static_cast<std::uint32_t>(itemId);
  item.quantity = quantity;
  item.unknown_0014[0] = static_cast<std::uint32_t>(field14);
  UIWindow* window = g_windowMgr.EnsureWindow(0x101);
  window->SendMsg(nullptr, 0x17, reinterpret_cast<std::intptr_t>(&item), field18, field8, 0);
  g_session.RemoveItem1758(item);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(
    1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xE7), item.DisplayName(0).c_str(), quantity).c_str()), 0xFFFF64, 0, 0, 0
  );
  if (g_windowMgr.unknown_02F4 != nullptr) {
    g_windowMgr.unknown_02F4->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (g_session.GetList9Size() == 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x137)), 0xFF00, 0, 0, 0);
    if (g_windowMgr.unknown_0304 != nullptr) {
      g_windowMgr.unknown_0304->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
      g_windowMgr.unknown_0304->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
    }
  } else if (g_windowMgr.unknown_0304 != nullptr) {
    g_windowMgr.unknown_0304->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (g_windowMgr.unknown_0304 != nullptr && g_session.m_muteSellSound) {
    g_soundMgr->PlayWave("effect\\ef_steal.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x00D08670
// FUNCTION: fn_21592
void __declspec(noinline) __stdcall sub_D08670(int field14, int itemId, int quantity, int field8, int fieldC, int field10) {
  ITEM_INFO item = g_session.FindItem1758ByItemId(itemId);
  item.SetItemId(itemId);
  item.quantity = quantity;
  item.unknown_0014[0] = static_cast<std::uint32_t>(field8);
  UIWindow* window = g_windowMgr.EnsureWindow(0x102);
  window->SendMsg(nullptr, 0x17, reinterpret_cast<std::intptr_t>(&item), field10, field14, 0);
  g_session.RemoveItem1758ByItemId(item);
  if (g_windowMgr.unknown_01D4 != nullptr) {
    g_windowMgr.unknown_01D4->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (g_windowMgr.unknown_0414 != nullptr) {
    g_windowMgr.unknown_0414->SendMsg(nullptr, 0x77, fieldC, 0, 0, 0);
    g_windowMgr.unknown_0414->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
    if (g_session.m_muteSellSound) {
      g_soundMgr->PlayWave("effect\\ef_steal.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    if (g_session.GetList9Size() == 0) {
      g_windowMgr.unknown_0414->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D08640
// FUNCTION: fn_21591
void __stdcall sub_D08640(const unsigned char* packet) {
  sub_D08670(
    *reinterpret_cast<const int*>(packet + 0x14),
    *reinterpret_cast<const int*>(packet + 2),
    *reinterpret_cast<const short*>(packet + 6),
    *reinterpret_cast<const int*>(packet + 8),
    *reinterpret_cast<const int*>(packet + 0xC),
    *reinterpret_cast<const int*>(packet + 0x10));
}

// OFFSET: 0x00D246B0
// FUNCTION: fn_21668
void CGameMode::ReceiveItem1740BasicList(const unsigned char* data) {
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(Item1740Record);
  const Item1740Record* record = reinterpret_cast<const Item1740Record*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.unknown_005D = record->unknown_000C;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    g_session.AddItem1740(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D24810
// FUNCTION: fn_21669
void CGameMode::ReceiveItem1740BasicListBind(const unsigned char* data) {
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(Item1740EquipRecord);
  const Item1740EquipRecord* record = reinterpret_cast<const Item1740EquipRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.unknown_005D = record->unknown_000C;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    item.unknown_0070 = record->sprite;
    g_session.AddItem1740(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D2D770
// FUNCTION: fn_21720
void CGameMode::ReceiveItemInventoryList(const unsigned char* data) {
  g_session.KeepEquipment();

  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(StoreNormalRecord);
  const StoreNormalRecord* record = reinterpret_cast<const StoreNormalRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[3] = record->location;
    item.unknown_0000[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = 0;
    item.unknown_0060 = 0;
    item.unknown_0068 = record->expireTime;
    item.unknown_0074[0] = (record->flags >> 1) & 1;
    g_session.UpdateInventoryItem(item, 0);
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D64930
// FUNCTION: fn_21994
void __stdcall Nullsub479(int value) {
}

// OFFSET: 0x00D64A40
// FUNCTION: fn_21996
int sub_D64A40(const unsigned char* entries) {
  int count = 0;
  if (*reinterpret_cast<const std::uint16_t*>(entries) != 0) {
    ++count;
  }
  if (*reinterpret_cast<const std::uint16_t*>(entries + 5) != 0) {
    ++count;
  }
  if (*reinterpret_cast<const std::uint16_t*>(entries + 10) != 0) {
    ++count;
  }
  if (*reinterpret_cast<const std::uint16_t*>(entries + 15) != 0) {
    ++count;
  }
  if (*reinterpret_cast<const std::uint16_t*>(entries + 20) != 0) {
    ++count;
  }
  return count;
}

// OFFSET: 0x00D77C60
// FUNCTION: fn_22153
void CGameMode::ReceivePartyJobLevel(const unsigned char* packet) {
  g_session.SetPartyJobLevel(
    *reinterpret_cast<const std::uint32_t*>(packet + 2), *reinterpret_cast<const std::uint16_t*>(packet + 6),
    *reinterpret_cast<const std::uint16_t*>(packet + 8)
  );
  sub_DFF150(&g_session);
}

// OFFSET: 0x00D8A260
// FUNCTION: fn_22217
void CGameMode::ReceivePartyDead(const unsigned char* packet) {
  MessengerMember member = g_session.PartyByAccount(*reinterpret_cast<const std::uint32_t*>(packet + 2));
  if (member.valid != 0) {
    g_session.SetPartyDead(*reinterpret_cast<const std::uint32_t*>(packet + 2), packet[6]);
    sub_DFF150(&g_session);
  }
}

// OFFSET: 0x00D5AB80
// FUNCTION: fn_21962
void CGameMode::ReceiveShopItemList(
  const std::vector<ITEM_INFO>& items, int value, int unused1, int argument, int unused2, std::uint32_t duration, unsigned char myShop
) {
  CSession* lists = &g_session;
  if (myShop == 1) {
    lists->ClearItems1758();
    for (ITEM_INFO item : items) {
      lists->AddItem1758(item);
    }

    if (UIMerchantItemMyShopWnd* window = static_cast<UIMerchantItemMyShopWnd*>(g_windowMgr.EnsureWindow(45))) {
      window->OpenShop();
      window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
      window->SendMsg(nullptr, 0x1C, value, 0, 0, 0);
      window->SetDuration(duration);
    }
  } else {
    lists->ClearItems1748();
    lists->ClearItems1760();
    for (ITEM_INFO item : items) {
      lists->AddItem1748(item);
    }

    if (UIMerchantItemShopWnd* window = static_cast<UIMerchantItemShopWnd*>(g_windowMgr.EnsureWindow(43))) {
      window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
      window->SendMsg(nullptr, 0x1C, value, 0, 0, 0);
      window->StartTimer(duration);
    }

    if (UIWindow* window = g_windowMgr.EnsureWindow(44)) {
      window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
      window->SendMsg(nullptr, 0x1C, value, argument, 0, 0);
    }
  }

  this->unknown_024C = 1;
}

// OFFSET: 0x00D79AF0
// FUNCTION: fn_22177
void CGameMode::ReceivePacket0B2E(short result) {
  switch (result) {
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x442)), 0xff, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xe31)), 0xff, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D2A1F0
// FUNCTION: fn_21702
void CGameMode::ReceiveUnknownD2A1F0(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 3:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x6CF)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x6D0)), 0xFF, 0, 0, 0);
    break;
  }

  this->SendMsg(0x28, 0, 0, 0, 0);
  g_windowMgr.CloseWindow(0xB0);
}

// OFFSET: 0x00D7A0E0
// FUNCTION: fn_22187
void CGameMode::ReceiveActorMessage7(const unsigned char* packet) {
  const std::int16_t length = *reinterpret_cast<const std::int16_t*>(packet + 2) - 8;
  if (length == 0) {
    return;
  }

  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  CRenderObject* actor = aid == g_session.m_aid ? this->scene->player : this->scene->GetGameActorByAID(aid);
  if (!actor) {
    return;
  }

  char text[256];
  std::memcpy(text, packet + 8, length);
  const std::int64_t value = reinterpret_cast<std::intptr_t>(text);
  actor->SendMsg(
    nullptr, 7, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  );
}
// OFFSET: 0x00D7AEB0
// FUNCTION: fn_22194
void CGameMode::Zc_Start_Baby(const unsigned char*) {
  this->unknown_040C = 20001;
  this->unknown_0410 = 5;
  this->unknown_0414 = 0;
  this->unknown_0408 = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AEE0
// FUNCTION: fn_22195
void CGameMode::Zc_Start_Capture(const unsigned char*) {
  this->unknown_040C = 10000;
  this->unknown_0410 = 5;
  this->unknown_0414 = 0;
  this->unknown_0408 = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AF10
// FUNCTION: fn_22196
void CGameMode::Zc_Start_Collection(const unsigned char*) {
  this->unknown_040C = 40000;
  this->unknown_0410 = 1;
  this->unknown_0414 = 0;
  this->unknown_0408 = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AF40
// FUNCTION: fn_22197
void CGameMode::Zc_Start_Couple(const unsigned char*) {
  this->unknown_040C = 20000;
  this->unknown_0410 = 5;
  this->unknown_0414 = 0;
  this->unknown_0408 = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D591A0
// FUNCTION: fn_21959
void CGameMode::sub_D591A0(std::uint32_t aid, int state, int flag, int options) {
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (!actor) {
    return;
  }

  actor->unknown_0210 = static_cast<std::uint16_t>(flag);
  if (!StateFlag06(state)) {
    actor->sub_CAA4D0();
  }

  if (actor->IsPlayerClass(actor->GetEffectiveJob(-1))) {
    if (StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
      actor->SendMsg(nullptr, 109, 0, 203, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, 109, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, 109, 0, 1132, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, 109, 0, 418, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, 109, 0, 586, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      return;
    }

    if (options & 0x1) {
      if (!actor->HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 130, 0, 0, 0);
        }
      }
    } else if (options & 0x2) {
      if (!actor->HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 128, 0, 0, 0);
        }
      }
    } else if (options & 0x4) {
      if (!actor->HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 169, 0, 0, 0);
        }
      }
    } else if (options & 0x4000) {
      if (!actor->HasId(617)) {
        actor->LaunchEffect(617, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }

    if ((options & 0x8) && !actor->HasId(203)) {
      actor->LaunchEffect(203, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x10) && !actor->HasId(254)) {
      actor->LaunchEffect(254, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      actor->LaunchEffect(1132, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x1000) && !actor->HasId(418)) {
      actor->LaunchEffect(418, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      actor->LaunchEffect(544, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x2000) && !actor->HasId(419)) {
      actor->LaunchEffect(419, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x8000) && !actor->HasId(424)) {
      actor->LaunchEffect(424, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x20000) && !actor->HasId(586)) {
      actor->LaunchEffect(586, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x10000) && !actor->HasId(655)) {
      actor->LaunchEffect(655, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x40) && !actor->HasId(474)) {
      actor->LaunchEffect(474, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x100) && !actor->HasId(382)) {
      actor->LaunchEffect(382, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x80) && !actor->HasId(368)) {
      actor->LaunchEffect(368, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x200) && !actor->HasId(394)) {
      actor->LaunchEffect(394, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x400) && !actor->HasId(396)) {
      actor->LaunchEffect(396, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }

    if (options & 0x20) {
      switch (actor->GetEffectiveJob(-1)) {
      case 15:
      case 4016:
      case 4038:
      case 4070:
      case 4077:
      case 4106:
      case 4262:
        actor->SetFrameSequence(96, 4, 4, 96, 4, 1, 99999999, 0);
        break;
      default:
        actor->render.isMotionFreezed = true;
        break;
      }
      actor->render.unknown_00A3 = 1;
      actor->SetColor(-1, 64, 64, 64);
    }

    if ((options & 0x800) && !actor->HasId(375)) {
      actor->LaunchEffect(375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x40000) && !actor->HasId(744)) {
      actor->LaunchEffect(744, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    return;
  }

  if (StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
    actor->SendMsg(nullptr, 109, 0, 203, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, 109, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, 109, 0, 1132, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, 109, 0, 418, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return;
  }

  if (options & 0x20) {
    actor->render.isMotionFreezed = true;
    actor->render.unknown_00A3 = 1;
    actor->SetColor(-1, 64, 64, 64);
  }

  if (options & 0x2) {
    if (!actor->HasId(420)) {
      actor->LaunchEffect(420, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(420)) {
      actor->SendMsg(nullptr, 109, 0, 420, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->LaunchEffect(540, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (options & 0x4) {
    if (!actor->HasId(427)) {
      actor->LaunchEffect(427, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(427)) {
      actor->SendMsg(nullptr, 109, 0, 427, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x10) {
    if (!actor->HasId(254)) {
      actor->LaunchEffect(254, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      actor->LaunchEffect(1132, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(254)) {
      actor->SendMsg(nullptr, 109, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, 109, 0, 1132, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x10000) {
    if (!actor->HasId(655)) {
      actor->LaunchEffect(655, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(655)) {
      actor->SendMsg(nullptr, 109, 0, 655, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->LaunchEffect(656, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (options & 0x8000) {
    if (!actor->HasId(667)) {
      actor->LaunchEffect(667, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(667)) {
      actor->SendMsg(nullptr, 109, 0, 667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x800) {
    if (!actor->HasId(375)) {
      actor->LaunchEffect(375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(375)) {
      actor->SendMsg(nullptr, 109, 0, 375, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x40000) {
    if (!actor->HasId(744)) {
      actor->LaunchEffect(744, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(744)) {
      actor->SendMsg(nullptr, 109, 0, 744, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D7B350
// FUNCTION: fn_22201
void CGameMode::ReceivePacket0AB8(const unsigned char*) {
  this->scene->player->SendMsg(nullptr, 149, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D78010
// FUNCTION: fn_22156
void CGameMode::ReceiveUnknownD78010(const unsigned char* packet) {
  this->sub_D591A0(
    *reinterpret_cast<const std::uint32_t*>(packet + 2),
    *reinterpret_cast<const int*>(packet + 6),
    *reinterpret_cast<const int*>(packet + 0xA),
    *reinterpret_cast<const int*>(packet + 0xE)
  );
}

// OFFSET: 0x00D89C60
// FUNCTION: fn_22212
void CGameMode::ReceiveUnknownD89C60(const unsigned char* packet) {
  this->sub_D5EEF0(
    reinterpret_cast<const char*>(packet + 0x2B),
    *reinterpret_cast<const int*>(packet + 2),
    reinterpret_cast<const char*>(packet + 0x43),
    *reinterpret_cast<const int*>(packet + 6),
    packet[0x12],
    *reinterpret_cast<const std::uint16_t*>(packet + 0xA),
    *reinterpret_cast<const std::uint16_t*>(packet + 0xC),
    reinterpret_cast<const char*>(packet + 0x13),
    packet[0x53],
    packet[0x54],
    0
  );
}

// OFFSET: 0x00D79390
// FUNCTION: fn_22165
void CGameMode::ReceiveLapineBoxIngredient(const unsigned char* packet) {
  if (g_windowMgr.LookupWindow(0x122) != nullptr) {
    return;
  }

  if (UIWindow* window = g_windowMgr.sub_A978D0(0x122)) {
    static_cast<UILapineBoxWnd*>(window)->SetIngredient(static_cast<int>(reinterpret_cast<std::intptr_t>(packet)));
  }
}

// OFFSET: 0x00D79B40
// FUNCTION: fn_22178
void CGameMode::ActivateMacroBlackListCheck(unsigned char state, int duration) {
  if (UIWindow* window = g_windowMgr.LookupWindow(0x167)) {
    static_cast<UIMacroBlackListCheckWnd*>(window)->Activate(state, duration);
  }
}

// OFFSET: 0x00D79B70
// FUNCTION: fn_22179
void CGameMode::SetMacroDetectorCountdown(unsigned char count, std::uint32_t duration) {
  if (UIWindow* window = g_windowMgr.LookupWindow(0x11D)) {
    window->Show(1);
    static_cast<UIMacroDetectorWnd*>(window)->SetCountdown(count, duration);
  }
}

// OFFSET: 0x00D79340
// FUNCTION: fn_22164
void CGameMode::ReceiveCaptchaPreviewImage(const char* key, const void* data, unsigned short size) {
  UICaptchaPreviewWnd* window = static_cast<UICaptchaPreviewWnd*>(g_windowMgr.LookupWindow(0x120));
  if (window != nullptr && std::strncmp(window->m_captchaKey, key, 4) == 0) {
    window->AddImageData(data, size);
  }
}

// OFFSET: 0x00D793D0
// FUNCTION: fn_22166
void CGameMode::ReceiveLapineUpgradeBoxValue(int value) {
  if (g_windowMgr.LookupWindow(0x12E) != nullptr) {
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(0x12E);
  if (window == nullptr) {
    return;
  }

  window->SendMsg(nullptr, 0x17, value, 0, 0, 0);
}

// OFFSET: 0x00D7B420
// FUNCTION: fn_22203
void CGameMode::Zc_Trycapture_Monster(const unsigned char* packet) {
  UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0394);
  if (packet[2] != 0) {
    if (window) {
      window->SendMsg(nullptr, 0x50, 1, 7, 0, 0);
    }
  } else {
    if (window) {
      window->SendMsg(nullptr, 0x50, 1, 3, 0, 0);
    }
  }
}

// OFFSET: 0x00D7B470
// FUNCTION: fn_22204
void CGameMode::Zc_Trycollection(const unsigned char* packet) {
  if (UIWindow* window = g_windowMgr.LookupWindow(0xC4)) {
    if (packet[2] != 0) {
      window->SendMsg(nullptr, 0x50, 1, 7, 0, 0);
    } else {
      window->SendMsg(nullptr, 0x50, 1, 3, 0, 0);
    }
  }
}

// OFFSET: 0x00D32830
// FUNCTION: fn_21745
void CGameMode::Zc_Msg(const unsigned char* packet) {
  const std::uint16_t message = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  std::uint32_t color = 0xFFFFFF;
  switch (message) {
  case 0x4C5:
  case 0x4C6:
  case 0x757:
  case 0x758:
  case 0x886:
    color = 0xFF;
    break;
  }

  int type = 0;
  switch (message) {
  case 0xD93:
  case 0xD94:
    type = 0x17;
    break;
  }

  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), color, type, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(message)), color, type, 0, 0);
}

// OFFSET: 0x00D328E0
// FUNCTION: fn_21746
void CGameMode::Zc_Msg_Color(const unsigned char* packet) {
  const std::uint16_t message = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  std::uint32_t color = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  switch (message) {
  case 0x4C5:
  case 0x4C6:
  case 0x757:
  case 0x758:
  case 0x886:
    color = 0xFF;
    break;
  }

  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), color, 0, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(message)), color, 0, 0, 0);
}

// OFFSET: 0x00D7B5E0
// FUNCTION: fn_22207
void CGameMode::ReceiveRouletteBonusPoints(const unsigned char* packet) {
  switch (packet[2]) {
  case 0:
    g_rouletteMgr->m_goldPoint = *reinterpret_cast<const int*>(packet + 3);
    break;
  case 1:
    g_rouletteMgr->m_silverPoint = *reinterpret_cast<const int*>(packet + 3);
    break;
  case 2:
    g_rouletteMgr->m_bronzePoint = *reinterpret_cast<const int*>(packet + 3);
    break;
  }

  if (g_windowMgr.unknown_0464 != nullptr) {
    g_windowMgr.RefreshRouletteWnd();
  }
}

// OFFSET: 0x00D7AE60
// FUNCTION: fn_22193
void CGameMode::ReceiveActorCharm(const unsigned char* packet) {
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  CRenderObject* actor;
  if (aid == *reinterpret_cast<const std::uint32_t*>(reinterpret_cast<const void*>(&g_session.m_aid))) {
    actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  } else {
    actor = g_session.FindGameActor(aid);
  }

  if (actor) {
    static_cast<CGameActor*>(actor)->SetCharm(
      *reinterpret_cast<const std::int16_t*>(packet + 6), *reinterpret_cast<const std::int16_t*>(packet + 8)
    );
  }
}

// OFFSET: 0x00D79970
// FUNCTION: fn_22175
void CGameMode::ReceivePacket0A29(int value) {
  ValuePacket packet;
  packet.id = 0x0A2A;
  packet.value = value;
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00D79E10
// FUNCTION: fn_22182
void CGameMode::ReceiveCashShopBanner(int value1, int value2, int value3) {
  if (!g_windowMgr.CloseWindow(0x13e)) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(0x13e)) {
      window->SendMsg(nullptr, 0xa4, value3, 0, 0, 0);
      window->SendMsg(nullptr, 0x78, value1, value2, 0, 0);
    } else {
      g_modeMgr.gameMode->SendMsg(0x143, value3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D7A180
// FUNCTION: fn_22188
void CGameMode::ReceiveWindow308TextEntries(unsigned char* packet) {
  g_windowMgr.CloseWindow(0x134);
  UIWindow* window = g_windowMgr.EnsureWindow(0x134);
  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 16;
  char* text = reinterpret_cast<char*>(packet + 4);
  for (int i = 0; i < count; ++i, text += 16) {
    text[15] = '\0';
    if (std::strlen(text) != 0) {
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D7B390
// FUNCTION: fn_22202
void CGameMode::ReceiveActorStateValue(std::uint32_t aid, int unused, std::int16_t value) {
  if (aid == 0 || !g_serviceOptions.Get(8)) {
    return;
  }

  CGameActor* actor;
  if (aid == g_session.m_aid) {
    actor = this->scene->player;
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  }
  if (actor == nullptr) {
    return;
  }

  if (actor->UpdateUnknown03E8(value)) {
    actor->LaunchEffect(0x2a5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x00D898B0
// FUNCTION: fn_22209
void CGameMode::ReceiveChatEntries(unsigned char* data) {
  ChatEntryListPacket* packet = reinterpret_cast<ChatEntryListPacket*>(data);
  const int count = (packet->length - 6) / static_cast<int>(sizeof(ChatTextEntry));
  g_windowMgr.CloseWindow(39);
  UIWindow* window = g_windowMgr.EnsureWindow(39);
  window->SendMsg(nullptr, 0x3B, packet->unknown_0004, 0, 0, 0);
  for (int i = 0; i < count; ++i) {
    ChatTextEntry* entry = &packet->entries[i];
    entry->text[15] = '\0';
    if (std::strlen(entry->text) != 0) {
      window->SendMsg(nullptr, 0x1B, reinterpret_cast<std::intptr_t>(entry->text), 0, 0, 0);
    }
  }
  window->SendMsg(nullptr, 0x1B, reinterpret_cast<std::intptr_t>("cancel"), 0, 0, 0);
}

// OFFSET: 0x00D7A200
// FUNCTION: fn_22189
void CGameMode::ReceivePacket0B73(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorValuePacket*>(data);
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAidIncludeMe(packet->accountId));
  if (!actor) {
    return;
  }

  const int job = actor->GetEffectiveJob(-1);
  if (job != 4240 && job != 4242 && job != 4303) {
    return;
  }

  if (StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
    g_effectMgr->Retire(actor->render.accountId, 1225);
  }

  CEffect* effect = g_effectMgr->Find(actor->render.accountId, 1225);
  if (!effect) {
    effect = g_effectMgr->Spawn(actor->render.accountId, 1225, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (!effect) {
      return;
    }
  }

  effect->unknown_00B0 = packet->value;
  effect->Slot01();
}

// OFFSET: 0x00D25850
// FUNCTION: fn_21673
void CGameMode::ReceiveUnknownD25850(const unsigned char* packet) {
  g_session.RemoveItem1710(*reinterpret_cast<const std::int16_t*>(packet + 2), 1);
  char number[12];
  char text[260];
  _itoa(*reinterpret_cast<const int*>(packet + 4), number, 10);
  const ITEM_DESC& description = FindItemDescription(std::atoi(number));
  std::sprintf(text, LookupMessage(1258), description.identifiedName);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
}

// OFFSET: 0x00D31A40
// FUNCTION: fn_21735
void CGameMode::ReceiveUnknownD31A40(const unsigned char* packet) {
  char text[512];
  const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) >> 2;
  const int* ids = reinterpret_cast<const int*>(packet + 4);
  std::strcpy(text, FindItemDescription(*ids).identifiedName);
  ++ids;
  for (unsigned int index = 1; index < count; ++index, ++ids) {
    std::sprintf(text, "%s %s", text, FindItemDescription(*ids).identifiedName);
  }

  std::sprintf(text, LookupMessage(2087), text);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D61100
// FUNCTION: fn_21976
void LaunchRollingCutterEffect(CGameActor* actor) {
  if (!actor || !actor->LaunchEffect(837, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    return;
  }

  if (static_cast<int>(actor->GetInstant(1)) <= 0) {
    actor->render.instant.Set(1, 0);
    actor->render.instant.Set(0, static_cast<std::int64_t>(actor->render.angle * 1000.0f));
  }

  actor->render.instant.Set(1, static_cast<int>(actor->GetInstant(1)) + 1);
}

// OFFSET: 0x00D2D8D0
// FUNCTION: fn_21721
void CGameMode::ReceiveInventoryItemUpdate(const unsigned char* packet) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t*>(packet + 2));
  if (item.unknown_0000[1] != 0) {
    item.unknown_0074[0] = *reinterpret_cast<const unsigned char*>(packet + 4) == 0;
    g_session.UpdateInventoryItem(item, 1);
  }
}

// OFFSET: 0x00D78030
// FUNCTION: fn_22157
void CGameMode::OpenWindow154() {
  if (g_windowMgr.LookupWindow(0x154) == nullptr) {
    g_windowMgr.EnsureWindow(0x154);
  }
}

// OFFSET: 0x00D2D990
// FUNCTION: fn_21722
void CGameMode::ReceiveUnknownD2D990(const unsigned char*) {
  if (g_windowMgr.unknown_03F8) {
    g_windowMgr.CloseWindow(137);
    GetRagConnection().Close();
    if (g_windowMgr.Confirm(LookupMessage(1349), 0, true, true, 0, "Message", 280, 120, 0) != 269) {
      this->SendMsg(141, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D4E3F0
// FUNCTION: fn_21876
void CGameMode::ReceiveUnknownD4E3F0(const unsigned char* packet) {
  unsigned int count = *reinterpret_cast<const std::uint16_t*>(packet + 2) - 4;
  UIWindow* window = g_windowMgr.EnsureWindow(0x49);
  window->SendMsg(nullptr, 0x4B, 0, 0, 0, 0);
  count >>= 1;
  const std::int16_t* ids = reinterpret_cast<const std::int16_t*>(packet + 4);
  for (unsigned int index = count; index != 0; --index, ++ids) {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *ids);
    if (item.unknown_0000[1] != 0) {
      window->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.unknown_0000[1], 0, 0);
    }
  }

  window->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
}

// OFFSET: 0x00D4E2A0
// FUNCTION: fn_21875
void CGameMode::ReceiveUnknownD4E2A0(const unsigned char* packet) {
  unsigned int count = *reinterpret_cast<const std::uint16_t*>(packet + 2) - 4;
  UIWindow* window = g_windowMgr.EnsureWindow(0x4A);
  window->SendMsg(nullptr, 0x4B, 0, 0, 0, 0);
  window->SendMsg(nullptr, 0x4D, *reinterpret_cast<const int*>(this->unknown_045C), 0, 0, 0);
  count >>= 1;
  const std::int16_t* ids = reinterpret_cast<const std::int16_t*>(packet + 4);
  for (unsigned int index = count; index != 0; --index, ++ids) {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *ids);
    if (item.unknown_0000[1] != 0) {
      window->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.unknown_0000[1], 0, 0);
    }
  }

  window->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
}

// OFFSET: 0x00D75960
// FUNCTION: fn_22149
void CGameMode::ReceiveNotifySkill(const unsigned char* packet) {
  ActorSkillMessage message;
  message.unknown_0000 = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  message.unknown_0004 = *reinterpret_cast<const std::int32_t*>(packet + 0x18);
  message.unknown_0008 = packet[0x20];
  message.unknown_000C = *reinterpret_cast<const std::uint32_t*>(packet + 8);
  message.unknown_0010 = *reinterpret_cast<const std::uint32_t*>(packet + 0xC);
  message.unknown_0014 = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  message.unknown_0018 = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  message.unknown_001C = -1;
  message.unknown_0020 = -1;
  message.unknown_0024 = *reinterpret_cast<const std::uint32_t*>(packet + 0x10);
  message.unknown_0028 = *reinterpret_cast<const std::uint32_t*>(packet + 0x14);

  const std::uint16_t skillId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  const std::uint32_t sourceAid = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  const std::uint32_t targetAid = *reinterpret_cast<const std::uint32_t*>(packet + 8);

  CGameActor* gameActor = nullptr;
  CGameActor* source = nullptr;
  int job = -1;

  if (skillId == 0x8C2 && targetAid == g_session.m_aid) {
    return;
  }

  CGameActor* target;
  if (targetAid == g_session.m_aid) {
    target = this->scene->player;
  } else {
    target = static_cast<CGameActor*>(this->scene->GetGameActorByAID(targetAid));
  }

  if (sourceAid == g_session.m_aid) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
    this->scene->player->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    source = this->scene->player;
    job = source->GetEffectiveJob(-1);
  } else {
    gameActor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(sourceAid));
    if (gameActor != nullptr) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      gameActor->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source = gameActor;
      job = source->GetEffectiveJob(-1);
    } else if (CRenderObject* skill = this->scene->GetSkillByAID(sourceAid)) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      skill->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  CRenderObject* targetSkill;
  if (target == nullptr) {
    targetSkill = this->scene->GetSkillByAID(targetAid);
  } else {
    targetSkill = nullptr;
  }

  CSkillActionMgr::GetInstance()->ActionSlot15(
    skillId, reinterpret_cast<int>(source), target, static_cast<CGameActor*>(targetSkill)
  );

  const vector3d origin{0.0f, 0.0f, 0.0f};
  int effectId;

  if (target != nullptr && source != nullptr) {
    switch (skillId) {
    case 0x2E1:
    case 0x911:
    case 0x96D: {
      effectId = skillId == 0x96D ? 0x62F : 0x593;
      const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(duration);
      }
      break;
    }
    case 0x2C4:
      target->LaunchEffect(0x5C, origin, 0.0f);
      source->Slot15(7, 1);
      break;
    case 0x2ED:
      if (source == this->scene->player) {
        source->LaunchEffect(0x461, origin, 0.0f);
      }
      if (target == this->scene->player) {
        target->LaunchEffect(0x461, origin, 0.0f);
      }
      break;
    case 0x296:
    case 0x300:
      if (source != target) {
        target->LaunchEffect(0x174, origin, 0.0f);
      }
      break;
    case 0x7D3:
      source->Slot15(2, 1);
      target->LaunchEffect(0xB7, origin, 0.0f);
      break;
    case 0x8A5:
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x5FE, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x5FF;
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x585, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x586;
      }
      g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      source->Slot15(7, 1);
      break;
    case 0x996:
      g_effectMgr->Spawn(target->render.accountId, 0x619, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x61A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x9AD: {
      CEffect* effect;
      if (g_serviceOptions.Get(0xE5)) {
        effect = g_effectMgr->Spawn(source->render.accountId, 0x5DA, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(200);
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x5D8, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(400);
        }
        effectId = 0x5D9;
      } else {
        effect = g_effectMgr->Spawn(source->render.accountId, 0x550, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(200);
        }
        effect = g_effectMgr->Spawn(source->render.accountId, 0x551, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(200);
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x54E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(400);
        }
        effectId = 0x54F;
      }
      effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(400);
      }
      break;
    }
    case 0x14C3:
      g_effectMgr->Spawn(
        source->render.accountId, 0x86A, source->render.position.x, source->render.position.y, source->render.position.z,
        target->render.accountId, -1, 0, 0
      );
      break;
    case 0x1F78: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const vector3d from = source->render.position;
      const vector3d to = target->render.position;
      const float yaw = mode->CameraDesiredYaw();
      const float heading = static_cast<float>(std::atan2(static_cast<double>(to.z - from.z), static_cast<double>(to.x - from.x)));
      const float angle = 360.0f - (heading / 3.141592f * 180.0f + yaw);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x91F, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot17(angle);
      }
      break;
    }
    }
  }

  if (source != nullptr && skillId == 0x89B) {
    source->Slot15(7, 1);
  }

  if (target != nullptr) {
    switch (skillId) {
    case 0x8D:
      if (*reinterpret_cast<const std::int16_t*>(packet + 0x1C) == -1) {
        target->LaunchEffect(0x81, origin, 0.0f);
      }
      break;
    case 0x2DB:
    case 0x7D8: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (mode == nullptr || source == nullptr) {
        break;
      }
      float angle = mode->CameraDesiredYaw() + source->render.angle;
      if (angle >= 360.0f) {
        angle -= 360.0f;
      }
      int directionId;
      if (angle > 0.0f && angle < 90.0f) {
        directionId = 0x565;
      } else if (angle >= 90.0f && angle < 180.0f) {
        directionId = 0x564;
      } else if (angle >= 180.0f && angle < 270.0f) {
        directionId = 0x567;
      } else {
        directionId = 0x566;
      }

      CEffect* effect;
      if (g_serviceOptions.Get(0xE5)) {
        directionId += 0x8D;
        if (g_effectMgr->Find(source->render.accountId, directionId) == nullptr) {
          effect = g_effectMgr->Spawn(source->render.accountId, directionId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
          if (effect) {
            effect->Slot16(200);
          }
        }
        effectId = 0x5F0;
      } else {
        if (g_effectMgr->Find(source->render.accountId, directionId) == nullptr) {
          effect = g_effectMgr->Spawn(source->render.accountId, directionId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
          if (effect) {
            effect->Slot16(200);
          }
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x562, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(350);
        }
        effectId = 0x563;
      }
      effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(350);
      }
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case 0x138C: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (mode == nullptr || source == nullptr) {
        break;
      }
      float angle = mode->CameraDesiredYaw() + source->render.angle;
      if (angle >= 360.0f) {
        angle -= 360.0f;
      }
      int directionId;
      if (angle > 0.0f && angle < 90.0f) {
        directionId = 0x56D;
      } else if (angle >= 90.0f && angle < 180.0f) {
        directionId = 0x56C;
      } else if (angle >= 180.0f && angle < 270.0f) {
        directionId = 0x56F;
      } else {
        directionId = 0x56E;
      }

      CEffect* effect;
      if (g_serviceOptions.Get(0xE5)) {
        directionId += 0x8B;
        if (g_effectMgr->Find(source->render.accountId, directionId) == nullptr) {
          effect = g_effectMgr->Spawn(source->render.accountId, directionId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
          if (effect) {
            effect->Slot16(200);
          }
        }
        effectId = 0x5F6;
      } else {
        if (g_effectMgr->Find(source->render.accountId, directionId) == nullptr) {
          effect = g_effectMgr->Spawn(source->render.accountId, directionId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
          if (effect) {
            effect->Slot16(200);
          }
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x56A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(350);
        }
        effectId = 0x56B;
      }
      effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(350);
      }
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case 0x7D5:
      if (target != source) {
        g_effectMgr->Spawn(target->render.accountId, 0x69D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      }
      break;
    case 0x89C: {
      const float step = static_cast<float>(std::max(std::rand() % 5, 0) + 8) * 10.0f;
      for (int index = 0; index < 3; ++index) {
        const float radius = static_cast<float>(std::max(std::rand() % 10, 0)) + 5.0f;
        matrix rotation;
        rotation.MakeYRotation(static_cast<float>(index) * step / 180.0f * 3.141592f);
        vector3d position;
        position.x = radius * rotation.values[0] + rotation.values[3] * origin.x + rotation.values[6] * origin.x
                   + rotation.values[9] + target->render.position.x;
        position.y = radius * rotation.values[1] + rotation.values[4] * origin.x + rotation.values[7] * origin.x
                   + rotation.values[10] + target->render.position.y;
        position.z = radius * rotation.values[2] + rotation.values[5] * origin.x + rotation.values[8] * origin.x
                   + rotation.values[11] + target->render.position.z;
        const int kind = std::max(std::rand() % 3, 0);
        int id;
        if (kind == 0) {
          id = 0x5A3;
        } else {
          id = (kind != 1) + 0x5A4;
        }
        if (g_serviceOptions.Get(0xE5)) {
          id += 0x61;
        }
        g_effectMgr->Spawn(0, id, position.x, position.y, position.z, 0, -1, 0, 0);
      }
      break;
    }
    case 0x8BE:
      g_soundMgr->PlayWave("effect\\hunter_shockwavetrap.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x92, origin, 0.0f);
      break;
    case 0x8BF:
      target->LaunchEffect(0x249, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\hunter_landmine.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case 0x8CD:
      target->LaunchEffect(0x247, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case 0x8CE:
      target->LaunchEffect(0x87, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\ef_frostdiver.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case 0x8D3:
      target->LaunchEffect(0x247, origin, 0.0f);
      target->LaunchEffect(0x31, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case 0x8D5: {
      for (int duration = 90; duration < 450; duration += 90) {
        const float x = target->render.position.x;
        const float y = target->render.position.y;
        const float z = target->render.position.z;
        int dx = std::max(std::rand() % 3, 0) * 5;
        int dz = std::max(std::rand() % 3, 0) * 5;
        const int direction = std::max(std::rand() % 4, 0);
        if (dx == 10 && dz == 10) {
          if (std::max(std::rand() % 2, 0) == 0) {
            dx = 5;
          } else {
            dz = 5;
          }
        }
        if (direction == 0) {
          dx = -dx;
        } else if (direction == 1) {
          dz = -dz;
        } else if (direction == 2) {
          dx = -dx;
          dz = -dz;
        }
        const vector3d position{static_cast<float>(dx) + x, y, static_cast<float>(dz) + z};

        CEffect* effect;
        if (g_serviceOptions.Get(0xE5)) {
          effect = g_effectMgr->Spawn(0, 0x5D2, position.x, position.y, position.z, 0, -1, 0, 0);
          if (effect) {
            effect->Slot16(duration);
          }
          effectId = 0x5D3;
        } else {
          effect = g_effectMgr->Spawn(0, 0x54B, position.x, position.y, position.z, 0, -1, 0, 0);
          if (effect) {
            effect->Slot16(duration);
          }
          effectId = 0x54C;
        }
        effect = g_effectMgr->Spawn(0, effectId, position.x, position.y, position.z, 0, -1, 0, 0);
        if (effect) {
          effect->Slot16(duration);
        }
      }

      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x5D2, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x5D3;
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x54B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x54C;
      }
      g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case 0x900:
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x5B7, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x5B8;
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x520, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x521;
      }
      g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x919: {
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x624, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(100);
      }
      break;
    }
    case 0x9B1:
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x385, origin, 0.0f);
      break;
    case 0x9BA:
      g_effectMgr->Spawn(target->render.accountId, 0x551, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_soundMgr->PlayWave("spore_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case 0x9D5:
      g_effectMgr->Spawn(target->render.accountId, 0x62C, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x1499:
      g_effectMgr->Spawn(target->render.accountId, 0x720, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x721, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x14AE:
      g_effectMgr->Spawn(target->render.accountId, 0x748, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x749, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x1F6F:
      g_effectMgr->Spawn(target->render.accountId, 0x919, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x1F70:
      g_effectMgr->Spawn(target->render.accountId, 0x916, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x917, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x1F72:
      g_effectMgr->Spawn(target->render.accountId, 0x921, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x1F75:
      g_effectMgr->Spawn(target->render.accountId, 0x912, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x1F76:
      g_effectMgr->Spawn(target->render.accountId, 0x913, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x914, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x20FD:
      g_effectMgr->Spawn(target->render.accountId, 0x7F3, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x7F4, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x2100:
      g_effectMgr->Spawn(target->render.accountId, 0x7E3, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x7E4, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x2103:
      g_effectMgr->Spawn(target->render.accountId, 0x832, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x833, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x2106:
      g_effectMgr->Spawn(target->render.accountId, 0x838, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x2109:
      g_effectMgr->Spawn(target->render.accountId, 0x7D9, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x2199:
      g_effectMgr->Spawn(target->render.accountId, 0x78E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x791, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x219A:
      g_effectMgr->Spawn(target->render.accountId, 0x77B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x77D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case 0x219D:
      g_effectMgr->Spawn(target->render.accountId, 0x772, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x773, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x775, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    }
  }

  if (source != nullptr) {
    if (g_session.IsMonsterActor(source) == 0) {
      this->SendMsg(0x56, skillId, reinterpret_cast<std::intptr_t>(source), *reinterpret_cast<const std::int16_t*>(packet + 0x1C), 0);
      return;
    }
    const unsigned char kind = source->unknown_0314;
    if (kind == 10 || kind == 13 || kind == 14) {
      this->SendMsg(0x56, skillId, reinterpret_cast<std::intptr_t>(source), *reinterpret_cast<const std::int16_t*>(packet + 0x1C), 0);
    }
  }

  int action;
  int loopType;
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.unknown_0000, &action, &loopType);
  if (action != -1 && gameActor != nullptr) {
    gameActor->SetAction(action * 8 - 8, 1, loopType == 0);
  }
}

// OFFSET: 0x00D77850
// FUNCTION: fn_22150
void CGameMode::ReceiveActorSkillAction(const unsigned char* packet) {
  ActorSkillMessage message;
  message.unknown_0000 = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  message.unknown_0004 = *reinterpret_cast<const std::int16_t*>(packet + 0x18);
  message.unknown_0008 = packet[0x1E];
  message.unknown_000C = *reinterpret_cast<const std::uint32_t*>(packet + 8);
  message.unknown_0010 = *reinterpret_cast<const std::uint32_t*>(packet + 0xC);
  message.unknown_0014 = *reinterpret_cast<const std::int16_t*>(packet + 0x1A);
  message.unknown_0018 = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  message.unknown_001C = -1;
  message.unknown_0020 = -1;
  message.unknown_0024 = *reinterpret_cast<const std::uint32_t*>(packet + 0x10);
  message.unknown_0028 = *reinterpret_cast<const std::uint32_t*>(packet + 0x14);

  CGameActor* actor = nullptr;
  CGameActor* gameActor = nullptr;
  int job = -1;

  if (*reinterpret_cast<const std::uint32_t*>(packet + 4) == g_session.m_aid) {
    actor = this->scene->player;
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
    actor->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    job = actor->GetEffectiveJob(-1);
  } else {
    gameActor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
    if (gameActor != nullptr) {
      actor = gameActor;
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      actor->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      job = actor->GetEffectiveJob(-1);
    } else if (CRenderObject* skill = this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4))) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      skill->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (g_session.IsMonsterActor(actor) == 0) {
    this->SendMsg(0x56, *reinterpret_cast<const std::uint16_t*>(packet + 2), reinterpret_cast<std::intptr_t>(actor), *reinterpret_cast<const std::int16_t*>(packet + 0x1A), 0);
    return;
  }

  int action;
  int loopType;
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.unknown_0000, &action, &loopType);
  if (action != -1 && gameActor != nullptr) {
    gameActor->SetAction(action * 8 - 8, 1, loopType == 0);
  }
}

// OFFSET: 0x00D77A30
// FUNCTION: fn_22151
void CGameMode::ReceiveActorSkillMessage(const unsigned char* packet) {
  ActorSkillMessage message;
  message.unknown_0000 = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  message.unknown_0004 = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  message.unknown_0008 = packet[0x22];
  message.unknown_000C = *reinterpret_cast<const std::uint32_t*>(packet + 8);
  message.unknown_0010 = *reinterpret_cast<const std::uint32_t*>(packet + 0xC);
  message.unknown_0014 = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  message.unknown_0018 = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  message.unknown_001C = *reinterpret_cast<const std::int16_t*>(packet + 0x18);
  message.unknown_0020 = *reinterpret_cast<const std::int16_t*>(packet + 0x1A);
  message.unknown_0024 = *reinterpret_cast<const std::uint32_t*>(packet + 0x10);
  message.unknown_0028 = *reinterpret_cast<const std::uint32_t*>(packet + 0x14);

  CGameActor* actor = nullptr;
  int job = -1;

  if (*reinterpret_cast<const std::uint32_t*>(packet + 4) == g_session.m_aid) {
    actor = this->scene->player;
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
    actor->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    job = actor->GetEffectiveJob(-1);
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
    if (actor != nullptr) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      actor->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      job = actor->GetEffectiveJob(-1);
    } else if (CRenderObject* skill = this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4))) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      skill->SendMsg(nullptr, 0x25, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor = nullptr;
    }
  }

  if (g_session.IsMonsterActor(actor) == 0) {
    this->SendMsg(0x56, *reinterpret_cast<const std::uint16_t*>(packet + 2), reinterpret_cast<std::intptr_t>(actor), *reinterpret_cast<const std::int16_t*>(packet + 0x1E), 0);
    return;
  }

  int action;
  int loopType;
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.unknown_0000, &action, &loopType);
}

// OFFSET: 0x00D25310
// FUNCTION: fn_21672
void CGameMode::ReceiveItem1740List(const unsigned char* packet) {
  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<int>(static_cast<unsigned int>(length - 4) / sizeof(StoreNormalRecord));
  const auto* record = reinterpret_cast<const StoreNormalRecord*>(packet + 4);
  for (int index = count; index > 0; --index, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = 0;
    item.unknown_0000[3] = record->location;
    item.quantity = record->amount;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.unknown_005D = 0;
    item.identified = record->flags & 1;
    item.unknown_0060 = 0;
    item.unknown_0068 = record->expireTime;
    item.unknown_0074[0] = (record->flags >> 1) & 1;
    g_session.AddItem1740(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D270B0
// FUNCTION: fn_21687
void CGameMode::ReceiveDispel(const unsigned char* packet) {
  const auto* dispel = reinterpret_cast<const DispelPacket*>(packet);

  if (dispel->aid == g_session.m_aid) {
    this->scene->player->SendMsg(nullptr, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, 87, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    CPlayer* player = this->scene->player;
    if (player->render.unknown_0070 == 8) {
      player->Slot15(0, 1);
    }
    return;
  }

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(dispel->aid));
  if (actor == nullptr) {
    return;
  }

  actor->SendMsg(nullptr, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  actor->SendMsg(nullptr, 87, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  actor->SendMsg(nullptr, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  g_effectMgr->Retire(dispel->aid, 1114);
  if (actor->render.unknown_0070 == 8) {
    actor->Slot15(0, 1);
  }
}

// OFFSET: 0x00D32E10
// FUNCTION: fn_21748
void CGameMode::ReceiveItem1758List(const unsigned char* packet) {
  if (g_replayAnimationScaling == 0) {
    reinterpret_cast<UIMerchantShopMakeWnd*>(g_windowMgr.unknown_040C)->PrintBuyList();
  }

  g_session.ClearItems1768();
  g_windowMgr.CloseWindow(0xAE);
  g_windowMgr.CloseWindow(0xAF);
  g_session.ClearItems1758();

  const int count = static_cast<unsigned int>(static_cast<int>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 12) / 11u;
  const Item1758Record* record = reinterpret_cast<const Item1758Record*>(packet + 12);
  for (int index = 0; index < count; ++index, ++record) {
    const Item1758Record entry = *record;
    ITEM_INFO item;
    item.unknown_0014[0] = entry.price;
    item.unknown_0014[1] = entry.price;
    item.quantity = entry.quantity;
    item.unknown_0000[0] = entry.type;
    item.unknown_0000[1] = index + 1;
    item.identified = 1;
    item.SetItemId(entry.itemId);
    g_session.AddItem1758(item);
  }

  UIWindow* window = g_windowMgr.EnsureWindow(0xB0);
  window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  window->SendMsg(nullptr, 0x77, *reinterpret_cast<const int*>(packet + 8), 0, 0, 0);
  this->unknown_024C = 1;
}

// OFFSET: 0x00D50810
// FUNCTION: fn_21886
void CGameMode::ReceiveUnknownD50810(const unsigned char* packet) {
  if (g_replayAnimationScaling != 0) {
    g_modeMgr.GetGameMode()->SendMsg(82, 0, 0, 0, 0);
  }

  g_session.ClearItems1758();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 8) / sizeof(Item1758StoreRecord);
  if (count == 0) {
    this->SendMsg(40, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(43);
    g_windowMgr.CloseWindow(44);
    g_session.ClearItems1760And1748();
    return;
  }

  const Item1758StoreRecord* record = reinterpret_cast<const Item1758StoreRecord*>(packet + 8);
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0014[0] = record->unknown_0000;
    item.unknown_0014[1] = record->unknown_0000;
    item.quantity = record->amount;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.unknown_005D = record->damaged;
    item.unknown_0060 = record->refine;
    item.unknown_0098 = CountStoreItemOptions(record->options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = record->options[index];
    }
    g_session.AddItem1758(item);
  }
}

// OFFSET: 0x00D50590
// FUNCTION: fn_21885
void CGameMode::ReceiveItem1748List(const unsigned char* packet) {
  g_session.ClearItems1748();
  g_session.ClearItems1760();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 0x0C) / sizeof(Item1748Record);
  for (int i = 0; i < count; ++i) {
    const Item1748Record record = *reinterpret_cast<const Item1748Record*>(packet + 0x0C + i * sizeof(Item1748Record));
    ITEM_INFO item;
    item.unknown_0000[0] = record.type;
    item.unknown_0000[1] = record.index;
    item.unknown_0000[2] = record.unknown_0039;
    item.unknown_0014[0] = record.unknown_0000;
    item.unknown_0014[1] = record.unknown_0000;
    item.quantity = record.amount;
    std::memcpy(item.unknown_001C, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.identified;
    item.unknown_005D = record.damaged;
    item.unknown_0060 = record.refine;
    item.unknown_0070 = record.sprite;
    item.unknown_0098 = CountStoreItemOptions(record.options);
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = record.options[index];
    }
    g_session.AddItem1748(item);
  }

  UIWindow* window = g_windowMgr.EnsureWindow(43);
  window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  window->SendMsg(nullptr, 28, *reinterpret_cast<const std::int32_t*>(packet + 4), 0, 0, 0);
  UIWindow* optionsWindow = g_windowMgr.EnsureWindow(44);
  optionsWindow->SendMsg(nullptr, 23, 0, 0, 0, 0);
  optionsWindow->SendMsg(nullptr, 28, *reinterpret_cast<const std::int32_t*>(packet + 4), *reinterpret_cast<const std::int32_t*>(packet + 8), 0, 0);
  this->unknown_024C = 1;
}

// OFFSET: 0x00D5F5C0
// FUNCTION: fn_21970
void CGameMode::ReceiveActorStatusValue(std::uint32_t aid, int status, int amount, int option) {
  CGameActor* actor;
  if (aid == g_session.m_aid) {
    actor = this->scene->player;
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  }
  if (actor == nullptr) {
    return;
  }

  switch (status) {
  case 0x14b:
    if (aid == g_session.m_aid) {
      g_session.UpdateRandomStats();
      const int hp = g_session.unknown_5568 + amount;
      if (g_session.unknown_5568 != hp) {
        g_session.unknown_5568 = hp;
        g_session.RefreshHpSpWindows();
      }
    }
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFF00FF00, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x14c:
    if (aid == g_session.m_aid) {
      g_session.UpdateRandomStats();
      int& sp = *reinterpret_cast<int*>(&g_session.unknown_5570[0]);
      const int value = sp + amount;
      if (sp != value) {
        sp = value;
        g_session.RefreshHpSpWindows();
      }
    }
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFF0000FF, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x291:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFF00FF00, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x292:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFF0000FF, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x293:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFFFF0000, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x294:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFFFF00FF, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x295:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFF000000, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x296:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFFFFFFFF, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x297:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFFFFFF00, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x298:
    actor->SendMsg(nullptr, 0x58, 0, amount, option, 0xFFFF55B1, 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    break;
  default:
    break;
  }

  CEffect* effect = g_effectMgr->Spawn(actor->render.accountId, status, actor->render.position.x, actor->render.position.y,
                                       actor->render.position.z, 0, 0xFFFFFFFF, 0, 0);
  if (effect == nullptr) {
    actor->LaunchEffect(status, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x00D39BB0
// FUNCTION: fn_21773
void CGameMode::ReceiveItem1718List(const unsigned char* packet) {
  g_session.ClearTradeItems();

  const std::int64_t count = (static_cast<std::int64_t>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 4) / 19;
  const Item1718Record* record = reinterpret_cast<const Item1718Record*>(packet + 4);

  for (std::int64_t index = 0; index < count; ++index) {
    Item1718Record entry;
    memcpy_s(&entry, sizeof(entry), &record[index], sizeof(entry));

    RawItemBasic item = {};
    _itoa(entry.itemId, item.itemId, 10);
    item.unknown_0004 = entry.unknown_0005;
    item.unknown_0008 = entry.unknown_0005;
    item.unknown_000C = entry.type;
    item.unknown_0034 = entry.unknown_000F;
    item.id = -1;
    item.identified = 1;
    item.unknown_0038 = FindItemDescription(std::atoi(item.itemId)).unknown_0040;
    g_session.AddItem1718(item, entry.quantity, static_cast<std::uint16_t>(entry.unknown_000D / 10));
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(0xFE)) {
    window->SendMsg(nullptr, 0x24, 0, 0, 0, 0);
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(0xFF)) {
    window->SendMsg(nullptr, 0x24, 0, 0, 0, 0);
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D77C90
// FUNCTION: fn_22154
void ReceiveTradeItems(int count, const TradeItemRecord* records) {
  g_session.ClearTradeItems();

  for (int i = 0; i < count; ++i) {
    const TradeItemRecord& record = records[i];
    ITEM_INFO item;
    item.SetItemId(record.itemId);
    item.unknown_0000[0] = record.unknown_0004;
    item.unknown_006C = record.unknown_000D / 10;
    item.quantity = record.quantity;
    item.unknown_0014[0] = record.unknown_0005;
    item.unknown_0014[1] = record.unknown_0005;
    item.unknown_0060 = record.unknown_000F;
    std::memcpy(item.unknown_001C, record.cards, sizeof(record.cards));
    item.unknown_0000[1] = record.unknown_0020;
    item.identified = 1;
    item.unknown_0074[3] = 1;
    if (item.quantity > 0) {
      g_session.AppendItem1718(item);
    }
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(0xfe)) {
    window->SendMsg(nullptr, 0x24, 0, 0, 0, 0);
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(0xff)) {
    window->SendMsg(nullptr, 0x24, 0, 0, 0, 0);
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D7A2F0
// FUNCTION: fn_22190
void CGameMode::ShowSpecialPopup(int popupId) {
  if (this->unknown_060C[0]) {
    return;
  }

  PopupData* popup = g_specialPopupMgr->GetPopup(popupId);
  if (!popup) {
    this->SendMsg(1, reinterpret_cast<int>(FormatOwnedString("No GetPopupData : %d ", popupId).c_str()), 0xFFFF00FF, 0, 0);
    return;
  }

  UIWindow window(0);
  std::string text;
  switch (popup->m_messageId) {
  case 0:
    for (const std::string& description : popup->m_descriptions) {
      text = RemoveTextColors(description.c_str());
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), popup->m_color, 0, 0, 0);
    }
    break;
  case 1:
    if (UIWindow* popupWindow = g_windowMgr.EnsureWindow(358)) {
      popupWindow->SendMsg(nullptr, 6, 510, popupId, 0, 0);
    }
    [[fallthrough]];
  case 2:
    for (const std::string& description : popup->m_descriptions) {
      text = RemoveTextColors(description.c_str());
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), popup->m_color, 0, 0, 0);
    }
    if (UIWindow* popupWindow = g_windowMgr.EnsureWindow(358)) {
      popupWindow->SendMsg(nullptr, 6, 510, popupId, 0, 0);
    }
    break;
  }
}

// OFFSET: 0x00D7A690
// FUNCTION: fn_22191
void CGameMode::Zc_Spirits2(const unsigned char* data) {
  if (!g_serviceOptions.Get(8)) {
    return;
  }

  const auto* packet = reinterpret_cast<const ActorValuePacket*>(data);
  CGameActor* actor;
  if (packet->accountId == g_session.m_aid) {
    actor = this->scene->player;
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->accountId));
  }

  if (!actor) {
    return;
  }

  if (StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
    int effectId;
    switch (actor->GetEffectiveJob(-1)) {
    case 24:
    case 4215:
    case 4228:
    case 4229:
    case 4306:
      effectId = 629;
      break;
    case 4038:
      effectId = 504;
      break;
    default:
      effectId = 228;
      break;
    }

    actor->SendMsg(nullptr, 109, 0, effectId, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return;
  }

  CRagEffect* effect;
  if (actor->GetEffectiveJob(-1) == 4038) {
    if (actor->HasId(504) || packet->accountId == 0) {
      return;
    }

    effect = actor->LaunchEffect(504, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  } else if (actor->GetEffectiveJob(-1) == 24 || actor->GetEffectiveJob(-1) == 4215 || actor->GetEffectiveJob(-1) == 4228 ||
             actor->GetEffectiveJob(-1) == 4229 || actor->GetEffectiveJob(-1) == 4306) {
    if (actor->HasId(629) || packet->accountId == 0) {
      return;
    }

    effect = actor->LaunchEffect(629, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  } else {
    if (actor->HasId(228) || packet->accountId == 0) {
      return;
    }

    effect = actor->LaunchEffect(228, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }

  vector3d count{static_cast<float>(packet->value), 0.0f, 0.0f};
  if (effect) {
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&count), 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D7A920
// FUNCTION: fn_22192
void CGameMode::UpdateActorSpiritCount(std::uint32_t aid, std::int16_t count) {
  if (!aid) {
    return;
  }

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAidIncludeMe(aid));
  if (!actor) {
    return;
  }

  const int job = actor->GetEffectiveJob(-1);
  switch (job) {
  case 4252:
  case 4265:
  case 4280:
    break;
  case 4260:
    if (count == 0) {
      g_effectMgr->Retire(actor->render.accountId, 2109);
      return;
    }
    break;
  case 24:
  case 4228:
    actor->SendMsg(nullptr, 109, 0, 629, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (count == 0) {
      return;
    }
    break;
  case 4038:
    actor->SendMsg(nullptr, 109, 0, 504, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (count == 0) {
      return;
    }
    break;
  case 4215:
  case 4229:
  case 4306:
    g_effectMgr->Retire(actor->render.accountId, 1135);
    if (count == 0) {
      return;
    }
    break;
  default:
    actor->SendMsg(nullptr, 109, 0, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (count == 0) {
      return;
    }
    break;
  }

  if (StateFlag01(actor->Slot13()) || StateFlag02(actor->Slot13())) {
    switch (job) {
    case 4252:
    case 4265:
    case 4280:
      g_effectMgr->Retire(g_effectMgr->Find(actor->render.accountId, 2460), 1);
      return;
    case 4260:
      g_effectMgr->Retire(actor->render.accountId, 2109);
      break;
    }
  }

  CRagEffect* effect;
  switch (job) {
  case 24:
  case 4228:
    effect = actor->LaunchEffect(629, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 4038:
    effect = actor->LaunchEffect(504, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 4215:
  case 4229:
  case 4306: {
    CEffect* coins = g_effectMgr->Spawn(actor->render.accountId, 1135, actor->render.position.x, actor->render.position.y,
                                        actor->render.position.z, 0, -1, 0, 0);
    if (!coins) {
      return;
    }
    static_cast<CRichsCoinEffect*>(coins)->unknown_00E4 = count;
    coins->Slot01();
    return;
  }
  case 4252:
  case 4265:
  case 4280: {
    CServantWeaponNewEffect* weapons =
      static_cast<CServantWeaponNewEffect*>(g_effectMgr->Spawn(actor->render.accountId, 2460, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0));
    if (!weapons) {
      return;
    }
    const int current = static_cast<int>(weapons->emitters.size());
    if (current < count) {
      for (int remaining = count - current; remaining != 0; --remaining) {
        weapons->AddWeapon();
      }
    } else if (current > count) {
      for (int remaining = current - count; remaining != 0; --remaining) {
        weapons->Shoot(5202);
      }
    }
    return;
  }
  case 4260: {
    CEffect* abyss = g_effectMgr->Find(actor->render.accountId, 2109);
    if (!abyss) {
      abyss = g_effectMgr->Spawn(actor->render.accountId, 2109, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    if (abyss) {
      static_cast<CFromTheAbyssEffect*>(abyss)->SetLevel(count);
    }
    for (auto it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end(); ++it) {
      if (it->id == 1244) {
        it->unknown_0008 = count;
        break;
      }
    }
    return;
  }
  default:
    effect = actor->LaunchEffect(228, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }

  if (effect && (effect->state.type == 228 || effect->state.type == 504 || effect->state.type == 629)) {
    vector3d value{static_cast<float>(count), 0.0f, 0.0f};
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&value), 0, 0, 0, 0);
  }
}

#pragma pack(push, 1)
struct BuyingStoreItemRecord {
  /* 0000 */ std::uint32_t price;
  /* 0004 */ std::int16_t count;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::int32_t itemId;
};
#pragma pack(pop)

// OFFSET: 0x00D1CFF0
// FUNCTION: fn_21628
void CGameMode::ReceiveBuyingStoreItemList(const unsigned char* packet) {
  CSession* lists = &g_session;
  lists->ClearItems1748();
  lists->ClearItems1760();

  const BuyingStoreItemRecord* record = reinterpret_cast<const BuyingStoreItemRecord*>(packet + 16);
  const unsigned count = (static_cast<int>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 16u) / 11;
  for (int index = 0; index < static_cast<int>(count);) {
    const BuyingStoreItemRecord entry = *record;
    ITEM_INFO item;
    ++index;
    item.unknown_0014[0] = entry.price;
    item.unknown_0014[1] = entry.price;
    item.unknown_0000[0] = entry.type;
    item.quantity = entry.count;
    item.identified = 1;
    item.unknown_0000[1] = index;
    item.SetItemId(entry.itemId);
    lists->AddItem1748(item);
    ++record;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(0xB1);
  window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  window->SendMsg(nullptr, 0x1C, *reinterpret_cast<const std::int32_t*>(packet + 4), 0, 0, 0);
  window->SendMsg(nullptr, 0x77, *reinterpret_cast<const std::int32_t*>(packet + 12), 0, 0, 0);

  window = g_windowMgr.EnsureWindow(0xB2);
  window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  window->SendMsg(nullptr, 0x1C, *reinterpret_cast<const std::int32_t*>(packet + 4), *reinterpret_cast<const std::int32_t*>(packet + 8), 0, 0);

  window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0420);
  if (window) {
    g_session.RefreshItems1750();
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  } else if ((window = g_windowMgr.EnsureWindow(0xB3)) != nullptr) {
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }

  this->unknown_024C = 1;
}

// OFFSET: 0x00D5F3E0
// FUNCTION: fn_21969
void CGameMode::SetActorNameCard(std::uint32_t id, const char* name, const char* value1, const char* value2, const char* value3, int job) {
  if (id == g_session.m_aid) {
    g_session.unknown_615C = job;
  }

  std::string jobTitle = GetJobTitleString(job);
  this->nameCard.Set(id, name, value1, value2, value3, jobTitle, std::string(), 0);
}

// OFFSET: 0x00D79BB0
// FUNCTION: fn_22180
void CGameMode::ReceiveShopName(const char* text, int shopId) {
  std::string name = text;
  name.resize(name.size() - 4);
  if (!g_session.AddShopName(name, shopId)) {
    ValuePacket packet;
    packet.id = 0x0BBD;
    packet.value = shopId;
    GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
  }
}

void SetPlayerName(std::uint32_t id, std::string name);

namespace {
  struct FriendRecord {
    /* 0000 */ std::uint32_t accountId;
    /* 0004 */ std::uint32_t characterId;
    /* 0008 */ char name[24];
  };

  static_assert(sizeof(FriendRecord) == 0x20);
}

// OFFSET: 0x00D2AB70
// FUNCTION: fn_21706
void CGameMode::ReceiveFriendList(const unsigned char* packet) {
  g_session.ClearFriends();

  const std::int64_t length = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  if (length == 4) {
    return;
  }

  const std::int64_t count = (length - 4) / static_cast<std::int64_t>(sizeof(FriendRecord));
  const FriendRecord* records = reinterpret_cast<const FriendRecord*>(packet + 4);
  for (std::int64_t index = 0; index < count; ++index) {
    FriendRecord record;
    memcpy_s(&record, sizeof(record), &records[index], sizeof(record));

    char name[25] = {};
    StringFormat(name, sizeof(name), static_cast<std::size_t>(-1), "%s", record.name);
    if (!g_playerNameDictionary->Contains(record.characterId)) {
      std::string text(name, sizeof(name));
      SetPlayerName(record.characterId, text.c_str());
    }

    MessengerMember member;
    member.name = name;
    member.accountId = record.accountId;
    member.characterId = record.characterId;
    member.offline = 1;
    g_session.AddFriend(member);
  }

  g_session.RefreshWindow02C8();
}

namespace {
#pragma pack(push, 1)
  struct SwapEquipmentRecord {
    /* 0000 */ std::uint16_t index;
    /* 0002 */ int location;
  };
#pragma pack(pop)

  static_assert(sizeof(SwapEquipmentRecord) == 6);

#pragma pack(push, 1)
  struct StatusBonusPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ int type;
    /* 0006 */ int value;
    /* 000A */ int bonus;
  };
#pragma pack(pop)

  static_assert(sizeof(StatusBonusPacket) == 0xE);
}

int GetEquipLocationIndex(const int& mask);

namespace EquipmentProperties {
class CEquipmentWearable;

class CEquipmentPropertiesMgr {
public:
  CEquipmentPropertiesMgr();
  virtual ~CEquipmentPropertiesMgr();
  static CEquipmentPropertiesMgr* GetInstance();
  void RefreshPretend();
  void RemoveExpansion(const int& kind, const int& itemId);
  void AddExpansion(const int& kind, const int& itemId, bool notify);
  void RefreshExpansion(const int& kind, const int& itemId);
  CEquipmentWearable* FindProperty1(const int& key);
  void UnequipWearable(const int& location);
  unsigned char unknown_0004[0x1F8];
};
}

// OFFSET: 0x00D4CAD0
// FUNCTION: fn_21869
void CGameMode::ReceiveStatusBonus(const unsigned char* data) {
  const StatusBonusPacket* packet = reinterpret_cast<const StatusBonusPacket*>(data);
  int* traitBonus = reinterpret_cast<int*>(g_session.unknown_7FC0 + 8);
  switch (packet->type) {
  case 13:
    g_session.m_standardStats[0] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.unknown_166C = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 14:
    g_session.m_standardStats[1] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.unknown_1670 = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 15:
    g_session.m_standardStats[2] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.unknown_1674 = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 16:
    g_session.m_standardStats[3] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.unknown_1678 = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 17:
    g_session.m_standardStats[4] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.unknown_167C = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 18:
    g_session.m_standardStats[5] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.unknown_1680 = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 219:
    g_session.SetTraitStat(219, packet->value);
    traitBonus[0] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 220:
    g_session.SetTraitStat(220, packet->value);
    traitBonus[1] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 221:
    g_session.SetTraitStat(221, packet->value);
    traitBonus[2] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 222:
    g_session.SetTraitStat(222, packet->value);
    traitBonus[3] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 223:
    g_session.SetTraitStat(223, packet->value);
    traitBonus[4] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case 224:
    g_session.SetTraitStat(224, packet->value);
    traitBonus[5] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  }

  switch (packet->type) {
  case 13:
  case 14:
  case 15:
  case 16:
  case 17:
  case 18:
  case 219:
  case 220:
  case 221:
  case 222:
  case 223:
  case 224:
    if (g_equipmentPropertiesMgr == nullptr) {
      g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
    }
    g_equipmentPropertiesMgr->RefreshPretend();
    break;
  }
}

// OFFSET: 0x00D8A600
// FUNCTION: fn_22220
void CGameMode::ReceiveSwapEquipmentList(const unsigned char* data) {
  g_session.ResetItems61B8();
  g_session.ResetItems6B68();

  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(SwapEquipmentRecord);
  const SwapEquipmentRecord* record = reinterpret_cast<const SwapEquipmentRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item = g_session.FindItemByIdentity(record->index, record->location);
    item.unknown_0000[3] = record->location;
    if (item.unknown_0000[1] == 0) {
      break;
    }

    if (g_session.IsCostumeLocation(record->location)) {
      g_session.EquipItem6B68(item);
    } else {
      g_session.EquipItem61B8(item);
    }
  }
}

// OFFSET: 0x00D8A730
// FUNCTION: fn_22221
void CGameMode::ProcessSkillUsePacket(const unsigned char* packet, int type) {
  std::uint32_t sourceAid;
  std::uint32_t targetAid;
  std::uint16_t x;
  std::uint16_t y;
  std::uint16_t skillId;
  int property;
  int delay;
  unsigned char flag = 0;
  if (type == 0) {
    sourceAid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
    targetAid = *reinterpret_cast<const std::uint32_t*>(packet + 6);
    x = *reinterpret_cast<const std::uint16_t*>(packet + 0xA);
    y = *reinterpret_cast<const std::uint16_t*>(packet + 0xC);
    skillId = *reinterpret_cast<const std::uint16_t*>(packet + 0xE);
    property = *reinterpret_cast<const int*>(packet + 0x10);
    delay = *reinterpret_cast<const int*>(packet + 0x14);
  } else if (type == 1) {
    sourceAid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
    targetAid = *reinterpret_cast<const std::uint32_t*>(packet + 6);
    x = *reinterpret_cast<const std::uint16_t*>(packet + 0xA);
    y = *reinterpret_cast<const std::uint16_t*>(packet + 0xC);
    skillId = *reinterpret_cast<const std::uint16_t*>(packet + 0xE);
    property = *reinterpret_cast<const int*>(packet + 0x10);
    delay = *reinterpret_cast<const int*>(packet + 0x14);
    flag = packet[0x18];
  } else {
    const std::uintptr_t value = reinterpret_cast<std::uintptr_t>(packet);
    sourceAid = static_cast<std::uint32_t>(value);
    targetAid = static_cast<std::uint32_t>(value);
    x = static_cast<std::uint16_t>(value);
    y = static_cast<std::uint16_t>(value);
    skillId = static_cast<std::uint16_t>(value);
    property = static_cast<int>(value);
    delay = static_cast<int>(value);
  }

  this->sub_D5B030(sourceAid, targetAid, x, y, skillId, property, delay, flag, 0);
}

namespace {
#pragma pack(push, 1)
  struct GuildMemberRecord {
    /* 0000 */ int accountId;
    /* 0004 */ int characterId;
    /* 0008 */ std::int16_t unknown_0008;
    /* 000A */ std::int16_t unknown_000A;
    /* 000C */ std::int16_t unknown_000C;
    /* 000E */ std::int16_t job;
    /* 0010 */ std::int16_t level;
    /* 0012 */ int exp;
    /* 0016 */ int online;
    /* 001A */ int positionId;
    /* 001E */ int unknown_001E;
  };
#pragma pack(pop)

  static_assert(sizeof(GuildMemberRecord) == 0x22);
}

// OFFSET: 0x00D89F20
// FUNCTION: fn_22215
void CGameMode::ReceiveGuildMemberList(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(GuildMemberRecord);
  guildInfo->unknown_0064 = count;
  guildInfo->ClearMembers();

  const GuildMemberRecord* record = reinterpret_cast<const GuildMemberRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.characterId = record->characterId;
    member.unknown_0020 = record->unknown_0008;
    member.unknown_0024 = record->unknown_000A;
    member.job = record->job;
    member.unknown_002C = record->unknown_000C;
    member.positionId = record->positionId;
    member.level = record->level;
    member.exp = record->exp;
    member.online = record->online;
    member.expRatio = 0.0f;
    *reinterpret_cast<int*>(member.unknown_0074) = record->unknown_001E;
    if (member.characterId == g_session.m_gid) {
      member.name = g_session.DecodePlayerName();
    }

    member.positionName.clear();
    guildInfo->AddMember(member);
  }

  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

namespace {
#pragma pack(push, 1)
  struct GuildMemberMemoRecord {
    /* 0000 */ int accountId;
    /* 0004 */ int characterId;
    /* 0008 */ std::int16_t unknown_0008;
    /* 000A */ std::int16_t unknown_000A;
    /* 000C */ std::int16_t unknown_000C;
    /* 000E */ std::int16_t job;
    /* 0010 */ std::int16_t level;
    /* 0012 */ int exp;
    /* 0016 */ int online;
    /* 001A */ int positionId;
    /* 001E */ char memo[50];
    /* 0050 */ char name[24];
  };
#pragma pack(pop)

  static_assert(sizeof(GuildMemberMemoRecord) == 0x68);
}

// OFFSET: 0x00D4EE00
// FUNCTION: fn_21880
void CGameMode::ReceiveGuildMemberMemoList(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(GuildMemberMemoRecord);
  guildInfo->unknown_0064 = count;
  guildInfo->ClearMembers();

  const GuildMemberMemoRecord* record = reinterpret_cast<const GuildMemberMemoRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.characterId = record->characterId;
    member.unknown_0020 = record->unknown_0008;
    member.unknown_0024 = record->unknown_000A;
    member.job = record->job;
    member.unknown_002C = record->unknown_000C;
    member.positionId = record->positionId;
    member.level = record->level;
    member.exp = record->exp;
    member.online = record->online;
    member.expRatio = 0.0f;

    char memo[51];
    std::memcpy(memo, record->memo, sizeof(record->memo));
    memo[50] = '\0';
    char name[25];
    std::memcpy(name, record->name, sizeof(record->name));
    name[24] = '\0';
    member.name = name;
    member.unknown_0050 = memo;
    member.positionName.clear();
    guildInfo->AddMember(member);
  }

  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D3A630
// FUNCTION: fn_21778
void CGameMode::ReceiveOpenStore(const unsigned char* packet) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    const ModeDragState* drag = &mode->drag;
    if (drag && drag->source) {
      this->SendMsg(81, 0, 0, 0, 0);
      return;
    }
  }

  const unsigned int count = packet[2];
  int& storeCount = *reinterpret_cast<int*>(&g_session.unknown_0F44[4]);
  if (count < 2) {
    storeCount = 2;
  } else {
    storeCount = count > 5 ? 5 : count;
  }
  g_session.ClearItems1768();
  if (!g_windowMgr.window_027C && !g_windowMgr.window_0274 && !g_windowMgr.unknown_02FC) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(174)) {
      static_cast<UIMerchantShopMakeWnd*>(window)->AddActionButton();
    }
    return;
  }

  g_windowMgr.Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(270, 0, 0, 0, 0);
}

struct ACTSTRUCT {
  virtual ~ACTSTRUCT();

  /* 0004 */ int damage;
  /* 0008 */ int damage2;
  /* 000C */ int type;
  /* 0010 */ std::uint32_t targetId;
  /* 0014 */ std::uint32_t tick;
  /* 0018 */ int count;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int attackMT;
  /* 0028 */ int attackedMT;
  /* 002C */ bool spDamaged;
};

// OFFSET: 0x00D051F0
// FUNCTION: fn_21563
ACTSTRUCT::~ACTSTRUCT() = default;
// OFFSET: 0x00D3A770
// FUNCTION: fn_21779
void CGameMode::ReceiveSearchStoreOpen(const unsigned char* packet) {
  g_session.unknown_5BE0 = packet[4];
  reinterpret_cast<std::int16_t*>(&g_session.unknown_5BE4)[1] = *reinterpret_cast<const std::int16_t*>(packet + 2);
  g_windowMgr.EnsureWindow(181);
}

#pragma pack(push, 1)
struct ParChangePacket {
  /* 0000 */ std::uint16_t header;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ std::int32_t value;
};
#pragma pack(pop)

static_assert(sizeof(ParChangePacket) == 8);

// OFFSET: 0x00D3A7A0
// FUNCTION: fn_21780
void CGameMode::ReceiveParChange(unsigned char* data) {
  auto* packet = reinterpret_cast<ParChangePacket*>(data);
  int* derivedStats = reinterpret_cast<int*>(g_session.unknown_169C);
  std::string text;

  g_modeMgr.GetGameMode();

  switch (packet->type) {
  case 0:
    if (*reinterpret_cast<int*>(&g_session.unknown_16F4[0]) != packet->value) {
      *reinterpret_cast<int*>(&g_session.unknown_16F4[0]) = packet->value;
      g_session.InvalidateWindowFrames();
    }
    break;
  case 3:
    *reinterpret_cast<int*>(&g_session.unknown_1704[0]) = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 4:
    if (packet->value < 0) {
      text = FormatOwnedString(LookupMessage(664), -packet->value);
      const int emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::ChatProhibit);
      this->scene->player->SendMsg(nullptr, 160, 0, 0, 0, emotion, emotion >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      text = LookupMessage(665);
      this->scene->player->SendMsg(nullptr, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
    *reinterpret_cast<int*>(&g_session.unknown_16F4[4]) = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 5:
    if (g_session.unknown_556C > 0 && (packet->value * 100) / g_session.unknown_556C < 25 &&
        this->scene->GetGameActorByAID(g_session.unknown_1010) && rand() % 100 < 50 &&
        g_session.CheckInterval(5000)) {
      const int hungerLevel = g_session.GetPetHungryState(g_session.unknown_1044);
      const int friendlyLevel = g_session.GetPetFriendlyState(g_session.m_petFriendly);
      this->SendMsg(148, g_session.GetPetEmotionNumber(g_session.unknown_103C, 2, hungerLevel), 0, 0, 0);
      this->SendMsg(148, g_session.GetPetActionNumber(hungerLevel, friendlyLevel, 2), 0, 0, 0);
    }
    if (g_session.unknown_5568 != packet->value) {
      g_session.unknown_5568 = packet->value;
      g_session.RefreshHpSpWindows();
    }
    break;
  case 6:
    if (g_session.unknown_556C != packet->value) {
      g_session.unknown_556C = packet->value;
      g_session.RefreshHpSpWindows();
    }
    if (g_windowMgr.window_02C8) {
      g_windowMgr.window_02C8->SendMsg(nullptr, 6, 34, g_session.m_aid, 0, 0);
    }
    break;
  case 7:
    if (*reinterpret_cast<int*>(&g_session.unknown_5570[0]) != packet->value) {
      *reinterpret_cast<int*>(&g_session.unknown_5570[0]) = packet->value;
      g_session.RefreshHpSpWindows();
    }
    break;
  case 8:
    if (*reinterpret_cast<int*>(&g_session.unknown_5570[4]) != packet->value) {
      *reinterpret_cast<int*>(&g_session.unknown_5570[4]) = packet->value;
      g_session.RefreshHpSpWindows();
    }
    break;
  case 9:
    *reinterpret_cast<int*>(&g_session.unknown_1654[0]) = packet->value;
    g_session.InvalidateWindowFrames();
    if (g_windowMgr.unknown_01C4) {
      g_windowMgr.unknown_01C4->SendMsg(nullptr, 35, 0, 0, 0, 0);
    }
    break;
  case 11: {
    g_session.m_level = packet->value;
    g_session.InvalidateWindowFrames();
    this->scene->player->unknown_0210 = static_cast<std::uint16_t>(g_session.m_level);
    g_soundMgr->PlayWave("levelup.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);

    const int job = g_session.GetDisplayJob();
    int effectId;
    if (g_session.IsSuperNoviceClass(job)) {
      effectId = 338;
    } else if (job < 4303) {
      if (job == 4302) {
        effectId = 582;
      } else {
        switch (job) {
        case 4046:
        case 4047:
        case 4048:
        case 4049:
        case 4225:
        case 4226:
        case 4227:
        case 4238:
        case 4239:
        case 4240:
        case 4241:
        case 4242:
        case 4243:
        case 4244:
          effectId = 582;
          break;
        case 4050:
        case 4051:
        case 4052:
          effectId = 338;
          break;
        default:
          effectId = 371;
          break;
        }
      }
    } else if (job == 4303 || job == 4316) {
      effectId = 582;
    } else {
      effectId = 371;
    }
    this->scene->player->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);

    if (g_replayAnimationScaling == 0) {
      g_windowMgr.EnsureWindow(21);
    }
    if (this->scene->GetGameActorByAID(g_session.unknown_1010)) {
      const int hungerLevel = g_session.GetPetHungryState(g_session.unknown_1044);
      const int friendlyLevel = g_session.GetPetFriendlyState(g_session.m_petFriendly);
      this->SendMsg(148, g_session.GetPetEmotionNumber(g_session.unknown_103C, 6, hungerLevel), 0, 0, 0);
      this->SendMsg(148, g_session.GetPetActionNumber(hungerLevel, friendlyLevel, 6), 0, 0, 0);
    }
    if (g_replayAnimationScaling == 0) {
      if (g_windowMgr.unknown_01DC) {
        reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC)->SetHeight(g_windowMgr.unknown_01DC->geometry.height);
      }
    }
    this->RefreshActorJobEffects(true);
    if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
      window->SendMsg(nullptr, 6, 286, 192, 1, 0);
    }
    break;
  }
  case 12:
    g_session.m_skillPoint = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.NotifySkillWindows();
    break;
  case 13:
    g_session.m_standardStats[0] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 14:
    g_session.m_standardStats[1] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 15:
    g_session.m_standardStats[2] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 16:
    g_session.m_standardStats[3] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 17:
    g_session.m_standardStats[4] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 18:
    g_session.m_standardStats[5] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 24: {
    packet->value = packet->value / 10;
    const int oldPercent = (g_session.m_weight * 100) / (g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight);
    g_session.m_weight = packet->value;
    g_session.RefreshHpSpWindows();
    g_session.InvalidateWindowFrames();
    if (g_session.m_maxWeight == 0) {
      break;
    }

    const int percent = (g_session.m_weight * 100) / (g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight);
    if (oldPercent < percent) {
      if (percent >= g_unknown173A5BC && oldPercent < g_unknown173A5BC) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        text = FormatOwnedString(LookupMessage(3311), g_unknown173A5BC);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 8, 0, 0);
      }
      if (percent > 89 && oldPercent < 90) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(307)), 0xFF, 8, 0, 0);
      }
      break;
    }
    if (oldPercent <= percent) {
      break;
    }
    if (percent < g_unknown173A5BC && g_unknown173A5BC <= oldPercent) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(308)), 0xFFFF00, 8, 0, 0);
    }
    if (percent > 89) {
      break;
    }
    if (oldPercent >= 90) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(309)), 0xFFFF00, 8, 0, 0);
    }
    break;
  }
  case 25: {
    packet->value = packet->value / 10;
    const int oldMaxWeight = g_session.m_maxWeight;
    if (g_session.m_maxWeight < 1) {
      g_session.m_maxWeight = 1;
    }
    const int oldPercent = (g_session.m_weight * 100) / g_session.m_maxWeight;
    g_session.m_maxWeight = packet->value;
    g_session.RefreshHpSpWindows();
    g_session.InvalidateWindowFrames();
    if (oldMaxWeight == 0 || g_session.m_maxWeight <= oldMaxWeight) {
      break;
    }

    const int percent = (g_session.m_weight * 100) / (g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight);
    if (percent < 50) {
      if (oldPercent > 49) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(308)), 0xFFFF00, 8, 0, 0);
      }
    } else if (percent > 89) {
      break;
    }
    if (oldPercent >= 90) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(309)), 0xFFFF00, 8, 0, 0);
    }
    break;
  }
  case 41:
    derivedStats[7] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 42:
    derivedStats[12] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 43:
    derivedStats[13] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 44:
    derivedStats[14] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 45:
    derivedStats[10] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 46:
    derivedStats[11] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 47:
    derivedStats[8] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 48:
    derivedStats[15] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 49:
    derivedStats[16] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 50:
    derivedStats[17] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 51:
    derivedStats[19] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 52:
    derivedStats[18] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 53:
    derivedStats[6] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 54:
    derivedStats[9] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case 55:
    g_session.m_jobLevel = packet->value;
    g_session.InvalidateWindowFrames();
    this->scene->player->LaunchEffect(packet->value < 51 ? 158 : 337, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);

    if (g_replayAnimationScaling == 0) {
      g_windowMgr.EnsureWindow(49);
      if (g_windowMgr.unknown_01DC) {
        reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC)->SetHeight(g_windowMgr.unknown_01DC->geometry.height);
      }
    }
    if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
      window->SendMsg(nullptr, 6, 286, 196, 1, 0);
    }
    break;
  case 60:
    if (packet->value < -60) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(858)), 0xFFFF, 0, 0, 0);
    }
    break;
  case 169:
    reinterpret_cast<bool&>(g_session.unknown_8128[4]) = packet->value != 0;
    break;
  case 219:
  case 220:
  case 221:
  case 222:
  case 223:
  case 224:
  case 225:
  case 226:
  case 227:
  case 228:
  case 229:
  case 230:
  case 231:
  case 232:
  case 233:
    g_session.SetTraitStat(packet->type, packet->value);
    if (g_windowMgr.unknown_01C4) {
      static_cast<UIStatusWnd*>(g_windowMgr.unknown_01C4)->ValidatePlacement();
    }
    g_session.InvalidateWindowFrames();
    break;
  }

  if (packet->type == 11 || packet->type == 55) {
    if (g_equipmentPropertiesMgr == nullptr) {
      g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
    }
    g_equipmentPropertiesMgr->RefreshPretend();
  }
}

// OFFSET: 0x00D55AB0
// FUNCTION: fn_21953
void CGameMode::ApplyStatusChange(
  std::uint32_t aid, unsigned char on, int type, std::uint32_t remaining, int value1, int value2, int value3, unsigned char flag, std::uint32_t total
) {
  std::uint32_t now;
  if (g_replayAnimationScaling != 0) {
    now = Replay::Instance().elapsed;
  } else {
    now = timeGetTime();
  }

  if (type == 64) {
    type = value1 + 897;
  }

  if (aid != g_session.m_aid) {
    CRenderObject* actor = this->scene->GetGameActorByAID(aid);
    if (actor) {
      actor->SendMsg(
        nullptr, 110, 0, type, type >> 31, on, 0, value1, value1 >> 31, value2, value2 >> 31, flag, 0, value3, value3 >> 31
      );
    }

    if (type == 432 && actor) {
      const unsigned char element = static_cast<unsigned char>(value3);
      if (element == 19) {
        g_effectMgr->Spawn(actor->render.accountId, 1665, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1666, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (element == 20) {
        g_effectMgr->Spawn(actor->render.accountId, 1669, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (element == 14) {
        g_effectMgr->Spawn(actor->render.accountId, 1667, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1668, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
  }

  MessengerMember member = g_session.PartyByAccount(aid);
  if (aid != g_session.m_aid && (!member.valid || member.offline)) {
    return;
  }

  char name[256];
  std::sprintf(name, "");
  if (aid != g_session.m_aid) {
    std::sprintf(name, LookupMessage(1275), member.name.c_str());
  }

  char message[256];
  std::memset(message, 0, sizeof(message));

  std::uint32_t color;
  if (on) {
    if (aid == g_session.m_aid) {
      if (type == 696) {
        if (this->scene->player) {
          this->scene->player->SetUnknown0305(1, value2);
        }
      } else if (type == 46) {
        ShortcutCooldownRecord timer;
        timer.id = 577;
        timer.end = now + remaining;
        timer.duration = total;
        g_session.m_requestTimers.push_back(timer);
      } else if (type == 684) {
        ShortcutCooldownRecord timer;
        timer.id = 9999;
        timer.end = now + remaining;
        timer.duration = total;
        g_session.m_requestTimers.push_back(timer);
      } else if (type == 175) {
        ShortcutCooldownRecord timer;
        for (int requestId = 10000; requestId < 10020; ++requestId) {
          std::list<ShortcutCooldownRecord>::iterator it = g_session.m_requestTimers.begin();
          for (; it != g_session.m_requestTimers.end(); ++it) {
            if (it->id == requestId) {
              break;
            }
          }

          timer.id = requestId;
          timer.end = now + remaining;
          timer.duration = total;
          if (it == g_session.m_requestTimers.end()) {
            g_session.m_requestTimers.push_back(timer);
          } else {
            it->end = timer.end;
          }
        }
      } else if (type == 824) {
        g_rodexMailStore->unknown_0014 = value2;
      }

      if (type != 824) {
        ActiveSkillEffect effect;
        effect.id = type;
        effect.unknown_0004 = remaining == 9999 ? 999999 : now + remaining;
        if (type >= 363 && type <= 367) {
          effect.unknown_0008 = value1 & 3;
        } else if (type == 339 || type == 895 || type == 1149) {
          effect.unknown_0008 = value1;
        }

        if (type == 1152 || type == 1196) {
          effect.unknown_0008 = value1;
        } else if (type == 1244) {
          effect.unknown_0008 = value2;
        } else if (CSkillActionMgr::GetInstance()->HasEfstAction(type) == true) {
          effect.unknown_000C = value2;
          effect.unknown_0008 = value1;
          effect.unknown_0010 = value3;
        }

        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == type && (it->id != 895 || it->unknown_0008 == effect.unknown_0008)) {
            g_cooldownOwner->Remove(it->id);
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }

        effect.unknown_0014 = total;
        effect.unknown_0018 = remaining;
        if (effect.unknown_0004 != 999999) {
          g_cooldownOwner->Register(effect.id, total, now, total - remaining);
        }
        g_activeSkillEffects.push_back(effect);
      }
    }

    color = 0xFFFF00;
    switch (type) {
    case 2:
    case 37:
    case 38:
    case 39:
    case 40:
    case 68:
    case 161:
      std::sprintf(message, LookupMessage(463));
      break;
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 363:
    case 364:
    case 365:
    case 366:
    case 367:
    case 448:
    case 452:
    case 453:
    case 454:
    case 645:
    case 646:
    case 655:
    case 656:
    case 657:
    case 659:
    case 660:
    case 674:
    case 716:
    case 717:
    case 718:
    case 719:
    case 720:
    case 721:
    case 728:
    case 730:
    case 734:
    case 735:
    case 736:
    case 737:
    case 755:
    case 760:
    case 779:
    case 822:
    case 823:
    case 825:
    case 826:
    case 849:
    case 856:
    case 857:
    case 859:
    case 863:
    case 864:
    case 872:
    case 895:
    case 896:
    case 917:
    case 919:
    case 937:
    case 982:
    case 1016:
    case 1017:
    case 1018:
    case 1019:
    case 1020:
    case 1021:
    case 1022:
    case 1023:
    case 1024:
    case 1025:
    case 1038:
    case 1039:
    case 1040:
    case 1041:
    case 1043:
    case 1050:
    case 1055:
    case 1126:
    case 1139:
    case 1141:
    case 1142:
    case 1148:
    case 1150:
    case 1165:
    case 1166:
    case 1167:
    case 1176:
    case 1184:
    case 1190:
    case 1196:
    case 1197:
    case 1205:
    case 1206:
    case 1207:
    case 1208:
    case 1209:
    case 1210:
    case 1211:
    case 1212:
    case 1213:
    case 1214:
    case 1215:
    case 1216:
    case 1230:
    case 1231:
    case 1232:
    case 1233:
    case 1234:
    case 1258:
    case 1318:
    case 1319:
    case 1337:
    case 1338:
    case 1339:
    case 1688:
      break;
    case 339:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        for (int i = 0; i < 10; ++i) {
          g_effectMgr->Retire(player->render.accountId, 1300 + i);
          g_effectMgr->Retire(player->render.accountId, 1467 + i);
        }
        g_effectMgr->Spawn(
          player->render.accountId, g_serviceOptions.Get(229) ? value1 + 1466 : value1 + 1299, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
        );
        std::sprintf(message, "");
      }
      break;
    case 434:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, 109, 0, 879, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        player->LaunchEffect(879, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case 439:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, 109, 0, 865, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        player->LaunchEffect(865, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case 441:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, 109, 0, 867, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        player->LaunchEffect(867, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case 451:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, 109, 0, 871, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        player->LaunchEffect(871, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case 435:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        std::sprintf(message, "");
      }
      break;
    case 433:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 109, 0, 453, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        std::sprintf(message, "");
      }
      break;
    case 358:
      if (aid == g_session.m_aid) {
        this->scene->player->LaunchEffect(752, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case 15:
      std::sprintf(message, LookupMessage(3463));
      break;
    case 16:
      std::sprintf(message, LookupMessage(467));
      break;
    case 6:
      std::sprintf(message, LookupMessage(469));
      break;
    case 17:
    case 64:
      std::sprintf(message, LookupMessage(470));
      break;
    case 18:
      std::sprintf(message, LookupMessage(472));
      break;
    case 19:
      std::sprintf(message, LookupMessage(474));
      break;
    case 24:
      std::sprintf(message, LookupMessage(476));
      break;
    case 25:
    case 188:
      std::sprintf(message, LookupMessage(465));
      break;
    case 26:
      std::sprintf(message, LookupMessage(480));
      break;
    case 12:
    case 41:
    case 42:
      std::sprintf(message, LookupMessage(209));
      break;
    case 13:
      std::sprintf(message, LookupMessage(210));
      break;
    case 7:
      std::sprintf(message, LookupMessage(620));
      break;
    case 8:
      std::sprintf(message, LookupMessage(622));
      break;
    case 9:
      std::sprintf(message, LookupMessage(3435));
      break;
    case 10:
      std::sprintf(message, LookupMessage(3434));
      break;
    case 14:
      std::sprintf(message, LookupMessage(630));
      break;
    case 20:
      std::sprintf(message, LookupMessage(632));
      break;
    case 21:
      std::sprintf(message, LookupMessage(634));
      break;
    case 22:
      std::sprintf(message, LookupMessage(636));
      break;
    case 23:
    case 147:
      std::sprintf(message, LookupMessage(638));
      break;
    case 29:
      std::sprintf(message, "");
      if (aid == g_session.m_aid) {
        this->scene->player->SetAction(64, 4, 1);
        this->scene->player->render.isMotionFreezed = true;
      }
      break;
    case 30:
      std::sprintf(message, LookupMessage(3433));
      break;
    case 31:
      std::sprintf(message, LookupMessage(648));
      break;
    case 32:
      std::sprintf(message, LookupMessage(650));
      break;
    case 33:
      std::sprintf(message, LookupMessage(652));
      break;
    case 182:
      if (aid == g_session.m_aid) {
        g_soundMgr->PlayWave("lava_golem_move.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
      std::sprintf(message, "");
      break;
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 11:
    case 27:
    case 28:
    case 34:
    case 54:
    case 55:
    case 56:
    case 57:
    case 61:
    case 86:
    case 87:
    case 95:
    case 97:
    case 98:
    case 103:
    case 104:
    case 105:
    case 107:
    case 110:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
    case 120:
    case 121:
    case 122:
    case 123:
    case 124:
    case 125:
    case 130:
    case 132:
    case 133:
    case 134:
    case 135:
    case 137:
    case 139:
    case 141:
    case 143:
    case 145:
    case 149:
    case 150:
    case 151:
    case 152:
    case 153:
    case 156:
    case 157:
    case 158:
    case 159:
    case 160:
    case 165:
    case 166:
    case 167:
    case 169:
    case 170:
    case 171:
    case 181:
    case 184:
    case 186:
    case 191:
    case 197:
    case 198:
    case 199:
    case 200:
    case 201:
    case 203:
    case 204:
    case 205:
    case 206:
    case 207:
    case 208:
    case 209:
    case 210:
    case 211:
    case 212:
    case 213:
    case 214:
    case 215:
    case 216:
    case 217:
    case 218:
    case 219:
    case 220:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 229:
    case 230:
    case 231:
    case 232:
    case 233:
    case 234:
    case 235:
    case 236:
    case 238:
    case 239:
    case 240:
    case 254:
    case 255:
    case 256:
    case 257:
    case 258:
    case 259:
    case 261:
    case 262:
    case 263:
    case 264:
    case 265:
    case 266:
    case 267:
    case 268:
    case 269:
    case 270:
    case 282:
    case 286:
    case 289:
    case 290:
    case 291:
    case 292:
    case 293:
    case 294:
    case 295:
    case 296:
    case 297:
    case 298:
    case 299:
    case 300:
    case 301:
    case 302:
    case 313:
    case 314:
    case 316:
    case 318:
    case 319:
    case 320:
    case 321:
    case 322:
    case 323:
    case 328:
    case 330:
    case 333:
    case 334:
    case 336:
    case 337:
    case 341:
    case 342:
    case 343:
    case 344:
    case 345:
    case 346:
    case 347:
    case 348:
    case 349:
    case 350:
    case 351:
    case 352:
    case 353:
    case 355:
    case 357:
    case 360:
    case 361:
    case 362:
    case 374:
    case 375:
    case 377:
    case 379:
    case 381:
    case 382:
    case 383:
    case 384:
    case 385:
    case 386:
    case 387:
    case 388:
    case 389:
    case 390:
    case 393:
    case 394:
    case 396:
    case 397:
    case 398:
    case 399:
    case 401:
    case 402:
    case 403:
    case 404:
    case 405:
    case 407:
    case 408:
    case 409:
    case 410:
    case 411:
    case 412:
    case 413:
    case 414:
    case 415:
    case 416:
    case 417:
    case 418:
    case 419:
    case 421:
    case 422:
    case 423:
    case 425:
    case 426:
    case 427:
    case 429:
    case 430:
    case 436:
    case 437:
    case 438:
    case 440:
    case 442:
    case 443:
    case 444:
    case 445:
    case 446:
    case 447:
    case 450:
    case 461:
    case 463:
    case 464:
    case 472:
    case 473:
    case 476:
    case 491:
    case 492:
    case 493:
    case 494:
    case 495:
    case 496:
    case 571:
    case 577:
    case 580:
    case 581:
    case 584:
    case 605:
    case 606:
    case 607:
    case 608:
    case 611:
    case 613:
    case 618:
    case 648:
    case 649:
    case 670:
    case 869:
    case 1056:
    case 1057:
    case 1058:
    case 1059:
    case 1121:
      std::sprintf(message, "");
      break;
    case 354:
      if (aid == g_session.m_aid) {
        this->scene->player->LaunchEffect(729, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      std::printf(message, "");
      break;
    case 260:
      if (aid == g_session.m_aid) {
        g_session.unknown_5450 = 1;
      }
      std::sprintf(message, "");
      break;
    case 241:
    case 271:
      std::sprintf(message, LookupMessage(1231));
      break;
    case 242:
    case 272:
      std::sprintf(message, LookupMessage(1233));
      break;
    case 243:
    case 273:
      std::sprintf(message, LookupMessage(1235));
      break;
    case 244:
    case 274:
      std::sprintf(message, LookupMessage(1239));
      break;
    case 245:
    case 275:
      std::sprintf(message, LookupMessage(1237));
      break;
    case 246:
    case 276:
      std::sprintf(message, LookupMessage(1241));
      break;
    case 247:
      std::sprintf(message, LookupMessage(1243));
      break;
    case 248:
      std::sprintf(message, LookupMessage(1245));
      break;
    case 249:
      std::sprintf(message, LookupMessage(1247));
      break;
    case 250:
    {
      const int minutes = remaining / 60000;
      if (minutes > 0) {
        const std::string text = FormatOwnedString(LookupMessage(2031), minutes, value1);
        std::strcpy(message, text.c_str());
      } else {
        message[0] = '\0';
      }
      break;
    }
    case 251:
      std::sprintf(message, LookupMessage(1250));
      break;
    case 252:
    {
      const int minutes = remaining / 60000;
      if (minutes > 0) {
        const int hours = minutes / 60;
        if (hours > 0) {
          const std::string text = FormatOwnedString(LookupMessage(2472), hours, minutes % 60, value1);
          std::strcpy(message, text.c_str());
        } else {
          const std::string text = FormatOwnedString(LookupMessage(2471), minutes, value1);
          std::strcpy(message, text.c_str());
        }
      } else {
        message[0] = '\0';
      }
      break;
    }
    case 253:
      if (aid == g_session.m_aid) {
        this->unknown_05CC[0] = 1;
      }
      std::sprintf(message, LookupMessage(1252));
      break;
    case 312:
    {
      const int minutes = remaining / 60000;
      if (minutes > 0) {
        const std::string text = FormatOwnedString(LookupMessage(2462), minutes, value1);
        std::strcpy(message, text.c_str());
      } else {
        message[0] = '\0';
      }
      break;
    }
    case 636:
    {
      const std::string text = FormatOwnedString(LookupMessage(1924), value2, value1 / 100.0 + 1.0);
      std::sprintf(message, text.c_str());
      break;
    }
    case 621:
    case 651:
    case 665:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 110, 0, type, type >> 31, on, 0, value1, value1 >> 31, 0, 0, 0, 0, 0, 0);
        this->sub_CFE480(aid);
      }
      return;
    case 673:
      if (aid == g_session.m_aid) {
        g_session.SetUnknown5C1C(value1);
      }
      break;
    case 675:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 110, 0, 675, 0, on, 0, value1, value1 >> 31, 0, 0, 0, 0, 0, 0);
        this->sub_CFE480(aid);
      }
      return;
    case 676:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 110, 0, 676, 0, on, 0, value1, value1 >> 31, 0, 0, 0, 0, 0, 0);
        this->sub_CFE480(aid);
      }
      break;
    case 710:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 110, 0, 710, 0, on, 0, value1, value1 >> 31, 0, 0, 0, 0, 0, 0);
        this->sub_CFE480(aid);
      }
      break;
    case 933:
      if (aid == g_session.m_aid) {
        this->scene->player->Slot15(48, 1);
        this->scene->player->render.isMotionFreezed = true;
      }
      break;
    case 893:
      if (aid == g_session.m_aid) {
        this->scene->GetGameActorByAID(aid);
        this->scene->player->Slot15(47, 1);
      }
      break;
    case 373:
      g_session.unknown_7ED4 = timeGetTime();
      g_session.unknown_7ED0 = value2 == 0 ? value1 : 0;
      if (value1 == 0) {
        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == 373) {
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }
      }
      break;
    case 1013:
      color = 0xFF;
      std::sprintf(message, LookupMessage(3247));
      break;
    case 1125:
      g_effectMgr->Spawn(aid, 1362, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\s포이즌버스터.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case 1149:
    {
      CGameActor* actor;
      if (aid == g_session.m_aid) {
        actor = this->scene->player;
      } else {
        actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
      }
      if (!actor) {
        break;
      }

      if (value1 == 2) {
        actor->Slot57(2);
        if (aid == g_session.m_aid) {
          g_session.unknown_8074 = 2;
        }
      } else {
        actor->Slot57(1);
        if (aid == g_session.m_aid) {
          g_session.unknown_8074 = 1;
        }
      }
      actor->SendMsg(nullptr, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 432:
    {
      CRenderObject* actor = this->scene->GetGameActorByAidIncludeMe(aid);
      if (!actor) {
        break;
      }

      const unsigned char element = static_cast<unsigned char>(value3);
      if (element == 19) {
        g_effectMgr->Spawn(actor->render.accountId, 1665, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1666, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (element == 20) {
        g_effectMgr->Spawn(actor->render.accountId, 1669, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (element == 14) {
        g_effectMgr->Spawn(actor->render.accountId, 1667, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1668, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      break;
    }
    case 1160:
      _snprintf_s(message, 256, LookupMessage(3726));
      break;
    case 1162:
      _snprintf_s(message, 256, LookupMessage(3728));
      break;
    case 1161:
      _snprintf_s(message, 256, LookupMessage(3730));
      break;
    case 1228:
      _snprintf_s(message, 256, LookupMessage(3732));
      break;
    case 1227:
      _snprintf_s(message, 256, LookupMessage(3734));
      break;
    case 1201:
      _snprintf_s(message, 256, LookupMessage(3736));
      break;
    case 1200:
      _snprintf_s(message, 256, LookupMessage(3738));
      break;
    case 1199:
      _snprintf_s(message, 256, LookupMessage(3740));
      break;
    case 1198:
      _snprintf_s(message, 256, LookupMessage(3742));
      break;
    case 1221:
      _snprintf_s(message, 256, LookupMessage(3758));
      break;
    case 1152:
      _snprintf_s(message, 256, LookupMessage(3760));
      break;
    case 1194:
      _snprintf_s(message, 256, LookupMessage(3764));
      break;
    case 1247:
      _snprintf_s(message, 256, LookupMessage(3782));
      break;
    case 1248:
      _snprintf_s(message, 256, LookupMessage(3778));
      break;
    case 1249:
      _snprintf_s(message, 256, LookupMessage(3784));
      break;
    case 1237:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(3788));
      break;
    case 1236:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(3786));
      break;
    case 1261:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(3799));
      break;
    case 1250:
      color = 0xFF;
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(3807));
      break;
    case 1245:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(3815));
      break;
    case 1271:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(3812));
      break;
    default:
      if (CSkillActionMgr::GetInstance()->HasEfstAction(type) != true) {
        this->sub_CFE480(aid);
        return;
      }
      {
        const EfstActionMessage action = CSkillActionMgr::GetInstance()->EfstSlot03(type);
        if (action.text) {
          color = action.color;
          StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", action.text);
        }
      }
      break;
    }
  } else {
    if (aid == g_session.m_aid) {
      for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
        if (it->id == type && (it->id != 895 || it->unknown_0008 == value1)) {
          g_activeSkillEffects.erase(it);
          it = g_activeSkillEffects.begin();
        } else {
          ++it;
        }
      }
    }

    color = 0xFF;
    switch (type) {
    case 2:
    case 37:
    case 38:
    case 39:
    case 68:
    case 161:
      std::sprintf(message, LookupMessage(464));
      break;
    case 15:
      std::sprintf(message, LookupMessage(3464));
      break;
    case 16:
      std::sprintf(message, LookupMessage(468));
      break;
    case 6:
    case 17:
    case 64:
      std::sprintf(message, LookupMessage(471));
      break;
    case 18:
      std::sprintf(message, LookupMessage(473));
      break;
    case 19:
      std::sprintf(message, LookupMessage(475));
      break;
    case 24:
      std::sprintf(message, LookupMessage(477));
      break;
    case 25:
    case 188:
      std::sprintf(message, LookupMessage(466));
      break;
    case 26:
      std::sprintf(message, LookupMessage(481));
      break;
    case 13:
      std::sprintf(message, LookupMessage(209));
      break;
    case 7:
      std::sprintf(message, LookupMessage(621));
      break;
    case 8:
      std::sprintf(message, LookupMessage(623));
      break;
    case 9:
      std::sprintf(message, LookupMessage(625));
      break;
    case 10:
      std::sprintf(message, LookupMessage(627));
      break;
    case 14:
      std::sprintf(message, LookupMessage(631));
      break;
    case 20:
      std::sprintf(message, LookupMessage(633));
      break;
    case 21:
      std::sprintf(message, LookupMessage(635));
      break;
    case 22:
      std::sprintf(message, LookupMessage(637));
      break;
    case 23:
    case 147:
      std::sprintf(message, LookupMessage(639));
      break;
    case 29:
      std::sprintf(message, "");
      if (aid == g_session.m_aid) {
        this->scene->player->Slot15(0, 1);
        this->scene->player->render.isMotionFreezed = false;
      }
      break;
    case 30:
      std::sprintf(message, LookupMessage(647));
      break;
    case 31:
      std::sprintf(message, LookupMessage(649));
      break;
    case 40:
    case 12:
    case 42:
      if (aid == g_session.m_aid) {
        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == type) {
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }
      }
      std::sprintf(message, LookupMessage(210));
      break;
    case 41:
      std::sprintf(message, LookupMessage(210));
      break;
    case 363:
    case 364:
    case 365:
    case 366:
    case 367:
      if (aid != g_session.m_aid) {
        break;
      }
      {
        CPlayer* player = this->scene->player;
        const int stage = value1 & 3;
        if (stage == 0) {
          player->SendMsg(nullptr, 109, 0, 735, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        } else if (stage == 1) {
          player->SendMsg(nullptr, 109, 0, 736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        } else if (stage == 2) {
          player->SendMsg(nullptr, 109, 0, 737, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        } else if (stage == 3) {
          player->SendMsg(nullptr, 109, 0, 738, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
      }
      std::sprintf(message, "");
      break;
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 11:
    case 27:
    case 28:
    case 32:
    case 33:
    case 34:
    case 54:
    case 55:
    case 56:
    case 57:
    case 86:
    case 87:
    case 95:
    case 97:
    case 98:
    case 103:
    case 104:
    case 105:
    case 107:
    case 110:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
    case 120:
    case 121:
    case 122:
    case 123:
    case 124:
    case 125:
    case 130:
    case 132:
    case 133:
    case 134:
    case 135:
    case 137:
    case 139:
    case 141:
    case 143:
    case 145:
    case 149:
    case 150:
    case 151:
    case 152:
    case 153:
    case 156:
    case 157:
    case 158:
    case 159:
    case 160:
    case 165:
    case 166:
    case 167:
    case 169:
    case 170:
    case 171:
    case 181:
    case 182:
    case 184:
    case 186:
    case 191:
    case 197:
    case 198:
    case 199:
    case 200:
    case 201:
    case 203:
    case 204:
    case 205:
    case 206:
    case 207:
    case 208:
    case 209:
    case 210:
    case 211:
    case 212:
    case 213:
    case 214:
    case 215:
    case 216:
    case 217:
    case 218:
    case 219:
    case 220:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 229:
    case 230:
    case 231:
    case 232:
    case 233:
    case 234:
    case 235:
    case 236:
    case 238:
    case 239:
    case 240:
    case 250:
    case 251:
    case 252:
    case 254:
    case 255:
    case 256:
    case 257:
    case 258:
    case 259:
    case 261:
    case 262:
    case 263:
    case 264:
    case 265:
    case 266:
    case 267:
    case 268:
    case 269:
    case 270:
    case 282:
    case 286:
    case 289:
    case 290:
    case 291:
    case 292:
    case 293:
    case 294:
    case 295:
    case 296:
    case 297:
    case 298:
    case 299:
    case 300:
    case 301:
    case 302:
    case 312:
    case 313:
    case 314:
    case 316:
    case 318:
    case 319:
    case 320:
    case 321:
    case 322:
    case 323:
    case 328:
    case 330:
    case 333:
    case 334:
    case 336:
    case 337:
    case 341:
    case 342:
    case 343:
    case 344:
    case 345:
    case 346:
    case 347:
    case 348:
    case 349:
    case 350:
    case 351:
    case 352:
    case 353:
    case 354:
    case 355:
    case 357:
    case 360:
    case 361:
    case 362:
    case 373:
    case 374:
    case 375:
    case 377:
    case 379:
    case 381:
    case 382:
    case 383:
    case 384:
    case 385:
    case 386:
    case 387:
    case 388:
    case 389:
    case 390:
    case 393:
    case 394:
    case 396:
    case 397:
    case 398:
    case 399:
    case 401:
    case 402:
    case 403:
    case 404:
    case 405:
    case 407:
    case 408:
    case 409:
    case 410:
    case 411:
    case 412:
    case 413:
    case 414:
    case 415:
    case 416:
    case 417:
    case 418:
    case 419:
    case 421:
    case 422:
    case 423:
    case 425:
    case 426:
    case 427:
    case 429:
    case 430:
    case 436:
    case 437:
    case 438:
    case 439:
    case 440:
    case 441:
    case 442:
    case 443:
    case 444:
    case 445:
    case 446:
    case 447:
    case 450:
    case 451:
    case 461:
    case 463:
    case 464:
    case 472:
    case 473:
    case 476:
    case 491:
    case 492:
    case 493:
    case 494:
    case 495:
    case 496:
    case 571:
    case 577:
    case 580:
    case 581:
    case 584:
    case 605:
    case 606:
    case 607:
    case 608:
    case 611:
    case 613:
    case 618:
    case 636:
    case 648:
    case 649:
    case 670:
    case 869:
    case 1056:
    case 1057:
    case 1058:
    case 1059:
    case 1121:
      std::sprintf(message, "");
      break;
    case 339:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        for (int i = 0; i < 10; ++i) {
          g_effectMgr->Retire(player->render.accountId, 1300 + i);
          g_effectMgr->Retire(player->render.accountId, 1467 + i);
        }
      }
      std::sprintf(message, "");
      break;
    case 434:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 109, 0, 879, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case 435:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case 433:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 109, 0, 453, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case 358:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        while (player->HasId(752)) {
          player->SendMsg(nullptr, 109, 0, 752, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        std::sprintf(message, "");
      }
      break;
    case 260:
      if (aid == g_session.m_aid) {
        g_session.unknown_5450 = 0;
      }
      std::sprintf(message, "");
      break;
    case 241:
    case 271:
      std::sprintf(message, LookupMessage(1232));
      break;
    case 242:
    case 272:
      std::sprintf(message, LookupMessage(1234));
      break;
    case 243:
    case 273:
      std::sprintf(message, LookupMessage(1236));
      break;
    case 244:
    case 274:
      std::sprintf(message, LookupMessage(1240));
      break;
    case 245:
    case 275:
      std::sprintf(message, LookupMessage(1238));
      break;
    case 246:
    case 276:
      std::sprintf(message, LookupMessage(1242));
      break;
    case 247:
      std::sprintf(message, LookupMessage(1244));
      break;
    case 248:
      std::sprintf(message, LookupMessage(1246));
      break;
    case 249:
      std::sprintf(message, LookupMessage(1248));
      break;
    case 253:
      if (aid == g_session.m_aid) {
        this->unknown_05CC[0] = 0;
      }
      break;
    case 933:
      if (aid == g_session.m_aid) {
        this->scene->player->Slot15(0, 1);
        this->scene->player->render.isMotionFreezed = false;
      }
      break;
    case 893:
      if (aid == g_session.m_aid) {
        this->scene->player->Slot15(0, 1);
      }
      break;
    case 1013:
      color = 0xFFFF00;
      std::sprintf(message, LookupMessage(3247));
      break;
    case 1125:
    {
      CEffect* effect = g_effectMgr->Find(aid, 1362);
      if (effect) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    }
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 448:
    case 452:
    case 453:
    case 454:
    case 673:
    case 674:
    case 716:
    case 717:
    case 718:
    case 719:
    case 720:
    case 721:
    case 728:
    case 730:
    case 734:
    case 735:
    case 736:
    case 737:
    case 779:
    case 822:
    case 823:
    case 825:
    case 826:
    case 849:
    case 856:
    case 857:
    case 859:
    case 863:
    case 864:
    case 872:
    case 895:
    case 896:
    case 917:
    case 919:
    case 937:
    case 982:
    case 1016:
    case 1017:
    case 1018:
    case 1019:
    case 1020:
    case 1021:
    case 1022:
    case 1023:
    case 1024:
    case 1025:
    case 1038:
    case 1039:
    case 1040:
    case 1041:
    case 1043:
    case 1050:
    case 1055:
    case 1126:
    case 1139:
    case 1141:
    case 1142:
    case 1149:
    case 1165:
    case 1166:
    case 1167:
    case 1176:
    case 1184:
    case 1197:
    case 1227:
    case 1688:
      break;
    default:
      this->sub_CFE480(aid);
      return;
    }
  }

  this->sub_CFE480(aid);
  if (std::strlen(message) > 1) {
    char text[512];
    std::sprintf(text, "%s%s", name, message);
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), color, std::strlen(name) > 1 ? 10 : 8, 0, 0);
  }

  if (aid == g_session.m_aid) {
    this->scene->player->SendMsg(
      nullptr, 110, 0, type, type >> 31, on, 0, value1, value1 >> 31, value2, value2 >> 31, flag, 0, value3, value3 >> 31
    );
  }
}

#pragma pack(push, 1)
struct Window107Entry {
  /* 0000 */ unsigned char unknown_0000[0x18];
};
#pragma pack(pop)

// OFFSET: 0x00D79540
// FUNCTION: fn_22170
void CGameMode::ReceiveObjectsDisappear(int count, const std::uint32_t* aids) {
  for (int i = 0; i < count; ++i) {
    const std::uint32_t aid = aids[i];
    if (this->scene->RemoveItem(aid) == true) {
      this->SendMsg(13, aid, 0, 0, 0);
    } else if (this->scene->RemoveGroundItem(aid) == true) {
      this->RemoveUnknown01A4(aid);
    } else {
      this->scene->RemoveSkill(aid);
    }
  }
}

// OFFSET: 0x00D795B0
// FUNCTION: fn_22171
void CGameMode::ReceiveWindow107List(const unsigned char* packet) {
  const std::int64_t count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(Window107Entry);
  if (count == 0) {
    g_windowMgr.Confirm(LookupMessage(424), 0, true, false, 0, "Message", 280, 120, 0);
    return;
  }

  g_windowMgr.CloseWindow(107);
  UIWindow* window = g_windowMgr.EnsureWindow(107);
  const Window107Entry* entry = reinterpret_cast<const Window107Entry*>(packet + 4);
  if (count != 0 && window != nullptr) {
    for (int i = 0; i < count; ++i) {
      window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(entry), 0, 0, 0);
      ++entry;
    }
  }
}

// OFFSET: 0x00D39AE0
// FUNCTION: fn_21772
void CGameMode::ReceiveActorColorChat(const unsigned char* packet) {
  char text[256];
  std::memset(text, 0, sizeof(text));
  std::memcpy(text, packet + 12, *reinterpret_cast<const std::uint16_t*>(packet + 2) - 12);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), *reinterpret_cast<const int*>(packet + 8), 0, 0, 0);

  CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4));
  if (actor != nullptr) {
    const int color = *reinterpret_cast<const int*>(packet + 8);
    actor->SendMsg(nullptr, 7, 0, reinterpret_cast<int>(text), reinterpret_cast<int>(text) >> 31, color, color >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  if (g_whisperRecord != 0) {
    this->WriteChatLog(text);
  }
}

// OFFSET: 0x00D1F400
// FUNCTION: fn_21637
void CGameMode::ReceiveWearEquipAck(unsigned char result, std::uint16_t index, std::uint16_t look, int location) {
  *reinterpret_cast<std::uint32_t*>(&this->unknown_0100[0x04]) = timeGetTime() - 2000;
  switch (result) {
  case 0: {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    if (item.unknown_0000[1]) {
      item.unknown_0070 = look;
      item.unknown_0000[3] = location;
      g_session.OnItemEquipped(item);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1953)), 0xFF, 7, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(372)), 0xFF, 7, 0, 0);
    break;
  }
}

// OFFSET: 0x00D32A10
// FUNCTION: fn_21747
void CGameMode::ReceiveFormattedMessage(const unsigned char* packet) {
  char text[500];
  std::memset(text, 0, sizeof(text));
  const std::uint16_t messageId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  if (messageId == 3278) {
    char name[260];
    std::memset(name, 0, sizeof(name));
    ITEM_INFO item;
    item.SetItemId(*reinterpret_cast<const int*>(packet + 4));
    int capacity = sizeof(name);
    item.sub_6D5810(name, capacity, 0);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3278), name);
  } else {
    std::sprintf(text, LookupMessage(messageId), *reinterpret_cast<const int*>(packet + 4));
  }

  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  }
}

#pragma pack(push, 1)
struct ParaResultEntry {
  /* 0000 */ int itemId;
  /* 0004 */ std::int16_t quantity;
  /* 0006 */ int location;
  /* 000A */ unsigned char refine;
  /* 000B */ int cards[4];
  /* 001B */ std::uint16_t index;
};
#pragma pack(pop)

// OFFSET: 0x00D77E70
// FUNCTION: fn_22155
void CGameMode::ReceiveParaItemResult(unsigned char success, int count, const unsigned char* data) {
  UIParaResultWnd* window = static_cast<UIParaResultWnd*>(g_windowMgr.EnsureWindow(256));
  if (window == nullptr) {
    return;
  }

  const ParaResultEntry* entry = reinterpret_cast<const ParaResultEntry*>(data);
  for (int i = 0; i < count; ++i, ++entry) {
    ITEM_INFO item;
    item.SetItemId(entry->itemId);
    item.quantity = entry->quantity;
    item.unknown_0014[0] = entry->location;
    item.unknown_0014[1] = entry->location;
    item.unknown_0060 = entry->refine;
    std::memcpy(item.unknown_001C, entry->cards, sizeof(entry->cards));
    item.unknown_0000[1] = entry->index;
    item.identified = 1;
    window->AddItem(item);
  }

  window->SendMsg(nullptr, 36, (count - 2 > 0 ? count : 0) * 32 + 120, 0, 0, 0);
  window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  if (!success) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3043)), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D79700
// FUNCTION: fn_22173
void CGameMode::ReceiveResultNotice4327(const unsigned char* packet) {
  std::string text;
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 0:
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(4327)));
    break;
  case 1:
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(4327)));
    break;
  case 2:
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(4327)));
    break;
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D78130
// FUNCTION: fn_22159
void CGameMode::OpenBuyingStoreMake(unsigned char slotCount) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    const ModeDragState* drag = &mode->drag;
    if (drag && drag->source) {
      this->SendMsg(299, 0, 0, 0, 0);
      return;
    }
  }

  const unsigned int count = slotCount;
  if (count < 2) {
    g_session.m_buyingStoreSlotCount = 2;
  } else {
    g_session.m_buyingStoreSlotCount = count > 5 ? 5 : count;
  }
  g_session.ClearItems1768();
  if (!g_windowMgr.window_027C && !g_windowMgr.window_0274 && !g_windowMgr.unknown_02FC) {
    g_windowMgr.EnsureWindow(174);
    if (g_windowMgr.unknown_040C) {
      static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.unknown_040C)->SetItems(0, nullptr);
    }
    return;
  }

  g_windowMgr.Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(299, 0, 0, 0, 0);
}

// OFFSET: 0x00D78210
// FUNCTION: fn_22160
void CGameMode::OpenVendingMake(unsigned char slotCount, int itemCount, const short* items) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    const ModeDragState* drag = &mode->drag;
    if (drag && drag->source) {
      this->SendMsg(297, 0, 0, 0, 0);
      return;
    }
  }

  const unsigned int count = slotCount;
  if (count < 3) {
    g_session.m_vendingSlotCount = 3;
  } else {
    g_session.m_vendingSlotCount = count > 13 ? 13 : count;
  }
  g_session.ClearItems1768();
  if (!g_windowMgr.window_027C && !g_windowMgr.window_0274) {
    g_windowMgr.EnsureWindow(41);
    if (g_windowMgr.unknown_02FC) {
      static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.unknown_02FC)->SetItems(itemCount, items);
    }
    return;
  }

  g_windowMgr.Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(297, 0, 0, 0, 0);
}

// OFFSET: 0x00D799C0
// FUNCTION: fn_22176
void CGameMode::ReceiveAgitNotice(unsigned char agitId, int first, int second) {
  if (!g_windowMgr.window_0320) {
    return;
  }

  char text[256] = {};
  const char* agitName = nullptr;
  g_session.m_hotkeyScript->Call("get_agit_name", "d>s", agitId, &agitName);
  sprintf_s(text, LookupMessage(3632), agitName, second, 100, first, 100);
  g_windowMgr.Confirm(text, 0, true, false, 0, "Message", 250, 130, 0);
}

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
  /* 003E */ std::uint16_t unknown_003E;
  /* 0040 */ unsigned char showEffect;
  /* 0042 */ std::uint16_t grade;
};

// OFFSET: 0x00D2DB80
// FUNCTION: fn_21724
void CGameMode::ReceiveItemFallEntry(const unsigned char* packet) {
  ItemFallEntry entry = {};
  entry.count = *reinterpret_cast<const std::int16_t*>(packet + 0x0F);
  entry.aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  entry.column = *reinterpret_cast<const std::int16_t*>(packet + 0x0B);
  entry.row = *reinterpret_cast<const std::int16_t*>(packet + 0x0D);
  entry.subColumn = packet[0x11];
  entry.subRow = packet[0x12];
  entry.falling = 0;
  _itoa(*reinterpret_cast<const int*>(packet + 6), entry.itemId, 10);
  entry.falling = 0;
  entry.identified = packet[0x0A];

  CItem* item = this->scene->GetItemByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2));
  if (!item) {
    item = new CItem;
    this->scene->objects.push_back(item);
    this->scene->unknown_0018.push_back(item);
  }

  const int entryAddress = reinterpret_cast<int>(&entry);
  item->SendMsg(nullptr, 4, 0, entryAddress, entryAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D266C0
// FUNCTION: fn_21681
void CGameMode::ReceiveFeedOption(const unsigned char* packet) {
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 0:
    *reinterpret_cast<int*>(g_session.unknown_5974) = *reinterpret_cast<const int*>(packet + 6);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(LookupMessage((*reinterpret_cast<int*>(g_session.unknown_5974) != 0) + 0x54E)), 0x64FFFF, 0, 0, 0
    );
    if (g_windowMgr.unknown_01E0) {
      g_windowMgr.unknown_01E0->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
    }
    break;
  case 1:
    g_serviceOptions.Set(231, *reinterpret_cast<const int*>(packet + 6) != 0);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(LookupMessage((*reinterpret_cast<const int*>(packet + 6) != 0) + 0xBA2)), 0x64FFFF, 0, 0, 0
    );
    g_windowMgr.sub_AAB070(231);
    break;
  case 2:
    g_session.SetPetAutoFeed(*reinterpret_cast<const int*>(packet + 6) > 0);
    break;
  case 3:
    g_session.SetHomunculusAutoFeed(*reinterpret_cast<const int*>(packet + 6) > 0);
    break;
  case 5:
    *reinterpret_cast<int*>(g_session.unknown_8128) = *reinterpret_cast<const int*>(packet + 6);
    break;
  case 6:
    sub_E489C0(*reinterpret_cast<const int*>(packet + 6) != 0);
    break;
  }
}

// OFFSET: 0x00D1E7B0
// FUNCTION: fn_21635
void CGameMode::ReceiveTakeOffEquipResult(unsigned char result, int location, std::uint16_t identity) {
  *reinterpret_cast<std::uint32_t*>(&this->unknown_0100[0x08]) = timeGetTime() - 2000;
  switch (result) {
  case 0:
    if (location) {
      if (g_session.IsCostumeLocation(location)) {
        g_session.UnequipItem2B50(location);
        return;
      }
      g_session.UnequipItem(location, identity);
    }
    break;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1953)), 0xFF, 7, 0, 0);
    break;
  }
}

// OFFSET: 0x00D25E90
// FUNCTION: fn_21677
void CGameMode::ReceiveHomunculusChange(const unsigned char* packet) {
  char text[256];
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
  switch (static_cast<char>(packet[3])) {
  case 0:
    *reinterpret_cast<std::uint32_t*>(&g_session.unknown_5570[0x08]) = *reinterpret_cast<const std::uint32_t*>(packet + 4);
    if (actor) {
      g_session.unknown_5614 = 1;
      g_session.m_homunJob = actor->GetEffectiveJob(-1);
    }
    break;
  case 1:
    g_session.unknown_55D4 = *reinterpret_cast<const int*>(packet + 8);
    if (actor) {
      g_session.m_homunJob = actor->GetEffectiveJob(-1);
    }
    if (g_windowMgr.window_03C0) {
      g_windowMgr.window_03C0->Invalidate();
    }
    if (g_session.IsAdminSubMode(3)) {
      std::sprintf(text, "%d", *reinterpret_cast<const int*>(packet + 8));
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xF5F5F5, 0, 0, 0);
    }
    break;
  case 2:
    g_session.unknown_55F0 = *reinterpret_cast<const int*>(packet + 8);
    g_session.RefreshHomunculusInfo();
    if (actor) {
      g_session.m_homunJob = actor->GetEffectiveJob(-1);
    }
    if (g_windowMgr.window_03C0) {
      g_windowMgr.window_03C0->Invalidate();
    }
    if (g_session.IsAdminSubMode(3)) {
      std::sprintf(text, "Full : %d", *reinterpret_cast<const int*>(packet + 8));
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x1EF5F5, 0, 0, 0);
    }
    break;
  case 3:
    if (actor) {
      if (*reinterpret_cast<const int*>(packet + 8)) {
        g_session.unknown_55D8 = *reinterpret_cast<const int*>(packet + 8);
      } else {
        g_session.unknown_55D8 = 0;
        actor->Slot21(actor->GetEffectiveJob(-1));
      }
      if (g_session.IsHomunAid(*reinterpret_cast<const std::uint32_t*>(packet + 4)) && g_windowMgr.window_03C0) {
        g_windowMgr.window_03C0->Invalidate();
      }
    }
    break;
  }
}

// OFFSET: 0x00D26950
// FUNCTION: fn_21683
void CGameMode::ReceiveItemConsumed(const unsigned char* packet) {
  if (g_windowMgr.unknown_0458) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2734)), 0x64FFFF, 0, 0, 0);
    g_modeMgr.GetGameMode()->SendMsg(293, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(264);
  }

  if (*reinterpret_cast<int*>(&this->unknown_0100[0x14])) {
    *reinterpret_cast<int*>(&this->unknown_0100[0x14]) = 0;
    g_session.ResetSlots();
  }

  g_session.RemoveItem1710(
    *reinterpret_cast<const std::uint16_t*>(packet + 4), *reinterpret_cast<const std::int16_t*>(packet + 6)
  );
  *reinterpret_cast<int*>(&this->unknown_02B8[0x18]) = 0;
  this->unknown_05CE = 1;
  if (g_session.unknown_103C != -1 && g_session.unknown_1050 == *reinterpret_cast<const std::uint16_t*>(packet + 4)) {
    g_session.ResetUnknown1050();
    EquipmentProperties::CEquipmentPropertiesMgr* properties = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
    int itemId = -1;
    int kind = 0;
    properties->RemoveExpansion(kind, itemId);
  }
}

// OFFSET: 0x00D25D50
// FUNCTION: fn_21676
void CGameMode::ReceivePartyLeaderChange(const unsigned char* packet) {
  char text[500];
  MessengerMember member = g_session.PartyByAccount(*reinterpret_cast<const std::uint32_t*>(packet + 6));
  if (member.valid != 0) {
    g_session.SetPartyLeader(*reinterpret_cast<const std::uint32_t*>(packet + 2), 1);
    g_session.SetPartyLeader(*reinterpret_cast<const std::uint32_t*>(packet + 6), 0);
    g_session.unknown_5464 = *reinterpret_cast<const std::uint32_t*>(packet + 6) == g_session.m_aid;
    std::sprintf(text, LookupMessage(1552), member.name.c_str());
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
    sub_DFF150(&g_session);
    if (g_windowMgr.unknown_03F8) {
      g_windowMgr.unknown_03F8->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D2DA10
// FUNCTION: fn_21723
void CGameMode::ReceiveItemSellResult(const unsigned char* packet) {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  const int count = *reinterpret_cast<const std::int16_t*>(packet + 4);
  const int price = *reinterpret_cast<const int*>(packet + 6);
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::int16_t*>(packet + 2));
  g_windowMgr.SendNotice(
    1,
    reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1746), item.DisplayName(0).c_str(), count, price * count).c_str()),
    0xFFFF64, 0, 0, 0
  );
  g_session.RemoveItem1710(
    *reinterpret_cast<const std::int16_t*>(packet + 2), *reinterpret_cast<const std::int16_t*>(packet + 4)
  );
}

// OFFSET: 0x00D58F60
// FUNCTION: fn_21958
std::FILE* CGameMode::OpenRandomOptionSimulatorFile(const std::string& folder, const std::string& itemName) {
  std::string directory = this->MakeSimulatorFolder(folder);
  if (directory.size() == 0) {
    return nullptr;
  }

  SYSTEMTIME time;
  GetLocalTime(&time);
  std::string name = FormatOwnedString("%s_RandomOption(%d_%d_%d)", itemName.c_str(), time.wYear, time.wMonth, time.wDay);
  std::string path = FormatOwnedString("%s\\%s.txt", directory.c_str(), name.c_str());
  int index = 0;
  while (_access(path.c_str(), 0) == 0) {
    ++index;
    path = FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index);
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("Simulator Result File Create %s %s!!!!!!!!!!!!", file ? "success" : "Fail", itemName.c_str()).c_str()),
                         file ? 0xFFFF : 0xFF, 0, 0, 0);
  return file;
}

#pragma pack(push, 1)
struct RandomOptionSimulationRecord {
  /* 0000 */ int optionId;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ unsigned char optionType;
  /* 000D */ int rate;
  /* 0011 */ int unknown_0011;
  /* 0015 */ int unknown_0015;
  /* 0019 */ int unknown_0019;
  /* 001D */ int unknown_001D;
  /* 0021 */ int unknown_0021;
};
#pragma pack(pop)

static_assert(sizeof(RandomOptionSimulationRecord) == 0x25);

// OFFSET: 0x00D606B0
// FUNCTION: fn_21974
void CGameMode::ReceiveRandomOptionSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  const char* name = reinterpret_cast<const char*>(header->unknown_000C);
  std::FILE* file = this->OpenRandomOptionSimulatorFile("\\PackageItemTest", std::string(name));
  if (!file) {
    return;
  }

  const char* typeNames[6] = {
    "RANDOM_OPT_TYPE_MUST_1", "RANDOM_OPT_TYPE_MUST_2", "RANDOM_OPT_TYPE_MUST_3",
    "RANDOM_OPT_TYPE_MUST_4", "RANDOM_OPT_TYPE_MUST_5", "RANDOM_OPT_TYPE_RANDOM",
  };
  const RandomOptionSimulationRecord* record = reinterpret_cast<const RandomOptionSimulationRecord*>(packet + sizeof(SimulationPacketHeader));
  const int count = static_cast<int>((header->length - sizeof(SimulationPacketHeader)) / sizeof(RandomOptionSimulationRecord));
  std::vector<const RandomOptionSimulationRecord*> options[6];
  for (int i = 0; i < count; ++i) {
    options[record->optionType].push_back(record);
    ++record;
  }

  std::fprintf(file, "\t%s", ToUtf8(std::string(name), g_textCodePage).c_str());
  std::fprintf(file, "============================\tSTART\t============================\n");
  int total = 0;
  for (int type = 0; type < 6; ++type) {
    if (options[type].empty()) {
      continue;
    }

    std::fprintf(file, "\n%s\n", typeNames[type]);
    for (const RandomOptionSimulationRecord* option : options[type]) {
      const char* optionName = nullptr;
      if (g_session.m_hotkeyScript->Call("GetVarOptionName", "d>s", option->optionId, &optionName)) {
        std::fprintf(file, "\t%s", ToUtf8(std::string(optionName), g_textCodePage).c_str());
        if (type == 5) {
          std::fprintf(
            file, "\t\t%d\t%d\t(%d/%d)\t%d\t(%d/%d)\t%.2f%%\n", option->unknown_0019, option->unknown_001D, option->unknown_0004,
            option->unknown_0008, option->unknown_0021, option->unknown_0011, option->unknown_0015,
            static_cast<float>(option->rate / 100.0)
          );
        } else {
          std::fprintf(
            file, "\t\t%d\t%d\t(%d/%d)\t%d\t(%d/%d)\n", option->unknown_0019, option->unknown_001D, option->unknown_0004,
            option->unknown_0008, option->unknown_0021, option->unknown_0011, option->unknown_0015
          );
        }
      }
      ++total;
    }
  }

  std::fprintf(file, "Try %d RandomOption\n", total);
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D2A020
// FUNCTION: fn_21701
void CGameMode::ReceiveBuyingStoreOpenResult(const unsigned char* packet) {
  char text[128];
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1741)), 0xFF, 0, 0, 0);
    g_session.ClearItems1768();
    g_windowMgr.CloseWindow(174);
    g_windowMgr.CloseWindow(175);
    break;
  case 2:
    std::sprintf(
      text, LookupMessage(1742),
      static_cast<int>(
        static_cast<float>(*reinterpret_cast<const int*>(packet + 4) / 10 + g_session.m_weight) - static_cast<float>(g_session.m_maxWeight) * 0.9f
      )
    );
    g_windowMgr.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 9:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1797)), 0xFF, 0, 0, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3229)), 0xFF, 0, 0, 0);
    g_session.ClearItems1768();
    g_windowMgr.CloseWindow(174);
    g_windowMgr.CloseWindow(175);
    break;
  }
}

// OFFSET: 0x00D4B050
// FUNCTION: fn_21863
void CGameMode::ReceiveItemObtainNotice(const unsigned char* packet) {
  ITEM_INFO item;
  item.SetItemId(*reinterpret_cast<const int*>(packet + 6));
  item.unknown_0060 = 0;
  item.identified = 1;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 0:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(911), item.DisplayName(0).c_str()).c_str()), 0xFFFF00, 0, 0, 0
    );
    break;
  case 1:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(912), item.DisplayName(0).c_str()).c_str()), 0xCDCD00, 0, 0, 0
    );
    break;
  case 2:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(913), item.DisplayName(0).c_str()).c_str()), 0xC8C8FF, 0, 0, 0
    );
    break;
  case 3:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(914), item.DisplayName(0).c_str()).c_str()), 0xC8C8FF, 0, 0, 0
    );
    break;
  }
}

// OFFSET: 0x00D8A460
// FUNCTION: fn_22219
void CGameMode::ReceiveTakeOffEquipItemResult(const unsigned char* packet) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t*>(packet + 2));
  item.unknown_0000[3] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 8)) {
  case 0: {
    if (g_session.IsCostumeLocation(*reinterpret_cast<const int*>(packet + 4))) {
      g_session.UnequipItem6B68(item);
    } else {
      g_session.UnequipItem61B8(item);
    }
    const char* suffix = LookupMessage(3144);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), suffix).c_str()), 0xC8FF00, 7, 0, 0
    );
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3009)), 0xFF, 0, 0, 0);
    if (g_session.IsCostumeLocation(*reinterpret_cast<const int*>(packet + 4))) {
      g_session.UnequipItem6B68(item);
    } else {
      g_session.UnequipItem61B8(item);
    }
    break;
  }
}

// OFFSET: 0x00D2A290
// FUNCTION: fn_21703
void CGameMode::ReceiveBuyingStoreSellResult(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 5:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(57)), 0xFF, 0, 0, 0);
    break;
  case 6: {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, *reinterpret_cast<const int*>(packet + 4), 0);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1747), item.DisplayName(0).c_str()).c_str()), 0xFF, 0, 0, 0
    );
    break;
  }
  case 7:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1745)), 0xFF, 0, 0, 0);
    break;
  }

  g_windowMgr.CloseWindow(177);
  g_windowMgr.CloseWindow(178);
  g_windowMgr.CloseWindow(179);
}

struct ReputePointEntry {
  /* 0000 */ std::uint64_t id;
  /* 0008 */ std::int64_t point;
};

// OFFSET: 0x00D78060
// FUNCTION: fn_22158
void CGameMode::ReceiveItemReformOpen(int itemId) {
  UIWindow* window = g_windowMgr.EnsureWindow(0x15C);
  if (!g_itemReformMgr) {
    g_itemReformMgr = new (std::nothrow) CItemReformMgr;
  }
  if (g_itemReformMgr->HasItem(itemId) == true) {
    if (window) {
      window->SendMsg(nullptr, 0x17, itemId, 0, 0, 0);
    }
  } else {
    g_windowMgr.CloseWindow(0x15C);
  }
}

// OFFSET: 0x00D79880
// FUNCTION: fn_22174
void CGameMode::ReceiveReputeInfo(bool received, unsigned int count, const ReputePointEntry* entries) {
  for (unsigned int index = 0; index < count; ++index) {
    if (!CReputeMgr::s_instance) {
      CReputeMgr::s_instance = new (std::nothrow) CReputeMgr;
    }
    CReputeMgr::s_instance->SetPoint(entries[index].id, entries[index].point);
  }
  if (!CReputeMgr::s_instance) {
    CReputeMgr::s_instance = new (std::nothrow) CReputeMgr;
  }
  CReputeMgr::s_instance->SetPointsReceived(received);
}

// OFFSET: 0x00D79D90
// FUNCTION: fn_22181
void CGameMode::ReceiveEnchantResult(int messageId, int itemId) {
  if (!CEnchantMgr::s_instance) {
    CEnchantMgr::s_instance = new (std::nothrow) CEnchantMgr;
  }
  CEnchantMgr::s_instance->HandleEnchantResult(messageId, itemId);
}

// OFFSET: 0x00D7B4C0
// FUNCTION: fn_22205
void CGameMode::ReceiveReputeOpen(std::uint64_t groupId, std::uint64_t reputeId) {
  if (!CReputeMgr::s_instance) {
    CReputeMgr::s_instance = new (std::nothrow) CReputeMgr;
  }
  CReputeMgr::s_instance->OpenRepute(groupId, reputeId);
}

// OFFSET: 0x00D7B550
// FUNCTION: fn_22206
void CGameMode::ReceiveUiOpen(unsigned char type, std::uint64_t data) {
  if (type == 10) {
    if (!CEnchantMgr::s_instance) {
      CEnchantMgr::s_instance = new (std::nothrow) CEnchantMgr;
    }
    CEnchantMgr::s_instance->OpenEnchant(data);
  }
}

// OFFSET: 0x00D17AD0
// FUNCTION: fn_21596
void CGameMode::RefreshEmotionPurchaseSchedule() {
  g_timeScheduler->sub_BA95F0(5);
  CCashEmotionMgr* manager = g_cashEmotionMgr;
  if (!manager) {
    return;
  }

  manager->sub_59C5A0();
  const int expiry = manager->GetEarliestExpiry();
  if (expiry < 0x7FFFFFFF) {
    const std::uint64_t now = static_cast<std::int64_t>(manager->sub_59FC50()) * 10000000;
    const std::int64_t interval = (static_cast<std::int64_t>(expiry) * 10000000 - static_cast<std::int64_t>(now)) / 10000;
    if (interval != 0 && interval >= 0) {
      g_timeScheduler->AddSchedule(new CEmotionPurchaseExpireSchedule(now, interval, 1));
    }
  }
  manager->RefreshCommands();
}

// OFFSET: 0x00D31390
// FUNCTION: fn_21733
void CGameMode::ReceivePartyItemPickupNotice(const unsigned char* packet) {
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  MessengerMember member = g_session.PartyByAccount(aid);
  if (aid != g_session.m_aid && member.valid) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet[0x1F];
    item.unknown_0000[2] = *reinterpret_cast<const std::uint16_t*>(packet + 0x1D);
    item.unknown_0000[1] = *reinterpret_cast<const std::uint32_t*>(packet + 6);
    item.unknown_0000[3] = 0;
    item.SetItemId(*reinterpret_cast<const int*>(packet + 6));
    std::memcpy(item.unknown_001C, packet + 0x0D, sizeof(item.unknown_001C));
    item.identified = packet[0x0A];
    item.unknown_005D = packet[0x0B];
    item.unknown_0060 = packet[0x0C];

    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1276), member.name.c_str(), item.DisplayName(0).c_str()).c_str()),
      0xFFFF00, 9, 0, 0
    );
  }
}

// OFFSET: 0x00D64940
// FUNCTION: fn_21995
void CGameMode::ReceiveEquipCardChange(const unsigned char* packet) {
  const EquipSlots slots = g_session.GetEquipSlots(*reinterpret_cast<const std::int16_t*>(packet + 2));
  ITEM_INFO item = g_session.GetItem17F0(slots.values[2]);
  if (item.unknown_0000[1] != 0) {
    int* cards = reinterpret_cast<int*>(item.unknown_001C);
    switch (packet[0xA]) {
    case 1:
      cards[*reinterpret_cast<const std::int16_t*>(packet + 4)] = *reinterpret_cast<const int*>(packet + 6);
      break;
    case 2:
    case 3:
      cards[*reinterpret_cast<const std::int16_t*>(packet + 4)] = 0;
      break;
    }
    g_session.SetEquipItem17F0(slots.values[2], item);
  }
}

// OFFSET: 0x00D284D0
// FUNCTION: fn_21694
void CGameMode::ReceiveEquipWindowMicroscope2(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetItems3500();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 38u;
  const unsigned char* entry = packet + 45;
  for (int index = 0; index < count; ++index, entry += 38) {
    ITEM_INFO item;
    item.unknown_0000[0] = entry[6];
    item.unknown_0000[2] = *reinterpret_cast<const std::uint16_t*>(entry + 8);
    item.unknown_0000[1] = *reinterpret_cast<const std::int16_t*>(entry);
    item.unknown_0000[3] = *reinterpret_cast<const std::uint16_t*>(entry + 10);
    item.quantity = 1;
    std::memcpy(item.unknown_001C, entry + 14, sizeof(item.unknown_001C));
    item.SetItemId(*reinterpret_cast<const int*>(entry + 2));
    item.identified = entry[7];
    item.unknown_005D = entry[12];
    item.unknown_0060 = entry[13];
    item.unknown_0068 = *reinterpret_cast<const std::uint32_t*>(entry + 30);
    item.unknown_0064 = *reinterpret_cast<const std::uint16_t*>(entry + 34);
    item.unknown_0070 = *reinterpret_cast<const std::uint16_t*>(entry + 36);

    if (item.unknown_0000[3] != 0) {
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem3500(item);
      } else {
        g_session.Equip(item);
      }
    }
  }

  g_equipWindowLook.job = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  g_equipWindowLook.sex = packet[0x2C];
  g_equipWindowLook.head = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  g_equipWindowLook.accessory = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  g_equipWindowLook.accessory2 = *reinterpret_cast<const std::int16_t*>(packet + 0x22);
  g_equipWindowLook.accessory3 = *reinterpret_cast<const std::int16_t*>(packet + 0x24);
  g_equipWindowLook.bodyPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x2A);
  g_equipWindowLook.headPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x28);
  std::memcpy(g_equipWindowLook.name, packet + 4, sizeof(g_equipWindowLook.name));
  g_equipWindowLook.robe = *reinterpret_cast<const std::int16_t*>(packet + 0x26);

  if (!g_windowMgr.CloseWindow(0x8B)) {
    g_windowMgr.EnsureWindow(0x8B);
  }
}

// OFFSET: 0x00D28700
// FUNCTION: fn_21695
void CGameMode::ReceiveEquipWindowMicroscope3(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetItems3500();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 41u;
  const unsigned char* entry = packet + 45;
  for (int index = 0; index < count; ++index, entry += 41) {
    ITEM_INFO item;
    item.unknown_0000[0] = entry[6];
    item.unknown_0000[2] = *reinterpret_cast<const std::uint32_t*>(entry + 7);
    item.unknown_0000[1] = *reinterpret_cast<const std::int16_t*>(entry);
    item.unknown_0000[3] = *reinterpret_cast<const std::uint32_t*>(entry + 11);
    item.quantity = 1;
    std::memcpy(item.unknown_001C, entry + 16, sizeof(item.unknown_001C));
    item.SetItemId(*reinterpret_cast<const int*>(entry + 2));

    const unsigned char flags = entry[40];
    item.identified = flags & 1;
    item.unknown_005D = (flags >> 1) & 1;
    item.unknown_0060 = entry[15];
    item.unknown_0068 = *reinterpret_cast<const std::uint32_t*>(entry + 32);
    item.unknown_0064 = *reinterpret_cast<const std::uint16_t*>(entry + 36);
    item.unknown_0070 = *reinterpret_cast<const std::uint16_t*>(entry + 38);

    if (item.unknown_0000[3] != 0) {
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem3500(item);
      } else {
        g_session.Equip(item);
      }
    }
  }

  g_equipWindowLook.job = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  g_equipWindowLook.sex = packet[0x2C];
  g_equipWindowLook.head = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  g_equipWindowLook.accessory = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  g_equipWindowLook.accessory2 = *reinterpret_cast<const std::int16_t*>(packet + 0x22);
  g_equipWindowLook.accessory3 = *reinterpret_cast<const std::int16_t*>(packet + 0x24);
  g_equipWindowLook.bodyPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x2A);
  g_equipWindowLook.headPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x28);
  memcpy_s(g_equipWindowLook.name, sizeof(g_equipWindowLook.name), packet + 4, sizeof(g_equipWindowLook.name));
  g_equipWindowLook.robe = *reinterpret_cast<const std::int16_t*>(packet + 0x26);

  if (!g_windowMgr.CloseWindow(0x8B)) {
    g_windowMgr.EnsureWindow(0x8B);
  }
}

// OFFSET: 0x00D4E5A0
// FUNCTION: fn_21877
void CGameMode::ReceiveGuildMemberAdd(const unsigned char* packet) {
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->unknown_00C0 == 0) {
    return;
  }

  char memo[51];
  std::memcpy(memo, packet + 0x20, 50);
  memo[50] = '\0';
  char name[25];
  std::memcpy(name, packet + 0x52, 24);
  name[24] = '\0';

  GuildInfo::Member member;
  member.accountId = *reinterpret_cast<const int*>(packet + 2);
  member.characterId = *reinterpret_cast<const int*>(packet + 6);
  member.name = name;
  member.unknown_0020 = *reinterpret_cast<const std::int16_t*>(packet + 0xA);
  member.unknown_0024 = *reinterpret_cast<const std::int16_t*>(packet + 0xC);
  member.job = *reinterpret_cast<const std::int16_t*>(packet + 0x10);
  member.unknown_002C = *reinterpret_cast<const std::int16_t*>(packet + 0xE);
  member.positionId = *reinterpret_cast<const int*>(packet + 0x1C);
  member.level = *reinterpret_cast<const std::int16_t*>(packet + 0x12);
  member.unknown_0050 = memo;
  member.exp = *reinterpret_cast<const int*>(packet + 0x14);
  member.online = *reinterpret_cast<const int*>(packet + 0x18);
  guildInfo->AddMember(member);

  const std::string playerName(name, 25);
  g_playerNameDictionary->Set(*reinterpret_cast<const std::uint32_t*>(packet + 6), playerName.c_str());
  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D510C0
// FUNCTION: fn_21889
void CGameMode::ReceiveGuildPositionInfo(const unsigned char* packet) {
  GuildInfo* guildInfo = &g_guildInfo;
  const int count = static_cast<unsigned>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) >> 4;
  guildInfo->unknown_00D4.clear();

  const unsigned char* entry = packet + 4;
  for (int index = 0; index < count; ++index, entry += 16) {
    const int positionId = *reinterpret_cast<const int*>(entry);
    std::string name;
    name = guildInfo->m_positions[positionId];

    const unsigned int mode = *reinterpret_cast<const unsigned int*>(entry + 4);
    GuildInfo::Unknown00D4Entry position;
    position.unknown_0000 = *reinterpret_cast<const int*>(entry + 8);
    position.unknown_0004 = positionId;
    position.unknown_0008 = name;
    position.unknown_0020 = mode & 1;
    position.unknown_0024 = (mode >> 4) & 1;
    position.unknown_0028 = *reinterpret_cast<const int*>(entry + 12);
    position.unknown_002C = (mode >> 8) & 1;
    guildInfo->unknown_00D4.push_back(position);
  }

  guildInfo->unknown_00D4.sort([](const GuildInfo::Unknown00D4Entry& left, const GuildInfo::Unknown00D4Entry& right) {
    return left.unknown_0004 < right.unknown_0004;
  });
  g_windowMgr.RefreshGuildWindows();
}
// OFFSET: 0x00D17BF0
// FUNCTION: fn_21597
void CGameMode::RefreshEmotionSaleSchedule() {
  g_timeScheduler->sub_BA95F0(6);
  CCashEmotionMgr* manager = g_cashEmotionMgr;
  if (!manager) {
    return;
  }

  const int expiry = manager->RefreshUnownedPacks().nextTime;
  if (expiry < 0x7FFFFFFF) {
    const std::uint64_t now = static_cast<std::int64_t>(manager->sub_59FC50()) * 10000000;
    const std::int64_t interval = (static_cast<std::int64_t>(expiry) * 10000000 - static_cast<std::int64_t>(now)) / 10000;
    if (interval != 0 && interval >= 0) {
      g_timeScheduler->AddSchedule(new CEmotionSaleExpireSchedule(now, interval, 1));
    }
  }
  manager->RefreshCommands();
}

#pragma pack(push, 1)
struct MiniMapEffectMarkerPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int key;
  /* 0006 */ std::int16_t x;
  /* 0008 */ std::int16_t y;
};
#pragma pack(pop)

struct MiniMapEffectMarker {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int x;
  /* 0008 */ int y;
};

// GLOBAL: gl_25132
std::map<int, MiniMapEffectMarker> g_miniMapEffectMarkers;

// OFFSET: 0x00D244B0
// FUNCTION: fn_21667
void CGameMode::AddMiniMapEffectMarker(const unsigned char* packet) {
  const MiniMapEffectMarkerPacket* marker = reinterpret_cast<const MiniMapEffectMarkerPacket*>(packet);
  auto found = g_miniMapEffectMarkers.find(marker->key);
  if (found != g_miniMapEffectMarkers.end()) {
    found->second.x = marker->x;
    found->second.y = marker->y;
    found->second.unknown_0000 = 0x78;
    return;
  }

  g_miniMapEffectMarkers.emplace(marker->key, MiniMapEffectMarker{0x78, marker->x, marker->y});
}

struct ActorMessage32 {
  /* 0000 */ std::string text;
  /* 0018 */ std::string extra;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int unknown_0038;
  /* 003C */ int unknown_003C;
};

// OFFSET: 0x00D25AC0
// FUNCTION: fn_21675
void CGameMode::ReceiveActorMessage34(const unsigned char* packet) {
  this->unknown_01AC[*reinterpret_cast<const std::uint32_t*>(packet + 8)] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  ActorMessage32 message;
  message.text.assign(reinterpret_cast<const char*>(packet + 17), *reinterpret_cast<const std::int16_t*>(packet + 2) - 17);
  message.unknown_0030 = packet[16];
  message.unknown_0034 = *reinterpret_cast<const std::int16_t*>(packet + 14);
  message.unknown_0038 = *reinterpret_cast<const std::int16_t*>(packet + 12);
  message.unknown_003C = *reinterpret_cast<const int*>(packet + 8);
  if (message.text.size() > 60) {
    message.text.resize(60);
  }

  UIWindow* window = g_windowMgr.EnsureWindow(0x1C);
  window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&message), 0, 0, 0);
}

#pragma pack(push, 1)
struct AddQuestPacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ unsigned int questId;
  /* 0006 */ unsigned char state;
  /* 0007 */ __time32_t startTime;
  /* 000B */ __time32_t endTime;
  /* 000F */ std::int16_t huntCount;
};

struct AddQuestHunt64 {
  /* 0000 */ std::int64_t huntId;
  /* 0008 */ int unknown_0008;
  /* 000C */ int mobId;
  /* 0010 */ std::int16_t unknown_0010;
  /* 0012 */ std::int16_t unknown_0012;
  /* 0014 */ std::int16_t unknown_0014;
  /* 0016 */ char name[24];
};

struct AddQuestHunt {
  /* 0000 */ int huntId;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int mobId;
  /* 000C */ std::int16_t unknown_000C;
  /* 000E */ std::int16_t unknown_000E;
  /* 0010 */ std::int16_t unknown_0010;
  /* 0012 */ char name[24];
};
#pragma pack(pop)

static_assert(sizeof(AddQuestPacket) == 0x11);
static_assert(sizeof(AddQuestHunt64) == 0x2E);
static_assert(sizeof(AddQuestHunt) == 0x2A);

// OFFSET: 0x00D20C00
// FUNCTION: fn_21651
void CGameMode::ReceiveAddQuest64(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const AddQuestPacket*>(data);
  if (g_renewQuestMgr->SetOngoingQuestState(packet->questId, packet->state) == true) {
    g_renewQuestMgr->sub_67B6B0();
    return;
  }

  RenewQuestOngoingInfo info;
  info.m_state = packet->state;
  info.m_huntCount = static_cast<char>(packet->huntCount);
  const tm* time = _localtime32(&packet->endTime);
  info.m_year = time->tm_year + 1900;
  info.m_month = time->tm_mon + 1;
  info.m_day = time->tm_mday;
  info.m_hour = packet->endTime != 0 ? time->tm_hour : 0;
  info.m_minute = packet->endTime != 0 ? time->tm_min : 0;
  _localtime32(&packet->startTime);

  const auto* hunts = reinterpret_cast<const AddQuestHunt64*>(packet + 1);
  for (int i = 0; i < packet->huntCount; ++i) {
    const AddQuestHunt64& hunt = hunts[i];
    info.m_huntId[i] = hunt.huntId;
    info.m_huntUnknown01A0[i] = hunt.unknown_0008;
    info.m_huntUnknown00D4[i] = hunt.mobId;
    info.m_huntUnknown00E0[i] = hunt.unknown_0014;
    info.m_huntUnknown01AC[i] = hunt.unknown_0010;
    info.m_huntUnknown01B2[i] = hunt.unknown_0012;
    const int mobId = hunt.mobId;
    if ((mobId < 1001 || mobId > 3998) && static_cast<unsigned int>(mobId - 20021) > 11978) {
      if (mobId == 0x1099) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(3558));
      } else if (mobId == 0x109A) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(3559));
      } else if (mobId == 0x287B) {
        const std::string mapName = FormatOwnedString("%s.rsw", hunt.name);
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetMapDisplayName(mapName.c_str()));
      } else {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetJobName(mobId, 99));
      }
    } else {
      StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", hunt.name);
    }
  }

  g_renewQuestMgr->AddOngoingQuest(packet->questId, info);
  g_renewQuestMgr->sub_67B6B0();
  g_windowMgr.EnsureWindow(0x8F);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x55E)), 0xFFFF, 0x13, 0, 0);
}

// OFFSET: 0x00D20EF0
// FUNCTION: fn_21652
void CGameMode::ReceiveAddQuest(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const AddQuestPacket*>(data);
  if (g_renewQuestMgr->SetOngoingQuestState(packet->questId, packet->state) == true) {
    g_renewQuestMgr->sub_67B6B0();
    return;
  }

  RenewQuestOngoingInfo info;
  info.m_state = packet->state;
  info.m_huntCount = static_cast<char>(packet->huntCount);
  const tm* time = _localtime32(&packet->endTime);
  info.m_year = time->tm_year + 1900;
  info.m_month = time->tm_mon + 1;
  info.m_day = time->tm_mday;
  info.m_hour = packet->endTime != 0 ? time->tm_hour : 0;
  info.m_minute = packet->endTime != 0 ? time->tm_min : 0;
  _localtime32(&packet->startTime);

  const auto* hunts = reinterpret_cast<const AddQuestHunt*>(packet + 1);
  for (int i = 0; i < packet->huntCount; ++i) {
    const AddQuestHunt& hunt = hunts[i];
    info.m_huntId[i] = hunt.huntId;
    info.m_huntUnknown01A0[i] = hunt.unknown_0004;
    info.m_huntUnknown00D4[i] = hunt.mobId;
    info.m_huntUnknown00E0[i] = hunt.unknown_0010;
    info.m_huntUnknown01AC[i] = hunt.unknown_000C;
    info.m_huntUnknown01B2[i] = hunt.unknown_000E;
    const int mobId = hunt.mobId;
    if ((mobId < 1001 || mobId > 3998) && static_cast<unsigned int>(mobId - 20021) > 11978) {
      if (mobId == 0x1099) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(3558));
      } else if (mobId == 0x109A) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(3559));
      } else if (mobId == 0x287B) {
        const std::string mapName = FormatOwnedString("%s.rsw", hunt.name);
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetMapDisplayName(mapName.c_str()));
      } else {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetJobName(mobId, 99));
      }
    } else {
      StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", hunt.name);
    }
  }

  g_renewQuestMgr->AddOngoingQuest(packet->questId, info);
  g_renewQuestMgr->sub_67B6B0();
  g_windowMgr.EnsureWindow(0x8F);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x55E)), 0xFFFF, 0x13, 0, 0);
}

#pragma pack(push, 1)
struct BarterMarketItemListPacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::int16_t length;
};

struct BarterMarketItemRecord {
  /* 0000 */ int itemId;
  /* 0004 */ unsigned char type;
  /* 0005 */ int amount;
  /* 0009 */ int currencyItemId;
  /* 000D */ int currencyAmount;
  /* 0011 */ int weight;
  /* 0015 */ int index;
  /* 0019 */ std::uint16_t viewSprite;
  /* 001B */ int location;
};
#pragma pack(pop)

static_assert(sizeof(BarterMarketItemRecord) == 0x1F);

// OFFSET: 0x00D215B0
// FUNCTION: fn_21655
void CGameMode::ReceiveBarterMarketItemList(const unsigned char* data) {
  if (data == nullptr) {
    return;
  }

  g_barterMarketItemMgr->Clear();
  const auto* packet = reinterpret_cast<const BarterMarketItemListPacket*>(data);
  const std::int64_t count = (static_cast<std::int64_t>(packet->length) - 4) / static_cast<std::int64_t>(sizeof(BarterMarketItemRecord));
  for (std::int64_t i = 0; i < count; ++i) {
    BarterMarketItemRecord record;
    memcpy_s(&record, sizeof(record), data + sizeof(BarterMarketItemListPacket) + i * sizeof(BarterMarketItemRecord), sizeof(record));

    BarterMarketItem item;
    char itemName[33] = {};
    _itoa(record.itemId, itemName, 10);
    item.name = itemName;
    item.field_1c = record.type;
    item.field_2c = record.currencyItemId;
    item.field_28 = record.currencyAmount;
    item.amount = record.weight / 10;
    item.value = record.amount;
    item.flag_24 = true;
    item.flag_38 = record.amount == -1;
    item.id = record.index;
    item.field_40 = record.location;
    item.field_44 = record.viewSprite;
    g_barterMarketItemMgr->items.push_back(item);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(0x14E)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(0x14F)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D17D30
// FUNCTION: fn_21598
void CGameMode::ReceiveNotifyBan(const unsigned char* packet) {
  const int code = packet[2];
  GetRagConnection().Close();
  std::string text;
  switch (code) {
  case 0:
    text = text + LookupMessage(3) + "(" + IntToString(0, 0) + ")";
    break;
  case 1:
    text = text + LookupMessage(4) + "(" + IntToString(1, 0) + ")";
    break;
  case 2:
    text = text + LookupMessage(5) + "(" + IntToString(2, 0) + ")";
    break;
  case 3:
    text = text + LookupMessage(241) + "(" + IntToString(3, 0) + ")";
    break;
  case 4:
    text = text + LookupMessage(264) + "(" + IntToString(4, 0) + ")";
    break;
  case 5:
    text = text + LookupMessage(305) + "(" + IntToString(5, 0) + ")";
    break;
  case 9:
    text = text + LookupMessage(529) + "(" + IntToString(9, 0) + ")";
    break;
  case 10:
    text = text + LookupMessage(530) + "(" + IntToString(10, 0) + ")";
    break;
  case 15:
    if (g_serviceType == 4) {
      text = text + LookupMessage(579) + "(" + IntToString(15, 0) + ")";
    }
    break;
  case 110:
    text = text + LookupMessage(1589) + "(" + IntToString(110, 0) + ")";
    break;
  case 113:
    text = text + LookupMessage(2021) + "(" + IntToString(113, 0) + ")";
    break;
  case 115:
    text = text + LookupMessage(2347) + "(" + IntToString(115, 0) + ")";
    break;
  default:
    text = text + LookupMessage(3) + "(" + IntToString(code, 0) + ")";
    break;
  }

  if (text.size() == 0) {
    text = LookupMessage(0x718);
  }
  g_windowMgr.Confirm(text.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
  this->SendMsg(2, 0, 0, 0, 0);
}

#pragma pack(push, 1)
struct SkillFailurePacket {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint16_t skillId;
  /* 0004 */ int value;
  /* 0008 */ int itemId;
  /* 000C */ unsigned char result;
  /* 000D */ unsigned char cause;
};
#pragma pack(pop)

static_assert(sizeof(SkillFailurePacket) == 0xE);

// OFFSET: 0x00D21220
// FUNCTION: fn_21654
void CGameMode::ReceiveBroadcastText(const unsigned char* data) {
  const int length = *reinterpret_cast<const std::uint16_t*>(data + 2) - 4;
  if (length <= 0 || length > 361) {
    return;
  }

  std::vector<char> text(length + 1, 0);
  memcpy_s(text.data(), text.size(), data + 4, length);
  if (g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked;
    g_forbiddenWords.Mask(text.data(), &masked);
  }
  if (!g_serviceOptions.Get(0x94)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0xFF9DFF, 5, 0, 0);
    if (g_whisperRecord != 0) {
      this->WriteChatLog(text.data());
    }
  }
}

// OFFSET: 0x00D219D0
// FUNCTION: fn_21656
void CGameMode::ReceiveActorBroadcastText(const unsigned char* data) {
  const int length = *reinterpret_cast<const std::uint16_t*>(data + 2) - 0x20;
  if (length <= 0 || length > 256) {
    return;
  }

  std::vector<char> text(length, 0);
  std::memcpy(text.data(), data + 0x20, text.size());
  if (g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked;
    g_forbiddenWords.Mask(text.data(), &masked);
  }
  const int aid = *reinterpret_cast<const int*>(data + 4);
  if (MonsterJobColor(aid)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), MonsterJobColor(aid), 0, 0, 0);
  } else if (!g_serviceOptions.Get(0x72) && !g_serviceOptions.Get(0x94)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0x9B00FF, 1, 0, 0);
  }
  if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(text.data());
    actor->SendMsg(nullptr, 7, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  if (g_whisperRecord != 0) {
    this->WriteChatLog(text.data());
  }
}

namespace {
#pragma pack(push, 1)
  struct EquipWindowGradeRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::uint32_t location;
    /* 000B */ std::uint32_t wearState;
    /* 000F */ std::uint32_t cards[4];
    /* 001F */ std::uint32_t expireTime;
    /* 0023 */ std::uint16_t bindOnEquip;
    /* 0025 */ std::uint16_t sprite;
    /* 0027 */ unsigned char optionCount;
    /* 0028 */ StoreItemOption options[5];
    /* 0041 */ unsigned char refine;
    /* 0042 */ unsigned char grade;
    /* 0043 */ unsigned char flags;
  };
#pragma pack(pop)

  static_assert(sizeof(EquipWindowGradeRecord) == 0x44);

  // INLINED
  __forceinline void FillEquipWindowItem(ITEM_INFO& item, const StoreEquipOptionRecord& record, const StoreItemOption* options) {
    item.unknown_0000[0] = record.type;
    item.unknown_0000[1] = record.index;
    item.unknown_0000[2] = record.location;
    item.unknown_0000[3] = record.wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.flags & 1;
    item.unknown_005D = (record.flags >> 1) & 1;
    item.unknown_0060 = record.refine;
    item.unknown_0068 = record.expireTime;
    item.unknown_0064 = record.bindOnEquip;
    item.unknown_0070 = record.sprite;
    item.unknown_0074[0] = (record.flags >> 2) & 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    item.unknown_0088 = 0;
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
  }

  // INLINED
  __forceinline void FillEquipWindowGradeItem(ITEM_INFO& item, const EquipWindowGradeRecord& record, const StoreItemOption* options) {
    item.unknown_0000[0] = record.type;
    item.unknown_0000[1] = record.index;
    item.unknown_0000[2] = record.location;
    item.unknown_0000[3] = record.wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.flags & 1;
    item.unknown_005D = (record.flags >> 1) & 1;
    item.unknown_0060 = record.refine;
    item.unknown_0068 = record.expireTime;
    item.unknown_0064 = record.bindOnEquip;
    item.unknown_0070 = record.sprite;
    item.unknown_0074[0] = (record.flags >> 2) & 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    item.unknown_0088 = record.grade;
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = options[index];
    }
  }

  // INLINED
  __forceinline void EquipWindowItem(const ITEM_INFO& item) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.unknown_005D) {
      g_session.unknown_1050 = item.unknown_0000[1];
    }

    if (item.unknown_0000[3] != 0) {
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem3500(item);
      } else {
        g_session.Equip(item);
      }
    }

    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.unknown_005D) {
      const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.unknown_1050).itemId.c_str());
      if (!g_petEvolutionMgr->IsAutoFeeding(petEgg) && g_petAutoFeed == true) {
        g_session.SendPetAutoFeedOption(0);
      }

      if (g_equipmentPropertiesMgr == nullptr) {
        g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
      }

      const int kind = 0;
      const int itemId = std::atoi(item.itemId.c_str());
      g_equipmentPropertiesMgr->AddExpansion(kind, itemId, false);
    }
  }
}

// OFFSET: 0x00D28950
// FUNCTION: fn_21696
void CGameMode::ReceiveEquipWindowMicroscope4(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetItems3500();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 67u;
  const StoreEquipOptionRecord* record = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 45);
  for (int index = 0; index < count; ++index, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    FillEquipWindowItem(item, *record, options);
    EquipWindowItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
  g_equipWindowLook.job = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  g_equipWindowLook.sex = packet[0x2C];
  g_equipWindowLook.head = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  g_equipWindowLook.accessory = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  g_equipWindowLook.accessory2 = *reinterpret_cast<const std::int16_t*>(packet + 0x22);
  g_equipWindowLook.accessory3 = *reinterpret_cast<const std::int16_t*>(packet + 0x24);
  g_equipWindowLook.bodyPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x2A);
  g_equipWindowLook.headPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x28);
  memcpy_s(g_equipWindowLook.name, sizeof(g_equipWindowLook.name), packet + 4, sizeof(g_equipWindowLook.name));
  g_equipWindowLook.robe = *reinterpret_cast<const std::int16_t*>(packet + 0x26);

  if (!g_windowMgr.CloseWindow(0x8B)) {
    g_windowMgr.EnsureWindow(0x8B);
  }
}

// OFFSET: 0x00D28EA0
// FUNCTION: fn_21697
void CGameMode::ReceiveEquipWindowMicroscope5(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetItems3500();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 47) / 67u;
  const StoreEquipOptionRecord* record = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 47);
  for (int index = 0; index < count; ++index, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    FillEquipWindowItem(item, *record, options);
    EquipWindowItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
  g_equipWindowLook.job = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  g_equipWindowLook.sex = packet[0x2E];
  g_equipWindowLook.head = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  g_equipWindowLook.accessory = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  g_equipWindowLook.accessory2 = *reinterpret_cast<const std::int16_t*>(packet + 0x22);
  g_equipWindowLook.accessory3 = *reinterpret_cast<const std::int16_t*>(packet + 0x24);
  g_equipWindowLook.bodyPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x2A);
  g_equipWindowLook.headPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x28);
  memcpy_s(g_equipWindowLook.name, sizeof(g_equipWindowLook.name), packet + 4, sizeof(g_equipWindowLook.name));
  g_equipWindowLook.robe = *reinterpret_cast<const std::int16_t*>(packet + 0x26);
  g_equipWindowLook.unknown_0024 = *reinterpret_cast<const std::int16_t*>(packet + 0x2C);

  if (!g_windowMgr.CloseWindow(0x8B)) {
    g_windowMgr.EnsureWindow(0x8B);
  }
}

// OFFSET: 0x00D29400
// FUNCTION: fn_21698
void CGameMode::ReceiveEquipWindowMicroscope6(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetItems3500();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 47) / 68u;
  const EquipWindowGradeRecord* record = reinterpret_cast<const EquipWindowGradeRecord*>(packet + 47);
  for (int index = 0; index < count; ++index, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    FillEquipWindowGradeItem(item, *record, options);
    EquipWindowItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
  g_equipWindowLook.job = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  g_equipWindowLook.sex = packet[0x2E];
  g_equipWindowLook.head = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  g_equipWindowLook.accessory = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  g_equipWindowLook.accessory2 = *reinterpret_cast<const std::int16_t*>(packet + 0x22);
  g_equipWindowLook.accessory3 = *reinterpret_cast<const std::int16_t*>(packet + 0x24);
  g_equipWindowLook.bodyPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x2A);
  g_equipWindowLook.headPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x28);
  memcpy_s(g_equipWindowLook.name, sizeof(g_equipWindowLook.name), packet + 4, sizeof(g_equipWindowLook.name));
  g_equipWindowLook.robe = *reinterpret_cast<const std::int16_t*>(packet + 0x26);
  g_equipWindowLook.unknown_0024 = *reinterpret_cast<const std::int16_t*>(packet + 0x2C);

  if (!g_windowMgr.CloseWindow(0x8B)) {
    g_windowMgr.EnsureWindow(0x8B);
  }
}

namespace {
  // INLINED
  __forceinline void AddInventoryEquipItem(const ITEM_INFO& item) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.unknown_005D) {
      g_session.unknown_1050 = item.unknown_0000[1];
    }

    if (item.unknown_0000[3] == 0) {
      g_session.AddItem1710(item, 0);
      g_session.UpdateInventoryItem(item, 0);
    } else {
      char equipped = 1;
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem2B50(item, &equipped);
      } else {
        g_session.EquipUnknown17F0(item, &equipped);
      }
    }

    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.unknown_005D) {
      const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.unknown_1050).itemId.c_str());
      if (!g_petEvolutionMgr->IsAutoFeeding(petEgg) && g_petAutoFeed == true) {
        g_session.SendPetAutoFeedOption(0);
      }

      if (g_equipmentPropertiesMgr == nullptr) {
        g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
      }

      const int kind = 0;
      const int itemId = std::atoi(item.itemId.c_str());
      g_equipmentPropertiesMgr->AddExpansion(kind, itemId, false);
    }
  }
}

// OFFSET: 0x00D2C7F0
// FUNCTION: fn_21717
void CGameMode::ReceiveInventoryEquipList(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetItems2B50();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 4) / 41u;
  const StoreEquipRecord* record = reinterpret_cast<const StoreEquipRecord*>(packet + 4);
  for (int index = 0; index < count; ++index, ++record) {
    StoreItemOption options[5] = {};
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[2] = record->location;
    item.unknown_0000[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.unknown_005D = (record->flags >> 1) & 1;
    item.unknown_0060 = record->refine;
    item.unknown_0068 = record->expireTime;
    item.unknown_0064 = record->bindOnEquip;
    item.unknown_0070 = record->sprite;
    item.unknown_0074[0] = (record->flags >> 2) & 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    item.unknown_0088 = 0;
    for (int option = 0; option < item.unknown_0098; ++option) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
    }
    AddInventoryEquipItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D2CC80
// FUNCTION: fn_21718
void CGameMode::ReceiveInventoryEquipList2(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetItems2B50();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 4) / 67u;
  const StoreEquipOptionRecord* record = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 4);
  for (int index = 0; index < count; ++index, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    FillEquipWindowItem(item, *record, options);
    AddInventoryEquipItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D2D140
// FUNCTION: fn_21719
void CGameMode::ReceiveInventoryEquipList3(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetItems2B50();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 4) / 68u;
  const EquipWindowGradeRecord* record = reinterpret_cast<const EquipWindowGradeRecord*>(packet + 4);
  for (int index = 0; index < count; ++index, ++record) {
    StoreItemOption options[5];
    std::memcpy(options, record->options, sizeof(options));
    ITEM_INFO item;
    FillEquipWindowGradeItem(item, *record, options);
    AddInventoryEquipItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
}

struct ItemCardSlots {
  /* 0000 */ std::uint32_t cards[4];
};

namespace {
#pragma pack(push, 1)
  struct ItemPickupAckPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t index;
    /* 0004 */ std::uint16_t amount;
    /* 0006 */ int itemId;
    /* 000A */ unsigned char identified;
    /* 000B */ unsigned char damaged;
    /* 000C */ unsigned char refine;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ std::uint16_t location;
    /* 001F */ unsigned char type;
    /* 0020 */ unsigned char result;
  };

  struct ItemPickupAck2Packet {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t index;
    /* 0004 */ std::uint16_t amount;
    /* 0006 */ int itemId;
    /* 000A */ unsigned char identified;
    /* 000B */ unsigned char damaged;
    /* 000C */ unsigned char refine;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ std::uint16_t location;
    /* 001F */ unsigned char type;
    /* 0020 */ unsigned char result;
    /* 0021 */ std::uint32_t expireTime;
  };

  struct ItemPickupAck3Packet {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t index;
    /* 0004 */ std::uint16_t amount;
    /* 0006 */ int itemId;
    /* 000A */ unsigned char identified;
    /* 000B */ unsigned char damaged;
    /* 000C */ unsigned char refine;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ std::uint16_t location;
    /* 001F */ unsigned char type;
    /* 0020 */ unsigned char result;
    /* 0021 */ std::uint32_t expireTime;
    /* 0025 */ std::uint16_t bindOnEquip;
  };

  struct ItemPickupAck7Packet {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t index;
    /* 0004 */ std::uint16_t amount;
    /* 0006 */ int itemId;
    /* 000A */ unsigned char identified;
    /* 000B */ unsigned char damaged;
    /* 000C */ unsigned char refine;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ std::uint32_t location;
    /* 0021 */ unsigned char type;
    /* 0022 */ unsigned char result;
    /* 0023 */ std::uint32_t expireTime;
    /* 0027 */ std::uint16_t bindOnEquip;
    /* 0029 */ StoreItemOption options[5];
  };
#pragma pack(pop)

  static_assert(sizeof(ItemPickupAckPacket) == 0x21);
  static_assert(sizeof(ItemPickupAck2Packet) == 0x25);
  static_assert(sizeof(ItemPickupAck3Packet) == 0x27);
  static_assert(sizeof(ItemPickupAck7Packet) == 0x42);
}

// OFFSET: 0x00D2DF80
// FUNCTION: fn_21725
void CGameMode::ReceiveItemPickupAck2(const unsigned char* data) {
  const ItemPickupAck2Packet* packet = reinterpret_cast<const ItemPickupAck2Packet*>(data);
  this->unknown_01A4.clear();

  if (packet->result == 0) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }

    g_session.AddItem1710(item, 1);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  } else if (packet->result == 3) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    switch (packet->type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
    default:
      item.unknown_0000[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.unknown_0000[1] != 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&this->unknown_0100[0x14]) != 0) {
      *reinterpret_cast<int*>(&this->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
  } else {
    this->NotifyItemResult(packet->result);
  }
}

// OFFSET: 0x00D2E4F0
// FUNCTION: fn_21726
void CGameMode::ReceiveItemPickupAck3(const unsigned char* data) {
  const ItemPickupAck3Packet* packet = reinterpret_cast<const ItemPickupAck3Packet*>(data);
  this->unknown_01A4.clear();

  if (packet->result == 0) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    item.unknown_0064 = packet->bindOnEquip;
    item.unknown_0074[1] = 1;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }

    g_session.AddItem1710(item, 1);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else if (packet->result == 3) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    item.unknown_0064 = packet->bindOnEquip;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    switch (packet->type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
    default:
      item.unknown_0000[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.unknown_0000[1] != 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&this->unknown_0100[0x14]) != 0) {
      *reinterpret_cast<int*>(&this->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else {
    this->NotifyItemResult(packet->result);
  }
}

// OFFSET: 0x00D2EAE0
// FUNCTION: fn_21727
void CGameMode::ReceiveItemPickupAck(const unsigned char* data) {
  const ItemPickupAckPacket* packet = reinterpret_cast<const ItemPickupAckPacket*>(data);
  this->unknown_01A4.clear();

  if (packet->result == 0) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }

    g_session.AddItem1710(item, 1);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  } else if (packet->result == 3) {
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    switch (packet->type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
    default:
      item.unknown_0000[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    std::string text;
    int amount;
    if (current.unknown_0000[1] != 0) {
      text = FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount - current.quantity);
      amount = packet->amount - current.quantity;
    } else {
      text = FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount);
      amount = packet->amount;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&this->unknown_0100[0x14]) != 0) {
      *reinterpret_cast<int*>(&this->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
  } else {
    this->NotifyItemResult(packet->result);
  }
}

// OFFSET: 0x00D2F200
// FUNCTION: fn_21729
void CGameMode::ReceiveItemPickupAck5(
  std::uint16_t index, std::uint16_t amount, int itemId, unsigned char identified, unsigned char damaged, unsigned char refine, ItemCardSlots slots,
  std::uint32_t location, unsigned char type, unsigned char result, std::uint32_t expireTime, std::uint16_t bindOnEquip
) {
  this->unknown_01A4.clear();

  if (result == 0) {
    {
      StoreItemOption options[5] = {};
      ITEM_INFO item;
      item.unknown_0000[0] = type;
      item.unknown_0000[2] = location;
      item.unknown_0000[1] = index;
      item.unknown_0000[3] = 0;
      item.quantity = amount;
      item.SetItemId(itemId);
      std::memcpy(item.unknown_001C, slots.cards, sizeof(slots.cards));
      item.identified = identified;
      item.unknown_005D = damaged;
      item.unknown_0060 = refine;
      item.unknown_0068 = expireTime;
      item.unknown_0064 = bindOnEquip;
      item.unknown_0074[1] = 1;
      item.unknown_0098 = CountStoreItemOptions(options);
      for (int option = 0; option < item.unknown_0098; ++option) {
        reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
      }
      if (std::atoi(item.itemId.c_str()) == 607) {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
      }
      if (type == 10 || type == 16 || type == 17 || type == 19) {
        item.unknown_0000[3] = 0x8000;
      }

      g_session.AddItem1710(item, 1);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

      UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
      window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
      window->SendMsg(nullptr, 0x24, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
        quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
      }
    }
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else if (result == 3) {
    StoreItemOption options[5] = {};
    ITEM_INFO item;
    item.unknown_0000[0] = type;
    item.unknown_0000[2] = location;
    item.unknown_0000[1] = index;
    item.unknown_0000[3] = 0;
    item.quantity = amount;
    item.SetItemId(itemId);
    std::memcpy(item.unknown_001C, slots.cards, sizeof(slots.cards));
    item.identified = identified;
    item.unknown_005D = damaged;
    item.unknown_0060 = refine;
    item.unknown_0068 = expireTime;
    item.unknown_0064 = bindOnEquip;
    item.unknown_0074[1] = 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int option = 0; option < item.unknown_0098; ++option) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
    }
    if (type == 10 || type == 16 || type == 17 || type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    switch (type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
    default:
      item.unknown_0000[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    int count = amount;
    if (current.unknown_0000[1] != 0) {
      count -= current.quantity;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), count).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, count, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else {
    this->NotifyItemResult(result);
  }
}

// OFFSET: 0x00D2F8F0
// FUNCTION: fn_21730
void CGameMode::ReceiveItemPickupAck7(const unsigned char* data) {
  const ItemPickupAck7Packet* packet = reinterpret_cast<const ItemPickupAck7Packet*>(data);
  this->unknown_01A4.clear();

  if (packet->result == 0) {
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    item.unknown_0064 = packet->bindOnEquip;
    item.unknown_0074[1] = 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int option = 0; option < item.unknown_0098; ++option) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }

    g_session.AddItem1710(item, 1);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else if (packet->result == 3) {
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    item.unknown_0064 = packet->bindOnEquip;
    item.unknown_0074[1] = 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int option = 0; option < item.unknown_0098; ++option) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    switch (packet->type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
    default:
      item.unknown_0000[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.unknown_0000[1] != 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else {
    this->NotifyItemResult(packet->result);
  }
}

// OFFSET: 0x00D27A60
// FUNCTION: fn_21691
void CGameMode::ReceiveEquipItemDamaged(const unsigned char* packet) {
  const std::uint16_t location = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  if (location == 0) {
    return;
  }

  std::string text;
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  if (aid != g_session.m_aid) {
    MessengerMember member = g_session.PartyByAccount(aid);
    if (member.valid) {
      switch (location) {
      case 0x20:
        text = FormatOwnedString(LookupMessage(0x515), member.name.c_str(), LookupMessage(0x109A));
        break;
      case 2:
        text = FormatOwnedString(LookupMessage(0x515), member.name.c_str(), LookupMessage(0x109B));
        break;
      case 0x10:
        text = FormatOwnedString(LookupMessage(0x515), member.name.c_str(), LookupMessage(0x517));
        break;
      case 0x100:
        text = FormatOwnedString(LookupMessage(0x515), member.name.c_str(), LookupMessage(0x10CB));
        break;
      }
    }
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    ITEM_INFO item = g_session.GetItem17F0(slots.values[2]);
    if (item.unknown_0000[1] != 0) {
      text = FormatOwnedString(LookupMessage(0x514), item.DisplayName(0).c_str());
    }
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0xD, 0, 0);
  }
}

namespace {
#pragma pack(push, 1)
  struct ExpandedBarterChunkRecord {
    /* 0000 */ int itemId;
    /* 0004 */ std::uint16_t refine;
    /* 0006 */ int amount;
    /* 000A */ std::uint16_t type;
  };

  struct ExpandedBarterRecord {
    /* 0000 */ int itemId;
    /* 0004 */ std::uint16_t type;
    /* 0006 */ int amount;
    /* 000A */ int price;
    /* 000E */ int index;
    /* 0012 */ int weight;
    /* 0016 */ int chunkCount;
  };

  struct ExpandedBarterRecord2 {
    /* 0000 */ int itemId;
    /* 0004 */ std::uint16_t type;
    /* 0006 */ int amount;
    /* 000A */ int price;
    /* 000E */ int index;
    /* 0012 */ int weight;
    /* 0016 */ std::uint16_t viewSprite;
    /* 0018 */ int location;
    /* 001C */ int chunkCount;
    /* 0020 */ int currency;
  };

  struct ExpandedBarterListPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t length;
    /* 0004 */ int count;
  };
#pragma pack(pop)

  static_assert(sizeof(ExpandedBarterChunkRecord) == 0xC);
  static_assert(sizeof(ExpandedBarterRecord) == 0x1A);
  static_assert(sizeof(ExpandedBarterRecord2) == 0x24);
  static_assert(sizeof(ExpandedBarterListPacket) == 0x8);
}

// OFFSET: 0x00D29A30
// FUNCTION: fn_21699
void CGameMode::ReceiveExpandedBarterMarketItemList2(const unsigned char* data) {
  if (data == nullptr) {
    return;
  }

  g_expandedBarterMarketItemMgr->Clear();
  const int count = reinterpret_cast<const ExpandedBarterListPacket*>(data)->count;
  const unsigned char* cursor = data + sizeof(ExpandedBarterListPacket);
  for (std::int64_t i = 0; i < count; ++i) {
    ExpandedBarterRecord2 record;
    memcpy_s(&record, sizeof(record), cursor, sizeof(record));

    BarterMarketEntry entry;
    char number[16] = {};
    _itoa(record.itemId, number, 10);
    entry.name = number;
    cursor += sizeof(record);
    entry.field_0 = record.type;
    entry.value = record.amount;
    entry.factor = static_cast<float>(record.price / 10);
    entry.id = record.index;
    entry.field_8 = record.weight;
    entry.field_38 = record.location;
    entry.field_3c = record.viewSprite;
    entry.field_28 = record.currency;
    for (std::int64_t j = 0; j < record.chunkCount; ++j) {
      ExpandedBarterChunkRecord source;
      memcpy_s(&source, sizeof(source), cursor, sizeof(source));
      BarterChunk16 chunk;
      chunk.field_0 = source.itemId;
      chunk.field_4 = source.refine;
      chunk.amount = source.amount;
      chunk.field_c = source.type;
      entry.chunks.push_back(chunk);
      cursor += sizeof(source);
    }
    g_expandedBarterMarketItemMgr->AddItem(entry);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(0x155)) {
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(0x156)) {
    window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D29D60
// FUNCTION: fn_21700
void CGameMode::ReceiveExpandedBarterMarketItemList(const unsigned char* data) {
  g_expandedBarterMarketItemMgr->Clear();
  const int count = reinterpret_cast<const ExpandedBarterListPacket*>(data)->count;
  const unsigned char* cursor = data + sizeof(ExpandedBarterListPacket);
  for (int i = 0; i < count; ++i) {
    const ExpandedBarterRecord record = *reinterpret_cast<const ExpandedBarterRecord*>(cursor);
    BarterMarketEntry entry;
    char number[16] = {};
    _itoa(record.itemId, number, 10);
    entry.name = number;
    cursor += sizeof(record);
    entry.field_0 = record.type;
    entry.value = record.amount;
    entry.factor = static_cast<float>(record.price / 10);
    entry.id = record.index;
    entry.field_8 = record.weight;
    for (int j = 0; j < record.chunkCount; ++j) {
      const ExpandedBarterChunkRecord source = *reinterpret_cast<const ExpandedBarterChunkRecord*>(cursor);
      BarterChunk16 chunk;
      chunk.field_0 = source.itemId;
      chunk.field_4 = source.refine;
      chunk.amount = source.amount;
      chunk.field_c = source.type;
      entry.chunks.push_back(chunk);
      cursor += sizeof(source);
    }
    g_expandedBarterMarketItemMgr->AddItem(entry);
  }

  g_windowMgr.EnsureWindow(0x155)->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  g_windowMgr.EnsureWindow(0x156)->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
}

namespace {
#pragma pack(push, 1)
  struct FeedResultPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ unsigned char result;
    /* 0003 */ int itemId;
  };
#pragma pack(pop)

  static_assert(sizeof(FeedResultPacket) == 7);
}

// OFFSET: 0x00D2A450
// FUNCTION: fn_21704
void CGameMode::ReceiveFeedResult(const unsigned char* data) {
  const FeedResultPacket* packet = reinterpret_cast<const FeedResultPacket*>(data);
  if (packet->packetType == 0x1A3) {
    if (packet->result != 0) {
      const int friendlyLevel = g_session.GetPetFriendlyState(g_session.m_petFriendly);
      const int hungerLevel = g_session.GetPetHungryState(g_session.unknown_1054);
      const int emotion = g_session.GetPetEmotionNumber(g_session.unknown_103C, 0, hungerLevel);
      const int action = g_session.GetPetActionNumber(hungerLevel, friendlyLevel, 0);
      this->SendMsg(148, emotion, 0, 0, 0);
      this->SendMsg(148, action, 0, 0, 0);

      const int count = LookupClientItem(g_session.m_inventoryItems, packet->itemId, 0).quantity;
      char name[260];
      std::memset(name, 0, sizeof(name));
      ITEM_INFO item;
      item.SetItemId(packet->itemId);
      int capacity = sizeof(name);
      item.sub_6D5810(name, capacity, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xA12), name, count).c_str()), 0xFFFF64, 0, 0, 0);
    } else {
      ITEM_INFO item;
      item.SetItemId(packet->itemId);
      const std::string text = FormatOwnedString(LookupMessage(0x24F), item.DisplayName(0).c_str());
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
      g_windowMgr.SendNotice(0x13, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
    }
  } else if (packet->packetType == 0x22F) {
    if (packet->result != 0) {
      g_session.m_homunActionState = 0x1D;

      const int count = LookupClientItem(g_session.m_inventoryItems, packet->itemId, 0).quantity;
      char name[260];
      std::memset(name, 0, sizeof(name));
      ITEM_INFO item;
      item.SetItemId(packet->itemId);
      int capacity = sizeof(name);
      item.sub_6D5810(name, capacity, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xCD1), name, count).c_str()), 0xFFFF64, 0, 0, 0);
    } else {
      ITEM_INFO item;
      item.SetItemId(packet->itemId);
      const std::string text = FormatOwnedString(LookupMessage(0x24F), item.DisplayName(0).c_str());
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
      g_windowMgr.SendNotice(0x13, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
      if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(&g_session.unknown_5570[8]))) {
        const int emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Sweat);
        actor->SendMsg(nullptr, 160, 0, 0, 0, emotion, emotion >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
  }
}

namespace {
#pragma pack(push, 1)
  struct PetChangeStatePacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ char type;
    /* 0003 */ std::uint32_t aid;
    /* 0007 */ int value;
  };
#pragma pack(pop)

  static_assert(sizeof(PetChangeStatePacket) == 0xB);
}

// OFFSET: 0x00D26080
// FUNCTION: fn_21678
void CGameMode::ReceivePetChangeState(const unsigned char* data) {
  const PetChangeStatePacket* packet = reinterpret_cast<const PetChangeStatePacket*>(data);
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  switch (packet->type) {
  case 0:
    g_session.unknown_1010 = packet->aid;
    if (actor) {
      g_session.unknown_103C = actor->GetEffectiveJob(-1);
      actor->SendMsg(nullptr, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->unknown_0314 = 7;
    }
    break;
  case 1: {
    const int previousFriendly = g_session.m_petFriendly;
    const int friendlyState = g_session.GetPetFriendlyState(previousFriendly);
    g_session.m_petFriendly = packet->value;
    if (actor) {
      g_session.unknown_103C = actor->GetEffectiveJob(-1);
    }
    if (UIPetInfoWnd* window = static_cast<UIPetInfoWnd*>(g_windowMgr.window_038C)) {
      window->UpdateFriendlyGauge();
    }
    if (g_session.GetDisplayJob() >= 0xFD2 && g_session.GetDisplayJob() <= 0xFD4 && packet->value < 100 && g_session.unknown_1044 < 30) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x4A4)), 0x3232FF, 0, 0, 0);
    }
    if (friendlyState != g_session.GetPetFriendlyState(g_session.m_petFriendly)) {
      EquipmentProperties::CEquipmentPropertiesMgr* properties = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
      const int itemId = -1;
      const int kind = 0;
      properties->RefreshExpansion(kind, itemId);
    }
    this->UpdatePetFriendlyWindow(previousFriendly);
    break;
  }
  case 2:
    g_session.unknown_1054 = g_session.unknown_1044;
    g_session.unknown_1044 = packet->value;
    if (actor) {
      g_session.unknown_103C = actor->GetEffectiveJob(-1);
      actor->SendMsg(nullptr, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_windowMgr.window_038C) {
      g_windowMgr.window_038C->Invalidate();
    }
    break;
  case 3:
    if (!actor) {
      break;
    }
    if (packet->value != 0) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(packet->value));
      g_session.unknown_1038 = packet->value;
    } else {
      g_session.unknown_1038 = 0;
      actor->Slot21(actor->GetEffectiveJob(-1));
    }
    if (packet->aid == g_session.unknown_1010 && g_windowMgr.window_038C) {
      g_windowMgr.window_038C->Invalidate();
    }
    break;
  case 4:
    if (actor) {
      actor->SetUnknown02C8(packet->value);
      actor->Slot15(0, 1);
    }
    break;
  case 6:
    if (packet->value == 0) {
      g_session.ResetUnknown1050();
      if (UIWindow* window = g_windowMgr.LookupWindow(8)) {
        window->Invalidate();
      }
      const int itemId = -1;
      const int kind = 0;
      EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->RemoveExpansion(kind, itemId);
      g_windowMgr.CloseWindow(0x58);
    } else if (packet->value == 1) {
      if (UIItemIdentifyWnd* window = static_cast<UIItemIdentifyWnd*>(g_windowMgr.LookupWindow(0x5A))) {
        int found = 0;
        ITEM_INFO item = window->GetSelectedItem(&found);
        if (found == 1) {
          item.unknown_005D = 1;
          g_session.UpdateInventoryItem(item, 1);
          g_session.unknown_1050 = item.unknown_0000[1];
          if (UIWindow* inventory = g_windowMgr.LookupWindow(8)) {
            inventory->Invalidate();
          }
          EquipmentProperties::CEquipmentPropertiesMgr* properties = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
          const int itemId = item.GetItemId();
          const int kind = 0;
          properties->AddExpansion(kind, itemId, true);
        }
        g_windowMgr.CloseWindow(0x5A);
      }
    }
    break;
  case 7:
    if (g_serviceOptions.Get(0x128) == 1) {
      g_windowMgr.AddMessageBox(LookupMessage(0x10F8), 0, true, 0, 0, LookupMessage(0xDF6), 0x118, 0x78, nullptr, false);
    }
    break;
  }
}

namespace {
#pragma pack(push, 1)
  struct GuildInfoPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint32_t guildId;
    /* 0006 */ int level;
    /* 000A */ int userCount;
    /* 000E */ int maxUserCount;
    /* 0012 */ int averageLevel;
    /* 0016 */ int exp;
    /* 001A */ int maxExp;
    /* 001E */ int point;
    /* 0022 */ int honor;
    /* 0026 */ int virtue;
    /* 002A */ int emblemVersion;
    /* 002E */ char guildName[24];
    /* 0046 */ char masterName[24];
    /* 005E */ char manageLand[24];
  };
#pragma pack(pop)

  static_assert(sizeof(GuildInfoPacket) == 0x76);
}

// OFFSET: 0x00D2B360
// FUNCTION: fn_21709
void CGameMode::ReceiveGuildInfo(const unsigned char* data) {
  const GuildInfoPacket* packet = reinterpret_cast<const GuildInfoPacket*>(data);
  GuildInfo* guildInfo = &g_guildInfo;

  char guildName[25];
  guildName[24] = '\0';
  char masterName[25];
  masterName[24] = '\0';
  char manageLand[25];
  manageLand[24] = '\0';
  std::memcpy(guildName, packet->guildName, sizeof(packet->guildName));
  std::memcpy(masterName, packet->masterName, sizeof(packet->masterName));
  std::memcpy(manageLand, packet->manageLand, sizeof(packet->manageLand));
  if (g_forbiddenWords.Contains(guildName) == true) {
    std::string masked;
    g_forbiddenWords.Mask(guildName, &masked);
  }

  guildInfo->unknown_0000 = guildName;
  g_guildInfo.unknown_0018 = masterName;
  g_guildInfo.unknown_00B4 = g_guildInfo.unknown_0018 == g_session.DecodePlayerName();
  guildInfo->unknown_0074 = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->unknown_0060 = packet->level;
  guildInfo->unknown_0064 = packet->userCount;
  guildInfo->unknown_0068 = packet->userCount;
  guildInfo->unknown_006C = packet->maxUserCount;
  guildInfo->unknown_0070 = packet->averageLevel;
  guildInfo->unknown_008C = packet->exp;
  guildInfo->unknown_0090 = packet->maxExp;
  guildInfo->unknown_0098 = packet->point;
  guildInfo->unknown_009C = packet->honor;
  guildInfo->unknown_00A0 = packet->virtue;
  g_guildMgr->SetEmblemVersion(packet->guildId, packet->emblemVersion);
  g_windowMgr.RefreshGuildWindows();
}

namespace {
#pragma pack(push, 1)
  struct ChatRoomMemberRecord {
    /* 0000 */ int role;
    /* 0004 */ char name[24];
  };

  struct ChatRoomMemberListPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::int16_t length;
    /* 0004 */ std::uint32_t roomId;
  };
#pragma pack(pop)

  static_assert(sizeof(ChatRoomMemberRecord) == 0x1C);
  static_assert(sizeof(ChatRoomMemberListPacket) == 0x8);
}

// OFFSET: 0x00D1E840
// FUNCTION: fn_21636
void CGameMode::ReceiveSkillFailure(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const SkillFailurePacket*>(data);
  this->scene->player->render.isMotionFreezed = false;
  if (packet->result != 0) {
    return;
  }

  const unsigned char showMessage = g_serviceOptions.Get(0x71);
  char text[256];
  const char* notice;
  switch (packet->cause) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    if (packet->skillId == 1) {
      switch (packet->value) {
      case 0:
        notice = LookupMessage(0x9F);
        break;
      case 1:
        this->unknown_026E[0x16] = 0;
        notice = LookupMessage(0xA0);
        break;
      case 2:
        notice = LookupMessage(0xA1);
        break;
      case 3:
        notice = LookupMessage(0xA2);
        break;
      case 4:
        notice = LookupMessage(0xA3);
        break;
      case 5:
        notice = LookupMessage(0xA4);
        break;
      case 6:
        notice = LookupMessage(0xA5);
        break;
      case 7:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x17F)), 0xFF, 0xB, 0, 0);
        [[fallthrough]];
      case 8:
        notice = LookupMessage(0x518);
        break;
      default:
        return;
      }
    } else {
      switch (packet->skillId) {
      case 27:
        notice = LookupMessage(0xD6);
        break;
      case 50:
        notice = LookupMessage(0xCD);
        break;
      case 52:
        notice = LookupMessage(0xCF);
        break;
      default:
        if (!showMessage) {
          return;
        }
        notice = LookupMessage(0xCC);
        break;
      }
    }
    break;
  case 1:
    notice = LookupMessage(0xCA);
    break;
  case 2:
    notice = LookupMessage(0xCB);
    break;
  case 3:
    notice = LookupMessage(0x328);
    break;
  case 4:
    if (HidesUseFailureMessage(packet->skillId) || !showMessage) {
      return;
    }
    std::sprintf(text, LookupMessage(0x5B4), CSkillInfo::DisplayName(*g_session.m_hotkeyScript, packet->skillId));
    notice = text;
    break;
  case 5:
    notice = LookupMessage(0xE9);
    break;
  case 6:
    notice = LookupMessage(0xEF);
    break;
  case 7:
    notice = LookupMessage(0xF6);
    break;
  case 8:
    notice = LookupMessage(0xF7);
    break;
  case 9:
    notice = LookupMessage(0x244);
    break;
  case 10:
    if (packet->skillId == 0x8A9 || packet->skillId == 0x8AA || packet->skillId == 0x8AB || packet->skillId == 0x8AC || packet->skillId == 0x8AD) {
      this->scene->player->SendMsg(nullptr, 0x8F, 0, 0x2DF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, 0x8F, 0, 0x2E0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, 0x8F, 0, 0x2E1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, 0x8F, 0, 0x2E2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (!showMessage) {
      return;
    }
    notice = LookupMessage(0x11D);
    break;
  case 11:
    notice = LookupMessage(0x574);
    break;
  case 12:
    notice = LookupMessage(0x575);
    break;
  case 13:
    notice = LookupMessage(0x576);
    break;
  case 14:
    notice = LookupMessage(0x577);
    break;
  case 15:
    notice = LookupMessage(0x578);
    break;
  case 16:
    notice = LookupMessage(0x579);
    break;
  case 17:
    notice = LookupMessage(0x583);
    break;
  case 18:
    notice = LookupMessage(0x584);
    break;
  case 19:
    notice = LookupMessage(0x585);
    break;
  case 20:
    notice = LookupMessage(0x586);
    break;
  case 21:
    notice = LookupMessage(0x587);
    break;
  case 22:
    notice = LookupMessage(0x588);
    break;
  case 23:
    notice = LookupMessage(0x589);
    break;
  case 24:
    notice = LookupMessage(0x591);
    break;
  case 25:
    notice = LookupMessage(0x748);
    break;
  case 26:
    notice = LookupMessage(0x593);
    break;
  case 27:
    notice = LookupMessage(0x594);
    break;
  case 28:
    notice = LookupMessage(0x789);
    break;
  case 29:
    notice = LookupMessage(0x78C);
    break;
  case 30:
    notice = LookupMessage(0x791);
    break;
  case 31:
    notice = LookupMessage(0x599);
    break;
  case 32:
    notice = LookupMessage(0x59A);
    break;
  case 33:
    notice = LookupMessage(0x59B);
    break;
  case 34:
    notice = LookupMessage(0x79C);
    break;
  case 35:
    notice = LookupMessage(0x7A0);
    break;
  case 36:
    notice = LookupMessage(0x79F);
    break;
  case 37:
    notice = LookupMessage(0x59F);
    break;
  case 40:
    notice = LookupMessage(0x5AA);
    break;
  case 43:
    notice = LookupMessage(0x5AD);
    break;
  case 50:
    notice = LookupMessage(0x5B6);
    break;
  case 51:
    notice = LookupMessage(0x5C2);
    break;
  case 52:
    notice = LookupMessage(0x5C3);
    break;
  case 53:
    notice = LookupMessage(0x5C4);
    break;
  case 54:
    notice = LookupMessage(0x5C5);
    break;
  case 57:
    notice = LookupMessage(0x5EF);
    break;
  case 60:
    notice = LookupMessage(0x5F2);
    break;
  case 71: {
    ITEM_INFO item;
    item.SetItemId(packet->itemId);
    const std::string message = FormatOwnedString(LookupMessage(0x600), item.DisplayName(0).c_str(), packet->value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 72: {
    ITEM_INFO item;
    item.SetItemId(packet->itemId);
    item.identified = 1;
    const std::string message = FormatOwnedString(LookupMessage(0x602), item.DisplayName(0).c_str());
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 73:
    if (packet->value == 0) {
      std::sprintf(text, LookupMessage(0x618));
    } else {
      std::sprintf(text, LookupMessage(0x617), CSkillInfo::DisplayName(*g_session.m_hotkeyScript, packet->value));
    }
    notice = text;
    break;
  case 74:
    if (packet->value == 0) {
      std::sprintf(text, LookupMessage(0x61A));
    } else {
      std::sprintf(text, LookupMessage(0x619), packet->value);
    }
    notice = text;
    break;
  case 78: {
    const int reason = packet->value;
    if (reason == 0x15) {
      notice = LookupMessage(0xA01);
    } else if (reason == 0x11) {
      notice = LookupMessage(0x9FD);
    } else if (reason == 0x12) {
      notice = LookupMessage(0x9FC);
    } else if (reason == 0x14) {
      notice = LookupMessage(0x9FB);
    } else if (reason == 0x13) {
      notice = LookupMessage(0x9FA);
    } else if (reason == 0xB) {
      notice = LookupMessage(0x69B);
    } else if (reason == 0xD || reason == 0xE) {
      notice = LookupMessage(0x69C);
    } else if (reason == 4) {
      notice = LookupMessage(0x747);
    } else if (reason == 0x16) {
      notice = LookupMessage(0x7CA);
    } else {
      return;
    }
    break;
  }
  case 79:
    notice = LookupMessage(0x6D4);
    break;
  case 80:
    notice = LookupMessage(0x71F);
    break;
  case 81:
    notice = LookupMessage(0x720);
    break;
  case 82:
    notice = LookupMessage(0x721);
    break;
  case 83:
    notice = LookupMessage(0x7A2);
    break;
  case 84:
    notice = LookupMessage(0x9A2);
    break;
  case 85:
    std::memset(text, 0, sizeof(text));
    if (packet->value > 0) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(0x9F9), packet->value);
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(0x9F8));
    }
    notice = text;
    break;
  case 86:
    notice = LookupMessage(packet->value);
    break;
  case 87:
    notice = LookupMessage(0xA03);
    break;
  case 88:
    notice = LookupMessage(0xA42);
    break;
  case 89:
    notice = LookupMessage(0xBA6);
    break;
  case 90: {
    const std::string message = FormatOwnedString(LookupMessage(0xBB5), packet->value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 91: {
    const std::string message = FormatOwnedString(LookupMessage(0xBBE), packet->value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 92: {
    const std::string message = FormatOwnedString(LookupMessage(0xD82), packet->value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 93:
    notice = LookupMessage(0xD9A);
    break;
  case 94:
    notice = LookupMessage(0xD9B);
    break;
  case 95:
    notice = LookupMessage(0xDC9);
    break;
  case 96:
    notice = LookupMessage(0xDCA);
    break;
  case 97:
    notice = LookupMessage(0xDCD);
    break;
  case 98: {
    const std::string message = FormatOwnedString(LookupMessage(0xE00), packet->value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 99:
    notice = LookupMessage(0xE14);
    break;
  case 100:
    notice = LookupMessage(0xE3B);
    break;
  case 101: {
    const std::string message = FormatOwnedString(LookupMessage(0xEBD), packet->value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 102:
    notice = LookupMessage(0xF75);
    break;
  case 103:
    notice = LookupMessage(0xF76);
    break;
  case 104:
    notice = LookupMessage(0xF77);
    break;
  case 105:
    notice = LookupMessage(0xF78);
    break;
  case 106:
    notice = LookupMessage(0xF79);
    break;
  case 107:
    notice = LookupMessage(0xF7A);
    break;
  case 108:
    notice = LookupMessage(0xF7B);
    break;
  case 109:
    notice = LookupMessage(0xF7C);
    break;
  case 110:
    notice = LookupMessage(0xF7D);
    break;
  case 111:
    notice = LookupMessage(0xF80);
    break;
  case 112:
    notice = LookupMessage(0xF93);
    break;
  case 113:
    notice = LookupMessage(0xFA4);
    break;
  case 114:
    notice = LookupMessage(0xFA5);
    break;
  case 115:
    notice = LookupMessage(0x1061);
    break;
  case 116:
  case 117:
    notice = LookupMessage(packet->value != 0 ? packet->value : 0x1119);
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(notice), 0xFF, 0xB, 0, 0);
}
// OFFSET: 0x00D27420
// FUNCTION: fn_21690
void CGameMode::ReceiveChatRoomMemberList(const unsigned char* data) {
  const ChatRoomMemberListPacket* packet = reinterpret_cast<const ChatRoomMemberListPacket*>(data);
  this->unknown_00FC = 0;
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(this->unknown_01AC[packet->roomId]));

  ChatRoomTitleInfo info;
  bool joined = false;
  if (actor) {
    if (UIChatRoomTitle* room = reinterpret_cast<UIChatRoomTitle*>(actor->unknown_0264[1])) {
      info.title = room->title;
      info.unknown_0018 = room->unknown_0098;
      info.type = room->roomType;
      info.memberCount = room->memberCount;
      info.memberLimit = room->memberLimit;
      info.id = room->roomId;
      if (packet->roomId == room->roomId) {
        joined = true;
      }
    }
    actor->SendMsg(nullptr, 0x21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (joined) {
      const int count = static_cast<unsigned>(packet->length - sizeof(ChatRoomMemberListPacket)) / sizeof(ChatRoomMemberRecord);
      std::list<std::pair<std::string, int>> members;
      const ChatRoomMemberRecord* record = reinterpret_cast<const ChatRoomMemberRecord*>(data + sizeof(ChatRoomMemberListPacket));
      for (int i = 0; i < count; ++i, ++record) {
        std::pair<std::string, int> member;
        char name[25];
        name[24] = '\0';
        std::memcpy(name, record->name, sizeof(record->name));
        member.first = name;
        member.second = record->role;
        members.push_back(member);
      }

      UIWindow* window = g_windowMgr.EnsureWindow(0x1C);
      window->SendMsg(nullptr, 0x2B, 0, 0, 0, 0);
      info.memberCount = static_cast<int>(members.size());
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      for (std::list<std::pair<std::string, int>>::iterator it = members.begin(); it != members.end(); ++it) {
        window->SendMsg(nullptr, 0x2C, reinterpret_cast<std::intptr_t>(it->first.c_str()), it->second, 0, 0);
      }

      this->unknown_03C8 = info.title;
      this->unknown_03E0 = info.unknown_0018;
      this->unknown_03F8 = info.type;
      this->unknown_03FC = info.memberCount;
      this->unknown_0400 = info.memberLimit;
      this->unknown_0404 = info.id;
    }
  }
}

// GLOBAL: gl_25381
std::string g_gameModeNamelessTag = "<NAMELESS>";

namespace {
#pragma pack(push, 1)
  struct ItemPickupAckV8Packet {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t index;
    /* 0004 */ std::uint16_t amount;
    /* 0006 */ int itemId;
    /* 000A */ unsigned char identified;
    /* 000B */ unsigned char damaged;
    /* 000C */ unsigned char refine;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ std::uint32_t location;
    /* 0021 */ unsigned char type;
    /* 0022 */ unsigned char result;
    /* 0023 */ std::uint32_t expireTime;
    /* 0027 */ std::uint16_t bindOnEquip;
    /* 0029 */ StoreItemOption options[5];
    /* 0042 */ unsigned char favorite;
    /* 0043 */ std::uint16_t look;
  };

  struct ItemPickupAckV9Packet {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t index;
    /* 0004 */ std::uint16_t amount;
    /* 0006 */ int itemId;
    /* 000A */ unsigned char identified;
    /* 000B */ unsigned char damaged;
    /* 000C */ std::uint32_t cards[4];
    /* 001C */ std::uint32_t location;
    /* 0020 */ unsigned char type;
    /* 0021 */ unsigned char result;
    /* 0022 */ std::uint32_t expireTime;
    /* 0026 */ std::uint16_t bindOnEquip;
    /* 0028 */ StoreItemOption options[5];
    /* 0041 */ unsigned char favorite;
    /* 0042 */ std::uint16_t look;
    /* 0044 */ unsigned char refine;
    /* 0045 */ unsigned char grade;
  };
#pragma pack(pop)

  static_assert(sizeof(ItemPickupAckV8Packet) == 0x45);
  static_assert(sizeof(ItemPickupAckV9Packet) == 0x46);
}

// OFFSET: 0x00D30130
// FUNCTION: fn_21731
void CGameMode::ReceiveItemPickupAck8(const unsigned char* data) {
  const ItemPickupAckV8Packet* packet = reinterpret_cast<const ItemPickupAckV8Packet*>(data);
  this->unknown_01A4.clear();

  if (packet->result == 0) {
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    item.unknown_0064 = packet->bindOnEquip;
    item.unknown_0074[0] = packet->favorite;
    item.unknown_0074[1] = 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int option = 0; option < item.unknown_0098; ++option) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    item.unknown_0088 = 0;
    item.unknown_0070 = packet->look;

    g_session.AddItem1710(item, 1);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else if (packet->result == 3) {
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    item.unknown_0064 = packet->bindOnEquip;
    item.unknown_0074[0] = packet->favorite;
    item.unknown_0074[1] = 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int option = 0; option < item.unknown_0098; ++option) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    switch (packet->type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
    default:
      item.unknown_0000[2] = 0;
      break;
    }
    item.unknown_0088 = 0;
    item.unknown_0070 = packet->look;

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.unknown_0000[1] != 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else {
    this->NotifyItemResult(packet->result);
  }
}

// OFFSET: 0x00D309D0
// FUNCTION: fn_21732
void CGameMode::ReceiveItemPickupAck9(const unsigned char* data) {
  const ItemPickupAckV9Packet* packet = reinterpret_cast<const ItemPickupAckV9Packet*>(data);
  this->unknown_01A4.clear();

  if (packet->result == 0) {
    {
      StoreItemOption options[5];
      std::memcpy(options, packet->options, sizeof(options));
      ITEM_INFO item;
      item.unknown_0000[0] = packet->type;
      item.unknown_0000[2] = packet->location;
      item.unknown_0000[1] = packet->index;
      item.unknown_0000[3] = 0;
      item.quantity = packet->amount;
      item.SetItemId(packet->itemId);
      std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
      item.identified = packet->identified;
      item.unknown_005D = packet->damaged;
      item.unknown_0060 = packet->refine;
      item.unknown_0068 = packet->expireTime;
      item.unknown_0064 = packet->bindOnEquip;
      item.unknown_0074[1] = 1;
      item.unknown_0098 = CountStoreItemOptions(options);
      for (int option = 0; option < item.unknown_0098; ++option) {
        reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
      }
      item.unknown_0074[0] = packet->favorite;
      if (std::atoi(item.itemId.c_str()) == 607) {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
      }
      if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
        item.unknown_0000[3] = 0x8000;
      }
      item.unknown_0070 = packet->look;
      item.unknown_0088 = packet->grade;

      g_session.AddItem1710(item, 1);
      g_windowMgr.SendNotice(
        1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
      );

      UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
      window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
      window->SendMsg(nullptr, 0x24, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
        quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
      }
    }

    if (this->unknown_0648 == 1) {
      auto found = this->unknown_064C.find(packet->itemId);
      if (found == this->unknown_064C.end()) {
        this->unknown_064C.insert(std::make_pair(packet->itemId, packet->amount));
      } else {
        found->second += packet->amount;
      }
    }
  } else if (packet->result == 3) {
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    ITEM_INFO item;
    item.unknown_0000[0] = packet->type;
    item.unknown_0000[2] = packet->location;
    item.unknown_0000[1] = packet->index;
    item.unknown_0000[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.unknown_005D = packet->damaged;
    item.unknown_0060 = packet->refine;
    item.unknown_0068 = packet->expireTime;
    item.unknown_0064 = packet->bindOnEquip;
    item.unknown_0074[1] = 1;
    item.unknown_0098 = CountStoreItemOptions(options);
    for (int option = 0; option < item.unknown_0098; ++option) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[option] = options[option];
    }
    item.unknown_0074[0] = packet->favorite;
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05B8 = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    switch (packet->type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
    default:
      item.unknown_0000[2] = 0;
      break;
    }
    item.unknown_0070 = packet->look;
    item.unknown_0088 = packet->grade;

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.unknown_0000[1] != 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x99), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(0x3A);
    window->SendMsg(nullptr, 0x4C, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, 0x24, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(0x133)) {
      quest->SendMsg(nullptr, 6, 0x11E, 0xC2, 1, 0);
    }
  } else {
    this->NotifyItemResult(packet->result);
  }
}

// OFFSET: 0x00D339F0
// FUNCTION: fn_21751
void CGameMode::ReceiveActorChat(const unsigned char* data) {
  const int length = *reinterpret_cast<const std::uint16_t*>(data + 2) - 8;
  if (length <= 0 || length > 361) {
    return;
  }

  std::vector<char> text(length + 1, 0);
  memcpy_s(text.data(), text.size(), data + 8, length);
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(data + 4);
  CGameActor* speaker = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (speaker && !speaker->unknown_0314 && g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked;
    g_forbiddenWords.Mask(text.data(), &masked);
  }

  if (g_windowMgr.window_027C) {
    g_windowMgr.SendNotice(5, reinterpret_cast<std::intptr_t>(text.data()), 0x222222, 0, 0, 0);
  } else {
    if (IsMonsterJob(aid)) {
      if (g_serviceType == 5) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0xFFFF00, 1, 0, 0);
      } else {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), g_serviceType == 10 ? 0x1D8DFF : 0xFFFF, 1, 0, 0);
      }
    } else if (MonsterJobColor(aid)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), MonsterJobColor(aid), 0, 0, 0);
    } else if (!g_serviceOptions.Get(0x72) && !g_serviceOptions.Get(0x94)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0xFFFFFF, 1, 0, 0);
    }
    if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
      actor->SendMsg(nullptr, 7, 0, reinterpret_cast<int>(text.data()), reinterpret_cast<int>(text.data()) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
  if (g_whisperRecord != 0) {
    this->WriteChatLog(text.data());
  }
}

// OFFSET: 0x00D33CE0
// FUNCTION: fn_21752
void CGameMode::ReceivePartyChat(const unsigned char* data) {
  const int length = *reinterpret_cast<const std::uint16_t*>(data + 2) - 8;
  if (length <= 0 || length > 361) {
    return;
  }

  std::vector<char> text(length + 1, 0);
  memcpy_s(text.data(), text.size(), data + 8, length);
  if (g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked;
    g_forbiddenWords.Mask(text.data(), &masked);
  }

  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(data + 4);
  if (aid == g_session.m_aid) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0xC8FF, 3, 0, 0);
    this->scene->player->SendMsg(nullptr, 7, 0, reinterpret_cast<int>(text.data()), reinterpret_cast<int>(text.data()) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0xC8C8FF, 3, 0, 0);
    if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
      actor->SendMsg(nullptr, 7, 0, reinterpret_cast<int>(text.data()), reinterpret_cast<int>(text.data()) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
  if (g_whisperRecord != 0) {
    this->WriteChatLog(text.data());
  }
}

#pragma pack(push, 1)
struct EffectNumberPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int count;
  /* 000A */ int value;
};
#pragma pack(pop)

static_assert(sizeof(EffectNumberPacket) == 0xE);

// OFFSET: 0x00D37700
// FUNCTION: fn_21764
void CGameMode::ReceiveEffectNumber(const unsigned char* data) {
  const EffectNumberPacket* packet = reinterpret_cast<const EffectNumberPacket*>(data);
  if (*reinterpret_cast<int*>(&g_session.unknown_5454[0x8]) == 0 && this->scene->unknown_0048 == 0) {
    return;
  }

  vector3d deltaPos{0.0f, 0.0f, 0.0f};
  CGameActor* actor;
  if (packet->aid == g_session.m_aid && (actor = this->scene->player) != nullptr) {
    if (packet->count == 1) {
      if (static_cast<CPlayer*>(actor)->unknown_0494 != 1) {
        g_timeAttackStart = timeGetTime();
      }
    } else {
      g_timeAttackStart = 0;
    }

    const int previous = this->scene->player->unknown_0494;
    const int count = packet->count;
    if (previous != count) {
      if (previous > 10) {
        if (count <= 10) {
          char text[260];
          std::sprintf(text, LookupMessage(0x246), g_session.DecodePlayerName(), count);
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
        }
      } else if (count > 10) {
        char text[260];
        std::sprintf(text, LookupMessage(0x247), g_session.DecodePlayerName(), count);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
      }
      CPlayer* player = this->scene->player;
      g_soundMgr->PlayWave("effect\\number_change.wav", player->render.position.x, 0.0f, player->render.position.z, 250, 40, 1.0f, 0);
    }
    this->scene->player->unknown_0498 = packet->value;
    this->scene->player->unknown_0494 = packet->count;
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
    const std::uint32_t state = actor->Slot13();
    if (StateFlag02(state) || StateFlag01(state) || StateFlag06(state)) {
      return;
    }
  }

  if (!IsAdminAccount(actor->render.accountId) || g_session.IsAdminSubMode(7)) {
    actor->SendMsg(nullptr, 0x72, 0, packet->value, packet->value >> 31, packet->count, packet->count >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, 0x6D, 0, 0x9F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (packet->count <= 10) {
      if (CRagEffect* effect = actor->LaunchEffect(0x9F, deltaPos, 0.0f)) {
        effect->SendMsg(nullptr, 0x2C, 0, packet->count, 0, 0, 0, 0);
      }
    }
  }
}

#pragma pack(push, 1)
struct GuildBanRecord {
  /* 0000 */ char name[24];
  /* 0018 */ char reason[40];
};

struct GuildBanCharacterRecord {
  /* 0000 */ int characterId;
  /* 0004 */ char reason[40];
};

struct GuildBanNamedRecord {
  /* 0000 */ int characterId;
  /* 0004 */ char reason[40];
  /* 002C */ char name[24];
};
#pragma pack(pop)

static_assert(sizeof(GuildBanRecord) == 0x40);
static_assert(sizeof(GuildBanCharacterRecord) == 0x2C);
static_assert(sizeof(GuildBanNamedRecord) == 0x44);

// OFFSET: 0x00D4B860
// FUNCTION: fn_21866
void CGameMode::ReceiveGuildBanList(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(GuildBanRecord);
  guildInfo->unknown_00EC.clear();

  const GuildBanRecord* record = reinterpret_cast<const GuildBanRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    char name[25];
    std::memcpy(name, record->name, sizeof(record->name));
    name[24] = '\0';
    char reason[41];
    std::memcpy(reason, record->reason, sizeof(record->reason));
    reason[40] = '\0';

    GuildInfo::Unknown00ECEntry entry;
    entry.name = name;
    entry.unknown_0034 = reason;
    guildInfo->unknown_00EC.push_back(entry);
  }

  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4BDD0
// FUNCTION: fn_21867
void CGameMode::ReceiveGuildBanList2(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(GuildBanCharacterRecord);
  guildInfo->unknown_00EC.clear();

  const GuildBanCharacterRecord* record = reinterpret_cast<const GuildBanCharacterRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    char reason[41];
    std::memcpy(reason, record->reason, sizeof(record->reason));
    reason[40] = '\0';

    GuildInfo::Unknown00ECEntry entry;
    entry.characterId = record->characterId;
    entry.name = LookupPlayerName(entry.characterId);
    entry.unknown_0034 = reason;
    if (entry.name.size() == 0) {
      entry.name = LookupMessage(0x245) + g_gameModeNamelessTag;
    }
    if (entry.characterId != 0) {
      guildInfo->unknown_00EC.push_back(entry);
    }
  }

  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4C3D0
// FUNCTION: fn_21868
void CGameMode::ReceiveGuildBanList3(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const std::int64_t count = (packet->length - static_cast<std::int64_t>(sizeof(PacketLengthHeader))) / static_cast<std::int64_t>(sizeof(GuildBanNamedRecord));
  guildInfo->unknown_00EC.clear();

  const GuildBanNamedRecord* record = reinterpret_cast<const GuildBanNamedRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    char reason[41];
    std::memcpy(reason, record->reason, sizeof(record->reason));
    reason[40] = '\0';

    GuildInfo::Unknown00ECEntry entry;
    entry.characterId = record->characterId;
    entry.unknown_0034 = reason;
    if (!g_playerNameDictionary->Contains(record->characterId)) {
      const int characterId = record->characterId;
      std::string name(record->name, record->name + sizeof(record->name));
      SetPlayerName(characterId, name.c_str());
    }
    entry.name = LookupPlayerName(entry.characterId);
    if (entry.name.size() == 0) {
      entry.name = LookupMessage(0x245) + g_gameModeNamelessTag;
    }
    if (entry.characterId != 0) {
      guildInfo->unknown_00EC.push_back(entry);
    }
  }

  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4D150
// FUNCTION: fn_21870
void CGameMode::ReceiveGuildChat(const unsigned char* data) {
  const int length = *reinterpret_cast<const std::uint16_t*>(data + 2) - 4;
  if (length <= 0 || length > 361) {
    return;
  }

  std::vector<char> text(length + 1, 0);
  memcpy_s(text.data(), text.size(), data + 4, length);
  if (g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked;
    g_forbiddenWords.Mask(text.data(), &masked);
  }
  if (!g_serviceOptions.Get(0x94)) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0xB4FFB4, 4, 0, 0);
    if (g_whisperRecord != 0) {
      this->WriteChatLog(text.data());
    }
  }
}

// OFFSET: 0x00D33500
// FUNCTION: fn_21749
void CGameMode::ReceiveActorAction(
  std::uint32_t sourceAid, std::uint32_t targetAid, std::uint32_t tick, int attackMT, int attackedMT, int damage, std::int16_t count, unsigned char type,
  int damage2, unsigned char spDamaged
) {
  if (damage == -30000) {
    return;
  }

  ACTSTRUCT act;
  act.damage = damage;
  act.damage2 = damage2;
  act.type = type;
  act.targetId = targetAid;
  act.tick = tick;
  act.count = count;
  act.unknown_001C = -1;
  act.unknown_0020 = -1;
  act.attackMT = attackMT;
  act.attackedMT = attackedMT;
  act.spDamaged = spDamaged != 0;
  if (attackMT == 0.0f) {
    act.attackMT = 0x5A0;
  }
  if (attackedMT == 0.0f && !g_session.IsType0Or4(act.type)) {
    act.attackedMT = 0x5A0;
  }

  CRenderObject* actor;
  if (sourceAid == g_session.m_aid) {
    actor = this->scene->player;
  } else {
    actor = this->scene->GetGameActorByAID(sourceAid);
  }
  if (!actor) {
    actor = this->scene->GetSkillByAID(sourceAid);
    if (!actor) {
      return;
    }
  }

  if (actor->render.unknown_0070 == 0 && static_cast<CGameActor*>(actor)->hitTargetAid != targetAid && act.type != 1 && act.type != 2 && act.type != 3) {
    if (sourceAid == g_session.m_aid) {
      if (this->scene->GetGameActorByAID(g_session.unknown_1010)) {
        const int hungerLevel = g_session.GetPetHungryState(g_session.unknown_1044);
        const int action = g_session.GetPetActionNumber(hungerLevel, g_session.GetPetFriendlyState(g_session.m_petFriendly), 1);
        if (rand() % 100 < 25 && g_session.CheckInterval(60000) == true) {
          this->SendMsg(148, g_session.GetPetEmotionNumber(g_session.unknown_103C, 1, hungerLevel), 0, 0, 0);
          this->SendMsg(148, action, 0, 0, 0);
        }
      }
    } else {
      char text[256];
      g_session.GetMonsterTalkText(static_cast<CGameActor*>(actor)->GetEffectiveJob(-1), 0, 0, text, sourceAid);
      if (std::strcmp(text, "") != 0) {
        actor->SendMsg(nullptr, 7, 0, reinterpret_cast<int>(text), reinterpret_cast<int>(text) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if (!g_serviceOptions.Get(114) && !g_serviceOptions.Get(148)) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFAFAFA, 0, 0, 0);
        }
      }
    }
  }
  actor->SendMsg(nullptr, 11, 0, reinterpret_cast<int>(&act), reinterpret_cast<int>(&act) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

#pragma pack(push, 1)
struct ActorDisappearPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ unsigned char type;
};
#pragma pack(pop)

static_assert(sizeof(ActorDisappearPacket) == 7);

// OFFSET: 0x00D39530
// FUNCTION: fn_21771
void CGameMode::ReceiveActorDisappear(const unsigned char* data) {
  const ActorDisappearPacket* packet = reinterpret_cast<const ActorDisappearPacket*>(data);
  this->nameCard.Erase(packet->aid);
  g_session.EraseUnknownKey(packet->aid);
  if (g_session.unknown_5BC0 == packet->aid) {
    g_session.SetUnknown5BC0(-1);
  }

  switch (packet->type) {
  case 0:
    if (CRenderObject* actor = this->scene->GetGameActorByAID(packet->aid)) {
      actor->SendMsg(nullptr, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->SendMsg(13, packet->aid, 0, 0, 0);
    }
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(13, packet->aid, 0, 0, 0);
    }
    if (g_windowMgr.window_02C8) {
      g_windowMgr.window_02C8->SendMsg(nullptr, 6, 34, packet->aid, 0, 0);
    }
    break;
  case 1: {
    if (CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid))) {
      CPlayer* player = this->scene->player;
      if (packet->aid == static_cast<std::uint32_t>(player->unknown_051C)) {
        player->unknown_051C = 0;
      }
      actor->SendMsg(nullptr, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      const int job = actor->GetEffectiveJob(-1);
      if (g_session.IsMonsterClass(job) || actor->unknown_0314 == 9 || g_session.IsSkillIdInRange6001(job) ||
          actor->GetEffectiveJob(-1) == 0x235) {
        this->SendMsg(13, packet->aid, 0, 0, 0);
      }
      if (g_windowMgr.unknown_041C) {
        g_windowMgr.unknown_041C->SendMsg(nullptr, 6, 185, 0, 0, 0);
      }
      if (g_windowMgr.unknown_0308) {
        g_windowMgr.unknown_0308->SendMsg(nullptr, 6, 185, 0, 0, 0);
      }
    }
    if (packet->aid != g_session.m_aid) {
      break;
    }

    this->scene->player->SendMsg(nullptr, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->unknown_024C = 1;
    this->unknown_0250 = 1;
    g_windowMgr.CloseWindow(0x30);
    g_windowMgr.CloseWindow(0x10);
    g_windowMgr.CloseWindow(0x11);
    g_windowMgr.CloseWindow(0x19);
    g_windowMgr.CloseWindow(0x16);
    g_windowMgr.CloseWindow(0x17);
    g_windowMgr.CloseWindow(0x18);
    g_windowMgr.CloseWindow(0x169);
    if (g_windowMgr.unknown_02FC) {
      g_windowMgr.unknown_02FC->SendMsg(nullptr, 6, 185, 0, 0, 0);
    }
    if (g_windowMgr.unknown_040C) {
      g_windowMgr.unknown_040C->SendMsg(nullptr, 6, 185, 0, 0, 0);
    }
    if (g_windowMgr.unknown_0304) {
      g_windowMgr.unknown_0304->SendMsg(nullptr, 6, 201, 0, 0, 0);
    }
    if (g_windowMgr.unknown_0414) {
      g_windowMgr.unknown_0414->SendMsg(nullptr, 6, 201, 0, 0, 0);
    }
    if (g_windowMgr.unknown_0308) {
      g_windowMgr.unknown_0308->SendMsg(nullptr, 6, 185, 0, 0, 0);
    }
    if (g_windowMgr.unknown_041C) {
      g_windowMgr.unknown_041C->SendMsg(nullptr, 6, 185, 0, 0, 0);
    }
    if (this->scene->GetGameActorByAID(g_session.unknown_1010)) {
      const int hungerLevel = g_session.GetPetHungryState(g_session.unknown_1044);
      const int action = g_session.GetPetActionNumber(hungerLevel, g_session.GetPetFriendlyState(g_session.m_petFriendly), 3);
      this->SendMsg(148, g_session.GetPetEmotionNumber(g_session.unknown_103C, 3, hungerLevel), 0, 0, 0);
      this->SendMsg(148, action, 0, 0, 0);
    }
    if (g_serviceType == 4) {
      g_windowMgr.Confirm(LookupMessage(0x2B7), 0, true, false, 0, "Message", 280, 120, 0);
    } else if (!g_windowMgr.CloseWindow(0x9B)) {
      g_windowMgr.EnsureWindow(0x9B);
    }
    sub_DFF150(&g_session);
    break;
  }
  case 2:
    if (CRenderObject* actor = this->scene->GetGameActorByAID(packet->aid)) {
      this->LaunchDummyEffect(&actor->render.position, 0x13A, 600, 0);
    }
    if (g_windowMgr.unknown_0300 && static_cast<UIMerchantItemShopWnd*>(g_windowMgr.unknown_0300)->unknown_0100 == static_cast<int>(packet->aid)) {
      this->SendMsg(245, 0, 0, 0, 0);
      g_session.ClearItems1760And1748();
    }
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(13, packet->aid, 0, 0, 0);
    }
    break;
  case 3:
    if (CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid))) {
      if (!StateFlag06(actor->Slot13())) {
        this->LaunchDummyEffect(&actor->render.position, 0x130, 300, 0);
      }
    }
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(13, packet->aid, 0, 0, 0);
    }
    if (g_windowMgr.window_02C8) {
      g_windowMgr.window_02C8->SendMsg(nullptr, 6, 34, packet->aid, 0, 0);
    }
    break;
  default:
    this->scene->GetGameActorByAID(packet->aid);
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(13, packet->aid, 0, 0, 0);
    }
    break;
  }
}

#pragma pack(push, 1)
struct GuildInfoV2Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t guildId;
  /* 0006 */ int level;
  /* 000A */ int userCount;
  /* 000E */ int maxUserCount;
  /* 0012 */ int averageLevel;
  /* 0016 */ int exp;
  /* 001A */ int maxExp;
  /* 001E */ int point;
  /* 0022 */ int honor;
  /* 0026 */ int virtue;
  /* 002A */ int emblemVersion;
  /* 002E */ char guildName[24];
  /* 0046 */ char masterName[24];
  /* 005E */ char manageLand[24];
};

struct GuildInfoV3Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t guildId;
  /* 0006 */ int level;
  /* 000A */ int userCount;
  /* 000E */ int maxUserCount;
  /* 0012 */ int averageLevel;
  /* 0016 */ int exp;
  /* 001A */ int maxExp;
  /* 001E */ int point;
  /* 0022 */ int honor;
  /* 0026 */ int virtue;
  /* 002A */ int emblemVersion;
  /* 002E */ char guildName[24];
  /* 0046 */ char manageLand[20];
  /* 005A */ std::uint32_t masterCharacterId;
};

struct GuildInfoV4Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t guildId;
  /* 0006 */ int level;
  /* 000A */ int userCount;
  /* 000E */ int maxUserCount;
  /* 0012 */ int averageLevel;
  /* 0016 */ int exp;
  /* 001A */ int maxExp;
  /* 001E */ int point;
  /* 0022 */ int honor;
  /* 0026 */ int virtue;
  /* 002A */ int emblemVersion;
  /* 002E */ char guildName[24];
  /* 0046 */ char manageLand[20];
  /* 005A */ std::uint32_t masterCharacterId;
  /* 005E */ char masterName[24];
};
#pragma pack(pop)

static_assert(sizeof(GuildInfoV2Packet) == 0x76);
static_assert(sizeof(GuildInfoV3Packet) == 0x5E);
static_assert(sizeof(GuildInfoV4Packet) == 0x76);

// OFFSET: 0x00D4D3B0
// FUNCTION: fn_21871
void CGameMode::ReceiveGuildInfo2(const unsigned char* data) {
  const GuildInfoV2Packet* packet = reinterpret_cast<const GuildInfoV2Packet*>(data);
  GuildInfo* guildInfo = &g_guildInfo;

  char guildName[25];
  guildName[24] = '\0';
  char masterName[25];
  masterName[24] = '\0';
  char manageLand[25];
  manageLand[24] = '\0';
  std::memcpy(guildName, packet->guildName, sizeof(packet->guildName));
  std::memcpy(masterName, packet->masterName, sizeof(packet->masterName));
  std::memcpy(manageLand, packet->manageLand, sizeof(packet->manageLand));
  if (g_forbiddenWords.Contains(guildName)) {
    std::string masked;
    g_forbiddenWords.Mask(guildName, &masked);
  }

  guildInfo->unknown_0000 = guildName;
  g_guildInfo.unknown_0018 = masterName;
  g_guildInfo.unknown_00B4 = g_guildInfo.unknown_0018 == g_session.DecodePlayerName();
  guildInfo->unknown_0074 = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->unknown_0060 = packet->level;
  guildInfo->unknown_0064 = packet->userCount;
  guildInfo->unknown_006C = packet->maxUserCount;
  guildInfo->unknown_0070 = packet->averageLevel;
  guildInfo->unknown_008C = packet->exp;
  guildInfo->unknown_0090 = packet->maxExp;
  guildInfo->unknown_0098 = packet->point;
  guildInfo->unknown_009C = packet->honor;
  guildInfo->unknown_00A0 = packet->virtue;
  g_guildMgr->SetEmblemVersion(packet->guildId, packet->emblemVersion);
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4D600
// FUNCTION: fn_21872
void CGameMode::ReceiveGuildInfo3(const unsigned char* data) {
  const GuildInfoV3Packet* packet = reinterpret_cast<const GuildInfoV3Packet*>(data);
  GuildInfo* guildInfo = &g_guildInfo;

  std::string masterName = LookupPlayerName(packet->masterCharacterId);
  if (masterName.size() == 0) {
    masterName = LookupMessage(0x245) + g_gameModeNamelessTag;
  }

  char guildName[25];
  guildName[24] = '\0';
  char manageLand[25];
  manageLand[24] = '\0';
  std::memcpy(guildName, packet->guildName, sizeof(packet->guildName));
  std::memcpy(manageLand, packet->manageLand, sizeof(manageLand) - 1);
  if (g_forbiddenWords.Contains(guildName)) {
    std::string masked;
    g_forbiddenWords.Mask(guildName, &masked);
  }

  if (masterName == g_session.DecodePlayerName()) {
    g_guildInfo.unknown_00B4 = 1;
  }
  guildInfo->unknown_0000 = guildName;
  g_guildInfo.unknown_0018 = masterName;
  g_guildInfo.unknown_00D0 = packet->masterCharacterId;
  guildInfo->unknown_0074 = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->unknown_0060 = packet->level;
  guildInfo->unknown_0068 = packet->userCount;
  guildInfo->unknown_006C = packet->maxUserCount;
  guildInfo->unknown_0070 = packet->averageLevel;
  guildInfo->unknown_008C = packet->exp;
  guildInfo->unknown_0090 = packet->maxExp;
  guildInfo->unknown_0098 = packet->point;
  guildInfo->unknown_009C = packet->honor;
  guildInfo->unknown_00A0 = packet->virtue;
  g_guildMgr->SetEmblemVersion(packet->guildId, packet->emblemVersion);
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4D900
// FUNCTION: fn_21873
void CGameMode::ReceiveGuildInfo4(const unsigned char* data) {
  const GuildInfoV4Packet* packet = reinterpret_cast<const GuildInfoV4Packet*>(data);
  GuildInfo* guildInfo = &g_guildInfo;

  if (!g_playerNameDictionary->Contains(packet->masterCharacterId)) {
    const std::uint32_t characterId = packet->masterCharacterId;
    std::string name(packet->masterName, packet->masterName + sizeof(packet->masterName));
    SetPlayerName(characterId, name.c_str());
  }
  g_guildInfo.unknown_0018 = LookupPlayerName(packet->masterCharacterId);
  if (g_guildInfo.unknown_0018.size() == 0) {
    g_guildInfo.unknown_0018 = LookupMessage(0x245) + g_gameModeNamelessTag;
  }

  char guildName[25];
  guildName[24] = '\0';
  char manageLand[25];
  manageLand[24] = '\0';
  if (g_guildInfo.unknown_0018 == g_session.DecodePlayerName()) {
    g_guildInfo.unknown_00B4 = 1;
  }
  std::memcpy(guildName, packet->guildName, sizeof(packet->guildName));
  std::memcpy(manageLand, packet->manageLand, sizeof(manageLand) - 1);
  if (g_forbiddenWords.Contains(guildName)) {
    std::string masked;
    g_forbiddenWords.Mask(guildName, &masked);
  }

  guildInfo->unknown_0000 = guildName;
  g_guildInfo.unknown_00D0 = packet->masterCharacterId;
  guildInfo->unknown_0074 = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->unknown_0060 = packet->level;
  guildInfo->unknown_0068 = packet->userCount;
  guildInfo->unknown_006C = packet->maxUserCount;
  guildInfo->unknown_0070 = packet->averageLevel;
  guildInfo->unknown_008C = packet->exp;
  guildInfo->unknown_0090 = packet->maxExp;
  guildInfo->unknown_0098 = packet->point;
  guildInfo->unknown_009C = packet->honor;
  guildInfo->unknown_00A0 = packet->virtue;
  g_guildMgr->SetEmblemVersion(packet->guildId, packet->emblemVersion);
  g_windowMgr.RefreshGuildWindows();
}

#pragma pack(push, 1)
struct GuildNoticePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char subject[60];
  /* 003E */ char notice[120];
};
#pragma pack(pop)

static_assert(sizeof(GuildNoticePacket) == 0xB6);

// OFFSET: 0x00D4DD70
// FUNCTION: fn_21874
void CGameMode::ReceiveGuildNotice(unsigned char* data) {
  GuildNoticePacket* packet = reinterpret_cast<GuildNoticePacket*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  packet->notice[119] = '\0';
  packet->subject[59] = '\0';
  guildInfo->unknown_0030 = packet->subject;
  guildInfo->unknown_0048 = packet->notice;
  if (!g_serviceOptions.Get(0xA5)) {
    return;
  }

  char subject[60];
  std::memset(subject, 0, sizeof(subject));
  strcpy_s(subject, sizeof(subject), packet->subject);
  if (g_forbiddenWords.Contains(subject) == true) {
    std::string masked;
    g_forbiddenWords.Mask(subject, &masked);
  }
  std::string subjectLine = "[ ";
  subjectLine += subject;
  subjectLine += " ]";
  if (std::strlen(subject) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(subjectLine.c_str()), 0x80FFFF, 0, 0, 0);
  }

  char notice[120];
  std::memset(notice, 0, sizeof(notice));
  strcpy_s(notice, sizeof(notice), packet->notice);
  if (g_forbiddenWords.Contains(notice) == true) {
    std::string masked;
    g_forbiddenWords.Mask(notice, &masked);
  }
  std::string noticeLine = "[ ";
  noticeLine += notice;
  noticeLine += " ]";
  if (std::strlen(notice) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(noticeLine.c_str()), 0x80FFFF, 0, 0, 0);
  }

  guildInfo->unknown_0030 = subject;
  guildInfo->unknown_0048 = notice;
}

#pragma pack(push, 1)
struct GuildMemberAddV2Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ int accountId;
  /* 0006 */ std::uint32_t characterId;
  /* 000A */ std::int16_t unknown_000A;
  /* 000C */ std::int16_t unknown_000C;
  /* 000E */ std::int16_t unknown_000E;
  /* 0010 */ std::int16_t job;
  /* 0012 */ std::int16_t level;
  /* 0014 */ int exp;
  /* 0018 */ int online;
  /* 001C */ int positionId;
  /* 0020 */ int unknown_0020;
  /* 0024 */ char name[24];
};

struct GuildMemberAddV3Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ int accountId;
  /* 0006 */ std::uint32_t characterId;
  /* 000A */ std::int16_t unknown_000A;
  /* 000C */ std::int16_t unknown_000C;
  /* 000E */ std::int16_t unknown_000E;
  /* 0010 */ std::int16_t job;
  /* 0012 */ std::int16_t level;
  /* 0014 */ int exp;
  /* 0018 */ int online;
  /* 001C */ int positionId;
  /* 0020 */ char memo[50];
};
#pragma pack(pop)

static_assert(sizeof(GuildMemberAddV2Packet) == 0x3C);
static_assert(sizeof(GuildMemberAddV3Packet) == 0x52);

// OFFSET: 0x00D4E800
// FUNCTION: fn_21878
void CGameMode::ReceiveGuildMemberAdd2(const unsigned char* data) {
  const GuildMemberAddV2Packet* packet = reinterpret_cast<const GuildMemberAddV2Packet*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->unknown_00C0 == 0) {
    return;
  }

  GuildInfo::Member member;
  member.accountId = packet->accountId;
  member.characterId = packet->characterId;
  member.unknown_0020 = packet->unknown_000A;
  member.unknown_0024 = packet->unknown_000C;
  member.job = packet->job;
  member.unknown_002C = packet->unknown_000E;
  member.positionId = packet->positionId;
  member.level = packet->level;
  member.exp = packet->exp;
  member.online = packet->online;
  *reinterpret_cast<int*>(member.unknown_0074) = packet->unknown_0020;
  if (!g_playerNameDictionary->Contains(packet->characterId)) {
    const std::uint32_t characterId = packet->characterId;
    std::string name(packet->name, packet->name + sizeof(packet->name));
    SetPlayerName(characterId, name.c_str());
  }
  member.name = LookupPlayerName(packet->characterId);
  if (member.name.size() == 0) {
    member.name = LookupMessage(0x245) + g_gameModeNamelessTag;
  }

  guildInfo->AddMember(member);
  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4EBA0
// FUNCTION: fn_21879
void CGameMode::ReceiveGuildMemberAdd3(const unsigned char* data) {
  const GuildMemberAddV3Packet* packet = reinterpret_cast<const GuildMemberAddV3Packet*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->unknown_00C0 == 0) {
    return;
  }

  char memo[51];
  std::memcpy(memo, packet->memo, sizeof(packet->memo));
  memo[50] = '\0';

  GuildInfo::Member member;
  member.accountId = packet->accountId;
  member.characterId = packet->characterId;
  member.name = LookupPlayerName(member.characterId);
  member.unknown_0020 = packet->unknown_000A;
  member.unknown_0024 = packet->unknown_000C;
  member.job = packet->job;
  member.unknown_002C = packet->unknown_000E;
  member.positionId = packet->positionId;
  member.level = packet->level;
  member.unknown_0050 = memo;
  member.exp = packet->exp;
  member.online = packet->online;
  if (member.name.size() == 0) {
    member.name = LookupMessage(0x245) + g_gameModeNamelessTag;
  }

  guildInfo->AddMember(member);
  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

namespace {
#pragma pack(push, 1)
  struct GuildMemberNamedRecord {
    /* 0000 */ int accountId;
    /* 0004 */ std::uint32_t characterId;
    /* 0008 */ std::int16_t unknown_0008;
    /* 000A */ std::int16_t unknown_000A;
    /* 000C */ std::int16_t unknown_000C;
    /* 000E */ std::int16_t job;
    /* 0010 */ std::int16_t level;
    /* 0012 */ int exp;
    /* 0016 */ int online;
    /* 001A */ int positionId;
    /* 001E */ int unknown_001E;
    /* 0022 */ char name[24];
  };

  struct GuildMemberMemoOnlyRecord {
    /* 0000 */ int accountId;
    /* 0004 */ std::uint32_t characterId;
    /* 0008 */ std::int16_t unknown_0008;
    /* 000A */ std::int16_t unknown_000A;
    /* 000C */ std::int16_t unknown_000C;
    /* 000E */ std::int16_t job;
    /* 0010 */ std::int16_t level;
    /* 0012 */ int exp;
    /* 0016 */ int online;
    /* 001A */ int positionId;
    /* 001E */ char memo[50];
  };

  struct GuildRelationRecord {
    /* 0000 */ int relation;
    /* 0004 */ int guildId;
    /* 0008 */ char name[24];
  };
#pragma pack(pop)

  static_assert(sizeof(GuildMemberNamedRecord) == 0x3A);
  static_assert(sizeof(GuildMemberMemoOnlyRecord) == 0x50);
  static_assert(sizeof(GuildRelationRecord) == 0x20);
}

// OFFSET: 0x00D4F180
// FUNCTION: fn_21881
void CGameMode::ReceiveGuildMemberList2(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const std::int64_t count =
    (packet->length - static_cast<std::int64_t>(sizeof(PacketLengthHeader))) / static_cast<std::int64_t>(sizeof(GuildMemberNamedRecord));
  guildInfo->unknown_0064 = static_cast<int>(count);
  guildInfo->ClearMembers();

  const GuildMemberNamedRecord* record = reinterpret_cast<const GuildMemberNamedRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.characterId = record->characterId;
    member.unknown_0020 = record->unknown_0008;
    member.unknown_0024 = record->unknown_000A;
    member.job = record->job;
    member.unknown_002C = record->unknown_000C;
    member.positionId = record->positionId;
    member.level = record->level;
    member.exp = record->exp;
    member.online = record->online;
    *reinterpret_cast<int*>(member.unknown_0074) = record->unknown_001E;
    member.expRatio = 0.0f;
    if (member.characterId == g_session.m_gid) {
      member.name = g_session.DecodePlayerName();
    } else {
      if (!g_playerNameDictionary->Contains(member.characterId)) {
        const std::uint32_t characterId = record->characterId;
        std::string name(record->name, record->name + sizeof(record->name));
        SetPlayerName(characterId, name.c_str());
      }
      member.name = LookupPlayerName(record->characterId);
      if (member.name.size() == 0) {
        member.name = LookupMessage(0x245) + g_gameModeNamelessTag;
      }
    }

    member.positionName.clear();
    guildInfo->AddMember(member);
  }

  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4F7A0
// FUNCTION: fn_21882
void CGameMode::ReceiveGuildMemberList3(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(GuildMemberMemoOnlyRecord);
  guildInfo->unknown_0064 = count;
  guildInfo->ClearMembers();

  const GuildMemberMemoOnlyRecord* record = reinterpret_cast<const GuildMemberMemoOnlyRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.characterId = record->characterId;
    member.unknown_0020 = record->unknown_0008;
    member.unknown_0024 = record->unknown_000A;
    member.job = record->job;
    member.unknown_002C = record->unknown_000C;
    member.positionId = record->positionId;
    member.level = record->level;
    member.exp = record->exp;
    member.online = record->online;
    member.expRatio = 0.0f;
    if (member.characterId == g_session.m_gid) {
      member.name = g_session.DecodePlayerName();
    } else {
      member.name = LookupPlayerName(member.characterId);
      if (member.name.size() == 0) {
        member.name = LookupMessage(0x245) + g_gameModeNamelessTag;
      }
    }

    char memo[51];
    std::memcpy(memo, record->memo, sizeof(record->memo));
    memo[50] = '\0';
    member.unknown_0050 = memo;
    member.positionName.clear();
    guildInfo->AddMember(member);
  }

  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4FD50
// FUNCTION: fn_21883
void CGameMode::ReceiveGuildRelationList(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(GuildRelationRecord);
  guildInfo->unknown_00E4.clear();

  const GuildRelationRecord* record = reinterpret_cast<const GuildRelationRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    char name[25];
    std::memcpy(name, record->name, sizeof(record->name));
    name[24] = '\0';
    if (g_forbiddenWords.Contains(name) == true) {
      std::string masked;
      g_forbiddenWords.Mask(name, &masked);
    }

    GuildInfo::RelatedGuild guild;
    guild.unknown_0000 = record->guildId;
    guild.unknown_0004 = record->relation;
    guild.unknown_0008 = name;
    guildInfo->unknown_00E4.push_back(guild);
  }

  g_windowMgr.RefreshGuildWindows();
}

#pragma pack(push, 1)
struct EquipItemNoticePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t index;
  /* 0004 */ std::int16_t value;
  /* 0006 */ unsigned char result;
};
#pragma pack(pop)

static_assert(sizeof(EquipItemNoticePacket) == 7);

#pragma pack(push, 1)
struct MapMovePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char mapName[16];
  /* 0012 */ std::int16_t x;
  /* 0014 */ std::int16_t y;
};

struct GuildPositionNameEntry {
  /* 0000 */ int positionId;
  /* 0004 */ char name[24];
};

struct GuildPositionNamePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t length;
  /* 0004 */ GuildPositionNameEntry entries[1];
};
#pragma pack(pop)

static_assert(sizeof(MapMovePacket) == 0x16);
static_assert(sizeof(GuildPositionNameEntry) == 0x1C);

struct GuildStorageLogItem {
  /* 0000 */ ITEM_INFO item;
  /* 00F8 */ unsigned char unknown_00F8;
};

static_assert(sizeof(GuildStorageLogItem) == 0x100);

// OFFSET: 0x00D3A1E0
// FUNCTION: fn_21776
void CGameMode::ReceiveMapMove(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const MapMovePacket*>(data);
  g_session.m_playerX = packet->x;
  g_session.m_playerY = packet->y;

  char name[17];
  std::memcpy(name, packet->mapName, sizeof(packet->mapName));
  name[16] = '\0';
  std::string mapName(name);
  mapName.resize(mapName.size() - 4);

  if (Replay::Instance().state == 2) {
    Replay::Instance().SetState(3);
    g_windowMgr.window_4F164->SendMsg(nullptr, 0x57, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(0xC6);
  } else if (Replay::Instance().state == 7) {
    Replay::Instance().SetState(8);
  }

  unsigned char sameMap;
  if (std::strcmp(mapName.c_str(), g_session.m_navigationPlayerMap) == 0) {
    this->ReloadScene();
    this->InitializeState();
    FadeOutMode(*g_renderer, *GetRenderSystemHolder()->system, ResourceMgr::Instance(), 0, false, 0.0f, 0.0f);
    sameMap = 1;
  } else {
    g_session.SetNavigationPlayerMap(mapName.c_str());
    g_session.m_loginTimeTicks = _Xtime_get_ticks();
    mapName.append(".rsw", 4);
    g_modeMgr.Switch(1, mapName.c_str());
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(6))->SetMessage(LookupMessage(121), 17, 1);
    g_windowMgr.unknown_0474 = 185;
    g_session.ClearEntryList5BD4();
    sameMap = 0;
  }
  this->unknown_060C[0] = sameMap;

  for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end(); ++it) {
    if (it->id == 200 || it->id == 201 || it->id == 4) {
      g_activeSkillEffects.erase(it);
      break;
    }
  }

  g_session.RestoreItems5C68();
  g_session.ClearItemQuantities();
}

// OFFSET: 0x00D50E40
// FUNCTION: fn_21888
void CGameMode::ReceiveGuildPositionNames(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const GuildPositionNamePacket*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  const int count = (packet->length - 4) / sizeof(GuildPositionNameEntry);
  for (int i = 0; i < count; ++i) {
    const GuildPositionNameEntry& position = packet->entries[i];
    char name[25];
    std::memcpy(name, position.name, sizeof(position.name));
    name[24] = '\0';
    if (g_forbiddenWords.Contains(name)) {
      std::string masked;
      g_forbiddenWords.Mask(name, &masked);
    }

    guildInfo->m_positions[position.positionId] = name;
  }

  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D5A4C0
// FUNCTION: fn_21961
void CGameMode::ApplyGuildStorageLog(std::uint16_t result, const std::vector<GuildStorageLogItem>& items) {
  for (std::vector<GuildStorageLogItem>::const_iterator it = items.begin(); it != items.end(); ++it) {
    GuildStorageLogEntry entry;
    entry.unknown_0000 = it->item.unknown_00B8[0];
    entry.unknown_0004 = it->item.unknown_00B8[1];
    entry.unknown_0008 = it->item.unknown_00C0;
    strncpy_s(entry.unknown_0009, sizeof(entry.unknown_0009), reinterpret_cast<const char*>(it->item.unknown_00C1), _TRUNCATE);
    strncpy_s(entry.unknown_0021, sizeof(entry.unknown_0021), reinterpret_cast<const char*>(it->item.unknown_00D9), _TRUNCATE);
    entry.unknown_003C = it->item.DisplayName(0);
    entry.unknown_0054 = it->item.identified;
    entry.unknown_0058 = it->item.unknown_0000[0];
    entry.unknown_005C = it->item.unknown_0060;
    std::memcpy(entry.unknown_0060, it->item.unknown_001C, sizeof(entry.unknown_0060));
    entry.unknown_0070 = it->unknown_00F8;
    entry.unknown_0072 = it->item.unknown_0088;
    g_guildStorageLogs[0].push_back(entry);
  }

  if (result == 0) {
    return;
  }

  if (result == 1) {
    GuildStorageLogEntry pivot;
    std::sort(g_guildStorageLogs[0].begin(), g_guildStorageLogs[0].end(), [&pivot](const GuildStorageLogEntry& left, const GuildStorageLogEntry& right) {
      return std::strcmp(left.unknown_0021, right.unknown_0021) > 0;
    });
    for (std::vector<GuildStorageLogEntry>::iterator it = g_guildStorageLogs[0].begin(); it != g_guildStorageLogs[0].end(); ++it) {
      if (it->unknown_0008 == 1) {
        g_guildStorageLogs[1].push_back(*it);
      } else {
        g_guildStorageLogs[2].push_back(*it);
      }
    }
    g_windowMgr.EnsureWindow(253);
    return;
  }

  char text[250];
  int messageId;
  if (result == 2) {
    std::memset(text, 0, sizeof(text));
    messageId = 2543;
  } else {
    std::memset(text, 0, sizeof(text));
    messageId = 1816;
  }
  sprintf_s(text, sizeof(text), LookupMessage(messageId));
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
  g_guildStorageLogs[0].clear();
  g_guildStorageLogs[1].clear();
  g_guildStorageLogs[2].clear();
}

// OFFSET: 0x00D5E930
// FUNCTION: fn_21967
void LaunchSkillCastEffect(CGameMode* mode, CGameActor* source, CGameActor* target, std::uint32_t targetAid, std::uint16_t skillId, short x, short y, int delay) {
  CSkillActionMgr::GetInstance()->ActionSlot10(skillId, delay, target, x, y);

  if (targetAid == 0) {
    float worldX = 0.0f;
    float worldZ = 0.0f;
    const int column = x;
    const int row = y;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(column, row, worldX, worldZ);
    source->Face(worldX, worldZ);

    CGroundScope* scope = new (std::nothrow) CGroundScope;
    if (!scope) {
      return;
    }

    scope->SendMsg(nullptr, 0x16, 0, source->render.accountId, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    scope->SendMsg(nullptr, 0xE, 0, column, column >> 31, row, row >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    scope->SendMsg(nullptr, 0x73, 0, timeGetTime() + delay, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int id = skillId;
    scope->SendMsg(nullptr, 0x75, 0, id, id >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    mode->scene->objects.push_back(scope);
    return;
  }

  if (!target || source == target) {
    return;
  }

  if (g_session.IsSkillIdInRange6018(target->GetEffectiveJob(-1)) || target->unknown_0314 == 9) {
    if (mode->scene->unknown_0048 != 0 || mode->scene->unknown_004C != 0 || !IsSupportSkill(skillId)) {
      source->SendMsg(nullptr, 0x51, 0, target->render.accountId, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  source->hitTargetAid = targetAid;
  source->Face(target->render.position.x, target->render.position.z);

  CEffect* effect;
  if (skillId == 0x14A4) {
    effect = g_effectMgr->Spawn(target->render.accountId, 0x78A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  }

  const int delayFrames = delay / 16;
  target->SendMsg(nullptr, 0x56, 0, 0x3C, 0, delayFrames, delayFrames >> 31, 0, 0, 0, 0, 0, 0, 0, 0);

  switch (skillId) {
  case 0x1EA:
    mode->LaunchTargetEffect(0x21B, source, target, -1);
    return;
  case 0x89A: {
    const float speed = target->render.speed;
    effect = g_effectMgr->Spawn(target->render.accountId, 0x635, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    const int duration = static_cast<int>(speed * 150.0f + 200.0f) + delay;
    if (effect) {
      effect->Slot14(duration);
    }
    effect = g_effectMgr->Spawn(target->render.accountId, 0x636, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(duration);
    }
    return;
  }
  case 0x89C:
    target->SendMsg(nullptr, 0x57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    target->SendMsg(nullptr, 0x56, 0, 0x5A8, 0, delayFrames, delayFrames >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    return;
  case 0x8A6:
    target->SendMsg(nullptr, 0x57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, 0x665, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    return;
  case 0x8BC:
    target->SendMsg(nullptr, 0x57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    effect = g_effectMgr->Spawn(target->render.accountId, 0x673, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x8D3:
    if (CRagEffect* ragEffect = source->LaunchEffect(0x313, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
      vector3d position = target->render.position;
      ragEffect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    return;
  default:
    return;
  }
}

// OFFSET: 0x00D50AA0
// FUNCTION: fn_21887
void CGameMode::ReceiveEquipItemNotice(const unsigned char* data) {
  const EquipItemNoticePacket* packet = reinterpret_cast<const EquipItemNoticePacket*>(data);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  switch (packet->result) {
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x69F)), 0x6464FF, 0, 0, 0);
    return;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x705)), 0xFF, 0, 0, 0);
    return;
  case 15:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xE75)), 0xFF, 0, 0, 0);
    return;
  }

  ITEM_INFO item = g_session.FindItem1770(packet->index);
  if (item.unknown_0000[1] > 0) {
    switch (packet->result) {
    case 1:
      g_windowMgr.SendNotice(
        1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xE5), item.DisplayName(0).c_str(), LookupMessage(0x37)).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 2:
      g_windowMgr.SendNotice(
        1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xE5), item.DisplayName(0).c_str(), LookupMessage(0x38)).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 4:
      g_windowMgr.SendNotice(
        1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xF0), item.DisplayName(0).c_str(), packet->value).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 5:
      g_windowMgr.SendNotice(
        1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0x119), item.DisplayName(0).c_str(), packet->value).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 8:
      g_windowMgr.SendNotice(
        1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xE5), item.DisplayName(0).c_str(), LookupMessage(0x756)).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    }
  }
}
// OFFSET: 0x00D5B3C0
// FUNCTION: fn_21964
void NoticeSkillUse(CGameActor* target, CGameActor* source, std::uint16_t skillId, unsigned char silent) {
  if (silent == 1) {
    return;
  }

  const bool partyMember = g_session.IsPartyMember(target->render.accountId, true) && g_session.m_aid != target->render.accountId;
  if (g_session.m_aid != target->render.accountId && !partyMember) {
    return;
  }

  int color = 16;
  char text[256];
  std::memset(text, 0, sizeof(text));
  std::string message;
  if (partyMember) {
    color = 17;
    std::string name = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(target->render.accountId);
    name = LookupMessage(1604);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(1612), name.c_str());
    message += text;
  }

  if (source) {
    std::string name = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(source->render.accountId);
    const std::size_t mark = name.find('#');
    if (mark != std::string::npos) {
      name.resize(mark);
    }
    if (name.empty()) {
      const CActorNameCard& card = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LookupNameCard(source->render.accountId);
      name = card.unknown_007C.empty() ? std::string(LookupMessage(1604)) : card.unknown_007C;
    }
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(1624), name.c_str());
    message += text;
  }

  const std::string skillName = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, skillId);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(1625), skillName.c_str());
  message += text;
  if (!message.empty()) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF, color, 0, 0);
  }
}

// OFFSET: 0x00D5E680
// FUNCTION: fn_21966
void ApplyActorSkillState(CGameActor* actor, std::uint16_t skillId, int duration) {
  switch (skillId) {
  case 0x39:
  case 0x3E:
  case 0x154B:
    reinterpret_cast<unsigned char*>(&actor->render.unknown_008C)[2] = 1;
    actor->unknown_0278 = timeGetTime() + duration;
    break;
  case 0x105:
  case 0x3F7:
    actor->SetFrameSequence(96, 0, 0, 96, 0, 0, 0, 0);
    break;
  case 0x10C:
    actor->SetFrameSequence(96, 0, 0, 96, 0, 1, duration + 1000, 0);
    break;
  case 0x191:
    actor->SetFrameSequence(96, 0, 0, 96, 0, 0, 0, 0);
    actor->LaunchEffect(0x192, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8A9:
    actor->SendMsg(nullptr, 0x8F, 0, 0x2DF, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, 0x8F, 0, 0x2E1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, 0x8F, 0, 0x2E0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, 0x8F, 0, 0x2E2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  case 0x14C4:
    actor->SetFrameSequence(88, 0, 0, 88, 0, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D5B030
// FUNCTION: fn_21963
void CGameMode::sub_D5B030(
  std::uint32_t sourceAid, std::uint32_t targetAid, std::uint16_t x, std::uint16_t y, std::uint16_t skillId, int property, int delay, unsigned char silent, int
) {
  CGameActor* source = static_cast<CGameActor*>(this->scene->GetGameActorByAidIncludeMe(sourceAid));
  if (!source) {
    return;
  }

  CGameActor* target = static_cast<CGameActor*>(this->scene->GetGameActorByAidIncludeMe(targetAid));
  NoticeSkillUse(source, target, skillId, silent);

  if (delay == 0 && skillId == 0xA04) {
    CEffect* effect = g_effectMgr->Find(source->render.accountId, 0x474);
    if (effect) {
      effect->Slot01();
    } else {
      g_effectMgr->Spawn(source->render.accountId, 0x474, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
  }

  if (silent == 0 && skillId != 0x3E && skillId != 0x39 && !g_session.IsMonsterActor(source)) {
    if (skillId == 0x14E || skillId == 0x14F) {
      g_modeMgr.GetGameMode()->SendMsg(0x56, skillId, reinterpret_cast<std::intptr_t>(source), targetAid, 0);
    } else {
      g_modeMgr.GetGameMode()->SendMsg(0x56, skillId, reinterpret_cast<std::intptr_t>(source), 0, 0);
    }
  }

  if (skillId != 0x10F) {
    if (skillId == 0x49) {
      if (target && source != target) {
        source->Face(target->render.position.x, target->render.position.z);
      }
      g_modeMgr.GetGameMode()->SendMsg(0x56, 0x49, reinterpret_cast<std::intptr_t>(source), 0, 0);
    } else if (skillId == 0x1479) {
      g_effectMgr->Spawn(source->render.accountId, 0x7C4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }

    CSkillActionMgr::GetInstance()->ActionSlot09(skillId, reinterpret_cast<int>(source), reinterpret_cast<int>(target), delay);
    if (delay == 0) {
      return;
    }
  }

  int effectId = 0;
  int motion = 0;
  g_session.GetSkillActionInfo(skillId, effectId, motion, property, source->GetEffectiveJob(-1));
  source->Slot15(motion, 1);

  if (skillId != 0x3E && skillId != 0x39 && delay != 0) {
    source->SendMsg(nullptr, 0x52, 0, delay, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  LaunchSkillActorEffect(source, x, y, skillId, delay, effectId);

  if (skillId != 0x3E && skillId != 0x39 && g_session.IsMonsterActor(source)) {
    int action = 0;
    int loopType = 0;
    g_session.GetMonsterSkillInfo(g_session.GetJobName(source->GetEffectiveJob(-1), -1), skillId, &action, &loopType);
    if (action != -1) {
      source->SetAction(action * 8 - 8, 1, loopType == 0);
      if (skillId == 0xC1) {
        source->SetAnimationSpeed(static_cast<float>(static_cast<unsigned>(delay) / 300));
      }
    }
  }

  ApplyActorSkillState(source, skillId, delay);
  LaunchSkillCastEffect(this, source, target, targetAid, skillId, x, y, delay);
}

// OFFSET: 0x00D79FD0
// FUNCTION: fn_22185
void SetActiveSkillEffect184(unsigned char enable) {
  if (enable == 1) {
    ActiveSkillEffect effect;
    effect.id = 184;
    effect.unknown_0004 = 999999;
    effect.unknown_0008 = 0;
    effect.unknown_0014 = 9999;
    effect.unknown_0018 = 9999;
    g_activeSkillEffects.push_back(effect);
    return;
  }

  std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin();
  for (; it != g_activeSkillEffects.end(); ++it) {
    if (it->id == 184) {
      break;
    }
  }
  if (it != g_activeSkillEffects.end()) {
    g_activeSkillEffects.erase(it);
  }
}

// OFFSET: 0x00D316D0
// FUNCTION: fn_21734
void CGameMode::ReceiveExpChange(const unsigned char* packet) {
  std::int64_t& exp = *reinterpret_cast<std::int64_t*>(&g_session.unknown_162C[0x04]);
  std::int64_t& nextExp = *reinterpret_cast<std::int64_t*>(&g_session.unknown_162C[0x0C]);
  std::int64_t& nextJobExp = *reinterpret_cast<std::int64_t*>(&g_session.unknown_162C[0x14]);
  std::int64_t& jobExp = *reinterpret_cast<std::int64_t*>(&g_session.unknown_162C[0x1C]);
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
  case 1:
    if (g_session.IsAdminSubMode(2)) {
      char text[256];
      std::memset(text, 0, sizeof(text));
      const int raw = *reinterpret_cast<const int*>(packet + 4);
      const std::int64_t value = raw;
      const std::int64_t gained = value - exp;
      if (raw != 0 && gained != 0) {
        StringFormat(
          text, sizeof(text), static_cast<std::size_t>(-1), "%lld : %lld/%lld More : %lld", gained, value, nextExp, (nextExp - exp) / gained
        );
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xF51EF5, 0, 0, 0);
      }
    }
    exp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC)->baseExpBar)->SetValues(exp, nextExp);
    break;
  case 2:
    jobExp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC)->jobExpBar)->SetValues(jobExp, nextJobExp);
    break;
  case 20:
    if (*reinterpret_cast<int*>(&this->unknown_0100[0x14])) {
      *reinterpret_cast<int*>(&this->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.m_zeny = *reinterpret_cast<const int*>(packet + 4);
    g_session.RefreshHpSpWindows();
    g_session.InvalidateWindowFrames();
    break;
  case 22:
    nextExp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC)->baseExpBar)->SetValues(exp, nextExp);
    break;
  case 23:
    nextJobExp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC)->jobExpBar)->SetValues(jobExp, nextJobExp);
    break;
  }
}

#pragma pack(push, 1)
struct ParaResultRecord {
  /* 0000 */ int itemId;
  /* 0004 */ std::int16_t quantity;
  /* 0006 */ int location;
};
#pragma pack(pop)

static_assert(sizeof(ParaResultRecord) == 0x0A);

// OFFSET: 0x00D39F20
// FUNCTION: fn_21774
void CGameMode::ReceiveParaResultList(const unsigned char* packet) {
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 6) / sizeof(ParaResultRecord);
  switch (*reinterpret_cast<const std::int16_t*>(packet + 4)) {
  case -1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3043)), 0xFF, 0, 0, 0);
    break;
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(54)), 0xFFFF00, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(55)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(56)), 0xFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(221)), 0xFF, 0, 0, 0);
    break;
  case 9:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1940)), 0xFF, 0, 0, 0);
    break;
  }

  if (count <= 0) {
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(292, 0, 0, 0, 0);
    }
    return;
  }

  UIParaResultWnd* window = dynamic_cast<UIParaResultWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(256)));
  const ParaResultRecord* record = reinterpret_cast<const ParaResultRecord*>(packet + 6);
  for (int i = 0; i < count; ++i) {
    RawItemBasic item = {};
    _itoa(record->itemId, item.itemId, 10);
    item.unknown_0004 = record->location;
    item.unknown_0008 = record->location;
    item.id = -1;
    item.identified = 1;
    window->AddItem(item, record->quantity);
    ++record;
  }

  window->SendMsg(nullptr, 36, (count - 2 > 0 ? count - 2 : 0) * 32 + 120, 0, 0, 0);
  window->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x00D2C5D0
// FUNCTION: fn_21716
void CGameMode::ReceiveEquipItemListFlags(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetItems2B50();
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 0x25;
  const unsigned char* record = packet + 4;
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.unknown_0000[0] = record[6];
    std::memcpy(item.unknown_001C, record + 0x0C, sizeof(item.unknown_001C));
    item.unknown_0000[2] = *reinterpret_cast<const std::uint16_t*>(record + 7);
    item.unknown_0000[1] = *reinterpret_cast<const std::int16_t*>(record);
    item.unknown_0000[3] = *reinterpret_cast<const std::uint16_t*>(record + 9);
    item.quantity = 1;
    item.SetItemId(*reinterpret_cast<const int*>(record + 2));
    const unsigned char flags = record[0x24];
    item.identified = flags & 1;
    item.unknown_005D = (flags >> 1) & 1;
    item.unknown_0074[0] = (flags >> 2) & 1;
    item.unknown_0060 = record[0x0B];
    item.unknown_0068 = *reinterpret_cast<const std::uint32_t*>(record + 0x1C);
    item.unknown_0064 = *reinterpret_cast<const std::uint16_t*>(record + 0x20);
    item.unknown_0070 = *reinterpret_cast<const std::uint16_t*>(record + 0x22);
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.unknown_005D) {
      g_session.unknown_1050 = item.unknown_0000[1];
    }

    if (item.unknown_0000[3] == 0) {
      g_session.AddItem1710(item, 0);
      g_session.UpdateInventoryItem(item, 0);
    } else {
      char equipped = 1;
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem2B50(item, &equipped);
      } else {
        g_session.EquipUnknown17F0(item, &equipped);
      }
    }
    record += 0x25;
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D27EC0
// FUNCTION: fn_21692
void CGameMode::ReceiveEquipItemList34(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 0x22;
  const unsigned char* record = packet + 4;
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.unknown_0000[1] = *reinterpret_cast<const std::int16_t*>(record);
    item.unknown_0000[0] = record[6];
    item.unknown_0000[2] = *reinterpret_cast<const std::uint16_t*>(record + 8);
    item.unknown_0000[3] = *reinterpret_cast<const std::uint16_t*>(record + 10);
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record + 0x0E, sizeof(item.unknown_001C));
    item.SetItemId(*reinterpret_cast<const int*>(record + 2));
    item.identified = record[7];
    item.unknown_005D = record[0x0C];
    item.unknown_0060 = record[0x0D];
    item.unknown_0068 = *reinterpret_cast<const std::uint32_t*>(record + 0x1E);
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str()))) {
      if (item.unknown_005D) {
        g_session.unknown_1050 = item.unknown_0000[1];
      }
    }

    if (item.unknown_0000[3] == 0) {
      g_session.AddItem1710(item, 0);
      g_session.UpdateInventoryItem(item, 0);
    } else {
      char equipped = 1;
      g_session.EquipUnknown17F0(item, &equipped);
    }
    record += 0x22;
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D280B0
// FUNCTION: fn_21693
void CGameMode::ReceiveEquipItemList38(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetItems2B50();
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 0x26;
  const unsigned char* record = packet + 4;
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.unknown_0000[1] = *reinterpret_cast<const std::int16_t*>(record);
    item.unknown_0000[0] = record[6];
    item.unknown_0000[2] = *reinterpret_cast<const std::uint16_t*>(record + 8);
    item.unknown_0000[3] = *reinterpret_cast<const std::uint16_t*>(record + 10);
    item.quantity = 1;
    std::memcpy(item.unknown_001C, record + 0x0E, sizeof(item.unknown_001C));
    item.SetItemId(*reinterpret_cast<const int*>(record + 2));
    item.identified = record[7];
    item.unknown_005D = record[0x0C];
    item.unknown_0060 = record[0x0D];
    item.unknown_0068 = *reinterpret_cast<const std::uint32_t*>(record + 0x1E);
    item.unknown_0064 = *reinterpret_cast<const std::uint16_t*>(record + 0x22);
    item.unknown_0070 = *reinterpret_cast<const std::uint16_t*>(record + 0x24);
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str()))) {
      if (item.unknown_005D) {
        g_session.unknown_1050 = item.unknown_0000[1];
      }
    }

    if (item.unknown_0000[3] == 0) {
      g_session.AddItem1710(item, 0);
      g_session.UpdateInventoryItem(item, 0);
    } else {
      char equipped = 1;
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem2B50(item, &equipped);
      } else {
        g_session.EquipUnknown17F0(item, &equipped);
      }
    }
    record += 0x26;
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D79500
// FUNCTION: fn_22169
void CGameMode::ApplyRefiningMaterial(short itemId, unsigned char count, const unsigned char* materials, unsigned char catalystCount) {
  UIRefiningWnd* window = static_cast<UIRefiningWnd*>(g_windowMgr.LookupWindow(0x127));
  if (window) {
    window->ApplyItem(itemId, count, reinterpret_cast<RefiningMaterial*>(const_cast<unsigned char*>(materials)), catalystCount);
  }
}

// OFFSET: 0x00D4B4D0
// FUNCTION: fn_21865
void CGameMode::ReceiveAddRelatedGuild(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->unknown_00BC == 0) {
    return;
  }

  char name[25];
  std::memcpy(name, data + 0xA, 24);
  name[24] = '\0';
  if (g_forbiddenWords.Contains(name) == true) {
    std::string masked;
    g_forbiddenWords.Mask(name, &masked);
  }

  GuildInfo::RelatedGuild guild;
  guild.unknown_0000 = *reinterpret_cast<const int*>(data + 6);
  guild.unknown_0004 = *reinterpret_cast<const int*>(data + 2);
  guild.unknown_0008 = name;
  guildInfo->unknown_00E4.push_back(guild);
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D89960
// FUNCTION: fn_22210
void CGameMode::ReceiveEquipItemResult(const unsigned char* packet) {
  const std::uint16_t result = *reinterpret_cast<const std::uint16_t*>(packet + 8);
  if (result != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(result == 3 ? 3009 : 3033)), 0xFF, 0, 0, 0);
    return;
  }

  ITEM_INFO item = g_session.FindItemByIdentity(*reinterpret_cast<const std::uint16_t*>(packet + 2), *reinterpret_cast<const int*>(packet + 4));
  item.unknown_0000[3] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  ITEM_DESC description = FindItemDescription(std::atoi(item.itemId.c_str()));
  item.unknown_0070 = description.unknown_0040;
  if (g_session.IsCostumeLocation(*reinterpret_cast<const int*>(packet + 4))) {
    g_session.EquipItem6B68(item);
  } else {
    g_session.EquipItem61B8(item);
  }

  const char* suffix = LookupMessage(3143);
  g_windowMgr.SendNotice(
    1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), suffix).c_str()), 0xC8FF00, 7, 0, 0
  );
}

#pragma pack(push, 1)
struct GuildMemberAddV4Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ int accountId;
  /* 0006 */ std::uint32_t characterId;
  /* 000A */ std::int16_t unknown_000A;
  /* 000C */ std::int16_t unknown_000C;
  /* 000E */ std::int16_t unknown_000E;
  /* 0010 */ std::int16_t job;
  /* 0012 */ std::int16_t level;
  /* 0014 */ int exp;
  /* 0018 */ int online;
  /* 001C */ int positionId;
  /* 0020 */ int unknown_0020;
};
#pragma pack(pop)

static_assert(sizeof(GuildMemberAddV4Packet) == 0x24);

// OFFSET: 0x00D89CC0
// FUNCTION: fn_22214
void CGameMode::ReceiveGuildMemberAdd4(const unsigned char* data) {
  const GuildMemberAddV4Packet* packet = reinterpret_cast<const GuildMemberAddV4Packet*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->unknown_00C0 == 0) {
    return;
  }

  GuildInfo::Member member;
  member.accountId = packet->accountId;
  member.characterId = packet->characterId;
  member.name = LookupPlayerName(packet->characterId);
  member.unknown_0020 = packet->unknown_000A;
  member.unknown_0024 = packet->unknown_000C;
  member.job = packet->job;
  member.unknown_002C = packet->unknown_000E;
  member.positionId = packet->positionId;
  member.level = packet->level;
  member.exp = packet->exp;
  member.online = packet->online;
  *reinterpret_cast<int*>(member.unknown_0074) = packet->unknown_0020;
  if (member.name.size() == 0) {
    member.name = LookupMessage(0x245) + g_gameModeNamelessTag;
  }

  guildInfo->AddMember(member);
  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

struct ActorStandEntry {
  ActorStandEntry()
      : job(0), unknown_0008(0), cellX(0), cellY(0), direction(0), speed(0), unknown_001C(0), unknown_0020(0), unknown_0028(0), unknown_002C(0),
        unknown_0030(0), state(0), bodyState(0), effectState(0), stateFlags(0), unknown_0044(0), unknown_0048(0), unknown_004C(0), unknown_0050(0),
        unknown_0054(0), unknown_0058(0), unknown_005C(0), guildId(0), guildVersion(0), unknown_0068(0), unknown_006C(0), robe(0), unknown_0074(0),
        unknown_0078(0) {
  }

  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int unknown_0008;
  /* 000C */ int cellX;
  /* 0010 */ int cellY;
  /* 0014 */ int direction;
  /* 0018 */ int speed;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ int unknown_0028;
  /* 002C */ int unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int state;
  /* 0038 */ int bodyState;
  /* 003C */ int effectState;
  /* 0040 */ std::uint32_t stateFlags;
  /* 0044 */ int unknown_0044;
  /* 0048 */ int unknown_0048;
  /* 004C */ int unknown_004C;
  /* 0050 */ int unknown_0050;
  /* 0054 */ int unknown_0054;
  /* 0058 */ int unknown_0058;
  /* 005C */ int unknown_005C;
  /* 0060 */ int guildId;
  /* 0064 */ int guildVersion;
  /* 0068 */ int unknown_0068;
  /* 006C */ int unknown_006C;
  /* 0070 */ int robe;
  /* 0074 */ int unknown_0074;
  /* 0078 */ int unknown_0078;
};

static_assert(sizeof(ActorStandEntry) == 0x7C);

// OFFSET: 0x00D7AFA0
// FUNCTION: fn_22199
void CGameMode::SpawnStandingPc(
  std::uint32_t aid, int job, short cellX, short cellY, unsigned char sex, short unknown_0024, int unknown_0028, int unknown_002C,
  short unknown_0030, short unknown_004C, short unknown_0050, short unknown_0054, short unknown_0058, short robe, std::string name
) {
  ActorStandEntry entry;
  entry.aid = aid;
  entry.job = job;
  entry.unknown_0008 = this->SubD07740(job, sex);
  entry.cellX = cellX;
  entry.cellY = cellY;
  entry.direction = 4;
  entry.unknown_0024 = unknown_0024;
  entry.unknown_0028 = unknown_0028;
  entry.unknown_002C = unknown_002C;
  entry.unknown_0030 = unknown_0030;
  entry.unknown_004C = unknown_004C;
  entry.unknown_0050 = unknown_0050;
  entry.unknown_0054 = unknown_0054;
  entry.unknown_0058 = unknown_0058;
  entry.robe = robe;
  if (name.size() != 0) {
    this->nameCard.SetName(aid, name);
  }

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (actor) {
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    CPc* pc = new CPc;
    this->scene->objects.push_back(pc);
    this->scene->actors.push_back(pc);
    actor = pc;
    actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  actor->unknown_03E0Low = 1;
}

// OFFSET: 0x00D7B170
// FUNCTION: fn_22200
void CGameMode::SpawnStandingPc2(
  std::uint32_t aid, int job, short cellX, short cellY, unsigned char sex, short unknown_0024, int unknown_0028, int unknown_002C,
  short unknown_0030, short unknown_004C, short unknown_0050, short unknown_0054, short unknown_0058, short robe, std::string name, short unknown_0078
) {
  ActorStandEntry entry;
  entry.aid = aid;
  entry.job = job;
  entry.unknown_0008 = this->SubD07740(job, sex);
  entry.cellX = cellX;
  entry.cellY = cellY;
  entry.direction = 4;
  entry.unknown_0024 = unknown_0024;
  entry.unknown_0028 = unknown_0028;
  entry.unknown_002C = unknown_002C;
  entry.unknown_0030 = unknown_0030;
  entry.unknown_004C = unknown_004C;
  entry.unknown_0050 = unknown_0050;
  entry.unknown_0054 = unknown_0054;
  entry.unknown_0058 = unknown_0058;
  entry.robe = robe;
  entry.unknown_0078 = unknown_0078;
  if (name.size() != 0) {
    this->nameCard.SetName(aid, name);
  }

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (actor) {
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    CPc* pc = new CPc;
    this->scene->objects.push_back(pc);
    this->scene->actors.push_back(pc);
    actor = pc;
    actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  actor->unknown_03E0Low = 1;
}

struct MirrorSkillSpawnData {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ std::uint32_t owner;
  /* 0008 */ int type;
  /* 000C */ int x;
  /* 0010 */ int y;
  /* 0014 */ unsigned char visible;
  /* 0018 */ int level;
};

static_assert(sizeof(MirrorSkillSpawnData) == 0x1C);

// OFFSET: 0x00D60D70
// FUNCTION: fn_21975
void CGameMode::SpawnMirrorActor(std::uint32_t aid, std::uint32_t owner, short x, short y) {
  if (owner == g_session.m_aid) {
    CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
    if (actor) {
      this->CellCenter(x, y, actor->render.position.x, actor->render.position.z);
      actor->render.position.y =
        this->scene->terrain->Height(actor->render.position.x, actor->render.position.z);
      return;
    }

    ActorStandEntry entry;
    entry.cellX = x;
    entry.aid = aid;
    entry.cellY = y;
    entry.job = g_session.GetDisplayJob();
    entry.unknown_0008 = g_session.SyncUnknown0E98();
    entry.unknown_0024 = g_session.m_head;
    entry.unknown_0028 = *reinterpret_cast<const int*>(&g_session.unknown_0EDC[0x4]);
    entry.unknown_002C = *reinterpret_cast<const int*>(&g_session.unknown_0EDC[0x8]);
    entry.unknown_0030 = *reinterpret_cast<const int*>(&g_session.unknown_0EF4[0x0]);
    entry.direction = 180;
    entry.unknown_0048 = *reinterpret_cast<const int*>(&g_session.unknown_0EDC[0xC]);
    entry.unknown_004C = *reinterpret_cast<const int*>(&g_session.unknown_0EF4[0x4]);
    entry.unknown_0050 = *reinterpret_cast<const int*>(&g_session.unknown_0EF4[0x8]);
    entry.unknown_0054 = g_session.m_headPalette;
    entry.unknown_0058 = g_session.m_bodyStyle;
    entry.robe = g_session.m_robe;
    entry.unknown_0078 = g_session.m_bodyCostume;

    CPc* pc = new CPc;
    pc->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    pc->unknown_0314 = 7;
    pc->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    this->scene->objects.push_back(pc);
    this->scene->actors.push_back(pc);
    this->spriteMirrorActorIds.push_back(aid);
    return;
  }

  if (this->scene->GetSkillByAID(aid)) {
    this->scene->RemoveSkill(aid);
  }

  MirrorSkillSpawnData data;
  data.aid = aid;
  data.owner = owner;
  data.type = 0x126;
  data.x = x;
  data.y = y;
  data.visible = 1;
  CSkill* skill = new CSkill(0);
  skill->SendMsg(nullptr, 0x2D, 0, reinterpret_cast<int>(&data), reinterpret_cast<int>(&data) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->scene->objects.push_back(skill);
  this->scene->unknown_0020.push_back(skill);
}

#pragma pack(push, 1)
struct Item1748Record2 {
  /* 0000 */ std::uint32_t price;
  /* 0004 */ std::int16_t amount;
  /* 0006 */ std::int16_t index;
  /* 0008 */ unsigned char type;
  /* 0009 */ int itemId;
  /* 000D */ unsigned char identified;
  /* 000E */ unsigned char damaged;
  /* 000F */ std::uint32_t cards[4];
  /* 001F */ StoreItemOption options[5];
  /* 0038 */ std::int32_t unknown_0038;
  /* 003C */ std::uint16_t sprite;
  /* 003E */ unsigned char refine;
  /* 003F */ unsigned char grade;
};

struct Item1758StoreRecord2 {
  /* 0000 */ std::uint32_t price;
  /* 0004 */ std::int16_t index;
  /* 0006 */ std::int16_t amount;
  /* 0008 */ unsigned char type;
  /* 0009 */ int itemId;
  /* 000D */ unsigned char identified;
  /* 000E */ unsigned char damaged;
  /* 000F */ std::uint32_t cards[4];
  /* 001F */ StoreItemOption options[5];
  /* 0038 */ unsigned char refine;
  /* 0039 */ unsigned char grade;
};
#pragma pack(pop)

static_assert(sizeof(Item1748Record2) == 0x40);
static_assert(sizeof(Item1758StoreRecord2) == 0x3A);

// OFFSET: 0x00D782F0
// FUNCTION: fn_22161
void CGameMode::ReceiveItem1748List2(const unsigned char* packet) {
  std::vector<ITEM_INFO> items;
  const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 0x0C) / sizeof(Item1748Record2);
  items.reserve(count);
  for (std::int64_t i = 0; i < count; ++i) {
    Item1748Record2 record;
    memcpy_s(&record, sizeof(record), packet + 0x0C + i * sizeof(Item1748Record2), sizeof(record));

    ITEM_INFO item;
    item.unknown_0000[1] = record.index;
    item.unknown_0014[0] = record.price;
    item.unknown_0014[1] = record.price;
    item.unknown_0000[0] = record.type;
    item.quantity = record.amount;
    item.identified = record.identified;
    item.unknown_005D = record.damaged;
    item.unknown_0060 = record.refine;
    item.SetItemId(record.itemId);
    std::memcpy(item.unknown_001C, record.cards, sizeof(record.cards));
    item.unknown_0098 = CountStoreItemOptions(record.options);
    for (std::int64_t index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = record.options[index];
    }
    item.unknown_0000[2] = record.unknown_0038;
    item.unknown_0070 = record.sprite;
    item.unknown_0088 = record.grade;
    items.push_back(item);
  }

  const std::int32_t value8 = *reinterpret_cast<const std::int32_t*>(packet + 8);
  const std::int32_t value4 = *reinterpret_cast<const std::int32_t*>(packet + 4);
  g_session.ClearItems1748();
  g_session.ClearItems1760();
  for (ITEM_INFO item : items) {
    g_session.AddItem1748(item);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(43)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
    window->SendMsg(nullptr, 28, value4, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(44)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
    window->SendMsg(nullptr, 28, value4, value8, 0, 0);
  }
  this->unknown_024C = 1;
}

#pragma pack(push, 1)
struct PartyMemberLeftPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ char name[24];
  /* 001E */ unsigned char type;
};
#pragma pack(pop)

static_assert(sizeof(PartyMemberLeftPacket) == 0x1F);

// OFFSET: 0x00D26BB0
// FUNCTION: fn_21684
void CGameMode::ReceivePartyMemberLeft(const unsigned char* data) {
  const PartyMemberLeftPacket* packet = reinterpret_cast<const PartyMemberLeftPacket*>(data);
  char name[25];
  name[24] = '\0';
  std::memcpy(name, packet->name, sizeof(packet->name));
  switch (packet->type) {
  case 0:
  case 1:
    this->SendMsg(0x40, g_session.GetPartyMemberAid(name), 0, 0, 0);
    g_session.RemovePartyMemberByName(name);
    break;
  case 2:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x750)), 0xFF, 0, 0, 0);
    return;
  case 3:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x751)), 0xFF, 0, 0, 0);
    return;
  }

  if (std::strcmp(g_session.DecodePlayerName(), name) == 0) {
    g_session.unknown_5464 = 0;
    g_session.ClearPartyMembers();
    g_session.unknown_54C4.assign("", 0);
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x54)), 0xFFFF00, 0, 0, 0);
    g_windowMgr.CloseWindow(0x35);
    this->SendMsg(0x41, 0, 0, 0, 0);
    if (g_windowMgr.unknown_03F8) {
      g_windowMgr.CloseWindow(0x89);
    }
    g_session.ClearRecords0734();
  }

  sub_DFF150(&g_session);
  if (CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid))) {
    actor->SetUnknown02E4(0);
  }
}

// OFFSET: 0x00D373C0
// FUNCTION: fn_21763
void CGameMode::ReceiveBroadcast(const unsigned char* packet) {
  const unsigned length = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  const unsigned size = length - 4;
  if (length - 5 > 0x168) {
    return;
  }

  std::vector<char> text(length - 3, 0);
  memcpy_s(text.data(), text.size(), packet + 4, size);
  if (g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked;
    g_forbiddenWords.Mask(text.data(), &masked);
  }

  if (g_windowMgr.window_027C == nullptr) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.data()), 0xFF00, 1, 0, 0);
    char* message = text.data();
    this->scene->player->SendMsg(
      nullptr, 7, 0, reinterpret_cast<int>(message), reinterpret_cast<int>(message) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    );
  } else {
    g_windowMgr.SendNotice(5, reinterpret_cast<std::intptr_t>(text.data()), 0x7800, 0, 0, 0);
  }

  if (g_whisperRecord != 0) {
    this->WriteChatLog(text.data());
  }
}
// OFFSET: 0x00D787E0
// FUNCTION: fn_22162
void CGameMode::ReceiveItem1758StoreList(const unsigned char* packet) {
  std::vector<ITEM_INFO> items;
  const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 8) / sizeof(Item1758StoreRecord2);
  items.reserve(count);
  for (std::int64_t i = 0; i < count; ++i) {
    Item1758StoreRecord2 record;
    memcpy_s(&record, sizeof(record), packet + 8 + i * sizeof(Item1758StoreRecord2), sizeof(record));

    ITEM_INFO item;
    item.unknown_0000[1] = record.index;
    item.unknown_0014[0] = record.price;
    item.unknown_0014[1] = record.price;
    item.quantity = record.amount;
    item.unknown_0000[0] = record.type;
    item.SetItemId(record.itemId);
    item.identified = record.identified;
    item.unknown_005D = record.damaged;
    item.unknown_0060 = record.refine;
    std::memcpy(item.unknown_001C, record.cards, sizeof(record.cards));
    item.unknown_0098 = CountStoreItemOptions(record.options);
    for (std::int64_t index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<StoreItemOption*>(item.unknown_009C)[index] = record.options[index];
    }
    item.unknown_0088 = record.grade;
    items.push_back(item);
  }

  if (g_replayAnimationScaling != 0) {
    g_modeMgr.GetGameMode()->SendMsg(82, 0, 0, 0, 0);
  }

  g_session.ClearItems1758();
  if (items.size() == 0) {
    this->SendMsg(40, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(43);
    g_windowMgr.CloseWindow(44);
    g_session.ClearItems1760And1748();
    return;
  }

  for (ITEM_INFO item : items) {
    g_session.AddItem1758(item);
  }
}

#pragma pack(push, 1)
struct ActorStandEntryPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ int effectState;
  /* 0010 */ short job;
  /* 0012 */ short head;
  /* 0014 */ int weapon;
  /* 0018 */ int shield;
  /* 001C */ short accessory;
  /* 001E */ short accessory2;
  /* 0020 */ short accessory3;
  /* 0022 */ short headPalette;
  /* 0024 */ short bodyPalette;
  /* 0026 */ short headDir;
  /* 0028 */ int guildId;
  /* 002C */ short guildVersion;
  /* 002E */ short honor;
  /* 0030 */ int virtue;
  /* 0034 */ unsigned char pkMode;
  /* 0035 */ unsigned char sex;
  /* 0036 */ unsigned char posDir[3];
  /* 0039 */ unsigned char xSize;
  /* 003A */ unsigned char ySize;
  /* 003B */ unsigned char state;
  /* 003C */ short level;
  /* 003E */ short font;
};

struct ActorTypedEntryPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ unsigned char objectType;
  /* 0003 */ std::uint32_t aid;
  /* 0007 */ short speed;
  /* 0009 */ short bodyState;
  /* 000B */ short healthState;
  /* 000D */ short effectState;
  /* 000F */ short job;
  /* 0011 */ short head;
  /* 0013 */ short weapon;
  /* 0015 */ short accessory;
  /* 0017 */ short shield;
  /* 0019 */ short accessory2;
  /* 001B */ short accessory3;
  /* 001D */ short headPalette;
  /* 001F */ short bodyPalette;
  /* 0021 */ short headDir;
  /* 0023 */ int guildId;
  /* 0027 */ short guildVersion;
  /* 0029 */ short honor;
  /* 002B */ short virtue;
  /* 002D */ unsigned char pkMode;
  /* 002E */ unsigned char sex;
  /* 002F */ unsigned char posDir[3];
  /* 0032 */ unsigned char xSize;
  /* 0033 */ unsigned char ySize;
  /* 0034 */ unsigned char state;
  /* 0035 */ short level;
};

struct NpcStandEntryPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ unsigned char objectType;
  /* 0003 */ std::uint32_t aid;
  /* 0007 */ short speed;
  /* 0009 */ short bodyState;
  /* 000B */ short healthState;
  /* 000D */ short effectState;
  /* 000F */ short head;
  /* 0011 */ short weapon;
  /* 0013 */ short accessory;
  /* 0015 */ short job;
  /* 0017 */ short shield;
  /* 0019 */ short accessory2;
  /* 001B */ short accessory3;
  /* 001D */ short headPalette;
  /* 001F */ short bodyPalette;
  /* 0021 */ short headDir;
  /* 0023 */ unsigned char pkMode;
  /* 0024 */ unsigned char sex;
  /* 0025 */ unsigned char posDir[3];
  /* 0028 */ unsigned char xSize;
  /* 0029 */ unsigned char ySize;
  /* 002A */ std::uint16_t unknown_002A;
};
#pragma pack(pop)

static_assert(sizeof(ActorStandEntryPacket) == 0x40);
static_assert(sizeof(ActorTypedEntryPacket) == 0x37);
static_assert(sizeof(NpcStandEntryPacket) == 0x2C);

// OFFSET: 0x00D38330
// FUNCTION: fn_21767
void CGameMode::ReceiveActorStandEntry(const unsigned char* data) {
  const ActorStandEntryPacket* packet = reinterpret_cast<const ActorStandEntryPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_002C = packet->shield;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.aid = packet->aid;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    CGameActor* created;
    if ((job >= JT_WARPNPC && job < 1000) || (job > 1000 && job < 3999) || static_cast<std::uint16_t>(job - 20021) <= 11978 ||
        g_session.IsSkillIdInRange6001(job)) {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      created = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    } else {
      created = new CPc;
    }

    actor = created;
    if (actor && entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D387B0
// FUNCTION: fn_21768
void CGameMode::ReceiveActorStandEntryFont(const unsigned char* data) {
  const ActorStandEntryPacket* packet = reinterpret_cast<const ActorStandEntryPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_002C = packet->shield;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.aid = packet->aid;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    *reinterpret_cast<std::uint16_t*>(actor->render.unknown_00FCBytes) = packet->font;
    if (entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    CGameActor* created;
    if ((job >= JT_WARPNPC && job < 1000) || (job > 1000 && job < 3999) || static_cast<std::uint16_t>(job - 20021) <= 11978 ||
        g_session.IsSkillIdInRange6001(job)) {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      created = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    } else {
      created = new CPc;
    }

    actor = created;
    if (actor && entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      *reinterpret_cast<std::uint16_t*>(actor->render.unknown_00FCBytes) = packet->font;
      if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D38C40
// FUNCTION: fn_21769
void CGameMode::ReceiveActorTypedEntry(const unsigned char* data) {
  const ActorTypedEntryPacket* packet = reinterpret_cast<const ActorTypedEntryPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_002C = packet->weapon >> 31;
  entry.unknown_0030 = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_0048 = packet->shield;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.aid = packet->aid;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
      const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
      const int guildVersion = packet->shield;
      if (guildId > 0 && !actor->Slot50()) {
        actor->Slot49(guildId, guildVersion);
      }
    }
    if (packet->objectType == 7) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && packet->objectType != 7 && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    CGameActor* created;
    if ((job >= JT_WARPNPC && job < 1000) || (job > 1000 && job < 3999) || static_cast<std::uint16_t>(job - 20021) <= 11978 ||
        g_session.IsSkillIdInRange6001(job) || packet->objectType == 9) {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      created = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    } else {
      created = new CPc;
    }

    actor = created;
    if (actor && packet->objectType == 7) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->unknown_0314 = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (g_session.GetSceneUnknown004C() && packet->objectType != 7 && actor->unknown_0188 == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      const int actorJob = actor->GetEffectiveJob(-1);
      if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
        const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
        if (guildId > 0) {
          actor->Slot49(guildId, packet->shield);
        }
      }
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D39160
// FUNCTION: fn_21770
void CGameMode::ReceiveNpcStandEntry(const unsigned char* data) {
  const NpcStandEntryPacket* packet = reinterpret_cast<const NpcStandEntryPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = packet->sex;
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_002C = packet->weapon >> 31;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_0048 = packet->shield;
  entry.aid = packet->aid;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (!actor) {
    actor = this->NewActor(packet->job);
    entry.unknown_0008 = 0;
    actor->unknown_0314 = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
      const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
      if (guildId > 0) {
        actor->Slot49(guildId, packet->shield);
      }
    }
    actor->unknown_0210 = packet->unknown_002A;
    return;
  }

  actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  const int actorJob = actor->GetEffectiveJob(-1);
  if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
    const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
    const int guildVersion = packet->shield;
    if (guildId > 0 && !actor->Slot50()) {
      actor->Slot49(guildId, guildVersion);
    }
  }
  actor->unknown_0210 = packet->unknown_002A;
}

struct ActorMoveEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int unknown_0008;
  /* 000C */ std::uint32_t moveStartTime;
  /* 0010 */ int srcX;
  /* 0014 */ int srcY;
  /* 0018 */ int subX;
  /* 001C */ int subY;
  /* 0020 */ int dstX;
  /* 0024 */ int dstY;
  /* 0028 */ int speed;
  /* 002C */ int head;
  /* 0030 */ int weapon;
  /* 0034 */ int shield;
  /* 0038 */ int accessory;
  /* 003C */ int bodyState;
  /* 0040 */ int healthState;
  /* 0044 */ int effectState;
  /* 0048 */ int pkMode;
  /* 004C */ int unknown_004C;
  /* 0050 */ int accessory2;
  /* 0054 */ int accessory3;
  /* 0058 */ int headPalette;
  /* 005C */ int bodyPalette;
  /* 0060 */ int headDir;
  /* 0064 */ int guildId;
  /* 0068 */ int guildVersion;
  /* 006C */ int honor;
  /* 0070 */ int virtue;
  /* 0074 */ int robe;
  /* 0078 */ std::uint32_t gid;
  /* 007C */ int body;
};

static_assert(sizeof(ActorMoveEntry) == 0x80);

#pragma pack(push, 1)
struct ActorMoveEntryPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ std::uint32_t gid;
  /* 000D */ short speed;
  /* 000F */ short bodyState;
  /* 0011 */ short healthState;
  /* 0013 */ int effectState;
  /* 0017 */ short job;
  /* 0019 */ short head;
  /* 001B */ int weapon;
  /* 001F */ int shield;
  /* 0023 */ short accessory;
  /* 0025 */ std::uint32_t moveStartTime;
  /* 0029 */ short accessory2;
  /* 002B */ short accessory3;
  /* 002D */ short headPalette;
  /* 002F */ short bodyPalette;
  /* 0031 */ short headDir;
  /* 0033 */ short robe;
  /* 0035 */ int guildId;
  /* 0039 */ short guildVersion;
  /* 003B */ short honor;
  /* 003D */ int virtue;
  /* 0041 */ unsigned char pkMode;
  /* 0042 */ unsigned char sex;
  /* 0043 */ unsigned char moveData[6];
  /* 0049 */ unsigned char xSize;
  /* 004A */ unsigned char ySize;
  /* 004B */ short level;
  /* 004D */ short font;
  /* 004F */ int maxHp;
  /* 0053 */ int hp;
  /* 0057 */ unsigned char isBoss;
};

struct ActorMoveEntryBodyPacket : ActorMoveEntryPacket {
  /* 0058 */ short body;
};

struct ActorMoveEntryLegacyPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ int weapon;
  /* 001B */ int shield;
  /* 001F */ short accessory;
  /* 0021 */ std::uint32_t moveStartTime;
  /* 0025 */ short accessory2;
  /* 0027 */ short accessory3;
  /* 0029 */ short headPalette;
  /* 002B */ short bodyPalette;
  /* 002D */ short headDir;
  /* 002F */ short robe;
  /* 0031 */ int guildId;
  /* 0035 */ short guildVersion;
  /* 0037 */ short honor;
  /* 0039 */ int virtue;
  /* 003D */ unsigned char pkMode;
  /* 003E */ unsigned char sex;
  /* 003F */ unsigned char moveData[6];
  /* 0045 */ unsigned char xSize;
  /* 0046 */ unsigned char ySize;
  /* 0047 */ short level;
  /* 0049 */ short font;
  /* 004B */ int maxHp;
  /* 004F */ int hp;
  /* 0053 */ unsigned char isBoss;
};
#pragma pack(pop)

static_assert(sizeof(ActorMoveEntryPacket) == 0x58);
static_assert(sizeof(ActorMoveEntryBodyPacket) == 0x5A);
static_assert(sizeof(ActorMoveEntryLegacyPacket) == 0x54);

// OFFSET: 0x00D61390
// FUNCTION: fn_21982
void CGameMode::ReceiveActorMoveEntry(const unsigned char* data) {
  const ActorMoveEntryPacket* packet = reinterpret_cast<const ActorMoveEntryPacket*>(data);
  const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorMoveEntryPacket));
  const std::string name(nameBegin, reinterpret_cast<const char*>(data + packet->packetLength));
  this->SpawnMovingActor(
    packet->objectType, packet->aid, packet->gid, packet->speed, packet->bodyState, packet->healthState, packet->effectState, packet->job, packet->head,
    packet->weapon, packet->shield, packet->accessory, packet->moveStartTime, packet->accessory2, packet->accessory3, packet->headPalette,
    packet->bodyPalette, packet->headDir, packet->robe, packet->guildId, packet->guildVersion, packet->honor, packet->virtue, packet->pkMode, packet->sex,
    packet->moveData, packet->xSize, packet->ySize, packet->level, packet->font, packet->maxHp, packet->hp, packet->isBoss, name, 0
  );
}

// OFFSET: 0x00D616B0
// FUNCTION: fn_21983
void CGameMode::ReceiveActorMoveEntryBody(const unsigned char* data) {
  const ActorMoveEntryBodyPacket* packet = reinterpret_cast<const ActorMoveEntryBodyPacket*>(data);
  const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorMoveEntryBodyPacket));
  const std::string name(nameBegin, reinterpret_cast<const char*>(data + packet->packetLength));
  this->SpawnMovingActor(
    packet->objectType, packet->aid, packet->gid, packet->speed, packet->bodyState, packet->healthState, packet->effectState, packet->job, packet->head,
    packet->weapon, packet->shield, packet->accessory, packet->moveStartTime, packet->accessory2, packet->accessory3, packet->headPalette,
    packet->bodyPalette, packet->headDir, packet->robe, packet->guildId, packet->guildVersion, packet->honor, packet->virtue, packet->pkMode, packet->sex,
    packet->moveData, packet->xSize, packet->ySize, packet->level, packet->font, packet->maxHp, packet->hp, packet->isBoss, name, packet->body
  );
}

// OFFSET: 0x00D619E0
// FUNCTION: fn_21984
void CGameMode::ReceiveActorMoveEntryLegacy(const unsigned char* data) {
  const ActorMoveEntryLegacyPacket* packet = reinterpret_cast<const ActorMoveEntryLegacyPacket*>(data);
  const int nameLength = packet->packetLength - static_cast<int>(sizeof(ActorMoveEntryLegacyPacket));
  if (nameLength > 0) {
    const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorMoveEntryLegacyPacket));
    this->nameCard.SetName(packet->aid, std::string(nameBegin, nameBegin + nameLength));
  }

  ActorMoveEntry entry;
  entry.robe = packet->robe;
  entry.job = packet->job;
  entry.aid = packet->aid;
  entry.moveStartTime = packet->moveStartTime;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.healthState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.effectState = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDir = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.shield = packet->shield;
  entry.virtue = packet->virtue;
  DecodeSrcDst(packet->moveData, &entry.srcX, &entry.srcY, &entry.dstX, &entry.dstY, &entry.subX, &entry.subY);
  g_session.SyncServerTime(entry.moveStartTime);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    if (actor->unknown_02E4) {
      auto member = this->unknown_01B4.find(actor->render.accountId);
      if (member != this->unknown_01B4.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->unknown_01BC.find(actor->render.accountId);
      if (member != this->unknown_01BC.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
    actor->SendMsg(nullptr, 0x7B, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    CGameActor* created;
    if (packet->objectType == 0) {
      created = new CPc;
    } else {
      created = this->NewActor(packet->job);
    }
    if (!created) {
      return;
    }

    created->unknown_0314 = packet->objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (actor->unknown_0314 == 0) {
      if (this->unknown_01B4.find(entry.aid) != this->unknown_01B4.end() || this->unknown_01BC.find(entry.aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
    } else {
      entry.unknown_0008 = 0;
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
    actor->SendMsg(nullptr, 0x7A, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  std::int64_t bossFlag = 0;
  if (packet->isBoss && actor->unknown_0304 == 0) {
    actor->unknown_0304 = packet->isBoss;
    bossFlag = 1;
  }
  actor->unknown_0210 = packet->level;
  *reinterpret_cast<std::uint16_t*>(actor->render.unknown_00FCBytes) = packet->font;
  if (packet->objectType == 7) {
    if (g_session.IsPetAid(packet->aid)) {
      g_session.unknown_1038 = entry.accessory;
    }
    if (entry.accessory && !actor->Slot50()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
    }
  } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
    g_session.unknown_5614 = 1;
  } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
    g_session.unknown_56B0 = 1;
  } else if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  switch (actor->unknown_0314) {
  case 0:
  case 5:
  case 8:
  case 9:
  case 10: {
    const std::int64_t hp = packet->hp;
    const std::int64_t maxHp = packet->maxHp;
    actor->SendMsg(
      nullptr, 0x91, 0, static_cast<int>(hp), static_cast<int>(hp >> 32), static_cast<int>(maxHp), static_cast<int>(maxHp >> 32),
      static_cast<int>(bossFlag), static_cast<int>(bossFlag >> 32), 0, 0, 0, 0, 0, 0
    );
    break;
  }
  case 13:
  case 14:
    actor->SendMsg(nullptr, 0x22, 0, packet->aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D61F40
// FUNCTION: fn_21985
void CGameMode::SpawnMovingActor(
  unsigned char objectType, std::uint32_t aid, std::uint32_t gid, short speed, short bodyState, short healthState, int effectState, short job, short head,
  int weapon, int shield, short accessory, std::uint32_t moveStartTime, short accessory2, short accessory3, short headPalette, short bodyPalette,
  short headDir, short robe, int guildId, short guildVersion, short honor, int virtue, unsigned char pkMode, unsigned char sex,
  const unsigned char* moveData, unsigned char xSize, unsigned char ySize, short level, short font, int maxHp, int hp, unsigned char isBoss,
  std::string name, short body
) {
  if (g_replayAnimationScaling != 0 && name.size() == 0) {
    return;
  }

  if (objectType == 0 && gid != 0) {
    g_playerNameDictionary->Set(gid, name);
  }
  this->nameCard.SetName(aid, name);

  ActorMoveEntry entry;
  entry.robe = robe;
  entry.moveStartTime = moveStartTime;
  entry.job = job;
  entry.aid = aid;
  entry.unknown_0008 = this->SubD07740(entry.job, sex);
  entry.speed = speed;
  entry.head = head;
  entry.weapon = weapon;
  entry.accessory = accessory;
  entry.healthState = healthState;
  entry.bodyState = bodyState;
  entry.effectState = effectState;
  entry.pkMode = pkMode;
  entry.accessory2 = accessory2;
  entry.accessory3 = accessory3;
  entry.headPalette = headPalette;
  entry.bodyPalette = bodyPalette;
  entry.headDir = headDir;
  entry.guildId = guildId;
  entry.guildVersion = guildVersion;
  entry.honor = honor;
  entry.shield = shield;
  entry.virtue = virtue;
  entry.gid = gid;
  entry.body = body;
  DecodeSrcDst(moveData, &entry.srcX, &entry.srcY, &entry.dstX, &entry.dstY, &entry.subX, &entry.subY);
  g_session.SyncServerTime(entry.moveStartTime);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (actor) {
    if (actor->unknown_02E4) {
      auto member = this->unknown_01B4.find(actor->render.accountId);
      if (member != this->unknown_01B4.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->unknown_01BC.find(actor->render.accountId);
      if (member != this->unknown_01BC.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
    actor->SendMsg(nullptr, 0x7B, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    CGameActor* created;
    if (objectType == 0) {
      created = new CPc;
    } else {
      created = this->NewActor(job);
    }
    if (!created) {
      return;
    }

    created->unknown_0314 = objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (actor->unknown_0314 == 0) {
      if (this->unknown_01B4.find(entry.aid) != this->unknown_01B4.end() || this->unknown_01BC.find(entry.aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
    } else if (!((objectType == 1 || objectType == 6 || objectType == 12) && IsPlayerJob(job))) {
      entry.unknown_0008 = 0;
    }
    actor->SendMsg(nullptr, 0x7A, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
  g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);

  std::int64_t bossFlag = 0;
  if (isBoss && actor->unknown_0304 == 0) {
    actor->unknown_0304 = isBoss;
    bossFlag = 1;
  }
  actor->unknown_0210 = level;
  *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = font;
  if (objectType == 7) {
    if (g_session.IsPetAid(aid)) {
      g_session.unknown_1038 = entry.accessory;
    }
    if (entry.accessory && !actor->Slot50()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
    }
  } else if (objectType == 8 && g_session.IsHomunAid(aid)) {
    g_session.unknown_5614 = 1;
  } else if (objectType == 9 && g_session.IsMercenaryAid(aid)) {
    g_session.unknown_56B0 = 1;
  } else if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, guildId, guildVersion);
  }

  switch (actor->unknown_0314) {
  case 0:
  case 5:
  case 8:
  case 9:
  case 10: {
    const std::int64_t currentHp = hp;
    const std::int64_t currentMaxHp = maxHp;
    actor->SendMsg(
      nullptr, 0x91, 0, static_cast<int>(currentHp), static_cast<int>(currentHp >> 32), static_cast<int>(currentMaxHp),
      static_cast<int>(currentMaxHp >> 32), static_cast<int>(bossFlag), static_cast<int>(bossFlag >> 32), 0, 0, 0, 0, 0, 0
    );
    break;
  }
  case 13:
  case 14:
    actor->SendMsg(nullptr, 0x22, 0, gid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }

  if (objectType == 0) {
    CellFloatPos position;
    position.x = actor->render.position.x;
    position.z = actor->render.position.z;
    this->scene->AddPcPosUnindexed(aid, position);
  }
}

#pragma pack(push, 1)
struct ActorMoveEntryOldPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ short effectState;
  /* 000E */ short job;
  /* 0010 */ short head;
  /* 0012 */ int weapon;
  /* 0016 */ int shield;
  /* 001A */ short accessory;
  /* 001C */ std::uint32_t moveStartTime;
  /* 0020 */ short accessory2;
  /* 0022 */ short accessory3;
  /* 0024 */ short headPalette;
  /* 0026 */ short bodyPalette;
  /* 0028 */ short headDir;
  /* 002A */ int guildId;
  /* 002E */ short guildVersion;
  /* 0030 */ short honor;
  /* 0032 */ short virtue;
  /* 0034 */ unsigned char pkMode;
  /* 0035 */ unsigned char sex;
  /* 0036 */ unsigned char moveData[6];
  /* 003C */ unsigned char xSize;
  /* 003D */ unsigned char ySize;
  /* 003E */ short level;
};
#pragma pack(pop)

static_assert(sizeof(ActorMoveEntryOldPacket) == 0x40);

// OFFSET: 0x00D346A0
// FUNCTION: fn_21754
void CGameMode::ReceiveActorMoveEntryOld(const unsigned char* data) {
  const ActorMoveEntryOldPacket* packet = reinterpret_cast<const ActorMoveEntryOldPacket*>(data);
  ActorMoveEntry entry;
  entry.body = 0;
  entry.aid = packet->aid;
  entry.job = packet->job;
  entry.moveStartTime = packet->moveStartTime;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.healthState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.effectState = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDir = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.shield = packet->shield;
  entry.virtue = packet->virtue;
  DecodeSrcDst(packet->moveData, &entry.srcX, &entry.srcY, &entry.dstX, &entry.dstY, &entry.subX, &entry.subY);
  g_session.SyncServerTime(entry.moveStartTime);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    actor->SendMsg(nullptr, 0x7B, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (entry.head == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory && !actor->Slot50()) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    if (g_session.GetSceneUnknown004C() && entry.head != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (actor->unknown_02E4) {
      auto member = this->unknown_01B4.find(actor->render.accountId);
      if (member != this->unknown_01B4.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->unknown_01BC.find(actor->render.accountId);
      if (member != this->unknown_01BC.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
  } else {
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job)) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 0x7A, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && entry.head != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->unknown_01B4.find(entry.aid) != this->unknown_01B4.end() || this->unknown_01BC.find(entry.aid) != this->unknown_01BC.end()) {
      actor->SetUnknown02E4(1);
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorMoveEntryOld2Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ unsigned char objectType;
  /* 0003 */ std::uint32_t aid;
  /* 0007 */ short speed;
  /* 0009 */ short bodyState;
  /* 000B */ short healthState;
  /* 000D */ int effectState;
  /* 0011 */ short job;
  /* 0013 */ short head;
  /* 0015 */ int weapon;
  /* 0019 */ int shield;
  /* 001D */ short accessory;
  /* 001F */ std::uint32_t moveStartTime;
  /* 0023 */ short accessory2;
  /* 0025 */ short accessory3;
  /* 0027 */ short headPalette;
  /* 0029 */ short bodyPalette;
  /* 002B */ short headDir;
  /* 002D */ int guildId;
  /* 0031 */ short guildVersion;
  /* 0033 */ short honor;
  /* 0035 */ int virtue;
  /* 0039 */ unsigned char pkMode;
  /* 003A */ unsigned char sex;
  /* 003B */ unsigned char moveData[6];
  /* 0041 */ unsigned char xSize;
  /* 0042 */ unsigned char ySize;
  /* 0043 */ short level;
};
#pragma pack(pop)

static_assert(sizeof(ActorMoveEntryOld2Packet) == 0x45);

// OFFSET: 0x00D34B90
// FUNCTION: fn_21755
void CGameMode::ReceiveActorMoveEntryOld2(const unsigned char* data) {
  const ActorMoveEntryOld2Packet* packet = reinterpret_cast<const ActorMoveEntryOld2Packet*>(data);
  ActorMoveEntry entry;
  entry.body = 0;
  entry.aid = packet->aid;
  entry.job = packet->job;
  entry.moveStartTime = packet->moveStartTime;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.healthState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.effectState = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDir = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.shield = packet->shield;
  entry.virtue = packet->virtue;
  DecodeSrcDst(packet->moveData, &entry.srcX, &entry.srcY, &entry.dstX, &entry.dstY, &entry.subX, &entry.subY);
  g_session.SyncServerTime(entry.moveStartTime);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    actor->unknown_0210 = packet->level;
    actor->SendMsg(nullptr, 0x7B, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (packet->objectType == 7) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory && !actor->Slot50()) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    if (g_session.GetSceneUnknown004C() && packet->objectType != 7 && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (actor->unknown_02E4) {
      auto member = this->unknown_01B4.find(actor->render.accountId);
      if (member != this->unknown_01B4.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->unknown_01BC.find(actor->render.accountId);
      if (member != this->unknown_01BC.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
  } else {
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job) &&
        packet->objectType != 9) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && packet->objectType == 7) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->unknown_0210 = packet->level;
    actor->unknown_0314 = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 0x7A, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && packet->objectType == 7 && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->unknown_01B4.find(entry.aid) != this->unknown_01B4.end() || this->unknown_01BC.find(entry.aid) != this->unknown_01BC.end()) {
      actor->SetUnknown02E4(1);
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorMoveEntryNamedPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ int weapon;
  /* 001B */ int shield;
  /* 001F */ short accessory;
  /* 0021 */ std::uint32_t moveStartTime;
  /* 0025 */ short accessory2;
  /* 0027 */ short accessory3;
  /* 0029 */ short headPalette;
  /* 002B */ short bodyPalette;
  /* 002D */ short headDir;
  /* 002F */ short robe;
  /* 0031 */ int guildId;
  /* 0035 */ short guildVersion;
  /* 0037 */ short honor;
  /* 0039 */ int virtue;
  /* 003D */ unsigned char pkMode;
  /* 003E */ unsigned char sex;
  /* 003F */ unsigned char moveData[6];
  /* 0045 */ unsigned char xSize;
  /* 0046 */ unsigned char ySize;
  /* 0047 */ short level;
  /* 0049 */ short font;
};
#pragma pack(pop)

static_assert(sizeof(ActorMoveEntryNamedPacket) == 0x4B);

// OFFSET: 0x00D350A0
// FUNCTION: fn_21756
void CGameMode::ReceiveActorMoveEntryNamed(const unsigned char* data) {
  const ActorMoveEntryNamedPacket* packet = reinterpret_cast<const ActorMoveEntryNamedPacket*>(data);
  ActorMoveEntry entry;
  entry.body = 0;
  entry.robe = packet->robe;
  const int nameLength = packet->packetLength - static_cast<int>(sizeof(ActorMoveEntryNamedPacket));
  if (nameLength > 0) {
    const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorMoveEntryNamedPacket));
    this->nameCard.SetName(packet->aid, std::string(nameBegin, nameBegin + nameLength));
  }

  entry.aid = packet->aid;
  entry.job = packet->job;
  entry.moveStartTime = packet->moveStartTime;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.healthState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.effectState = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDir = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.shield = packet->shield;
  entry.virtue = packet->virtue;
  DecodeSrcDst(packet->moveData, &entry.srcX, &entry.srcY, &entry.dstX, &entry.dstY, &entry.subX, &entry.subY);
  g_session.SyncServerTime(entry.moveStartTime);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    actor->unknown_0210 = packet->level;
    *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
    actor->SendMsg(nullptr, 0x7B, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (packet->objectType == 7) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory && !actor->Slot50()) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    if (g_session.GetSceneUnknown004C() && packet->objectType == 7 && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (actor->unknown_02E4) {
      auto member = this->unknown_01B4.find(actor->render.accountId);
      if (member != this->unknown_01B4.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->unknown_01BC.find(actor->render.accountId);
      if (member != this->unknown_01BC.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
  } else {
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job) &&
        packet->objectType != 9) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && packet->objectType != 0) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->unknown_0314 = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 0x7A, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
    actor->unknown_0210 = packet->level;
    if (g_session.GetSceneUnknown004C() && packet->objectType == 7 && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->unknown_01B4.find(entry.aid) != this->unknown_01B4.end() || this->unknown_01BC.find(entry.aid) != this->unknown_01BC.end()) {
      actor->SetUnknown02E4(1);
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorMoveEntryOld0Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ short effectState;
  /* 000E */ short job;
  /* 0010 */ short head;
  /* 0012 */ short weapon;
  /* 0014 */ short accessory;
  /* 0016 */ std::uint32_t moveStartTime;
  /* 001A */ short shield;
  /* 001C */ short accessory2;
  /* 001E */ short accessory3;
  /* 0020 */ short headPalette;
  /* 0022 */ short bodyPalette;
  /* 0024 */ short headDir;
  /* 0026 */ int guildId;
  /* 002A */ short guildVersion;
  /* 002C */ short honor;
  /* 002E */ short virtue;
  /* 0030 */ unsigned char pkMode;
  /* 0031 */ unsigned char sex;
  /* 0032 */ unsigned char moveData[6];
  /* 0038 */ unsigned char xSize;
  /* 0039 */ unsigned char ySize;
  /* 003A */ short level;
};
#pragma pack(pop)

static_assert(sizeof(ActorMoveEntryOld0Packet) == 0x3C);

// OFFSET: 0x00D35630
// FUNCTION: fn_21757
void CGameMode::ReceiveActorMoveEntryOld0(const unsigned char* data) {
  const ActorMoveEntryOld0Packet* packet = reinterpret_cast<const ActorMoveEntryOld0Packet*>(data);
  ActorMoveEntry entry;
  entry.body = 0;
  entry.aid = packet->aid;
  entry.job = packet->job;
  entry.moveStartTime = packet->moveStartTime;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.shield = packet->weapon >> 31;
  entry.accessory = packet->accessory;
  entry.healthState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.effectState = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.unknown_004C = packet->shield;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDir = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  DecodeSrcDst(packet->moveData, &entry.srcX, &entry.srcY, &entry.dstX, &entry.dstY, &entry.subX, &entry.subY);
  g_session.SyncServerTime(entry.moveStartTime);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    actor->SendMsg(nullptr, 5, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > 45 && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned int>(actorJob - 20021) <= 11978) {
      const std::uint32_t emblem = (packet->accessory2 << 16) | packet->accessory3;
      if (emblem > 0) {
        actor->Slot49(emblem, packet->shield);
      }
    }
    if (entry.head == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory && !actor->Slot50()) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
  } else {
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job)) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 6, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > 45 && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned int>(actorJob - 20021) <= 11978) {
      const std::uint32_t emblem = (packet->accessory2 << 16) | packet->accessory3;
      if (emblem > 0 && !actor->Slot50()) {
        actor->Slot49(emblem, packet->shield);
      }
    }
  }

  if (g_session.GetSceneUnknown004C() && entry.head != 0x2E && actor->unknown_0188 == 0 &&
      (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorStandEntryLegacyPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ int weapon;
  /* 001B */ int shield;
  /* 001F */ short accessory;
  /* 0021 */ short accessory2;
  /* 0023 */ short accessory3;
  /* 0025 */ short headPalette;
  /* 0027 */ short bodyPalette;
  /* 0029 */ short headDir;
  /* 002B */ short robe;
  /* 002D */ int guildId;
  /* 0031 */ short guildVersion;
  /* 0033 */ short honor;
  /* 0035 */ int virtue;
  /* 0039 */ unsigned char pkMode;
  /* 003A */ unsigned char sex;
  /* 003B */ unsigned char posDir[3];
  /* 003E */ unsigned char xSize;
  /* 003F */ unsigned char ySize;
  /* 0040 */ short level;
  /* 0042 */ short font;
  /* 0044 */ int maxHp;
  /* 0048 */ int hp;
  /* 004C */ unsigned char isBoss;
};

struct ActorStateEntryLegacyPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ int weapon;
  /* 001B */ int shield;
  /* 001F */ short accessory;
  /* 0021 */ short accessory2;
  /* 0023 */ short accessory3;
  /* 0025 */ short headPalette;
  /* 0027 */ short bodyPalette;
  /* 0029 */ short headDir;
  /* 002B */ short robe;
  /* 002D */ int guildId;
  /* 0031 */ short guildVersion;
  /* 0033 */ short honor;
  /* 0035 */ int virtue;
  /* 0039 */ unsigned char pkMode;
  /* 003A */ unsigned char sex;
  /* 003B */ unsigned char posDir[3];
  /* 003E */ unsigned char xSize;
  /* 003F */ unsigned char ySize;
  /* 0040 */ unsigned char state;
  /* 0041 */ short level;
  /* 0043 */ short font;
  /* 0045 */ int maxHp;
  /* 0049 */ int hp;
  /* 004D */ unsigned char isBoss;
};

struct ActorStandEntryNamedPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ std::uint32_t gid;
  /* 000D */ short speed;
  /* 000F */ short bodyState;
  /* 0011 */ short healthState;
  /* 0013 */ int effectState;
  /* 0017 */ short job;
  /* 0019 */ short head;
  /* 001B */ int weapon;
  /* 001F */ int shield;
  /* 0023 */ short accessory;
  /* 0025 */ short accessory2;
  /* 0027 */ short accessory3;
  /* 0029 */ short headPalette;
  /* 002B */ short bodyPalette;
  /* 002D */ short headDir;
  /* 002F */ short robe;
  /* 0031 */ int guildId;
  /* 0035 */ short guildVersion;
  /* 0037 */ short honor;
  /* 0039 */ int virtue;
  /* 003D */ unsigned char pkMode;
  /* 003E */ unsigned char sex;
  /* 003F */ unsigned char posDir[3];
  /* 0042 */ unsigned char xSize;
  /* 0043 */ unsigned char ySize;
  /* 0044 */ short level;
  /* 0046 */ short font;
  /* 0048 */ int maxHp;
  /* 004C */ int hp;
  /* 0050 */ unsigned char isBoss;
};

struct ActorStandEntryNamedBodyPacket : ActorStandEntryNamedPacket {
  /* 0051 */ short body;
};

struct ActorStateEntryNamedPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ std::uint32_t gid;
  /* 000D */ short speed;
  /* 000F */ short bodyState;
  /* 0011 */ short healthState;
  /* 0013 */ int effectState;
  /* 0017 */ short job;
  /* 0019 */ short head;
  /* 001B */ int weapon;
  /* 001F */ int shield;
  /* 0023 */ short accessory;
  /* 0025 */ short accessory2;
  /* 0027 */ short accessory3;
  /* 0029 */ short headPalette;
  /* 002B */ short bodyPalette;
  /* 002D */ short headDir;
  /* 002F */ short robe;
  /* 0031 */ int guildId;
  /* 0035 */ short guildVersion;
  /* 0037 */ short honor;
  /* 0039 */ int virtue;
  /* 003D */ unsigned char pkMode;
  /* 003E */ unsigned char sex;
  /* 003F */ unsigned char posDir[3];
  /* 0042 */ unsigned char xSize;
  /* 0043 */ unsigned char ySize;
  /* 0044 */ unsigned char state;
  /* 0045 */ short level;
  /* 0047 */ short font;
  /* 0049 */ int maxHp;
  /* 004D */ int hp;
  /* 0051 */ unsigned char isBoss;
};

struct ActorStateEntryNamedBodyPacket : ActorStateEntryNamedPacket {
  /* 0052 */ short body;
};
#pragma pack(pop)

static_assert(sizeof(ActorStandEntryLegacyPacket) == 0x4D);
static_assert(sizeof(ActorStateEntryLegacyPacket) == 0x4E);
static_assert(sizeof(ActorStandEntryNamedPacket) == 0x51);
static_assert(sizeof(ActorStandEntryNamedBodyPacket) == 0x53);
static_assert(sizeof(ActorStateEntryNamedPacket) == 0x52);
static_assert(sizeof(ActorStateEntryNamedBodyPacket) == 0x54);

// OFFSET: 0x00D62540
// FUNCTION: fn_21986
void CGameMode::ReceiveActorStandEntryNamed(const unsigned char* data) {
  const ActorStandEntryNamedPacket* packet = reinterpret_cast<const ActorStandEntryNamedPacket*>(data);
  const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStandEntryNamedPacket));
  const std::string name(nameBegin, reinterpret_cast<const char*>(data + packet->packetLength));
  this->SpawnStandingActor(
    packet->objectType, packet->aid, packet->gid, packet->speed, packet->bodyState, packet->healthState, packet->effectState, packet->job, packet->head,
    packet->weapon, packet->shield, packet->accessory, packet->accessory2, packet->accessory3, packet->headPalette, packet->bodyPalette, packet->headDir,
    packet->robe, packet->guildId, packet->guildVersion, packet->honor, packet->virtue, packet->pkMode, packet->sex, packet->posDir, packet->xSize,
    packet->ySize, packet->level, packet->font, packet->maxHp, packet->hp, packet->isBoss, name, 0
  );
}

// OFFSET: 0x00D62850
// FUNCTION: fn_21987
void CGameMode::ReceiveActorStandEntryNamedBody(const unsigned char* data) {
  const ActorStandEntryNamedBodyPacket* packet = reinterpret_cast<const ActorStandEntryNamedBodyPacket*>(data);
  const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStandEntryNamedBodyPacket));
  const std::string name(nameBegin, reinterpret_cast<const char*>(data + packet->packetLength));
  this->SpawnStandingActor(
    packet->objectType, packet->aid, packet->gid, packet->speed, packet->bodyState, packet->healthState, packet->effectState, packet->job, packet->head,
    packet->weapon, packet->shield, packet->accessory, packet->accessory2, packet->accessory3, packet->headPalette, packet->bodyPalette, packet->headDir,
    packet->robe, packet->guildId, packet->guildVersion, packet->honor, packet->virtue, packet->pkMode, packet->sex, packet->posDir, packet->xSize,
    packet->ySize, packet->level, packet->font, packet->maxHp, packet->hp, packet->isBoss, name, packet->body
  );
}

// OFFSET: 0x00D62B70
// FUNCTION: fn_21988
void CGameMode::ReceiveActorStandEntryLegacy(const unsigned char* data) {
  const ActorStandEntryLegacyPacket* packet = reinterpret_cast<const ActorStandEntryLegacyPacket*>(data);
  ActorStandEntry entry;
  const int nameLength = packet->packetLength - static_cast<int>(sizeof(ActorStandEntryLegacyPacket));
  if (nameLength > 0) {
    const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStandEntryLegacyPacket));
    this->nameCard.SetName(packet->aid, std::string(nameBegin, nameBegin + nameLength));
  }

  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_002C = packet->shield;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  if (packet->objectType == 0) {
    entry.state = 0;
  } else {
    entry.unknown_0048 = entry.guildVersion;
  }
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (packet->objectType == 0) {
      actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  } else {
    CGameActor* created;
    if (packet->objectType == 0) {
      created = new CPc;
    } else {
      created = this->NewActor(packet->job);
    }
    if (!created) {
      return;
    }

    created->unknown_0314 = packet->objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (packet->objectType == 0) {
      if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      entry.unknown_0008 = 0;
      actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (packet->isBoss && actor->unknown_0304 == 0) {
    actor->unknown_0304 = packet->isBoss;
    bossFlag = 1;
  }
  actor->unknown_0210 = packet->level;
  *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
  if (packet->objectType == 7) {
    if (g_session.IsPetAid(packet->aid)) {
      g_session.unknown_1038 = entry.unknown_0030;
    }
    if (entry.unknown_0030 && !actor->Slot50()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
    }
  } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
    g_session.unknown_5614 = 1;
  } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
    g_session.unknown_56B0 = 1;
  } else if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  switch (actor->unknown_0314) {
  case 0:
  case 5:
  case 8:
  case 9:
  case 10: {
    const std::int64_t hp = packet->hp;
    const std::int64_t maxHp = packet->maxHp;
    actor->SendMsg(
      nullptr, 0x91, 0, static_cast<int>(hp), static_cast<int>(hp >> 32), static_cast<int>(maxHp), static_cast<int>(maxHp >> 32),
      static_cast<int>(bossFlag), static_cast<int>(bossFlag >> 32), 0, 0, 0, 0, 0, 0
    );
    break;
  }
  case 13:
  case 14:
    actor->SendMsg(nullptr, 0x22, 0, packet->aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }

  if (packet->objectType != 0) {
    const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (guildId > 0 && !actor->Slot50()) {
      actor->Slot49(guildId, packet->guildVersion);
    }
  }
  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorStandEntryNamedOldPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ int weapon;
  /* 001B */ int shield;
  /* 001F */ short accessory;
  /* 0021 */ short accessory2;
  /* 0023 */ short accessory3;
  /* 0025 */ short headPalette;
  /* 0027 */ short bodyPalette;
  /* 0029 */ short headDir;
  /* 002B */ short robe;
  /* 002D */ int guildId;
  /* 0031 */ short guildVersion;
  /* 0033 */ short honor;
  /* 0035 */ int virtue;
  /* 0039 */ unsigned char pkMode;
  /* 003A */ unsigned char sex;
  /* 003B */ unsigned char posDir[3];
  /* 003E */ unsigned char xSize;
  /* 003F */ unsigned char ySize;
  /* 0040 */ short level;
  /* 0042 */ short font;
};
#pragma pack(pop)

static_assert(sizeof(ActorStandEntryNamedOldPacket) == 0x44);

// OFFSET: 0x00D35AF0
// FUNCTION: fn_21758
void CGameMode::ReceiveActorStandEntryNamedOld(const unsigned char* data) {
  const ActorStandEntryNamedOldPacket* packet = reinterpret_cast<const ActorStandEntryNamedOldPacket*>(data);
  ActorStandEntry entry;
  const int nameLength = packet->packetLength - static_cast<int>(sizeof(ActorStandEntryNamedOldPacket));
  if (nameLength > 0) {
    const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStandEntryNamedOldPacket));
    this->nameCard.SetName(packet->aid, std::string(nameBegin, nameBegin + nameLength));
  }

  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_002C = packet->shield;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  if (packet->objectType == 0) {
    entry.state = 0;
  } else {
    entry.unknown_0048 = entry.guildVersion;
  }
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  int created = 0;
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (!actor) {
    created = 1;
    if (packet->objectType == 0) {
      actor = new CPc;
    } else {
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }
    actor->unknown_0314 = packet->objectType;
  }

  actor->unknown_0210 = packet->level;
  if (packet->objectType == 0) {
    *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
    if (created) {
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
      this->scene->objects.push_back(actor);
      this->scene->actors.push_back(actor);
    } else {
      actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    if (packet->objectType == 7) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
      g_session.unknown_5614 = 1;
    } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
      g_session.unknown_56B0 = 1;
    }

    if (created) {
      this->scene->objects.push_back(actor);
      this->scene->actors.push_back(actor);
      actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_session.IsGuardianJob(packet->job) && g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }

    const int guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (created) {
      if (guildId != 0) {
        actor->Slot49(guildId, packet->guildVersion);
      }
    } else if (guildId != 0 && !actor->Slot50()) {
      actor->Slot49(guildId, packet->guildVersion);
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorStandEntryOldPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ short effectState;
  /* 000E */ short job;
  /* 0010 */ short head;
  /* 0012 */ int weapon;
  /* 0016 */ int shield;
  /* 001A */ short accessory;
  /* 001C */ short accessory2;
  /* 001E */ short accessory3;
  /* 0020 */ short headPalette;
  /* 0022 */ short bodyPalette;
  /* 0024 */ short headDir;
  /* 0026 */ int guildId;
  /* 002A */ short guildVersion;
  /* 002C */ short honor;
  /* 002E */ short virtue;
  /* 0030 */ unsigned char pkMode;
  /* 0031 */ unsigned char sex;
  /* 0032 */ unsigned char posDir[3];
  /* 0035 */ unsigned char xSize;
  /* 0036 */ unsigned char ySize;
  /* 0037 */ short level;
};
#pragma pack(pop)

static_assert(sizeof(ActorStandEntryOldPacket) == 0x39);

// OFFSET: 0x00D35FD0
// FUNCTION: fn_21759
void CGameMode::ReceiveActorStandEntryOld(const unsigned char* data) {
  const ActorStandEntryOldPacket* packet = reinterpret_cast<const ActorStandEntryOldPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.unknown_002C = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  entry.state = 0;

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    if (entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (isPc && !IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
      actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
      actor->SetUnknown02E4(1);
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorStandEntryOld2Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ int effectState;
  /* 0010 */ short job;
  /* 0012 */ short head;
  /* 0014 */ int weapon;
  /* 0018 */ int shield;
  /* 001C */ short accessory;
  /* 001E */ short accessory2;
  /* 0020 */ short accessory3;
  /* 0022 */ short headPalette;
  /* 0024 */ short bodyPalette;
  /* 0026 */ short headDir;
  /* 0028 */ int guildId;
  /* 002C */ short guildVersion;
  /* 002E */ short honor;
  /* 0030 */ int virtue;
  /* 0034 */ unsigned char pkMode;
  /* 0035 */ unsigned char sex;
  /* 0036 */ unsigned char posDir[3];
  /* 0039 */ unsigned char xSize;
  /* 003A */ unsigned char ySize;
  /* 003B */ short level;
};
#pragma pack(pop)

static_assert(sizeof(ActorStandEntryOld2Packet) == 0x3D);

// OFFSET: 0x00D364C0
// FUNCTION: fn_21760
void CGameMode::ReceiveActorStandEntryOld2(const unsigned char* data) {
  const ActorStandEntryOld2Packet* packet = reinterpret_cast<const ActorStandEntryOld2Packet*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.unknown_002C = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  entry.state = 0;

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    if (entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->unknown_0210 = packet->level;
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->unknown_0210 = packet->level;
    if (isPc && !IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
      actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
      actor->SetUnknown02E4(1);
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorStandEntryOld3Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ int effectState;
  /* 0010 */ short job;
  /* 0012 */ short head;
  /* 0014 */ int weapon;
  /* 0018 */ int shield;
  /* 001C */ short accessory;
  /* 001E */ short accessory2;
  /* 0020 */ short accessory3;
  /* 0022 */ short headPalette;
  /* 0024 */ short bodyPalette;
  /* 0026 */ short headDir;
  /* 0028 */ int guildId;
  /* 002C */ short guildVersion;
  /* 002E */ short honor;
  /* 0030 */ int virtue;
  /* 0034 */ unsigned char pkMode;
  /* 0035 */ unsigned char sex;
  /* 0036 */ unsigned char posDir[3];
  /* 0039 */ unsigned char xSize;
  /* 003A */ unsigned char ySize;
  /* 003B */ short level;
  /* 003D */ short font;
};
#pragma pack(pop)

static_assert(sizeof(ActorStandEntryOld3Packet) == 0x3F);

// OFFSET: 0x00D369C0
// FUNCTION: fn_21761
void CGameMode::ReceiveActorStandEntryOld3(const unsigned char* data) {
  const ActorStandEntryOld3Packet* packet = reinterpret_cast<const ActorStandEntryOld3Packet*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.unknown_002C = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  entry.state = 0;

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
    actor->unknown_0210 = packet->level;
    if (entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    actor->unknown_0210 = packet->level;
    if (isPc) {
      *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
      if (!IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
        actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
      actor->SetUnknown02E4(1);
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}
#pragma pack(push, 1)
struct ActorStandEntryOld0Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ short effectState;
  /* 000E */ short job;
  /* 0010 */ short head;
  /* 0012 */ short weapon;
  /* 0014 */ short accessory;
  /* 0016 */ short shield;
  /* 0018 */ short accessory2;
  /* 001A */ short accessory3;
  /* 001C */ short headPalette;
  /* 001E */ short bodyPalette;
  /* 0020 */ short headDir;
  /* 0022 */ int guildId;
  /* 0026 */ short guildVersion;
  /* 0028 */ short honor;
  /* 002A */ short virtue;
  /* 002C */ unsigned char pkMode;
  /* 002D */ unsigned char sex;
  /* 002E */ unsigned char posDir[3];
  /* 0031 */ unsigned char xSize;
  /* 0032 */ unsigned char ySize;
  /* 0033 */ short level;
};
#pragma pack(pop)

static_assert(sizeof(ActorStandEntryOld0Packet) == 0x35);

// OFFSET: 0x00D36ED0
// FUNCTION: fn_21762
void CGameMode::ReceiveActorStandEntryOld0(const unsigned char* data) {
  const ActorStandEntryOld0Packet* packet = reinterpret_cast<const ActorStandEntryOld0Packet*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_002C = packet->weapon >> 31;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_0048 = packet->shield;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  entry.state = 0;

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > 45 && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned int>(actorJob - 20021) <= 11978) {
      const std::uint32_t emblem = (packet->accessory2 << 16) | packet->accessory3;
      if (emblem > 0 && !actor->Slot50()) {
        actor->Slot49(emblem, packet->shield);
      }
    }
    if (entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob >= 45 && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned int>(actorJob - 20021) <= 11978) {
      const std::uint32_t emblem = (packet->accessory2 << 16) | packet->accessory3;
      if (emblem > 0) {
        actor->Slot49(emblem, packet->shield);
      }
    }
    if (isPc && !IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
      actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
      (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorStateEntryNamedOldPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::int16_t packetLength;
  /* 0004 */ unsigned char objectType;
  /* 0005 */ std::uint32_t aid;
  /* 0009 */ short speed;
  /* 000B */ short bodyState;
  /* 000D */ short healthState;
  /* 000F */ int effectState;
  /* 0013 */ short job;
  /* 0015 */ short head;
  /* 0017 */ int weapon;
  /* 001B */ int shield;
  /* 001F */ short accessory;
  /* 0021 */ short accessory2;
  /* 0023 */ short accessory3;
  /* 0025 */ short headPalette;
  /* 0027 */ short bodyPalette;
  /* 0029 */ short headDir;
  /* 002B */ short robe;
  /* 002D */ int guildId;
  /* 0031 */ short guildVersion;
  /* 0033 */ short honor;
  /* 0035 */ int virtue;
  /* 0039 */ unsigned char pkMode;
  /* 003A */ unsigned char sex;
  /* 003B */ unsigned char posDir[3];
  /* 003E */ unsigned char xSize;
  /* 003F */ unsigned char ySize;
  /* 0040 */ unsigned char state;
  /* 0041 */ short level;
  /* 0043 */ short font;
};
#pragma pack(pop)

static_assert(sizeof(ActorStateEntryNamedOldPacket) == 0x45);

// OFFSET: 0x00D37A10
// FUNCTION: fn_21765
void CGameMode::ReceiveActorStateEntryNamedOld(const unsigned char* data) {
  const ActorStateEntryNamedOldPacket* packet = reinterpret_cast<const ActorStateEntryNamedOldPacket*>(data);
  ActorStandEntry entry;
  const int nameLength = packet->packetLength - static_cast<int>(sizeof(ActorStateEntryNamedOldPacket));
  if (nameLength > 0) {
    const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStateEntryNamedOldPacket));
    this->nameCard.SetName(packet->aid, std::string(nameBegin, nameBegin + nameLength));
  }

  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_002C = packet->shield;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  if (packet->objectType != 0) {
    entry.unknown_0048 = entry.guildVersion;
  }
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  int created = 0;
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (!actor) {
    created = 1;
    if (packet->objectType == 0) {
      actor = new CPc;
    } else {
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }
    actor->unknown_0314 = packet->objectType;
  }

  if (packet->objectType == 0) {
    *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
    actor->unknown_0210 = packet->level;
    if (created) {
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
    } else {
      actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    if (packet->objectType == 7) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
      g_session.unknown_5614 = 1;
    } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
      g_session.unknown_56B0 = 1;
    }

    if (created) {
      actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (g_session.IsGuardianJob(packet->job) && g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }

    const int guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (created) {
      if (guildId != 0) {
        actor->Slot49(guildId, packet->guildVersion);
      }
    } else if (guildId != 0 && !actor->Slot50()) {
      actor->Slot49(guildId, packet->guildVersion);
    }
  }

  if (created) {
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
  }
  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

#pragma pack(push, 1)
struct ActorStateEntryOldPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ short speed;
  /* 0008 */ short bodyState;
  /* 000A */ short healthState;
  /* 000C */ short effectState;
  /* 000E */ short job;
  /* 0010 */ short head;
  /* 0012 */ int weapon;
  /* 0016 */ int shield;
  /* 001A */ short accessory;
  /* 001C */ short accessory2;
  /* 001E */ short accessory3;
  /* 0020 */ short headPalette;
  /* 0022 */ short bodyPalette;
  /* 0024 */ short headDir;
  /* 0026 */ int guildId;
  /* 002A */ short guildVersion;
  /* 002C */ short honor;
  /* 002E */ short virtue;
  /* 0030 */ unsigned char pkMode;
  /* 0031 */ unsigned char sex;
  /* 0032 */ unsigned char posDir[3];
  /* 0035 */ unsigned char xSize;
  /* 0036 */ unsigned char ySize;
  /* 0037 */ unsigned char state;
  /* 0038 */ short level;
};
#pragma pack(pop)

static_assert(sizeof(ActorStateEntryOldPacket) == 0x3A);

// OFFSET: 0x00D37EA0
// FUNCTION: fn_21766
void CGameMode::ReceiveActorStateEntryOld(const unsigned char* data) {
  const ActorStateEntryOldPacket* packet = reinterpret_cast<const ActorStateEntryOldPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.unknown_002C = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    if (entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    if ((job < 45 || job > 999) && (job < 1001 || job > 3998) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsSkillIdInRange6001(job)) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.unknown_5614 = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.unknown_56B0 = 1;
      }
      actor = this->NewActor(packet->job);
      entry.unknown_0008 = 0;
    }

    if (actor && entry.unknown_0024 == 0x2E) {
      if (g_session.unknown_1010 == packet->aid) {
        g_session.unknown_1038 = entry.unknown_0030;
      }
      if (entry.unknown_0030) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (g_session.GetSceneUnknown004C() && entry.unknown_0024 != 0x2E && actor->unknown_0188 == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
    }
  }

  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D630F0
// FUNCTION: fn_21989
void CGameMode::SpawnStandingActor(
  unsigned char objectType, std::uint32_t aid, std::uint32_t gid, short speed, short bodyState, short healthState, int effectState, short job, short head,
  int weapon, int shield, short accessory, short accessory2, short accessory3, short headPalette, short bodyPalette, short headDir, short robe,
  int guildId, short guildVersion, short honor, int virtue, unsigned char pkMode, unsigned char sex, const unsigned char* posDir, unsigned char xSize,
  unsigned char ySize, short level, short font, int maxHp, int hp, unsigned char isBoss, std::string name, short body
) {
  ActorStandEntry entry;
  if (objectType == 0 && gid != 0) {
    g_playerNameDictionary->Set(gid, name);
  }
  if (name.size() != 0) {
    this->nameCard.SetName(aid, name);
  }

  entry.aid = aid;
  entry.job = job;
  entry.unknown_0008 = this->SubD07740(entry.job, sex);
  entry.speed = speed;
  entry.unknown_001C = xSize;
  entry.unknown_0020 = ySize;
  entry.unknown_0024 = head;
  entry.unknown_0028 = weapon;
  entry.unknown_0030 = accessory;
  entry.effectState = healthState;
  entry.bodyState = bodyState;
  entry.stateFlags = effectState;
  entry.unknown_0044 = pkMode;
  entry.unknown_004C = accessory2;
  entry.unknown_0050 = accessory3;
  entry.unknown_0054 = headPalette;
  entry.unknown_0058 = bodyPalette;
  entry.unknown_005C = headDir;
  entry.guildId = guildId;
  entry.unknown_0068 = honor;
  entry.unknown_006C = virtue;
  entry.robe = robe;
  entry.unknown_002C = shield;
  entry.guildVersion = guildVersion;
  if (objectType == 0) {
    entry.state = 0;
  } else {
    entry.unknown_0048 = entry.guildVersion;
  }
  entry.unknown_0074 = gid;
  entry.unknown_0078 = body;
  entry.cellX = (posDir[0] << 2) | (posDir[1] >> 6);
  entry.direction = posDir[2] & 0x0F;
  entry.cellY = ((posDir[1] & 0x3F) << 4) | (posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (objectType == 0) {
      actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  } else {
    CGameActor* created;
    if (objectType == 0) {
      created = new CPc;
    } else {
      created = this->NewActor(job);
    }
    if (!created) {
      return;
    }

    created->unknown_0314 = objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (objectType == 0) {
      if (this->unknown_01B4.find(aid) != this->unknown_01B4.end() || this->unknown_01BC.find(aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      if (!((objectType == 1 || objectType == 6 || objectType == 5) && IsPlayerJob(job))) {
        entry.unknown_0008 = 0;
      }
      actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (isBoss && actor->unknown_0304 == 0) {
    actor->unknown_0304 = isBoss;
    bossFlag = 1;
  }
  actor->unknown_0210 = level;
  *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = font;
  if (objectType == 7) {
    if (g_session.IsPetAid(aid)) {
      g_session.unknown_1038 = entry.unknown_0030;
    }
    if (entry.unknown_0030 && !actor->Slot50()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
    }
  } else if (objectType == 8 && g_session.IsHomunAid(aid)) {
    g_session.unknown_5614 = 1;
  } else if (objectType == 9 && g_session.IsMercenaryAid(aid)) {
    g_session.unknown_56B0 = 1;
  } else if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, guildId, guildVersion);
  }

  switch (actor->unknown_0314) {
  case 0:
  case 5:
  case 8:
  case 9:
  case 10: {
    const std::int64_t currentHp = hp;
    const std::int64_t currentMaxHp = maxHp;
    actor->SendMsg(
      nullptr, 0x91, 0, static_cast<int>(currentHp), static_cast<int>(currentHp >> 32), static_cast<int>(currentMaxHp),
      static_cast<int>(currentMaxHp >> 32), static_cast<int>(bossFlag), static_cast<int>(bossFlag >> 32), 0, 0, 0, 0, 0, 0
    );
    break;
  }
  case 13:
  case 14:
    actor->SendMsg(nullptr, 0x22, 0, gid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }

  if (objectType != 0) {
    const std::uint32_t emblemGuildId = (accessory2 << 16) | accessory3;
    if (emblemGuildId > 0 && !actor->Slot50()) {
      actor->Slot49(emblemGuildId, guildVersion);
    }
    this->sub_D591A0(aid, effectState, level, virtue);
  } else {
    this->sub_D591A0(aid, effectState, level, virtue);
    int cell[2];
    cell[0] = entry.cellX;
    cell[1] = entry.cellY;
    this->scene->AddPcPosFromCell(aid, cell);
  }
}

// OFFSET: 0x00D63720
// FUNCTION: fn_21990
void CGameMode::ReceiveActorStateEntryNamed(const unsigned char* data) {
  const ActorStateEntryNamedPacket* packet = reinterpret_cast<const ActorStateEntryNamedPacket*>(data);
  const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStateEntryNamedPacket));
  const std::string name(nameBegin, reinterpret_cast<const char*>(data + packet->packetLength));
  this->SpawnStateActor(
    packet->objectType, packet->aid, packet->gid, packet->speed, packet->bodyState, packet->healthState, packet->effectState, packet->job, packet->head,
    packet->weapon, packet->shield, packet->accessory, packet->accessory2, packet->accessory3, packet->headPalette, packet->bodyPalette, packet->headDir,
    packet->robe, packet->guildId, packet->guildVersion, packet->honor, packet->virtue, packet->pkMode, packet->sex, packet->posDir, packet->xSize,
    packet->ySize, packet->state, packet->level, packet->font, packet->maxHp, packet->hp, packet->isBoss, name, 0
  );
}

// OFFSET: 0x00D63A40
// FUNCTION: fn_21991
void CGameMode::ReceiveActorStateEntryNamedBody(const unsigned char* data) {
  const ActorStateEntryNamedBodyPacket* packet = reinterpret_cast<const ActorStateEntryNamedBodyPacket*>(data);
  const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStateEntryNamedBodyPacket));
  const std::string name(nameBegin, reinterpret_cast<const char*>(data + packet->packetLength));
  this->SpawnStateActor(
    packet->objectType, packet->aid, packet->gid, packet->speed, packet->bodyState, packet->healthState, packet->effectState, packet->job, packet->head,
    packet->weapon, packet->shield, packet->accessory, packet->accessory2, packet->accessory3, packet->headPalette, packet->bodyPalette, packet->headDir,
    packet->robe, packet->guildId, packet->guildVersion, packet->honor, packet->virtue, packet->pkMode, packet->sex, packet->posDir, packet->xSize,
    packet->ySize, packet->state, packet->level, packet->font, packet->maxHp, packet->hp, packet->isBoss, name, packet->body
  );
}

#pragma pack(push, 1)
struct RefineBroadcastPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t unknown_0002;
  /* 0004 */ int itemId;
  /* 0008 */ char name[24];
  /* 0020 */ int materialId;
  /* 0024 */ int refine;
  /* 0028 */ unsigned char grade;
};

struct TradeBroadcastPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t unknown_0002;
  /* 0004 */ int itemId;
  /* 0008 */ char name[24];
  /* 0020 */ char otherName[24];
};

struct ItemBroadcastPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t unknown_0002;
  /* 0004 */ int itemId;
  /* 0008 */ char name[24];
};
#pragma pack(pop)

static_assert(sizeof(RefineBroadcastPacket) == 0x29);
static_assert(sizeof(TradeBroadcastPacket) == 0x38);
static_assert(sizeof(ItemBroadcastPacket) == 0x20);

// OFFSET: 0x00D23B70
// FUNCTION: fn_21663
void CGameMode::ReceiveRefineBroadcast(const unsigned char* data) {
  const RefineBroadcastPacket* packet = reinterpret_cast<const RefineBroadcastPacket*>(data);
  std::string name(packet->name, 24);
  ITEM_INFO item;
  item.SetItemId(packet->itemId);
  item.unknown_0060 = packet->refine;
  item.identified = 1;
  item.unknown_0088 = packet->grade;
  ITEM_INFO material;
  material.SetItemId(packet->materialId);
  material.identified = 1;

  std::string text = FormatOwnedString(LookupMessage(0x65C), name.c_str(), material.DisplayName(0).c_str(), item.DisplayName(1).c_str());
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    std::uint32_t color = 0x62B0FF;
    message.lines.emplace_back(text, color);
    balloon->AddMessage(0, message);
  }
}

// OFFSET: 0x00D23E20
// FUNCTION: fn_21664
void CGameMode::ReceiveTradeBroadcast(const unsigned char* data) {
  const TradeBroadcastPacket* packet = reinterpret_cast<const TradeBroadcastPacket*>(data);
  std::string name(packet->name, 24);
  ITEM_INFO item;
  item.SetItemId(packet->itemId);
  item.identified = 1;
  std::string otherName(packet->otherName, 24);

  std::string text = FormatOwnedString(LookupMessage(0x65C), name.c_str(), otherName.c_str(), item.DisplayName(1).c_str());
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    std::uint32_t color = 0x62B0FF;
    message.lines.emplace_back(text, color);
    balloon->AddMessage(0, message);
  }
}

// OFFSET: 0x00D240F0
// FUNCTION: fn_21665
void CGameMode::ReceiveItemBroadcast(const unsigned char* data) {
  const ItemBroadcastPacket* packet = reinterpret_cast<const ItemBroadcastPacket*>(data);
  std::string name(packet->name, 24);
  ITEM_INFO item;
  item.SetItemId(packet->itemId);
  item.identified = 1;

  std::string text = FormatOwnedString(LookupMessage(0x74C), name.c_str(), item.DisplayName(0).c_str());
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    message.lines.emplace_back(text, 0x62B0FF, 0);
    balloon->AddMessage(0, message);
  }
}

#pragma pack(push, 1)
struct RateInfoRecord {
  /* 0000 */ unsigned char type;
  /* 0001 */ int exp;
  /* 0005 */ int death;
  /* 0009 */ int drop;
};
#pragma pack(pop)

static_assert(sizeof(RateInfoRecord) == 0xD);

// OFFSET: 0x00D642F0
// FUNCTION: fn_21993
void CGameMode::SpawnStateActor(
  unsigned char objectType, std::uint32_t aid, std::uint32_t gid, short speed, short bodyState, short healthState, int effectState, short job, short head,
  int weapon, int shield, short accessory, short accessory2, short accessory3, short headPalette, short bodyPalette, short headDir, short robe,
  int guildId, short guildVersion, short honor, int virtue, unsigned char pkMode, unsigned char sex, const unsigned char* posDir, unsigned char xSize,
  unsigned char ySize, unsigned char state, short level, short font, int maxHp, int hp, unsigned char isBoss, std::string name, short body
) {
  ActorStandEntry entry;
  if (objectType == 0 && gid != 0) {
    g_playerNameDictionary->Set(gid, name);
  }
  if (name.size() != 0) {
    this->nameCard.SetName(aid, name);
  }

  entry.aid = aid;
  entry.job = job;
  entry.unknown_0008 = this->SubD07740(entry.job, sex);
  entry.speed = speed;
  entry.unknown_001C = xSize;
  entry.unknown_0020 = ySize;
  entry.unknown_0024 = head;
  entry.unknown_0028 = weapon;
  entry.unknown_0030 = accessory;
  entry.effectState = healthState;
  entry.bodyState = bodyState;
  entry.stateFlags = effectState;
  entry.unknown_0044 = pkMode;
  entry.unknown_004C = accessory2;
  entry.unknown_0050 = accessory3;
  entry.unknown_0054 = headPalette;
  entry.unknown_0058 = bodyPalette;
  entry.unknown_005C = headDir;
  entry.guildId = guildId;
  entry.unknown_0068 = honor;
  entry.unknown_006C = virtue;
  entry.robe = robe;
  entry.unknown_002C = shield;
  entry.guildVersion = guildVersion;
  entry.state = state;
  if (objectType != 0) {
    entry.unknown_0048 = entry.guildVersion;
  }
  entry.unknown_0074 = gid;
  entry.unknown_0078 = body;
  entry.cellX = (posDir[0] << 2) | (posDir[1] >> 6);
  entry.direction = posDir[2] & 0x0F;
  entry.cellY = ((posDir[1] & 0x3F) << 4) | (posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (objectType == 0) {
      actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  } else {
    CGameActor* created;
    if (objectType == 0) {
      created = new CPc;
    } else {
      created = this->NewActor(job);
    }
    if (!created) {
      return;
    }

    created->unknown_0314 = objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (objectType == 0) {
      if (this->unknown_01B4.find(aid) != this->unknown_01B4.end() || this->unknown_01BC.find(aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      if ((objectType == 1 || objectType == 6 || objectType == 12) && IsPlayerJob(job)) {
        entry.unknown_0078 = job;
      } else {
        entry.unknown_0008 = 0;
      }
      actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (isBoss && actor->unknown_0304 == 0) {
    actor->unknown_0304 = isBoss;
    bossFlag = 1;
  }
  actor->unknown_0210 = level;
  *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = font;
  if (objectType == 7) {
    if (g_session.IsPetAid(aid)) {
      g_session.unknown_1038 = entry.unknown_0030;
    }
    if (entry.unknown_0030 && !actor->Slot50()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
    }
  } else if (objectType == 8 && g_session.IsHomunAid(aid)) {
    g_session.unknown_5614 = 1;
  } else if (objectType == 9 && g_session.IsMercenaryAid(aid)) {
    g_session.unknown_56B0 = 1;
  } else if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, guildId, guildVersion);
  }

  switch (actor->unknown_0314) {
  case 0:
  case 5:
  case 8:
  case 9:
  case 10: {
    const std::int64_t currentHp = hp;
    const std::int64_t currentMaxHp = maxHp;
    actor->SendMsg(
      nullptr, 0x91, 0, static_cast<int>(currentHp), static_cast<int>(currentHp >> 32), static_cast<int>(currentMaxHp),
      static_cast<int>(currentMaxHp >> 32), static_cast<int>(bossFlag), static_cast<int>(bossFlag >> 32), 0, 0, 0, 0, 0, 0
    );
    break;
  }
  case 13:
  case 14:
    actor->SendMsg(nullptr, 0x22, 0, gid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }

  if (objectType != 0 && job == 0x2D2) {
    actor->Slot49(actor->Slot50(), guildVersion);
    this->sub_D591A0(aid, effectState, level, virtue);
  } else {
    this->sub_D591A0(aid, effectState, level, virtue);
    if (objectType == 0) {
      int cell[2];
      cell[0] = entry.cellX;
      cell[1] = entry.cellY;
      this->scene->AddPcPosFromCell(aid, cell);
    }
  }
}

// OFFSET: 0x00D78C70
// FUNCTION: fn_22163
void CGameMode::ReceiveRateInfo(const unsigned char* packet) {
  g_session.m_skillBundle.extraIds.clear();
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 16) / sizeof(RateInfoRecord);
  if (count <= 0) {
    return;
  }

  RateInfoRecord basic = {};
  RateInfoRecord premium = {};
  RateInfoRecord server = {};
  const RateInfoRecord* record = reinterpret_cast<const RateInfoRecord*>(packet + 16);
  for (int i = count; i != 0; --i, ++record) {
    const RateInfoRecord entry = *record;
    if (entry.type == 0) {
      basic = entry;
    } else if (entry.type != 1) {
      if (entry.type == 2) {
        premium = entry;
      } else if (entry.type == 3) {
        server = entry;
      }
    }
  }

  std::string expLine = LookupMessage(1933);
  std::string dropLine = LookupMessage(1934);
  std::string deathLine = LookupMessage(1935);
  expLine += FormatOwnedString(
    LookupMessage(3032), static_cast<float>(*reinterpret_cast<const int*>(packet + 4)) * 0.001f, static_cast<float>(basic.exp) * 0.001f,
    static_cast<float>(server.exp) * 0.001f, g_session.m_serverName.c_str(), static_cast<float>(premium.exp) * 0.001f
  );
  dropLine += FormatOwnedString(
    LookupMessage(3032), static_cast<float>(*reinterpret_cast<const int*>(packet + 12)) * 0.001f, static_cast<float>(basic.drop) * 0.001f,
    static_cast<float>(server.drop) * 0.001f, g_session.m_serverName.c_str(), static_cast<float>(premium.drop) * 0.001f
  );
  deathLine += FormatOwnedString(
    LookupMessage(3032), static_cast<float>(*reinterpret_cast<const int*>(packet + 8)) * 0.001f, static_cast<float>(basic.death) * 0.001f,
    static_cast<float>(server.death) * 0.001f, g_session.m_serverName.c_str(), static_cast<float>(premium.death) * 0.001f
  );

  std::uint32_t color = 0xFFFF;
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    message.lines.emplace_back(expLine, color);
    message.lines.emplace_back(dropLine, color);
    message.lines.emplace_back(deathLine, color);
    balloon->AddMessage(0, message);
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("=================================================================="), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(expLine.c_str()), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(dropLine.c_str()), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(deathLine.c_str()), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("=================================================================="), 0x62B0FF, 0, 0, 0);
}

// OFFSET: 0x00D21EB0
// FUNCTION: fn_21658
void CGameMode::ReceiveColorBroadcast(const unsigned char* data) {
  char text[256];
  std::memcpy(text, data + 16, *reinterpret_cast<const std::uint16_t*>(data + 2) - 16);
  const int length = static_cast<int>(std::strlen(text));
  if (g_forbiddenWords.Contains(text)) {
    std::string masked;
    g_forbiddenWords.Mask(text, &masked);
  }

  std::string message(text);
  const int hashPosition = static_cast<int>(message.find('#'));
  const int colonPosition = static_cast<int>(message.find(':'));
  if (hashPosition > 0 && hashPosition < length) {
    char part[256];
    std::memcpy(part, text, hashPosition);
    part[hashPosition] = '\0';
    message = part;
    if (colonPosition > 0 && colonPosition < length) {
      std::memcpy(part, text + colonPosition, length - colonPosition + 1);
      message += part;
    }
  }

  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    const int color = (data[4] << 16) | (data[5] << 8) | data[6];
    BroadcastBalloonMessage balloonMessage{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    balloonMessage.start = std::chrono::system_clock::now();
    balloonMessage.fontHeight = *reinterpret_cast<const short*>(data + 10);
    message = static_cast<UINewChatWnd*>(g_windowMgr.window_01C8)->ConvertChat(message);
    balloonMessage.lines.emplace_back(message, color);
    balloon->AddMessage(0, balloonMessage);
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), color, 0x19, 0, 0);
    g_windowMgr.SendNotice(0x13, reinterpret_cast<std::intptr_t>(message.c_str()), color, 0x19, 0, 0);
  }
}

// OFFSET: 0x00D223F0
// FUNCTION: fn_21659
void CGameMode::ReceiveTaggedBroadcast(const unsigned char* data) {
  char senderName[25] = {};
  char text[0x400];
  char head[0x400];
  char body[0x400];
  std::memset(text, 0, sizeof(text));
  std::memset(head, 0, sizeof(head));
  std::memset(body, 0, sizeof(body));

  char tag[5] = {};
  unsigned int size = *reinterpret_cast<const std::uint16_t*>(data + 2) - 4;
  if (size >= 0x3FF) {
    size = 0x3FF;
  }
  std::memcpy(text, data + 4, size);
  text[size] = '\0';
  const int length = static_cast<int>(std::strlen(text));

  std::string message(text);
  std::uint32_t color = 0xFFFF;
  std::memcpy(tag, text, std::min<std::size_t>(std::strlen(text), 4));
  tag[4] = '\0';
  _strlwr(tag);

  int fontType = 0x19;
  const int hashPosition = static_cast<int>(message.find('#'));
  const int colonPosition = static_cast<int>(message.find(':'));
  bool systemMessage = false;
  unsigned long rgb = 0;
  if (std::strcmp(tag, "tool") == 0) {
    std::memcpy(head, text + 4, 6);
    head[6] = '\0';
    std::sscanf(head, "%lx", &rgb);
    color = ((rgb & 0xFF) << 16) | (rgb & 0xFF00) | ((rgb >> 16) & 0xFF);
    std::memcpy(body, text + 10, length - 10);
    body[length - 10] = '\0';
    message = body;
  } else if (std::strcmp(tag, "blue") == 0 || std::strcmp(tag, "블루") == 0) {
    std::memcpy(head, text + 4, length - 4);
    head[length - 4] = '\0';
    message = head;
    color = 0xFFFF00;
  } else if (std::strcmp(tag, "micc") == 0) {
    std::memcpy(senderName, text + 4, 24);
    senderName[24] = '\0';
    const std::set<std::string>& blocked = g_whisperBlockMgr->Names();
    if (std::find(blocked.begin(), blocked.end(), senderName) != blocked.end()) {
      return;
    }

    std::memcpy(head, text + 0x1C, 6);
    head[6] = '\0';
    std::sscanf(head, "%lx", &rgb);
    color = ((rgb & 0xFF) << 16) | (rgb & 0xFF00) | ((rgb >> 16) & 0xFF);
    const unsigned int bodySize = size - 0x22;
    std::memcpy(body, text + 0x22, bodySize);
    body[size - 0x22] = '\0';
    message = body;
    fontType = 0;

    char check[0x400];
    std::memset(check, 0, sizeof(check));
    memcpy_s(check, sizeof(check), text + 0x40, bodySize);
    if (std::strlen(check) >= 0x16A) {
      return;
    }

    char filtered[0x188];
    std::memset(filtered, 0, sizeof(filtered));
    strcpy_s(filtered, sizeof(filtered), message.c_str());
    if (g_forbiddenWords.Contains(filtered) == true) {
      std::string masked;
      g_forbiddenWords.Mask(filtered, &masked);
      message = filtered;
    }
  } else if (std::strcmp(tag, "ssss") == 0) {
    std::memcpy(head, text + 4, length - 4);
    head[length - 4] = '\0';
    message = head;
    color = 0xFFFF;
    fontType = 14;
    systemMessage = true;
  } else if (hashPosition > 0 && hashPosition < length) {
    std::memcpy(head, text, hashPosition);
    head[hashPosition] = '\0';
    message = head;
    if (colonPosition > 0 && colonPosition < length) {
      std::memcpy(body, text + colonPosition, length - colonPosition + 1);
      message += body;
    }
  }

  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(
    1, reinterpret_cast<std::intptr_t>(message.c_str()), color, fontType, reinterpret_cast<std::intptr_t>(senderName), 0
  );
  if (!systemMessage) {
    g_windowMgr.SendNotice(
      0x13, reinterpret_cast<std::intptr_t>(message.c_str()), color, fontType, reinterpret_cast<std::intptr_t>(senderName), 0
    );
  }

  message = static_cast<UINewChatWnd*>(g_windowMgr.window_01C8)->ConvertChat(message);
  std::string linked;
  DisplayNavigationLink(linked, message);
  message = std::move(linked);

  std::vector<std::string> lines;
  WrapPlainText(message.c_str(), lines, 0x48);
  if (lines.begin() != lines.end()) {
    if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
      BroadcastBalloonMessage balloonMessage{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
      balloonMessage.start = std::chrono::system_clock::now();
      for (const std::string& line : lines) {
        balloonMessage.lines.emplace_back(line, color);
      }
      balloon->AddMessage(0, balloonMessage);
    }
  }
}

namespace {
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedText {
  public:
    explicit EncryptedText(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedText() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };
}

// OFFSET: 0x00D231F0
// FUNCTION: fn_21661
void CGameMode::ReceiveRefineStatusGraded(const char* playerName, int itemId, unsigned char refine, unsigned char grade, bool success) {
  ITEM_INFO item;
  item.SetItemId(itemId);
  item.unknown_0060 = refine;
  item.unknown_0088 = grade;
  item.identified = 1;

  int job = 0;
  const std::uint8_t signatureSource[] = {0x1B, 0x2D, 0xBB, 0xD1};
  thread_local EncryptedText<sizeof(signatureSource), 0xED796785D1DF137FULL> signature(signatureSource);
  const char* signatureText = signature.Text();
  const std::uint8_t functionSource[] = {0xB6, 0xA2, 0x4D, 0x4C, 0x66, 0x0C, 0x26, 0x81, 0xBB, 0xA8, 0x5B, 0x09};
  thread_local EncryptedText<sizeof(functionSource), 0xF14F79170939C7F1ULL> function(functionSource);
  g_session.m_hotkeyScript->Call(function.Text(), signatureText, itemId, &job);
  if (job != 0) {
    item.unknown_0000[0] = 0x15;
  }

  std::string itemName = item.DisplayName(1);
  std::string text = FormatOwnedString(LookupMessage(!success + 0x1108), playerName, itemName.c_str());
  std::string line = text;
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0x62B0FF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    std::uint32_t color = 0x62B0FF;
    message.lines.emplace_back(line, color);
    balloon->AddMessage(0, message);
  }
}

// OFFSET: 0x00D23580
// FUNCTION: fn_21662
void CGameMode::ReceiveItemObtainBroadcast(const unsigned char* data, bool package) {
  std::string name;
  std::string source;
  name.assign(reinterpret_cast<const char*>(data + 10), static_cast<signed char>(data[9]));
  const unsigned char* tail = data + static_cast<signed char>(data[9]);
  if (data[4] == 0) {
    ITEM_INFO box;
    box.SetItemId(*reinterpret_cast<const int*>(tail + 11));
    box.identified = 1;
    source = box.DisplayName(0);
  } else if (data[4] == 1) {
    source.assign(reinterpret_cast<const char*>(tail + 11), static_cast<signed char>(tail[10]));
  }

  ITEM_INFO item;
  item.SetItemId(*reinterpret_cast<const int*>(data + 5));
  item.identified = 1;
  std::string text;
  if (data[4] == 2) {
    text = FormatOwnedString(LookupMessage(0x74C), name.c_str(), item.DisplayName(0).c_str());
  } else if (package && data[4] == 0) {
    const int count = *reinterpret_cast<const int*>(tail + 16);
    if (count > 0) {
      text = FormatOwnedString(LookupMessage(0xF91), name.c_str(), source.c_str(), count, item.DisplayName(0).c_str());
    } else {
      text = FormatOwnedString(LookupMessage(0x65C), name.c_str(), source.c_str(), item.DisplayName(0).c_str());
    }
  } else {
    text = FormatOwnedString(LookupMessage(0x65C), name.c_str(), source.c_str(), item.DisplayName(0).c_str());
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message{{}, {}, std::chrono::seconds(20), 12, 0x40000000, 0, 0};
    message.start = std::chrono::system_clock::now();
    std::uint32_t color = 0x62B0FF;
    message.lines.emplace_back(text, color);
    balloon->AddMessage(0, message);
  }
}

// OFFSET: 0x00D63D70
// FUNCTION: fn_21992
void CGameMode::ReceiveActorStateEntryLegacy(const unsigned char* data) {
  const ActorStateEntryLegacyPacket* packet = reinterpret_cast<const ActorStateEntryLegacyPacket*>(data);
  ActorStandEntry entry;
  const int nameLength = packet->packetLength - static_cast<int>(sizeof(ActorStateEntryLegacyPacket));
  if (nameLength > 0) {
    const char* nameBegin = reinterpret_cast<const char*>(data + sizeof(ActorStateEntryLegacyPacket));
    this->nameCard.SetName(packet->aid, std::string(nameBegin, nameBegin + nameLength));
  }

  entry.job = packet->job;
  entry.unknown_0008 = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.unknown_001C = packet->xSize;
  entry.unknown_0020 = packet->ySize;
  entry.unknown_0024 = packet->head;
  entry.unknown_0028 = packet->weapon;
  entry.unknown_0030 = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.unknown_002C = packet->shield;
  entry.unknown_0044 = packet->pkMode;
  entry.unknown_004C = packet->accessory2;
  entry.unknown_0050 = packet->accessory3;
  entry.unknown_0054 = packet->headPalette;
  entry.unknown_0058 = packet->bodyPalette;
  entry.unknown_005C = packet->headDir;
  entry.guildId = packet->guildId;
  entry.unknown_0068 = packet->honor;
  entry.unknown_006C = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  entry.state = packet->state;
  if (packet->objectType != 0) {
    entry.unknown_0048 = entry.guildVersion;
  }
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (packet->objectType == 0) {
      actor->SendMsg(nullptr, 0x7C, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, 3, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  } else {
    CGameActor* created;
    if (packet->objectType == 0) {
      created = new CPc;
    } else {
      created = this->NewActor(packet->job);
    }
    if (!created) {
      return;
    }

    created->unknown_0314 = packet->objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (packet->objectType == 0) {
      if (this->unknown_01B4.find(packet->aid) != this->unknown_01B4.end() || this->unknown_01BC.find(packet->aid) != this->unknown_01BC.end()) {
        actor->SetUnknown02E4(1);
      }
      actor->SendMsg(nullptr, 0x79, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
      entry.unknown_0008 = 0;
      actor->SendMsg(nullptr, 4, 0, reinterpret_cast<int>(&entry), reinterpret_cast<int>(&entry) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (packet->isBoss && actor->unknown_0304 == 0) {
    actor->unknown_0304 = packet->isBoss;
    bossFlag = 1;
  }
  actor->unknown_0210 = packet->level;
  *reinterpret_cast<short*>(actor->render.unknown_00FCBytes) = packet->font;
  if (packet->objectType == 7) {
    if (g_session.IsPetAid(packet->aid)) {
      g_session.unknown_1038 = entry.unknown_0030;
    }
    if (entry.unknown_0030 && !actor->Slot50()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.unknown_0030));
    }
  } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
    g_session.unknown_5614 = 1;
  } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
    g_session.unknown_56B0 = 1;
  } else if (g_session.GetSceneUnknown004C() && actor->unknown_0188 == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  switch (actor->unknown_0314) {
  case 0:
  case 5:
  case 8:
  case 9:
  case 10: {
    const std::int64_t hp = packet->hp;
    const std::int64_t maxHp = packet->maxHp;
    actor->SendMsg(
      nullptr, 0x91, 0, static_cast<int>(hp), static_cast<int>(hp >> 32), static_cast<int>(maxHp), static_cast<int>(maxHp >> 32),
      static_cast<int>(bossFlag), static_cast<int>(bossFlag >> 32), 0, 0, 0, 0, 0, 0
    );
    break;
  }
  case 13:
  case 14:
    actor->SendMsg(nullptr, 0x22, 0, packet->aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    break;
  }

  if (packet->objectType != 0) {
    const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (guildId > 0 && !actor->Slot50()) {
      actor->Slot49(guildId, packet->guildVersion);
    }
  }
  this->sub_D591A0(packet->aid, packet->effectState, packet->level, packet->virtue);
}

namespace {
#pragma pack(push, 1)
struct PartyMemberInfoPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ std::int32_t leader;
  /* 000A */ unsigned char unknown_000A[4];
  /* 000E */ unsigned char offline;
  /* 000F */ char partyName[24];
  /* 0027 */ char memberName[24];
  /* 003F */ char mapName[16];
};

struct PartyMemberListHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ char partyName[24];
};

struct PartyMemberListEntry {
  /* 0000 */ std::uint32_t accountId;
  /* 0004 */ char name[24];
  /* 001C */ char mapName[16];
  /* 002C */ unsigned char leader;
  /* 002D */ unsigned char offline;
};
#pragma pack(pop)

static_assert(sizeof(PartyMemberInfoPacket) == 0x4F);
static_assert(sizeof(PartyMemberListHeader) == 0x1C);
static_assert(sizeof(PartyMemberListEntry) == 0x2E);
}

// OFFSET: 0x00D20900
// FUNCTION: fn_21650
void CGameMode::ReceivePartyMemberInfo(const unsigned char* data) {
  const PartyMemberInfoPacket* packet = reinterpret_cast<const PartyMemberInfoPacket*>(data);
  char name[25];
  name[24] = '\0';
  std::memcpy(name, packet->memberName, sizeof(packet->memberName));
  char partyName[25];
  partyName[24] = '\0';
  std::strncpy(partyName, packet->partyName, sizeof(packet->partyName));
  if (g_forbiddenWords.Contains(partyName)) {
    std::string masked;
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.unknown_54C4 = partyName;

  char map[17];
  map[16] = '\0';
  std::memcpy(map, packet->mapName, sizeof(packet->mapName));
  MessengerMember member;
  member.name = name;
  member.accountId = packet->accountId;
  member.map = map;
  member.leader = packet->leader;
  member.offline = packet->offline;

  if (member.accountId == g_session.m_aid) {
    if (member.leader == 0) {
      g_session.unknown_5464 = 1;
    }
  } else if (member.offline == 1) {
    this->SendMsg(0x40, member.accountId, 0, 0, 0);
  } else {
    std::string memberMap = member.map;
    std::string currentMap = this->mapName;
    if (memberMap.size() > 4) {
      memberMap.resize(memberMap.size() - 3);
    }
    if (currentMap.size() > 4) {
      currentMap.resize(currentMap.size() - 3);
    }
    if (memberMap != currentMap) {
      this->SendMsg(0x40, member.accountId, 0, 0, 0);
    }
  }

  g_session.AddPartyMember(member);
  sub_DFF150(&g_session);
}

// OFFSET: 0x00D2AFA0
// FUNCTION: fn_21708
void CGameMode::ReceivePartyMemberList(const unsigned char* data) {
  const PartyMemberListHeader* packet = reinterpret_cast<const PartyMemberListHeader*>(data);
  g_session.ClearPartyMembers();
  char partyName[25];
  partyName[24] = '\0';
  std::memcpy(partyName, packet->partyName, sizeof(packet->partyName));
  if (g_forbiddenWords.Contains(partyName)) {
    std::string masked;
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.unknown_54C4 = partyName;

  if (packet->length != 0x1C) {
    const int count = (packet->length - 0x1C) / 0x2E;
    const PartyMemberListEntry* entries = reinterpret_cast<const PartyMemberListEntry*>(data + sizeof(*packet));
    for (int index = 0; index < count; ++index) {
      const PartyMemberListEntry& entry = entries[index];
      char name[25];
      name[24] = '\0';
      std::memcpy(name, entry.name, sizeof(entry.name));
      char map[17];
      map[16] = '\0';
      std::memcpy(map, entry.mapName, sizeof(entry.mapName));
      MessengerMember member;
      member.accountId = entry.accountId;
      member.name = name;
      member.map = map;
      member.leader = entry.leader;
      member.offline = entry.offline;
      g_session.AddPartyMember(member);
      if (member.accountId == g_session.m_aid && member.leader == 0) {
        g_session.unknown_5464 = 1;
      }
    }
    sub_DFF150(&g_session);
  }
}

// OFFSET: 0x00D59FD0
// FUNCTION: fn_21960
void CGameMode::sub_D59FD0(const char* value, std::vector<PartyRosterEntry>* members) {
  g_session.ClearPartyMembers();
  char partyName[25];
  partyName[24] = '\0';
  std::memcpy(partyName, value, 24);
  if (g_forbiddenWords.Contains(partyName)) {
    std::string masked;
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.unknown_54C4 = partyName;

  for (const PartyRosterEntry& entry : *members) {
    char name[25];
    name[24] = '\0';
    std::memcpy(name, entry.name, sizeof(entry.name));
    char map[17];
    map[16] = '\0';
    std::memcpy(map, entry.mapName, sizeof(entry.mapName));
    MessengerMember member;
    member.accountId = entry.accountId;
    member.characterId = entry.characterId;
    member.name = name;
    member.map = map;
    member.leader = entry.member;
    member.offline = entry.offline;
    member.job = static_cast<std::uint16_t>(entry.x);
    member.level = static_cast<std::uint16_t>(entry.y);
    member.color = member.offline == 0 ? 0x493A37 : 0xA38A86;
    if (member.accountId == g_session.m_aid && member.leader == 0) {
      member.color = 0x6F3007;
      g_session.unknown_5464 = 1;
    }
    g_session.AddPartyMember(member);
  }

  g_session.LoadMiniPartyInfo();
  g_windowMgr.RestoreMiniPartyWindows();
  sub_DFF150(&g_session);
}

// OFFSET: 0x00D5EEF0
// FUNCTION: fn_21968
void CGameMode::sub_D5EEF0(
  const char* name, int accountId, const char* mapName, int leader, unsigned char offline, std::uint16_t job, std::uint16_t level,
  const char* value, unsigned char itemShare, unsigned char itemDistribute, int characterId
) {
  MessengerMember member;
  member.name = name;
  member.accountId = accountId;
  member.characterId = characterId;
  member.map = mapName;
  member.leader = leader;
  member.offline = offline;
  member.job = job;
  member.level = level;
  char partyName[25];
  partyName[24] = '\0';
  std::strncpy(partyName, value, 24);
  if (g_forbiddenWords.Contains(partyName)) {
    std::string masked;
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.unknown_54C4 = partyName;

  if (member.accountId == g_session.m_aid) {
    if (member.leader == 0) {
      g_session.unknown_5464 = 1;
    }
  } else if (member.offline == 1) {
    this->SendMsg(0x40, accountId, 0, 0, 0);
    this->SetActorUnknown02E4(accountId, 0);
  } else {
    std::string memberMap = member.map;
    std::string currentMap = this->mapName;
    if (memberMap.size() > 4) {
      memberMap.resize(memberMap.size() - 3);
    }
    if (currentMap.size() > 4) {
      currentMap.resize(currentMap.size() - 3);
    }
    const bool sameMap = memberMap == currentMap;
    if (!sameMap) {
      this->SendMsg(0x40, accountId, 0, 0, 0);
    }
    this->SetActorUnknown02E4(accountId, sameMap);
  }

  g_session.unknown_54A8 = itemDistribute != 0;
  g_session.unknown_54A4 = itemShare != 0;
  g_session.PartyCount();
  g_session.AddPartyMember(member);
  g_windowMgr.NotifyMemberWindows(&member);
  this->SetActorUnknown02E4(accountId, 1);
}

// OFFSET: 0x00D5B940
// FUNCTION: fn_21965
void LaunchSkillActorEffect(CGameActor* actor, short x, short y, std::uint16_t skillId, int delay, int effectId) {
  if (skillId == 0x3E || skillId == 0x39) {
    return;
  }

  if (g_effectMgr->HasAllocator(effectId)) {
    g_effectMgr->Spawn(actor->render.accountId, effectId, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, -1, 0, 0);
  } else {
    const int delayFrames = delay / 16;
    actor->SendMsg(nullptr, 0x55, 0, effectId, effectId >> 31, delayFrames, delayFrames >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
  }

  const vector3d origin{0.0f, 0.0f, 0.0f};
  CEffect* effect;

  switch (skillId) {
  default: {
    if (CSkillActionMgr::GetInstance()->ActionSlot08(skillId, delay, actor) == true) {
      return;
    }

    std::map<std::int64_t, std::int64_t> primaryEffects;
    std::map<std::int64_t, std::int64_t> secondaryEffects;
    primaryEffects[0x14F9] = 0x7EC;
    secondaryEffects[0x14F9] = 0x7ED;
    primaryEffects[0x14FB] = 0x7F0;
    secondaryEffects[0x14FB] = 0x7F1;
    primaryEffects[0x14FF] = 0x7F5;
    secondaryEffects[0x14FF] = 0x7F6;
    primaryEffects[0x14FA] = 0x7FB;
    secondaryEffects[0x14FA] = 0x7FC;
    primaryEffects[0x14FC] = 0x809;
    secondaryEffects[0x14FC] = 0x80A;
    primaryEffects[0x14ED] = 0x80D;
    secondaryEffects[0x14ED] = 0x80E;
    primaryEffects[0x1501] = 0x834;
    secondaryEffects[0x1501] = 0x835;
    primaryEffects[0x1502] = 0x839;
    secondaryEffects[0x1502] = 0x83A;
    primaryEffects[0x14C7] = 0x829;
    secondaryEffects[0x14C7] = 0x82A;
    primaryEffects[0x14FD] = 0x857;
    secondaryEffects[0x14FD] = 0x858;
    primaryEffects[0x1504] = 0x85A;
    primaryEffects[0x14C3] = 0x86B;
    primaryEffects[0x14C9] = 0x86B;
    primaryEffects[0x14DB] = 0x873;
    secondaryEffects[0x14DB] = 0x874;
    primaryEffects[0x14DA] = 0x88B;
    primaryEffects[0x14E4] = 0x8BC;
    secondaryEffects[0x14E4] = 0x8BD;
    primaryEffects[0x14DF] = 0x89C;
    secondaryEffects[0x14DF] = 0x89D;
    primaryEffects[0x14DC] = 0x8A4;
    secondaryEffects[0x14DC] = 0x8A5;
    primaryEffects[0x14DE] = 0x8AC;
    secondaryEffects[0x14DE] = 0x8AD;
    primaryEffects[0x14DD] = 0x8B4;
    secondaryEffects[0x14DD] = 0x8B5;
    primaryEffects[0x14E3] = 0x8C2;
    secondaryEffects[0x14E3] = 0x8C3;
    primaryEffects[0x14E0] = 0x8C7;
    secondaryEffects[0x14E0] = 0x8C8;
    primaryEffects[0x14E1] = 0x8CE;
    secondaryEffects[0x14E1] = 0x8D0;
    primaryEffects[0x14E2] = 0x8C7;
    secondaryEffects[0x14E2] = 0x8C8;

    const std::int64_t key = skillId;
    const vector3d position{0.0f, 0.0f, 0.0f};
    if (primaryEffects.find(key) != primaryEffects.end()) {
      effect = g_effectMgr->Spawn(actor->render.accountId, static_cast<int>(primaryEffects[key]), position.x, position.y, position.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
    }
    if (secondaryEffects.find(key) != secondaryEffects.end()) {
      effect = g_effectMgr->Spawn(actor->render.accountId, static_cast<int>(secondaryEffects[key]), position.x, position.y, position.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
    }
    return;
  }
  case 0x7D6:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x65A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x7F0:
    actor->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    return;
  case 0x89E:
    actor->LaunchEffect(0x323, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    return;
  case 0x8A9:
    if (g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x5A0, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x597, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    } else {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x4FD, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x4ED, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    }
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x8A5:
    if (g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x600, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x603, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    } else {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x587, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x58D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    }
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x89C:
    effect = g_effectMgr->Spawn(actor->render.accountId, g_serviceOptions.Get(0xE5) ? 0x607 : 0x5A6, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x8A4:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x610, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x804:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x617, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x89A:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x633, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x634, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x7F6:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x65D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x65E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x8A6:
    g_effectMgr->Spawn(actor->render.accountId, 0x663, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x664, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x8BC:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x671, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x8D5: {
    float angle = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CameraDesiredYaw() + actor->render.angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
    }

    int id;
    if (angle > 0.0f && angle < 90.0f) {
      id = 0x545;
    } else if (angle >= 90.0f && angle < 180.0f) {
      id = 0x543;
    } else if (angle >= 180.0f && angle < 270.0f) {
      id = 0x549;
    } else {
      id = 0x547;
    }

    if (g_serviceOptions.Get(0xE5)) {
      if (id == 0x545) {
        id = 0x5CF;
      } else if (id == 0x543) {
        id = 0x5CE;
      } else if (id == 0x549) {
        id = 0x5D1;
      } else if (id == 0x547) {
        id = 0x5D0;
      }
      g_effectMgr->Spawn(actor->render.accountId, id, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x5D4, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x5D5, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    } else {
      g_effectMgr->Spawn(actor->render.accountId, id, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, id + 1, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x568, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x569, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    }
    return;
  }
  case 0x9A0: {
    g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    CRagEffect* ragEffect = actor->LaunchEffect(0xD8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (ragEffect) {
      vector3d position = actor->render.position;
      ragEffect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    return;
  }
  case 0x98F: {
    vector3d position{0.0f, 0.0f, 0.0f};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(x, y, position.x, position.z);
    position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(position.x, position.z);
    effect = g_effectMgr->Spawn(0, g_serviceOptions.Get(0xE5) ? 0x5AC : 0x504, position.x, position.y, position.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  }
  case 0x2E0:
  case 0x991: {
    vector3d position{0.0f, 0.0f, 0.0f};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(x, y, position.x, position.z);
    position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(position.x, position.z);
    effect = g_effectMgr->Spawn(0, g_serviceOptions.Get(0xE5) ? 0x5B0 : 0x50A, position.x, position.y, position.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  }
  case 0x9AD:
    if (g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x5DB, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x5DC, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    } else {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x58E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x58F, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    }
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x2E1:
  case 0x911:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x591, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x592, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x996:
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x61B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x61C, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x8FE:
    actor->LaunchEffect(0x10, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    return;
  case 0x927:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x625, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x9D5:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x96D:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x9B2:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x654, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    if (g_serviceOptions.Get(0xE5)) {
      return;
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x655, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x9BC:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x669, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x97D:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x66A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x66B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x90D:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x697, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    if (g_serviceOptions.Get(0xE5)) {
      return;
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x698, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x145E:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CA, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CB, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x145F:
  case 0x1472:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6C6, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6C7, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
      effect->AddEvent(0, "effect\\EF_BeginSpell.wav", 0, 0);
    }
    return;
  case 0x1464:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CE, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CF, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1471:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6D3, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6D4, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1460:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6D9, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6DA, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1473:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6DD, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6DE, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1462:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6E1, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6E2, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x146E:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6E9, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6EA, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1461:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6ED, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6EE, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x146B:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F2, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F3, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1475:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F6, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F7, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1470:
    g_effectMgr->Spawn(actor->render.accountId, 0x70A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x70B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot16(200);
      effect->Slot14(delay - 200);
    }
    return;
  case 0x146D:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x70D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x70E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1466:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x704, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x705, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x147E:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7B8, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x147F:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7BB, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1486:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7BF, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1495:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x711, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1497:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x714, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1498:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x716, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x149B:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x718, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1499:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x71F, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    if (g_serviceOptions.Get(0xE5)) {
      return;
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x71E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x149E: {
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x73B, actor->render.position.x, actor->render.position.y, actor->render.position.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
    }

    CGroundScope* scope = new (std::nothrow) CGroundScope;
    if (!scope) {
      return;
    }

    int column;
    int row;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->WorldCell(actor->render.position.x, actor->render.position.z, column, row);
    scope->SendMsg(nullptr, 0x16, 0, actor->render.accountId, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    scope->SendMsg(nullptr, 0xE, 0, column, column >> 31, row, row >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
    scope->SendMsg(nullptr, 0x73, 0, timeGetTime() + delay, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    const int id = skillId;
    scope->SendMsg(nullptr, 0x75, 0, id, id >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(scope);
    return;
  }
  case 0x1469:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x740, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x741, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14B8:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x76D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14A1:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x745, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14D7:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x781, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14D3:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x751, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14D5:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x755, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14D4:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x758, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x759, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14C6:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x84F, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x850, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14A2:
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x760, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x761, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x149F:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7A6, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14A0:
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x785, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot14(delay);
      }
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x786, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1465:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x762, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x763, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14B9:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x774, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14B6:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x78F, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14B7:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x77C, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14D1:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7B1, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14F8:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7C9, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7CA, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14F7:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7D4, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7D5, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1503:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DA, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DB, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14C4:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DE, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DF, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x1500:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7E5, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7E6, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    return;
  case 0x14F3:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x81C, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x81D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->unknown_002C = delay;
    }
    return;
  case 0x14F1:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x84B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay + 50);
    }
    return;
  case 0x1F79:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x91D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot14(delay + 50);
    }
    return;
  }
}
// OFFSET: 0x00D7B640
// FUNCTION: fn_22208
void CGameMode::ReceiveSkillUse(std::uint16_t skillId, int level, std::uint32_t sourceAid, std::uint32_t targetAid, bool result) {
  CGameActor* source;
  if (sourceAid == g_session.m_aid) {
    source = this->scene->player;
  } else {
    source = static_cast<CGameActor*>(this->scene->GetGameActorByAID(sourceAid));
  }

  int action = -1;
  if (source != nullptr) {
    if (skillId == 0xF9 || skillId == 0x201C) {
      source->Slot15(2, 1);
    }
    if (g_session.IsMonsterActor(source) == 0 && skillId != 9) {
      if (skillId == 0x14E || skillId == 0x14F) {
        this->SendMsg(0x56, skillId, reinterpret_cast<std::intptr_t>(source), targetAid, 0);
      } else {
        this->SendMsg(0x56, skillId, reinterpret_cast<std::intptr_t>(source), level, 0);
      }
    } else {
      int loopType;
      g_session.GetMonsterSkillInfo(g_session.GetJobName(source->GetEffectiveJob(-1), -1), skillId, &action, &loopType);
      if (action != -1) {
        source->SetAction(action * 8 - 8, 1, loopType == 0);
      }
    }
    if (g_session.IsMonsterActor(source) == 1) {
      this->SendMsg(0x56, skillId, reinterpret_cast<std::intptr_t>(source), level, 0);
    }
    if (action == -1) {
      switch (skillId) {
      case 0x107:
      case 0x110:
      case 0x111:
      case 0x173:
      case 0x917:
      case 0x918:
      case 0x919:
      case 0xA13:
      case 0x14C4:
      case 0x14CA:
      case 0x14CF:
        break;
      default: {
        int effectId;
        int motion;
        g_session.GetSkillActionInfo2(skillId, effectId, motion);
        if (source->render.unknown_0070 != 1 && motion == 1) {
          source->Slot15(7, motion);
        } else {
          if (motion == 2 || motion == 9) {
            source->SendMsg(nullptr, 0x51, 0, targetAid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            if (CSkillActionMgr::GetInstance()->ActionSlot06(skillId)) {
              source->Slot59(true);
            }
          }
          source->Slot15(motion, 1);
        }
        break;
      }
      }
    }
  }

  CGameActor* target;
  if (targetAid == g_session.m_aid) {
    target = this->scene->player;
  } else {
    target = static_cast<CGameActor*>(this->scene->GetGameActorByAID(targetAid));
  }

  switch (skillId) {
  case 0x7C:
  case 0x83:
    if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
      if (source != nullptr) {
        source->Face(skill->render.position.x, skill->render.position.z);
      }
    }
    break;
  case 0xF4:
    if (sourceAid == g_session.m_aid) {
      if (g_windowMgr.window_03C0 != nullptr) {
        g_windowMgr.CloseWindow(0x71);
      }
      if (g_windowMgr.window_03C4 != nullptr) {
        g_windowMgr.CloseWindow(0x72);
      }
      g_session.ClearHomunStatus();
      g_session.unknown_5614 = 0;
    }
    break;
  case 0x309:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x985, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&target->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x30A:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x984, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&target->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case 0xC:
  case 0x12:
  case 0x15:
  case 0x46:
  case 0x4F:
  case 0x53:
  case 0x55:
  case 0x59:
  case 0x6E:
  case 0x1DE:
  case 0x2D3:
  case 0x2DD:
  case 0x301:
  case 0x138E:
  case 0x13A4:
  case 0x13B2:
    if (source != nullptr) {
      float x;
      float z;
      this->CellCenter(targetAid >> 16, targetAid & 0xFFFF, x, z);
      source->Face(x, z);
    }
    break;
  default:
    if (source != nullptr && target != nullptr && source != target) {
      source->Face(target->render.position.x, target->render.position.z);
    }
    break;
  }

  if (!result) {
    return;
  }

  int effectLevel = level;
  if (level < 1) {
    effectLevel = 1;
  } else if (level > 10) {
    effectLevel = skillId != 9 ? 10 : level;
  }

  switch (skillId) {
  case 0x4B:
    if (source != nullptr) {
      source->LaunchEffect(0x4B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x47:
    if (g_serviceType == 0 && target != nullptr) {
      target->LaunchEffect(0x88, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x70:
    if (target != nullptr) {
      target->LaunchEffect(0x67, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x72:
    if (source != nullptr) {
      source->LaunchEffect(0x68, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8A:
  case 0x8D:
    if (target != nullptr) {
      target->LaunchEffect(0x7D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8B:
    if (target != nullptr) {
      target->LaunchEffect(0x7E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x3C:
  case 0x1EF:
    if (source != nullptr) {
      source->SendMsg(nullptr, 109, 0, 0x82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x82, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x3D:
    if (source != nullptr) {
      source->SendMsg(nullptr, 0x52, 0, level * 400, (level * 400) >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->Slot15(13, 1);
      if (sourceAid != g_session.m_aid) {
        source->SendMsg(nullptr, 0x53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
    break;
  case 0x42:
    if (target != nullptr) {
      target->LaunchEffect(0x54, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x43:
    if (target != nullptr) {
      target->LaunchEffect(0x58, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x44:
    if (target != nullptr) {
      target->LaunchEffect(0x56, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x23:
    if (target != nullptr) {
      target->LaunchEffect(0x42, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x48:
    if (target != nullptr) {
      target->LaunchEffect(0x4E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x4E:
  case 0x2D7:
  case 0x13D0:
    if (target != nullptr) {
      target->LaunchEffect(0x55, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x51:
    if (source != nullptr) {
      source->LaunchEffect(0x3E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x58:
    if (target != nullptr) {
      target->LaunchEffect(0x1C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x83:
    if (source != nullptr) {
      if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
        static_cast<EffectActor*>(skill)->LaunchEffect(0x6F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (source->unknown_037C[0] != 0) {
          const int address = reinterpret_cast<int>(&skill->render.position);
          reinterpret_cast<CGameObject*>(source->unknown_037C[0])->SendMsg(nullptr, 0x1D, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
      }
    }
    break;
  case 0x7C:
    if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
      static_cast<EffectActor*>(skill)->LaunchEffect(0x64, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x36:
    if (target != nullptr) {
      target->LaunchEffect(0x4D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (IsPlayerJob(target->GetEffectiveJob(-1))) {
        target->LaunchEffect(0x8C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case 0x35:
    if (target != nullptr) {
      target->LaunchEffect(0x15, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x32:
    if (target != nullptr) {
      target->LaunchEffect(0x12, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x10:
    if (target != nullptr && skillId == 0x10) {
      target->LaunchEffect(0x17, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("_stonecurse.wav", target->render.position.x - this->scene->player->render.position.x, 0.0f, target->render.position.z - this->scene->player->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case 0x1E:
    if (target != nullptr) {
      target->LaunchEffect(0x26, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F:
    if (source != nullptr) {
      source->LaunchEffect(0x27, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x20:
    if (source != nullptr) {
      source->LaunchEffect(0x28, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x21:
    if (source != nullptr) {
      source->LaunchEffect(0x29, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8:
    if (source != nullptr) {
      source->LaunchEffect(0xB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x6:
    if (target != nullptr) {
      target->LaunchEffect(0x43, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2D:
    if (source != nullptr) {
      source->LaunchEffect(0x99, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x9D:
    if (source != nullptr) {
      source->LaunchEffect(0xA9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA2:
    if (source != nullptr) {
      source->LaunchEffect(0xAE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA3:
    if (source != nullptr) {
      source->LaunchEffect(0xB1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA4:
    if (source != nullptr) {
      source->LaunchEffect(0xAD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA5:
    if (source != nullptr) {
      source->LaunchEffect(0xAF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA6:
    if (source != nullptr) {
      source->LaunchEffect(0xB3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA7:
    if (source != nullptr) {
      source->LaunchEffect(0xB2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA8:
    if (source != nullptr) {
      source->LaunchEffect(0xAC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA9:
    if (source != nullptr) {
      source->LaunchEffect(0xB0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xAF:
    if (source != nullptr) {
      if (CRagEffect* effect = this->scene->player->LaunchEffect(0xB9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&source->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case 0xC4:
    if (source != nullptr) {
      source->LaunchEffect(0xD7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xC9:
    if (source != nullptr) {
      source->LaunchEffect(0xD6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xCD:
    if (source != nullptr) {
      source->LaunchEffect(0xD5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xFC:
    if (source != nullptr) {
      source->LaunchEffect(0xFC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x9B:
    if (source != nullptr) {
      source->LaunchEffect(0x137, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xD3:
    if (target != nullptr) {
      if (sourceAid == g_session.m_aid) {
        char notice[512];
        std::sprintf(notice, LookupMessage(0x651), static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(target->render.accountId).c_str(), level);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(notice), 0xC8C8FF, 0x10, 0, 0);
      }
      if (CRagEffect* effect = target->LaunchEffect(0x112, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      target->LaunchEffect(0x10C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xD6:
    if (source != nullptr) {
      source->LaunchEffect(0x114, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xD8:
    if (target != nullptr) {
      target->LaunchEffect(0x10E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xD9:
    if (target != nullptr) {
      target->LaunchEffect(0x10F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xDA:
    if (target != nullptr) {
      target->LaunchEffect(0x110, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xEA:
  case 0xEB:
  case 0xEC:
  case 0xED:
    if (target != nullptr) {
      target->LaunchEffect(0x12C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xE7:
    this->LaunchTargetEffect(0x12B, source, target, effectLevel);
    break;
  case 0x9:
    if (target != nullptr) {
      target->SendMsg(nullptr, 0x58, 0, effectLevel, effectLevel >> 31, static_cast<int>(0xFF0000FF), 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 0xFF:
    if (source != nullptr) {
      source->LaunchEffect(0xFB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x115:
    if (target != nullptr) {
      target->LaunchEffect(0xEA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x121:
  case 0x2EA:
    if (target != nullptr) {
      target->LaunchEffect(0xEB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x114:
    if (source != nullptr) {
      source->LaunchEffect(0xF4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x100:
    if (target != nullptr) {
      target->LaunchEffect(0xF8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x102:
    if (source != nullptr) {
      source->SendMsg(nullptr, 109, 0, 0xFA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0xFA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x106:
    if (source != nullptr) {
      source->LaunchEffect(0xFD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x10D:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 3, 3, 0x60, 3, 1, 3000, 0);
    }
    break;
  case 0x10E:
    if (source != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\mon_폭기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->SendMsg(nullptr, 109, 0, 0xCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0xCB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x105, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetSequenceInterval(30);
      source->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, 1000, 0);
    }
    break;
  case 0x10C:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      source->LaunchEffect(0x105, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 20, 0);
    }
    break;
  case 0x6F:
  case 0x1CB:
    if (source != nullptr) {
      source->LaunchEffect(0x62, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x71:
  case 0x1E6:
    if (source != nullptr) {
      source->SendMsg(nullptr, 109, 0, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x80, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x156:
    if (target != nullptr) {
      target->LaunchEffect(0x1C5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x14F:
    if (target != nullptr) {
      target->LaunchEffect(0xFD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->SendMsg(nullptr, 0x58, 0, level, level >> 31, static_cast<int>(0xFF0000FF), 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 0x198:
    if (target != nullptr) {
      target->LaunchEffect(0x198, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x163:
    if (source != nullptr) {
      source->SendMsg(nullptr, 109, 0, 0x16F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x16F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SendMsg(nullptr, 109, 0, 0x1DA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x1DA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetFrameSequence(0x20, 0, 5, 0x20, 5, 1, 1000, 0);
      g_soundMgr->PlayWave("effect\\오라 블레이드.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x165:
    if (source != nullptr) {
      source->SendMsg(nullptr, 109, 0, 0x171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x171, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1DC:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x1EF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x1DB:
    if (source != nullptr) {
      source->LaunchEffect(0x1F0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x15D:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\mon_폭기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x1C7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xD7:
  case 0x157:
    if (target != nullptr) {
      target->LaunchEffect(0x10D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x146:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 1200, 0);
      if (source == this->scene->player) {
        source->LaunchEffect(0x128, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case 0x13E:
    if (source != nullptr) {
      source->SetSequenceInterval(10);
      source->SetFrameSequence(0x60, 4, 5, 0x60, 4, 1, 1800, 1);
      if (source == this->scene->player) {
        source->LaunchEffect(0x127, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case 0x140:
    g_soundMgr->PlayWave("effect\\석양의 어쌔신.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x348, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x141:
    g_soundMgr->PlayWave("effect\\브라기의 시.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x142:
    g_soundMgr->PlayWave("effect\\이둔의 사과.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x349, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x147:
    g_soundMgr->PlayWave("effect\\흥얼거림.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x148:
    g_soundMgr->PlayWave("effect\\나를잊지말아요.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x14A:
    g_soundMgr->PlayWave("effect\\당신을 위한 서비스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x348, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x132:
    g_soundMgr->PlayWave("effect\\자장가.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1E8:
    g_soundMgr->PlayWave("effect\\헤르모드의 지팡이.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x1E9:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\priest_slowpoison.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x1F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x133:
    g_soundMgr->PlayWave("effect\\김서방돈.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x134:
    g_soundMgr->PlayWave("effect\\영원의 혼돈.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x135:
    g_soundMgr->PlayWave("effect\\전장의.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x136:
    g_soundMgr->PlayWave("effect\\니벨룽겐의 반지.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x137:
    g_soundMgr->PlayWave("effect\\로키.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x138:
    g_soundMgr->PlayWave("effect\\심연속으로.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x139:
    g_soundMgr->PlayWave("effect\\불사신.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x13F:
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x149:
    g_soundMgr->PlayWave("effect\\행운의.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x348, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x15E:
  case 0x1FA:
    if (source != nullptr) {
      source->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x16C:
    if (source != nullptr && target != nullptr) {
      source->SetFrameSequence(0x58, 0, 3, 0x58, 3, 1, 1000, 0);
      if (CRagEffect* effect = source->LaunchEffect(0x17A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      target->LaunchEffect(0x17B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x16E:
    if (target != nullptr) {
      target->SendMsg(nullptr, 0x6D, 0, 0x17E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x17E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\마법력 증폭.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x175:
    if (source != nullptr) {
      source->LaunchEffect(0x17F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x176:
    if (source != nullptr && target != nullptr) {
      vector3d offset{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = source->LaunchEffect(0x180, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&offset), 0, 0, 0, 0);
      }
      source->LaunchEffect(0x181, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x181, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x197, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x197, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x177:
    if (target != nullptr) {
      target->LaunchEffect(0x196, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x17C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x17A:
    if (source != nullptr) {
      source->LaunchEffect(0x1ED, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x17C:
    if (source != nullptr) {
      source->LaunchEffect(0x182, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x17F:
    if (source != nullptr) {
      source->LaunchEffect(0x185, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x180:
    if (source != nullptr) {
      source->LaunchEffect(0x186, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x183:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\EF_IncAgility.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x187, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x185:
    if (source != nullptr) {
      source->SendMsg(nullptr, 0x3F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 0x186:
    if (source != nullptr) {
      source->LaunchEffect(0x188, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x18B:
    if (source != nullptr && !source->HasId(0x18A)) {
      source->LaunchEffect(0x18A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    g_soundMgr->PlayWave("effect\\달빛.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x18C:
    if (source != nullptr && target != nullptr) {
      vector3d offset{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = source->LaunchEffect(0x18B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&offset), 0, 0, 0, 0);
      }
      source->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x192:
    if (target != nullptr) {
      target->LaunchEffect(0x193, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x193:
  case 0x1F44:
    if (source != nullptr) {
      source->LaunchEffect(0x1F9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x196:
    if (source != nullptr) {
      source->LaunchEffect(0x199, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x19B:
    if (source != nullptr) {
      if (source->HasId(0x1BB)) {
        source->LaunchEffect(0x1BC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (!source->HasId(0x1BA)) {
        source->LaunchEffect(0x1BA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case 0x19D:
    if (source != nullptr) {
      source->LaunchEffect(0x1B3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1A4:
    g_soundMgr->PlayWave("effect\\T_낙법.wav", 0.0f, -150.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x1A9:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\T_바람방출.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      while (source->HasId(0x1CD)) {
        source->SendMsg(nullptr, 0x6D, 0, 0x1CD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(0x1CD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      const int effectId = effectLevel + 0x1D2;
      while (source->HasId(effectId)) {
        source->SendMsg(nullptr, 0x6D, 0, effectId, effectId >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1AA:
    if (source != nullptr) {
      if (source->HasId(0x1BE)) {
        source->SendMsg(nullptr, 0x6D, 0, 0x1BE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      while (source->HasId(0x1BD)) {
        source->SendMsg(nullptr, 0x6D, 0, 0x1BD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
    break;
  case 0x1AC:
  case 0x1AD:
  case 0x1AE:
    if (source != nullptr && target != nullptr) {
      source->SendMsg(nullptr, 0x6D, 0, 0x1A2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x1A2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SendMsg(nullptr, 0x6D, 0, 0x220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x23C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1AF:
  case 0x1B0:
  case 0x1B1:
    if (source != nullptr) {
      source->SendMsg(nullptr, 0x6D, 0, 0x1E8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x1E8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1B2:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_등록.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, 0x6D, 0, 0x21F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1BC:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\T_변신.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    }
    break;
  case 0x1BE:
    if (source != nullptr && target != nullptr) {
      this->LaunchTargetEffect(0x21D, source, target, 4);
      target->LaunchEffect(0xDC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1CE:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\priest_resurrection.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x24E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1CF:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_보조마법.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1D0:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_치잉.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x21E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1D1:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_마법반사.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, 0x6D, 0, 0x1A3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x21E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1D6:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_슈웃.wav", 0.0f, -50.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, 0x6D, 0, 0x1A4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x24D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1D7:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_공격력.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, 0x6D, 0, 0x1AB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1AB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1D8:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_방어형.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      target->SendMsg(nullptr, 0x6D, 0, 0xFE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->SendMsg(nullptr, 0x6D, 0, 0x46C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0xFE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x46C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x105, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1DF:
    if (target != nullptr) {
      target->LaunchEffect(0x12C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1E2:
    if (target != nullptr) {
      target->LaunchEffect(0x209, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\마법력 증폭.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x1E7:
    g_soundMgr->PlayWave("effect\\ac_concentration.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1ED:
    g_soundMgr->PlayWave("effect\\T_피링.wav", 0.0f, -100.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x1F4:
    g_soundMgr->PlayWave("effect\\플립.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x1F8:
    if (source != nullptr) {
      source->LaunchEffect(0x271, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F9:
  case 0x205:
    if (source != nullptr) {
      source->LaunchEffect(0x272, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1FC:
    g_soundMgr->PlayWave("effect\\크래커.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x201:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x273, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x204:
    if (source != nullptr) {
      source->LaunchEffect(0x27D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x20F:
    g_soundMgr->PlayWave("effect\\다다미뒤집기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x218:
    if (target != nullptr) {
      target->LaunchEffect(0x27B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x21B:
    if (source != nullptr) {
      source->LaunchEffect(0x27C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x21D:
    if (source != nullptr) {
      source->LaunchEffect(0x26E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x22B:
    if (source != nullptr) {
      source->SendMsg(nullptr, 0x3F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if (target != nullptr) {
      target->SendMsg(nullptr, 0x3F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8E:
  case 0x7E0:
    if (source != nullptr) {
      source->LaunchEffect(0x135, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x118:
  case 0x3FA:
    if (target != nullptr) {
      target->LaunchEffect(0xFF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x119:
  case 0x3F0:
    if (target != nullptr) {
      target->LaunchEffect(0x100, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x11A:
  case 0x3FB:
    if (target != nullptr) {
      target->LaunchEffect(0x101, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x11B:
  case 0x3F9:
    if (target != nullptr) {
      target->LaunchEffect(0x102, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x13D:
  case 0x145:
    if (source != nullptr) {
      source->LaunchEffect(0x34B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x169:
  case 0x2B3:
    if (target != nullptr) {
      target->LaunchEffect(0x177, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1B8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1BD:
  case 0x1BF:
  case 0x1C0:
  case 0x1C1:
  case 0x1C2:
  case 0x1C3:
  case 0x1C4:
  case 0x1C5:
  case 0x1C6:
  case 0x1C7:
  case 0x1C8:
  case 0x1C9:
  case 0x1CA:
  case 0x1CC:
  case 0x1CD:
  case 0x1EE:
  case 0x23C:
  case 0x23D:
  case 0x23E:
  case 0x23F:
    if (target != nullptr) {
      vector3d offset{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = target->LaunchEffect(0x1F7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&offset), 0, 0, 0, 0);
      }
      target->SendMsg(nullptr, 109, 0, 0x1A8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x242:
    if (source != nullptr) {
      source->LaunchEffect(0xDC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2B4:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\2008cat.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2B5:
    if (target != nullptr) {
      target->LaunchEffect(0x2BB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2B6:
    if (target != nullptr) {
      target->LaunchEffect(0x5B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2B7:
    if (source != nullptr) {
      g_soundMgr->PlayWave("dream_device_edq.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2BA:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\wewish.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x2CD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2CC:
  case 0x2CD:
    if (source != nullptr) {
      source->LaunchEffect(0x408, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2CE:
    if (source != nullptr) {
      source->LaunchEffect(0x409, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2CF:
    if (source != nullptr) {
      source->LaunchEffect(0x19, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2D1:
    if (source != nullptr) {
      source->LaunchEffect(0x41D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2D2:
    if (source != nullptr && target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x41C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = source->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x2DA:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_CURSEDCIRCLE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2DF:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\G하울링오브만드라고라.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2E2:
  case 0x7E5:
    if (target != nullptr) {
      target->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\수둔.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2E4:
    g_soundMgr->PlayWave("effect\\wl_jackfrost.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x2D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2E7:
  case 0x7F3:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\HALLUCINATIONWALK.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2EB:
  case 0x2EC:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x459, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x459, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x303:
    if (target != nullptr) {
      target->LaunchEffect(0x640, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x310:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x94E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x3EA:
    if (source != nullptr) {
      source->LaunchEffect(0x257, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x3ED:
    if (target != nullptr) {
      target->LaunchEffect(0x25A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x3F2:
    if (source != nullptr && target != nullptr) {
      source->SetFrameSequence(0x60, 4, 5, 0x60, 4, 1, 0x708, 1);
      g_soundMgr->PlayWave("amon_ra_die01.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (CRagEffect* effect = source->LaunchEffect(0x25E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x3F3:
    if (source != nullptr && target != nullptr) {
      const std::int64_t emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Chup);
      source->SendMsg(nullptr, 160, 0, 0, 0, static_cast<int>(emotion), static_cast<int>(emotion >> 32), 0, 0, 0, 0, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\vallentine.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (CRagEffect* effect = source->LaunchEffect(0x25F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x3F5:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\ef_entry.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x7D1:
    if (source != nullptr) {
      source->LaunchEffect(0x2F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\닌자_던지기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x7EA:
    if (target != nullptr) {
      target->LaunchEffect(0x15, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7EB:
    if (source != nullptr) {
      source->LaunchEffect(0x303, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7EC:
    if (source != nullptr) {
      source->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\t_낙법.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x7ED:
    if (source != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x570, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (!g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(source->render.accountId, 0x571, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case 0x7F4:
    if (source != nullptr) {
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(source->render.accountId, 0x5BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        LaunchRollingCutterEffect(source);
      } else {
        g_effectMgr->Spawn(source->render.accountId, 0x513, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        LaunchRollingCutterEffect(source);
      }
    }
    break;
  case 0x8AA:
    if (target != nullptr) {
      if (source != nullptr) {
        g_soundMgr->PlayWave("effect\\wizard_meteo.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\hunter_blastmine.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\wl_tetravortex.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
      }
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x59C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x598, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x4F6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x4F2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      for (int i = 0; i < 4; ++i) {
        vector3d position = target->render.position;
        position.z -= 5.0f;
        if (i == 0) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 1) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 2) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 3) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        }
        g_effectMgr->Spawn(0, 0x4FE, position.x, position.y, position.z, 0, -1, 0, 0);
      }
    }
    break;
  case 0x8AB:
    if (target != nullptr) {
      if (source != nullptr) {
        g_soundMgr->PlayWave("effect\\wizard_meteo.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\hunter_blastmine.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\wl_tetravortex.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
      }
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x59D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x599, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x4F7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x4F3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      for (int i = 0; i < 4; ++i) {
        vector3d position = target->render.position;
        position.z -= 5.0f;
        if (i == 0) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 1) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 2) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 3) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        }
        g_effectMgr->Spawn(0, 0x4FF, position.x, position.y, position.z, 0, -1, 0, 0);
      }
    }
    break;
  case 0x8AC:
    if (target != nullptr) {
      if (source != nullptr) {
        g_soundMgr->PlayWave("effect\\wizard_meteo.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\hunter_blastmine.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\wl_tetravortex.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
      }
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x59E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x59A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x4F8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x4F4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      for (int i = 0; i < 4; ++i) {
        vector3d position = target->render.position;
        position.z -= 5.0f;
        if (i == 0) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 1) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 2) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 3) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        }
        g_effectMgr->Spawn(0, 0x500, position.x, position.y, position.z, 0, -1, 0, 0);
      }
    }
    break;
  case 0x8AD:
    if (target != nullptr) {
      if (source != nullptr) {
        g_soundMgr->PlayWave("effect\\wizard_meteo.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\hunter_blastmine.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
        g_soundMgr->PlayWave("effect\\wl_tetravortex.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
      }
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x59F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x59B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x4F9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(target->render.accountId, 0x4F5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      for (int i = 0; i < 4; ++i) {
        vector3d position = target->render.position;
        position.z -= 5.0f;
        if (i == 0) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 1) {
          position.x += static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 2) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z -= static_cast<float>(std::max(std::rand() % 35, 10));
        } else if (i == 3) {
          position.x -= static_cast<float>(std::max(std::rand() % 35, 10));
          position.z += static_cast<float>(std::max(std::rand() % 35, 10));
        }
        g_effectMgr->Spawn(0, 0x501, position.x, position.y, position.z, 0, -1, 0, 0);
      }
    }
    break;
  case 0x8B6:
    if (source != nullptr && target != nullptr) {
      if (CRagEffect* effect = source->LaunchEffect(0x2EF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x1D:
  case 0x2B2:
  case 0x7FA:
  case 0x2030:
    if (target != nullptr) {
      target->LaunchEffect(0x25, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x22:
  case 0x2B1:
  case 0x7F9:
  case 0x202F:
    if (target != nullptr) {
      target->LaunchEffect(0x2A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x4C:
  case 0x809:
    if (target != nullptr) {
      target->LaunchEffect(0x57, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x2C3:
  case 0x7DB:
    if (source != nullptr) {
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x2ED, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\ef_soulstrike.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2D0:
  case 0x89C:
    if (source != nullptr && target != nullptr) {
      for (int count = 0; count < 10; ++count) {
        CRagEffect* effect = source->LaunchEffect(0x321, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          vector3d position = source->render.position;
          const short index = static_cast<short>(std::max(std::rand() % 36, 0) + count * 36);
          position.x += g_cosTable[index] * 80.0f;
          position.z += g_sinTable[index] * 80.0f;
          effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
        }
      }
      g_soundMgr->PlayWave("jack_frost.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x2E5:
  case 0x7E4:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7D3:
    if (source != nullptr) {
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7D5:
    if (source != nullptr) {
      float angle = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CameraDesiredYaw() + source->render.angle;
      if (angle >= 360.0f) {
        angle -= 360.0f;
      }
      int effectId;
      if (angle > 0.0f && angle < 90.0f) {
        effectId = 0x69C;
      } else if (angle >= 90.0f && angle < 180.0f) {
        effectId = 0x69B;
      } else if (angle >= 180.0f && angle < 270.0f) {
        effectId = 0x69C;
      } else {
        effectId = 0x69B;
      }
      g_effectMgr->Spawn(source->render.accountId, 0x69A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x7D6:
    g_soundMgr->PlayWave("effect\\wl_jackfrost.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x658, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x659, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x7D9:
    if (source != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      source->LaunchEffect(0x2DB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("dragon_h.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x7DD:
    if (source != nullptr) {
      source->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x7DE:
    if (source != nullptr) {
      source->LaunchEffect(0x2F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\닌자_던지기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x7DF:
  case 0x7E2:
    if (source != nullptr) {
      source->LaunchEffect(0x2F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7E1:
    if (source != nullptr) {
      source->LaunchEffect(0x11, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x107, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x14A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7E3:
    if (source != nullptr) {
      source->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\흡기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x7F7:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\AB_ancilla.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7FD:
    if (target != nullptr) {
      target->LaunchEffect(0x70, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7FE:
    if (source != nullptr) {
      source->LaunchEffect(0x2D5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7FF:
  case 0x800:
    if (target != nullptr) {
      target->LaunchEffect(0x42, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("lauagnus.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x802:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\AB_renovatio.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x2CF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x804:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x615, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x616, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x805:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\바실리카.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x806:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\AB_duplelight.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (!target->HasId(0x341)) {
        CRagEffect* effect = target->LaunchEffect(0x341, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        vector3d scale{2.0f, 0.0f, 0.0f};
        if (effect) {
          effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&scale), 0, 0, 0, 0);
        }
      }
    }
    break;
  case 0x89D:
    if (target != nullptr) {
      target->LaunchEffect(0x2D9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x89E:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\wl_recognizedspell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x89F:
    if (target != nullptr) {
      target->LaunchEffect(0x31D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8A1:
    if (source != nullptr) {
      source->LaunchEffect(0x31F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8BA:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x60D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x8C0:
    g_soundMgr->PlayWave("wolf_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8C1:
    g_soundMgr->PlayWave("wolf_stand.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8D6:
    if (source != nullptr) {
      source->LaunchEffect(0x309, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x90B:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\LG_SHIELDSPELL.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x90E:
    if (source != nullptr) {
      source->LaunchEffect(0x38C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\LG_PRESTIGE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x915:
    if (source != nullptr) {
      source->LaunchEffect(0x38E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x921:
    if (source != nullptr) {
      source->LaunchEffect(0x1B3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_WINDMILL.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x98D:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\S스펠피스트.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_soundMgr->PlayWave("effect\\S파이어워크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_soundMgr->PlayWave("effect\\S일렉트릭워크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x993:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\S스트라이킹.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x997:
    if (target != nullptr) {
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\S포이즌버스터.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      vector3d position{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = target->LaunchEffect(0x3A3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x99D:
    if (source != nullptr) {
      source->LaunchEffect(0x3A6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x9AC:
    if (source != nullptr) {
      source->LaunchEffect(0x555, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(source->render.accountId, 0x5DD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(source->render.accountId, 0x5DE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else {
        g_effectMgr->Spawn(source->render.accountId, 0x555, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        g_effectMgr->Spawn(source->render.accountId, 0x556, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case 0x9AD:
    if (source != nullptr && target != nullptr) {
      const std::uint32_t accountId = target->render.accountId;
      CEffect* effect = g_effectMgr->Spawn(accountId, 0x54D, source->render.position.x, source->render.position.y, source->render.position.z, accountId, -1, 0, 0);
      if (effect) {
        effect->Slot16(200);
      }
      g_soundMgr->PlayWave("effect\\mon_침투경.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x9B0:
    if (source != nullptr && target != nullptr) {
      g_soundMgr->PlayWave("effect\\S바큠익스트림.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (CRagEffect* effect = target->LaunchEffect(0xD8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = source->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x9B1:
    this->LaunchTargetEffect(0x392, source, target, 4);
    break;
  case 0x9B2:
    g_soundMgr->PlayWave("effect\\G가시나무덫.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x9BC:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x667, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x668, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\G하울링오브만드라고라.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x9BD:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (CRagEffect* effect = source->LaunchEffect(0x1B7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
        g_soundMgr->PlayWave("effect\\G스포어익스플로젼.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
    }
    break;
  case 0x9BE:
  case 0x9BF:
  case 0x9C0:
  case 0x9C1:
    if (source != nullptr) {
      source->LaunchEffect(0x38F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x907:
  case 0x909:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\LG_REFLECTDAMAGE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x929:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x92A:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\세크리파이스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x92B:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\샤프슈팅.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x92C:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\mon_탄지신통.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x922:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_RAISINGDRAGON.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x925:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_POWERVELOCITY.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x91D:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_CRESCENTELBOW.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x91F:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_LIGHTNINGWALK.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x91E:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetFrameSequence(96, 3, 3, 96, 3, 1, 3000, 0);
      g_soundMgr->PlayWave("effect\\SR_CURSEDCIRCLE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x926:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      CRagEffect* effect = source->LaunchEffect(0x37C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect != nullptr) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
        g_soundMgr->PlayWave("effect\\SR_CRESCENTELBOW.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
    }
    break;
  case 0x978:
    if (target != nullptr) {
      target->LaunchEffect(0x35E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\AB_ancilla.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x92E:
    if (target != nullptr) {
      target->LaunchEffect(0x34F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\스윙댄스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x92F:
    if (target != nullptr) {
      target->LaunchEffect(0x34F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\연인들을위한심포니.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x930:
    if (target != nullptr) {
      target->LaunchEffect(0x34F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\AB_judex.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x94D:
    if (target != nullptr) {
      target->LaunchEffect(0x350, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\풍차를향해돌격.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x94E:
    if (target != nullptr) {
      target->LaunchEffect(0x350, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\메아리의노래.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x974:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\세이렌의목소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x94F:
    if (target != nullptr && source != nullptr) {
      vector3d position{0.0f, 0.0f, 0.0f};
      position.y = static_cast<float>(target->render.accountId);
      CRagEffect* targetEffect = target->LaunchEffect(0x351, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (targetEffect != nullptr) {
        targetEffect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      position.y = static_cast<float>(source->render.accountId);
      CRagEffect* sourceEffect = source->LaunchEffect(0x351, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (sourceEffect != nullptr) {
        sourceEffect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      g_soundMgr->PlayWave("effect\\하모나이즈.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x8E3:
    if (target != nullptr) {
      target->LaunchEffect(0x311, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8ED:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x631, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x632, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\reproduce.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8EE:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x6C2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x6C3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\autoshadow.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8F1:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\bodypaint.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x32B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8F2:
    g_soundMgr->PlayWave("effect\\invisibility.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8F3:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\deadlyinfect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x32C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8F4:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\enervation.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x32D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8F5:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\groomy.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x32E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8F6:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\ignorance.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x32F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8F7:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\laziness.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x330, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8F8:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\unlucky.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x331, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8F9:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\weakness.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x332, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8FA:
    if (target != nullptr) {
      CRagEffect* effect = target->LaunchEffect(0x334, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect != nullptr) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x8EF: {
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\shadowform.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x33A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    vector3d position{static_cast<float>(sourceAid), static_cast<float>(targetAid), 0.0f};
    if (position.y != 0.0f && position.x != 0.0f && source != nullptr) {
      CRagEffect* effect = source->LaunchEffect(0x333, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect != nullptr) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  }
  case 0x8DD:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\ef_decagility.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8E2:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\t_등록.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x1F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8E1:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\mon_금강불괴.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x30E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8DC:
    if (source != nullptr) {
      source->LaunchEffect(0x30B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8D7:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\HOVERING.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x30A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8D9:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\B_SIDESLIDE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x30A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x8D8:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\F_SIDESLIDE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x30A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x977:
    if (target != nullptr) {
      target->LaunchEffect(0x35D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\순환하는자연의소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x979:
    if (target != nullptr) {
      target->LaunchEffect(0x360, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\AB_renovatio.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x975:
    if (target != nullptr) {
      target->LaunchEffect(0x35D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x355, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\사망의골짜기에서.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x97B:
    if (target != nullptr) {
      target->LaunchEffect(0x364, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\마나의노래.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x97C:
    if (target != nullptr) {
      target->LaunchEffect(0x362, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\워그와함께춤을.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x97E:
    if (target != nullptr) {
      target->LaunchEffect(0x366, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\새터데이나이트피버.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x97F:
    if (target != nullptr) {
      target->LaunchEffect(0x368, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_LIGHTNINGWALK.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x980:
    if (target != nullptr) {
      target->LaunchEffect(0x36A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\멜로디오브싱크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x981:
    if (target != nullptr) {
      target->LaunchEffect(0x36C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\비욘드오브워크라이.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x982:
    if (target != nullptr) {
      target->LaunchEffect(0x36E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\언리미티드허밍보이스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x976:
    target->LaunchEffect(0x3C9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\안식의자장가.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x9AE:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\G카트부스트.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x68B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x68C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x97D:
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x913:
    if (source != nullptr) {
      vector3d position = source->render.position;
      float scale = 1.0f;
      switch (effectLevel) {
      case 1:
      case 2:
        scale = 0.5f;
        break;
      case 3:
      case 4:
        scale = 0.7f;
        break;
      case 5:
        scale = 0.9f;
        break;
      }
      CEffect* effect = g_effectMgr->Spawn(0, 0x6AA, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot22(scale);
      }
      effect = g_effectMgr->Spawn(0, 0x6AB, position.x, position.y, position.z, 0, -1, 0, 0);
      if (effect) {
        effect->Slot22(scale);
      }
    }
    break;
  case 0x9D3:
    if (target != nullptr) {
      if (source != nullptr) {
        g_soundMgr->PlayWave(
          "effect\\priest_suffragium.wav", source->render.position.x - target->render.position.x, 0.0f,
          source->render.position.z - target->render.position.z, 250, 40, 1.0f, 0
        );
      }
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x5AD, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, effectLevel, 0, 0);
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x507, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, effectLevel, 0, 0);
      }
    }
    break;
  case 0x9D5:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x628, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x629, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\SR_HOWLINGOFLION.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x9F9:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x427, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x9FF:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x434, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0xA04:
    if (source != nullptr) {
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      while (source->HasId(0x430)) {
        source->SendMsg(nullptr, 0x6D, 0, 0x430, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
    break;
  case 0xA05:
    if (source != nullptr) {
      source->LaunchEffect(0x27D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_effectMgr->Spawn(0, 0x1B3, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0xA0D:
    if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
      g_effectMgr->Spawn(0, 0x432, skill->render.position.x, skill->render.position.y, skill->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0xA03:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x435, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0xA15:
    if (source != nullptr) {
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_effectMgr->Spawn(source->render.accountId, 0x4B3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      while (source->HasId(0x1BD)) {
        source->SendMsg(nullptr, 0x6D, 0, 0x1BD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
    break;
  case 0xA34:
    if (source != nullptr) {
      source->LaunchEffect(0x4B8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      g_soundMgr->PlayWave(
        "effect\\흡기.wav", target->render.position.x - this->scene->player->render.position.x, 0.0f,
        target->render.position.z - this->scene->player->render.position.z, 250, 40, 1.0f, 0
      );
      target->LaunchEffect(0x4B7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA2D:
    if (target != nullptr) {
      if (source != nullptr) {
        vector3d data{0.0f, static_cast<float>(source->render.accountId), 0.0f};
        if (CRagEffect* effect = target->LaunchEffect(0x4BA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
          effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&data), 0, 0, 0, 0);
        }
      }
      target->LaunchEffect(0x197, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xA24:
  case 0xA25:
  case 0xA26:
  case 0xA27:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x1F7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d data{0.0f, static_cast<float>(target->render.accountId), 0.0f};
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&data), 0, 0, 0, 0);
      }
      target->SendMsg(nullptr, 0x6D, 0, 0x1A8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      switch (skillId) {
      case 0xA24:
        g_effectMgr->Spawn(target->render.accountId, 0x4CC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      case 0xA25:
        g_effectMgr->Spawn(target->render.accountId, 0x4D3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      case 0xA26:
        g_effectMgr->Spawn(target->render.accountId, 0x4D5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      case 0xA27:
        g_effectMgr->Spawn(target->render.accountId, 0x4D4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      }
    }
    break;
  case 0xA21:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x4D7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0xA29:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x4D6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0xA30:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x4DB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave(
        "effect\\t_방어형.wav", target->render.position.x - this->scene->player->render.position.x, 0.0f,
        target->render.position.z - this->scene->player->render.position.z, 250, 40, 1.0f, 0
      );
    }
    break;
  case 0xBC1:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x3D8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x3E8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x3EA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1396:
  case 0x13CF:
    if (source != nullptr) {
      source->LaunchEffect(0x412, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1391:
    if (target != nullptr) {
      target->LaunchEffect(0x413, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x138D:
    if (source != nullptr) {
      source->LaunchEffect(0x414, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1393:
    if (source != nullptr) {
      source->LaunchEffect(0x417, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1392:
    if (source != nullptr && !source->HasId(0x439)) {
      vector3d position = source->render.position;
      if (CRagEffect* effect = source->LaunchEffect(0x439, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        const std::int64_t value = reinterpret_cast<std::intptr_t>(&position);
        effect->SendMsg(nullptr, 0x2C, 0, 0xFFFF, 0x422, 0, static_cast<int>(value), static_cast<int>(value >> 32));
      }
    }
    break;
  case 0x138F:
    if (source != nullptr && g_effectMgr->HasAllocator(0x423)) {
      const float spacing = static_cast<float>(this->scene->terrain->cellSize) * static_cast<float>(level + 2);
      for (int row = 1; row >= -1; --row) {
        for (int column = -1; column <= 1; ++column) {
          vector3d position = source->render.position;
          position.z += static_cast<float>(row) * spacing;
          position.x += static_cast<float>(column) * spacing;
          g_effectMgr->Spawn(0, 0x423, position.x, position.y, position.z, 0, -1, 0, 0);
        }
      }
    }
    break;
  case 0x1390:
    if (source != nullptr) {
      source->LaunchEffect(0x416, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1394:
    if (source != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave(
        "effect\\Wl_Telekinesis_Intense.wav", player->render.position.x - source->render.position.x, 0.0f,
        player->render.position.z - source->render.position.z, 250, 40, 1.0f, 0
      );
    }
    break;
  case 0x13B1:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x44A, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13AE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x44E, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x139D:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x44F, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x139B:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x451, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13A8:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x453, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13AB:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x454, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13A9:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x455, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13AC:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x44B, target->render.position.x, target->render.position.y, target->render.position.z, 0, level, 0, 0);
    }
    break;
  case 0x13B3:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x457, target->render.position.x, target->render.position.y, target->render.position.z, 0, level, 0, 0);
    }
    break;
  case 0x139F:
    if (source != nullptr) {
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      while (source->HasId(0x1BD)) {
        source->SendMsg(nullptr, 0x6D, 0, 0x1BD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(0x19B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x13B0:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x45C, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13B9:
  case 0x13BA:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x463, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13BF:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x464, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13B7:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x466, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13B5:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x467, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13C8:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x475, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x475, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x13C9:
    if (target != nullptr) {
      target->LaunchEffect(0x198, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x476, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case 0x130:
  case 0x13CC:
    if (source != nullptr) {
      source->LaunchEffect(0x350, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x13D4:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\priest_sanctuary.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x557, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x558, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x145A:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x6BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x6BB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x6BC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(100);
      }
      effect = g_effectMgr->Spawn(target->render.accountId, 0x6BD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(100);
      }
    }
    break;
  case 0x145E:
  case 0x1464:
    if (source != nullptr) {
      source->SetFrameSequence(96, 1, 1, 96, 1, 1, 500, 0);
    }
    break;
  case 0x145F:
    g_effectMgr->Spawn(source->render.accountId, 0x6C4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, 0x6C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 0x1469:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x73E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x73F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x6C:
  case 0x14B1:
    if (target != nullptr) {
      target->LaunchEffect(0x65, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1476:
    if (source != nullptr) {
      source->SetFrameSequence(96, 1, 1, 96, 1, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7C3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1477:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 1, 0x60, 1, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7CF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x147A:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 1, 0x60, 1, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7AC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7AD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x147E:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7B9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x7B7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(1000);
      }
    }
    break;
  case 0x147F:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 5, 5, 0x60, 5, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7BC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x7BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(1000);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x7BD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(1000);
      }
    }
    break;
  case 0x1486:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 2, 2, 0x60, 2, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7C0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x7BE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(1000);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x7C1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(1000);
      }
    }
    break;
  case 0x1479:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x147C:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7CB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7CC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->SetSequenceInterval(5);
      source->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, 1100, 0);
    }
    break;
  case 0x147B:
    if (source != nullptr) {
      g_effectMgr->Spawn(0, 0x82E, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
      source->SetSequenceInterval(5);
      source->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, 1100, 0);
    }
    break;
  case 0x1484:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x825, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x826, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1485:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x831, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8E7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1483:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x840, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x841, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1495:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x710, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14A2:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x75F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14A1:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x744, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x149E:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x739, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (!g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x73A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case 0x1497:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x713, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x149B:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x717, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1498:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x715, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14A4:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x789, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x78B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1471:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x6D1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x6D2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1472:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        0, 0x6DB, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
      );
    }
    break;
  case 0x1475:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
      g_effectMgr->Spawn(
        source->render.accountId, 0x6F5, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0
      );
      for (int i = 0; i < 8; ++i) {
        g_effectMgr->Spawn(
          source->render.accountId, i + 0x6F9, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0
        );
      }
    }
    break;
  case 0x14BA:
    g_effectMgr->Spawn(source->render.accountId, 0x6C8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 0x1473:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        source->render.accountId, 0x6DC, source->render.position.x, source->render.position.y, source->render.position.z,
        target->render.accountId, -1, 0, 0
      );
    }
    break;
  case 0x1470:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x709, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14B8:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x76F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x770, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14B6:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x78E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14B7:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x77B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14B9:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x772, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14B3: {
    g_effectMgr->Spawn(targetAid, 0x71A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(targetAid, 0x71B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    CEffect* effect = g_effectMgr->Spawn(targetAid, 0x71C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot16(500);
    }
    effect = g_effectMgr->Spawn(targetAid, 0x719, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot16(1430);
    }
    break;
  }
  case 0x14B2:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x730, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14AF: {
    float scale = 1.0f;
    if (level == 1 || level == 2) {
      scale = 0.7f;
    } else if (level == 5) {
      scale = 1.2f;
    }
    if (source != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x794, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot22(scale);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x795, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot22(scale);
      }
      g_effectMgr->Spawn(
        source->render.accountId, source->render.previousAnimation - source->render.animation + 0x796, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
      );
    }
    break;
  }
  case 0x14B0:
    g_effectMgr->Spawn(targetAid, 0x75C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(targetAid, 0x75D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 0x1480:
    if (target != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x823, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(500);
      }
      effect = g_effectMgr->Spawn(target->render.accountId, 0x824, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(500);
      }
    }
    break;
  case 0x1494: {
    if (source != nullptr) {
      if (!g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(source->render.accountId, 0x727, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      g_effectMgr->Spawn(source->render.accountId, 0x728, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x729, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(500);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x72A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(500);
        effect->AddEvent(0, "effect\\CD_REPARATIO1.wav", 0x1F, 0);
      }
    }
    if (target != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x72B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(800);
        effect->AddEvent(0, "effect\\CD_REPARATIO2.wav", 0x32, 0);
      }
    }
    break;
  }
  case 0x14A0:
    if (source != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x784, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->Slot16(400);
      }
      if (target != nullptr) {
        if (source->render.accountId != target->render.accountId) {
          if (g_effectMgr->Find(target->render.accountId, 0x787) == nullptr) {
            g_effectMgr->Spawn(target->render.accountId, 0x787, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
          }
          if (g_effectMgr->Find(target->render.accountId, 0x788) == nullptr) {
            g_effectMgr->Spawn(target->render.accountId, 0x788, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
          }
        } else if (effect) {
          effect->AddEvent(0, "effect\\CD_DILECTIO_HEAL.wav", 0x19, 0);
        }
      }
    }
    break;
  case 0x14A7:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        source->render.accountId, 0x72D, source->render.position.x, source->render.position.y, source->render.position.z,
        target->render.accountId, -1, 0, 0
      );
    }
    break;
  case 0x149D:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x736, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x737, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14F1:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x84A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14F3:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x81B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1460:
  case 0x146E:
  case 0x14E6:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
    }
    break;
  case 0x14CC:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x76B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14CE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x771, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14A3:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x777, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14A9:
    if (source != nullptr) {
      LaunchRollingCutterEffect(source);
      g_effectMgr->Spawn(source->render.accountId, 0x778, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14AA:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x78D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14C2:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7AE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->Slot15(2, 1);
    }
    break;
  case 0x14D1:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        0, 0x7B3, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
      );
    }
    break;
  case 0x14F8:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7C8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14F7:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7D3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14C4:
    if (target != nullptr) {
      if (source != nullptr) {
        g_effectMgr->Spawn(
          source->render.accountId, 0x7E2, source->render.position.x, source->render.position.y, source->render.position.z,
          target->render.accountId, -1, 0, 0
        );
      }
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave(
        "effect\\ABC_CHAIN_REACTION_SHOT.wav", player->render.position.x - target->render.position.x, 0.0f,
        player->render.position.z - target->render.position.z, 250, 40, 1.0f, 0
      );
    }
    break;
  case 0x14F6:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7E9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14C5:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x83E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 0x2BC, 0);
    }
    break;
  case 0x14C8:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x854, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14CA:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x842, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case 0x150D:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x85B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x85C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1510:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x85E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x85F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1511:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x861, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x862, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x150E:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x864, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x865, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x150F:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x867, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x868, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14DB:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x871, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x872, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14DA:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x88A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x150A:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8BF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14DF:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x89E, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case 0x1508:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8CC, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case 0x1509:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8CA, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case 0x150C:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8D1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14DC:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8A6, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case 0x14DE:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8AE, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case 0x14DD:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8B6, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case 0x14E3:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8C1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x14C1:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x1EF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&target->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case 0x14D8:
    if (source != nullptr) {
      source->LaunchEffect(0x3A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1482:
    if (target != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\IQ_SECOND_JUDGEMENT.wav", player->render.position.x - target->render.position.x, 0.0f, player->render.position.z - target->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case 0x14C3:
    if (target != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\ABC_UNLUCKY_RUSH.wav", player->render.position.x - target->render.position.x, 0.0f, player->render.position.z - target->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case 0x14C6:
    if (source != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\ABC_ABYSS_SLAYER.wav", player->render.position.x - source->render.position.x, 0.0f, player->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case 0x1C:
  case 0x14E:
  case 0x2D9:
  case 0x7FB:
  case 0x803:
  case 0x1F41:
    if (target != nullptr) {
      int effectId;
      if (level >= 4000) {
        effectId = 0x145;
      } else if (level >= 2000) {
        effectId = 0x139;
      } else if (level >= 200) {
        effectId = 0x138;
      } else {
        effectId = 7;
      }
      target->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->SendMsg(nullptr, 0x58, 0, level, level >> 31, static_cast<int>(0xFF00FF00), 0, 0x72, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 0x1F5A:
    if (source != nullptr) {
      source->LaunchEffect(0x3C1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F67:
    if (source != nullptr) {
      source->LaunchEffect(0x3C2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xAD:
  case 0x2D4:
  case 0x8DB:
  case 0x1F50:
    if (source != nullptr) {
      source->LaunchEffect(0xB7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x4A:
  case 0x201E:
    if (source != nullptr) {
      source->LaunchEffect(0x4C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x49:
  case 0x202E:
    if (source != nullptr) {
      source->LaunchEffect(0x70, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x7:
  case 0x200A:
    if (source != nullptr) {
      source->LaunchEffect(0x11, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0xF9:
  case 0x164:
  case 0x200C:
  case 0x201C:
    if (source != nullptr) {
      source->SendMsg(nullptr, 0x7E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    break;
  case 0x101:
  case 0x201B:
    if (source != nullptr) {
      source->LaunchEffect(0xDE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x167:
  case 0x168:
  case 0x200E:
    if (source != nullptr) {
      source->SendMsg(nullptr, 0x6D, 0, 0x170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x170, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\버서크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x1F42:
    if (source != nullptr) {
      source->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      target->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F4A:
  case 0x1F4B:
    if (source != nullptr) {
      source->LaunchEffect(0x234, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F46:
    g_soundMgr->PlayWave("effect\\h_defence.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x239, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      target->LaunchEffect(0x239, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F45:
    g_soundMgr->PlayWave("effect\\h_castling.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x23A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      target->LaunchEffect(0x23A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F48:
    if (source != nullptr) {
      source->LaunchEffect(0x23B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F75:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x910, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x911, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1F79:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x91B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x91C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1F7A:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x91E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1F73:
    if (source != nullptr) {
      source->LaunchEffect(0x922, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_effectMgr->Spawn(source->render.accountId, 0x923, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x1F6D:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x924, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x20FB:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x219B:
    if (target != nullptr) {
      target->LaunchEffect(0x139, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (source != nullptr && g_effectMgr->Find(source->render.accountId, 0x76F) == nullptr) {
        g_effectMgr->Spawn(source->render.accountId, 0x76F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case 0x219C:
    if (target != nullptr) {
      target->LaunchEffect(0xFD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (source != nullptr && g_effectMgr->Find(source->render.accountId, 0x770) == nullptr) {
        g_effectMgr->Spawn(source->render.accountId, 0x770, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case 0x2108: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7DD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->unknown_02EC);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x7DD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_SERPENS_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case 0x2107:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x20FF: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7E8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->unknown_02EC);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x7E8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_DILUVIO_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case 0x20FE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x20FC: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7F8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->unknown_02EC);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x7F8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_ARDOR_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case 0x2102: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x837, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->unknown_02EC);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x837, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_PROCELLA_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case 0x2101:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x2105: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x83C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->unknown_02EC);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x83C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_TERREMOTUS_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case 0x2104:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8DA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8DB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0xC:
  case 0xF:
  case 0x12:
  case 0x18:
  case 0x108:
  case 0x259:
  case 0x8AE:
  case 0x8AF:
  case 0x8B0:
  case 0x8B5:
  case 0x900:
    break;
  default:
    LaunchLuaSkillEffect(skillId, level, source, target);
    CSkillActionMgr::GetInstance()->ActionSlot11(skillId, level, source, target);
    break;
  }
}

#pragma pack(push, 1)
struct CameraInfoPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ unsigned char action;
  /* 0003 */ float distance;
  /* 0007 */ float yaw;
  /* 000B */ float pitch;
};
#pragma pack(pop)

static_assert(sizeof(CameraInfoPacket) == 0x0F);
static_assert(offsetof(CameraInfoPacket, distance) == 3);
static_assert(offsetof(CameraInfoPacket, yaw) == 7);
static_assert(offsetof(CameraInfoPacket, pitch) == 0x0B);

// OFFSET: 0x00D08820
// FUNCTION: fn_45485
void CGameMode::HandleCameraInfo(const unsigned char* data) {
  const CameraInfoPacket* packet = reinterpret_cast<const CameraInfoPacket*>(data);
  if (packet->action == 1) {
    UIWindow* window = g_windowMgr.LookupWindow(289);
    if (window == nullptr) {
      window = g_windowMgr.sub_A978D0(289);
    }
    g_session.unknown_61B1 = g_session.unknown_61B1 == 0;
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_session.unknown_61B1 == 1 ? "Camera Info On" : "Camera Info Off"), 0x64FFFF, 0, 0, 0);
    window->Show(g_session.unknown_61B1);
  } else if (this->camera != nullptr) {
    this->camera->camera.desiredDistance = packet->distance;
    this->camera->camera.desiredYaw = packet->yaw;
    this->camera->camera.desiredPitch = packet->pitch;
  }
}

#pragma pack(push, 1)
struct ActorStopPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::uint16_t x;
  /* 0008 */ std::uint16_t y;
};

struct ActorActionPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t sourceAid;
  /* 0006 */ std::uint32_t targetAid;
  /* 000A */ std::uint32_t tick;
  /* 000E */ int attackMT;
  /* 0012 */ int attackedMT;
  /* 0016 */ std::int16_t damage;
  /* 0018 */ std::uint16_t count;
  /* 001A */ unsigned char type;
  /* 001B */ std::int16_t damage2;
};

struct ActorRestPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t sourceAid;
  /* 0006 */ std::uint32_t targetAid;
  /* 000A */ std::uint32_t tick;
  /* 000E */ std::int16_t attackMT;
  /* 0010 */ std::int16_t attackedMT;
  /* 0012 */ std::int16_t damage;
  /* 0014 */ std::int16_t count;
  /* 0016 */ unsigned char type;
};

struct ActorNamePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ char name[24];
};
struct ActorDirectionPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::int16_t headDirection;
  /* 0008 */ unsigned char direction;
};

struct NormalItemRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ unsigned char identified;
  /* 0008 */ std::int16_t amount;
  /* 000A */ std::uint16_t location;
};

struct CardItemRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ unsigned char identified;
  /* 0008 */ std::int16_t amount;
  /* 000A */ std::uint16_t location;
  /* 000C */ int cards[4];
};

struct CardItemExpiryRecord {
  /* 0000 */ CardItemRecord base;
  /* 001C */ std::uint32_t expireTime;
};

struct FlaggedItemRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::int16_t amount;
  /* 0009 */ std::uint16_t location;
  /* 000B */ int cards[4];
  /* 001B */ std::uint32_t expireTime;
  /* 001F */ unsigned char flags;
};

struct FlaggedEquipRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ std::uint16_t location;
  /* 0009 */ std::uint16_t wearState;
  /* 000B */ unsigned char refine;
  /* 000C */ int cards[4];
  /* 001C */ std::uint32_t expireTime;
  /* 0020 */ std::uint16_t bindOnEquip;
  /* 0022 */ std::uint16_t look;
  /* 0024 */ unsigned char flags;
};

struct EquipItemRecord {
  /* 0000 */ std::int16_t index;
  /* 0002 */ int itemId;
  /* 0006 */ unsigned char type;
  /* 0007 */ unsigned char identified;
  /* 0008 */ std::uint16_t location;
  /* 000A */ std::uint16_t wearState;
  /* 000C */ unsigned char damaged;
  /* 000D */ unsigned char refine;
  /* 000E */ unsigned char cards[16];
};

struct ItemUseAckPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t index;
  /* 0004 */ std::int16_t amount;
  /* 0006 */ unsigned char result;
};
struct ShopItemRecord {
  /* 0000 */ std::uint32_t price;
  /* 0004 */ std::uint32_t discountPrice;
  /* 0008 */ unsigned char type;
  /* 0009 */ std::int32_t itemId;
};
struct SkillListRecord {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ int type;
  /* 0006 */ std::int16_t level;
  /* 0008 */ std::int16_t spCost;
  /* 000A */ std::int16_t range;
  /* 000C */ char name[24];
  /* 0024 */ unsigned char upgradable;
};
#pragma pack(pop)

struct ActorMoveRoute {
  /* 0000 */ std::uint32_t tick;
  /* 0004 */ int srcX;
  /* 0008 */ int srcY;
  /* 000C */ int subX;
  /* 0010 */ int subY;
  /* 0014 */ int dstX;
  /* 0018 */ int dstY;
};

#pragma pack(push, 1)
struct ItemSlotOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};

struct ItemSlotOptions {
  /* 0000 */ ItemSlotOption options[5];
};
#pragma pack(pop)

struct ChatRoomMemberUpdateInfo {
  /* 0000 */ std::string title;
  /* 0018 */ std::string unknown_0018;
  /* 0030 */ int type;
  /* 0034 */ int memberCount;
  /* 0038 */ int memberLimit;
  /* 003C */ int id;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ChatRoomMemberUpdateInfo) == 0x40);
static_assert(offsetof(ChatRoomMemberUpdateInfo, unknown_0018) == 0x18);
static_assert(offsetof(ChatRoomMemberUpdateInfo, type) == 0x30);
#endif

// OFFSET: 0x00D08910
// FUNCTION: fn_21595
void CGameMode::sub_D08910() {
  memset(g_gameModePacketBuffer, 0, sizeof(g_gameModePacketBuffer));
  if (!g_replayAnimationScaling) {
    const std::uint32_t elapsedHours = (timeGetTime() - g_loginAcceptTime) / 3600000;
    if (!GetRagConnection().Update()) {
      g_modeMgr.gameMode->SendMsg(1, 0, 0, 0, 0);
    }

    if (g_charServerAccountIdPending == 1) {
      int accountId;
      if (GetRagConnection().Read(&accountId, 4, 1)) {
        g_charServerAccountIdPending = 0;
      }
      return;
    }
  }

  const int replayState = Replay::Instance().state;
  if (replayState == 7) {
    g_windowMgr.window_4F168->SendMsg(nullptr, 60, 0, 0, 0, 0);
  } else if (replayState == 2) {
    Replay::Instance().WriteHeader(12);
    if (g_windowMgr.window_4F164) {
      g_windowMgr.window_4F164->SendMsg(nullptr, 116, 0, 0, 0, 0);
    }
  }

  unsigned short size = 0;
  std::uint16_t type = 0;
  for (;;) {
    bool received;
    if (!g_replayAnimationScaling) {
      received = GetRagConnection().ReadPacket(g_gameModePacketBuffer, &size, g_packetLengths);
      if (received) {
        type = GetRagConnection().GetPacketType(reinterpret_cast<const char *>(g_gameModePacketBuffer));
        if (replayState == 2) {
          Replay::Instance().Record(size, g_gameModePacketBuffer, 42, 0);
        }
      }
      Replay::Instance().CachePacket(type, size, g_gameModePacketBuffer);
    } else {
      received = Replay::Instance().ReadRecord(g_gameModePacketBuffer, type, false);
    }
    if (!received) {
      return;
    }

    g_session.m_lastReceiveTick = timeGetTime();
    switch (static_cast<short>(type)) {
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x7A:
    case 0x83:
    case 0x84:
    case 0x98:
    case 0x164:
    case 0x187:
    case 0x1CC:
    case 0x1E0:
    case 0x1E9:
    case 0x227:
    case 0x283:
    case 0x851:
    case 0x853:
    case 0x855:
    case 0x90D:
    case 0x9B7:
    case 0x9B9:
    case 0x9CC:
    case 0x9EB:
    case 0xA7B:
    case 0xAB1:
    case 0xAD2:
    case 0xAD3:
    case 0xAD4:
    case 0xAD6:
    case 0xAD9:
    case 0xB00:
    case 0xB1D:
    case 0xC20:
    case 0xC29:
    case 0xC35:
      break;
    case 0x78:
      this->ReceiveActorTypedEntry(g_gameModePacketBuffer);
      break;
    case 0x79:
      this->ReceiveActorStandEntryOld0(g_gameModePacketBuffer);
      break;
    case 0x7B:
      this->ReceiveActorMoveEntryOld0(g_gameModePacketBuffer);
      break;
    case 0x7C:
      this->ReceiveNpcStandEntry(g_gameModePacketBuffer);
      break;
    case 0x80:
      this->ReceiveActorDisappear(g_gameModePacketBuffer);
      break;
    case 0x81:
      this->ReceiveNotifyBan(g_gameModePacketBuffer);
      break;
    case 0x8D:
      this->ReceiveActorChat(g_gameModePacketBuffer);
      break;
    case 0x8E:
      this->ReceiveBroadcast(g_gameModePacketBuffer);
      break;
    case 0x91:
      this->ReceiveMapMove(g_gameModePacketBuffer);
      break;
    case 0x92:
      this->ReceiveZoneServerChange(g_gameModePacketBuffer);
      break;
    case 0x97:
      this->sub_D48530(g_gameModePacketBuffer);
      break;
    case 0xA0:
      this->ReceiveItemPickupAck(g_gameModePacketBuffer);
      break;
    case 0xB0:
      this->ReceiveParChange(g_gameModePacketBuffer);
      break;
    case 0xB1:
      this->ReceiveExpChange(g_gameModePacketBuffer);
      break;
    case 0xB6:
      this->ReceiveUnknownD264E0(g_gameModePacketBuffer);
      break;
    case 0xB7:
      this->ReceiveWindow17Entries(g_gameModePacketBuffer);
      break;
    case 0xBC:
      this->ReceiveStatusChangeAck(g_gameModePacketBuffer);
      break;
    case 0xBD:
      this->ReceiveStatus(g_gameModePacketBuffer);
      break;
    case 0xBE:
      this->ReceiveStatusChange(g_gameModePacketBuffer);
      break;
    case 0xC3:
      this->ReceiveSpriteChange(g_gameModePacketBuffer);
      break;
    case 0xC7:
      this->ReceiveItem1718IndexList(g_gameModePacketBuffer);
      break;
    case 0xCA:
      this->ReceiveItem1718Result(g_gameModePacketBuffer);
      break;
    case 0xD1:
      this->ReceiveResultNotice0118(g_gameModePacketBuffer);
      break;
    case 0xD7:
      this->ReceiveActorMessage32(g_gameModePacketBuffer);
      break;
    case 0xDA:
      this->ReceiveResultNotice00FC(g_gameModePacketBuffer);
      break;
    case 0xDB:
      this->ReceiveChatRoomMemberList(g_gameModePacketBuffer);
      break;
    case 0xDF:
      this->ReceiveActorMessage34(g_gameModePacketBuffer);
      break;
    case 0xE7:
      this->ReceiveExchangeRequestAck(g_gameModePacketBuffer);
      break;
    case 0xEA:
      this->ReceiveExchangeItemAddAck(g_gameModePacketBuffer);
      break;
    case 0xFB:
      this->ReceivePartyMemberList(g_gameModePacketBuffer);
      break;
    case 0xFD:
    case 0x2C5:
      this->ReceiveNamedActionResult(g_gameModePacketBuffer);
      break;
    case 0x104:
      this->ReceivePartyMemberInfo(g_gameModePacketBuffer);
      break;
    case 0x105:
      this->ReceivePartyMemberLeft(g_gameModePacketBuffer);
      break;
    case 0x109:
      this->ReceivePartyChat(g_gameModePacketBuffer);
      break;
    case 0x10E:
    case 0x239:
    case 0x29E:
      this->ApplySkillUpdatePacket(g_gameModePacketBuffer);
      break;
    case 0x114:
      this->ReceiveActorSkillAction(g_gameModePacketBuffer);
      break;
    case 0x115:
      this->ReceiveActorSkillMessage(g_gameModePacketBuffer);
      break;
    case 0x117:
      this->ReceiveNotifyGroundSkill(g_gameModePacketBuffer);
      break;
    case 0x11F:
      this->sub_D41250(g_gameModePacketBuffer);
      break;
    case 0x133:
    case 0x800:
      this->ReceiveItem1748List(g_gameModePacketBuffer);
      break;
    case 0x135:
      this->ReceiveEquipItemNotice(g_gameModePacketBuffer);
      break;
    case 0x136:
      this->ReceiveUnknownD50810(g_gameModePacketBuffer);
      break;
    case 0x13B:
      this->ReceiveEquipAck(g_gameModePacketBuffer);
      break;
    case 0x141:
      this->ReceiveStatusBonus(g_gameModePacketBuffer);
      break;
    case 0x14C:
      this->ReceiveGuildRelationList(g_gameModePacketBuffer);
      break;
    case 0x150:
      this->ReceiveGuildInfo2(g_gameModePacketBuffer);
      break;
    case 0x154:
      this->ReceiveGuildMemberMemoList(g_gameModePacketBuffer);
      break;
    case 0x15A:
      this->ReceiveGuildMemberLeft(g_gameModePacketBuffer);
      break;
    case 0x160:
      this->ReceiveGuildPositionInfo(g_gameModePacketBuffer);
      break;
    case 0x163:
      this->ReceiveGuildBanList(g_gameModePacketBuffer);
      break;
    case 0x166:
      this->ReceiveGuildPositionNames(g_gameModePacketBuffer);
      break;
    case 0x16F:
      this->ReceiveGuildNotice(g_gameModePacketBuffer);
      break;
    case 0x174:
      this->ReceiveGuildPositionUpdate(g_gameModePacketBuffer);
      break;
    case 0x17D:
      this->ReceiveCardCompositionResult(g_gameModePacketBuffer);
      break;
    case 0x17F:
      this->ReceiveGuildChat(g_gameModePacketBuffer);
      break;
    case 0x182:
      this->ReceiveGuildMemberAdd(g_gameModePacketBuffer);
      break;
    case 0x185:
      this->ReceiveAddRelatedGuild(g_gameModePacketBuffer);
      break;
    case 0x188:
      this->ReceiveRefineResult(g_gameModePacketBuffer);
      break;
    case 0x18F:
      this->ReceiveItemEffectResult(g_gameModePacketBuffer);
      break;
    case 0x195:
      this->ReceiveActorNameAll(reinterpret_cast<const ActorNameAllPacket *>(g_gameModePacketBuffer));
      break;
    case 0x196:
      this->ReceiveStatusChangeNotice(g_gameModePacketBuffer);
      break;
    case 0x19A:
      this->ReceiveEffectNumber(g_gameModePacketBuffer);
      break;
    case 0x19B:
      this->ReceivePartyMemberEffect(g_gameModePacketBuffer);
      break;
    case 0x19E:
      this->Zc_Start_Capture(g_gameModePacketBuffer);
      break;
    case 0x1A0:
      this->Zc_Trycapture_Monster(g_gameModePacketBuffer);
      break;
    case 0x1A3:
    case 0x22F:
      this->ReceiveFeedResult(g_gameModePacketBuffer);
      break;
    case 0x1A4:
      this->ReceivePetChangeState(g_gameModePacketBuffer);
      break;
    case 0x1AA:
      this->ReceivePetAct(g_gameModePacketBuffer);
      break;
    case 0x1B6:
      this->ReceiveGuildInfo(g_gameModePacketBuffer);
      break;
    case 0x1B9:
      this->ReceiveDispel(g_gameModePacketBuffer);
      break;
    case 0x1C1:
      this->ReceiveBillingInfo(g_gameModePacketBuffer);
      break;
    case 0x1C4:
      this->ReceiveItem1738Add(g_gameModePacketBuffer);
      break;
    case 0x1C5:
      this->ReceiveItem1740Add(g_gameModePacketBuffer);
      break;
    case 0x1C9:
      this->sub_D40730(g_gameModePacketBuffer);
      break;
    case 0x1CF:
      this->ReceiveDevotionList(g_gameModePacketBuffer);
      break;
    case 0x1D1:
      this->ReceiveActorPairFreeze(g_gameModePacketBuffer);
      break;
    case 0x1D2:
    case 0xC36:
      this->ReceiveActorSequenceDeadline(g_gameModePacketBuffer);
      break;
    case 0x1D3:
      this->ReceiveSound(g_gameModePacketBuffer);
      break;
    case 0x1D7:
      this->ReceiveSpriteChange2Mirrored(g_gameModePacketBuffer);
      break;
    case 0x1D8:
      this->ReceiveActorStateEntryOld(g_gameModePacketBuffer);
      break;
    case 0x1D9:
      this->ReceiveActorStandEntryOld(g_gameModePacketBuffer);
      break;
    case 0x1DA:
      this->ReceiveActorMoveEntryOld(g_gameModePacketBuffer);
      break;
    case 0x1DE:
      this->ReceiveNotifySkill(g_gameModePacketBuffer);
      break;
    case 0x1E1:
      this->Zc_Spirits2(g_gameModePacketBuffer);
      break;
    case 0x1E4:
      this->Zc_Start_Couple(g_gameModePacketBuffer);
      break;
    case 0x1E6:
      this->ReceiveUnknown0294Text(g_gameModePacketBuffer);
      break;
    case 0x1EA:
      this->ReceiveActorEffect347(g_gameModePacketBuffer);
      break;
    case 0x1F3:
      this->ReceiveNotifyEffect(g_gameModePacketBuffer);
      break;
    case 0x1F5:
      this->ReceiveExchangeRequestAck2(g_gameModePacketBuffer);
      break;
    case 0x1F8:
      this->Zc_Start_Baby(g_gameModePacketBuffer);
      break;
    case 0x201:
      this->ReceiveFriendList(g_gameModePacketBuffer);
      break;
    case 0x20E:
      this->ReceiveSpecialBroadcast(g_gameModePacketBuffer);
      break;
    case 0x214:
      this->ReceiveStatDump(g_gameModePacketBuffer);
      break;
    case 0x215:
      this->ReceiveStatusNotice(g_gameModePacketBuffer);
      break;
    case 0x223:
      this->ReceiveItemObtainNotice(g_gameModePacketBuffer);
      break;
    case 0x22A:
      this->ReceiveActorStandEntry(g_gameModePacketBuffer);
      break;
    case 0x22B:
      this->ReceiveActorStandEntryOld2(g_gameModePacketBuffer);
      break;
    case 0x22C:
      this->ReceiveActorMoveEntryOld2(g_gameModePacketBuffer);
      break;
    case 0x22E:
      this->ReceiveHomunculusInfo(g_gameModePacketBuffer);
      break;
    case 0x230:
      this->ReceiveHomunculusChange(g_gameModePacketBuffer);
      break;
    case 0x27A:
      this->ReceiveHuntingList(g_gameModePacketBuffer);
      break;
    case 0x27B:
      this->ReceiveBroadcastBalloonNotice(g_gameModePacketBuffer);
      break;
    case 0x284:
      this->ReceiveActorStatusValuePacket(g_gameModePacketBuffer);
      break;
    case 0x287:
      this->ReceivePendingItem1718List(g_gameModePacketBuffer);
      break;
    case 0x289:
      this->ReceivePendingResult(g_gameModePacketBuffer);
      break;
    case 0x28A:
      this->ReceiveUnknownD78010(g_gameModePacketBuffer);
      break;
    case 0x291:
      this->Zc_Msg(g_gameModePacketBuffer);
      break;
    case 0x293:
      this->ReceiveUnknown05C4Result(g_gameModePacketBuffer);
      break;
    case 0x294:
      this->ReceiveBookOpen(g_gameModePacketBuffer);
      break;
    case 0x295:
      this->ReceiveEquipItemList34(g_gameModePacketBuffer);
      break;
    case 0x296:
      this->ReceiveStorageEquipList(g_gameModePacketBuffer);
      break;
    case 0x297:
      this->ReceiveItem1740BasicList(g_gameModePacketBuffer);
      break;
    case 0x298:
      this->ReceiveRentalItemTime(g_gameModePacketBuffer);
      break;
    case 0x299:
      this->ReceiveUnknownD25850(g_gameModePacketBuffer);
      break;
    case 0x29A:
      this->ReceiveItemPickupAck2(g_gameModePacketBuffer);
      break;
    case 0x29B:
      this->Zc_Mer_Init(g_gameModePacketBuffer);
      break;
    case 0x2A2:
      this->Zc_Mer_Par_Change(g_gameModePacketBuffer);
      break;
    case 0x2B8:
      this->ReceivePartyItemPickupNotice(g_gameModePacketBuffer);
      break;
    case 0x2BB:
      this->ReceiveEquipItemDamaged(g_gameModePacketBuffer);
      break;
    case 0x2C1:
      this->ReceiveActorColorChat(g_gameModePacketBuffer);
      break;
    case 0x2C2:
      this->ReceiveFormatStringMessage(g_gameModePacketBuffer);
      break;
    case 0x2C9:
      this->ReceiveToggleNotice(g_gameModePacketBuffer);
      break;
    case 0x2CB:
      this->ReceiveMemorialDungeonSubscriptionInfo(g_gameModePacketBuffer);
      break;
    case 0x2CC:
      this->ReceiveMemorialDungeonSubscriptionNotify(g_gameModePacketBuffer);
      break;
    case 0x2CD:
      this->ReceiveMemorialDungeonInfo(g_gameModePacketBuffer);
      break;
    case 0x2CE:
      this->ReceiveMemorialDungeonNotify(g_gameModePacketBuffer);
      break;
    case 0x2D0:
      this->ReceiveEquipItemList38(g_gameModePacketBuffer);
      break;
    case 0x2D1:
      this->ReceiveStorageEquipList2(g_gameModePacketBuffer);
      break;
    case 0x2D2:
      this->ReceiveItem1740BasicListBind(g_gameModePacketBuffer);
      break;
    case 0x2D4:
      this->ReceiveItemPickupAck3(g_gameModePacketBuffer);
      break;
    case 0x2D5:
      this->ReceiveUnknownD2D990(g_gameModePacketBuffer);
      break;
    case 0x2D7:
    case 0x859:
      this->ReceiveEquipWindowMicroscope2(g_gameModePacketBuffer);
      break;
    case 0x2D9:
      this->ReceiveFeedOption(g_gameModePacketBuffer);
      break;
    case 0x2DA:
      this->ReceiveUnknownD26800(g_gameModePacketBuffer);
      break;
    case 0x2DC:
      this->ReceiveActorBroadcastText(g_gameModePacketBuffer);
      break;
    case 0x2EB:
      this->ReceiveAcceptEnter(g_gameModePacketBuffer);
      break;
    case 0x2EC:
    case 0x7F7:
    case 0x856:
      this->ReceiveActorMoveEntryNamed(g_gameModePacketBuffer);
      break;
    case 0x2ED:
      this->ReceiveActorStandEntryOld3(g_gameModePacketBuffer);
      break;
    case 0x2EE:
      this->ReceiveActorStandEntryFont(g_gameModePacketBuffer);
      break;
    case 0x43D:
      this->ApplyCooldownStart(g_gameModePacketBuffer);
      break;
    case 0x43E:
      this->LoadCooldownRecords(g_gameModePacketBuffer);
      break;
    case 0x43F:
      this->ReceiveStatusChangeExtended(g_gameModePacketBuffer);
      break;
    case 0x440:
      this->ReceiveActorEffect228(g_gameModePacketBuffer);
      break;
    case 0x446:
      this->ReceiveMinimapMarker(g_gameModePacketBuffer);
      break;
    case 0x7D8:
      this->ReceiveServerRateNotice(g_gameModePacketBuffer);
      break;
    case 0x7DB:
      this->ReceiveHomunculusProperty(g_gameModePacketBuffer);
      break;
    case 0x7E2:
      this->ReceiveFormattedMessage(g_gameModePacketBuffer);
      break;
    case 0x7E3:
      this->ReceiveTradeItemFilter(g_gameModePacketBuffer);
      break;
    case 0x7F6:
      this->Zc_Notify_Exp(g_gameModePacketBuffer);
      break;
    case 0x7F8:
    case 0x858:
      this->ReceiveActorStandEntryNamedOld(g_gameModePacketBuffer);
      break;
    case 0x7F9:
    case 0x857:
      this->ReceiveActorStateEntryNamedOld(g_gameModePacketBuffer);
      break;
    case 0x7FA:
      this->ReceiveItemConsumed(g_gameModePacketBuffer);
      break;
    case 0x7FC:
      this->ReceivePartyLeaderChange(g_gameModePacketBuffer);
      break;
    case 0x7FE:
      this->ReceiveNpcBgm(g_gameModePacketBuffer);
      break;
    case 0x803:
      this->ReceivePartyBookingUpdateResult(g_gameModePacketBuffer);
      break;
    case 0x805:
      this->ReceivePartyBookingSearchResult(g_gameModePacketBuffer);
      break;
    case 0x807:
      this->ReceivePartyBookingRegisterResult(g_gameModePacketBuffer);
      break;
    case 0x809:
      this->ReceivePartyBookingInsert(g_gameModePacketBuffer);
      break;
    case 0x80A:
      this->ReceivePartyBookingUpdate(g_gameModePacketBuffer);
      break;
    case 0x80B:
      this->ReceivePartyBookingDelete(g_gameModePacketBuffer);
      break;
    case 0x810:
      this->ReceiveOpenStore(g_gameModePacketBuffer);
      break;
    case 0x812:
      this->ReceiveBuyingStoreOpenResult(g_gameModePacketBuffer);
      break;
    case 0x813:
      this->ReceiveItem1758List(g_gameModePacketBuffer);
      break;
    case 0x814:
      this->ReceiveActorText(g_gameModePacketBuffer);
      break;
    case 0x816:
      this->ReceiveActorSignal(g_gameModePacketBuffer);
      break;
    case 0x818:
      this->ReceiveBuyingStoreItemList(g_gameModePacketBuffer);
      break;
    case 0x81A:
      this->ReceiveUnknownD2A1F0(g_gameModePacketBuffer);
      break;
    case 0x81B:
      this->ReceiveVendingItemSoldNow(g_gameModePacketBuffer);
      break;
    case 0x81C:
      this->ReceiveItemSellResult(g_gameModePacketBuffer);
      break;
    case 0x81D:
      this->ReceiveActorStatusBlock(g_gameModePacketBuffer);
      break;
    case 0x81E:
      this->ReceiveActorStatusUpdate(g_gameModePacketBuffer);
      break;
    case 0x81F:
      this->ReceiveColoredNotice(g_gameModePacketBuffer);
      break;
    case 0x824:
      this->ReceiveBuyingStoreSellResult(g_gameModePacketBuffer);
      break;
    case 0x836:
      this->ReceiveSearchStoreItemsOld(g_gameModePacketBuffer);
      break;
    case 0x837:
      this->ReceiveWindow181Message(g_gameModePacketBuffer);
      break;
    case 0x839:
      this->ReceiveGuildMemberExpelled(g_gameModePacketBuffer);
      break;
    case 0x83A:
      this->ReceiveSearchStoreOpen(g_gameModePacketBuffer);
      break;
    case 0x849:
      this->ReceiveRequestResult(g_gameModePacketBuffer);
      break;
    case 0x8AE:
      this->ProcessUiMessage(g_gameModePacketBuffer);
      break;
    case 0x8B1:
      this->ReceiveUnknownD31A40(g_gameModePacketBuffer);
      break;
    case 0x8B3:
      this->ReceiveActorMessage7(g_gameModePacketBuffer);
      break;
    case 0x8B4:
      this->Zc_Start_Collection(g_gameModePacketBuffer);
      break;
    case 0x8B6:
      this->Zc_Trycollection(g_gameModePacketBuffer);
      break;
    case 0x8C7:
      this->ReceiveGroundSkill8(g_gameModePacketBuffer);
      break;
    case 0x8CA:
      this->ReceiveTypedItemList(g_gameModePacketBuffer);
      break;
    case 0x8CB:
    case 0x97B:
      this->ReceiveRateInfo(g_gameModePacketBuffer);
      break;
    case 0x8CE:
      this->ReceiveNotice1950(g_gameModePacketBuffer);
      break;
    case 0x8CF:
      this->ReceiveActorCharm(g_gameModePacketBuffer);
      break;
    case 0x8D2:
      this->ReceiveActorMessage144(g_gameModePacketBuffer);
      break;
    case 0x8D6:
      this->RefreshWindow16(g_gameModePacketBuffer);
      break;
    case 0x8FF:
      this->ReceiveStatusChangeEffect(g_gameModePacketBuffer);
      break;
    case 0x901:
      this->ReceiveEquipItemListFlags(g_gameModePacketBuffer);
      break;
    case 0x906:
      this->ReceiveEquipWindowMicroscope(g_gameModePacketBuffer);
      break;
    case 0x908:
      this->ReceiveInventoryItemUpdate(g_gameModePacketBuffer);
      break;
    case 0x90F:
      this->ReceiveActorStandEntryLegacy(g_gameModePacketBuffer);
      break;
    case 0x914:
      this->ReceiveActorMoveEntryLegacy(g_gameModePacketBuffer);
      break;
    case 0x915:
      this->ReceiveActorStateEntryLegacy(g_gameModePacketBuffer);
      break;
    case 0x96B:
      this->ReceiveUnknown16FC588(g_gameModePacketBuffer);
      break;
    case 0x96D:
      this->ReceiveWindow227Entries(g_gameModePacketBuffer);
      break;
    case 0x96F:
      this->ReceiveWindow227Result(g_gameModePacketBuffer);
      break;
    case 0x975:
      this->ReceiveStorageNormalList(g_gameModePacketBuffer);
      break;
    case 0x976:
      this->ReceiveStorageEquipList3(g_gameModePacketBuffer);
      break;
    case 0x977:
      this->ReceiveActorInt64Pair(g_gameModePacketBuffer);
      break;
    case 0x97D:
      this->ReceiveRankingList(g_gameModePacketBuffer);
      break;
    case 0x97E:
      this->ReceiveTypedNotice(g_gameModePacketBuffer);
      break;
    case 0x97F:
      this->ReceiveWindow95Data(g_gameModePacketBuffer);
      break;
    case 0x983:
      this->ReceiveStatusChangeEffect2(g_gameModePacketBuffer);
      break;
    case 0x984:
      this->sub_D65FC0(g_gameModePacketBuffer);
      break;
    case 0x985:
      this->LoadCooldownSnapshot(g_gameModePacketBuffer);
      break;
    case 0x988:
      this->ReceiveClanMemberCount(g_gameModePacketBuffer);
      break;
    case 0x989:
      this->ReceiveClanLeave(g_gameModePacketBuffer);
      break;
    case 0x98A:
      this->ReceiveClanInfo(g_gameModePacketBuffer);
      break;
    case 0x98E:
      this->ReceiveClanChat(g_gameModePacketBuffer);
      break;
    case 0x991:
      this->ReceiveItemInventoryList(g_gameModePacketBuffer);
      break;
    case 0x992:
      this->ReceiveInventoryEquipList(g_gameModePacketBuffer);
      break;
    case 0x993:
      this->ReceiveItem1740List(g_gameModePacketBuffer);
      break;
    case 0x994:
      this->ReceiveItem1740EquipList(g_gameModePacketBuffer);
      break;
    case 0x995:
      this->ReceiveStorageNormalList2(g_gameModePacketBuffer);
      break;
    case 0x996:
      this->ReceiveStorageEquipList4(g_gameModePacketBuffer);
      break;
    case 0x997:
      this->ReceiveEquipWindowMicroscope3(g_gameModePacketBuffer);
      break;
    case 0x99B:
      this->ReceiveMapProperty(g_gameModePacketBuffer);
      break;
    case 0x99F:
      this->ReceiveGroundSkill32(g_gameModePacketBuffer);
      break;
    case 0x9A6:
      this->ReceiveBankCheck(g_gameModePacketBuffer);
      break;
    case 0x9A8:
      this->ReceiveBankDepositAck(g_gameModePacketBuffer);
      break;
    case 0x9AA:
      this->ReceiveBankWithdrawAck(g_gameModePacketBuffer);
      break;
    case 0x9C1:
      this->AddMiniMapEffectMarker(g_gameModePacketBuffer);
      break;
    case 0x9CA:
      this->ReceiveGroundSkillExtended(g_gameModePacketBuffer);
      break;
    case 0x9CD:
      this->Zc_Msg_Color(g_gameModePacketBuffer);
      break;
    case 0x9D2:
      this->ReceiveItem1738NormalList(g_gameModePacketBuffer);
      break;
    case 0x9D3:
      this->ReceiveItem1738EquipList(g_gameModePacketBuffer);
      break;
    case 0x9DA:
      this->ReceiveGuildStorageLog(g_gameModePacketBuffer);
      break;
    case 0x9DB:
      this->ReceiveActorMoveEntry(g_gameModePacketBuffer);
      break;
    case 0x9DC:
      this->ReceiveActorStandEntryNamed(g_gameModePacketBuffer);
      break;
    case 0x9DD:
      this->ReceiveActorStateEntryNamed(g_gameModePacketBuffer);
      break;
    case 0x9DE:
      this->sub_D1A330(g_gameModePacketBuffer);
      break;
    case 0x9E7:
      this->ReceiveUnknownD77BF0(g_gameModePacketBuffer);
      break;
    case 0x9ED:
      this->ReceiveUnknown0454Result(g_gameModePacketBuffer);
      break;
    case 0x9F0:
      this->ReceiveRodexMailListAppend(g_gameModePacketBuffer);
      break;
    case 0x9F2:
      this->ReceiveRodexMailReturn(g_gameModePacketBuffer);
      break;
    case 0x9F4:
      this->ReceiveRodexItemReturn(g_gameModePacketBuffer);
      break;
    case 0x9F6:
      this->ReceiveRodexMailDeleted(g_gameModePacketBuffer);
      break;
    case 0x9F8:
      this->ReceiveOngoingQuestList(g_gameModePacketBuffer);
      break;
    case 0x9F9:
      this->ReceiveAddQuest(g_gameModePacketBuffer);
      break;
    case 0x9FA:
      this->ReceiveQuestHuntUpdate2(g_gameModePacketBuffer);
      break;
    case 0x9FD:
      this->ReceiveActorMoveEntryBody(g_gameModePacketBuffer);
      break;
    case 0x9FE:
      this->ReceiveActorStandEntryNamedBody(g_gameModePacketBuffer);
      break;
    case 0x9FF:
      this->ReceiveActorStateEntryNamedBody(g_gameModePacketBuffer);
      break;
    case 0xA02:
      this->EnsureWindow259(g_gameModePacketBuffer);
      break;
    case 0xA05:
      this->ReceiveItemReward(g_gameModePacketBuffer);
      break;
    case 0xA07:
      this->ReceiveStackedItemReturn(g_gameModePacketBuffer);
      break;
    case 0xA0A:
      this->ReceiveItem1738AddOptions(g_gameModePacketBuffer);
      break;
    case 0xA0B:
      this->ReceiveItem1740AddOptions(g_gameModePacketBuffer);
      break;
    case 0xA0C:
      this->ReceiveItemPickupAck7(g_gameModePacketBuffer);
      break;
    case 0xA0D:
      this->ReceiveInventoryEquipList2(g_gameModePacketBuffer);
      break;
    case 0xA0F:
      this->ReceiveItem1740EquipListOptions(g_gameModePacketBuffer);
      break;
    case 0xA10:
      this->ReceiveStorageEquipList5(g_gameModePacketBuffer);
      break;
    case 0xA11:
      this->ReceiveItem1738EquipListOptions(g_gameModePacketBuffer);
      break;
    case 0xA12:
      this->ReceiveRodexOpen(g_gameModePacketBuffer);
      break;
    case 0xA15:
      this->ReceiveUnknownD612A0(g_gameModePacketBuffer);
      break;
    case 0xA17:
      this->ReceiveDynamicNpcNotice(g_gameModePacketBuffer);
      break;
    case 0xA1A:
      this->ReceiveRouletteOpen(g_gameModePacketBuffer);
      break;
    case 0xA1C:
      this->ReceiveRouletteInfo(g_gameModePacketBuffer);
      break;
    case 0xA1E:
      this->ReceiveRouletteClose(g_gameModePacketBuffer);
      break;
    case 0xA20:
      this->ReceiveRouletteResult(g_gameModePacketBuffer);
      break;
    case 0xA22:
      this->ReceiveRouletteSetResult(g_gameModePacketBuffer);
      break;
    case 0xA2D:
      this->ReceiveEquipWindowMicroscope4(g_gameModePacketBuffer);
      break;
    case 0xA30:
      this->ReceiveActorNameCard(g_gameModePacketBuffer);
      break;
    case 0xA31:
      this->ReceivePackageItemTest(g_gameModePacketBuffer);
      break;
    case 0xA33:
      this->ReceiveRouletteBonusPoints(g_gameModePacketBuffer);
      break;
    case 0xA36:
      this->ReceiveActorInt8(g_gameModePacketBuffer);
      break;
    case 0xA37:
      this->ReceiveItemPickupAck8(g_gameModePacketBuffer);
      break;
    case 0xA3B:
      this->ReceiveStatusChangeList(g_gameModePacketBuffer);
      break;
    case 0xA3F:
      this->ReceiveEquipCardChange(g_gameModePacketBuffer);
      break;
    case 0xA43:
      this->ReceiveUnknownD89C60(g_gameModePacketBuffer);
      break;
    case 0xA44:
      this->sub_D6F900(g_gameModePacketBuffer);
      break;
    case 0xA47:
      this->ReceiveCharacterRenameResult(g_gameModePacketBuffer);
      break;
    case 0xA7D:
      this->ReceiveRodexMailListRefresh(g_gameModePacketBuffer);
      break;
    case 0xA82:
      this->sub_D48C60(g_gameModePacketBuffer);
      break;
    case 0xA83:
      this->sub_D4A060(g_gameModePacketBuffer);
      break;
    case 0xA84:
      this->ReceiveGuildInfo3(g_gameModePacketBuffer);
      break;
    case 0xA85:
      this->ReceiveGuildMemberAdd3(g_gameModePacketBuffer);
      break;
    case 0xA86:
      this->ReceiveGuildMemberList3(g_gameModePacketBuffer);
      break;
    case 0xA87:
      this->ReceiveGuildBanList2(g_gameModePacketBuffer);
      break;
    case 0xA96:
      this->ReceiveItemSlotAddOptions(g_gameModePacketBuffer);
      break;
    case 0xA98:
      this->ReceiveEquipItemResult(g_gameModePacketBuffer);
      break;
    case 0xA9A:
      this->ReceiveTakeOffEquipItemResult(g_gameModePacketBuffer);
      break;
    case 0xA9B:
      this->ReceiveSwapEquipmentList(g_gameModePacketBuffer);
      break;
    case 0xA9D:
      this->ReceiveCharViewResult(g_gameModePacketBuffer);
      break;
    case 0xAA5:
      this->ReceiveGuildMemberList(g_gameModePacketBuffer);
      break;
    case 0xAA6:
      this->ReceiveGuildMemberAdd4(g_gameModePacketBuffer);
      break;
    case 0xAA7:
      this->ClearItemLists(g_gameModePacketBuffer);
      break;
    case 0xAB2:
      this->ReceivePartyDead(g_gameModePacketBuffer);
      break;
    case 0xAB8:
      this->ReceivePacket0AB8(g_gameModePacketBuffer);
      break;
    case 0xABD:
      this->ReceivePartyJobLevel(g_gameModePacketBuffer);
      break;
    case 0xABE:
      this->ReceiveChatEntries(g_gameModePacketBuffer);
      break;
    case 0xABF:
      this->ReceiveWindow308TextEntries(g_gameModePacketBuffer);
      break;
    case 0xAC2:
      this->ReceiveRodexMailList(g_gameModePacketBuffer);
      break;
    case 0xAC7:
      this->sub_D08390(g_gameModePacketBuffer);
      break;
    case 0xAC8:
      this->ResetItemLists(g_gameModePacketBuffer);
      break;
    case 0xACB:
      this->ReceiveLongParChange(g_gameModePacketBuffer);
      break;
    case 0xACC:
      this->ReceiveExpGainNotice(g_gameModePacketBuffer);
      break;
    case 0xADD:
      this->ReceiveItemDropEntry(g_gameModePacketBuffer);
      break;
    case 0xADE:
      this->ReceiveUnknown173A5BC(g_gameModePacketBuffer);
      break;
    case 0xADF:
      this->ReceiveActorNameTitle(reinterpret_cast<const ActorNameTitlePacket *>(g_gameModePacketBuffer));
      break;
    case 0xAE1:
      this->ReceiveNamedResultNotice(g_gameModePacketBuffer);
      break;
    case 0xAE4:
      this->ReceiveUnknownD65870(g_gameModePacketBuffer);
      break;
    case 0xAE5:
      this->ReceivePartyRoster(g_gameModePacketBuffer);
      break;
    case 0xAF6:
      this->ReceiveShowRankingSchedule(g_gameModePacketBuffer);
      break;
    case 0xAF7:
      this->ReceivePlayerNameUpdate(reinterpret_cast<const PlayerNameUpdatePacket *>(g_gameModePacketBuffer));
      break;
    case 0xAFD:
      this->ReceiveActorClanName(g_gameModePacketBuffer);
      break;
    case 0xAFE:
      this->ReceiveQuestHuntUpdate(g_gameModePacketBuffer);
      break;
    case 0xAFF:
      this->ReceiveOngoingQuestList2(g_gameModePacketBuffer);
      break;
    case 0xB03:
      this->ReceiveEquipWindowMicroscope5(g_gameModePacketBuffer);
      break;
    case 0xB08:
      this->ReceiveInventoryStart(g_gameModePacketBuffer);
      break;
    case 0xB0C:
      this->ReceiveAddQuest64(g_gameModePacketBuffer);
      break;
    case 0xB0D:
      this->ReceiveEffectRemove(g_gameModePacketBuffer);
      break;
    case 0xB15:
      this->ReceiveExtraInventoryResult(g_gameModePacketBuffer);
      break;
    case 0xB17:
      this->NotifyResult3562(g_gameModePacketBuffer);
      break;
    case 0xB18:
      this->ReceiveUnknown7FBC(g_gameModePacketBuffer);
      break;
    case 0xB1A:
      this->ReceiveActorSkillResult(g_gameModePacketBuffer);
      break;
    case 0xB1B:
      this->ReceiveDeferredReset(g_gameModePacketBuffer);
      break;
    case 0xB20:
      this->ReceiveShortcutPagePacket(g_gameModePacketBuffer);
      break;
    case 0xB27:
      this->ReceiveGuildUnknown0114List(g_gameModePacketBuffer);
      break;
    case 0xB2F:
      this->ReceiveHomunculusInfo2(g_gameModePacketBuffer);
      break;
    case 0xB37:
      this->ReceiveEquipWindowMicroscope6(g_gameModePacketBuffer);
      break;
    case 0xB39:
      this->ReceiveEquipItemList(g_gameModePacketBuffer);
      break;
    case 0xB3C:
      this->ReceiveInventoryEquipList3(g_gameModePacketBuffer);
      break;
    case 0xB3D:
      this->ReceiveItem1748List2(g_gameModePacketBuffer);
      break;
    case 0xB3E:
      this->ReceiveGuildStorageLogItems(g_gameModePacketBuffer);
      break;
    case 0xB3F:
      this->ReceiveItemReward2(g_gameModePacketBuffer);
      break;
    case 0xB40:
      this->ReceiveItem1758StoreList(g_gameModePacketBuffer);
      break;
    case 0xB41:
      this->ReceiveItemPickupAck9(g_gameModePacketBuffer);
      break;
    case 0xB42:
      this->ReceiveItemSlotAddGrade(g_gameModePacketBuffer);
      break;
    case 0xB43:
      this->RefreshInventoryOrEquipItem2(g_gameModePacketBuffer);
      break;
    case 0xB44:
      this->ReceiveItem1738AddGrade(g_gameModePacketBuffer);
      break;
    case 0xB45:
      this->ReceiveItem1740AddGrade(g_gameModePacketBuffer);
      break;
    case 0xB4D:
      this->ReceiveItem1784List(g_gameModePacketBuffer);
      break;
    case 0xB4E:
      this->ReceiveParaResultList(g_gameModePacketBuffer);
      break;
    case 0xB56:
      this->ReceiveExpandedBarterMarketItemList(g_gameModePacketBuffer);
      break;
    case 0xB5A:
      this->ReceiveGradeEnchantInfo(g_gameModePacketBuffer);
      break;
    case 0xB5D:
      this->ReceiveGradeEnchantResult(g_gameModePacketBuffer);
      break;
    case 0xB5E:
      this->ReceiveGradeEnchantBroadcast(g_gameModePacketBuffer);
      break;
    case 0xB62:
      this->ReceiveShopItemListPacket(g_gameModePacketBuffer);
      break;
    case 0xB63:
      this->ReceiveRodexReadMail(g_gameModePacketBuffer);
      break;
    case 0xB64:
      this->ReceiveSearchStoreItems(g_gameModePacketBuffer);
      break;
    case 0xB65:
      this->ReceiveWindow107List(g_gameModePacketBuffer);
      break;
    case 0xB67:
      this->ReceivePartyItemPickup(g_gameModePacketBuffer);
      break;
    case 0xB69:
      this->ReceiveActorStatusValuePacket2(g_gameModePacketBuffer);
      break;
    case 0xB6B:
      this->ReceiveSummonHpInit(g_gameModePacketBuffer);
      break;
    case 0xB6C:
      this->ReceiveSummonHpUpdate(g_gameModePacketBuffer);
      break;
    case 0xB73:
      this->ReceivePacket0B73(g_gameModePacketBuffer);
      break;
    case 0xB77:
      this->ReceiveItem1718BasicList(g_gameModePacketBuffer);
      break;
    case 0xB78:
      this->ReceiveBarterMarketItemList(g_gameModePacketBuffer);
      break;
    case 0xB79:
      this->ReceiveExpandedBarterMarketItemList2(g_gameModePacketBuffer);
      break;
    case 0xB7A:
      this->ReceiveItem1718List(g_gameModePacketBuffer);
      break;
    case 0xB7B:
      this->ReceiveGuildInfo4(g_gameModePacketBuffer);
      break;
    case 0xB7C:
      this->ReceiveGuildBanList3(g_gameModePacketBuffer);
      break;
    case 0xB7D:
      this->ReceiveGuildMemberList2(g_gameModePacketBuffer);
      break;
    case 0xB7E:
      this->ReceiveGuildMemberAdd2(g_gameModePacketBuffer);
      break;
    case 0xBA4:
      this->ReceiveHomunculusInfo3(g_gameModePacketBuffer);
      break;
    case 0xBA5:
      this->ReceiveHomunculusProperty64(g_gameModePacketBuffer);
      break;
    case 0xBB3:
      this->ReceiveDdukddakBoxTest(g_gameModePacketBuffer);
      break;
    case 0xBB4:
      this->ReceiveItemOptionsRefresh(g_gameModePacketBuffer);
      break;
    case 0xBBB:
      this->ReceiveShopItemList(g_gameModePacketBuffer);
      break;
    case 0xBCC:
      this->ReceiveRuneActivation(g_gameModePacketBuffer);
      break;
    case 0xBCD:
      this->ReceiveRuneSetGrades(g_gameModePacketBuffer);
      break;
    case 0xBD9:
      this->ReceiveRuneRewardPacket(g_gameModePacketBuffer);
      break;
    case 0xBDE:
      this->ReceiveBroadcastText(g_gameModePacketBuffer);
      break;
    case 0xC10:
      this->ReceiveStovePurchaseResult(g_gameModePacketBuffer);
      break;
    case 0xC14:
      this->ReceiveMessageBox(g_gameModePacketBuffer);
      break;
    case 0xC15:
      this->ReceiveRuneRewardSteps(g_gameModePacketBuffer);
      break;
    case 0xC2B:
      this->ReceiveUnknownD79660(g_gameModePacketBuffer);
      break;
    case 0xC2C:
      this->ReceiveResultNotice4327(g_gameModePacketBuffer);
      break;
    case 0xC38:
      this->ReceiveItemBroadcast(g_gameModePacketBuffer);
      break;
    case 0xC39:
      this->ReceiveTradeBroadcast(g_gameModePacketBuffer);
      break;
    case 0xC3A:
      this->ReceiveRefineBroadcast(g_gameModePacketBuffer);
      break;
    case 0xC44:
      this->ReceiveSimulationResult(g_gameModePacketBuffer);
      break;
    case 0x73: {
      g_session.SetServerTime(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2));
      const unsigned char *posDir = g_gameModePacketBuffer + 6;
      g_session.SetPlayerPosition((posDir[0] << 2) | (posDir[1] >> 6), ((posDir[1] & 0x3F) << 4) | (posDir[2] >> 4), posDir[2] & 0xF);

      char mapName[36];
      sprintf(mapName, "%s.rsw", g_session.m_navigationPlayerMap);
      g_modeMgr.Switch(1, mapName);
      GetRagConnection().SetBlock(0);

      if (g_pingLogEnabled) {
        FILE *file = fopen("PingLog.txt", "at");
        if (file) {
          __time32_t now;
          _time32(&now);
          fprintf(file, "\n");
          fprintf(file, "server:%s\n", g_session.m_accountName.c_str());
          fprintf(file, "ip:%s\n", g_zoneServerIp);
          fprintf(file, "map:%s.gat\n", g_session.m_navigationPlayerMap);
          fprintf(file, "time:%s\n\n", _ctime32(&now));
          fclose(file);
        }
        g_pingLogTicks.clear();
        g_pingLogFirstTick = 0;
      }
      break;
    }
    case 0x7F: {
      if (g_pingLogEnabled) {
        FILE *file = fopen("PingLog.txt", "at");
        if (file) {
          const std::uint32_t sent = g_pingLogTicks.front();
          fprintf(file, "%10.2f\t%10d\n", static_cast<float>(sent - g_pingLogFirstTick) / 1000.0f, timeGetTime() - sent);
          fclose(file);
        }
        g_pingLogTicks.pop_front();
      }

      const std::uint32_t serverTime = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (this->unknown_0454 == 0) {
        g_session.SetServerTime(serverTime);
      } else {
        g_session.SyncServerTime(serverTime);
      }
      ++this->unknown_0454;
      *reinterpret_cast<int *>(&this->unknown_026E[0x02]) = 1;
      g_session.AddPingSample(timeGetTime() - *reinterpret_cast<const std::uint32_t *>(&this->unknown_026E[0x06]));
      break;
    }
    case 0x86: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      ActorMoveRoute path;
      path.tick = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 12);
      DecodeSrcDst(g_gameModePacketBuffer + 6, &path.srcX, &path.srcY, &path.dstX, &path.dstY, &path.subX, &path.subY);
      CRenderObject *actor = this->scene->GetGameActorByAID(aid);
      g_session.SyncServerTime(path.tick);
      if (actor) {
        const int pathAddress = reinterpret_cast<int>(&path);
        actor->SendMsg(nullptr, 0, 0, pathAddress, pathAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case 0x87: {
      ActorMoveRoute path;
      path.tick = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      DecodeSrcDst(g_gameModePacketBuffer + 6, &path.srcX, &path.srcY, &path.dstX, &path.dstY, &path.subX, &path.subY);
      g_session.SyncServerTime(path.tick);
      const int pathAddress = reinterpret_cast<int>(&path);
      this->scene->player->SendMsg(nullptr, 0, 0, pathAddress, pathAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0x88: {
      const ActorStopPacket *packet = reinterpret_cast<const ActorStopPacket *>(g_gameModePacketBuffer);
      this->StopActorMove(packet->aid, packet->x, packet->y, 0);
      break;
    }
    case 0x8A: {
      const ActorActionPacket *packet = reinterpret_cast<const ActorActionPacket *>(g_gameModePacketBuffer);
      this->ReceiveActorAction(packet->sourceAid, packet->targetAid, packet->tick, packet->attackMT, packet->attackedMT, packet->damage, packet->count,
                               packet->type, packet->damage2, 0);
      break;
    }
    case 0x8B: {
      const ActorRestPacket *packet = reinterpret_cast<const ActorRestPacket *>(g_gameModePacketBuffer);
      if (packet->damage == -30000) {
        break;
      }

      ACTSTRUCT act;
      act.damage = packet->damage;
      act.type = packet->type;
      act.targetId = packet->targetAid;
      act.tick = packet->tick;
      act.count = packet->count;
      act.unknown_001C = packet->attackMT;
      act.unknown_0020 = packet->attackedMT;
      act.damage2 = 0;
      act.attackMT = 432;
      act.attackedMT = 288;
      CRenderObject *actor;
      if (packet->sourceAid == g_session.m_aid) {
        actor = this->scene->player;
      } else {
        actor = this->scene->GetGameActorByAID(packet->sourceAid);
        if (!actor) {
          break;
        }
      }
      const int actAddress = reinterpret_cast<int>(&act);
      actor->SendMsg(nullptr, 11, 0, actAddress, actAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0x93:
      this->unknown_0244 = 0;
      break;
    case 0x95: {
      const ActorNamePacket *packet = reinterpret_cast<const ActorNamePacket *>(g_gameModePacketBuffer);
      this->nameCard.SetName(packet->aid, std::string(packet->name, sizeof(packet->name)).c_str());
      if (packet->aid == g_session.unknown_1010) {
        sprintf(g_session.unknown_1018, packet->name);
        if (UIPetInfoWnd *window = static_cast<UIPetInfoWnd *>(g_windowMgr.window_038C)) {
          window->nameEdit->SetText(packet->name);
        }
      }
      break;
    }
    case 0x9A:
      this->ReceiveTaggedBroadcast(g_gameModePacketBuffer);
      break;
    case 0x9C: {
      const ActorDirectionPacket *packet = reinterpret_cast<const ActorDirectionPacket *>(g_gameModePacketBuffer);
      CRenderObject *actor;
      if (packet->aid == g_session.m_aid) {
        actor = this->scene->player;
      } else {
        actor = this->scene->GetGameActorByAID(packet->aid);
        if (!actor) {
          break;
        }
      }
      actor->render.angle = static_cast<float>(45 * (packet->direction + 4));
      actor->SendMsg(nullptr, 100, 0, packet->headDirection, packet->headDirection >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0x9D:
    case 0x9E: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const bool falling = static_cast<short>(type) == 0x9E;
      ItemFallEntry entry = {};
      entry.count = *reinterpret_cast<const std::int16_t *>(packet + (falling ? 0x11 : 0x0F));
      entry.aid = *reinterpret_cast<const std::uint32_t *>(packet + 2);
      entry.column = *reinterpret_cast<const std::int16_t *>(packet + 0x0B);
      entry.row = *reinterpret_cast<const std::int16_t *>(packet + 0x0D);
      entry.subColumn = packet[falling ? 0x0F : 0x11];
      entry.subRow = packet[falling ? 0x10 : 0x12];
      entry.falling = 0;
      _itoa(*reinterpret_cast<const int *>(packet + 6), entry.itemId, 10);
      entry.falling = falling;
      entry.identified = packet[0x0A];

      CItem *item = this->scene->GetItemByAID(entry.aid);
      if (!item) {
        item = new CItem;
        this->scene->objects.push_back(item);
        this->scene->unknown_0018.push_back(item);
      }

      const int entryAddress = reinterpret_cast<int>(&entry);
      item->SendMsg(nullptr, 4, 0, entryAddress, entryAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0xA1: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      this->scene->RemoveGroundItem(aid);
      this->RemoveUnknown01A4(aid);
      break;
    }
    case 0xA3: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(NormalItemRecord);
      const NormalItemRecord *record = reinterpret_cast<const NormalItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        std::memset(item.unknown_001C, 0, sizeof(item.unknown_001C));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case 0xA4: {
      g_session.RemoveEquipment();
      g_session.ResetEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(EquipItemRecord);
      const EquipItemRecord *record = reinterpret_cast<const EquipItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = record->location;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->wearState;
        item.quantity = 1;
        std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = record->damaged;
        item.unknown_0060 = record->refine;
        if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.unknown_005D) {
          g_session.unknown_1050 = item.unknown_0000[1];
        }

        if (item.unknown_0000[3] == 0) {
          g_session.AddItem1710(item, 0);
          g_session.UpdateInventoryItem(item, 0);
        } else {
          char equipped = 1;
          g_session.EquipUnknown17F0(item, &equipped);
        }
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case 0xA5: {
      g_windowMgr.sub_A978D0(33);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(NormalItemRecord);
      const NormalItemRecord *record = reinterpret_cast<const NormalItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        std::memset(item.unknown_001C, 0, sizeof(item.unknown_001C));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        g_session.AddItem1738(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case 0xA6: {
      g_windowMgr.sub_A978D0(33);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(EquipItemRecord);
      const EquipItemRecord *record = reinterpret_cast<const EquipItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = record->location;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->wearState;
        item.quantity = 1;
        std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = record->damaged;
        item.unknown_0060 = record->refine;
        g_session.AddItem1738(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case 0xA8: {
      const ItemUseAckPacket *packet = reinterpret_cast<const ItemUseAckPacket *>(g_gameModePacketBuffer);
      if (packet->result) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
        if (item.unknown_0000[1]) {
          g_session.RemoveItem1710(packet->index, item.quantity - packet->amount);
          if (item.IsAboveItemId600()) {
            this->scene->player->SendMsg(nullptr, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
          }
        }
      }
      this->unknown_02CC = 0;
      break;
    }
    case 0xAA: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->unknown_0100[0x04]) = timeGetTime() - 2000;
      switch (packet[8]) {
      case 0:
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(372)), 0xFF, 7, 0, 0);
        break;
      case 1: {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(packet + 2));
        if (item.unknown_0000[1]) {
          item.unknown_0070 = *reinterpret_cast<const std::uint16_t *>(packet + 6);
          item.unknown_0000[3] = *reinterpret_cast<const std::uint16_t *>(packet + 4);
          g_session.OnItemEquipped(item);
        }
        break;
      }
      }
      break;
    }
    case 0xAC: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->unknown_0100[0x08]) = timeGetTime() - 2000;
      const std::uint16_t location = *reinterpret_cast<const std::uint16_t *>(packet + 4);
      if (packet[6] && location) {
        if (g_session.IsCostumeLocation(location)) {
          g_session.UnequipItem2B50(location);
        } else {
          g_session.UnequipItem(location, 0);
        }
      }
      break;
    }
    case 0xAF: {
      const unsigned char *packet = g_gameModePacketBuffer;
      if (*reinterpret_cast<int *>(&this->unknown_0100[0x14])) {
        *reinterpret_cast<int *>(&this->unknown_0100[0x14]) = 0;
        g_session.ResetSlots();
      }

      const std::uint16_t index = *reinterpret_cast<const std::uint16_t *>(packet + 2);
      const std::int16_t amount = *reinterpret_cast<const std::int16_t *>(packet + 4);
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
      if (item.unknown_0000[1] && amount > 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1609), item.DisplayName(0).c_str(), amount).c_str()),
                               0xFFFF00, 6, 0, 0);
      }
      g_session.RemoveItem1710(index, amount);
      *reinterpret_cast<int *>(this->unknown_02D0) = 0;
      break;
    }
    case 0xB3:
      g_navigationManager.Reset(true);
      g_navigationManager.ClearData();
      g_navigationManager.unknown_125C = 0;
      g_clanInfo->Clear();
      if (g_gameModePacketBuffer[2] == 1) {
        g_timeScheduler->sub_BA95F0(6);
        g_timeScheduler->sub_BA95F0(5);
        g_timeScheduler->sub_BA95F0(7);
        GetRagConnection().Close();
        g_loginStartMode = 5;
        g_modeMgr.Switch(0, "login.rsw");
      }
      break;
    case 0xB4: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->unknown_024C = 1;
      this->unknown_02DC = *reinterpret_cast<const int *>(packet + 4);
      unsigned int length = *reinterpret_cast<const std::uint16_t *>(packet + 2) - 8;
      UIWindow *window = g_windowMgr.sub_A978D0(16);
      char text[0x803];
      if (g_serviceType == 10) {
        sprintf(text, "|%02x", LanguageCharset(31));
        memcpy(text + 3, packet + 8, length);
        length += 3;
      } else {
        memcpy(text, packet + 8, length);
      }
      text[length] = 0;
      window->SendMsg(nullptr, 26, reinterpret_cast<std::intptr_t>(text), *reinterpret_cast<const int *>(packet + 4), 0, 0);
      break;
    }
    case 0xB5:
      this->unknown_024C = 1;
      this->unknown_02DC = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      g_windowMgr.sub_A978D0(16)->SendMsg(nullptr, 25, 0, 0, 0, 0);
      break;
    case 0xC0: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      CRenderObject *actor;
      if (aid == g_session.m_aid) {
        actor = this->scene->player;
      } else {
        actor = this->scene->GetGameActorByAID(aid);
        if (!actor) {
          break;
        }
      }
      const int emotion = g_gameModePacketBuffer[6];
      actor->SendMsg(nullptr, 160, 0, 0, 0, emotion, emotion >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0xC2: {
      char text[128];
      if (g_serviceType == 3 && !IsAdminAccount(g_session.m_aid)) {
        strcpy(text, GetUserCountText(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)));
      } else {
        sprintf(text, LookupMessage(178), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2));
      }
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
      break;
    }
    case 0xC4:
      this->unknown_024C = 1;
      g_windowMgr.sub_A978D0(25)->SendMsg(nullptr, 28, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      g_windowMgr.CloseWindow(22);
      g_windowMgr.CloseWindow(23);
      g_windowMgr.CloseWindow(24);
      break;
    case 0xC6: {
      g_session.ClearTradeItems();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(ShopItemRecord);
      const ShopItemRecord *records = reinterpret_cast<const ShopItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i) {
        const ShopItemRecord record = records[i];
        RawItemBasic item = {};
        _itoa(record.itemId, item.itemId, 10);
        item.unknown_0004 = record.price;
        item.unknown_0008 = record.discountPrice;
        item.unknown_000C = record.type;
        item.id = -1;
        item.identified = 1;
        g_session.AddItem1718(item, 0);
      }

      UIWindow *buyWindow = g_windowMgr.sub_A978D0(22);
      buyWindow->SendMsg(nullptr, 36, 0, 0, 0, 0);
      buyWindow->SendMsg(nullptr, 23, 0, 0, 0, 0);
      UIWindow *sellWindow = g_windowMgr.sub_A978D0(23);
      sellWindow->SendMsg(nullptr, 36, 0, 0, 0, 0);
      sellWindow->SendMsg(nullptr, 23, 0, 0, 0, 0);
      break;
    }
    case 0xCB:
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(54)), 0xFFFF00, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(57)), 0xFF, 0, 0, 0);
        break;
      }
      g_session.ClearTradeItems();
      g_windowMgr.CloseWindow(22);
      g_windowMgr.CloseWindow(24);
      break;
    case 0xCD: {
      char text[80];
      sprintf(text, LookupMessage(g_gameModePacketBuffer[2] == 0 ? 152 : 151));
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x32FF, 0, 0, 0);
      break;
    }
    case 0xD2:
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        switch (g_gameModePacketBuffer[3]) {
        case 0:
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3427)), 0xFFFF, 0, 0, 0);
          break;
        case 1:
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3428)), 0xFFFF, 0, 0, 0);
          break;
        }
        break;
      case 1:
        switch (g_gameModePacketBuffer[3]) {
        case 0:
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3429)), 0xFFFF, 0, 0, 0);
          break;
        case 1:
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3430)), 0xFFFF, 0, 0, 0);
          break;
        }
        break;
      }
      break;
    case 0xD4: {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      const int length = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      const int count = (length - 4) / 24;
      char name[25];
      name[24] = '\0';
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(count != 0 ? 3396 : 3395)), 0xFFFF00, 0, 0, 0);
      for (int i = 0; i < count; ++i) {
        if (i * 24 + 4 > length) {
          break;
        }
        std::memcpy(name, g_gameModePacketBuffer + 4 + i * 24, 24);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(name), 0xFFFF00, 0, 0, 0);
      }
      break;
    }
    case 0xD6:
      this->unknown_00FC = 0;
      switch (g_gameModePacketBuffer[2]) {
      case 0: {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(64)), 0xFFFF00, 0, 0, 0);
        UIWindow *window = g_windowMgr.sub_A978D0(28);
        this->unknown_03FC = 1;
        window->SendMsg(nullptr, 34, reinterpret_cast<std::intptr_t>(&this->unknown_03C8), 0, 0, 0);
        window->SendMsg(nullptr, 43, 0, 0, 0, 0);
        window->SendMsg(nullptr, 44, reinterpret_cast<std::intptr_t>(g_session.DecodePlayerName()), 0, 0, 0);
        break;
      }
      case 1:
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(65)), 0xFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(66)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0xD8: {
      const std::uint32_t roomId = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(this->unknown_01AC[roomId])) {
        actor->SendMsg(nullptr, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (g_windowMgr.window_027C && static_cast<UIChatRoomWnd *>(g_windowMgr.window_027C)->roomStatus.roomId == static_cast<int>(roomId)) {
        g_windowMgr.CloseWindow(28);
        g_windowMgr.CloseWindow(30);
      }
      break;
    }
    case 0xDC: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 4, 24);
      name[24] = '\0';
      g_session.DecodePlayerName();
      UIChatRoomWnd *window = static_cast<UIChatRoomWnd *>(g_windowMgr.sub_A978D0(28));
      ChatRoomMemberUpdateInfo info{window->title, window->unknown_00CC, window->roomStatus.roomType, window->roomStatus.memberCount,
                             window->roomStatus.memberLimit, window->roomStatus.roomId};
      info.memberCount = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      window->SendMsg(nullptr, 34, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      window->SendMsg(nullptr, 44, reinterpret_cast<std::intptr_t>(name), 1, 0, 0);
      char text[128];
      sprintf(text, LookupMessage(179), name);
      g_windowMgr.SendNotice(5, reinterpret_cast<std::intptr_t>(text), 0x808000, 0, 0, 0);
      break;
    }
    case 0xDD: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 4, 24);
      name[24] = '\0';
      if (strcmp(g_session.DecodePlayerName(), name) == 0) {
        g_windowMgr.CloseWindow(28);
        g_windowMgr.CloseWindow(30);
        break;
      }

      UIChatRoomWnd *window = static_cast<UIChatRoomWnd *>(g_windowMgr.sub_A978D0(28));
      ChatRoomMemberUpdateInfo info{window->title, window->unknown_00CC, window->roomStatus.roomType, window->roomStatus.memberCount,
                             window->roomStatus.memberLimit, window->roomStatus.roomId};
      info.memberCount = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      window->SendMsg(nullptr, 34, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      window->SendMsg(nullptr, 45, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
      char text[128];
      switch (g_gameModePacketBuffer[0x1C]) {
      case 0:
        sprintf(text, LookupMessage(180), name);
        g_windowMgr.SendNotice(5, reinterpret_cast<std::intptr_t>(text), 0x80, 0, 0, 0);
        break;
      case 1:
        sprintf(text, LookupMessage(181), name);
        g_windowMgr.SendNotice(5, reinterpret_cast<std::intptr_t>(text), 0x80, 0, 0, 0);
        break;
      }
      break;
    }
    case 0xE1: {
      UIWindow *window = g_windowMgr.sub_A978D0(28);
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 6, 24);
      name[24] = '\0';
      window->SendMsg(nullptr, 46, reinterpret_cast<std::intptr_t>(name), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0);
      break;
    }
    case 0xE5: {
      if (g_serviceOptions.Get(136)) {
        g_modeMgr.gameMode->SendMsg(50, 4, 0, 0, 0);
        break;
      }

      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 2, 24);
      name[24] = '\0';
      g_windowMgr.sub_A978D0(32)->SendMsg(nullptr, 50, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
      this->exchangeName.assign(name, strlen(name));
      break;
    }
    case 0xEC:
      if (UIWindow *window = g_windowMgr.LookupWindow(10011)) {
        switch (g_gameModePacketBuffer[2]) {
        case 0:
          window->SendMsg(nullptr, 51, 0, 0, 0, 0);
          break;
        case 1:
          window->SendMsg(nullptr, 52, 0, 0, 0, 0);
          break;
        }
      }
      break;
    case 0xEE:
      *reinterpret_cast<int *>(&this->unknown_0100[0x00]) = 0;
      this->SendMsg(18, 0, 0, 0, 0);
      g_windowMgr.CloseWindow(10011);
      g_windowMgr.CloseWindow(32);
      *reinterpret_cast<int *>(&this->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(74)), 0xFF, 0, 0, 0);
      break;
    case 0xF0:
      this->SendMsg(18, 0, 0, 0, 0);
      g_windowMgr.CloseWindow(10011);
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(75)), 0xFFFF00, 0, 0, 0);
        break;
      case 1:
        *reinterpret_cast<int *>(&this->unknown_0100[0x14]) = 0;
        g_session.ResetSlots();
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(76)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0xF1:
      *reinterpret_cast<int *>(&this->unknown_0100[0x14]) = 0;
      g_session.ResetSlots();
      break;
    case 0xF2:
      g_windowMgr.sub_A978D0(33)->SendMsg(nullptr, 55, *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2),
                                          *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4), 0, 0);
      break;
    case 0xF6: {
      ITEM_INFO item;
      item.unknown_0000[1] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      item.quantity = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      g_session.RemoveItem1738(item);
      this->unknown_05CE = 1;
      break;
    }
    case 0xF8:
      g_windowMgr.CloseWindow(33);
      g_session.ClearItems1738();
      break;
    case 0xFA:
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(77)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.sub_A978D0(34);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(78)), 0xFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(79)), 0xFF, 0, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1387)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0xFE:
    case 0x2C6: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 6), 24);
      name[24] = 0;
      if (g_forbiddenWords.Contains(name)) {
        std::string masked("");
        g_forbiddenWords.Mask(name, &masked);
      }
      g_session.unknown_54C4.assign(name, strlen(name));
      g_windowMgr.sub_A978D0(35)->SendMsg(nullptr, 56, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), reinterpret_cast<std::intptr_t>(name), 0, 0);
      break;
    }
    case 0x101: {
      char text[128];
      g_session.unknown_54A0 = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      if (g_windowMgr.unknown_030C) {
        g_windowMgr.unknown_030C->SendMsg(nullptr, 60, 0, 0, 0, 0);
      }
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      if (g_session.unknown_54A0 == 0 || g_session.unknown_54A0 == 1) {
        sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(292), LookupMessage(g_session.unknown_54A0 == 0 ? 287 : 288));
      } else if (g_session.unknown_54A0 == 2) {
        StringFormat(text, 128, static_cast<std::size_t>(-1), LookupMessage(373), LookupMessage(292), LookupMessage(288));
      } else {
        break;
      }
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
      break;
    }
    case 0x106: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (g_session.IsMercenaryAid(aid)) {
        break;
      }
      const int hp = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6);
      const int maxHp = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        actor->SendMsg(nullptr, 34, 0, hp, hp >> 31, maxHp, maxHp >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (g_windowMgr.window_02C8) {
        g_windowMgr.window_02C8->SendMsg(nullptr, 6, 34, aid, hp, maxHp);
      }
      g_windowMgr.NotifyItemWindows(aid, hp, maxHp);
      break;
    }
    case 0x107: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->SendMsg(63, *reinterpret_cast<const int *>(packet + 2), *reinterpret_cast<const std::int16_t *>(packet + 6),
                    *reinterpret_cast<const std::int16_t *>(packet + 8), 0);
      break;
    }
    case 0x10A: {
      char itemId[25];
      _itoa(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), itemId, 10);
      itemId[24] = 0;
      ITEM_INFO item;
      item.itemId.assign(itemId, strlen(itemId));
      item.unknown_0060 = 0;
      item.identified = 0;
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      const char *suffix = LookupMessage(144);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", LookupMessage(143), item.DisplayName(0).c_str(), suffix).c_str()),
                             0xFFFF00, 0, 0, 0);
      break;
    }
    case 0x10B: {
      char text[256];
      memset(text, 0, sizeof(text));
      StringFormat(text, 256, static_cast<std::size_t>(-1), "%s%s", LookupMessage(145), LookupMessage(146));
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 0, 0, 0);
      break;
    }
    case 0x10C: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      } else if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        actor->SendMsg(nullptr, 36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case 0x10D:
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(147)), 0xFF, 0, 0, 0);
      break;
    case 0x10F:
    case 0x235:
    case 0x29D: {
      std::vector<SkillListEntry> skills;
      const int count = static_cast<int>((*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4u) / sizeof(SkillListRecord));
      const SkillListRecord *record = reinterpret_cast<const SkillListRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        SkillListEntry entry = {};
        entry.id = record->id;
        entry.type = record->type;
        entry.level = record->level;
        entry.spCost = record->spCost;
        entry.range = record->range;
        entry.upgradable = record->upgradable;
        entry.unknown_000D = record->level;
        skills.push_back(entry);
      }
      this->ReceiveSkillList(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer), skills);
      break;
    }
    case 0x110:
      this->ReceiveSkillFailure(g_gameModePacketBuffer);
      break;
    case 0x111: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->ReceiveLearnedSkill(*reinterpret_cast<const std::uint16_t *>(packet + 2), *reinterpret_cast<const std::uint16_t *>(packet + 8),
                                *reinterpret_cast<const int *>(packet + 4), packet[0x26], *reinterpret_cast<const std::uint16_t *>(packet + 0x0A),
                                *reinterpret_cast<const std::uint16_t *>(packet + 0x0C), *reinterpret_cast<const std::uint16_t *>(packet + 8));
      break;
    }
    case 0x119: {
      const unsigned char *packet = g_gameModePacketBuffer;
      int state[4];
      state[0] = *reinterpret_cast<const std::int16_t *>(packet + 6);
      state[1] = *reinterpret_cast<const std::int16_t *>(packet + 0x0A);
      state[2] = *reinterpret_cast<const std::int16_t *>(packet + 8);
      state[3] = packet[0x0C];
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(packet + 2);
      CRenderObject *actor;
      if (aid == g_session.m_aid) {
        actor = this->scene->player;
        *reinterpret_cast<int *>(&g_session.unknown_0F0C[0x00]) = state[0];
        *reinterpret_cast<int *>(&g_session.unknown_0F0C[0x04]) = state[2];
        g_session.m_jobFlags = state[1];
        if (g_windowMgr.unknown_01E0) {
          g_windowMgr.unknown_01E0->SendMsg(nullptr, 60, 0, 0, 0, 0);
        }
        if (g_session.unknown_5C1C <= 0) {
          g_session.ClearItems1740();
          g_windowMgr.CloseWindow(40);
        }
      } else {
        actor = this->scene->GetGameActorByAID(aid);
      }
      if (actor) {
        const int stateAddress = reinterpret_cast<int>(state);
        actor->SendMsg(nullptr, 43, 0, stateAddress, stateAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case 0x11A: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->ReceiveSkillUse(*reinterpret_cast<const std::uint16_t *>(packet + 2), *reinterpret_cast<const std::int16_t *>(packet + 4),
                            *reinterpret_cast<const std::uint32_t *>(packet + 0x0A), *reinterpret_cast<const std::uint32_t *>(packet + 6), packet[0x0E]);
      break;
    }
    case 0x11C: {
      g_windowMgr.CloseWindow(39);
      UIWindow *window = g_windowMgr.sub_A978D0(39);
      window->SendMsg(nullptr, 59, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      const char *name = reinterpret_cast<const char *>(g_gameModePacketBuffer + 4);
      for (unsigned int i = 0; i < 4; ++i, name += 16) {
        if (strlen(name)) {
          window->SendMsg(nullptr, 27, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
        }
      }
      window->SendMsg(nullptr, 27, reinterpret_cast<std::intptr_t>("cancel"), 0, 0, 0);
      break;
    }
    case 0x11E:
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(217)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(217)), 0xFFFF00, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(214)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(214)), 0xFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(216)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(216)), 0xFF, 0, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2535)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(2535)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0x120: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      std::vector<std::uint32_t>::iterator it = this->spriteMirrorActorIds.begin();
      while (it != this->spriteMirrorActorIds.end() && *it != aid) {
        ++it;
      }
      if (it == this->spriteMirrorActorIds.end()) {
        this->scene->RemoveSkill(aid);
      } else if (this->scene->RemoveItem(aid) == true) {
        this->SendMsg(13, aid, 0, 0, 0);
        this->spriteMirrorActorIds.erase(it);
      }
      break;
    }
    case 0x121: {
      const unsigned char *packet = g_gameModePacketBuffer;
      g_session.cartCount = *reinterpret_cast<const std::int16_t *>(packet + 2);
      g_session.cartMaxCount = *reinterpret_cast<const std::int16_t *>(packet + 4);
      g_session.cartWeight = *reinterpret_cast<const int *>(packet + 6) / 10;
      g_session.cartMaxWeight = *reinterpret_cast<const int *>(packet + 0x0A) / 10;
      if (g_windowMgr.unknown_02F4) {
        g_windowMgr.unknown_02F4->SendMsg(nullptr, 55, 0, 0, 0, 0);
      }
      break;
    }
    case 0x122: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(EquipItemRecord);
      const EquipItemRecord *record = reinterpret_cast<const EquipItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = record->location;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->wearState;
        item.quantity = 1;
        std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = record->damaged;
        item.unknown_0060 = record->refine;
        g_session.AddItem1740(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case 0x123: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(NormalItemRecord);
      const NormalItemRecord *record = reinterpret_cast<const NormalItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        std::memset(item.unknown_001C, 0, sizeof(item.unknown_001C));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        g_session.AddItem1740(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case 0x124: {
      const unsigned char *packet = g_gameModePacketBuffer;
      ITEM_INFO item;
      item.unknown_0000[1] = *reinterpret_cast<const std::int16_t *>(packet + 2);
      item.quantity = *reinterpret_cast<const int *>(packet + 4);
      item.SetItemId(*reinterpret_cast<const int *>(packet + 8));
      item.identified = packet[0x0C];
      item.unknown_005D = packet[0x0D];
      item.unknown_0060 = packet[0x0E];
      std::memcpy(item.unknown_001C, packet + 0x0F, sizeof(item.unknown_001C));
      g_session.AddItem1740(item);
      break;
    }
    case 0x125: {
      ITEM_INFO item;
      item.unknown_0000[1] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      item.quantity = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      g_session.RemoveItem1740(item);
      this->unknown_05CE = 1;
      break;
    }
    case 0x12B:
      g_session.ClearItems1740();
      g_windowMgr.CloseWindow(40);
      break;
    case 0x12C:
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(56)), 0xFF, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(221)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0x12D: {
      CGameMode *mode = static_cast<CGameMode *>(g_modeMgr.GetGameMode());
      if (mode && mode->drag.source != 0) {
        this->SendMsg(81, 0, 0, 0, 0);
        break;
      }

      const int slots = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      if (slots < 3) {
        g_session.m_vendingSlotCount = 3;
      } else {
        g_session.m_vendingSlotCount = slots > 13 ? 13 : slots;
      }
      g_session.ClearItems1768();
      if (g_windowMgr.window_027C || g_windowMgr.window_0274) {
        g_windowMgr.Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
        this->SendMsg(81, 0, 0, 0, 0);
      } else if (UIMerchantShopMakeWnd *window = static_cast<UIMerchantShopMakeWnd *>(g_windowMgr.sub_A978D0(41))) {
        window->AddActionButton();
      }
      break;
    }
    case 0x131: {
      char title[81];
      strncpy(title, reinterpret_cast<const char *>(g_gameModePacketBuffer + 6), 80);
      title[80] = 0;
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        const int titleAddress = reinterpret_cast<int>(title);
        actor->SendMsg(nullptr, 55, 0, titleAddress, titleAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case 0x132:
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        actor->SendMsg(nullptr, 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x137:
      sub_D08110(_time32(nullptr), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                 *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4), 0, 0);
      break;
    case 0x139: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(packet + 2);
      g_session.unknown_0F4C = *reinterpret_cast<const std::int16_t *>(packet + 0x0E);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        const int x = *reinterpret_cast<const std::int16_t *>(packet + 6);
        const int y = *reinterpret_cast<const std::int16_t *>(packet + 8);
        actor->SendMsg(nullptr, 61, 0, x, x >> 31, y, y >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      const int playerX = *reinterpret_cast<const std::int16_t *>(packet + 0x0A);
      const int playerY = *reinterpret_cast<const std::int16_t *>(packet + 0x0C);
      this->scene->player->SendMsg(nullptr, 61, 0, playerX, playerX >> 31, playerY, playerY >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, 60, 0, aid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0x13A:
      g_session.unknown_0F4C = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      break;
    case 0x13C:
      if (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) == 0 && g_session.unknown_16EC) {
        const int location = 0x8000;
        EquipmentProperties::CEquipmentPropertiesMgr *properties = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
        const int index = GetEquipLocationIndex(location);
        if (properties->FindProperty1(index)) {
          const int unequipLocation = 0x8000;
          EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->UnequipWearable(unequipLocation);
        }
      }
      g_session.SetUnknown16EC(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2));
      *reinterpret_cast<std::uint32_t *>(&this->unknown_0100[0x04]) = timeGetTime() - 2000;
      break;
    case 0x13D:
      this->ReceiveHpSpRecovery(g_gameModePacketBuffer, 0);
      break;
    case 0x13E:
      this->ProcessSkillUsePacket(g_gameModePacketBuffer, 0);
      break;
    case 0x142: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (this->scene->GetGameActorByAID(aid)) {
        g_windowMgr.sub_A978D0(56)->SendMsg(nullptr, 28, aid, 0, 0, 0);
      }
      break;
    }
    case 0x144: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const int type = *reinterpret_cast<const int *>(packet + 6);
      const std::uint32_t id = packet[0x12];
      if (type == 2) {
        this->RemoveMiniMapPing(id);
        break;
      }
      this->unknown_01C4[id].x = *reinterpret_cast<const int *>(packet + 0x0A);
      this->unknown_01C4[id].y = *reinterpret_cast<const int *>(packet + 0x0E);
      this->unknown_01C4[id].color = *reinterpret_cast<const std::uint32_t *>(packet + 0x13) | 0xFF000000;
      this->unknown_01C4[id].persistent = type;
      this->unknown_01C4[id].tick = timeGetTime();
      break;
    }
    case 0x145: {
      char name[17];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), 16);
      int index = g_gameModePacketBuffer[0x12];
      name[16] = 0;
      if (index == 0xFF) {
        index = -1;
      }
      this->SendMsg(100, reinterpret_cast<int>(name), index, 0, 0);
      break;
    }
    case 0x147: {
      const unsigned char *packet = g_gameModePacketBuffer;
      CSkillInfo skill;
      skill.level = *reinterpret_cast<const std::int16_t *>(packet + 8);
      skill.unknown_000C = *reinterpret_cast<const int *>(packet + 4);
      skill.id = *reinterpret_cast<const std::uint16_t *>(packet + 2);
      skill.unknown_0018 = packet[0x26];
      skill.name = skill.IdName(*g_session.m_hotkeyScript);
      skill.unknown_0014 = *reinterpret_cast<const std::int16_t *>(packet + 0x0A);
      skill.unknown_001C = *reinterpret_cast<const std::int16_t *>(packet + 0x0C);
      skill.unknown_0034 = skill.UseApAmount(*g_session.m_hotkeyScript, *reinterpret_cast<const std::int16_t *>(packet + 8));
      g_session.m_skillBundle.UpdatePending(skill);
      this->SendMsg(113, reinterpret_cast<int>(&skill), skill.level, 1, -1);
      break;
    }
    case 0x148: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        actor->SendMsg(nullptr, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        this->unknown_024C = 0;
        this->unknown_0250 = 0;
      }
      MessengerMember member = g_session.PartyByAccount(aid);
      if (member.valid != 0) {
        g_session.SetPartyDead(aid, 0);
        sub_DFF150(&g_session);
      }
      break;
    }
    case 0x14A:
      switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(854)), 0xFFFF00, 0, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(830)), 0xFFFF, 0, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(833)), 0xFFFF, 0, 0, 0);
        break;
      }
      break;
    case 0x14B: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 3, 24);
      name[24] = 0;
      if (g_gameModePacketBuffer[2] == 0) {
        char text[128];
        sprintf(text, LookupMessage(387), name);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 0, 0, 0);
      }
      break;
    }
    case 0x14E:
      g_guildInfo.unknown_00A4 = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      break;
    case 0x152: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const std::uint32_t guildId = *reinterpret_cast<const std::uint32_t *>(packet + 4);
      const int version = *reinterpret_cast<const int *>(packet + 8);
      g_session.StoreEmblem(packet + 0x0C, *reinterpret_cast<const std::int16_t *>(packet + 2) - 12, guildId, version);
      if (g_guildInfo.m_guildId == guildId) {
        this->scene->player->Slot49(guildId, version);
        if (g_windowMgr.window_0320) {
          g_windowMgr.window_0320->SendMsg(nullptr, 60, 0, 0, 0, 0);
        }
      }
      break;
    }
    case 0x156: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 12;
      const int *record = reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, record += 3) {
        g_guildInfo.ChangeMemberPosition(record[0], record[1], record[2]);
        if (record[2] == 0) {
          g_guildInfo.SetMasterCharacterId(record[1]);
        }
      }
      if (UIWindow *window = g_windowMgr.LookupWindow(61)) {
        window->SendMsg(nullptr, 106, 0, 0, 0, 0);
      }
      g_windowMgr.RefreshGuildWindows();
      break;
    }
    case 0x15E:
      switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
      case 0:
        g_session.ClearAdditionalShortcutSkills();
        g_guildInfo.Init();
        g_windowMgr.CloseWindow(59);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(400)), 0xFFFF, 4, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(401)), 0xFFFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(402)), 0xFFFF, 4, 0, 0);
        break;
      }
      break;
    case 0x15F: {
      char reason[41];
      std::memcpy(reason, g_gameModePacketBuffer + 2, 40);
      reason[40] = 0;
      char text[1024];
      sprintf(text, LookupMessage(367), reason);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 4, 0, 0);
      break;
    }
    case 0x162:
      this->ApplyAdditionalSkillPacket(g_gameModePacketBuffer);
      break;
    case 0x167:
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(374)), 0xFFFF00, 4, 0, 0);
        g_windowMgr.CloseWindow(214);
        g_windowMgr.CloseWindow(213);
        g_windowMgr.CloseWindow(212);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(375)), 0xFFFF, 4, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(376)), 0xFFFF, 4, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(405)), 0xFFFF, 4, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2687)), 0xFFFF, 4, 0, 0);
        break;
      }
      break;
    case 0x169:
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(378)), 0xFF, 4, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(379)), 0xFF, 4, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(380)), 0xFFFF00, 4, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(381)), 0xFF, 4, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2090)), 0xFF, 4, 0, 0);
        break;
      }
      break;
    case 0x16A: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 6, 24);
      name[24] = 0;
      if (g_forbiddenWords.Contains(name) == true) {
        std::string masked("");
        g_forbiddenWords.Mask(name, &masked);
      }
      g_windowMgr.sub_A978D0(70)->SendMsg(nullptr, 73, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), reinterpret_cast<std::intptr_t>(name), 0, 0);
      break;
    }
    case 0x16C: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const int flags = *reinterpret_cast<const int *>(packet + 0x0A);
      const std::uint32_t guildId = *reinterpret_cast<const std::uint32_t *>(packet + 2);
      g_guildInfo.unknown_00AC = flags & 1;
      g_guildInfo.m_guildId = guildId;
      g_guildInfo.unknown_00B0 = (flags >> 4) & 1;
      g_guildInfo.unknown_00B4 = packet[0x0E];
      this->scene->player->Slot49(guildId, *reinterpret_cast<const int *>(packet + 6));
      if (g_guildInfo.unknown_00C8 == 0) {
        g_guildInfo.unknown_00C8 = 1;
        g_modeMgr.gameMode->SendMsg(107, 3, 0, 0, 0);
      }
      if (g_guildInfo.unknown_00C0 == 0) {
        g_guildInfo.unknown_00C0 = 1;
        g_modeMgr.gameMode->SendMsg(107, 1, 0, 0, 0);
      }
      break;
    }
    case 0x16D: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const int online = *reinterpret_cast<const int *>(packet + 0x0A);
      g_guildInfo.SetMemberOnline(*reinterpret_cast<const int *>(packet + 2), online);
      std::string name = LookupPlayerName(*reinterpret_cast<const std::uint32_t *>(packet + 6));
      if (name.size()) {
        this->SendMsg(140, reinterpret_cast<int>(name.c_str()), online, 0, 0);
      }
      if (online == 0) {
        this->SendMsg(171, *reinterpret_cast<const int *>(packet + 2), 0, 0, 0);
      }
      break;
    }
    case 0x171: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 6, 24);
      name[24] = 0;
      if (g_forbiddenWords.Contains(name) == true) {
        std::string masked("");
        g_forbiddenWords.Mask(name, &masked);
      }
      g_windowMgr.sub_A978D0(72)->SendMsg(nullptr, 74, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), reinterpret_cast<std::intptr_t>(name), 0, 0);
      break;
    }
    case 0x173:
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(394)), 0xFFFF, 4, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(395)), 0xFFFF, 4, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(396)), 0xFFFF00, 4, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(397)), 0xFFFF, 4, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(398)), 0xFFFF, 4, 0, 0);
        break;
      case 5:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1717)), 0xFFFF, 4, 0, 0);
        break;
      }
      break;
    case 0x177: {
      unsigned int count = (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4u) >> 1;
      UIWindow *window = g_windowMgr.sub_A978D0(73);
      window->SendMsg(nullptr, 75, 0, 0, 0, 0);
      const std::int16_t *index = reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      for (; count != 0; --count, ++index) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *index);
        if (item.unknown_0000[1]) {
          window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.unknown_0000[1], 0, 0);
        }
      }
      window->SendMsg(nullptr, 60, 0, 0, 0, 0);
      break;
    }
    case 0x179:
      switch (g_gameModePacketBuffer[4]) {
      case 0: {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(491)), 0xFFFF00, 0, 0, 0);
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2));
        item.identified = 1;
        g_session.UpdateInventoryItem(item, 1);
        break;
      }
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(492)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0x17B: {
      unsigned int count = (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4u) >> 1;
      UIWindow *window = g_windowMgr.sub_A978D0(74);
      window->SendMsg(nullptr, 75, 0, 0, 0, 0);
      window->SendMsg(nullptr, 77, *reinterpret_cast<const int *>(this->unknown_045C), 0, 0, 0);
      const std::int16_t *index = reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      for (; count != 0; --count, ++index) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *index);
        if (item.unknown_0000[1]) {
          window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.unknown_0000[1], 0, 0);
        }
      }
      window->SendMsg(nullptr, 60, 0, 0, 0, 0);
      break;
    }
    case 0x181:
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(495)), 0xFFFF00, 4, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(496)), 0xFF, 4, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(497)), 0xFF, 4, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1718)), 0xFF, 4, 0, 0);
        break;
      }
      break;
    case 0x184:
      if (g_guildInfo.unknown_00BC) {
        g_guildInfo.RemoveRelatedGuild(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6));
      }
      break;
    case 0x189:
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(500)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(500)), 0xFF, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(501)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(501)), 0xFFFF00, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1334)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(1334)), 0xFFFF00, 0, 0, 0);
        break;
      case 3:
        this->unknown_02CC = 0;
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1335)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(LookupMessage(1335)), 0xFFFF00, 0, 0, 0);
        break;
      }
      break;
    case 0x18B:
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)) {
      case 0:
        GetRagConnection().Close();
        this->SendMsg(2, 0, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(502)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0x18C: {
      const std::uint16_t id = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2);
      if (id != 1285 && id != 1286 && id != 1287 && id != 1829 && id != 1830) {
        g_windowMgr.sub_A978D0(77)->SendMsg(nullptr, 79, reinterpret_cast<std::intptr_t>(g_gameModePacketBuffer), 0, 0, 0);
      }
      break;
    }
    case 0x18D: {
      unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) >> 4;
      if (count == 0) {
        g_windowMgr.Confirm(LookupMessage(424), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      g_windowMgr.CloseWindow(79);
      UIWindow *window = g_windowMgr.sub_A978D0(79);
      const int *record = reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      for (; count != 0; --count, record += 4) {
        window->SendMsg(nullptr, 31, *record, 0, 0, 0);
      }
      break;
    }
    case 0x191:
      if (CRenderObject *skill = this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        const int textAddress = reinterpret_cast<int>(g_gameModePacketBuffer + 6);
        skill->SendMsg(nullptr, 7, 0, textAddress, textAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x192: {
      CGameMode *mode = static_cast<CGameMode *>(g_modeMgr.GetGameMode());
      const int column = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      const int row = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      const_cast<TerrainHeightCell *>(mode->scene->terrain->CellAt(column, row))->type = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6);
      break;
    }
    case 0x194:
      SetPlayerName(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2),
          std::string(reinterpret_cast<const char *>(g_gameModePacketBuffer + 6), reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x1E)).c_str());
      break;
    case 0x199:
      g_serviceOptions.Set(229, *reinterpret_cast<const int *>(g_session.unknown_5454) != 0);
      switch (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)) {
      case 1:
      case 5:
        this->scene->unknown_0048 = 1;
        break;
      case 2:
        this->scene->unknown_0054 = 1;
        break;
      case 3:
        this->scene->unknown_004C = 1;
        g_serviceOptions.Set(229, 1);
        break;
      case 4:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(855)), 0x9B9BFF, 0, 0, 0);
        break;
      }
      if (g_session.GetMapProperty0F()) {
        g_serviceOptions.Set(229, 1);
      }
      if (g_serviceType) {
        g_mapPropertyMgr->Clear();
      }
      break;
    case 0x1A2: {
      unsigned char *packet = g_gameModePacketBuffer;
      g_session.unknown_103C = *reinterpret_cast<const std::int16_t *>(packet + 0x23);
      std::int16_t &value = *reinterpret_cast<std::int16_t *>(packet + 0x1B);
      if (value >= 10000) {
        value -= 10000;
        if (UIItemCollectionWnd *window = static_cast<UIItemCollectionWnd *>(g_windowMgr.sub_A978D0(12))) {
          window->unknown_0218 = value;
          window->unknown_021A = *reinterpret_cast<const std::int16_t *>(packet + 0x1D);
          window->unknown_021C = *reinterpret_cast<const std::int16_t *>(packet + 0x1F);
          window->unknown_0236 = *reinterpret_cast<const std::uint16_t *>(packet + 0x21);
          window->unknown_0238 = packet[0x1A];
          sprintf(reinterpret_cast<char *>(window->unknown_021E), reinterpret_cast<const char *>(packet + 2));
          window->SendMsg(nullptr, 29, 0, 0, 0, 0);
        }
        break;
      }

      sprintf(g_session.unknown_1018, reinterpret_cast<const char *>(packet + 2));
      g_session.unknown_1054 = g_session.unknown_1044;
      g_session.unknown_1044 = *reinterpret_cast<const std::int16_t *>(packet + 0x1D);
      g_session.m_petFriendly = *reinterpret_cast<const std::int16_t *>(packet + 0x1F);
      g_session.unknown_1038 = *reinterpret_cast<const std::uint16_t *>(packet + 0x21);
      if (UIPetInfoWnd *window = static_cast<UIPetInfoWnd *>(g_windowMgr.window_038C)) {
        window->UpdateFriendlyGauge();
        window->nameEdit->SetText(g_session.unknown_1018);
        window->Invalidate();
      }
      *reinterpret_cast<int *>(g_session.unknown_1014) = packet[0x1A];
      g_session.unknown_1040 = value;
      break;
    }
    case 0x1A6: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) >> 1;
      UIWindow *window = g_windowMgr.sub_A978D0(90);
      window->SendMsg(nullptr, 75, 0, 0, 0, 0);
      const std::int16_t *index = reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++index) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *index);
        if (item.unknown_0000[1]) {
          window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.unknown_0000[1], 0, 0);
        }
      }
      window->SendMsg(nullptr, 60, 0, 0, 0, 0);
      break;
    }
    case 0x1AB:
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        if (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6) == 4) {
          actor->Slot40(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 8));
        }
      }
      break;
    case 0x1AC:
      if (CGameActor *skill = static_cast<CGameActor *>(this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        if (skill->GetEffectiveJob(-1) > 138 && skill->GetEffectiveJob(-1) <= 153) {
          skill->SendMsg(nullptr, 119, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        switch (skill->GetEffectiveJob(-1)) {
        case 0x5174:
        case 0x5175:
        case 0x5176:
        case 0x5177:
          skill->SendMsg(nullptr, 119, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
          break;
        }
      }
      break;
    case 0x1AD: {
      const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4) >> 2;
      UIWindow *window = g_windowMgr.sub_A978D0(94);
      window->SendMsg(nullptr, 75, 0, 0, 0, 0);
      const unsigned char *entry = g_gameModePacketBuffer + 4;
      for (unsigned int i = count; i != 0; --i, entry += 4) {
        window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(entry), 0, 0, 0);
      }
      window->SendMsg(nullptr, 34, 2, 0, 0, 0);
      break;
    }
    case 0x1B0:
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        actor->Slot18(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 7), 0, 0);
      }
      break;
    case 0x1B1: {
      const int seconds = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 3);
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        *reinterpret_cast<int *>(g_session.unknown_079C) = 0;
        g_session.m_timeAttackVisible = 1;
        g_session.m_timeAttackCountdown = seconds * 1000;
        break;
      case 1:
        g_session.m_timeAttackCountdown = timeGetTime() + seconds * 1000;
        *reinterpret_cast<int *>(g_session.unknown_079C) = 1;
        g_session.m_timeAttackVisible = 1;
        break;
      case 2:
        g_session.m_timeAttackCountdown = timeGetTime() - seconds * 1000;
        *reinterpret_cast<int *>(g_session.unknown_079C) = 2;
        g_session.m_timeAttackVisible = 1;
        break;
      case 3:
        *reinterpret_cast<int *>(g_session.unknown_079C) = 3;
        g_session.m_timeAttackCountdown = seconds * 30 + 29;
        g_session.m_timeAttackVisible = 1;
        break;
      default:
        g_session.m_timeAttackVisible = 1;
        break;
      }
      break;
    }
    case 0x1B3: {
      char name[65];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), 64);
      int position = g_gameModePacketBuffer[0x42];
      name[64] = 0;
      if (position == 255) {
        position = -1;
      }
      this->SendMsg(100, reinterpret_cast<int>(name), position, 0, 0);
      break;
    }
    case 0x1B4:
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        actor->Slot49(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0xA));
      }
      break;
    case 0x1C2: {
      char message[256];
      const int remaining = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
      if (g_serviceType == 0) {
        switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
        case 100:
          sprintf(message, "\xb0\xb3\xc0\xce \xc1\xa4\xbe\xd7\xc1\xa6 \xbd\xc3\xb0\xa3\xc0\xcc %d\xba\xd0 %d\xc3\xca \xb3\xb2\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.",
                  remaining / 60, remaining % 60);
          break;
        case 200:
          sprintf(message, "\xb0\xb3\xc0\xce \xc1\xa4\xb7\xae\xc1\xa6 \xbd\xc3\xb0\xa3\xc0\xcc %d\xba\xd0 %d\xc3\xca \xb3\xb2\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.",
                  remaining / 60, remaining % 60);
          break;
        case 300:
          sprintf(message, "PC\xb9\xe6 \xc1\xa4\xb7\xae\xc1\xa6 \xbd\xc3\xb0\xa3\xc0\xcc %d\xba\xd0 %d\xc3\xca \xb3\xb2\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.",
                  remaining / 60, remaining % 60);
          break;
        }
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
      } else if (g_serviceType == 6) {
        sprintf(message, "Sisa waktu voucher kamu adalah %d menit.", remaining / 60);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
      }
      break;
    }
    case 0x1C3:
      this->ReceiveColorBroadcast(g_gameModePacketBuffer);
      break;
    case 0x1C7:
      ShowErrorDialog(GetRenderSystemHolder()->system, "No Packet Encryption !!!", 0);
      break;
    case 0x1C8: {
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 8) == g_session.m_aid) {
        this->unknown_02CC = 0;
        if (g_gameModePacketBuffer[0xE]) {
          ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2));
          if (item.unknown_0000[1]) {
            g_session.RemoveItem1710(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                     item.quantity - *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0xC));
            if (item.GetItemId() == 607) {
              this->unknown_05B4 = 0;
            }
          }
        } else {
          if (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4) != 103061) {
            break;
          }
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4209)), 0xFF, 0, 0, 0);
        }
      }
      if (g_gameModePacketBuffer[0xE]) {
        this->PlayItemUseEffect(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 8));
      }
      break;
    }
    case 0x1CD: {
      char count = 0;
      for (const int *skill = reinterpret_cast<const int *>(g_gameModePacketBuffer + 2); skill < reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E);
           ++skill) {
        if (*skill) {
          ++count;
        }
      }
      if (count <= 0) {
        break;
      }
      if (UIWindow *window = g_windowMgr.sub_A978D0(99)) {
        for (const int *skill = reinterpret_cast<const int *>(g_gameModePacketBuffer + 2); skill < reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E);
             ++skill) {
          if (*skill) {
            window->SendMsg(nullptr, 31, *skill, 0, 0, 0);
          }
        }
      }
      break;
    }
    case 0x1D0:
      this->UpdateActorSpiritCount(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2),
                                   *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6));
      break;
    case 0x1D4:
      if (this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)) ||
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == 0) {
        g_windowMgr.sub_A978D0(100)->SendMsg(nullptr, 28, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      }
      break;
    case 0x1D6: {
      g_serviceOptions.Set(229, *reinterpret_cast<const int *>(g_session.unknown_5454) != 0);
      const std::int16_t type = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      g_session.unknown_0000 = type;
      if (type == 5 || type == 6) {
        this->scene->unknown_0048 = 1;
        this->forcedAttack = 1;
      } else if (type != 11 && type != 12) {
        if (g_session.GetMapProperty09() || *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) == 18 ||
            *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) == 8) {
          this->scene->unknown_004C = 1;
          g_serviceOptions.Set(229, 1);
        } else if (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) == 19) {
          this->scene->unknown_0050 = 1;
          this->scene->unknown_0048 = 1;
        }
      }
      if (g_session.GetMapProperty0F()) {
        g_serviceOptions.Set(229, 1);
      }
      if (!g_statusRefreshSuppressed) {
        static_cast<CGameMode *>(g_modeMgr.GetGameMode())->scene->player->ReapplySkillEffects();
      }
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)) {
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 11:
      case 14:
      case 17:
      case 18:
      case 19:
      case 25:
      case 31:
      case 32:
        this->scene->unknown_00FD = true;
        break;
      }
      break;
    }
    case 0x1E2: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      *reinterpret_cast<int *>(this->unknown_028C) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      *reinterpret_cast<int *>(this->unknown_028C + 4) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
      g_windowMgr.sub_A978D0(104)->SendMsg(nullptr, 90, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), reinterpret_cast<std::intptr_t>(name), 0,
                                           0);
      break;
    }
    case 0x1EB:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) != g_session.m_aid) {
        this->SendMsg(170, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6),
                      *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8), 0);
      }
      break;
    case 0x1EC:
      if (strcmp(g_session.m_navigationPlayerMap, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA))) {
        this->SendMsg(171, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), 0, 0, 0);
        if (CGameActor *actor =
                static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6)))) {
          actor->SetUnknown02E4(0);
        }
      } else if (CGameActor *actor =
                     static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6)))) {
        actor->SetUnknown02E4(1);
      }
      break;
    case 0x1EE: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemRecord);
      const CardItemRecord *record = reinterpret_cast<const CardItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case 0x1EF: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemRecord);
      const CardItemRecord *record = reinterpret_cast<const CardItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        g_session.AddItem1740(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case 0x1F0: {
      g_windowMgr.sub_A978D0(33);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemRecord);
      const CardItemRecord *record = reinterpret_cast<const CardItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        g_session.AddItem1738(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case 0x1F2: {
      g_guildInfo.UpdateMemberStatus(
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xE),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x10), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x12));
      std::string name = LookupPlayerName(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6));
      if (name.size()) {
        this->SendMsg(140, reinterpret_cast<int>(name.c_str()), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA), 0, 0);
      }
      if (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA) == 0) {
        this->SendMsg(171, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      }
      break;
    }
    case 0x1F4: {
      if (g_serviceOptions.Get(136)) {
        g_modeMgr.gameMode->SendMsg(50, 4, 0, 0, 0);
        break;
      }

      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 2, 24);
      name[24] = '\0';
      g_windowMgr.sub_A978D0(32)->SendMsg(nullptr, 50, reinterpret_cast<std::intptr_t>(name), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1A),
                                          *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1E), 0);
      this->exchangeName.assign(name, strlen(name));
      break;
    }
    case 0x1F6: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      *reinterpret_cast<int *>(this->unknown_028C) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      *reinterpret_cast<int *>(this->unknown_028C + 4) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
      g_windowMgr.sub_A978D0(105)->SendMsg(nullptr, 91, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), reinterpret_cast<std::intptr_t>(name), 0,
                                           0);
      break;
    }
    case 0x1FC: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 23;
      if (count == 0) {
        g_windowMgr.Confirm(LookupMessage(424), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      g_windowMgr.CloseWindow(107);
      UIWindow *window = g_windowMgr.sub_A978D0(107);
      const unsigned char *record = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, record += 23) {
        window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(record), 0, 0, 0);
      }
      break;
    }
    case 0x1FE:
      switch (g_gameModePacketBuffer[4]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(813)), 0xFFFF00, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(814)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0x1FF: {
      vector3d position{0.0f, 0.0f, 0.0f};
      static_cast<CGameMode *>(g_modeMgr.GetGameMode())
          ->CellCenter(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8),
                       position.x, position.z);
      position.y = this->scene->terrain->Height(position.x, position.z);
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        this->scene->player->unknown_02D0 = position.x;
        this->scene->player->unknown_02D4 = position.z;
        this->scene->player->render.position = position;
      } else if (CGameActor *actor =
                     static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        actor->unknown_02D0 = position.x;
        actor->unknown_02D4 = position.z;
        actor->render.position = position;
      }
      break;
    }
    case 0x205: {
      char message[1024];
      sprintf(message, LookupMessage(836), g_session.DecodePlayerName(), reinterpret_cast<const char *>(g_gameModePacketBuffer + 2));
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFF00, 0, 0, 0);
      break;
    }
    case 0x206:
      g_session.UpdateFriendName(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
                                 reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xB));
      if (g_session.SetFriendOffline(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2),
                                     *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6), g_gameModePacketBuffer[0xA])) {
        g_session.RefreshWindow02C8();
      }
      break;
    case 0x207: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      const unsigned char option = g_serviceOptions.Get(136);
      if (IsMonsterJob(g_session.m_aid) || option) {
        g_modeMgr.gameMode->SendMsg(175, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                                    0, 0);
      } else {
        g_windowMgr.sub_A978D0(109)->SendMsg(nullptr, 95, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2),
                                             *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), reinterpret_cast<std::intptr_t>(name), 0);
      }
      break;
    }
    case 0x209:
      sub_D1FBA0(g_gameModePacketBuffer);
      break;
    case 0x20A:
      if (g_session.RemoveFriendByCharacterId(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6))) {
        g_session.RefreshWindow02C8();
      }
      break;
    case 0x216:
      switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(899)), 0x9B9BFF, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(900)), 0x9B9BFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(916)), 0x9B9BFF, 0, 0, 0);
        break;
      }
      break;
    case 0x221: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 23;
      if (count == 0) {
        g_windowMgr.Confirm(LookupMessage(424), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      if (g_windowMgr.LookupWindow(111)) {
        g_windowMgr.CloseWindow(111);
      }
      UIWindow *window = g_windowMgr.sub_A978D0(111);
      const unsigned char *record = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, record += 23) {
        window->SendMsg(nullptr, 31, *reinterpret_cast<const int *>(record + 2), *reinterpret_cast<const std::int16_t *>(record), 0, 0);
      }
      break;
    }
    case 0x229: {
      int state[4];
      state[1] = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA);
      state[3] = g_gameModePacketBuffer[0xE];
      state[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6);
      state[2] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8);
      CRenderObject *actor;
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        actor = this->scene->player;
        this->ApplyStatusIcons(state[0], state[2]);
        reinterpret_cast<int *>(g_session.unknown_0F0C)[0] = state[0];
        reinterpret_cast<int *>(g_session.unknown_0F0C)[1] = state[2];
        g_session.m_jobFlags = state[1];
        if (g_windowMgr.unknown_01E0) {
          g_windowMgr.unknown_01E0->SendMsg(nullptr, 60, 0, 0, 0, 0);
        }
        if (g_session.unknown_5C1C <= 0) {
          g_session.ClearItems1740();
          g_windowMgr.CloseWindow(40);
        }
      } else {
        actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2));
      }
      if (actor) {
        const int stateAddress = reinterpret_cast<int>(state);
        actor->SendMsg(nullptr, 43, 0, stateAddress, stateAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case 0x253:
      g_windowMgr.sub_A978D0(119)->SendMsg(nullptr, 91, static_cast<signed char>(g_gameModePacketBuffer[2]), 0, 0, 0);
      break;
    case 0x25A: {
      const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 6) >> 2;
      UIWindow *window = g_windowMgr.sub_A978D0(94);
      if (!window) {
        break;
      }
      window->SendMsg(nullptr, 75, 0, 0, 0, 0);
      window->SendMsg(nullptr, 34, *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4), 0, 0, 0);
      const unsigned char *entry = g_gameModePacketBuffer + 6;
      for (unsigned int i = count; i != 0; --i, entry += 4) {
        window->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(entry), 0, 0, 0);
      }
      break;
    }
    case 0x2B4: {
      std::string title = g_renewQuestMgr->GetOngoingQuest(*reinterpret_cast<const unsigned int *>(g_gameModePacketBuffer + 2)).m_title;
      CRenewQuestMgr *manager = g_renewQuestMgr;
      unsigned int id = *reinterpret_cast<const unsigned int *>(g_gameModePacketBuffer + 2);
      manager->m_ongoingQuests.erase(id);
      manager->sub_67B6B0();
      std::string message = FormatOwnedString(LookupMessage(1611), title.c_str());
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF, 19, 0, 0);
      break;
    }
    case 0x2B7:
      g_renewQuestMgr->SetOngoingQuestState(*reinterpret_cast<const unsigned int *>(g_gameModePacketBuffer + 2), g_gameModePacketBuffer[6]);
      g_renewQuestMgr->sub_67B6B0();
      break;
    case 0x2D3: {
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2));
      if (item.unknown_0000[1]) {
        std::string name = item.DisplayName(0);
        std::string message = FormatOwnedString(LookupMessage(1328), name.c_str());
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0x6EFFFF, 0, 0, 0);
      }
      break;
    }
    case 0x2DD:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        g_session.unknown_0F24[0] = g_gameModePacketBuffer[0x1E];
        break;
      }
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        const unsigned char team = g_gameModePacketBuffer[0x1E];
        reinterpret_cast<unsigned char *>(&actor->render.flags)[3] = team;
        if (team != g_session.unknown_0F24[0]) {
          OpenWordDisplay(actor);
        } else if (actor->unknown_018C) {
          g_windowMgr.Schedule(reinterpret_cast<UIWindow *>(actor->unknown_018C));
          actor->unknown_018C = 0;
        }
      }
      break;
    case 0x2DE:
      g_session.m_score1 = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      g_session.m_score2 = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      break;
    case 0x2DF:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) != g_session.m_aid) {
        this->SendMsg(63, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x20),
                      *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x22), 0);
      }
      break;
    case 0x2E0:
      if (g_session.IsCompanionAid(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        break;
      }
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        const int x = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1E);
        const int y = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x20);
        actor->SendMsg(nullptr, 34, 0, x, x >> 31, y, y >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0x2E1:
      this->ReceiveActorAction(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x16),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1A), g_gameModePacketBuffer[0x1C],
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1D), 0);
      break;
    case 0x2E8: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemExpiryRecord);
      const CardItemExpiryRecord *entry = reinterpret_cast<const CardItemExpiryRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++entry) {
        const CardItemRecord *record = &entry->base;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        item.unknown_0068 = entry->expireTime;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case 0x2E9: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemExpiryRecord);
      const CardItemExpiryRecord *entry = reinterpret_cast<const CardItemExpiryRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++entry) {
        const CardItemRecord *record = &entry->base;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        item.unknown_0068 = entry->expireTime;
        g_session.AddItem1740(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case 0x2EA: {
      g_windowMgr.sub_A978D0(33);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemExpiryRecord);
      const CardItemExpiryRecord *entry = reinterpret_cast<const CardItemExpiryRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++entry) {
        const CardItemRecord *record = &entry->base;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.unknown_005D = 0;
        item.unknown_0060 = 0;
        item.unknown_0068 = entry->expireTime;
        g_session.AddItem1738(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case 0x2EF:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        g_session.unknown_0F30[0] = g_gameModePacketBuffer[6];
      } else if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        *reinterpret_cast<std::uint16_t *>(actor->render.unknown_00ECBytes) = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6);
      }
      break;
    case 0x2F0:
      if (CPlayer *player = this->scene->player) {
        player->SendMsg(nullptr, 82, 0, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6) * 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        g_session.unknown_555C[0x0A] = true;
      }
      break;
    case 0x2F2:
      if (CPlayer *player = this->scene->player) {
        player->SendMsg(nullptr, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if (*reinterpret_cast<const int *>(&player->render.unknown_0060) == 8) {
          player->Slot15(0, 1);
        }
      }
      break;
    case 0x2F7: {
      const std::uint32_t flags = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA);
      g_guildInfo.unknown_00AC = flags & 1;
      g_guildInfo.unknown_00B4 = g_gameModePacketBuffer[0xE];
      g_guildInfo.m_guildId = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      g_guildInfo.unknown_00B0 = (flags >> 4) & 1;
      g_guildInfo.unknown_00D0 = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2B);
      this->scene->player->Slot49(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6));
      if (!g_guildInfo.unknown_00C8) {
        g_guildInfo.unknown_00C8 = 1;
        g_modeMgr.gameMode->SendMsg(107, 3, 0, 0, 0);
      }
      if (!g_guildInfo.unknown_00C0) {
        g_guildInfo.unknown_00C0 = 1;
        g_modeMgr.gameMode->SendMsg(107, 1, 0, 0, 0);
      }
      break;
    }
    case 0x441:
      g_session.RemoveSkill(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2));
      break;
    case 0x442: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 8) >> 1;
      if (count == 0) {
        break;
      }
      if (UIWindow *window = g_windowMgr.sub_A978D0(99)) {
        window->SendMsg(nullptr, 27, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), 0, 0, 0);
        const std::uint16_t *skills = reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8);
        for (int i = 0; i < count; ++i) {
          window->SendMsg(nullptr, 31, skills[i], 0, 0, 0);
        }
      }
      break;
    }
    case 0x7DD: {
      char message[256];
      memset(message, 0, sizeof(message));
      const char *job = g_session.GetJobName(*reinterpret_cast<const unsigned *>(g_gameModePacketBuffer + 0x1A), -1);
      sprintf(message, LookupMessage(1930), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E),
              reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), job);
      if (g_windowMgr.window_027C) {
        g_windowMgr.SendNotice(5, reinterpret_cast<std::intptr_t>(message), 0x222222, 0, 0, 0);
      } else if (!g_serviceOptions.Get(114) && !g_serviceOptions.Get(148)) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFF00, 1, 0, 0);
      }
      if (g_session.m_whisperRecord) {
        this->WriteChatLog(message);
      }
      break;
    }
    case 0x7E1:
      this->ReceiveSkillUpdate(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), g_gameModePacketBuffer[0xE],
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8));
      break;
    case 0x7E6: {
      char text[256];
      sprintf(text, LookupMessage(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4)));
      char message[256];
      sprintf(message, "[%s] %s", CSkillInfo::DisplayName(*g_session.m_hotkeyScript, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)),
              text);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFCDCD, 11, 0, 0);
      break;
    }
    case 0x7FB:
      this->ProcessSkillUsePacket(g_gameModePacketBuffer, 1);
      break;
    case 0x85A: {
      std::uint16_t packetType = 0x884;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x85B: {
      std::uint16_t packetType = 0x885;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x85C: {
      std::uint16_t packetType = 0x886;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x85D: {
      std::uint16_t packetType = 0x887;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x85E: {
      std::uint16_t packetType = 0x888;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x85F:
      SendPacketHeader(0x889);
      break;
    case 0x860: {
      std::uint16_t packetType = 0x88A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x861:
      SendPacketHeader2(0x88B);
      break;
    case 0x862: {
      std::uint16_t packetType = 0x88C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x863:
      SendPacketHeader2(0x88D);
      break;
    case 0x864: {
      std::uint16_t packetType = 0x88E;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x865: {
      std::uint16_t packetType = 0x88F;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x866: {
      std::uint16_t packetType = 0x890;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x867: {
      std::uint16_t packetType = 0x891;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x868: {
      std::uint16_t packetType = 0x892;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x869: {
      std::uint16_t packetType = 0x893;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x86A: {
      std::uint16_t packetType = 0x894;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x86B: {
      std::uint16_t packetType = 0x895;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x86C: {
      std::uint16_t packetType = 0x896;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x86D: {
      std::uint16_t packetType = 0x897;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x86E: {
      std::uint16_t packetType = 0x898;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x86F: {
      std::uint16_t packetType = 0x899;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x870: {
      std::uint16_t packetType = 0x89A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x871: {
      std::uint16_t packetType = 0x89B;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x872: {
      std::uint16_t packetType = 0x89C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x873: {
      std::uint16_t packetType = 0x89D;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x874: {
      std::uint16_t packetType = 0x89E;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x875: {
      std::uint16_t packetType = 0x89F;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x876: {
      std::uint16_t packetType = 0x8A0;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x877: {
      std::uint16_t packetType = 0x8A1;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x878:
      SendPacketHeader(0x8A2);
      break;
    case 0x879: {
      std::uint16_t packetType = 0x8A3;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x87A: {
      std::uint16_t packetType = 0x8A4;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x87B:
      SendPacketHeader2(0x8A5);
      break;
    case 0x87C: {
      std::uint16_t packetType = 0x8A6;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x87D: {
      std::uint16_t packetType = 0x8A7;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x87E: {
      std::uint16_t packetType = 0x8A8;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x87F: {
      std::uint16_t packetType = 0x8A9;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x880: {
      std::uint16_t packetType = 0x8AA;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x881: {
      std::uint16_t packetType = 0x8AB;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x882:
      SendPacketHeader(0x8AC);
      break;
    case 0x883: {
      std::uint16_t packetType = 0x8AD;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x917:
      SendPacketHeader2(0x941);
      break;
    case 0x918: {
      std::uint16_t packetType = 0x942;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x919: {
      std::uint16_t packetType = 0x943;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x91A: {
      std::uint16_t packetType = 0x944;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x91B:
      SendPacketHeader2(0x945);
      break;
    case 0x91C:
      SendPacketHeader(0x946);
      break;
    case 0x91D: {
      std::uint16_t packetType = 0x947;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x91E: {
      std::uint16_t packetType = 0x948;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x91F: {
      std::uint16_t packetType = 0x949;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x920: {
      std::uint16_t packetType = 0x94A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x921:
      SendPacketHeader(0x94B);
      break;
    case 0x922: {
      std::uint16_t packetType = 0x94C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x923: {
      std::uint16_t packetType = 0x94D;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x924: {
      std::uint16_t packetType = 0x94E;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x925:
      SendPacketHeader2(0x94F);
      break;
    case 0x926: {
      std::uint16_t packetType = 0x950;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x927: {
      std::uint16_t packetType = 0x951;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x928: {
      std::uint16_t packetType = 0x952;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x929: {
      std::uint16_t packetType = 0x953;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x92A: {
      std::uint16_t packetType = 0x954;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x92B:
      SendPacketHeader(0x955);
      break;
    case 0x92C: {
      std::uint16_t packetType = 0x956;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x92D:
      SendPacketHeader2(0x957);
      break;
    case 0x92E: {
      std::uint16_t packetType = 0x958;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x92F:
      SendPacketHeader(0x959);
      break;
    case 0x930: {
      std::uint16_t packetType = 0x95A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x931:
      SendPacketHeader2(0x95B);
      break;
    case 0x932: {
      std::uint16_t packetType = 0x95C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x933:
      SendPacketHeader2(0x95D);
      break;
    case 0x934:
      SendPacketHeader2(0x95E);
      break;
    case 0x935: {
      std::uint16_t packetType = 0x95F;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x936: {
      std::uint16_t packetType = 0x960;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x937: {
      std::uint16_t packetType = 0x961;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x938: {
      std::uint16_t packetType = 0x962;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x939: {
      std::uint16_t packetType = 0x963;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x93A:
      SendPacketHeader(0x964);
      break;
    case 0x93B:
      SendPacketHeader(0x965);
      break;
    case 0x93C: {
      std::uint16_t packetType = 0x966;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x93D: {
      std::uint16_t packetType = 0x967;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x93E: {
      std::uint16_t packetType = 0x968;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x93F: {
      std::uint16_t packetType = 0x969;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x940: {
      std::uint16_t packetType = 0x96A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case 0x7FD:
      this->ReceiveItemObtainBroadcast(g_gameModePacketBuffer, false);
      break;
    case 0x80E: {
      if (!g_session.IsCompanionAid(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
          const int hp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
          const int maxHp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA);
          actor->SendMsg(nullptr, 34, 0, hp, hp >> 31, maxHp, maxHp >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        if (g_windowMgr.window_02C8) {
          g_windowMgr.window_02C8->SendMsg(nullptr, 6, 34, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2),
                                           *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                                           *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
        }
        g_windowMgr.NotifyItemWindows(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                                      *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
      }
      break;
    }
    case 0x80F:
      sub_D1F770(g_gameModePacketBuffer);
      break;
    case 0x83D:
      g_session.unknown_5BE8 = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      g_session.unknown_5BEC = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      break;
    case 0x845:
    case 0xA2B:
      if (!g_windowMgr.CloseWindow(318)) {
        if (UIWindow *window = g_windowMgr.sub_A978D0(318)) {
          window->SendMsg(nullptr, 120, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                          0, 0);
        } else {
          std::int16_t packetType = 0x844;
          GetRagConnection().Send(reinterpret_cast<const PacketLengths &>(GetRagConnection()).FixedLength(packetType), &packetType);
        }
      }
      break;
    case 0x84B: {
      const unsigned char *packet = g_gameModePacketBuffer;
      ItemFallEntry entry = {};
      entry.count = *reinterpret_cast<const std::int16_t *>(packet + 0x13);
      entry.aid = *reinterpret_cast<const std::uint32_t *>(packet + 2);
      entry.column = *reinterpret_cast<const std::int16_t *>(packet + 0x0D);
      entry.row = *reinterpret_cast<const std::int16_t *>(packet + 0x0F);
      entry.subColumn = packet[0x11];
      entry.subRow = packet[0x12];
      entry.falling = 0;
      _itoa(*reinterpret_cast<const int *>(packet + 6), entry.itemId, 10);
      entry.identified = packet[0x0C];
      entry.falling = 1;
      entry.unknown_003E = *reinterpret_cast<const std::uint16_t *>(packet + 0x0A);

      CItem *item = this->scene->GetItemByAID(entry.aid);
      if (!item) {
        item = new CItem;
        this->scene->objects.push_back(item);
        this->scene->unknown_0018.push_back(item);
      }

      const int entryAddress = reinterpret_cast<int>(&entry);
      item->SendMsg(nullptr, 4, 0, entryAddress, entryAddress >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0x8C8:
      this->ReceiveActorAction(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x16),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1B), g_gameModePacketBuffer[0x1D],
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E), g_gameModePacketBuffer[0x1A]);
      break;
    case 0x8CD: {
      const ActorStopPacket *packet = reinterpret_cast<const ActorStopPacket *>(g_gameModePacketBuffer);
      this->StopActorMove(packet->aid, packet->x, packet->y, 1);
      break;
    }
    case 0x8D0: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->unknown_0100[0x04]) = timeGetTime() - 2000;
      switch (packet[8]) {
      case 0: {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(packet + 2));
        if (item.unknown_0000[1]) {
          item.unknown_0070 = *reinterpret_cast<const std::uint16_t *>(packet + 6);
          item.unknown_0000[3] = *reinterpret_cast<const std::uint16_t *>(packet + 4);
          g_session.OnItemEquipped(item);
        }
        break;
      }
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1953)), 0xFF, 7, 0, 0);
        break;
      }
      break;
    }
    case 0x8D1: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->unknown_0100[0x08]) = timeGetTime() - 2000;
      switch (packet[6]) {
      case 0: {
        const std::uint16_t location = *reinterpret_cast<const std::uint16_t *>(packet + 4);
        if (location) {
          if (g_session.IsCostumeLocation(location)) {
            g_session.UnequipItem2B50(location);
          } else {
            g_session.UnequipItem(location, 0);
          }
        }
        break;
      }
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1953)), 0xFF, 7, 0, 0);
        break;
      }
      break;
    }
    case 0x8D3:
      if (UIWindow *window = g_windowMgr.LookupWindow(318)) {
        window->SendMsg(nullptr, 122, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), 0,
                        0);
      }
      break;
    case 0x8D8:
      g_entryQueueMgr->OnApplyResult(reinterpret_cast<const EntryQueueApplyResult *>(g_gameModePacketBuffer));
      break;
    case 0x8D9:
      g_entryQueueMgr->OnStatus(reinterpret_cast<const EntryQueueStatus *>(g_gameModePacketBuffer));
      break;
    case 0x8DB:
      g_entryQueueMgr->OnCancelResult(reinterpret_cast<const EntryQueueNotice *>(g_gameModePacketBuffer));
      break;
    case 0x8DC:
      g_entryQueueMgr->IgnorePacket(g_gameModePacketBuffer);
      break;
    case 0x8DE:
      g_entryQueueMgr->OnResultNotice(reinterpret_cast<const EntryQueueNotice *>(g_gameModePacketBuffer));
      break;
    case 0x8DF:
      g_entryQueueMgr->OnReady(reinterpret_cast<const EntryQueueReady *>(g_gameModePacketBuffer));
      break;
    case 0x8E1:
      g_entryQueueMgr->OnNotify(reinterpret_cast<const EntryQueueNotice *>(g_gameModePacketBuffer));
      break;
    case 0x8E2:
      sub_D73A10(g_gameModePacketBuffer);
      break;
    case 0x90E:
      g_entryQueueMgr->ResetCurrent(reinterpret_cast<int>(g_gameModePacketBuffer));
      break;
    case 0x99A:
      this->ReceiveTakeOffEquipResult(g_gameModePacketBuffer[0x8], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4),
                                      *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0x9BB:
      ReceiveBuyingStoreResult(reinterpret_cast<const BuyingStoreResultPacket *>(g_gameModePacketBuffer));
      break;
    case 0x9BF:
      sub_D1C050(reinterpret_cast<const AchievementStatePacket *>(g_gameModePacketBuffer));
      break;
    case 0x9CF:
      g_nProtectGameGuardMgr->ProcessAuth(reinterpret_cast<int>(g_gameModePacketBuffer + 0x4),
                                          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2) - 4);
      break;
    case 0x9DF:
      this->sub_D079A0(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0x9E6:
      sub_D08640(g_gameModePacketBuffer);
      break;
    case 0x9FC:
      this->NotifyResult2571(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xA09:
      sub_D1F970(g_gameModePacketBuffer);
      break;
    case 0xA26:
      this->ReceiveAchievementResult(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xA27:
      this->ReceiveHpSpRecovery(g_gameModePacketBuffer, 1);
      break;
    case 0xA29:
      this->ReceivePacket0A29(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xA2F:
      this->ReceiveJobTitle(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xA38:
      this->OpenServiceWindow(g_gameModePacketBuffer[0x2]);
      break;
    case 0xA3A:
      this->UpdateSkillCooldown(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x4),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x8));
      break;
    case 0xA4A:
      this->NotifyWindow4F258(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xA4B:
      this->RequestLocalMapMove(reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x2),
                                *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x12),
                                *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x14));
      break;
    case 0xA4E:
      this->ReceiveLapineBoxIngredient(reinterpret_cast<const unsigned char *>(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2)));
      break;
    case 0xA50:
      this->ReceiveLapineBoxResult(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xA53:
      this->ReceiveCaptchaUploadRequest(reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2),
                                        *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case 0xA55:
      this->NotifyCaptchaRegistered();
      break;
    case 0xA57:
      this->NotifyResult2877(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xA58:
      this->BeginCaptchaImage(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2),
                              reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x4));
      break;
    case 0xA5B:
      this->SetMacroDetectorCountdown(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xA5D:
      this->ReceiveMacroDetectorResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xA60:
      this->NotifyResult3995(g_gameModePacketBuffer[0x2]);
      break;
    case 0xA6A:
      this->ReceiveCaptchaPreview(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2),
                                  *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x6),
                                  reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x8));
      break;
    case 0xA6D:
      sub_D6AD60((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 2),
                 reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4));
      break;
    case 0xA6F:
      this->ShowFormattedNotice(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x4),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x6), reinterpret_cast<int>(g_gameModePacketBuffer + 0xA));
      break;
    case 0xA81:
      this->ReceiveStoreResult(g_gameModePacketBuffer[0x2], g_gameModePacketBuffer[0x3]);
      break;
    case 0xA8A:
      this->ReceiveItemDisappear(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xA93:
      this->OpenBuyingStoreMake(g_gameModePacketBuffer[0x2]);
      break;
    case 0xA95:
      this->ReceiveFeedConfig(g_gameModePacketBuffer[0x2], g_gameModePacketBuffer[0x3]);
      break;
    case 0xAA0:
      g_windowMgr.sub_A978D0(295);
      break;
    case 0xAB4:
      this->ReceiveLapineUpgradeBoxValue(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xAB7:
      this->ReceiveLapineUpgradeResult(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xAC3:
      this->IgnoreRemovedPacket6();
      break;
    case 0xAE2:
      this->ReceiveServiceRequest(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xAF0:
      this->ReceiveWindow328Result(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case 0xAF5:
      SetActiveSkillEffect184(g_gameModePacketBuffer[0x2]);
      break;
    case 0xAFA:
      ReceivePartyJoinResult(reinterpret_cast<const void *>(g_gameModePacketBuffer));
      break;
    case 0xAFB:
      this->ReceiveWindow99Values((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 2),
                                  reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4));
      break;
    case 0xB0B:
      this->RefreshItemListWindow(g_gameModePacketBuffer[0x2], g_gameModePacketBuffer[0x3]);
      break;
    case 0xB2B:
      this->ReceiveGuildUnknown0114Change(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x3),
                                          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x7));
      break;
    case 0xB2D:
      this->ReceiveAgitNotice(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3),
                              *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x7));
      break;
    case 0xB2E:
      this->ReceivePacket0B2E(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xB30:
      this->ReceiveObjectsDisappear((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 2),
                                    reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x4));
      break;
    case 0xB47:
      this->ReceiveGuildEmblemVersion(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA),
                                      *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2),
                                      *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case 0xB49:
      this->ReceiveWindow154Result(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xB4B:
      this->ReceiveWindow154Close(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xB50:
      this->OpenWindow154();
      break;
    case 0xB52:
      sub_D6E440();
      break;
    case 0xB54:
      this->ReceiveUnknownD79E90(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2),
                                 *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4));
      break;
    case 0xB68:
      this->ReceiveActorStateValue(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2),
                                   *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                                   *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA));
      break;
    case 0xB6E:
      this->ReceiveCashShopBanner(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                                  *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
      break;
    case 0xB8E:
      this->ReceiveReputeOpen(*reinterpret_cast<const std::uint64_t *>(g_gameModePacketBuffer + 0x2),
                              *reinterpret_cast<const std::uint64_t *>(g_gameModePacketBuffer + 0xA));
      break;
    case 0xB8F:
      this->ReceiveItemReformOpen(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xB92:
      this->ReceiveWindow348Result(g_gameModePacketBuffer[0x4], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xB94:
      this->ReceiveGameOptionResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                                    *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
      break;
    case 0xB95:
      this->ReceiveGameOptions((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 3),
                               reinterpret_cast<const GameOptionEntry *>(g_gameModePacketBuffer + 0x4));
      break;
    case 0xB99:
      this->ReceiveRodexMailDeleteResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2),
                                         *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case 0xB9A:
      this->ReceiveUiOpen(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint64_t *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xB9F:
      this->ReceiveEnchantResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case 0xBA1:
      this->SetWindow16Align(g_gameModePacketBuffer[0x2]);
      break;
    case 0xBA2:
      this->ResizeWindow16(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xBA3:
      this->MoveWindow16(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case 0xBAA:
      this->RejectRequest(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
                          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12));
      break;
    case 0xBAB:
      this->IgnoreRemovedPacket12(reinterpret_cast<const unsigned char *>(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2)),
                                  *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA),
                                  *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12));
      break;
    case 0xBAE:
      this->ReceiveBasicInfoResult(g_gameModePacketBuffer[0x2]);
      break;
    case 0xBB5:
      this->PlaceWindow16(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2),
                          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x6));
      break;
    case 0xBBA:
      this->ReceiveItemObtainBroadcast(g_gameModePacketBuffer, 1);
      break;
    case 0xBBC:
      this->ReceiveShopName(reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12));
      break;
    case 0xBBE:
      this->ShowSpecialPopup(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xBC2:
      this->NotifyResult4013(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xBC5:
      this->ActivateMacroBlackListCheck(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xBC7:
      this->ReceiveMacroBlackListResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xBD7:
      this->ApplyRuneSetResult(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x3),
                               *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x5));
      break;
    case 0xBDB:
      this->ReceiveMacroBlackListCode(g_gameModePacketBuffer[0x2], reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x3));
      break;
    case 0xBDF:
      this->ToggleWindow169(g_gameModePacketBuffer[0x2]);
      break;
    case 0xBE1:
      this->RequestWindow169(g_gameModePacketBuffer[0x2]);
      break;
    case 0xC06:
      this->ReceiveResultNotice(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4));
      break;
    case 0xC0C:
      this->NotifyResult4138(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xC3C:
      this->ReceiveToggle128(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xAA9:
      ReceiveTradeItems(static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(TradeItemRecord),
                        reinterpret_cast<const TradeItemRecord *>(g_gameModePacketBuffer + 4));
      break;
    case 0xA59:
      this->ReceiveCaptchaImageData(reinterpret_cast<const char *>(g_gameModePacketBuffer + 4), g_gameModePacketBuffer + 8,
                                    static_cast<std::uint16_t>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 8));
      break;
    case 0xA6B:
      this->ReceiveCaptchaPreviewImage(reinterpret_cast<const char *>(g_gameModePacketBuffer + 4), g_gameModePacketBuffer + 8,
                                       static_cast<std::uint16_t>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 8));
      break;
    case 0xBF4:
      this->ReceiveQuestStates(static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 5,
                               g_gameModePacketBuffer + 4);
      break;
    case 0xA7E:
      this->OpenVendingMake(g_gameModePacketBuffer[4], static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) >> 1,
                            reinterpret_cast<const short *>(g_gameModePacketBuffer + 5));
      break;
    case 0xAA8:
      this->ReceiveFeedConfig2(g_gameModePacketBuffer[2], g_gameModePacketBuffer[3], g_gameModePacketBuffer[4]);
      break;
    case 0xB8D:
      this->ReceiveReputeInfo(g_gameModePacketBuffer[4],
                              static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) >> 4,
                              reinterpret_cast<const ReputePointEntry *>(g_gameModePacketBuffer + 5));
      break;
    case 0xB25:
      this->ReceiveTraitStatusChangeAck(g_gameModePacketBuffer[4], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                        g_gameModePacketBuffer[5]);
      break;
    case 0xA51:
      this->ReceiveRodexRecipient(
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8), reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA));
      break;
    case 0xADA:
      this->ReceiveRefineStatus(reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1A),
                                g_gameModePacketBuffer[0x1E], g_gameModePacketBuffer[0x1F]);
      break;
    case 0xBD5:
      this->ApplyRuneSetResultWithRequest(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3),
                                          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 5), -1, -1);
      break;
    case 0xB96:
      this->ShowNoticeText(reinterpret_cast<const char *>(g_gameModePacketBuffer + 2));
      break;
    case 0xBCF:
      this->ReceiveRuneGradeResult(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3), 1,
                                   *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 5), 0, 0);
      break;
    case 0x999:
      this->ReceiveWearEquipAck(g_gameModePacketBuffer[0xA], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
                                *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4));
      break;
    case 0xB09:
      this->ReceiveItemListPart(g_gameModePacketBuffer[4],
                                static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) / 34,
                                g_gameModePacketBuffer + 5);
      break;
    case 0xB0A:
      this->ReceiveEquipListPart(g_gameModePacketBuffer[4],
                                 static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) / 67,
                                 g_gameModePacketBuffer + 5);
      break;
    case 0xAAB:
      this->ReceiveParaItemResult(g_gameModePacketBuffer[4],
                                  static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) / 29,
                                  g_gameModePacketBuffer + 5);
      break;
    case 0x9E5:
      sub_D08110(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                 *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
                 *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6));
      break;
    case 0xC17:
      this->ReceiveRuneRewardResult(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3),
                                    *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 5), g_gameModePacketBuffer[9]);
      break;
    case 0xADC:
      this->ReceiveFeedConfig3(g_gameModePacketBuffer[2], g_gameModePacketBuffer[3], g_gameModePacketBuffer[4], g_gameModePacketBuffer[5]);
      break;
    case 0x9CB:
      this->ReceiveSkillUse(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
                            *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xC),
                            *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 8), g_gameModePacketBuffer[0x10]);
      break;
    case 0xA23:
      sub_D6C460(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 8),
                 *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
                 *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12),
                 reinterpret_cast<const AchievementRecordPacket *>(g_gameModePacketBuffer + 0x16));
      break;
    case 0xB36:
      CGuildMgr::ReceiveEmblem(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
                               *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
                               *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA), g_gameModePacketBuffer + 0xE,
                               *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 14);
      break;
    case 0xA4C:
      this->ReceiveZoneServerChange(
          reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x12),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x14), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x16),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1A));
      break;
    case 0xC3B:
      this->ReceiveRefineStatusGraded(reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1A),
                                      g_gameModePacketBuffer[0x1E], g_gameModePacketBuffer[0x1F], g_gameModePacketBuffer[0x20]);
      break;
    case 0xBC9:
      this->ReceiveFeedConfig4(g_gameModePacketBuffer[2], g_gameModePacketBuffer[3], g_gameModePacketBuffer[4], g_gameModePacketBuffer[5],
                               *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6));
      break;
    case 0xAA2:
      this->ApplyRefiningMaterial(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
                                  static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 7) / 9,
                                  g_gameModePacketBuffer + 7, g_gameModePacketBuffer[6]);
      break;
    case 0xBE3:
      this->ReceiveNamedResult(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), reinterpret_cast<const char *>(g_gameModePacketBuffer + 6),
                               *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E));
      break;
    case 0xA41:
      this->SpawnSkillWarningPlanes(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6),
          *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE));
      break;
    case 0xBD1:
    case 0xBD3:
      this->ReceiveRuneGradeResult(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3), 0,
                                   *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 5),
                                   *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 9),
                                   *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xB));
      break;
    case 0x973:
      this->unknown_024C = 1;
      this->unknown_02DC = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      g_windowMgr.sub_A978D0(226)->SendMsg(nullptr, 25, 0, 0, 0, 0);
      break;
    case 0xBEE:
      switch (g_gameModePacketBuffer[4]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_NOT_ENOUGH_NYANGVINE")), 0xFF, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_DATE")), 0xFF, 0, 0, 0);
        this->RefreshEmotionSaleSchedule();
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_ALREADY_BUY")), 0xFF, 0, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_ANOTHER_SALE_BUY")), 0xFF, 0, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_NOT_ENOUGH_BASICSKILL_LEVEL")), 0xFF, 0,
                               0, 0);
        break;
      case 5:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_NOT_YET_SALE_START_TIME")), 0xFF, 0, 0, 0);
        this->RefreshEmotionSaleSchedule();
        break;
      default:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_UNKNOWN")), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0xB8C: {
      const unsigned char type = g_gameModePacketBuffer[4];
      this->PlayBgm(
          type, std::string(reinterpret_cast<const char *>(g_gameModePacketBuffer + 5), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)));
      break;
    }
    case 0xA8D:
      this->ReceiveVendingItemList2(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 8),
                                    g_gameModePacketBuffer[0xC],
                                    static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 0x11) / 63,
                                    *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xD), g_gameModePacketBuffer + 0x11);
      break;
    case 0xA24:
      this->ReceiveAchievementUpdate(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2),
                                     *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6),
                                     *reinterpret_cast<const int *>(g_gameModePacketBuffer + 8), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xC),
                                     *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x10), g_gameModePacketBuffer[0x14],
                                     reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x15),
                                     *reinterpret_cast<const __time32_t *>(g_gameModePacketBuffer + 0x3D), g_gameModePacketBuffer[0x41]);
      break;
    case 0xB33:
      this->ReceiveSkillUpdate(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), g_gameModePacketBuffer[0xE],
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xF));
      break;
    case 0xB31:
      this->ReceiveLearnedSkill(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), g_gameModePacketBuffer[0xE],
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xF));
      break;
    case 0xA91:
      this->ReceiveVendingItemList(
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 8), g_gameModePacketBuffer[0xC],
          static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 0x19) / 11,
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xD), g_gameModePacketBuffer + 0x19,
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x11), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x15));
      break;
    case 0xBF2:
      g_runeSystemMgr->ClearCurRuneSet();
      this->ApplyRuneSetResultWithRequest(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3),
                                          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 5),
                                          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 9),
                                          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xB));
      break;
    case 0x9D1:
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        actor->SendMsg(nullptr, 82, 0, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA) * 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      break;
    case 0xAE7: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      g_requestJoinPartyMgr->Add(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), name,
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x22), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x24));
      break;
    }
    case 0xAB9:
      this->ReceiveItemUpgrade(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const ItemCardSlots *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const ItemSlotOptions *>(g_gameModePacketBuffer + 0x16));
      break;
    case 0xB13:
      this->RefreshInventoryOrEquipItem(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), g_gameModePacketBuffer[4],
                                        *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 5),
                                        *reinterpret_cast<const ItemCardSlots *>(g_gameModePacketBuffer + 7),
                                        *reinterpret_cast<const ItemSlotOptions *>(g_gameModePacketBuffer + 0x17));
      break;
    case 0xBED:
      g_cashEmotionMgr->AcquirePack(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                    g_gameModePacketBuffer[4] ? *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 5) : 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_BUY_SUCCESS")), 0xFFFF, 0, 0, 0);
      this->RefreshEmotionPurchaseSchedule();
      break;
    case 0xA0E:
      if (!g_session.IsCompanionAid(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
          const int hp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
          const int maxHp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA);
          actor->SendMsg(nullptr, 34, 0, hp, hp >> 31, maxHp, maxHp >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
        }
      }
      break;
    case 0x990:
      this->ReceiveItemPickupAck5(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), g_gameModePacketBuffer[0xA], g_gameModePacketBuffer[0xB], g_gameModePacketBuffer[0xC],
          *reinterpret_cast<const ItemCardSlots *>(g_gameModePacketBuffer + 0xD), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x1D),
          g_gameModePacketBuffer[0x21], g_gameModePacketBuffer[0x22], *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x23),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x27));
      break;
    case 0xC08:
      this->ReceiveUnknownD264E0(g_gameModePacketBuffer);
      this->miniMapTooltipId = -1;
      this->miniMapTooltipText.assign("", 0);
      g_windowMgr.CloseWindow(78);
      g_windowMgr.RestoreHiddenWindows();
      this->miniMapTooltipWindows.clear();
      break;
    case 0xBEA: {
      CRenderObject *actor;
      if (g_session.m_aid == *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)) {
        actor = this->scene->player;
      } else {
        actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2));
        if (!actor) {
          this->unknown_026E[0x16] = 0;
          break;
        }
      }
      const int first = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6);
      const int second = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8);
      actor->SendMsg(nullptr, 160, 0, first, first >> 31, second, second >> 31, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    }
    case 0xBEB:
      switch (g_gameModePacketBuffer[6]) {
      case 0:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_USE_FAIL_DATE")), 0xFF, 0, 0, 0);
        g_cashEmotionMgr->RemoveOwnedPack(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), true, true);
        this->RefreshEmotionPurchaseSchedule();
        break;
      case 1:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_USE_FAIL_UNPURCHASED")), 0xFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_USE_FAIL_SKILL_LEVEL")), 0xFF, 0, 0, 0);
        break;
      default:
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_USE_FAIL_UNKNOWN")), 0xFF, 0, 0, 0);
        break;
      }
      this->unknown_026E[0x16] = 0;
      break;
    case 0x972: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->unknown_024C = 1;
      this->unknown_02DC = *reinterpret_cast<const int *>(packet + 4);
      unsigned int length = *reinterpret_cast<const std::uint16_t *>(packet + 2) - 9;
      char text[0x803];
      if (g_serviceType == 10) {
        sprintf(text, "|%02x", LanguageCharset(31));
        memcpy(text + 3, packet + 9, length);
        length += 3;
      } else {
        memcpy(text, packet + 9, length);
      }
      text[length] = 0;
      g_windowMgr.sub_A978D0(226)->SendMsg(nullptr, 26, reinterpret_cast<std::intptr_t>(text), *reinterpret_cast<const int *>(packet + 4), 0, 0);
      break;
    }
    case 0xBF6: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 10) / 7;
      CCashEmotionMgr *manager = g_cashEmotionMgr;
      if (!manager) {
        break;
      }
      manager->sub_59CDE0(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4));
      manager->SetServerTimeZone(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8));
      const unsigned char *entry = g_gameModePacketBuffer + 0xA;
      for (int i = 0; i < count; ++i, entry += 7) {
        manager->SetOwnedPack(*reinterpret_cast<const std::uint16_t *>(entry), entry[2] ? *reinterpret_cast<const std::uint32_t *>(entry + 3) : 0);
      }
      this->RefreshEmotionSaleSchedule();
      this->RefreshEmotionPurchaseSchedule();
      break;
    }
    case 0xA89:
      this->SpawnStandingPc(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC),
          g_gameModePacketBuffer[0xE], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xF),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x11), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x15),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x19), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1B),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1D), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1F),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x21), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x23),
          reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x25));
      break;
    case 0xB05:
      this->SpawnStandingPc2(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC),
          g_gameModePacketBuffer[0xE], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xF),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x11), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x15),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x19), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1B),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1D), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1F),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x21), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x23),
          reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x25), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x3D));
      break;
    case 0xB32: {
      std::vector<SkillListEntry> skills;
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(SkillListEntry);
      const SkillListEntry *record = reinterpret_cast<const SkillListEntry *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        skills.push_back(*record);
      }
      this->ReceiveSkillList(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer), skills);
      break;
    }
    case 0xA78:
      if (g_gameModePacketBuffer[2] == 1) {
        UIWindow *window = g_windowMgr.LookupWindow(289);
        if (!window) {
          window = g_windowMgr.sub_A978D0(289);
        }
        g_session.unknown_61B1 = !g_session.unknown_61B1;
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(g_session.unknown_61B1 == true ? "Camera Info On" : "Camera Info Off"), 0x64FFFF, 0, 0, 0);
        window->Show(g_session.unknown_61B1);
      } else if (CView *camera = this->camera) {
        camera->camera.desiredDistance = *reinterpret_cast<const float *>(g_gameModePacketBuffer + 3);
        this->camera->camera.desiredYaw = *reinterpret_cast<const float *>(g_gameModePacketBuffer + 7);
        this->camera->camera.desiredPitch = *reinterpret_cast<const float *>(g_gameModePacketBuffer + 0xB);
      }
      break;
    case 0xBC8:
      if (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x14) != 176) {
        this->SpawnGroundSkill(
            *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 8),
            *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x10),
            *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x12), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x14),
            g_gameModePacketBuffer[0x18], g_gameModePacketBuffer[0x19], g_gameModePacketBuffer[0x1A]);
        break;
      }
      if (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 27 > 0) {
        CGraffiSkill *skill = new CGraffiSkill;
        skill->SendMsg(nullptr, 22, 0, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        const int address = reinterpret_cast<int>(g_gameModePacketBuffer + 0x1B);
        const int x = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x10);
        const int y = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x12);
        skill->SendMsg(nullptr, 117, 0, address, address >> 31, x, x >> 31, y, y >> 31, 0, 0, 0, 0, 0, 0);
        this->scene->objects.push_back(skill);
        this->scene->unknown_0020.push_back(skill);
      }
      break;
    case 0xADB: {
      std::string text(reinterpret_cast<const char *>(g_gameModePacketBuffer + 8),
                       reinterpret_cast<const char *>(g_gameModePacketBuffer + *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)));
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), 0, 0, 0);
      break;
    }
    case 0xA28:
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        if (UIWindow *window = g_windowMgr.sub_A978D0(45)) {
          window->SendMsg(nullptr, 23, 0, 0, 0, 0);
        }
        if (UIMerchantShopMakeWnd *window = static_cast<UIMerchantShopMakeWnd *>(g_windowMgr.sub_A978D0(41))) {
          window->PrintSellList();
        }
        g_windowMgr.CloseWindow(41);
        this->unknown_024C = 1;
        break;
      case 1:
        this->SendMsg(40, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(43);
        g_windowMgr.CloseWindow(44);
        g_session.ClearItems1760And1748();
        g_session.ClearItems1758();
        g_windowMgr.Confirm(LookupMessage(2639), 0, true, false, 0, "Message", 280, 120, 0);
        g_windowMgr.CloseWindow(41);
        break;
      case 3:
        this->SendMsg(40, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(43);
        g_windowMgr.CloseWindow(44);
        g_session.ClearItems1760And1748();
        g_session.ClearItems1758();
        g_windowMgr.CloseWindow(41);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3229)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case 0xB55: {
      std::string text(reinterpret_cast<const char *>(g_gameModePacketBuffer + 4),
                       reinterpret_cast<const char *>(g_gameModePacketBuffer + *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)));
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    case 0x8FE: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 12;
      const unsigned char *entry = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, entry += 12) {
        const RenewQuestOngoingInfo &quest = g_renewQuestMgr->GetOngoingQuest(*reinterpret_cast<const unsigned int *>(entry));
        if (quest.m_title.empty()) {
          continue;
        }

        RenewQuestOngoingInfo info = quest;
        for (int index = i; index < 3; ++index) {
          if (*reinterpret_cast<const int *>(entry + 4) == info.m_huntUnknown00D4[index]) {
            info.m_huntUnknown00E0[index] = *reinterpret_cast<const std::int16_t *>(entry + 0xA);
            info.m_huntUnknown0180[index] = *reinterpret_cast<const std::int16_t *>(entry + 8);
            g_renewQuestMgr->SetOngoingQuest(*reinterpret_cast<const unsigned int *>(entry), info);
            break;
          }
        }
      }
      if (g_windowMgr.window_0428) {
        g_windowMgr.window_0428->Invalidate();
      }
      g_renewQuestMgr->sub_67B690();
      break;
    }
    case 0x900: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedItemRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedItemRecord *record = reinterpret_cast<const FlaggedItemRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.unknown_005D = 0;
        item.identified = record->flags & 1;
        item.unknown_0060 = 0;
        item.unknown_0068 = record->expireTime;
        item.unknown_0074[0] = (record->flags >> 1) & 1;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case 0x902: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedItemRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedItemRecord *record = reinterpret_cast<const FlaggedItemRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.unknown_005D = 0;
        item.identified = record->flags & 1;
        item.unknown_0060 = 0;
        item.unknown_0068 = record->expireTime;
        item.unknown_0074[0] = (record->flags >> 1) & 1;
        g_session.AddItem1740(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case 0x904: {
      g_windowMgr.sub_A978D0(33);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedItemRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedItemRecord *record = reinterpret_cast<const FlaggedItemRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = 0;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.unknown_005D = 0;
        item.identified = record->flags & 1;
        item.unknown_0060 = 0;
        item.unknown_0068 = record->expireTime;
        item.unknown_0074[0] = (record->flags >> 1) & 1;
        g_session.AddItem1738(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case 0x903: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedEquipRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedEquipRecord *record = reinterpret_cast<const FlaggedEquipRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = record->location;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->wearState;
        item.quantity = 1;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->flags & 1;
        item.unknown_005D = (record->flags >> 1) & 1;
        item.unknown_0060 = record->refine;
        item.unknown_0068 = record->expireTime;
        item.unknown_0064 = record->bindOnEquip;
        item.unknown_0070 = record->look;
        item.unknown_0074[0] = (record->flags >> 2) & 1;
        g_session.AddItem1740(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case 0x905: {
      g_windowMgr.sub_A978D0(33);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedEquipRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedEquipRecord *record = reinterpret_cast<const FlaggedEquipRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.unknown_0000[0] = record->type;
        item.unknown_0000[2] = record->location;
        item.unknown_0000[1] = record->index;
        item.unknown_0000[3] = record->wearState;
        item.quantity = 1;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.unknown_001C)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->flags & 1;
        item.unknown_005D = (record->flags >> 1) & 1;
        item.unknown_0060 = record->refine;
        item.unknown_0068 = record->expireTime;
        item.unknown_0064 = record->bindOnEquip;
        item.unknown_0070 = record->look;
        item.unknown_0074[0] = (record->flags >> 2) & 1;
        g_session.AddItem1738(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case 0xB76:
      StringFormat(reinterpret_cast<char *>(g_session.unknown_5570 + 0xC), 32, static_cast<std::size_t>(-1), "%s",
                   reinterpret_cast<const char *>(g_gameModePacketBuffer + 2));
      g_session.unknown_55F8 = g_gameModePacketBuffer[0x1A];
      *reinterpret_cast<int *>(g_session.unknown_55C0) = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1B);
      g_session.unknown_55F0 = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D);
      g_session.RefreshHomunculusInfo();
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D)) {
      case 1:
      case 5:
      case 10:
        if (*reinterpret_cast<int *>(g_session.unknown_5620) != *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D)) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1018)), 0xFF, 0, 0, 0);
          *reinterpret_cast<int *>(g_session.unknown_5620) = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D);
        }
        break;
      }
      g_session.unknown_55D4 = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1F);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x2C)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x21);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x30)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x23);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x34)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x25);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x38)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x27);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x3C)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x29);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x40)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2B);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x44)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2D);
      reinterpret_cast<int *>(g_session.unknown_5570 + 0x48)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2F);
      g_session.m_homunHp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x31);
      g_session.RefreshHomunculusInfo();
      *reinterpret_cast<int *>(g_session.unknown_55C8) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x35);
      g_session.RefreshHomunculusInfo();
      g_session.m_homunSp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x39);
      g_session.RefreshHomunculusInfo();
      g_session.unknown_55D0 = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3D);
      g_session.RefreshHomunculusInfo();
      g_session.m_homunExp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x41);
      g_session.RefreshHomunculusInfo();
      g_session.m_homunMaxExp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x45);
      g_session.RefreshHomunculusInfo();
      *reinterpret_cast<int *>(g_session.unknown_1664) = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x49);
      g_session.InvalidateWindowFrames();
      if (g_windowMgr.window_03C4) {
        g_windowMgr.window_03C4->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
      reinterpret_cast<int *>(g_session.unknown_5620)[1] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x4B);
      break;
    case 0xB0E: {
      g_barterMarketItemMgr->Clear();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 25;
      const unsigned char *record = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, record += 25) {
        BarterMarketItem item;
        char itemName[33] = {};
        _itoa(*reinterpret_cast<const int *>(record), itemName, 10);
        item.name.assign(itemName, strlen(itemName));
        item.field_1c = record[4];
        item.value = *reinterpret_cast<const int *>(record + 5);
        item.field_2c = *reinterpret_cast<const int *>(record + 9);
        item.field_28 = *reinterpret_cast<const int *>(record + 0xD);
        item.flag_24 = true;
        item.amount = *reinterpret_cast<const int *>(record + 0x11) / 10;
        item.flag_38 = *reinterpret_cast<const int *>(record + 5) == -1;
        item.id = *reinterpret_cast<const int *>(record + 0x15);
        g_barterMarketItemMgr->items.push_back(item);
      }
      g_windowMgr.sub_A978D0(334)->SendMsg(nullptr, 23, 0, 0, 0, 0);
      g_windowMgr.sub_A978D0(335)->SendMsg(nullptr, 23, 0, 0, 0, 0);
      break;
    }
    case 0x9D5: {
      g_session.ClearTradeItems();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 15;
      const unsigned char *record = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, record += 15) {
        unsigned char entry[15];
        memcpy(entry, record, sizeof(entry));
        RawItemBasic item = {};
        _itoa(*reinterpret_cast<const int *>(entry), item.itemId, 10);
        item.unknown_0004 = *reinterpret_cast<const std::uint32_t *>(entry + 5);
        item.unknown_0008 = *reinterpret_cast<const std::uint32_t *>(entry + 5);
        item.unknown_000C = entry[4];
        item.id = -1;
        item.identified = 1;
        g_session.AddItem1718(item, *reinterpret_cast<const int *>(entry + 9),
                              static_cast<std::uint16_t>(*reinterpret_cast<const std::uint16_t *>(entry + 0xD) / 10));
      }
      UIWindow *window = g_windowMgr.sub_A978D0(254);
      window->SendMsg(nullptr, 36, 0, 0, 0, 0);
      window->SendMsg(nullptr, 23, 0, 0, 0, 0);
      window = g_windowMgr.sub_A978D0(255);
      window->SendMsg(nullptr, 36, 0, 0, 0, 0);
      window->SendMsg(nullptr, 23, 0, 0, 0, 0);
      break;
    }
    default:
      GetRagConnection().ResetQueue();
      break;
    }
  }
}
