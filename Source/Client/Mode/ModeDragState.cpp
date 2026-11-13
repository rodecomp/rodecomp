#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/CGameMode.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Contents/Items/BarterMarketItemMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemResources.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstring>

// OFFSET: 0x00CEC930
// FUNCTION: fn_21357
void CGameMode::ClearDrag() {
  this->drag.identified = 0;
  this->drag.id = 0;
  this->drag.name = "";
  this->drag.itemValue = 0;
  this->drag.resource = "";
  this->drag.source = 0;
  this->drag.quantity = 0;
  this->drag.slot = -1;
  this->drag.skillLevel = 0;
  this->drag.refine = 0;
  for (int card = 0; card < 4; ++card) {
    this->drag.cards[card] = 0;
  }
  this->drag.barterId = -1;
  this->drag.grade = 0;
}

// OFFSET: 0x00CFE140
// FUNCTION: fn_21429
void CGameMode::BeginDragItem(int sourceValue, const ITEM_INFO& item, int quantityValue, int slotValue) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);

  this->ClearDrag();
  if (item.identity[1] > 0) {
    this->drag.source = sourceValue;
    this->drag.skill = 0;
    this->drag.id = item.identity[1];
    this->drag.quantity = quantityValue;
    this->drag.resource = g_session.ItemDragResource(item.itemId.c_str(), item.identified);
    this->drag.name = item.itemId.c_str();
    this->drag.itemValue = item.prices[0];
    this->drag.price = item.prices[1];
    this->drag.type = item.identity[0];
    this->drag.identified = item.identified;
    this->drag.slot = slotValue;
    this->drag.refine = item.refiningLevel;
    std::memcpy(this->drag.cards, item.cards, sizeof(this->drag.cards));
    this->drag.optionCount = item.optionCount;

    for (int index = 0; index < this->drag.optionCount; ++index) {
      std::memcpy(this->drag.options + 5 * index, item.options + 5 * index, 5);
    }

    this->drag.grade = static_cast<std::uint16_t>(item.grade);
    this->drag.uniqueItem = item.flags[3];
    this->drag.equipmentLocation = item.identity[2];
    this->drag.viewId = item.sprite;
  }
}

// OFFSET: 0x00CFE2A0
// FUNCTION: fn_21430
void CGameMode::BeginDragSkill(int sourceValue, std::uint32_t idValue, const char* nameValue, int level, int slotValue) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
  this->ClearDrag();
  this->drag.source = sourceValue;
  this->drag.id = idValue;
  this->drag.identified = 0;
  this->drag.resource = g_session.SkillDragResource(nameValue);
  this->drag.name = nameValue;
  this->drag.skillName = nameValue;
  this->drag.skillLevel = level;
  this->drag.skill = 1;
  this->drag.slot = slotValue;
}

// OFFSET: 0x00CFDF90
// FUNCTION: fn_21427
void CGameMode::BeginDragBarterItem(int sourceValue, const BarterMarketItem& item, int quantityValue, int slotValue) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
  this->ClearDrag();
  this->drag.source = sourceValue;
  this->drag.quantity = quantityValue;
  this->drag.skill = 0;
  this->drag.resource = g_session.ItemDragResource(item.name.c_str(), item.flag_24);
  this->drag.name = item.name.c_str();
  this->drag.itemValue = item.field_20;
  this->drag.type = item.field_1c;
  this->drag.identified = item.flag_24;
  this->drag.slot = slotValue;
  this->drag.barterId = item.id;
}

// OFFSET: 0x00CFE060
// FUNCTION: fn_21428
void CGameMode::BeginDragBarterEntry(int sourceValue, const BarterMarketEntry& entry, int quantityValue, int slotValue) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
  this->ClearDrag();
  this->drag.source = sourceValue;
  this->drag.quantity = quantityValue;
  this->drag.skill = 0;
  this->drag.resource = g_session.ItemDragResource(entry.name.c_str(), entry.flag_24);
  this->drag.name = entry.name.c_str();
  this->drag.itemValue = entry.field_8;
  this->drag.type = entry.field_0;
  this->drag.identified = entry.flag_24;
  this->drag.slot = slotValue;
  this->drag.refine = entry.field_28;
  this->drag.barterId = entry.id;
}

// INLINED
void ModeDragState::Clear() {
  this->identified = 0;
  this->id = 0;
  this->name = "";
  this->itemValue = 0;
  this->resource = "";
  this->source = 0;
  this->quantity = 0;
  this->slot = -1;
  this->skillLevel = 0;
  this->refine = 0;
  std::memset(this->cards, 0, sizeof(this->cards));
  this->barterId = -1;
  this->grade = 0;
}

// INLINED
void ModeDragState::BeginItem(
  WindowManagerWindows& windows, int sourceValue, const ITEM_INFO& item, int quantityValue, int slotValue
) {
  windows.ShowWindowById(WID_ITEMDROPCNTWND, 0);

  this->Clear();
  if (item.identity[1] == 0) {
    return;
  }

  this->source = sourceValue;
  this->skill = 0;
  this->id = item.identity[1];
  this->quantity = quantityValue;
  this->resource = g_session.ItemDragResource(item.itemId.c_str(), item.identified);
  this->name = item.itemId;
  this->itemValue = item.prices[0];
  this->price = item.prices[1];
  this->type = item.identity[0];
  this->identified = item.identified;
  this->slot = slotValue;
  this->refine = item.refiningLevel;
  std::memcpy(this->cards, item.cards, sizeof(this->cards));
  this->optionCount = item.optionCount;

  for (int index = 0; index < this->optionCount; ++index) {
    std::memcpy(this->options + 5 * index, item.options + 5 * index, 5);
  }

  this->grade = static_cast<std::uint16_t>(item.grade);
  this->uniqueItem = item.flags[3];
  this->equipmentLocation = item.identity[2];
  this->viewId = item.sprite;
}

// INLINED
void ModeDragState::BeginSkill(
  WindowManagerWindows& windows, ResourceMgr& resources, int sourceValue, std::uint32_t idValue, const char* nameValue, int level, int slotValue
) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
  this->Clear();
  this->source = sourceValue;
  this->id = idValue;
  this->identified = 0;
  this->resource = g_session.SkillDragResource(nameValue);
  this->name = nameValue;
  this->skillName = nameValue;
  this->skillLevel = level;
  this->skill = 1;
  this->slot = slotValue;
}

// INLINED
void ModeDragState::BeginBarterItem(int sourceValue, const BarterMarketItem& item, int quantityValue, int slotValue) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
  this->Clear();
  this->source = sourceValue;
  this->quantity = quantityValue;
  this->skill = 0;
  this->resource = g_session.ItemDragResource(item.name.c_str(), item.flag_24);
  this->name = item.name.c_str();
  this->itemValue = item.field_20;
  this->type = item.field_1c;
  this->identified = item.flag_24;
  this->slot = slotValue;
  this->barterId = item.id;
}

// INLINED
void ModeDragState::BeginBarterEntry(int sourceValue, const BarterMarketEntry& entry, int quantityValue, int slotValue) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
  this->Clear();
  this->source = sourceValue;
  this->quantity = quantityValue;
  this->skill = 0;
  this->resource = g_session.ItemDragResource(entry.name.c_str(), entry.flag_24);
  this->name = entry.name.c_str();
  this->itemValue = entry.field_8;
  this->type = entry.field_0;
  this->identified = entry.flag_24;
  this->slot = slotValue;
  this->refine = entry.field_28;
  this->barterId = entry.id;
}
