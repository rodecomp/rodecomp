#ifndef RAGNAROK_SOURCE_UI_UIMACRODETECTORWND_H
#define RAGNAROK_SOURCE_UI_UIMACRODETECTORWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

class UIMacroDetectorWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;

  void sub_8D2CE0(bool closeButton, const char* title, int width);
  void sub_A84360(int x, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold, unsigned char italic);
  void sub_A86440(int right, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold);

  /* 00BC */ unsigned char unknown_00BC[0x14];
  /* 00D0 */ unsigned char unknown_00D0;
  /* 00D1 */ unsigned char unknown_00D1[3];
  /* 00D4 */ std::uint32_t totalDuration;
  /* 00D8 */ std::uint32_t startTick;
  /* 00DC */ std::uint32_t messageDuration;
  /* 00E0 */ unsigned char unknown_00E0[0x18];
  /* 00F8 */ std::string message;
  /* 0110 */ std::uint32_t unknown_0110;
  /* 0114 */ std::uint32_t messageTick;
};

#endif
