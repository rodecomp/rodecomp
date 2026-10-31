#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPAYCONFIRMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPAYCONFIRMWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct BitmapImage;
class UITextViewer;

class UIPayConfirmWnd : public UIFrameWnd {
public:
  void Configure(const std::string& text, int itemId);
  void Paint() override;

  /* 00B4 */ UITextViewer* viewer;
  /* 00B8 */ unsigned char unknown_00B8[0x48];
  /* 0100 */ std::string m_title;
  /* 0118 */ unsigned char unknown_0118[0x8];
  /* 0120 */ const BitmapImage* m_image;

protected:
  UIButtonContext* m_controls;
};

#endif
