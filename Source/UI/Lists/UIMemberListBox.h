#ifndef RAGNAROK_SOURCE_UI_LISTS_UIMEMBERLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIMEMBERLISTBOX_H

#include "UI/Lists/UIChatHisBox.h"

class UIChatHisBox2 : public UIChatHisBox {
public:
  UIChatHisBox2();
  bool IsTransmitMouseInput() override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void OnRBtnUp(int x, int y) override;

  /* 0124 */ int backgroundRed;
  /* 0128 */ int backgroundGreen;
  /* 012C */ int backgroundBlue;
};

class UIChatHisBox3 : public UIChatHisBox2 {
public:
  UIChatHisBox3();
  void Paint() override;
  void OnCreate(int width, int height) override;
  void Resize(int width, int height) override;
  virtual void AddText(const char* text, int color);

  /* 0130 */ int unknown_0130;
  /* 0134 */ int unknown_0134;
};

class UIMemberListBox : public UIChatHisBox2 {
public:
  UIMemberListBox();
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
};

#endif
