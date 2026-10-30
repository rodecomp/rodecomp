#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXWND_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXWND_H

#include "UI/Window/UIFrameWnd.h"

class UIListBox;

class UIComboBoxWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIListBox* m_list;
  /* 00B8 */ UIWindow* m_owner;

  void Resize(int width, int height) override;
  void Paint() override;

  void SetOwner(UIWindow* owner);
};

class UIComboBoxIDWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIListBox* m_list;

  void Resize(int width, int height) override;
  void Paint() override;
};

#endif
