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
#include "Contents/Actors/RenderObject.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/MonsterSkillEffects.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Configuration/ServiceConfiguration.h"
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
#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "UI/UIBarGraph.h"
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
#include "UI/Window/WindowDialogs.h"
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

// OFFSET: 0x00D7AF70
// FUNCTION: fn_22198
void CGameMode::ReceiveItemDisappear(std::uint32_t itemAid) {
  if (this->scene->RemoveItem(itemAid)) {
    this->SendMsg(13, static_cast<int>(itemAid), 0, 0, 0);
  }
}

struct SessionMessengerContext {
  void UpdateRandomStats();
  int GetList9Size() const;
  void KeepEquipment();
  void ResetEquipment();
  void ResetSlots();
};

struct SessionItemLists {
  void ClearTradeItems();
  void RemoveEquipment();
};

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

struct ClientItemLists {
  bool AddItem1738(const ITEM_INFO& item, unsigned char refresh);
  bool AddItem1740(const ITEM_INFO& item);
  void AddItem1758(const ITEM_INFO& item);
  void AddItem1718(const RawItemBasic& item, int quantity, std::uint16_t unknown_006C);
  void AppendItem1718(const ITEM_INFO& item);
  void AddItem1748(const ITEM_INFO& item);
  void RemoveItem1758(ITEM_INFO& item);
  void RemoveItem1758ByItemId(ITEM_INFO& item);
  void ClearItems1768();
  void ClearItems1760And1748();
  void ClearItems1758();
  void ClearItems1760();
  void ClearItems1748();
  void ResetEquipped();
  bool Equip(const ITEM_INFO& item);
  void RemoveItem1710(std::uint32_t id, int quantity);
  bool AddItem1710(const ITEM_INFO& item, unsigned char refresh);
  bool EquipItem6B68(const ITEM_INFO& item);
};

void __fastcall sub_DFF150(CSession* session);

// GLOBAL: gl_24992
char g_pendingWhisperCount;

// GLOBAL: gl_25102
int g_whisperRecord;

// GLOBAL: gl_25097
UIWindow* g_saleOpenWindow;

// GLOBAL: gl_25098
UIWindow* g_saleResultWindow;

// GLOBAL: gl_25099
UIWindow* g_vendingOpenWindow;

