#include "Client/Mode/CGameMode.h"
#include "UI/Window/WindowId.h"
#include "Contents/Items/InventoryType.h"
#include "Client/Session/Session.h"
#include "Contents/ClientState.h"
#include "Contents/CombatTypes.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/EquipmentPropertiesMgr.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "UI/Items/UIItemStoreWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

namespace {
#pragma pack(push, 1)
  using EquipListOption = ItemOptionRecord;

  struct EquipListOptions {
    /* 0000 */ EquipListOption values[5];
  };

  struct EquipListRecord {
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
    /* 0029 */ EquipListOptions options;
    /* 0042 */ unsigned char identified : 1;
    /* 0042 */ unsigned char damaged : 1;
    /* 0042 */ unsigned char etcTab : 1;
  };

  struct InventoryStartHeader {
    /* 0000 */ std::int16_t length;
    /* 0002 */ unsigned char listType;
  };
#pragma pack(pop)

  static_assert(sizeof(EquipListRecord) == 0x43);

  // INLINED
  __forceinline int CountEquipListOptions(const EquipListOption* options) {
    int count = 0;
    for (int index = 0; index < 5; ++index) {
      if (0 != options[index].id) {
        ++count;
      }
    }
    return count;
  }

  // INLINED
  __forceinline void AddStorageEquipListItem(
    unsigned char type, std::uint32_t location, std::int16_t index, std::uint32_t wearState, std::uint32_t card1, std::uint32_t card2, std::uint32_t card3,
    std::uint32_t card4, int itemId, unsigned char identified, unsigned char damaged, unsigned char refine, std::uint32_t expireTime,
    std::uint16_t bindOnEquip, std::uint16_t sprite, unsigned char placeEtcTab, int grade, EquipListOptions options
  ) {
    if (location == EQP_AMMO) {
      return;
    }

    ITEM_INFO item;
    item.identity[0] = type;
    item.identity[2] = location;
    item.identity[1] = index;
    item.identity[3] = wearState;
    item.quantity = 1;
    std::uint32_t* cards = item.cards;
    cards[0] = card1;
    cards[1] = card2;
    cards[2] = card3;
    cards[3] = card4;
    item.SetItemId(itemId);
    item.identified = identified;
    item.damaged = damaged;
    item.refiningLevel = refine;
    item.expireTime = expireTime;
    item.bindOnEquip = bindOnEquip;
    item.sprite = sprite;
    item.placeEtcTab = placeEtcTab;
    item.optionCount = CountEquipListOptions(options.values);
    item.grade = grade;
    for (int index = 0; index < item.optionCount; ++index) {
      item.options[index] = options.values[index];
    }
    g_session.AddCartItem(item, 0);
  }

  // INLINED
  __forceinline void AddInventoryEquipListItem(
    unsigned char type, std::uint32_t location, std::int16_t index, std::uint32_t wearState, std::uint32_t card1, std::uint32_t card2, std::uint32_t card3,
    std::uint32_t card4, int itemId, unsigned char identified, unsigned char damaged, unsigned char refine, std::uint32_t expireTime,
    std::uint16_t bindOnEquip, std::uint16_t sprite, unsigned char placeEtcTab, int grade, EquipListOptions options
  ) {
    ITEM_INFO item;
    item.identity[0] = type;
    item.identity[2] = location;
    item.identity[1] = index;
    item.identity[3] = wearState;
    item.quantity = 1;
    std::uint32_t* cards = item.cards;
    cards[0] = card1;
    cards[1] = card2;
    cards[2] = card3;
    cards[3] = card4;
    item.SetItemId(itemId);
    item.identified = identified;
    item.damaged = damaged;
    item.refiningLevel = refine;
    item.expireTime = expireTime;
    item.bindOnEquip = bindOnEquip;
    item.sprite = sprite;
    item.placeEtcTab = placeEtcTab;
    item.optionCount = CountEquipListOptions(options.values);
    item.grade = grade;
    for (int index = 0; index < item.optionCount; ++index) {
      item.options[index] = options.values[index];
    }

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
      const int petEgg = std::atoi(g_session.LookupClientItemByIdentity(g_session.petEggIndex).itemId.c_str());
      if (!g_petEvolutionMgr->IsAutoFeeding(petEgg) && g_session.m_petAutoFeed == true) {
        g_session.SendPetAutoFeedOption(0);
      }

      EquipmentProperties::CEquipmentPropertiesMgr* manager = g_equipmentPropertiesMgr;
      if (manager == nullptr) {
        manager = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
        g_equipmentPropertiesMgr = manager;
      }

      const int itemId = std::atoi(item.itemId.c_str());
      const int kind = 0;
      manager->AddExpansion(kind, itemId, false);
    }
  }

  // INLINED
  __forceinline void AddCartEquipListItem(
    unsigned char type, std::uint32_t location, std::int16_t index, std::uint32_t wearState, std::uint32_t card1, std::uint32_t card2, std::uint32_t card3,
    std::uint32_t card4, int itemId, unsigned char identified, unsigned char damaged, unsigned char refine, std::uint32_t expireTime,
    std::uint16_t bindOnEquip, std::uint16_t sprite, unsigned char placeEtcTab, int grade, EquipListOptions options
  ) {
    ITEM_INFO item;
    item.identity[0] = type;
    item.identity[2] = location;
    item.identity[1] = index;
    item.identity[3] = wearState;
    item.quantity = 1;
    std::uint32_t* cards = item.cards;
    cards[0] = card1;
    cards[1] = card2;
    cards[2] = card3;
    cards[3] = card4;
    item.SetItemId(itemId);
    item.identified = identified;
    item.damaged = damaged;
    item.refiningLevel = refine;
    item.expireTime = expireTime;
    item.bindOnEquip = bindOnEquip;
    item.sprite = sprite;
    item.placeEtcTab = placeEtcTab;
    item.optionCount = CountEquipListOptions(options.values);
    item.grade = grade;
    for (int index = 0; index < item.optionCount; ++index) {
      item.options[index] = options.values[index];
    }
    g_session.AddMerchantShopItem(item);
  }
}


