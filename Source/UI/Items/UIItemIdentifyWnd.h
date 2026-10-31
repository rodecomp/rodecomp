#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMIDENTIFYWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMIDENTIFYWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

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
  /* 00D4 */ unsigned char unknown_00D4[0x18];
  /* 00EC */ WindowPlacement* m_placement;

  void Slot11() override;
  void UpdateScroll();
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;
};

#endif
