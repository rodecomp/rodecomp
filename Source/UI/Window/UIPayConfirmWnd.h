#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPAYCONFIRMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPAYCONFIRMWND_H

#include "UI/Window/UIMessageBox.h"

#include <string>

struct BitmapImage;

class UIPayConfirmWnd : public UIMessageBox {
public:
  // INLINED
  UIPayConfirmWnd(UIWindowContext& windows, UIMessageBoxContext& dialog) : UIMessageBox(windows, dialog) {
  }

  void Configure(const std::string& text, int itemId);
  void Paint() override;

  // INLINED
  std::string& Title() {
    return this->caption;
  }

  __declspec(property(get = Title)) std::string& m_title;

  /* 0120 */ const BitmapImage* m_image;

protected:
  void* unknown_controls;
};

#endif
