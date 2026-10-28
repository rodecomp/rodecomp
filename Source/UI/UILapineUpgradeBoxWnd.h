#ifndef RAGNAROK_SOURCE_UI_UILAPINEUPGRADEBOXWND_H
#define RAGNAROK_SOURCE_UI_UILAPINEUPGRADEBOXWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIBitmapButton;

class UILapineUpgradeBox_IngredientInfoWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int argument) override;

  /* 00B8 */ ItemInfo item;
  /* 01B0 */ unsigned char selected;
  /* 01B4 */ std::string tooltip;
  /* 01CC */ int quantity;
};

class UILapineUpgradeBoxWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  void SetWaiting(unsigned char value);

  /* 00B4 */ UIBitmapButton* closeButton;
  /* 00B8 */ UIBitmapButton* upgradeButton;
  /* 00BC */ UIBitmapButton* selectButton;
  /* 00C0 */ UIWindow* ingredientList;
  /* 00C4 */ int ingredientId;
  /* 00C8 */ std::string title;
  /* 00E0 */ ItemInfo item;
  /* 01DC */ std::string description;
  /* 01F4 */ std::string notice;
  /* 020C */ std::uint32_t noticeTick;
  /* 0210 */ std::uint32_t noticeDuration;
  /* 0214 */ unsigned char waiting;
};

#endif
