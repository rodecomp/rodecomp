#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/World/World.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <utility>
#include <vector>

namespace {
  struct EquipSlotDecoder {
    EquipSlots Decode(int mask) const;
  };

#pragma pack(push, 1)
  struct ItemRandomOption {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t value;
    /* 0004 */ unsigned char param;
  };

  struct ItemAddPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t index;
    /* 0004 */ int amount;
    /* 0008 */ int itemId;
    /* 000C */ unsigned char type;
    /* 000D */ unsigned char identified;
    /* 000E */ unsigned char damaged;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
  };

  struct ItemAddOptionsPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t index;
    /* 0004 */ int amount;
    /* 0008 */ int itemId;
    /* 000C */ unsigned char type;
    /* 000D */ unsigned char identified;
    /* 000E */ unsigned char damaged;
    /* 000F */ unsigned char refine;
    /* 0010 */ std::uint32_t cards[4];
    /* 0020 */ ItemRandomOption options[5];
  };

  struct ItemAddGradePacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t index;
    /* 0004 */ int amount;
    /* 0008 */ int itemId;
    /* 000C */ unsigned char type;
    /* 000D */ unsigned char identified;
    /* 000E */ unsigned char damaged;
    /* 000F */ std::uint32_t cards[4];
    /* 001F */ ItemRandomOption options[5];
    /* 0038 */ unsigned char refine;
    /* 0039 */ unsigned char grade;
  };

  struct ItemSlotPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ int slot;
    /* 0006 */ unsigned char type;
    /* 0007 */ int itemId;
    /* 000B */ unsigned char identified;
    /* 000C */ unsigned char damaged;
    /* 000D */ unsigned char refine;
    /* 000E */ std::uint32_t cards[4];
  };

  struct ItemSlotOptionsPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ int slot;
    /* 0006 */ unsigned char type;
    /* 0007 */ int itemId;
    /* 000B */ unsigned char identified;
    /* 000C */ unsigned char damaged;
    /* 000D */ unsigned char refine;
    /* 000E */ std::uint32_t cards[4];
    /* 001E */ ItemRandomOption options[5];
  };

  struct FriendNoticePacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t result;
    /* 0004 */ int accountId;
    /* 0008 */ int characterId;
    /* 000C */ char name[24];
  };

  struct ItemListRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::int16_t amount;
    /* 0009 */ int location;
    /* 000D */ std::uint32_t cards[4];
    /* 001D */ int expireTime;
    /* 0021 */ unsigned char flags;
  };
#pragma pack(pop)

  bool IsSameItem(const ITEM_INFO& value, const ITEM_INFO& item) {
    if (value.itemId != item.itemId || std::memcmp(value.unknown_001C, item.unknown_001C, sizeof(value.unknown_001C)) != 0 ||
        value.identified != item.identified || value.unknown_0060 != item.unknown_0060 || value.unknown_0088 != item.unknown_0088) {
      return false;
    }

    const int count = item.unknown_0098;

    if (value.unknown_0098 != count || value.unknown_0098 >= 5) {
      return false;
    }

    if (value.unknown_0098 != 0) {
      const auto* left = reinterpret_cast<const ItemRandomOption*>(value.unknown_009C);
      const auto* right = reinterpret_cast<const ItemRandomOption*>(item.unknown_009C);

      for (int index = 0; index < count; ++index) {
        if (left[index].id != right[index].id || left[index].value != right[index].value || left[index].param != right[index].param) {
          return false;
        }
      }
    }

    return true;
  }

  void CountSameNameChar(char left, char right, short& matched) {
    if (left == 'I' || left == 'l') {
      if (right == 'I' || right == 'l') {
        ++matched;
      }
    } else if (left == right) {
      ++matched;
    }
  }
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ItemRandomOption) == 5);
static_assert(sizeof(ItemAddPacket) == 0x20);
static_assert(sizeof(ItemAddOptionsPacket) == 0x39);
static_assert(sizeof(ItemAddGradePacket) == 0x3A);
static_assert(sizeof(ItemListRecord) == 0x22);
#endif

struct ClientItemLists {
  /* 0000 */ unsigned char unknown_0000[0xC0];
  /* 00C0 */ std::string hotkeyDescriptions[36];
  /* 0420 */ unsigned char unknown_0420[0x314];
  /* 0734 */ std::list<SessionRecord0734> records0734;
  /* 073C */ unsigned char unknown_073C[0xF30];
  /* 166C */ int unknown_166C;
  /* 1670 */ int unknown_1670;
  /* 1674 */ int unknown_1674;
  /* 1678 */ int unknown_1678;
  /* 167C */ int unknown_167C;
  /* 1680 */ int unknown_1680;
  /* 1684 */ unsigned char unknown_1684[0x8C];
  /* 1710 */ std::list<ITEM_INFO> inventory;
  /* 1718 */ std::list<ITEM_INFO> items_1718;
  /* 1720 */ std::list<ITEM_INFO> items_1720;
  /* 1728 */ std::list<ITEM_INFO> items_1728;
  /* 1730 */ std::list<ITEM_INFO> items_1730;
  /* 1738 */ std::list<ITEM_INFO> items_1738;
  /* 1740 */ std::list<ITEM_INFO> items_1740;
  /* 1748 */ std::list<ITEM_INFO> items_1748;
  /* 1750 */ std::list<ITEM_INFO> items_1750;
  /* 1758 */ std::list<ITEM_INFO> items_1758;
  /* 1760 */ std::list<ITEM_INFO> items_1760;
  /* 1768 */ std::list<ITEM_INFO> items_1768;
  /* 1770 */ std::list<ITEM_INFO> items_1770;
  /* 1778 */ unsigned char unknown_1778[0xC];
  /* 1784 */ std::list<ITEM_INFO> items_1784[11];
  /* 17DC */ std::list<MessengerMember> partyMembers;
  /* 17E4 */ unsigned char unknown_17E4[0x9BC];
  /* 21A0 */ ITEM_INFO equipped[10];
  /* 2B50 */ unsigned char unknown_2B50[0x1360];
  /* 3EB0 */ ITEM_INFO exchangeItems[10];
  /* 4860 */ ITEM_INFO itemSlots[10];
  /* 5210 */ int unknown_5210;
  /* 5214 */ int unknown_5214;
  /* 5218 */ unsigned char unknown_5218[0x24C];
  /* 5464 */ int unknown_5464;
  /* 5468 */ unsigned char unknown_5468[0x5C];
  /* 54C4 */ std::string unknown_54C4;
  /* 54DC */ unsigned char unknown_54DC[0xCDC];
  /* 61B8 */ ITEM_INFO items_61B8[10];
  /* 6B68 */ ITEM_INFO items_6B68[10];

