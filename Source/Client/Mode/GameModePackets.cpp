#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/CombatTypes.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Items/ItemType.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillUnitId.h"
#include "Network/Packets/NoticeEnums.h"
#include "Network/Packets/PacketId.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Chat/UINewChatWnd.h"
#include "Resource/World/TerrainResource.h"
#include "Client/Application/Launcher.h"
#include "File/ShortcutFilename.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/EquipWindowLook.h"
#include "UI/Macro/UIMacroDetectorWnd.h"
#include "UI/Macro/UIMacroBlackListCheckWnd.h"
#include "UI/Items/UILapineScrollBar.h"
#include "UI/Macro/UICaptchaPreviewWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeFadeOut.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/MousePointer.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Skills/MonsterSkillEffects.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Rodex/RodexMailStore.h"
#include "Contents/Events/RouletteMgr.h"
#include "Contents/World/World.h"
#include "Contents/Camera/Camera.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/RenderSystemBase.h"
#include "Render/Renderer.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/ResourceMgr.h"
#include "Network/TextEncoding.h"
#include "Contents/Events/SpecialPopupMgr.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Items/UIItemStoreWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "UI/Gauges/UIBarGraph.h"
#include "UI/Text/UIBroadcastBalloon.h"
#include "UI/Status/UIHomunInfoWnd.h"
#include "Contents/Sound/SoundMgr.h"
#include "UI/Status/UIStatusWnd.h"
#include "UI/Status/UIBasicInfoWnd.h"
#include "UI/Shop/UIMerchantItemMyShopWnd.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Shop/UIMerchantItemShopWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Items/UIRefiningWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "Utility/Math/Vector3d.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Items/EnchantMgr.h"
#include "Contents/Items/ItemReformMgr.h"
#include "Contents/Repute/ReputeMgr.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Items/BarterMarketItemMgr.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/EmotionSchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "UI/Navigation/NavigationLink.h"
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
#include "Contents/Monster/ChampionMonsterMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/GraffiSkill.h"
#include "UI/Party/UIRequestJoinPartyMgr.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "Contents/Party/PartyJoinResult.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "UI/Buttons/UIBalloonOpenBtn.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Client/Application/ErrorDialog.h"
#include "UI/Items/UIItemCollectionWnd.h"
#include "Contents/Map/MapPropertyMgr.h"
#include <ctime>
#include "UI/Chat/UIChatRoomWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "Contents/Clan/ClanInfo.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Utility/Text/LanguageCharset.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Mode/CLoginMode.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Items/UIParaItemShopWnd.h"
#include "Script/LuaScript.h"
#include "Contents/Actors/Item.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ClientItemLists.h"
#include "Contents/Sound/SoundMaker.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "UI/Status/UIPetInfoWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "Network/Packets/ActorNamePacket.h"
#include "UI/Guild/UIGuild_Storage_Log.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <new>

template __declspec(noinline) decltype(auto) std::list<BroadcastBalloonLine>::emplace_back<std::string&, const int&>(std::string&, const int&);

// OFFSET: 0x00D7AF70
// FUNCTION: fn_22198
__declspec(noinline) void CGameMode::ReceiveItemDisappear(std::uint32_t itemAid) {
  if (this->scene->RemoveItem(itemAid) == true) {
    this->SendMsg(MM_DELETEREGISTERPOS, static_cast<int>(itemAid), 0, 0, 0);
  }
}

#pragma pack(push, 1)
struct TradeItemRecord {
  /* 0000 */ int itemId;
  /* 0004 */ unsigned char type;
  /* 0005 */ int price;
  /* 0009 */ int quantity;
  /* 000D */ std::uint16_t weight;
  /* 000F */ unsigned char refine;
  /* 0010 */ std::uint32_t cards[4];
  /* 0020 */ std::uint16_t index;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TradeItemRecord) == 0x22);
#endif

// GLOBAL: gl_24992
char g_pendingWhisperCount;

namespace {
#pragma pack(push, 1)
  struct SimulationPacketHeader {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t length;
    /* 0004 */ std::uint32_t padding_0004;
    /* 0008 */ int value;
    /* 000C */ unsigned char name[0x18];
  };

  struct EnchantSimulationRecord {
    /* 0000 */ int itemId;
    /* 0004 */ int resultItemId;
    /* 0008 */ std::uint32_t padding_0008[2];
    /* 0010 */ int rate;
    /* 0014 */ std::uint32_t padding_0014;
    /* 0018 */ int count;
  };

  struct PackageSimulationRecord {
    /* 0000 */ int itemId;
    /* 0004 */ unsigned char refine;
    /* 0005 */ unsigned char packageType;
    /* 0006 */ int itemRefine;
    /* 000A */ int reportValue;
    /* 000E */ char text[24];
    /* 0026 */ int reportCount;
    /* 002A */ int reportTotal;
    /* 002E */ int count;
  };

  struct NamedProgressPacket {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ unsigned char result;
    /* 0003 */ int first;
    /* 0007 */ int second;
    /* 000B */ std::uint16_t current;
    /* 000D */ std::uint16_t total;
    /* 000F */ unsigned char padding_000F[4];
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
    /* 000C */ unsigned char damaged;
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
    /* 000C */ unsigned char damaged;
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
    /* 0004 */ std::uint16_t skillId;
    /* 0006 */ ChatTextEntry entries[1];
  };

  struct ActorValuePacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint32_t accountId;
    /* 0006 */ short value;
  };

  struct ActorSkillMessage {
    /* 0000 */ int skillId;
    /* 0004 */ int damage;
    /* 0008 */ int actionType;
    /* 000C */ int targetId;
    /* 0010 */ int tick;
    /* 0014 */ int skillLevel;
    /* 0018 */ int hitCount;
    /* 001C */ int cellX;
    /* 0020 */ int cellY;
    /* 0024 */ int attackMotion;
    /* 0028 */ int damageMotion;
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
    /* 0000 */ std::uint32_t price;
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
    /* 0005 */ std::uint32_t price;
    /* 0009 */ std::int32_t quantity;
    /* 000D */ std::uint16_t weight;
    /* 000F */ std::uint32_t itemFlags;
  };

  struct Item1748Record {
    /* 0000 */ std::uint32_t price;
    /* 0004 */ std::int16_t amount;
    /* 0006 */ std::int16_t index;
    /* 0008 */ unsigned char type;
    /* 0009 */ int itemId;
    /* 000D */ unsigned char identified;
    /* 000E */ unsigned char damaged;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ StoreItemOption options[5];
    /* 0039 */ std::int32_t location;
    /* 003D */ std::uint16_t sprite;
  };
#pragma pack(pop)

  struct StoreEquipEntry {
    std::int16_t index;
    int itemId;
    unsigned char type;
    std::uint32_t location;
    std::uint32_t wearState;
    unsigned char refine;
    std::uint32_t cards[4];
    std::uint32_t expireTime;
    std::uint16_t bindOnEquip;
    std::uint16_t sprite;
    bool identified;
    bool damaged;
    bool placeEtcTab;
    StoreItemOption options[5];
    unsigned char grade;
  };

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
      if (0 != options[index].id) {
        ++count;
      }
    }
    return count;
  }

  // INLINED
  __forceinline void ReadStoreEquipEntry(StoreEquipEntry& entry, const StoreEquipRecord& record) {
    entry.index = record.index;
    entry.itemId = record.itemId;
    entry.type = record.type;
    entry.location = record.location;
    entry.wearState = record.wearState;
    entry.refine = record.refine;
    std::memcpy(entry.cards, record.cards, sizeof(entry.cards));
    entry.expireTime = record.expireTime;
    entry.bindOnEquip = record.bindOnEquip;
    entry.sprite = record.sprite;
    entry.identified = record.flags & 1;
    entry.damaged = (record.flags >> 1) & 1;
    entry.placeEtcTab = (record.flags >> 2) & 1;
  }

  // INLINED
  __forceinline void ReadStoreEquipEntry(StoreEquipEntry& entry, const StoreEquipOptionRecord& record) {
    entry.index = record.index;
    entry.itemId = record.itemId;
    entry.type = record.type;
    entry.location = record.location;
    entry.wearState = record.wearState;
    entry.refine = record.refine;
    std::memcpy(entry.cards, record.cards, sizeof(entry.cards));
    entry.expireTime = record.expireTime;
    entry.bindOnEquip = record.bindOnEquip;
    entry.sprite = record.sprite;
    entry.identified = record.flags & 1;
    entry.damaged = (record.flags >> 1) & 1;
    entry.placeEtcTab = (record.flags >> 2) & 1;
    std::memcpy(entry.options, record.options, sizeof(entry.options));
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
void CGameMode::ReceiveBossMonsterInfo(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const NamedProgressPacket*>(data);
  char message[256];
  char name[51] = {};
  std::sprintf(name, "%s", packet->name);
  this->bossMonsterX = -1;
  this->bossMonsterY = -1;

  switch (packet->result) {
  case BOSS_INFO_NOT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTFIND_BOSSMON)), 0xFFFFFF, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTFIND_BOSSMON)), 0xFFFFFF, 0, 0, 0);
    return;
  case BOSS_INFO_ALIVE:
  case BOSS_INFO_ALIVE_WITHMSG:
    this->bossMonsterX = packet->first;
    this->bossMonsterY = packet->second;
    if (packet->result != 2) {
      return;
    }
    std::sprintf(message, LookupMessage(MSI_SHOW_POSITION_OF_BOSSMON), name);
    break;
  case BOSS_INFO_DEAD:
    if (packet->current == 0 && packet->total == 1) {
      std::sprintf(message, LookupMessage(MSI_APPEARANCE_TIME_OF_BOSSMON2), name);
    } else {
      std::sprintf(message, LookupMessage(MSI_APPEARANCE_TIME_OF_BOSSMON), name, packet->current, packet->total);
    }
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
}

// OFFSET: 0x00D230E0
// FUNCTION: fn_21660
__declspec(noinline) void CGameMode::ReceiveColoredNotice(const unsigned char* packet) {
  std::string text(reinterpret_cast<const char*>(packet + 9), *reinterpret_cast<const short*>(packet + 2) - 9);
  if (!text.empty()) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), *reinterpret_cast<const std::uint32_t*>(packet + 5), packet[4], 0, 0);
  }
}

// OFFSET: 0x00D1D690
// FUNCTION: fn_21630
void CGameMode::ReceiveNamedActionResult(const unsigned char* packet) {
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  const std::string name = reinterpret_cast<const char*>(packet + 2);

  switch (*reinterpret_cast<const int*>(packet + 0x1A)) {
  case PARTY_REPLY_JOIN_OTHER_PARTY:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_ALREADY_IN_PARTY)), 0xFF, 0, 0, 0);
    break;
  case PARTY_REPLY_REJECTED:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_CHARACTER_REJECT_PARTY_INVITATION)).c_str()), 0xFF, 0, 0, 0);
    break;
  case PARTY_REPLY_ACCEPTED:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>((name + LookupMessage(MSI_CHARACTER_ACCEPT_PARTY_INVITATION)).c_str()), 0xFFFF00, 0, 0, 0);
    break;
  case PARTY_REPLY_FULL:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_MANY_PEOPLE_IN_PARTY)), 0xFF, 0, 0, 0);
    break;
  case PARTY_REPLY_DUAL:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ALREADY_SAME_AID_JOINED)), 0xFF, 0, 0, 0);
    break;
  }

  switch (*reinterpret_cast<const int*>(packet + 0x1A)) {
  case PARTY_REPLY_JOINMSG_REFUSE:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_JOINMSG_REFUSE)), 0xFF, 0, 0, 0);
    break;
  case PARTY_REPLY_OFFLINE:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_NOT_EXIST)), 0xFF, 0, 0, 0);
    break;
  case PARTY_REPLY_INVALID_MAPPROPERTY: {
    char text[256];
    std::sprintf(text, LookupMessage(MSI_NOPARTY2), reinterpret_cast<const char*>(packet + 2));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case PARTY_REPLY_INVALID_MAPPROPERTY_ME:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PREVENT_PARTY_JOIN)), 0xFF, 0, 0, 0);
    break;
  case PARTY_REPLY_MEMORIALDUNGEON:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MUST_PASSWORD_EXCEED_CHAR)), 0xFF, 0, 0, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MERCHANTSHOP_FAIL_POSITION)), 0xFF, 0, 0, 0);
    break;
  case 18:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MCSTORE_BACKUP)), 0xFF, 0, 0, 0);
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
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  std::sprintf(
    text, "standard STR=%3d      AGI=%3d      VIT=%3d      INT=%3d      DEX=%3d      LUK=%2d", packet[3], packet[5], packet[7], packet[9], packet[11], packet[13]
  );
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF37, 0, 0, 0);
  std::sprintf(
    text, "    attPower=%3d    refiningPow=%3d    MAXmatPow=%3d    MINmatPower=%3d       ASPD=%3d", values[7], values[8], values[9], values[10], values[19]
  );
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  std::sprintf(
    text, "itemdefPow=%3d     plusdefPow=%3d    mdefPower=%3d    plusmdefPow=%3d    plusASPD=%3d", values[11], values[12], values[13], values[14], values[20]
  );
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF37, 0, 0, 0);
  std::sprintf(text, "hitSuccessVal=%3d    avoidSuccessVal=%3d    plusAvoidSuccessValue=%3d", values[15], values[16], values[17]);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
}

// OFFSET: 0x00D24430
// FUNCTION: fn_21666
__declspec(noinline) void CGameMode::ReceiveActorText(const unsigned char* packet) {
  char text[81];
  std::strncpy(text, reinterpret_cast<const char*>(packet + 6), 80);
  text[80] = '\0';
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(text);
    actor->SendMsg(nullptr, AM_MAKE_BUYING_SHOP_TITLE, value, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D25920
// FUNCTION: fn_21674
__declspec(noinline) void CGameMode::ReceiveRentalItemTime(const unsigned char* packet) {
  char number[12];
  char text[260];
  _itoa(*reinterpret_cast<const int*>(packet + 2), number, 10);
  const ITEM_DESC& description = FindItemDescription(std::atoi(number));
  const std::uint32_t seconds = *reinterpret_cast<const std::uint32_t*>(packet + 6);
  const char* name = description.identifiedName;
  const int minutes = static_cast<int>(seconds / 60);
  if (minutes > 0) {
    std::sprintf(text, LookupMessage(MSI_NOTICE_TO_REMAIN_ITEMUSINGTIME), name, minutes);
  } else {
    const char* format = LookupMessage(MSI_NOTICE_TO_REMAIN_ITEMUSINGTIME2);
    std::sprintf(text, format, name, seconds);
  }
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  if (minutes == 1) {
    std::sprintf(text, LookupMessage(MSI_LAST_NOTICE_TO_REMAIN_ITEMUSINGTIME), name);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D264E0
// FUNCTION: fn_21679
void CGameMode::ReceiveNpcDialogEnd(const unsigned char* packet) {
  if (UIWindow* window = g_windowMgr.sayDialogWnd) {
    window->SendMsg(nullptr, UM_DISABLENEXTBUTTON, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_ENABLECLOSEBUTTON, 0, 0, 0, 0);
  } else {
    this->isOnQuest = 0;
    this->lastNaid = *reinterpret_cast<const int*>(packet + 2);
    g_windowMgr.CloseWindow(WID_SAYDIALOGWND);
    g_windowMgr.CloseWindow(WID_CHOOSEWND);
    g_windowMgr.CloseWindow(WID_SAYDIALOGWND_OVERLAY);
  }

  if (UIWindow* window = g_windowMgr.sayDialogOverlayWnd) {
    window->SendMsg(nullptr, UM_DISABLENEXTBUTTON, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_ENABLECLOSEBUTTON, 0, 0, 0, 0);
  } else {
    this->isOnQuest = 0;
    this->lastNaid = *reinterpret_cast<const int*>(packet + 2);
    g_windowMgr.CloseWindow(WID_SAYDIALOGWND_OVERLAY);
    g_windowMgr.CloseWindow(WID_CHOOSEWND);
    g_windowMgr.CloseWindow(WID_SAYDIALOGWND_OVERLAY);
  }
}

// OFFSET: 0x00D26600
// FUNCTION: fn_21680
__declspec(noinline) void CGameMode::ReceiveActorSequenceDeadline(const unsigned char* packet) {
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
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 232, 0, 0, 0, 0, 0);
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
          effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
        }
      }
    } else {
      char message[256];
      sprintf(message, "DEVOTION OVERFLOW : %d -- %d , %d , %d , %d , %d", list->aid, list->devotees[0], list->devotees[1], list->devotees[2],
        list->devotees[3], list->devotees[4]);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x3232FA, 0, 0, 0);
      sprintf(message, "DEVOTION OVERFLOW : 그라비티에 메일로 문의해주십시요");
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0x3232FA, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D27060
// FUNCTION: fn_21686
__declspec(noinline) void CGameMode::ReceiveActorSignal(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    actor->SendMsg(nullptr, AM_DELETE_BUYING_SHOP_TITLE, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D2A990
// FUNCTION: fn_21705
__declspec(noinline) void CGameMode::ReceiveFormatStringMessage(const unsigned char* packet) {
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

  switch (count) {
  case 0:
    _snprintf(text, 260, format);
    break;
  case 1:
    _snprintf(text, 260, format, reinterpret_cast<const char*>(message + 1));
    break;
  case 2: {
    const char* first = reinterpret_cast<const char*>(message + 1);
    const char* second = first + std::strlen(first) + 1;
    _snprintf(text, 260, format, first, second);
    break;
  }
  case 3: {
    const char* first = reinterpret_cast<const char*>(message + 1);
    const char* second = first + std::strlen(first) + 1;
    const char* third = second + std::strlen(second) + 1;
    _snprintf(text, 260, format, first, second, third);
    break;
  }
  default:
    sprintf(text, "unknown argment list");
    return;
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFFFF, 0, 0, 0);
}

// OFFSET: 0x00D2AF90
// FUNCTION: fn_21707
void CGameMode::IgnorePacket(const unsigned char*) {
}

// OFFSET: 0x00D2B710
// FUNCTION: fn_21710
void CGameMode::ReceiveItem1738EquipList(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreEquipRecord);
  const StoreEquipRecord* records = reinterpret_cast<const StoreEquipRecord*>(data + sizeof(ItemStoreListHeader));
  StoreItemOption options[5] = {0};
  for (int i = 0; i < count; ++i) {
    const StoreEquipRecord* record = &records[i];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    item.identity[0] = entry.type;
    item.identity[1] = entry.index;
    item.identity[2] = entry.location;
    item.identity[3] = entry.wearState;
    item.quantity = 1;
    std::memcpy(item.cards, entry.cards, sizeof(entry.cards));
    item.SetItemId(entry.itemId);
    item.identified = entry.identified;
    item.damaged = entry.damaged;
    item.refiningLevel = entry.refine;
    item.expireTime = entry.expireTime;
    item.bindOnEquip = entry.bindOnEquip;
    item.sprite = entry.sprite;
    item.flags[0] = entry.placeEtcTab;
    item.optionCount = CountStoreItemOptions(options);
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = options[index];
    }
    g_session.AddCartItem(item, 1);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2BA50
// FUNCTION: fn_21711
void CGameMode::ReceiveItem1738EquipListOptions(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreEquipOptionRecord);
  const StoreEquipOptionRecord* records = reinterpret_cast<const StoreEquipOptionRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i) {
    const StoreEquipOptionRecord* record = &records[i];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    item.identity[0] = entry.type;
    item.identity[1] = entry.index;
    item.identity[2] = entry.location;
    item.identity[3] = entry.wearState;
    item.quantity = 1;
    std::memcpy(item.cards, entry.cards, sizeof(entry.cards));
    item.SetItemId(entry.itemId);
    item.identified = entry.identified;
    item.damaged = entry.damaged;
    item.refiningLevel = entry.refine;
    item.expireTime = entry.expireTime;
    item.bindOnEquip = entry.bindOnEquip;
    item.sprite = entry.sprite;
    item.flags[0] = entry.placeEtcTab;
    item.optionCount = CountStoreItemOptions(entry.options);
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = entry.options[index];
    }
    g_session.AddCartItem(item, 1);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2BDF0
// FUNCTION: fn_21712
void CGameMode::ReceiveItem1738NormalList(const unsigned char* data) {
  const ItemStoreListHeader* packet = reinterpret_cast<const ItemStoreListHeader*>(data);
  UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
  if (packet->title[0] != '\0') {
    std::string title(packet->title);
    window->title->SetTextFitted(title.c_str());
  }

  const int count = (packet->length - sizeof(ItemStoreListHeader)) / sizeof(StoreNormalRecord);
  const StoreNormalRecord* records = reinterpret_cast<const StoreNormalRecord*>(data + sizeof(ItemStoreListHeader));
  for (int i = 0; i < count; ++i) {
    const StoreNormalRecord* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[3] = record->location;
    item.identity[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.damaged = 0;
    item.refiningLevel = 0;
    item.expireTime = record->expireTime;
    item.flags[0] = (record->flags >> 1) & 1;
    g_session.AddCartItem(item, 0);
  }

  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D2C190
// FUNCTION: fn_21713
__declspec(noinline) void CGameMode::ReceiveHomunculusProperty64(const unsigned char* data) {
  if (!data) {
    return;
  }

  const HomunculusProperty64Packet* packet = reinterpret_cast<const HomunculusProperty64Packet*>(data);
  switch (packet->type) {
  case SP_BASEEXP:
    g_session.m_homunExp = packet->value;
    g_session.RefreshHomunculusInfo();
    if (UIHomunInfoWnd* window = static_cast<UIHomunInfoWnd*>(g_windowMgr.homunInfoWnd)) {
      window->expBar->SetValues(g_session.m_homunExp, g_session.m_homunMaxExp);
    }
    break;
  case SP_HP:
    g_session.m_homunHp = static_cast<int>(packet->value);
    g_session.RefreshHomunculusInfo();
    break;
  case SP_SP:
    g_session.m_homunSp = static_cast<int>(packet->value);
    g_session.RefreshHomunculusInfo();
    break;
  }

  g_session.RefreshHomunculusInfo();
}

// OFFSET: 0x00D2C250
// FUNCTION: fn_21714
__declspec(noinline) void CGameMode::ReceiveHomunculusProperty(const unsigned char* data) {
  const HomunculusPropertyPacket* packet = reinterpret_cast<const HomunculusPropertyPacket*>(data);
  if (packet->type == 5) {
    g_session.m_homunHp = packet->value;
    g_session.RefreshHomunculusInfo();
  } else if (packet->type == 7) {
    g_session.m_homunSp = packet->value;
    g_session.RefreshHomunculusInfo();
  } else if (packet->type == 1) {
    g_session.m_homunExp = packet->value;
    g_session.RefreshHomunculusInfo();
  }

  g_session.RefreshHomunculusInfo();
}

// OFFSET: 0x00D2C2B0
// FUNCTION: fn_21715
__declspec(noinline) void CGameMode::ReceiveHuntingList(const unsigned char* packet) {
  char text[256];
  const unsigned char* data = packet + 4;
  int size = *reinterpret_cast<const short*>(packet + 2) - 4;
  if (size > 5000) {
    size = 5000;
  }
  char* list = new char[size + 1];
  std::memcpy(list, data, size);
  list[size] = '\0';
  std::sprintf(text, "=========== HUNTING LIST =============");
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(list), 0xCDFFCD, 0, 0, 0);
  std::sprintf(text, "========================================");
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
}

// OFFSET: 0x00D2F150
// FUNCTION: fn_21728
bool CGameMode::NotifyItemResult(int result) {
  int message;
  switch (result) {
  case PICKUP_OVERCOUNT_ONEITEM:
    message = 279;
    break;
  case PICKUP_OVERCOUNT:
    message = 220;
    break;
  case PICKUP_OVERWEIGHT:
    message = 52;
    break;
  case PICKUP_FAILED:
  case PICKUP_REFUSED_TIME:
    message = 53;
    break;
  case ADDITEM_STACKLIMIT:
    message = 1418;
    break;
  case 9:
    message = 3984;
    break;
  default:
    return false;
  }
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  const char* text = LookupMessage(message);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
  return true;
}

// OFFSET: 0x00D4B2C0
// FUNCTION: fn_21864
void CGameMode::ReceiveEquipAck(const unsigned char* packet) {
  switch (*reinterpret_cast<const short*>(packet + 2)) {
  case 0:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ERR_ATTACK_ARROW)), 0xFF, 0, 0, 0);
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ERR_ATTACK_WEIGHT)), 0xFFFF64, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ERR_SKILL_WEIGHT)), 0xFFFF64, 0, 0, 0);
    break;
  case 3:
    switch (g_session.GetDisplayJob()) {
    case JT_ASSASSIN:
    case JT_ASSASSIN_H:
    case JT_ASSASSIN_B:
    case JT_GUILLOTINE_CROSS:
    case JT_GUILLOTINE_CROSS_H:
    case JT_GUILLOTINE_CROSS_B:
    case JT_SHADOW_CROSS:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MSG_KNIFE_EQUIPMENT_SUCCESS)), 0xFFFF64, 0, 0, 0);
      break;
    case JT_GUNSLINGER:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MSG_BULLET_EQUIPMENT_SUCCESS)), 0xFFFF64, 0, 0, 0);
      break;
    case JT_NINJA:
    case JT_NINJA_B:
      break;
    default:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MSG_ARROW_EQUIPMENT_SUCCESS)), 0xFFFF64, 0, 0, 0);
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
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("Create Folder Fail!!! %s", path.c_str()).c_str()), 0xFF, 0, 0, 0);
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
    path = static_cast<const std::string&>(FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index));
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  const std::string message = FormatOwnedString("Simulator Result File Create %s %d!!!!!!!!!!!!", file ? "success" : "Fail", itemId);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), file ? 0xFFFF : 0xFF, 0, 0, 0);
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
    path = static_cast<const std::string&>(FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index));
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  const std::string message = FormatOwnedString("%s Simulator File Create %s !!!!!!!!!!!!", name.c_str(), file ? "success" : "Fail");
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()),
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
    item.identity[0] = 0x15;
    ITEM_INFO result;
    result.SetItemId(record->resultItemId);
    result.identified = 1;
    result.identity[0] = 0x15;
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
    item.identity[0] = 6;
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
      item.grade = package->refine;
      item.identified = 1;
      item.refiningLevel = package->itemRefine;
      if (package->refine != 0 || package->itemRefine != 0) {
        item.identity[0] = 0x15;
      }
      std::fprintf(file, "\t%-24s\t\t\t\t\t\t\t%d\t(%d)\t%d/%d\t%s\n", ToUtf8(item.DisplayName(0), g_textCodePage).c_str(), package->count,
                   package->reportValue, package->reportCount, package->reportTotal, ToUtf8(std::string(package->text), g_textCodePage).c_str());
      unpacked += package->count;
    }
    std::fprintf(file, "\n\t\t\t\t\tUnpacked\t%d\tPackages\n", unpacked);
  }
  std::fprintf(file, "============================\tEND\t============================\n");
  std::fclose(file);
}

// OFFSET: 0x00D612A0
// FUNCTION: fn_21979
__declspec(noinline) void CGameMode::ReceiveMileageInfo(const unsigned char* packet) {
  UIWindow* window = g_windowMgr.EnsureWindow(WID_MILEAGEWND);
  if (!window) {
    return;
  }

  window->SendMsg(
    nullptr, UM_SETINFO, packet[2] != 0, *reinterpret_cast<const int*>(packet + 4), *reinterpret_cast<const int*>(packet + 8), static_cast<signed char>(packet[3])
  );
}

// OFFSET: 0x00D77BF0
// FUNCTION: fn_22152
__declspec(noinline) void CGameMode::ReceiveMailboxAvailability(const unsigned char* packet) {
  if (packet[2] == 1) {
    if (!g_windowMgr.openMailBoxWnd) {
      g_windowMgr.EnsureWindow(WID_OPENMAILBOXWND);
    }

    if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 476, 1, 0);
    }
    return;
  }

  g_windowMgr.CloseWindow(WID_OPENMAILBOXWND);
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
  case SP_HP: {
    g_session.UpdateRandomStats();
    const int hp = g_session.hp + amount;
    if (g_session.hp != hp) {
      g_session.hp = hp;
      g_session.RefreshHpSpWindows();
    }
    player->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFF00FF00u, 114, 0, 0, 0);
    player->LaunchEffect(331, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  case SP_SP: {
    g_session.UpdateRandomStats();
    int& sp = *reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0]);
    const int value = sp + amount;
    if (sp != value) {
      sp = value;
      g_session.RefreshHpSpWindows();
    }
    player->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFF0000FFu, 114, 0, 0, 0);
    player->LaunchEffect(332, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  }
}

// GLOBAL: gl_24965
int g_overweightThresholdPercent;

// OFFSET: 0x00D50580
// FUNCTION: fn_21884
void CGameMode::IgnoreRemovedPacket7(const unsigned char*) {
}

// OFFSET: 0x00D79420
// FUNCTION: fn_22167
__declspec(noinline) void CGameMode::ReceiveOverweightThreshold(const unsigned char* packet) {
  if (*reinterpret_cast<const int*>(packet + 2) > 0) {
    g_overweightThresholdPercent = *reinterpret_cast<const int*>(packet + 2);
  }
}

// OFFSET: 0x00D7A0B0
// FUNCTION: fn_22186
__declspec(noinline) void CGameMode::ReceiveNavigationRouteResult(const unsigned char* packet) {
  const signed char value = *reinterpret_cast<const signed char*>(packet + 2);
  if (value != 1 && value != 2) {
    g_navigationManager.routeSide = 0;
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
__declspec(noinline) void CGameMode::ReceiveCharViewResult(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
  case 0: {
    CMode* mode = g_modeMgr.gameMode;
    CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    if (player->render.state != 0 && mode != nullptr) {
      mode->SendMsg(MM_REQUEST_ACT, 3, player->proceedTargetGid, 0, 0);
    }
    return;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LAST_LOGIN)), 0xFF, 0, 0, 0);
    return;
  default:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PARA_PURCHASE_ITEM_FAIL2)), 0xFF, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x00D89CA0
// FUNCTION: fn_22213
__declspec(noinline) void CGameMode::ResetItemLists(const unsigned char*) {
  g_session.ResetSwapCostumeItems();
  g_session.ResetSwapEquippedItems();
}

// OFFSET: 0x00D8A1F0
// FUNCTION: fn_22216
__declspec(noinline) void CGameMode::ClearItemLists(const unsigned char* packet) {
  g_session.UnflagInventoryItem(*reinterpret_cast<const std::uint16_t*>(packet + 2));
  this->inventoryChanged = 1;
  RefreshItemWindows(&g_windowMgr);
  g_session.UnflagMerchantShopItem(*reinterpret_cast<const std::uint16_t*>(packet + 2));
  g_session.RefreshItem1740Window();
  g_session.UnflagCartItem(*reinterpret_cast<const std::uint16_t*>(packet + 2));
  g_session.RefreshItem1738Window();
}

// OFFSET: 0x00D31E50
// FUNCTION: fn_21736
__declspec(noinline) void CGameMode::ReceiveMemorialDungeonInfo(const unsigned char* packet) {
  g_session.memorialDungeonName = reinterpret_cast<const char*>(packet + 2);
  g_session.memorialCreationTime = *reinterpret_cast<const int*>(packet + 63);
  g_session.memorialExpirationTime = *reinterpret_cast<const int*>(packet + 67);
  const int state = *reinterpret_cast<const int*>(packet + 67) != 0 ? 1 : 2;
  const int previous = g_session.memorialDungeonState;
  g_session.memorialDungeonState = state;
  if (previous != state || state != 2 || g_session.memorialWindowOpen != 0) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_MEMORIALDUNWND)) {
      window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D31EE0
// FUNCTION: fn_21737
__declspec(noinline) void CGameMode::ReceiveMemorialDungeonNotify(const unsigned char* packet) {
  const int previous = g_session.memorialDungeonState;
  g_session.memorialExpirationTime = *reinterpret_cast<const int*>(packet + 6);
  const int time = *reinterpret_cast<const int*>(packet + 6);
  g_session.memorialDungeonState = 0 != time ? 1 : 2;
  if (*reinterpret_cast<const int*>(packet + 2) == 0) {
    if (previous != (time != 0 ? 1 : 2) || time != 0 || g_session.memorialWindowOpen != 0) {
      if (UIWindow* window = g_windowMgr.EnsureWindow(WID_MEMORIALDUNWND)) {
        window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      }
    }
    return;
  }

  g_windowMgr.CloseWindow(WID_MEMORIALDUNWND);
  const int type = *reinterpret_cast<const int*>(packet + 2);
  if (type == 1) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MEMORIAL_DUN_LIVE_TIME_OUT)), 0x64FFFF, 0, 0, 0);
  } else if (type == 2) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MEMORIAL_DUN_ENTER_TIME_OUT)), 0x64FFFF, 0, 0, 0);
  } else if (type == 3) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MEMORIAL_DUN_DESTROY_REQUEST)), 0x64FFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D31FD0
// FUNCTION: fn_21738
__declspec(noinline) void CGameMode::ReceiveMemorialDungeonSubscriptionInfo(const unsigned char* packet) {
  g_session.memorialDungeonName = reinterpret_cast<const char*>(packet + 2);
  g_session.memorialQueuePosition = *reinterpret_cast<const short*>(packet + 63);
  g_session.memorialDungeonState = 0;
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_MEMORIALDUNWND)) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D32040
// FUNCTION: fn_21739
__declspec(noinline) void CGameMode::ReceiveMemorialDungeonSubscriptionNotify(const unsigned char* packet) {
  g_session.memorialQueuePosition = *reinterpret_cast<const short*>(packet + 2);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_MEMORIALDUNWND);
  if (!window) {
    return;
  }

  if (*reinterpret_cast<const short*>(packet + 2) > -1) {
    g_session.memorialDungeonState = 0;
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.CloseWindow(WID_MEMORIALDUNWND);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MEMORIAL_DUN_CANCEL)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D320D0
// FUNCTION: fn_21740
void CGameMode::ReceiveWindow17Entries(const unsigned char* packet) {
  this->isOnQuest = 1;
  g_windowMgr.CloseWindow(WID_CHOOSEWND);
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_CHOOSEWND)) {
    const int length = *reinterpret_cast<const unsigned short*>(packet + 2) - 9;
    std::string text(reinterpret_cast<const char*>(packet + 8), length);
    const std::vector<std::string> entries = Split(text, ":");
    for (const std::string& entry : entries) {
      if (!entry.empty()) {
        window->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>(entry.c_str()), 0, 0, 0);
      }
    }
    window->SendMsg(nullptr, UM_SETNAID, *reinterpret_cast<const int*>(packet + 4), 0, 0, 0);
  }

  if (g_windowMgr.sayDialogWnd) {
    g_windowMgr.sayDialogWnd->SendMsg(nullptr, UM_DISABLENEXTBUTTON, 0, 0, 0, 0);
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
    std::sprintf(buffer, LookupMessage(MSI_NOTIFY_EXP), amount);
    text += buffer;
  } else if (data->varId == 2) {
    std::sprintf(buffer, LookupMessage(MSI_NOTIFY_JOBEXP), amount);
    text += buffer;
  }

  if (data->amount >= 0) {
    text += LookupMessage(MSI_GET);
  } else {
    text += LookupMessage(MSI_LOSS);
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), color, 18, 0, 0);
}

// OFFSET: 0x00D61290
// FUNCTION: fn_21978
void CGameMode::IgnorePacket478(const unsigned char*) {
}

// OFFSET: 0x00D3A1D0
// FUNCTION: fn_21775
void CGameMode::ClearMovementRestriction(const unsigned char*) {
  this->noMove = 0;
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
  this->ChangeZoneServer(std::string(change->mapName, change->mapName + sizeof(change->mapName)).c_str(), change->x, change->y, static_cast<int>(change->ip), change->port, std::string(""));
}

// OFFSET: 0x00D79F00
// FUNCTION: fn_22184
void CGameMode::ReceiveZoneServerChange(const char* mapName, std::int16_t x, std::int16_t y, int ip, std::int16_t port) {
  g_session.m_localMapMove = 1;
  this->ChangeZoneServer(mapName, x, y, ip, port, std::string());
}

// OFFSET: 0x00D272A0
// FUNCTION: fn_21688
__declspec(noinline) void CGameMode::ReceiveActorStatusBlock(const unsigned char* packet) {
  g_session.SetElementalGid(*reinterpret_cast<const int*>(packet + 2));
  g_session.elementalHp = *reinterpret_cast<const int*>(packet + 6);
  g_session.elementalSp = *reinterpret_cast<const int*>(packet + 0xE);
  g_session.elementalMaxHp = *reinterpret_cast<const int*>(packet + 0xA);
  g_session.elementalMaxSp = *reinterpret_cast<const int*>(packet + 0x12);
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    actor->SendMsg(nullptr, AM_UPDATEGAGE, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D27320
// FUNCTION: fn_21689
__declspec(noinline) void CGameMode::ReceiveActorStatusUpdate(const unsigned char* packet) {
  if (g_session.elementalGid != -1) {
    const unsigned int field = *reinterpret_cast<const std::uint16_t*>(packet + 2) - 5;
    switch (field) {
    case 0:
      g_session.elementalHp = *reinterpret_cast<const int*>(packet + 4);
      break;
    case 2:
      g_session.elementalSp = *reinterpret_cast<const int*>(packet + 4);
      break;
    case 1:
      g_session.elementalMaxHp = *reinterpret_cast<const int*>(packet + 4);
      break;
    case 3:
      g_session.elementalMaxSp = *reinterpret_cast<const int*>(packet + 4);
      break;
    }
    if (CRenderObject* actor = this->scene->GetGameActorByAID(g_session.elementalGid)) {
      actor->SendMsg(nullptr, AM_UPDATEGAGE, 0, 0, 0, 0, 0, 0);
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
    actor->SendMsg(nullptr, AM_MAKE_SHOP_TITLE, value, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D61340
// FUNCTION: fn_21981
__declspec(noinline) void CGameMode::ReceiveActorInt8(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t value = *reinterpret_cast<const signed char*>(packet + 6);
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, value, 0x14, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D612F0
// FUNCTION: fn_21980
__declspec(noinline) void CGameMode::ReceiveActorInt64Pair(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t first = *reinterpret_cast<const int*>(packet + 6);
    const std::int64_t second = *reinterpret_cast<const int*>(packet + 0xA);
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, first, second, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D24C40
// FUNCTION: fn_21670
__declspec(noinline) void CGameMode::ReceiveItem1740EquipList(const unsigned char* packet) {
  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(StoreEquipRecord);
  const StoreEquipRecord* records = reinterpret_cast<const StoreEquipRecord*>(packet + 4);
  StoreItemOption options[5] = {0};
  for (int i = 0; i < count; ++i) {
    const StoreEquipRecord* record = &records[i];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    item.identity[0] = entry.type;
    item.identity[1] = entry.index;
    item.identity[2] = entry.location;
    item.identity[3] = entry.wearState;
    item.quantity = 1;
    std::memcpy(item.cards, entry.cards, sizeof(entry.cards));
    item.SetItemId(entry.itemId);
    item.identified = entry.identified;
    item.damaged = entry.damaged;
    item.refiningLevel = entry.refine;
    item.expireTime = entry.expireTime;
    item.bindOnEquip = entry.bindOnEquip;
    item.sprite = entry.sprite;
    item.flags[0] = entry.placeEtcTab;
    item.optionCount = CountStoreItemOptions(options);
    item.grade = 0;
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = options[index];
    }
    g_session.AddMerchantShopItem(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D24EF0
// FUNCTION: fn_21671
void CGameMode::ReceiveItem1740EquipListOptions(const unsigned char* packet) {
  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(StoreEquipOptionRecord);
  const StoreEquipOptionRecord* records = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 4);
  for (int i = 0; i < count; ++i) {
    const StoreEquipOptionRecord* record = &records[i];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    item.identity[0] = entry.type;
    item.identity[1] = entry.index;
    item.identity[2] = entry.location;
    item.identity[3] = entry.wearState;
    item.quantity = 1;
    std::memcpy(item.cards, entry.cards, sizeof(entry.cards));
    item.SetItemId(entry.itemId);
    item.identified = entry.identified;
    item.damaged = entry.damaged;
    item.refiningLevel = entry.refine;
    item.expireTime = entry.expireTime;
    item.bindOnEquip = entry.bindOnEquip;
    item.sprite = entry.sprite;
    item.flags[0] = entry.placeEtcTab;
    item.grade = 0;
    item.optionCount = CountStoreItemOptions(entry.options);
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = entry.options[index];
    }
    g_session.AddMerchantShopItem(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D26800
// FUNCTION: fn_21682
__declspec(noinline) void CGameMode::ReceiveEquipmentViewState(const unsigned char* packet) {
  const unsigned char value = packet[2];
  *reinterpret_cast<int*>(g_session.equipmentViewState) = value;
  const char* text = LookupMessage(value != 0 ? 0x54F : 0x54E);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
  if (g_windowMgr.equipWnd) {
    g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D79660
// FUNCTION: fn_22172
__declspec(noinline) void CGameMode::ReceiveDebuffRemovalResult(const unsigned char* packet) {
  UIWindow* window = g_windowMgr.LookupWindow(WID_DEBUFFREMOVEWND);
  if (!window) {
    return;
  }

  switch (*reinterpret_cast<const std::int16_t*>(packet + 6) - 1) {
  case 0: {
    window->Show(0);
    const char* text = LookupMessage(0x10E8);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    return;
  }
  case 1:
    g_windowMgr.CloseWindow(WID_DEBUFFREMOVEWND);
    return;
  case 2: {
    const char* text = LookupMessage(MSI_CASH_FAILED_BUSY);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    return;
  }
  case 3:
    window->Show(1);
    return;
  }
}

// OFFSET: 0x00D79E90
// FUNCTION: fn_22183
__declspec(noinline) void CGameMode::ReceiveAccountLimitedToolApplyResult(std::int16_t result, std::intptr_t value) {
  switch (result) {
  case 0:
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_ACCOUNTLIMITEDTOOLWND)) {
      window->SendMsg(nullptr, UM_PARAM, 0, value, 0, 0);
    }
    break;
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BARGAINSALE_TOOLWND_APPLY_FAILED_MSG)), 0xFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D79440
// FUNCTION: fn_22168
__declspec(noinline) void CGameMode::ReceiveRouletteSetResult(const unsigned char* packet) {
  switch (packet[2]) {
  case GENERATE_ROULETTE_SUCCESS:
    g_rouletteMgr->SetAdditionItem(*reinterpret_cast<const int*>(packet + 3));
    g_rouletteMgr->SetResult(0, 0, 0);
    g_rouletteMgr->Reset();
    if (g_windowMgr.roulletteWnd) {
      g_windowMgr.RefreshRouletteWnd();
    }
    break;
  case GENERATE_ROULETTE_FAILED:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MERCHANTSHOP_MAKING_FAIL)), 0x6464FF, 0, 0, 0);
    break;
  case GENERATE_ROULETTE_NO_ENOUGH_POINT:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_CLOSE_ROULLETTEWND)), 0x6464FF, 0, 0, 0);
    break;
  case GENERATE_ROULETTE_LOSING:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ROULLETTE_WND)), 0x6464FF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D611B0
// FUNCTION: fn_21977
__declspec(noinline) void CGameMode::ReceiveDynamicNpcNotice(const unsigned char* packet) {
  char text[0x400];
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case DYNAMICNPC_RESULT_DUPLICATE:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_GENERATE_ROULLETTE)), 0x64FFFF, 0, 0, 0);
    break;
  case DYNAMICNPC_RESULT_UNKNOWNNPC:
    std::memset(text, 0, sizeof(text));
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "dynamic npc not found ");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
    break;
  case DYNAMICNPC_RESULT_UNKNOWN:
    std::memset(text, 0, sizeof(text));
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "dynamic npc create error found(unknown) ");
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
    break;
  case DYNAMICNPC_RESULT_OUTOFTIME:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAIL_STRIPSHADOW_TO_MADO_NOTICE)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D079A0
// FUNCTION: fn_21584
void CGameMode::ReceiveWhisperResult(unsigned char kind, int argument) {
  switch (kind) {
  case ACKWHISPER_SUCCESS: {
    std::string line(g_session.DecodePlayerName());
    line.append(" : ", 3);
    line.append(this->lastWhisper.data(), this->lastWhisper.size());
    if (g_windowMgr.ShowWhisperFrom(this->lastWhisperName.c_str(), line.c_str(), 0x7800, static_cast<std::uint32_t>(argument)) != 0) {
      return;
    }

    const std::string copied = this->lastWhisperName;
    std::string message;
    if (g_windowMgr.IsFriend(this->lastWhisperName.c_str()) || g_session.IsGuildMemberName(this->lastWhisperName.c_str())) {
      message = "[ To " + copied + " (";
      message += ShortcutFilename(static_cast<std::uint32_t>(argument));
      message += ") ] : " + this->lastWhisper;
    } else {
      message = "( To " + copied + " (";
      message += ShortcutFilename(static_cast<std::uint32_t>(argument));
      message += ") : " + this->lastWhisper;
    }

    if (g_pendingWhisperCount > 0) {
      --g_pendingWhisperCount;
    } else {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF, 2, reinterpret_cast<std::intptr_t>(copied.c_str()), 0);
    }
    if (g_session.m_whisperRecord != 0) {
      this->WriteChatLog(message.c_str());
    }
    return;
  }
  case ACKWHISPER_TARGET_OFFLINE: {
    const std::string text = "(" + this->lastWhisperName + ")" + LookupMessage(MSI_NOT_EXIST_CHARACTER);
    if (g_pendingWhisperCount > 0) {
      --g_pendingWhisperCount;
      return;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 2, 0, 0);
    return;
  }
  case ACKWHISPER_IGNORED: {
    const std::string text = "(" + this->lastWhisperName + ")" + LookupMessage(MSI_NO_RECEIVE_MODE);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 2, 0, 0);
    return;
  }
  case ACKWHISPER_ALL_IGNORED: {
    const std::string text = "(" + this->lastWhisperName + ")" + LookupMessage(MSI_NO_RECEIVE_MODE_FOR_ALL);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 2, 0, 0);
    return;
  }
  default: {
    const std::string text = "(" + this->lastWhisperName + ")" + LookupMessage(MSI_NOT_EXIST_CHARACTER);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 2, 0, 0);
    if (g_session.m_whisperRecord != 0) {
      this->WriteChatLog(text.c_str());
    }
    return;
  }
  }
}

// OFFSET: 0x00D08390
// FUNCTION: fn_21588
void CGameMode::ReceiveChangeZoneServerWithHost(const unsigned char* packet) {
  const int port = *reinterpret_cast<const std::int16_t*>(packet + 0x1A);
  const int ip = *reinterpret_cast<const int*>(packet + 0x16);
  const int y = *reinterpret_cast<const std::int16_t*>(packet + 0x14);
  const int x = *reinterpret_cast<const std::int16_t*>(packet + 0x12);
  const std::string host(reinterpret_cast<const char*>(packet + 0x1C), reinterpret_cast<const char*>(packet + 0x9C));
  this->ChangeZoneServer(std::string(reinterpret_cast<const char*>(packet + 2), reinterpret_cast<const char*>(packet + 0x12)).c_str(), x, y, ip, port, host.c_str());
}

// OFFSET: 0x00D08110
// FUNCTION: fn_21585
void __stdcall ProcessVendingSale(int field8, int itemIdWord, int quantityWord, int field14, int field18) {
  const int itemId = static_cast<short>(itemIdWord);
  const int quantity = static_cast<short>(quantityWord);
  ITEM_INFO item = g_session.FindMerchantPurchaseItem(static_cast<unsigned int>(itemId));
  item.identity[1] = static_cast<std::uint32_t>(itemId);
  item.quantity = quantity;
  item.prices[0] = static_cast<std::uint32_t>(field14);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMLOGWND_MODE_0);
  window->SendMsg(nullptr, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&item), field18, field8, 0);
  g_session.RemoveMerchantPurchaseItem(item);
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(
    UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ITEM_IS_SOLD), item.DisplayName(0).c_str(), quantity).c_str()), 0xFFFF64, 0, 0, 0
  );
  if (g_windowMgr.merchantItemWnd != nullptr) {
    g_windowMgr.merchantItemWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (g_session.GetList9Size() == 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_IS_SOLD_OUT)), 0xFF00, 0, 0, 0);
    if (g_windowMgr.merchantItemMyShopWnd != nullptr) {
      g_windowMgr.merchantItemMyShopWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      g_windowMgr.merchantItemMyShopWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
    }
  } else if (g_windowMgr.merchantItemMyShopWnd != nullptr) {
    g_windowMgr.merchantItemMyShopWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (g_windowMgr.merchantItemMyShopWnd != nullptr && g_session.m_muteSellSound) {
    g_soundMgr->PlayWave("effect\\ef_steal.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x00D08670
// FUNCTION: fn_21592
__declspec(noinline) void CGameMode::ProcessBuyingStorePurchase(int updateTime, int itemId, int amount, int zeny, int zenyLimit, int charId) {
  ITEM_INFO item = g_session.FindItem1758ByItemId(itemId);
  item.SetItemId(itemId);
  item.quantity = amount;
  item.prices[0] = static_cast<std::uint32_t>(zeny);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMLOGWND_MODE_1);
  window->SendMsg(nullptr, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&item), charId, updateTime, 0);
  g_session.RemoveItem1758ByItemId(item);
  if (g_windowMgr.itemWnd != nullptr) {
    g_windowMgr.itemWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (g_windowMgr.buyingShopMyItemsWnd != nullptr) {
    g_windowMgr.buyingShopMyItemsWnd->SendMsg(nullptr, UM_SET_BUYING_SHOP_ZENY, zenyLimit, 0, 0, 0);
    g_windowMgr.buyingShopMyItemsWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    if (g_session.m_muteSellSound) {
      g_soundMgr->PlayWave("effect\\ef_steal.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    if (g_session.GetList9Size() == 0) {
      g_windowMgr.buyingShopMyItemsWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D08640
// FUNCTION: fn_21591
__declspec(noinline) void CGameMode::ReceiveBuyingStorePurchase(const unsigned char* packet) {
  this->ProcessBuyingStorePurchase(
    *reinterpret_cast<const int*>(packet + 0x14),
    *reinterpret_cast<const int*>(packet + 2),
    *reinterpret_cast<const short*>(packet + 6),
    *reinterpret_cast<const int*>(packet + 8),
    *reinterpret_cast<const int*>(packet + 0xC),
    *reinterpret_cast<const int*>(packet + 0x10));
}

// OFFSET: 0x00D246B0
// FUNCTION: fn_21668
__declspec(noinline) void CGameMode::ReceiveItem1740BasicList(const unsigned char* data) {
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(Item1740Record);
  const Item1740Record* records = reinterpret_cast<const Item1740Record*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const Item1740Record* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[2] = record->location;
    item.identity[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.damaged = record->damaged;
    item.refiningLevel = record->refine;
    item.expireTime = record->expireTime;
    g_session.AddMerchantShopItem(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D24810
// FUNCTION: fn_21669
__declspec(noinline) void CGameMode::ReceiveItem1740BasicListBind(const unsigned char* data) {
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(Item1740EquipRecord);
  const Item1740EquipRecord* record = reinterpret_cast<const Item1740EquipRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[2] = record->location;
    item.identity[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.damaged = record->damaged;
    item.refiningLevel = record->refine;
    item.expireTime = record->expireTime;
    item.bindOnEquip = record->bindOnEquip;
    item.sprite = record->sprite;
    g_session.AddMerchantShopItem(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D2D770
// FUNCTION: fn_21720
__declspec(noinline) void CGameMode::ReceiveItemInventoryList(const unsigned char* data) {
  g_session.KeepEquipment();

  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(StoreNormalRecord);
  const StoreNormalRecord* records = reinterpret_cast<const StoreNormalRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const StoreNormalRecord* record = &records[i];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[3] = record->location;
    item.identity[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.identified = record->flags & 1;
    item.damaged = 0;
    item.refiningLevel = 0;
    item.expireTime = record->expireTime;
    item.flags[0] = (record->flags >> 1) & 1;
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
int CountItemRandomOptions(const unsigned char* entries) {
  int count = 0;
  for (int index = 0; index < 5; ++index) {
    if (0 != *reinterpret_cast<const std::uint16_t*>(entries + index * 5)) {
      ++count;
    }
  }
  return count;
}

// OFFSET: 0x00D77C60
// FUNCTION: fn_22153
__declspec(noinline) void CGameMode::ReceivePartyJobLevel(const unsigned char* packet) {
  g_session.SetPartyJobLevel(
    *reinterpret_cast<const std::uint32_t*>(packet + 2), *reinterpret_cast<const std::uint16_t*>(packet + 6),
    *reinterpret_cast<const std::uint16_t*>(packet + 8)
  );
  RefreshPartyWindows(&g_session);
}

// OFFSET: 0x00D8A260
// FUNCTION: fn_22217
__declspec(noinline) void CGameMode::ReceivePartyDead(const unsigned char* packet) {
  MessengerMember member = g_session.PartyByAccount(*reinterpret_cast<const std::uint32_t*>(packet + 2));
  if (member.valid != 0) {
    g_session.SetPartyDead(*reinterpret_cast<const std::uint32_t*>(packet + 2), packet[6]);
    RefreshPartyWindows(&g_session);
  }
}

// OFFSET: 0x00D5AB80
// FUNCTION: fn_21962
void CGameMode::ReceiveShopItemList(
  const std::vector<ITEM_INFO>& items, int value, int unused1, int argument, int unused2, std::uint32_t duration, unsigned char myShop
) {
  CSession* lists = &g_session;
  if (myShop == 1) {
    lists->ClearMerchantPurchaseItems();
    for (ITEM_INFO item : items) {
      lists->AddMerchantPurchaseItem(item);
    }

    if (UIMerchantItemMyShopWnd* window = static_cast<UIMerchantItemMyShopWnd*>(g_windowMgr.EnsureWindow(WID_MERCHANTITEMMYSHOPWND))) {
      window->OpenShop();
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETNAID, value, 0, 0, 0);
      window->SetDuration(duration);
    }
  } else {
    lists->ClearMerchantMirrorItems();
    lists->ClearMerchantHopeToSellItems();
    for (ITEM_INFO item : items) {
      lists->AddMerchantMirrorItem(item);
    }

    if (UIMerchantItemShopWnd* window = static_cast<UIMerchantItemShopWnd*>(g_windowMgr.EnsureWindow(WID_MERCHANTITEMSHOPWND))) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETNAID, value, 0, 0, 0);
      window->StartTimer(duration);
    }

    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMPURCHASEWND)) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETNAID, value, argument, 0, 0);
    }
  }

  this->isOnQuest = 1;
}

// OFFSET: 0x00D79AF0
// FUNCTION: fn_22177
__declspec(noinline) void CGameMode::ReceiveGuildAgitMoveResult(short result) {
  switch (result) {
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RESULT_BUY_FALSE_MONEY)), 0xff, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOOLTIP_OF_OPTIONOFF_BTN)), 0xff, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D2A1F0
// FUNCTION: fn_21702
__declspec(noinline) void CGameMode::ReceiveBuyingStoreClose(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 3:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUYINGSTORE_TRADE_OVERLIMITZENY)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUYINGSTORE_TRADE_BUYCOMPLETE)), 0xFF, 0, 0, 0);
    break;
  }

  this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
  g_windowMgr.CloseWindow(WID_MERCHANTITEMMYSHOPWND_MODE_1);
}

// OFFSET: 0x00D7A0E0
// FUNCTION: fn_22187
__declspec(noinline) void CGameMode::ReceiveActorMessage7(const unsigned char* packet) {
  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2) - 8;
  if (length == 0) {
    return;
  }

  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  CRenderObject* actor = g_session.m_aid == aid ? this->scene->player : this->scene->GetGameActorByAID(aid);
  if (!actor) {
    return;
  }

  char text[256];
  std::memcpy(text, packet + 8, length);
  const std::int64_t value = reinterpret_cast<std::intptr_t>(text);
  actor->SendMsg(nullptr, AM_CHAT, value, 0, 0, 0, 0, 0);
}
// OFFSET: 0x00D7AEB0
// FUNCTION: fn_22194
__declspec(noinline) void CGameMode::Zc_Start_Baby(const unsigned char*) {
  this->skillId = 20001;
  this->skillAttackRange = 5;
  this->skillUseLevel = 0;
  this->skillUseType = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AEE0
// FUNCTION: fn_22195
__declspec(noinline) void CGameMode::Zc_Start_Capture(const unsigned char*) {
  this->skillId = 10000;
  this->skillAttackRange = 5;
  this->skillUseLevel = 0;
  this->skillUseType = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AF10
// FUNCTION: fn_22196
__declspec(noinline) void CGameMode::Zc_Start_Collection(const unsigned char*) {
  this->skillId = 40000;
  this->skillAttackRange = 1;
  this->skillUseLevel = 0;
  this->skillUseType = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AF40
// FUNCTION: fn_22197
__declspec(noinline) void CGameMode::Zc_Start_Couple(const unsigned char*) {
  this->skillId = 20000;
  this->skillAttackRange = 5;
  this->skillUseLevel = 0;
  this->skillUseType = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D591A0
// FUNCTION: fn_21959
void CGameMode::ApplyActorStatusEffects(std::uint32_t aid, int state, int flag, int options) {
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (!actor) {
    return;
  }

  actor->characterLevel = static_cast<std::uint16_t>(flag);
  if (!StateFlag06(state)) {
    actor->UpdateMaxLevelAura();
  }

  if (actor->IsPlayerClass(actor->GetEffectiveJob(-1))) {
    if (StateFlag01(actor->GetStateFlags()) || StateFlag02(actor->GetStateFlags())) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 203, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 254, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 1132, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 418, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 586, 0, 0, 0, 0, 0);
      return;
    }

    if (options & 0x1) {
      if (!actor->HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 130, 0, 0, 0);
        }
      }
    } else if (options & 0x2) {
      if (!actor->HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 128, 0, 0, 0);
        }
      }
    } else if (options & 0x4) {
      if (!actor->HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 169, 0, 0, 0);
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
      case JT_MONK:
      case JT_MONK_H:
      case JT_MONK_B:
      case JT_SURA:
      case JT_SURA_H:
      case JT_SURA_B:
      case JT_INQUISITOR:
        actor->SetFrameSequence(96, 4, 4, 96, 4, 1, 99999999, 0);
        break;
      default:
        actor->render.isMotionFreezed = true;
        break;
      }
      actor->render.colorLocked = 1;
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

  if (StateFlag01(actor->GetStateFlags()) || StateFlag02(actor->GetStateFlags())) {
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 203, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 254, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 1132, 0, 0, 0, 0, 0);
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 418, 0, 0, 0, 0, 0);
    return;
  }

  if (options & 0x20) {
    actor->render.isMotionFreezed = true;
    actor->render.colorLocked = 1;
    actor->SetColor(-1, 64, 64, 64);
  }

  if (options & 0x2) {
    if (!actor->HasId(420)) {
      actor->LaunchEffect(420, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(420)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 420, 0, 0, 0, 0, 0);
      actor->LaunchEffect(540, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (options & 0x4) {
    if (!actor->HasId(427)) {
      actor->LaunchEffect(427, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(427)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 427, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x10) {
    if (!actor->HasId(254)) {
      actor->LaunchEffect(254, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      actor->LaunchEffect(1132, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(254)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 254, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 1132, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x10000) {
    if (!actor->HasId(655)) {
      actor->LaunchEffect(655, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(655)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 655, 0, 0, 0, 0, 0);
      actor->LaunchEffect(656, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (options & 0x8000) {
    if (!actor->HasId(667)) {
      actor->LaunchEffect(667, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(667)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 667, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x800) {
    if (!actor->HasId(375)) {
      actor->LaunchEffect(375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(375)) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 375, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x40000) {
    if (!actor->HasId(744)) {
      actor->LaunchEffect(744, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->HasId(744) == 1) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 744, 0, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D7B350
// FUNCTION: fn_22201
__declspec(noinline) void CGameMode::ReceivePlayerRouteClear(const unsigned char*) {
  this->scene->player->SendMsg(nullptr, AM_CLEAR_ROUTE, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D78010
// FUNCTION: fn_22156
__declspec(noinline) void CGameMode::ReceiveActorStatusEffects(const unsigned char* packet) {
  this->ApplyActorStatusEffects(
    *reinterpret_cast<const std::uint32_t*>(packet + 2),
    *reinterpret_cast<const int*>(packet + 6),
    *reinterpret_cast<const int*>(packet + 0xA),
    *reinterpret_cast<const int*>(packet + 0xE)
  );
}

// OFFSET: 0x00D89C60
// FUNCTION: fn_22212
__declspec(noinline) void CGameMode::ReceivePartyMemberInfoWithoutCharacterId(const unsigned char* packet) {
  this->UpdatePartyMemberInfo(
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
__declspec(noinline) void CGameMode::ReceiveLapineBoxIngredient(const unsigned char* packet) {
  if (g_windowMgr.LookupWindow(WID_LAPINEBOXWND) != nullptr) {
    return;
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_LAPINEBOXWND)) {
    static_cast<UILapineBoxWnd*>(window)->SetIngredient(static_cast<int>(reinterpret_cast<std::intptr_t>(packet)));
  }
}

// OFFSET: 0x00D79B40
// FUNCTION: fn_22178
__declspec(noinline) void CGameMode::ActivateMacroBlackListCheck(unsigned char state, int duration) {
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MACROBLACKLISTCHECKWND)) {
    static_cast<UIMacroBlackListCheckWnd*>(window)->Activate(state, duration);
  }
}

// OFFSET: 0x00D79B70
// FUNCTION: fn_22179
__declspec(noinline) void CGameMode::SetMacroDetectorCountdown(unsigned char count, std::uint32_t duration) {
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MACRODETECTORWND)) {
    window->Show(1);
    static_cast<UIMacroDetectorWnd*>(window)->SetCountdown(count, duration);
  }
}

// OFFSET: 0x00D79340
// FUNCTION: fn_22164
__declspec(noinline) void CGameMode::ReceiveCaptchaPreviewImage(const char* key, const void* data, unsigned short size) {
  UICaptchaPreviewWnd* window = static_cast<UICaptchaPreviewWnd*>(g_windowMgr.LookupWindow(WID_CAPTCHAPREVIEWWND));
  if (window != nullptr && std::strncmp(window->m_captchaKey, key, 4) == 0) {
    window->AddImageData(data, size);
  }
}

// OFFSET: 0x00D793D0
// FUNCTION: fn_22166
__declspec(noinline) void CGameMode::ReceiveLapineUpgradeBoxValue(int value) {
  if (g_windowMgr.LookupWindow(WID_LAPINEUPGRADEBOXWND) != nullptr) {
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_LAPINEUPGRADEBOXWND);
  if (window == nullptr) {
    return;
  }

  window->SendMsg(nullptr, UM_REFRESHITEM, value, 0, 0, 0);
}

// OFFSET: 0x00D7B420
// FUNCTION: fn_22203
__declspec(noinline) void CGameMode::Zc_Trycapture_Monster(const unsigned char* packet) {
  if (packet[2] != 0) {
    if (g_windowMgr.petTamingDeceiveWnd) {
      g_windowMgr.petTamingDeceiveWnd->SendMsg(nullptr, UM_SET_TARGET, 1, 7, 0, 0);
    }
  } else {
    if (g_windowMgr.petTamingDeceiveWnd) {
      g_windowMgr.petTamingDeceiveWnd->SendMsg(nullptr, UM_SET_TARGET, 1, 3, 0, 0);
    }
  }
}

// OFFSET: 0x00D7B470
// FUNCTION: fn_22204
__declspec(noinline) void CGameMode::Zc_Trycollection(const unsigned char* packet) {
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_COLLECTIONSYSTEMWND)) {
    if (packet[2] != 0) {
      window->SendMsg(nullptr, UM_SET_TARGET, 1, 7, 0, 0);
    } else {
      window->SendMsg(nullptr, UM_SET_TARGET, 1, 3, 0, 0);
    }
  }
}

// OFFSET: 0x00D32830
// FUNCTION: fn_21745
__declspec(noinline) void CGameMode::Zc_Msg(const unsigned char* packet) {
  std::uint32_t color = 0xFFFFFF;
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
  case MSI_CANNOT_PARTYCALL:
  case MSI_NO_PARTYMEM_ON_THISMAP:
  case MSI_INVALID_CLASS:
  case MSI_INVALID_GENDER:
  case MSI_QUESTUI_TAB_HOLD:
    color = 0xFF;
    break;
  }

  int type = 0;
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
  case MSI_SEND_TO_TWITTER_FAILED:
  case MSI_KEYWORD_FILTER:
    type = 0x17;
    break;
  }

  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  const char* text = LookupMessage(*reinterpret_cast<const std::uint16_t*>(packet + 2));
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), color, type, 0, 0);
  const char* chatText = LookupMessage(*reinterpret_cast<const std::uint16_t*>(packet + 2));
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(chatText), color, type, 0, 0);
}

// OFFSET: 0x00D328E0
// FUNCTION: fn_21746
__declspec(noinline) void CGameMode::Zc_Msg_Color(const unsigned char* packet) {
  std::uint32_t color = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
  case MSI_CANNOT_PARTYCALL:
  case MSI_NO_PARTYMEM_ON_THISMAP:
  case MSI_INVALID_CLASS:
  case MSI_INVALID_GENDER:
  case MSI_QUESTUI_TAB_HOLD:
    color = 0xFF;
    break;
  }

  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(*reinterpret_cast<const std::uint16_t*>(packet + 2))), color, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(*reinterpret_cast<const std::uint16_t*>(packet + 2))), color, 0, 0, 0);
}

// OFFSET: 0x00D7B5E0
// FUNCTION: fn_22207
__declspec(noinline) void CGameMode::ReceiveRouletteBonusPoints(const unsigned char* packet) {
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

  if (g_windowMgr.roulletteWnd != nullptr) {
    g_windowMgr.RefreshRouletteWnd();
  }
}

// OFFSET: 0x00D7AE60
// FUNCTION: fn_22193
__declspec(noinline) void CGameMode::ReceiveActorCharm(const unsigned char* packet) {
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
__declspec(noinline) void CGameMode::ReceiveAntiBotChallenge(int value) {
  ValuePacket packet;
  packet.id = ID_CZ_ACK_AU_BOT;
  packet.value = value;
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
}

// OFFSET: 0x00D79E10
// FUNCTION: fn_22182
__declspec(noinline) void CGameMode::ReceiveCashShopBanner(int value1, int value2, int value3) {
  if (!g_windowMgr.CloseWindow(WID_CASHSHOP_WND)) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_CASHSHOP_WND)) {
      window->SendMsg(nullptr, UM_SELECT_CASHSHOP_TAB, value3, 0, 0, 0);
      window->SendMsg(nullptr, UM_SET_CASHSHOP_BALANCE, value1, value2, 0, 0);
    } else {
      g_modeMgr.gameMode->SendMsg(MM_REQUEST_CASH_SHOP, value3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D7A180
// FUNCTION: fn_22188
__declspec(noinline) void CGameMode::ReceiveWindow308TextEntries(unsigned char* packet) {
  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 16;
  g_windowMgr.CloseWindow(WID_SHOWWARPWND);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_SHOWWARPWND);
  for (int i = 0; i < count; ++i) {
    char* text = reinterpret_cast<char*>(packet + 4 + i * 16);
    text[15] = '\0';
    if (std::strlen(text) != 0) {
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D7B390
// FUNCTION: fn_22202
__declspec(noinline) void CGameMode::ReceiveActorStateValue(std::uint32_t aid, int unused, std::int16_t value) {
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

  if (actor->UpdateMaximumValue(value) == true) {
    actor->LaunchEffect(0x2a5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x00D898B0
// FUNCTION: fn_22209
__declspec(noinline) void CGameMode::ReceiveChatEntries(unsigned char* data) {
  ChatEntryListPacket* packet = reinterpret_cast<ChatEntryListPacket*>(data);
  const int count = (packet->length - 6) / static_cast<int>(sizeof(ChatTextEntry));
  g_windowMgr.CloseWindow(WID_CHOOSEWARPWND);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_CHOOSEWARPWND);
  window->SendMsg(nullptr, UM_SETSKID, packet->skillId, 0, 0, 0);
  for (int i = 0; i < count; ++i) {
    ChatTextEntry* entry = &packet->entries[i];
    entry->text[15] = '\0';
    if (std::strlen(entry->text) != 0) {
      window->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>(entry->text), 0, 0, 0);
    }
  }
  window->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>("cancel"), 0, 0, 0);
}

// OFFSET: 0x00D7A200
// FUNCTION: fn_22189
__declspec(noinline) void CGameMode::ReceiveSoulEnergy(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorValuePacket*>(data);
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAidIncludeMe(packet->accountId));
  if (!actor) {
    return;
  }

  const int job = actor->GetEffectiveJob(-1);
  if (job != 4240 && job != 4242 && job != 4303) {
    return;
  }

  if (StateFlag01(actor->GetStateFlags()) || StateFlag02(actor->GetStateFlags())) {
    g_effectMgr->Retire(actor->render.accountId, 1225);
  }

  CEffect* effect = g_effectMgr->Find(actor->render.accountId, 1225);
  if (!effect) {
    effect = g_effectMgr->Spawn(actor->render.accountId, 1225, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (!effect) {
      return;
    }
  }

  effect->effectLevel = packet->value;
  effect->Open();
}

// OFFSET: 0x00D25850
// FUNCTION: fn_21673
__declspec(noinline) void CGameMode::ReceiveItemDeletionNotice(const unsigned char* packet) {
  g_session.RemoveInventoryItem(*reinterpret_cast<const std::int16_t*>(packet + 2), 1);
  char number[12];
  char text[260];
  _itoa(*reinterpret_cast<const int*>(packet + 4), number, 10);
  const char* name = FindItemDescription(std::atoi(number)).identifiedName;
  std::sprintf(text, LookupMessage(MSI_NOTICE_TO_DELETE_ITEM), name);
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
}

// OFFSET: 0x00D31A40
// FUNCTION: fn_21735
__declspec(noinline) void CGameMode::ReceiveItemNameListNotice(const unsigned char* packet) {
  char text[512];
  const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) >> 2;
  const int* ids = reinterpret_cast<const int*>(packet + 4);
  std::strcpy(text, FindItemDescription(*ids).identifiedName);
  ++ids;
  for (unsigned int index = 1; index < count; ++index, ++ids) {
    std::sprintf(text, "%s, %s", text, FindItemDescription(*ids).identifiedName);
  }

  std::sprintf(text, LookupMessage(MSI_JOINGUILD_NOTEXIST_USER), text);
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
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
__declspec(noinline) void CGameMode::ReceiveInventoryItemUpdate(const unsigned char* packet) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t*>(packet + 2));
  if (item.identity[1] != 0) {
    item.flags[0] = *reinterpret_cast<const unsigned char*>(packet + 4) == 0;
    g_session.UpdateInventoryItem(item, 1);
  }
}

// OFFSET: 0x00D78030
// FUNCTION: fn_22157
__declspec(noinline) void CGameMode::OpenWindow154() {
  if (g_windowMgr.LookupWindow(WID_ACCOUNTLIMITEDTOOLWND) == nullptr) {
    g_windowMgr.EnsureWindow(WID_ACCOUNTLIMITEDTOOLWND);
  }
}

// OFFSET: 0x00D2D990
// FUNCTION: fn_21722
__declspec(noinline) void CGameMode::ReceiveMemorialDungeonError(const unsigned char*) {
  if (g_windowMgr.memorialDunWnd) {
    g_windowMgr.CloseWindow(WID_MEMORIALDUNWND);
    GetRagConnection().Close();
    if (g_windowMgr.Confirm(LookupMessage(MSI_MEMORIAL_DUN_ERROR), 0, true, true, 0, "Message", 280, 120, 0) != 269) {
      this->SendMsg(MM_QUIT_TO_IDENTRY, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D4E3F0
// FUNCTION: fn_21876
void CGameMode::ReceiveItemIdentificationList(const unsigned char* packet) {
  unsigned int count = *reinterpret_cast<const std::uint16_t*>(packet + 2) - 4;
  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMIDENTIFYWND);
  window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
  count >>= 1;
  const std::int16_t* ids = reinterpret_cast<const std::int16_t*>(packet + 4);
  for (unsigned int index = count; index != 0; --index, ++ids) {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *ids);
    if (item.identity[1] != 0) {
      window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.identity[1], 0, 0);
    }
  }

  window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
}

// OFFSET: 0x00D4E2A0
// FUNCTION: fn_21875
void CGameMode::ReceiveCardCompositionItemList(const unsigned char* packet) {
  unsigned int count = *reinterpret_cast<const std::uint16_t*>(packet + 2) - 4;
  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOMPOSITIONWND);
  window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
  window->SendMsg(nullptr, UM_SET_CARD_ITEM_INDEX, *reinterpret_cast<const int*>(this->cardAndSkillBalloonIds), 0, 0, 0);
  count >>= 1;
  const std::int16_t* ids = reinterpret_cast<const std::int16_t*>(packet + 4);
  for (unsigned int index = count; index != 0; --index, ++ids) {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *ids);
    if (item.identity[1] != 0) {
      window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.identity[1], 0, 0);
    }
  }

  window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
}

// OFFSET: 0x00D75960
// FUNCTION: fn_22149
void CGameMode::ReceiveNotifySkill(const unsigned char* packet) {
  ActorSkillMessage message;
  message.skillId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  message.damage = *reinterpret_cast<const std::int32_t*>(packet + 0x18);
  message.actionType = packet[0x20];
  message.targetId = *reinterpret_cast<const std::uint32_t*>(packet + 8);
  message.tick = *reinterpret_cast<const std::uint32_t*>(packet + 0xC);
  message.skillLevel = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  message.hitCount = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  message.cellX = -1;
  message.cellY = -1;
  message.attackMotion = *reinterpret_cast<const std::uint32_t*>(packet + 0x10);
  message.damageMotion = *reinterpret_cast<const std::uint32_t*>(packet + 0x14);

  const std::uint16_t skillId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  const std::uint32_t sourceAid = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  const std::uint32_t targetAid = *reinterpret_cast<const std::uint32_t*>(packet + 8);

  CGameActor* gameActor = nullptr;
  CGameActor* source = nullptr;
  int job = -1;

  if (skillId == RA_WUGDASH && targetAid == g_session.m_aid) {
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
    this->scene->player->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
    source = this->scene->player;
    job = source->GetEffectiveJob(-1);
  } else {
    gameActor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(sourceAid));
    if (gameActor != nullptr) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      gameActor->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
      source = gameActor;
      job = source->GetEffectiveJob(-1);
    } else if (CRenderObject* skill = this->scene->GetSkillByAID(sourceAid)) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      skill->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
    }
  }

  CRenderObject* targetSkill;
  if (target == nullptr) {
    targetSkill = this->scene->GetSkillByAID(targetAid);
  } else {
    targetSkill = nullptr;
  }

  CSkillActionMgr::GetInstance()->OnSkillHit(
    skillId, reinterpret_cast<int>(source), target, static_cast<CGameActor*>(targetSkill)
  );

  const vector3d origin{0.0f, 0.0f, 0.0f};
  int effectId;

  if (target != nullptr && source != nullptr) {
    switch (skillId) {
    case NPC_RAYOFGENESIS:
    case LG_RAYOFGENESIS:
    case WM_METALICSOUND: {
      effectId = skillId == WM_METALICSOUND ? 0x62F : 0x593;
      const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(duration);
      }
      break;
    }
    case NPC_COMET:
      target->LaunchEffect(0x5C, origin, 0.0f);
      source->SetState(7, 1);
      break;
    case NPC_WIDECRITICALWOUND:
      if (source == this->scene->player) {
        source->LaunchEffect(0x461, origin, 0.0f);
      }
      if (target == this->scene->player) {
        target->LaunchEffect(0x461, origin, 0.0f);
      }
      break;
    case NPC_HELLJUDGEMENT:
    case NPC_HELLJUDGEMENT2:
      if (source != target) {
        target->LaunchEffect(0x174, origin, 0.0f);
      }
      break;
    case RK_DEATHBOUND:
      source->SetState(2, 1);
      target->LaunchEffect(0xB7, origin, 0.0f);
      break;
    case WL_COMET:
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x5FE, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x5FF;
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x585, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x586;
      }
      g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      source->SetState(7, 1);
      break;
    case SO_VARETYR_SPEAR:
      g_effectMgr->Spawn(target->render.accountId, 0x619, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x61A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case GN_CARTCANNON: {
      CEffect* effect;
      if (g_serviceOptions.Get(0xE5)) {
        effect = g_effectMgr->Spawn(source->render.accountId, 0x5DA, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(200);
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x5D8, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(400);
        }
        effectId = 0x5D9;
      } else {
        effect = g_effectMgr->Spawn(source->render.accountId, 0x550, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(200);
        }
        effect = g_effectMgr->Spawn(source->render.accountId, 0x551, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(200);
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x54E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(400);
        }
        effectId = 0x54F;
      }
      effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(400);
      }
      break;
    }
    case ABC_UNLUCKY_RUSH:
      g_effectMgr->Spawn(
        source->render.accountId, 0x86A, source->render.position.x, source->render.position.y, source->render.position.z,
        target->render.accountId, -1, 0, 0
      );
      break;
    case MH_GLANZEN_SPIES: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const vector3d from = source->render.position;
      const vector3d to = target->render.position;
      const float yaw = mode->CameraDesiredYaw();
      const float heading = static_cast<float>(std::atan2(static_cast<double>(to.z - from.z), static_cast<double>(to.x - from.x)));
      const float angle = 360.0f - (heading / 3.141592f * 180.0f + yaw);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x91F, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->SetAngle(angle);
      }
      break;
    }
    }
  }

  if (source != nullptr && skillId == WL_FROSTMISTY) {
    source->SetState(7, 1);
  }

  if (target != nullptr) {
    switch (skillId) {
    case AS_SPLASHER:
      if (*reinterpret_cast<const std::int16_t*>(packet + 0x1C) == -1) {
        target->LaunchEffect(0x81, origin, 0.0f);
      }
      break;
    case NPC_DRAGONBREATH:
    case RK_DRAGONBREATH: {
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
            effect->SetDelay(200);
          }
        }
        effectId = 0x5F0;
      } else {
        if (g_effectMgr->Find(source->render.accountId, directionId) == nullptr) {
          effect = g_effectMgr->Spawn(source->render.accountId, directionId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
          if (effect) {
            effect->SetDelay(200);
          }
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x562, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(350);
        }
        effectId = 0x563;
      }
      effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(350);
      }
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case RK_DRAGONBREATH_WATER: {
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
            effect->SetDelay(200);
          }
        }
        effectId = 0x5F6;
      } else {
        if (g_effectMgr->Find(source->render.accountId, directionId) == nullptr) {
          effect = g_effectMgr->Spawn(source->render.accountId, directionId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
          if (effect) {
            effect->SetDelay(200);
          }
        }
        effect = g_effectMgr->Spawn(target->render.accountId, 0x56A, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(350);
        }
        effectId = 0x56B;
      }
      effect = g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(350);
      }
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    }
    case RK_WINDCUTTER:
      if (target != source) {
        g_effectMgr->Spawn(target->render.accountId, 0x69D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      }
      break;
    case WL_JACKFROST: {
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
    case RA_ELECTRICSHOCKER:
      g_soundMgr->PlayWave("effect\\hunter_shockwavetrap.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x92, origin, 0.0f);
      break;
    case RA_CLUSTERBOMB:
      target->LaunchEffect(0x249, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\hunter_landmine.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case RA_FIRINGTRAP:
      target->LaunchEffect(0x247, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case RA_ICEBOUNDTRAP:
      target->LaunchEffect(0x87, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\ef_frostdiver.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case NC_FLAMELAUNCHER:
      target->LaunchEffect(0x247, origin, 0.0f);
      target->LaunchEffect(0x31, origin, 0.0f);
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case NC_ARMSCANNON: {
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
            effect->SetDelay(duration);
          }
          effectId = 0x5D3;
        } else {
          effect = g_effectMgr->Spawn(0, 0x54B, position.x, position.y, position.z, 0, -1, 0, 0);
          if (effect) {
            effect->SetDelay(duration);
          }
          effectId = 0x54C;
        }
        effect = g_effectMgr->Spawn(0, effectId, position.x, position.y, position.z, 0, -1, 0, 0);
        if (effect) {
          effect->SetDelay(duration);
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
    case SC_FEINTBOMB:
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x5B7, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x5B8;
      } else {
        g_effectMgr->Spawn(target->render.accountId, 0x520, origin.x, origin.y, origin.z, 0, -1, 0, 0);
        effectId = 0x521;
      }
      g_effectMgr->Spawn(target->render.accountId, effectId, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case SR_FALLENEMPIRE: {
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x624, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(100);
      }
      break;
    }
    case GN_SPORE_EXPLOSION:
      g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x385, origin, 0.0f);
      break;
    case GN_HELLS_PLANT:
      g_effectMgr->Spawn(target->render.accountId, 0x551, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_soundMgr->PlayWave("spore_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case SR_HOWLINGOFLION:
      g_effectMgr->Spawn(target->render.accountId, 0x62C, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case CD_ARBITRIUM:
      g_effectMgr->Spawn(target->render.accountId, 0x720, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x721, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case SHC_FATAL_SHADOW_CROW:
      g_effectMgr->Spawn(target->render.accountId, 0x748, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x749, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case MH_TWISTER_CUTTER:
      g_effectMgr->Spawn(target->render.accountId, 0x919, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case MH_ABSOLUTE_ZEPHYR:
      g_effectMgr->Spawn(target->render.accountId, 0x916, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x917, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case MH_BLAZING_AND_FURIOUS:
      g_effectMgr->Spawn(target->render.accountId, 0x921, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case MH_TOXIN_OF_MANDARA:
      g_effectMgr->Spawn(target->render.accountId, 0x912, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case MH_NEEDLE_STINGER:
      g_effectMgr->Spawn(target->render.accountId, 0x913, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x914, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case EM_EL_FLAMEROCK:
      g_effectMgr->Spawn(target->render.accountId, 0x7F3, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x7F4, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case EM_EL_AGE_OF_ICE:
      g_effectMgr->Spawn(target->render.accountId, 0x7E3, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x7E4, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case EM_EL_STORM_WIND:
      g_effectMgr->Spawn(target->render.accountId, 0x832, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x833, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case EM_EL_AVALANCHE:
      g_effectMgr->Spawn(target->render.accountId, 0x838, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case EM_EL_DEADLY_POISON:
      g_effectMgr->Spawn(target->render.accountId, 0x7D9, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case ABR_BATTLE_BUSTER:
      g_effectMgr->Spawn(target->render.accountId, 0x78E, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x791, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case ABR_DUAL_CANNON_FIRE:
      g_effectMgr->Spawn(target->render.accountId, 0x77B, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x77D, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    case ABR_INFINITY_BUSTER:
      g_effectMgr->Spawn(target->render.accountId, 0x772, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x773, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x775, origin.x, origin.y, origin.z, 0, -1, 0, 0);
      break;
    }
  }

  if (source != nullptr) {
    if (g_session.IsMonsterActor(source) == 0) {
      this->SendMsg(MM_MAKE_SKILL_USED_MSG, skillId, reinterpret_cast<std::intptr_t>(source), *reinterpret_cast<const std::int16_t*>(packet + 0x1C), 0);
      return;
    }
    const unsigned char kind = source->objectType;
    if (kind == 10 || kind == 13 || kind == 14) {
      this->SendMsg(MM_MAKE_SKILL_USED_MSG, skillId, reinterpret_cast<std::intptr_t>(source), *reinterpret_cast<const std::int16_t*>(packet + 0x1C), 0);
    }
  }

  int action;
  int loopType;
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.skillId, &action, &loopType);
  if (action != -1 && gameActor != nullptr) {
    gameActor->SetAction(action * 8 - 8, 1, loopType == 0);
  }
}

// OFFSET: 0x00D77850
// FUNCTION: fn_22150
void CGameMode::ReceiveActorSkillAction(const unsigned char* packet) {
  ActorSkillMessage message;
  message.skillId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  message.actionType = packet[0x1E];
  message.damage = *reinterpret_cast<const std::int16_t*>(packet + 0x18);
  message.tick = *reinterpret_cast<const std::uint32_t*>(packet + 0xC);
  message.targetId = *reinterpret_cast<const std::uint32_t*>(packet + 8);
  message.skillLevel = *reinterpret_cast<const std::int16_t*>(packet + 0x1A);
  message.hitCount = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  message.cellX = -1;
  message.cellY = -1;
  message.attackMotion = *reinterpret_cast<const std::uint32_t*>(packet + 0x10);
  message.damageMotion = *reinterpret_cast<const std::uint32_t*>(packet + 0x14);

  CGameActor* actor = nullptr;
  CGameActor* gameActor = nullptr;
  int job = -1;

  if (*reinterpret_cast<const std::uint32_t*>(packet + 4) == g_session.m_aid) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
    this->scene->player->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
    actor = this->scene->player;
    job = actor->GetEffectiveJob(-1);
  } else {
    gameActor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
    if (gameActor != nullptr) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      gameActor->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
      actor = gameActor;
      job = actor->GetEffectiveJob(-1);
    } else if (CRenderObject* skill = this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4))) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      skill->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
    }
  }

  if (g_session.IsMonsterActor(actor) == 0) {
    this->SendMsg(MM_MAKE_SKILL_USED_MSG, *reinterpret_cast<const std::uint16_t*>(packet + 2), reinterpret_cast<std::intptr_t>(actor), *reinterpret_cast<const std::int16_t*>(packet + 0x1A), 0);
    return;
  }

  int action;
  int loopType;
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.skillId, &action, &loopType);
  if (action != -1 && gameActor != nullptr) {
    gameActor->SetAction(action * 8 - 8, 1, loopType == 0);
  }
}

// OFFSET: 0x00D77A30
// FUNCTION: fn_22151
void CGameMode::ReceiveActorSkillMessage(const unsigned char* packet) {
  ActorSkillMessage message;
  message.skillId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  message.damage = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  message.actionType = packet[0x22];
  message.targetId = *reinterpret_cast<const std::uint32_t*>(packet + 8);
  message.tick = *reinterpret_cast<const std::uint32_t*>(packet + 0xC);
  message.skillLevel = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  message.hitCount = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  message.cellX = *reinterpret_cast<const std::int16_t*>(packet + 0x18);
  message.cellY = *reinterpret_cast<const std::int16_t*>(packet + 0x1A);
  message.attackMotion = *reinterpret_cast<const std::uint32_t*>(packet + 0x10);
  message.damageMotion = *reinterpret_cast<const std::uint32_t*>(packet + 0x14);

  CGameActor* actor = nullptr;
  int job = -1;

  if (*reinterpret_cast<const std::uint32_t*>(packet + 4) == g_session.m_aid) {
    actor = this->scene->player;
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
    actor->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
    job = actor->GetEffectiveJob(-1);
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
    if (actor != nullptr) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      actor->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
      job = actor->GetEffectiveJob(-1);
    } else if (CRenderObject* skill = this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4))) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&message);
      skill->SendMsg(nullptr, AM_SKILL, value, 0, 0, 0, 0, 0);
      actor = nullptr;
    }
  }

  if (g_session.IsMonsterActor(actor) == 0) {
    this->SendMsg(MM_MAKE_SKILL_USED_MSG, *reinterpret_cast<const std::uint16_t*>(packet + 2), reinterpret_cast<std::intptr_t>(actor), *reinterpret_cast<const std::int16_t*>(packet + 0x1E), 0);
    return;
  }

  int action;
  int loopType;
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.skillId, &action, &loopType);
}

// OFFSET: 0x00D25310
// FUNCTION: fn_21672
__declspec(noinline) void CGameMode::ReceiveItem1740List(const unsigned char* packet) {
  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<int>(static_cast<unsigned int>(length - 4) / sizeof(StoreNormalRecord));
  const auto* record = reinterpret_cast<const StoreNormalRecord*>(packet + 4);
  for (int index = count; index > 0; --index, ++record) {
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[2] = 0;
    item.identity[3] = record->location;
    item.quantity = record->amount;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.damaged = 0;
    item.identified = record->flags & 1;
    item.refiningLevel = 0;
    item.expireTime = record->expireTime;
    item.flags[0] = (record->flags >> 1) & 1;
    g_session.AddMerchantShopItem(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D270B0
// FUNCTION: fn_21687
void CGameMode::ReceiveDispel(const unsigned char* packet) {
  const auto* dispel = reinterpret_cast<const DispelPacket*>(packet);

  if (dispel->aid == g_session.m_aid) {
    this->scene->player->SendMsg(nullptr, AM_DELETE_BEGIN_SPELL, 0, 0, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, AM_DELETE_MAGIC_TARGET, 0, 0, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, AM_DELETE_SKILL_RECHARGEGAGE, 0, 0, 0, 0, 0, 0);
    CPlayer* player = this->scene->player;
    if (player->render.state == 8) {
      player->SetState(0, 1);
    }
    return;
  }

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(dispel->aid));
  if (actor == nullptr) {
    return;
  }

  actor->SendMsg(nullptr, AM_DELETE_BEGIN_SPELL, 0, 0, 0, 0, 0, 0);
  actor->SendMsg(nullptr, AM_DELETE_MAGIC_TARGET, 0, 0, 0, 0, 0, 0);
  actor->SendMsg(nullptr, AM_DELETE_SKILL_RECHARGEGAGE, 0, 0, 0, 0, 0, 0);
  g_effectMgr->Retire(dispel->aid, 1114);
  if (actor->render.state == 8) {
    actor->SetState(0, 1);
  }
}

// OFFSET: 0x00D32E10
// FUNCTION: fn_21748
__declspec(noinline) void CGameMode::ReceiveItem1758List(const unsigned char* packet) {
  if (g_replayAnimationScaling == 0) {
    reinterpret_cast<UIMerchantShopMakeWnd*>(g_windowMgr.buyingShopMakeWnd)->PrintBuyList();
  }

  g_session.ClearMerchantPurchaseSavedItems();
  g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
  g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_1);
  g_session.ClearMerchantPurchaseItems();

  const int count = static_cast<unsigned int>(static_cast<int>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 12) / 11u;
  const Item1758Record* record = reinterpret_cast<const Item1758Record*>(packet + 12);
  for (int index = 0; index < count; ++index, ++record) {
    const Item1758Record entry = *record;
    ITEM_INFO item;
    item.prices[0] = entry.price;
    item.prices[1] = entry.price;
    item.quantity = entry.quantity;
    item.identity[0] = entry.type;
    item.identity[1] = index + 1;
    item.identified = 1;
    item.SetItemId(entry.itemId);
    g_session.AddMerchantPurchaseItem(item);
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMMYSHOPWND_MODE_1);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  window->SendMsg(nullptr, UM_SET_BUYING_SHOP_ZENY, *reinterpret_cast<const int*>(packet + 8), 0, 0, 0);
  this->isOnQuest = 1;
}

// OFFSET: 0x00D50810
// FUNCTION: fn_21886
void CGameMode::ReceiveOpenedVendingItemList(const unsigned char* packet) {
  if (g_replayAnimationScaling != 0) {
    g_modeMgr.GetGameMode()->SendMsg(MM_REQ_OPENSTORE, 0, 0, 0, 0);
  }

  g_session.ClearMerchantPurchaseItems();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 8) / sizeof(Item1758StoreRecord);
  if (count == 0) {
    this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND);
    g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND);
    g_session.ClearItems1760And1748();
    return;
  }

  const Item1758StoreRecord* record = reinterpret_cast<const Item1758StoreRecord*>(packet + 8);
  for (int i = 0; i < count; ++i, ++record) {
    const Item1758StoreRecord entry = *record;
    ITEM_INFO item;
    item.identity[0] = entry.type;
    item.identity[1] = entry.index;
    item.prices[0] = entry.price;
    item.prices[1] = entry.price;
    item.quantity = entry.amount;
    std::memcpy(item.cards, entry.cards, sizeof(entry.cards));
    item.SetItemId(entry.itemId);
    item.identified = entry.identified;
    item.damaged = entry.damaged;
    item.refiningLevel = entry.refine;
    item.optionCount = CountStoreItemOptions(entry.options);
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = entry.options[index];
    }
    g_session.AddMerchantPurchaseItem(item);
  }
}

// OFFSET: 0x00D50590
// FUNCTION: fn_21885
void CGameMode::ReceiveItem1748List(const unsigned char* packet) {
  g_session.ClearMerchantMirrorItems();
  g_session.ClearMerchantHopeToSellItems();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 0x0C) / sizeof(Item1748Record);
  for (int i = 0; i < count; ++i) {
    const Item1748Record record = *reinterpret_cast<const Item1748Record*>(packet + 0x0C + i * sizeof(Item1748Record));
    ITEM_INFO item;
    item.identity[0] = record.type;
    item.identity[1] = record.index;
    item.identity[2] = record.location;
    item.prices[0] = record.price;
    item.prices[1] = record.price;
    item.quantity = record.amount;
    std::memcpy(item.cards, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.identified;
    item.damaged = record.damaged;
    item.refiningLevel = record.refine;
    item.sprite = record.sprite;
    item.optionCount = CountStoreItemOptions(record.options);
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = record.options[index];
    }
    g_session.AddMerchantMirrorItem(item);
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMSHOPWND);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  window->SendMsg(nullptr, UM_SETNAID, *reinterpret_cast<const std::int32_t*>(packet + 4), 0, 0, 0);
  UIWindow* optionsWindow = g_windowMgr.EnsureWindow(WID_MERCHANTITEMPURCHASEWND);
  optionsWindow->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  optionsWindow->SendMsg(nullptr, UM_SETNAID, *reinterpret_cast<const std::int32_t*>(packet + 4), *reinterpret_cast<const std::int32_t*>(packet + 8), 0, 0);
  this->isOnQuest = 1;
}

// OFFSET: 0x00D5F5C0
// FUNCTION: fn_21970
void CGameMode::ReceiveActorStatusValue(std::uint32_t aid, int status, std::int64_t amount) {
  CGameActor* actor;
  if (aid == g_session.m_aid) {
    actor = this->scene->player;
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  }
  if (actor == nullptr) {
    return;
  }

  if (status == EF_HPTIME) {
    if (aid == g_session.m_aid) {
      g_session.UpdateRandomStats();
      const int hp = g_session.hp + static_cast<int>(amount);
      if (g_session.hp != hp) {
        g_session.hp = hp;
        g_session.RefreshHpSpWindows();
      }
    }
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFF00FF00, 0x72, 0, 0, 0);
  } else if (status == EF_SPTIME) {
    if (aid == g_session.m_aid) {
      g_session.UpdateRandomStats();
      int& sp = *reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0]);
      const int value = sp + static_cast<int>(amount);
      if (sp != value) {
        sp = value;
        g_session.RefreshHpSpWindows();
      }
    }
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFF0000FF, 0x72, 0, 0, 0);
  } else if (status == EF_GREEN_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFF00FF00, 0x72, 0, 0, 0);
  } else if (status == EF_BLUE_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFF0000FF, 0x72, 0, 0, 0);
  } else if (status == EF_RED_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFFFF0000, 0x72, 0, 0, 0);
  } else if (status == EF_PURPLE_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFFFF00FF, 0x72, 0, 0, 0);
  } else if (status == EF_BLACK_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFF000000, 0x72, 0, 0, 0);
  } else if (status == EF_WHITE_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFFFFFFFF, 0x72, 0, 0, 0);
  } else if (status == EF_YELLOW_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFFFFFF00, 0x72, 0, 0, 0);
  } else if (status == EF_PINK_NUMBER) {
    actor->SendMsg(nullptr, AM_MAKE_NUMBER, amount, 0xFFFF55B1, 0x72, 0, 0, 0);
  }

  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(actor->render.accountId, status, actor->render.position, 0, 0xFFFFFFFF, 0, 0);
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
    item.price = entry.price;
    item.realPrice = entry.price;
    item.type = entry.type;
    item.location = entry.itemFlags;
    item.id = -1;
    item.identified = 1;
    item.sprite = FindItemDescription(std::atoi(item.itemId)).viewId;
    g_session.AddShopItem(item, entry.quantity, static_cast<std::uint16_t>(entry.weight / 10));
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_PARAITEMSHOPWND)) {
    window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_PARAITEMPURCHASEWND)) {
    window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
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
    item.identity[0] = record.type;
    item.priceFactor = record.weight / 10;
    item.quantity = record.quantity;
    item.prices[0] = record.price;
    item.prices[1] = record.price;
    item.refiningLevel = record.refine;
    std::memcpy(item.cards, record.cards, sizeof(record.cards));
    item.identity[1] = record.index;
    item.identified = 1;
    item.flags[3] = 1;
    if (item.quantity > 0) {
      g_session.AppendShopItem(item);
    }
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_PARAITEMSHOPWND)) {
    window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_PARAITEMPURCHASEWND)) {
    window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D7A2F0
// FUNCTION: fn_22190
void CGameMode::ShowSpecialPopup(int popupId) {
  if (this->sameMapMove[0]) {
    return;
  }

  PopupData* popup = g_specialPopupMgr->GetPopup(popupId);
  if (!popup) {
    this->SendMsg(MM_SOCKETERROR, reinterpret_cast<int>(FormatOwnedString("No GetPopupData : %d ", popupId).c_str()), 0xFFFF00FF, 0, 0);
    return;
  }

  UIWindow window(0);
  std::string text;
  switch (popup->m_messageId) {
  case 0:
    for (auto description = popup->m_descriptions.begin(); description != popup->m_descriptions.end(); ++description) {
      text = RemoveTextColors(description->c_str());
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), popup->m_color, 0, 0, 0);
    }
    break;
  case 1:
    if (UIWindow* popupWindow = g_windowMgr.EnsureWindow(WID_BALLOONOPENBTN)) {
      popupWindow->SendMsg(nullptr, UM_COMMAND, 510, popupId, 0, 0);
    }
    [[fallthrough]];
  case 2:
    for (auto description = popup->m_descriptions.begin(); description != popup->m_descriptions.end(); ++description) {
      text = RemoveTextColors(description->c_str());
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), popup->m_color, 0, 0, 0);
    }
    if (UIWindow* popupWindow = g_windowMgr.EnsureWindow(WID_BALLOONOPENBTN)) {
      popupWindow->SendMsg(nullptr, UM_COMMAND, 510, popupId, 0, 0);
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

  if (StateFlag01(actor->GetStateFlags()) || StateFlag02(actor->GetStateFlags())) {
    int effectId;
    switch (actor->GetEffectiveJob(-1)) {
    case JT_GUNSLINGER:
    case JT_REBELLION:
    case JT_GUNSLINGER_B:
    case JT_REBELLION_B:
    case JT_NIGHT_WATCH:
      effectId = 629;
      break;
    case JT_MONK_B:
      effectId = 504;
      break;
    default:
      effectId = 228;
      break;
    }

    actor->SendMsg(nullptr, AM_DELETE_EFFECT, static_cast<std::uint32_t>(effectId), 0, 0, 0, 0, 0);
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
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&count), 0, 0, 0, 0);
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
  case JT_DRAGON_KNIGHT:
  case JT_DRAGON_KNIGHT_RIDING:
  case JT_DRAGON_KNIGHT_CHICKEN:
    break;
  case JT_ABYSS_CHASER:
    if (0 == count) {
      g_effectMgr->Retire(actor->render.accountId, 2109);
      return;
    }
    break;
  case JT_GUNSLINGER:
  case JT_GUNSLINGER_B:
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 629, 0, 0, 0, 0, 0);
    if (count == 0) {
      return;
    }
    break;
  case JT_MONK_B:
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 504, 0, 0, 0, 0, 0);
    if (count == 0) {
      return;
    }
    break;
  case JT_REBELLION:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    g_effectMgr->Retire(actor->render.accountId, 1135);
    if (count == 0) {
      return;
    }
    break;
  default:
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 228, 0, 0, 0, 0, 0);
    if (count == 0) {
      return;
    }
    break;
  }

  if (StateFlag01(actor->GetStateFlags()) || StateFlag02(actor->GetStateFlags())) {
    switch (job) {
    case JT_DRAGON_KNIGHT:
    case JT_DRAGON_KNIGHT_RIDING:
    case JT_DRAGON_KNIGHT_CHICKEN:
      g_effectMgr->Retire(g_effectMgr->Find(actor->render.accountId, 2460), 1);
      return;
    case JT_ABYSS_CHASER:
      g_effectMgr->Retire(actor->render.accountId, 2109);
      break;
    }
  }

  CRagEffect* effect;
  switch (job) {
  case JT_GUNSLINGER:
  case JT_GUNSLINGER_B:
    effect = actor->LaunchEffect(629, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case JT_MONK_B:
    effect = actor->LaunchEffect(504, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case JT_REBELLION:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH: {
    CEffect* coins = g_effectMgr->Spawn(actor->render.accountId, 1135, actor->render.position.x, actor->render.position.y,
                                        actor->render.position.z, 0, -1, 0, 0);
    if (!coins) {
      return;
    }
    static_cast<CRichsCoinEffect*>(coins)->coinCount = count;
    coins->Open();
    return;
  }
  case JT_DRAGON_KNIGHT:
  case JT_DRAGON_KNIGHT_RIDING:
  case JT_DRAGON_KNIGHT_CHICKEN: {
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
  case JT_ABYSS_CHASER: {
    CEffect* abyss = g_effectMgr->Find(actor->render.accountId, 2109);
    if (!abyss) {
      abyss = g_effectMgr->Spawn(actor->render.accountId, 2109, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    if (abyss) {
      static_cast<CFromTheAbyssEffect*>(abyss)->SetLevel(count);
    }
    auto it = std::find_if(g_activeSkillEffects.begin(), g_activeSkillEffects.end(), [](const ActiveSkillEffect& effect) { return effect.id == 1244; });
    if (it != g_activeSkillEffects.end()) {
      it->value1 = count;
    }
    return;
  }
  default:
    effect = actor->LaunchEffect(228, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }

  if (effect && (effect->state.type == 228 || effect->state.type == 504 || effect->state.type == 629)) {
    vector3d value{static_cast<float>(count), 0.0f, 0.0f};
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&value), 0, 0, 0, 0);
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
  lists->ClearMerchantMirrorItems();
  lists->ClearMerchantHopeToSellItems();

  const BuyingStoreItemRecord* record = reinterpret_cast<const BuyingStoreItemRecord*>(packet + 16);
  const unsigned count = (static_cast<int>(*reinterpret_cast<const std::int16_t*>(packet + 2)) - 16u) / 11;
  for (int index = 0; index < static_cast<int>(count);) {
    const BuyingStoreItemRecord entry = *record;
    ITEM_INFO item;
    ++index;
    item.prices[0] = entry.price;
    item.prices[1] = entry.price;
    item.identity[0] = entry.type;
    item.quantity = entry.count;
    item.identified = 1;
    item.identity[1] = index;
    item.SetItemId(entry.itemId);
    lists->AddMerchantMirrorItem(item);
    ++record;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMSHOPWND_MODE_1);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  window->SendMsg(nullptr, UM_SETNAID, *reinterpret_cast<const std::int32_t*>(packet + 4), 0, 0, 0);
  window->SendMsg(nullptr, UM_SET_BUYING_SHOP_ZENY, *reinterpret_cast<const std::int32_t*>(packet + 12), 0, 0, 0);

  window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMPURCHASEWND_MODE_1);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  window->SendMsg(nullptr, UM_SETNAID, *reinterpret_cast<const std::int32_t*>(packet + 4), *reinterpret_cast<const std::int32_t*>(packet + 8), 0, 0);

  window = reinterpret_cast<UIWindow*>(g_windowMgr.purchaseMirrorItemWnd);
  if (!window) {
    window = g_windowMgr.EnsureWindow(WID_MERCHANTMIRRORITEMWND_MODE_2);
  } else {
    g_session.RefreshMerchantMyShopItems();
  }
  if (window) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  this->isOnQuest = 1;
}

// OFFSET: 0x00D5F3E0
// FUNCTION: fn_21969
void CGameMode::SetActorNameCard(std::uint32_t id, const char* name, const char* value1, const char* value2, const char* value3, int job) {
  if (id == g_session.m_aid) {
    g_session.jobTitle = job;
  }

  std::string jobTitle = GetJobTitleString(job);
  this->nameCard.Set(id, name, value1, value2, value3, jobTitle, std::string(""), 0);
}

// OFFSET: 0x00D79BB0
// FUNCTION: fn_22180
void CGameMode::ReceiveShopName(const char* text, int shopId) {
  std::string name = text;
  name.resize(name.size() - 4);
  if (!g_session.AddShopName(name, shopId)) {
    ValuePacket packet;
    packet.id = ID_CZ_RES_LISTOFMODIFIED_ITEMNSKILL;
    packet.value = shopId;
    GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
  }
}

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

  g_session.RefreshMessengerGroupWindow();
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
  unsigned char padding_0004[0x1F8];
};
}

// OFFSET: 0x00D4CAD0
// FUNCTION: fn_21869
void CGameMode::ReceiveStatusBonus(const unsigned char* data) {
  const StatusBonusPacket* packet = reinterpret_cast<const StatusBonusPacket*>(data);
  int* traitBonus = g_session.m_traits.bonus;
  switch (packet->type) {
  case SP_STR:
    g_session.m_standardStats[0] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.strengthBonus = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_AGI:
    g_session.m_standardStats[1] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.agilityBonus = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_VIT:
    g_session.m_standardStats[2] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.vitalityBonus = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_INT:
    g_session.m_standardStats[3] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.intelligenceBonus = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_DEX:
    g_session.m_standardStats[4] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.dexterityBonus = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_LUK:
    g_session.m_standardStats[5] = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.luckBonus = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_POW:
    g_session.SetTraitStat(219, packet->value);
    traitBonus[0] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_STA:
    g_session.SetTraitStat(220, packet->value);
    traitBonus[1] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_WIS:
    g_session.SetTraitStat(221, packet->value);
    traitBonus[2] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_SPL:
    g_session.SetTraitStat(222, packet->value);
    traitBonus[3] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_CON:
    g_session.SetTraitStat(223, packet->value);
    traitBonus[4] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  case SP_CRT:
    g_session.SetTraitStat(224, packet->value);
    traitBonus[5] = packet->bonus;
    g_session.InvalidateWindowFrames();
    break;
  }

  switch (packet->type) {
  case SP_STR:
  case SP_AGI:
  case SP_VIT:
  case SP_INT:
  case SP_DEX:
  case SP_LUK:
  case SP_POW:
  case SP_STA:
  case SP_WIS:
  case SP_SPL:
  case SP_CON:
  case SP_CRT:
    if (g_equipmentPropertiesMgr == nullptr) {
      g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
    }
    g_equipmentPropertiesMgr->RefreshPretend();
    break;
  }
}

// OFFSET: 0x00D8A600
// FUNCTION: fn_22220
__declspec(noinline) void CGameMode::ReceiveSwapEquipmentList(const unsigned char* data) {
  g_session.ResetSwapCostumeItems();
  g_session.ResetSwapEquippedItems();

  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(SwapEquipmentRecord);
  const SwapEquipmentRecord* records = reinterpret_cast<const SwapEquipmentRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const SwapEquipmentRecord* record = &records[i];
    ITEM_INFO item = g_session.FindItemByIdentity(record->index, record->location);
    item.identity[3] = record->location;
    if (item.identity[1] <= 0) {
      break;
    }

    if (g_session.IsCostumeLocation(record->location)) {
      g_session.EquipSwapEquipmentItem(item);
    } else {
      g_session.EquipSwapCostumeItem(item);
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
    property = *reinterpret_cast<const int*>(packet + 0x10);
    delay = *reinterpret_cast<const int*>(packet + 0x14);
    skillId = *reinterpret_cast<const std::uint16_t*>(packet + 0xE);
    x = *reinterpret_cast<const std::uint16_t*>(packet + 0xA);
    y = *reinterpret_cast<const std::uint16_t*>(packet + 0xC);
  } else if (type == 1) {
    sourceAid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
    targetAid = *reinterpret_cast<const std::uint32_t*>(packet + 6);
    property = *reinterpret_cast<const int*>(packet + 0x10);
    delay = *reinterpret_cast<const int*>(packet + 0x14);
    skillId = *reinterpret_cast<const std::uint16_t*>(packet + 0xE);
    x = *reinterpret_cast<const std::uint16_t*>(packet + 0xA);
    y = *reinterpret_cast<const std::uint16_t*>(packet + 0xC);
    flag = packet[0x18];
  }

  this->NotifySkillCasting(sourceAid, targetAid, x, y, skillId, property, delay, flag, 0);
}

namespace {
#pragma pack(push, 1)
  struct GuildMemberRecord {
    /* 0000 */ int accountId;
    /* 0004 */ int characterId;
    /* 0008 */ std::int16_t head;
    /* 000A */ std::int16_t headPalette;
    /* 000C */ std::int16_t sex;
    /* 000E */ std::int16_t job;
    /* 0010 */ std::int16_t level;
    /* 0012 */ int exp;
    /* 0016 */ int online;
    /* 001A */ int positionId;
    /* 001E */ int lastTimeOffline;
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
  guildInfo->memberCount = count;
  guildInfo->ClearMembers();

  const GuildMemberRecord* records = reinterpret_cast<const GuildMemberRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const GuildMemberRecord* record = &records[i];
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.characterId = record->characterId;
    member.head = record->head;
    member.headPalette = record->headPalette;
    member.job = record->job;
    member.sex = record->sex;
    member.positionId = record->positionId;
    member.level = record->level;
    member.exp = record->exp;
    member.online = record->online;
    member.expRatio = 0.0f;
    *reinterpret_cast<int*>(member.lastOfflineTime) = record->lastTimeOffline;
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
    /* 0008 */ std::int16_t head;
    /* 000A */ std::int16_t headPalette;
    /* 000C */ std::int16_t sex;
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
  guildInfo->memberCount = count;
  guildInfo->ClearMembers();

  const GuildMemberMemoRecord* records = reinterpret_cast<const GuildMemberMemoRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const GuildMemberMemoRecord* record = &records[i];
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.characterId = record->characterId;
    member.head = record->head;
    member.headPalette = record->headPalette;
    member.job = record->job;
    member.sex = record->sex;
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
    member.memo = memo;
    member.positionName.clear();
    guildInfo->AddMember(member);
  }

  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D3A630
// FUNCTION: fn_21778
__declspec(noinline) void CGameMode::ReceiveOpenStore(const unsigned char* packet) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    const ModeDragState* drag = &mode->drag;
    if (drag && drag->source) {
      this->SendMsg(MM_REQ_CLOSESTORE, 0, 0, 0, 0);
      return;
    }
  }

  const unsigned int count = packet[2];
  int& storeCount = *reinterpret_cast<int*>(&g_session.storeSlotCounts[4]);
  if (count < 2) {
    storeCount = 2;
  } else {
    storeCount = count > 5 ? 5 : count;
  }
  g_session.ClearMerchantPurchaseSavedItems();
  if (!g_windowMgr.chatRoomWnd && !g_windowMgr.chatRoomMakeWnd && !g_windowMgr.merchantShopMakeWnd) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTSHOPMAKEWND_MODE_1)) {
      static_cast<UIMerchantShopMakeWnd*>(window)->AddActionButton();
    }
    return;
  }

  g_windowMgr.Confirm(LookupMessage(MSI_CANT_OPEN_STORE_WHILE_CHAT), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(MM_REQ_CLOSE_BUYING_STORE, 0, 0, 0, 0);
}

struct ACTSTRUCT {
  virtual ~ACTSTRUCT();

  /* 0004 */ int damage;
  /* 0008 */ int damage2;
  /* 000C */ int type;
  /* 0010 */ std::uint32_t targetId;
  /* 0014 */ std::uint32_t tick;
  /* 0018 */ int count;
  /* 001C */ int attackMotion;
  /* 0020 */ int damageMotion;
  /* 0024 */ int attackMT;
  /* 0028 */ int attackedMT;
  /* 002C */ bool spDamaged;
};

// OFFSET: 0x00D051F0
// FUNCTION: fn_21563
ACTSTRUCT::~ACTSTRUCT() = default;
// OFFSET: 0x00D3A770
// FUNCTION: fn_21779
__declspec(noinline) void CGameMode::ReceiveSearchStoreOpen(const unsigned char* packet) {
  g_session.storeSearchCount = packet[4];
  reinterpret_cast<std::int16_t*>(&g_session.storeSearchPageFlags)[1] = *reinterpret_cast<const std::int16_t*>(packet + 2);
  g_windowMgr.EnsureWindow(WID_SEARCH_STORE_INFO);
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
  int* derivedStats = reinterpret_cast<int*>(g_session.combatStats);
  g_modeMgr.GetGameMode();
  std::string text;

  switch (packet->type) {
  case SP_SPEED:
    if (*reinterpret_cast<int*>(&g_session.healthPoints[0]) != packet->value) {
      *reinterpret_cast<int*>(&g_session.healthPoints[0]) = packet->value;
      g_session.InvalidateWindowFrames();
    }
    break;
  case SP_KARMA:
    *reinterpret_cast<int*>(&g_session.optionFlags[0]) = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MANNER:
    if (packet->value < 0) {
      text = FormatOwnedString(LookupMessage(MSI_BAD_MANNER_REMAIN), -packet->value);
      const int emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::ChatProhibit);
      this->scene->player->SendMsg(nullptr, AM_CASH_EMOTION, 0, emotion, 0, 0, 0, 0);
    } else {
      text = LookupMessage(MSI_BAD_MANNER_END);
      this->scene->player->SendMsg(nullptr, AM_DELETE_PROHIBITEFFECT, 0, 0, 0, 0, 0, 0);
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
    *reinterpret_cast<int*>(&g_session.healthPoints[4]) = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_UNBREAKABLE:
    if (packet->value < -60) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PK_HONOR)), 0xFFFF, 0, 0, 0);
    }
    break;
  case SP_HP:
    if (g_session.maxHp > 0 && (packet->value * 100) / g_session.maxHp < 25 &&
        this->scene->GetGameActorByAID(g_session.petGid) && rand() % 100 < 50 &&
        g_session.CheckInterval(5000) == true) {
      const int hungerLevel = g_session.GetPetHungryState(g_session.petHunger);
      const int friendlyLevel = g_session.GetPetFriendlyState(g_session.m_petFriendly);
      this->SendMsg(MM_PET_ACT, g_session.GetPetEmotionNumber(g_session.petJob, 2, hungerLevel), 0, 0, 0);
      this->SendMsg(MM_PET_ACT, g_session.GetPetActionNumber(hungerLevel, friendlyLevel, 2), 0, 0, 0);
    }
    if (g_session.hp != packet->value) {
      g_session.hp = packet->value;
      g_session.RefreshHpSpWindows();
    }
    break;
  case SP_MAXHP:
    if (g_session.maxHp != packet->value) {
      g_session.maxHp = packet->value;
      g_session.RefreshHpSpWindows();
    }
    if (g_windowMgr.messengerGroupWnd) {
      g_windowMgr.messengerGroupWnd->SendMsg(nullptr, UM_COMMAND, 34, g_session.m_aid, 0, 0);
    }
    break;
  case SP_SP:
    if (*reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0]) != packet->value) {
      *reinterpret_cast<int*>(&g_session.spAndHomunculusStats[0]) = packet->value;
      g_session.RefreshHpSpWindows();
    }
    break;
  case SP_MAXSP:
    if (*reinterpret_cast<int*>(&g_session.spAndHomunculusStats[4]) != packet->value) {
      *reinterpret_cast<int*>(&g_session.spAndHomunculusStats[4]) = packet->value;
      g_session.RefreshHpSpWindows();
    }
    break;
  case SP_STATUSPOINT:
    *reinterpret_cast<int*>(&g_session.statusPoints[0]) = packet->value;
    g_session.InvalidateWindowFrames();
    if (g_windowMgr.statusWnd) {
      g_windowMgr.statusWnd->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case SP_BASELEVEL: {
    g_session.m_level = packet->value;
    g_session.InvalidateWindowFrames();
    this->scene->player->characterLevel = static_cast<std::uint16_t>(g_session.m_level);
    g_soundMgr->PlayWave("levelup.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);

    int effectId;
    if (g_session.IsSuperNoviceClass(g_session.GetDisplayJob())) {
      effectId = 338;
    } else {
      switch (g_session.GetDisplayJob()) {
      case JT_TAEKWON:
      case JT_STAR:
      case JT_STAR2:
      case JT_LINKER:
      case JT_TAEKWON_B:
      case JT_STAR_B:
      case JT_LINKER_B:
      case JT_STAR2_B:
      case JT_STAR_EMPEROR:
      case JT_SOUL_REAPER:
      case JT_STAR_EMPEROR_B:
      case JT_SOUL_REAPER_B:
      case JT_STAR2_EMPEROR:
      case JT_STAR2_EMPEROR_B:
      case JT_SKY_EMPEROR:
      case JT_SOUL_ASCETIC:
      case JT_SKY_EMPEROR2:
        effectId = 582;
        break;
      case JT_GANGSI:
      case JT_DEATHKNIGHT:
      case JT_COLLECTOR:
        effectId = 338;
        break;
      default:
        effectId = 371;
        break;
      }
    }
    this->scene->player->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);

    if (g_replayAnimationScaling == 0) {
      g_windowMgr.EnsureWindow(WID_NOTIFYLEVELUPWND);
    }
    if (this->scene->GetGameActorByAID(g_session.petGid)) {
      const int hungerLevel = g_session.GetPetHungryState(g_session.petHunger);
      const int friendlyLevel = g_session.GetPetFriendlyState(g_session.m_petFriendly);
      this->SendMsg(MM_PET_ACT, g_session.GetPetEmotionNumber(g_session.petJob, 6, hungerLevel), 0, 0, 0);
      this->SendMsg(MM_PET_ACT, g_session.GetPetActionNumber(hungerLevel, friendlyLevel, 6), 0, 0, 0);
    }
    if (g_replayAnimationScaling == 0) {
      if (g_windowMgr.basicInfoWnd) {
        reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->SetHeight(g_windowMgr.basicInfoWnd->geometry.height);
      }
    }
    this->RefreshActorJobEffects(true);
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 192, 1, 0);
    }
    break;
  }
  case SP_JOBLEVEL:
    g_session.m_jobLevel = packet->value;
    g_session.InvalidateWindowFrames();
    this->scene->player->LaunchEffect(packet->value > 50 ? 337 : 158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);

    if (g_replayAnimationScaling == 0) {
      g_windowMgr.EnsureWindow(WID_NOTIFYJOBLEVELUPWND);
      if (g_windowMgr.basicInfoWnd) {
        reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->SetHeight(g_windowMgr.basicInfoWnd->geometry.height);
      }
    }
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 196, 1, 0);
    }
    break;
  case SP_SKILLPOINT:
    g_session.m_skillPoint = packet->value;
    g_session.InvalidateWindowFrames();
    g_session.NotifySkillWindows();
    break;
  case SP_STR:
    g_session.m_standardStats[0] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_AGI:
    g_session.m_standardStats[1] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_VIT:
    g_session.m_standardStats[2] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_INT:
    g_session.m_standardStats[3] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_DEX:
    g_session.m_standardStats[4] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_LUK:
    g_session.m_standardStats[5] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_WEIGHT: {
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
      if (percent >= g_overweightThresholdPercent && oldPercent < g_overweightThresholdPercent) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_USA_LOGINERRORMSG_UNKNOWN), g_overweightThresholdPercent).c_str()), 0xFF, 8, 0, 0);
      }
      if (percent >= 90 && oldPercent < 90) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_ATTACK_OVERWEIGHT)), 0xFF, 8, 0, 0);
      }
      break;
    }
    if (oldPercent <= percent) {
      break;
    }
    if (percent < g_overweightThresholdPercent && g_overweightThresholdPercent <= oldPercent) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_RECOVER_OVERWEIGHT_RELEASED)), 0xFFFF00, 8, 0, 0);
    }
    if (percent < 90 && oldPercent >= 90) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_ATTACK_OVERWEIGHT_RELEASED)), 0xFFFF00, 8, 0, 0);
    }
    break;
  }
  case SP_MAXWEIGHT: {
    packet->value = packet->value / 10;
    const int oldMaxWeight = g_session.m_maxWeight;
    const int oldPercent = (g_session.m_weight * 100) / (oldMaxWeight < 1 ? 1 : oldMaxWeight);
    g_session.m_maxWeight = packet->value;
    g_session.RefreshHpSpWindows();
    g_session.InvalidateWindowFrames();
    if (oldMaxWeight == 0 || oldMaxWeight >= g_session.m_maxWeight) {
      break;
    }

    const int percent = (g_session.m_weight * 100) / (g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight);
    if (percent < 50) {
      if (oldPercent >= 50) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_RECOVER_OVERWEIGHT_RELEASED)), 0xFFFF00, 8, 0, 0);
      }
    } else if (percent >= 90) {
      break;
    }
    if (oldPercent >= 90) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_ATTACK_OVERWEIGHT_RELEASED)), 0xFFFF00, 8, 0, 0);
    }
    break;
  }
  case SP_ATK1:
    derivedStats[7] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_ATK2:
    derivedStats[12] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MATK1:
    derivedStats[13] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MATK2:
    derivedStats[14] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_DEF1:
    derivedStats[10] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_DEF2:
    derivedStats[11] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MDEF1:
    derivedStats[8] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_MDEF2:
    derivedStats[15] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_HIT:
    derivedStats[16] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_FLEE1:
    derivedStats[17] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_FLEE2:
    derivedStats[19] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_CRITICAL:
    derivedStats[18] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_ASPD:
    derivedStats[6] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_36:
    derivedStats[9] = packet->value;
    g_session.InvalidateWindowFrames();
    break;
  case SP_POW:
  case SP_STA:
  case SP_WIS:
  case SP_SPL:
  case SP_CON:
  case SP_CRT:
  case SP_PATK:
  case SP_SMATK:
  case SP_RES:
  case SP_MRES:
  case SP_HPLUS:
  case SP_CRATE:
  case SP_TRAITPOINT:
  case SP_AP:
  case SP_MAXAP:
    g_session.SetTraitStat(packet->type, packet->value);
    if (g_windowMgr.statusWnd) {
      static_cast<UIStatusWnd*>(g_windowMgr.statusWnd)->ValidatePlacement();
    }
    g_session.InvalidateWindowFrames();
    break;
  case 169:
    reinterpret_cast<bool&>(g_session.feedOptions[4]) = packet->value != 0;
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

  if (type == EFST_WEAPONPROPERTY) {
    type = value1 + 897;
  }

  if (aid != g_session.m_aid) {
    CRenderObject* actor = this->scene->GetGameActorByAID(aid);
    if (actor) {
      actor->SendMsg(nullptr, AM_STATUS_CHANGE, type, on, value1, value2, flag, value3);
    }

    if (type == EFST_SPELLFIST && actor) {
      const unsigned char element = static_cast<unsigned char>(value3);
      if (element == 19) {
        g_effectMgr->Spawn(actor->render.accountId, 1665, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1666, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (element == 20) {
        g_effectMgr->Spawn(actor->render.accountId, 1669, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (element == 14) {
        g_effectMgr->Spawn(actor->render.accountId, 1667, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1668, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    std::sprintf(name, LookupMessage(MSI_CHARACTER_NAME), member.name.c_str());
  }

  char message[256];
  std::memset(message, 0, sizeof(message));

  std::uint32_t color;
  if (on) {
    if (aid == g_session.m_aid) {
      if (type == EFST_CLAN_INFO) {
        if (this->scene->player) {
          this->scene->player->SetSpGauge(1, value2);
        }
      } else if (type == EFST_POSTDELAY) {
        ShortcutCooldownRecord timer;
        timer.id = 577;
        timer.end = now + remaining;
        timer.duration = total;
        g_session.m_requestTimers.push_back(timer);
      } else if (type == EFST_HOMUN_SKILL_POSTDELAY) {
        ShortcutCooldownRecord timer;
        timer.id = 9999;
        timer.end = now + remaining;
        timer.duration = total;
        g_session.m_requestTimers.push_back(timer);
      } else if (type == EFST_GDSKILL_POSTDELAY) {
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
      } else if (type == EFST_DAILYSENDMAILCNT) {
        g_rodexMailStore->dailySendCount = value2;
      }

      if (type != EFST_DAILYSENDMAILCNT) {
        ActiveSkillEffect effect;
        effect.id = type;
        effect.expiration = remaining == 9999 ? 999999 : now + remaining;
        if (type >= 363 && type <= 367) {
          effect.value1 = value1 & 3;
        } else if (type == EFST_ROLLINGCUTTER || type == EFST_HEAD_EQUIPMENT_EFFECT || type == EFST_MADOGEAR_TYPE) {
          effect.value1 = value1;
        }

        if (type == EFST_CLIMAX || type == EFST_E_SLASH_COUNT) {
          effect.value1 = value1;
        } else if (type == EFST_ABYSSFORCEWEAPON) {
          effect.value1 = value2;
        } else if (CSkillActionMgr::GetInstance()->HasEfstAction(type) == true) {
          effect.value2 = value2;
          effect.value1 = value1;
          effect.value3 = value3;
        }

        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == type && (it->id != EFST_HEAD_EQUIPMENT_EFFECT || it->value1 == effect.value1)) {
            g_cooldownOwner->Remove(it->id);
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }

        effect.totalDuration = total;
        effect.remainingDuration = remaining;
        if (effect.expiration != 999999) {
          g_cooldownOwner->Register(effect.id, total, now, total - remaining);
        }
        g_activeSkillEffects.push_back(effect);
      }
    }

    color = 0xFFFF00;
    switch (type) {
    case EFST_TWOHANDQUICKEN:
    case EFST_ATTHASTE_POTION1:
    case EFST_ATTHASTE_POTION2:
    case EFST_ATTHASTE_POTION3:
    case EFST_ATTHASTE_INFINITY:
    case EFST_SPEARQUICKEN:
    case EFST_ONEHANDQUICKEN:
      std::sprintf(message, LookupMessage(MSI_INCATTACKSPEED));
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
    case EFST_SUMMON1:
    case EFST_SUMMON2:
    case EFST_SUMMON3:
    case EFST_SUMMON4:
    case EFST_SUMMON5:
    case EFST_SATURDAY_NIGHT_FEVER:
    case EFST_MELODYOFSINK:
    case EFST_BEYOND_OF_WARCRY:
    case EFST_UNLIMITED_HUMMING_VOICE:
    case EFST_KO_JYUMONJIKIRI:
    case EFST_MEIKYOUSISUI:
    case EFST_KYOMU:
    case EFST_KAGEMUSYA:
    case EFST_ZANGETSU:
    case EFST_GENSOU:
    case EFST_AKAITSUKI:
    case EFST_HAT_EFFECT:
    case EFST_OFFERTORIUM:
    case EFST_TELEKINESIS_INTENSE:
    case EFST_MOONSTAR:
    case EFST_STRANGELIGHTS:
    case EFST_FULL_THROTTLE:
    case EFST_REBOUND:
    case EFST_SUPER_STAR:
    case EFST_DARKCROW:
    case EFST_ILLUSIONDOPING:
    case EFST_WIDEWEB:
    case EFST_CHILL:
    case EFST_BURNT:
    case EFST_C_MARKER:
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
    case EFST_SOUND_OF_DESTRUCTION:
    case EFST_EARTHSHAKER:
    case EFST_MISTY_FROST:
    case EFST_MAGIC_POISON:
    case EFST_PRISON:
    case EFST_DEADLY_DEFEASANCE:
    case EFST_HELLS_PLANT_ARMOR:
    case EFST_RELIEVE_DAMAGE:
    case EFST_LOCKON_LASER:
    case EFST_DRAGONIC_AURA:
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
    case EFST_SOUNDBLEND:
    case EFST_CLOUD_POISON:
    case EFST_SPORE_EXPLOSION_DEBUFF:
    case EFST_CURSE_R_CUBE:
    case EFST_CURSE_B_CUBE:
    case EFST_KILLING_AURA:
    case EFST_BLOCK:
      break;
    case EFST_ROLLINGCUTTER:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        for (int i = 0; i < 10; ++i) {
          g_effectMgr->Retire(player->render.accountId, 1300 + i);
          g_effectMgr->Retire(player->render.accountId, 1467 + i);
        }
        g_effectMgr->Spawn(
          player->render.accountId, g_serviceOptions.Get(229) ? value1 + 1466 : value1 + 1299, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
        );
        std::sprintf(message, "");
      }
      break;
    case EFST_SIREN:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, AM_DELETE_EFFECT, 879, 0, 0, 0, 0, 0);
        player->LaunchEffect(879, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case EFST_SONG_OF_MANA:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, AM_DELETE_EFFECT, 865, 0, 0, 0, 0, 0);
        player->LaunchEffect(865, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case EFST_DANCE_WITH_WUG:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, AM_DELETE_EFFECT, 867, 0, 0, 0, 0, 0);
        player->LaunchEffect(867, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case EFST_LERADS_DEW:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        player->SendMsg(nullptr, AM_DELETE_EFFECT, 871, 0, 0, 0, 0, 0);
        player->LaunchEffect(871, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
      break;
    case EFST_DEEP_SLEEP:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_MAKE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0);
        std::sprintf(message, "");
      }
      break;
    case EFST_NETHERWORLD:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 453, 0, 0, 0, 0, 0);
        std::sprintf(message, "");
      }
      break;
    case EFST_WUGDASH:
      if (aid == g_session.m_aid) {
        this->scene->player->LaunchEffect(752, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        std::sprintf(message, "");
      }
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
      if (aid == g_session.m_aid) {
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
      if (aid == g_session.m_aid) {
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
    case EFST_RECOGNIZEDSPELL:
    case EFST_WUGRIDER:
    case EFST_CAMOUFLAGE:
    case EFST_ACCELERATION:
    case EFST_HOVERING:
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
    case EFST_CLOUD_KILL:
    case EFST_RUSH_WINDMILL:
    case EFST_ECHOSONG:
    case EFST_HARMONIZE:
    case EFST_STRIKING:
    case EFST_WARMER:
    case EFST_MOONLIT_SERENADE:
    case EFST_ANALYZE:
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
    case EFST_MARSHOFABYSS:
      if (aid == g_session.m_aid) {
        this->scene->player->LaunchEffect(729, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
      std::printf(message, "");
      break;
    case EFST_DA_ILLUSION:
      if (aid == g_session.m_aid) {
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
    case EFST_CASH_PLUSEXP:
    {
      const int minutes = remaining / 60000;
      if (minutes > 0) {
        const std::string text = FormatOwnedString(LookupMessage(MSI_PLUSEXP_VALUE), minutes, value1);
        std::strcpy(message, text.c_str());
      } else {
        message[0] = '\0';
      }
      break;
    }
    case EFST_CASH_DEATHPENALTY:
      std::sprintf(message, LookupMessage(MSI_DDEATHPENALTY));
      break;
    case EFST_CASH_RECEIVEITEM:
    {
      const int minutes = remaining / 60000;
      if (minutes > 0) {
        const int hours = minutes / 60;
        if (hours > 0) {
          const std::string text = FormatOwnedString(LookupMessage(MSI_WARNING_PRICE3), hours, minutes % 60, value1);
          std::strcpy(message, text.c_str());
        } else {
          const std::string text = FormatOwnedString(LookupMessage(MSI_WARNING_PRICE2), minutes, value1);
          std::strcpy(message, text.c_str());
        }
      } else {
        message[0] = '\0';
      }
      break;
    }
    case EFST_CASH_BOSS_ALARM:
      if (aid == g_session.m_aid) {
        this->bossAlarmAndCopyNameFlags[0] = 1;
      }
      std::sprintf(message, LookupMessage(MSI_BOSS_ALARM));
      break;
    case EFST_CASH_PLUSONLYJOBEXP:
    {
      const int minutes = remaining / 60000;
      if (minutes > 0) {
        const std::string text = FormatOwnedString(LookupMessage(MSI_BANK_WARNING), minutes, value1);
        std::strcpy(message, text.c_str());
      } else {
        message[0] = '\0';
      }
      break;
    }
    case EFST_REWARD_PLUSONLYJOBEXP:
    {
      const std::string text = FormatOwnedString(LookupMessage(MSI_PLUSONLYJOBEXP2), value2, value1 / 100.0 + 1.0);
      std::sprintf(message, text.c_str());
      break;
    }
    case EFST_MONSTER_TRANSFORM:
    case EFST_KYOUGAKU:
    case EFST_ACTIVE_MONSTER_TRANSFORM:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_STATUS_CHANGE, type, on, value1, 0, 0, 0);
        this->RefreshStatusStateEffect(aid);
      }
      return;
    case EFST_ON_PUSH_CART:
      if (aid == g_session.m_aid) {
        g_session.SetReplayPacketMode(value1);
      }
      break;
    case EFST_FLOWER_LEAF:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_STATUS_CHANGE, 675, on, value1, 0, 0, 0);
        this->RefreshStatusStateEffect(aid);
      }
      return;
    case EFST_RAY_OF_PROTECTION:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_STATUS_CHANGE, 676, on, value1, 0, 0, 0);
        this->RefreshStatusStateEffect(aid);
      }
      break;
    case EFST_HANDICAPSTATE_ICEEXPLO:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_STATUS_CHANGE, 710, on, value1, 0, 0, 0);
        this->RefreshStatusStateEffect(aid);
      }
      break;
    case EFST_SUHIDE:
      if (aid == g_session.m_aid) {
        this->scene->player->SetState(48, 1);
        this->scene->player->render.isMotionFreezed = true;
      }
      break;
    case EFST_SU_STOOP:
      if (aid == g_session.m_aid) {
        this->scene->GetGameActorByAID(aid);
        this->scene->player->SetState(47, 1);
      }
      break;
    case EFST_OVERHEAT:
      g_session.nameValidationTick = timeGetTime();
      g_session.minimumNameLength = value2 == 0 ? value1 : 0;
      if (value1 == 0) {
        for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end();) {
          if (it->id == EFST_OVERHEAT) {
            g_activeSkillEffects.erase(it);
            it = g_activeSkillEffects.begin();
          } else {
            ++it;
          }
        }
      }
      break;
    case EFST_ALL_STAT_DOWN:
      color = 0xFF;
      std::sprintf(message, LookupMessage(MSI_ACCOUNTCLASS_INTEGRATION));
      break;
    case EFST_SOULCURSE:
      g_effectMgr->Spawn(aid, 1362, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\s포이즌버스터.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      break;
    case EFST_MADOGEAR_TYPE:
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
        actor->SetMadogearType(2);
        if (aid == g_session.m_aid) {
          g_session.madogearType = 2;
        }
      } else {
        actor->SetMadogearType(1);
        if (aid == g_session.m_aid) {
          g_session.madogearType = 1;
        }
      }
      actor->SendMsg(nullptr, AM_MAKE_CHICKEN, 0, 0, 0, 0, 0, 0);
      break;
    }
    case EFST_SPELLFIST:
    {
      CRenderObject* actor = this->scene->GetGameActorByAidIncludeMe(aid);
      if (!actor) {
        break;
      }

      const unsigned char element = static_cast<unsigned char>(value3);
      if (element == 19) {
        g_effectMgr->Spawn(actor->render.accountId, 1665, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1666, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (element == 20) {
        g_effectMgr->Spawn(actor->render.accountId, 1669, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      } else if (element == 14) {
        g_effectMgr->Spawn(actor->render.accountId, 1667, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
        g_effectMgr->Spawn(actor->render.accountId, 1668, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      }
      break;
    }
    case EFST_POWERFUL_FAITH:
      _snprintf_s(message, 256, LookupMessage(MSI_IQ_FIRM_FAITH_OFF));
      break;
    case EFST_FIRM_FAITH:
      _snprintf_s(message, 256, LookupMessage(MSI_IQ_SINCERE_FAITH_OFF));
      break;
    case EFST_SINCERE_FAITH:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_BENEDICTUM_OFF));
      break;
    case EFST_BENEDICTUM:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_RELIGIO_OFF));
      break;
    case EFST_RELIGIO:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_COMPETENTIA_OFF));
      break;
    case EFST_COMPETENTIA:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_PRESENS_ACIES_OFF));
      break;
    case EFST_PRE_ACIES:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_ARGUTUS_TELUM_OFF));
      break;
    case EFST_A_TELUM:
      _snprintf_s(message, 256, LookupMessage(MSI_CD_ARGUTUS_VITA_OFF));
      break;
    case EFST_A_VITA:
      _snprintf_s(message, 256, LookupMessage(MSI_DK_SERVANTWEAPON_OFF));
      break;
    case EFST_ULTIMATE_S:
      _snprintf_s(message, 256, LookupMessage(MSI_AG_CLIMAX_OFF));
      break;
    case EFST_CLIMAX:
      _snprintf_s(message, 256, LookupMessage(MSI_SHC_SHADOW_EXCEED_OFF));
      break;
    case EFST_POTENT_VENOM:
      _snprintf_s(message, 256, LookupMessage(MSI_SHC_ENCHANTING_SHADOW_OFF));
      break;
    case EFST_PROTECTSHADOWEQUIP:
      _snprintf_s(message, 256, LookupMessage(MSI_BO_HELLTREE_OFF));
      break;
    case EFST_RESEARCHREPORT:
      _snprintf_s(message, 256, LookupMessage(MSI_BO_BIONIC_PHARMACY_FAIL));
      break;
    case EFST_BO_HELL_DUSTY:
      _snprintf_s(message, 256, LookupMessage(MSI_MT_A_MACHINE_OFF));
      break;
    case EFST_D_MACHINE:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_TR_KVASIR_SONATA_ON));
      break;
    case EFST_A_MACHINE:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_MT_D_MACHINE_OFF));
      break;
    case EFST_MUSICAL_INTERLUDE:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_TR_JAWAII_SERENADE_OFF));
      break;
    case EFST_WINDSIGN:
      color = 0xFF;
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_THAI_PERSONALINFO_DROP2));
      break;
    case EFST_ABYSS_SLAYER:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_BAN_THAI_GGT_RESTRICTED_MAKE_CHAR));
      break;
    case EFST_SPELL_ENCHANTING:
      StringFormat(message, 256, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_ABC_ABYSS_SLAYER_ON));
      break;
    default:
      if (CSkillActionMgr::GetInstance()->HasEfstAction(type) != true) {
        this->RefreshStatusStateEffect(aid);
        return;
      }
      {
        const EfstActionMessage action = CSkillActionMgr::GetInstance()->GetEfstBeginMessage(type);
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
        if (it->id == type && (it->id != EFST_HEAD_EQUIPMENT_EFFECT || it->value1 == value1)) {
          g_activeSkillEffects.erase(it);
          it = g_activeSkillEffects.begin();
        } else {
          ++it;
        }
      }
    }

    color = 0xFF;
    switch (type) {
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
    case EFST_ENCHANTPOISON:
    case EFST_ASPERSIO:
    case EFST_WEAPONPROPERTY:
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
      if (aid == g_session.m_aid) {
        this->scene->player->SetState(0, 1);
        this->scene->player->render.isMotionFreezed = false;
      }
      break;
    case EFST_SHOUT:
      std::sprintf(message, LookupMessage(MSI_DSST_SHOUT));
      break;
    case EFST_ENERGYCOAT:
      std::sprintf(message, LookupMessage(MSI_DSST_ENERGYCOAT));
      break;
    case EFST_ATTHASTE_INFINITY:
    case EFST_INC_AGI:
    case EFST_MOVHASTE_INFINITY:
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
      std::sprintf(message, LookupMessage(MSI_SPEEDDOWN));
      break;
    case EFST_MOVHASTE_POTION:
      std::sprintf(message, LookupMessage(MSI_SPEEDDOWN));
      break;
    case EFST_SUMMON1:
    case EFST_SUMMON2:
    case EFST_SUMMON3:
    case EFST_SUMMON4:
    case EFST_SUMMON5:
      if (aid != g_session.m_aid) {
        break;
      }
      {
        CPlayer* player = this->scene->player;
        const int stage = value1 & 3;
        if (stage == 0) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 735, 0, 0, 0, 0, 0);
        } else if (stage == 1) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 736, 0, 0, 0, 0, 0);
        } else if (stage == 2) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 737, 0, 0, 0, 0, 0);
        } else if (stage == 3) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 738, 0, 0, 0, 0, 0);
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
    case EFST_ANALYZE:
    case EFST_LERADS_DEW:
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
    case EFST_ROLLINGCUTTER:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        for (int i = 0; i < 10; ++i) {
          g_effectMgr->Retire(player->render.accountId, 1300 + i);
          g_effectMgr->Retire(player->render.accountId, 1467 + i);
        }
      }
      std::sprintf(message, "");
      break;
    case EFST_SIREN:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 879, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case EFST_DEEP_SLEEP:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_DELETE_SLEEP_EFFECT, 0, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case EFST_NETHERWORLD:
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_DELETE_EFFECT, 453, 0, 0, 0, 0, 0);
      }
      std::sprintf(message, "");
      break;
    case EFST_WUGDASH:
      if (aid == g_session.m_aid) {
        CPlayer* player = this->scene->player;
        while (player->HasId(752)) {
          player->SendMsg(nullptr, AM_DELETE_EFFECT, 752, 0, 0, 0, 0, 0);
        }
        std::sprintf(message, "");
      }
      break;
    case EFST_DA_ILLUSION:
      if (aid == g_session.m_aid) {
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
      if (aid == g_session.m_aid) {
        this->bossAlarmAndCopyNameFlags[0] = 0;
      }
      break;
    case EFST_SUHIDE:
      if (aid == g_session.m_aid) {
        this->scene->player->SetState(0, 1);
        this->scene->player->render.isMotionFreezed = false;
      }
      break;
    case EFST_SU_STOOP:
      if (aid == g_session.m_aid) {
        this->scene->player->SetState(0, 1);
      }
      break;
    case EFST_ALL_STAT_DOWN:
      color = 0xFFFF00;
      std::sprintf(message, LookupMessage(MSI_ACCOUNTCLASS_INTEGRATION));
      break;
    case EFST_SOULCURSE:
    {
      CEffect* effect = g_effectMgr->Find(aid, 1362);
      if (effect) {
        g_effectMgr->Retire(effect, 1);
      }
      break;
    }
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
    case EFST_SATURDAY_NIGHT_FEVER:
    case EFST_MELODYOFSINK:
    case EFST_BEYOND_OF_WARCRY:
    case EFST_UNLIMITED_HUMMING_VOICE:
    case EFST_ON_PUSH_CART:
    case EFST_HAT_EFFECT:
    case EFST_OFFERTORIUM:
    case EFST_TELEKINESIS_INTENSE:
    case EFST_MOONSTAR:
    case EFST_STRANGELIGHTS:
    case EFST_FULL_THROTTLE:
    case EFST_REBOUND:
    case EFST_SUPER_STAR:
    case EFST_DARKCROW:
    case EFST_ILLUSIONDOPING:
    case EFST_WIDEWEB:
    case EFST_CHILL:
    case EFST_BURNT:
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
    case EFST_SOUND_OF_DESTRUCTION:
    case EFST_EARTHSHAKER:
    case EFST_MISTY_FROST:
    case EFST_MAGIC_POISON:
    case EFST_MADOGEAR_TYPE:
    case EFST_HELLS_PLANT_ARMOR:
    case EFST_RELIEVE_DAMAGE:
    case EFST_LOCKON_LASER:
    case EFST_DRAGONIC_AURA:
    case EFST_CLIMAX_CRYIMP:
    case EFST_MEDIALE:
    case EFST_RELIGIO:
    case EFST_BLOCK:
      break;
    default:
      this->RefreshStatusStateEffect(aid);
      return;
    }
  }

  this->RefreshStatusStateEffect(aid);
  if (std::strlen(message) > 1) {
    char text[512];
    std::sprintf(text, "%s%s", name, message);
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), color, std::strlen(name) > 1 ? 10 : 8, 0, 0);
  }

  if (aid == g_session.m_aid) {
    this->scene->player->SendMsg(nullptr, AM_STATUS_CHANGE, type, on, value1, value2, flag, value3);
  }
}

#pragma pack(push, 1)
struct Window107Entry {
  /* 0000 */ unsigned char repairItemData[0x18];
};
#pragma pack(pop)

// OFFSET: 0x00D79540
// FUNCTION: fn_22170
__declspec(noinline) void CGameMode::ReceiveObjectsDisappear(int count, const std::uint32_t* aids) {
  for (int i = 0; i < count; ++i) {
    const std::uint32_t aid = aids[i];
    if (this->scene->RemoveItem(aid) == true) {
      this->SendMsg(MM_DELETEREGISTERPOS, aid, 0, 0, 0);
    } else if (this->scene->RemoveGroundItem(aid) == true) {
      this->RemovePendingItemPickup(aid);
    } else {
      this->scene->RemoveSkill(aid);
    }
  }
}

// OFFSET: 0x00D795B0
// FUNCTION: fn_22171
__declspec(noinline) void CGameMode::ReceiveWindow107List(const unsigned char* packet) {
  const std::int64_t count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / sizeof(Window107Entry);
  if (count == 0) {
    g_windowMgr.Confirm(LookupMessage(MSI_CANT_MAKE_ITEM), 0, true, false, 0, "Message", 280, 120, 0);
    return;
  }

  g_windowMgr.CloseWindow(WID_ITEMREPAIRWND);
  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMREPAIRWND);
  const Window107Entry* entry = reinterpret_cast<const Window107Entry*>(packet + 4);
  for (int i = 0; i < count; ++i) {
    if (window) {
      window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(entry), 0, 0, 0);
    }
    ++entry;
  }
}

// OFFSET: 0x00D39AE0
// FUNCTION: fn_21772
__declspec(noinline) void CGameMode::ReceiveActorColorChat(const unsigned char* packet) {
  char text[256];
  std::memset(text, 0, sizeof(text));
  std::memcpy(text, packet + 12, *reinterpret_cast<const std::uint16_t*>(packet + 2) - 12);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), *reinterpret_cast<const int*>(packet + 8), 0, 0, 0);

  CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4));
  if (actor != nullptr) {
    const int color = *reinterpret_cast<const int*>(packet + 8);
    actor->SendMsg(nullptr, AM_CHAT, reinterpret_cast<int>(text), color, 0, 0, 0, 0);
  }

  if (g_session.m_whisperRecord != 0) {
    this->WriteChatLog(text);
  }
}

// OFFSET: 0x00D1F400
// FUNCTION: fn_21637
__declspec(noinline) void CGameMode::ReceiveWearEquipAck(unsigned char result, std::uint16_t index, std::uint16_t look, int location) {
  *reinterpret_cast<std::uint32_t*>(&this->equipmentRequestState[0x04]) = timeGetTime() - 2000;
  switch (result) {
  case ITEM_EQUIP_ACK_OK: {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    if (item.identity[1] > 0) {
      item.sprite = look;
      item.identity[3] = location;
      g_session.OnItemEquipped(item);
    }
    break;
  }
  case ITEM_EQUIP_ACK_FAILLEVEL: {
    const char* text = LookupMessage(MSI_CAN_NOT_EQUIP_ITEM_FORBID);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 7, 0, 0);
    break;
  }
  case ITEM_EQUIP_ACK_FAIL: {
    const char* text = LookupMessage(MSI_CAN_NOT_EQUIP_ITEM);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 7, 0, 0);
    break;
  }
  }
}

// OFFSET: 0x00D32A10
// FUNCTION: fn_21747
__declspec(noinline) void CGameMode::ReceiveFormattedMessage(const unsigned char* packet) {
  char text[500];
  std::memset(text, 0, sizeof(text));
  const std::uint16_t messageId = *reinterpret_cast<const std::uint16_t*>(packet + 2);
  if (messageId == 3278) {
    char name[260];
    std::memset(name, 0, sizeof(name));
    ITEM_INFO item;
    item.SetItemId(*reinterpret_cast<const int*>(packet + 4));
    int capacity = sizeof(name);
    item.GetIdDisplayName(name, capacity, 0);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_HOMUN_SELF_FEEDING_REST), name);
  } else {
    std::sprintf(text, LookupMessage(messageId), *reinterpret_cast<const int*>(packet + 4));
  }

  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
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
__declspec(noinline) void CGameMode::ReceiveParaItemResult(unsigned char success, int count, const unsigned char* data) {
  UIParaResultWnd* window = static_cast<UIParaResultWnd*>(g_windowMgr.EnsureWindow(WID_PARARESULTWND));
  if (window == nullptr) {
    return;
  }

  const ParaResultEntry* entry = reinterpret_cast<const ParaResultEntry*>(data);
  for (int i = count; i > 0; --i, ++entry) {
    ITEM_INFO item;
    item.SetItemId(entry->itemId);
    item.quantity = entry->quantity;
    item.prices[0] = entry->location;
    item.prices[1] = entry->location;
    item.refiningLevel = entry->refine;
    std::memcpy(item.cards, entry->cards, sizeof(entry->cards));
    item.identity[1] = entry->index;
    item.identified = 1;
    window->AddItem(item);
  }

  window->SendMsg(nullptr, UM_SETTITLE, (count - 2 > 0 ? count : 0) * 32 + 120, 0, 0, 0);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  if (!success) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RULE)), 0xFF, 0, 0, 0);
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
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D78130
// FUNCTION: fn_22159
__declspec(noinline) void CGameMode::OpenBuyingStoreMake(unsigned char slotCount) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    const ModeDragState* drag = &mode->drag;
    if (drag && drag->source) {
      this->SendMsg(MM_CANCEL_BUYING_STORE_MAKE, 0, 0, 0, 0);
      return;
    }
  }

  const unsigned int count = slotCount;
  if (count < 2) {
    g_session.m_buyingStoreSlotCount = 2;
  } else {
    g_session.m_buyingStoreSlotCount = count > 5 ? 5 : count;
  }
  g_session.ClearMerchantPurchaseSavedItems();
  if (!g_windowMgr.chatRoomWnd && !g_windowMgr.chatRoomMakeWnd && !g_windowMgr.merchantShopMakeWnd) {
    g_windowMgr.EnsureWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
    if (g_windowMgr.buyingShopMakeWnd) {
      static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.buyingShopMakeWnd)->SetItems(0, nullptr);
    }
    return;
  }

  g_windowMgr.Confirm(LookupMessage(MSI_CANT_OPEN_STORE_WHILE_CHAT), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(MM_CANCEL_BUYING_STORE_MAKE, 0, 0, 0, 0);
}

// OFFSET: 0x00D78210
// FUNCTION: fn_22160
__declspec(noinline) void CGameMode::OpenVendingMake(unsigned char slotCount, int itemCount, const short* items) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode) {
    const ModeDragState* drag = &mode->drag;
    if (drag && drag->source) {
      this->SendMsg(MM_CANCEL_VENDING_MAKE, 0, 0, 0, 0);
      return;
    }
  }

  const unsigned int count = slotCount;
  if (count < 3) {
    g_session.m_vendingSlotCount = 3;
  } else {
    g_session.m_vendingSlotCount = count > 13 ? 13 : count;
  }
  g_session.ClearMerchantPurchaseSavedItems();
  if (!g_windowMgr.chatRoomWnd && !g_windowMgr.chatRoomMakeWnd) {
    g_windowMgr.EnsureWindow(WID_MERCHANTSHOPMAKEWND);
    if (g_windowMgr.merchantShopMakeWnd) {
      static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.merchantShopMakeWnd)->SetItems(itemCount, items);
    }
    return;
  }

  g_windowMgr.Confirm(LookupMessage(MSI_CANT_OPEN_STORE_WHILE_CHAT), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(MM_CANCEL_VENDING_MAKE, 0, 0, 0, 0);
}

// OFFSET: 0x00D799C0
// FUNCTION: fn_22176
__declspec(noinline) void CGameMode::ReceiveAgitNotice(unsigned char agitId, int first, int second) {
  if (!g_windowMgr.guildInfoManageWnd) {
    return;
  }

  char text[256] = {};
  const char* agitName = nullptr;
  g_session.m_hotkeyScript->Call("get_agit_name", "d>s", agitId, &agitName);
  sprintf_s(text, LookupMessage(MSI_BAN_IP_BLOCK_FOR_ZERO), agitName, second, 100, first, 100);
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
  /* 003E */ std::uint16_t type;
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
    this->scene->items.push_back(item);
  }

  const int entryAddress = reinterpret_cast<int>(&entry);
  item->SendMsg(nullptr, AM_INITSTANDENTRY, entryAddress, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D266C0
// FUNCTION: fn_21681
__declspec(noinline) void CGameMode::ReceiveFeedOption(const unsigned char* packet) {
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case CONFIG_OPEN_EQUIPMENT_WINDOW: {
    *reinterpret_cast<int*>(g_session.equipmentViewState) = *reinterpret_cast<const int*>(packet + 6);
    const char* text = LookupMessage((*reinterpret_cast<int*>(g_session.equipmentViewState) != 0) + 0x54E);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x64FFFF, 0, 0, 0);
    if (g_windowMgr.equipWnd) {
      g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    }
    break;
  }
  case CONFIG_CALL:
    g_serviceOptions.Set(231, *reinterpret_cast<const int*>(packet + 6) != 0);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage((*reinterpret_cast<const int*>(packet + 6) != 0) + 0xBA2)), 0x64FFFF, 0, 0, 0
    );
    g_windowMgr.RefreshGameSettingOption(231);
    break;
  case CONFIG_PET_AUTOFEED:
    g_session.SetPetAutoFeed(*reinterpret_cast<const int*>(packet + 6) > 0);
    break;
  case CONFIG_HOMUNCULUS_AUTOFEED:
    g_session.SetHomunculusAutoFeed(*reinterpret_cast<const int*>(packet + 6) > 0);
    break;
  case CONFIG_DISABLE_SHOWCOSTUMES:
    *reinterpret_cast<int*>(g_session.feedOptions) = *reinterpret_cast<const int*>(packet + 6);
    break;
  case 6:
    g_session.SetPetInfoAlert(*reinterpret_cast<const int*>(packet + 6) != 0);
    break;
  }
}

// OFFSET: 0x00D1E7B0
// FUNCTION: fn_21635
__declspec(noinline) void CGameMode::ReceiveTakeOffEquipResult(unsigned char result, int location, std::uint16_t identity) {
  *reinterpret_cast<std::uint32_t*>(&this->equipmentRequestState[0x08]) = timeGetTime() - 2000;
  switch (result) {
  case 0:
    if (location) {
      if (g_session.IsCostumeLocation(location)) {
        g_session.UnequipCostumeItem(location);
        return;
      }
      g_session.UnequipItem(location, identity);
    }
    break;
  case 1: {
    const char* text = LookupMessage(MSI_CAN_NOT_EQUIP_ITEM_FORBID);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 7, 0, 0);
    break;
  }
  }
}

// OFFSET: 0x00D25E90
// FUNCTION: fn_21677
void CGameMode::ReceiveHomunculusChange(const unsigned char* packet) {
  char text[256];
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
  switch (static_cast<char>(packet[3])) {
  case HO_PRE_INIT:
    *reinterpret_cast<std::uint32_t*>(&g_session.spAndHomunculusStats[0x08]) = *reinterpret_cast<const std::uint32_t*>(packet + 4);
    if (actor) {
      g_session.homunAlive = 1;
      g_session.m_homunJob = actor->GetEffectiveJob(-1);
    }
    break;
  case HO_RELATIONSHIP_CHANGED:
    g_session.homunFriendly = *reinterpret_cast<const int*>(packet + 8);
    if (actor) {
      g_session.m_homunJob = actor->GetEffectiveJob(-1);
    }
    if (g_windowMgr.homunInfoWnd) {
      g_windowMgr.homunInfoWnd->Invalidate();
    }
    if (g_session.IsAdminSubMode(3)) {
      std::sprintf(text, "%d", *reinterpret_cast<const int*>(packet + 8));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xF5F5F5, 0, 0, 0);
    }
    break;
  case HO_FULLNESS_CHANGED:
    g_session.homunHunger = *reinterpret_cast<const int*>(packet + 8);
    g_session.RefreshHomunculusInfo();
    if (actor) {
      g_session.m_homunJob = actor->GetEffectiveJob(-1);
    }
    if (g_windowMgr.homunInfoWnd) {
      g_windowMgr.homunInfoWnd->Invalidate();
    }
    if (g_session.IsAdminSubMode(3)) {
      std::sprintf(text, "Full : %d", *reinterpret_cast<const int*>(packet + 8));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x1EF5F5, 0, 0, 0);
    }
    break;
  case HO_ACCESSARY_CHANGED:
    if (actor) {
      if (*reinterpret_cast<const int*>(packet + 8)) {
        g_session.homunAccessory = *reinterpret_cast<const int*>(packet + 8);
      } else {
        g_session.homunAccessory = 0;
        actor->SetSprJob(actor->GetEffectiveJob(-1));
      }
      if (g_session.IsHomunAid(*reinterpret_cast<const std::uint32_t*>(packet + 4)) && g_windowMgr.homunInfoWnd) {
        g_windowMgr.homunInfoWnd->Invalidate();
      }
    }
    break;
  }
}

// OFFSET: 0x00D26950
// FUNCTION: fn_21683
__declspec(noinline) void CGameMode::ReceiveItemConsumed(const unsigned char* packet) {
  if (g_windowMgr.rodexComposeWnd) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_FRIEND_ADD__ALREADY_REQUEST)), 0x64FFFF, 0, 0, 0);
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_WRITE_RODEX, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_RODEXCOMPOSEWND);
  }

  if (*reinterpret_cast<int*>(&this->equipmentRequestState[0x14])) {
    *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 0;
    g_session.ResetSlots();
  }

  g_session.RemoveInventoryItem(
    *reinterpret_cast<const std::uint16_t*>(packet + 4), *reinterpret_cast<const std::int16_t*>(packet + 6)
  );
  *reinterpret_cast<int*>(&this->balloonDisplayState[0x18]) = 0;
  this->inventoryChanged = 1;
  if (g_session.petJob != -1 && g_session.petEggIndex == *reinterpret_cast<const std::uint16_t*>(packet + 4)) {
    g_session.ResetPetEggAndJob();
    EquipmentProperties::CEquipmentPropertiesMgr* properties = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
    int itemId = -1;
    int kind = 0;
    properties->RemoveExpansion(kind, itemId);
  }
}

// OFFSET: 0x00D25D50
// FUNCTION: fn_21676
__declspec(noinline) void CGameMode::ReceivePartyLeaderChange(const unsigned char* packet) {
  char text[500];
  MessengerMember member = g_session.PartyByAccount(*reinterpret_cast<const std::uint32_t*>(packet + 6));
  if (member.valid != 0) {
    g_session.SetPartyLeader(*reinterpret_cast<const std::uint32_t*>(packet + 2), 1);
    g_session.SetPartyLeader(*reinterpret_cast<const std::uint32_t*>(packet + 6), 0);
    g_session.partyLeader = *reinterpret_cast<const std::uint32_t*>(packet + 6) == g_session.m_aid;
    std::sprintf(text, LookupMessage(MSI_NOTIFY_PARTY_ROLE_MASTER), member.name.c_str());
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
    RefreshPartyWindows(&g_session);
    if (g_windowMgr.memorialDunWnd) {
      g_windowMgr.memorialDunWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00D2DA10
// FUNCTION: fn_21723
__declspec(noinline) void CGameMode::ReceiveItemSellResult(const unsigned char* packet) {
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  {
    const int count = *reinterpret_cast<const std::int16_t*>(packet + 4);
    const int total = *reinterpret_cast<const int*>(packet + 6) * count;
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::int16_t*>(packet + 2));
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY,
      reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_BUYINGSTORE_TRADE_SELLCOMPLETE), item.DisplayName(0).c_str(), count, total).c_str()),
      0xFFFF64, 0, 0, 0
    );
  }
  g_session.RemoveInventoryItem(
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
    path = static_cast<const std::string&>(FormatOwnedString("%s\\%s_%d.txt", directory.c_str(), name.c_str(), index));
  }

  std::FILE* file = _wfopen(DecodeText(g_textCodePage, path).c_str(), L"w");
  const std::string message = FormatOwnedString("Simulator Result File Create %s %s!!!!!!!!!!!!", file ? "success" : "Fail", itemName.c_str());
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), file ? 0xFFFF : 0xFF, 0, 0, 0);
  return file;
}

#pragma pack(push, 1)
struct RandomOptionSimulationRecord {
  /* 0000 */ int optionId;
  /* 0004 */ int firstRatioNumerator;
  /* 0008 */ int firstRatioDenominator;
  /* 000C */ unsigned char optionType;
  /* 000D */ int rate;
  /* 0011 */ int secondRatioNumerator;
  /* 0015 */ int secondRatioDenominator;
  /* 0019 */ int reportValue;
  /* 001D */ int firstRatioValue;
  /* 0021 */ int secondRatioValue;
};
#pragma pack(pop)

static_assert(sizeof(RandomOptionSimulationRecord) == 0x25);

// OFFSET: 0x00D606B0
// FUNCTION: fn_21974
void CGameMode::ReceiveRandomOptionSimulation(const unsigned char* packet) {
  const SimulationPacketHeader* header = reinterpret_cast<const SimulationPacketHeader*>(packet);
  std::string itemName;
  const char* name = reinterpret_cast<const char*>(header->name);
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

  int total = 0;
  std::fprintf(file, "\t%s", ToUtf8(std::string(name), g_textCodePage).c_str());
  std::fprintf(file, "============================\tSTART\t============================\n");
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
            file, "\t\t%d\t%d\t(%d/%d)\t%d\t(%d/%d)\t%.2f%%\n", option->reportValue, option->firstRatioValue, option->firstRatioNumerator,
            option->firstRatioDenominator, option->secondRatioValue, option->secondRatioNumerator, option->secondRatioDenominator,
            static_cast<float>(option->rate / 100.0)
          );
        } else {
          std::fprintf(
            file, "\t\t%d\t%d\t(%d/%d)\t%d\t(%d/%d)\n", option->reportValue, option->firstRatioValue, option->firstRatioNumerator,
            option->firstRatioDenominator, option->secondRatioValue, option->secondRatioNumerator, option->secondRatioDenominator
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
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUYINGSTORE_OPEN_FAILED)), 0xFF, 0, 0, 0);
    g_session.ClearMerchantPurchaseSavedItems();
    g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
    g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_1);
    break;
  case 2:
    std::sprintf(
      text, LookupMessage(MSI_BUYINGSTORE_OVERWEIGHT),
      static_cast<int>(
        static_cast<float>(*reinterpret_cast<const int*>(packet + 4) / 10 + g_session.m_weight) - static_cast<float>(g_session.m_maxWeight) * 0.9f
      )
    );
    g_windowMgr.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 9:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAIL_OPEN_STORE)), 0xFF, 0, 0, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CONNECT_SNS_FAIL_SEND)), 0xFF, 0, 0, 0);
    g_session.ClearMerchantPurchaseSavedItems();
    g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
    g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_1);
    break;
  }
}

// OFFSET: 0x00D4B050
// FUNCTION: fn_21863
void CGameMode::ReceiveItemObtainNotice(const unsigned char* packet) {
  ITEM_INFO item;
  item.SetItemId(*reinterpret_cast<const int*>(packet + 6));
  item.refiningLevel = 0;
  item.identified = 1;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case MSG_WEAPONREFINE_SUCCESS:
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ITEM_REFINE_SUCCEESS), item.DisplayName(0).c_str()).c_str()), 0xFFFF00, 0, 0, 0
    );
    break;
  case MSG_WEAPONREFINE_FAIL:
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ITEM_REFINE_FAIL), item.DisplayName(0).c_str()).c_str()), 0xCDCD00, 0, 0, 0
    );
    break;
  case MSG_WEAPONREFINE_FAIL_LEVEL:
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ITEM_REFINE_FAIL_LEVEL), item.DisplayName(0).c_str()).c_str()), 0xC8C8FF, 0, 0, 0
    );
    break;
  case MSG_WEAPONREFINE_FAIL_ITEM:
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ITEM_REFINE_FAIL_MATERIAL), item.DisplayName(0).c_str()).c_str()), 0xC8C8FF, 0, 0, 0
    );
    break;
  }
}

// OFFSET: 0x00D8A460
// FUNCTION: fn_22219
__declspec(noinline) void CGameMode::ReceiveTakeOffEquipItemResult(const unsigned char* packet) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t*>(packet + 2));
  item.identity[3] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 8)) {
  case 0: {
    if (g_session.IsCostumeLocation(*reinterpret_cast<const int*>(packet + 4))) {
      g_session.UnequipSwapEquippedItem(item);
    } else {
      g_session.UnequipSwapCostumeItem(item);
    }
    const char* suffix = LookupMessage(MSI_CHINESE_CHARACTER);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), suffix).c_str()), 0xC8FF00, 7, 0, 0
    );
    break;
  }
  case 1:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LAST_LOGIN)), 0xFF, 0, 0, 0);
    if (g_session.IsCostumeLocation(*reinterpret_cast<const int*>(packet + 4))) {
      g_session.UnequipSwapEquippedItem(item);
    } else {
      g_session.UnequipSwapCostumeItem(item);
    }
    break;
  }
}

// OFFSET: 0x00D2A290
// FUNCTION: fn_21703
void CGameMode::ReceiveBuyingStoreSellResult(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 5:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_FAIL)), 0xFF, 0, 0, 0);
    break;
  case 6: {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, *reinterpret_cast<const int*>(packet + 4), 0);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_BUYINGSTORE_TRADE_OVERCOUNT), item.DisplayName(0).c_str()).c_str()), 0xFF, 0, 0, 0
    );
    break;
  }
  case 7:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUYINGSTORE_TRADE_LACKBUYERZENY)), 0xFF, 0, 0, 0);
    break;
  }

  g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND_MODE_1);
  g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND_MODE_1);
  g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_2);
}

struct ReputePointEntry {
  /* 0000 */ std::uint64_t id;
  /* 0008 */ std::int64_t point;
};

// OFFSET: 0x00D78060
// FUNCTION: fn_22158
__declspec(noinline) void CGameMode::ReceiveItemReformOpen(int itemId) {
  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMREFORMWND);
  if (!g_itemReformMgr) {
    g_itemReformMgr = new (std::nothrow) CItemReformMgr;
  }
  if (g_itemReformMgr->HasItem(itemId) == true) {
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHITEM, itemId, 0, 0, 0);
    }
  } else {
    g_windowMgr.CloseWindow(WID_ITEMREFORMWND);
  }
}

// OFFSET: 0x00D79880
// FUNCTION: fn_22174
__declspec(noinline) void CGameMode::ReceiveReputeInfo(bool received, unsigned int count, const ReputePointEntry* entries) {
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
__declspec(noinline) void CGameMode::ReceiveEnchantResult(int messageId, int itemId) {
  if (!CEnchantMgr::s_instance) {
    CEnchantMgr::s_instance = new (std::nothrow) CEnchantMgr;
  }
  CEnchantMgr::s_instance->HandleEnchantResult(messageId, itemId);
}

// OFFSET: 0x00D7B4C0
// FUNCTION: fn_22205
__declspec(noinline) void CGameMode::ReceiveReputeOpen(std::uint64_t groupId, std::uint64_t reputeId) {
  if (!CReputeMgr::s_instance) {
    CReputeMgr::s_instance = new (std::nothrow) CReputeMgr;
  }
  CReputeMgr::s_instance->OpenRepute(groupId, reputeId);
}

// OFFSET: 0x00D7B550
// FUNCTION: fn_22206
__declspec(noinline) void CGameMode::ReceiveUiOpen(unsigned char type, std::uint64_t data) {
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
  g_timeScheduler->RemoveSchedulesByKey(5);
  CCashEmotionMgr* manager = g_cashEmotionMgr;
  if (!manager) {
    return;
  }

  manager->RemoveExpiredPacks();
  const int expiry = manager->GetEarliestExpiry();
  if (expiry < 0x7FFFFFFF) {
    const std::uint64_t now = static_cast<std::uint64_t>(manager->GetServerTime()) * 10000000;
    const std::int64_t interval = static_cast<std::int64_t>(static_cast<std::uint64_t>(expiry) * 10000000 - now) / 10000;
    if (interval != 0 && interval >= 0) {
      g_timeScheduler->AddSchedule(new CEmotionPurchaseExpireSchedule(now, interval, 1));
    }
  }
  manager->RefreshCommands();
}

// OFFSET: 0x00D31390
// FUNCTION: fn_21733
__declspec(noinline) void CGameMode::ReceivePartyItemPickupNotice(const unsigned char* packet) {
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  MessengerMember member = g_session.PartyByAccount(aid);
  if (aid != g_session.m_aid && member.valid) {
    ITEM_INFO item;
    item.identity[0] = packet[0x1F];
    item.identity[2] = *reinterpret_cast<const std::uint16_t*>(packet + 0x1D);
    item.identity[1] = *reinterpret_cast<const std::uint32_t*>(packet + 6);
    item.identity[3] = 0;
    item.SetItemId(*reinterpret_cast<const int*>(packet + 6));
    std::memcpy(item.cards, packet + 0x0D, sizeof(item.cards));
    item.identified = packet[0x0A];
    item.damaged = packet[0x0B];
    item.refiningLevel = packet[0x0C];

    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_ITEM_PICKUP_PARTY), member.name.c_str(), item.DisplayName(0).c_str()).c_str()),
      0xFFFF00, 9, 0, 0
    );
  }
}

// OFFSET: 0x00D64940
// FUNCTION: fn_21995
__declspec(noinline) void CGameMode::ReceiveEquipCardChange(const unsigned char* packet) {
  const EquipSlots slots = g_session.GetEquipSlots(*reinterpret_cast<const std::int16_t*>(packet + 2));
  ITEM_INFO item = g_session.GetEquippedItemAt(slots.values[2]);
  if (item.identity[1] != 0) {
    int* cards = reinterpret_cast<int*>(item.cards);
    switch (packet[0xA]) {
    case 1:
      cards[*reinterpret_cast<const std::int16_t*>(packet + 4)] = *reinterpret_cast<const int*>(packet + 6);
      break;
    case 2:
    case 3:
      cards[*reinterpret_cast<const std::int16_t*>(packet + 4)] = 0;
      break;
    }
    g_session.SetEquippedItem(slots.values[2], item);
  }
}

// OFFSET: 0x00D284D0
// FUNCTION: fn_21694
void CGameMode::ReceiveEquipWindowMicroscope2(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetAlternateCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 38u;
  const unsigned char* entry = packet + 45;
  for (int index = 0; index < count; ++index, entry += 38) {
    ITEM_INFO item;
    item.identity[0] = entry[6];
    item.identity[2] = *reinterpret_cast<const std::uint16_t*>(entry + 8);
    item.identity[1] = *reinterpret_cast<const std::int16_t*>(entry);
    item.identity[3] = *reinterpret_cast<const std::uint16_t*>(entry + 10);
    item.quantity = 1;
    std::memcpy(item.cards, entry + 14, sizeof(item.cards));
    item.SetItemId(*reinterpret_cast<const int*>(entry + 2));
    item.identified = entry[7];
    item.damaged = entry[12];
    item.refiningLevel = entry[13];
    item.expireTime = *reinterpret_cast<const std::uint32_t*>(entry + 30);
    item.bindOnEquip = *reinterpret_cast<const std::uint16_t*>(entry + 34);
    item.sprite = *reinterpret_cast<const std::uint16_t*>(entry + 36);

    if (item.identity[3] != 0) {
      if (g_session.IsCostumeLocation(item.identity[3])) {
        g_session.EquipAlternateCostumeItem(item);
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

  if (!g_windowMgr.CloseWindow(WID_EQUIPWND_VIEWOTHER_1)) {
    g_windowMgr.EnsureWindow(WID_EQUIPWND_VIEWOTHER_1);
  }
}

// OFFSET: 0x00D28700
// FUNCTION: fn_21695
void CGameMode::ReceiveEquipWindowMicroscope3(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetAlternateCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 41u;
  const unsigned char* entry = packet + 45;
  for (int index = 0; index < count; ++index, entry += 41) {
    ITEM_INFO item;
    item.identity[0] = entry[6];
    item.identity[2] = *reinterpret_cast<const std::uint32_t*>(entry + 7);
    item.identity[1] = *reinterpret_cast<const std::int16_t*>(entry);
    item.identity[3] = *reinterpret_cast<const std::uint32_t*>(entry + 11);
    item.quantity = 1;
    std::memcpy(item.cards, entry + 16, sizeof(item.cards));
    item.SetItemId(*reinterpret_cast<const int*>(entry + 2));

    const unsigned char flags = entry[40];
    item.identified = flags & 1;
    item.damaged = (flags >> 1) & 1;
    item.refiningLevel = entry[15];
    item.expireTime = *reinterpret_cast<const std::uint32_t*>(entry + 32);
    item.bindOnEquip = *reinterpret_cast<const std::uint16_t*>(entry + 36);
    item.sprite = *reinterpret_cast<const std::uint16_t*>(entry + 38);

    if (item.identity[3] != 0) {
      if (g_session.IsCostumeLocation(item.identity[3])) {
        g_session.EquipAlternateCostumeItem(item);
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

  if (!g_windowMgr.CloseWindow(WID_EQUIPWND_VIEWOTHER_1)) {
    g_windowMgr.EnsureWindow(WID_EQUIPWND_VIEWOTHER_1);
  }
}

// OFFSET: 0x00D4E5A0
// FUNCTION: fn_21877
void CGameMode::ReceiveGuildMemberAdd(const unsigned char* packet) {
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->membersRequested == 0) {
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
  member.head = *reinterpret_cast<const std::int16_t*>(packet + 0xA);
  member.headPalette = *reinterpret_cast<const std::int16_t*>(packet + 0xC);
  member.job = *reinterpret_cast<const std::int16_t*>(packet + 0x10);
  member.sex = *reinterpret_cast<const std::int16_t*>(packet + 0xE);
  member.positionId = *reinterpret_cast<const int*>(packet + 0x1C);
  member.level = *reinterpret_cast<const std::int16_t*>(packet + 0x12);
  member.memo = memo;
  member.exp = *reinterpret_cast<const int*>(packet + 0x14);
  member.online = *reinterpret_cast<const int*>(packet + 0x18);
  guildInfo->AddMember(member);

  g_playerNameDictionary->Set(*reinterpret_cast<const std::uint32_t*>(packet + 6), std::string(name, 25).c_str());
  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D510C0
// FUNCTION: fn_21889
void CGameMode::ReceiveGuildPositionInfo(const unsigned char* packet) {
  GuildInfo* guildInfo = &g_guildInfo;
  const int count = static_cast<unsigned>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) >> 4;
  guildInfo->positions.clear();

  const unsigned char* entry = packet + 4;
  for (int index = 0; index < count; ++index, entry += 16) {
    const int positionId = *reinterpret_cast<const int*>(entry);
    GuildInfo::Unknown00D4Entry position;
    position.name = guildInfo->m_positions[positionId];

    const unsigned int mode = *reinterpret_cast<const unsigned int*>(entry + 4);
    position.ranking = *reinterpret_cast<const int*>(entry + 8);
    position.positionId = positionId;
    position.canInvite = mode & 1;
    position.canExpel = (mode >> 4) & 1;
    position.tax = *reinterpret_cast<const int*>(entry + 12);
    position.canStorage = (mode >> 8) & 1;
    guildInfo->positions.push_back(position);
  }

  guildInfo->positions.sort([](const GuildInfo::Unknown00D4Entry& left, const GuildInfo::Unknown00D4Entry& right) {
    return left.positionId < right.positionId;
  });
  g_windowMgr.RefreshGuildWindows();
}
// OFFSET: 0x00D17BF0
// FUNCTION: fn_21597
void CGameMode::RefreshEmotionSaleSchedule() {
  g_timeScheduler->RemoveSchedulesByKey(6);
  CCashEmotionMgr* manager = g_cashEmotionMgr;
  if (!manager) {
    return;
  }

  const CashEmotionPackRefresh refresh = manager->RefreshUnownedPacks();
  const int expiry = refresh.nextTime;
  if (expiry < 0x7FFFFFFF) {
    const std::uint64_t now = static_cast<std::uint64_t>(manager->GetServerTime()) * 10000000;
    const std::int64_t interval = static_cast<std::int64_t>(static_cast<std::uint64_t>(expiry) * 10000000 - now) / 10000;
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

// GLOBAL: gl_25132
std::map<int, MiniMapEffectMarker> g_miniMapEffectMarkers;

// OFFSET: 0x00D244B0
// FUNCTION: fn_21667
__declspec(noinline) void CGameMode::AddMiniMapEffectMarker(const unsigned char* packet) {
  const MiniMapEffectMarkerPacket* marker = reinterpret_cast<const MiniMapEffectMarkerPacket*>(packet);
  auto found = g_miniMapEffectMarkers.find(marker->key);
  if (found != g_miniMapEffectMarkers.end()) {
    found->second.x = marker->x;
    found->second.y = marker->y;
    found->second.count = 0x78;
    return;
  }

  g_miniMapEffectMarkers.insert({marker->key, {0x78, marker->x, marker->y}});
}

struct ActorMessage32 {
  /* 0000 */ std::string text;
  /* 0018 */ std::string extra;
  /* 0030 */ int flags;
  /* 0034 */ int userCount;
  /* 0038 */ int maxUsers;
  /* 003C */ int chatId;
};

// OFFSET: 0x00D25AC0
// FUNCTION: fn_21675
void CGameMode::ReceiveActorMessage34(const unsigned char* packet) {
  this->chatRoomActorIds[*reinterpret_cast<const std::uint32_t*>(packet + 8)] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  ActorMessage32 message;
  message.text.assign(reinterpret_cast<const char*>(packet + 17), *reinterpret_cast<const std::int16_t*>(packet + 2) - 17);
  message.flags = packet[16];
  message.userCount = *reinterpret_cast<const std::int16_t*>(packet + 14);
  message.maxUsers = *reinterpret_cast<const std::int16_t*>(packet + 12);
  message.chatId = *reinterpret_cast<const int*>(packet + 8);
  if (message.text.size() > 60) {
    message.text.resize(60);
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_CHATROOMWND);
  window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&message), 0, 0, 0);
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
  /* 0008 */ int condition;
  /* 000C */ int mobId;
  /* 0010 */ std::int16_t minLevel;
  /* 0012 */ std::int16_t maxLevel;
  /* 0014 */ std::int16_t count;
  /* 0016 */ char name[24];
};

struct AddQuestHunt {
  /* 0000 */ int huntId;
  /* 0004 */ int condition;
  /* 0008 */ int mobId;
  /* 000C */ std::int16_t minLevel;
  /* 000E */ std::int16_t maxLevel;
  /* 0010 */ std::int16_t count;
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
    g_renewQuestMgr->UpdateQuestWindows();
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
    info.m_huntConditionFlags[i] = hunt.condition;
    info.m_huntMobId[i] = hunt.mobId;
    info.m_huntKillCount[i] = hunt.count;
    info.m_huntMinLevel[i] = hunt.minLevel;
    info.m_huntMaxLevel[i] = hunt.maxLevel;
    const int mobId = hunt.mobId;
    if ((mobId > 1000 && mobId < 3999) || static_cast<unsigned int>(mobId - 20021) <= 11978) {
      StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", hunt.name);
    } else {
      if (mobId == 0x1099) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND));
      } else if (mobId == 0x109A) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND_FAILED));
      } else if (mobId == 0x287B) {
        const std::string mapName = FormatOwnedString("%s.rsw", hunt.name);
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetMapDisplayName(mapName.c_str()));
      } else {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetJobName(mobId, 99));
      }
    }
  }

  g_renewQuestMgr->AddOngoingQuest(packet->questId, info);
  g_renewQuestMgr->UpdateQuestWindows();
  g_windowMgr.EnsureWindow(WID_NOTIFYQUESTWND);
  const char* text = LookupMessage(MSI_QUESTGET);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0x13, 0, 0);
}

// OFFSET: 0x00D20EF0
// FUNCTION: fn_21652
void CGameMode::ReceiveAddQuest(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const AddQuestPacket*>(data);
  if (g_renewQuestMgr->SetOngoingQuestState(packet->questId, packet->state) == true) {
    g_renewQuestMgr->UpdateQuestWindows();
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
    info.m_huntConditionFlags[i] = hunt.condition;
    info.m_huntMobId[i] = hunt.mobId;
    info.m_huntKillCount[i] = hunt.count;
    info.m_huntMinLevel[i] = hunt.minLevel;
    info.m_huntMaxLevel[i] = hunt.maxLevel;
    const int mobId = hunt.mobId;
    if ((mobId > 1000 && mobId < 3999) || static_cast<unsigned int>(mobId - 20021) <= 11978) {
      StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", hunt.name);
    } else {
      if (mobId == 0x1099) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND));
      } else if (mobId == 0x109A) {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_INVENTORY_EXTEND_FAILED));
      } else if (mobId == 0x287B) {
        const std::string mapName = FormatOwnedString("%s.rsw", hunt.name);
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetMapDisplayName(mapName.c_str()));
      } else {
        StringFormat(info.m_huntName[i], 0x33, static_cast<std::size_t>(-1), "%s", g_session.GetJobName(mobId, 99));
      }
    }
  }

  g_renewQuestMgr->AddOngoingQuest(packet->questId, info);
  g_renewQuestMgr->UpdateQuestWindows();
  g_windowMgr.EnsureWindow(WID_NOTIFYQUESTWND);
  const char* text = LookupMessage(MSI_QUESTGET);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0x13, 0, 0);
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

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_BARTERMARKETWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_BARTERITEMPURCHASEWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D17D30
// FUNCTION: fn_21598
void CGameMode::ReceiveNotifyBan(const unsigned char* packet) {
  const int code = packet[2];
  GetRagConnection().Close();
  std::string text;
  switch (code) {
  case BAN_UNFAIR:
    text = text + LookupMessage(MSI_BANNED) + "(" + IntToString(0, 0) + ")";
    break;
  case BAN_SERVER_SHUTDOWN:
    text = text + LookupMessage(MSI_SERVER_OFF) + "(" + IntToString(1, 0) + ")";
    break;
  case BAN_ANOTHER_LOGIN:
    text = text + LookupMessage(MSI_DOUBLE_LOGIN_PROHIBITED) + "(" + IntToString(2, 0) + ")";
    break;
  case BAN_DETECTED_SPEEDHACK:
    text = text + LookupMessage(MSI_SPEEDHACK) + "(" + IntToString(3, 0) + ")";
    break;
  case BAN_PC_OVERFLOW:
    text = text + LookupMessage(MSI_PC_OVERFLOW) + "(" + IntToString(4, 0) + ")";
    break;
  case BAN_UNDER_AGE:
    text = text + LookupMessage(MSI_UNDER_AGE) + "(" + IntToString(5, 0) + ")";
    break;
  case BAN_IP_OVERFLOW:
    text = text + LookupMessage(MSI_BAN_IP_OVERFLOW) + "(" + IntToString(9, 0) + ")";
    break;
  case BAN_PAY_OUT:
    text = text + LookupMessage(MSI_BAN_PAY_OUT) + "(" + IntToString(10, 0) + ")";
    break;
  case BAN_OP_FORCE:
    if (g_serviceType == 4) {
      text = text + LookupMessage(MSI_BAN_OP_FORCE) + "(" + IntToString(15, 0) + ")";
    }
    break;
  case 110:
    text = text + LookupMessage(MSI_BAN_NOT_ALLOWED_JOBCLASS) + "(" + IntToString(110, 0) + ")";
    break;
  case 113:
    text = text + LookupMessage(MSI_BAN_SHUTDOWN_TEENAGER) + "(" + IntToString(113, 0) + ")";
    break;
  case 115:
    text = text + LookupMessage(MSI_MENU_REPLAYWND) + "(" + IntToString(115, 0) + ")";
    break;
  default:
    text = text + LookupMessage(MSI_BANNED) + "(" + IntToString(code, 0) + ")";
    break;
  }

  if (text.size() == 0) {
    text = LookupMessage(MSI_RESULT_FAIL_UNKNOWN);
  }
  g_windowMgr.Confirm(text.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
  this->SendMsg(MM_QUIT, 0, 0, 0, 0);
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
    std::string masked("");
    g_forbiddenWords.Mask(text.data(), &masked);
  }
  if (!g_serviceOptions.Get(0x94)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), 0xFF9DFF, 5, 0, 0);
    if (g_session.m_whisperRecord != 0) {
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
    std::string masked("");
    g_forbiddenWords.Mask(text.data(), &masked);
  }
  const int aid = *reinterpret_cast<const int*>(data + 4);
  if (MonsterJobColor(aid)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), MonsterJobColor(aid), 0, 0, 0);
  } else if (!g_serviceOptions.Get(0x72) && !g_serviceOptions.Get(0x94)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), 0x9B00FF, 1, 0, 0);
  }
  if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(text.data());
    actor->SendMsg(nullptr, AM_CHAT, value, 0, 0, 0, 0, 0);
  }
  if (g_session.m_whisperRecord != 0) {
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
  __forceinline void ReadStoreEquipEntry(StoreEquipEntry& entry, const EquipWindowGradeRecord& record) {
    entry.index = record.index;
    entry.itemId = record.itemId;
    entry.type = record.type;
    entry.location = record.location;
    entry.wearState = record.wearState;
    entry.refine = record.refine;
    std::memcpy(entry.cards, record.cards, sizeof(entry.cards));
    entry.expireTime = record.expireTime;
    entry.bindOnEquip = record.bindOnEquip;
    entry.sprite = record.sprite;
    entry.identified = record.flags & 1;
    entry.damaged = (record.flags >> 1) & 1;
    entry.placeEtcTab = (record.flags >> 2) & 1;
    std::memcpy(entry.options, record.options, sizeof(entry.options));
    entry.grade = record.grade;
  }

  // INLINED
  __forceinline void FillEquipWindowItem(ITEM_INFO& item, const StoreEquipEntry& record, const StoreItemOption* options) {
    item.identity[0] = record.type;
    item.identity[1] = record.index;
    item.identity[2] = record.location;
    item.identity[3] = record.wearState;
    item.quantity = 1;
    std::memcpy(item.cards, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.identified;
    item.damaged = record.damaged;
    item.refiningLevel = record.refine;
    item.expireTime = record.expireTime;
    item.bindOnEquip = record.bindOnEquip;
    item.sprite = record.sprite;
    item.flags[0] = record.placeEtcTab;
    item.optionCount = CountStoreItemOptions(options);
    item.grade = 0;
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = options[index];
    }
  }

  // INLINED
  __forceinline void FillEquipWindowGradeItem(ITEM_INFO& item, const StoreEquipEntry& record) {
    item.identity[0] = record.type;
    item.identity[1] = record.index;
    item.identity[2] = record.location;
    item.identity[3] = record.wearState;
    item.quantity = 1;
    std::memcpy(item.cards, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.identified;
    item.damaged = record.damaged;
    item.refiningLevel = record.refine;
    item.expireTime = record.expireTime;
    item.bindOnEquip = record.bindOnEquip;
    item.sprite = record.sprite;
    item.flags[0] = record.placeEtcTab;
    item.optionCount = CountStoreItemOptions(record.options);
    item.grade = record.grade;
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = record.options[index];
    }
  }

  // INLINED
  __forceinline void EquipWindowItem(const ITEM_INFO& item) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
      g_session.petEggIndex = item.identity[1];
    }

    if (item.identity[3] != 0) {
      if (g_session.IsCostumeLocation(item.identity[3])) {
        g_session.EquipAlternateCostumeItem(item);
      } else {
        g_session.Equip(item);
      }
    }

    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
      const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.petEggIndex).itemId.c_str());
      if (!g_petEvolutionMgr->IsAutoFeeding(petEgg) && g_petAutoFeed == true) {
        g_session.SendPetAutoFeedOption(0);
      }

      EquipmentProperties::CEquipmentPropertiesMgr* manager = g_equipmentPropertiesMgr;
      if (manager == nullptr) {
        manager = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
        g_equipmentPropertiesMgr = manager;
      }

      const int kind = 0;
      const int itemId = std::atoi(item.itemId.c_str());
      manager->AddExpansion(kind, itemId, false);
    }
  }
}

// OFFSET: 0x00D28950
// FUNCTION: fn_21696
void CGameMode::ReceiveEquipWindowMicroscope4(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetAlternateCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 67u;
  const StoreEquipOptionRecord* records = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 45);
  for (int index = 0; index < count; ++index) {
    const StoreEquipOptionRecord* record = &records[index];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    FillEquipWindowItem(item, entry, entry.options);
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

  if (!g_windowMgr.CloseWindow(WID_EQUIPWND_VIEWOTHER_1)) {
    g_windowMgr.EnsureWindow(WID_EQUIPWND_VIEWOTHER_1);
  }
}

// OFFSET: 0x00D28EA0
// FUNCTION: fn_21697
void CGameMode::ReceiveEquipWindowMicroscope5(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetAlternateCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 47) / 67u;
  const StoreEquipOptionRecord* records = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 47);
  for (int index = 0; index < count; ++index) {
    const StoreEquipOptionRecord* record = &records[index];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    FillEquipWindowItem(item, entry, entry.options);
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
  g_equipWindowLook.bodyCostume = *reinterpret_cast<const std::int16_t*>(packet + 0x2C);

  if (!g_windowMgr.CloseWindow(WID_EQUIPWND_VIEWOTHER_1)) {
    g_windowMgr.EnsureWindow(WID_EQUIPWND_VIEWOTHER_1);
  }
}

// OFFSET: 0x00D29400
// FUNCTION: fn_21698
void CGameMode::ReceiveEquipWindowMicroscope6(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetAlternateCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 47) / 68u;
  const EquipWindowGradeRecord* records = reinterpret_cast<const EquipWindowGradeRecord*>(packet + 47);
  for (int index = 0; index < count; ++index) {
    const EquipWindowGradeRecord* record = &records[index];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    FillEquipWindowGradeItem(item, entry);
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
  g_equipWindowLook.bodyCostume = *reinterpret_cast<const std::int16_t*>(packet + 0x2C);

  if (!g_windowMgr.CloseWindow(WID_EQUIPWND_VIEWOTHER_1)) {
    g_windowMgr.EnsureWindow(WID_EQUIPWND_VIEWOTHER_1);
  }
}

namespace {
  // INLINED
  __forceinline void AddInventoryEquipItem(const ITEM_INFO& item) {
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
      g_session.petEggIndex = item.identity[1];
    }

    if (item.identity[3] != 0) {
      if (g_session.IsCostumeLocation(item.identity[3])) {
        char equipped = 1;
        g_session.EquipCostumeItem(item, &equipped);
      } else {
        char equipped = 1;
        g_session.EquipInventoryItem(item, &equipped);
      }
    } else {
      g_session.AddInventoryItem(item, 0);
      g_session.UpdateInventoryItem(item, 0);
    }

    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
      const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.petEggIndex).itemId.c_str());
      if (!g_petEvolutionMgr->IsAutoFeeding(petEgg) && g_petAutoFeed == true) {
        g_session.SendPetAutoFeedOption(0);
      }

      EquipmentProperties::CEquipmentPropertiesMgr* manager = g_equipmentPropertiesMgr;
      if (manager == nullptr) {
        manager = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
        g_equipmentPropertiesMgr = manager;
      }

      const int kind = 0;
      const int itemId = std::atoi(item.itemId.c_str());
      manager->AddExpansion(kind, itemId, false);
    }
  }
}

// OFFSET: 0x00D2C7F0
// FUNCTION: fn_21717
void CGameMode::ReceiveInventoryEquipList(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 4) / 41u;
  const StoreEquipRecord* record = reinterpret_cast<const StoreEquipRecord*>(packet + 4);
  StoreItemOption options[5] = {0};
  for (int index = 0; index < count; ++index, ++record) {
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    FillEquipWindowItem(item, entry, options);
    AddInventoryEquipItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D2CC80
// FUNCTION: fn_21718
void CGameMode::ReceiveInventoryEquipList2(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 4) / 67u;
  const StoreEquipOptionRecord* records = reinterpret_cast<const StoreEquipOptionRecord*>(packet + 4);
  for (int index = 0; index < count; ++index) {
    const StoreEquipOptionRecord* record = &records[index];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    FillEquipWindowItem(item, entry, entry.options);
    AddInventoryEquipItem(item);
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D2D140
// FUNCTION: fn_21719
void CGameMode::ReceiveInventoryEquipList3(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetCostumeItems();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 4) / 68u;
  const EquipWindowGradeRecord* records = reinterpret_cast<const EquipWindowGradeRecord*>(packet + 4);
  for (int index = 0; index < count; ++index) {
    const EquipWindowGradeRecord* record = &records[index];
    StoreEquipEntry entry;
    ReadStoreEquipEntry(entry, *record);
    ITEM_INFO item;
    FillEquipWindowGradeItem(item, entry);
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
  this->pickupReqItemNaidList.erase(this->pickupReqItemNaidList.begin(), this->pickupReqItemNaidList.end());

  switch (packet->result) {
  case PICKUP_SUCCEED: {
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    item.expireTime = packet->expireTime;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }

    g_session.AddInventoryItem(item, 1);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    break;
  }
  case RECEIVE_ITEM: {
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    item.expireTime = packet->expireTime;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }
    switch (packet->type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      break;
    default:
      item.identity[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.identity[1] > 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) != 0) {
      *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    break;
  }
  default:
    this->NotifyItemResult(packet->result);
    break;
  }
}

// OFFSET: 0x00D2E4F0
// FUNCTION: fn_21726
void CGameMode::ReceiveItemPickupAck3(const unsigned char* data) {
  const ItemPickupAck3Packet* packet = reinterpret_cast<const ItemPickupAck3Packet*>(data);
  this->pickupReqItemNaidList.erase(this->pickupReqItemNaidList.begin(), this->pickupReqItemNaidList.end());

  switch (packet->result) {
  case PICKUP_SUCCEED: {
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    item.expireTime = packet->expireTime;
    item.bindOnEquip = packet->bindOnEquip;
    item.flags[1] = 1;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }

    g_session.AddInventoryItem(item, 1);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  case RECEIVE_ITEM: {
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    item.expireTime = packet->expireTime;
    item.bindOnEquip = packet->bindOnEquip;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }
    switch (packet->type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      break;
    default:
      item.identity[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.identity[1] > 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) != 0) {
      *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  default:
    this->NotifyItemResult(packet->result);
    break;
  }
}

// OFFSET: 0x00D2EAE0
// FUNCTION: fn_21727
void CGameMode::ReceiveItemPickupAck(const unsigned char* data) {
  const ItemPickupAckPacket* packet = reinterpret_cast<const ItemPickupAckPacket*>(data);
  this->pickupReqItemNaidList.erase(this->pickupReqItemNaidList.begin(), this->pickupReqItemNaidList.end());

  switch (packet->result) {
  case PICKUP_SUCCEED: {
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }

    g_session.AddInventoryItem(item, 1);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    break;
  }
  case RECEIVE_ITEM: {
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    if (std::atoi(item.itemId.c_str()) == 607) {
      this->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }
    switch (packet->type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      break;
    default:
      item.identity[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    std::string text;
    int amount;
    if (current.identity[1] > 0) {
      text = FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), packet->amount - current.quantity);
      amount = packet->amount - current.quantity;
    } else {
      text = FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), packet->amount);
      amount = packet->amount;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) != 0) {
      *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    break;
  }
  default:
    this->NotifyItemResult(packet->result);
    break;
  }
}

// OFFSET: 0x00D2F200
// FUNCTION: fn_21729
void CGameMode::ReceiveItemPickupAck5(
  std::uint16_t index, std::uint16_t amount, int itemId, unsigned char identified, unsigned char damaged, unsigned char refine, ItemCardSlots slots,
  std::uint32_t location, unsigned char type, unsigned char result, std::uint32_t expireTime, std::uint16_t bindOnEquip
) {
  this->pickupReqItemNaidList.erase(this->pickupReqItemNaidList.begin(), this->pickupReqItemNaidList.end());

  switch (result) {
  case PICKUP_SUCCEED: {
    {
      StoreItemOption options[5] = {0};
      ITEM_INFO item;
      item.identity[0] = type;
      item.identity[2] = location;
      item.identity[1] = index;
      item.identity[3] = 0;
      item.quantity = amount;
      item.SetItemId(itemId);
      std::memcpy(item.cards, slots.cards, sizeof(slots.cards));
      item.identified = identified;
      item.damaged = damaged;
      item.refiningLevel = refine;
      item.expireTime = expireTime;
      item.bindOnEquip = bindOnEquip;
      item.flags[1] = 1;
      item.optionCount = CountStoreItemOptions(options);
      for (int option = 0; option < item.optionCount; ++option) {
        reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
      }
      if (std::atoi(item.itemId.c_str()) == 607) {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
      }
      if (type == 10 || type == 16 || type == 17 || type == 19) {
        item.identity[3] = 0x8000;
      }

      g_session.AddInventoryItem(item, 1);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

      UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
      window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
        quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
      }
    }
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  case RECEIVE_ITEM: {
    StoreItemOption options[5] = {0};
    ITEM_INFO item;
    item.identity[0] = type;
    item.identity[2] = location;
    item.identity[1] = index;
    item.identity[3] = 0;
    item.quantity = amount;
    item.SetItemId(itemId);
    std::memcpy(item.cards, slots.cards, sizeof(slots.cards));
    item.identified = identified;
    item.damaged = damaged;
    item.refiningLevel = refine;
    item.expireTime = expireTime;
    item.bindOnEquip = bindOnEquip;
    item.flags[1] = 1;
    item.optionCount = CountStoreItemOptions(options);
    for (int option = 0; option < item.optionCount; ++option) {
      reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
    }
    if (type == 10 || type == 16 || type == 17 || type == 19) {
      item.identity[3] = 0x8000;
    }
    switch (type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      break;
    default:
      item.identity[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    int count = amount;
    if (current.identity[1] > 0) {
      count -= current.quantity;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), count).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, count, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  default:
    this->NotifyItemResult(result);
    break;
  }
}

// OFFSET: 0x00D2F8F0
// FUNCTION: fn_21730
void CGameMode::ReceiveItemPickupAck7(const unsigned char* data) {
  const ItemPickupAck7Packet* packet = reinterpret_cast<const ItemPickupAck7Packet*>(data);
  this->pickupReqItemNaidList.erase(this->pickupReqItemNaidList.begin(), this->pickupReqItemNaidList.end());

  switch (packet->result) {
  case PICKUP_SUCCEED: {
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    item.expireTime = packet->expireTime;
    item.bindOnEquip = packet->bindOnEquip;
    item.flags[1] = 1;
    item.optionCount = CountStoreItemOptions(options);
    for (int option = 0; option < item.optionCount; ++option) {
      reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }

    g_session.AddInventoryItem(item, 1);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), packet->amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, packet->amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  case RECEIVE_ITEM: {
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    ITEM_INFO item;
    item.identity[0] = packet->type;
    item.identity[2] = packet->location;
    item.identity[1] = packet->index;
    item.identity[3] = 0;
    item.quantity = packet->amount;
    item.SetItemId(packet->itemId);
    std::memcpy(item.cards, packet->cards, sizeof(packet->cards));
    item.identified = packet->identified;
    item.damaged = packet->damaged;
    item.refiningLevel = packet->refine;
    item.expireTime = packet->expireTime;
    item.bindOnEquip = packet->bindOnEquip;
    item.flags[1] = 1;
    item.optionCount = CountStoreItemOptions(options);
    for (int option = 0; option < item.optionCount; ++option) {
      reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
    }
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.identity[3] = 0x8000;
    }
    switch (packet->type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      break;
    default:
      item.identity[2] = 0;
      break;
    }

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
    int amount = packet->amount;
    if (current.identity[1] > 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  default:
    this->NotifyItemResult(packet->result);
    break;
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
      case EQP_HAND_L:
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_DAMAGED_EQUIP_PARTY), member.name.c_str(), LookupMessage(0x109A)));
        break;
      case EQP_HAND_R:
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_DAMAGED_EQUIP_PARTY), member.name.c_str(), LookupMessage(0x109B)));
        break;
      case EQP_ARMOR:
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_DAMAGED_EQUIP_PARTY), member.name.c_str(), LookupMessage(MSI_DAMAGED_BODY)));
        break;
      case EQP_HEAD_TOP:
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_DAMAGED_EQUIP_PARTY), member.name.c_str(), LookupMessage(0x10CB)));
        break;
      }
    }
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    ITEM_INFO item = g_session.GetEquippedItemAt(slots.values[2]);
    if (item.identity[1] != 0) {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_DAMAGED_EQUIP), item.DisplayName(0).c_str()));
    }
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0xD, 0, 0);
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

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_EXPANDEDBARTERMARKETWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_EXPANDEDBARTERITEMPURCHASEWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
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

  g_windowMgr.EnsureWindow(WID_EXPANDEDBARTERMARKETWND)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  g_windowMgr.EnsureWindow(WID_EXPANDEDBARTERITEMPURCHASEWND)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
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
      const int hungerLevel = g_session.GetPetHungryState(g_session.petPreviousHunger);
      const int emotion = g_session.GetPetEmotionNumber(g_session.petJob, 0, hungerLevel);
      const int action = g_session.GetPetActionNumber(hungerLevel, friendlyLevel, 0);
      this->SendMsg(MM_PET_ACT, emotion, 0, 0, 0);
      this->SendMsg(MM_PET_ACT, action, 0, 0, 0);

      const int count = LookupClientItem(g_session.m_inventoryItems, packet->itemId, 0).quantity;
      char name[260];
      std::memset(name, 0, sizeof(name));
      ITEM_INFO item;
      item.SetItemId(packet->itemId);
      int capacity = sizeof(name);
      item.GetIdDisplayName(name, capacity, 0);
      const std::string message = FormatOwnedString(LookupMessage(MSI_REFRESH), name, count);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF64, 0, 0, 0);
    } else {
      ITEM_INFO item;
      item.SetItemId(packet->itemId);
      const std::string text = FormatOwnedString(LookupMessage(MSI_NOT_EXIST_PET_FOOD), item.DisplayName(0).c_str());
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
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
      item.GetIdDisplayName(name, capacity, 0);
      const std::string message = FormatOwnedString(LookupMessage(MSI_HOMUN_SELF_FEEDING_EXP_INFORMATION), name, count);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF64, 0, 0, 0);
    } else {
      ITEM_INFO item;
      item.SetItemId(packet->itemId);
      const std::string text = FormatOwnedString(LookupMessage(MSI_NOT_EXIST_PET_FOOD), item.DisplayName(0).c_str());
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(text.c_str()), 0x1E1EF5, 0, 0, 0);
      if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(&g_session.spAndHomunculusStats[8]))) {
        const int emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Sweat);
        actor->SendMsg(nullptr, AM_CASH_EMOTION, 0, emotion, 0, 0, 0, 0);
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
  case CHANGESTATEPET_INIT:
    g_session.petGid = packet->aid;
    if (actor) {
      g_session.petJob = actor->GetEffectiveJob(-1);
      actor->SendMsg(nullptr, AM_UPDATEGAGE, 0, 0, 0, 0, 0, 0);
      actor->objectType = 7;
    }
    break;
  case CHANGESTATEPET_INTIMACY: {
    const int previousFriendly = g_session.m_petFriendly;
    const int friendlyState = g_session.GetPetFriendlyState(previousFriendly);
    g_session.m_petFriendly = packet->value;
    if (actor) {
      g_session.petJob = actor->GetEffectiveJob(-1);
    }
    if (UIPetInfoWnd* window = static_cast<UIPetInfoWnd*>(g_windowMgr.petInfoWnd)) {
      window->UpdateFriendlyGauge();
    }
    if (g_session.GetDisplayJob() >= 0xFD2 && g_session.GetDisplayJob() <= 0xFD4 && packet->value < 100 && g_session.petHunger < 30) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PET_STARVING)), 0x3232FF, 0, 0, 0);
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
  case CHANGESTATEPET_HUNGER:
    g_session.petPreviousHunger = g_session.petHunger;
    g_session.petHunger = packet->value;
    if (actor) {
      g_session.petJob = actor->GetEffectiveJob(-1);
      actor->SendMsg(nullptr, AM_UPDATEGAGE, 0, 0, 0, 0, 0, 0);
    }
    if (g_windowMgr.petInfoWnd) {
      g_windowMgr.petInfoWnd->Invalidate();
    }
    break;
  case CHANGESTATEPET_ACCESSORY:
    if (!actor) {
      break;
    }
    if (packet->value != 0) {
      const char* actionName = g_session.GetPetAccActName(packet->value);
      actor->SetActionName(actionName);
      g_session.petAccessory = packet->value;
    } else {
      g_session.petAccessory = 0;
      actor->SetSprJob(actor->GetEffectiveJob(-1));
    }
    if (packet->aid == g_session.petGid && g_windowMgr.petInfoWnd) {
      g_windowMgr.petInfoWnd->Invalidate();
    }
    break;
  case CHANGESTATEPET_PERFORMANCE:
    if (actor) {
      actor->SetHealthState(packet->value);
      actor->SetState(0, 1);
    }
    break;
  case CHANGESTATEPET_UPDATE_EGG:
    if (packet->value == 0) {
      g_session.ResetPetEggAndJob();
      if (UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMWND)) {
        window->Invalidate();
      }
      const int itemId = -1;
      const int kind = 0;
      EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->RemoveExpansion(kind, itemId);
      g_windowMgr.CloseWindow(WID_PETINFOWND);
    } else if (packet->value == 1) {
      if (UIItemIdentifyWnd* window = static_cast<UIItemIdentifyWnd*>(g_windowMgr.LookupWindow(WID_PETEGGLISTWND))) {
        int found = 0;
        ITEM_INFO item = window->GetSelectedItem(&found);
        if (found == 1) {
          item.damaged = 1;
          g_session.UpdateInventoryItem(item, 1);
          g_session.petEggIndex = item.identity[1];
          if (UIWindow* inventory = g_windowMgr.LookupWindow(WID_ITEMWND)) {
            inventory->Invalidate();
          }
          EquipmentProperties::CEquipmentPropertiesMgr* properties = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
          const int itemId = item.GetItemId();
          const int kind = 0;
          properties->AddExpansion(kind, itemId, true);
        }
        g_windowMgr.CloseWindow(WID_PETEGGLISTWND);
      }
    }
    break;
  case 7:
    if (g_serviceOptions.Get(0x128) == 1) {
      g_windowMgr.AddMessageBox(LookupMessage(0x10F8), 0, true, 0, 0, LookupMessage(MSI_SHOW_NAME_OF_ENTERED_MAP), 0x118, 0x78, nullptr, false);
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
  if (g_forbiddenWords.Contains(guildName)) {
    std::string masked("");
    g_forbiddenWords.Mask(guildName, &masked);
  }

  guildInfo->guildName = guildName;
  g_guildInfo.masterName = masterName;
  g_guildInfo.isMaster = g_guildInfo.masterName == g_session.DecodePlayerName();
  guildInfo->manageLand = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->guildLevel = packet->level;
  guildInfo->memberCount = packet->userCount;
  guildInfo->onlineMemberCount = packet->userCount;
  guildInfo->maxMemberCount = packet->maxUserCount;
  guildInfo->memberAverageLevel = packet->averageLevel;
  guildInfo->exp = packet->exp;
  guildInfo->nextExp = packet->maxExp;
  guildInfo->points = packet->point;
  guildInfo->honor = packet->honor;
  guildInfo->virtue = packet->virtue;
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
  case USESKILL_FAIL_LEVEL:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    if (packet->skillId == NV_BASIC) {
      switch (packet->value) {
      case ZCSK_ITEMEXCHANGE:
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE1);
        break;
      case ZCSK_EMOTION:
        this->requestTimers[0x16] = 0;
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE2);
        break;
      case ZCSK_SIT:
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE3);
        break;
      case ZCSK_CHAT:
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE4);
        break;
      case ZCSK_PARTY:
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE5);
        break;
      case ZCSK_SHOUT:
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE6);
        break;
      case ZCSK_PK:
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE7);
        break;
      case ZCSK_MANNERPOINT: {
        const char* text = LookupMessage(MSI_NOT_ENOUGH_FOR_MANNER_POINT);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0xB, 0, 0);
      }
        [[fallthrough]];
      case ZCSK_JOINPARTY:
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE5_2);
        break;
      default:
        return;
      }
    } else {
      switch (packet->skillId) {
      case AL_WARP:
        notice = LookupMessage(MSI_NOTENOUGHSKILLLEVEL);
        break;
      case TF_STEAL:
        notice = LookupMessage(MSI_FAIL_STEAL);
        break;
      case TF_POISON:
        notice = LookupMessage(MSI_FAIL_POISON);
        break;
      default:
        if (!showMessage) {
          return;
        }
        notice = LookupMessage(MSI_NOT_ENOUGH_SKILLLEVE8);
        break;
      }
    }
    break;
  case USESKILL_FAIL_SP_INSUFFICIENT:
    notice = LookupMessage(MSI_USESKILL_FAIL_SP_INSUFFICIENT);
    break;
  case USESKILL_FAIL_HP_INSUFFICIENT:
    notice = LookupMessage(MSI_USESKILL_FAIL_HP_INSUFFICIENT);
    break;
  case USESKILL_FAIL_STUFF_INSUFFICIENT:
    notice = LookupMessage(MSI_NOT_ENOUGH_SOURCE);
    break;
  case USESKILL_FAIL_SKILLINTERVAL:
    if (HidesUseFailureMessage(packet->skillId) || !showMessage) {
      return;
    }
    std::sprintf(text, LookupMessage(MSI_SKILLINTERVAL2), CSkillInfo::DisplayName(*g_session.m_hotkeyScript, packet->skillId));
    notice = text;
    break;
  case USESKILL_FAIL_MONEY:
    notice = LookupMessage(MSI_SKILL_FAIL_MONEY);
    break;
  case USESKILL_FAIL_THIS_WEAPON:
    notice = LookupMessage(MSI_USESKILL_FAIL_NOT_SUITABLE_WEAPON);
    break;
  case USESKILL_FAIL_REDJAMSTONE:
    notice = LookupMessage(MSI_NEED_REDJAMSTONE);
    break;
  case USESKILL_FAIL_BLUEJAMSTONE:
    notice = LookupMessage(MSI_NEED_BLUEJAMSTONE);
    break;
  case USESKILL_FAIL_WEIGHTOVER:
    notice = LookupMessage(MSI_USESKILL_FAIL_WEIGHTOVER);
    break;
  case USESKILL_FAIL:
    if (packet->skillId == WL_TETRAVORTEX || packet->skillId == WL_TETRAVORTEX_FIRE || packet->skillId == WL_TETRAVORTEX_WATER || packet->skillId == WL_TETRAVORTEX_WIND || packet->skillId == WL_TETRAVORTEX_GROUND) {
      this->scene->player->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2DF, 0, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2E0, 0, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2E1, 0, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2E2, 0, 0, 0, 0, 0);
    }
    if (!showMessage) {
      return;
    }
    notice = LookupMessage(MSI_USESKILL_FAIL);
    break;
  case USESKILL_FAIL_TOTARGET:
    notice = LookupMessage(MSI_USESKILL_FAIL_TOTARGET);
    break;
  case USESKILL_FAIL_ANCILLA_NUMOVER:
    notice = LookupMessage(MSI_USESKILL_FAIL_ANCILLA_NUMOVER);
    break;
  case USESKILL_FAIL_HOLYWATER:
    notice = LookupMessage(MSI_USESKILL_FAIL_HOLYWATER);
    break;
  case USESKILL_FAIL_ANCILLA:
    notice = LookupMessage(MSI_USESKILL_FAIL_ANCILLA);
    break;
  case USESKILL_FAIL_DUPLICATE_RANGEIN:
    notice = LookupMessage(MSI_USESKILL_FAIL_DUPLICATE_RANGEIN);
    break;
  case USESKILL_FAIL_NEED_OTHER_SKILL:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_OTHER_SKILL);
    break;
  case USESKILL_FAIL_NEED_HELPER:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_HELPER);
    break;
  case USESKILL_FAIL_INVALID_DIR:
    notice = LookupMessage(MSI_USESKILL_FAIL_INVALID_DIR);
    break;
  case USESKILL_FAIL_SUMMON:
    notice = LookupMessage(MSI_USESKILL_FAIL_SUMMON);
    break;
  case USESKILL_FAIL_SUMMON_NONE:
    notice = LookupMessage(MSI_USESKILL_FAIL_SUMMON_NONE);
    break;
  case USESKILL_FAIL_IMITATION_SKILL_NONE:
    notice = LookupMessage(MSI_USESKILL_FAIL_IMITATION_SKILL_NONE);
    break;
  case USESKILL_FAIL_DUPLICATE:
    notice = LookupMessage(MSI_USESKILL_FAIL_DUPLICATE);
    break;
  case USESKILL_FAIL_CONDITION:
    notice = LookupMessage(MSI_USESKILL_FAIL_CONDITION);
    break;
  case USESKILL_FAIL_PAINTBRUSH:
    notice = LookupMessage(MSI_USESKILL_FAIL_PAINTBRUSH);
    break;
  case USESKILL_FAIL_DRAGON:
    notice = LookupMessage(MSI_USESKILL_FAIL_DRAGON);
    break;
  case USESKILL_FAIL_POS:
    notice = LookupMessage(MSI_USESKILL_FAIL_POS);
    break;
  case USESKILL_FAIL_HELPER_SP_INSUFFICIENT:
    notice = LookupMessage(MSI_USESKILL_FAIL_HELPER_SP_INSUFFICIENT);
    break;
  case USESKILL_FAIL_NEER_WALL:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEER_WALL);
    break;
  case USESKILL_FAIL_NEED_EXP_1PERCENT:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_EXP_1PERCENT);
    break;
  case USESKILL_FAIL_CHORUS_SP_INSUFFICIENT:
    notice = LookupMessage(MSI_USESKILL_FAIL_CHORUS_SP_INSUFFICIENT);
    break;
  case USESKILL_FAIL_GC_WEAPONBLOCKING:
    notice = LookupMessage(MSI_USESKILL_FAIL_GC_WEAPONBLOCKING);
    break;
  case USESKILL_FAIL_GC_POISONINGWEAPON:
    notice = LookupMessage(MSI_USESKILL_FAIL_GC_POISONINGWEAPON);
    break;
  case USESKILL_FAIL_MADOGEAR:
    notice = LookupMessage(MSI_USESKILL_FAIL_MADOGEAR);
    break;
  case USESKILL_FAIL_NEED_EQUIPMENT_KUNAI:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_EQUIPMENT_KUNAI);
    break;
  case USESKILL_FAIL_TOTARGET_PLAYER:
    notice = LookupMessage(MSI_USESKILL_FAIL_TOTARGET_PLAYER);
    break;
  case USESKILL_FAIL_SIZE:
    notice = LookupMessage(MSI_USESKILL_FAIL_SIZE);
    break;
  case USESKILL_FAIL_CANONBALL:
    notice = LookupMessage(MSI_USESKILL_FAIL_CANONBALL);
    break;
  case USESKILL_FAIL_MADOGEAR_HOVERING:
    notice = LookupMessage(MSI_USESKILL_FAIL_MADOGEAR_HOVERING);
    break;
  case USESKILL_FAIL_GUILLONTINE_POISON:
    notice = LookupMessage(MSI_USESKILL_FAIL_GUILLONTINE_POISON);
    break;
  case USESKILL_FAIL_MADOGEAR_RIDE:
    notice = LookupMessage(MSI_USESKILL_FAIL_MADOGEAR_RIDE);
    break;
  case USESKILL_FAIL_SPELLBOOK:
    notice = LookupMessage(MSI_USESKILL_FAIL_SPELLBOOK);
    break;
  case USESKILL_FAIL_SPELLBOOK_DIFFICULT_SLEEP:
    notice = LookupMessage(MSI_USESKILL_FAIL_SPELLBOOK_DIFFICULT_SLEEP);
    break;
  case USESKILL_FAIL_SPELLBOOK_PRESERVATION_POINT:
    notice = LookupMessage(MSI_USESKILL_FAIL_SPELLBOOK_PRESERVATION_POINT);
    break;
  case USESKILL_FAIL_SPELLBOOK_READING:
    notice = LookupMessage(MSI_USESKILL_FAIL_SPELLBOOK_READING);
    break;
  case USESKILL_FAIL_CART:
    notice = LookupMessage(MSI_USESKILL_FAIL_CART);
    break;
  case USESKILL_FAIL_NO_MORE_SPELL:
    notice = LookupMessage(MSI_USESKILL_FAIL_NO_MORE_SPELL);
    break;
  case USESKILL_FAIL_NEED_ITEM: {
    ITEM_INFO item;
    item.SetItemId(packet->itemId);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_USESKILL_FAIL_NEED_ITEM), item.DisplayName(0).c_str(), packet->value).c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case USESKILL_FAIL_NEED_EQUIPMENT: {
    ITEM_INFO item;
    item.SetItemId(packet->itemId);
    item.identified = 1;
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_USESKILL_FAIL_NEED_EQUIPMENT), item.DisplayName(0).c_str()).c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case USESKILL_FAIL_COMBOSKILL:
    if (packet->value == 0) {
      std::sprintf(text, LookupMessage(MSI_USESKILL_FAIL_COMBOSKILL2));
    } else {
      std::sprintf(text, LookupMessage(MSI_USESKILL_FAIL_COMBOSKILL), CSkillInfo::DisplayName(*g_session.m_hotkeyScript, packet->value));
    }
    notice = text;
    break;
  case USESKILL_FAIL_SPIRITS:
    if (packet->value == 0) {
      std::sprintf(text, LookupMessage(MSI_USESKILL_FAIL_SPIRITS2));
    } else {
      std::sprintf(text, LookupMessage(MSI_USESKILL_FAIL_SPIRITS), packet->value);
    }
    notice = text;
    break;
  case USESKILL_FAIL_NEED_EQUIPPED_WEAPON_CLASS: {
    const int reason = packet->value;
    if (reason == 0x15) {
      notice = LookupMessage(MSI_GUILD_STORAGE_WARNING);
    } else if (reason == 0x11) {
      notice = LookupMessage(MSI_MERCHANT_ITEM_BUY_LOG);
    } else if (reason == 0x12) {
      notice = LookupMessage(MSI_MERCHANT_SOLD_ITEM_LOG);
    } else if (reason == 0x14) {
      notice = LookupMessage(MSI_FAIL_NEED_EQUIPPED_PROPERTY_SAINT_BULLET);
    } else if (reason == 0x13) {
      notice = LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_HANDGUN);
    } else if (reason == 0xB) {
      notice = LookupMessage(MSI_FAIL_NEED_EQUIPPED_BOW);
    } else if (reason == 0xD || reason == 0xE) {
      notice = LookupMessage(MSI_FAIL_NEED_EQUIPPED_INSTRUMENT_WHIP);
    } else if (reason == 4) {
      notice = LookupMessage(MSI_FAIL_NEED_EQUIPPED_SPEAR);
    } else if (reason == 0x16) {
      notice = LookupMessage(MSI_FAIL_NEED_EQUIPPED_SYURIKEN);
    } else {
      return;
    }
    break;
  }
  case USESKILL_FAIL_EL_SUMMON:
    notice = LookupMessage(MSI_USESKILL_FAIL_EL_SUMMON);
    break;
  case USESKILL_FAIL_RELATIONGRADE:
    notice = LookupMessage(MSI_USESKILL_FAIL_RELATIONGRADE);
    break;
  case USESKILL_FAIL_STYLE_CHANGE_FIGHTER:
    notice = LookupMessage(MSI_USESKILL_FAIL_STYLE_CHANGE_FIGHTER);
    break;
  case USESKILL_FAIL_STYLE_CHANGE_GRAPPLER:
    notice = LookupMessage(MSI_USESKILL_FAIL_STYLE_CHANGE_GRAPPLER);
    break;
  case USESKILL_FAIL_THERE_ARE_NPC_AROUND:
    notice = LookupMessage(MSI_USESKILL_FAIL_THERE_ARE_NPC_AROUND);
    break;
  case USESKILL_FAIL_NEED_MORE_BULLET:
    notice = LookupMessage(MSI_STEAMAGENCY_ERROR);
    break;
  case USESKILL_FAIL_COINS:
    std::memset(text, 0, sizeof(text));
    if (packet->value > 0) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_RIFLE), packet->value);
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_SHOTGUN));
    }
    notice = text;
    break;
  case USESKILL_FAIL_SH_TEMPORARY_COMMUNION:
    notice = LookupMessage(packet->value);
    break;
  case USESKILL_FAIL_MAP:
    notice = LookupMessage(MSI_SECOND_PASSWORD_INFO_4);
    break;
  case USESKILL_FAIL_NEED_SUMMON_SP:
    notice = LookupMessage(MSI_SET_ITEMS_TO_MAIL_FAILED);
    break;
  case USESKILL_FAIL_SAME_VEND:
    notice = LookupMessage(MSI_FAILED_PET_AUTO_FEEDING_CLOSE_RODEX);
    break;
  case USESKILL_FAIL_NEED_BULLETS: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_CASTING), packet->value).c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case USESKILL_FAIL_NEED_ARROWS: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_NO_ITEM), packet->value).c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case USESKILL_FAIL_NEED_PARTY: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_CANNOT_OFFER_GUILD_EXP_MAX_LEVEL), packet->value).c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case USESKILL_FAIL_NEED_PARTY_LEADER:
    notice = LookupMessage(MSI_STORE_ASSISTANT_TRADE_FORCED_REMOVAL_CONTENT);
    break;
  case USESKILL_FAIL_NEED_PARTNER_SP:
    notice = LookupMessage(MSI_CAN_NOT_OFFER_EXP_LIMIT_RATE);
    break;
  case USESKILL_FAIL_NEED_WEAPON:
    notice = LookupMessage(MSI_DRAW_EMBLEM_FRAME_ONOFF);
    break;
  case USESKILL_FAIL_NEED_ENERGY_SPHERE:
    notice = LookupMessage(MSI_USESKILL_FAIL_GC_WEAPONBLOCK_ON);
    break;
  case USESKILL_FAIL_NEED_WEAPON_BLOCKING:
    notice = LookupMessage(MSI_RUSSIA_PERSONALINFO_DROP);
    break;
  case USESKILL_FAIL_MAX_TRAPS: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_EMBLEM_REGIST_FAILED_INCORRECT_SIZE), packet->value).c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case USESKILL_FAIL_NEED_MAGIC_SPELLBOOK:
    notice = LookupMessage(MSI_USE_WORLDMAP_WITH_PERIOD_MSG3);
    break;
  case USESKILL_FAIL_AP_INSUFFICIENT:
    notice = LookupMessage(MSI_LAPINE_WARNING_HAS_CARD_OR_ENCHANTED);
    break;
  case USESKILL_FAIL_NEED_SERVANT_WEAPON: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_CHANGE_MATERIAL_SEARCHTEXT), packet->value).c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case USESKILL_FAIL_NEED_TWINKLING_GALAXY:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_GATLING_OR_SHOTGUN);
    break;
  case USESKILL_FAIL_NEED_SOUL_ENGERY:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_RIFLE_OR_HANDGUN);
    break;
  case USESKILL_FAIL_NEED_AMULET:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_GRANADE_OR_RIFLE);
    break;
  case USESKILL_FAIL_NEED_GATLING_SHOTGUT:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_GATLING_OR_HANDGUN);
    break;
  case USESKILL_FAIL_NEED_RIFLE_REVOLVER:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_SHOTGUN_OR_GRANADE);
    break;
  case USESKILL_FAIL_NEED_GLAUNCHER_RIFLE:
    notice = LookupMessage(MSI_USESKILL_FAIL_NEED_SHIELD_WEAPON);
    break;
  case USESKILL_FAIL_NEED_GATLING_REVOLVER:
    notice = LookupMessage(MSI_CLOSE_BANKING_FOR_EXPANDED_BARTERMARKET);
    break;
  case USESKILL_FAIL_NEED_SHOTGUN_GLAUNCHER:
    notice = LookupMessage(MSI_CANNOT_OPEN_BANKING_DURING_EXPANDED_BARTERMARKET);
    break;
  case USESKILL_FAIL_NEED_SHIELD:
    notice = LookupMessage(MSI_FAIL_NO_SPACE_BEHIND_THE_TARGET);
    break;
  case USESKILL_FAIL_TARGET_SHADOW_SPACE:
    notice = LookupMessage(MSI_PACKAGEITEM_BURROWFAIL);
    break;
  case USESKILL_FAIL_MAX:
    notice = LookupMessage(MSI_DAMAGE_EQUIPMENT_TOOLTIP);
    break;
  case 113:
    notice = LookupMessage(MSI_MACRO_REPORTER_SELECT_MODE_QUIZ);
    break;
  case 114:
    notice = LookupMessage(MSI_MACRO_REPORTER_TOOLTIP_MAP);
    break;
  case 115:
    notice = LookupMessage(MSI_ERRORPOPUP_DETAIL);
    break;
  case 116:
  case 117:
    notice = LookupMessage(packet->value != 0 ? packet->value : 0x1119);
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice), 0xFF, 0xB, 0, 0);
}
// OFFSET: 0x00D27420
// FUNCTION: fn_21690
void CGameMode::ReceiveChatRoomMemberList(const unsigned char* data) {
  const ChatRoomMemberListPacket* packet = reinterpret_cast<const ChatRoomMemberListPacket*>(data);
  this->isWaitingEnterRoom = 0;
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(this->chatRoomActorIds[packet->roomId]));

  ChatRoomTitleInfo info;
  bool joined = false;
  if (actor) {
    if (UIChatRoomTitle* room = reinterpret_cast<UIChatRoomTitle*>(actor->actorWindows[1])) {
      info.title = room->title;
      info.password = room->password;
      info.type = room->roomType;
      info.memberCount = room->memberCount;
      info.memberLimit = room->memberLimit;
      info.id = room->roomId;
      if (packet->roomId == room->roomId) {
        joined = true;
      }
    }
    actor->SendMsg(nullptr, AM_DELETE_CHAT_TITLE, 0, 0, 0, 0, 0, 0);
    if (joined) {
      const int count = static_cast<unsigned>(packet->length - sizeof(ChatRoomMemberListPacket)) / sizeof(ChatRoomMemberRecord);
      std::list<std::pair<std::string, int>> members;
      const ChatRoomMemberRecord* records = reinterpret_cast<const ChatRoomMemberRecord*>(data + sizeof(ChatRoomMemberListPacket));
      for (int i = 0; i < count; ++i) {
        const ChatRoomMemberRecord* record = &records[i];
        std::pair<std::string, int> member;
        char name[25];
        name[24] = '\0';
        std::memcpy(name, record->name, sizeof(record->name));
        member.first = name;
        member.second = record->role;
        members.push_back(member);
      }

      UIWindow* window = g_windowMgr.EnsureWindow(WID_CHATROOMWND);
      window->SendMsg(nullptr, UM_CLEARMEMBER, 0, 0, 0, 0);
      info.memberCount = static_cast<int>(members.size());
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      for (std::list<std::pair<std::string, int>>::iterator it = members.begin(); it != members.end(); ++it) {
        window->SendMsg(nullptr, UM_ADDMEMBER, reinterpret_cast<std::intptr_t>(it->first.c_str()), it->second, 0, 0);
      }

      this->chatRoomTitle = info.title;
      this->chatRoomPassword = info.password;
      this->chatRoomPublic = info.type;
      this->chatRoomMemberCount = info.memberCount;
      this->chatRoomMemberLimit = info.memberLimit;
      this->chatRoomId = info.id;
    }
  }
}

// GLOBAL: gl_25381
std::string g_gameModeNamelessTag = "<NAMELESS>";

// GLOBAL: gl_25955
std::string g_guildMemberNamelessTag = "<NAMELESS>";

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
  this->pickupReqItemNaidList.erase(this->pickupReqItemNaidList.begin(), this->pickupReqItemNaidList.end());

  switch (packet->result) {
  case PICKUP_SUCCEED: {
    const std::uint16_t index = packet->index;
    const std::uint16_t amount = packet->amount;
    const int itemId = packet->itemId;
    const unsigned char identified = packet->identified;
    const unsigned char damaged = packet->damaged;
    const unsigned char refine = packet->refine;
    ItemCardSlots slots;
    std::memcpy(slots.cards, packet->cards, sizeof(slots.cards));
    const std::uint32_t location = packet->location;
    const unsigned char type = packet->type;
    const std::uint32_t expireTime = packet->expireTime;
    const std::uint16_t bindOnEquip = packet->bindOnEquip;
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    const unsigned char favorite = packet->favorite;
    const std::uint16_t look = packet->look;
    ITEM_INFO item;
    item.identity[0] = type;
    item.identity[2] = location;
    item.identity[1] = index;
    item.identity[3] = 0;
    item.quantity = amount;
    item.SetItemId(itemId);
    std::memcpy(item.cards, slots.cards, sizeof(slots.cards));
    item.identified = identified;
    item.damaged = damaged;
    item.refiningLevel = refine;
    item.expireTime = expireTime;
    item.bindOnEquip = bindOnEquip;
    item.flags[1] = 1;
    item.optionCount = CountStoreItemOptions(options);
    for (int option = 0; option < item.optionCount; ++option) {
      reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
    }
    item.flags[0] = favorite;
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
    }
    if (type == 10 || type == 16 || type == 17 || type == 19) {
      item.identity[3] = 0x8000;
    }
    item.sprite = look;
    item.grade = 0;

    g_session.AddInventoryItem(item, 1);
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0
    );

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  case RECEIVE_ITEM: {
    const std::uint16_t index = packet->index;
    int amount = packet->amount;
    const int itemId = packet->itemId;
    const unsigned char identified = packet->identified;
    const unsigned char damaged = packet->damaged;
    const unsigned char refine = packet->refine;
    ItemCardSlots slots;
    std::memcpy(slots.cards, packet->cards, sizeof(slots.cards));
    const std::uint32_t location = packet->location;
    const unsigned char type = packet->type;
    const std::uint32_t expireTime = packet->expireTime;
    const std::uint16_t bindOnEquip = packet->bindOnEquip;
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    const unsigned char favorite = packet->favorite;
    const std::uint16_t look = packet->look;
    ITEM_INFO item;
    item.identity[0] = type;
    item.identity[2] = location;
    item.identity[1] = index;
    item.identity[3] = 0;
    item.quantity = amount;
    item.SetItemId(itemId);
    std::memcpy(item.cards, slots.cards, sizeof(slots.cards));
    item.identified = identified;
    item.damaged = damaged;
    item.refiningLevel = refine;
    item.expireTime = expireTime;
    item.bindOnEquip = bindOnEquip;
    item.flags[1] = 1;
    item.optionCount = CountStoreItemOptions(options);
    for (int option = 0; option < item.optionCount; ++option) {
      reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
    }
    item.flags[0] = favorite;
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
    }
    if (type == 10 || type == 16 || type == 17 || type == 19) {
      item.identity[3] = 0x8000;
    }
    switch (type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      break;
    default:
      item.identity[2] = 0;
      break;
    }
    item.sprite = look;
    item.grade = 0;

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    if (current.identity[1] > 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  default:
    this->NotifyItemResult(packet->result);
    break;
  }
}

// OFFSET: 0x00D309D0
// FUNCTION: fn_21732
void CGameMode::ReceiveItemPickupAck9(const unsigned char* data) {
  const ItemPickupAckV9Packet* packet = reinterpret_cast<const ItemPickupAckV9Packet*>(data);
  this->pickupReqItemNaidList.erase(this->pickupReqItemNaidList.begin(), this->pickupReqItemNaidList.end());

  switch (packet->result) {
  case PICKUP_SUCCEED: {
    {
      const std::uint16_t index = packet->index;
      const std::uint16_t amount = packet->amount;
      const int itemId = packet->itemId;
      const unsigned char identified = packet->identified;
      const unsigned char damaged = packet->damaged;
      const unsigned char refine = packet->refine;
      ItemCardSlots slots;
      std::memcpy(slots.cards, packet->cards, sizeof(slots.cards));
      const std::uint32_t location = packet->location;
      const unsigned char type = packet->type;
      const std::uint32_t expireTime = packet->expireTime;
      const std::uint16_t bindOnEquip = packet->bindOnEquip;
      StoreItemOption options[5];
      std::memcpy(options, packet->options, sizeof(options));
      const unsigned char favorite = packet->favorite;
      const std::uint16_t look = packet->look;
      const unsigned char grade = packet->grade;
      ITEM_INFO item;
      item.identity[0] = type;
      item.identity[2] = location;
      item.identity[1] = index;
      item.identity[3] = 0;
      item.quantity = amount;
      item.SetItemId(itemId);
      std::memcpy(item.cards, slots.cards, sizeof(slots.cards));
      item.identified = identified;
      item.damaged = damaged;
      item.refiningLevel = refine;
      item.expireTime = expireTime;
      item.bindOnEquip = bindOnEquip;
      item.flags[1] = 1;
      item.optionCount = CountStoreItemOptions(options);
      for (int option = 0; option < item.optionCount; ++option) {
        reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
      }
      item.flags[0] = favorite;
      if (std::atoi(item.itemId.c_str()) == 607) {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
      }
      if (type == 10 || type == 16 || type == 17 || type == 19) {
        item.identity[3] = 0x8000;
      }
      item.sprite = look;
      item.grade = grade;

      g_session.AddInventoryItem(item, 1);
      g_windowMgr.SendNotice(
        UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0
      );

      UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
      window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
        quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
      }
    }

    if (this->recordItemPickups == 1) {
      auto found = this->itemPickupCounts.find(packet->itemId);
      if (found == this->itemPickupCounts.end()) {
        this->itemPickupCounts.insert(std::make_pair(packet->itemId, packet->amount));
      } else {
        found->second += packet->amount;
      }
    }
    break;
  }
  case RECEIVE_ITEM: {
    const std::uint16_t index = packet->index;
    int amount = packet->amount;
    const int itemId = packet->itemId;
    const unsigned char identified = packet->identified;
    const unsigned char damaged = packet->damaged;
    const unsigned char refine = packet->refine;
    ItemCardSlots slots;
    std::memcpy(slots.cards, packet->cards, sizeof(slots.cards));
    const std::uint32_t location = packet->location;
    const unsigned char type = packet->type;
    const std::uint32_t expireTime = packet->expireTime;
    const std::uint16_t bindOnEquip = packet->bindOnEquip;
    StoreItemOption options[5];
    std::memcpy(options, packet->options, sizeof(options));
    const unsigned char favorite = packet->favorite;
    const std::uint16_t look = packet->look;
    const unsigned char grade = packet->grade;
    ITEM_INFO item;
    item.identity[0] = type;
    item.identity[2] = location;
    item.identity[1] = index;
    item.identity[3] = 0;
    item.quantity = amount;
    item.SetItemId(itemId);
    std::memcpy(item.cards, slots.cards, sizeof(slots.cards));
    item.identified = identified;
    item.damaged = damaged;
    item.refiningLevel = refine;
    item.expireTime = expireTime;
    item.bindOnEquip = bindOnEquip;
    item.flags[1] = 1;
    item.optionCount = CountStoreItemOptions(options);
    for (int option = 0; option < item.optionCount; ++option) {
      reinterpret_cast<StoreItemOption*>(item.options)[option] = options[option];
    }
    item.flags[0] = favorite;
    if (std::atoi(item.itemId.c_str()) == 607) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->lastPickItemTime = 0;
    }
    if (type == 10 || type == 16 || type == 17 || type == 19) {
      item.identity[3] = 0x8000;
    }
    switch (type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      break;
    default:
      item.identity[2] = 0;
      break;
    }
    item.sprite = look;
    item.grade = grade;

    const ITEM_INFO current = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
    if (current.identity[1] > 0) {
      amount -= current.quantity;
    }
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_GET_ITEM), item.DisplayName(0).c_str(), amount).c_str()), 0xFFFF00, 6, 0, 0);

    UIWindow* window = g_windowMgr.EnsureWindow(WID_NOTIFYITEMOBTAINWND);
    window->SendMsg(nullptr, UM_SET_IDENTIFIY_INFO, item.identified, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETTITLE, 0, amount, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), 0);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    if (*reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) != 0) {
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
    if (UIWindow* quest = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      quest->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 1, 0);
    }
    break;
  }
  default:
    this->NotifyItemResult(packet->result);
    break;
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
  const std::uint32_t& aid = *reinterpret_cast<const std::uint32_t*>(data + 4);
  CGameActor* speaker = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (speaker && !speaker->objectType && g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked("");
    g_forbiddenWords.Mask(text.data(), &masked);
  }

  if (g_windowMgr.chatRoomWnd) {
    g_windowMgr.SendNotice(UIM_PUSH_INTO_CHATROOM, reinterpret_cast<std::intptr_t>(text.data()), 0x222222, 0, 0, 0);
  } else {
    if (IsMonsterJob(aid)) {
      g_windowMgr.SendNotice(
        UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), g_serviceType == 5 ? 0xFFFF00 : (g_serviceType == 10 ? 0x1D8DFF : 0xFFFF), 1, 0, 0
      );
    } else if (MonsterJobColor(aid)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), MonsterJobColor(aid), 0, 0, 0);
    } else if (!g_serviceOptions.Get(0x72) && !g_serviceOptions.Get(0x94)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), 0xFFFFFF, 1, 0, 0);
    }
    if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
      actor->SendMsg(nullptr, AM_CHAT, reinterpret_cast<int>(text.data()), 0, 0, 0, 0, 0);
    }
  }
  if (g_session.m_whisperRecord != 0) {
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
    std::string masked("");
    g_forbiddenWords.Mask(text.data(), &masked);
  }

  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(data + 4);
  if (aid == g_session.m_aid) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), 0xC8FF, 3, 0, 0);
    this->scene->player->SendMsg(nullptr, AM_CHAT, reinterpret_cast<int>(text.data()), 0, 0, 0, 0, 0);
  } else {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), 0xC8C8FF, 3, 0, 0);
    if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
      actor->SendMsg(nullptr, AM_CHAT, reinterpret_cast<int>(text.data()), 0, 0, 0, 0, 0);
    }
  }
  if (g_session.m_whisperRecord != 0) {
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
  if (*reinterpret_cast<int*>(&g_session.interactionFlags[0x8]) == 0 && this->scene->isPKZone == 0) {
    return;
  }

  vector3d deltaPos{0.0f, 0.0f, 0.0f};
  CGameActor* actor;
  if (packet->aid == g_session.m_aid && (actor = this->scene->player) != nullptr) {
    if (packet->count == 1) {
      if (static_cast<CPlayer*>(actor)->pkRank != 1) {
        g_session.m_timeAttackStart = timeGetTime();
      }
    } else {
      g_session.m_timeAttackStart = 0;
    }

    const int previous = this->scene->player->pkRank;
    const int count = packet->count;
    if (previous != count) {
      if (previous > 10) {
        if (count <= 10) {
          char text[260];
          std::sprintf(text, LookupMessage(MSI_RANK_IN_TEN), g_session.DecodePlayerName(), count);
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
        }
      } else if (count > 10) {
        char text[260];
        std::sprintf(text, LookupMessage(MSI_RANK_OUT_TEN), g_session.DecodePlayerName(), count);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
      }
      CPlayer* player = this->scene->player;
      g_soundMgr->PlayWave("effect\\number_change.wav", player->render.position.x, 0.0f, player->render.position.z, 250, 40, 1.0f, 0);
    }
    this->scene->player->pkTotal = packet->value;
    this->scene->player->pkRank = packet->count;
  } else {
    actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
    const std::uint32_t state = actor->GetStateFlags();
    if (StateFlag02(state) || StateFlag01(state) || StateFlag06(state)) {
      return;
    }
  }

  if (!IsAdminAccount(actor->render.accountId) || g_session.IsAdminSubMode(7)) {
    actor->SendMsg(nullptr, AM_CHANGE_RANK, packet->value, packet->count, 0, 0, 0, 0);
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0x9F, 0, 0, 0, 0, 0);
    if (packet->count <= 10) {
      if (CRagEffect* effect = actor->LaunchEffect(0x9F, deltaPos, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, packet->count, 0, 0, 0, 0);
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
  guildInfo->bannedMembers.clear();

  for (int i = 0; i < count; ++i) {
    const GuildBanRecord* record = &reinterpret_cast<const GuildBanRecord*>(data + sizeof(PacketLengthHeader))[i];
    char name[25];
    std::memcpy(name, record->name, sizeof(record->name));
    name[24] = '\0';
    char reason[41];
    std::memcpy(reason, record->reason, sizeof(record->reason));
    reason[40] = '\0';

    GuildInfo::Unknown00ECEntry entry;
    entry.name = name;
    entry.reason = reason;
    guildInfo->bannedMembers.push_back(entry);
  }

  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D4BDD0
// FUNCTION: fn_21867
void CGameMode::ReceiveGuildBanList2(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  const PacketLengthHeader* packet = reinterpret_cast<const PacketLengthHeader*>(data);
  const int count = (packet->length - sizeof(PacketLengthHeader)) / sizeof(GuildBanCharacterRecord);
  guildInfo->bannedMembers.clear();

  const GuildBanCharacterRecord* records = reinterpret_cast<const GuildBanCharacterRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const GuildBanCharacterRecord* record = &records[i];
    char reason[41];
    std::memcpy(reason, record->reason, sizeof(record->reason));
    reason[40] = '\0';

    GuildInfo::Unknown00ECEntry entry;
    entry.characterId = record->characterId;
    entry.name = LookupPlayerName(entry.characterId);
    entry.reason = reason;
    if (entry.name.size() == 0) {
      entry.name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
    }
    if (entry.characterId != 0) {
      guildInfo->bannedMembers.push_back(entry);
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
  guildInfo->bannedMembers.clear();

  const GuildBanNamedRecord* records = reinterpret_cast<const GuildBanNamedRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const GuildBanNamedRecord* record = &records[i];
    char reason[41];
    std::memcpy(reason, record->reason, sizeof(record->reason));
    reason[40] = '\0';

    GuildInfo::Unknown00ECEntry entry;
    entry.characterId = record->characterId;
    entry.reason = reason;
    if (!g_playerNameDictionary->Contains(record->characterId)) {
      const int characterId = record->characterId;
      std::string name(record->name, record->name + sizeof(record->name));
      SetPlayerName(characterId, name.c_str());
    }
    entry.name = LookupPlayerName(entry.characterId);
    if (entry.name.size() == 0) {
      entry.name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
    }
    if (entry.characterId != 0) {
      guildInfo->bannedMembers.push_back(entry);
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
    std::string masked("");
    g_forbiddenWords.Mask(text.data(), &masked);
  }
  if (!g_serviceOptions.Get(0x94)) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.data()), 0xB4FFB4, 4, 0, 0);
    if (g_session.m_whisperRecord != 0) {
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
  act.attackMotion = -1;
  act.damageMotion = -1;
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

  if (actor->render.state == 0 && static_cast<CGameActor*>(actor)->hitTargetAid != targetAid && act.type != 1 && act.type != 2 && act.type != 3) {
    if (sourceAid == g_session.m_aid) {
      if (this->scene->GetGameActorByAID(g_session.petGid)) {
        const int hungerLevel = g_session.GetPetHungryState(g_session.petHunger);
        const int action = g_session.GetPetActionNumber(hungerLevel, g_session.GetPetFriendlyState(g_session.m_petFriendly), 1);
        if (rand() % 100 < 25 && g_session.CheckInterval(60000) == true) {
          this->SendMsg(MM_PET_ACT, g_session.GetPetEmotionNumber(g_session.petJob, 1, hungerLevel), 0, 0, 0);
          this->SendMsg(MM_PET_ACT, action, 0, 0, 0);
        }
      }
    } else {
      char text[256];
      g_session.GetMonsterTalkText(static_cast<CGameActor*>(actor)->GetEffectiveJob(-1), 0, 0, text, sourceAid);
      if (std::strcmp(text, "") != 0) {
        actor->SendMsg(nullptr, AM_CHAT, reinterpret_cast<int>(text), 0, 0, 0, 0, 0);
        if (!g_serviceOptions.Get(114) && !g_serviceOptions.Get(148)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFAFAFA, 0, 0, 0);
        }
      }
    }
  }
  const std::int64_t value = reinterpret_cast<std::intptr_t>(&act);
  actor->SendMsg(nullptr, AM_ACTION, value, 0, 0, 0, 0, 0);
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
  g_session.EraseSummonHp(packet->aid);
  if (g_session.elementalGid == packet->aid) {
    g_session.SetElementalGid(-1);
  }

  switch (packet->type) {
  case CLR_OUTSIGHT:
    if (CRenderObject* actor = this->scene->GetGameActorByAID(packet->aid)) {
      actor->SendMsg(nullptr, AM_OUTOFSIGHT, 0, 0, 0, 0, 0, 0);
      this->SendMsg(MM_DELETEREGISTERPOS, packet->aid, 0, 0, 0);
    }
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(MM_DELETEREGISTERPOS, packet->aid, 0, 0, 0);
    }
    if (g_windowMgr.messengerGroupWnd) {
      g_windowMgr.messengerGroupWnd->SendMsg(nullptr, UM_COMMAND, 34, packet->aid, 0, 0);
    }
    break;
  case CLR_DEAD: {
    if (CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid))) {
      CPlayer* player = this->scene->player;
      if (packet->aid == static_cast<std::uint32_t>(player->proceedTargetGid)) {
        player->proceedTargetGid = 0;
      }
      actor->SendMsg(nullptr, AM_WILL_BE_DEAD, 0, 0, 0, 0, 0, 0);
      const int job = actor->GetEffectiveJob(-1);
      if (g_session.IsMonsterClass(job) || actor->objectType == 9 || g_session.IsCompanionJob(job) ||
          actor->GetEffectiveJob(-1) == 0x235) {
        this->SendMsg(MM_DELETEREGISTERPOS, packet->aid, 0, 0, 0);
      }
      if (g_windowMgr.buyingShopPurchaseWnd) {
        g_windowMgr.buyingShopPurchaseWnd->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
      }
      if (g_windowMgr.merchantItemPurchaseWnd) {
        g_windowMgr.merchantItemPurchaseWnd->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
      }
    }
    if (packet->aid != g_session.m_aid) {
      break;
    }

    this->scene->player->SendMsg(nullptr, AM_CANCELATTACK, 0, 0, 0, 0, 0, 0);
    this->scene->player->SendMsg(nullptr, AM_WILL_BE_DEAD, 0, 0, 0, 0, 0, 0);
    this->isOnQuest = 1;
    this->isPlayerDead = 1;
    g_windowMgr.CloseWindow(WID_QUITWND);
    g_windowMgr.CloseWindow(WID_SAYDIALOGWND);
    g_windowMgr.CloseWindow(WID_CHOOSEWND);
    g_windowMgr.CloseWindow(WID_CHOOSESELLBUYWND);
    g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
    g_windowMgr.CloseWindow(WID_ITEMPURCHASEWND);
    g_windowMgr.CloseWindow(WID_ITEMSELLWND);
    g_windowMgr.CloseWindow(WID_RUNESYSTEMWND);
    if (g_windowMgr.merchantShopMakeWnd) {
      g_windowMgr.merchantShopMakeWnd->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    if (g_windowMgr.buyingShopMakeWnd) {
      g_windowMgr.buyingShopMakeWnd->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    if (g_windowMgr.merchantItemMyShopWnd) {
      g_windowMgr.merchantItemMyShopWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
    }
    if (g_windowMgr.buyingShopMyItemsWnd) {
      g_windowMgr.buyingShopMyItemsWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
    }
    if (g_windowMgr.merchantItemPurchaseWnd) {
      g_windowMgr.merchantItemPurchaseWnd->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    if (g_windowMgr.buyingShopPurchaseWnd) {
      g_windowMgr.buyingShopPurchaseWnd->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    }
    if (this->scene->GetGameActorByAID(g_session.petGid)) {
      const int hungerLevel = g_session.GetPetHungryState(g_session.petHunger);
      const int action = g_session.GetPetActionNumber(hungerLevel, g_session.GetPetFriendlyState(g_session.m_petFriendly), 3);
      this->SendMsg(MM_PET_ACT, g_session.GetPetEmotionNumber(g_session.petJob, 3, hungerLevel), 0, 0, 0);
      this->SendMsg(MM_PET_ACT, action, 0, 0, 0);
    }
    if (g_serviceType != 4) {
      if (!g_windowMgr.CloseWindow(WID_ESCOPTIONWND)) {
        g_windowMgr.EnsureWindow(WID_ESCOPTIONWND);
      }
    } else {
      g_windowMgr.Confirm(LookupMessage(MSI_PRESS_ESC_TO_RESTART), 0, true, false, 0, "Message", 280, 120, 0);
    }
    RefreshPartyWindows(&g_session);
    break;
  }
  case CLR_RESPAWN:
    if (CRenderObject* actor = this->scene->GetGameActorByAID(packet->aid)) {
      this->LaunchDummyEffect(&actor->render.position, 0x13A, 600, 0);
    }
    if (g_windowMgr.merchantItemShopWnd && static_cast<UIMerchantItemShopWnd*>(g_windowMgr.merchantItemShopWnd)->ownerAid == static_cast<int>(packet->aid)) {
      this->SendMsg(MM_RELEASE_QUEST_SH, 0, 0, 0, 0);
      g_session.ClearItems1760And1748();
    }
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(MM_DELETEREGISTERPOS, packet->aid, 0, 0, 0);
    }
    break;
  case CLR_TELEPORT:
    if (CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid))) {
      if (!StateFlag06(actor->GetStateFlags())) {
        this->LaunchDummyEffect(&actor->render.position, 0x130, 300, 0);
      }
    }
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(MM_DELETEREGISTERPOS, packet->aid, 0, 0, 0);
    }
    if (g_windowMgr.messengerGroupWnd) {
      g_windowMgr.messengerGroupWnd->SendMsg(nullptr, UM_COMMAND, 34, packet->aid, 0, 0);
    }
    break;
  default:
    this->scene->GetGameActorByAID(packet->aid);
    if (this->scene->RemoveItem(packet->aid) == true) {
      this->SendMsg(MM_DELETEREGISTERPOS, packet->aid, 0, 0, 0);
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
    std::string masked("");
    g_forbiddenWords.Mask(guildName, &masked);
  }

  guildInfo->guildName = guildName;
  g_guildInfo.masterName = masterName;
  g_guildInfo.isMaster = g_guildInfo.masterName == g_session.DecodePlayerName();
  guildInfo->manageLand = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->guildLevel = packet->level;
  guildInfo->memberCount = packet->userCount;
  guildInfo->maxMemberCount = packet->maxUserCount;
  guildInfo->memberAverageLevel = packet->averageLevel;
  guildInfo->exp = packet->exp;
  guildInfo->nextExp = packet->maxExp;
  guildInfo->points = packet->point;
  guildInfo->honor = packet->honor;
  guildInfo->virtue = packet->virtue;
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
    masterName = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
  }

  char guildName[25];
  guildName[24] = '\0';
  char manageLand[25];
  manageLand[24] = '\0';
  std::memcpy(guildName, packet->guildName, sizeof(packet->guildName));
  std::memcpy(manageLand, packet->manageLand, sizeof(manageLand) - 1);
  if (g_forbiddenWords.Contains(guildName)) {
    std::string masked("");
    g_forbiddenWords.Mask(guildName, &masked);
  }

  if (masterName == g_session.DecodePlayerName()) {
    g_guildInfo.isMaster = 1;
  }
  guildInfo->guildName = guildName;
  g_guildInfo.masterName = masterName;
  g_guildInfo.masterCharacterId = packet->masterCharacterId;
  guildInfo->manageLand = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->guildLevel = packet->level;
  guildInfo->onlineMemberCount = packet->userCount;
  guildInfo->maxMemberCount = packet->maxUserCount;
  guildInfo->memberAverageLevel = packet->averageLevel;
  guildInfo->exp = packet->exp;
  guildInfo->nextExp = packet->maxExp;
  guildInfo->points = packet->point;
  guildInfo->honor = packet->honor;
  guildInfo->virtue = packet->virtue;
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
  g_guildInfo.masterName = LookupPlayerName(packet->masterCharacterId);
  if (g_guildInfo.masterName.size() == 0) {
    g_guildInfo.masterName = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
  }

  char guildName[25];
  guildName[24] = '\0';
  char manageLand[25];
  manageLand[24] = '\0';
  if (g_guildInfo.masterName == g_session.DecodePlayerName()) {
    g_guildInfo.isMaster = 1;
  }
  std::memcpy(guildName, packet->guildName, sizeof(packet->guildName));
  std::memcpy(manageLand, packet->manageLand, sizeof(manageLand) - 1);
  if (g_forbiddenWords.Contains(guildName)) {
    std::string masked("");
    g_forbiddenWords.Mask(guildName, &masked);
  }

  guildInfo->guildName = guildName;
  g_guildInfo.masterCharacterId = packet->masterCharacterId;
  guildInfo->manageLand = manageLand;
  g_guildInfo.m_guildId = packet->guildId;
  guildInfo->guildLevel = packet->level;
  guildInfo->onlineMemberCount = packet->userCount;
  guildInfo->maxMemberCount = packet->maxUserCount;
  guildInfo->memberAverageLevel = packet->averageLevel;
  guildInfo->exp = packet->exp;
  guildInfo->nextExp = packet->maxExp;
  guildInfo->points = packet->point;
  guildInfo->honor = packet->honor;
  guildInfo->virtue = packet->virtue;
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
  guildInfo->subject = packet->subject;
  guildInfo->notice = packet->notice;
  if (!g_serviceOptions.Get(0xA5)) {
    return;
  }

  char subject[60];
  std::memset(subject, 0, sizeof(subject));
  strcpy_s(subject, sizeof(subject), packet->subject);
  if (g_forbiddenWords.Contains(subject) == true) {
    std::string masked("");
    g_forbiddenWords.Mask(subject, &masked);
  }
  std::string subjectLine = "[ ";
  subjectLine += subject;
  subjectLine += " ]";
  if (std::strlen(subject) != 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(subjectLine.c_str()), 0x80FFFF, 0, 0, 0);
  }

  char notice[120];
  std::memset(notice, 0, sizeof(notice));
  strcpy_s(notice, sizeof(notice), packet->notice);
  if (g_forbiddenWords.Contains(notice) == true) {
    std::string masked("");
    g_forbiddenWords.Mask(notice, &masked);
  }
  std::string noticeLine = "[ ";
  noticeLine += notice;
  noticeLine += " ]";
  if (std::strlen(notice) != 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(noticeLine.c_str()), 0x80FFFF, 0, 0, 0);
  }

  guildInfo->subject = subject;
  guildInfo->notice = notice;
}

#pragma pack(push, 1)
struct GuildMemberAddV2Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ int accountId;
  /* 0006 */ std::uint32_t characterId;
  /* 000A */ std::int16_t head;
  /* 000C */ std::int16_t headPalette;
  /* 000E */ std::int16_t sex;
  /* 0010 */ std::int16_t job;
  /* 0012 */ std::int16_t level;
  /* 0014 */ int exp;
  /* 0018 */ int online;
  /* 001C */ int positionId;
  /* 0020 */ int lastTimeOffline;
  /* 0024 */ char name[24];
};

struct GuildMemberAddV3Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ int accountId;
  /* 0006 */ std::uint32_t characterId;
  /* 000A */ std::int16_t head;
  /* 000C */ std::int16_t headPalette;
  /* 000E */ std::int16_t sex;
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
  if (guildInfo->membersRequested == 0) {
    return;
  }

  GuildInfo::Member member;
  member.accountId = packet->accountId;
  member.characterId = packet->characterId;
  member.head = packet->head;
  member.headPalette = packet->headPalette;
  member.job = packet->job;
  member.sex = packet->sex;
  member.positionId = packet->positionId;
  member.level = packet->level;
  member.exp = packet->exp;
  member.online = packet->online;
  *reinterpret_cast<int*>(member.lastOfflineTime) = packet->lastTimeOffline;
  if (!g_playerNameDictionary->Contains(packet->characterId)) {
    const std::uint32_t characterId = packet->characterId;
    std::string name(packet->name, packet->name + sizeof(packet->name));
    SetPlayerName(characterId, name.c_str());
  }
  member.name = LookupPlayerName(packet->characterId);
  if (member.name.size() == 0) {
    member.name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
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
  if (guildInfo->membersRequested == 0) {
    return;
  }

  char memo[51];
  std::memcpy(memo, packet->memo, sizeof(packet->memo));
  memo[50] = '\0';

  GuildInfo::Member member;
  member.accountId = packet->accountId;
  member.characterId = packet->characterId;
  member.name = LookupPlayerName(member.characterId);
  member.head = packet->head;
  member.headPalette = packet->headPalette;
  member.job = packet->job;
  member.sex = packet->sex;
  member.positionId = packet->positionId;
  member.level = packet->level;
  member.memo = memo;
  member.exp = packet->exp;
  member.online = packet->online;
  if (member.name.size() == 0) {
    member.name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
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
    /* 0008 */ std::int16_t head;
    /* 000A */ std::int16_t headPalette;
    /* 000C */ std::int16_t sex;
    /* 000E */ std::int16_t job;
    /* 0010 */ std::int16_t level;
    /* 0012 */ int exp;
    /* 0016 */ int online;
    /* 001A */ int positionId;
    /* 001E */ int lastTimeOffline;
    /* 0022 */ char name[24];
  };

  struct GuildMemberMemoOnlyRecord {
    /* 0000 */ int accountId;
    /* 0004 */ std::uint32_t characterId;
    /* 0008 */ std::int16_t head;
    /* 000A */ std::int16_t headPalette;
    /* 000C */ std::int16_t sex;
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
  guildInfo->memberCount = static_cast<int>(count);
  guildInfo->ClearMembers();

  const GuildMemberNamedRecord* records = reinterpret_cast<const GuildMemberNamedRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const GuildMemberNamedRecord* record = &records[i];
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.head = record->head;
    member.headPalette = record->headPalette;
    member.job = record->job;
    member.sex = record->sex;
    member.positionId = record->positionId;
    member.level = record->level;
    member.exp = record->exp;
    member.online = record->online;
    const int lastTimeOffline = record->lastTimeOffline;
    member.characterId = record->characterId;
    member.expRatio = 0.0f;
    *reinterpret_cast<int*>(member.lastOfflineTime) = lastTimeOffline;
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
        member.name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
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
  guildInfo->memberCount = count;
  guildInfo->ClearMembers();

  const GuildMemberMemoOnlyRecord* records = reinterpret_cast<const GuildMemberMemoOnlyRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const GuildMemberMemoOnlyRecord* record = &records[i];
    GuildInfo::Member member;
    member.accountId = record->accountId;
    member.characterId = record->characterId;
    member.head = record->head;
    member.headPalette = record->headPalette;
    member.job = record->job;
    member.sex = record->sex;
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
        member.name = LookupMessage(MSI_NAMELESS_USER) + g_gameModeNamelessTag;
      }
    }

    char memo[51];
    std::memcpy(memo, record->memo, sizeof(record->memo));
    memo[50] = '\0';
    member.memo = memo;
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
  guildInfo->relatedGuilds.clear();

  const GuildRelationRecord* records = reinterpret_cast<const GuildRelationRecord*>(data + sizeof(PacketLengthHeader));
  for (int i = 0; i < count; ++i) {
    const GuildRelationRecord* record = &records[i];
    char name[25];
    std::memcpy(name, record->name, sizeof(record->name));
    name[24] = '\0';
    if (g_forbiddenWords.Contains(name) == true) {
      std::string masked("");
      g_forbiddenWords.Mask(name, &masked);
    }

    GuildInfo::RelatedGuild guild;
    guild.relation = record->relation;
    guild.guildId = record->guildId;
    guild.name = name;
    guildInfo->relatedGuilds.push_back(guild);
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
  /* 00F8 */ unsigned char logFlags;
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
    g_windowMgr.replayRecordControlWnd->SendMsg(nullptr, UM_SETPOS, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_REPLAYRECCONTROLWND);
  } else if (Replay::Instance().state == 7) {
    Replay::Instance().SetState(8);
  }

  unsigned char sameMap;
  if (std::strcmp(mapName.c_str(), g_session.m_navigationPlayerMap) != 0) {
    g_session.SetNavigationPlayerMap(mapName.c_str());
    g_session.m_loginTimeTicks = _Xtime_get_ticks();
    mapName.append(".rsw", 4);
    g_modeMgr.Switch(1, mapName.c_str());
    static_cast<UIWaitWnd*>(g_windowMgr.EnsureWindow(WID_WAITWND))->SetMessage(LookupMessage(MSI_PLEASE_BE_PATIENT), 17, 1);
    g_windowMgr.modalResponse = 185;
    g_session.ClearStoreSearchResults();
    sameMap = 0;
  } else {
    this->ReloadScene();
    this->InitializeState();
    FadeOutMode(*g_renderer, *GetRenderSystemHolder()->system, ResourceMgr::Instance(), 0, false, 0.0f, 0.0f);
    sameMap = 1;
  }
  this->sameMapMove[0] = sameMap;

  for (std::vector<ActiveSkillEffect>::iterator it = g_activeSkillEffects.begin(); it != g_activeSkillEffects.end(); ++it) {
    if (it->id == 200 || it->id == 201 || it->id == 4) {
      g_activeSkillEffects.erase(it);
      break;
    }
  }

  g_session.RestoreSelectionItems();
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
      std::string masked("");
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
    entry.logId = it->item.recordIds[0];
    entry.quantity = it->item.recordIds[1];
    entry.operation = it->item.recordType;
    strncpy_s(entry.characterName, sizeof(entry.characterName), reinterpret_cast<const char*>(it->item.recordName), _TRUNCATE);
    strncpy_s(entry.timestamp, sizeof(entry.timestamp), reinterpret_cast<const char*>(it->item.recordDate), _TRUNCATE);
    entry.itemName = it->item.DisplayName(0);
    entry.identified = it->item.identified;
    entry.itemType = it->item.identity[0];
    entry.refineLevel = it->item.refiningLevel;
    std::memcpy(entry.cards, it->item.cards, sizeof(entry.cards));
    entry.category = it->logFlags;
    entry.grade = it->item.grade;
    g_guildStorageLogs[0].push_back(entry);
  }

  if (0 == result) {
    return;
  }

  if (1 == result) {
    GuildStorageLogEntry pivot;
    std::sort(g_guildStorageLogs[0].begin(), g_guildStorageLogs[0].end(), [&pivot](const GuildStorageLogEntry& left, const GuildStorageLogEntry& right) {
      return std::strcmp(left.timestamp, right.timestamp) > 0;
    });
    for (std::vector<GuildStorageLogEntry>::iterator it = g_guildStorageLogs[0].begin(); it != g_guildStorageLogs[0].end(); ++it) {
      if (it->operation != 1) {
        g_guildStorageLogs[2].push_back(*it);
      } else {
        g_guildStorageLogs[1].push_back(*it);
      }
    }
    g_windowMgr.EnsureWindow(WID_GUILD_STORAGE_LOG);
    return;
  }

  char text[250];
  int messageId;
  if (2 == result) {
    std::memset(text, 0, sizeof(text));
    messageId = 2543;
  } else {
    std::memset(text, 0, sizeof(text));
    messageId = 1816;
  }
  sprintf_s(text, sizeof(text), LookupMessage(messageId));
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
  g_guildStorageLogs[0].clear();
  g_guildStorageLogs[1].clear();
  g_guildStorageLogs[2].clear();
}

// OFFSET: 0x00D5E930
// FUNCTION: fn_21967
void LaunchSkillCastEffect(CGameMode* mode, CGameActor* source, CGameActor* target, std::uint32_t targetAid, std::uint16_t skillId, short x, short y, int delay) {
  CSkillActionMgr::GetInstance()->OnGroundSkillCast(skillId, delay, target, x, y);

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

    scope->SendMsg(nullptr, AM_SETMASTERGID, static_cast<std::uint32_t>(source->render.accountId), 0, 0, 0, 0, 0);
    scope->SendMsg(nullptr, AM_SETPOS, column, row, 0, 0, 0, 0);
    scope->SendMsg(nullptr, AM_SETTIME, static_cast<std::uint32_t>(timeGetTime() + delay), 0, 0, 0, 0, 0);
    const int id = skillId;
    scope->SendMsg(nullptr, AM_SET_SKILL, id, 0, 0, 0, 0, 0);
    mode->scene->objects.push_back(scope);
    return;
  }

  if (!target || source == target) {
    return;
  }

  if (g_session.IsHirelingJob(target->GetEffectiveJob(-1)) || target->objectType == 9) {
    if (mode->scene->isPKZone != 0 || mode->scene->isSiegeMode != 0 || !IsSupportSkill(skillId)) {
      source->SendMsg(nullptr, AM_SET_TARGET_AID, static_cast<std::uint32_t>(target->render.accountId), 0, 0, 0, 0, 0);
    }
  }

  source->hitTargetAid = targetAid;
  source->Face(target->render.position.x, target->render.position.z);

  CEffect* effect;
  if (skillId == CD_FRAMEN) {
    effect = g_effectMgr->Spawn(target->render.accountId, 0x78A, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  }

  const int delayFrames = delay / 16;
  target->SendMsg(nullptr, AM_MAKE_MAGIC_TARGET, 0x3C, delayFrames, 0, 0, 0, 0);

  switch (skillId) {
  case CR_ACIDDEMONSTRATION:
    mode->LaunchTargetEffect(0x21B, source, target, -1);
    return;
  case WL_SOULEXPANSION: {
    const float speed = target->render.speed;
    effect = g_effectMgr->Spawn(target->render.accountId, 0x635, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    const int duration = static_cast<int>(speed * 150.0f + 200.0f) + delay;
    if (effect) {
      effect->SetLifeTime(duration);
    }
    effect = g_effectMgr->Spawn(target->render.accountId, 0x636, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(duration);
    }
    return;
  }
  case WL_JACKFROST:
    target->SendMsg(nullptr, AM_DELETE_MAGIC_TARGET, 0, 0, 0, 0, 0, 0);
    target->SendMsg(nullptr, AM_MAKE_MAGIC_TARGET, 0x5A8, delayFrames, 0, 0, 0, 0);
    return;
  case WL_CHAINLIGHTNING:
    target->SendMsg(nullptr, AM_DELETE_MAGIC_TARGET, 0, 0, 0, 0, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, 0x665, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    return;
  case RA_AIMEDBOLT:
    target->SendMsg(nullptr, AM_DELETE_MAGIC_TARGET, 0, 0, 0, 0, 0, 0);
    effect = g_effectMgr->Spawn(target->render.accountId, 0x673, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case NC_FLAMELAUNCHER:
    if (CRagEffect* ragEffect = source->LaunchEffect(0x313, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
      vector3d position = target->render.position;
      ragEffect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
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
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  switch (packet->result) {
  case 6:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAIL_BUY_ITEM_INVALID_MCSTORE)), 0x6464FF, 0, 0, 0);
    return;
  case 7:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAIL_OPEN_STORE)), 0xFF, 0, 0, 0);
    return;
  case 15:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_GRADE_ENCHANT_FAIL_DOWNGRADE)), 0xFF, 0, 0, 0);
    return;
  }

  ITEM_INFO item = g_session.FindFilteredInventoryItem(packet->index);
  if (item.identity[1] > 0) {
    switch (packet->result) {
    case 1:
      g_windowMgr.SendNotice(
        UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_FAIL_BUY_ITEM_FROM_MERCHANT), item.DisplayName(0).c_str(), LookupMessage(MSI_INSUFFICIENT_MONEY)).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 2:
      g_windowMgr.SendNotice(
        UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_FAIL_BUY_ITEM_FROM_MERCHANT), item.DisplayName(0).c_str(), LookupMessage(MSI_OVER_WEIGHT)).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 4:
      g_windowMgr.SendNotice(
        UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_FAIL_BUY_ITEM_FROM_MERCHANT_NO_STOCK), item.DisplayName(0).c_str(), packet->value).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 5:
      g_windowMgr.SendNotice(
        UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_FAIL_BUY_ITEM_ITEM_EXCHANGING), item.DisplayName(0).c_str(), packet->value).c_str()), 0x6464FF, 0, 0, 0
      );
      break;
    case 8:
      g_windowMgr.SendNotice(
        UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_FAIL_BUY_ITEM_FROM_MERCHANT), item.DisplayName(0).c_str(), LookupMessage(MSI_MCSTORE_FAIL_SELLER_MONEY_GIVE)).c_str()), 0x6464FF, 0, 0, 0
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

  const int partyMember = g_session.IsPartyMember(target->render.accountId, true) && g_session.m_aid != target->render.accountId;
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
    name = LookupMessage(MSI_WHO_IS);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_NOTIFY_WHO), name.c_str());
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
      name = card.displayName.empty() ? std::string(LookupMessage(MSI_WHO_IS)) : card.displayName;
    }
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_NOTIFY_TARGET_WHO), name.c_str());
    message += text;
  }

  const std::string skillName = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, skillId);
  StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_NOTIFY_SKILL_TO_TARGET), skillName.c_str());
  message += text;
  if (!message.empty()) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF, color, 0, 0);
  }
}

// OFFSET: 0x00D5E680
// FUNCTION: fn_21966
void ApplyActorSkillState(CGameActor* actor, std::uint16_t skillId, int duration) {
  switch (skillId) {
  case KN_BRANDISHSPEAR:
  case KN_BOWLINGBASH:
  case HN_DOUBLEBOWLINGBASH:
    actor->render.isMotionFreezed = true;
    actor->freezeEndTick = timeGetTime() + duration;
    break;
  case MO_CALLSPIRITS:
  case MO_KITRANSLATION:
    actor->SetFrameSequence(96, 0, 0, 96, 0, 0, 0, 0);
    break;
  case MO_STEELBODY:
    actor->SetFrameSequence(96, 0, 0, 96, 0, 1, duration + 1000, 0);
    break;
  case CH_SOULCOLLECT:
    actor->SetFrameSequence(96, 0, 0, 96, 0, 0, 0, 0);
    actor->LaunchEffect(0x192, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case WL_TETRAVORTEX:
    actor->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2DF, 1, 0, 0, 0, 0);
    actor->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2E1, 1, 0, 0, 0, 0);
    actor->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2E0, 1, 0, 0, 0, 0);
    actor->SendMsg(nullptr, AM_EFFECT_LOOP_COUNT, 0x2E2, 1, 0, 0, 0, 0);
    break;
  case ABC_CHAIN_REACTION_SHOT:
    actor->SetFrameSequence(88, 0, 0, 88, 0, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D5B030
// FUNCTION: fn_21963
void CGameMode::NotifySkillCasting(
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
      effect->Open();
    } else {
      g_effectMgr->Spawn(source->render.accountId, 0x474, source->render.position, 0, -1, 0, 0);
    }
  }

  if (silent == 0 && skillId != 0x3E && skillId != 0x39 && !g_session.IsMonsterActor(source)) {
    if (skillId == 0x14E || skillId == 0x14F) {
      g_modeMgr.GetGameMode()->SendMsg(MM_MAKE_SKILL_USED_MSG, skillId, reinterpret_cast<std::intptr_t>(source), targetAid, 0);
    } else {
      g_modeMgr.GetGameMode()->SendMsg(MM_MAKE_SKILL_USED_MSG, skillId, reinterpret_cast<std::intptr_t>(source), 0, 0);
    }
  }

  if (skillId != 0x10F) {
    if (skillId == 0x49) {
      if (target && source != target) {
        source->Face(target->render.position.x, target->render.position.z);
      }
      g_modeMgr.GetGameMode()->SendMsg(MM_MAKE_SKILL_USED_MSG, 0x49, reinterpret_cast<std::intptr_t>(source), 0, 0);
    } else if (skillId == 0x1479) {
      g_effectMgr->Spawn(source->render.accountId, 0x7C4, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }

    CSkillActionMgr::GetInstance()->OnTargetSkillCast(skillId, reinterpret_cast<int>(source), reinterpret_cast<int>(target), delay);
    if (delay == 0) {
      return;
    }
  }

  int effectId = 0;
  int motion = 0;
  g_session.GetSkillActionInfo(skillId, effectId, motion, property, source->GetEffectiveJob(-1));
  source->SetState(motion, 1);

  if (skillId != 0x3E && skillId != 0x39 && delay != 0) {
    source->SendMsg(nullptr, AM_MAKE_SKILL_RECHARGEGAGE, static_cast<std::uint32_t>(delay), 0, 0, 0, 0, 0);
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
__declspec(noinline) void SetActiveSkillEffect184(unsigned char enable) {
  if (enable == 1) {
    ActiveSkillEffect effect;
    effect.id = 184;
    effect.expiration = 999999;
    effect.value1 = 0;
    effect.totalDuration = 9999;
    effect.remainingDuration = 9999;
    g_activeSkillEffects.push_back(effect);
    return;
  }

  std::vector<ActiveSkillEffect>::iterator it = std::find_if(g_activeSkillEffects.begin(), g_activeSkillEffects.end(), [](const ActiveSkillEffect& effect) {
    return effect.id == 184;
  });
  if (it != g_activeSkillEffects.end()) {
    g_activeSkillEffects.erase(it);
  }
}

// OFFSET: 0x00D316D0
// FUNCTION: fn_21734
void CGameMode::ReceiveExpChange(const unsigned char* packet) {
  std::int64_t& exp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x04]);
  std::int64_t& nextExp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x0C]);
  std::int64_t& nextJobExp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x14]);
  std::int64_t& jobExp = *reinterpret_cast<std::int64_t*>(&g_session.playerExperienceState[0x1C]);
  switch (*reinterpret_cast<const std::uint16_t*>(packet + 2)) {
  case SP_BASEEXP:
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
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xF51EF5, 0, 0, 0);
      }
    }
    exp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->baseExpBar)->SetValues(exp, nextExp);
    break;
  case SP_JOBEXP:
    jobExp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->jobExpBar)->SetValues(jobExp, nextJobExp);
    break;
  case SP_ZENY:
    if (*reinterpret_cast<int*>(&this->equipmentRequestState[0x14])) {
      *reinterpret_cast<int*>(&this->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
    }
    g_session.m_zeny = *reinterpret_cast<const int*>(packet + 4);
    g_session.RefreshHpSpWindows();
    g_session.InvalidateWindowFrames();
    break;
  case SP_NEXTBASEEXP:
    nextExp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->baseExpBar)->SetValues(exp, nextExp);
    break;
  case SP_NEXTJOBEXP:
    nextJobExp = *reinterpret_cast<const int*>(packet + 4);
    g_session.InvalidateWindowFrames();
    static_cast<UIINT64BarGraph*>(reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd)->jobExpBar)->SetValues(jobExp, nextJobExp);
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
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RULE)), 0xFF, 0, 0, 0);
    break;
  case PURCHASE_SUCCEED:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_SECCESS)), 0xFFFF00, 0, 0, 0);
    break;
  case PURCHASE_FAIL_MONEY:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_MONEY)), 0xFF, 0, 0, 0);
    break;
  case PURCHASE_FAIL_WEIGHT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OVER_WEIGHT)), 0xFF, 0, 0, 0);
    break;
  case PURCHASE_FAIL_COUNT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_OVER_COUNT)), 0xFF, 0, 0, 0);
    break;
  case PURCHASE_FAIL_EACHITEM_COUNT:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PURCHASE_FAIL_EACHITEM_COUNT)), 0xFF, 0, 0, 0);
    break;
  }

  if (count <= 0) {
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(MM_NPC_MARKET_CLOSE, 0, 0, 0, 0);
    }
    return;
  }

  UIParaResultWnd* window = dynamic_cast<UIParaResultWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_PARARESULTWND)));
  const ParaResultRecord* record = reinterpret_cast<const ParaResultRecord*>(packet + 6);
  for (int i = 0; i < count; ++i) {
    RawItemBasic item = {};
    _itoa(record->itemId, item.itemId, 10);
    item.price = record->location;
    item.realPrice = record->location;
    item.id = -1;
    item.identified = 1;
    window->AddItem(item, record->quantity);
    ++record;
  }

  window->SendMsg(nullptr, UM_SETTITLE, (count - 2 > 0 ? count - 2 : 0) * 32 + 120, 0, 0, 0);
  window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00D2C5D0
// FUNCTION: fn_21716
void CGameMode::ReceiveEquipItemListFlags(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  g_session.ResetCostumeItems();
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 0x25;
  const unsigned char* record = packet + 4;
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.identity[0] = record[6];
    std::memcpy(item.cards, record + 0x0C, sizeof(item.cards));
    item.identity[2] = *reinterpret_cast<const std::uint16_t*>(record + 7);
    item.identity[1] = *reinterpret_cast<const std::int16_t*>(record);
    item.identity[3] = *reinterpret_cast<const std::uint16_t*>(record + 9);
    item.quantity = 1;
    item.SetItemId(*reinterpret_cast<const int*>(record + 2));
    const unsigned char flags = record[0x24];
    item.identified = flags & 1;
    item.damaged = (flags >> 1) & 1;
    item.flags[0] = (flags >> 2) & 1;
    item.refiningLevel = record[0x0B];
    item.expireTime = *reinterpret_cast<const std::uint32_t*>(record + 0x1C);
    item.bindOnEquip = *reinterpret_cast<const std::uint16_t*>(record + 0x20);
    item.sprite = *reinterpret_cast<const std::uint16_t*>(record + 0x22);
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
      g_session.petEggIndex = item.identity[1];
    }

    if (item.identity[3] != 0) {
      char equipped = 1;
      if (g_session.IsCostumeLocation(item.identity[3])) {
        g_session.EquipCostumeItem(item, &equipped);
      } else {
        g_session.EquipInventoryItem(item, &equipped);
      }
    } else {
      g_session.AddInventoryItem(item, 0);
      g_session.UpdateInventoryItem(item, 0);
    }
    record += 0x25;
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D27EC0
// FUNCTION: fn_21692
__declspec(noinline) void CGameMode::ReceiveEquipItemList34(const unsigned char* packet) {
  g_session.RemoveEquipment();
  g_session.ResetEquipment();
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 0x22;
  const unsigned char* record = packet + 4;
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.identity[1] = *reinterpret_cast<const std::int16_t*>(record);
    item.identity[0] = record[6];
    item.identity[2] = *reinterpret_cast<const std::uint16_t*>(record + 8);
    item.identity[3] = *reinterpret_cast<const std::uint16_t*>(record + 10);
    item.quantity = 1;
    std::memcpy(item.cards, record + 0x0E, sizeof(item.cards));
    item.SetItemId(*reinterpret_cast<const int*>(record + 2));
    item.identified = record[7];
    item.damaged = record[0x0C];
    item.refiningLevel = record[0x0D];
    item.expireTime = *reinterpret_cast<const std::uint32_t*>(record + 0x1E);
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str()))) {
      if (item.damaged) {
        g_session.petEggIndex = item.identity[1];
      }
    }

    if (item.identity[3] != 0) {
      char equipped = 1;
      g_session.EquipInventoryItem(item, &equipped);
    } else {
      g_session.AddInventoryItem(item, 0);
      g_session.UpdateInventoryItem(item, 0);
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
  g_session.ResetCostumeItems();
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 4) / 0x26;
  const unsigned char* record = packet + 4;
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item.identity[1] = *reinterpret_cast<const std::int16_t*>(record);
    item.identity[0] = record[6];
    item.identity[2] = *reinterpret_cast<const std::uint16_t*>(record + 8);
    item.identity[3] = *reinterpret_cast<const std::uint16_t*>(record + 10);
    item.quantity = 1;
    std::memcpy(item.cards, record + 0x0E, sizeof(item.cards));
    item.SetItemId(*reinterpret_cast<const int*>(record + 2));
    item.identified = record[7];
    item.damaged = record[0x0C];
    item.refiningLevel = record[0x0D];
    item.expireTime = *reinterpret_cast<const std::uint32_t*>(record + 0x1E);
    item.bindOnEquip = *reinterpret_cast<const std::uint16_t*>(record + 0x22);
    item.sprite = *reinterpret_cast<const std::uint16_t*>(record + 0x24);
    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str()))) {
      if (item.damaged) {
        g_session.petEggIndex = item.identity[1];
      }
    }

    if (item.identity[3] != 0) {
      char equipped = 1;
      if (g_session.IsCostumeLocation(item.identity[3])) {
        g_session.EquipCostumeItem(item, &equipped);
      } else {
        g_session.EquipInventoryItem(item, &equipped);
      }
    } else {
      g_session.AddInventoryItem(item, 0);
      g_session.UpdateInventoryItem(item, 0);
    }
    record += 0x26;
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00D79500
// FUNCTION: fn_22169
__declspec(noinline) void CGameMode::ApplyRefiningMaterial(short itemId, unsigned char count, const unsigned char* materials, unsigned char catalystCount) {
  UIRefiningWnd* window = static_cast<UIRefiningWnd*>(g_windowMgr.LookupWindow(WID_REFININGWND));
  if (window) {
    window->ApplyItem(itemId, count, reinterpret_cast<RefiningMaterial*>(const_cast<unsigned char*>(materials)), catalystCount);
  }
}

// OFFSET: 0x00D4B4D0
// FUNCTION: fn_21865
void CGameMode::ReceiveAddRelatedGuild(const unsigned char* data) {
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->infoRequested == 0) {
    return;
  }

  char name[25];
  std::memcpy(name, data + 0xA, 24);
  name[24] = '\0';
  if (g_forbiddenWords.Contains(name) == true) {
    std::string masked("");
    g_forbiddenWords.Mask(name, &masked);
  }

  GuildInfo::RelatedGuild guild;
  guild.relation = *reinterpret_cast<const int*>(data + 2);
  guild.guildId = *reinterpret_cast<const int*>(data + 6);
  guild.name = name;
  guildInfo->relatedGuilds.push_back(guild);
  g_windowMgr.RefreshGuildWindows();
}

// OFFSET: 0x00D89960
// FUNCTION: fn_22210
void CGameMode::ReceiveEquipItemResult(const unsigned char* packet) {
  const std::uint16_t result = *reinterpret_cast<const std::uint16_t*>(packet + 8);
  if (result != 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(result == 3 ? 3009 : 3033)), 0xFF, 0, 0, 0);
    return;
  }

  ITEM_INFO item = g_session.FindItemByIdentity(*reinterpret_cast<const std::uint16_t*>(packet + 2), *reinterpret_cast<const int*>(packet + 4));
  item.identity[3] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  ITEM_DESC description = FindItemDescription(std::atoi(item.itemId.c_str()));
  item.sprite = description.viewId;
  if (g_session.IsCostumeLocation(*reinterpret_cast<const int*>(packet + 4))) {
    g_session.EquipSwapEquipmentItem(item);
  } else {
    g_session.EquipSwapCostumeItem(item);
  }

  const char* suffix = LookupMessage(MSI_BASIC_EXP_MSG_FRA);
  g_windowMgr.SendNotice(
    UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), suffix).c_str()), 0xC8FF00, 7, 0, 0
  );
}

#pragma pack(push, 1)
struct GuildMemberAddV4Packet {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ int accountId;
  /* 0006 */ std::uint32_t characterId;
  /* 000A */ std::int16_t head;
  /* 000C */ std::int16_t headPalette;
  /* 000E */ std::int16_t sex;
  /* 0010 */ std::int16_t job;
  /* 0012 */ std::int16_t level;
  /* 0014 */ int exp;
  /* 0018 */ int online;
  /* 001C */ int positionId;
  /* 0020 */ int lastTimeOffline;
};
#pragma pack(pop)

static_assert(sizeof(GuildMemberAddV4Packet) == 0x24);

// OFFSET: 0x00D89CC0
// FUNCTION: fn_22214
void CGameMode::ReceiveGuildMemberAdd4(const unsigned char* data) {
  const GuildMemberAddV4Packet* packet = reinterpret_cast<const GuildMemberAddV4Packet*>(data);
  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->membersRequested == 0) {
    return;
  }

  GuildInfo::Member member;
  member.accountId = packet->accountId;
  member.characterId = packet->characterId;
  member.name = LookupPlayerName(packet->characterId);
  member.head = packet->head;
  member.headPalette = packet->headPalette;
  member.job = packet->job;
  member.sex = packet->sex;
  member.positionId = packet->positionId;
  member.level = packet->level;
  member.exp = packet->exp;
  member.online = packet->online;
  *reinterpret_cast<int*>(member.lastOfflineTime) = packet->lastTimeOffline;
  if (member.name.size() == 0) {
    member.name = LookupMessage(MSI_NAMELESS_USER) + g_guildMemberNamelessTag;
  }

  guildInfo->AddMember(member);
  guildInfo->UpdateMemberExpRatios();
  g_windowMgr.RefreshGuildWindows();
}

struct ActorStandEntry {
  ActorStandEntry()
      : job(0), appearanceVariant(0), cellX(0), cellY(0), direction(0), speed(0), xSize(0), ySize(0), weapon(0), shield(0),
        accessory(0), state(0), bodyState(0), effectState(0), stateFlags(0), pkMode(0), shieldAppearance(0), accessory2(0), accessory3(0),
        headPalette(0), bodyPalette(0), headDirection(0), guildId(0), guildVersion(0), honor(0), virtue(0), robe(0), gid(0),
        bodyCostume(0) {
  }

  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int appearanceVariant;
  /* 000C */ int cellX;
  /* 0010 */ int cellY;
  /* 0014 */ int direction;
  /* 0018 */ int speed;
  /* 001C */ int xSize;
  /* 0020 */ int ySize;
  /* 0024 */ int head;
  /* 0028 */ int weapon;
  /* 002C */ int shield;
  /* 0030 */ int accessory;
  /* 0034 */ int state;
  /* 0038 */ int bodyState;
  /* 003C */ int effectState;
  /* 0040 */ std::uint32_t stateFlags;
  /* 0044 */ int pkMode;
  /* 0048 */ int shieldAppearance;
  /* 004C */ int accessory2;
  /* 0050 */ int accessory3;
  /* 0054 */ int headPalette;
  /* 0058 */ int bodyPalette;
  /* 005C */ int headDirection;
  /* 0060 */ int guildId;
  /* 0064 */ int guildVersion;
  /* 0068 */ int honor;
  /* 006C */ int virtue;
  /* 0070 */ int robe;
  /* 0074 */ int gid;
  /* 0078 */ int bodyCostume;
};

static_assert(sizeof(ActorStandEntry) == 0x7C);

// OFFSET: 0x00D7AFA0
// FUNCTION: fn_22199
void CGameMode::SpawnStandingPc(
  std::uint32_t aid, int job, short cellX, short cellY, unsigned char sex, short head, int weapon, int shield,
  short accessory, short accessory2, short accessory3, short headPalette, short bodyPalette, short robe, std::string name
) {
  ActorStandEntry entry;
  entry.aid = aid;
  entry.job = job;
  entry.appearanceVariant = this->SubD07740(job, sex);
  entry.cellX = cellX;
  entry.cellY = cellY;
  entry.direction = 4;
  entry.head = head;
  entry.weapon = weapon;
  entry.shield = shield;
  entry.accessory = accessory;
  entry.accessory2 = accessory2;
  entry.accessory3 = accessory3;
  entry.headPalette = headPalette;
  entry.bodyPalette = bodyPalette;
  entry.robe = robe;
  if (name.size() != 0) {
    this->nameCard.SetName(aid, name);
  }

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (actor) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_STANDENTRY2, value, 0, 0, 0, 0, 0);
  } else {
    CPc* pc = new CPc;
    this->scene->objects.push_back(pc);
    this->scene->actors.push_back(pc);
    actor = pc;
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 0, 0, 0, 0, 0);
  }
  actor->storedActorEntry = 1;
}

// OFFSET: 0x00D7B170
// FUNCTION: fn_22200
void CGameMode::SpawnStandingPc2(
  std::uint32_t aid, int job, short cellX, short cellY, unsigned char sex, short head, int weapon, int shield,
  short accessory, short accessory2, short accessory3, short headPalette, short bodyPalette, short robe, std::string name, short bodyCostume
) {
  ActorStandEntry entry;
  entry.aid = aid;
  entry.job = job;
  entry.appearanceVariant = this->SubD07740(job, sex);
  entry.cellX = cellX;
  entry.cellY = cellY;
  entry.direction = 4;
  entry.head = head;
  entry.weapon = weapon;
  entry.shield = shield;
  entry.accessory = accessory;
  entry.accessory2 = accessory2;
  entry.accessory3 = accessory3;
  entry.headPalette = headPalette;
  entry.bodyPalette = bodyPalette;
  entry.robe = robe;
  entry.bodyCostume = bodyCostume;
  if (name.size() != 0) {
    this->nameCard.SetName(aid, name);
  }

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(aid));
  if (actor) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_STANDENTRY2, value, 0, 0, 0, 0, 0);
  } else {
    CPc* pc = new CPc;
    this->scene->objects.push_back(pc);
    this->scene->actors.push_back(pc);
    actor = pc;
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 0, 0, 0, 0, 0);
  }
  actor->storedActorEntry = 1;
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
    if (!actor) {
      ActorStandEntry entry;
      entry.cellX = x;
      entry.aid = aid;
      entry.cellY = y;
      entry.job = g_session.GetDisplayJob();
      entry.appearanceVariant = g_session.GetCurrentSex();
      entry.head = g_session.m_head;
      entry.weapon = *reinterpret_cast<const int*>(&g_session.weaponAppearance[0x4]);
      entry.shield = *reinterpret_cast<const int*>(&g_session.weaponAppearance[0x8]);
      entry.accessory = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0x0]);
      entry.direction = 180;
      entry.shieldAppearance = *reinterpret_cast<const int*>(&g_session.weaponAppearance[0xC]);
      entry.accessory2 = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0x4]);
      entry.accessory3 = *reinterpret_cast<const int*>(&g_session.accessoryAppearance[0x8]);
      entry.headPalette = g_session.m_headPalette;
      entry.bodyPalette = g_session.m_bodyStyle;
      entry.robe = g_session.m_robe;
      entry.bodyCostume = g_session.m_bodyCostume;

      CPc* pc = new CPc;
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      pc->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 0, 0, 0, 0, 0);
      pc->objectType = 7;
      pc->LaunchEffect(0x3C3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->scene->objects.push_back(pc);
      this->scene->actors.push_back(pc);
      this->spriteMirrorActorIds.push_back(aid);
      return;
    }

    this->CellCenter(x, y, actor->render.position.x, actor->render.position.z);
    actor->render.position.y = this->scene->terrain->Height(actor->render.position.x, actor->render.position.z);
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
  const std::int64_t value = reinterpret_cast<std::intptr_t>(&data);
  skill->SendMsg(nullptr, AM_SKILLSTANDENTRY, value, 0, 0, 0, 0, 0);
  this->scene->objects.push_back(skill);
  this->scene->skills.push_back(skill);
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
  /* 0038 */ std::int32_t location;
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
    item.optionCount = CountStoreItemOptions(record.options);
    for (std::int64_t index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = record.options[index];
    }
    item.identity[2] = record.location;
    item.sprite = record.sprite;
    item.grade = record.grade;
    items.push_back(item);
  }

  const std::int32_t value8 = *reinterpret_cast<const std::int32_t*>(packet + 8);
  const std::int32_t value4 = *reinterpret_cast<const std::int32_t*>(packet + 4);
  g_session.ClearMerchantMirrorItems();
  g_session.ClearMerchantHopeToSellItems();
  for (ITEM_INFO item : items) {
    g_session.AddMerchantMirrorItem(item);
  }

  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMSHOPWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETNAID, value4, 0, 0, 0);
  }
  if (UIWindow* window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMPURCHASEWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    window->SendMsg(nullptr, UM_SETNAID, value4, value8, 0, 0);
  }
  this->isOnQuest = 1;
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
  case GROUPMEMBER_DELETE_LEAVE:
  case GROUPMEMBER_DELETE_EXPEL:
    this->SendMsg(MM_DELETEPARTYPOS, g_session.GetPartyMemberAid(name), 0, 0, 0);
    g_session.RemovePartyMemberByName(name);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PREVENT_PARTY_LEAVE)), 0xFF, 0, 0, 0);
    return;
  case 3:
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PREVENT_PARTY_EXPEL)), 0xFF, 0, 0, 0);
    return;
  }

  if (std::strcmp(g_session.DecodePlayerName(), name) == 0) {
    g_session.partyLeader = 0;
    g_session.ClearPartyMembers();
    g_session.partyName.assign("", 0);
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_YOU_LEAVE_FROM_PARTY)), 0xFFFF00, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
    this->SendMsg(MM_DELETEPARTYPOSALL, 0, 0, 0, 0);
    if (g_windowMgr.memorialDunWnd) {
      g_windowMgr.CloseWindow(WID_MEMORIALDUNWND);
    }
    g_session.ClearMiniPartyRecords();
  }

  RefreshPartyWindows(&g_session);
  if (CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid))) {
    actor->SetGroupMemberVisible(0);
  }
}

// OFFSET: 0x00D373C0
// FUNCTION: fn_21763
void CGameMode::ReceiveBroadcast(const unsigned char* packet) {
  const unsigned size = *reinterpret_cast<const std::uint16_t*>(packet + 2) - 4;
  if (size - 1 > 0x168) {
    return;
  }

  std::vector<char> text(size + 1, 0);
  memcpy_s(text.data(), text.size(), packet + 4, size);
  if (g_forbiddenWords.Contains(text.data()) == true) {
    std::string masked("");
    g_forbiddenWords.Mask(text.data(), &masked);
  }

  if (g_windowMgr.chatRoomWnd != nullptr) {
    g_windowMgr.SendNotice(UIM_PUSH_INTO_CHATROOM, reinterpret_cast<std::intptr_t>(text.empty() ? nullptr : &text[0]), 0x7800, 0, 0, 0);
  } else {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.empty() ? nullptr : &text[0]), 0xFF00, 1, 0, 0);
    char* message = text.empty() ? nullptr : &text[0];
    this->scene->player->SendMsg(nullptr, AM_CHAT, reinterpret_cast<int>(message), 0, 0, 0, 0, 0);
  }

  if (g_session.m_whisperRecord != 0) {
    this->WriteChatLog(text.empty() ? nullptr : &text[0]);
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
    item.identity[1] = record.index;
    item.prices[0] = record.price;
    item.prices[1] = record.price;
    item.quantity = record.amount;
    item.identity[0] = record.type;
    item.SetItemId(record.itemId);
    item.identified = record.identified;
    item.damaged = record.damaged;
    item.refiningLevel = record.refine;
    std::memcpy(item.cards, record.cards, sizeof(record.cards));
    item.optionCount = CountStoreItemOptions(record.options);
    for (std::int64_t index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<StoreItemOption*>(item.options)[index] = record.options[index];
    }
    item.grade = record.grade;
    items.push_back(item);
  }

  if (g_replayAnimationScaling != 0) {
    g_modeMgr.GetGameMode()->SendMsg(MM_REQ_OPENSTORE, 0, 0, 0, 0);
  }

  g_session.ClearMerchantPurchaseItems();
  if (items.size() == 0) {
    this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND);
    g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND);
    g_session.ClearItems1760And1748();
    return;
  }

  for (ITEM_INFO item : items) {
    g_session.AddMerchantPurchaseItem(item);
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
  /* 002A */ std::uint16_t actorFlags;
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.shield = packet->shield;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.aid = packet->aid;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_STANDENTRY2, value, 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    CGameActor* created;
    if ((job >= JT_WARPNPC && job < 1000) || (job > 1000 && job < 3999) || static_cast<std::uint16_t>(job - 20021) <= 11978 ||
        g_session.IsCompanionJob(job)) {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      created = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    } else {
      created = new CPc;
    }

    actor = created;
    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 0, 0, 0, 0, 0);
      if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D387B0
// FUNCTION: fn_21768
void CGameMode::ReceiveActorStandEntryFont(const unsigned char* data) {
  const ActorStandEntryPacket* packet = reinterpret_cast<const ActorStandEntryPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.shield = packet->shield;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.aid = packet->aid;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    *reinterpret_cast<std::uint16_t*>(actor->render.fontSequenceBytes) = packet->font;
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_STANDENTRY2, value, 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    CGameActor* created;
    if ((JT_WARPNPC <= job && 1000 > job) || (1000 < job && 3999 > job) || static_cast<std::uint16_t>(job - 20021) <= 11978 ||
        g_session.IsCompanionJob(job)) {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      created = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    } else {
      created = new CPc;
    }

    actor = created;
    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 0, 0, 0, 0, 0);
      if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      *reinterpret_cast<std::uint16_t*>(actor->render.fontSequenceBytes) = packet->font;
      if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D38C40
// FUNCTION: fn_21769
void CGameMode::ReceiveActorTypedEntry(const unsigned char* data) {
  const ActorTypedEntryPacket* packet = reinterpret_cast<const ActorTypedEntryPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.shield = packet->weapon >> 31;
  entry.accessory = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.shieldAppearance = packet->shield;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.aid = packet->aid;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
      const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
      const int guildVersion = packet->shield;
      if (guildId > 0 && !actor->GetGdid()) {
        actor->SetGuildInfo(guildId, guildVersion);
      }
    }
    if (packet->objectType == 7) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_STANDENTRY, value, 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && packet->objectType != 7 && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    CGameActor* created;
    if ((JT_WARPNPC <= job && 1000 > job) || (1000 < job && 3999 > job) || static_cast<std::uint16_t>(job - 20021) <= 11978 ||
        g_session.IsCompanionJob(job) || packet->objectType == 9) {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      created = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    } else {
      created = new CPc;
    }

    actor = created;
    if (actor && packet->objectType == 7) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->objectType = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY, value, 0, 0, 0, 0, 0);
      if (g_session.GetSiegeMode() && packet->objectType != 7 && actor->sceneNameWindow == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      const int actorJob = actor->GetEffectiveJob(-1);
      if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
        const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
        if (guildId > 0) {
          actor->SetGuildInfo(guildId, packet->shield);
        }
      }
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
}

// OFFSET: 0x00D39160
// FUNCTION: fn_21770
void CGameMode::ReceiveNpcStandEntry(const unsigned char* data) {
  const NpcStandEntryPacket* packet = reinterpret_cast<const NpcStandEntryPacket*>(data);
  ActorStandEntry entry;
  entry.job = packet->job;
  entry.appearanceVariant = packet->sex;
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.shield = packet->weapon >> 31;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.shieldAppearance = packet->shield;
  entry.aid = packet->aid;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_STANDENTRY, value, 0, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
      const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
      const int guildVersion = packet->shield;
      if (guildId > 0 && !actor->GetGdid()) {
        actor->SetGuildInfo(guildId, guildVersion);
      }
    }
    actor->characterLevel = packet->actorFlags;
  } else {
    actor = this->NewActor(packet->job);
    entry.appearanceVariant = 0;
    actor->objectType = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_INITSTANDENTRY, value, 1, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > JT_WARPNPC && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned>(actorJob - 20021) <= 11978) {
      const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
      if (guildId > 0) {
        actor->SetGuildInfo(guildId, packet->shield);
      }
    }
    actor->characterLevel = packet->actorFlags;
  }
}

struct ActorMoveEntry {
  /* 0000 */ std::uint32_t aid;
  /* 0004 */ int job;
  /* 0008 */ int appearanceVariant;
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
  /* 004C */ int shieldAppearance;
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

  ActorMoveEntry() {}
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
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
    if (actor->isGroupMemberVisible) {
      auto member = this->partyPosList.find(actor->render.accountId);
      if (member != this->partyPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->guildPosList.find(actor->render.accountId);
      if (member != this->guildPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
    actor->SendMsg(nullptr, AM_MOVEENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
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

    created->objectType = packet->objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (actor->objectType == 0) {
      if (this->partyPosList.find(entry.aid) != this->partyPosList.end() || this->guildPosList.find(entry.aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
    } else {
      entry.appearanceVariant = 0;
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
    actor->SendMsg(nullptr, AM_INITMOVEENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
  }

  std::int64_t bossFlag = 0;
  if (packet->isBoss && actor->hpGaugeFlags == 0) {
    actor->hpGaugeFlags = packet->isBoss;
    bossFlag = 1;
  }
  actor->characterLevel = packet->level;
  *reinterpret_cast<std::uint16_t*>(actor->render.fontSequenceBytes) = packet->font;
  if (packet->objectType == 7) {
    if (g_session.IsPetAid(packet->aid)) {
      g_session.petAccessory = entry.accessory;
    }
    if (entry.accessory && !actor->GetGdid()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
    }
  } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
    g_session.homunAlive = 1;
  } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
    g_session.mercenaryAlive = 1;
  } else if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  switch (actor->objectType) {
  case PC_TYPE:
  case NPC_MOB_TYPE:
  case NPC_HO_TYPE:
  case NPC_MERSOL_TYPE:
  case NPC_ELEMENTAL_TYPE: {
    const std::int64_t hp = packet->hp;
    const std::int64_t maxHp = packet->maxHp;
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, hp, maxHp, bossFlag, 0, 0, 0);
    break;
  }
  case NPC_ABR_TYPE:
  case NPC_BIONIC_TYPE:
    actor->SendMsg(nullptr, AM_UPDATEGAGE, packet->aid, 0, 0, 0, 0, 0);
    break;
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, sex);
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
    if (actor->isGroupMemberVisible) {
      auto member = this->partyPosList.find(actor->render.accountId);
      if (member != this->partyPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->guildPosList.find(actor->render.accountId);
      if (member != this->guildPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
    actor->SendMsg(nullptr, AM_MOVEENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
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

    created->objectType = objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (actor->objectType == 0) {
      if (this->partyPosList.find(entry.aid) != this->partyPosList.end() || this->guildPosList.find(entry.aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
    } else if (!((objectType == 1 || objectType == 6 || objectType == 12) && IsPlayerJob(job))) {
      entry.appearanceVariant = 0;
    }
    actor->SendMsg(nullptr, AM_INITMOVEENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
  }
  g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);

  std::int64_t bossFlag = 0;
  if (isBoss && actor->hpGaugeFlags == 0) {
    actor->hpGaugeFlags = isBoss;
    bossFlag = 1;
  }
  actor->characterLevel = level;
  *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = font;
  if (objectType == 7) {
    if (g_session.IsPetAid(aid)) {
      g_session.petAccessory = entry.accessory;
    }
    if (entry.accessory && !actor->GetGdid()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
    }
  } else if (objectType == 8 && g_session.IsHomunAid(aid)) {
    g_session.homunAlive = 1;
  } else if (objectType == 9 && g_session.IsMercenaryAid(aid)) {
    g_session.mercenaryAlive = 1;
  } else if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, guildId, guildVersion);
  }

  switch (actor->objectType) {
  case PC_TYPE:
  case NPC_MOB_TYPE:
  case NPC_HO_TYPE:
  case NPC_MERSOL_TYPE:
  case NPC_ELEMENTAL_TYPE: {
    const std::int64_t currentHp = hp;
    const std::int64_t currentMaxHp = maxHp;
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, currentHp, currentMaxHp, bossFlag, 0, 0, 0);
    break;
  }
  case NPC_ABR_TYPE:
  case NPC_BIONIC_TYPE:
    actor->SendMsg(nullptr, AM_UPDATEGAGE, gid, 0, 0, 0, 0, 0);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
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
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_MOVEENTRY2, value, 0, 0, 0, 0, 0);
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory && !actor->GetGdid()) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (actor->isGroupMemberVisible) {
      auto member = this->partyPosList.find(actor->render.accountId);
      if (member != this->partyPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->guildPosList.find(actor->render.accountId);
      if (member != this->guildPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
  } else {
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job)) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_INITMOVEENTRY2, value, 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->partyPosList.find(entry.aid) != this->partyPosList.end() || this->guildPosList.find(entry.aid) != this->guildPosList.end()) {
      actor->SetGroupMemberVisible(1);
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
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
    actor->characterLevel = packet->level;
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_MOVEENTRY2, value, 0, 0, 0, 0, 0);
    if (packet->objectType == 7) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory && !actor->GetGdid()) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    if (g_session.GetSiegeMode() && packet->objectType != 7 && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (actor->isGroupMemberVisible) {
      auto member = this->partyPosList.find(actor->render.accountId);
      if (member != this->partyPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->guildPosList.find(actor->render.accountId);
      if (member != this->guildPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
  } else {
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job) &&
        packet->objectType != 9) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && packet->objectType == 7) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->characterLevel = packet->level;
    actor->objectType = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
    actor->SendMsg(nullptr, AM_INITMOVEENTRY2, value, 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && packet->objectType == 7 && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->partyPosList.find(entry.aid) != this->partyPosList.end() || this->guildPosList.find(entry.aid) != this->guildPosList.end()) {
      actor->SetGroupMemberVisible(1);
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
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
    actor->characterLevel = packet->level;
    *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
    actor->SendMsg(nullptr, AM_MOVEENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    if (packet->objectType == 7) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory && !actor->GetGdid()) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    if (g_session.GetSiegeMode() && packet->objectType == 7 && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (actor->isGroupMemberVisible) {
      auto member = this->partyPosList.find(actor->render.accountId);
      if (member != this->partyPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
      member = this->guildPosList.find(actor->render.accountId);
      if (member != this->guildPosList.end()) {
        member->second.x = entry.dstX;
        member->second.y = entry.dstY;
      }
    }
  } else {
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job) &&
        packet->objectType != 9) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && packet->objectType != 0) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->objectType = packet->objectType;
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, AM_INITMOVEENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
    actor->characterLevel = packet->level;
    if (g_session.GetSiegeMode() && packet->objectType == 7 && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->partyPosList.find(entry.aid) != this->partyPosList.end() || this->guildPosList.find(entry.aid) != this->guildPosList.end()) {
      actor->SetGroupMemberVisible(1);
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.shield = packet->weapon >> 31;
  entry.accessory = packet->accessory;
  entry.healthState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.effectState = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.shieldAppearance = packet->shield;
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
    actor->SendMsg(nullptr, AM_MOVEENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > 45 && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned int>(actorJob - 20021) <= 11978) {
      const std::uint32_t emblem = (packet->accessory2 << 16) | packet->accessory3;
      if (emblem > 0) {
        actor->SetGuildInfo(emblem, packet->shield);
      }
    }
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory && !actor->GetGdid()) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
  } else {
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job)) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, AM_INITMOVEENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob > 45 && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned int>(actorJob - 20021) <= 11978) {
      const std::uint32_t emblem = (packet->accessory2 << 16) | packet->accessory3;
      if (emblem > 0 && !actor->GetGdid()) {
        actor->SetGuildInfo(emblem, packet->shield);
      }
    }
  }

  if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
      (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.shield = packet->shield;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  if (packet->objectType == 0) {
    entry.state = 0;
  } else {
    entry.shieldAppearance = entry.guildVersion;
  }
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (packet->objectType == 0) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_STANDENTRY2, value, 0, 0, 0, 0, 0);
    } else {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_STANDENTRY, value, 0, 0, 0, 0, 0);
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

    created->objectType = packet->objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (packet->objectType == 0) {
      if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 2, 0, 0, 0, 0);
    } else {
      entry.appearanceVariant = 0;
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY, value, 1, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (packet->isBoss && actor->hpGaugeFlags == 0) {
    actor->hpGaugeFlags = packet->isBoss;
    bossFlag = 1;
  }
  actor->characterLevel = packet->level;
  *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
  if (packet->objectType == 7) {
    if (g_session.IsPetAid(packet->aid)) {
      g_session.petAccessory = entry.accessory;
    }
    if (entry.accessory && !actor->GetGdid()) {
      actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
    }
  } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
    g_session.homunAlive = 1;
  } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
    g_session.mercenaryAlive = 1;
  } else if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  switch (actor->objectType) {
  case PC_TYPE:
  case NPC_MOB_TYPE:
  case NPC_HO_TYPE:
  case NPC_MERSOL_TYPE:
  case NPC_ELEMENTAL_TYPE: {
    const std::int64_t hp = packet->hp;
    const std::int64_t maxHp = packet->maxHp;
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, hp, maxHp, bossFlag, 0, 0, 0);
    break;
  }
  case NPC_ABR_TYPE:
  case NPC_BIONIC_TYPE:
    actor->SendMsg(nullptr, AM_UPDATEGAGE, packet->aid, 0, 0, 0, 0, 0);
    break;
  }

  if (packet->objectType != 0) {
    const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (guildId > 0 && !actor->GetGdid()) {
      actor->SetGuildInfo(guildId, packet->guildVersion);
    }
  }
  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.shield = packet->shield;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  if (packet->objectType == 0) {
    entry.state = 0;
  } else {
    entry.shieldAppearance = entry.guildVersion;
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
      entry.appearanceVariant = 0;
    }
    actor->objectType = packet->objectType;
  }

  actor->characterLevel = packet->level;
  if (packet->objectType == 0) {
    *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
    if (created) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 2, 0, 0, 0, 0);
      if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
      this->scene->objects.push_back(actor);
      this->scene->actors.push_back(actor);
    } else {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_STANDENTRY2, value, 0, 0, 0, 0, 0);
    }
    if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    if (packet->objectType == 7) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
      g_session.homunAlive = 1;
    } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
      g_session.mercenaryAlive = 1;
    }

    if (created) {
      this->scene->objects.push_back(actor);
      this->scene->actors.push_back(actor);
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY, value, 1, 0, 0, 0, 0);
    } else {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_STANDENTRY, value, 0, 0, 0, 0, 0);
    }
    if (g_session.IsGuardianJob(packet->job) && g_session.GetSiegeMode() && actor->sceneNameWindow == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }

    const int guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (created) {
      if (guildId != 0) {
        actor->SetGuildInfo(guildId, packet->guildVersion);
      }
    } else if (guildId != 0 && !actor->GetGdid()) {
      actor->SetGuildInfo(guildId, packet->guildVersion);
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.shield = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  entry.state = 0;

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->SendMsg(nullptr, AM_STANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, AM_INITSTANDENTRY2, reinterpret_cast<int>(&entry), 2, 0, 0, 0, 0);
    if (isPc && !IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
      actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
      actor->SetGroupMemberVisible(1);
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.shield = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  entry.state = 0;

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->SendMsg(nullptr, AM_STANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    actor->characterLevel = packet->level;
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, AM_INITSTANDENTRY2, reinterpret_cast<int>(&entry), 2, 0, 0, 0, 0);
    actor->characterLevel = packet->level;
    if (isPc && !IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
      actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
      actor->SetGroupMemberVisible(1);
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.shield = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);
  entry.state = 0;

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
    actor->characterLevel = packet->level;
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->SendMsg(nullptr, AM_STANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, AM_INITSTANDENTRY2, reinterpret_cast<int>(&entry), 2, 0, 0, 0, 0);
    actor->characterLevel = packet->level;
    if (isPc) {
      *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
      if (!IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
        actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
    if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
      actor->SetGroupMemberVisible(1);
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.shield = packet->weapon >> 31;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.shieldAppearance = packet->shield;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
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
      if (emblem > 0 && !actor->GetGdid()) {
        actor->SetGuildInfo(emblem, packet->shield);
      }
    }
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->SendMsg(nullptr, AM_STANDENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
  } else {
    int isPc = 0;
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job)) {
      isPc = 1;
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    actor->SendMsg(nullptr, AM_INITSTANDENTRY, reinterpret_cast<int>(&entry), 2, 0, 0, 0, 0);
    const int actorJob = actor->GetEffectiveJob(-1);
    if ((actorJob >= 45 && actorJob < 1000) || (actorJob > 1000 && actorJob < 3999) || static_cast<unsigned int>(actorJob - 20021) <= 11978) {
      const std::uint32_t emblem = (packet->accessory2 << 16) | packet->accessory3;
      if (emblem > 0) {
        actor->SetGuildInfo(emblem, packet->shield);
      }
    }
    if (isPc && !IsAdminAccount(packet->aid) && !StateFlag06(packet->effectState)) {
      actor->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
      (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.shield = packet->shield;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  if (packet->objectType != 0) {
    entry.shieldAppearance = entry.guildVersion;
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
      entry.appearanceVariant = 0;
    }
    actor->objectType = packet->objectType;
  }

  if (packet->objectType == 0) {
    *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
    actor->characterLevel = packet->level;
    if (created) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, value, 0, 0, 0, 0, 0);
      if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
    } else {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_STANDENTRY2, value, 0, 0, 0, 0, 0);
    }
    if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    if (packet->objectType == 7) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
      }
    } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
      g_session.homunAlive = 1;
    } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
      g_session.mercenaryAlive = 1;
    }

    if (created) {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_INITSTANDENTRY, value, 0, 0, 0, 0, 0);
    } else {
      const std::int64_t value = reinterpret_cast<std::intptr_t>(&entry);
      actor->SendMsg(nullptr, AM_STANDENTRY, value, 0, 0, 0, 0, 0);
    }
    if (g_session.IsGuardianJob(packet->job) && g_session.GetSiegeMode() && actor->sceneNameWindow == 0) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }

    const int guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (created) {
      if (guildId != 0) {
        actor->SetGuildInfo(guildId, packet->guildVersion);
      }
    } else if (guildId != 0 && !actor->GetGdid()) {
      actor->SetGuildInfo(guildId, packet->guildVersion);
    }
  }

  if (created) {
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
  }
  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.state = packet->state;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.guildVersion = packet->guildVersion;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.shield = packet->shield;
  entry.aid = packet->aid;
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(packet->aid));
  if (actor) {
    if (entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    actor->SendMsg(nullptr, AM_STANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
        (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
      this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
    }
  } else {
    const short job = packet->job;
    if ((45 > job || 1000 <= job) && (1000 >= job || 3999 <= job) && static_cast<unsigned short>(job - 20021) > 11978 && !g_session.IsCompanionJob(job)) {
      actor = new CPc;
    } else {
      if (g_session.IsHomunAid(packet->aid)) {
        g_session.homunAlive = 1;
      }
      if (g_session.IsMercenaryAid(packet->aid)) {
        g_session.mercenaryAlive = 1;
      }
      actor = this->NewActor(packet->job);
      entry.appearanceVariant = 0;
    }

    if (actor && entry.head == 0x2E) {
      if (g_session.petGid == packet->aid) {
        g_session.petAccessory = entry.accessory;
      }
      if (entry.accessory) {
        actor->SetActionName(g_session.GetPetAccActName(entry.accessory));
      }
    }
    this->scene->objects.push_back(actor);
    this->scene->actors.push_back(actor);
    if (actor) {
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
      if (g_session.GetSiegeMode() && entry.head != 0x2E && actor->sceneNameWindow == 0 &&
          (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
        this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
      }
      if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
    }
  }

  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
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
  entry.appearanceVariant = this->SubD07740(entry.job, sex);
  entry.speed = speed;
  entry.xSize = xSize;
  entry.ySize = ySize;
  entry.head = head;
  entry.weapon = weapon;
  entry.accessory = accessory;
  entry.effectState = healthState;
  entry.bodyState = bodyState;
  entry.stateFlags = effectState;
  entry.pkMode = pkMode;
  entry.accessory2 = accessory2;
  entry.accessory3 = accessory3;
  entry.headPalette = headPalette;
  entry.bodyPalette = bodyPalette;
  entry.headDirection = headDir;
  entry.guildId = guildId;
  entry.honor = honor;
  entry.virtue = virtue;
  entry.robe = robe;
  entry.shield = shield;
  entry.guildVersion = guildVersion;
  if (objectType == 0) {
    entry.state = 0;
  } else {
    entry.shieldAppearance = entry.guildVersion;
  }
  entry.gid = gid;
  entry.bodyCostume = body;
  entry.cellX = (posDir[0] << 2) | (posDir[1] >> 6);
  entry.direction = posDir[2] & 0x0F;
  entry.cellY = ((posDir[1] & 0x3F) << 4) | (posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (objectType == 0) {
      actor->SendMsg(nullptr, AM_STANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, AM_STANDENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
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

    created->objectType = objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (objectType == 0) {
      if (this->partyPosList.find(aid) != this->partyPosList.end() || this->guildPosList.find(aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, reinterpret_cast<int>(&entry), 2, 0, 0, 0, 0);
    } else {
      if (!((objectType == 1 || objectType == 6 || objectType == 5) && IsPlayerJob(job))) {
        entry.appearanceVariant = 0;
      }
      actor->SendMsg(nullptr, AM_INITSTANDENTRY, reinterpret_cast<int>(&entry), 1, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (isBoss && actor->hpGaugeFlags == 0) {
    actor->hpGaugeFlags = isBoss;
    bossFlag = 1;
  }
  actor->characterLevel = level;
  *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = font;
  if (objectType == 7) {
    if (g_session.IsPetAid(aid)) {
      g_session.petAccessory = entry.accessory;
    }
    if (entry.accessory && !actor->GetGdid()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
    }
  } else if (objectType == 8 && g_session.IsHomunAid(aid)) {
    g_session.homunAlive = 1;
  } else if (objectType == 9 && g_session.IsMercenaryAid(aid)) {
    g_session.mercenaryAlive = 1;
  } else if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, guildId, guildVersion);
  }

  switch (actor->objectType) {
  case PC_TYPE:
  case NPC_MOB_TYPE:
  case NPC_HO_TYPE:
  case NPC_MERSOL_TYPE:
  case NPC_ELEMENTAL_TYPE: {
    const std::int64_t currentHp = hp;
    const std::int64_t currentMaxHp = maxHp;
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, currentHp, currentMaxHp, bossFlag, 0, 0, 0);
    break;
  }
  case NPC_ABR_TYPE:
  case NPC_BIONIC_TYPE:
    actor->SendMsg(nullptr, AM_UPDATEGAGE, gid, 0, 0, 0, 0, 0);
    break;
  }

  if (objectType != 0) {
    const std::uint32_t emblemGuildId = (accessory2 << 16) | accessory3;
    if (emblemGuildId > 0 && !actor->GetGdid()) {
      actor->SetGuildInfo(emblemGuildId, guildVersion);
    }
    this->ApplyActorStatusEffects(aid, effectState, level, virtue);
  } else {
    this->ApplyActorStatusEffects(aid, effectState, level, virtue);
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
  /* 0002 */ std::uint16_t padding_0002;
  /* 0004 */ int itemId;
  /* 0008 */ char name[24];
  /* 0020 */ int materialId;
  /* 0024 */ int refine;
  /* 0028 */ unsigned char grade;
};

struct TradeBroadcastPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t padding_0002;
  /* 0004 */ int itemId;
  /* 0008 */ char name[24];
  /* 0020 */ char otherName[24];
};

struct ItemBroadcastPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t padding_0002;
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
  item.refiningLevel = packet->refine;
  item.identified = 1;
  item.grade = packet->grade;
  ITEM_INFO material;
  material.SetItemId(packet->materialId);
  material.identified = 1;

  std::string text = FormatOwnedString(LookupMessage(MSI_BROADCASTING_SPECIAL_ITEM_OBTAIN), name.c_str(), material.DisplayName(0).c_str(), item.DisplayName(1).c_str());
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message;
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

  std::string text = FormatOwnedString(LookupMessage(MSI_BROADCASTING_SPECIAL_ITEM_OBTAIN), name.c_str(), otherName.c_str(), item.DisplayName(1).c_str());
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message;
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

  std::string text = FormatOwnedString(LookupMessage(MSI_BROADCASTING_SPECIAL_ITEM_OBTAIN2), name.c_str(), item.DisplayName(0).c_str());
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message;
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
  entry.appearanceVariant = this->SubD07740(entry.job, sex);
  entry.speed = speed;
  entry.xSize = xSize;
  entry.ySize = ySize;
  entry.head = head;
  entry.weapon = weapon;
  entry.accessory = accessory;
  entry.effectState = healthState;
  entry.bodyState = bodyState;
  entry.stateFlags = effectState;
  entry.pkMode = pkMode;
  entry.accessory2 = accessory2;
  entry.accessory3 = accessory3;
  entry.headPalette = headPalette;
  entry.bodyPalette = bodyPalette;
  entry.headDirection = headDir;
  entry.guildId = guildId;
  entry.honor = honor;
  entry.virtue = virtue;
  entry.robe = robe;
  entry.shield = shield;
  entry.guildVersion = guildVersion;
  entry.state = state;
  if (objectType != 0) {
    entry.shieldAppearance = entry.guildVersion;
  }
  entry.gid = gid;
  entry.bodyCostume = body;
  entry.cellX = (posDir[0] << 2) | (posDir[1] >> 6);
  entry.direction = posDir[2] & 0x0F;
  entry.cellY = ((posDir[1] & 0x3F) << 4) | (posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (objectType == 0) {
      actor->SendMsg(nullptr, AM_STANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, AM_STANDENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
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

    created->objectType = objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (objectType == 0) {
      if (this->partyPosList.find(aid) != this->partyPosList.end() || this->guildPosList.find(aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    } else {
      if ((objectType == 1 || objectType == 6 || objectType == 12) && IsPlayerJob(job)) {
        entry.bodyCostume = job;
      } else {
        entry.appearanceVariant = 0;
      }
      actor->SendMsg(nullptr, AM_INITSTANDENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (isBoss && actor->hpGaugeFlags == 0) {
    actor->hpGaugeFlags = isBoss;
    bossFlag = 1;
  }
  actor->characterLevel = level;
  *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = font;
  if (objectType == 7) {
    if (g_session.IsPetAid(aid)) {
      g_session.petAccessory = entry.accessory;
    }
    if (entry.accessory && !actor->GetGdid()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
    }
  } else if (objectType == 8 && g_session.IsHomunAid(aid)) {
    g_session.homunAlive = 1;
  } else if (objectType == 9 && g_session.IsMercenaryAid(aid)) {
    g_session.mercenaryAlive = 1;
  } else if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, guildId, guildVersion);
  }

  switch (actor->objectType) {
  case PC_TYPE:
  case NPC_MOB_TYPE:
  case NPC_HO_TYPE:
  case NPC_MERSOL_TYPE:
  case NPC_ELEMENTAL_TYPE: {
    const std::int64_t currentHp = hp;
    const std::int64_t currentMaxHp = maxHp;
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, currentHp, currentMaxHp, bossFlag, 0, 0, 0);
    break;
  }
  case NPC_ABR_TYPE:
  case NPC_BIONIC_TYPE:
    actor->SendMsg(nullptr, AM_UPDATEGAGE, gid, 0, 0, 0, 0, 0);
    break;
  }

  if (objectType != 0 && 0x2D2 == job) {
    actor->SetGuildInfo(actor->GetGdid(), guildVersion);
    this->ApplyActorStatusEffects(aid, effectState, level, virtue);
  } else {
    this->ApplyActorStatusEffects(aid, effectState, level, virtue);
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

  std::string expLine = LookupMessage(MSI_EXP_MSG);
  std::string dropLine = LookupMessage(MSI_DROP_MSG);
  std::string deathLine = LookupMessage(MSI_DEATH_MSG);
  expLine += FormatOwnedString(
    LookupMessage(MSI_MAX_WEIGHT_IN_RODEX_CLIENT), static_cast<float>(*reinterpret_cast<const int*>(packet + 4)) * 0.001f, static_cast<float>(basic.exp) * 0.001f,
    static_cast<float>(server.exp) * 0.001f, g_session.m_serverName.c_str(), static_cast<float>(premium.exp) * 0.001f
  );
  dropLine += FormatOwnedString(
    LookupMessage(MSI_MAX_WEIGHT_IN_RODEX_CLIENT), static_cast<float>(*reinterpret_cast<const int*>(packet + 12)) * 0.001f, static_cast<float>(basic.drop) * 0.001f,
    static_cast<float>(server.drop) * 0.001f, g_session.m_serverName.c_str(), static_cast<float>(premium.drop) * 0.001f
  );
  deathLine += FormatOwnedString(
    LookupMessage(MSI_MAX_WEIGHT_IN_RODEX_CLIENT), static_cast<float>(*reinterpret_cast<const int*>(packet + 8)) * 0.001f, static_cast<float>(basic.death) * 0.001f,
    static_cast<float>(server.death) * 0.001f, g_session.m_serverName.c_str(), static_cast<float>(premium.death) * 0.001f
  );

  std::uint32_t color = 0xFFFF;
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message;
    message.lines.emplace_back(expLine, color);
    message.lines.emplace_back(dropLine, color);
    message.lines.emplace_back(deathLine, color);
    balloon->AddMessage(0, message);
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("=================================================================="), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(expLine.c_str()), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(dropLine.c_str()), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(deathLine.c_str()), 0x62B0FF, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("=================================================================="), 0x62B0FF, 0, 0, 0);
}

// OFFSET: 0x00D21EB0
// FUNCTION: fn_21658
void CGameMode::ReceiveColorBroadcast(const unsigned char* data) {
  char text[256];
  std::memcpy(text, data + 16, *reinterpret_cast<const std::uint16_t*>(data + 2) - 16);
  const int length = static_cast<int>(std::strlen(text));
  if (g_forbiddenWords.Contains(text)) {
    std::string masked("");
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
    BroadcastBalloonMessage balloonMessage;
    balloonMessage.fontHeight = *reinterpret_cast<const short*>(data + 10);
    message = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(message);
    balloonMessage.lines.emplace_back(message, color);
    balloon->AddMessage(0, balloonMessage);
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), color, 0x19, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(message.c_str()), color, 0x19, 0, 0);
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
  const unsigned int size = std::min<unsigned int>(0x3FF, *reinterpret_cast<const std::uint16_t*>(data + 2) - 4);
  std::memcpy(text, data + 4, size);
  text[size] = '\0';
  const int length = static_cast<int>(std::strlen(text));

  std::string message(text);
  std::uint32_t color = 0xFFFF;
  std::memcpy(tag, text, std::min<std::size_t>(4, std::strlen(text)));
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

  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(
    UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), color, fontType, reinterpret_cast<std::intptr_t>(senderName), 0
  );
  if (!systemMessage) {
    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(message.c_str()), color, fontType, reinterpret_cast<std::intptr_t>(senderName), 0
    );
  }

  message = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(message);
  std::string linked;
  DisplayNavigationLink(linked, message);
  message = std::move(linked);

  std::vector<std::string> lines;
  WrapPlainText(message.c_str(), lines, 0x48);
  if (lines.begin() != lines.end()) {
    if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
      BroadcastBalloonMessage balloonMessage;
      for (const std::string& line : lines) {
        balloonMessage.lines.emplace_back(line, color);
      }
      balloon->AddMessage(0, balloonMessage);
    }
  }
}

// OFFSET: 0x00D231F0
// FUNCTION: fn_21661
void CGameMode::ReceiveRefineStatusGraded(const char* playerName, int itemId, unsigned char refine, unsigned char grade, bool success) {
  ITEM_INFO item;
  item.SetItemId(itemId);
  item.refiningLevel = refine;
  item.grade = grade;
  item.identified = 1;

  int job = 0;
  const char* signatureText = OBFUSCATE_BYTES_TEXT("d>d", 0xED796785D1DF137FULL);
  g_session.m_hotkeyScript->Call(OBFUSCATE_BYTES_TEXT("GetEquipJob", 0xF14F79170939C7F1ULL), signatureText, itemId, &job);
  if (job != 0) {
    item.identity[0] = 0x15;
  }

  std::string itemName = item.DisplayName(1);
  std::string text = FormatOwnedString(LookupMessage(!success + 0x1108), playerName, itemName.c_str());
  std::string line = text;
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x62B0FF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message;
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
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_BROADCASTING_SPECIAL_ITEM_OBTAIN2), name.c_str(), item.DisplayName(0).c_str()));
  } else if (package && data[4] == 0) {
    const int count = *reinterpret_cast<const int*>(tail + 16);
    if (count > 0) {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_FAIL_ITEMCOMPOSITION_LIST), name.c_str(), source.c_str(), count, item.DisplayName(0).c_str()));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_BROADCASTING_SPECIAL_ITEM_OBTAIN), name.c_str(), source.c_str(), item.DisplayName(0).c_str()));
    }
  } else {
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_BROADCASTING_SPECIAL_ITEM_OBTAIN), name.c_str(), source.c_str(), item.DisplayName(0).c_str()));
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
  if (UIBroadcastBalloon* balloon = this->EnsureBroadcastBalloon()) {
    BroadcastBalloonMessage message;
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
  entry.appearanceVariant = this->SubD07740(entry.job, packet->sex);
  entry.speed = packet->speed;
  entry.xSize = packet->xSize;
  entry.ySize = packet->ySize;
  entry.head = packet->head;
  entry.weapon = packet->weapon;
  entry.accessory = packet->accessory;
  entry.effectState = packet->healthState;
  entry.bodyState = packet->bodyState;
  entry.stateFlags = packet->effectState;
  entry.shield = packet->shield;
  entry.pkMode = packet->pkMode;
  entry.accessory2 = packet->accessory2;
  entry.accessory3 = packet->accessory3;
  entry.headPalette = packet->headPalette;
  entry.bodyPalette = packet->bodyPalette;
  entry.headDirection = packet->headDir;
  entry.guildId = packet->guildId;
  entry.honor = packet->honor;
  entry.virtue = packet->virtue;
  entry.guildVersion = packet->guildVersion;
  entry.robe = packet->robe;
  entry.aid = packet->aid;
  entry.state = packet->state;
  if (packet->objectType != 0) {
    entry.shieldAppearance = entry.guildVersion;
  }
  entry.cellX = (packet->posDir[0] << 2) | (packet->posDir[1] >> 6);
  entry.direction = packet->posDir[2] & 0x0F;
  entry.cellY = ((packet->posDir[1] & 0x3F) << 4) | (packet->posDir[2] >> 4);

  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.aid));
  if (actor) {
    if (packet->objectType == 0) {
      actor->SendMsg(nullptr, AM_STANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    } else {
      actor->SendMsg(nullptr, AM_STANDENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
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

    created->objectType = packet->objectType;
    this->scene->objects.push_back(created);
    this->scene->actors.push_back(created);
    actor = created;
    if (packet->objectType == 0) {
      if (this->partyPosList.find(packet->aid) != this->partyPosList.end() || this->guildPosList.find(packet->aid) != this->guildPosList.end()) {
        actor->SetGroupMemberVisible(1);
      }
      actor->SendMsg(nullptr, AM_INITSTANDENTRY2, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    } else {
      entry.appearanceVariant = 0;
      actor->SendMsg(nullptr, AM_INITSTANDENTRY, reinterpret_cast<int>(&entry), 0, 0, 0, 0, 0);
    }
    g_championMonsterMgr->ApplyEffects(entry.job, actor, entry.aid);
  }

  std::int64_t bossFlag = 0;
  if (packet->isBoss && actor->hpGaugeFlags == 0) {
    actor->hpGaugeFlags = packet->isBoss;
    bossFlag = 1;
  }
  actor->characterLevel = packet->level;
  *reinterpret_cast<short*>(actor->render.fontSequenceBytes) = packet->font;
  if (packet->objectType == 7) {
    if (g_session.IsPetAid(packet->aid)) {
      g_session.petAccessory = entry.accessory;
    }
    if (entry.accessory && !actor->GetGdid()) {
      actor->SetActionName(ResourceMgr::Instance(), g_session.GetPetAccActName(entry.accessory));
    }
  } else if (packet->objectType == 8 && g_session.IsHomunAid(packet->aid)) {
    g_session.homunAlive = 1;
  } else if (packet->objectType == 9 && g_session.IsMercenaryAid(packet->aid)) {
    g_session.mercenaryAlive = 1;
  } else if (g_session.GetSiegeMode() && actor->sceneNameWindow == 0 &&
             (g_session.IsPlayerJobId(actor->GetEffectiveJob(-1)) || g_session.IsGuardianJob(actor->GetEffectiveJob(-1)))) {
    this->ShowGuildEmblem(actor, packet->guildId, packet->guildVersion);
  }

  switch (actor->objectType) {
  case PC_TYPE:
  case NPC_MOB_TYPE:
  case NPC_HO_TYPE:
  case NPC_MERSOL_TYPE:
  case NPC_ELEMENTAL_TYPE: {
    const std::int64_t hp = packet->hp;
    const std::int64_t maxHp = packet->maxHp;
    actor->SendMsg(nullptr, AM_UPDATE_MONSTER_GAGE, hp, maxHp, bossFlag, 0, 0, 0);
    break;
  }
  case NPC_ABR_TYPE:
  case NPC_BIONIC_TYPE:
    actor->SendMsg(nullptr, AM_UPDATEGAGE, static_cast<std::uint32_t>(packet->aid), 0, 0, 0, 0, 0);
    break;
  }

  if (packet->objectType != 0) {
    const std::uint32_t guildId = (packet->accessory2 << 16) | packet->accessory3;
    if (guildId > 0 && !actor->GetGdid()) {
      actor->SetGuildInfo(guildId, packet->guildVersion);
    }
  }
  this->ApplyActorStatusEffects(packet->aid, packet->effectState, packet->level, packet->virtue);
}

namespace {
#pragma pack(push, 1)
struct PartyMemberInfoPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ std::int32_t leader;
  /* 000A */ unsigned char position[4];
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
    std::string masked("");
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.partyName = partyName;

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
      g_session.partyLeader = 1;
    }
  } else if (member.offline == 1) {
    this->SendMsg(MM_DELETEPARTYPOS, packet->accountId, 0, 0, 0);
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
      this->SendMsg(MM_DELETEPARTYPOS, member.accountId, 0, 0, 0);
    }
  }

  g_session.AddPartyMember(member);
  RefreshPartyWindows(&g_session);
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
    std::string masked("");
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.partyName = partyName;

  if (packet->length != 0x1C) {
    const int count = (packet->length - 0x1C) / 0x2E;
    const PartyMemberListEntry* entries = reinterpret_cast<const PartyMemberListEntry*>(data + sizeof(*packet));
    for (int index = 0; index < count; ++index) {
      const PartyMemberListEntry entry = entries[index];
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
        g_session.partyLeader = 1;
      }
    }
    RefreshPartyWindows(&g_session);
  }
}

// OFFSET: 0x00D59FD0
// FUNCTION: fn_21960
void CGameMode::SetPartyMemberList(const char* value, std::vector<PartyRosterEntry>* members) {
  g_session.ClearPartyMembers();
  char partyName[25];
  partyName[24] = '\0';
  std::memcpy(partyName, value, 24);
  if (g_forbiddenWords.Contains(partyName)) {
    std::string masked("");
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.partyName = partyName;

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
      g_session.partyLeader = 1;
    }
    g_session.AddPartyMember(member);
  }

  g_session.LoadMiniPartyInfo();
  g_windowMgr.RestoreMiniPartyWindows();
  RefreshPartyWindows(&g_session);
}

// OFFSET: 0x00D5EEF0
// FUNCTION: fn_21968
void CGameMode::UpdatePartyMemberInfo(
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
    std::string masked("");
    g_forbiddenWords.Mask(partyName, &masked);
  }
  g_session.partyName = partyName;

  if (member.accountId == g_session.m_aid) {
    if (member.leader == 0) {
      g_session.partyLeader = 1;
    }
  } else if (member.offline == 1) {
    this->SendMsg(MM_DELETEPARTYPOS, accountId, 0, 0, 0);
    this->SetActorGroupMemberVisible(accountId, 0);
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
      this->SendMsg(MM_DELETEPARTYPOS, accountId, 0, 0, 0);
    }
    this->SetActorGroupMemberVisible(accountId, sameMap);
  }

  g_session.partyItemDistribute = itemDistribute != 0;
  g_session.partyItemCollect = itemShare != 0;
  g_session.PartyCount();
  g_session.AddPartyMember(member);
  g_windowMgr.NotifyMemberWindows(&member);
  this->SetActorGroupMemberVisible(accountId, 1);
}

// OFFSET: 0x00D5B940
// FUNCTION: fn_21965
void LaunchSkillActorEffect(CGameActor* actor, short x, short y, std::uint16_t skillId, int delay, int effectId) {
  if (skillId == KN_BOWLINGBASH || skillId == KN_BRANDISHSPEAR) {
    return;
  }

  if (g_effectMgr->HasAllocator(effectId)) {
    g_effectMgr->Spawn(actor->render.accountId, effectId, actor->render.position, 0, -1, 0, 0);
  } else {
    const int delayFrames = delay / 16;
    actor->SendMsg(nullptr, AM_MAKE_BEGIN_SPELL, effectId, delayFrames, 0, 0, 0, 0);
  }

  CEffect* effect;

  switch (skillId) {
  default: {
    if (CSkillActionMgr::GetInstance()->OnSkillCast(skillId, delay, actor) == true) {
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
      effect = g_effectMgr->Spawn(actor->render.accountId, static_cast<int>(primaryEffects[key]), position, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
    }
    if (secondaryEffects.find(key) != secondaryEffects.end()) {
      effect = g_effectMgr->Spawn(actor->render.accountId, static_cast<int>(secondaryEffects[key]), position, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
    }
    return;
  }
  case RK_IGNITIONBREAK:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x65A, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case GC_POISONSMOKE:
    actor->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    return;
  case WL_RECOGNIZEDSPELL:
    actor->LaunchEffect(0x323, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    return;
  case WL_TETRAVORTEX:
    if (g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x5A0, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x597, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    } else {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x4FD, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x4ED, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WL_COMET:
    if (g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x600, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x603, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    } else {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x587, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x58D, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WL_JACKFROST:
    effect = g_effectMgr->Spawn(actor->render.accountId, g_serviceOptions.Get(0xE5) ? 0x607 : 0x5A6, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WL_HELLINFERNO:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x610, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AB_CLEARANCE:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x617, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WL_SOULEXPANSION:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x633, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x634, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AB_JUDEX:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x65D, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x65E, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WL_CHAINLIGHTNING:
    g_effectMgr->Spawn(actor->render.accountId, 0x663, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x664, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case RA_AIMEDBOLT:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x671, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case NC_ARMSCANNON: {
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
      g_effectMgr->Spawn(actor->render.accountId, id, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x5D4, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x5D5, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    } else {
      g_effectMgr->Spawn(actor->render.accountId, id, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, id + 1, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x568, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      g_effectMgr->Spawn(actor->render.accountId, 0x569, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
    return;
  }
  case SO_EL_CURE: {
    g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    CRagEffect* ragEffect = actor->LaunchEffect(0xD8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (ragEffect) {
      vector3d position = actor->render.position;
      ragEffect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    return;
  }
  case SO_DIAMONDDUST: {
    vector3d position{0.0f, 0.0f, 0.0f};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(x, y, position.x, position.z);
    position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(position.x, position.z);
    effect = g_effectMgr->Spawn(0, g_serviceOptions.Get(0xE5) ? 0x5AC : 0x504, position, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  }
  case NPC_PSYCHIC_WAVE:
  case SO_PSYCHIC_WAVE: {
    vector3d position{0.0f, 0.0f, 0.0f};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(x, y, position.x, position.z);
    position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(position.x, position.z);
    effect = g_effectMgr->Spawn(0, g_serviceOptions.Get(0xE5) ? 0x5B0 : 0x50A, position, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  }
  case GN_CARTCANNON:
    if (g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x5DB, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x5DC, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    } else {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x58E, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x58F, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case NPC_RAYOFGENESIS:
  case LG_RAYOFGENESIS:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x591, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x592, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case SO_VARETYR_SPEAR:
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x61B, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x61C, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case SC_MAELSTROM:
    actor->LaunchEffect(0x10, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    return;
  case SR_GATEOFHELL:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x625, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case SR_HOWLINGOFLION:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62A, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62B, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WM_METALICSOUND:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62D, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x62E, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case GN_WALLOFTHORN:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x654, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    if (g_serviceOptions.Get(0xE5)) {
      return;
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x655, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case GN_MANDRAGORA:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x669, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WM_SOUND_OF_DESTRUCTION:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x66A, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x66B, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case LG_OVERBRAND:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x697, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    if (g_serviceOptions.Get(0xE5)) {
      return;
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x698, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_DEADLY_PROJECTION:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CB, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_DESTRUCTIVE_HURRICANE:
  case AG_STORM_CANNON:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6C6, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6C7, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
      effect->AddEvent(0, "effect\\EF_BeginSpell.wav", 0, 0);
    }
    return;
  case AG_SOUL_VC_STRIKE:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6CF, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_ROCK_DOWN:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6D3, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6D4, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_RAIN_OF_CRYSTAL:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6D9, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6DA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_CRIMSON_ARROW:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6DD, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6DE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_VIOLENT_QUAKE:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6E1, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6E2, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_ASTRAL_STRIKE:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6E9, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6EA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_MYSTERY_ILLUSION:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6ED, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6EE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_TORNADO_STORM:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F2, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F3, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_FROZEN_SLASH:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F6, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x6F7, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_CLIMAX:
    g_effectMgr->Spawn(actor->render.accountId, 0x70A, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x70B, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetDelay(200);
      effect->SetLifeTime(delay - 200);
    }
    return;
  case AG_FLORAL_FLARE_ROAD:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x70D, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x70E, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_ALL_BLOOM:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x704, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x705, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case IQ_FIRST_FAITH_POWER:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7B8, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case IQ_JUDGE:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7BB, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case IQ_THIRD_EXOR_FLAME:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7BF, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_MEDIALE_VOTUM:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x711, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_ARGUTUS_VITA:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x714, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_ARGUTUS_TELUM:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x716, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_PRESENS_ACIES:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x718, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_ARBITRIUM:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x71F, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    if (g_serviceOptions.Get(0xE5)) {
      return;
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x71E, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_COMPETENTIA: {
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x73B, actor->render.position, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
    }

    CGroundScope* scope = new (std::nothrow) CGroundScope;
    if (!scope) {
      return;
    }

    int column;
    int row;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->WorldCell(actor->render.position.x, actor->render.position.z, column, row);
    scope->SendMsg(nullptr, AM_SETMASTERGID, static_cast<std::uint32_t>(actor->render.accountId), 0, 0, 0, 0, 0);
    scope->SendMsg(nullptr, AM_SETPOS, column, row, 0, 0, 0, 0);
    scope->SendMsg(nullptr, AM_SETTIME, static_cast<std::uint32_t>(timeGetTime() + delay), 0, 0, 0, 0, 0);
    const int id = skillId;
    scope->SendMsg(nullptr, AM_SET_SKILL, id, 0, 0, 0, 0, 0);
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(scope);
    return;
  }
  case AG_CRYSTAL_IMPACT:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x740, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x741, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case MT_SUMMON_ABR_MOTHER_NET:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x76D, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_RELIGIO:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x745, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WH_FLAMETRAP:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x781, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WH_DEEPBLINDTRAP:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x751, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WH_SWIFTTRAP:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x755, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WH_SOLIDTRAP:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x758, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x759, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case ABC_ABYSS_SLAYER:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x84F, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x850, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_BENEDICTUM:
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x760, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x761, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_PNEUMATICUS_PROCELLA:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7A6, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case CD_DILECTIO_HEAL:
    if (!g_serviceOptions.Get(0xE5)) {
      effect = g_effectMgr->Spawn(actor->render.accountId, 0x785, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      if (effect) {
        effect->SetLifeTime(delay);
      }
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x786, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case AG_STRANTUM_TREMOR:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x762, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x763, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case MT_SUMMON_ABR_INFINITY:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x774, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case MT_SUMMON_ABR_BATTLE_WARIOR:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x78F, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case MT_SUMMON_ABR_DUAL_CANNON:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x77C, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case WH_HAWKBOOMERANG:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7B1, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case EM_INCREASING_ACTIVITY:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7C9, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7CA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case EM_ACTIVITY_BURN:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7D4, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7D5, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case EM_SUMMON_ELEMENTAL_SERPENS:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DA, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DB, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case ABC_CHAIN_REACTION_SHOT:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7DF, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case EM_SUMMON_ELEMENTAL_DILUVIO:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7E5, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x7E6, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    return;
  case TR_NIPELHEIM_REQUIEM:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x81C, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay);
    }
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x81D, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->effectLifetime = delay;
    }
    return;
  case TR_MUSICAL_INTERLUDE:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x84B, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay + 50);
    }
    return;
  case MH_HEILIGE_PFERD:
    effect = g_effectMgr->Spawn(actor->render.accountId, 0x91D, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetLifeTime(delay + 50);
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
    if (skillId == CR_AUTOGUARD || skillId == ML_AUTOGUARD) {
      source->SetState(2, 1);
    }
    if (g_session.IsMonsterActor(source) == 0 && skillId != MG_SRECOVERY) {
      if (skillId == WE_MALE || skillId == WE_FEMALE) {
        this->SendMsg(MM_MAKE_SKILL_USED_MSG, skillId, reinterpret_cast<std::intptr_t>(source), targetAid, 0);
      } else {
        this->SendMsg(MM_MAKE_SKILL_USED_MSG, skillId, reinterpret_cast<std::intptr_t>(source), level, 0);
      }
    } else {
      int loopType;
      g_session.GetMonsterSkillInfo(g_session.GetJobName(source->GetEffectiveJob(-1), -1), skillId, &action, &loopType);
      if (action != -1) {
        source->SetAction(action * 8 - 8, 1, loopType == 0);
      }
    }
    if (g_session.IsMonsterActor(source) == 1) {
      this->SendMsg(MM_MAKE_SKILL_USED_MSG, skillId, reinterpret_cast<std::intptr_t>(source), level, 0);
    }
    if (action == -1) {
      switch (skillId) {
      case MO_TRIPLEATTACK:
      case MO_CHAINCOMBO:
      case MO_COMBOFINISH:
      case CH_TIGERFIST:
      case SR_SKYNETBLOW:
      case SR_EARTHSHAKER:
      case SR_FALLENEMPIRE:
      case SJ_NEWMOONKICK:
      case ABC_CHAIN_REACTION_SHOT:
      case ABC_FRENZY_SHOT:
      case WH_HAWK_M:
        break;
      default: {
        int effectId;
        int motion;
        g_session.GetSkillActionInfo2(skillId, effectId, motion);
        if (source->render.state != 1 && motion == 1) {
          source->SetState(7, motion);
        } else {
          if (motion == 2 || motion == 9) {
            source->SendMsg(nullptr, AM_SET_TARGET_AID, targetAid, 0, 0, 0, 0, 0);
            if (CSkillActionMgr::GetInstance()->SuppressSupportSkillArrow(skillId)) {
              source->SetArrowEffectSuppressed(true);
            }
          }
          source->SetState(motion, 1);
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
  case HT_REMOVETRAP:
  case HT_SPRINGTRAP:
    if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
      if (source != nullptr) {
        source->Face(skill->render.position.x, skill->render.position.z);
      }
    }
    break;
  case AM_REST:
    if (sourceAid == g_session.m_aid) {
      if (g_windowMgr.homunInfoWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_HOMUNINFOWND);
      }
      if (g_windowMgr.homunculusSkillWnd != nullptr) {
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_0);
      }
      g_session.ClearHomunStatus();
      g_session.homunAlive = 0;
    }
    break;
  case NPC_RANDOMBREAK:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x985, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&target->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case NPC_STRIP_SHADOW:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x984, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&target->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case MG_SAFETYWALL:
  case MG_FIREWALL:
  case MG_THUNDERSTORM:
  case PR_SANCTUARY:
  case PR_MAGNUS:
  case WZ_METEOR:
  case WZ_VERMILION:
  case WZ_STORMGUST:
  case BS_HAMMERFALL:
  case CR_SLIMPITCHER:
  case NPC_STORMGUST2:
  case NPC_MAGMA_ERUPTION:
  case NPC_RAINOFMETEOR:
  case NC_MAGMA_ERUPTION:
  case SU_CN_METEOR:
  case SU_CN_METEOR2:
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
    effectLevel = skillId != MG_SRECOVERY ? 10 : level;
  }

  switch (skillId) {
  case PR_GLORIA:
    if (source != nullptr) {
      source->LaunchEffect(0x4B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_SLOWPOISON:
    if (g_serviceType == 0 && target != nullptr) {
      target->LaunchEffect(0x88, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BS_WEAPONPERFECT:
    if (target != nullptr) {
      target->LaunchEffect(0x67, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BS_MAXIMIZE:
    if (source != nullptr) {
      source->LaunchEffect(0x68, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AS_ENCHANTPOISON:
  case AS_SPLASHER:
    if (target != nullptr) {
      target->LaunchEffect(0x7D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AS_POISONREACT:
    if (target != nullptr) {
      target->LaunchEffect(0x7E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case KN_TWOHANDQUICKEN:
  case KN_ONEHAND:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x82, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x82, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case KN_AUTOCOUNTER:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_MAKE_SKILL_RECHARGEGAGE, level * 400, 0, 0, 0, 0, 0);
      source->SetState(13, 1);
      if (sourceAid != g_session.m_aid) {
        source->SendMsg(nullptr, AM_DELETE_SKILL_RECHARGEGAGE, 0, 0, 0, 0, 0, 0);
      }
    }
    break;
  case PR_IMPOSITIO:
    if (target != nullptr) {
      target->LaunchEffect(0x54, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_SUFFRAGIUM:
    if (target != nullptr) {
      target->LaunchEffect(0x58, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_ASPERSIO:
    if (target != nullptr) {
      target->LaunchEffect(0x56, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AL_CURE:
    if (target != nullptr) {
      target->LaunchEffect(0x42, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_STRECOVERY:
    if (target != nullptr) {
      target->LaunchEffect(0x4E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_LEXAETERNA:
  case NPC_LEX_AETERNA:
  case AB_VITUPERATUM:
    if (target != nullptr) {
      target->LaunchEffect(0x55, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WZ_SIGHTRASHER:
    if (source != nullptr) {
      source->LaunchEffect(0x3E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WZ_FROSTNOVA:
    if (target != nullptr) {
      target->LaunchEffect(0x1C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HT_SPRINGTRAP:
    if (source != nullptr) {
      if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
        static_cast<EffectActor*>(skill)->LaunchEffect(0x6F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (source->statusEffects[0] != 0) {
          const int address = reinterpret_cast<int>(&skill->render.position);
          reinterpret_cast<CGameObject*>(source->statusEffects[0])->SendMsg(nullptr, AM_REQ_ATTACK, address, 0, 0, 0, 0, 0);
        }
      }
    }
    break;
  case HT_REMOVETRAP:
    if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
      static_cast<EffectActor*>(skill)->LaunchEffect(0x64, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ALL_RESURRECTION:
    if (target != nullptr) {
      target->LaunchEffect(0x4D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (IsPlayerJob(target->GetEffectiveJob(-1))) {
        target->LaunchEffect(0x8C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case TF_DETOXIFY:
    if (target != nullptr) {
      target->LaunchEffect(0x15, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case TF_STEAL:
    if (target != nullptr) {
      target->LaunchEffect(0x12, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MG_STONECURSE:
    if (target != nullptr && skillId == MG_STONECURSE) {
      target->LaunchEffect(0x17, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("_stonecurse.wav", target->render.position.x - this->scene->player->render.position.x, 0.0f, target->render.position.z - this->scene->player->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case AL_DECAGI:
    if (target != nullptr) {
      target->LaunchEffect(0x26, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AL_HOLYWATER:
    if (source != nullptr) {
      source->LaunchEffect(0x27, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AL_CRUCIS:
    if (source != nullptr) {
      source->LaunchEffect(0x28, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AL_ANGELUS:
    if (source != nullptr) {
      source->LaunchEffect(0x29, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SM_ENDURE:
    if (source != nullptr) {
      source->LaunchEffect(0xB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SM_PROVOKE:
    if (target != nullptr) {
      target->LaunchEffect(0x43, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AC_CONCENTRATION:
    if (source != nullptr) {
      source->LaunchEffect(0x99, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MG_ENERGYCOAT:
    if (source != nullptr) {
      source->LaunchEffect(0xA9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGEWATER:
    if (source != nullptr) {
      source->LaunchEffect(0xAE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGEGROUND:
    if (source != nullptr) {
      source->LaunchEffect(0xB1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGEFIRE:
    if (source != nullptr) {
      source->LaunchEffect(0xAD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGEWIND:
    if (source != nullptr) {
      source->LaunchEffect(0xAF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGEPOISON:
    if (source != nullptr) {
      source->LaunchEffect(0xB3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGEHOLY:
    if (source != nullptr) {
      source->LaunchEffect(0xB2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGEDARKNESS:
    if (source != nullptr) {
      source->LaunchEffect(0xAC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_CHANGETELEKINESIS:
    if (source != nullptr) {
      source->LaunchEffect(0xB0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_SUICIDE:
    if (source != nullptr) {
      if (CRagEffect* effect = this->scene->player->LaunchEffect(0xB9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&source->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case NPC_SUMMONSLAVE:
    if (source != nullptr) {
      source->LaunchEffect(0xD7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_KEEPING:
    if (source != nullptr) {
      source->LaunchEffect(0xD6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_DEFENDER:
    if (source != nullptr) {
      source->LaunchEffect(0xD5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case CR_REFLECTSHIELD:
    if (source != nullptr) {
      source->LaunchEffect(0xFC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MC_LOUD:
    if (source != nullptr) {
      source->LaunchEffect(0x137, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RG_STEALCOIN:
    if (target != nullptr) {
      if (sourceAid == g_session.m_aid) {
        char notice[512];
        std::sprintf(notice, LookupMessage(MSI_NOTIFY_STEAL_COIN), static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(target->render.accountId).c_str(), level);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(notice), 0xC8C8FF, 0x10, 0, 0);
      }
      if (CRagEffect* effect = target->LaunchEffect(0x112, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      target->LaunchEffect(0x10C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RG_RAID:
    if (source != nullptr) {
      source->LaunchEffect(0x114, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RG_STRIPSHIELD:
    if (target != nullptr) {
      target->LaunchEffect(0x10E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RG_STRIPARMOR:
    if (target != nullptr) {
      target->LaunchEffect(0x10F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RG_STRIPHELM:
    if (target != nullptr) {
      target->LaunchEffect(0x110, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AM_CP_WEAPON:
  case AM_CP_SHIELD:
  case AM_CP_ARMOR:
  case AM_CP_HELM:
    if (target != nullptr) {
      target->LaunchEffect(0x12C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AM_POTIONPITCHER:
    this->LaunchTargetEffect(0x12B, source, target, effectLevel);
    break;
  case MG_SRECOVERY:
    if (target != nullptr) {
      target->SendMsg(nullptr, AM_MAKE_NUMBER, effectLevel, 0xFF0000FFu, 0x72, 0, 0, 0);
    }
    break;
  case CR_DEVOTION:
    if (source != nullptr) {
      source->LaunchEffect(0xFB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SA_SPELLBREAKER:
    if (target != nullptr) {
      target->LaunchEffect(0xEA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SA_DISPELL:
  case NPC_WIDEDISPEL:
    if (target != nullptr) {
      target->LaunchEffect(0xEB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SA_MAGICROD:
    if (source != nullptr) {
      source->LaunchEffect(0xF4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case CR_PROVIDENCE:
    if (target != nullptr) {
      target->LaunchEffect(0xF8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case CR_SPEARQUICKEN:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0xFA, 0, 0, 0, 0, 0);
      source->LaunchEffect(0xFA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MO_ABSORBSPIRITS:
    if (source != nullptr) {
      source->LaunchEffect(0xFD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MO_BLADESTOP:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 3, 3, 0x60, 3, 1, 3000, 0);
    }
    break;
  case MO_EXPLOSIONSPIRITS:
    if (source != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\mon_폭기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0xCB, 0, 0, 0, 0, 0);
      source->LaunchEffect(0xCB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x105, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetSequenceInterval(30);
      source->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, 1000, 0);
    }
    break;
  case MO_STEELBODY:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      source->LaunchEffect(0x105, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 20, 0);
    }
    break;
  case BS_ADRENALINE:
  case BS_ADRENALINE2:
    if (source != nullptr) {
      source->LaunchEffect(0x62, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BS_OVERTHRUST:
  case WS_OVERTHRUSTMAX:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x80, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x80, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_STOP:
    if (target != nullptr) {
      target->LaunchEffect(0x1C5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WE_FEMALE:
    if (target != nullptr) {
      target->LaunchEffect(0xFD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->SendMsg(nullptr, AM_MAKE_NUMBER, level, 0xFF0000FFu, 0x72, 0, 0, 0);
    }
    break;
  case WE_BABY:
    if (target != nullptr) {
      target->LaunchEffect(0x198, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case LK_AURABLADE:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x16F, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x16F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1DA, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x1DA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetFrameSequence(0x20, 0, 5, 0x20, 5, 1, 1000, 0);
      g_soundMgr->PlayWave("effect\\오라 블레이드.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case LK_CONCENTRATION:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x171, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x171, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ST_FULLSTRIP:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x1EF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case ST_PRESERVE:
    if (source != nullptr) {
      source->LaunchEffect(0x1F0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_POWERUP:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\mon_폭기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x1C7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RG_STRIPWEAPON:
  case NPC_WEAPONBRAKER:
    if (target != nullptr) {
      target->LaunchEffect(0x10D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case DC_SCREAM:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 1200, 0);
      if (source == this->scene->player) {
        source->LaunchEffect(0x128, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case BA_FROSTJOKE:
    if (source != nullptr) {
      source->SetSequenceInterval(10);
      source->SetFrameSequence(0x60, 4, 5, 0x60, 4, 1, 1800, 1);
      if (source == this->scene->player) {
        source->LaunchEffect(0x127, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case BA_ASSASSINCROSS:
    g_soundMgr->PlayWave("effect\\석양의 어쌔신.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x348, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BA_POEMBRAGI:
    g_soundMgr->PlayWave("effect\\브라기의 시.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BA_APPLEIDUN:
    g_soundMgr->PlayWave("effect\\이둔의 사과.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x349, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case DC_HUMMING:
    g_soundMgr->PlayWave("effect\\흥얼거림.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case DC_DONTFORGETME:
    g_soundMgr->PlayWave("effect\\나를잊지말아요.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case DC_SERVICEFORYOU:
    g_soundMgr->PlayWave("effect\\당신을 위한 서비스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x348, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_LULLABY:
    g_soundMgr->PlayWave("effect\\자장가.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case CG_HERMODE:
    g_soundMgr->PlayWave("effect\\헤르모드의 지팡이.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case CG_TAROTCARD:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\priest_slowpoison.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x1F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_RICHMANKIM:
    g_soundMgr->PlayWave("effect\\김서방돈.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_ETERNALCHAOS:
    g_soundMgr->PlayWave("effect\\영원의 혼돈.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_DRUMBATTLEFIELD:
    g_soundMgr->PlayWave("effect\\전장의.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_RINGNIBELUNGEN:
    g_soundMgr->PlayWave("effect\\니벨룽겐의 반지.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_ROKISWEIL:
    g_soundMgr->PlayWave("effect\\로키.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_INTOABYSS:
    g_soundMgr->PlayWave("effect\\심연속으로.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BD_SIEGFRIED:
    g_soundMgr->PlayWave("effect\\불사신.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BA_WHISTLE:
    if (source != nullptr) {
      source->LaunchEffect(0x34C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case DC_FORTUNEKISS:
    g_soundMgr->PlayWave("effect\\행운의.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x348, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_AGIUP:
  case GS_INCREASING:
    if (source != nullptr) {
      source->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HW_SOULDRAIN:
    if (source != nullptr && target != nullptr) {
      source->SetFrameSequence(0x58, 0, 3, 0x58, 3, 1, 1000, 0);
      if (CRagEffect* effect = source->LaunchEffect(0x17A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      target->LaunchEffect(0x17B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HW_MAGICPOWER:
    if (target != nullptr) {
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x17E, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x17E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\마법력 증폭.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case PF_HPCONVERSION:
    if (source != nullptr) {
      source->LaunchEffect(0x17F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PF_SOULCHANGE:
    if (source != nullptr && target != nullptr) {
      vector3d offset{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = source->LaunchEffect(0x180, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&offset), 0, 0, 0, 0);
      }
      source->LaunchEffect(0x181, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x181, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x197, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x197, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PF_SOULBURN:
    if (target != nullptr) {
      target->LaunchEffect(0x196, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x17C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ASC_EDP:
    if (source != nullptr) {
      source->LaunchEffect(0x1ED, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SN_SIGHT:
    if (source != nullptr) {
      source->LaunchEffect(0x182, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SN_WINDWALK:
    if (source != nullptr) {
      source->LaunchEffect(0x185, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WS_MELTDOWN:
    if (source != nullptr) {
      source->LaunchEffect(0x186, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WS_CARTBOOST:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\EF_IncAgility.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x187, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ST_CHASEWALK:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_MAKE_SMOKE_FFECT, 0, 0, 0, 0, 0, 0);
    }
    break;
  case ST_REJECTSWORD:
    if (source != nullptr) {
      source->LaunchEffect(0x188, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case CG_MOONLIT:
    if (source != nullptr && !source->HasId(0x18A)) {
      source->LaunchEffect(0x18A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    g_soundMgr->PlayWave("effect\\달빛.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case CG_MARIONETTE:
    if (source != nullptr && target != nullptr) {
      vector3d offset{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = source->LaunchEffect(0x18B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&offset), 0, 0, 0, 0);
      }
      source->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x18C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PF_MINDBREAKER:
    if (target != nullptr) {
      target->LaunchEffect(0x193, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PF_MEMORIZE:
  case HLIF_CHANGE:
    if (source != nullptr) {
      source->LaunchEffect(0x1F9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ASC_METEORASSAULT:
    if (source != nullptr) {
      source->LaunchEffect(0x199, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case TK_RUN:
    if (source != nullptr) {
      if (source->HasId(0x1BB)) {
        source->LaunchEffect(0x1BC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (!source->HasId(0x1BA)) {
        source->LaunchEffect(0x1BA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;
  case TK_STORMKICK:
    if (source != nullptr) {
      source->LaunchEffect(0x1B3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case TK_DODGE:
    g_soundMgr->PlayWave("effect\\T_낙법.wav", 0.0f, -150.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case TK_SEVENWIND:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\T_바람방출.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      while (source->HasId(0x1CD)) {
        source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1CD, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(0x1CD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      const int effectId = effectLevel + 0x1D2;
      while (source->HasId(effectId)) {
        source->SendMsg(nullptr, AM_DELETE_EFFECT, effectId, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case TK_HIGHJUMP:
    if (source != nullptr) {
      if (source->HasId(0x1BE)) {
        source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1BE, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      while (source->HasId(0x1BD)) {
        source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1BD, 0, 0, 0, 0, 0);
      }
    }
    break;
  case SG_SUN_WARM:
  case SG_MOON_WARM:
  case SG_STAR_WARM:
    if (source != nullptr && target != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1A2, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x1A2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x220, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x220, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x23C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SG_SUN_COMFORT:
  case SG_MOON_COMFORT:
  case SG_STAR_COMFORT:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1E8, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x1E8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SG_HATE:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_등록.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x21F, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SG_FUSION:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\T_변신.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
    }
    break;
  case AM_BERSERKPITCHER:
    if (source != nullptr && target != nullptr) {
      this->LaunchTargetEffect(0x21D, source, target, 4);
      target->LaunchEffect(0xDC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_KAIZEL:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\priest_resurrection.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x24E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_KAAHI:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_보조마법.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_KAUPE:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_치잉.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x21E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_KAITE:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_마법반사.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1A3, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x21E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_SWOO:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_슈웃.wav", 0.0f, -50.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1A4, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x24D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_SKE:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_공격력.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1AB, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1AB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_SKA:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\T_방어형.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0xFE, 0, 0, 0, 0, 0);
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x46C, 0, 0, 0, 0, 0);
      target->LaunchEffect(0xFE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x46C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x105, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case CR_FULLPROTECTION:
    if (target != nullptr) {
      target->LaunchEffect(0x12C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PF_DOUBLECASTING:
    if (target != nullptr) {
      target->LaunchEffect(0x209, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\마법력 증폭.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case CG_LONGINGFREEDOM:
    g_soundMgr->PlayWave("effect\\ac_concentration.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x34B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case TK_MISSION:
    g_soundMgr->PlayWave("effect\\T_피링.wav", 0.0f, -100.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case GS_GLITTERING:
    g_soundMgr->PlayWave("effect\\플립.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case GS_MADNESSCANCEL:
    if (source != nullptr) {
      source->LaunchEffect(0x271, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case GS_ADJUSTMENT:
  case GS_GATLINGFEVER:
    if (source != nullptr) {
      source->LaunchEffect(0x272, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case GS_CRACKER:
    g_soundMgr->PlayWave("effect\\크래커.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case GS_DISARM:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x273, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case GS_DESPERADO:
    if (source != nullptr) {
      source->LaunchEffect(0x27D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NJ_TATAMIGAESHI:
    g_soundMgr->PlayWave("effect\\다다미뒤집기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case NJ_BAKUENRYU:
    if (target != nullptr) {
      target->LaunchEffect(0x27B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NJ_HYOUSYOURAKU:
    if (source != nullptr) {
      source->LaunchEffect(0x27C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NJ_RAIGEKISAI:
    if (source != nullptr) {
      source->LaunchEffect(0x26E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MB_M_TELEPORT:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_MAKE_SMOKE_FFECT, 0, 0, 0, 0, 0, 0);
    }
    if (target != nullptr) {
      target->SendMsg(nullptr, AM_MAKE_SMOKE_FFECT, 0, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x158, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NV_FIRSTAID:
  case RK_VITALITYACTIVATION:
    if (source != nullptr) {
      source->LaunchEffect(0x135, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SA_FLAMELAUNCHER:
  case SA_ELEMENTFIRE:
    if (target != nullptr) {
      target->LaunchEffect(0xFF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SA_FROSTWEAPON:
  case SA_ELEMENTWATER:
    if (target != nullptr) {
      target->LaunchEffect(0x100, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SA_LIGHTNINGLOADER:
  case SA_ELEMENTWIND:
    if (target != nullptr) {
      target->LaunchEffect(0x101, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SA_SEISMICWEAPON:
  case SA_ELEMENTGROUND:
    if (target != nullptr) {
      target->LaunchEffect(0x102, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BA_DISSONANCE:
  case DC_UGLYDANCE:
    if (source != nullptr) {
      source->LaunchEffect(0x34B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HP_ASSUMPTIO:
  case CASH_ASSUMPTIO:
    if (target != nullptr) {
      target->LaunchEffect(0x177, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1B8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SL_ALCHEMIST:
  case SL_MONK:
  case SL_STAR:
  case SL_SAGE:
  case SL_CRUSADER:
  case SL_SUPERNOVICE:
  case SL_KNIGHT:
  case SL_WIZARD:
  case SL_PRIEST:
  case SL_BARDDANCER:
  case SL_ROGUE:
  case SL_ASSASIN:
  case SL_BLACKSMITH:
  case SL_HUNTER:
  case SL_SOULLINKER:
  case SL_HIGH:
  case SL_DEATHKNIGHT:
  case SL_COLLECTOR:
  case SL_NINJA:
  case SL_GUNNER:
    if (target != nullptr) {
      vector3d offset{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = target->LaunchEffect(0x1F7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&offset), 0, 0, 0, 0);
      }
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1A8, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case DE_BERSERKAIZER:
    if (source != nullptr) {
      source->LaunchEffect(0xDC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ALL_CATCRY:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\2008cat.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case ALL_PARTYFLEE:
    if (target != nullptr) {
      target->LaunchEffect(0x2BB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ALL_ANGEL_PROTECT:
    if (target != nullptr) {
      target->LaunchEffect(0x5B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case ALL_DREAM_SUMMERNIGHT:
    if (source != nullptr) {
      g_soundMgr->PlayWave("dream_device_edq.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case ALL_WEWISH:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\wewish.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x2CD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_MAXPAIN:
  case NPC_MAXPAIN_ATK:
    if (source != nullptr) {
      source->LaunchEffect(0x408, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_DEATHSUMMON:
    if (source != nullptr) {
      source->LaunchEffect(0x409, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_HELLBURNING:
    if (source != nullptr) {
      source->LaunchEffect(0x19, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_WIDEWEB:
    if (source != nullptr) {
      source->LaunchEffect(0x41D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_WIDESUCK:
    if (source != nullptr && target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x41C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = source->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case NPC_SR_CURSEDCIRCLE:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_CURSEDCIRCLE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case NPC_MANDRAGORA:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\G하울링오브만드라고라.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case NPC_VENOMIMPRESS:
  case GC_VENOMIMPRESS:
    if (target != nullptr) {
      target->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\수둔.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case NPC_IGNITIONBREAK:
    g_soundMgr->PlayWave("effect\\wl_jackfrost.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x2D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case NPC_HALLUCINATIONWALK:
  case GC_HALLUCINATIONWALK:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\HALLUCINATIONWALK.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case NPC_LEASH:
  case NPC_WIDELEASH:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x459, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x459, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case NPC_RELIEVE_ON:
    if (target != nullptr) {
      target->LaunchEffect(0x640, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case All_EVENT_20TH_ANNIVERSARY:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x94E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CR_SHRINK:
    if (source != nullptr) {
      source->LaunchEffect(0x257, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RG_CLOSECONFINE:
    if (target != nullptr) {
      target->LaunchEffect(0x25A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case BA_PANGVOICE:
    if (source != nullptr && target != nullptr) {
      source->SetFrameSequence(0x60, 4, 5, 0x60, 4, 1, 0x708, 1);
      g_soundMgr->PlayWave("amon_ra_die01.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (CRagEffect* effect = source->LaunchEffect(0x25E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case DC_WINKCHARM:
    if (source != nullptr && target != nullptr) {
      const std::int64_t emotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Chup);
      source->SendMsg(nullptr, AM_CASH_EMOTION, 0, emotion, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\vallentine.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (CRagEffect* effect = source->LaunchEffect(0x25F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case BS_GREED:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\ef_entry.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case RK_ENCHANTBLADE:
    if (source != nullptr) {
      source->LaunchEffect(0x2F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\닌자_던지기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case GC_ANTIDOTE:
    if (target != nullptr) {
      target->LaunchEffect(0x15, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case GC_POISONINGWEAPON:
    if (source != nullptr) {
      source->LaunchEffect(0x303, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case GC_WEAPONBLOCKING:
    if (source != nullptr) {
      source->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\t_낙법.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case GC_COUNTERSLASH:
    if (source != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x570, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (!g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(source->render.accountId, 0x571, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case GC_ROLLINGCUTTER:
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
  case WL_TETRAVORTEX_FIRE:
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
  case WL_TETRAVORTEX_WATER:
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
  case WL_TETRAVORTEX_WIND:
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
  case WL_TETRAVORTEX_GROUND:
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
  case WL_RELEASE:
    if (source != nullptr && target != nullptr) {
      if (CRagEffect* effect = source->LaunchEffect(0x2EF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case AL_INCAGI:
  case CASH_INCAGI:
  case AB_CANTO:
  case MER_INCAGI:
    if (target != nullptr) {
      target->LaunchEffect(0x25, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AL_BLESSING:
  case CASH_BLESSING:
  case AB_CLEMENTIA:
  case MER_BLESSING:
    if (target != nullptr) {
      target->LaunchEffect(0x2A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_LEXDIVINA:
  case AB_SILENTIUM:
    if (target != nullptr) {
      target->LaunchEffect(0x57, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_MILLENNIUMSHIELD:
  case RK_MILLENNIUMSHIELD:
    if (source != nullptr) {
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x2ED, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\ef_soulstrike.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case NPC_JACKFROST:
  case WL_JACKFROST:
    if (source != nullptr && target != nullptr) {
      for (int count = 0; count < 10; ++count) {
        CRagEffect* effect = source->LaunchEffect(0x321, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          vector3d position = source->render.position;
          const short index = static_cast<short>(std::max(std::rand() % 36, 0) + count * 36);
          position.x += g_cosTable[index] * 80.0f;
          position.z += g_sinTable[index] * 80.0f;
          effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
        }
      }
      g_soundMgr->PlayWave("jack_frost.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case NPC_PHANTOMTHRUST:
  case RK_PHANTOMTHRUST:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RK_DEATHBOUND:
    if (source != nullptr) {
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RK_WINDCUTTER:
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
  case RK_IGNITIONBREAK:
    g_soundMgr->PlayWave("effect\\wl_jackfrost.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x658, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x659, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case RK_DRAGONHOWLING:
    if (source != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      source->LaunchEffect(0x2DB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("dragon_h.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case RK_REFRESH:
    if (source != nullptr) {
      source->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case RK_GIANTGROWTH:
    if (source != nullptr) {
      source->LaunchEffect(0x2F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\닌자_던지기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case RK_STONEHARDSKIN:
  case RK_FIGHTINGSPIRIT:
    if (source != nullptr) {
      source->LaunchEffect(0x2F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RK_STORMBLAST:
    if (source != nullptr) {
      source->LaunchEffect(0x11, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x107, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x14A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RK_ABUNDANCE:
    if (source != nullptr) {
      source->LaunchEffect(0x21A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\흡기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case AB_ANCILLA:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\AB_ancilla.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AB_PRAEFATIO:
    if (target != nullptr) {
      target->LaunchEffect(0x70, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AB_ORATIO:
    if (source != nullptr) {
      source->LaunchEffect(0x2D5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AB_LAUDAAGNUS:
  case AB_LAUDARAMUS:
    if (target != nullptr) {
      target->LaunchEffect(0x42, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("lauagnus.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case AB_RENOVATIO:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\AB_renovatio.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x2CF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AB_CLEARANCE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x615, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x616, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case AB_EXPIATIO:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\바실리카.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case AB_DUPLELIGHT:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\AB_duplelight.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (!target->HasId(0x341)) {
        CRagEffect* effect = target->LaunchEffect(0x341, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        vector3d scale{2.0f, 0.0f, 0.0f};
        if (effect) {
          effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&scale), 0, 0, 0, 0);
        }
      }
    }
    break;
  case WL_MARSHOFABYSS:
    if (target != nullptr) {
      target->LaunchEffect(0x2D9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WL_RECOGNIZEDSPELL:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\wl_recognizedspell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WL_SIENNAEXECRATE:
    if (target != nullptr) {
      target->LaunchEffect(0x31D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WL_STASIS:
    if (source != nullptr) {
      source->LaunchEffect(0x31F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RA_FEARBREEZE:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x60D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case RA_WUGMASTERY:
    g_soundMgr->PlayWave("wolf_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case RA_WUGRIDER:
    g_soundMgr->PlayWave("wolf_stand.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case NC_ACCELERATION:
    if (source != nullptr) {
      source->LaunchEffect(0x309, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case LG_SHIELDSPELL:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\LG_SHIELDSPELL.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case LG_PRESTIGE:
    if (source != nullptr) {
      source->LaunchEffect(0x38C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\LG_PRESTIGE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case LG_INSPIRATION:
    if (source != nullptr) {
      source->LaunchEffect(0x38E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SR_WINDMILL:
    if (source != nullptr) {
      source->LaunchEffect(0x1B3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_WINDMILL.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SO_SPELLFIST:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\S스펠피스트.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_soundMgr->PlayWave("effect\\S파이어워크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_soundMgr->PlayWave("effect\\S일렉트릭워크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SO_STRIKING:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\S스트라이킹.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SO_ARRULLO:
    if (target != nullptr) {
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\S포이즌버스터.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      vector3d position{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      if (CRagEffect* effect = target->LaunchEffect(0x3A3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case SO_EL_ACTION:
    if (source != nullptr) {
      source->LaunchEffect(0x3A6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case GN_CART_TORNADO:
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
  case GN_CARTCANNON:
    if (source != nullptr && target != nullptr) {
      const std::uint32_t accountId = target->render.accountId;
      CEffect* effect = g_effectMgr->Spawn(accountId, 0x54D, source->render.position.x, source->render.position.y, source->render.position.z, accountId, -1, 0, 0);
      if (effect) {
        effect->SetDelay(200);
      }
      g_soundMgr->PlayWave("effect\\mon_침투경.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case GN_BLOOD_SUCKER:
    if (source != nullptr && target != nullptr) {
      g_soundMgr->PlayWave("effect\\S바큠익스트림.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (CRagEffect* effect = target->LaunchEffect(0xD8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = source->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case GN_SPORE_EXPLOSION:
    this->LaunchTargetEffect(0x392, source, target, 4);
    break;
  case GN_WALLOFTHORN:
    g_soundMgr->PlayWave("effect\\G가시나무덫.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case GN_MANDRAGORA:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x667, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x668, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\G하울링오브만드라고라.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case GN_SLINGITEM:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (CRagEffect* effect = source->LaunchEffect(0x1B7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
        g_soundMgr->PlayWave("effect\\G스포어익스플로젼.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
    }
    break;
  case GN_CHANGEMATERIAL:
  case GN_MIX_COOKING:
  case GN_MAKEBOMB:
  case GN_S_PHARMACY:
    if (source != nullptr) {
      source->LaunchEffect(0x38F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case LG_REFLECTDAMAGE:
  case LG_FORCEOFVANGUARD:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\LG_REFLECTDAMAGE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_GENTLETOUCH_CURE:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("_heal_effect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_GENTLETOUCH_ENERGYGAIN:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\세크리파이스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_GENTLETOUCH_CHANGE:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\샤프슈팅.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_GENTLETOUCH_REVITALIZE:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\mon_탄지신통.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_RAISINGDRAGON:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_RAISINGDRAGON.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_POWERVELOCITY:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_POWERVELOCITY.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_CRESCENTELBOW:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_CRESCENTELBOW.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_LIGHTNINGWALK:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_LIGHTNINGWALK.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_CURSEDCIRCLE:
    if (source != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->SetFrameSequence(96, 3, 3, 96, 3, 1, 3000, 0);
      g_soundMgr->PlayWave("effect\\SR_CURSEDCIRCLE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case SR_CRESCENTELBOW_AUTOSPELL:
    if (source != nullptr && target != nullptr) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      CRagEffect* effect = source->LaunchEffect(0x37C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect != nullptr) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
        g_soundMgr->PlayWave("effect\\SR_CRESCENTELBOW.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
    }
    break;
  case WM_RANDOMIZESPELL:
    if (target != nullptr) {
      target->LaunchEffect(0x35E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\AB_ancilla.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WA_SWING_DANCE:
    if (target != nullptr) {
      target->LaunchEffect(0x34F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\스윙댄스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WA_SYMPHONY_OF_LOVER:
    if (target != nullptr) {
      target->LaunchEffect(0x34F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\연인들을위한심포니.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WA_MOONLIT_SERENADE:
    if (target != nullptr) {
      target->LaunchEffect(0x34F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\AB_judex.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case MI_RUSH_WINDMILL:
    if (target != nullptr) {
      target->LaunchEffect(0x350, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\풍차를향해돌격.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case MI_ECHOSONG:
    if (target != nullptr) {
      target->LaunchEffect(0x350, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\메아리의노래.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_VOICEOFSIREN:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\세이렌의목소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case MI_HARMONIZE:
    if (target != nullptr && source != nullptr) {
      vector3d position{0.0f, 0.0f, 0.0f};
      position.y = static_cast<float>(target->render.accountId);
      CRagEffect* targetEffect = target->LaunchEffect(0x351, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (targetEffect != nullptr) {
        targetEffect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      position.y = static_cast<float>(source->render.accountId);
      CRagEffect* sourceEffect = source->LaunchEffect(0x351, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (sourceEffect != nullptr) {
        sourceEffect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
      g_soundMgr->PlayWave("effect\\하모나이즈.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case NC_REPAIR:
    if (target != nullptr) {
      target->LaunchEffect(0x311, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_REPRODUCE:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x631, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x632, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\reproduce.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case SC_AUTOSHADOWSPELL:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x6C2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x6C3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\autoshadow.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case SC_BODYPAINT:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\bodypaint.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x32B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_INVISIBILITY:
    g_soundMgr->PlayWave("effect\\invisibility.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case SC_DEADLYINFECT:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\deadlyinfect.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x32C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_ENERVATION:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\enervation.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x32D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_GROOMY:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\groomy.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x32E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_IGNORANCE:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\ignorance.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x32F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_LAZINESS:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\laziness.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x330, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_UNLUCKY:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\unlucky.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x331, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_WEAKNESS:
    if (target != nullptr) {
      g_soundMgr->PlayWave("effect\\weakness.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      target->LaunchEffect(0x332, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_STRIPACCESSARY:
    if (target != nullptr) {
      CRagEffect* effect = target->LaunchEffect(0x334, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect != nullptr) {
        vector3d position = target->render.position;
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  case SC_SHADOWFORM: {
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\shadowform.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x33A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    vector3d position{static_cast<float>(sourceAid), static_cast<float>(targetAid), 0.0f};
    if (position.y != 0.0f && position.x != 0.0f && source != nullptr) {
      CRagEffect* effect = source->LaunchEffect(0x333, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect != nullptr) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&position), 0, 0, 0, 0);
      }
    }
    break;
  }
  case NC_EMERGENCYCOOL:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\ef_decagility.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NC_STEALTHFIELD:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\t_등록.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x1F4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NC_NEUTRALBARRIER:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\mon_금강불괴.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x30E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NC_SHAPESHIFT:
    if (source != nullptr) {
      source->LaunchEffect(0x30B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NC_HOVERING:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\HOVERING.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x21F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x30A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NC_B_SIDESLIDE:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\B_SIDESLIDE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x30A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NC_F_SIDESLIDE:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\F_SIDESLIDE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      source->LaunchEffect(0x30A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WM_SIRCLEOFNATURE:
    if (target != nullptr) {
      target->LaunchEffect(0x35D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\순환하는자연의소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_GLOOMYDAY:
    if (target != nullptr) {
      target->LaunchEffect(0x360, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\AB_renovatio.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_DEADHILLHERE:
    if (target != nullptr) {
      target->LaunchEffect(0x35D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      target->LaunchEffect(0x355, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\사망의골짜기에서.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_SONG_OF_MANA:
    if (target != nullptr) {
      target->LaunchEffect(0x364, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\마나의노래.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_DANCE_WITH_WUG:
    if (target != nullptr) {
      target->LaunchEffect(0x362, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\워그와함께춤을.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_SATURDAY_NIGHT_FEVER:
    if (target != nullptr) {
      target->LaunchEffect(0x366, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\새터데이나이트피버.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_LERADS_DEW:
    if (target != nullptr) {
      target->LaunchEffect(0x368, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_LIGHTNINGWALK.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_MELODYOFSINK:
    if (target != nullptr) {
      target->LaunchEffect(0x36A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\멜로디오브싱크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_BEYOND_OF_WARCRY:
    if (target != nullptr) {
      target->LaunchEffect(0x36C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\비욘드오브워크라이.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_UNLIMITED_HUMMING_VOICE:
    if (target != nullptr) {
      target->LaunchEffect(0x36E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\언리미티드허밍보이스.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case WM_LULLABY_DEEPSLEEP:
    target->LaunchEffect(0x3C9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\안식의자장가.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case GN_CARTBOOST:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\G카트부스트.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x68B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x68C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case WM_SOUND_OF_DESTRUCTION:
    if (source != nullptr) {
      source->LaunchEffect(0x34A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case LG_EARTHDRIVE:
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
        effect->SetScale(scale);
      }
      effect = g_effectMgr->Spawn(0, 0x6AB, position.x, position.y, position.z, 0, -1, 0, 0);
      if (effect) {
        effect->SetScale(scale);
      }
    }
    break;
  case AB_SECRAMENT:
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
  case SR_HOWLINGOFLION:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x628, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x629, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\SR_HOWLINGOFLION.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case RL_MASS_SPIRAL:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x427, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case RL_QD_SHOT:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x434, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case RL_FALLEN_ANGEL:
    if (source != nullptr) {
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      while (source->HasId(0x430)) {
        source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x430, 0, 0, 0, 0, 0);
      }
    }
    break;
  case RL_R_TRIP:
    if (source != nullptr) {
      source->LaunchEffect(0x27D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_effectMgr->Spawn(0, 0x1B3, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case RL_B_FLICKER_ATK:
    if (CRenderObject* skill = this->scene->GetSkillByAID(targetAid)) {
      g_effectMgr->Spawn(0, 0x432, skill->render.position.x, skill->render.position.y, skill->render.position.z, 0, -1, 0, 0);
    }
    break;
  case RL_P_ALTER:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x435, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case SJ_STAREMPEROR:
    if (source != nullptr) {
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_effectMgr->Spawn(source->render.accountId, 0x4B3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      while (source->HasId(0x1BD)) {
        source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1BD, 0, 0, 0, 0, 0);
      }
    }
    break;
  case SP_KAUTE:
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
  case SP_SOULUNITY:
    if (target != nullptr) {
      if (source != nullptr) {
        vector3d data{0.0f, static_cast<float>(source->render.accountId), 0.0f};
        if (CRagEffect* effect = target->LaunchEffect(0x4BA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
          effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&data), 0, 0, 0, 0);
        }
      }
      target->LaunchEffect(0x197, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SP_SOULGOLEM:
  case SP_SOULSHADOW:
  case SP_SOULFALCON:
  case SP_SOULFAIRY:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x1F7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        vector3d data{0.0f, static_cast<float>(target->render.accountId), 0.0f};
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&data), 0, 0, 0, 0);
      }
      target->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1A8, 0, 0, 0, 0, 0);
      target->LaunchEffect(0x1A8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      switch (skillId) {
      case SP_SOULGOLEM:
        g_effectMgr->Spawn(target->render.accountId, 0x4CC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      case SP_SOULSHADOW:
        g_effectMgr->Spawn(target->render.accountId, 0x4D3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      case SP_SOULFALCON:
        g_effectMgr->Spawn(target->render.accountId, 0x4D5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      case SP_SOULFAIRY:
        g_effectMgr->Spawn(target->render.accountId, 0x4D4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        break;
      }
    }
    break;
  case SJ_PROMINENCEKICK:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x4D7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case SP_SOULCURSE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x4D6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case SP_SOULREVOLVE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x4DB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_soundMgr->PlayWave(
        "effect\\t_방어형.wav", target->render.position.x - this->scene->player->render.position.x, 0.0f,
        target->render.position.z - this->scene->player->render.position.z, 250, 40, 1.0f, 0
      );
    }
    break;
  case KO_HUUMARANKA:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x3D8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x3E8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x3EA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case ALL_FULL_THROTTLE:
  case EVT_FULL_THROTTLE:
    if (source != nullptr) {
      source->LaunchEffect(0x412, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SR_FLASHCOMBO:
    if (target != nullptr) {
      target->LaunchEffect(0x413, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case RK_LUXANIMA:
    if (source != nullptr) {
      source->LaunchEffect(0x414, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case AB_OFFERTORIUM:
    if (source != nullptr) {
      source->LaunchEffect(0x417, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SC_ESCAPE:
    if (source != nullptr && !source->HasId(0x439)) {
      vector3d position = source->render.position;
      if (CRagEffect* effect = source->LaunchEffect(0x439, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        const std::int64_t value = reinterpret_cast<std::intptr_t>(&position);
        effect->SendMsg(nullptr, AM_SETCOUNT, 0, 0xFFFF, 0x422, 0, static_cast<int>(value), static_cast<int>(value >> 32));
      }
    }
    break;
  case WM_FRIGG_SONG:
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
  case SO_ELEMENTAL_SHIELD:
    if (source != nullptr) {
      source->LaunchEffect(0x416, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case WL_TELEKINESIS_INTENSE:
    if (source != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave(
        "effect\\Wl_Telekinesis_Intense.wav", player->render.position.x - source->render.position.x, 0.0f,
        player->render.position.z - source->render.position.z, 250, 40, 1.0f, 0
      );
    }
    break;
  case SU_FRESHSHRIMP:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x44A, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_TUNABELLY:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x44E, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_SCRATCH:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x44F, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_BITE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x451, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_SCAROFTAROU:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x453, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_ARCLOUSEDASH:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x454, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_PICKYPECK:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x455, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_LUNATICCARROTBEAT:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x44B, target->render.position.x, target->render.position.y, target->render.position.z, 0, level, 0, 0);
    }
    break;
  case SU_LUNATICCARROTBEAT2:
    if (target != nullptr) {
      g_effectMgr->Spawn(0, 0x457, target->render.position.x, target->render.position.y, target->render.position.z, 0, level, 0, 0);
    }
    break;
  case SU_LOPE:
    if (source != nullptr) {
      source->LaunchEffect(0x1BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      while (source->HasId(0x1BD)) {
        source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x1BD, 0, 0, 0, 0, 0);
      }
      source->LaunchEffect(0x19B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SU_BUNCHOFSHRIMP:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x45C, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_GROOMING:
  case SU_PURRING:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x463, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_CHATTERING:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x464, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_HISS:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x466, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case SU_POWEROFFLOCK:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x467, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    break;
  case WE_ONEFOREVER:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x475, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0);
    }
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x475, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case WE_CHEERUP:
    if (target != nullptr) {
      target->LaunchEffect(0x198, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x476, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    }
    break;
  case BD_ADAPTATION:
  case CG_SPECIALSINGER:
    if (source != nullptr) {
      source->LaunchEffect(0x350, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NV_HELPANGEL:
    if (source != nullptr) {
      g_soundMgr->PlayWave("effect\\priest_sanctuary.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x557, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x558, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case DK_DRAGONIC_AURA:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x6BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x6BB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x6BC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(100);
      }
      effect = g_effectMgr->Spawn(target->render.accountId, 0x6BD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(100);
      }
    }
    break;
  case AG_DEADLY_PROJECTION:
  case AG_SOUL_VC_STRIKE:
    if (source != nullptr) {
      source->SetFrameSequence(96, 1, 1, 96, 1, 1, 500, 0);
    }
    break;
  case AG_DESTRUCTIVE_HURRICANE:
    g_effectMgr->Spawn(source->render.accountId, 0x6C4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, 0x6C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case AG_CRYSTAL_IMPACT:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x73E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x73F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case BS_REPAIRWEAPON:
  case MT_M_MACHINE:
    if (target != nullptr) {
      target->LaunchEffect(0x65, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case IQ_POWERFUL_FAITH:
    if (source != nullptr) {
      source->SetFrameSequence(96, 1, 1, 96, 1, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7C3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case IQ_FIRM_FAITH:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 1, 0x60, 1, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7CF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case IQ_SINCERE_FAITH:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 1, 0x60, 1, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7AC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7AD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case IQ_FIRST_FAITH_POWER:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7B9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x7B7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(1000);
      }
    }
    break;
  case IQ_JUDGE:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 5, 5, 0x60, 5, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7BC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x7BA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(1000);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x7BD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(1000);
      }
    }
    break;
  case IQ_THIRD_EXOR_FLAME:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 2, 2, 0x60, 2, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7C0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x7BE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(1000);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x7C1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(1000);
      }
    }
    break;
  case IQ_OLEUM_SANCTUM:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7C5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case IQ_EXPOSION_BLASTER:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7CB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x7CC, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->SetSequenceInterval(5);
      source->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, 1100, 0);
    }
    break;
  case IQ_MASSIVE_F_BLASTER:
    if (source != nullptr) {
      g_effectMgr->Spawn(0, 0x82E, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
      source->SetSequenceInterval(5);
      source->SetFrameSequence(0x60, 1, 2, 0x60, 2, 1, 1100, 0);
    }
    break;
  case IQ_THIRD_FLAME_BOMB:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x825, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x826, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case IQ_THIRD_CONSECRATION:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x831, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8E7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case IQ_THIRD_PUNISH:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x840, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x841, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_MEDIALE_VOTUM:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x710, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_BENEDICTUM:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x75F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_RELIGIO:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x744, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_COMPETENTIA:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x739, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (!g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(target->render.accountId, 0x73A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case CD_ARGUTUS_VITA:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x713, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_PRESENS_ACIES:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x717, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_ARGUTUS_TELUM:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x715, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_FRAMEN:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x789, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x78B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case AG_ROCK_DOWN:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x6D1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x6D2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case AG_STORM_CANNON:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        0, 0x6DB, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
      );
    }
    break;
  case AG_FROZEN_SLASH:
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
  case AG_CRIMSON_ARROW:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        source->render.accountId, 0x6DC, source->render.position.x, source->render.position.y, source->render.position.z,
        target->render.accountId, -1, 0, 0
      );
    }
    break;
  case AG_CLIMAX:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x709, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MT_SUMMON_ABR_MOTHER_NET:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x76F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x770, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MT_SUMMON_ABR_BATTLE_WARIOR:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x78E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MT_SUMMON_ABR_DUAL_CANNON:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x77B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MT_SUMMON_ABR_INFINITY:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x772, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MT_D_MACHINE: {
    g_effectMgr->Spawn(targetAid, 0x71A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(targetAid, 0x71B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    CEffect* effect = g_effectMgr->Spawn(targetAid, 0x71C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->SetDelay(500);
    }
    effect = g_effectMgr->Spawn(targetAid, 0x719, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->SetDelay(1430);
    }
    break;
  }
  case MT_A_MACHINE:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x730, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MT_AXE_STOMP: {
    float scale = 1.0f;
    if (level == 1 || level == 2) {
      scale = 0.7f;
    } else if (level == 5) {
      scale = 1.2f;
    }
    if (source != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x794, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetScale(scale);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x795, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetScale(scale);
      }
      g_effectMgr->Spawn(
        source->render.accountId, source->render.previousAnimation - source->render.animation + 0x796, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0
      );
    }
    break;
  }
  case MT_RUSH_QUAKE:
    g_effectMgr->Spawn(targetAid, 0x75C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(targetAid, 0x75D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case IQ_SECOND_FLAME:
    if (target != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x823, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(500);
      }
      effect = g_effectMgr->Spawn(target->render.accountId, 0x824, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(500);
      }
    }
    break;
  case CD_REPARATIO: {
    if (source != nullptr) {
      if (!g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(source->render.accountId, 0x727, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
      g_effectMgr->Spawn(source->render.accountId, 0x728, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x729, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(500);
      }
      effect = g_effectMgr->Spawn(source->render.accountId, 0x72A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(500);
        effect->AddEvent(0, "effect\\CD_REPARATIO1.wav", 0x1F, 0);
      }
    }
    if (target != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(target->render.accountId, 0x72B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(800);
        effect->AddEvent(0, "effect\\CD_REPARATIO2.wav", 0x32, 0);
      }
    }
    break;
  }
  case CD_DILECTIO_HEAL:
    if (source != nullptr) {
      CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 0x784, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (effect) {
        effect->SetDelay(400);
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
  case SHC_SAVAGE_IMPACT:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        source->render.accountId, 0x72D, source->render.position.x, source->render.position.y, source->render.position.z,
        target->render.accountId, -1, 0, 0
      );
    }
    break;
  case CD_EFFLIGO:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x736, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x737, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case TR_MUSICAL_INTERLUDE:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x84A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case TR_NIPELHEIM_REQUIEM:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x81B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case AG_RAIN_OF_CRYSTAL:
  case AG_ASTRAL_STRIKE:
  case TR_RETROSPECTION:
    if (source != nullptr) {
      source->SetFrameSequence(0x60, 1, 3, 0x60, 3, 1, 500, 0);
    }
    break;
  case WH_WIND_SIGN:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x76B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case WH_HAWKRUSH:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x771, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case CD_PETITIO:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x777, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case SHC_ETERNAL_SLASH:
    if (source != nullptr) {
      LaunchRollingCutterEffect(source);
      g_effectMgr->Spawn(source->render.accountId, 0x778, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case SHC_POTENT_VENOM:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x78D, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case ABC_ABYSS_DAGGER:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x7AE, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->SetState(2, 1);
    }
    break;
  case WH_HAWKBOOMERANG:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(
        0, 0x7B3, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
      );
    }
    break;
  case EM_INCREASING_ACTIVITY:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7C8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_ACTIVITY_BURN:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7D3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case ABC_CHAIN_REACTION_SHOT:
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
  case EM_SPELL_ENCHANTING:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7E9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case ABC_FROM_THE_ABYSS:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x83E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->SetFrameSequence(0x60, 0, 0, 0x60, 0, 1, 0x2BC, 0);
    }
    break;
  case ABC_DEFT_STAB:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x854, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case ABC_FRENZY_SHOT:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x842, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case EM_ELEMENTAL_BUSTER_FIRE:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x85B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x85C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_ELEMENTAL_BUSTER_GROUND:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x85E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x85F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_ELEMENTAL_BUSTER_POISON:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x861, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x862, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_ELEMENTAL_BUSTER_WATER:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x864, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x865, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_ELEMENTAL_BUSTER_WIND:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x867, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x868, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case BO_ADVANCE_PROTECTION:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x871, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x872, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case BO_THE_WHOLE_PROTECTION:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x88A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case NPC_BO_HELL_HOWLING:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8BF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case BO_ACIDIFIED_ZONE_FIRE:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x89E, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case NPC_BO_THROWROCK:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8CC, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case NPC_BO_WOODEN_ATTACK:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8CA, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case NPC_BO_FAIRY_DUSTY:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8D1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case BO_ACIDIFIED_ZONE_WATER:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8A6, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case BO_ACIDIFIED_ZONE_WIND:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8AE, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case BO_ACIDIFIED_ZONE_GROUND:
    if (source != nullptr && target != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8B6, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0);
    }
    break;
  case BO_RESEARCHREPORT:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x8C1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case ABC_STRIP_SHADOW:
    if (target != nullptr) {
      if (CRagEffect* effect = target->LaunchEffect(0x1EF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&target->render.position), 0, 0, 0, 0);
      }
    }
    break;
  case BO_BIONIC_PHARMACY:
    if (source != nullptr) {
      source->LaunchEffect(0x3A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case IQ_SECOND_JUDGEMENT:
    if (target != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\IQ_SECOND_JUDGEMENT.wav", player->render.position.x - target->render.position.x, 0.0f, player->render.position.z - target->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case ABC_UNLUCKY_RUSH:
    if (target != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\ABC_UNLUCKY_RUSH.wav", player->render.position.x - target->render.position.x, 0.0f, player->render.position.z - target->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case ABC_ABYSS_SLAYER:
    if (source != nullptr) {
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\ABC_ABYSS_SLAYER.wav", player->render.position.x - source->render.position.x, 0.0f, player->render.position.z - source->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  case AL_HEAL:
  case WE_MALE:
  case NPC_CHEAL:
  case AB_CHEAL:
  case AB_HIGHNESSHEAL:
  case HLIF_HEAL:
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
      target->SendMsg(nullptr, AM_MAKE_NUMBER, level, 0xFF00FF00u, 0x72, 0, 0, 0);
    }
    break;
  case MH_SILENT_BREEZE:
    if (source != nullptr) {
      source->LaunchEffect(0x3C1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MH_MAGMA_FLOW:
    if (source != nullptr) {
      source->LaunchEffect(0x3C2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case NPC_SELFDESTRUCTION:
  case NPC_FIRESTORM:
  case NC_SELFDESTRUCTION:
  case HVAN_EXPLOSION:
    if (source != nullptr) {
      source->LaunchEffect(0xB7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_MAGNIFICAT:
  case MER_MAGNIFICAT:
    if (source != nullptr) {
      source->LaunchEffect(0x4C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case PR_KYRIE:
  case MER_KYRIE:
    if (source != nullptr) {
      source->LaunchEffect(0x70, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case SM_MAGNUM:
  case MS_MAGNUM:
    if (source != nullptr) {
      source->LaunchEffect(0x11, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case CR_AUTOGUARD:
  case LK_PARRYING:
  case MS_PARRYING:
  case ML_AUTOGUARD:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_MAKE_GUARD, 0, 0, 0, 0, 0, 0);
    }
    break;
  case CR_DEFENDER:
  case ML_DEFENDER:
    if (source != nullptr) {
      source->LaunchEffect(0xDE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case LK_BERSERK:
  case LK_FURY:
  case MS_BERSERK:
    if (source != nullptr) {
      source->SendMsg(nullptr, AM_DELETE_EFFECT, 0x170, 0, 0, 0, 0, 0);
      source->LaunchEffect(0x170, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\버서크.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case HLIF_AVOID:
    if (source != nullptr) {
      source->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      target->LaunchEffect(0x1C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HFLI_FLEET:
  case HFLI_SPEED:
    if (source != nullptr) {
      source->LaunchEffect(0x234, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HAMI_DEFENCE:
    g_soundMgr->PlayWave("effect\\h_defence.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x239, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      target->LaunchEffect(0x239, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HAMI_CASTLE:
    g_soundMgr->PlayWave("effect\\h_castling.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (source != nullptr) {
      source->LaunchEffect(0x23A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (target != nullptr) {
      target->LaunchEffect(0x23A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case HAMI_BLOODLUST:
    if (source != nullptr) {
      source->LaunchEffect(0x23B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case MH_TOXIN_OF_MANDARA:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x910, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x911, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MH_HEILIGE_PFERD:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x91B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x91C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MH_GOLDENE_TONE:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x91E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MH_THE_ONE_FIGHTER_RISES:
    if (source != nullptr) {
      source->LaunchEffect(0x922, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_effectMgr->Spawn(source->render.accountId, 0x923, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MH_TEMPERING:
    if (source != nullptr) {
      g_effectMgr->Spawn(source->render.accountId, 0x924, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_EL_FLAMETECHNIC:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case ABR_NET_REPAIR:
    if (target != nullptr) {
      target->LaunchEffect(0x139, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (source != nullptr && g_effectMgr->Find(source->render.accountId, 0x76F) == nullptr) {
        g_effectMgr->Spawn(source->render.accountId, 0x76F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case ABR_NET_SUPPORT:
    if (target != nullptr) {
      target->LaunchEffect(0xFD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (source != nullptr && g_effectMgr->Find(source->render.accountId, 0x770) == nullptr) {
        g_effectMgr->Spawn(source->render.accountId, 0x770, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;
  case EM_EL_POISON_SHIELD: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7DD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->emblemVersion);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x7DD, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_SERPENS_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case EM_EL_DEEP_POISONING:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_EL_CRYSTAL_ARMOR: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7E8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->emblemVersion);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x7E8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_DILUVIO_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case EM_EL_COLD_FORCE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_EL_FLAMEARMOR: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x7F8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->emblemVersion);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x7F8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_ARDOR_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case EM_EL_EYES_OF_STORM: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x837, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->emblemVersion);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x837, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_PROCELLA_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case EM_EL_GRACE_BREEZE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8D8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8D9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case EM_EL_STRONG_PROTECTION: {
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x83C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    CRenderObject* summoner = this->scene->GetGameActorByAidIncludeMe(target->emblemVersion);
    if (summoner != nullptr) {
      g_effectMgr->Spawn(summoner->render.accountId, 0x83C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave("effect\\EM_SUMMON_ELEMENTAL_TERREMOTUS_1.wav", player->render.position.x - summoner->render.position.x, 0.0f, player->render.position.z - summoner->render.position.z, 250, 40, 1.0f, 0);
    }
    break;
  }
  case EM_EL_EARTH_CARE:
    if (target != nullptr) {
      g_effectMgr->Spawn(target->render.accountId, 0x8DA, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(target->render.accountId, 0x8DB, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case MG_SAFETYWALL:
  case MG_FROSTDIVER:
  case MG_FIREWALL:
  case AL_RUWACH:
  case MO_BODYRELOCATION:
  case DE_INSTANT:
  case WL_SUMMONFB:
  case WL_SUMMONBL:
  case WL_SUMMONWB:
  case WL_SUMMONSTONE:
  case SC_FEINTBOMB:
    break;
  default:
    LaunchLuaSkillEffect(skillId, level, source, target);
    CSkillActionMgr::GetInstance()->OnSkillEffect(skillId, level, source, target);
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
    UIWindow* window = g_windowMgr.LookupWindow(WID_VIEWCAMERAINFOWND);
    if (window == nullptr) {
      window = g_windowMgr.EnsureWindow(WID_VIEWCAMERAINFOWND);
    }
    g_session.showCameraInfo = g_session.showCameraInfo == 0;
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_session.showCameraInfo == 1 ? "Camera Info On" : "Camera Info Off"), 0x64FFFF, 0, 0, 0);
    window->Show(g_session.showCameraInfo);
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
  /* 0018 */ std::string padding_0018;
  /* 0030 */ int type;
  /* 0034 */ int memberCount;
  /* 0038 */ int memberLimit;
  /* 003C */ int id;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ChatRoomMemberUpdateInfo) == 0x40);
static_assert(offsetof(ChatRoomMemberUpdateInfo, padding_0018) == 0x18);
static_assert(offsetof(ChatRoomMemberUpdateInfo, type) == 0x30);
#endif

// OFFSET: 0x00D08910
// FUNCTION: fn_21595
void CGameMode::ProcessIncomingPackets() {
  memset(g_gameModePacketBuffer, 0, sizeof(g_gameModePacketBuffer));
  if (!g_replayAnimationScaling) {
    const std::uint32_t elapsedHours = (timeGetTime() - g_loginAcceptTime) / 3600000;
    if (!GetRagConnection().Update()) {
      g_modeMgr.gameMode->SendMsg(MM_SOCKETERROR, 0, 0, 0, 0);
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
    g_windowMgr.replayControlWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  } else if (replayState == 2) {
    Replay::Instance().WriteHeader(12);
    if (g_windowMgr.replayRecordControlWnd) {
      g_windowMgr.replayRecordControlWnd->SendMsg(nullptr, UM_UPDATE_RECORD_TIME, 0, 0, 0, 0);
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
    case ID_ZC_NOTIFY_INITCHAR:
    case ID_ZC_NOTIFY_UPDATECHAR:
    case ID_ZC_NOTIFY_UPDATEPLAYER:
    case ID_ZC_NOTIFY_ACTENTRY:
    case ID_ZC_ACCEPT_QUIT:
    case ID_ZC_REFUSE_QUIT:
    case ID_ZC_ACK_WHISPER:
    case ID_ZC_OTHER_GUILD_LIST:
    case ID_PING:
    case ID_ZC_MONSTER_TALK:
    case ID_ZC_ACK_ACCOUNTNAME:
    case ID_ZC_ADD_MEMBER_TO_GROUP2:
    case ID_ZC_GAME_GUARD:
    case ID_ZC_AID:
    case ID_ZC_GPK_DYNCODE:
    case ID_ZC_GPK_AUTH:
    case ID_ZC_MACRO_ITEMPICKUP_FAIL:
    case ID_ZC_PREMIUM_CAMPAIGN_INFO:
    case ID_ZC_ACK_OPEN_BANKING:
    case ID_ZC_ACK_CLOSE_BANKING:
    case ID_ZC_SECRETSCAN_DATA:
    case ID_ZC_ACK_READ_RODEX:
    case ID_ZC_SEND_EAC_MESSAGE:
    case ID_ZC_PLAYTIME_NOTIFY_CHN_TAREN:
    case ID_ZC_WORLDSTORE_INFO:
    case ID_ZC_WORLDSTORE_ITEMLIST_NORMAL_V5:
    case ID_ZC_WORLDSTORE_ITEMLIST_EQUIP_V6:
    case ID_ZC_WORLDSTORE_CLOSE:
    case ID_ZC_WORLDSTORE_MOVEITEMINFO:
    case ID_ZC_CELLATTR_UPDATE:
    case ID_ZC_PING_LIVE:
    case 0xC20:
    case 0xC29:
    case 0xC35:
      break;
    case idle_unit2Type:
      this->ReceiveActorTypedEntry(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY:
      this->ReceiveActorStandEntryOld0(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MOVEENTRY:
      this->ReceiveActorMoveEntryOld0(g_gameModePacketBuffer);
      break;
    case spawn_unit2Type:
      this->ReceiveNpcStandEntry(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_VANISH:
      this->ReceiveActorDisappear(g_gameModePacketBuffer);
      break;
    case ID_SC_NOTIFY_BAN:
      this->ReceiveNotifyBan(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_CHAT:
      this->ReceiveActorChat(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_PLAYERCHAT:
      this->ReceiveBroadcast(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPCACK_MAPMOVE:
      this->ReceiveMapMove(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPCACK_SERVERMOVE:
      this->ReceiveZoneServerChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_WHISPER:
      this->ReceiveWhisper(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_ACK:
      this->ReceiveItemPickupAck(g_gameModePacketBuffer);
      break;
    case ID_ZC_PAR_CHANGE:
      this->ReceiveParChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_LONGPAR_CHANGE:
      this->ReceiveExpChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_CLOSE_DIALOG:
      this->ReceiveNpcDialogEnd(g_gameModePacketBuffer);
      break;
    case ID_ZC_MENU_LIST:
      this->ReceiveWindow17Entries(g_gameModePacketBuffer);
      break;
    case ID_ZC_STATUS_CHANGE_ACK:
      this->ReceiveStatusChangeAck(g_gameModePacketBuffer);
      break;
    case ID_ZC_STATUS:
      this->ReceiveStatus(g_gameModePacketBuffer);
      break;
    case ID_ZC_STATUS_CHANGE:
      this->ReceiveStatusChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_SPRITE_CHANGE:
      this->ReceiveSpriteChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_SELL_ITEMLIST:
      this->ReceiveItem1718IndexList(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_PURCHASE_RESULT:
      this->ReceiveItem1718Result(g_gameModePacketBuffer);
      break;
    case ID_ZC_SETTING_WHISPER_PC:
      this->ReceiveWhisperFilterResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_ROOM_NEWENTRY:
      this->ReceiveActorMessage32(g_gameModePacketBuffer);
      break;
    case ID_ZC_REFUSE_ENTER_ROOM:
      this->ReceiveChatRoomEntryFailure(g_gameModePacketBuffer);
      break;
    case ID_ZC_ENTER_ROOM:
      this->ReceiveChatRoomMemberList(g_gameModePacketBuffer);
      break;
    case ID_ZC_CHANGE_CHATROOM:
      this->ReceiveActorMessage34(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_EXCHANGE_ITEM:
      this->ReceiveExchangeRequestAck(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ADD_EXCHANGE_ITEM:
      this->ReceiveExchangeItemAddAck(g_gameModePacketBuffer);
      break;
    case ID_ZC_GROUP_LIST:
      this->ReceivePartyMemberList(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_REQ_JOIN_GROUP:
    case ID_ZC_PARTY_JOIN_REQ_ACK:
      this->ReceiveNamedActionResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_MEMBER_TO_GROUP:
      this->ReceivePartyMemberInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_DELETE_MEMBER_FROM_GROUP:
      this->ReceivePartyMemberLeft(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_CHAT_PARTY:
      this->ReceivePartyChat(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILLINFO_UPDATE:
    case ID_ZC_HOSKILLINFO_UPDATE:
    case ID_ZC_MER_SKILLINFO_UPDATE:
      this->ApplySkillUpdatePacket(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_SKILL:
      this->ReceiveActorSkillAction(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_SKILL_POSITION:
      this->ReceiveActorSkillMessage(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_GROUNDSKILL:
      this->ReceiveNotifyGroundSkill(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILL_ENTRY:
      this->Zc_Skill_Entry(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_PURCHASE_ITEMLIST_FROMMC:
    case ID_ZC_PC_PURCHASE_ITEMLIST_FROMMC2:
      this->ReceiveItem1748List(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_PURCHASE_RESULT_FROMMC:
      this->ReceiveEquipItemNotice(g_gameModePacketBuffer);
      break;
    case openvendingType:
      this->ReceiveOpenedVendingItemList(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACTION_FAILURE:
      this->ReceiveEquipAck(g_gameModePacketBuffer);
      break;
    case ID_ZC_COUPLESTATUS:
      this->ReceiveStatusBonus(g_gameModePacketBuffer);
      break;
    case ID_ZC_MYGUILD_BASIC_INFO:
      this->ReceiveGuildRelationList(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILD_INFO:
      this->ReceiveGuildInfo2(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBERMGR_INFO:
      this->ReceiveGuildMemberMemoList(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_LEAVE_GUILD:
      this->ReceiveGuildMemberLeft(g_gameModePacketBuffer);
      break;
    case ID_ZC_POSITION_INFO:
      this->ReceiveGuildPositionInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_BAN_LIST:
      this->ReceiveGuildBanList(g_gameModePacketBuffer);
      break;
    case ID_ZC_POSITION_ID_NAME_INFO:
      this->ReceiveGuildPositionNames(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILD_NOTICE:
      this->ReceiveGuildNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_CHANGE_GUILD_POSITIONINFO:
      this->ReceiveGuildPositionUpdate(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ITEMCOMPOSITION:
      this->ReceiveCardCompositionResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILD_CHAT:
      this->ReceiveGuildChat(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBER_ADD:
      this->ReceiveGuildMemberAdd(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_RELATED_GUILD:
      this->ReceiveAddRelatedGuild(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ITEMREFINING:
      this->ReceiveRefineResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_REQMAKINGITEM:
      this->ReceiveItemEffectResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_REQNAMEALL:
      this->ReceiveActorNameAll(reinterpret_cast<const ActorNameAllPacket *>(g_gameModePacketBuffer));
      break;
    case sc_notickType:
      this->ReceiveStatusChangeNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_RANKING:
      this->ReceiveEffectNumber(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_EFFECT:
      this->ReceivePartyMemberEffect(g_gameModePacketBuffer);
      break;
    case ID_ZC_START_CAPTURE:
      this->Zc_Start_Capture(g_gameModePacketBuffer);
      break;
    case ID_ZC_TRYCAPTURE_MONSTER:
      this->Zc_Trycapture_Monster(g_gameModePacketBuffer);
      break;
    case ID_ZC_FEED_PET:
    case ID_ZC_FEED_MER:
      this->ReceiveFeedResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_CHANGESTATE_PET:
      this->ReceivePetChangeState(g_gameModePacketBuffer);
      break;
    case ID_ZC_PET_ACT:
      this->ReceivePetAct(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILD_INFO2:
      this->ReceiveGuildInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_DISPEL:
      this->ReceiveDispel(g_gameModePacketBuffer);
      break;
    case ID_ZC_REPLY_REMAINTIME:
      this->ReceiveBillingInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_ITEM_TO_STORE2:
      this->ReceiveItem1738Add(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_ITEM_TO_CART2:
      this->ReceiveItem1740Add(g_gameModePacketBuffer);
      break;
    case graffiti_entryType:
      this->Zc_Skill_Entry2(g_gameModePacketBuffer);
      break;
    case ID_ZC_DEVOTIONLIST:
      this->ReceiveDevotionList(g_gameModePacketBuffer);
      break;
    case ID_ZC_BLADESTOP:
      this->ReceiveActorPairFreeze(g_gameModePacketBuffer);
      break;
    case ID_ZC_COMBODELAY:
    case 0xC36:
      this->ReceiveActorSequenceDeadline(g_gameModePacketBuffer);
      break;
    case ID_ZC_SOUND:
      this->ReceiveSound(g_gameModePacketBuffer);
      break;
    case ID_ZC_SPRITE_CHANGE2:
      this->ReceiveSpriteChange2Mirrored(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_STANDENTRY2:
      this->ReceiveActorStateEntryOld(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY2:
      this->ReceiveActorStandEntryOld(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MOVEENTRY2:
      this->ReceiveActorMoveEntryOld(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_SKILL2:
      this->ReceiveNotifySkill(g_gameModePacketBuffer);
      break;
    case ID_ZC_SPIRITS2:
      this->Zc_Spirits2(g_gameModePacketBuffer);
      break;
    case ID_ZC_START_COUPLE:
      this->Zc_Start_Couple(g_gameModePacketBuffer);
      break;
    case ID_ZC_COUPLENAME:
      this->ReceiveCoupleName(g_gameModePacketBuffer);
      break;
    case ID_ZC_CONGRATULATION:
      this->ReceiveActorEffect347(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_EFFECT2:
      this->ReceiveNotifyEffect(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_EXCHANGE_ITEM2:
      this->ReceiveExchangeRequestAck2(g_gameModePacketBuffer);
      break;
    case ID_ZC_START_BABY:
      this->Zc_Start_Baby(g_gameModePacketBuffer);
      break;
    case ID_ZC_FRIENDS_LIST:
      this->ReceiveFriendList(g_gameModePacketBuffer);
      break;
    case ID_ZC_STARSKILL:
      this->ReceiveSpecialBroadcast(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_STATUS_GM:
      this->ReceiveStatDump(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILLMSG:
      this->ReceiveStatusNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_WEAPONREFINE:
      this->ReceiveItemObtainNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_STANDENTRY3:
      this->ReceiveActorStandEntry(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY3:
      this->ReceiveActorStandEntryOld2(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MOVEENTRY3:
      this->ReceiveActorMoveEntryOld2(g_gameModePacketBuffer);
      break;
    case ID_ZC_PROPERTY_HOMUN:
      this->ReceiveHomunculusInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_CHANGESTATE_MER:
      this->ReceiveHomunculusChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_HUNTINGLIST:
      this->ReceiveHuntingList(g_gameModePacketBuffer);
      break;
    case ID_ZC_PCBANG_EFFECT:
      this->ReceiveBroadcastBalloonNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_EFFECT3:
      this->ReceiveActorStatusValuePacket(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_CASH_POINT_ITEMLIST:
      this->ReceivePendingItem1718List(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_CASH_POINT_UPDATE:
      this->ReceivePendingResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPC_SHOWEFST_UPDATE:
      this->ReceiveActorStatusEffects(g_gameModePacketBuffer);
      break;
    case ID_ZC_MSG:
      this->Zc_Msg(g_gameModePacketBuffer);
      break;
    case ID_ZC_BOSS_INFO:
      this->ReceiveBossMonsterInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_READ_BOOK:
      this->ReceiveBookOpen(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPMENT_ITEMLIST2:
      this->ReceiveEquipItemList34(g_gameModePacketBuffer);
      break;
    case ID_ZC_STORE_EQUIPMENT_ITEMLIST2:
      this->ReceiveStorageEquipList(g_gameModePacketBuffer);
      break;
    case ID_ZC_CART_EQUIPMENT_ITEMLIST2:
      this->ReceiveItem1740BasicList(g_gameModePacketBuffer);
      break;
    case ID_ZC_CASH_TIME_COUNTER:
      this->ReceiveRentalItemTime(g_gameModePacketBuffer);
      break;
    case ID_ZC_CASH_ITEM_DELETE:
      this->ReceiveItemDeletionNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_ACK2:
      this->ReceiveItemPickupAck2(g_gameModePacketBuffer);
      break;
    case ID_ZC_MER_INIT:
      this->Zc_Mer_Init(g_gameModePacketBuffer);
      break;
    case ID_ZC_MER_PAR_CHANGE:
      this->Zc_Mer_Par_Change(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_PARTY:
      this->ReceivePartyItemPickupNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPITEM_DAMAGED:
      this->ReceiveEquipItemDamaged(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPC_CHAT:
      this->ReceiveActorColorChat(g_gameModePacketBuffer);
      break;
    case ID_ZC_FORMATSTRING_MSG:
      this->ReceiveFormatStringMessage(g_gameModePacketBuffer);
      break;
    case ID_ZC_PARTY_CONFIG:
      this->ReceiveToggleNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMORIALDUNGEON_SUBSCRIPTION_INFO:
      this->ReceiveMemorialDungeonSubscriptionInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMORIALDUNGEON_SUBSCRIPTION_NOTIFY:
      this->ReceiveMemorialDungeonSubscriptionNotify(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMORIALDUNGEON_INFO:
      this->ReceiveMemorialDungeonInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMORIALDUNGEON_NOTIFY:
      this->ReceiveMemorialDungeonNotify(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPMENT_ITEMLIST3:
      this->ReceiveEquipItemList38(g_gameModePacketBuffer);
      break;
    case ID_ZC_STORE_EQUIPMENT_ITEMLIST3:
      this->ReceiveStorageEquipList2(g_gameModePacketBuffer);
      break;
    case ID_ZC_CART_EQUIPMENT_ITEMLIST3:
      this->ReceiveItem1740BasicListBind(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_ACK3:
      this->ReceiveItemPickupAck3(g_gameModePacketBuffer);
      break;
    case ID_ZC_ISVR_DISCONNECT:
      this->ReceiveMemorialDungeonError(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPWIN_MICROSCOPE:
    case ID_ZC_EQUIPWIN_MICROSCOPE2:
      this->ReceiveEquipWindowMicroscope2(g_gameModePacketBuffer);
      break;
    case ID_ZC_CONFIG:
      this->ReceiveFeedOption(g_gameModePacketBuffer);
      break;
    case ID_ZC_CONFIG_NOTIFY:
      this->ReceiveEquipmentViewState(g_gameModePacketBuffer);
      break;
    case ID_ZC_BATTLEFIELD_CHAT:
      this->ReceiveActorBroadcastText(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACCEPT_ENTER2:
      this->ReceiveAcceptEnter(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MOVEENTRY4:
    case ID_ZC_NOTIFY_MOVEENTRY7:
    case ID_ZC_NOTIFY_MOVEENTRY8:
      this->ReceiveActorMoveEntryNamed(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY4:
      this->ReceiveActorStandEntryOld3(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_STANDENTRY4:
      this->ReceiveActorStandEntryFont(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILL_POSTDELAY:
      this->ApplyCooldownStart(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILL_POSTDELAY_LIST:
      this->LoadCooldownRecords(g_gameModePacketBuffer);
      break;
    case status_change2Type:
      this->ReceiveStatusChangeExtended(g_gameModePacketBuffer);
      break;
    case ID_ZC_MILLENNIUMSHIELD:
      this->ReceiveActorEffect228(g_gameModePacketBuffer);
      break;
    case ID_ZC_QUEST_NOTIFY_EFFECT:
      this->ReceiveMinimapMarker(g_gameModePacketBuffer);
      break;
    case ID_ZC_REQ_GROUPINFO_CHANGE_V2:
      this->ReceiveServerRateNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_HO_PAR_CHANGE:
      this->ReceiveHomunculusProperty(g_gameModePacketBuffer);
      break;
    case ID_ZC_MSG_VALUE:
      this->ReceiveFormattedMessage(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEMLISTWIN_OPEN:
      this->ReceiveTradeItemFilter(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_EXP:
      this->Zc_Notify_Exp(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY5:
    case ID_ZC_NOTIFY_NEWENTRY6:
      this->ReceiveActorStandEntryNamedOld(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_STANDENTRY5:
    case ID_ZC_NOTIFY_STANDENTRY7:
      this->ReceiveActorStateEntryNamedOld(g_gameModePacketBuffer);
      break;
    case ID_ZC_DELETE_ITEM_FROM_BODY:
      this->ReceiveItemConsumed(g_gameModePacketBuffer);
      break;
    case partyleaderchangedType:
      this->ReceivePartyLeaderChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_PLAY_NPC_BGM:
      this->ReceiveNpcBgm(g_gameModePacketBuffer);
      break;
    case ID_ZC_PARTY_BOOKING_ACK_REGISTER:
      this->ReceivePartyBookingUpdateResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_PARTY_BOOKING_ACK_SEARCH:
      this->ReceivePartyBookingSearchResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_PARTY_BOOKING_ACK_DELETE:
      this->ReceivePartyBookingRegisterResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_PARTY_BOOKING_NOTIFY_INSERT:
      this->ReceivePartyBookingInsert(g_gameModePacketBuffer);
      break;
    case ID_ZC_PARTY_BOOKING_NOTIFY_UPDATE:
      this->ReceivePartyBookingUpdate(g_gameModePacketBuffer);
      break;
    case ID_ZC_PARTY_BOOKING_NOTIFY_DELETE:
      this->ReceivePartyBookingDelete(g_gameModePacketBuffer);
      break;
    case ID_ZC_OPEN_BUYING_STORE:
      this->ReceiveOpenStore(g_gameModePacketBuffer);
      break;
    case ID_ZC_FAILED_OPEN_BUYING_STORE_TO_BUYER:
      this->ReceiveBuyingStoreOpenResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_MYITEMLIST_BUYING_STORE:
      this->ReceiveItem1758List(g_gameModePacketBuffer);
      break;
    case ID_ZC_BUYING_STORE_ENTRY:
      this->ReceiveActorText(g_gameModePacketBuffer);
      break;
    case ID_ZC_DISAPPEAR_BUYING_STORE_ENTRY:
      this->ReceiveActorSignal(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ITEMLIST_BUYING_STORE:
      this->ReceiveBuyingStoreItemList(g_gameModePacketBuffer);
      break;
    case ID_ZC_FAILED_TRADE_BUYING_STORE_TO_BUYER:
      this->ReceiveBuyingStoreClose(g_gameModePacketBuffer);
      break;
    case ID_ZC_UPDATE_ITEM_FROM_BUYING_STORE:
      this->ReceiveVendingItemSoldNow(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_DELETE_BUYING_STORE:
      this->ReceiveItemSellResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_EL_INIT:
      this->ReceiveActorStatusBlock(g_gameModePacketBuffer);
      break;
    case ID_ZC_EL_PAR_CHANGE:
      this->ReceiveActorStatusUpdate(g_gameModePacketBuffer);
      break;
    case ID_ZC_BROADCAST4:
      this->ReceiveColoredNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_FAILED_TRADE_BUYING_STORE_TO_SELLER:
      this->ReceiveBuyingStoreSellResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_SEARCH_STORE_INFO_ACK:
      this->ReceiveSearchStoreItemsOld(g_gameModePacketBuffer);
      break;
    case ID_ZC_SEARCH_STORE_INFO_FAILED:
      this->ReceiveWindow181Message(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_BAN_GUILD_SSO:
      this->ReceiveGuildMemberExpelled(g_gameModePacketBuffer);
      break;
    case ID_ZC_OPEN_SEARCH_STORE_INFO:
      this->ReceiveSearchStoreOpen(g_gameModePacketBuffer);
      break;
    case ID_ZC_SE_PC_BUY_CASHITEM_RESULT:
      this->ReceiveRequestResult(g_gameModePacketBuffer);
      break;
    case ID_CC_REPLAYPACKET:
      this->ProcessUiMessage(g_gameModePacketBuffer);
      break;
    case ID_ZC_MCSTORE_NOTMOVEITEM_LIST:
      this->ReceiveItemNameListNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_SHOWSCRIPT:
      this->ReceiveActorMessage7(g_gameModePacketBuffer);
      break;
    case ID_ZC_START_COLLECTION:
      this->Zc_Start_Collection(g_gameModePacketBuffer);
      break;
    case ID_ZC_TRYCOLLECTION:
      this->Zc_Trycollection(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILL_ENTRY3:
      this->ReceiveGroundSkill8(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_SCHEDULER_CASHITEM:
      this->ReceiveTypedItemList(g_gameModePacketBuffer);
      break;
    case ID_ZC_PERSONAL_INFOMATION:
    case ID_ZC_PERSONAL_INFOMATION2:
      this->ReceiveRateInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_FAILED_GET_ITEM_FROM_ZONEDA:
      this->ReceiveZoneItemRetrievalFailure(g_gameModePacketBuffer);
      break;
    case ID_ZC_SPIRITS_ATTRIBUTE:
      this->ReceiveActorCharm(g_gameModePacketBuffer);
      break;
    case ID_ZC_FASTMOVE:
      this->ReceiveActorMessage144(g_gameModePacketBuffer);
      break;
    case ID_ZC_CLEAR_DIALOG:
      this->RefreshWindow16(g_gameModePacketBuffer);
      break;
    case ID_ZC_EFST_SET_ENTER:
      this->ReceiveStatusChangeEffect(g_gameModePacketBuffer);
      break;
    case ID_ZC_INVENTORY_ITEMLIST_EQUIP:
      this->ReceiveEquipItemListFlags(g_gameModePacketBuffer);
      break;
    case ID_ZC_MICROSCOPE:
      this->ReceiveEquipWindowMicroscope(g_gameModePacketBuffer);
      break;
    case ID_ZC_INVENTORY_TAB:
      this->ReceiveInventoryItemUpdate(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY7:
      this->ReceiveActorStandEntryLegacy(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MOVEENTRY9:
      this->ReceiveActorMoveEntryLegacy(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_STANDENTRY8:
      this->ReceiveActorStateEntryLegacy(g_gameModePacketBuffer);
      break;
    case ID_ZC_PRNPC_STATE:
      this->ReceiveNavigationRouteResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_MERGE_ITEM_OPEN:
      this->ReceiveWindow227Entries(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_MERGE_ITEM:
      this->ReceiveWindow227Result(g_gameModePacketBuffer);
      break;
    case ID_ZC_STORE_ITEMLIST_NORMAL_V2:
      this->ReceiveStorageNormalList(g_gameModePacketBuffer);
      break;
    case ID_ZC_STORE_ITEMLIST_EQUIP_V2:
      this->ReceiveStorageEquipList3(g_gameModePacketBuffer);
      break;
    case monsterhpType:
      this->ReceiveActorInt64Pair(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_RANKING:
      this->ReceiveRankingList(g_gameModePacketBuffer);
      break;
    case ID_ZC_UPDATE_RANKING_POINT:
      this->ReceiveTypedNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_SELECTCART:
      this->ReceiveWindow95Data(g_gameModePacketBuffer);
      break;
    case ID_ZC_MSG_STATE_CHANGE3:
      this->ReceiveStatusChangeEffect2(g_gameModePacketBuffer);
      break;
    case ID_ZC_EFST_SET_ENTER2:
      this->ReceiveStatusChangeEntry(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILL_POSTDELAY_LIST2:
      this->LoadCooldownSnapshot(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_CLAN_CONNECTINFO:
      this->ReceiveClanMemberCount(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_CLAN_LEAVE:
      this->ReceiveClanLeave(g_gameModePacketBuffer);
      break;
    case ID_ZC_CLANINFO:
      this->ReceiveClanInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_CLAN_CHAT:
      this->ReceiveClanChat(g_gameModePacketBuffer);
      break;
    case ID_ZC_INVENTORY_ITEMLIST_NORMAL_V5:
      this->ReceiveItemInventoryList(g_gameModePacketBuffer);
      break;
    case ID_ZC_INVENTORY_ITEMLIST_EQUIP_V5:
      this->ReceiveInventoryEquipList(g_gameModePacketBuffer);
      break;
    case ID_ZC_CART_ITEMLIST_NORMAL_V5:
      this->ReceiveItem1740List(g_gameModePacketBuffer);
      break;
    case ID_ZC_CART_ITEMLIST_EQUIP_V5:
      this->ReceiveItem1740EquipList(g_gameModePacketBuffer);
      break;
    case ID_ZC_STORE_ITEMLIST_NORMAL_V5:
      this->ReceiveStorageNormalList2(g_gameModePacketBuffer);
      break;
    case ID_ZC_STORE_ITEMLIST_EQUIP_V5:
      this->ReceiveStorageEquipList4(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPWIN_MICROSCOPE_V5:
      this->ReceiveEquipWindowMicroscope3(g_gameModePacketBuffer);
      break;
    case ID_ZC_MAPPROPERTY_R2:
      this->ReceiveMapProperty(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILL_ENTRY4:
      this->ReceiveGroundSkill32(g_gameModePacketBuffer);
      break;
    case ID_ZC_BANKING_CHECK:
      this->ReceiveBankCheck(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_BANKING_DEPOSIT:
      this->ReceiveBankDepositAck(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_BANKING_WITHDRAW:
      this->ReceiveBankWithdrawAck(g_gameModePacketBuffer);
      break;
    case ID_ZC_C_MARKERINFO:
      this->AddMiniMapEffectMarker(g_gameModePacketBuffer);
      break;
    case ID_ZC_SKILL_ENTRY5:
      this->ReceiveGroundSkillExtended(g_gameModePacketBuffer);
      break;
    case ID_ZC_MSG_COLOR:
      this->Zc_Msg_Color(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILDSTORAGE_ITEMLIST_NORMAL_V5:
      this->ReceiveItem1738NormalList(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILDSTORAGE_ITEMLIST_EQUIP_V5:
      this->ReceiveItem1738EquipList(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_GUILDSTORAGE_LOG:
      this->ReceiveGuildStorageLog(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MOVEENTRY10:
      this->ReceiveActorMoveEntry(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY10:
      this->ReceiveActorStandEntryNamed(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_STANDENTRY10:
      this->ReceiveActorStateEntryNamed(g_gameModePacketBuffer);
      break;
    case ID_ZC_WHISPER02:
      this->ReceiveWhisper2(g_gameModePacketBuffer);
      break;
    case rodexicon:
      this->ReceiveMailboxAvailability(g_gameModePacketBuffer);
      break;
    case rodexwriteresult:
      this->ReceiveRodexComposeResult(g_gameModePacketBuffer);
      break;
    case rodexnextpage:
      this->ReceiveRodexMailListAppend(g_gameModePacketBuffer);
      break;
    case rodexgetzeny:
      this->ReceiveRodexMailReturn(g_gameModePacketBuffer);
      break;
    case rodexgetitem:
      this->ReceiveRodexItemReturn(g_gameModePacketBuffer);
      break;
    case rodexdelete:
      this->ReceiveRodexMailDeleted(g_gameModePacketBuffer);
      break;
    case ID_ZC_ALL_QUEST_LIST3:
      this->ReceiveOngoingQuestList(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_QUEST_EX:
      this->ReceiveAddQuest(g_gameModePacketBuffer);
      break;
    case ID_ZC_UPDATE_MISSION_HUNT_EX:
      this->ReceiveQuestHuntUpdate2(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MOVEENTRY11:
      this->ReceiveActorMoveEntryBody(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_NEWENTRY11:
      this->ReceiveActorStandEntryNamedBody(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_STANDENTRY11:
      this->ReceiveActorStateEntryNamedBody(g_gameModePacketBuffer);
      break;
    case ID_ZC_DRESSROOM_OPEN:
      this->EnsureWindow259(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ADD_ITEM_RODEX:
      this->ReceiveItemReward(g_gameModePacketBuffer);
      break;
    case rodexremoveitem:
      this->ReceiveStackedItemReturn(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_ITEM_TO_STORE3:
      this->ReceiveItem1738AddOptions(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_ITEM_TO_CART3:
      this->ReceiveItem1740AddOptions(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_ACK_V6:
      this->ReceiveItemPickupAck7(g_gameModePacketBuffer);
      break;
    case ID_ZC_INVENTORY_ITEMLIST_EQUIP_V6:
      this->ReceiveInventoryEquipList2(g_gameModePacketBuffer);
      break;
    case ID_ZC_CART_ITEMLIST_EQUIP_V6:
      this->ReceiveItem1740EquipListOptions(g_gameModePacketBuffer);
      break;
    case ID_ZC_STORE_ITEMLIST_EQUIP_V6:
      this->ReceiveStorageEquipList5(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILDSTORAGE_ITEMLIST_EQUIP_V6:
      this->ReceiveItem1738EquipListOptions(g_gameModePacketBuffer);
      break;
    case rodexopenwrite:
      this->ReceiveRodexOpen(g_gameModePacketBuffer);
      break;
    case ID_ZC_GOLDPCCAFE_POINT:
      this->ReceiveMileageInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_DYNAMICNPC_CREATE_RESULT:
      this->ReceiveDynamicNpcNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_OPEN_ROULETTE:
      this->ReceiveRouletteOpen(g_gameModePacketBuffer);
      break;
    case rouletteinfoackType:
      this->ReceiveRouletteInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_CLOSE_ROULETTE:
      this->ReceiveRouletteClose(g_gameModePacketBuffer);
      break;
    case roulettgenerateackType:
      this->ReceiveRouletteResult(g_gameModePacketBuffer);
      break;
    case roulettercvitemackType:
      this->ReceiveRouletteSetResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPWIN_MICROSCOPE_V6:
      this->ReceiveEquipWindowMicroscope4(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_REQNAMEALL2:
      this->ReceiveActorNameCard(g_gameModePacketBuffer);
      break;
    case ID_ZC_RESULT_PACKAGE_ITEM_TEST:
      this->ReceivePackageItemTest(g_gameModePacketBuffer);
      break;
    case ID_ZC_UPDATE_ROULETTE_COIN:
      this->ReceiveRouletteBonusPoints(g_gameModePacketBuffer);
      break;
    case ID_ZC_HP_INFO_TINY:
      this->ReceiveActorInt8(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_ACK_V7:
      this->ReceiveItemPickupAck8(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPMENT_EFFECT:
      this->ReceiveStatusChangeList(g_gameModePacketBuffer);
      break;
    case ID_ZC_UPDATE_CARDSLOT:
      this->ReceiveEquipCardChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_MEMBER_TO_GROUP3:
      this->ReceivePartyMemberInfoWithoutCharacterId(g_gameModePacketBuffer);
      break;
    case ID_ZC_GROUP_LIST2:
      this->ReceivePartyInfo2(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_BUY_STYLINGSHOP:
      this->ReceiveCharacterRenameResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_LATEST_RODEX_LIST:
      this->ReceiveRodexMailListRefresh(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_BAN_GUILD_DELNAME:
      this->ReceiveGuildExpulsion2(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_LEAVE_GUILD_DELNAME:
      this->ReceiveGuildLeave2(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILD_INFO_DELNAME:
      this->ReceiveGuildInfo3(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBER_ADD_DELNAME:
      this->ReceiveGuildMemberAdd3(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBERMGR_INFO_DELNAME:
      this->ReceiveGuildMemberList3(g_gameModePacketBuffer);
      break;
    case ID_ZC_BAN_LIST_DELNAME:
      this->ReceiveGuildBanList2(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_EXCHANGE_ITEM4:
      this->ReceiveItemSlotAddOptions(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ADD_SWAP_EQUIPITEM:
      this->ReceiveEquipItemResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_REMOVE_SWQP_EQUIPITEM:
      this->ReceiveTakeOffEquipItemResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_SEND_SWAP_EQUIPITEM_INFO:
      this->ReceiveSwapEquipmentList(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_DO_SWAP_EQUIPITEM:
      this->ReceiveCharViewResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBERMGR_INFO2:
      this->ReceiveGuildMemberList(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBER_ADD2:
      this->ReceiveGuildMemberAdd4(g_gameModePacketBuffer);
      break;
    case ID_ZC_MOVE_ITEM_FAILED:
      this->ClearItemLists(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_DEATH_TO_GROUPM:
      this->ReceivePartyDead(g_gameModePacketBuffer);
      break;
    case ID_ZC_SYNC_ATTACKED_POS:
      this->ReceivePlayerRouteClear(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_MEMBERINFO_TO_GROUPM:
      this->ReceivePartyJobLevel(g_gameModePacketBuffer);
      break;
    case ID_ZC_WARPLIST2:
      this->ReceiveChatEntries(g_gameModePacketBuffer);
      break;
    case ID_ZC_SHOW_WARPLIST:
      this->ReceiveWindow308TextEntries(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ALL_RODEX_LIST:
      this->ReceiveRodexMailList(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPCACK_SERVERMOVE_DOMAIN:
      this->ReceiveChangeZoneServerWithHost(g_gameModePacketBuffer);
      break;
    case ID_ZC_CLEAR_SWAP_EQUIPITEM_LIST:
      this->ResetItemLists(g_gameModePacketBuffer);
      break;
    case ID_ZC_LONGLONGPAR_CHANGE:
      this->ReceiveLongParChange(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_EXP2:
      this->ReceiveExpGainNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_FALL_ENTRY5:
      this->ReceiveItemDropEntry(g_gameModePacketBuffer);
      break;
    case ID_ZC_RECOVER_PENALTY_OVERWEIGHT:
      this->ReceiveOverweightThreshold(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_REQNAMEALL_NPC:
      this->ReceiveActorNameTitle(reinterpret_cast<const ActorNameTitlePacket *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_SETTING_WHISPER_PC2:
      this->ReceiveNamedResultNotice(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_MEMBER_TO_GROUP4:
      this->ReceivePartyMemberInfoWithCharacterId(g_gameModePacketBuffer);
      break;
    case ID_ZC_GROUP_LIST3:
      this->ReceivePartyRoster(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_RANKING2:
      this->ReceiveShowRankingSchedule(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_REQNAME_BYGID2:
      this->ReceivePlayerNameUpdate(reinterpret_cast<const PlayerNameUpdatePacket *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_GUILD_POSITIONNAME:
      this->ReceiveActorClanName(g_gameModePacketBuffer);
      break;
    case ID_ZC_UPDATE_MISSION_HUNT_EX2:
      this->ReceiveQuestHuntUpdate(g_gameModePacketBuffer);
      break;
    case ID_ZC_ALL_QUEST_LIST4:
      this->ReceiveOngoingQuestList2(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPWIN_MICROSCOPE_V7:
      this->ReceiveEquipWindowMicroscope5(g_gameModePacketBuffer);
      break;
    case ID_ZC_SPLIT_SEND_ITEMLIST_SET:
      this->ReceiveInventoryStart(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_QUEST_EX2:
      this->ReceiveAddQuest64(g_gameModePacketBuffer);
      break;
    case ID_ZC_CLEAR_EFFECT:
      this->ReceiveEffectRemove(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_OPEN_MSGBOX_EXTEND_BODYITEM_SIZE:
      this->ReceiveExtraInventoryResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_EXTEND_BODYITEM_SIZE:
      this->NotifyInventoryExpansionResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_EXTEND_BODYITEM_SIZE:
      this->ReceiveExtraInventorySlots(g_gameModePacketBuffer);
      break;
    case ID_ZC_USESKILL_ACK3:
      this->ReceiveActorSkillResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_ACTORINIT:
      this->ReceiveDeferredReset(g_gameModePacketBuffer);
      break;
    case ID_ZC_SHORTCUT_KEY_LIST_V4:
      this->ReceiveShortcutPagePacket(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILD_AGIT_INFO:
      this->ReceiveGuildAgitList(g_gameModePacketBuffer);
      break;
    case ID_ZC_PROPERTY_HOMUN_NO_ACCESSORY:
      this->ReceiveHomunculusInfo2(g_gameModePacketBuffer);
      break;
    case ID_ZC_EQUIPWIN_MICROSCOPE_V8:
      this->ReceiveEquipWindowMicroscope6(g_gameModePacketBuffer);
      break;
    case ID_ZC_SPLIT_SEND_ITEMLIST_EQUIPV8:
      this->ReceiveEquipItemList(g_gameModePacketBuffer);
      break;
    case ID_ZC_INVENTORY_ITEMLIST_EQUIP_V8:
      this->ReceiveInventoryEquipList3(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_PURCHASE_ITEMLIST_FROMMC4:
      this->ReceiveItem1748List2(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_GUILDSTORAGE_LOG2:
      this->ReceiveGuildStorageLogItems(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_ADD_ITEM_RODEX2:
      this->ReceiveItemReward2(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_PURCHASE_MYITEMLIST3:
      this->ReceiveItem1758StoreList(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_ACK_V8:
      this->ReceiveItemPickupAck9(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_EXCHANGE_ITEM5:
      this->ReceiveItemSlotAddGrade(g_gameModePacketBuffer);
      break;
    case ID_ZC_CHANGE_ITEM_OPTION3:
      this->RefreshInventoryOrEquipItem2(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_ITEM_TO_STORE4:
      this->ReceiveItem1738AddGrade(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_ITEM_TO_CART4:
      this->ReceiveItem1740AddGrade(g_gameModePacketBuffer);
      break;
    case ID_ZC_GET_ACCOUNT_LIMTIED_SALE_LIST:
      this->ReceiveItem1784List(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPC_MARKET_PURCHASE_RESULT_V2:
      this->ReceiveParaResultList(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPC_EXPANDED_BARTER_MARKET_ITEMINFO:
      this->ReceiveExpandedBarterMarketItemList(g_gameModePacketBuffer);
      break;
    case ID_ZC_GRADE_ENCHANT_MATERIAL_LIST:
      this->ReceiveGradeEnchantInfo(g_gameModePacketBuffer);
      break;
    case ID_ZC_GRADE_ENCHANT_ACK:
      this->ReceiveGradeEnchantResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_GRADE_ENCHANT_BROADCAST_RESULT:
      this->ReceiveGradeEnchantBroadcast(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEMLIST_FROM_ASSISTANT_VENDING4:
      this->ReceiveShopItemListPacket(g_gameModePacketBuffer);
      break;
    case ID_ZC_ACK_READ_RODEX2:
      this->ReceiveRodexReadMail(g_gameModePacketBuffer);
      break;
    case ID_ZC_SEARCH_STORE_INFO_ACK2:
      this->ReceiveSearchStoreItems(g_gameModePacketBuffer);
      break;
    case ID_ZC_REPAIRITEMLIST2:
      this->ReceiveWindow107List(g_gameModePacketBuffer);
      break;
    case ID_ZC_ITEM_PICKUP_PARTY2:
      this->ReceivePartyItemPickup(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_EFFECT4:
      this->ReceiveActorStatusValuePacket2(g_gameModePacketBuffer);
      break;
    case ID_ZC_ABR_INIT:
      this->ReceiveSummonHpInit(g_gameModePacketBuffer);
      break;
    case ID_ZC_ABR_PAR_CHANGE:
      this->ReceiveSummonHpUpdate(g_gameModePacketBuffer);
      break;
    case ID_ZC_SOULENERGY:
      this->ReceiveSoulEnergy(g_gameModePacketBuffer);
      break;
    case ID_ZC_PC_PURCHASE_ITEMLIST2:
      this->ReceiveItem1718BasicList(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPC_BARTER_MARKET_ITEMINFO2:
      this->ReceiveBarterMarketItemList(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPC_EXPANDED_BARTER_MARKET_ITEMINFO2:
      this->ReceiveExpandedBarterMarketItemList2(g_gameModePacketBuffer);
      break;
    case ID_ZC_NPC_MARKET_OPEN2:
      this->ReceiveItem1718List(g_gameModePacketBuffer);
      break;
    case ID_ZC_GUILD_INFO_NEW:
      this->ReceiveGuildInfo4(g_gameModePacketBuffer);
      break;
    case ID_ZC_BAN_LIST_NEW:
      this->ReceiveGuildBanList3(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBERMGR_INFO_NEW:
      this->ReceiveGuildMemberList2(g_gameModePacketBuffer);
      break;
    case ID_ZC_MEMBER_ADD_NEW:
      this->ReceiveGuildMemberAdd2(g_gameModePacketBuffer);
      break;
    case ID_ZC_PROPERTY_HOMUN_NO_ACCESSORY3:
      this->ReceiveHomunculusInfo3(g_gameModePacketBuffer);
      break;
    case ID_ZC_HO_PAR_CHANGE2:
      this->ReceiveHomunculusProperty64(g_gameModePacketBuffer);
      break;
    case ID_ZC_RESULT_DDUKDDAKBOX_TEST:
      this->ReceiveDdukddakBoxTest(g_gameModePacketBuffer);
      break;
    case ID_ZC_CHANGE_ITEM_OPTION4:
      this->ReceiveItemOptionsRefresh(g_gameModePacketBuffer);
      break;
    case ID_ZC_NOTIFY_LISTOFMODIFIED_ITEMNSKILL:
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
    case ID_ZC_ALLY_CHAT:
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
      this->ReceiveDebuffRemovalResult(g_gameModePacketBuffer);
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
    case ID_ZC_ACCEPT_ENTER: {
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
          fprintf(file, "ip:%s\n", g_zoneServerEndpoint.address);
          fprintf(file, "map:%s.gat\n", g_session.m_navigationPlayerMap);
          fprintf(file, "time:%s\n\n", _ctime32(&now));
          fclose(file);
        }
        g_pingLogTicks.clear();
        g_pingLogFirstTick = 0;
      }
      break;
    }
    case ID_ZC_NOTIFY_TIME: {
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
      if (this->serverTimeSyncCount == 0) {
        g_session.SetServerTime(serverTime);
      } else {
        g_session.SyncServerTime(serverTime);
      }
      ++this->serverTimeSyncCount;
      *reinterpret_cast<int *>(&this->requestTimers[0x02]) = 1;
      g_session.AddPingSample(timeGetTime() - *reinterpret_cast<const std::uint32_t *>(&this->requestTimers[0x06]));
      break;
    }
    case ID_ZC_NOTIFY_MOVE: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      ActorMoveRoute path;
      path.tick = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 12);
      DecodeSrcDst(g_gameModePacketBuffer + 6, &path.srcX, &path.srcY, &path.dstX, &path.dstY, &path.subX, &path.subY);
      CRenderObject *actor = this->scene->GetGameActorByAID(aid);
      g_session.SyncServerTime(path.tick);
      if (actor) {
        const int pathAddress = reinterpret_cast<int>(&path);
        actor->SendMsg(nullptr, AM_MOVE, pathAddress, 0, 0, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_NOTIFY_PLAYERMOVE: {
      ActorMoveRoute path;
      path.tick = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      DecodeSrcDst(g_gameModePacketBuffer + 6, &path.srcX, &path.srcY, &path.dstX, &path.dstY, &path.subX, &path.subY);
      g_session.SyncServerTime(path.tick);
      const int pathAddress = reinterpret_cast<int>(&path);
      this->scene->player->SendMsg(nullptr, AM_MOVE, pathAddress, 0, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_STOPMOVE: {
      const ActorStopPacket *packet = reinterpret_cast<const ActorStopPacket *>(g_gameModePacketBuffer);
      this->StopActorMove(packet->aid, packet->x, packet->y, 0);
      break;
    }
    case ID_ZC_NOTIFY_ACT: {
      const ActorActionPacket *packet = reinterpret_cast<const ActorActionPacket *>(g_gameModePacketBuffer);
      this->ReceiveActorAction(packet->sourceAid, packet->targetAid, packet->tick, packet->attackMT, packet->attackedMT, packet->damage, packet->count,
                               packet->type, packet->damage2, 0);
      break;
    }
    case ID_ZC_NOTIFY_ACT_POSITION: {
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
      act.attackMotion = packet->attackMT;
      act.damageMotion = packet->attackedMT;
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
      actor->SendMsg(nullptr, AM_ACTION, actAddress, 0, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_NPCACK_ENABLE:
      this->noMove = 0;
      break;
    case reqName: {
      const ActorNamePacket *packet = reinterpret_cast<const ActorNamePacket *>(g_gameModePacketBuffer);
      this->nameCard.SetName(packet->aid, std::string(packet->name, sizeof(packet->name)).c_str());
      if (packet->aid == g_session.petGid) {
        sprintf(g_session.petName, packet->name);
        if (UIPetInfoWnd *window = static_cast<UIPetInfoWnd *>(g_windowMgr.petInfoWnd)) {
          window->nameEdit->SetText(packet->name);
        }
      }
      break;
    }
    case ID_ZC_BROADCAST:
      this->ReceiveTaggedBroadcast(g_gameModePacketBuffer);
      break;
    case ID_ZC_CHANGE_DIRECTION: {
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
      actor->SendMsg(nullptr, AM_SET_HEAD_DIR, packet->headDirection, 0, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_ITEM_ENTRY:
    case ID_ZC_ITEM_FALL_ENTRY: {
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
        this->scene->items.push_back(item);
      }

      const int entryAddress = reinterpret_cast<int>(&entry);
      item->SendMsg(nullptr, AM_INITSTANDENTRY, entryAddress, 0, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_ITEM_DISAPPEAR: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      this->scene->RemoveGroundItem(aid);
      this->RemovePendingItemPickup(aid);
      break;
    }
    case ID_ZC_NORMAL_ITEMLIST: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(NormalItemRecord);
      const NormalItemRecord *record = reinterpret_cast<const NormalItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        std::memset(item.cards, 0, sizeof(item.cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case ID_ZC_EQUIPMENT_ITEMLIST: {
      g_session.RemoveEquipment();
      g_session.ResetEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(EquipItemRecord);
      const EquipItemRecord *record = reinterpret_cast<const EquipItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = record->location;
        item.identity[1] = record->index;
        item.identity[3] = record->wearState;
        item.quantity = 1;
        std::memcpy(item.cards, record->cards, sizeof(record->cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = record->damaged;
        item.refiningLevel = record->refine;
        if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
          g_session.petEggIndex = item.identity[1];
        }

        if (item.identity[3] == 0) {
          g_session.AddInventoryItem(item, 0);
          g_session.UpdateInventoryItem(item, 0);
        } else {
          char equipped = 1;
          g_session.EquipInventoryItem(item, &equipped);
        }
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case ID_ZC_STORE_NORMAL_ITEMLIST: {
      g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(NormalItemRecord);
      const NormalItemRecord *record = reinterpret_cast<const NormalItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        std::memset(item.cards, 0, sizeof(item.cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        g_session.AddCartItem(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case ID_ZC_STORE_EQUIPMENT_ITEMLIST: {
      g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(EquipItemRecord);
      const EquipItemRecord *record = reinterpret_cast<const EquipItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = record->location;
        item.identity[1] = record->index;
        item.identity[3] = record->wearState;
        item.quantity = 1;
        std::memcpy(item.cards, record->cards, sizeof(record->cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = record->damaged;
        item.refiningLevel = record->refine;
        g_session.AddCartItem(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case ID_ZC_USE_ITEM_ACK: {
      const ItemUseAckPacket *packet = reinterpret_cast<const ItemUseAckPacket *>(g_gameModePacketBuffer);
      if (packet->result) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, packet->index);
        if (item.identity[1]) {
          g_session.RemoveInventoryItem(packet->index, item.quantity - packet->amount);
          if (item.IsAboveItemId600()) {
            this->scene->player->SendMsg(nullptr, AM_USEITEM_EFFECT, 0, 0, 0, 0, 0, 0);
          }
        }
      }
      this->waitingUseItemAck = 0;
      break;
    }
    case ID_ZC_REQ_WEAR_EQUIP_ACK: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->equipmentRequestState[0x04]) = timeGetTime() - 2000;
      switch (packet[8]) {
      case ACK_REQ_EQUIP_FAILURE:
      case ACK_REQ_EQUIP_FAILURE_LEVEL:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CAN_NOT_EQUIP_ITEM)), 0xFF, 7, 0, 0);
        break;
      case ACK_REQ_EQUIP_SUCCESS: {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(packet + 2));
        if (item.identity[1]) {
          item.sprite = *reinterpret_cast<const std::uint16_t *>(packet + 6);
          item.identity[3] = *reinterpret_cast<const std::uint16_t *>(packet + 4);
          g_session.OnItemEquipped(item);
        }
        break;
      }
      }
      break;
    }
    case ID_ZC_REQ_TAKEOFF_EQUIP_ACK: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->equipmentRequestState[0x08]) = timeGetTime() - 2000;
      const std::uint16_t location = *reinterpret_cast<const std::uint16_t *>(packet + 4);
      if (packet[6] && location) {
        if (g_session.IsCostumeLocation(location)) {
          g_session.UnequipCostumeItem(location);
        } else {
          g_session.UnequipItem(location, 0);
        }
      }
      break;
    }
    case ID_ZC_ITEM_THROW_ACK: {
      const unsigned char *packet = g_gameModePacketBuffer;
      if (*reinterpret_cast<int *>(&this->equipmentRequestState[0x14])) {
        *reinterpret_cast<int *>(&this->equipmentRequestState[0x14]) = 0;
        g_session.ResetSlots();
      }

      const std::uint16_t index = *reinterpret_cast<const std::uint16_t *>(packet + 2);
      const std::int16_t amount = *reinterpret_cast<const std::int16_t *>(packet + 4);
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
      if (item.identity[1] && amount > 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_DROP_ITEM), item.DisplayName(0).c_str(), amount).c_str()),
                               0xFFFF00, 6, 0, 0);
      }
      g_session.RemoveInventoryItem(index, amount);
      *reinterpret_cast<int *>(this->waitingItemThrowAck) = 0;
      break;
    }
    case ID_ZC_RESTART_ACK:
      g_navigationManager.Reset(true);
      g_navigationManager.ClearData();
      g_navigationManager.displayMode = 0;
      g_clanInfo->Clear();
      if (g_gameModePacketBuffer[2] == 1) {
        g_timeScheduler->RemoveSchedulesByKey(6);
        g_timeScheduler->RemoveSchedulesByKey(5);
        g_timeScheduler->RemoveSchedulesByKey(7);
        GetRagConnection().Close();
        g_loginStartMode = 5;
        g_modeMgr.Switch(0, "login.rsw");
      }
      break;
    case ID_ZC_SAY_DIALOG: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->isOnQuest = 1;
      this->lastNaid = *reinterpret_cast<const int *>(packet + 4);
      unsigned int length = *reinterpret_cast<const std::uint16_t *>(packet + 2) - 8;
      UIWindow *window = g_windowMgr.EnsureWindow(WID_SAYDIALOGWND);
      char text[0x803];
      if (g_serviceType == 10) {
        sprintf(text, "|%02x", LanguageCharset(31));
        memcpy(text + 3, packet + 8, length);
        length += 3;
      } else {
        memcpy(text, packet + 8, length);
      }
      text[length] = 0;
      window->SendMsg(nullptr, UM_SETSAYDIALOG, reinterpret_cast<std::intptr_t>(text), *reinterpret_cast<const int *>(packet + 4), 0, 0);
      break;
    }
    case ID_ZC_WAIT_DIALOG:
      this->isOnQuest = 1;
      this->lastNaid = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      g_windowMgr.EnsureWindow(WID_SAYDIALOGWND)->SendMsg(nullptr, UM_ENABLENEXTBUTTON, 0, 0, 0, 0);
      break;
    case ID_ZC_EMOTION: {
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
      actor->SendMsg(nullptr, AM_CASH_EMOTION, 0, emotion, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_USER_COUNT: {
      char text[128];
      if (g_serviceType == 3 && !IsAdminAccount(g_session.m_aid)) {
        strcpy(text, GetUserCountText(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)));
      } else {
        sprintf(text, LookupMessage(MSI_NUMPEOPLE), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2));
      }
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
      break;
    }
    case ID_ZC_SELECT_DEALTYPE:
      this->isOnQuest = 1;
      g_windowMgr.EnsureWindow(WID_CHOOSESELLBUYWND)->SendMsg(nullptr, UM_SETNAID, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_ITEMPURCHASEWND);
      g_windowMgr.CloseWindow(WID_ITEMSELLWND);
      break;
    case ID_ZC_PC_PURCHASE_ITEMLIST: {
      g_session.ClearTradeItems();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(ShopItemRecord);
      const ShopItemRecord *records = reinterpret_cast<const ShopItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i) {
        const ShopItemRecord record = records[i];
        RawItemBasic item = {};
        _itoa(record.itemId, item.itemId, 10);
        item.price = record.price;
        item.realPrice = record.discountPrice;
        item.type = record.type;
        item.id = -1;
        item.identified = 1;
        g_session.AddShopItem(item, 0);
      }

      UIWindow *buyWindow = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND);
      buyWindow->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
      buyWindow->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      UIWindow *sellWindow = g_windowMgr.EnsureWindow(WID_ITEMPURCHASEWND);
      sellWindow->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
      sellWindow->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_PC_SELL_RESULT:
      switch (g_gameModePacketBuffer[2]) {
      case SELL_SUCCEED:
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_SECCESS)), 0xFFFF00, 0, 0, 0);
        break;
      case SELL_FAIL:
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_FAIL)), 0xFF, 0, 0, 0);
        break;
      }
      g_session.ClearTradeItems();
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_ITEMSELLWND);
      break;
    case ID_ZC_ACK_DISCONNECT_CHARACTER: {
      char text[80];
      sprintf(text, LookupMessage(g_gameModePacketBuffer[2] == 0 ? 152 : 151));
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0x32FF, 0, 0, 0);
      break;
    }
    case ID_ZC_SETTING_WHISPER_STATE:
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case WT_EXCLUDE_WHISPER:
        switch (g_gameModePacketBuffer[3]) {
        case WHISPER_SETTING_SUCCEED:
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FAILED_TO_ASK_ALL_CHAT)), 0xFFFF, 0, 0, 0);
          break;
        case WHIPSER_SETTING_FAILED:
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OPENING)), 0xFFFF, 0, 0, 0);
          break;
        }
        break;
      case WT_INCLUDE_WHISPER:
        switch (g_gameModePacketBuffer[3]) {
        case WHISPER_SETTING_SUCCEED:
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DO_YOU_WANT_TO_BACK_LOGINWND)), 0xFFFF, 0, 0, 0);
          break;
        case WHIPSER_SETTING_FAILED:
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ENST_SHOUT2)), 0xFFFF, 0, 0, 0);
          break;
        }
        break;
      }
      break;
    case ID_ZC_WHISPER_LIST: {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      const int length = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      const int count = (length - 4) / 24;
      char name[25];
      name[24] = '\0';
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(count != 0 ? 3396 : 3395)), 0xFFFF00, 0, 0, 0);
      for (int i = 0; i < count; ++i) {
        if (i * 24 + 4 > length) {
          break;
        }
        std::memcpy(name, g_gameModePacketBuffer + 4 + i * 24, 24);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(name), 0xFFFF00, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_ACK_CREATE_CHATROOM:
      this->isWaitingEnterRoom = 0;
      switch (g_gameModePacketBuffer[2]) {
      case ENTERROOM_FULL: {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ROOM_IS_MADE)), 0xFFFF00, 0, 0, 0);
        UIWindow *window = g_windowMgr.EnsureWindow(WID_CHATROOMWND);
        this->chatRoomMemberCount = 1;
        window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&this->chatRoomTitle), 0, 0, 0);
        window->SendMsg(nullptr, UM_CLEARMEMBER, 0, 0, 0, 0);
        window->SendMsg(nullptr, UM_ADDMEMBER, reinterpret_cast<std::intptr_t>(g_session.DecodePlayerName()), 0, 0, 0);
        break;
      }
      case ENTERROOM_WRONG_PASSWORD:
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_MANY_ROOM)), 0xFF, 0, 0, 0);
        break;
      case ENTERROOM_KICKED:
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SAME_ROOM_TITLE)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_DESTROY_ROOM: {
      const std::uint32_t roomId = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(this->chatRoomActorIds[roomId])) {
        actor->SendMsg(nullptr, AM_DELETE_CHAT_TITLE, 0, 0, 0, 0, 0, 0);
      }
      if (g_windowMgr.chatRoomWnd && static_cast<UIChatRoomWnd *>(g_windowMgr.chatRoomWnd)->roomStatus.roomId == static_cast<int>(roomId)) {
        g_windowMgr.CloseWindow(WID_CHATROOMWND);
        g_windowMgr.CloseWindow(WID_CHATROOMCHANGEWND);
      }
      break;
    }
    case ID_ZC_MEMBER_NEWENTRY: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 4, 24);
      name[24] = '\0';
      g_session.DecodePlayerName();
      UIChatRoomWnd *window = static_cast<UIChatRoomWnd *>(g_windowMgr.EnsureWindow(WID_CHATROOMWND));
      ChatRoomMemberUpdateInfo info{window->title, window->roomSettingsText, window->roomStatus.roomType, window->roomStatus.memberCount,
                             window->roomStatus.memberLimit, window->roomStatus.roomId};
      info.memberCount = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      window->SendMsg(nullptr, UM_ADDMEMBER, reinterpret_cast<std::intptr_t>(name), 1, 0, 0);
      char text[128];
      sprintf(text, LookupMessage(MSI_USERGETINTHEROOM), name);
      g_windowMgr.SendNotice(UIM_PUSH_INTO_CHATROOM, reinterpret_cast<std::intptr_t>(text), 0x808000, 0, 0, 0);
      break;
    }
    case ID_ZC_MEMBER_EXIT: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 4, 24);
      name[24] = '\0';
      if (strcmp(g_session.DecodePlayerName(), name) == 0) {
        g_windowMgr.CloseWindow(WID_CHATROOMWND);
        g_windowMgr.CloseWindow(WID_CHATROOMCHANGEWND);
        break;
      }

      UIChatRoomWnd *window = static_cast<UIChatRoomWnd *>(g_windowMgr.EnsureWindow(WID_CHATROOMWND));
      ChatRoomMemberUpdateInfo info{window->title, window->roomSettingsText, window->roomStatus.roomType, window->roomStatus.memberCount,
                             window->roomStatus.memberLimit, window->roomStatus.roomId};
      info.memberCount = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      window->SendMsg(nullptr, UM_MEMBER_EXIT, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
      char text[128];
      switch (g_gameModePacketBuffer[0x1C]) {
      case ROOMEXITTYPE_EXIT:
        sprintf(text, LookupMessage(MSI_USERGETOUTOFTHEROOM), name);
        g_windowMgr.SendNotice(UIM_PUSH_INTO_CHATROOM, reinterpret_cast<std::intptr_t>(text), 0x80, 0, 0, 0);
        break;
      case ROOMEXITTYPE_EXPEL:
        sprintf(text, LookupMessage(MSI_USERGETOUTOFTHEROOMBYFORCE), name);
        g_windowMgr.SendNotice(UIM_PUSH_INTO_CHATROOM, reinterpret_cast<std::intptr_t>(text), 0x80, 0, 0, 0);
        break;
      }
      break;
    }
    case ID_ZC_ROLE_CHANGE: {
      UIWindow *window = g_windowMgr.EnsureWindow(WID_CHATROOMWND);
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 6, 24);
      name[24] = '\0';
      window->SendMsg(nullptr, UM_ROLE_CHANGE, reinterpret_cast<std::intptr_t>(name), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0);
      break;
    }
    case ID_ZC_REQ_EXCHANGE_ITEM: {
      if (g_serviceOptions.Get(136)) {
        g_modeMgr.gameMode->SendMsg(MM_ACK_EXCHANGE_ITEM, 4, 0, 0, 0);
        break;
      }

      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 2, 24);
      name[24] = '\0';
      g_windowMgr.EnsureWindow(WID_EXCHANGEACCEPTWND)->SendMsg(nullptr, UM_SET_EXCHANGE_CHAR_NAME, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
      this->exchangeName.assign(name, strlen(name));
      break;
    }
    case ID_ZC_CONCLUDE_EXCHANGE_ITEM:
      if (UIWindow *window = g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
        switch (g_gameModePacketBuffer[2]) {
        case EXI_CONCLUDE_ME:
          window->SendMsg(nullptr, UM_MYDRAGCLOSED, 0, 0, 0, 0);
          break;
        case EXI_CONCLUDE_OTHER:
          window->SendMsg(nullptr, UM_OTHERDRAGCLOSED, 0, 0, 0, 0);
          break;
        }
      }
      break;
    case ID_ZC_CANCEL_EXCHANGE_ITEM:
      *reinterpret_cast<int *>(&this->equipmentRequestState[0x00]) = 0;
      this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_EXCHANGEUI);
      g_windowMgr.CloseWindow(WID_EXCHANGEACCEPTWND);
      *reinterpret_cast<int *>(&this->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_IS_CANCELED)), 0xFF, 0, 0, 0);
      break;
    case ID_ZC_EXEC_EXCHANGE_ITEM:
      this->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_EXCHANGEUI);
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case EXITEM_EXCHANGE_SUCCEED:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_SUCCESS)), 0xFFFF00, 0, 0, 0);
        break;
      case EXITEM_EXCHANGE_FAILED:
        *reinterpret_cast<int *>(&this->equipmentRequestState[0x14]) = 0;
        g_session.ResetSlots();
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DEAL_FAILED)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_EXCHANGEITEM_UNDO:
      *reinterpret_cast<int *>(&this->equipmentRequestState[0x14]) = 0;
      g_session.ResetSlots();
      break;
    case ID_ZC_NOTIFY_STOREITEM_COUNTINFO:
      g_windowMgr.EnsureWindow(WID_ITEMSTOREWND)->SendMsg(nullptr, UM_SETCOUNTINFO, *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2),
                                          *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4), 0, 0);
      break;
    case ID_ZC_DELETE_ITEM_FROM_STORE: {
      ITEM_INFO item;
      item.identity[1] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      item.quantity = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      g_session.RemoveCartItem(item);
      this->inventoryChanged = 1;
      break;
    }
    case ID_ZC_CLOSE_STORE:
      g_windowMgr.CloseWindow(WID_ITEMSTOREWND);
      g_session.ClearCartItems();
      break;
    case ID_ZC_ACK_MAKE_GROUP:
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case GROUP_MAKE_SUCCEED:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PARTY_MAKE_SUCCEED)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.EnsureWindow(WID_MESSENGERGROUPWND);
        break;
      case GROUP_MAKE_FAILED_SAMENAME:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SAME_PARTY_NAME)), 0xFF, 0, 0, 0);
        break;
      case GROUP_MAKE_FAILED_ALEADY_GROUP_MEMBER:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_YOU_ARE_ALREADY_IN_PARTY)), 0xFF, 0, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOPARTY)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_REQ_JOIN_GROUP:
    case ID_ZC_PARTY_JOIN_REQ: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 6), 24);
      name[24] = 0;
      if (g_forbiddenWords.Contains(name)) {
        std::string masked("");
        g_forbiddenWords.Mask(name, &masked);
      }
      g_session.partyName.assign(name, strlen(name));
      g_windowMgr.EnsureWindow(WID_JOINPARTYACCEPTWND)->SendMsg(nullptr, UM_SET_JOIN_PARTY_MSG, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), reinterpret_cast<std::intptr_t>(name), 0, 0);
      break;
    }
    case ID_ZC_GROUPINFO_CHANGE: {
      char text[128];
      g_session.partyExpShare = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      if (g_windowMgr.partySettingWnd) {
        g_windowMgr.partySettingWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      }
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      if (g_session.partyExpShare == 0 || g_session.partyExpShare == 1) {
        sprintf(text, "%s - %s : %s", LookupMessage(MSI_PARTYSETTING), LookupMessage(MSI_HOWEXPDIV), LookupMessage(g_session.partyExpShare == 0 ? 287 : 288));
      } else if (g_session.partyExpShare == 2) {
        StringFormat(text, 128, static_cast<std::size_t>(-1), LookupMessage(MSI_PARTYSETTING_CHANGE_IMPOSSIBLE), LookupMessage(MSI_HOWEXPDIV), LookupMessage(MSI_EXPDIV2));
      } else {
        break;
      }
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
      break;
    }
    case ID_ZC_NOTIFY_HP_TO_GROUPM: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (g_session.IsMercenaryAid(aid)) {
        break;
      }
      const int hp = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6);
      const int maxHp = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        actor->SendMsg(nullptr, AM_UPDATEGAGE, hp, maxHp, 0, 0, 0, 0);
      }
      if (g_windowMgr.messengerGroupWnd) {
        g_windowMgr.messengerGroupWnd->SendMsg(nullptr, UM_COMMAND, 34, aid, hp, maxHp);
      }
      g_windowMgr.NotifyItemWindows(aid, hp, maxHp);
      break;
    }
    case ID_ZC_NOTIFY_POSITION_TO_GROUPM: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->SendMsg(MM_ADDPARTYPOS, *reinterpret_cast<const int *>(packet + 2), *reinterpret_cast<const std::int16_t *>(packet + 6),
                    *reinterpret_cast<const std::int16_t *>(packet + 8), 0);
      break;
    }
    case ID_ZC_MVP_GETTING_ITEM: {
      char itemId[25];
      _itoa(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), itemId, 10);
      itemId[24] = 0;
      ITEM_INFO item;
      item.itemId.assign(itemId, strlen(itemId));
      item.refiningLevel = 0;
      item.identified = 0;
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      const char *suffix = LookupMessage(MSI_YOU_RECEIVE_MVP_ITEM2);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", LookupMessage(MSI_YOU_RECEIVE_MVP_ITEM), item.DisplayName(0).c_str(), suffix).c_str()),
                             0xFFFF00, 0, 0, 0);
      break;
    }
    case ID_ZC_MVP_GETTING_SPECIAL_EXP: {
      char text[256];
      memset(text, 0, sizeof(text));
      StringFormat(text, 256, static_cast<std::size_t>(-1), "%s%s", LookupMessage(MSI_YOU_RECEIVE_MVP_EXP), LookupMessage(MSI_YOU_RECEIVE_MVP_EXP2));
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 0, 0, 0);
      break;
    }
    case ID_ZC_MVP: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_MVP, 0, 0, 0, 0, 0, 0);
      } else if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        actor->SendMsg(nullptr, AM_MVP, 0, 0, 0, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_THROW_MVPITEM:
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_YOU_THROW_MVPITEM)), 0xFF, 0, 0, 0);
      break;
    case ID_ZC_SKILLINFO_LIST:
    case ID_ZC_HOSKILLINFO_LIST:
    case ID_ZC_MER_SKILLINFO_LIST: {
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
        entry.learnedLevel = record->level;
        skills.push_back(entry);
      }
      this->ReceiveSkillList(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer), skills);
      break;
    }
    case ID_ZC_ACK_TOUSESKILL:
      this->ReceiveSkillFailure(g_gameModePacketBuffer);
      break;
    case ID_ZC_ADD_SKILL: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->ReceiveLearnedSkill(*reinterpret_cast<const std::uint16_t *>(packet + 2), *reinterpret_cast<const std::uint16_t *>(packet + 8),
                                *reinterpret_cast<const int *>(packet + 4), packet[0x26], *reinterpret_cast<const std::uint16_t *>(packet + 0x0A),
                                *reinterpret_cast<const std::uint16_t *>(packet + 0x0C), *reinterpret_cast<const std::uint16_t *>(packet + 8));
      break;
    }
    case ID_ZC_STATE_CHANGE: {
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
        *reinterpret_cast<int *>(&g_session.statusLevelEffects[0x00]) = state[0];
        *reinterpret_cast<int *>(&g_session.statusLevelEffects[0x04]) = state[2];
        g_session.m_jobFlags = state[1];
        if (g_windowMgr.equipWnd) {
          g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
        }
        if (g_session.replayPacketMode <= 0) {
          g_session.ClearMerchantShopItems();
          g_windowMgr.CloseWindow(WID_MERCHANTITEMWND);
        }
      } else {
        actor = this->scene->GetGameActorByAID(aid);
      }
      if (actor) {
        const int stateAddress = reinterpret_cast<int>(state);
        actor->SendMsg(nullptr, AM_ATTR_STATE_CHANGE, stateAddress, 0, 0, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_USE_SKILL: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->ReceiveSkillUse(*reinterpret_cast<const std::uint16_t *>(packet + 2), *reinterpret_cast<const std::int16_t *>(packet + 4),
                            *reinterpret_cast<const std::uint32_t *>(packet + 0x0A), *reinterpret_cast<const std::uint32_t *>(packet + 6), packet[0x0E]);
      break;
    }
    case ID_ZC_WARPLIST: {
      g_windowMgr.CloseWindow(WID_CHOOSEWARPWND);
      UIWindow *window = g_windowMgr.EnsureWindow(WID_CHOOSEWARPWND);
      window->SendMsg(nullptr, UM_SETSKID, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      const char *name = reinterpret_cast<const char *>(g_gameModePacketBuffer + 4);
      for (unsigned int i = 0; i < 4; ++i, name += 16) {
        if (strlen(name)) {
          window->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>(name), 0, 0, 0);
        }
      }
      window->SendMsg(nullptr, UM_SETCHOOSE, reinterpret_cast<std::intptr_t>("cancel"), 0, 0, 0);
      break;
    }
    case ID_ZC_ACK_REMEMBER_WARPPOINT:
      switch (g_gameModePacketBuffer[2]) {
      case WARPPOINT_SUCCESS:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_WARPPOINTSTORED)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_WARPPOINTSTORED)), 0xFFFF00, 0, 0, 0);
        break;
      case WARPPOINT_LOW_LEVEL:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTENOUGHSKILLLEVEL)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTENOUGHSKILLLEVEL)), 0xFF, 0, 0, 0);
        break;
      case WARPPOINT_NOT_LEARNED:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOWARPSKILL)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOWARPSKILL)), 0xFF, 0, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_STORAGE_ITEM)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_STORAGE_ITEM)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_SKILL_DISAPPEAR: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      std::vector<std::uint32_t>::iterator it = this->spriteMirrorActorIds.begin();
      while (it != this->spriteMirrorActorIds.end() && *it != aid) {
        ++it;
      }
      if (it == this->spriteMirrorActorIds.end()) {
        this->scene->RemoveSkill(aid);
      } else if (this->scene->RemoveItem(aid) == true) {
        this->SendMsg(MM_DELETEREGISTERPOS, aid, 0, 0, 0);
        this->spriteMirrorActorIds.erase(it);
      }
      break;
    }
    case ID_ZC_NOTIFY_CARTITEM_COUNTINFO: {
      const unsigned char *packet = g_gameModePacketBuffer;
      g_session.cartCount = *reinterpret_cast<const std::int16_t *>(packet + 2);
      g_session.cartMaxCount = *reinterpret_cast<const std::int16_t *>(packet + 4);
      g_session.cartWeight = *reinterpret_cast<const int *>(packet + 6) / 10;
      g_session.cartMaxWeight = *reinterpret_cast<const int *>(packet + 0x0A) / 10;
      if (g_windowMgr.merchantItemWnd) {
        g_windowMgr.merchantItemWnd->SendMsg(nullptr, UM_SETCOUNTINFO, 0, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_CART_EQUIPMENT_ITEMLIST: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(EquipItemRecord);
      const EquipItemRecord *record = reinterpret_cast<const EquipItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = record->location;
        item.identity[1] = record->index;
        item.identity[3] = record->wearState;
        item.quantity = 1;
        std::memcpy(item.cards, record->cards, sizeof(record->cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = record->damaged;
        item.refiningLevel = record->refine;
        g_session.AddMerchantShopItem(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case ID_ZC_CART_NORMAL_ITEMLIST: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(NormalItemRecord);
      const NormalItemRecord *record = reinterpret_cast<const NormalItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        std::memset(item.cards, 0, sizeof(item.cards));
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        g_session.AddMerchantShopItem(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case ID_ZC_ADD_ITEM_TO_CART: {
      const unsigned char *packet = g_gameModePacketBuffer;
      ITEM_INFO item;
      item.identity[1] = *reinterpret_cast<const std::int16_t *>(packet + 2);
      item.quantity = *reinterpret_cast<const int *>(packet + 4);
      item.SetItemId(*reinterpret_cast<const int *>(packet + 8));
      item.identified = packet[0x0C];
      item.damaged = packet[0x0D];
      item.refiningLevel = packet[0x0E];
      std::memcpy(item.cards, packet + 0x0F, sizeof(item.cards));
      g_session.AddMerchantShopItem(item);
      break;
    }
    case ID_ZC_DELETE_ITEM_FROM_CART: {
      ITEM_INFO item;
      item.identity[1] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      item.quantity = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      g_session.RemoveMerchantShopItem(item);
      this->inventoryChanged = 1;
      break;
    }
    case ID_ZC_CARTOFF:
      g_session.ClearMerchantShopItems();
      g_windowMgr.CloseWindow(WID_MERCHANTITEMWND);
      break;
    case ID_ZC_ACK_ADDITEM_TO_CART:
      switch (g_gameModePacketBuffer[2]) {
      case ADDITEM_TO_CART_FAIL_WEIGHT:
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OVER_WEIGHT)), 0xFF, 0, 0, 0);
        break;
      case ADDITEM_TO_CART_FAIL_COUNT:
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_OVER_COUNT)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_OPENSTORE: {
      CGameMode *mode = static_cast<CGameMode *>(g_modeMgr.GetGameMode());
      if (mode && mode->drag.source != 0) {
        this->SendMsg(MM_REQ_CLOSESTORE, 0, 0, 0, 0);
        break;
      }

      const int slots = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      if (slots < 3) {
        g_session.m_vendingSlotCount = 3;
      } else {
        g_session.m_vendingSlotCount = slots > 13 ? 13 : slots;
      }
      g_session.ClearMerchantPurchaseSavedItems();
      if (g_windowMgr.chatRoomWnd || g_windowMgr.chatRoomMakeWnd) {
        g_windowMgr.Confirm(LookupMessage(MSI_CANT_OPEN_STORE_WHILE_CHAT), 0, true, false, 0, "Message", 280, 120, 0);
        this->SendMsg(MM_REQ_CLOSESTORE, 0, 0, 0, 0);
      } else if (UIMerchantShopMakeWnd *window = static_cast<UIMerchantShopMakeWnd *>(g_windowMgr.EnsureWindow(WID_MERCHANTSHOPMAKEWND))) {
        window->AddActionButton();
      }
      break;
    }
    case ID_ZC_STORE_ENTRY: {
      char title[81];
      strncpy(title, reinterpret_cast<const char *>(g_gameModePacketBuffer + 6), 80);
      title[80] = 0;
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        const int titleAddress = reinterpret_cast<int>(title);
        actor->SendMsg(nullptr, AM_MAKE_SHOP_TITLE, titleAddress, 0, 0, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_DISAPPEAR_ENTRY:
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        actor->SendMsg(nullptr, AM_DELETE_SHOP_TITLE, 0, 0, 0, 0, 0, 0);
      }
      break;
    case ID_ZC_DELETEITEM_FROM_MCSTORE:
      ProcessVendingSale(_time32(nullptr), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                 *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4), 0, 0);
      break;
    case ID_ZC_ATTACK_FAILURE_FOR_DISTANCE: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(packet + 2);
      g_session.storeItemLimit = *reinterpret_cast<const std::int16_t *>(packet + 0x0E);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        const int x = *reinterpret_cast<const std::int16_t *>(packet + 6);
        const int y = *reinterpret_cast<const std::int16_t *>(packet + 8);
        actor->SendMsg(nullptr, AM_CORRECT_POS, x, y, 0, 0, 0, 0);
      }
      const int playerX = *reinterpret_cast<const std::int16_t *>(packet + 0x0A);
      const int playerY = *reinterpret_cast<const std::int16_t *>(packet + 0x0C);
      this->scene->player->SendMsg(nullptr, AM_CORRECT_POS, playerX, playerY, 0, 0, 0, 0);
      this->scene->player->SendMsg(nullptr, AM_RESUMEPROCEEDATTACK, aid, 0, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_ATTACK_RANGE:
      g_session.storeItemLimit = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      break;
    case ID_ZC_EQUIP_ARROW:
      if (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) == 0 && g_session.equippedArrowIndex) {
        const int location = 0x8000;
        EquipmentProperties::CEquipmentPropertiesMgr *properties = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
        const int index = GetEquipLocationIndex(location);
        if (properties->FindProperty1(index)) {
          const int unequipLocation = 0x8000;
          EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->UnequipWearable(unequipLocation);
        }
      }
      g_session.SetEquippedArrowIndex(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2));
      *reinterpret_cast<std::uint32_t *>(&this->equipmentRequestState[0x04]) = timeGetTime() - 2000;
      break;
    case ID_ZC_RECOVERY:
      this->ReceiveHpSpRecovery(g_gameModePacketBuffer, 0);
      break;
    case ID_ZC_USESKILL_ACK:
      this->ProcessSkillUsePacket(g_gameModePacketBuffer, 0);
      break;
    case ID_ZC_OPEN_EDITDLG: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (this->scene->GetGameActorByAID(aid)) {
        g_windowMgr.EnsureWindow(WID_NPCEDITDIALOGWND)->SendMsg(nullptr, UM_SETNAID, aid, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_COMPASS: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const int type = *reinterpret_cast<const int *>(packet + 6);
      const std::uint32_t id = packet[0x12];
      if (type == 2) {
        this->RemoveMiniMapPing(id);
        break;
      }
      this->compassPosList[id].x = *reinterpret_cast<const int *>(packet + 0x0A);
      this->compassPosList[id].y = *reinterpret_cast<const int *>(packet + 0x0E);
      this->compassPosList[id].color = *reinterpret_cast<const std::uint32_t *>(packet + 0x13) | 0xFF000000;
      this->compassPosList[id].persistent = type;
      this->compassPosList[id].tick = timeGetTime();
      break;
    }
    case ID_ZC_SHOW_IMAGE: {
      char name[17];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), 16);
      int index = g_gameModePacketBuffer[0x12];
      name[16] = 0;
      if (index == 0xFF) {
        index = -1;
      }
      this->SendMsg(MM_SHOW_IMAGE, reinterpret_cast<int>(name), index, 0, 0);
      break;
    }
    case ID_ZC_AUTORUN_SKILL: {
      const unsigned char *packet = g_gameModePacketBuffer;
      CSkillInfo skill;
      skill.level = *reinterpret_cast<const std::int16_t *>(packet + 8);
      skill.type = *reinterpret_cast<const int *>(packet + 4);
      skill.id = *reinterpret_cast<const std::uint16_t *>(packet + 2);
      skill.upgradable = packet[0x26];
      skill.name = skill.IdName(*g_session.m_hotkeyScript);
      skill.spCost = *reinterpret_cast<const std::int16_t *>(packet + 0x0A);
      skill.range = *reinterpret_cast<const std::int16_t *>(packet + 0x0C);
      skill.apCost = skill.UseApAmount(*g_session.m_hotkeyScript, *reinterpret_cast<const std::int16_t *>(packet + 8));
      g_session.m_skillBundle.UpdatePending(skill);
      this->SendMsg(MM_USING_SKILL, reinterpret_cast<int>(&skill), skill.level, 1, -1);
      break;
    }
    case ID_ZC_RESURRECTION: {
      const std::uint32_t aid = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      if (CRenderObject *actor = this->scene->GetGameActorByAID(aid)) {
        actor->SendMsg(nullptr, AM_RESURRECTION, 0, 0, 0, 0, 0, 0);
      }
      if (aid == g_session.m_aid) {
        this->scene->player->SendMsg(nullptr, AM_RESURRECTION, 0, 0, 0, 0, 0, 0);
        this->isOnQuest = 0;
        this->isPlayerDead = 0;
      }
      MessengerMember member = g_session.PartyByAccount(aid);
      if (member.valid != 0) {
        g_session.SetPartyDead(aid, 0);
        RefreshPartyWindows(&g_session);
      }
      break;
    }
    case ID_ZC_ACK_GIVE_MANNER_POINT:
      switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
      case MP_SUCCESS:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SEND_MANNER_POINT)), 0xFFFF00, 0, 0, 0);
        break;
      case MP_SUCCESS_BY_ADMIN:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GM_BAD_MANNER_START)), 0xFFFF, 0, 0, 0);
        break;
      case MP_BYSELF:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_AUTO_BAD_MANNER_START)), 0xFFFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_NOTIFY_MANNER_POINT_GIVEN: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 3, 24);
      name[24] = 0;
      if (g_gameModePacketBuffer[2] == 0) {
        char text[128];
        sprintf(text, LookupMessage(MSI_RECEIVE_PLUS_MANNER_POINT), name);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF00, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_ACK_GUILD_MENUINTERFACE:
      g_guildInfo.guildTabBitmask = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      break;
    case ID_ZC_GUILD_EMBLEM_IMG: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const std::uint32_t guildId = *reinterpret_cast<const std::uint32_t *>(packet + 4);
      const int version = *reinterpret_cast<const int *>(packet + 8);
      g_session.StoreEmblem(packet + 0x0C, *reinterpret_cast<const std::int16_t *>(packet + 2) - 12, guildId, version);
      if (g_guildInfo.m_guildId == guildId) {
        this->scene->player->SetGuildInfo(guildId, version);
        if (g_windowMgr.guildInfoManageWnd) {
          g_windowMgr.guildInfoManageWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
        }
      }
      break;
    }
    case ID_ZC_ACK_REQ_CHANGE_MEMBERS: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 12;
      const int *record = reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, record += 3) {
        g_guildInfo.ChangeMemberPosition(record[0], record[1], record[2]);
        if (record[2] == 0) {
          g_guildInfo.SetMasterCharacterId(record[1]);
        }
      }
      if (UIWindow *window = g_windowMgr.LookupWindow(WID_GUILDMEMBERMANAGEWND)) {
        window->SendMsg(nullptr, UM_SET_DISABLE, 0, 0, 0, 0);
      }
      g_windowMgr.RefreshGuildWindows();
      break;
    }
    case ID_ZC_ACK_DISORGANIZE_GUILD_RESULT:
      switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
      case DISRESULT_SUCCESS:
        g_session.ClearAdditionalShortcutSkills();
        g_guildInfo.Init();
        g_windowMgr.CloseWindow(WID_GUILDWND);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DISORGANIZE_GUILD_SUCCESS)), 0xFFFF, 4, 0, 0);
        break;
      case DISRESULT_INVALIDKEY:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DISORGANIZE_GUILD_INVALIDKEY)), 0xFFFF, 0, 0, 0);
        break;
      case DISRESULT_MEMBEREXIST:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_DISORGANIZE_GUILD_MEMBEREXIST)), 0xFFFF, 4, 0, 0);
        break;
      }
      break;
    case ID_ZC_ACK_DISORGANIZE_GUILD: {
      char reason[41];
      std::memcpy(reason, g_gameModePacketBuffer + 2, 40);
      reason[40] = 0;
      char text[1024];
      sprintf(text, LookupMessage(MSI_DISORGANIZATION_REASON), reason);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 4, 0, 0);
      break;
    }
    case ID_ZC_GUILD_SKILLINFO:
      this->ApplyAdditionalSkillPacket(g_gameModePacketBuffer);
      break;
    case ID_ZC_RESULT_MAKE_GUILD:
      switch (g_gameModePacketBuffer[2]) {
      case GM_SUCCESS:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_MAKE_SUCCESS)), 0xFFFF00, 4, 0, 0);
        g_windowMgr.CloseWindow(WID_GUILDTIPWND);
        g_windowMgr.CloseWindow(WID_CREATEGUILDWND_MODE_0);
        g_windowMgr.CloseWindow(WID_GUILDHELPERWND);
        break;
      case GM_ALREADY_MEMBER:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_MAKE_ALREADY_MEMBER)), 0xFFFF, 4, 0, 0);
        break;
      case GM_GUILD_EXIST:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_MAKE_GUILD_EXIST)), 0xFFFF, 4, 0, 0);
        break;
      case GM_NONE_ITEM:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_MAKE_GUILD_NONE_ITEM)), 0xFFFF, 4, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_LEAVE_GUILD_NOT_PERMITTED_MAP)), 0xFFFF, 4, 0, 0);
        break;
      }
      break;
    case ID_ZC_ACK_REQ_JOIN_GUILD:
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      switch (g_gameModePacketBuffer[2]) {
      case REQJOINGUILD_ALREADY_OTHERGUILDM:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_IS_ALREADY_IN_GUILD)), 0xFF, 4, 0, 0);
        break;
      case REQJOINGUILD_REJECT:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_REJECT_JOIN_GUILD)), 0xFF, 4, 0, 0);
        break;
      case REQJOINGUILD_ACCEPT:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHARACTER_ACCEPT_JOIN_GUILD)), 0xFFFF00, 4, 0, 0);
        break;
      case REQJOINGUILD_OVERSIZE:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_MANY_PEOPLE_IN_GUILD)), 0xFF, 4, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_JOINGUILD_TARGET_USER_IN_SIEGEMAP)), 0xFF, 4, 0, 0);
        break;
      }
      break;
    case ID_ZC_REQ_JOIN_GUILD: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 6, 24);
      name[24] = 0;
      if (g_forbiddenWords.Contains(name) == true) {
        std::string masked("");
        g_forbiddenWords.Mask(name, &masked);
      }
      g_windowMgr.EnsureWindow(WID_JOINGUILDACCEPTWND)->SendMsg(nullptr, UM_SET_JOIN_GUILD_MSG, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), reinterpret_cast<std::intptr_t>(name), 0, 0);
      break;
    }
    case ID_ZC_UPDATE_GDID: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const int flags = *reinterpret_cast<const int *>(packet + 0x0A);
      const std::uint32_t guildId = *reinterpret_cast<const std::uint32_t *>(packet + 2);
      g_guildInfo.canInvite = flags & 1;
      g_guildInfo.m_guildId = guildId;
      g_guildInfo.canExpel = (flags >> 4) & 1;
      g_guildInfo.isMaster = packet[0x0E];
      this->scene->player->SetGuildInfo(guildId, *reinterpret_cast<const int *>(packet + 6));
      if (g_guildInfo.skillsRequested == 0) {
        g_guildInfo.skillsRequested = 1;
        g_modeMgr.gameMode->SendMsg(MM_REQ_GUILD_MENU, 3, 0, 0, 0);
      }
      if (g_guildInfo.membersRequested == 0) {
        g_guildInfo.membersRequested = 1;
        g_modeMgr.gameMode->SendMsg(MM_REQ_GUILD_MENU, 1, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_UPDATE_CHARSTAT: {
      const unsigned char *packet = g_gameModePacketBuffer;
      const int online = *reinterpret_cast<const int *>(packet + 0x0A);
      g_guildInfo.SetMemberOnline(*reinterpret_cast<const int *>(packet + 2), online);
      std::string name = LookupPlayerName(*reinterpret_cast<const std::uint32_t *>(packet + 6));
      if (name.size()) {
        this->SendMsg(MM_GUILD_MEMBER_STATUS_CHANGE, reinterpret_cast<int>(name.c_str()), online, 0, 0);
      }
      if (online == 0) {
        this->SendMsg(MM_DELETEGUILDPOS, *reinterpret_cast<const int *>(packet + 2), 0, 0, 0);
      }
      break;
    }
    case ID_ZC_REQ_ALLY_GUILD: {
      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 6, 24);
      name[24] = 0;
      if (g_forbiddenWords.Contains(name) == true) {
        std::string masked("");
        g_forbiddenWords.Mask(name, &masked);
      }
      g_windowMgr.EnsureWindow(WID_ALLYGUILDACCEPTWND)->SendMsg(nullptr, UM_SET_ALLY_GUILD_MSG, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), reinterpret_cast<std::intptr_t>(name), 0, 0);
      break;
    }
    case ID_ZC_ACK_REQ_ALLY_GUILD:
      switch (g_gameModePacketBuffer[2]) {
      case REQALLYGUILD_ALREADY_ALLIED:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REQALLYGUILD_ALREADY_ALLIED)), 0xFFFF, 4, 0, 0);
        break;
      case REQALLYGUILD_REJECT:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REQALLYGUILD_REJECT)), 0xFFFF, 4, 0, 0);
        break;
      case REQALLYGUILD_ACCEPT:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REQALLYGUILD_ACCEPT)), 0xFFFF00, 4, 0, 0);
        break;
      case REQALLYGUILD_OVERSIZE:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REQALLYGUILD_OVERSIZE)), 0xFFFF, 4, 0, 0);
        break;
      case REQMYGUILD_OVERSIZE:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_REQALLYMYGUILD_OVERSIZE)), 0xFFFF, 4, 0, 0);
        break;
      case 5:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_ALLY_REQUEST_DISABLE)), 0xFFFF, 4, 0, 0);
        break;
      }
      break;
    case ID_ZC_ITEMIDENTIFY_LIST: {
      unsigned int count = (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4u) >> 1;
      UIWindow *window = g_windowMgr.EnsureWindow(WID_ITEMIDENTIFYWND);
      window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
      const std::int16_t *index = reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      for (; count != 0; --count, ++index) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *index);
        if (item.identity[1]) {
          window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.identity[1], 0, 0);
        }
      }
      window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_ACK_ITEMIDENTIFY:
      switch (g_gameModePacketBuffer[4]) {
      case ITEMIDENTIFY_SUCCESS: {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_IDENTIFY_SUCCEESS)), 0xFFFF00, 0, 0, 0);
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2));
        item.identified = 1;
        g_session.UpdateInventoryItem(item, 1);
        break;
      }
      case ITEMIDENTIFY_FAIL:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_IDENTIFY_FAIL)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_ITEMCOMPOSITION_LIST: {
      unsigned int count = (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4u) >> 1;
      UIWindow *window = g_windowMgr.EnsureWindow(WID_ITEMCOMPOSITIONWND);
      window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_SET_CARD_ITEM_INDEX, *reinterpret_cast<const int *>(this->cardAndSkillBalloonIds), 0, 0, 0);
      const std::int16_t *index = reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      for (; count != 0; --count, ++index) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *index);
        if (item.identity[1]) {
          window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.identity[1], 0, 0);
        }
      }
      window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_ACK_REQ_HOSTILE_GUILD:
      switch (g_gameModePacketBuffer[2]) {
      case RESULT_HOSTILE_SUCCESS:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_HOSTILE_GUILD_SUCCEESS)), 0xFFFF00, 4, 0, 0);
        break;
      case RESULT_HOSTILE_OVERSIZE:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TOO_MANY_HOSTILE_GUILD)), 0xFF, 4, 0, 0);
        break;
      case RESULT_HOSTILE_ALREADY_HOSTILE:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ALREADY_REGISTERED_HOSTILE_GUILD)), 0xFF, 4, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_HOSTILE_REQUEST_DISABLE)), 0xFF, 4, 0, 0);
        break;
      }
      break;
    case ID_ZC_DELETE_RELATED_GUILD:
      if (g_guildInfo.infoRequested) {
        g_guildInfo.RemoveRelatedGuild(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6));
      }
      break;
    case ID_ZC_NOTIFY_MAPINFO:
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)) {
      case TELEPORT_IMPOSSIBLE_AREA:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_IMPOSSIBLE_TELEPORT_AREA)), 0xFF, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_IMPOSSIBLE_TELEPORT_AREA)), 0xFF, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_POSSIBLE_TELEPORT_AREA)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_POSSIBLE_TELEPORT_AREA)), 0xFFFF00, 0, 0, 0);
        break;
      case IMPOSSIBLE_AREA_SKILL:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_IMPOSSIBLE_SKILL_AREA)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_IMPOSSIBLE_SKILL_AREA)), 0xFFFF00, 0, 0, 0);
        break;
      case IMPOSSIBLE_AREA_USEITEM:
        this->waitingUseItemAck = 0;
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_IMPOSSIBLE_USEITEM_AREA)), 0xFFFF00, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY2, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_IMPOSSIBLE_USEITEM_AREA)), 0xFFFF00, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_ACK_REQ_DISCONNECT:
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)) {
      case DISCONNECTABLE_STATE:
        GetRagConnection().Close();
        this->SendMsg(MM_QUIT, 0, 0, 0, 0);
        break;
      case NOTDISCONNECTABLE_STATE:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANT_EXIT_NOW)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_MONSTER_INFO: {
      const std::uint16_t id = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2);
      if (id != 1285 && id != 1286 && id != 1287 && id != 1829 && id != 1830) {
        g_windowMgr.EnsureWindow(WID_MONSTERINFOWND)->SendMsg(nullptr, UM_SET_MONSTER_INFO, reinterpret_cast<std::intptr_t>(g_gameModePacketBuffer), 0, 0, 0);
      }
      break;
    }
    case ID_ZC_MAKABLEITEMLIST: {
      unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) >> 4;
      if (count == 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_CANT_MAKE_ITEM), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      g_windowMgr.CloseWindow(WID_MAKETARGETLISTWND);
      UIWindow *window = g_windowMgr.EnsureWindow(WID_MAKETARGETLISTWND);
      const int *record = reinterpret_cast<const int *>(g_gameModePacketBuffer + 4);
      for (; count != 0; --count, record += 4) {
        window->SendMsg(nullptr, UM_ADDITEM, *record, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_TALKBOX_CHATCONTENTS:
      if (CRenderObject *skill = this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        const int textAddress = reinterpret_cast<int>(g_gameModePacketBuffer + 6);
        skill->SendMsg(nullptr, AM_CHAT, textAddress, 0, 0, 0, 0, 0);
      }
      break;
    case ID_ZC_UPDATE_MAPINFO: {
      CGameMode *mode = static_cast<CGameMode *>(g_modeMgr.GetGameMode());
      const int column = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      const int row = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      const_cast<TerrainHeightCell *>(mode->scene->terrain->CellAt(column, row))->type = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6);
      break;
    }
    case ID_ZC_ACK_REQNAME_BYGID:
      SetPlayerName(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2),
          std::string(reinterpret_cast<const char *>(g_gameModePacketBuffer + 6), reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x1E)).c_str());
      break;
    case ID_ZC_NOTIFY_MAPPROPERTY:
      g_serviceOptions.Set(229, *reinterpret_cast<const int *>(g_session.interactionFlags) != 0);
      switch (*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)) {
      case MAPPROPERTY_FREEPVPZONE:
      case MAPPROPERTY_PVPSERVERZONE:
        this->scene->isPKZone = 1;
        break;
      case MAPPROPERTY_EVENTPVPZONE:
        this->scene->isEventPVPMode = 1;
        break;
      case MAPPROPERTY_AGITZONE:
        this->scene->isSiegeMode = 1;
        g_serviceOptions.Set(229, 1);
        break;
      case MAPPROPERTY_PKSERVERZONE:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PKZONE)), 0x9B9BFF, 0, 0, 0);
        break;
      }
      if (g_session.GetMapProperty0F()) {
        g_serviceOptions.Set(229, 1);
      }
      if (g_serviceType) {
        g_mapPropertyMgr->Clear();
      }
      break;
    case ID_ZC_PROPERTY_PET: {
      unsigned char *packet = g_gameModePacketBuffer;
      g_session.petJob = *reinterpret_cast<const std::int16_t *>(packet + 0x23);
      std::int16_t &value = *reinterpret_cast<std::int16_t *>(packet + 0x1B);
      if (value >= 10000) {
        value -= 10000;
        if (UIItemCollectionWnd *window = static_cast<UIItemCollectionWnd *>(g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND))) {
          window->m_petLevel = value;
          window->m_petHunger = *reinterpret_cast<const std::int16_t *>(packet + 0x1D);
          window->m_petFriendly = *reinterpret_cast<const std::int16_t *>(packet + 0x1F);
          window->m_petAccessory = *reinterpret_cast<const std::uint16_t *>(packet + 0x21);
          window->m_petRenamed = packet[0x1A];
          sprintf(reinterpret_cast<char *>(window->m_petName), reinterpret_cast<const char *>(packet + 2));
          window->SendMsg(nullptr, UM_SETTEXT, 0, 0, 0, 0);
        }
        break;
      }

      sprintf(g_session.petName, reinterpret_cast<const char *>(packet + 2));
      g_session.petPreviousHunger = g_session.petHunger;
      g_session.petHunger = *reinterpret_cast<const std::int16_t *>(packet + 0x1D);
      g_session.m_petFriendly = *reinterpret_cast<const std::int16_t *>(packet + 0x1F);
      g_session.petAccessory = *reinterpret_cast<const std::uint16_t *>(packet + 0x21);
      if (UIPetInfoWnd *window = static_cast<UIPetInfoWnd *>(g_windowMgr.petInfoWnd)) {
        window->UpdateFriendlyGauge();
        window->nameEdit->SetText(g_session.petName);
        window->Invalidate();
      }
      *reinterpret_cast<int *>(g_session.petNameModified) = packet[0x1A];
      g_session.petLevel = value;
      break;
    }
    case ID_ZC_PETEGG_LIST: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) >> 1;
      UIWindow *window = g_windowMgr.EnsureWindow(WID_PETEGGLISTWND);
      window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
      const std::int16_t *index = reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++index) {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *index);
        if (item.identity[1]) {
          window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(item.itemId.c_str()), item.identity[1], 0, 0);
        }
      }
      window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_PAR_CHANGE_USER:
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        if (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6) == 4) {
          actor->SetHonor(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 8));
        }
      }
      break;
    case ID_ZC_SKILL_UPDATE:
      if (CGameActor *skill = static_cast<CGameActor *>(this->scene->GetSkillByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        if (skill->GetEffectiveJob(-1) > 138 && skill->GetEffectiveJob(-1) <= 153) {
          skill->SendMsg(nullptr, AM_SKILL_UPDATE, 0, 0, 0, 0, 0, 0);
        }
        switch (skill->GetEffectiveJob(-1)) {
        case UNT_DEEPBLINDTRAP:
        case UNT_SOLIDTRAP:
        case UNT_SWIFTTRAP:
        case UNT_FLAMETRAP:
          skill->SendMsg(nullptr, AM_SKILL_UPDATE, 0, 0, 0, 0, 0, 0);
          break;
        }
      }
      break;
    case ID_ZC_MAKINGARROW_LIST: {
      const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4) >> 2;
      UIWindow *window = g_windowMgr.EnsureWindow(WID_MAKINGARROWLISTWND);
      window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
      const unsigned char *entry = g_gameModePacketBuffer + 4;
      for (unsigned int i = count; i != 0; --i, entry += 4) {
        window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(entry), 0, 0, 0);
      }
      window->SendMsg(nullptr, UM_SETINFO, 2, 0, 0, 0);
      break;
    }
    case ID_ZC_NPCSPRITE_CHANGE:
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        actor->SetSprAct(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 7), 0, 0);
      }
      break;
    case ID_ZC_SHOWDIGIT: {
      const int seconds = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 3);
      switch (g_gameModePacketBuffer[2]) {
      case 0:
        *reinterpret_cast<int *>(g_session.timeAttackMode) = 0;
        g_session.m_timeAttackVisible = 1;
        g_session.m_timeAttackCountdown = seconds * 1000;
        break;
      case 1:
        g_session.m_timeAttackCountdown = timeGetTime() + seconds * 1000;
        *reinterpret_cast<int *>(g_session.timeAttackMode) = 1;
        g_session.m_timeAttackVisible = 1;
        break;
      case 2:
        g_session.m_timeAttackCountdown = timeGetTime() - seconds * 1000;
        *reinterpret_cast<int *>(g_session.timeAttackMode) = 2;
        g_session.m_timeAttackVisible = 1;
        break;
      case 3:
        *reinterpret_cast<int *>(g_session.timeAttackMode) = 3;
        g_session.m_timeAttackCountdown = seconds * 30 + 29;
        g_session.m_timeAttackVisible = 1;
        break;
      default:
        g_session.m_timeAttackVisible = 1;
        break;
      }
      break;
    }
    case ID_ZC_SHOW_IMAGE2: {
      char name[65];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), 64);
      int position = g_gameModePacketBuffer[0x42];
      name[64] = 0;
      if (position == 255) {
        position = -1;
      }
      this->SendMsg(MM_SHOW_IMAGE, reinterpret_cast<int>(name), position, 0, 0);
      break;
    }
    case ID_ZC_CHANGE_GUILD:
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        actor->SetGuildInfo(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0xA));
      }
      break;
    case ID_ZC_INFO_REMAINTIME: {
      char message[256];
      const int remaining = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
      if (g_serviceType == 0) {
        switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
        case ID_CA_LOGIN:
          sprintf(message, "\xb0\xb3\xc0\xce \xc1\xa4\xbe\xd7\xc1\xa6 \xbd\xc3\xb0\xa3\xc0\xcc %d\xba\xd0 %d\xc3\xca \xb3\xb2\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.",
                  remaining / 60, remaining % 60);
          break;
        case ID_CZ_PC_PURCHASE_ITEMLIST:
          sprintf(message, "\xb0\xb3\xc0\xce \xc1\xa4\xb7\xae\xc1\xa6 \xbd\xc3\xb0\xa3\xc0\xcc %d\xba\xd0 %d\xc3\xca \xb3\xb2\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.",
                  remaining / 60, remaining % 60);
          break;
        case ID_ZC_ACK_ADDITEM_TO_CART:
          sprintf(message, "PC\xb9\xe6 \xc1\xa4\xb7\xae\xc1\xa6 \xbd\xc3\xb0\xa3\xc0\xcc %d\xba\xd0 %d\xc3\xca \xb3\xb2\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.",
                  remaining / 60, remaining % 60);
          break;
        }
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
      } else if (g_serviceType == 6) {
        sprintf(message, "Sisa waktu voucher kamu adalah %d menit.", remaining / 60);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFFFF, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_BROADCAST2:
      this->ReceiveColorBroadcast(g_gameModePacketBuffer);
      break;
    case ID_SC_ACK_ENCRYPTION:
      ShowErrorDialog(GetRenderSystemHolder()->system, "No Packet Encryption !!!", 0);
      break;
    case ID_ZC_USE_ITEM_ACK2: {
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 8) == g_session.m_aid) {
        this->waitingUseItemAck = 0;
        if (g_gameModePacketBuffer[0xE]) {
          ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2));
          if (item.identity[1]) {
            g_session.RemoveInventoryItem(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                     item.quantity - *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0xC));
            if (item.GetItemId() == 607) {
              this->yggdrasilUsePending = 0;
            }
          }
        } else {
          if (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4) != 103061) {
            break;
          }
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_REWARD_OVER_ITEM_COUNT)), 0xFF, 0, 0, 0);
        }
      }
      if (g_gameModePacketBuffer[0xE]) {
        this->PlayItemUseEffect(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 8));
      }
      break;
    }
    case ID_ZC_AUTOSPELLLIST: {
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
      if (UIWindow *window = g_windowMgr.EnsureWindow(WID_SPELLLISTWND)) {
        for (const int *skill = reinterpret_cast<const int *>(g_gameModePacketBuffer + 2); skill < reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E);
             ++skill) {
          if (*skill) {
            window->SendMsg(nullptr, UM_ADDITEM, *skill, 0, 0, 0);
          }
        }
      }
      break;
    }
    case ID_ZC_SPIRITS:
      this->UpdateActorSpiritCount(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2),
                                   *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6));
      break;
    case ID_ZC_OPEN_EDITDLGSTR:
      if (this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)) ||
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == 0) {
        g_windowMgr.EnsureWindow(WID_NPCTEXTEDITDIALOGWND)->SendMsg(nullptr, UM_SETNAID, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      }
      break;
    case ID_ZC_NOTIFY_MAPPROPERTY2: {
      g_serviceOptions.Set(229, *reinterpret_cast<const int *>(g_session.interactionFlags) != 0);
      const std::int16_t type = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      g_session.mapPropertyId = type;
      if (type == 5 || type == 6) {
        this->scene->isPKZone = 1;
        this->forcedAttack = 1;
      } else if (type != 11 && type != 12) {
        if (g_session.GetMapProperty09() || *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) == 18 ||
            *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) == 8) {
          this->scene->isSiegeMode = 1;
          g_serviceOptions.Set(229, 1);
        } else if (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) == 19) {
          this->scene->isBattleFieldMode = 1;
          this->scene->isPKZone = 1;
        }
      }
      if (g_session.GetMapProperty0F()) {
        g_serviceOptions.Set(229, 1);
      }
      if (!g_statusRefreshSuppressed) {
        static_cast<CGameMode *>(g_modeMgr.GetGameMode())->scene->player->ReapplySkillEffects();
      }
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)) {
      case MAPTYPE_PENALTY_FREEPKZONE:
      case MAPTYPE_NOPENALTY_FREEPKZONE:
      case MAPTYPE_EVENT_GUILDWAR:
      case MAPTYPE_AGIT:
      case MAPTYPE_DUNGEON2:
      case MAPTYPE_PKSERVER:
      case MAPTYPE_TURBOTRACK:
      case MAPTYPE_PORINGBATTLE:
      case MAPTYPE_AGIT_SIEGEV15:
      case MAPTYPE_BATTLEFIELD:
      case MAPTYPE_SIEGE_LOWLEVEL:
      case 31:
      case 32:
        this->scene->partyRestricted = true;
        break;
      }
      break;
    }
    case ID_ZC_REQ_COUPLE: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      *reinterpret_cast<int *>(this->lastCoupleIds) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      *reinterpret_cast<int *>(this->lastCoupleIds + 4) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
      g_windowMgr.EnsureWindow(WID_COUPLEACCEPTWND)->SendMsg(nullptr, UM_SET_COUPLE_MSG, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), reinterpret_cast<std::intptr_t>(name), 0,
                                           0);
      break;
    }
    case ID_ZC_NOTIFY_POSITION_TO_GUILDM:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) != g_session.m_aid) {
        this->SendMsg(MM_ADDGUILDPOS, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6),
                      *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8), 0);
      }
      break;
    case ID_ZC_GUILD_MEMBER_MAP_CHANGE:
      if (strcmp(g_session.m_navigationPlayerMap, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA))) {
        this->SendMsg(MM_DELETEGUILDPOS, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), 0, 0, 0);
        if (CGameActor *actor =
                static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6)))) {
          actor->SetGroupMemberVisible(0);
        }
      } else if (CGameActor *actor =
                     static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6)))) {
        actor->SetGroupMemberVisible(1);
      }
      break;
    case ID_ZC_NORMAL_ITEMLIST2: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemRecord);
      const CardItemRecord *record = reinterpret_cast<const CardItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case ID_ZC_CART_NORMAL_ITEMLIST2: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemRecord);
      const CardItemRecord *record = reinterpret_cast<const CardItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        g_session.AddMerchantShopItem(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case ID_ZC_STORE_NORMAL_ITEMLIST2: {
      g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemRecord);
      const CardItemRecord *record = reinterpret_cast<const CardItemRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        g_session.AddCartItem(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case ID_ZC_UPDATE_CHARSTAT2: {
      g_guildInfo.UpdateMemberStatus(
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xE),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x10), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x12));
      std::string name = LookupPlayerName(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6));
      if (name.size()) {
        this->SendMsg(MM_GUILD_MEMBER_STATUS_CHANGE, reinterpret_cast<int>(name.c_str()), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA), 0, 0);
      }
      if (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA) == 0) {
        this->SendMsg(MM_DELETEGUILDPOS, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), 0, 0, 0);
      }
      break;
    }
    case ID_ZC_REQ_EXCHANGE_ITEM2: {
      if (g_serviceOptions.Get(136)) {
        g_modeMgr.gameMode->SendMsg(MM_ACK_EXCHANGE_ITEM, 4, 0, 0, 0);
        break;
      }

      char name[25];
      std::memcpy(name, g_gameModePacketBuffer + 2, 24);
      name[24] = '\0';
      g_windowMgr.EnsureWindow(WID_EXCHANGEACCEPTWND)->SendMsg(nullptr, UM_SET_EXCHANGE_CHAR_NAME, reinterpret_cast<std::intptr_t>(name), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1A),
                                          *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1E), 0);
      this->exchangeName.assign(name, strlen(name));
      break;
    }
    case ID_ZC_REQ_BABY: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      *reinterpret_cast<int *>(this->lastCoupleIds) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      *reinterpret_cast<int *>(this->lastCoupleIds + 4) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
      g_windowMgr.EnsureWindow(WID_BABYACCEPTWND)->SendMsg(nullptr, UM_SET_BABY_MSG, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), reinterpret_cast<std::intptr_t>(name), 0,
                                           0);
      break;
    }
    case ID_ZC_REPAIRITEMLIST: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 23;
      if (count == 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_CANT_MAKE_ITEM), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      g_windowMgr.CloseWindow(WID_ITEMREPAIRWND);
      UIWindow *window = g_windowMgr.EnsureWindow(WID_ITEMREPAIRWND);
      const unsigned char *record = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, record += 23) {
        window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(record), 0, 0, 0);
      }
      break;
    }
    case ID_ZC_ACK_ITEMREPAIR:
      switch (g_gameModePacketBuffer[4]) {
      case ITEMREPAIR_SUCCESS:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REPAIR_SUCCEESS)), 0xFFFF00, 0, 0, 0);
        break;
      case ITEMREPAIR_FAIL:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REPAIR_FAIL)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_HIGHJUMP: {
      vector3d position{0.0f, 0.0f, 0.0f};
      static_cast<CGameMode *>(g_modeMgr.GetGameMode())
          ->CellCenter(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8),
                       position.x, position.z);
      position.y = this->scene->terrain->Height(position.x, position.z);
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        this->scene->player->damageDestX = position.x;
        this->scene->player->damageDestZ = position.z;
        this->scene->player->render.position = position;
      } else if (CGameActor *actor =
                     static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        actor->damageDestX = position.x;
        actor->damageDestZ = position.z;
        actor->render.position = position;
      }
      break;
    }
    case ID_ZC_DIVORCE: {
      char message[1024];
      sprintf(message, LookupMessage(MSI_DIVORCE), g_session.DecodePlayerName(), reinterpret_cast<const char *>(g_gameModePacketBuffer + 2));
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF00, 0, 0, 0);
      break;
    }
    case ID_ZC_FRIENDS_STATE:
      g_session.UpdateFriendName(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
                                 reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xB));
      if (g_session.SetFriendOffline(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2),
                                     *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6), g_gameModePacketBuffer[0xA])) {
        g_session.RefreshMessengerGroupWindow();
      }
      break;
    case ID_ZC_REQ_ADD_FRIENDS: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      const unsigned char option = g_serviceOptions.Get(136);
      if (IsMonsterJob(g_session.m_aid) || option) {
        g_modeMgr.gameMode->SendMsg(MM_JOIN_FRIEND, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                                    0, 0);
      } else {
        g_windowMgr.EnsureWindow(WID_JOINFRIENDACCEPTWND)->SendMsg(nullptr, UM_SET_JOIN_FRIEND_MSG, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2),
                                             *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), reinterpret_cast<std::intptr_t>(name), 0);
      }
      break;
    }
    case ID_ZC_ADD_FRIENDS_LIST:
      ReceiveFriendAddResult(g_gameModePacketBuffer);
      break;
    case ID_ZC_DELETE_FRIENDS:
      if (g_session.RemoveFriendByCharacterId(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6))) {
        g_session.RefreshMessengerGroupWindow();
      }
      break;
    case ID_ZC_BABYMSG:
      switch (*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2)) {
      case 0:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BABY1)), 0x9B9BFF, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BABY2)), 0x9B9BFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BABY3)), 0x9B9BFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_NOTIFY_WEAPONITEMLIST: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 23;
      if (count == 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_CANT_MAKE_ITEM), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }
      if (g_windowMgr.LookupWindow(WID_WEAPONREFINEWND)) {
        g_windowMgr.CloseWindow(WID_WEAPONREFINEWND);
      }
      UIWindow *window = g_windowMgr.EnsureWindow(WID_WEAPONREFINEWND);
      const unsigned char *record = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, record += 23) {
        window->SendMsg(nullptr, UM_ADDITEM, *reinterpret_cast<const int *>(record + 2), *reinterpret_cast<const std::int16_t *>(record), 0, 0);
      }
      break;
    }
    case ID_ZC_STATE_CHANGE3: {
      int state[4];
      state[1] = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA);
      state[3] = g_gameModePacketBuffer[0xE];
      state[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 6);
      state[2] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 8);
      CRenderObject *actor;
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        actor = this->scene->player;
        this->ApplyStatusIcons(state[0], state[2]);
        reinterpret_cast<int *>(g_session.statusLevelEffects)[0] = state[0];
        reinterpret_cast<int *>(g_session.statusLevelEffects)[1] = state[2];
        g_session.m_jobFlags = state[1];
        if (g_windowMgr.equipWnd) {
          g_windowMgr.equipWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
        }
        if (g_session.replayPacketMode <= 0) {
          g_session.ClearMerchantShopItems();
          g_windowMgr.CloseWindow(WID_MERCHANTITEMWND);
        }
      } else {
        actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2));
      }
      if (actor) {
        const int stateAddress = reinterpret_cast<int>(state);
        actor->SendMsg(nullptr, AM_ATTR_STATE_CHANGE, stateAddress, 0, 0, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_STARPLACE:
      g_windowMgr.EnsureWindow(WID_STARPLACEACCEPTWND)->SendMsg(nullptr, UM_SET_BABY_MSG, static_cast<signed char>(g_gameModePacketBuffer[2]), 0, 0, 0);
      break;
    case ID_ZC_MAKINGITEM_LIST: {
      const unsigned int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 6) >> 2;
      UIWindow *window = g_windowMgr.EnsureWindow(WID_MAKINGARROWLISTWND);
      if (!window) {
        break;
      }
      window->SendMsg(nullptr, UM_CLEAR_ITEM, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_SETINFO, *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4), 0, 0, 0);
      const unsigned char *entry = g_gameModePacketBuffer + 6;
      for (unsigned int i = count; i != 0; --i, entry += 4) {
        window->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(entry), 0, 0, 0);
      }
      break;
    }
    case ID_ZC_DEL_QUEST: {
      std::string title = g_renewQuestMgr->GetOngoingQuest(*reinterpret_cast<const unsigned int *>(g_gameModePacketBuffer + 2)).m_title;
      CRenewQuestMgr *manager = g_renewQuestMgr;
      unsigned int id = *reinterpret_cast<const unsigned int *>(g_gameModePacketBuffer + 2);
      manager->m_ongoingQuests.erase(id);
      manager->UpdateQuestWindows();
      std::string message = FormatOwnedString(LookupMessage(MSI_DELETE_QUEST), title.c_str());
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFFFF, 19, 0, 0);
      break;
    }
    case ID_ZC_ACTIVE_QUEST:
      g_renewQuestMgr->SetOngoingQuestState(*reinterpret_cast<const unsigned int *>(g_gameModePacketBuffer + 2), g_gameModePacketBuffer[6]);
      g_renewQuestMgr->UpdateQuestWindows();
      break;
    case ID_ZC_NOTIFY_BIND_ON_EQUIP: {
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2));
      if (item.identity[1]) {
        std::string name = item.DisplayName(0);
        std::string message = FormatOwnedString(LookupMessage(MSI_YOURITEM_EQUIPED), name.c_str());
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0x6EFFFF, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_BATTLEFIELD_NOTIFY_CAMPINFO:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        g_session.playerTeam[0] = g_gameModePacketBuffer[0x1E];
        break;
      }
      if (CGameActor *actor = static_cast<CGameActor *>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2)))) {
        const unsigned char team = g_gameModePacketBuffer[0x1E];
        reinterpret_cast<unsigned char *>(&actor->render.flags)[3] = team;
        if (team != g_session.playerTeam[0]) {
          OpenWordDisplay(actor);
        } else if (actor->actorTitleWindow) {
          g_windowMgr.Schedule(reinterpret_cast<UIWindow *>(actor->actorTitleWindow));
          actor->actorTitleWindow = 0;
        }
      }
      break;
    case ID_ZC_BATTLEFIELD_NOTIFY_POINT:
      g_session.m_score1 = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      g_session.m_score2 = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      break;
    case ID_ZC_BATTLEFIELD_NOTIFY_POSITION:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) != g_session.m_aid) {
        this->SendMsg(MM_ADDPARTYPOS, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x20),
                      *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x22), 0);
      }
      break;
    case ID_ZC_BATTLEFIELD_NOTIFY_HP:
      if (g_session.IsCompanionAid(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        break;
      }
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        const int x = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1E);
        const int y = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x20);
        actor->SendMsg(nullptr, AM_UPDATEGAGE, x, y, 0, 0, 0, 0);
      }
      break;
    case ID_ZC_NOTIFY_ACT2:
      this->ReceiveActorAction(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x16),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1A), g_gameModePacketBuffer[0x1C],
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1D), 0);
      break;
    case ID_ZC_NORMAL_ITEMLIST3: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemExpiryRecord);
      const CardItemExpiryRecord *entry = reinterpret_cast<const CardItemExpiryRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++entry) {
        const CardItemRecord *record = &entry->base;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        item.expireTime = entry->expireTime;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case ID_ZC_CART_NORMAL_ITEMLIST3: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemExpiryRecord);
      const CardItemExpiryRecord *entry = reinterpret_cast<const CardItemExpiryRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++entry) {
        const CardItemRecord *record = &entry->base;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        item.expireTime = entry->expireTime;
        g_session.AddMerchantShopItem(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case ID_ZC_STORE_NORMAL_ITEMLIST3: {
      g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(CardItemExpiryRecord);
      const CardItemExpiryRecord *entry = reinterpret_cast<const CardItemExpiryRecord *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++entry) {
        const CardItemRecord *record = &entry->base;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->identified;
        item.damaged = 0;
        item.refiningLevel = 0;
        item.expireTime = entry->expireTime;
        g_session.AddCartItem(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case ID_ZC_NOTIFY_FONT:
      if (*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2) == g_session.m_aid) {
        g_session.chatFont[0] = g_gameModePacketBuffer[6];
      } else if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        *reinterpret_cast<std::uint16_t *>(actor->render.effectPhaseBytes) = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6);
      }
      break;
    case ID_ZC_PROGRESS:
      if (CPlayer *player = this->scene->player) {
        player->SendMsg(nullptr, AM_MAKE_SKILL_RECHARGEGAGE, static_cast<std::uint32_t>(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 6) * 1000), 0, 0, 0, 0, 0);
        g_session.playerStatusFlags[0x0A] = true;
      }
      break;
    case ID_ZC_PROGRESS_CANCEL:
      if (CPlayer *player = this->scene->player) {
        player->SendMsg(nullptr, AM_DELETE_SKILL_RECHARGEGAGE, 0, 0, 0, 0, 0, 0);
        if (*reinterpret_cast<const int *>(&player->render.motionFinishThreshold) == 8) {
          player->SetState(0, 1);
        }
      }
      break;
    case ID_ZC_UPDATE_GDID_02f7: {
      const std::uint32_t flags = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA);
      g_guildInfo.canInvite = flags & 1;
      g_guildInfo.isMaster = g_gameModePacketBuffer[0xE];
      g_guildInfo.m_guildId = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2);
      g_guildInfo.canExpel = (flags >> 4) & 1;
      g_guildInfo.masterCharacterId = *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2B);
      this->scene->player->SetGuildInfo(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6));
      if (!g_guildInfo.skillsRequested) {
        g_guildInfo.skillsRequested = 1;
        g_modeMgr.gameMode->SendMsg(MM_REQ_GUILD_MENU, 3, 0, 0, 0);
      }
      if (!g_guildInfo.membersRequested) {
        g_guildInfo.membersRequested = 1;
        g_modeMgr.gameMode->SendMsg(MM_REQ_GUILD_MENU, 1, 0, 0, 0);
      }
      break;
    }
    case ID_ZC_SKILLINFO_DELETE:
      g_session.RemoveSkill(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2));
      break;
    case ID_ZC_SKILL_SELECT_REQUEST: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 8) >> 1;
      if (count == 0) {
        break;
      }
      if (UIWindow *window = g_windowMgr.EnsureWindow(WID_SPELLLISTWND)) {
        window->SendMsg(nullptr, UM_SETCHOOSE, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), 0, 0, 0);
        const std::uint16_t *skills = reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8);
        for (int i = 0; i < count; ++i) {
          window->SendMsg(nullptr, UM_ADDITEM, skills[i], 0, 0, 0);
        }
      }
      break;
    }
    case ID_ZC_SEEK_PARTY: {
      char message[256];
      memset(message, 0, sizeof(message));
      const char *job = g_session.GetJobName(*reinterpret_cast<const unsigned *>(g_gameModePacketBuffer + 0x1A), -1);
      sprintf(message, LookupMessage(MSI_SEEK_PARTY_SET), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E),
              reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), job);
      if (g_windowMgr.chatRoomWnd) {
        g_windowMgr.SendNotice(UIM_PUSH_INTO_CHATROOM, reinterpret_cast<std::intptr_t>(message), 0x222222, 0, 0, 0);
      } else if (!g_serviceOptions.Get(114) && !g_serviceOptions.Get(148)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF00, 1, 0, 0);
      }
      if (g_session.m_whisperRecord) {
        this->WriteChatLog(message);
      }
      break;
    }
    case ID_ZC_SKILLINFO_UPDATE2:
      this->ReceiveSkillUpdate(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), g_gameModePacketBuffer[0xE],
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8));
      break;
    case ID_ZC_MSG_SKILL: {
      char text[256];
      sprintf(text, LookupMessage(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4)));
      char message[256];
      sprintf(message, "[%s] %s", CSkillInfo::DisplayName(*g_session.m_hotkeyScript, *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)),
              text);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFCDCD, 11, 0, 0);
      break;
    }
    case ID_ZC_USESKILL_ACK2:
      this->ProcessSkillUsePacket(g_gameModePacketBuffer, 1);
      break;
    case ID_ZC_REASSEMBLY_AUTH01: {
      std::uint16_t packetType = 0x884;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH02: {
      std::uint16_t packetType = 0x885;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH03: {
      std::uint16_t packetType = 0x886;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH04: {
      std::uint16_t packetType = 0x887;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH05: {
      std::uint16_t packetType = 0x888;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH06:
      SendPacketHeader(0x889);
      break;
    case ID_ZC_REASSEMBLY_AUTH07: {
      std::uint16_t packetType = 0x88A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH08:
      SendPacketHeader2(0x88B);
      break;
    case ID_ZC_REASSEMBLY_AUTH09: {
      std::uint16_t packetType = 0x88C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH10:
      SendPacketHeader2(0x88D);
      break;
    case ID_ZC_REASSEMBLY_AUTH11: {
      std::uint16_t packetType = 0x88E;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH12: {
      std::uint16_t packetType = 0x88F;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH13: {
      std::uint16_t packetType = 0x890;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH14: {
      std::uint16_t packetType = 0x891;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH15: {
      std::uint16_t packetType = 0x892;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH16: {
      std::uint16_t packetType = 0x893;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH17: {
      std::uint16_t packetType = 0x894;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH18: {
      std::uint16_t packetType = 0x895;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH19: {
      std::uint16_t packetType = 0x896;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH20: {
      std::uint16_t packetType = 0x897;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH21: {
      std::uint16_t packetType = 0x898;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH22: {
      std::uint16_t packetType = 0x899;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH23: {
      std::uint16_t packetType = 0x89A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH24: {
      std::uint16_t packetType = 0x89B;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH25: {
      std::uint16_t packetType = 0x89C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH26: {
      std::uint16_t packetType = 0x89D;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH27: {
      std::uint16_t packetType = 0x89E;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH28: {
      std::uint16_t packetType = 0x89F;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH29: {
      std::uint16_t packetType = 0x8A0;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH30: {
      std::uint16_t packetType = 0x8A1;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH31:
      SendPacketHeader(0x8A2);
      break;
    case ID_ZC_REASSEMBLY_AUTH32: {
      std::uint16_t packetType = 0x8A3;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH33: {
      std::uint16_t packetType = 0x8A4;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH34:
      SendPacketHeader2(0x8A5);
      break;
    case ID_ZC_REASSEMBLY_AUTH35: {
      std::uint16_t packetType = 0x8A6;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH36: {
      std::uint16_t packetType = 0x8A7;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH37: {
      std::uint16_t packetType = 0x8A8;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH38: {
      std::uint16_t packetType = 0x8A9;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH39: {
      std::uint16_t packetType = 0x8AA;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH40: {
      std::uint16_t packetType = 0x8AB;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH41:
      SendPacketHeader(0x8AC);
      break;
    case ID_ZC_REASSEMBLY_AUTH42: {
      std::uint16_t packetType = 0x8AD;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH43:
      SendPacketHeader2(0x941);
      break;
    case ID_ZC_REASSEMBLY_AUTH44: {
      std::uint16_t packetType = 0x942;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH45: {
      std::uint16_t packetType = 0x943;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH46: {
      std::uint16_t packetType = 0x944;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH47:
      SendPacketHeader2(0x945);
      break;
    case ID_ZC_REASSEMBLY_AUTH48:
      SendPacketHeader(0x946);
      break;
    case ID_ZC_REASSEMBLY_AUTH49: {
      std::uint16_t packetType = 0x947;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH50: {
      std::uint16_t packetType = 0x948;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH51: {
      std::uint16_t packetType = 0x949;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH52: {
      std::uint16_t packetType = 0x94A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH53:
      SendPacketHeader(0x94B);
      break;
    case ID_ZC_REASSEMBLY_AUTH54: {
      std::uint16_t packetType = 0x94C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH55: {
      std::uint16_t packetType = 0x94D;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH56: {
      std::uint16_t packetType = 0x94E;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH57:
      SendPacketHeader2(0x94F);
      break;
    case ID_ZC_REASSEMBLY_AUTH58: {
      std::uint16_t packetType = 0x950;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH59: {
      std::uint16_t packetType = 0x951;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH60: {
      std::uint16_t packetType = 0x952;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH61: {
      std::uint16_t packetType = 0x953;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH62: {
      std::uint16_t packetType = 0x954;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH63:
      SendPacketHeader(0x955);
      break;
    case ID_ZC_REASSEMBLY_AUTH64: {
      std::uint16_t packetType = 0x956;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH65:
      SendPacketHeader2(0x957);
      break;
    case ID_ZC_REASSEMBLY_AUTH66: {
      std::uint16_t packetType = 0x958;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH67:
      SendPacketHeader(0x959);
      break;
    case ID_ZC_REASSEMBLY_AUTH68: {
      std::uint16_t packetType = 0x95A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH69:
      SendPacketHeader2(0x95B);
      break;
    case ID_ZC_REASSEMBLY_AUTH70: {
      std::uint16_t packetType = 0x95C;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH71:
      SendPacketHeader2(0x95D);
      break;
    case ID_ZC_REASSEMBLY_AUTH72:
      SendPacketHeader2(0x95E);
      break;
    case ID_ZC_REASSEMBLY_AUTH73: {
      std::uint16_t packetType = 0x95F;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH74: {
      std::uint16_t packetType = 0x960;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH75: {
      std::uint16_t packetType = 0x961;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH76: {
      std::uint16_t packetType = 0x962;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH77: {
      std::uint16_t packetType = 0x963;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH78:
      SendPacketHeader(0x964);
      break;
    case ID_ZC_REASSEMBLY_AUTH79:
      SendPacketHeader(0x965);
      break;
    case ID_ZC_REASSEMBLY_AUTH80: {
      std::uint16_t packetType = 0x966;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH81: {
      std::uint16_t packetType = 0x967;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH82: {
      std::uint16_t packetType = 0x968;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH83: {
      std::uint16_t packetType = 0x969;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case ID_ZC_REASSEMBLY_AUTH84: {
      std::uint16_t packetType = 0x96A;
      GetRagConnection().Send(sizeof(packetType), &packetType);
      break;
    }
    case package_item_announceType:
      this->ReceiveItemObtainBroadcast(g_gameModePacketBuffer, false);
      break;
    case ID_ZC_NOTIFY_HP_TO_GROUPM_R2: {
      if (!g_session.IsCompanionAid(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
          const int hp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
          const int maxHp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA);
          actor->SendMsg(nullptr, AM_UPDATEGAGE, hp, maxHp, 0, 0, 0, 0);
        }
        if (g_windowMgr.messengerGroupWnd) {
          g_windowMgr.messengerGroupWnd->SendMsg(nullptr, UM_COMMAND, 34, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2),
                                           *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                                           *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
        }
        g_windowMgr.NotifyItemWindows(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                                      *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
      }
      break;
    }
    case ID_ZC_ADD_EXCHANGE_ITEM2:
      ReceiveItemSlot(g_gameModePacketBuffer);
      break;
    case ID_ZC_SSILIST_ITEM_CLICK_ACK:
      g_session.storeSearchX = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2);
      g_session.storeSearchY = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 4);
      break;
    case ID_ZC_SE_CASHSHOP_OPEN:
    case ID_ZC_SE_CASHSHOP_OPEN2:
      if (!g_windowMgr.CloseWindow(WID_CASHSHOP_WND)) {
        if (UIWindow *window = g_windowMgr.EnsureWindow(WID_CASHSHOP_WND)) {
          window->SendMsg(nullptr, UM_SET_CASHSHOP_BALANCE, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6),
                          0, 0);
        } else {
          std::int16_t packetType = 0x844;
          GetRagConnection().Send(reinterpret_cast<const PacketLengths &>(GetRagConnection()).FixedLength(packetType), &packetType);
        }
      }
      break;
    case ID_ZC_ITEM_FALL_ENTRY4: {
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
      entry.type = *reinterpret_cast<const std::uint16_t *>(packet + 0x0A);

      CItem *item = this->scene->GetItemByAID(entry.aid);
      if (!item) {
        item = new CItem;
        this->scene->objects.push_back(item);
        this->scene->items.push_back(item);
      }

      const int entryAddress = reinterpret_cast<int>(&entry);
      item->SendMsg(nullptr, AM_INITSTANDENTRY, entryAddress, 0, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_NOTIFY_ACT3:
      this->ReceiveActorAction(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x16),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x1B), g_gameModePacketBuffer[0x1D],
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E), g_gameModePacketBuffer[0x1A]);
      break;
    case ID_ZC_STOPMOVE_FORCE: {
      const ActorStopPacket *packet = reinterpret_cast<const ActorStopPacket *>(g_gameModePacketBuffer);
      this->StopActorMove(packet->aid, packet->x, packet->y, 1);
      break;
    }
    case ID_ZC_REQ_WEAR_EQUIP_ACK2: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->equipmentRequestState[0x04]) = timeGetTime() - 2000;
      switch (packet[8]) {
      case ITEM_EQUIP_ACK_OK: {
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::uint16_t *>(packet + 2));
        if (item.identity[1]) {
          item.sprite = *reinterpret_cast<const std::uint16_t *>(packet + 6);
          item.identity[3] = *reinterpret_cast<const std::uint16_t *>(packet + 4);
          g_session.OnItemEquipped(item);
        }
        break;
      }
      case ITEM_EQUIP_ACK_FAILLEVEL:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CAN_NOT_EQUIP_ITEM_FORBID)), 0xFF, 7, 0, 0);
        break;
      }
      break;
    }
    case ID_ZC_REQ_TAKEOFF_EQUIP_ACK2: {
      const unsigned char *packet = g_gameModePacketBuffer;
      *reinterpret_cast<std::uint32_t *>(&this->equipmentRequestState[0x08]) = timeGetTime() - 2000;
      switch (packet[6]) {
      case 0: {
        const std::uint16_t location = *reinterpret_cast<const std::uint16_t *>(packet + 4);
        if (location) {
          if (g_session.IsCostumeLocation(location)) {
            g_session.UnequipCostumeItem(location);
          } else {
            g_session.UnequipItem(location, 0);
          }
        }
        break;
      }
      case 1:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CAN_NOT_EQUIP_ITEM_FORBID)), 0xFF, 7, 0, 0);
        break;
      }
      break;
    }
    case ID_ZC_SE_CASHSHOP_UPDATE:
      if (UIWindow *window = g_windowMgr.LookupWindow(WID_CASHSHOP_WND)) {
        window->SendMsg(nullptr, UM_UPDATE_CASHSHOP_BALANCE, *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), 0,
                        0);
      }
      break;
    case bgqueue_ackType:
      g_entryQueueMgr->OnApplyResult(reinterpret_cast<const EntryQueueApplyResult *>(g_gameModePacketBuffer));
      break;
    case bgqueue_updateinfoType:
      g_entryQueueMgr->OnStatus(reinterpret_cast<const EntryQueueStatus *>(g_gameModePacketBuffer));
      break;
    case bgqueue_notice_deleteType:
      g_entryQueueMgr->OnCancelResult(reinterpret_cast<const EntryQueueNotice *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_NOTIFY_ENTRY_QUEUE_ADMISSION:
      g_entryQueueMgr->IgnorePacket(g_gameModePacketBuffer);
      break;
    case ID_ZC_REPLY_ACK_ENTRY_QUEUE_ADMISSION:
      g_entryQueueMgr->OnResultNotice(reinterpret_cast<const EntryQueueNotice *>(g_gameModePacketBuffer));
      break;
    case bgqueue_battlebeginsType:
      g_entryQueueMgr->OnReady(reinterpret_cast<const EntryQueueReady *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_REPLY_ACK_LOBBY_ADMISSION:
      g_entryQueueMgr->OnNotify(reinterpret_cast<const EntryQueueNotice *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_NAVIGATION_ACTIVE:
      ReceiveNavigation(g_gameModePacketBuffer);
      break;
    case ID_ZC_ENTRY_QUEUE_INIT:
      g_entryQueueMgr->ResetCurrent(reinterpret_cast<int>(g_gameModePacketBuffer));
      break;
    case ID_ZC_ACK_TAKEOFF_EQUIP_V5:
      this->ReceiveTakeOffEquipResult(g_gameModePacketBuffer[0x8], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4),
                                      *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ACK_OPEN_GUILD_STORAGE:
      ReceiveBuyingStoreResult(reinterpret_cast<const BuyingStoreResultPacket *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_ACK_CLOSE_GUILD_STORAGE:
      ReceiveStorageClose(reinterpret_cast<const AchievementStatePacket *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_NPROTECTGAMEGUARDCSAUTH:
      g_nProtectGameGuardMgr->ProcessAuth(reinterpret_cast<int>(g_gameModePacketBuffer + 0x4),
                                          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2) - 4);
      break;
    case ID_ZC_ACK_WHISPER02:
      this->ReceiveWhisperResult(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_UPDATE_ITEM_FROM_BUYING_STORE2:
      ReceiveBuyingStorePurchase(g_gameModePacketBuffer);
      break;
    case ID_ZC_PET_EVOLUTION_RESULT:
      this->NotifyPetEvolutionResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ADD_EXCHANGE_ITEM3:
      ReceiveItemSlotOptions(g_gameModePacketBuffer);
      break;
    case achievementRewardAckType:
      this->ReceiveAchievementResult(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_RECOVERY2:
      this->ReceiveHpSpRecovery(g_gameModePacketBuffer, 1);
      break;
    case ID_ZC_REQ_AU_BOT:
      this->ReceiveAntiBotChallenge(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ACK_CHANGE_TITLE:
      this->ReceiveJobTitle(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_UI_OPEN:
      this->OpenServiceWindow(g_gameModePacketBuffer[0x2]);
      break;
    case ID_ZC_SKILL_POSTDELAY2:
      this->UpdateSkillCooldown(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x4),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x8));
      break;
    case ID_ZC_ACK_PRIVATE_AIRPLANE_CALL:
      this->NotifyPrivateAirplaneResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_MAPMOVE_PRIVATE_AIRPLANE:
      this->RequestLocalMapMove(reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x2),
                                *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x12),
                                *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x14));
      break;
    case ID_ZC_RANDOM_COMBINE_ITEM_UI_OPEN:
      this->ReceiveLapineBoxIngredient(reinterpret_cast<const unsigned char *>(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2)));
      break;
    case ID_ZC_ACK_RANDOM_COMBINE_ITEM:
      this->ReceiveLapineBoxResult(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ACK_UPLOAD_MACRO_DETECTOR:
      this->ReceiveCaptchaUploadRequest(reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2),
                                        *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case ID_ZC_COMPLETE_UPLOAD_MACRO_DETECTOR_CAPTCHA:
      this->NotifyCaptchaRegistered();
      break;
    case ID_ZC_ACK_APPLY_MACRO_DETECTOR:
      this->NotifyCaptchaRegistrationResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_APPLY_MACRO_DETECTOR:
      this->BeginCaptchaImage(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2),
                              reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x4));
      break;
    case ID_ZC_REQ_ANSWER_MACRO_DETECTOR:
      this->SetMacroDetectorCountdown(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_CLOSE_MACRO_DETECTOR:
      this->ReceiveMacroDetectorResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_MACRO_CHECK_REMOVE_CHEAT:
      this->NotifyMacroCheckRemovalResult(g_gameModePacketBuffer[0x2]);
      break;
    case ID_ZC_ACK_PREVIEW_MACRO_DETECTOR:
      this->ReceiveCaptchaPreview(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2),
                                  *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x6),
                                  reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x8));
      break;
    case ID_ZC_ACK_PLAYER_AID_IN_RANGE:
      ReceiveMacroReporterList((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 2),
                 reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4));
      break;
    case ID_ZC_MSG_VALUE_STRING:
      this->ShowFormattedNotice(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x4),
                                *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x6), reinterpret_cast<int>(g_gameModePacketBuffer + 0xA));
      break;
    case ID_ZC_ACK_CREATE_ASSISTANT:
      this->ReceiveStoreResult(g_gameModePacketBuffer[0x2], g_gameModePacketBuffer[0x3]);
      break;
    case ID_ZC_STORE_ASSISTANT_DISAPPEAR:
      this->ReceiveItemDisappear(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_OPEN_UI_ASSISTANT_BUYING:
      this->OpenBuyingStoreMake(g_gameModePacketBuffer[0x2]);
      break;
    case ID_ZC_CONFIG_NOTIFY2:
      this->ReceiveFeedConfig(g_gameModePacketBuffer[0x2], g_gameModePacketBuffer[0x3]);
      break;
    case ID_ZC_OPEN_REFINING_UI:
      g_windowMgr.EnsureWindow(WID_REFININGWND);
      break;
    case ID_ZC_RANDOM_UPGRADE_ITEM_UI_OPEN:
      this->ReceiveLapineUpgradeBoxValue(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ACK_RANDOM_UPGRADE_ITEM:
      this->ReceiveLapineUpgradeResult(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ACK_FAILED_ALL_RODEX_LIST:
      this->IgnoreRemovedPacket6();
      break;
    case ID_ZC_UI_OPEN_V2:
      this->ReceiveServiceRequest(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_ACK_CHECK_ATTENDANCE:
      this->ReceiveWindow328Result(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case ID_ZC_SET_CLAIRVOYANCE:
      SetActiveSkillEffect184(g_gameModePacketBuffer[0x2]);
      break;
    case ID_ZC_PARTY_JOIN_REQ_ACK_FROM_MASTER:
      ReceivePartyJoinResult(reinterpret_cast<const void *>(g_gameModePacketBuffer));
      break;
    case ID_ZC_AUTOSPELLLIST2:
      this->ReceiveWindow99Values((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 2),
                                  reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4));
      break;
    case ID_ZC_SPLIT_SEND_ITEMLIST_RESULT:
      this->RefreshItemListWindow(g_gameModePacketBuffer[0x2], g_gameModePacketBuffer[0x3]);
      break;
    case ID_ZC_CHANGE_AGIT_OWNER:
      this->ReceiveGuildAgitOwnershipChange(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x3),
                                          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x7));
      break;
    case ID_ZC_REQ_ACK_AGIT_INVESTMENT:
      this->ReceiveAgitNotice(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3),
                              *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x7));
      break;
    case ID_ZC_REQ_ACK_MOVE_GUILD_AGIT:
      this->ReceiveGuildAgitMoveResult(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_REMOVE_NEIGHBORHOOD_CHARACTERS:
      this->ReceiveObjectsDisappear((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 2),
                                    reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x4));
      break;
    case ID_ZC_ACK_ADD_NEW_EMBLEM:
      this->ReceiveGuildEmblemVersion(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA),
                                      *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2),
                                      *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case ID_ZC_ADD_ACCOUNT_LIMTIED_SALE_ITEM:
      this->ReceiveWindow154Result(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_DEL_ACCOUNT_LIMTIED_SALE_ITEM:
      this->ReceiveWindow154Close(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_OPEN_ACCOUNT_LIMTIED_SALE_TOOL:
      this->OpenWindow154();
      break;
    case ID_ZC_CLOSE_ACCOUNT_LIMTIED_SALE_TOOL:
      CloseAccountLimitedTool();
      break;
    case ID_ZC_SEARCH_ACCOUNT_LIMTIED_SALE_ITEM:
      this->ReceiveAccountLimitedToolApplyResult(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2),
                                 *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x4));
      break;
    case ID_ZC_TARGET_SPIRITS:
      this->ReceiveActorStateValue(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2),
                                   *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                                   *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA));
      break;
    case ID_ZC_SE_CASHSHOP_OPEN_V2:
      this->ReceiveCashShopBanner(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                                  *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
      break;
    case ID_ZC_UI_OPEN_REPUTE:
      this->ReceiveReputeOpen(*reinterpret_cast<const std::uint64_t *>(g_gameModePacketBuffer + 0x2),
                              *reinterpret_cast<const std::uint64_t *>(g_gameModePacketBuffer + 0xA));
      break;
    case ID_ZC_OPEN_REFORM_UI:
      this->ReceiveItemReformOpen(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ITEM_REFORM_ACK:
      this->ReceiveWindow348Result(g_gameModePacketBuffer[0x4], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_SET_USERCONFIG_ACK:
      this->ReceiveGameOptionResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                                    *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA));
      break;
    case ID_ZC_GET_USERCONFIG_LIST:
      this->ReceiveGameOptions((static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2) - 4) >> 3),
                               reinterpret_cast<const GameOptionEntry *>(g_gameModePacketBuffer + 0x4));
      break;
    case ID_ZC_RODEX_RETURN_ACK:
      this->ReceiveRodexMailDeleteResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2),
                                         *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case ID_ZC_UI_OPEN_V3:
      this->ReceiveUiOpen(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint64_t *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_RESPONSE_ENCHANT:
      this->ReceiveEnchantResult(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case ID_ZC_DIALOG_TEXT_ALIGN:
      this->SetWindow16Align(g_gameModePacketBuffer[0x2]);
      break;
    case ID_ZC_DIALOG_WINDOW_SIZE:
      this->ResizeWindow16(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_DIALOG_WINDOW_POS:
      this->MoveWindow16(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6));
      break;
    case ID_ZC_BATTLEFIELD_NOTIFY_HPSP:
      this->RejectRequest(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6),
                          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
                          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12));
      break;
    case ID_ZC_NOTIFY_HPSP_TO_GROUPM_R2:
      this->IgnoreRemovedPacket12(reinterpret_cast<const unsigned char *>(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2)),
                                  *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x6), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA),
                                  *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12));
      break;
    case ID_ZC_ACK_TAKEOFF_EQUIP_ALL:
      this->ReceiveBasicInfoResult(g_gameModePacketBuffer[0x2]);
      break;
    case ID_ZC_DIALOG_WINDOW_POS2:
      this->PlaceWindow16(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x2),
                          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x6));
      break;
    case ID_ZC_BROADCASTING_SPECIAL_ITEM_OBTAIN_WITH_REFINE_LEVEL:
      this->ReceiveItemObtainBroadcast(g_gameModePacketBuffer, 1);
      break;
    case ID_ZC_REQ_LISTOFMODIFIED_ITEMNSKILL:
      this->ReceiveShopName(reinterpret_cast<const char *>(g_gameModePacketBuffer + 0x2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12));
      break;
    case ID_ZC_SPECIALPOPUP:
      this->ShowSpecialPopup(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_ACK_MACRO_BLACKLIST_USER_HUMAN_CHECK:
      this->NotifyMacroBlacklistCheckResult(g_gameModePacketBuffer[0x2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_REQ_ANSWER_MACROBLUSER_HUMAN_CHECK:
      this->ActivateMacroBlackListCheck(g_gameModePacketBuffer[0x2], *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3));
      break;
    case ID_ZC_CLOSE_MACROBLUSER_HUMAN_CHECK:
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
    case ID_ZC_GM_CHECKER:
      this->NotifyGmCheckerResult(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2));
      break;
    case 0xC3C:
      this->ReceiveToggle128(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x2));
      break;
    case ID_ZC_NPC_MARKETGEAR_OPEN:
      ReceiveTradeItems(static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(TradeItemRecord),
                        reinterpret_cast<const TradeItemRecord *>(g_gameModePacketBuffer + 4));
      break;
    case ID_ZC_APPLY_MACRO_DETECTOR_CAPTCHA:
      this->ReceiveCaptchaImageData(reinterpret_cast<const char *>(g_gameModePacketBuffer + 4), g_gameModePacketBuffer + 8,
                                    static_cast<std::uint16_t>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 8));
      break;
    case ID_ZC_PREVIEW_MACRO_DETECTOR_CAPTCHA:
      this->ReceiveCaptchaPreviewImage(reinterpret_cast<const char *>(g_gameModePacketBuffer + 4), g_gameModePacketBuffer + 8,
                                       static_cast<std::uint16_t>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 8));
      break;
    case 0xBF4:
      this->ReceiveQuestStates(static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 5,
                               g_gameModePacketBuffer + 4);
      break;
    case ID_ZC_OPEN_UI_ASSISTANT_VENDING:
      this->OpenVendingMake(g_gameModePacketBuffer[4], static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) >> 1,
                            reinterpret_cast<const short *>(g_gameModePacketBuffer + 5));
      break;
    case ID_ZC_CONFIG_NOTIFY3:
      this->ReceiveFeedConfig2(g_gameModePacketBuffer[2], g_gameModePacketBuffer[3], g_gameModePacketBuffer[4]);
      break;
    case ID_ZC_REPUTE_INFO:
      this->ReceiveReputeInfo(g_gameModePacketBuffer[4],
                              static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) >> 4,
                              reinterpret_cast<const ReputePointEntry *>(g_gameModePacketBuffer + 5));
      break;
    case ID_ZC_ADVANCED_STATUS_CHANGE_ACK:
      this->ReceiveTraitStatusChangeAck(g_gameModePacketBuffer[4], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                        g_gameModePacketBuffer[5]);
      break;
    case ID_ZC_CHECK_RECEIVE_CHARACTER_NAME2:
      this->ReceiveRodexRecipient(
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8), reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA));
      break;
    case ID_ZC_BROADCAST_ITEMREFINING_RESULT:
      this->ReceiveRefineStatus(reinterpret_cast<const char *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1A),
                                g_gameModePacketBuffer[0x1E], g_gameModePacketBuffer[0x1F]);
      break;
    case 0xBD5:
      this->ApplyRuneSetResultWithRequest(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3),
                                          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 5), -1, -1);
      break;
    case ID_ZC_RODEX_BLOCK_RECEIVE:
      this->ShowNoticeText(reinterpret_cast<const char *>(g_gameModePacketBuffer + 2));
      break;
    case 0xBCF:
      this->ReceiveRuneGradeResult(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3), 1,
                                   *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 5), 0, 0);
      break;
    case ID_ZC_ACK_WEAR_EQUIP_V5:
      this->ReceiveWearEquipAck(g_gameModePacketBuffer[0xA], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
                                *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4));
      break;
    case ID_ZC_SPLIT_SEND_ITEMLIST_NORMAL:
      this->ReceiveItemListPart(g_gameModePacketBuffer[4],
                                static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) / 34,
                                g_gameModePacketBuffer + 5);
      break;
    case ID_ZC_SPLIT_SEND_ITEMLIST_EQUIP:
      this->ReceiveEquipListPart(g_gameModePacketBuffer[4],
                                 static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) / 67,
                                 g_gameModePacketBuffer + 5);
      break;
    case ID_ZC_NPC_MARKETGEAR_PURCHASE_RESULT:
      this->ReceiveParaItemResult(g_gameModePacketBuffer[4],
                                  static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 5) / 29,
                                  g_gameModePacketBuffer + 5);
      break;
    case ID_ZC_DELETEITEM_FROM_MCSTORE2:
      ProcessVendingSale(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                 *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
                 *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6));
      break;
    case 0xC17:
      this->ReceiveRuneRewardResult(g_gameModePacketBuffer[2], *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 3),
                                    *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 5), g_gameModePacketBuffer[9]);
      break;
    case ID_ZC_CONFIG_NOTIFY4:
      this->ReceiveFeedConfig3(g_gameModePacketBuffer[2], g_gameModePacketBuffer[3], g_gameModePacketBuffer[4], g_gameModePacketBuffer[5]);
      break;
    case ID_ZC_USE_SKILL2:
      this->ReceiveSkillUse(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
                            *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xC),
                            *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 8), g_gameModePacketBuffer[0x10]);
      break;
    case achievementListType:
      ReceiveAchievementList(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 8),
                 *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xE),
                 *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x12),
                 reinterpret_cast<const AchievementRecordPacket *>(g_gameModePacketBuffer + 0x16));
      break;
    case ID_CR_NEW_EMBLEM_DOWNLOAD:
      CGuildMgr::ReceiveEmblem(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
                               *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6),
                               *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xA), g_gameModePacketBuffer + 0xE,
                               *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 14);
      break;
    case ID_ZC_SERVERMOVE_PRIVATE_AIRPLANE:
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
    case ID_ZC_REFINING_MATERIAL_LIST:
      this->ApplyRefiningMaterial(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
                                  static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 7) / 9,
                                  g_gameModePacketBuffer + 7, g_gameModePacketBuffer[6]);
      break;
    case 0xBE3:
      this->ReceiveNamedResult(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), reinterpret_cast<const char *>(g_gameModePacketBuffer + 6),
                               *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x1E));
      break;
    case skillscale:
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
    case ID_ZC_WAIT_DIALOG2:
      this->isOnQuest = 1;
      this->lastNaid = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 2);
      g_windowMgr.EnsureWindow(WID_SAYDIALOGWND_OVERLAY)->SendMsg(nullptr, UM_ENABLENEXTBUTTON, 0, 0, 0, 0);
      break;
    case 0xBEE:
      switch (g_gameModePacketBuffer[4]) {
      case 0:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_NOT_ENOUGH_NYANGVINE")), 0xFF, 0, 0, 0);
        break;
      case 1:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_DATE")), 0xFF, 0, 0, 0);
        this->RefreshEmotionSaleSchedule();
        break;
      case 2:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_ALREADY_BUY")), 0xFF, 0, 0, 0);
        break;
      case 3:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_ANOTHER_SALE_BUY")), 0xFF, 0, 0, 0);
        break;
      case 4:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_NOT_ENOUGH_BASICSKILL_LEVEL")), 0xFF, 0,
                               0, 0);
        break;
      case 5:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_NOT_YET_SALE_START_TIME")), 0xFF, 0, 0, 0);
        this->RefreshEmotionSaleSchedule();
        break;
      default:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_FAIL_UNKNOWN")), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_PLAY_NPC_BGM2: {
      const unsigned char type = g_gameModePacketBuffer[4];
      this->PlayBgm(
          type, std::string(reinterpret_cast<const char *>(g_gameModePacketBuffer + 5), *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2)));
      break;
    }
    case ID_ZC_ITEMLIST_FROM_ASSISTANT_VENDING:
      this->ReceiveVendingItemList2(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 8),
                                    g_gameModePacketBuffer[0xC],
                                    static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 0x11) / 63,
                                    *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0xD), g_gameModePacketBuffer + 0x11);
      break;
    case achievementUpdateType:
      this->ReceiveAchievementUpdate(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 2),
                                     *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6),
                                     *reinterpret_cast<const int *>(g_gameModePacketBuffer + 8), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xC),
                                     *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x10), g_gameModePacketBuffer[0x14],
                                     reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x15),
                                     *reinterpret_cast<const __time32_t *>(g_gameModePacketBuffer + 0x3D), g_gameModePacketBuffer[0x41]);
      break;
    case ID_ZC_SKILLINFO_UPDATE3:
      this->ReceiveSkillUpdate(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), g_gameModePacketBuffer[0xE],
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xF));
      break;
    case ID_ZC_ADD_SKILL3:
      this->ReceiveLearnedSkill(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), g_gameModePacketBuffer[0xE],
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xA), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xC),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0xF));
      break;
    case ID_ZC_ITEMLIST_FROM_ASSISTANT_BUYING:
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
    case progressbarunit:
      if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        actor->SendMsg(nullptr, AM_MAKE_SKILL_RECHARGEGAGE, static_cast<std::uint32_t>(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA) * 1000), 0, 0, 0, 0, 0);
      }
      break;
    case ID_ZC_PARTY_REQ_MASTER_TO_JOIN: {
      char name[25];
      strncpy(name, reinterpret_cast<const char *>(g_gameModePacketBuffer + 0xA), 24);
      name[24] = 0;
      g_requestJoinPartyMgr->Add(
          *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2), name,
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x22), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x24));
      break;
    }
    case ID_ZC_CHANGE_ITEM_OPTION:
      this->ReceiveItemUpgrade(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const ItemCardSlots *>(g_gameModePacketBuffer + 6), *reinterpret_cast<const ItemSlotOptions *>(g_gameModePacketBuffer + 0x16));
      break;
    case ID_ZC_CHANGE_ITEM_OPTION2:
      this->RefreshInventoryOrEquipItem(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), g_gameModePacketBuffer[4],
                                        *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 5),
                                        *reinterpret_cast<const ItemCardSlots *>(g_gameModePacketBuffer + 7),
                                        *reinterpret_cast<const ItemSlotOptions *>(g_gameModePacketBuffer + 0x17));
      break;
    case 0xBED:
      g_cashEmotionMgr->AcquirePack(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2),
                                    g_gameModePacketBuffer[4] ? *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 5) : 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_BUY_SUCCESS")), 0xFFFF, 0, 0, 0);
      this->RefreshEmotionPurchaseSchedule();
      break;
    case ID_ZC_BATTLEFIELD_NOTIFY_HP2:
      if (!g_session.IsCompanionAid(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
        if (CRenderObject *actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 2))) {
          const int hp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6);
          const int maxHp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0xA);
          actor->SendMsg(nullptr, AM_UPDATEGAGE, hp, maxHp, 0, 0, 0, 0);
        }
      }
      break;
    case ID_ZC_ITEM_PICKUP_ACK_V5:
      this->ReceiveItemPickupAck5(
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 4),
          *reinterpret_cast<const int *>(g_gameModePacketBuffer + 6), g_gameModePacketBuffer[0xA], g_gameModePacketBuffer[0xB], g_gameModePacketBuffer[0xC],
          *reinterpret_cast<const ItemCardSlots *>(g_gameModePacketBuffer + 0xD), *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x1D),
          g_gameModePacketBuffer[0x21], g_gameModePacketBuffer[0x22], *reinterpret_cast<const std::uint32_t *>(g_gameModePacketBuffer + 0x23),
          *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 0x27));
      break;
    case 0xC08:
      this->ReceiveNpcDialogEnd(g_gameModePacketBuffer);
      this->miniMapTooltipId = -1;
      this->miniMapTooltipText.assign("", 0);
      g_windowMgr.CloseWindow(WID_ILLUSTWND);
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
          this->requestTimers[0x16] = 0;
          break;
        }
      }
      const int first = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 6);
      const int second = *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8);
      actor->SendMsg(nullptr, AM_CASH_EMOTION, first, second, 0, 0, 0, 0);
      break;
    }
    case 0xBEB:
      switch (g_gameModePacketBuffer[6]) {
      case 0:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_USE_FAIL_DATE")), 0xFF, 0, 0, 0);
        g_cashEmotionMgr->RemoveOwnedPack(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2), true, true);
        this->RefreshEmotionPurchaseSchedule();
        break;
      case 1:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_USE_FAIL_UNPURCHASED")), 0xFF, 0, 0, 0);
        break;
      case 2:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_USE_FAIL_SKILL_LEVEL")), 0xFF, 0, 0, 0);
        break;
      default:
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_USE_FAIL_UNKNOWN")), 0xFF, 0, 0, 0);
        break;
      }
      this->requestTimers[0x16] = 0;
      break;
    case ID_ZC_SAY_DIALOG2: {
      const unsigned char *packet = g_gameModePacketBuffer;
      this->isOnQuest = 1;
      this->lastNaid = *reinterpret_cast<const int *>(packet + 4);
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
      g_windowMgr.EnsureWindow(WID_SAYDIALOGWND_OVERLAY)->SendMsg(nullptr, UM_SETSAYDIALOG, reinterpret_cast<std::intptr_t>(text), *reinterpret_cast<const int *>(packet + 4), 0, 0);
      break;
    }
    case 0xBF6: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 10) / 7;
      CCashEmotionMgr *manager = g_cashEmotionMgr;
      if (!manager) {
        break;
      }
      manager->SetServerTime(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4));
      manager->SetServerTimeZone(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 8));
      const unsigned char *entry = g_gameModePacketBuffer + 0xA;
      for (int i = 0; i < count; ++i, entry += 7) {
        manager->SetOwnedPack(*reinterpret_cast<const std::uint16_t *>(entry), entry[2] ? *reinterpret_cast<const std::uint32_t *>(entry + 3) : 0);
      }
      this->RefreshEmotionSaleSchedule();
      this->RefreshEmotionPurchaseSchedule();
      break;
    }
    case ID_ZC_STORE_ASSISTANT_ENTRY:
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
    case ID_ZC_STORE_ASSISTANT_ENTRY_V2:
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
    case ID_ZC_SKILLINFO_LIST3: {
      std::vector<SkillListEntry> skills;
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(SkillListEntry);
      const SkillListEntry *record = reinterpret_cast<const SkillListEntry *>(g_gameModePacketBuffer + 4);
      for (int i = 0; i < count; ++i, ++record) {
        skills.push_back(*record);
      }
      this->ReceiveSkillList(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer), skills);
      break;
    }
    case ID_ZC_VIEW_CAMERAINFO:
      if (g_gameModePacketBuffer[2] == 1) {
        UIWindow *window = g_windowMgr.LookupWindow(WID_VIEWCAMERAINFOWND);
        if (!window) {
          window = g_windowMgr.EnsureWindow(WID_VIEWCAMERAINFOWND);
        }
        g_session.showCameraInfo = !g_session.showCameraInfo;
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_session.showCameraInfo == true ? "Camera Info On" : "Camera Info Off"), 0x64FFFF, 0, 0, 0);
        window->Show(g_session.showCameraInfo);
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
        skill->SendMsg(nullptr, AM_SETMASTERGID, static_cast<std::uint32_t>(*reinterpret_cast<const int *>(g_gameModePacketBuffer + 4)), 0, 0, 0, 0, 0);
        const int address = reinterpret_cast<int>(g_gameModePacketBuffer + 0x1B);
        const int x = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x10);
        const int y = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x12);
        skill->SendMsg(nullptr, AM_SET_SKILL, address, x, y, 0, 0, 0);
        this->scene->objects.push_back(skill);
        this->scene->skills.push_back(skill);
      }
      break;
    case ID_ZC_DEBUGMSG: {
      std::string text(reinterpret_cast<const char *>(g_gameModePacketBuffer + 8),
                       reinterpret_cast<const char *>(g_gameModePacketBuffer + *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), *reinterpret_cast<const int *>(g_gameModePacketBuffer + 4), 0, 0, 0);
      break;
    }
    case ID_ZC_ACK_OPENSTORE2:
      switch (g_gameModePacketBuffer[2]) {
      case OPENSTORE2_SUCCESS:
        if (UIWindow *window = g_windowMgr.EnsureWindow(WID_MERCHANTITEMMYSHOPWND)) {
          window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
        if (UIMerchantShopMakeWnd *window = static_cast<UIMerchantShopMakeWnd *>(g_windowMgr.EnsureWindow(WID_MERCHANTSHOPMAKEWND))) {
          window->PrintSellList();
        }
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
        this->isOnQuest = 1;
        break;
      case OPENSTORE2_FAILED:
        this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND);
        g_session.ClearItems1760And1748();
        g_session.ClearMerchantPurchaseItems();
        g_windowMgr.Confirm(LookupMessage(MSI_SOUNDEFFECT_ITEMSELLWND), 0, true, false, 0, "Message", 280, 120, 0);
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
        break;
      case OPENSTORE2_NOVENDING:
        this->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND);
        g_session.ClearItems1760And1748();
        g_session.ClearMerchantPurchaseItems();
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CONNECT_SNS_FAIL_SEND)), 0xFF, 0, 0, 0);
        break;
      }
      break;
    case ID_ZC_NORMAL_SYSTEM_MSG: {
      std::string text(reinterpret_cast<const char *>(g_gameModePacketBuffer + 4),
                       reinterpret_cast<const char *>(g_gameModePacketBuffer + *reinterpret_cast<const std::uint16_t *>(g_gameModePacketBuffer + 2)));
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    case questUpdateType2: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 12;
      const unsigned char *entry = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, entry += 12) {
        const RenewQuestOngoingInfo &quest = g_renewQuestMgr->GetOngoingQuest(*reinterpret_cast<const unsigned int *>(entry));
        if (quest.m_title.empty()) {
          continue;
        }

        RenewQuestOngoingInfo info = quest;
        for (int index = i; index < 3; ++index) {
          if (*reinterpret_cast<const int *>(entry + 4) == info.m_huntMobId[index]) {
            info.m_huntKillCount[index] = *reinterpret_cast<const std::int16_t *>(entry + 0xA);
            info.m_huntMaxCount[index] = *reinterpret_cast<const std::int16_t *>(entry + 8);
            g_renewQuestMgr->SetOngoingQuest(*reinterpret_cast<const unsigned int *>(entry), info);
            break;
          }
        }
      }
      if (g_windowMgr.questDisplayWnd) {
        g_windowMgr.questDisplayWnd->Invalidate();
      }
      g_renewQuestMgr->UpdateOngoingQuestInfo();
      break;
    }
    case ID_ZC_INVENTORY_ITEMLIST_NORMAL: {
      g_session.KeepEquipment();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedItemRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedItemRecord *record = reinterpret_cast<const FlaggedItemRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.damaged = 0;
        item.identified = record->flags & 1;
        item.refiningLevel = 0;
        item.expireTime = record->expireTime;
        item.flags[0] = (record->flags >> 1) & 1;
        g_session.UpdateInventoryItem(item, 0);
      }
      RefreshItemWindows(&g_windowMgr);
      break;
    }
    case ID_ZC_CART_ITEMLIST_NORMAL: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedItemRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedItemRecord *record = reinterpret_cast<const FlaggedItemRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.damaged = 0;
        item.identified = record->flags & 1;
        item.refiningLevel = 0;
        item.expireTime = record->expireTime;
        item.flags[0] = (record->flags >> 1) & 1;
        g_session.AddMerchantShopItem(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case ID_ZC_STORE_ITEMLIST_NORMAL: {
      g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedItemRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedItemRecord *record = reinterpret_cast<const FlaggedItemRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = 0;
        item.identity[1] = record->index;
        item.identity[3] = record->location;
        item.quantity = record->amount;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.damaged = 0;
        item.identified = record->flags & 1;
        item.refiningLevel = 0;
        item.expireTime = record->expireTime;
        item.flags[0] = (record->flags >> 1) & 1;
        g_session.AddCartItem(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case ID_ZC_CART_ITEMLIST_EQUIP: {
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedEquipRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedEquipRecord *record = reinterpret_cast<const FlaggedEquipRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = record->location;
        item.identity[1] = record->index;
        item.identity[3] = record->wearState;
        item.quantity = 1;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->flags & 1;
        item.damaged = (record->flags >> 1) & 1;
        item.refiningLevel = record->refine;
        item.expireTime = record->expireTime;
        item.bindOnEquip = record->bindOnEquip;
        item.sprite = record->look;
        item.flags[0] = (record->flags >> 2) & 1;
        g_session.AddMerchantShopItem(item);
      }
      g_session.RefreshItem1740Window();
      break;
    }
    case ID_ZC_STORE_ITEMLIST_EQUIP: {
      g_windowMgr.EnsureWindow(WID_ITEMSTOREWND);
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / sizeof(FlaggedEquipRecord);
      for (int i = 0; i < count; ++i) {
        const FlaggedEquipRecord *record = reinterpret_cast<const FlaggedEquipRecord *>(g_gameModePacketBuffer + 4) + i;
        ITEM_INFO item;
        item.identity[0] = record->type;
        item.identity[2] = record->location;
        item.identity[1] = record->index;
        item.identity[3] = record->wearState;
        item.quantity = 1;
        for (int card = 0; card < 4; ++card) {
          reinterpret_cast<int *>(item.cards)[card] = record->cards[card];
        }
        item.SetItemId(record->itemId);
        item.identified = record->flags & 1;
        item.damaged = (record->flags >> 1) & 1;
        item.refiningLevel = record->refine;
        item.expireTime = record->expireTime;
        item.bindOnEquip = record->bindOnEquip;
        item.sprite = record->look;
        item.flags[0] = (record->flags >> 2) & 1;
        g_session.AddCartItem(item, 0);
      }
      g_session.RefreshItem1738Window();
      break;
    }
    case ID_ZC_PROPERTY_HOMUN_NO_ACCESSORY2:
      StringFormat(reinterpret_cast<char *>(g_session.spAndHomunculusStats + 0xC), 32, static_cast<std::size_t>(-1), "%s",
                   reinterpret_cast<const char *>(g_gameModePacketBuffer + 2));
      g_session.homunNameModified = g_gameModePacketBuffer[0x1A];
      *reinterpret_cast<int *>(g_session.homunLevel) = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1B);
      g_session.homunHunger = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D);
      g_session.RefreshHomunculusInfo();
      switch (*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D)) {
      case 1:
      case 5:
      case 10:
        if (g_session.m_homunHungerNotified != *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D)) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_HOMUN_HUNGRY)), 0xFF, 0, 0, 0);
          g_session.m_homunHungerNotified = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1D);
        }
        break;
      }
      g_session.homunFriendly = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x1F);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x2C)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x21);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x30)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x23);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x34)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x25);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x38)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x27);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x3C)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x29);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x40)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2B);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x44)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2D);
      reinterpret_cast<int *>(g_session.spAndHomunculusStats + 0x48)[0] = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x2F);
      g_session.m_homunHp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x31);
      g_session.RefreshHomunculusInfo();
      *reinterpret_cast<int *>(g_session.homunMaxHp) = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x35);
      g_session.RefreshHomunculusInfo();
      g_session.m_homunSp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x39);
      g_session.RefreshHomunculusInfo();
      g_session.homunMaxSp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x3D);
      g_session.RefreshHomunculusInfo();
      g_session.m_homunExp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x41);
      g_session.RefreshHomunculusInfo();
      g_session.m_homunMaxExp = *reinterpret_cast<const int *>(g_gameModePacketBuffer + 0x45);
      g_session.RefreshHomunculusInfo();
      *reinterpret_cast<int *>(g_session.companionSkillPoints) = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x49);
      g_session.InvalidateWindowFrames();
      if (g_windowMgr.homunculusSkillWnd) {
        g_windowMgr.homunculusSkillWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      g_session.m_homunAttackRange = *reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 0x4B);
      break;
    case ID_ZC_NPC_BARTER_MARKET_ITEMINFO: {
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
      g_windowMgr.EnsureWindow(WID_BARTERMARKETWND)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      g_windowMgr.EnsureWindow(WID_BARTERITEMPURCHASEWND)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      break;
    }
    case ID_ZC_NPC_MARKET_OPEN: {
      g_session.ClearTradeItems();
      const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t *>(g_gameModePacketBuffer + 2) - 4) / 15;
      const unsigned char *record = g_gameModePacketBuffer + 4;
      for (int i = 0; i < count; ++i, record += 15) {
        unsigned char entry[15];
        memcpy(entry, record, sizeof(entry));
        RawItemBasic item = {};
        _itoa(*reinterpret_cast<const int *>(entry), item.itemId, 10);
        item.price = *reinterpret_cast<const std::uint32_t *>(entry + 5);
        item.realPrice = *reinterpret_cast<const std::uint32_t *>(entry + 5);
        item.type = entry[4];
        item.id = -1;
        item.identified = 1;
        g_session.AddShopItem(item, *reinterpret_cast<const int *>(entry + 9),
                              static_cast<std::uint16_t>(*reinterpret_cast<const std::uint16_t *>(entry + 0xD) / 10));
      }
      UIWindow *window = g_windowMgr.EnsureWindow(WID_PARAITEMSHOPWND);
      window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      window = g_windowMgr.EnsureWindow(WID_PARAITEMPURCHASEWND);
      window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      break;
    }
    default:
      GetRagConnection().ResetQueue();
      break;
    }
  }
}
