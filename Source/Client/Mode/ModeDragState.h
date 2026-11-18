#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODEDRAGSTATE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODEDRAGSTATE_H

#include <cstdint>
#include <string>

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/WindowManagerWindows.h"
class ResourceMgr;
class UIWindow;
struct ITEM_INFO;
struct BarterMarketItem;
struct BarterMarketEntry;

enum DragSource : int {
  DRAG_NONE = 0,
  DRAG_ITEM_INVENTORY = 1,
  DRAG_ITEM_EQUIP = 2,
  DRAG_ITEM_NPC_SHOP = 3,
  DRAG_ITEM_PURCHASE_CART = 4,
  DRAG_ITEM_SELL = 5,
  DRAG_ITEM_STORAGE = 6,
  DRAG_SHORTCUT = 7,
  DRAG_SKILL = 8,
  DRAG_ITEM_MERCHANT_INVENTORY = 9,
  DRAG_VENDING_PURCHASE_LIST = 10,
  DRAG_VENDING_SHOP_LIST = 11,
  DRAG_VENDING_ITEM_SHOP = 12,
  DRAG_VENDING_ITEM_PURCHASE = 13,
  DRAG_METAL_PROCESS = 14,
  DRAG_RODEX_COMPOSE = 15,
  DRAG_QUICKSLOT = 16,
  DRAG_SWAP_EQUIP = 17,
  DRAG_SWAP_EQUIP_COSTUME = 18,
  DRAG_BUYINGSTORE_PURCHASE_LIST = 19,
  DRAG_BUYINGSTORE_SHOP_LIST = 20,
  DRAG_BUYINGSTORE_ITEM_PURCHASE = 21,
  DRAG_PURCHASE_MIRROR_LIST = 22,
  DRAG_ITEM_EQUIP_COSTUME = 24,
  DRAG_LAPINE_BOX = 26,
  DRAG_LAPINE_INGREDIENT = 27,
  DRAG_REFINING = 28,
  DRAG_MESSENGER_MEMBER = 29,
  DRAG_BARTER_MARKET = 31,
  DRAG_EXPANDED_BARTER = 32,
  DRAG_GRADE_ENCHANT = 33,
};

struct ModeDragState {
  /* 0308 */ int source;
  /* 030C */ std::uint32_t id;
  /* 0310 */ std::uint32_t itemValue;
  /* 0314 */ std::uint32_t price;
  /* 0318 */ int quantity;
  /* 031C */ int slot;
  /* 0320 */ std::uint32_t type;
  /* 0324 */ std::uint32_t refine;
  /* 0328 */ unsigned char identified;
  /* 032C */ std::string resource;
  /* 0344 */ std::string name;
  /* 035C */ std::string skillName;
  /* 0374 */ int skillLevel;
  /* 0378 */ std::uint32_t cards[4];
  /* 0388 */ unsigned char skill;
  /* 038C */ UIWindow* sourceWindow;
  /* 0390 */ int optionCount;
  /* 0394 */ ItemOptionRecord options[5];
  /* 03B0 */ int barterId;
  /* 03B4 */ std::uint16_t grade;
  /* 03B6 */ unsigned char uniqueItem;
  /* 03B8 */ std::uint32_t equipmentLocation;
  /* 03C0 */ std::int64_t viewId;

  void Clear();
  void BeginSkill(WindowManagerWindows& windows, ResourceMgr& resources, int source, std::uint32_t id, const char* name, int level, int slot);
  void BeginBarterItem(int source, const BarterMarketItem& item, int quantity, int slot);
  void BeginBarterEntry(int source, const BarterMarketEntry& entry, int quantity, int slot);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModeDragState) == 192);
#endif

#endif
