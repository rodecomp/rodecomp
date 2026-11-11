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

  UIItemIdentifyWnd();
  // INLINED
  ~UIItemIdentifyWnd() override {
  }
  void StoreInfo() override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  void UpdateScroll();
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnWheel(int delta) override;
  std::vector<unsigned char> GetReplayData() override;
  ITEM_INFO GetItem(int index, int* found);
  ITEM_INFO GetSelectedItem(int* found);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UIMakingArrowListWnd : public UIItemIdentifyWnd {
public:
  UIMakingArrowListWnd();
  void Paint() override;
  std::vector<unsigned char> GetReplayData() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00F0 */ std::int16_t listType;
};

class UIPetEggListWnd : public UIItemIdentifyWnd {
public:
  UIPetEggListWnd();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIItemCompositionWnd : public UIItemIdentifyWnd {
public:
  UIItemCompositionWnd();
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  std::vector<unsigned char> GetReplayData() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00F0 */ WindowPlacement* m_compositionPlacement;
  /* 00F4 */ int m_compositionState;
};

#endif
