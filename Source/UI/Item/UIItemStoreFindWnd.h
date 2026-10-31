#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREFINDWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREFINDWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <string>

class UIBitmapButton;
class UIResizer;
class UIStaticText;

class UIItemStoreFindWnd : public UIItemBaseWnd {
public:
  UIItemStoreFindWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot35(int value) override;
  void Paint() override;

  __declspec(noinline) ITEM_INFO GetItemAt(int x, int y);
  void SetTitle(std::string title);
  void SetSearchText(std::string text);
  void Slot15(int width, int height) override;

  /* 00F0 */ unsigned char unknown_00F0[0xC];
  /* 00FC */ UIBitmapButton* closeButton;
  /* 0100 */ unsigned char unknown_0100[0x14];
  /* 0114 */ UIResizer* resizeHandle;
  /* 0118 */ UIStaticText* title;
  /* 011C */ unsigned char unknown_011C[0x4];
  /* 0120 */ int unknown_0120;
  /* 0124 */ std::string unknown_0124;
  /* 013C */ std::string searchText;
  /* 0154 */ ItemStoreFindWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreFindWnd, unknown_0124) == 0x124);
static_assert(offsetof(UIItemStoreFindWnd, searchText) == 0x13C);
static_assert(offsetof(UIItemStoreFindWnd, saved) == 0x154);
static_assert(offsetof(UIItemStoreFindWnd, closeButton) == 0xFC);
static_assert(offsetof(UIItemStoreFindWnd, title) == 0x118);
#endif

#endif
