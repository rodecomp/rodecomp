#ifndef RAGNAROK_SOURCE_UI_CASHSHOP_UIPAYCONFIRMWND_H
#define RAGNAROK_SOURCE_UI_CASHSHOP_UIPAYCONFIRMWND_H

#include "UI/Dialogs/UIMessageBox.h"

#include <string>

class CBitmapRes;

class UIPayConfirmWnd : public UIMessageBox {
public:
  // INLINED
  UIPayConfirmWnd() : UIMessageBox() {
  }

  void Configure(const std::string& text, int itemId);
  void Paint() override;

  // INLINED
  std::string& Title() {
    return this->caption;
  }

  __declspec(property(get = Title)) std::string& m_title;

  /* 0120 */ const CBitmapRes* m_image;
};

#endif
