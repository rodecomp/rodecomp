#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <string>

struct UIButtonContext;
struct WindowPlacement;

class UIItemParamChangeDisplayWnd : public UIFrameWnd {
public:
  UIItemParamChangeDisplayWnd();

  /* 00B4 */ int m_before;
  /* 00B8 */ int m_after;
  /* 00BC */ int m_beforeSecond;
  /* 00C0 */ int m_afterSecond;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ std::string m_label;
  /* 00EC */ int unknown_00EC;
  /* 00F0 */ WindowPlacement* m_placement;

  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  ITEM_INFO FindEquippedItem(int location);
  void SumEquippedStats();

protected:
  UIButtonContext* m_controls;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemParamChangeDisplayWnd, m_placement) - offsetof(UIItemParamChangeDisplayWnd, unknown_00C4) == 0x2C);
#endif

#endif
