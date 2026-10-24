#ifndef RAGNAROK_SOURCE_CONTENTS_CHARACTERCONFIGLOAD_H
#define RAGNAROK_SOURCE_CONTENTS_CHARACTERCONFIGLOAD_H

struct NormalItemSlot;
struct NormalItemSlotWindow;
struct ShortcutVisibility;
struct ServiceOptions;
struct ClientOptions;
struct SkillLevelRegistry;
class WindowManagerWindows;

struct CharacterConfigLoad {
  int* useLevels;
  int* homunculusLevels;
  ServiceOptions& serviceOptions;
  ClientOptions& options;
  SkillLevelRegistry& registry;
  bool& sortPrivate;
  NormalItemSlot* items;
  NormalItemSlotWindow& saved;
  bool& itemSlotActive;
  WindowManagerWindows& windows;
  ShortcutVisibility& visibility;
};

#endif
