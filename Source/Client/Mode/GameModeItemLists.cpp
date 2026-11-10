#include "Client/Mode/CGameMode.h"
#include "UI/Window/WindowId.h"
#include "Contents/Items/InventoryType.h"
#include "Client/Session.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/PetEvolutionMgr.h"
#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  CEquipmentPropertiesMgr();
  void AddExpansion(const int& kind, const int& itemId, bool notify);

  unsigned char padding_0000[0x1FC];
};
}



namespace {
#pragma pack(push, 1)
  struct EquipListOption {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int16_t value;
    /* 0004 */ unsigned char param;
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
    /* 0029 */ EquipListOption options[5];
    /* 0042 */ unsigned char flags;
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
      if (options[index].id != 0) {
        ++count;
      }
    }
    return count;
  }

  // INLINED
  __forceinline void FillEquipListItem(ITEM_INFO& item, const EquipListRecord& record, const EquipListOption* options) {
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
    item.optionCount = CountEquipListOptions(options);
    item.grade = 0;
    for (int index = 0; index < item.optionCount; ++index) {
      reinterpret_cast<EquipListOption*>(item.options)[index] = options[index];
    }
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
      const EquipListRecord record = records[i];
      if (record.location == 0x8000) {
        continue;
      }

      EquipListOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipListItem(item, record, options);
      g_session.AddCartItem(item, 0);
    }
    break;
  case INVTYPE_INVENTORY:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord record = records[i];
      EquipListOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipListItem(item, record, options);

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
        const int petEgg = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.petEggIndex).itemId.c_str());
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
  case INVTYPE_CART:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord record = records[i];
      EquipListOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipListItem(item, record, options);
      g_session.AddMerchantShopItem(item);
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
