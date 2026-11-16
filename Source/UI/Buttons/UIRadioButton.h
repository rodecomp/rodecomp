#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIRADIOBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIRADIOBUTTON_H

#include "UI/Window/UIControl.h"

#include <cstddef>
#include <functional>

class CUICheckBox : public CUIImageButton {
public:
  /* 0120 */ bool checked;
  /* 0128 */ std::function<void(bool)> onCheck;

  CUICheckBox();

  void SetOnCheck(std::function<void(bool)> callback);
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
};

class CUIRadioButton : public CUICheckBox {
public:
  /* 0150 */ std::function<void(CUIRadioButton*)> onSelect;

  CUIRadioButton();
  void SetOnSelect(std::function<void(CUIRadioButton*)> callback);
  void OnLBtnDown(int x, int y) override;
};

class CUIRadioItem : public CUIControl {
public:
  /* 0080 */ CUIRadioButton* radioButton;
  /* 0088 */ std::function<void(CUIRadioItem*)> onSelect;

  void OnCreate(int width, int height) override;
  bool IsChecked();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUICheckBox, onCheck) == 0x128);
static_assert(sizeof(CUICheckBox) == 0x150);
static_assert(offsetof(CUIRadioButton, onSelect) == 0x150);
static_assert(sizeof(CUIRadioButton) == 0x178);
static_assert(offsetof(CUIRadioItem, onSelect) == 0x88);
#endif

// OFFSET: 0x009E8430
// FUNCTION: fn_14492
inline CUICheckBox::CUICheckBox() : CUIImageButton(), checked(false), onCheck() {
}

// OFFSET: 0x00A19D30
// FUNCTION: fn_15347
inline CUIRadioButton::CUIRadioButton() : CUICheckBox(), onSelect() {
}

#endif
