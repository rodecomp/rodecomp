#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIINGREDIENTINFOWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIINGREDIENTINFOWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

class UILapineBox_IngredientListWnd;

class UIIngredientInfoWnd : public UIFrameWnd {
  friend class UILapineBox_IngredientListWnd;

public:
  // OFFSET: 0x005FEF40
  // FUNCTION: fn_04139
  UIIngredientInfoWnd()
      : UIFrameWnd(0), selected(0), insufficient(0), quantity(0) {}
  ~UIIngredientInfoWnd() override;
  void Move(int x, int y) override;
  void DragAndDrop(int argument1, int argument2, std::intptr_t argument3) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnWheel(int argument) override;
  void Paint() override;
  void OnLBtnDblClk(int button, int unused) override;
  void SetIngredientItem(std::uint32_t id);

  /* 00B8 */ ITEM_INFO item;
  /* 01B0 */ unsigned char selected;
  /* 01B1 */ unsigned char insufficient;
  /* 01B4 */ std::string tooltip;
  /* 01CC */ int quantity;
};

#endif