// GLOBAL: gl_25100
UIWindow* g_vendingOpenResultWindow;

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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 1);
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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 1);
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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 0);
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
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    std::fprintf(file, "\t\t%s\t%s\t%d\t\t%.5f%%\n", ToUtf8(naming.DisplayName(item, 0), g_textCodePage).c_str(),
                 ToUtf8(naming.DisplayName(result, 0), g_textCodePage).c_str(), record->count, static_cast<float>(record->rate) / 1000.0f);
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
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    std::fprintf(file, "\t\t%s\t%d\t%.5f%%\n", ToUtf8(naming.DisplayName(item, 0), g_textCodePage).c_str(), record->count,
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
      ItemDescriptions descriptions{g_itemDescriptions};
      bool languagePrefix = false;
      WORD language = g_serverLanguage->language;
      ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
      std::fprintf(file, "\t%-24s\t\t\t\t\t\t\t%d\t(%d)\t%d/%d\t%s\n", ToUtf8(naming.DisplayName(item, 0), g_textCodePage).c_str(), package->count,
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
    reinterpret_cast<SessionMessengerContext*>(&g_session)->UpdateRandomStats();
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
    reinterpret_cast<SessionMessengerContext*>(&g_session)->UpdateRandomStats();
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
  *reinterpret_cast<int*>(this->unknown_01F8 + 0x4C) = 0;
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
  g_localMapMove = 1;
  this->ChangeZoneServer(mapName, x, y, ip, port, std::string());
}

// OFFSET: 0x00D3A1E0

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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
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
  std::string& speaker = *reinterpret_cast<std::string*>(this->unknown_01F8 + 0x1C);
  std::string& target = *reinterpret_cast<std::string*>(this->unknown_01F8 + 0x34);

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
  reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1758(item);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  g_windowMgr.SendNotice(
    1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xE7), naming.DisplayName(item, 0).c_str(), quantity).c_str()), 0xFFFF64, 0, 0, 0
  );
  if (g_vendingOpenWindow != nullptr) {
    g_vendingOpenWindow->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (reinterpret_cast<const SessionMessengerContext*>(&g_session)->GetList9Size() == 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x137)), 0xFF00, 0, 0, 0);
    if (g_vendingOpenResultWindow != nullptr) {
      g_vendingOpenResultWindow->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
      g_vendingOpenResultWindow->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
    }
  } else if (g_vendingOpenResultWindow != nullptr) {
    g_vendingOpenResultWindow->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (g_vendingOpenResultWindow != nullptr && g_vendingSaleAlarm) {
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
  reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1758ByItemId(item);
  if (g_saleOpenWindow != nullptr) {
    g_saleOpenWindow->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
  }
  if (g_saleResultWindow != nullptr) {
    g_saleResultWindow->SendMsg(nullptr, 0x77, fieldC, 0, 0, 0);
    g_saleResultWindow->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
    if (g_vendingSaleAlarm) {
      g_soundMgr->PlayWave("effect\\ef_steal.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    if (reinterpret_cast<SessionMessengerContext*>(&g_session)->GetList9Size() == 0) {
      g_saleResultWindow->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
  }

  g_session.RefreshItem1740Window();
}

// OFFSET: 0x00D2D770
// FUNCTION: fn_21720
void CGameMode::ReceiveItemInventoryList(const unsigned char* data) {
  reinterpret_cast<SessionMessengerContext*>(&g_session)->KeepEquipment();

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
  ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.SetPartyJobLevel(
    *reinterpret_cast<const std::uint32_t*>(packet + 2), *reinterpret_cast<const std::uint16_t*>(packet + 6),
    *reinterpret_cast<const std::uint16_t*>(packet + 8)
  );
  sub_DFF150(&g_session);
}

// OFFSET: 0x00D8A260
// FUNCTION: fn_22217
void CGameMode::ReceivePartyDead(const unsigned char* packet) {
  ClientMessenger messenger{g_session.m_partyMembers, g_session.m_friends};
  MessengerMember member = messenger.PartyByAccount(*reinterpret_cast<const std::uint32_t*>(packet + 2));
  if (member.valid != 0) {
    messenger.SetPartyDead(*reinterpret_cast<const std::uint32_t*>(packet + 2), packet[6]);
    sub_DFF150(&g_session);
  }
}

// OFFSET: 0x00D5AB80
// FUNCTION: fn_21962
void CGameMode::ReceiveShopItemList(
  const std::vector<ITEM_INFO>& items, int value, int unused1, int argument, int unused2, std::uint32_t duration, unsigned char myShop
) {
  ClientItemLists* lists = reinterpret_cast<ClientItemLists*>(&g_session);
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
  *reinterpret_cast<int*>(this->unknown_0410) = 5;
  this->unknown_0414 = 0;
  this->unknown_0408 = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AEE0
// FUNCTION: fn_22195
void CGameMode::Zc_Start_Capture(const unsigned char*) {
  this->unknown_040C = 10000;
  *reinterpret_cast<int*>(this->unknown_0410) = 5;
  this->unknown_0414 = 0;
  this->unknown_0408 = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AF10
// FUNCTION: fn_22196
void CGameMode::Zc_Start_Collection(const unsigned char*) {
  this->unknown_040C = 40000;
  *reinterpret_cast<int*>(this->unknown_0410) = 1;
  this->unknown_0414 = 0;
  this->unknown_0408 = reinterpret_cast<void*>(2);
}

// OFFSET: 0x00D7AF40
// FUNCTION: fn_22197
void CGameMode::Zc_Start_Couple(const unsigned char*) {
  this->unknown_040C = 20000;
  *reinterpret_cast<int*>(this->unknown_0410) = 5;
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
      if (!actor->state.Attachments().HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 130, 0, 0, 0);
        }
      }
    } else if (options & 0x2) {
      if (!actor->state.Attachments().HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 128, 0, 0, 0);
        }
      }
    } else if (options & 0x4) {
      if (!actor->state.Attachments().HasId(166)) {
        CRagEffect* effect = actor->LaunchEffect(166, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
        if (effect) {
          effect->SendMsg(nullptr, 44, 0, 0xFFFF, 169, 0, 0, 0);
        }
      }
    } else if (options & 0x4000) {
      if (!actor->state.Attachments().HasId(617)) {
        actor->LaunchEffect(617, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }

    if ((options & 0x8) && !actor->state.Attachments().HasId(203)) {
      actor->LaunchEffect(203, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x10) && !actor->state.Attachments().HasId(254)) {
      actor->LaunchEffect(254, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      actor->LaunchEffect(1132, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x1000) && !actor->state.Attachments().HasId(418)) {
      actor->LaunchEffect(418, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      actor->LaunchEffect(544, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x2000) && !actor->state.Attachments().HasId(419)) {
      actor->LaunchEffect(419, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x8000) && !actor->state.Attachments().HasId(424)) {
      actor->LaunchEffect(424, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x20000) && !actor->state.Attachments().HasId(586)) {
      actor->LaunchEffect(586, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x10000) && !actor->state.Attachments().HasId(655)) {
      actor->LaunchEffect(655, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x40) && !actor->state.Attachments().HasId(474)) {
      actor->LaunchEffect(474, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x100) && !actor->state.Attachments().HasId(382)) {
      actor->LaunchEffect(382, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x80) && !actor->state.Attachments().HasId(368)) {
      actor->LaunchEffect(368, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x200) && !actor->state.Attachments().HasId(394)) {
      actor->LaunchEffect(394, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x400) && !actor->state.Attachments().HasId(396)) {
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

    if ((options & 0x800) && !actor->state.Attachments().HasId(375)) {
      actor->LaunchEffect(375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if ((options & 0x40000) && !actor->state.Attachments().HasId(744)) {
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
    if (!actor->state.Attachments().HasId(420)) {
      actor->LaunchEffect(420, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->state.Attachments().HasId(420)) {
      actor->SendMsg(nullptr, 109, 0, 420, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->LaunchEffect(540, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (options & 0x4) {
    if (!actor->state.Attachments().HasId(427)) {
      actor->LaunchEffect(427, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->state.Attachments().HasId(427)) {
      actor->SendMsg(nullptr, 109, 0, 427, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x10) {
    if (!actor->state.Attachments().HasId(254)) {
      actor->LaunchEffect(254, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      actor->LaunchEffect(1132, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->state.Attachments().HasId(254)) {
      actor->SendMsg(nullptr, 109, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->SendMsg(nullptr, 109, 0, 1132, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x10000) {
    if (!actor->state.Attachments().HasId(655)) {
      actor->LaunchEffect(655, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->state.Attachments().HasId(655)) {
      actor->SendMsg(nullptr, 109, 0, 655, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      actor->LaunchEffect(656, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }

  if (options & 0x8000) {
    if (!actor->state.Attachments().HasId(667)) {
      actor->LaunchEffect(667, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->state.Attachments().HasId(667)) {
      actor->SendMsg(nullptr, 109, 0, 667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x800) {
    if (!actor->state.Attachments().HasId(375)) {
      actor->LaunchEffect(375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->state.Attachments().HasId(375)) {
      actor->SendMsg(nullptr, 109, 0, 375, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  if (options & 0x40000) {
    if (!actor->state.Attachments().HasId(744)) {
      actor->LaunchEffect(744, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  } else {
    while (actor->state.Attachments().HasId(744)) {
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

  if (UIWindow* window = g_windowMgr.EnsureWindow(0x122)) {
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

// OFFSET: 0x00D795B0

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
  reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1710(*reinterpret_cast<const std::int16_t*>(packet + 2), 1);
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
    if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1349), 0, true, true, 0, "Message", 280, 120, 0) != 269) {
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
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.unknown_0000, &action, &loopType, *reinterpret_cast<WindowDialogs*>(&g_windowMgr));
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
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.unknown_0000, &action, &loopType, *reinterpret_cast<WindowDialogs*>(&g_windowMgr));
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
  g_session.GetMonsterSkillInfo(g_session.GetJobName(job, -1), message.unknown_0000, &action, &loopType, *reinterpret_cast<WindowDialogs*>(&g_windowMgr));
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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
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

  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1768();
  g_windowMgr.CloseWindow(0xAE);
  g_windowMgr.CloseWindow(0xAF);
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1758();

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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1758(item);
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

  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1758();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 8) / sizeof(Item1758StoreRecord);
  if (count == 0) {
    this->SendMsg(40, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(43);
    g_windowMgr.CloseWindow(44);
    reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1760And1748();
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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1758(item);
  }
}

// OFFSET: 0x00D50590
// FUNCTION: fn_21885
void CGameMode::ReceiveItem1748List(const unsigned char* packet) {
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1748();
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1760();

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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1748(item);
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->UpdateRandomStats();
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->UpdateRandomStats();
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
  reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();

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
    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1718(item, entry.quantity, static_cast<std::uint16_t>(entry.unknown_000D / 10));
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
  reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();

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
      reinterpret_cast<ClientItemLists*>(&g_session)->AppendItem1718(item);
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
    if (actor->state.Attachments().HasId(504) || packet->accountId == 0) {
      return;
    }

    effect = actor->LaunchEffect(504, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  } else if (actor->GetEffectiveJob(-1) == 24 || actor->GetEffectiveJob(-1) == 4215 || actor->GetEffectiveJob(-1) == 4228 ||
             actor->GetEffectiveJob(-1) == 4229 || actor->GetEffectiveJob(-1) == 4306) {
    if (actor->state.Attachments().HasId(629) || packet->accountId == 0) {
      return;
    }

    effect = actor->LaunchEffect(629, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  } else {
    if (actor->state.Attachments().HasId(228) || packet->accountId == 0) {
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
  ClientItemLists* lists = reinterpret_cast<ClientItemLists*>(&g_session);
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

#include "Contents/Jobs/JobInheritance.h"

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
  ClientMessenger messenger{g_session.m_partyMembers, g_session.m_friends};
  messenger.ClearFriends();

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
    messenger.AddFriend(member);
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

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  CEquipmentPropertiesMgr();
  virtual ~CEquipmentPropertiesMgr();
  static CEquipmentPropertiesMgr* GetInstance();
  void RefreshPretend();
  void RemoveExpansion(const int& kind, const int& itemId);
  void AddExpansion(const int& kind, const int& itemId, bool notify);
  void RefreshExpansion(const int& kind, const int& itemId);
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

// OFFSET: 0x00D27EC0

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
      reinterpret_cast<ClientItemLists*>(&g_session)->EquipItem6B68(item);
    } else {
      g_session.EquipItem61B8(item);
    }
  }
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1768();
  if (!g_windowMgr.window_027C && !g_windowMgr.window_0274 && !g_windowMgr.unknown_02FC) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(174)) {
      static_cast<UIMerchantShopMakeWnd*>(window)->AddActionButton();
    }
    return;
  }

  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(270, 0, 0, 0, 0);
}

// OFFSET: 0x00D3A770

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
    *reinterpret_cast<int*>(&g_session.unknown_165C[0]) = packet->value;
    g_session.InvalidateWindowFrames();
    reinterpret_cast<SkillWindows*>(&g_session)->Notify();
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

  MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyByAccount(aid);
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
        SessionRequestTimer timer;
        timer.requestId = 577;
        timer.readyTime = now + remaining;
        timer.duration = total;
        g_session.m_requestTimers.push_back(timer);
      } else if (type == 684) {
        SessionRequestTimer timer;
        timer.requestId = 9999;
        timer.readyTime = now + remaining;
        timer.duration = total;
        g_session.m_requestTimers.push_back(timer);
      } else if (type == 175) {
        SessionRequestTimer timer;
        for (int requestId = 10000; requestId < 10020; ++requestId) {
          std::list<SessionRequestTimer>::iterator it = g_session.m_requestTimers.begin();
          for (; it != g_session.m_requestTimers.end(); ++it) {
            if (it->requestId == requestId) {
              break;
            }
          }

          timer.requestId = requestId;
          timer.readyTime = now + remaining;
          timer.duration = total;
          if (it == g_session.m_requestTimers.end()) {
            g_session.m_requestTimers.push_back(timer);
          } else {
            it->readyTime = timer.readyTime;
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
        g_seasonalMonsterSprites = 1;
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
        while (player->state.Attachments().HasId(752)) {
          player->SendMsg(nullptr, 109, 0, 752, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        std::sprintf(message, "");
      }
      break;
    case 260:
      if (aid == g_session.m_aid) {
        g_seasonalMonsterSprites = 0;
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
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(424), 0, true, false, 0, "Message", 280, 120, 0);
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
    ItemDescriptions{g_itemDescriptions}.Name(item, name, capacity, 0);
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3278), name);
  } else {
    std::sprintf(text, LookupMessage(messageId), *reinterpret_cast<const int*>(packet + 4));
  }

  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  }
}

#include "UI/Item/UIParaItemShopWnd.h"

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
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1768();
  if (!g_windowMgr.window_027C && !g_windowMgr.window_0274 && !g_windowMgr.unknown_02FC) {
    g_windowMgr.EnsureWindow(174);
    if (g_windowMgr.unknown_040C) {
      static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.unknown_040C)->SetItems(0, nullptr);
    }
    return;
  }

  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
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
  reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1768();
  if (!g_windowMgr.window_027C && !g_windowMgr.window_0274) {
    g_windowMgr.EnsureWindow(41);
    if (g_windowMgr.unknown_02FC) {
      static_cast<UIMerchantShopMakeWnd*>(g_windowMgr.unknown_02FC)->SetItems(itemCount, items);
    }
    return;
  }

  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
  this->SendMsg(297, 0, 0, 0, 0);
}

#include "Script/LuaScript.h"

// OFFSET: 0x00D799C0
// FUNCTION: fn_22176
void CGameMode::ReceiveAgitNotice(unsigned char agitId, int first, int second) {
  if (!g_windowMgr.window_0320) {
    return;
  }

  char text[256] = {};
  const char* agitName = nullptr;
  g_luaScript->Call("get_agit_name", "d>s", agitId, &agitName);
  sprintf_s(text, LookupMessage(3632), agitName, second, 100, first, 100);
  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 0, true, false, 0, "Message", 250, 130, 0);
}

#include "Contents/Actors/Item.h"

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

#include "Contents/ClientState.h"

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

#include "Contents/Sound/SoundMaker.h"

// OFFSET: 0x00D25E90
// FUNCTION: fn_21677
void CGameMode::ReceiveHomunculusChange(const unsigned char* packet) {
  char text[256];
  CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4)));
  switch (static_cast<char>(packet[3])) {
  case 0:
    *reinterpret_cast<std::uint32_t*>(&g_session.unknown_5570[0x08]) = *reinterpret_cast<const std::uint32_t*>(packet + 4);
    if (actor) {
      g_groundMarkEnabled = 1;
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
    reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
  }

  reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1710(
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
  ClientMessenger messenger{g_session.m_partyMembers, g_session.m_friends};
  MessengerMember member = messenger.PartyByAccount(*reinterpret_cast<const std::uint32_t*>(packet + 6));
  if (member.valid != 0) {
    messenger.SetPartyLeader(*reinterpret_cast<const std::uint32_t*>(packet + 2), 1);
    messenger.SetPartyLeader(*reinterpret_cast<const std::uint32_t*>(packet + 6), 0);
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
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  g_windowMgr.SendNotice(
    1,
    reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1746), naming.DisplayName(item, 0).c_str(), count, price * count).c_str()),
    0xFFFF64, 0, 0, 0
  );
  reinterpret_cast<ClientItemLists*>(&g_session)->RemoveItem1710(
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
      if (g_luaScript->Call("GetVarOptionName", "d>s", option->optionId, &optionName)) {
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
    reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1768();
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
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 9:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1797)), 0xFF, 0, 0, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3229)), 0xFF, 0, 0, 0);
    reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1768();
    g_windowMgr.CloseWindow(174);
    g_windowMgr.CloseWindow(175);
    break;
  }
}

// OFFSET: 0x00D316D0

// OFFSET: 0x00D4B050
// FUNCTION: fn_21863
void CGameMode::ReceiveItemObtainNotice(const unsigned char* packet) {
  ITEM_INFO item;
  item.SetItemId(*reinterpret_cast<const int*>(packet + 6));
  item.unknown_0060 = 0;
  item.identified = 1;
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 0:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(911), naming.DisplayName(item, 0).c_str()).c_str()), 0xFFFF00, 0, 0, 0
    );
    break;
  case 1:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(912), naming.DisplayName(item, 0).c_str()).c_str()), 0xCDCD00, 0, 0, 0
    );
    break;
  case 2:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(913), naming.DisplayName(item, 0).c_str()).c_str()), 0xC8C8FF, 0, 0, 0
    );
    break;
  case 3:
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(914), naming.DisplayName(item, 0).c_str()).c_str()), 0xC8C8FF, 0, 0, 0
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
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    const char* suffix = LookupMessage(3144);
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", naming.DisplayName(item, 0).c_str(), suffix).c_str()), 0xC8FF00, 7, 0, 0
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
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1747), naming.DisplayName(item, 0).c_str()).c_str()), 0xFF, 0, 0, 0
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

// OFFSET: 0x00D31390

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
  reinterpret_cast<CTimeScheduler*>(g_timeScheduler)->sub_BA95F0(5);
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
      reinterpret_cast<CTimeScheduler*>(g_timeScheduler)->AddSchedule(new CEmotionPurchaseExpireSchedule(now, interval, 1));
    }
  }
  manager->RefreshCommands();
}

// OFFSET: 0x00D17BF0
// FUNCTION: fn_21733
void CGameMode::ReceivePartyItemPickupNotice(const unsigned char* packet) {
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyByAccount(aid);
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

    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    g_windowMgr.SendNotice(
      1, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(1276), member.name.c_str(), naming.DisplayName(item, 0).c_str()).c_str()),
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
  reinterpret_cast<ClientItemLists*>(&g_session)->ResetEquipped();
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
        reinterpret_cast<ClientItemLists*>(&g_session)->Equip(item);
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
  reinterpret_cast<ClientItemLists*>(&g_session)->ResetEquipped();
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
        reinterpret_cast<ClientItemLists*>(&g_session)->Equip(item);
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
// FUNCTION: fn_21597
void CGameMode::RefreshEmotionSaleSchedule() {
  reinterpret_cast<CTimeScheduler*>(g_timeScheduler)->sub_BA95F0(6);
  CCashEmotionMgr* manager = g_cashEmotionMgr;
  if (!manager) {
    return;
  }

  const int expiry = manager->RefreshUnownedPacks().nextTime;
  if (expiry < 0x7FFFFFFF) {
    const std::uint64_t now = static_cast<std::int64_t>(manager->sub_59FC50()) * 10000000;
    const std::int64_t interval = (static_cast<std::int64_t>(expiry) * 10000000 - static_cast<std::int64_t>(now)) / 10000;
    if (interval != 0 && interval >= 0) {
      reinterpret_cast<CTimeScheduler*>(g_timeScheduler)->AddSchedule(new CEmotionSaleExpireSchedule(now, interval, 1));
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
  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 0, true, true, 0, "Message", 280, 120, 0);
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

// OFFSET: 0x00D1E840

#include "Contents/ForbiddenWords.h"
#include "Contents/PetEvolutionMgr.h"
#include "UI/Window/UIPetInfoWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Chat/UIChatRoomTitle.h"

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
      g_activePetEggIndex = item.unknown_0000[1];
    }

    if (item.unknown_0000[3] != 0) {
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem3500(item);
      } else {
        reinterpret_cast<ClientItemLists*>(&g_session)->Equip(item);
      }
    }

    if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.unknown_005D) {
      const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_activePetEggIndex).itemId.c_str());
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
  reinterpret_cast<ClientItemLists*>(&g_session)->ResetEquipped();
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
  reinterpret_cast<ClientItemLists*>(&g_session)->ResetEquipped();
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
  reinterpret_cast<ClientItemLists*>(&g_session)->ResetEquipped();
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
      g_activePetEggIndex = item.unknown_0000[1];
    }

    if (item.unknown_0000[3] == 0) {
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 0);
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
      const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_activePetEggIndex).itemId.c_str());
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
  reinterpret_cast<SessionItemLists*>(&g_session)->RemoveEquipment();
  reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetEquipment();
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
  reinterpret_cast<SessionItemLists*>(&g_session)->RemoveEquipment();
  reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetEquipment();
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
  reinterpret_cast<SessionItemLists*>(&g_session)->RemoveEquipment();
  reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetEquipment();
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
      *reinterpret_cast<int*>(&this->unknown_04F4[0xC4]) = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }

    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 1);
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
      *reinterpret_cast<int*>(&this->unknown_04F4[0xC4]) = 0;
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
    }
    g_session.UpdateInventoryItem(item, 1);
  } else {
    this->NotifyItemResult(packet->result);
  }
}

