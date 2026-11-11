#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_UISHORTCUTWND_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_UISHORTCUTWND_H

#include "UI/Shortcuts/ShortcutGrid.h"
#include "UI/Window/UIFrameWnd.h"

struct ModeDragState;
class UICheckBox;
class UIBitmapButton;
struct NormalItemSlot;
struct NormalItemSlotWindow;

class UIShortCutWnd : public UIFrameWnd {
public:
  UIShortCutWnd();
  ~UIShortCutWnd() override;

  bool NeedsFrameRefresh() override;
  void Move(int x, int y) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void StoreInfo() override;
  void Show(int visible) override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  friend class CGameMode;

private:
  void ClickControl(int id);
  int HitTestSlot(int x, int y);
  void PlaceRowButtons();
  void ResizeRows();
  void RestoreLayout(NormalItemSlotWindow* value);

private:
  UICheckBox* m_bank;
  NormalItemSlotWindow* m_saved;
  UIBitmapButton* m_decrease;
  UIBitmapButton* m_increase;
  NormalItemSlot* m_slots[36];
  ShortcutGrid m_grid;
  NormalItemSlot* m_itemSlots[9];
  UIBitmapButton* m_showItems;
  UIBitmapButton* m_hideItems;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIShortCutWnd) == 0x18C);
#endif

#endif
