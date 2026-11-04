#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/ClientState.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/GuildInfo.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/PetEvolutionMgr.h"
#include "Contents/World/World.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <new>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

extern template std::list<ITEM_INFO>::iterator std::list<ITEM_INFO>::erase(std::list<ITEM_INFO>::const_iterator) noexcept;

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


int GetEquipLocationIndex(const int& mask);

namespace EquipmentProperties {
class CEquipment {
public:
  virtual ~CEquipment();
  virtual void Load(bool force);
  virtual void Clear();
  int& GetId();
};

class CEquipmentAttachable : public CEquipment {};

class CEquipmentWearable : public CEquipment {
public:
  bool IsActive();
  bool UpdateActive();
  std::vector<CEquipmentAttachable>& GetAttachables();
};

class CEquipmentPropertiesMgr {
public:
  virtual ~CEquipmentPropertiesMgr();
  unsigned char unknown_0004[0x9];
  bool m_dirty;
  unsigned char unknown_000E[0x12];
  UIWindow* unknown_0020;
  /* 0024 */ std::unordered_map<int, CEquipmentWearable*> properties1;
  /* 0044 */ unsigned char unknown_0044[0x1B8];

  CEquipmentPropertiesMgr();
  CEquipmentWearable* FindProperty1(const int& key);
  void AddExpansion(const int& kind, const int& itemId, bool notify);
  void EquipItem(const int& itemId, bool notify);
  __declspec(noinline) void EquipWearable(const ITEM_INFO* item, bool notify);
  void UnequipItem(const int& itemId, bool notify);
  void UnequipWearable(const int& location);
};
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentProperties::CEquipmentPropertiesMgr) == 0x1FC);
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
  g_session.AddItem1738(item, 1);
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
    g_modeMgr.gameMode->SendMsg(0x35, 0, 0, 0, 0);
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
    g_modeMgr.gameMode->SendMsg(0x35, 0, 0, 0, 0);
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
  int message;
  switch (packet->result) {
    case 0: {
      std::sprintf(buffer, LookupMessage(821), name);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 0xFFFF00, 0, 0, 0);

      MessengerMember member;
      member.name = name;
      member.accountId = packet->accountId;
      member.characterId = packet->characterId;
      member.offline = 0;
      g_session.AddFriend(member);
      g_session.RefreshWindow02C8();
      return;
    }
    case 1:
      message = 822;
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(819)), 0xFF, 0, 0, 0);
      return;
    case 3:
      message = 820;
      break;
    case 4:
      message = 2737;
      break;
    case 6:
      message = 3448;
      break;
    case 7:
      message = 3449;
      break;
    case 8:
      message = 3450;
      break;
    default:
      return;
  }

  std::sprintf(buffer, LookupMessage(message), name);
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
  g_session.AddItem1740(item);
}

// OFFSET: 0x00D1FF70
// FUNCTION: fn_21645
void CGameMode::ReceiveItem1740AddOptions(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddOptionsPacket*>(data);
  ItemRandomOption options[5];
  std::memcpy(options, packet->options, sizeof(options));
  ITEM_INFO item;
  item.unknown_0000[1] = packet->index;
  item.quantity = packet->amount;
  item.SetItemId(packet->itemId);
  item.identified = packet->identified;
  item.unknown_005D = packet->damaged;
  item.unknown_0060 = packet->refine;
  item.unknown_0000[0] = packet->type;
  std::memcpy(item.unknown_001C, packet->cards, sizeof(packet->cards));
  item.unknown_0098 = CountItemOptions(options);
  for (int index = 0; index < item.unknown_0098; ++index) {
    reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
  }
  item.unknown_0088 = 0;
  g_session.AddItem1740(item);
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
  g_session.AddItem1740(item);
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
  g_session.AddItem1738(item, 1);
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
  g_session.AddItem1738(item, 1);
}

namespace {
#pragma pack(push, 1)
  struct EquipItemListHeader {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t length;
    /* 0004 */ unsigned char listType;
  };

  struct EquipItemListRecord {
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
    /* 0028 */ ItemRandomOption options[5];
    /* 0041 */ unsigned char refine;
    /* 0042 */ unsigned char grade;
    /* 0043 */ unsigned char flags;
  };
#pragma pack(pop)