// OFFSET: 0x00D2E4F0

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
      *reinterpret_cast<int*>(&this->unknown_04F4[0xC4]) = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }

    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 1);
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
      *reinterpret_cast<int*>(&this->unknown_04F4[0xC4]) = 0;
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
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
        *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
      }
      if (type == 10 || type == 16 || type == 17 || type == 19) {
        item.unknown_0000[3] = 0x8000;
      }

      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 1);
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
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
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
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }

    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 1);
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
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
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
    MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyByAccount(aid);
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
      ItemDescriptions{g_itemDescriptions}.Name(item, name, capacity, 0);
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
      ItemDescriptions{g_itemDescriptions}.Name(item, name, capacity, 0);
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
          g_activePetEggIndex = item.unknown_0000[1];
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
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->AddMessageBox(LookupMessage(0x10F8), 0, true, 0, 0, LookupMessage(0xDF6), 0x118, 0x78, nullptr, false);
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));

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
  g_guildName = masterName;
  g_guildIsMaster = g_guildName == g_session.DecodePlayerName();
  guildInfo->unknown_0074 = manageLand;
  g_guildId = packet->guildId;
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

// OFFSET: 0x00D27420
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
    std::sprintf(text, LookupMessage(0x5B4), CSkillInfo::DisplayName(*g_luaScript, packet->skillId));
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
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    const std::string message = FormatOwnedString(LookupMessage(0x600), naming.DisplayName(item, 0).c_str(), packet->value);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 72: {
    ITEM_INFO item;
    item.SetItemId(packet->itemId);
    item.identified = 1;
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    const std::string message = FormatOwnedString(LookupMessage(0x602), naming.DisplayName(item, 0).c_str());
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0xB, 0, 0);
    return;
  }
  case 73:
    if (packet->value == 0) {
      std::sprintf(text, LookupMessage(0x618));
    } else {
      std::sprintf(text, LookupMessage(0x617), CSkillInfo::DisplayName(*g_luaScript, packet->value));
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

#include "Contents/ForbiddenWords.h"
#include "Network/ActorNamePacket.h"

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
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
    }
    if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
      item.unknown_0000[3] = 0x8000;
    }
    item.unknown_0088 = 0;
    item.unknown_0070 = packet->look;

    reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 1);
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
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
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
        *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
      }
      if (packet->type == 10 || packet->type == 16 || packet->type == 17 || packet->type == 19) {
        item.unknown_0000[3] = 0x8000;
      }
      item.unknown_0070 = packet->look;
      item.unknown_0088 = packet->grade;

      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 1);
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
      *reinterpret_cast<int*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_04F4[0xC4]) = 0;
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
      if (reinterpret_cast<CGameActor*>(&g_session)->IsMonsterClass(job) || actor->unknown_0314 == 9 || g_session.IsSkillIdInRange6001(job) ||
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
    if (g_vendingOpenResultWindow) {
      g_vendingOpenResultWindow->SendMsg(nullptr, 6, 201, 0, 0, 0);
    }
    if (g_saleResultWindow) {
      g_saleResultWindow->SendMsg(nullptr, 6, 201, 0, 0, 0);
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
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x2B7), 0, true, false, 0, "Message", 280, 120, 0);
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
      reinterpret_cast<ClientItemLists*>(&g_session)->ClearItems1760And1748();
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));

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
  g_guildName = masterName;
  g_guildIsMaster = g_guildName == g_session.DecodePlayerName();
  guildInfo->unknown_0074 = manageLand;
  g_guildId = packet->guildId;
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));

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
    g_guildIsMaster = 1;
  }
  guildInfo->unknown_0000 = guildName;
  g_guildName = masterName;
  g_proximityCompareId = packet->masterCharacterId;
  guildInfo->unknown_0074 = manageLand;
  g_guildId = packet->guildId;
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));

  if (!g_playerNameDictionary->Contains(packet->masterCharacterId)) {
    const std::uint32_t characterId = packet->masterCharacterId;
    std::string name(packet->masterName, packet->masterName + sizeof(packet->masterName));
    SetPlayerName(characterId, name.c_str());
  }
  g_guildName = LookupPlayerName(packet->masterCharacterId);
  if (g_guildName.size() == 0) {
    g_guildName = LookupMessage(0x245) + g_gameModeNamelessTag;
  }

  char guildName[25];
  guildName[24] = '\0';
  char manageLand[25];
  manageLand[24] = '\0';
  if (g_guildName == g_session.DecodePlayerName()) {
    g_guildIsMaster = 1;
  }
  std::memcpy(guildName, packet->guildName, sizeof(packet->guildName));
  std::memcpy(manageLand, packet->manageLand, sizeof(manageLand) - 1);
  if (g_forbiddenWords.Contains(guildName)) {
    std::string masked;
    g_forbiddenWords.Mask(guildName, &masked);
  }

  guildInfo->unknown_0000 = guildName;
  g_proximityCompareId = packet->masterCharacterId;
  guildInfo->unknown_0074 = manageLand;
  g_guildId = packet->guildId;
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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