// OFFSET: 0x00D42FB0
// FUNCTION: fn_21831
void CGameMode::ReceiveEquipListPart(unsigned char listType, int count, const unsigned char* data) {
  const EquipListRecord* records = reinterpret_cast<const EquipListRecord*>(data);
  switch (listType) {
  case INVTYPE_STORAGE:
  case INVTYPE_GUILD_STORAGE:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord& record = records[i];
      AddStorageEquipListItem(
        record.type, record.location, record.index, record.wearState, record.cards[0], record.cards[1], record.cards[2], record.cards[3], record.itemId,
        record.identified, record.damaged, record.refine, record.expireTime, record.bindOnEquip, record.sprite, record.etcTab, 0, record.options
      );
    }
    break;
  case INVTYPE_INVENTORY:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord& record = records[i];
      AddInventoryEquipListItem(
        record.type, record.location, record.index, record.wearState, record.cards[0], record.cards[1], record.cards[2], record.cards[3], record.itemId,
        record.identified, record.damaged, record.refine, record.expireTime, record.bindOnEquip, record.sprite, record.etcTab, 0, record.options
      );
    }
    break;
  case INVTYPE_CART:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord& record = records[i];
      AddCartEquipListItem(
        record.type, record.location, record.index, record.wearState, record.cards[0], record.cards[1], record.cards[2], record.cards[3], record.itemId,
        record.identified, record.damaged, record.refine, record.expireTime, record.bindOnEquip, record.sprite, record.etcTab, 0, record.options
      );
    }
    break;
  }
}

// OFFSET: 0x00D44600
// FUNCTION: fn_21835
void CGameMode::ReceiveInventoryStart(const unsigned char* packet) {
  const InventoryStartHeader& header = *reinterpret_cast<const InventoryStartHeader*>(g_gameModePacketBuffer + 2);
  const int length = header.length - 5;
  const unsigned char listType = header.listType;
  if (length > 24) {
    return;
  }

  switch (listType) {
  case INVTYPE_STORAGE:
  case INVTYPE_GUILD_STORAGE: {
    char title[24] = {};
    memcpy_s(title, sizeof(title), packet + 5, length);
    UIItemStoreWnd* window = static_cast<UIItemStoreWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREWND));
    if (window) {
      const std::size_t titleLength = std::strlen(title);
      if (titleLength != 0) {
        window->SetTitle(std::string(title, titleLength));
      }
    }
    break;
  }
  case INVTYPE_INVENTORY:
    g_session.ClearInventory();
    g_session.ResetEquipment();
    g_session.ResetCostumeItems();
    break;
  case INVTYPE_CART:
    g_session.ClearMerchantShopItems();
    break;
  }
}

// GLOBAL: gl_25206
unsigned char g_gameModePacketBuffer[0x10000];
