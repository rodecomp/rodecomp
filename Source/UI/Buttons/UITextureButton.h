#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITEXTUREBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITEXTUREBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <cstddef>
#include <cstdint>
#include <string>

class UITextureButton : public UIButton {
public:
  UITextureButton();
  void Paint() override;
  void OnLBtnUp(int x, int y) override;
  void VisitChildren() override;
  bool SetTexture(const char* name, int state);

  /* 00B4 */ std::uint32_t textureWidth;
  /* 00B8 */ std::uint32_t textureHeight;

  /* 00BC */ std::string normalTexture;
  /* 00D4 */ std::string hoverTexture;
  /* 00EC */ std::string pressedTexture;
  /* 0104 */ std::string disabledTexture;
  /* 011C */ unsigned char toggleEnabled;
  /* 011D */ unsigned char toggled;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UITextureButton, normalTexture) == 0xBC);
static_assert(offsetof(UITextureButton, disabledTexture) == 0x104);
static_assert(offsetof(UITextureButton, toggleEnabled) == 0x11C);
#endif

#endif