  bool AddItem1710(const ITEM_INFO& item, unsigned char refresh);
  void AddItem1718(const RawItemBasic& item, int quantity, std::uint16_t unknown_006C);
  void AddItem1718(const RawItemBasic& item, int mode);
  bool AddItem1768(const ITEM_INFO& item, unsigned char alternate, unsigned char refresh);
  bool AddItem1740(const ITEM_INFO& item);
  bool AddItem1750(const ITEM_INFO& item, int type, unsigned char refresh);
  void AddItem1758(const ITEM_INFO& item);
  void AddItem1760(const ITEM_INFO& item);
  void AddItem1748(const ITEM_INFO& item);
  bool Equip(const ITEM_INFO& item);
  void AppendItem1718(const ITEM_INFO& item);
  void MergeItem1718ByName(const ITEM_INFO& item);
  void MergeItem1720ByName(const ITEM_INFO& item);
  void MergeItem1720(const ITEM_INFO& item);
  void AddItem1730(const ITEM_INFO& item);
  void MergeItem1718(const ITEM_INFO& item);
  bool AddItem1738(const ITEM_INFO& item, unsigned char refresh);
  void RemoveItem1758ByItemId(ITEM_INFO& item);
  __declspec(noinline) void RemoveItem1710(std::uint32_t id, int quantity);
  bool AddExchangeItem(ITEM_INFO item);
  bool RemoveItem1768(const ITEM_INFO& item, unsigned char alternate);
  bool RemoveItem1740(const ITEM_INFO& item);
  bool RemoveItem1750(const ITEM_INFO& item, int type, unsigned char refresh);
  void RemoveItem1758(ITEM_INFO& item);
  void RemoveItem1760(const ITEM_INFO& item);
  void RemoveItem1718ByItemId(const ITEM_INFO& item);
  void RemoveItem1718(const ITEM_INFO& item);
  void RemoveItem1720ByName(const ITEM_INFO& item);
  void RemoveItem1720(const ITEM_INFO& item);
  void RemoveItem1730(const ITEM_INFO& item);
  void ReduceItem1718(const ITEM_INFO& item);
  bool RemoveItem1738(const ITEM_INFO& item);
  void ClearInventory();
  void ClearItems1760And1748();
  void ClearItems1768();
  void ClearItems1740();
  void ClearItems1750();
  void SetItems1750(int count, const short* ids);
  void ClearItems1758();
  void ClearItems1760();
  void ClearItems1748();
  void ClearItems1784(int index);
  void ClearItemSlotQuantities();
  void ResetEquipped();
  void ClearItems1720();
  void ClearItems1738();
  void CopyItems1740To1750();
  void CopyItems1760To1770();
  std::pair<bool, ITEM_INFO> FindItem1750(const ITEM_INFO& item, unsigned char unknown);
  void ClearRecords0734();
  void ClearPartyMembers();
  void ResetUnknown166C();
  void ResetHotkeyDescriptions();
  int IsDifferentName(const char* name, const char* other);
  bool EquipItem6B68(const ITEM_INFO& item);
  void ResetItems6B68();
  void ResetItems61B8();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ClientItemLists, inventory) == 0x1710);
static_assert(offsetof(ClientItemLists, items_1768) == 0x1768);
static_assert(offsetof(ClientItemLists, items_1784) == 0x1784);
static_assert(offsetof(ClientItemLists, equipped) == 0x21A0);
static_assert(offsetof(ClientItemLists, itemSlots) == 0x4860);
static_assert(offsetof(ClientItemLists, unknown_5214) == 0x5214);
static_assert(offsetof(ClientItemLists, hotkeyDescriptions) == 0xC0);
static_assert(offsetof(ClientItemLists, records0734) == 0x734);
static_assert(offsetof(ClientItemLists, unknown_166C) == 0x166C);
static_assert(offsetof(ClientItemLists, partyMembers) == 0x17DC);
static_assert(offsetof(ClientItemLists, unknown_5464) == 0x5464);
static_assert(offsetof(ClientItemLists, unknown_54C4) == 0x54C4);
static_assert(offsetof(ClientItemLists, items_61B8) == 0x61B8);
static_assert(offsetof(ClientItemLists, items_6B68) == 0x6B68);
#endif