  static_assert(sizeof(EquipItemListHeader) == 5);
  static_assert(sizeof(EquipItemListRecord) == 0x44);

  // INLINED
  __forceinline void FillEquipItemListItem(ITEM_INFO& item, const EquipItemListRecord& record, const ItemRandomOption* options) {
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
    item.unknown_0098 = CountItemOptions(options);
    item.unknown_0088 = record.grade;
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<ItemRandomOption*>(item.unknown_009C)[index] = options[index];
    }
  }
}

// OFFSET: 0x00D43940
// FUNCTION: fn_21832
void CGameMode::ReceiveEquipItemList(const unsigned char* packet) {
  const EquipItemListHeader* header = reinterpret_cast<const EquipItemListHeader*>(packet);
  const EquipItemListRecord* records = reinterpret_cast<const EquipItemListRecord*>(packet + sizeof(EquipItemListHeader));
  const std::size_t count = (header->length - sizeof(EquipItemListHeader)) / sizeof(EquipItemListRecord);
  switch (header->listType) {
  case 2:
  case 3:
    for (std::int64_t i = 0; i < count; ++i) {
      const EquipItemListRecord record = records[i];
      if (record.location == 0x8000) {
        continue;
      }

      ItemRandomOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipItemListItem(item, record, options);
      g_session.AddItem1738(item, 0);
    }
    break;
  case 0:
    for (std::int64_t i = 0; i < count; ++i) {
      const EquipItemListRecord record = records[i];
      ItemRandomOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipItemListItem(item, record, options);

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
    break;
  case 1:
    for (std::int64_t i = 0; i < count; ++i) {
      const EquipItemListRecord record = records[i];
      ItemRandomOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipItemListItem(item, record, options);
      g_session.AddItem1740(item);
    }
    break;
  }
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
      g_session.AddItem1740(item);
      break;
    case 2:
    case 3:
      g_session.AddItem1738(item, 0);
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
__declspec(noinline) bool CSession::AddItem1710(const ITEM_INFO& item, unsigned char refresh) {
  for (ITEM_INFO& value : this->m_inventoryItems) {
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

  this->m_inventoryItems.push_back(item);
  if (refresh) {
    RefreshItemWindows(&g_windowMgr);
  }
  return true;
}

// OFFSET: 0x00DD7210
// FUNCTION: fn_23284
bool CSession::AddItem1768(const ITEM_INFO& item, unsigned char alternate, unsigned char refresh) {
  for (ITEM_INFO& value : this->unknown_1768) {
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

  this->unknown_1768.push_back(item);
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
bool CSession::AddItem1740(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1740) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02F4)) {
        window->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
      return false;
    }
  }

  this->unknown_1740.push_back(item);
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02F4)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DD7350
// FUNCTION: fn_23286
bool CSession::AddItem1750(const ITEM_INFO& item, int type, unsigned char refresh) {
  for (ITEM_INFO& value : this->unknown_1750) {
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

  this->unknown_1750.push_back(item);
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
void CSession::AddItem1758(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1758) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->unknown_1758.push_back(item);
}

// OFFSET: 0x00DD7470
// FUNCTION: fn_23288
void CSession::AddItem1760(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1760) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->unknown_1760.push_back(item);
}

// OFFSET: 0x00DD74C0
// FUNCTION: fn_23289
void CSession::AddItem1748(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1748) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->unknown_1748.push_back(item);
}

