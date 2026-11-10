#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTORESUBWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTORESUBWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <string>

class UIResizer;
class UIStaticText;

class UIItemStoreSubWnd : public UIItemBaseWnd {
public:
  UIItemStoreSubWnd();
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void StoreInfo() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnWheel(int value) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  __declspec(noinline) ITEM_INFO GetItemAt(int x, int y);
  void SetTabIndex(int index);
  void SetTitle(std::string title);
  bool Sort1();
  bool Sort2();
  bool Sort3();
  bool SortReversed1();
  bool SortReversed2();
  bool SortReversed3();
  bool UpdateItemNames1();
  bool UpdateItemNames2();
  bool UpdateItemNames3();
  bool FilterByName1(std::string query);
  bool FilterByName2(std::string query);
  bool FilterByName3(std::string query);
  void OnCreate(int width, int height) override;

  /* 00F0 */ unsigned char padding_00F0[0x20];
  /* 0110 */ UIResizer* resizeHandle;
  /* 0114 */ UIStaticText* title;
  /* 0118 */ unsigned char padding_0118[0x04];
  /* 011C */ int padding_011C;
  /* 0120 */ std::string footerText;
  /* 0138 */ std::string nameFilter;
  /* 0150 */ ItemStoreFindSlotInfo* saved;
  /* 0154 */ int tabIndex;
  /* 0158 */ unsigned char sortEnabled;
  /* 0159 */ unsigned char padding_0159[7];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreSubWnd, saved) == 0x150);
static_assert(offsetof(UIItemStoreSubWnd, tabIndex) == 0x154);
static_assert(offsetof(UIItemStoreSubWnd, title) == 0x114);
#endif

#endif
