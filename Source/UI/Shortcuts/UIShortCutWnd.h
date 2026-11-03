#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_UISHORTCUTWND_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_UISHORTCUTWND_H

#include "UI/Shortcuts/ShortcutControls.h"
#include "UI/Shortcuts/ShortcutGrid.h"
#include "UI/Window/UIFrameWnd.h"

struct ModeDragState;
struct ShortcutPainting;
struct NormalItemSlot;
struct NormalItemSlotWindow;

class UIShortCutWnd : public UIFrameWnd {
public:
  UIShortCutWnd();
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

  friend class CGameMode;

private:
  void ClickControl(int id);
  void PlaceRowButtons();
  void ResizeRows();
  void RestoreLayout(NormalItemSlotWindow* value);

private:
  ShortcutControls m_controls;
  NormalItemSlotWindow* m_saved;
  NormalItemSlot* m_slots[36];
  ShortcutGrid m_grid;
  NormalItemSlot* m_itemSlots[9];
  ShortcutPainting* m_painting;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIShortCutWnd) == 408);
#endif

#endif
