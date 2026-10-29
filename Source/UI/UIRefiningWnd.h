#ifndef RAGNAROK_SOURCE_UI_UIREFININGWND_H
#define RAGNAROK_SOURCE_UI_UIREFININGWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

#pragma pack(push, 1)
struct RefiningMaterial {
  /* 0000 */ unsigned char unknown_0000;
  /* 0001 */ std::uint32_t unknown_0001;
  /* 0005 */ std::uint32_t unknown_0005;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RefiningMaterial) == 9);
#endif

class UIRefining_BtnAgain : public UIBitmapButton {
public:
  void Enable(unsigned char value) override;

  /* 0120 */ int unknown_0120;
  /* 0124 */ unsigned char unknown_0124;
  /* 0125 */ unsigned char unknown_0125;
};

class UIRefining_BtnRefining : public UIBitmapButton {
public:
  void Enable(unsigned char value) override;

  /* 0120 */ int cost;
};

class UIRefining_BtnReturn : public UIBitmapButton {
public:
  void Paint() override;

  /* 0120 */ unsigned char highlighted;
};

class UIRefining_Notice : public UIStaticText {
public:
  void Slot19() override;
  void SetNotice(const char* value, int duration, unsigned char highlight);

  /* 00D8 */ std::string savedText;
  /* 00F0 */ unsigned char savedHighlight;
  /* 00F4 */ std::uint32_t expireTick;
  /* 00F8 */ unsigned char autoShown;
  /* 00F9 */ unsigned char highlight;
};

class UIRefiningWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  int SlotAt(int x, int y);
  void Reset();
  std::uint32_t SelectedMaterialValue();

  /* 00B4 */ UIRefining_BtnAgain* againButton;
  /* 00B8 */ UIRefining_BtnRefining* refiningButton;
  /* 00BC */ UIRefining_BtnReturn* returnButton;
  /* 00C0 */ UIButton* unknown_00C0;
  /* 00C4 */ UIRefining_Notice* notice;
  /* 00C8 */ int state;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ std::uint32_t stateTick;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ int selectedMaterial;
  /* 00E0 */ std::vector<RefiningMaterial> materials;
  /* 00EC */ unsigned char unknown_00EC;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ int unknown_00F4;
  /* 00F8 */ ITEM_INFO item;
  /* 01F0 */ unsigned char unknown_01F0;
  /* 01F4 */ std::vector<RefiningMaterial> unknown_01F4;
  /* 0200 */ int unknown_0200;
  /* 0208 */ ITEM_INFO unknown_0208;
};

#endif
