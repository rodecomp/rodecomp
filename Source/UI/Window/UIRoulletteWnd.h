#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIROULLETTEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIROULLETTEWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>

class UIBitmapButton;
class UISpriteButton;
class UIStaticText;

extern int g_effectDetailLevel;

class UIRoulletteWnd : public UIFrameWnd {
public:
  ~UIRoulletteWnd() override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  int CoinType() const;
  ITEM_INFO ItemAt(int x, int y);
  void UpdateButtons();

  /* 00BC */ UIBitmapButton* m_prizeButton;
  /* 00C0 */ UISpriteButton* m_prizeSprite;
  /* 00C4 */ UISpriteButton* m_stageArrow;
  /* 00C8 */ unsigned char m_enabled;
  /* 00B4 */ UIBitmapButton* m_startButton;
  /* 00D0 */ ITEM_INFO m_item;
  /* 01C8 */ std::list<ITEM_INFO> m_items;
  /* 01D0 */ int m_column;
  /* 01D4 */ int m_row;
  /* 01D8 */ UIStaticText* m_goldText;
  /* 01DC */ UIStaticText* m_silverText;
  /* 01E0 */ UIStaticText* m_bronzeText;
  /* 00B8 */ UISpriteButton* m_startSprite;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIRoulletteWnd, m_prizeButton) == 0xBC);
static_assert(offsetof(UIRoulletteWnd, m_enabled) == 0xC8);
static_assert(offsetof(UIRoulletteWnd, m_item) == 0xD0);
static_assert(offsetof(UIRoulletteWnd, m_items) == 0x1C8);
static_assert(offsetof(UIRoulletteWnd, m_column) == 0x1D0);
static_assert(offsetof(UIRoulletteWnd, m_bronzeText) == 0x1E0);
static_assert(sizeof(UIRoulletteWnd) == 0x1E8);
#endif

#endif
