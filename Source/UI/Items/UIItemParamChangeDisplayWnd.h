#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <string>

struct WindowPlacement;

class UIItemParamChangeDisplayWnd : public UIFrameWnd {
public:
  UIItemParamChangeDisplayWnd();

  /* 00B4 */ int m_before;
  /* 00B8 */ int m_after;
  /* 00BC */ int m_beforeSecond;
  /* 00C0 */ int m_afterSecond;
  /* 00C4 */ int itemParam;
  /* 00C8 */ int itemSecondParam;
  /* 00CC */ int equippedParam;
  /* 00D0 */ int equippedSecondParam;
  /* 00D4 */ std::string m_label;
  /* 00EC */ int itemIndex;
  /* 00F0 */ WindowPlacement* m_placement;

  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  ITEM_INFO FindEquippedItem(int location);
  void SumEquippedStats();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemParamChangeDisplayWnd, m_placement) - offsetof(UIItemParamChangeDisplayWnd, itemParam) == 0x2C);
#endif

#endif
