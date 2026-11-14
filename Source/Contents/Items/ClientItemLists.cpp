#include "Network/Packets/NoticeEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/CombatTypes.h"
#include "Contents/Items/InventoryType.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/ClientState.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "Contents/World/World.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
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

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

namespace {
  struct EquipSlotDecoder {
    EquipSlots Decode(int mask) const;
  };

#pragma pack(push, 1)
  using ItemRandomOption = ItemOptionRecord;

  struct ItemRandomOptions {
    /* 0000 */ ItemRandomOption values[5];
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
    /* 0020 */ ItemRandomOptions options;
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
    /* 001F */ ItemRandomOptions options;
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
    const auto* valueCards = value.cards;
    const auto* itemCards = item.cards;

    if (value.itemId != item.itemId || valueCards[0] != itemCards[0] || valueCards[1] != itemCards[1] ||
        valueCards[2] != itemCards[2] || valueCards[3] != itemCards[3] || value.identified != item.identified ||
        value.refiningLevel != item.refiningLevel || value.grade != item.grade) {
      return false;
    }

    const int count = item.optionCount;

    if (value.optionCount != count || value.optionCount >= 5) {
      return false;
    }

    if (value.optionCount != 0) {
      const auto* left = value.options;
      const auto* right = item.options;

      for (int index = 0; index < count; ++index) {
        if (left[index].id != right[index].id || left[index].value != right[index].value || left[index].parameter != right[index].parameter) {
          return false;
        }
      }
    }

    return true;
  }

