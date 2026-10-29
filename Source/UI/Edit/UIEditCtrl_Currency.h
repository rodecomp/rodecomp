#ifndef RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_CURRENCY_H
#define RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_CURRENCY_H

#include "UI/Edit/UIEditCtrl.h"

#include <cstddef>

class UIEditCtrl_Custom : public UIEditCtrl2 {
public:
  /* 011C */ unsigned char unknown_011C;
  /* 0120 */ int unknown_0120;
  /* 0124 */ int unknown_0124;
  /* 0128 */ int unknown_0128;
};

class UIEditCtrl_Currency : public UIEditCtrl_Custom {
public:
  void Slot19() override;

  /* 012C */ std::size_t formattedLength;
};

#endif
