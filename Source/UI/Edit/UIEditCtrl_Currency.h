#ifndef RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_CURRENCY_H
#define RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_CURRENCY_H

#include "UI/Edit/UIEditCtrl.h"

#include <cstddef>
#include <cstdint>

class UIEditCtrl_Custom : public UIEditCtrl2 {
public:
  void Synchronize() override;
  void Trim() override;
  void DrawLabel(int x, int y, const char* text);

  /* 011C */ unsigned char unknown_011C;
  /* 0120 */ int unknown_0120;
  /* 0124 */ int unknown_0124;
  /* 0128 */ int unknown_0128;
};

class UIEditCtrl_Currency : public UIEditCtrl_Custom {
public:
  void Slot19() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 012C */ std::size_t formattedLength;
};

#endif
