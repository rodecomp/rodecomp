#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIRPIMAGEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIRPIMAGEWND_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIRPImageWnd : public UIWindow {
public:
  UIRPImageWnd(UIWindowContext& context, std::string imageName, int imageCount);
  void Paint() override;

protected:
  /* 007C */ std::string m_imageName;
  /* 0094 */ int m_imageCount;
};

#endif
