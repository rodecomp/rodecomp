#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIColorButton.h"

// OFFSET: 0x0085C9A0
// FUNCTION: fn_10804
void UIColorButton::Paint() {
  UIButtonContext& controls = *this->m_buttonContext;
  const int textWidth = this->TextWidth(controls.fonts, controls.locale, this->label.c_str(), 0, 0, 12, 0, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 22, 2, red, green, blue);
  this->ClearSurface(static_cast<unsigned char>(red) | (static_cast<unsigned char>(green) << 8) | (static_cast<unsigned char>(blue) << 16));

  std::uint32_t color;
  switch (this->geometry.unknown_0030) {
  case 0:
    color = static_cast<unsigned char>(this->normalRed) | (static_cast<unsigned char>(this->normalGreen) << 8) | (static_cast<unsigned char>(this->normalBlue) << 16);
    break;
  case 1:
    color = static_cast<unsigned char>(this->activeRed) | (static_cast<unsigned char>(this->activeGreen) << 8) | (static_cast<unsigned char>(this->activeBlue) << 16);
    break;
  case 2:
    color = static_cast<unsigned char>(this->activeRed) | (static_cast<unsigned char>(this->activeGreen) << 8) | (static_cast<unsigned char>(this->activeBlue) << 16);
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, color);
    this->DrawTextAt(
      controls.fonts, controls.locale, (this->geometry.width - textWidth) / 2 + 2, (this->geometry.height - 12) / 2 + 2, this->label.c_str(), 0, 0, 12,
      0xFFFFFF, 0, 0
    );
    this->DrawTextAt(
      controls.fonts, controls.locale, (this->geometry.width - textWidth) / 2 + 1, (this->geometry.height - 12) / 2 + 1, this->label.c_str(), 0, 0, 12, 0,
      0, 0
    );
    return;
  default:
    return;
  }

  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, color);
  this->DrawTextAt(
    controls.fonts, controls.locale, (this->geometry.width - textWidth) / 2 + 1, (this->geometry.height - 12) / 2 + 1, this->label.c_str(), 0, 0, 12, 0xFFFFFF,
    0, 0
  );
  this->DrawTextAt(controls.fonts, controls.locale, (this->geometry.width - textWidth) / 2, (this->geometry.height - 12) / 2, this->label.c_str(), 0, 0, 12, 0, 0, 0);
}
