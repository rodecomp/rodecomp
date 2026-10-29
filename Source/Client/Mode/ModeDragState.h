#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODEDRAGSTATE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODEDRAGSTATE_H

#include <cstdint>
#include <string>

class WindowManagerWindows;
class ResourceMgr;
struct ItemDescriptions;
struct ITEM_INFO;

struct ModeDragState {
  /* 0308 */ int source;
  /* 030C */ std::uint32_t id;
  /* 0310 */ std::uint32_t unknown_0310;
  /* 0314 */ std::uint32_t unknown_0314;
  /* 0318 */ int quantity;
  /* 031C */ int slot;
  /* 0320 */ std::uint32_t type;
  /* 0324 */ std::uint32_t refine;
  /* 0328 */ unsigned char identified;
  /* 032C */ std::string resource;
  /* 0344 */ std::string name;
  /* 035C */ std::string skillName;
  /* 0374 */ int skillLevel;
  /* 0378 */ unsigned char cards[16];
  /* 0388 */ unsigned char skill;
  /* 038C */ std::uint32_t unknown_038C;
  /* 0390 */ int optionCount;
  /* 0394 */ unsigned char options[25];
  /* 03B0 */ int unknown_03B0;
  /* 03B4 */ std::uint16_t grade;
  /* 03B6 */ unsigned char unknown_03B6;
  /* 03B8 */ std::uint32_t unknown_03B8;
  /* 03C0 */ std::int64_t unknown_03C0;

  void Clear();
  void BeginItem(WindowManagerWindows& windows, ItemDescriptions& descriptions, int source, const ITEM_INFO& item, int quantity, int slot);
  void BeginSkill(WindowManagerWindows& windows, ResourceMgr& resources, int source, std::uint32_t id, const char* name, int level, int slot);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModeDragState) == 192);
#endif

#endif
