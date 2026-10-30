#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct WindowPlacement;

class UIItemParamChangeDisplayWnd : public UIFrameWnd {
public:
  /* 00B4 */ int m_before;
  /* 00B8 */ int m_after;
  /* 00BC */ int m_beforeSecond;
  /* 00C0 */ int m_afterSecond;
  /* 00C4 */ unsigned char unknown_00C4[0x2C];
  /* 00F0 */ WindowPlacement* m_placement;

  void Slot11() override;
  void Paint() override;

  ITEM_INFO FindEquippedItem(int location);
  void SumEquippedStats();

protected:
  UIButtonContext* m_controls;
};

#endif