// OFFSET: 0x00AA6440
// FUNCTION: fn_17229
void __fastcall RefreshItemWindows(UIWindowMgr* mgr) {
  if (mgr->unknown_01D4) {
    reinterpret_cast<UIWindow*>(mgr->unknown_01D4)->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  if (mgr->window_01E8) {
    mgr->window_01E8->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  if (UIWindow* window = mgr->LookupWindow(159)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  if (mgr->window_4F260) {
    mgr->window_4F260->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// INLINED
static int CountItemOptions(const ItemRandomOption* options) {
  int count = 0;
  for (int index = 0; index < 5; ++index) {
    if (options[index].id != 0) {
      ++count;
    }
  }
  return count;
}

// OFFSET: 0x00D1F5A0
// FUNCTION: fn_21640
void CGameMode::ReceiveItem1738Add(const unsigned char* data) {
  const ItemAddPacket packet = *reinterpret_cast<const ItemAddPacket*>(data);
  ItemRandomOption options[5] = {};
  ITEM_INFO item;
  item.unknown_0000[1] = packet.index;
  item.quantity = packet.amount;
  item.SetItemId(packet.itemId);
  item.identified = packet.identified;
  item.unknown_005D = packet.damaged;
  item.unknown_0060 = packet.refine;
  item.unknown_0000[0] = packet.type;
  std::memcpy(item.unknown_001C, packet.cards, sizeof(packet.cards));
  item.unknown_0098 = CountItemOptions(options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
  }
  item.unknown_0088 = 0;
  reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 1);
}

  struct ItemAddSlotPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ int slot;
    /* 000B */ unsigned char identified;
    /* 000C */ unsigned char damaged;
    /* 000D */ unsigned char refine;
    /* 000E */ std::uint32_t cards[4];
  };

  struct ItemAddSlotOptionsPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ int slot;
    /* 000B */ unsigned char identified;
    /* 000C */ unsigned char damaged;
    /* 000D */ unsigned char refine;
    /* 000E */ std::uint32_t cards[4];
    /* 001E */ ItemRandomOption options[5];
  };

// OFFSET: 0x00D1F770
// FUNCTION: fn_21641
void sub_D1F770(const unsigned char* data) {
  const ItemAddSlotPacket* packet = reinterpret_cast<const ItemAddSlotPacket*>(data);
  if (packet->slot < 0) {
    (*reinterpret_cast<CMode**>(0x013495BC))->SendMsg(0x35, 0, 0, 0, 0);
  }

  if (packet->itemId == 0) {
    g_session.SetUnknown5214(packet->slot);
    return;
  }

  ITEM_INFO item;
  item.unknown_0000[0] = packet->type;
  item.SetItemId(packet->itemId);
  item.unknown_0000[1] = 0xFFFFFFFF;
  item.quantity = packet->slot;
  std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
  item.identified = packet->identified;
  item.unknown_005D = packet->damaged;
  item.unknown_0060 = packet->refine;
  item.unknown_0098 = 0;
  g_session.AddItemSlot(item);
}

// OFFSET: 0x00D1F970
// FUNCTION: fn_21642
void sub_D1F970(const unsigned char* data) {
  const ItemAddSlotOptionsPacket* packet = reinterpret_cast<const ItemAddSlotOptionsPacket*>(data);
  if (packet->slot < 0) {
    (*reinterpret_cast<CMode**>(0x013495BC))->SendMsg(0x35, 0, 0, 0, 0);
  }

  if (packet->itemId == 0) {
    g_session.SetUnknown5214(packet->slot);
    return;
  }

  ITEM_INFO item;
  item.unknown_0000[0] = packet->type;
  item.SetItemId(packet->itemId);
  item.unknown_0000[1] = 0xFFFFFFFF;
  item.quantity = packet->slot;
  std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
  item.identified = packet->identified;
  item.unknown_005D = packet->damaged;
  item.unknown_0060 = packet->refine;

  int count = 0;
  if (packet->options[0].id != 0) {
    ++count;
  }

  if (packet->options[1].id != 0) {
    ++count;
  }

  if (packet->options[2].id != 0) {
    ++count;
  }

  if (packet->options[3].id != 0) {
    ++count;
  }

  if (packet->options[4].id != 0) {
    ++count;
  }

  item.unknown_0098 = count;
  for (int index = 0; index < count; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = packet->options[index];
  }

  g_session.AddItemSlot(item);
}

// OFFSET: 0x00D1FBA0
// FUNCTION: fn_21643
void sub_D1FBA0(const unsigned char* data) {
  const FriendNoticePacket* packet = reinterpret_cast<const FriendNoticePacket*>(data);
  char name[25];
  std::memcpy(name, packet->name, 24);
  name[24] = '\0';

  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);

  char buffer[128];
  const char* format = nullptr;
  switch (packet->result) {
    case 0: {
      std::sprintf(buffer, LookupMessage(821), name);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 0xFFFF00, 0, 0, 0);

      MessengerMember member;
      member.name = name;
      member.accountId = packet->accountId;
      member.characterId = packet->characterId;
      member.offline = 0;
      ClientMessenger messenger{g_session.m_partyMembers, g_session.m_friends};
      messenger.AddFriend(member);
      g_session.RefreshWindow02C8();
      return;
    }
    case 1:
      format = LookupMessage(822);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(819)), 0xFF, 0, 0, 0);
      return;
    case 3:
      format = LookupMessage(820);
      break;
    case 4:
      format = LookupMessage(2737);
      break;
    case 6:
      format = LookupMessage(3448);
      break;
    case 7:
      format = LookupMessage(3449);
      break;
    case 8:
      format = LookupMessage(3450);
      break;
    default:
      return;
  }

  std::sprintf(buffer, format, name);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D1FDA0
// FUNCTION: fn_21644
void CGameMode::ReceiveItem1740Add(const unsigned char* data) {
  const ItemAddPacket packet = *reinterpret_cast<const ItemAddPacket*>(data);
  ItemRandomOption options[5] = {};
  ITEM_INFO item;
  item.unknown_0000[1] = packet.index;
  item.quantity = packet.amount;
  item.SetItemId(packet.itemId);
  item.identified = packet.identified;
  item.unknown_005D = packet.damaged;
  item.unknown_0060 = packet.refine;
  item.unknown_0000[0] = packet.type;
  std::memcpy(item.unknown_001C, packet.cards, sizeof(packet.cards));
  item.unknown_0098 = CountItemOptions(options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
  }
  item.unknown_0088 = 0;
  reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
}

// OFFSET: 0x00D1FF70
// FUNCTION: fn_21645
void CGameMode::ReceiveItem1740AddOptions(const unsigned char* data) {
  const ItemAddOptionsPacket packet = *reinterpret_cast<const ItemAddOptionsPacket*>(data);
  ItemRandomOption options[5];
  std::memcpy(options, packet.options, sizeof(options));
  ITEM_INFO item;
  item.unknown_0000[1] = packet.index;
  item.quantity = packet.amount;
  item.SetItemId(packet.itemId);
  item.identified = packet.identified;
  item.unknown_005D = packet.damaged;
  item.unknown_0060 = packet.refine;
  item.unknown_0000[0] = packet.type;
  std::memcpy(item.unknown_001C, packet.cards, sizeof(packet.cards));
  item.unknown_0098 = CountItemOptions(options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
  }
  item.unknown_0088 = 0;
  reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
}

