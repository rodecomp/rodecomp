#ifndef RAGNAROK_SOURCE_UI_ITEMS_UICARDITEMILLUSTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UICARDITEMILLUSTWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UICardItemIllustWnd : public UIFrameWnd {
public:
  UICardItemIllustWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ std::string m_bitmapPath;
  /* 00CC */ std::string m_name;
  /* 00E4 */ unsigned char identified;
};

#endif
