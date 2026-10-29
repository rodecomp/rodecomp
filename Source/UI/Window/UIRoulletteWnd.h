#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIROULLETTEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIROULLETTEWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>

class UIRoulletteWnd : public UIFrameWnd {
public:
  ~UIRoulletteWnd() override;
  void Slot25(int x, int y) override;
  int CoinType() const;

  /* 00BC */ std::uint32_t unknown_00BC;
  /* 00C0 */ std::uint32_t unknown_00C0;
  /* 00C4 */ std::uint32_t unknown_00C4;
  /* 00C8 */ unsigned char m_enabled;
  /* 00D0 */ ITEM_INFO m_item;
  /* 01C8 */ std::list<ITEM_INFO> m_items;
  /* 01D0 */ UIWindow* unknown_01D0;
  /* 01D4 */ UIWindow* unknown_01D4;
  /* 01D8 */ UIWindow* unknown_01D8;
  /* 01DC */ UIWindow* unknown_01DC;
  /* 01E0 */ UIWindow* unknown_01E0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIRoulletteWnd, m_enabled) == 0xC8);
static_assert(offsetof(UIRoulletteWnd, m_item) == 0xD0);
static_assert(offsetof(UIRoulletteWnd, m_items) == 0x1C8);
static_assert(offsetof(UIRoulletteWnd, unknown_01E0) == 0x1E0);
static_assert(sizeof(UIRoulletteWnd) == 0x1E8);
#endif

#endif
