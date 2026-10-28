#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXWND_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXWND_H

#include "UI/Window/UIFrameWnd.h"

class UIListBox;

class UIComboBoxWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIListBox* m_list;

  void Resize(int width, int height) override;
  void Paint() override;
};

class UIComboBoxIDWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIListBox* m_list;

  void Resize(int width, int height) override;
  void Paint() override;
};

#endif
