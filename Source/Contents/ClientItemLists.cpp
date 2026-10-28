#include "Client/Replay.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstddef>
#include <list>
#include <vector>

namespace {
  struct EquipSlots {
    /* 0000 */ int values[3];
  };

  struct EquipSlotDecoder {
    EquipSlots Decode(int mask) const;
  };
}

struct ClientItemLists {
  /* 0000 */ unsigned char unknown_0000[0x1710];
  /* 1710 */ std::list<ItemInfo> inventory;
  /* 1718 */ std::list<ItemInfo> items_1718;
  /* 1720 */ std::list<ItemInfo> items_1720;
  /* 1728 */ std::list<ItemInfo> items_1728;
  /* 1730 */ std::list<ItemInfo> items_1730;
  /* 1738 */ std::list<ItemInfo> items_1738;
  /* 1740 */ std::list<ItemInfo> items_1740;
  /* 1748 */ std::list<ItemInfo> items_1748;
  /* 1750 */ std::list<ItemInfo> items_1750;
  /* 1758 */ std::list<ItemInfo> items_1758;
  /* 1760 */ std::list<ItemInfo> items_1760;
  /* 1768 */ std::list<ItemInfo> items_1768;
  /* 1770 */ unsigned char unknown_1770[0xA30];
  /* 21A0 */ ItemInfo equipped[10];

  bool AddItem1710(const ItemInfo& item, unsigned char refresh);
  void AddItem1718(const ItemInfo& item, int quantity, std::uint16_t unknown_006C);
  void AddItem1718(const ItemInfo& item, int mode);
  bool AddItem1768(const ItemInfo& item, unsigned char alternate, unsigned char refresh);
  bool AddItem1740(const ItemInfo& item);
  bool AddItem1750(const ItemInfo& item, int type, unsigned char refresh);
  void AddItem1758(const ItemInfo& item);
  void AddItem1760(const ItemInfo& item);
  void AddItem1748(const ItemInfo& item);
  bool Equip(const ItemInfo& item);
  void AppendItem1718(const ItemInfo& item);
  void MergeItem1718ByName(const ItemInfo& item);
  void MergeItem1720ByName(const ItemInfo& item);
  void MergeItem1720(const ItemInfo& item);
  void AddItem1730(const ItemInfo& item);
  void MergeItem1718(const ItemInfo& item);
  bool AddItem1738(const ItemInfo& item, unsigned char refresh);
  EquipSlots GetEquipSlots(int location);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ClientItemLists, inventory) == 0x1710);
static_assert(offsetof(ClientItemLists, items_1768) == 0x1768);
static_assert(offsetof(ClientItemLists, equipped) == 0x21A0);
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
bool ClientItemLists::AddItem1710(const ItemInfo& item, unsigned char refresh) {
  for (ItemInfo& value : this->inventory) {
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
bool ClientItemLists::AddItem1768(const ItemInfo& item, unsigned char alternate, unsigned char refresh) {
  for (ItemInfo& value : this->items_1768) {
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
bool ClientItemLists::AddItem1740(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1740) {
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
bool ClientItemLists::AddItem1750(const ItemInfo& item, int type, unsigned char refresh) {
  for (ItemInfo& value : this->items_1750) {
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
void ClientItemLists::AddItem1758(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1758) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1758.push_back(item);
}

// OFFSET: 0x00DD7470
// FUNCTION: fn_23288
void ClientItemLists::AddItem1760(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1760) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1760.push_back(item);
}

// OFFSET: 0x00DD74C0
// FUNCTION: fn_23289
void ClientItemLists::AddItem1748(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1748) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1748.push_back(item);
}

// OFFSET: 0x00DD7510
// FUNCTION: fn_23290
bool ClientItemLists::Equip(const ItemInfo& item) {
  const EquipSlots slots = this->GetEquipSlots(item.unknown_0000[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ItemInfo& equipped = this->equipped[slot];
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

// OFFSET: 0x00DD76C0
// FUNCTION: fn_23292
void ClientItemLists::AppendItem1718(const ItemInfo& item) {
  this->items_1718.push_back(item);
}

// OFFSET: 0x00DD76D0
// FUNCTION: fn_23293
void ClientItemLists::MergeItem1718ByName(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1718) {
    if (value.itemId == item.itemId) {
      value.quantity = value.quantity == -1 ? -1 : value.quantity + item.quantity;
      return;
    }
  }

  this->items_1718.push_back(item);
}

// OFFSET: 0x00DD7770
// FUNCTION: fn_23294
void ClientItemLists::MergeItem1720ByName(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1720) {
    if (value.itemId == item.itemId) {
      value.quantity += item.quantity;
      return;
    }
  }

  this->items_1720.push_back(item);
}

// OFFSET: 0x00DD77F0
// FUNCTION: fn_23295
void ClientItemLists::MergeItem1720(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1720) {
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
void ClientItemLists::AddItem1730(const ItemInfo& item) {
  const ItemInfo owned = LookupClientItemByIdentity(this->inventory, item.unknown_0000[1]);
  if (owned.unknown_0000[1] > 0) {
    for (ItemInfo& value : this->items_1730) {
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

// OFFSET: 0x00DD7B30
// FUNCTION: fn_23298
void ClientItemLists::MergeItem1718(const ItemInfo& item) {
  for (ItemInfo& value : this->items_1718) {
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
bool ClientItemLists::AddItem1738(const ItemInfo& item, unsigned char refresh) {
  if (g_replayAnimationScaling && !Replay::Instance().header.options.Get(ReplayRecordOption::Item)) {
    return true;
  }

  for (ItemInfo& value : this->items_1738) {
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
EquipSlots ClientItemLists::GetEquipSlots(int location) {
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