// OFFSET: 0x00D20180
// FUNCTION: fn_21646
void CGameMode::ReceiveItem1740AddGrade(const unsigned char* data) {
  const ItemAddGradePacket packet = *reinterpret_cast<const ItemAddGradePacket*>(data);
  ItemRandomOption options[5];
  std::memcpy(options, packet.options, sizeof(options));
  ITEM_INFO item;
  item.unknown_0000[1] = packet.index;
  item.quantity = packet.amount;
  item.SetItemId(packet.itemId);
  item.identified = packet.identified;
  item.unknown_005D = packet.damaged;
  item.unknown_0060 = packet.refine;
  item.unknown_0000[0] = packet.type;
  std::memcpy(item.unknown_001C, packet.cards, sizeof(packet.cards));
  item.unknown_0098 = CountItemOptions(options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
  }
  item.unknown_0088 = packet.grade;
  reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
}

// OFFSET: 0x00D204B0
// FUNCTION: fn_21647
void CGameMode::ReceiveItem1738AddOptions(const unsigned char* data) {
  const ItemAddOptionsPacket packet = *reinterpret_cast<const ItemAddOptionsPacket*>(data);
  ItemRandomOption options[5];
  std::memcpy(options, packet.options, sizeof(options));
  ITEM_INFO item;
  item.unknown_0000[1] = packet.index;
  item.quantity = packet.amount;
  item.SetItemId(packet.itemId);
  item.identified = packet.identified;
  item.unknown_005D = packet.damaged;
  item.unknown_0060 = packet.refine;
  item.unknown_0000[0] = packet.type;
  std::memcpy(item.unknown_001C, packet.cards, sizeof(packet.cards));
  item.unknown_0098 = CountItemOptions(options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
  }
  item.unknown_0088 = 0;
  reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 1);
}

// OFFSET: 0x00D206C0
// FUNCTION: fn_21648
void CGameMode::ReceiveItem1738AddGrade(const unsigned char* data) {
  const ItemAddGradePacket packet = *reinterpret_cast<const ItemAddGradePacket*>(data);
  ItemRandomOption options[5];
  std::memcpy(options, packet.options, sizeof(options));
  ITEM_INFO item;
  item.unknown_0000[1] = packet.index;
  item.quantity = packet.amount;
  item.SetItemId(packet.itemId);
  item.identified = packet.identified;
  item.unknown_005D = packet.damaged;
  item.unknown_0060 = packet.refine;
  item.unknown_0000[0] = packet.type;
  std::memcpy(item.unknown_001C, packet.cards, sizeof(packet.cards));
  item.unknown_0098 = CountItemOptions(options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
  }
  item.unknown_0088 = packet.grade;
  reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 1);
}

// OFFSET: 0x00D443F0
// FUNCTION: fn_21833
void CGameMode::ReceiveItemListPart(unsigned char listType, int count, const unsigned char* data) {
  const ItemListRecord* record = reinterpret_cast<const ItemListRecord*>(data);
  for (int i = 0; i < count; ++i, ++record) {
    ITEM_INFO item;
    item.unknown_0000[0] = record->type;
    item.unknown_0000[1] = record->index;
    item.unknown_0000[3] = record->location;
    item.unknown_0000[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.unknown_001C, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.unknown_005D = 0;
    item.identified = record->flags & 1;
    item.unknown_0068 = record->expireTime;
    item.unknown_0060 = 0;
    item.unknown_0074[0] = (record->flags >> 1) & 1;
    switch (listType) {
    case 0:
      g_session.UpdateInventoryItem(item, 0);
      break;
    case 1:
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
      break;
    case 2:
    case 3:
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 0);
      break;
    }
  }
}

// OFFSET: 0x00D44580
// FUNCTION: fn_21834
void CGameMode::RefreshItemListWindow(unsigned char listType, bool partial) {
  if (partial) {
    return;
  }

  switch (listType) {
  case 0:
    RefreshItemWindows(&g_windowMgr);
    break;
  case 1:
    g_session.RefreshItem1740Window();
    break;
  case 2:
  case 3:
    if (UIWindow* window = g_windowMgr.window_0288) {
      window->SendMsg(nullptr, 23, 0, 0, 0, 0);
      reinterpret_cast<unsigned char*>(window)[0x190] = 1;
    }
    break;
  }
}

// OFFSET: 0x00DD43A0
// FUNCTION: fn_23232
EquipSlots EquipSlotDecoder::Decode(int mask) const {
  std::vector<int> slots;

  for (int index = 0; index < 10; ++index) {
    if (mask & 1) {
      slots.push_back(index);
    }
    mask >>= 1;
  }

  if (slots.empty()) {
    return {-1, -1, -1};
  }
  if (slots.size() == 1) {
    return {-1, -1, slots[0]};
  }
  if (slots.size() == 2) {
    return {-1, slots[1], slots[0]};
  }
  return {slots[2], slots[1], slots[0]};
}

// OFFSET: 0x00DD7030
// FUNCTION: fn_23282
bool ClientItemLists::AddItem1710(const ITEM_INFO& item, unsigned char refresh) {
  for (ITEM_INFO& value : this->inventory) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      value.unknown_0074[1] = item.unknown_0074[1];
      value.unknown_0074[0] = item.unknown_0074[0];
      if (refresh) {
        RefreshItemWindows(&g_windowMgr);
      }
      return false;
    }
  }

  this->inventory.push_back(item);
  if (refresh) {
    RefreshItemWindows(&g_windowMgr);
  }
  return true;
}

// OFFSET: 0x00DD7210
// FUNCTION: fn_23284
bool ClientItemLists::AddItem1768(const ITEM_INFO& item, unsigned char alternate, unsigned char refresh) {
  for (ITEM_INFO& value : this->items_1768) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      if (!alternate) {
        value.quantity += item.quantity;
        if (refresh == 1) {
          if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02FC)) {
            window->SendMsg(nullptr, 23, 0, 0, 0, 0);
          }
        }
      }
      return false;
    }
  }

  this->items_1768.push_back(item);
  if (refresh == 1) {
    UIWindow* window = reinterpret_cast<UIWindow*>(alternate ? g_windowMgr.unknown_040C : g_windowMgr.unknown_02FC);
    if (window) {
      window->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }
  }
  return true;
}

