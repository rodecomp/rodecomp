#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITAGBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITAGBUTTON_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UITextButton.h"

#include <cstddef>
#include <string>
#include <vector>

class UITagButton : public UITextButton {
  friend class UITagListBox;

public:
  UITagButton(UIWindowContext& windows, UIButtonContext& buttons);

  /* 00F8 */ ITEM_INFO item;
  /* 01F0 */ std::vector<std::string> m_strings;

  void Slot31(int x, int y) override;

  void AddString(std::string text);
  std::string GetString(std::size_t index);
  int GetStringCount();
};

#endif
