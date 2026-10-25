#include "Client/Mode/ModeDragState.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemResources.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstring>

// OFFSET: 0x00CEC930
// FUNCTION: fn_21357
void ModeDragState::Clear() {
  this->identified = 0;
  this->id = 0;
  this->name.clear();
  this->unknown_0310 = 0;
  this->resource.clear();
  this->source = 0;
  this->quantity = 0;
  this->slot = -1;
  this->skillLevel = 0;
  this->refine = 0;
  std::memset(this->cards, 0, sizeof(this->cards));
  this->unknown_03B0 = -1;
  this->grade = 0;
}

// OFFSET: 0x00CFE140
// FUNCTION: fn_21429
void ModeDragState::BeginItem(
  WindowManagerWindows& windows, ItemDescriptions& descriptions, int sourceValue, const ItemInfo& item, int quantityValue, int slotValue
) {
  UIWindow* window = windows.EnsureWindow(15);
  if (window->geometry.visible != 0) {
    window->Show(0);
  }

  this->Clear();
  if (item.unknown_0000[1] == 0) {
    return;
  }

  this->source = sourceValue;
  this->skill = 0;
  this->id = item.unknown_0000[1];
  this->quantity = quantityValue;
  this->resource = ItemDragResource(descriptions, item.itemId.c_str(), item.identified);
  this->name = item.itemId;
  this->unknown_0310 = item.unknown_0014[0];
  this->unknown_0314 = item.unknown_0014[1];
  this->type = item.unknown_0000[0];
  this->identified = item.identified;
  this->slot = slotValue;
  this->refine = item.unknown_0060;
  std::memcpy(this->cards, item.unknown_001C, sizeof(this->cards));
  this->optionCount = item.unknown_0098;

  for (int index = 0; index < this->optionCount; ++index) {
    std::memcpy(this->options + 5 * index, item.unknown_009C + 5 * index, 5);
  }

  this->grade = static_cast<std::uint16_t>(item.unknown_0088);
  this->unknown_03B6 = item.unknown_0074[3];
  this->unknown_03B8 = item.unknown_0000[2];
  this->unknown_03C0 = item.unknown_0070;
}

// OFFSET: 0x00CFE2A0
// FUNCTION: fn_21430
void ModeDragState::BeginSkill(
  WindowManagerWindows& windows, ResourceMgr& resources, int sourceValue, std::uint32_t idValue, const char* nameValue, int level, int slotValue
) {
  UIWindow* window = windows.EnsureWindow(15);
  if (window->geometry.visible != 0) {
    window->Show(0);
  }

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
