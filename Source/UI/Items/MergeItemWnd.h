#ifndef RAGNAROK_SOURCE_UI_ITEMS_MERGEITEMWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_MERGEITEMWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <vector>

class UIBitmapButton;
class UICheckBox;
class UIResizer;
class UIScrollBar;
struct UIButtonContext;

class CMergeItemWnd : public UIFrameWnd {
public:
  CMergeItemWnd(UIWindowContext& windows, UIFrameContext& frames);
  ~CMergeItemWnd() override;
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  __declspec(noinline) void CreateControls();
  void SelectSameItems(int id);
  __declspec(noinline) int ItemIdOf(int id);
  __declspec(noinline) int ItemAt(int x, int y);
  void FitToGrid(int width, int height);
  bool RequestMerge();
  __declspec(noinline) void UpdateScroll();

  /* 00B4 */ int m_cellSize;
  /* 00B8 */ int m_gridTop;
  /* 00BC */ int m_gridLeft;
  /* 00C0 */ int m_marginRight;
  /* 00C4 */ int m_marginBottom;
  /* 00C8 */ int m_slotOffsetX;
  /* 00CC */ int m_slotOffsetY;
  /* 00D0 */ int m_checkOffsetX;
  /* 00D4 */ int m_checkOffsetY;
  /* 00D8 */ int m_slotRadius;
  /* 00DC */ unsigned int m_minSelection;
  /* 00E0 */ UIBitmapButton* m_mergeButton;
  /* 00E4 */ UIBitmapButton* m_cancelButton;
  /* 00E8 */ UIBitmapButton* m_closeButton;
  /* 00EC */ std::vector<UICheckBox*> m_items;
  /* 00F8 */ UIResizer* m_resizer;
  /* 00FC */ UIScrollBar* m_scrollBar;
  /* 0100 */ int m_itemCount;
  /* 0104 */ int m_columns;
  /* 0108 */ int m_rows;
  /* 010C */ int m_visibleCount;
  /* 0110 */ int m_scrollRow;
  /* 0114 */ int m_scrollable;

protected:
  UIButtonContext* m_controls;
};

#endif
