#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBMPTEXTBUTTON2_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBMPTEXTBUTTON2_H

#include "UI/Buttons/UIBitmapButton.h"

#include <cstdint>
#include <string>
#include <vector>

struct BmpTextButtonLabel {
  /* 0000 */ std::string text;
  /* 0018 */ int unknown_0018;
  /* 001C */ int x;
  /* 0020 */ int y;
  /* 0024 */ int height;
  /* 0028 */ unsigned char bold;
  /* 002C */ std::uint32_t color;
  /* 0030 */ unsigned char shadow;
};

class UIBmpTextButton2 : public UIBitmapButton {
public:
  void DrawLabels();

  /* 0120 */ std::vector<BmpTextButtonLabel> labels;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BmpTextButtonLabel) == 52);
#endif

#endif
