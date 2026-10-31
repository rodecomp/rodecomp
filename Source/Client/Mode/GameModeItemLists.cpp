#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/PetEvolutionMgr.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  CEquipmentPropertiesMgr();
  void AddExpansion(const int& kind, const int& itemId, bool notify);

  unsigned char unknown_0000[0x1FC];
};
}

struct ClientItemLists {
  bool AddItem1710(const ITEM_INFO& item, unsigned char refresh);
  bool AddItem1738(const ITEM_INFO& item, unsigned char refresh);
  bool AddItem1740(const ITEM_INFO& item);
};

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
    item.unknown_0098 = CountEquipListOptions(options);
    item.unknown_0088 = 0;
    for (int index = 0; index < item.unknown_0098; ++index) {
      reinterpret_cast<EquipListOption*>(item.unknown_009C)[index] = options[index];
    }
  }
}

// GLOBAL: gl_24395
int g_activePetEggIndex;

// OFFSET: 0x00D42FB0
// FUNCTION: fn_21831
void CGameMode::ReceiveEquipListPart(unsigned char listType, int count, const unsigned char* data) {
  const EquipListRecord* records = reinterpret_cast<const EquipListRecord*>(data);
  switch (listType) {
  case 2:
  case 3:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord record = records[i];
      if (record.location == 0x8000) {
        continue;
      }

      EquipListOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipListItem(item, record, options);
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1738(item, 0);
    }
    break;
  case 0:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord record = records[i];
      EquipListOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipListItem(item, record, options);

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
    break;
  case 1:
    for (int i = 0; i < count; ++i) {
      const EquipListRecord record = records[i];
      EquipListOption options[5];
      std::memcpy(options, record.options, sizeof(options));
      ITEM_INFO item;
      FillEquipListItem(item, record, options);
      reinterpret_cast<ClientItemLists*>(&g_session)->AddItem1740(item);
    }
    break;
  }
}
