#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_UISHORTCUTWND_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_UISHORTCUTWND_H

#include "UI/Shortcuts/ShortcutControls.h"
#include "UI/Shortcuts/ShortcutGrid.h"
#include "UI/Window/UIFrameWnd.h"

struct ShortcutBindings;
struct ShortcutPainting;
struct ShortcutPress;
struct ShortcutDrop;
struct ShortcutTooltip;
struct ShortcutDetails;
struct ShortcutUse;
struct ShortcutVisibility;
struct UIButtonContext;
struct CooldownClock;
struct ModeDragState;
struct NormalItemSlot;
struct NormalItemSlotWindow;
class AutomaticSkills;

struct ShortcutWindowContext {
  UIButtonContext& buttons;
  ShortcutBindings& bindings;
  ShortcutPainting& painting;
  ShortcutPress& press;
  ShortcutDrop& drop;
  ShortcutTooltip& tooltip;
  ShortcutDetails& details;
  ShortcutUse& use;
  ShortcutVisibility& visibility;
  AutomaticSkills& automatic;
  const CooldownClock& clock;
  bool& itemSlotActive;
  unsigned char& currentRow;
  int& rotation0;
  int& rotation1;
  const int& cursorX;
  const int& cursorY;
};

class UIShortCutWnd : public UIFrameWnd {
public:
  UIShortCutWnd(UIWindowContext& windows, UIFrameContext& frames, ShortcutWindowContext& shortcuts);
  ~UIShortCutWnd() override;

  bool Slot03() override;
  void Move(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  void Show(int visible) override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

private:
  ShortcutControls m_controls;
  NormalItemSlotWindow* m_saved;
  NormalItemSlot* m_slots[36];
  ShortcutGrid m_grid;
  NormalItemSlot* m_itemSlots[9];
  ShortcutWindowContext* m_shortcuts;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIShortCutWnd) == 408);
#endif

#endif
