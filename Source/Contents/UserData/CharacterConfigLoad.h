#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_CHARACTERCONFIGLOAD_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_CHARACTERCONFIGLOAD_H

struct NormalItemSlot;
struct NormalItemSlotWindow;
struct ShortcutVisibility;
struct ServiceOptions;
struct SkillLevelRegistry;
class WindowManagerWindows;

struct CharacterConfigLoad {
  int* useLevels;
  int* homunculusLevels;
  ServiceOptions& serviceOptions;
  SkillLevelRegistry& registry;
  bool& sortPrivate;
  NormalItemSlot* items;
  NormalItemSlotWindow& saved;
  bool& itemSlotActive;
  WindowManagerWindows& windows;
  ShortcutVisibility& visibility;
};

#endif
