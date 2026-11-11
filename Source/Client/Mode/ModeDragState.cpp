#include "Client/Mode/ModeDragState.h"
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

// OFFSET: 0x00CFE140
// FUNCTION: fn_21429
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

// OFFSET: 0x00CFE2A0
// FUNCTION: fn_21430
void ModeDragState::BeginSkill(
  WindowManagerWindows& windows, ResourceMgr& resources, int sourceValue, std::uint32_t idValue, const char* nameValue, int level, int slotValue
) {
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
  this->Clear();
  this->source = sourceValue;
  this->id = idValue;
  this->identified = 0;
  this->resource = SkillDragResource(resources, nameValue);
  this->name = nameValue;
  this->skillName = nameValue;
  this->skillLevel = level;
  this->skill = 1;
  this->slot = slotValue;
}

// OFFSET: 0x00CFDF90
// FUNCTION: fn_21427
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

// OFFSET: 0x00CFE060
// FUNCTION: fn_21428
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
