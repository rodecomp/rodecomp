#ifndef RAGNAROK_SOURCE_UI_UICAPTCHAPREVIEWWND_H
#define RAGNAROK_SOURCE_UI_UICAPTCHAPREVIEWWND_H

#include "UI/Window/UIFrameWnd.h"

class UICaptchaImage;
struct UIButtonContext;

class UICaptchaPreviewWnd : public UIFrameWnd {
public:
  UICaptchaPreviewWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  void AddImageData(const void* data, int size);
  void ClearImageData();
  void InitImageData(unsigned short size);

  /* 00B4 */ UICaptchaImage* m_image;
  /* 00B8 */ char m_captchaKey[4];
  /* 00BC */ unsigned char* m_imageData;
  /* 00C0 */ int m_imageSize;
  /* 00C4 */ int m_imageReceived;
  UIButtonContext* m_controls;
};

#endif
