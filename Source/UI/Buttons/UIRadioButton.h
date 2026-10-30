#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIRADIOBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIRADIOBUTTON_H

#include "UI/Window/UIControl.h"

#include <cstddef>
#include <functional>

class CUICheckBox : public CUIImageButton {
public:
  /* 0120 */ bool checked;
  /* 0128 */ std::function<void(bool)> onCheck;
};

class CUIRadioButton : public CUICheckBox {
public:
  /* 0150 */ std::function<void(CUIRadioButton*)> onSelect;

  void SetOnSelect(std::function<void(CUIRadioButton*)> callback);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUICheckBox, onCheck) == 0x128);
static_assert(sizeof(CUICheckBox) == 0x150);
static_assert(offsetof(CUIRadioButton, onSelect) == 0x150);
static_assert(sizeof(CUIRadioButton) == 0x178);
#endif

#endif