// OFFSET: 0x00DD7510
// FUNCTION: fn_23290
bool CSession::Equip(const ITEM_INFO& item) {
  const EquipSlots slots = reinterpret_cast<CSession*>(this)->GetEquipSlots(item.unknown_0000[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->unknown_21A0[slot];
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
void CSession::AddItem1718(const RawItemBasic& item, int quantity, std::uint16_t unknown_006C) {
  ITEM_INFO value;
  value.AssignBasic(item);
  value.quantity = quantity;
  value.unknown_006C = unknown_006C;
  value.unknown_0060 = 0;
  value.unknown_005D = 0;
  value.SetItemId(std::atoi(value.itemId.c_str()));
  this->unknown_1718.push_back(value);
}

// OFFSET: 0x00DD76C0
// FUNCTION: fn_23292
void CSession::AppendItem1718(const ITEM_INFO& item) {
  this->unknown_1718.push_back(item);
}

// OFFSET: 0x00DD76D0
// FUNCTION: fn_23293
void CSession::MergeItem1718ByName(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1718) {
    if (value.itemId == item.itemId) {
      value.quantity = value.quantity == -1 ? -1 : value.quantity + item.quantity;
      return;
    }
  }

  this->unknown_1718.push_back(item);
}

// OFFSET: 0x00DD7770
// FUNCTION: fn_23294
void CSession::MergeItem1720ByName(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1720) {
    if (value.itemId == item.itemId) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->unknown_1720.push_back(item);
}

// OFFSET: 0x00DD77F0
// FUNCTION: fn_23295
void CSession::MergeItem1720(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1720) {
    const bool same = item.unknown_0074[3] ? value.unknown_0000[1] == item.unknown_0000[1] : value.itemId == item.itemId;
    if (same) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->unknown_1720.push_back(item);
}

// OFFSET: 0x00DD7890
// FUNCTION: fn_23296
void CSession::AddItem1730(const ITEM_INFO& item) {
  const ITEM_INFO owned = LookupClientItemByIdentity(this->m_inventoryItems, item.unknown_0000[1]);
  if (owned.unknown_0000[1] > 0) {
    for (ITEM_INFO& value : this->unknown_1730) {
      if (value.unknown_0000[1] == item.unknown_0000[1]) {
        const int quantity = value.quantity + item.quantity;
        if (owned.quantity >= quantity) {
          value.quantity = quantity;
        }
        return;
      }
    }

    if (owned.quantity >= item.quantity) {
      this->unknown_1730.push_back(item);
    }
  }
}

// OFFSET: 0x00DD7970
// FUNCTION: fn_23297
void CSession::AddItem1718(const RawItemBasic& item, int mode) {
  if (mode != 0) {
    const ITEM_INFO owned = LookupClientItemByIdentity(this->m_inventoryItems, item.id);
    if (owned.unknown_0000[1] != 0) {
      ITEM_INFO value(owned);
      value.AssignBasic(item);
      value.SetItemId(std::atoi(value.itemId.c_str()));
      this->unknown_1718.push_back(value);
    }
  } else {
    ITEM_INFO value;
    value.AssignBasic(item);
    value.unknown_0060 = 0;
    value.unknown_005D = 0;
    value.SetItemId(std::atoi(value.itemId.c_str()));
    value.unknown_0000[2] = item.unknown_0034;
    value.unknown_0070 = item.unknown_0038;
    this->unknown_1718.push_back(value);
  }
}

// OFFSET: 0x00DD7B30
// FUNCTION: fn_23298
void CSession::MergeItem1718(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->unknown_1718) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      if (value.quantity != -1) {
        value.quantity += item.quantity;
      }
      return;
    }
  }

  this->unknown_1718.push_back(item);
}

// OFFSET: 0x00DD7B80
// FUNCTION: fn_23299
bool CSession::AddItem1738(const ITEM_INFO& item, unsigned char refresh) {
  if (g_replayAnimationScaling && !Replay::Instance().header.options.Get(ReplayRecordOption::Item)) {
    return true;
  }

  for (ITEM_INFO& value : this->unknown_1738) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      if (refresh && g_windowMgr.window_0288) {
        g_windowMgr.window_0288->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
      return false;
    }
  }

  this->unknown_1738.push_back(item);
  if (refresh && g_windowMgr.window_0288) {
    g_windowMgr.window_0288->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DD6D70
// FUNCTION: fn_23280
bool CSession::EquipUnknown17F0(const ITEM_INFO& item, const char* name) {
  const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1 && slot < 10) {
      ITEM_INFO& equipped = this->unknown_17F0[slot];
      equipped = item;
      equipped.quantity = 1;
    }
  }

  RequestBasicInfoRefresh();
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->EquipWearable(&item, *name == '\0');
  g_windowMgr.InvalidateFrames();
  return true;
}

