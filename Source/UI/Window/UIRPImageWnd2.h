#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIRPIMAGEWND2_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIRPIMAGEWND2_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIRPImageWnd2 : public UIFrameWnd {
public:
  void Paint() override;

protected:
  /* 00B4 */ std::string m_imageName;
  /* 00CC */ int m_imageCount;
  /* 00D0 */ unsigned char m_drawImage;
};

#endif
