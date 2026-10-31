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
  UIItemStoreSubWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot35(int value) override;
  void Paint() override;

  __declspec(noinline) ITEM_INFO GetItemAt(int x, int y);
  void SetTabIndex(int index);
  void SetTitle(std::string title);
  bool Sort1();
  bool Sort2();
  bool Sort3();
  bool SortReversed1();
  bool SortReversed2();
  bool SortReversed3();
  bool sub_996950();
  bool sub_996AF0();
  bool sub_996C90();
  bool FilterByName1(std::string query);
  bool FilterByName2(std::string query);
  bool FilterByName3(std::string query);
  void Slot15(int width, int height) override;

  /* 00F0 */ unsigned char unknown_00F0[0x20];
  /* 0110 */ UIResizer* resizeHandle;
  /* 0114 */ UIStaticText* title;
  /* 0118 */ unsigned char unknown_0118[0x04];
  /* 011C */ int unknown_011C;
  /* 0120 */ std::string unknown_0120;
  /* 0138 */ std::string unknown_0138;
  /* 0150 */ WindowPlacement* saved;
  /* 0154 */ int tabIndex;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreSubWnd, saved) == 0x150);
static_assert(offsetof(UIItemStoreSubWnd, tabIndex) == 0x154);
static_assert(offsetof(UIItemStoreSubWnd, title) == 0x114);
#endif

#endif
