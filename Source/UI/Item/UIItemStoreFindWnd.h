#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREFINDWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREFINDWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <string>

class UIItemStoreFindWnd : public UIItemBaseWnd {
public:
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot35(int value) override;

  __declspec(noinline) ITEM_INFO GetItemAt(int x, int y);
  void SetSearchText(std::string text);

  /* 00F0 */ unsigned char unknown_00F0[0x34];
  /* 0124 */ std::string unknown_0124;
  /* 013C */ std::string searchText;
  /* 0154 */ ItemStoreFindWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreFindWnd, unknown_0124) == 0x124);
static_assert(offsetof(UIItemStoreFindWnd, searchText) == 0x13C);
static_assert(offsetof(UIItemStoreFindWnd, saved) == 0x154);
#endif

#endif
