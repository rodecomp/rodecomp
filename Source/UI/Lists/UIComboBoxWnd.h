#ifndef RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXWND_H
#define RAGNAROK_SOURCE_UI_LISTS_UICOMBOBOXWND_H

#include "UI/Window/UIFrameWnd.h"

class UIListBox;
class UIListBoxID;

class UIComboBoxWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIListBox* m_list;
  /* 00B8 */ UIWindow* m_owner;

  void Resize(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void SetOwner(UIWindow* owner);
};

class UIComboBoxIDWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIListBoxID* m_list;
  /* 00B8 */ UIWindow* m_owner;

  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif
