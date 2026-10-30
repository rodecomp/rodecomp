#ifndef RAGNAROK_SOURCE_UI_UIMACRODETECTORWND_H
#define RAGNAROK_SOURCE_UI_UIMACRODETECTORWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

class UIEditCtrl;
class UICaptchaImage;

class UIMacroDetectorWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  void sub_8D2CE0(bool closeButton, const char* title, int width);
  void sub_A84360(int x, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold, unsigned char italic);
  void sub_A86440(int right, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold);
  void AddImageData(const void* data, int size);
  void ClearImageData();
  void SetCountdown(unsigned char count, std::uint32_t duration);
  void InitImageData(unsigned short size);

  /* 00BC */ unsigned char unknown_00BC[4];
  /* 00C0 */ char m_captchaKey[4];
  /* 00C4 */ unsigned char* m_imageData;
  /* 00C8 */ int m_imageSize;
  /* 00CC */ int m_imageReceived;
  /* 00D0 */ unsigned char unknown_00D0;
  /* 00D1 */ unsigned char unknown_00D1[3];
  /* 00D4 */ std::uint32_t totalDuration;
  /* 00D8 */ std::uint32_t startTick;
  /* 00DC */ std::uint32_t messageDuration;
  /* 00E0 */ std::string m_lastAnswer;
  /* 00F8 */ std::string message;
  /* 0110 */ std::uint32_t unknown_0110;
  /* 0114 */ std::uint32_t messageTick;

  /* 00B4 */ UICaptchaImage* m_image;
  /* 00B8 */ UIEditCtrl* m_editCtrl;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMacroDetectorWnd, m_captchaKey) == 0xC0);
static_assert(offsetof(UIMacroDetectorWnd, m_imageData) == 0xC4);
static_assert(offsetof(UIMacroDetectorWnd, m_lastAnswer) == 0xE0);
static_assert(offsetof(UIMacroDetectorWnd, message) == 0xF8);
static_assert(offsetof(UIMacroDetectorWnd, messageTick) == 0x114);
#endif

#endif
