#ifndef RAGNAROK_SOURCE_UI_LISTS_UIMEMBERLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIMEMBERLISTBOX_H

#include "UI/Lists/UIListBox.h"

struct UIButtonContext;

class UIChatHisBox : public UIListBox {
};

class UIChatHisBox2 : public UIChatHisBox {
public:
  void Slot33(int x, int y) override;
};

class UIMemberListBox : public UIChatHisBox2 {
public:
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

protected:
  UIButtonContext* m_controls;
};

#endif