// OFFSET: 0x00DD72C0
// FUNCTION: fn_23285
bool ClientItemLists::AddItem1740(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1740) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02F4)) {
        window->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
      return false;
    }
  }

  this->items_1740.push_back(item);
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02F4)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DD7350
// FUNCTION: fn_23286
bool ClientItemLists::AddItem1750(const ITEM_INFO& item, int type, unsigned char refresh) {
  for (ITEM_INFO& value : this->items_1750) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      if (refresh == 1) {
        std::uint32_t window;
        if (type == 1) {
          window = g_windowMgr.unknown_0410;
        } else {
          window = type == 2 ? g_windowMgr.unknown_0420 : g_windowMgr.unknown_02F8;
        }
        if (window) {
          reinterpret_cast<UIWindow*>(window)->SendMsg(nullptr, 23, 0, 0, 0, 0);
        }
      }
      return false;
    }
  }

  this->items_1750.push_back(item);
  if (refresh == 1) {
    std::uint32_t window;
    if (type == 1) {
      window = g_windowMgr.unknown_0410;
    } else {
      window = type == 2 ? g_windowMgr.unknown_0420 : g_windowMgr.unknown_02F8;
    }
    if (window) {
      reinterpret_cast<UIWindow*>(window)->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }
  }
  return true;
}

// OFFSET: 0x00DD7420
// FUNCTION: fn_23287
void ClientItemLists::AddItem1758(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1758) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1758.push_back(item);
}

// OFFSET: 0x00DD7470
// FUNCTION: fn_23288
void ClientItemLists::AddItem1760(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1760) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1760.push_back(item);
}

// OFFSET: 0x00DD74C0
// FUNCTION: fn_23289
void ClientItemLists::AddItem1748(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1748) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1748.push_back(item);
}

// OFFSET: 0x00DD7510
// FUNCTION: fn_23290
bool ClientItemLists::Equip(const ITEM_INFO& item) {
  const EquipSlots slots = reinterpret_cast<CSession*>(this)->GetEquipSlots(item.unknown_0000[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->equipped[slot];
      equipped = item;
      equipped.quantity = 1;
      equipped.unknown_0068 = item.unknown_0068;
      equipped.unknown_0064 = item.unknown_0064;
      equipped.unknown_0070 = item.unknown_0070;
    }
  }

  return true;
}

// OFFSET: 0x00DD75C0
// FUNCTION: fn_23291
void ClientItemLists::AddItem1718(const RawItemBasic& item, int quantity, std::uint16_t unknown_006C) {
  ITEM_INFO value;
  value.AssignBasic(item);
  value.quantity = quantity;
  value.unknown_006C = unknown_006C;
  value.unknown_0060 = 0;
  value.unknown_005D = 0;
  value.SetItemId(std::atoi(value.itemId.c_str()));
  this->items_1718.push_back(value);
}

// OFFSET: 0x00DD76C0
// FUNCTION: fn_23292
void ClientItemLists::AppendItem1718(const ITEM_INFO& item) {
  this->items_1718.push_back(item);
}

// OFFSET: 0x00DD76D0
// FUNCTION: fn_23293
void ClientItemLists::MergeItem1718ByName(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1718) {
    if (value.itemId == item.itemId) {
      value.quantity = value.quantity == -1 ? -1 : value.quantity + item.quantity;
      return;
    }
  }

  this->items_1718.push_back(item);
}

// OFFSET: 0x00DD7770
// FUNCTION: fn_23294
void ClientItemLists::MergeItem1720ByName(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1720) {
    if (value.itemId == item.itemId) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1720.push_back(item);
}

// OFFSET: 0x00DD77F0
// FUNCTION: fn_23295
void ClientItemLists::MergeItem1720(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1720) {
    const bool same = item.unknown_0074[3] ? value.unknown_0000[1] == item.unknown_0000[1] : value.itemId == item.itemId;
    if (same) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1720.push_back(item);
}

// OFFSET: 0x00DD7890
// FUNCTION: fn_23296
void ClientItemLists::AddItem1730(const ITEM_INFO& item) {
  const ITEM_INFO owned = LookupClientItemByIdentity(this->inventory, item.unknown_0000[1]);
  if (owned.unknown_0000[1] > 0) {
    for (ITEM_INFO& value : this->items_1730) {
      if (value.unknown_0000[1] == item.unknown_0000[1]) {
        const int quantity = value.quantity + item.quantity;
        if (owned.quantity >= quantity) {
          value.quantity = quantity;
        }
        return;
      }
    }

    if (owned.quantity >= item.quantity) {
      this->items_1730.push_back(item);
    }
  }
}

// OFFSET: 0x00DD7970
// FUNCTION: fn_23297
void ClientItemLists::AddItem1718(const RawItemBasic& item, int mode) {
  if (mode != 0) {
    const ITEM_INFO owned = LookupClientItemByIdentity(this->inventory, item.id);
    if (owned.unknown_0000[1] != 0) {
      ITEM_INFO value(owned);
      value.AssignBasic(item);
      value.SetItemId(std::atoi(value.itemId.c_str()));
      this->items_1718.push_back(value);
    }
  } else {
    ITEM_INFO value;
    value.AssignBasic(item);
    value.unknown_0060 = 0;
    value.unknown_005D = 0;
    value.SetItemId(std::atoi(value.itemId.c_str()));
    value.unknown_0000[2] = item.unknown_0034;
    value.unknown_0070 = item.unknown_0038;
    this->items_1718.push_back(value);
  }
}

// OFFSET: 0x00DD7B30
// FUNCTION: fn_23298
void ClientItemLists::MergeItem1718(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->items_1718) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      if (value.quantity != -1) {
        value.quantity += item.quantity;
      }
      return;
    }
  }

  this->items_1718.push_back(item);
}