  // INLINED
  void CountSameNameChar(const char& left, const char& right, short& matched) {
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
  unsigned char padding_0004[0x9];
  bool m_dirty;
  unsigned char padding_000E[0x12];
  UIWindow* m_window;
  /* 0024 */ std::unordered_map<int, CEquipmentWearable*> properties1;
  /* 0044 */ unsigned char padding_0044[0x1B8];

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
  if (mgr->itemWnd) {
    static_cast<UIWindow*>(mgr->itemWnd)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (mgr->shortCutWnd) {
    mgr->shortCutWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (UIWindow* window = mgr->LookupWindow(WID_QUIKSLOTWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  if (mgr->barterMarketWnd) {
    mgr->barterMarketWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// INLINED
static int CountItemOptions(const ItemRandomOption* options) {
  int count = 0;
  for (int index = 0; index < 5; ++index) {
    if (0 != options[index].id) {
      ++count;
    }
  }
  return count;
}

// INLINED
static __forceinline void AddCartPacketItem(std::int16_t index, int amount, int itemId, unsigned char type,
                                            unsigned char identified, unsigned char damaged, unsigned char refine,
                                            std::uint32_t card1, std::uint32_t card2, std::uint32_t card3,
                                            std::uint32_t card4, ItemRandomOptions options, int grade) {
  ITEM_INFO item;
  item.identity[1] = index;
  item.quantity = amount;
  item.SetItemId(itemId);
  item.identified = identified;
  item.damaged = damaged;
  item.refiningLevel = refine;
  item.identity[0] = type;
  std::uint32_t* cards = item.cards;
  cards[0] = card1;
  cards[1] = card2;
  cards[2] = card3;
  cards[3] = card4;
  item.optionCount = CountItemOptions(options.values);
  for (int index = 0; index < item.optionCount; ++index) {
    item.options[index] = options.values[index];
  }
  item.grade = static_cast<std::int16_t>(grade);
  g_session.AddCartItem(item, 1);
}

// INLINED
static __forceinline void AddMerchantShopPacketItem(std::int16_t index, int amount, int itemId, unsigned char type,
                                                    unsigned char identified, unsigned char damaged,
                                                    unsigned char refine, std::uint32_t card1, std::uint32_t card2,
                                                    std::uint32_t card3, std::uint32_t card4,
                                                    ItemRandomOptions options, int grade) {
  ITEM_INFO item;
  item.identity[1] = index;
  item.quantity = amount;
  item.SetItemId(itemId);
  item.identified = identified;
  item.damaged = damaged;
  item.refiningLevel = refine;
  item.identity[0] = type;
  std::uint32_t* cards = item.cards;
  cards[0] = card1;
  cards[1] = card2;
  cards[2] = card3;
  cards[3] = card4;
  item.optionCount = CountItemOptions(options.values);
  for (int index = 0; index < item.optionCount; ++index) {
    item.options[index] = options.values[index];
  }
  item.grade = static_cast<std::int16_t>(grade);
  g_session.AddMerchantShopItem(item);
}

// OFFSET: 0x00D1F5A0
// FUNCTION: fn_21640
void CGameMode::ReceiveItem1738Add(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddPacket*>(data);
  ItemRandomOptions options = {};
  AddCartPacketItem(packet->index, packet->amount, packet->itemId, packet->type, packet->identified, packet->damaged,
                    packet->refine, packet->cards[0], packet->cards[1], packet->cards[2], packet->cards[3], options, 0);
}

#pragma pack(push, 1)
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
    /* 001E */ ItemRandomOptions options;
  };
#pragma pack(pop)

// INLINED
static __forceinline void AddExchangePacketItem(int itemId, unsigned char type, int slot, unsigned char identified,
                                                unsigned char damaged, unsigned char refine, std::uint32_t card1,
                                                std::uint32_t card2, std::uint32_t card3, std::uint32_t card4,
                                                ItemRandomOptions options) {
  if (slot < 0) {
    g_modeMgr.gameMode->SendMsg(MM_CANCEL_EXCHANGE_ITEM, 0, 0, 0, 0);
  }

  if (itemId == 0) {
    g_session.SetOtherExchangeMoney(slot);
    return;
  }

  ITEM_INFO item;
  item.identity[0] = type;
  item.SetItemId(itemId);
  item.damaged = damaged;
  item.identified = identified;
  item.refiningLevel = refine;
  std::uint32_t* cards = item.cards;
  cards[0] = card1;
  cards[1] = card2;
  cards[2] = card3;
  cards[3] = card4;
  item.identity[1] = 0xFFFFFFFF;
  item.quantity = slot;
  item.optionCount = CountItemOptions(options.values);
  for (int index = 0; index < item.optionCount; ++index) {
    item.options[index] = options.values[index];
  }
  g_session.AddItemSlot(item);
}

// OFFSET: 0x00D1F770
// FUNCTION: fn_21641
void CGameMode::ReceiveItemSlot(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddSlotPacket*>(data);
  ItemRandomOptions options = {};
  AddExchangePacketItem(packet->itemId, packet->type, packet->slot, packet->identified, packet->damaged,
                        packet->refine, packet->cards[0], packet->cards[1], packet->cards[2], packet->cards[3],
                        options);
}

// OFFSET: 0x00D1F970
// FUNCTION: fn_21642
void CGameMode::ReceiveItemSlotOptions(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddSlotOptionsPacket*>(data);
  AddExchangePacketItem(packet->itemId, packet->type, packet->slot, packet->identified, packet->damaged,
                        packet->refine, packet->cards[0], packet->cards[1], packet->cards[2], packet->cards[3],
                        packet->options);
}

// OFFSET: 0x00D1FBA0
// FUNCTION: fn_21643
void CGameMode::ReceiveFriendAddResult(const unsigned char* data) {
  const FriendNoticePacket* packet = reinterpret_cast<const FriendNoticePacket*>(data);
  char name[25];
  std::memcpy(name, packet->name, 24);
  name[24] = '\0';

  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);

  char buffer[128];
  switch (packet->result) {
    case FRIEND_ADD_FALSE:
      std::sprintf(buffer, LookupMessage(MSI_FRIEND_ADD_FALSE), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0xFF, 0, 0, 0);
      break;
    case FRIEND_ADD_SUCCEED: {
      std::sprintf(buffer, LookupMessage(MSI_FRIEND_ADD_SUCCEED), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0xFFFF00, 0, 0, 0);

      MessengerMember member;
      member.name = name;
      member.accountId = packet->accountId;
      member.characterId = packet->characterId;
      member.offline = 0;
      g_session.AddFriend(member);
      g_session.RefreshMessengerGroupWindow();
      break;
    }
    case FRIEND_ADD_ME_OVERSIZE:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_FRIEND_ADD_ME_OVERSIZE)), 0xFF, 0, 0, 0);
      break;
    case FRIEND_ADD_OTHER_OVERSIZE:
      std::sprintf(buffer, LookupMessage(MSI_FRIEND_ADD_OTHER_OVERSIZE), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0xFF, 0, 0, 0);
      break;
    case 4:
      std::sprintf(buffer, LookupMessage(2737), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0xFF, 0, 0, 0);
      break;
    case 6:
      std::sprintf(buffer, LookupMessage(3448), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0xFF, 0, 0, 0);
      break;
    case 7:
      std::sprintf(buffer, LookupMessage(3449), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0xFF, 0, 0, 0);
      break;
    case 8:
      std::sprintf(buffer, LookupMessage(3450), name);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0xFF, 0, 0, 0);
      break;
  }
}

// OFFSET: 0x00D1FDA0
// FUNCTION: fn_21644
void CGameMode::ReceiveItem1740Add(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddPacket*>(data);
  ItemRandomOptions options = {};
  AddMerchantShopPacketItem(packet->index, packet->amount, packet->itemId, packet->type, packet->identified,
                            packet->damaged, packet->refine, packet->cards[0], packet->cards[1], packet->cards[2],
                            packet->cards[3], options, 0);
}

// OFFSET: 0x00D1FF70
// FUNCTION: fn_21645
void CGameMode::ReceiveItem1740AddOptions(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddOptionsPacket*>(data);
  AddMerchantShopPacketItem(packet->index, packet->amount, packet->itemId, packet->type, packet->identified,
                            packet->damaged, packet->refine, packet->cards[0], packet->cards[1], packet->cards[2],
                            packet->cards[3], packet->options, 0);
}

// OFFSET: 0x00D20180
// FUNCTION: fn_21646
void CGameMode::ReceiveItem1740AddGrade(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddGradePacket*>(data);
  AddMerchantShopPacketItem(packet->index, packet->amount, packet->itemId, packet->type, packet->identified,
                            packet->damaged, packet->refine, packet->cards[0], packet->cards[1], packet->cards[2],
                            packet->cards[3], packet->options, packet->grade);
}

// OFFSET: 0x00D204B0
// FUNCTION: fn_21647
void CGameMode::ReceiveItem1738AddOptions(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddOptionsPacket*>(data);
  AddCartPacketItem(packet->index, packet->amount, packet->itemId, packet->type, packet->identified, packet->damaged,
                    packet->refine, packet->cards[0], packet->cards[1], packet->cards[2], packet->cards[3],
                    packet->options, 0);
}

// OFFSET: 0x00D206C0
// FUNCTION: fn_21648
void CGameMode::ReceiveItem1738AddGrade(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ItemAddGradePacket*>(data);
  AddCartPacketItem(packet->index, packet->amount, packet->itemId, packet->type, packet->identified, packet->damaged,
                    packet->refine, packet->cards[0], packet->cards[1], packet->cards[2], packet->cards[3],
                    packet->options, packet->grade);
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
    item.identity[0] = record.type;
    item.identity[1] = record.index;
    item.identity[2] = record.location;
    item.identity[3] = record.wearState;
    item.quantity = 1;
    std::memcpy(item.cards, record.cards, sizeof(record.cards));
    item.SetItemId(record.itemId);
    item.identified = record.flags & 1;
    item.damaged = (record.flags >> 1) & 1;
    item.refiningLevel = record.refine;
    item.expireTime = record.expireTime;
    item.bindOnEquip = record.bindOnEquip;
    item.sprite = record.sprite;
    item.flags[0] = (record.flags >> 2) & 1;
    item.optionCount = CountItemOptions(options);
    item.grade = record.grade;
    for (int index = 0; index < item.optionCount; ++index) {
      item.options[index] = options[index];
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
  case INVTYPE_STORAGE:
  case INVTYPE_GUILD_STORAGE:
    for (std::int64_t i = 0; i < count; ++i) {
      const EquipItemListRecord record = records[i];
      if (record.location == EQP_AMMO) {
        continue;
      }

      ItemRandomOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipItemListItem(item, record, options);
      g_session.AddCartItem(item, 0);
    }
    break;
  case INVTYPE_INVENTORY:
    for (std::int64_t i = 0; i < count; ++i) {
      const EquipItemListRecord record = records[i];
      ItemRandomOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipItemListItem(item, record, options);

      if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
        g_session.petEggIndex = item.identity[1];
      }

      if (item.identity[3] == 0) {
        g_session.AddInventoryItem(item, 0);
        g_session.UpdateInventoryItem(item, 0);
      } else {
        char equipped = 1;
        if (g_session.IsCostumeLocation(item.identity[3])) {
          g_session.EquipCostumeItem(item, &equipped);
        } else {
          g_session.EquipInventoryItem(item, &equipped);
        }
      }

      if (g_session.IsPetEgg(std::atoi(item.itemId.c_str())) && item.damaged) {
        const int petEgg = std::atoi(g_session.LookupClientItemByIdentity(g_session.petEggIndex).itemId.c_str());
        if (!g_petEvolutionMgr->IsAutoFeeding(petEgg) && g_session.m_petAutoFeed == true) {
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
  case INVTYPE_CART:
    for (std::int64_t i = 0; i < count; ++i) {
      const EquipItemListRecord record = records[i];
      ItemRandomOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipItemListItem(item, record, options);
      g_session.AddMerchantShopItem(item);
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
    item.identity[0] = record->type;
    item.identity[1] = record->index;
    item.identity[3] = record->location;
    item.identity[2] = 0;
    item.quantity = record->amount;
    std::memcpy(item.cards, record->cards, sizeof(record->cards));
    item.SetItemId(record->itemId);
    item.damaged = 0;
    item.identified = record->flags & 1;
    item.expireTime = record->expireTime;
    item.refiningLevel = 0;
    item.flags[0] = (record->flags >> 1) & 1;
    switch (listType) {
    case INVTYPE_STORAGE:
    case INVTYPE_GUILD_STORAGE:
      g_session.AddCartItem(item, 0);
      break;
    case INVTYPE_INVENTORY:
      g_session.UpdateInventoryItem(item, 0);
      break;
    case INVTYPE_CART:
      g_session.AddMerchantShopItem(item);
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
  case INVTYPE_STORAGE:
  case INVTYPE_GUILD_STORAGE:
    if (UIWindow* window = g_windowMgr.itemStoreWnd) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      reinterpret_cast<unsigned char*>(window)[0x190] = 1;
    }
    break;
  case INVTYPE_INVENTORY:
    RefreshItemWindows(&g_windowMgr);
    break;
  case INVTYPE_CART:
    g_session.RefreshItem1740Window();
    break;
  }
}

// OFFSET: 0x00DD43A0
// FUNCTION: fn_23232
__declspec(noinline) EquipSlots EquipSlotDecoder::Decode(int mask) const {
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
__declspec(noinline) bool CSession::AddInventoryItem(const ITEM_INFO& item, unsigned char refresh) {
  for (ITEM_INFO& value : this->m_inventoryItems) {
    if (value.identity[1] == item.identity[1]) {
      value.quantity += item.quantity;
      value.flags[1] = item.flags[1];
      value.flags[0] = item.flags[0];
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
bool CSession::AddMerchantPurchaseSavedItem(const ITEM_INFO& item, unsigned char alternate, unsigned char refresh) {
  for (ITEM_INFO& value : this->merchantPurchaseSavedItems) {
    if (value.identity[1] == item.identity[1]) {
      if (!alternate) {
        value.quantity += item.quantity;
        if (refresh == 1) {
          if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.merchantShopMakeWnd)) {
            window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          }
        }
      }
      return false;
    }
  }

  this->merchantPurchaseSavedItems.push_back(item);
  if (refresh == 1) {
    UIWindow* window = static_cast<UIWindow*>(alternate ? g_windowMgr.buyingShopMakeWnd : g_windowMgr.merchantShopMakeWnd);
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
  }
  return true;
}

// OFFSET: 0x00DD72C0
// FUNCTION: fn_23285
bool CSession::AddMerchantShopItem(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->merchantShopItems) {
    if (value.identity[1] == item.identity[1]) {
      value.quantity += item.quantity;
      if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.merchantItemWnd)) {
        window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      return false;
    }
  }

  this->merchantShopItems.push_back(item);
  if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.merchantItemWnd)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DD7350
// FUNCTION: fn_23286
bool CSession::AddMerchantMyShopItem(const ITEM_INFO& item, int type, unsigned char refresh) {
  for (ITEM_INFO& value : this->merchantMyShopItems) {
    if (value.identity[1] == item.identity[1]) {
      value.quantity += item.quantity;
      if (refresh == 1) {
        UIWindow* window;
        if (type == 1) {
          window = g_windowMgr.buyingShopMirrorItemWnd;
        } else {
          window = type == 2 ? g_windowMgr.purchaseMirrorItemWnd : g_windowMgr.merchantMirrorItemWnd;
        }
        if (window) {
          window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
      }
      return false;
    }
  }

  this->merchantMyShopItems.push_back(item);
  if (refresh == 1) {
    UIWindow* window;
    if (type == 1) {
      window = g_windowMgr.buyingShopMirrorItemWnd;
    } else {
      window = type == 2 ? g_windowMgr.purchaseMirrorItemWnd : g_windowMgr.merchantMirrorItemWnd;
    }
    if (window) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
  }
  return true;
}

// OFFSET: 0x00DD7420
// FUNCTION: fn_23287
void CSession::AddMerchantPurchaseItem(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->merchantPurchaseItems) {
    if (value.identity[1] == item.identity[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->merchantPurchaseItems.push_back(item);
}

// OFFSET: 0x00DD7470
// FUNCTION: fn_23288
void CSession::AddMerchantHopeToSellItem(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->merchantHopeToSellItems) {
    if (value.identity[1] == item.identity[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->merchantHopeToSellItems.push_back(item);
}

// OFFSET: 0x00DD74C0
// FUNCTION: fn_23289
void CSession::AddMerchantMirrorItem(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->merchantMirrorItems) {
    if (value.identity[1] == item.identity[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->merchantMirrorItems.push_back(item);
}

// OFFSET: 0x00DD7510
// FUNCTION: fn_23290
bool CSession::Equip(const ITEM_INFO& item) {
  const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->alternateEquippedItems[slot];
      equipped = item;
      equipped.quantity = 1;
      equipped.expireTime = item.expireTime;
      equipped.bindOnEquip = item.bindOnEquip;
      equipped.sprite = item.sprite;
    }
  }

  return true;
}

// OFFSET: 0x00DD75C0
// FUNCTION: fn_23291
void CSession::AddShopItem(const RawItemBasic& item, int quantity, std::uint16_t priceFactor) {
  ITEM_INFO value;
  value.AssignBasic(item);
  value.quantity = quantity;
  value.priceFactor = priceFactor;
  value.refiningLevel = 0;
  value.damaged = 0;
  value.SetItemId(std::atoi(value.itemId.c_str()));
  this->shopItems.push_back(value);
}

// OFFSET: 0x00DD76C0
// FUNCTION: fn_23292
void CSession::AppendShopItem(const ITEM_INFO& item) {
  this->shopItems.push_back(item);
}

// OFFSET: 0x00DD76D0
// FUNCTION: fn_23293
void CSession::MergeItem1718ByName(const ITEM_INFO& item) {
  for (auto it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    if (it->itemId == item.itemId) {
      it->quantity = it->quantity == -1 ? -1 : it->quantity + item.quantity;
      return;
    }
  }

  this->shopItems.push_back(item);
}

// OFFSET: 0x00DD7770
// FUNCTION: fn_23294
void CSession::MergeItem1720ByName(const ITEM_INFO& item) {
  for (auto it = this->purchaseItems.begin(); it != this->purchaseItems.end(); ++it) {
    if (it->itemId == item.itemId) {
      it->quantity += item.quantity;
      return;
    }
  }

  this->purchaseItems.push_back(item);
}

// OFFSET: 0x00DD77F0
// FUNCTION: fn_23295
void CSession::MergePurchaseItem(const ITEM_INFO& item) {
  for (auto it = this->purchaseItems.begin(); it != this->purchaseItems.end(); ++it) {
    bool same = it->identity[1] == item.identity[1];
    if (!item.flags[3]) {
      same = it->itemId == item.itemId;
    }
    if (same == true) {
      it->quantity += item.quantity;
      return;
    }
  }

  this->purchaseItems.push_back(item);
}

// OFFSET: 0x00DD7890
// FUNCTION: fn_23296
void CSession::AddStorageItem(const ITEM_INFO& item) {
  const ITEM_INFO owned = this->LookupClientItemByIdentity(item.identity[1]);
  if (owned.identity[1] > 0) {
    for (ITEM_INFO& value : this->storageItems) {
      if (value.identity[1] == item.identity[1]) {
        const int quantity = value.quantity + item.quantity;
        if (owned.quantity >= quantity) {
          value.quantity = quantity;
        }
        return;
      }
    }

    if (owned.quantity >= item.quantity) {
      this->storageItems.push_back(item);
    }
  }
}

// OFFSET: 0x00DD7970
// FUNCTION: fn_23297
void CSession::AddShopItem(const RawItemBasic& item, int mode) {
  if (mode != 0) {
    const ITEM_INFO owned = this->LookupClientItemByIdentity(item.id);
    if (owned.identity[1] > 0) {
      ITEM_INFO value(owned);
      value.AssignBasic(item);
      value.SetItemId(std::atoi(value.itemId.c_str()));
      this->shopItems.push_back(value);
    }
  } else {
    ITEM_INFO value;
    value.AssignBasic(item);
    value.refiningLevel = 0;
    value.damaged = 0;
    value.SetItemId(std::atoi(value.itemId.c_str()));
    value.identity[2] = item.location;
    value.sprite = item.sprite;
    this->shopItems.push_back(value);
  }
}

// OFFSET: 0x00DD7B30
// FUNCTION: fn_23298
void CSession::MergeShopItem(const ITEM_INFO& item) {
  for (ITEM_INFO& value : this->shopItems) {
    if (value.identity[1] == item.identity[1]) {
      if (value.quantity != -1) {
        value.quantity += item.quantity;
      }
      return;
    }
  }

  this->shopItems.push_back(item);
}

// OFFSET: 0x00DD7B80
// FUNCTION: fn_23299
__declspec(noinline) bool CSession::AddCartItem(const ITEM_INFO& item, unsigned char refresh) {
  if (g_replayAnimationScaling && !Replay::Instance().header.options.Get(ReplayRecordOption::Item)) {
    return true;
  }

  for (ITEM_INFO& value : this->cartItems) {
    if (value.identity[1] == item.identity[1]) {
      value.quantity += item.quantity;
      if (refresh && g_windowMgr.itemStoreWnd) {
        g_windowMgr.itemStoreWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      return false;
    }
  }

  this->cartItems.push_back(item);
  if (refresh && g_windowMgr.itemStoreWnd) {
    g_windowMgr.itemStoreWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DD6D70
// FUNCTION: fn_23280
bool CSession::EquipInventoryItem(const ITEM_INFO& item, const char* name) {
  const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1 && slot < 10) {
      ITEM_INFO& equipped = this->equippedItems[slot];
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
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), LookupMessage(MSI_ITEM_IS_EQUIPED)).c_str()), 0xC8FF00, 7, 0, 0);
  if (item.bindOnEquip == 1) {
    item.bindOnEquip = 2;
  }

  if (g_session.IsCostumeLocation(item.identity[3])) {
    char name[1] = "";
    g_session.EquipCostumeItem(item, name);
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
    const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

    for (int index = 0; index < 3; ++index) {
      const int slot = order[index];
      if (slot != -1 && slot < 10) {
        ITEM_INFO& equipped = this->equippedItems[slot];
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

  if (item.identity[3] != EQP_AMMO) {
    this->RemoveInventoryItem(item.identity[1], 1);
  }
  if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.itemParamChangeDisplayWnd)) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DFC620
// FUNCTION: fn_23598
bool CSession::UnequipItem(int location, std::uint32_t identity) {
  if (location == EQP_AMMO) {
    const ITEM_INFO item = g_session.LookupClientItemByIdentity(identity);
    if (item.identity[1] != 0) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), LookupMessage(MSI_ITEM_IS_REMOVED)).c_str()), 0xFF, 7, 0, 0);
    }
    this->equippedArrowIndex = 0;
    g_windowMgr.InvalidateFrames();

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode != nullptr) {
      auto it = std::find_if(g_activeSkillEffects.begin(), g_activeSkillEffects.end(), [](const ActiveSkillEffect& effect) { return effect.id == 0x2B7; });
      if (it != g_activeSkillEffects.end()) {
        g_activeSkillEffects.erase(it);
      }

      if (mode->scene && mode->scene->player && mode->scene->player->skillEffect) {
        mode->scene->player->skillEffect->SendMsg(nullptr, AM_SETCOUNT, 0, 1, 0, 0, 0, 0);
      }
    }
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    if (slots.values[2] < 10) {
      if (slots.values[2] != -1) {
        ITEM_INFO& item = this->equippedItems[slots.values[2]];
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s%s", item.DisplayName(0).c_str(), LookupMessage(MSI_ITEM_IS_REMOVED)).c_str()), 0xFF, 7, 0, 0);
        item.identity[3] = 0;

        bool merged = false;
        for (ITEM_INFO& value : this->m_inventoryItems) {
          if (value.identity[1] == item.identity[1]) {
            value.quantity += item.quantity;
            value.flags[1] = item.flags[1];
            value.flags[0] = item.flags[0];
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
        this->equippedItems[slots.values[1]].Reset();
      }
      if (slots.values[0] != -1 && slots.values[0] < 10) {
        this->equippedItems[slots.values[0]].Reset();
      }
    }
    RequestBasicInfoRefresh();
  }

  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->UnequipWearable(location);
  g_windowMgr.InvalidateFrames();
  if (UIWindow* window = g_windowMgr.itemParamChangeDisplayWnd) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
  return true;
}

// OFFSET: 0x00DD7E10
// FUNCTION: fn_23300
EquipSlots CSession::GetEquipSlots(int location) {
  EquipSlots result;
  if (location == EQP_AMMO) {
    result.values[0] = -1;
    result.values[1] = -1;
    result.values[2] = 15;
    return result;
  }

  int mask = location;
  switch (location) {
  case EQP_COSTUME_HEAD_TOP:
    mask = EQP_HEAD_TOP;
    break;
  case EQP_COSTUME_HEAD_MID:
    mask = EQP_HEAD_MID;
    break;
  case EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_MID:
    mask = EQP_HEAD_TOP | EQP_HEAD_MID;
    break;
  case EQP_COSTUME_HEAD_LOW:
    mask = EQP_HEAD_LOW;
    break;
  case EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_LOW:
    mask = EQP_HEAD_TOP | EQP_HEAD_LOW;
    break;
  case EQP_COSTUME_HEAD_MID | EQP_COSTUME_HEAD_LOW:
    mask = EQP_HEAD_MID | EQP_HEAD_LOW;
    break;
  case EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_MID | EQP_COSTUME_HEAD_LOW:
    mask = EQP_HEAD_TOP | EQP_HEAD_MID | EQP_HEAD_LOW;
    break;
  case EQP_COSTUME_GARMENT:
    mask = EQP_GARMENT;
    break;
  case 0x4000:
  case EQP_SHADOW_WEAPON:
    mask = EQP_HAND_R;
    break;
  case EQP_SHADOW_ARMOR:
    mask = EQP_ARMOR;
    break;
  case EQP_SHADOW_SHIELD:
    mask = EQP_HAND_L;
    break;
  case EQP_SHADOW_SHOES:
    mask = EQP_SHOES;
    break;
  case EQP_SHADOW_ACC_R:
    mask = EQP_ACC_R;
    break;
  case EQP_SHADOW_ACC_L:
    mask = EQP_ACC_L;
    break;
  }

  const EquipSlots decoded = EquipSlotDecoder().Decode(mask);
  result.values[0] = decoded.values[0];
  result.values[1] = decoded.values[1];
  result.values[2] = decoded.values[2];
  return result;
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
  this->merchantHopeToSellItems.erase(this->merchantHopeToSellItems.begin(), this->merchantHopeToSellItems.end());
  this->merchantMirrorItems.erase(this->merchantMirrorItems.begin(), this->merchantMirrorItems.end());
}

// OFFSET: 0x00DD8900
// FUNCTION: fn_23315
void CSession::ClearMerchantPurchaseSavedItems() {
  this->merchantPurchaseSavedItems.erase(this->merchantPurchaseSavedItems.begin(), this->merchantPurchaseSavedItems.end());
}

// OFFSET: 0x00DD8920
// FUNCTION: fn_23316
void CSession::ClearMerchantShopItems() {
  this->merchantShopItems.erase(this->merchantShopItems.begin(), this->merchantShopItems.end());
}

// OFFSET: 0x00DD8940
// FUNCTION: fn_23317
__declspec(noinline) void CSession::ClearMerchantMyShopItems() {
  this->merchantMyShopItems.erase(this->merchantMyShopItems.begin(), this->merchantMyShopItems.end());
}

// OFFSET: 0x00DD8960
// FUNCTION: fn_23318
void CSession::ClearMerchantPurchaseItems() {
  this->merchantPurchaseItems.erase(this->merchantPurchaseItems.begin(), this->merchantPurchaseItems.end());
}

// OFFSET: 0x00DD8980
// FUNCTION: fn_23319
void CSession::ClearMerchantHopeToSellItems() {
  this->merchantHopeToSellItems.erase(this->merchantHopeToSellItems.begin(), this->merchantHopeToSellItems.end());
}

// OFFSET: 0x00DD89A0
// FUNCTION: fn_23320
void CSession::ClearMerchantMirrorItems() {
  this->merchantMirrorItems.erase(this->merchantMirrorItems.begin(), this->merchantMirrorItems.end());
}

// OFFSET: 0x00DD89C0
// FUNCTION: fn_23321
void CSession::ClearMiniPartyRecords() {
  this->miniPartyRecords.clear();
}

// OFFSET: 0x00DD8A20
// FUNCTION: fn_23322
void CSession::ClearInventoryCategoryItems(int index) {
  if (index >= 0 && static_cast<unsigned>(index) < 11) {
    this->inventoryCategories[index].erase(this->inventoryCategories[index].begin(), this->inventoryCategories[index].end());
  }
}

// OFFSET: 0x00DD8A50
// FUNCTION: fn_23323
void CSession::ClearItemSlotQuantities() {
  this->otherExchangeMoney = 0;

  for (int index = 0; index < 10; ++index) {
    this->m_itemSlots[index].quantity = 0;
  }
}

// OFFSET: 0x00DD8AC0
// FUNCTION: fn_23324
void CSession::ResetEquipped() {
  for (int index = 0; index < 10; ++index) {
    this->alternateEquippedItems[index].Reset();
  }
}

// OFFSET: 0x00DD8AF0
// FUNCTION: fn_23325
void CSession::ClearPartyMembers() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (mode) {
    for (auto member = this->m_partyMembers.begin(); member != this->m_partyMembers.end(); ++member) {
      CRenderObject* actor = mode->scene->GetGameActorByAidIncludeMe(member->accountId);
      if (actor && static_cast<EffectActor*>(actor)->HasId(EF_PARTY) == true) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_PARTY, 0, 0, 0, 0, 0);
      }
    }
  }

  this->m_partyMembers.clear();
  this->partyLeader = 0;
  this->partyName = "";
}

// OFFSET: 0x00DD8BD0
// FUNCTION: fn_23326
void CSession::ResetStatBonuses() {
  this->strengthBonus = 0;
  this->dexterityBonus = 0;
  this->luckBonus = 0;
  this->agilityBonus = 0;
  this->vitalityBonus = 0;
  this->intelligenceBonus = 0;
}

// OFFSET: 0x00DD8C10
// FUNCTION: fn_23327
void CSession::ClearPurchaseItems() {
  this->purchaseItems.erase(this->purchaseItems.begin(), this->purchaseItems.end());
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
void CSession::ClearCartItems() {
  this->cartItems.erase(this->cartItems.begin(), this->cartItems.end());
}

// OFFSET: 0x00DD8CC0
// FUNCTION: fn_23330
void CSession::CopyItems1740To1750() {
  this->merchantMyShopItems.erase(this->merchantMyShopItems.begin(), this->merchantMyShopItems.end());

  for (auto item = this->merchantShopItems.begin(); item != this->merchantShopItems.end(); ++item) {
    this->merchantMyShopItems.push_back(*item);
  }
}

// OFFSET: 0x00DD8DB0
// FUNCTION: fn_23331
void CSession::CopyItems1760To1770() {
  this->filteredInventoryItems.erase(this->filteredInventoryItems.begin(), this->filteredInventoryItems.end());

  for (auto item = this->merchantHopeToSellItems.begin(); item != this->merchantHopeToSellItems.end(); ++item) {
    this->filteredInventoryItems.push_back(*item);
  }
}

// OFFSET: 0x00DD8EA0
// FUNCTION: fn_23332
std::pair<bool, ITEM_INFO> CSession::FindMerchantMyShopItem(const ITEM_INFO& item, unsigned char) {
  auto found = this->merchantMyShopItems.begin();

  for (; found != this->merchantMyShopItems.end(); ++found) {
    if (IsSameItem(*found, item)) {
      break;
    }
  }

  return found != this->merchantMyShopItems.end() ? std::make_pair(true, *found) : std::make_pair(false, ITEM_INFO());
}

// OFFSET: 0x00DD91C0
// FUNCTION: fn_23333
__declspec(noinline) int CSession::IsDifferentName(const char* name, const char* other) {
  if (std::strlen(other) == 0) {
    return 1;
  }

  char first[24];
  std::sprintf(first, "%s", name);
  const short firstLength = static_cast<short>(std::strlen(first));

  char second[24];
  std::sprintf(second, "%s", other);
  const short secondLength = static_cast<short>(std::strlen(second));

  const short difference = firstLength < secondLength ? secondLength - firstLength : firstLength - secondLength;
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
  for (auto it = this->merchantPurchaseItems.begin(); it != this->merchantPurchaseItems.end(); ++it) {
    const int itemId = std::atoi(it->itemId.c_str());
    if (itemId == std::atoi(item.itemId.c_str())) {
      item.itemId = it->itemId;
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->merchantPurchaseItems.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA510
// FUNCTION: fn_23339
void CSession::RemoveInventoryItem(std::uint32_t id, int quantity) {
  for (auto it = this->m_inventoryItems.begin(); it != this->m_inventoryItems.end(); ++it) {
    if (it->identity[1] == id) {
      if (it->quantity <= quantity) {
        this->m_inventoryItems.erase(it);
      } else {
        it->quantity -= quantity;
      }
      for (auto it2 = this->m_inventoryItems.begin(); it2 != this->m_inventoryItems.end(); ++it2) {
        if (it2->identity[1] == id) {
          it2->flags[2] = 0;
          break;
        }
      }
      break;
    }
  }
  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00DFF0E0
// FUNCTION: fn_23609
void CSession::RefreshItem1768Window(unsigned char alternate, int argument) {
  if (UIWindow* window = static_cast<UIWindow*>(alternate ? g_windowMgr.buyingShopMakeWnd : g_windowMgr.merchantShopMakeWnd)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, argument, 0, 0, 0);
  }
}

// OFFSET: 0x00DDA5A0
// FUNCTION: fn_23340
bool CSession::RemoveMerchantPurchaseSavedItem(const ITEM_INFO& item, unsigned char alternate) {
  int index = 0;
  for (auto it = this->merchantPurchaseSavedItems.begin(); it != this->merchantPurchaseSavedItems.end(); ++it, ++index) {
    if (it->identity[1] == item.identity[1]) {
      if (alternate) {
        this->merchantPurchaseSavedItems.erase(it);
        this->RefreshItem1768Window(1, 0);
        return true;
      }
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->merchantPurchaseSavedItems.erase(it);
        this->RefreshItem1768Window(0, index + 1);
        return true;
      }
      this->RefreshItem1768Window(0, 0);
      return true;
    }
  }
  this->RefreshItem1768Window(alternate, 0);
  return false;
}

// OFFSET: 0x00DDA690
// FUNCTION: fn_23341
bool CSession::RemoveMerchantShopItem(const ITEM_INFO& item) {
  for (auto it = this->merchantShopItems.begin(); it != this->merchantShopItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->merchantShopItems.erase(it);
      }
      const std::uint32_t index = item.identity[1];
      for (auto it2 = this->merchantShopItems.begin(); it2 != this->merchantShopItems.end(); ++it2) {
        if (it2->identity[1] == index) {
          it2->flags[2] = 0;
          break;
        }
      }
      if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.merchantItemWnd)) {
        window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      return true;
    }
  }
  if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.merchantItemWnd)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  return false;
}

// OFFSET: 0x00DDA8B0
// FUNCTION: fn_23343
void CSession::RemoveMerchantPurchaseItem(ITEM_INFO& item) {
  for (auto it = this->merchantPurchaseItems.begin(); it != this->merchantPurchaseItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      item.itemId = it->itemId;
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->merchantPurchaseItems.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA920
// FUNCTION: fn_23344
void CSession::RemoveMerchantHopeToSellItem(const ITEM_INFO& item) {
  for (auto it = this->merchantHopeToSellItems.begin(); it != this->merchantHopeToSellItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->merchantHopeToSellItems.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDA9C0
// FUNCTION: fn_23345
void CSession::RemoveItem1718ByItemId(const ITEM_INFO& item) {
  for (auto it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    const int itemId = std::atoi(it->itemId.c_str());
    if (itemId == std::atoi(item.itemId.c_str())) {
      if (it->quantity != -1) {
        it->quantity -= item.quantity;
        if (it->quantity <= 0) {
          this->shopItems.erase(it);
        }
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAA50
// FUNCTION: fn_23346
void CSession::RemoveShopItem(const ITEM_INFO& item) {
  for (auto it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      if (it->quantity != -1) {
        it->quantity -= item.quantity;
        if (it->quantity <= 0) {
          this->shopItems.erase(it);
        }
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAAB0
// FUNCTION: fn_23347
void CSession::RemoveItem1720ByName(const ITEM_INFO& item) {
  for (auto it = this->purchaseItems.begin(); it != this->purchaseItems.end(); ++it) {
    if (it->itemId == item.itemId) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->purchaseItems.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAC40
// FUNCTION: fn_23349
void CSession::RemoveStorageItem(const ITEM_INFO& item) {
  for (auto it = this->storageItems.begin(); it != this->storageItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->storageItems.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDAC90
// FUNCTION: fn_23350
void CSession::ReduceShopItemQuantity(const ITEM_INFO& item) {
  for (auto it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->shopItems.erase(it);
      }
      return;
    }
  }
}

// OFFSET: 0x00DDACE0
// FUNCTION: fn_23351
bool CSession::RemoveCartItem(const ITEM_INFO& item) {
  for (auto it = this->cartItems.begin(); it != this->cartItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->cartItems.erase(it);
      }
      const std::uint32_t index = item.identity[1];
      for (auto it2 = this->cartItems.begin(); it2 != this->cartItems.end(); ++it2) {
        if (it2->identity[1] == index) {
          it2->flags[2] = 0;
          break;
        }
      }
      if (UIWindow* window = g_windowMgr.itemStoreWnd) {
        window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      return true;
    }
  }
  if (UIWindow* window = g_windowMgr.itemStoreWnd) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  return false;
}

// OFFSET: 0x00E2EC10
// FUNCTION: fn_24149
bool CSession::EquipSwapEquipmentItem(const ITEM_INFO& item) {
  const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->swapEquippedItems[slot];
      equipped = item;
      equipped.quantity = 1;
      equipped.expireTime = item.expireTime;
      equipped.bindOnEquip = item.bindOnEquip;
      equipped.sprite = item.sprite;
    }
  }

  if (g_windowMgr.equipWnd) {
    static_cast<UIEquipWnd*>(g_windowMgr.equipWnd)->NotifyEquipmentChanged();
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

  ITEM_INFO item = this->LookupClientItemByIdentity(identity);
  if (item.identity[1] > 0) {
    return item;
  }

  const EquipSlots slots = g_session.GetEquipSlots(location);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      if (this->IsCostumeLocation(location)) {
        item = this->GetCostumeItemAt(slot);
      } else {
        item = this->GetEquippedItemAt(slot);
      }
      if (item.identity[1] == identity) {
        break;
      }
    }
  }

  if (item.identity[1] != 0) {
    return item;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E49F40
// FUNCTION: fn_24247
void CSession::SetMerchantMyShopItems(int count, const short* ids) {
  this->ClearMerchantMyShopItems();
  for (int i = 0; i < count; ++i) {
    const ITEM_INFO item = g_session.LookupClientItemByIdentity(ids[i]);
    if (item.identity[1] > 0) {
      this->merchantMyShopItems.push_back(item);
    }
  }
}

// OFFSET: 0x00DF9560
// FUNCTION: fn_23520
int CSession::IsGuildMemberNameDifferent(const char* name) {
  std::list<GuildInfo::Member> members;
  members = g_guildInfo.m_members;
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

  this->RemoveInventoryItem(item.identity[1], item.quantity);

  ITEM_INFO added = item;
  bool merged = false;
  for (int index = 0; index < 10; ++index) {
    if (this->m_exchangeItems[index].quantity != 0 && this->m_exchangeItems[index].identity[1] == added.identity[1]) {
      this->m_exchangeItems[index].quantity += added.quantity;
      merged = true;
      break;
    }
  }

  if (!merged) {
    for (int i = 0; i < 10; ++i) {
      if (this->m_exchangeItems[i].quantity == 0) {
        this->m_exchangeItems[i] = added;
        break;
      }
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
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
