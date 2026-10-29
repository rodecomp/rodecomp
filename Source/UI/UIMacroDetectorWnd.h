#ifndef RAGNAROK_SOURCE_UI_UIMACRODETECTORWND_H
#define RAGNAROK_SOURCE_UI_UIMACRODETECTORWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

class UIMacroDetectorWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;

  /* 00B4 */ unsigned char unknown_00B4[0x28];
  /* 00DC */ std::uint32_t messageDuration;
  /* 00E0 */ unsigned char unknown_00E0[0x18];
  /* 00F8 */ std::string message;
  /* 0110 */ std::uint32_t unknown_0110;
  /* 0114 */ std::uint32_t messageTick;
};

#endif
