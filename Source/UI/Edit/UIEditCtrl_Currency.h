#ifndef RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_CURRENCY_H
#define RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_CURRENCY_H

#include "UI/Edit/UIEditCtrl.h"

#include <cstddef>
#include <cstdint>

class UIEditCtrl_Custom : public UIEditCtrl2 {
public:
  UIEditCtrl_Custom();
  void PaintText() override;
  void Synchronize() override;
  void Trim() override;
  void DrawLabel(int x, int y, const char* text);

  /* 011C */ unsigned char labelBold;
  /* 0120 */ int labelOffsetX;
  /* 0124 */ int labelOffsetY;
  /* 0128 */ int labelAlignment;
};

class UIEditCtrl_Currency : public UIEditCtrl_Custom {
public:
  UIEditCtrl_Currency();
  void OnProcess() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 012C */ std::size_t formattedLength;
};

#endif
