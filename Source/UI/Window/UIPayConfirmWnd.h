#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPAYCONFIRMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPAYCONFIRMWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct BitmapImage;

class UIPayConfirmWnd : public UIFrameWnd {
public:
  void Paint() override;

  /* 00B4 */ unsigned char unknown_00B4[0x4C];
  /* 0100 */ std::string m_title;
  /* 0118 */ unsigned char unknown_0118[0x8];
  /* 0120 */ const BitmapImage* m_image;

protected:
  UIButtonContext* m_controls;
};

#endif
