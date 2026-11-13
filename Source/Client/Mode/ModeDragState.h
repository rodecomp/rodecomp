#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODEDRAGSTATE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODEDRAGSTATE_H

#include <cstdint>
#include <string>

#include "UI/Window/WindowManagerWindows.h"
class ResourceMgr;
struct ITEM_INFO;
struct BarterMarketItem;
struct BarterMarketEntry;

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
  /* 038C */ std::uint32_t sourceWindow;
  /* 0390 */ int optionCount;
  /* 0394 */ unsigned char options[25];
  /* 03B0 */ int barterId;
  /* 03B4 */ std::uint16_t grade;
  /* 03B6 */ unsigned char uniqueItem;
  /* 03B8 */ std::uint32_t equipmentLocation;
  /* 03C0 */ std::int64_t viewId;

  void Clear();
  void BeginItem(WindowManagerWindows& windows, int source, const ITEM_INFO& item, int quantity, int slot);
  void BeginSkill(WindowManagerWindows& windows, ResourceMgr& resources, int source, std::uint32_t id, const char* name, int level, int slot);
  void BeginBarterItem(int source, const BarterMarketItem& item, int quantity, int slot);
  void BeginBarterEntry(int source, const BarterMarketEntry& entry, int quantity, int slot);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModeDragState) == 192);
#endif

#endif
