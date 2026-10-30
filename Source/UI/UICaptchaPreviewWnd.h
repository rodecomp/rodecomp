#ifndef RAGNAROK_SOURCE_UI_UICAPTCHAPREVIEWWND_H
#define RAGNAROK_SOURCE_UI_UICAPTCHAPREVIEWWND_H

#include "UI/Window/UIFrameWnd.h"

class UICaptchaImage;

class UICaptchaPreviewWnd : public UIFrameWnd {
public:
  void Paint() override;

  void AddImageData(const void* data, int size);
  void ClearImageData();
  void InitImageData(unsigned short size);

  /* 00B4 */ UICaptchaImage* m_image;
  /* 00B8 */ char m_captchaKey[4];
  /* 00BC */ unsigned char* m_imageData;
  /* 00C0 */ int m_imageSize;
  /* 00C4 */ int m_imageReceived;
};

#endif
