#ifndef RAGNAROK_SOURCE_UI_ITEMS_UICARDITEMILLUSTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UICARDITEMILLUSTWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;

class UICardItemIllustWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;

  /* 00B4 */ std::string m_bitmapPath;
  /* 00CC */ std::string m_name;
  /* 00E4 */ unsigned char unknown_00E4;

protected:
  UIButtonContext* m_controls;
};

#endif
