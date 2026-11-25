#include "UI/Buttons/ButtonState.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Buttons/UIColorButton.h"

// OFFSET: 0x0084F190
// FUNCTION: fn_45630
UIColorButton::UIColorButton() : UIButton() {
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(6, 2, red, green, blue);
  this->activeRed = red;
  this->activeGreen = green;
  this->activeBlue = blue;

  g_skinInfo.PaletteChannels(18, 2, red, green, blue);
  this->normalRed = red;
  this->normalGreen = green;
  this->normalBlue = blue;
}

// OFFSET: 0x0085C9A0
// FUNCTION: fn_10804
void UIColorButton::Paint() {
  const int textWidth = this->TextWidth(this->label.c_str(), 0, 0, 12, 0, 0);
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(22, 2, red, green, blue);
  this->ClearSurface(static_cast<unsigned char>(red) | (static_cast<unsigned char>(green) << 8) | (static_cast<unsigned char>(blue) << 16));

  std::uint32_t color;
  switch (this->geometry.state) {
  case BUTTON_STATE_NORMAL:
    color = static_cast<unsigned char>(this->normalRed) | (static_cast<unsigned char>(this->normalGreen) << 8) | (static_cast<unsigned char>(this->normalBlue) << 16);
    break;
  case BUTTON_STATE_HOVER:
    color = static_cast<unsigned char>(this->activeRed) | (static_cast<unsigned char>(this->activeGreen) << 8) | (static_cast<unsigned char>(this->activeBlue) << 16);
    break;
  case BUTTON_STATE_PRESSED:
    color = static_cast<unsigned char>(this->activeRed) | (static_cast<unsigned char>(this->activeGreen) << 8) | (static_cast<unsigned char>(this->activeBlue) << 16);
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, color);
    this->DrawTextAt((this->geometry.width - textWidth) / 2 + 2, (this->geometry.height - 12) / 2 + 2, this->label.c_str(), 0, 0, 12,
      0xFFFFFF, 0, 0
    );
    this->DrawTextAt((this->geometry.width - textWidth) / 2 + 1, (this->geometry.height - 12) / 2 + 1, this->label.c_str(), 0, 0, 12, 0,
      0, 0
    );
    return;
  default:
    return;
  }

  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, color);
  this->DrawTextAt((this->geometry.width - textWidth) / 2 + 1, (this->geometry.height - 12) / 2 + 1, this->label.c_str(), 0, 0, 12, 0xFFFFFF,
    0, 0
  );
  this->DrawTextAt((this->geometry.width - textWidth) / 2, (this->geometry.height - 12) / 2, this->label.c_str(), 0, 0, 12, 0, 0, 0);
}
