#ifndef RAGNAROK_SOURCE_UI_LISTS_UIMEMBERLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIMEMBERLISTBOX_H

#include "UI/Lists/UIChatHisBox.h"

struct UIButtonContext;

class UIChatHisBox2 : public UIChatHisBox {
public:
  void Slot15(int width, int height) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Slot33(int x, int y) override;
};

class UIChatHisBox3 : public UIChatHisBox2 {
public:
  void Slot15(int width, int height) override;
  void Resize(int width, int height) override;
};

class UIMemberListBox : public UIChatHisBox2 {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

  /* 0124 */ int backgroundRed;
  /* 0128 */ int backgroundGreen;
  /* 012C */ int backgroundBlue;

protected:
  UIButtonContext* m_controls;
};

#endif
