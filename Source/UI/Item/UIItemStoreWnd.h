#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <string>

class UICheckBox;
class UIStaticText;

class UIItemStoreWnd : public UIItemBaseWnd {
public:
  UIItemStoreWnd(UIWindowContext& windows, UIFrameContext& frames);
  __declspec(noinline) ~UIItemStoreWnd() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Paint() override;
  __declspec(noinline) ITEM_INFO GetItemAt(int x, int y);
  void SetTabChecked(int index, bool checked);
  void SetTitle(std::string title);

  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ unsigned char unknown_00F4[0x04];
  /* 00F8 */ unsigned char unknown_00F8[0x08];
  /* 0100 */ UICheckBox* tabs[8];
  /* 0120 */ int unknown_0120[8];
  /* 0140 */ int unknown_0140;
  /* 0144 */ std::string unknown_0144;
  /* 015C */ UIStaticText* title;
  /* 0160 */ unsigned char unknown_0160[0x04];
  /* 0164 */ WindowPlacement* saved;
  /* 0168 */ unsigned char unknown_0168[0x04];
  /* 016C */ int unknown_016C;
  /* 0170 */ int unknown_0170;
  /* 0174 */ int unknown_0174;
  /* 0178 */ int unknown_0178;
  /* 017C */ int unknown_017C;
  /* 0180 */ int unknown_0180;
  /* 0184 */ unsigned char unknown_0184[0x04];
  /* 0188 */ int unknown_0188;
  /* 018C */ int unknown_018C;
  /* 0190 */ unsigned char unknown_0190;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreWnd, tabs) == 0x100);
static_assert(offsetof(UIItemStoreWnd, saved) == 0x164);
#endif

#endif
