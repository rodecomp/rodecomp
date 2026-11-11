#ifndef RAGNAROK_SOURCE_UI_ITEMS_UILAPINEUPGRADEBOXWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UILAPINEUPGRADEBOXWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

class UIBitmapButton;
class UILapineUpgradeBox_IngredientListWnd;

class UILapineUpgradeBox_IngredientInfoWnd : public UIFrameWnd {
public:
  UILapineUpgradeBox_IngredientInfoWnd();

  void Move(int x, int y) override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnWheel(int argument) override;
  void SetItem(std::uint32_t identity);

  /* 00B8 */ ITEM_INFO item;
  /* 01B0 */ unsigned char selected;
  /* 01B4 */ std::string tooltip;
  /* 01CC */ int quantity;
};

class UILapineUpgradeBoxWnd : public UIFrameWnd {
  friend class UILapineUpgradeBox_IngredientListWnd;

public:
  UILapineUpgradeBoxWnd();
  ~UILapineUpgradeBoxWnd() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SetWaiting(unsigned char value);
  UILapineUpgradeBox_IngredientInfoWnd* FindSelected();
  void SetIngredient(int id);
  void AddMaterial(int id);
  // INLINED

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
};

#endif