// OFFSET: 0x00DD7B80
// FUNCTION: fn_23299
bool ClientItemLists::AddItem1738(const ITEM_INFO& item, unsigned char refresh) {
  if (g_replayAnimationScaling && !Replay::Instance().header.options.Get(ReplayRecordOption::Item)) {
    return true;
  }

  for (ITEM_INFO& value : this->items_1738) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      if (refresh && g_windowMgr.window_0288) {
        g_windowMgr.window_0288->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
      return false;
    }
  }

  this->items_1738.push_back(item);
  if (refresh && g_windowMgr.window_0288) {
    g_windowMgr.window_0288->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DD7E10
// FUNCTION: fn_23300
EquipSlots CSession::GetEquipSlots(int location) {
  int mask = location;
  switch (location) {
  case 0x8000: {
    EquipSlots result;
    result.values[0] = -1;
    result.values[1] = -1;
    result.values[2] = 15;
    return result;
  }
  case 0x400:
    mask = 0x100;
    break;
  case 0x800:
    mask = 0x200;
    break;
  case 0xC00:
    mask = 0x300;
    break;
  case 0x1000:
    mask = 1;
    break;
  case 0x1400:
    mask = 0x101;
    break;
  case 0x1800:
    mask = 0x201;
    break;
  case 0x1C00:
    mask = 0x301;
    break;
  case 0x2000:
    mask = 4;
    break;
  case 0x4000:
  case 0x20000:
    mask = 2;
    break;
  case 0x10000:
    mask = 0x10;
    break;
  case 0x40000:
    mask = 0x20;
    break;
  case 0x80000:
    mask = 0x40;
    break;
  case 0x100000:
    mask = 8;
    break;
  case 0x200000:
    mask = 0x80;
    break;
  }

  const EquipSlots slots = EquipSlotDecoder().Decode(mask);
  return slots;
}

// OFFSET: 0x00DD8890
// FUNCTION: fn_23313
void ClientItemLists::ClearInventory() {
  this->inventory.erase(this->inventory.begin(), this->inventory.end());
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00DD88C0
// FUNCTION: fn_23314
void ClientItemLists::ClearItems1760And1748() {
  this->items_1760.erase(this->items_1760.begin(), this->items_1760.end());
  this->items_1748.erase(this->items_1748.begin(), this->items_1748.end());
}

// OFFSET: 0x00DD8900
// FUNCTION: fn_23315
void ClientItemLists::ClearItems1768() {
  this->items_1768.erase(this->items_1768.begin(), this->items_1768.end());
}

// OFFSET: 0x00DD8920
// FUNCTION: fn_23316
void ClientItemLists::ClearItems1740() {
  this->items_1740.erase(this->items_1740.begin(), this->items_1740.end());
}

// OFFSET: 0x00DD8940
// FUNCTION: fn_23317
void ClientItemLists::ClearItems1750() {
  this->items_1750.erase(this->items_1750.begin(), this->items_1750.end());
}

// OFFSET: 0x00DD8960
// FUNCTION: fn_23318
void ClientItemLists::ClearItems1758() {
  this->items_1758.erase(this->items_1758.begin(), this->items_1758.end());
}

// OFFSET: 0x00DD8980
// FUNCTION: fn_23319
void ClientItemLists::ClearItems1760() {
  this->items_1760.erase(this->items_1760.begin(), this->items_1760.end());
}

// OFFSET: 0x00DD89A0
// FUNCTION: fn_23320
void ClientItemLists::ClearItems1748() {
  this->items_1748.erase(this->items_1748.begin(), this->items_1748.end());
}

// OFFSET: 0x00DD89C0
// FUNCTION: fn_23321
void ClientItemLists::ClearRecords0734() {
  this->records0734.clear();
}

// OFFSET: 0x00DD8A20
// FUNCTION: fn_23322
void ClientItemLists::ClearItems1784(int index) {
  if (index >= 0 && static_cast<unsigned>(index) < 11) {
    this->items_1784[index].erase(this->items_1784[index].begin(), this->items_1784[index].end());
  }
}

// OFFSET: 0x00DD8A50
// FUNCTION: fn_23323
void ClientItemLists::ClearItemSlotQuantities() {
  this->unknown_5214 = 0;

  for (int index = 0; index < 10; ++index) {
    this->itemSlots[index].quantity = 0;
  }
}

// OFFSET: 0x00DD8AC0
// FUNCTION: fn_23324
void ClientItemLists::ResetEquipped() {
  for (int index = 0; index < 10; ++index) {
    this->equipped[index].Reset();
  }
}

// OFFSET: 0x00DD8AF0
// FUNCTION: fn_23325
void ClientItemLists::ClearPartyMembers() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (mode) {
    for (auto member = this->partyMembers.begin(); member != this->partyMembers.end(); ++member) {
      CRenderObject* actor = mode->scene->GetGameActorByAidIncludeMe(member->accountId);
      if (actor && static_cast<EffectActor*>(actor)->state.Attachments().HasId(160) == true) {
        actor->SendMsg(nullptr, 109, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
  }

  this->partyMembers.clear();
  this->unknown_5464 = 0;
  this->unknown_54C4 = "";
}

// OFFSET: 0x00DD8BD0
// FUNCTION: fn_23326
void ClientItemLists::ResetUnknown166C() {
  this->unknown_166C = 0;
  this->unknown_167C = 0;
  this->unknown_1680 = 0;
  this->unknown_1670 = 0;
  this->unknown_1674 = 0;
  this->unknown_1678 = 0;
}

// OFFSET: 0x00DD8C10
// FUNCTION: fn_23327
void ClientItemLists::ClearItems1720() {
  this->items_1720.erase(this->items_1720.begin(), this->items_1720.end());
}

// OFFSET: 0x00DD8C70
// FUNCTION: fn_23328
void ClientItemLists::ResetHotkeyDescriptions() {
  for (int index = 0; index < 9; ++index) {
    this->hotkeyDescriptions[index] = "nothing";
  }
}

// OFFSET: 0x00DD8CA0
// FUNCTION: fn_23329
void ClientItemLists::ClearItems1738() {
  this->items_1738.erase(this->items_1738.begin(), this->items_1738.end());
}

// OFFSET: 0x00DD8CC0
// FUNCTION: fn_23330
void ClientItemLists::CopyItems1740To1750() {
  this->items_1750.erase(this->items_1750.begin(), this->items_1750.end());

  for (auto item = this->items_1740.begin(); item != this->items_1740.end(); ++item) {
    this->items_1750.push_back(*item);
  }
}

// OFFSET: 0x00DD8DB0
// FUNCTION: fn_23331
void ClientItemLists::CopyItems1760To1770() {
  this->items_1770.erase(this->items_1770.begin(), this->items_1770.end());

  for (auto item = this->items_1760.begin(); item != this->items_1760.end(); ++item) {
    this->items_1770.push_back(*item);
  }
}

// OFFSET: 0x00DD8EA0
// FUNCTION: fn_23332
std::pair<bool, ITEM_INFO> ClientItemLists::FindItem1750(const ITEM_INFO& item, unsigned char) {
  auto found = this->items_1750.begin();

  for (; found != this->items_1750.end(); ++found) {
    if (IsSameItem(*found, item)) {
      break;
    }
  }

  return found != this->items_1750.end() ? std::make_pair(true, *found) : std::make_pair(false, ITEM_INFO());
}

// OFFSET: 0x00DD91C0
// FUNCTION: fn_23333
int ClientItemLists::IsDifferentName(const char* name, const char* other) {
  if (std::strlen(other) == 0) {
    return 1;
  }

  char first[24];
  std::sprintf(first, "%s", name);
  const short firstLength = static_cast<short>(std::strlen(first));

  char second[24];
  std::sprintf(second, "%s", other);
  const short secondLength = static_cast<short>(std::strlen(second));

  const short difference = firstLength >= secondLength ? firstLength - secondLength : secondLength - firstLength;
  const short length = firstLength >= secondLength ? secondLength : firstLength;

  if (firstLength >= secondLength + 2 && firstLength <= secondLength + 4) {
    short matched = 0;
    for (short index = 0; index < length; ++index) {
      CountSameNameChar(first[index + 2], second[index], matched);
    }

    if (matched == length) {
      return 0;
    }
  }

  if (firstLength >= secondLength + 1 && firstLength <= secondLength + 4) {
    short matched = 0;
    for (short index = 0; index < length; ++index) {
      CountSameNameChar(first[index + 1], second[index], matched);
    }

    if (matched == length) {
      return 0;
    }
  }

  if (difference > 2) {
    return 1;
  }

  short matched = 0;
  for (int index = 0; index < length; ++index) {
    CountSameNameChar(first[index], second[index], matched);
  }

  if (matched >= length - 2) {
    return 0;
  }

  matched = 0;
  char skip = 1;
  short position = 0;
  for (short index = 0; index < length; ++index) {
    CountSameNameChar(first[index], second[position], matched);

    if (matched != index + 1 && skip == 1) {
      skip = 2;
      --position;
    }

    ++position;
  }

  if (matched != length - 1) {
    return 1;
  }

  return 0;
}

// OFFSET: 0x00DDA470
// FUNCTION: fn_23338
void ClientItemLists::RemoveItem1758ByItemId(ITEM_INFO& item) {
  for (auto it = this->items_1758.begin(); it != this->items_1758.end(); ++it) {
    if (std::atoi(it->itemId.c_str()) == std::atoi(item.itemId.c_str())) {
      item.itemId = it->itemId;
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1758.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA510
// FUNCTION: fn_23339
void ClientItemLists::RemoveItem1710(std::uint32_t id, int quantity) {
  for (auto it = this->inventory.begin(); it != this->inventory.end(); ++it) {
    if (it->unknown_0000[1] == id) {
      if (quantity < it->quantity) {
        it->quantity -= quantity;
      } else {
        this->inventory.erase(it);
      }
      for (auto it2 = this->inventory.begin(); it2 != this->inventory.end(); ++it2) {
        if (it2->unknown_0000[1] == id) {
          it2->unknown_0074[2] = 0;
          break;
        }
      }
      break;
    }
  }
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00DDA5A0
// FUNCTION: fn_23340
bool ClientItemLists::RemoveItem1768(const ITEM_INFO& item, unsigned char alternate) {
  int index = 0;
  for (auto it = this->items_1768.begin(); it != this->items_1768.end(); ++it, ++index) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      if (!alternate) {
        it->quantity -= item.quantity;
        if (it->quantity < 1) {
          this->items_1768.erase(it);
          reinterpret_cast<CSession*>(this)->RefreshItem1768Window(0, index + 1);
          return true;
        }
        reinterpret_cast<CSession*>(this)->RefreshItem1768Window(0, 0);
        return true;
      }
      this->items_1768.erase(it);
      reinterpret_cast<CSession*>(this)->RefreshItem1768Window(1, 0);
      return true;
    }
  }
  reinterpret_cast<CSession*>(this)->RefreshItem1768Window(alternate, 0);
  return false;
}

// OFFSET: 0x00DDA690
// FUNCTION: fn_23341
bool ClientItemLists::RemoveItem1740(const ITEM_INFO& item) {
  for (auto it = this->items_1740.begin(); it != this->items_1740.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1740.erase(it);
      }
      for (auto it2 = this->items_1740.begin(); it2 != this->items_1740.end(); ++it2) {
        if (it2->unknown_0000[1] == item.unknown_0000[1]) {
          it2->unknown_0074[2] = 0;
          break;
        }
      }
      reinterpret_cast<CSession*>(this)->RefreshItem1740Window();
      return true;
    }
  }
  reinterpret_cast<CSession*>(this)->RefreshItem1740Window();
  return false;
}

// OFFSET: 0x00DDA740
// FUNCTION: fn_23342
bool ClientItemLists::RemoveItem1750(const ITEM_INFO& item, int type, unsigned char refresh) {
  for (auto it = this->items_1750.begin(); it != this->items_1750.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      if (type == 0) {
        it->quantity -= item.quantity;
        if (it->quantity < 1) {
          this->items_1750.erase(it);
        }
      } else {
        this->items_1750.erase(it);
      }

      if (refresh == 1) {
        std::uint32_t window;
        if (type == 1) {
          window = g_windowMgr.unknown_0410;
        } else {
          window = type == 2 ? g_windowMgr.unknown_0420 : g_windowMgr.unknown_02F8;
        }

        if (window) {
          reinterpret_cast<UIWindow*>(window)->SendMsg(nullptr, 23, 0, 0, 0, 0);
        }
      }
      return true;
    }
  }

  if (refresh == 1) {
    std::uint32_t window;
    if (type == 1) {
      window = g_windowMgr.unknown_0410;
    } else {
      window = type == 2 ? g_windowMgr.unknown_0420 : g_windowMgr.unknown_02F8;
    }

    if (window) {
      reinterpret_cast<UIWindow*>(window)->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }
  }
  return false;
}

// OFFSET: 0x00DDA8B0
// FUNCTION: fn_23343
void ClientItemLists::RemoveItem1758(ITEM_INFO& item) {
  for (auto it = this->items_1758.begin(); it != this->items_1758.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      item.itemId = it->itemId;
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1758.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA920
// FUNCTION: fn_23344
void ClientItemLists::RemoveItem1760(const ITEM_INFO& item) {
  for (auto it = this->items_1760.begin(); it != this->items_1760.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1760.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA9C0
// FUNCTION: fn_23345
void ClientItemLists::RemoveItem1718ByItemId(const ITEM_INFO& item) {
  for (auto it = this->items_1718.begin(); it != this->items_1718.end(); ++it) {
    if (std::atoi(it->itemId.c_str()) == std::atoi(item.itemId.c_str())) {
      if (it->quantity != -1) {
        it->quantity -= item.quantity;
        if (it->quantity < 1) {
          this->items_1718.erase(it);
        }
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAA50
// FUNCTION: fn_23346
void ClientItemLists::RemoveItem1718(const ITEM_INFO& item) {
  for (auto it = this->items_1718.begin(); it != this->items_1718.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      if (it->quantity != -1) {
        it->quantity -= item.quantity;
        if (it->quantity < 1) {
          this->items_1718.erase(it);
        }
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAAB0
// FUNCTION: fn_23347
void ClientItemLists::RemoveItem1720ByName(const ITEM_INFO& item) {
  for (auto it = this->items_1720.begin(); it != this->items_1720.end(); ++it) {
    if (it->itemId == item.itemId) {
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1720.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAB30
// FUNCTION: fn_23348
void ClientItemLists::RemoveItem1720(const ITEM_INFO& item) {
  const bool hasFlag = item.unknown_0074[3] != 0;
  for (auto it = this->items_1720.begin(); it != this->items_1720.end(); ++it) {
    const bool same = hasFlag ? it->unknown_0000[1] == item.unknown_0000[1] : it->itemId == item.itemId;
    if (same) {
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1720.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAC40
// FUNCTION: fn_23349
void ClientItemLists::RemoveItem1730(const ITEM_INFO& item) {
  for (auto it = this->items_1730.begin(); it != this->items_1730.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1730.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAC90
// FUNCTION: fn_23350
void ClientItemLists::ReduceItem1718(const ITEM_INFO& item) {
  for (auto it = this->items_1718.begin(); it != this->items_1718.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1718.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDACE0
// FUNCTION: fn_23351
bool ClientItemLists::RemoveItem1738(const ITEM_INFO& item) {
  for (auto it = this->items_1738.begin(); it != this->items_1738.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity < 1) {
        this->items_1738.erase(it);
      }
      for (auto it2 = this->items_1738.begin(); it2 != this->items_1738.end(); ++it2) {
        if (it2->unknown_0000[1] == item.unknown_0000[1]) {
          it2->unknown_0074[2] = 0;
          break;
        }
      }
      reinterpret_cast<CSession*>(this)->RefreshItem1738Window();
      return true;
    }
  }
  reinterpret_cast<CSession*>(this)->RefreshItem1738Window();
  return false;
}

// OFFSET: 0x00E2EC10
// FUNCTION: fn_24149
bool ClientItemLists::EquipItem6B68(const ITEM_INFO& item) {
  const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->items_6B68[slot];
      equipped = item;
      equipped.quantity = 1;
      equipped.unknown_0068 = item.unknown_0068;
      equipped.unknown_0064 = item.unknown_0064;
      equipped.unknown_0070 = item.unknown_0070;
    }
  }

  if (g_windowMgr.unknown_01E0) {
    static_cast<UIEquipWnd*>(g_windowMgr.unknown_01E0)->NotifyEquipmentChanged();
  }
  RefreshItemWindows(&g_windowMgr);
  return true;
}

// OFFSET: 0x00E2F540
// FUNCTION: fn_24155
void ClientItemLists::ResetItems6B68() {
  int count = 10;
  ITEM_INFO* item = this->items_6B68;
  do {
    item->Reset();
    ++item;
  } while (--count);
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E2F570
// FUNCTION: fn_24156
void ClientItemLists::ResetItems61B8() {
  int count = 10;
  ITEM_INFO* item = this->items_61B8;
  do {
    item->Reset();
    ++item;
  } while (--count);
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E2FC50
// FUNCTION: fn_24164
ITEM_INFO CSession::FindItemByIdentity(std::uint32_t identity, int location) {
  if (identity == 0) {
    return ITEM_INFO(0);
  }

  ITEM_INFO item = LookupClientItemByIdentity(this->m_inventoryItems, identity);
  if (item.unknown_0000[1] > 0) {
    return item;
  }

  const EquipSlots slots = g_session.GetEquipSlots(location);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      if (this->IsCostumeLocation(location)) {
        item = this->GetItem2B50(slot);
      } else {
        item = this->GetItem17F0(slot);
      }
      if (item.unknown_0000[1] == identity) {
        break;
      }
    }
  }

  if (item.unknown_0000[1] != 0) {
    return item;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E49F40
// FUNCTION: fn_24247
void ClientItemLists::SetItems1750(int count, const short* ids) {
  this->ClearItems1750();
  for (int i = 0; i < count; ++i) {
    const ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, ids[i]);
    if (item.unknown_0000[1] > 0) {
      this->items_1750.push_back(item);
    }
  }
}

// OFFSET: 0x00DFCBB0
// FUNCTION: fn_23600
bool ClientItemLists::AddExchangeItem(ITEM_INFO item) {
  int count = 0;
  for (int i = 0; i < 10; ++i) {
    if (this->exchangeItems[i].quantity != 0) {
      ++count;
    }
  }

  if (count == 10) {
    return false;
  }

  this->RemoveItem1710(item.unknown_0000[1], item.quantity);

  ITEM_INFO added = item;
  int index = 0;
  for (; index < 10; ++index) {
    if (this->exchangeItems[index].quantity != 0 && this->exchangeItems[index].unknown_0000[1] == added.unknown_0000[1]) {
      this->exchangeItems[index].quantity += added.quantity;
      break;
    }
  }

  if (index == 10) {
    for (int i = 0; i < 10; ++i) {
      if (this->exchangeItems[i].quantity == 0) {
        this->exchangeItems[i] = added;
        break;
      }
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(10011)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  return true;
}
