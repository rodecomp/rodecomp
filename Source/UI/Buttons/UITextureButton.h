#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITEXTUREBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITEXTUREBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <cstddef>
#include <string>

class UITextureButton : public UIButton {
public:
  void VisitChildren() override;

  /* 00BC */ std::string normalTexture;
  /* 00D4 */ std::string hoverTexture;
  /* 00EC */ std::string pressedTexture;
  /* 0104 */ std::string disabledTexture;
  /* 011C */ unsigned char unknown_011C;
  /* 011D */ unsigned char unknown_011D;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UITextureButton, normalTexture) == 0xBC);
static_assert(offsetof(UITextureButton, disabledTexture) == 0x104);
static_assert(offsetof(UITextureButton, unknown_011C) == 0x11C);
#endif

#endif
