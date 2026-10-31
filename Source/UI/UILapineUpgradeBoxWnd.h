#ifndef RAGNAROK_SOURCE_UI_UILAPINEUPGRADEBOXWND_H
#define RAGNAROK_SOURCE_UI_UILAPINEUPGRADEBOXWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIBitmapButton;
struct UIButtonContext;
class UILapineUpgradeBox_IngredientListWnd;

class UILapineUpgradeBox_IngredientInfoWnd : public UIFrameWnd {
public:
  // INLINED
  UILapineUpgradeBox_IngredientInfoWnd(UIWindowContext& windows, UIFrameContext& frames)
      : UIFrameWnd(windows, frames, 0), selected(0), quantity(0) {}

  void Move(int x, int y) override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int argument) override;

  /* 00B8 */ ITEM_INFO item;
  /* 01B0 */ unsigned char selected;
  /* 01B4 */ std::string tooltip;
  /* 01CC */ int quantity;

protected:
  UIButtonContext* m_controls;
};

class UILapineUpgradeBoxWnd : public UIFrameWnd {
  friend class UILapineUpgradeBox_IngredientListWnd;

public:
  ~UILapineUpgradeBoxWnd() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void SetWaiting(unsigned char value);
  UILapineUpgradeBox_IngredientInfoWnd* FindSelected();
  void SetIngredient(int id);
  void AddMaterial(int id);
  // INLINED
  UIFrameContext& FrameContext() { return *this->m_frameContext; }

  /* 00B4 */ UIBitmapButton* closeButton;
  /* 00B8 */ UIBitmapButton* upgradeButton;
  /* 00BC */ UIBitmapButton* selectButton;
  /* 00C0 */ UILapineUpgradeBox_IngredientListWnd* ingredientList;
  /* 00C4 */ int ingredientId;
  /* 00C8 */ std::string title;
  /* 00E0 */ ITEM_INFO item;
  /* 01D8 */ int minOptionCount;
  /* 01DC */ std::string description;
  /* 01F4 */ std::string notice;
  /* 020C */ std::uint32_t noticeTick;
  /* 0210 */ std::uint32_t noticeDuration;
  /* 0214 */ unsigned char waiting;

protected:
  UIButtonContext* m_controls;
};

#endif
