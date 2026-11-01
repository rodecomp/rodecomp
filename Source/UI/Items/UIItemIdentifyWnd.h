#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMIDENTIFYWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMIDENTIFYWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <string>

struct WindowPlacement;

class UIItemIdentifyWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIWindow* m_scrollBar;
  /* 00B8 */ int m_scroll;
  /* 00BC */ int m_selected;
  /* 00C0 */ int m_columns;
  /* 00C4 */ int m_rows;
  /* 00C8 */ int m_scrollable;
  /* 00CC */ std::list<ITEM_INFO> m_items;
  /* 00D4 */ std::string m_title;
  /* 00EC */ WindowPlacement* m_placement;

  UIItemIdentifyWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;
  void Paint() override;
  void Slot15(int width, int height) override;
  void UpdateScroll();
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;
  std::vector<unsigned char> Slot44() override;
  ITEM_INFO GetItem(int index, int* found);
  ITEM_INFO GetSelectedItem(int* found);
};

class UIMakingArrowListWnd : public UIItemIdentifyWnd {
public:
  UIMakingArrowListWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  std::vector<unsigned char> Slot44() override;

  /* 00F0 */ std::int16_t unknown_00F0;
};

class UIPetEggListWnd : public UIItemIdentifyWnd {
public:
  UIPetEggListWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
};

class UIItemCompositionWnd : public UIItemIdentifyWnd {
public:
  UIItemCompositionWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;
  void Slot15(int width, int height) override;
  std::vector<unsigned char> Slot44() override;

  /* 00F0 */ WindowPlacement* m_compositionPlacement;
  /* 00F4 */ int m_compositionState;
};

#endif
