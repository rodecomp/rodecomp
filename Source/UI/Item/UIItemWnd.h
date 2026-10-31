#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UITabControl;

class UIItemWnd : public UIItemBaseWnd {
public:
  UIItemWnd();
  void Slot11() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot18() override;
  void Slot19() override;
  void Slot28(int x, int y) override;

  ITEM_INFO GetItemAt(int x, int y);
  void ReleaseSelection(unsigned char keep);
  void DrawNewItemMark(int x, int y);
  void RefreshOptionBox();

  /* 00F0 */ ItemWndInfo* saved;
  /* 00F4 */ unsigned char unknown_00F4[0xC];
  /* 0100 */ int unknown_0100;
  /* 0104 */ int unknown_0104;
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ int unknown_0110;
  /* 0114 */ unsigned char unknown_0114[0x4];
  /* 0118 */ UITabControl* tabs;
  /* 011C */ UIWindow* optionBox;
  /* 0120 */ unsigned char unknown_0120[0x8];
  /* 0128 */ UIWindow* unknown_0128;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemWnd, saved) == 0xF0);
static_assert(offsetof(UIItemWnd, unknown_0110) == 0x110);
static_assert(offsetof(UIItemWnd, tabs) == 0x118);
static_assert(offsetof(UIItemWnd, unknown_0128) == 0x128);
#endif

#endif