// OFFSET: 0x00DFC9B0
// FUNCTION: fn_23599
bool CSession::OnItemEquipped(ITEM_INFO& item) {
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s %s", item.DisplayName(0).c_str(), LookupMessage(0xAA)).c_str()), 0xC8FF00, 7, 0, 0);
  if (item.unknown_0064 == 1) {
    item.unknown_0064 = 2;
  }

  if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
    char name[1] = "";
    g_session.EquipItem2B50(item, name);
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
    const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

    for (int index = 0; index < 3; ++index) {
      const int slot = order[index];
      if (slot != -1 && slot < 10) {
        ITEM_INFO& equipped = this->unknown_17F0[slot];
        equipped = item;
        equipped.quantity = 1;
      }
    }

    RequestBasicInfoRefresh();
    if (g_equipmentPropertiesMgr == nullptr) {
      g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
    }
    g_equipmentPropertiesMgr->EquipWearable(&item, true);
    g_windowMgr.InvalidateFrames();
  }

  if (item.unknown_0000[3] != 0x8000) {
    this->RemoveItem1710(item.unknown_0000[1], 1);
  }
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_022C)) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  return true;
}

void EquipmentProperties::CEquipmentPropertiesMgr::UnequipWearable(const int& location) {
  this->m_dirty = true;
  auto found = this->properties1.find(GetEquipLocationIndex(location));
  if (found != this->properties1.end()) {
    CEquipmentWearable*& wearable = found->second;
    for (CEquipmentAttachable& attachable : wearable->GetAttachables()) {
      this->UnequipItem(attachable.GetId(), true);
    }
    this->UnequipItem(wearable->GetId(), true);
    wearable->Clear();
    delete wearable;
    this->properties1.erase(found);
  }

  if (this->unknown_0020 != nullptr) {
    this->unknown_0020->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFC620
// FUNCTION: fn_23598
bool CSession::UnequipItem(int location, std::uint32_t identity) {
  if (location == 0x8000) {
    const ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, identity);
    if (item.unknown_0000[1] != 0) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), LookupMessage(0xAB)).c_str()), 0xFF, 7, 0, 0);
    }
    this->unknown_16EC = 0;
    g_windowMgr.InvalidateFrames();

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode != nullptr) {
      auto it = g_activeSkillEffects.begin();
      for (; it != g_activeSkillEffects.end(); ++it) {
        if (it->id == 0x2B7) {
          break;
        }
      }
      if (it != g_activeSkillEffects.end()) {
        g_activeSkillEffects.erase(it);
      }

      if (mode->scene && mode->scene->player && mode->scene->player->unknown_0544) {
        mode->scene->player->unknown_0544->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
      }
    }
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    if (slots.values[2] < 10) {
      if (slots.values[2] != -1) {
        ITEM_INFO& item = this->unknown_17F0[slots.values[2]];
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), LookupMessage(0xAB)).c_str()), 0xFF, 7, 0, 0);
        item.unknown_0000[3] = 0;

        bool merged = false;
        for (ITEM_INFO& value : this->m_inventoryItems) {
          if (value.unknown_0000[1] == item.unknown_0000[1]) {
            value.quantity += item.quantity;
            value.unknown_0074[1] = item.unknown_0074[1];
            value.unknown_0074[0] = item.unknown_0074[0];
            merged = true;
            break;
          }
        }
        if (!merged) {
          this->m_inventoryItems.push_back(item);
        }

        RefreshItemWindows(&g_windowMgr);
        item.Reset();
      }
      if (slots.values[1] != -1 && slots.values[1] < 10) {
        this->unknown_17F0[slots.values[1]].Reset();
      }
      if (slots.values[0] != -1 && slots.values[0] < 10) {
        this->unknown_17F0[slots.values[0]].Reset();
      }
    }
    RequestBasicInfoRefresh();
  }

  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->UnequipWearable(location);
  g_windowMgr.InvalidateFrames();
  if (UIWindow* window = g_windowMgr.unknown_022C) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
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
void CSession::ClearInventory() {
  this->m_inventoryItems.erase(this->m_inventoryItems.begin(), this->m_inventoryItems.end());
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00DD88C0
// FUNCTION: fn_23314
void CSession::ClearItems1760And1748() {
  this->unknown_1760.erase(this->unknown_1760.begin(), this->unknown_1760.end());
  this->unknown_1748.erase(this->unknown_1748.begin(), this->unknown_1748.end());
}

// OFFSET: 0x00DD8900
// FUNCTION: fn_23315
void CSession::ClearItems1768() {
  this->unknown_1768.erase(this->unknown_1768.begin(), this->unknown_1768.end());
}

// OFFSET: 0x00DD8920
// FUNCTION: fn_23316
void CSession::ClearItems1740() {
  this->unknown_1740.erase(this->unknown_1740.begin(), this->unknown_1740.end());
}

// OFFSET: 0x00DD8940
// FUNCTION: fn_23317
__declspec(noinline) void CSession::ClearItems1750() {
  this->unknown_1750.erase(this->unknown_1750.begin(), this->unknown_1750.end());
}

// OFFSET: 0x00DD8960
// FUNCTION: fn_23318
void CSession::ClearItems1758() {
  this->unknown_1758.erase(this->unknown_1758.begin(), this->unknown_1758.end());
}

// OFFSET: 0x00DD8980
// FUNCTION: fn_23319
void CSession::ClearItems1760() {
  this->unknown_1760.erase(this->unknown_1760.begin(), this->unknown_1760.end());
}

// OFFSET: 0x00DD89A0
// FUNCTION: fn_23320
void CSession::ClearItems1748() {
  this->unknown_1748.erase(this->unknown_1748.begin(), this->unknown_1748.end());
}

// OFFSET: 0x00DD89C0
// FUNCTION: fn_23321
void CSession::ClearRecords0734() {
  this->unknown_0734.clear();
}

// OFFSET: 0x00DD8A20
// FUNCTION: fn_23322
void CSession::ClearItems1784(int index) {
  if (index >= 0 && static_cast<unsigned>(index) < 11) {
    this->unknown_1784[index].erase(this->unknown_1784[index].begin(), this->unknown_1784[index].end());
  }
}

// OFFSET: 0x00DD8A50
// FUNCTION: fn_23323
void CSession::ClearItemSlotQuantities() {
  this->unknown_5214 = 0;

  for (int index = 0; index < 10; ++index) {
    this->m_itemSlots[index].quantity = 0;
  }
}

// OFFSET: 0x00DD8AC0
// FUNCTION: fn_23324
void CSession::ResetEquipped() {
  for (int index = 0; index < 10; ++index) {
    this->unknown_21A0[index].Reset();
  }
}

// OFFSET: 0x00DD8AF0
// FUNCTION: fn_23325
void CSession::ClearPartyMembers() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (mode) {
    for (auto member = this->m_partyMembers.begin(); member != this->m_partyMembers.end(); ++member) {
      CRenderObject* actor = mode->scene->GetGameActorByAidIncludeMe(member->accountId);
      if (actor && static_cast<EffectActor*>(actor)->HasId(160) == true) {
        actor->SendMsg(nullptr, 109, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
  }

  this->m_partyMembers.clear();
  this->unknown_5464 = 0;
  this->unknown_54C4 = "";
}

// OFFSET: 0x00DD8BD0
// FUNCTION: fn_23326
void CSession::ResetUnknown166C() {
  this->unknown_166C = 0;
  this->unknown_167C = 0;
  this->unknown_1680 = 0;
  this->unknown_1670 = 0;
  this->unknown_1674 = 0;
  this->unknown_1678 = 0;
}

// OFFSET: 0x00DD8C10
// FUNCTION: fn_23327
void CSession::ClearItems1720() {
  this->unknown_1720.erase(this->unknown_1720.begin(), this->unknown_1720.end());
}

// OFFSET: 0x00DD8C70
// FUNCTION: fn_23328
void CSession::ResetHotkeyDescriptions() {
  for (int index = 0; index < 9; ++index) {
    this->m_hotkeyDescriptions[index] = "nothing";
  }
}

// OFFSET: 0x00DD8CA0
// FUNCTION: fn_23329
void CSession::ClearItems1738() {
  this->unknown_1738.erase(this->unknown_1738.begin(), this->unknown_1738.end());
}

// OFFSET: 0x00DD8CC0
// FUNCTION: fn_23330
void CSession::CopyItems1740To1750() {
  this->unknown_1750.erase(this->unknown_1750.begin(), this->unknown_1750.end());

  for (auto item = this->unknown_1740.begin(); item != this->unknown_1740.end(); ++item) {
    this->unknown_1750.push_back(*item);
  }
}

// OFFSET: 0x00DD8DB0
// FUNCTION: fn_23331
void CSession::CopyItems1760To1770() {
  this->unknown_1770.erase(this->unknown_1770.begin(), this->unknown_1770.end());

  for (auto item = this->unknown_1760.begin(); item != this->unknown_1760.end(); ++item) {
    this->unknown_1770.push_back(*item);
  }
}

// OFFSET: 0x00DD8EA0
// FUNCTION: fn_23332
std::pair<bool, ITEM_INFO> CSession::FindItem1750(const ITEM_INFO& item, unsigned char) {
  auto found = this->unknown_1750.begin();

  for (; found != this->unknown_1750.end(); ++found) {
    if (IsSameItem(*found, item)) {
      break;
    }
  }

  return found != this->unknown_1750.end() ? std::make_pair(true, *found) : std::make_pair(false, ITEM_INFO());
}

// OFFSET: 0x00DD91C0
// FUNCTION: fn_23333
int CSession::IsDifferentName(const char* name, const char* other) {
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
void CSession::RemoveItem1758ByItemId(ITEM_INFO& item) {
  for (auto it = this->unknown_1758.begin(); it != this->unknown_1758.end(); ++it) {
    if (std::atoi(it->itemId.c_str()) == std::atoi(item.itemId.c_str())) {
      item.itemId = it->itemId;
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1758.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA510
// FUNCTION: fn_23339
void CSession::RemoveItem1710(std::uint32_t id, int quantity) {
  for (auto it = this->m_inventoryItems.begin(); it != this->m_inventoryItems.end(); ++it) {
    if (it->unknown_0000[1] == id) {
      if (quantity < it->quantity) {
        it->quantity -= quantity;
      } else {
        this->m_inventoryItems.erase(it);
      }
      for (auto it2 = this->m_inventoryItems.begin(); it2 != this->m_inventoryItems.end(); ++it2) {
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
bool CSession::RemoveItem1768(const ITEM_INFO& item, unsigned char alternate) {
  int index = 0;
  for (auto it = this->unknown_1768.begin(); it != this->unknown_1768.end(); ++it, ++index) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      if (!alternate) {
        it->quantity -= item.quantity;
        if (it->quantity <= 0) {
          this->unknown_1768.erase(it);
          reinterpret_cast<CSession*>(this)->RefreshItem1768Window(0, index + 1);
          return true;
        }
        reinterpret_cast<CSession*>(this)->RefreshItem1768Window(0, 0);
        return true;
      }
      this->unknown_1768.erase(it);
      reinterpret_cast<CSession*>(this)->RefreshItem1768Window(1, 0);
      return true;
    }
  }
  reinterpret_cast<CSession*>(this)->RefreshItem1768Window(alternate, 0);
  return false;
}

// OFFSET: 0x00DDA690
// FUNCTION: fn_23341
bool CSession::RemoveItem1740(const ITEM_INFO& item) {
  for (auto it = this->unknown_1740.begin(); it != this->unknown_1740.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1740.erase(it);
      }
      for (auto it2 = this->unknown_1740.begin(); it2 != this->unknown_1740.end(); ++it2) {
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

// OFFSET: 0x00DDA8B0
// FUNCTION: fn_23343
void CSession::RemoveItem1758(ITEM_INFO& item) {
  for (auto it = this->unknown_1758.begin(); it != this->unknown_1758.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      item.itemId = it->itemId;
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1758.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA920
// FUNCTION: fn_23344
void CSession::RemoveItem1760(const ITEM_INFO& item) {
  for (auto it = this->unknown_1760.begin(); it != this->unknown_1760.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1760.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA9C0
// FUNCTION: fn_23345
void CSession::RemoveItem1718ByItemId(const ITEM_INFO& item) {
  for (auto it = this->unknown_1718.begin(); it != this->unknown_1718.end(); ++it) {
    if (std::atoi(it->itemId.c_str()) == std::atoi(item.itemId.c_str())) {
      if (it->quantity != -1) {
        it->quantity -= item.quantity;
        if (it->quantity <= 0) {
          this->unknown_1718.erase(it);
        }
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAA50
// FUNCTION: fn_23346
void CSession::RemoveItem1718(const ITEM_INFO& item) {
  for (auto it = this->unknown_1718.begin(); it != this->unknown_1718.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      if (it->quantity != -1) {
        it->quantity -= item.quantity;
        if (it->quantity <= 0) {
          this->unknown_1718.erase(it);
        }
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAAB0
// FUNCTION: fn_23347
void CSession::RemoveItem1720ByName(const ITEM_INFO& item) {
  for (auto it = this->unknown_1720.begin(); it != this->unknown_1720.end(); ++it) {
    if (it->itemId == item.itemId) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1720.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAC40
// FUNCTION: fn_23349
void CSession::RemoveItem1730(const ITEM_INFO& item) {
  for (auto it = this->unknown_1730.begin(); it != this->unknown_1730.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1730.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAC90
// FUNCTION: fn_23350
void CSession::ReduceItem1718(const ITEM_INFO& item) {
  for (auto it = this->unknown_1718.begin(); it != this->unknown_1718.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1718.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDACE0
// FUNCTION: fn_23351
bool CSession::RemoveItem1738(const ITEM_INFO& item) {
  for (auto it = this->unknown_1738.begin(); it != this->unknown_1738.end(); ++it) {
    if (it->unknown_0000[1] == item.unknown_0000[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->unknown_1738.erase(it);
      }
      for (auto it2 = this->unknown_1738.begin(); it2 != this->unknown_1738.end(); ++it2) {
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
bool CSession::EquipItem6B68(const ITEM_INFO& item) {
  const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->unknown_6B68[slot];
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
void CSession::SetItems1750(int count, const short* ids) {
  this->ClearItems1750();
  for (int i = 0; i < count; ++i) {
    const ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, ids[i]);
    if (item.unknown_0000[1] > 0) {
      this->unknown_1750.push_back(item);
    }
  }
}

// OFFSET: 0x00DF9560
// FUNCTION: fn_23520
int CSession::IsGuildMemberNameDifferent(const char* name) {
  std::list<GuildInfo::Member> members = g_guildInfo.m_members;
  for (auto it = members.begin(); it != members.end(); ++it) {
    if (this->IsDifferentName(name, it->name.c_str()) == 0) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00DF9690
// FUNCTION: fn_23521
int CSession::IsFriendNameDifferent(const char* name) {
  for (auto it = this->m_friends.begin(); it != this->m_friends.end(); ++it) {
    if (this->IsDifferentName(name, it->name.c_str()) == 0) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00DFCBB0
// FUNCTION: fn_23600
bool CSession::AddExchangeItem(ITEM_INFO item) {
  int count = 0;
  for (int i = 0; i < 10; ++i) {
    if (this->m_exchangeItems[i].quantity != 0) {
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
    if (this->m_exchangeItems[index].quantity != 0 && this->m_exchangeItems[index].unknown_0000[1] == added.unknown_0000[1]) {
      this->m_exchangeItems[index].quantity += added.quantity;
      break;
    }
  }

  if (index == 10) {
    for (int i = 0; i < 10; ++i) {
      if (this->m_exchangeItems[i].quantity == 0) {
        this->m_exchangeItems[i] = added;
        break;
      }
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(10011)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  return true;
}

// OFFSET: 0x00D1F550
// FUNCTION: fn_21639
void CGameMode::ReceiveShowRankingSchedule(const unsigned char* packet) {
  if (packet) {
    g_session.SetShowRankingSchedule(true, packet);
  }
}