// OFFSET: 0x00D50AA0

#include "UI/Guild/UIGuild_Storage_Log.h"
#include <algorithm>

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
  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
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

// OFFSET: 0x00D5B3C0

#include "Contents/Actors/GroundScope.h"
#include "Contents/Ground/TerrainHeight.h"
#include <new>

// OFFSET: 0x00D5E930
// FUNCTION: fn_21967
void LaunchSkillCastEffect(CGameMode* mode, CGameActor* source, CGameActor* target, std::uint32_t targetAid, std::uint16_t skillId, short x, short y, int delay) {
  CSkillActionMgr::GetInstance()->ActionSlot10(skillId, delay, target, x, y);

  if (targetAid == 0) {
    float worldX = 0.0f;
    float worldZ = 0.0f;
    const int column = x;
    const int row = y;
    reinterpret_cast<const ::TerrainHeight*>(g_modeMgr.GetGameMode())->CellCenter(column, row, worldX, worldZ);
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

// OFFSET: 0x00D5B940
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

  const std::string skillName = CSkillInfo::DisplayName(*g_luaScript, skillId);
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
      reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetSlots();
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
  reinterpret_cast<SessionItemLists*>(&g_session)->RemoveEquipment();
  reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetEquipment();
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
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 0);
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
  reinterpret_cast<SessionItemLists*>(&g_session)->RemoveEquipment();
  reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetEquipment();
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
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 0);
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
  reinterpret_cast<SessionItemLists*>(&g_session)->RemoveEquipment();
  reinterpret_cast<SessionMessengerContext*>(&g_session)->ResetEquipment();
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
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1710(item, 0);
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
