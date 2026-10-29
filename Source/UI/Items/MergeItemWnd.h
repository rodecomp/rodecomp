#ifndef RAGNAROK_SOURCE_UI_ITEMS_MERGEITEMWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_MERGEITEMWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <vector>

class UICheckBox;

class CMergeItemWnd : public UIFrameWnd {
public:
  ~CMergeItemWnd() override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot35(int delta) override;
  void SelectSameItems(int id);
  __declspec(noinline) int ItemIdOf(int id);
  __declspec(noinline) int ItemAt(int x, int y);
  void FitToGrid(int width, int height);
  bool RequestMerge();

  /* 00B4 */ int m_cellSize;
  /* 00B8 */ int m_gridTop;
  /* 00BC */ int m_gridLeft;
  /* 00C0 */ int m_marginRight;
  /* 00C4 */ int m_marginBottom;
  /* 00C8 */ int m_slotOffsetX;
  /* 00CC */ int m_slotOffsetY;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int m_slotRadius;
  /* 00DC */ unsigned int m_minSelection;
  /* 00E0 */ int unknown_00E0;
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ int unknown_00E8;
  /* 00EC */ std::vector<UICheckBox*> m_items;
  /* 00F8 */ UIWindow* unknown_00F8;
  /* 00FC */ UIWindow* m_scrollBar;
  /* 0100 */ int m_itemCount;
  /* 0104 */ int m_columns;
  /* 0108 */ int m_rows;
  /* 010C */ int m_visibleCount;
  /* 0110 */ int m_scrollRow;
  /* 0114 */ int m_scrollable;
};

#endif
