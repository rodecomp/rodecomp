#include "UI/UIBarGraph.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <string>
#include <windows.h>

class UIBarGraph2 : public UIBarGraph {
public:
  explicit UIBarGraph2(UIWindowContext& context);

  void Paint() override;

  /* 0080 */ int m_vertical;
  /* 0084 */ int m_backgroundRed;
  /* 0088 */ int m_backgroundGreen;
  /* 008C */ int m_backgroundBlue;
  /* 0090 */ int m_fillRed;
  /* 0094 */ int m_fillGreen;
  /* 0098 */ int m_fillBlue;
};

// OFFSET: 0x0086C190
// FUNCTION: fn_11022
UIBarGraph2::UIBarGraph2(UIWindowContext& context) : UIBarGraph(context) {
  this->m_vertical = 0;
  this->m_backgroundRed = 0x8E;
  this->m_backgroundGreen = 0x8E;
  this->m_backgroundBlue = 0x8E;
  this->m_fillRed = 0x2A;
  this->m_fillGreen = 0x4B;
  this->m_fillBlue = 0x8F;
}

// OFFSET: 0x0086C240
// FUNCTION: fn_11023
UIBarGraph3::UIBarGraph3(UIWindowContext& context) : UIWindow(context, 0), m_current(nullptr), m_maximum(nullptr) {
}

// OFFSET: 0x0086C310
// FUNCTION: fn_11024
UIBarGraphHomun::UIBarGraphHomun(UIWindowContext& context) : UIWindow(context, 0) {
}

// OFFSET: 0x0086C390
// FUNCTION: fn_11025
UIBarGraphPlayer::UIBarGraphPlayer(UIWindowContext& context) : UIWindow(context, 0) {
}

// OFFSET: 0x0086FFA0
// FUNCTION: fn_11097
UIBarGraph3::~UIBarGraph3() = default;

// OFFSET: 0x008700F0
// FUNCTION: fn_11098
UIBarGraphPlayer::~UIBarGraphPlayer() = default;

// OFFSET: 0x00887440
// FUNCTION: fn_11337
void UIBarGraph2::Paint() {
  const std::uint32_t backgroundColor = RGB(this->m_backgroundRed, this->m_backgroundGreen, this->m_backgroundBlue);
  const std::uint32_t fillColor = RGB(this->m_fillRed, this->m_fillGreen, this->m_fillBlue);

  if (this->m_vertical == 0) {
    this->FillRegion(0, 0, this->geometry.width, this->geometry.height, 0x00FFFF00);
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, backgroundColor);
    if (this->m_value != 0.0f) {
      this->FillRegion(1, 1, static_cast<int>((this->geometry.width - 2) * this->m_value / 100.0f), this->geometry.height - 2, fillColor);
    }
    char text[16];
    sprintf_s(text, sizeof(text), "%3ld", static_cast<long>(this->m_value));
    this->DrawTextAt(
      this->m_context->fonts, this->m_context->locale, (this->geometry.width >> 1) - 20, (this->geometry.height >> 1) - 7,
      text, 0, 1, 16, 0xFFFF, 0, 0
    );
    return;
  }

  this->ClearSurface(backgroundColor);
  if (this->m_value != 0.0f) {
    const int fillHeight = static_cast<int>(this->geometry.height * this->m_value / 100.0f);
    this->FillRegion(0, this->geometry.height - fillHeight, this->geometry.width, fillHeight, fillColor);
  }
}

// OFFSET: 0x00887600
// FUNCTION: fn_11338
void UIBarGraph4::Paint() {
  if (!this->m_current || !this->m_maximum || *this->m_maximum < *this->m_current) {
    return;
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 26, 2, red, green, blue);
  this->ClearSurface(RGB(red, green, green));
  SkinPaletteChannels(ResourceMgr::Instance(), 30, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  if (*this->m_maximum == 0) {
    return;
  }

  const float ratio = static_cast<float>(*this->m_current) / static_cast<float>(*this->m_maximum);
  if (ratio == 0.0f) {
    return;
  }

  SkinPaletteChannels(ResourceMgr::Instance(), 2, 6, red, green, blue);
  if (!this->m_plainColor && ratio * 100.0f < 25.0f) {
    red = 255;
    green = 0;
    blue = 0;
  }
  this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * ratio), this->geometry.height - 2, RGB(red, green, blue));
}

// OFFSET: 0x00887790
// FUNCTION: fn_11339
void UIBarGraph::Paint() {
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg.bmp")))->image, false);
  if (this->m_value == 0.0f) {
    return;
  }

  const std::string color = this->m_value >= 25.0f ? "blue" : "red";
  const int fill = (this->geometry.width - 8) * static_cast<int>(this->m_value) / 100;

  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
  this->DrawBitmap(0, 0, &left->image, true);

  const auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
  for (int x = 0; x < fill; ++x) {
    this->DrawBitmap(x + 4, 0, &middle->image, true);
  }

  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
  this->DrawBitmap(fill + 4, 0, &right->image, true);
}

// OFFSET: 0x00887CB0
// FUNCTION: fn_11340
void UIBarGraphHomun::Paint() {
  if (!this->m_current || !this->m_maximum) {
    return;
  }

  const std::int64_t current = this->m_current();
  const std::int64_t maximum = this->m_maximum();
  if (current > maximum) {
    return;
  }

  this->ClearSurface(0xFFFF00FF);
  const auto* backLeft = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_left.bmp")));
  const auto* backMiddle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_mid.bmp")));
  const auto* backRight = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_right.bmp")));
  if (backLeft && backMiddle && backRight) {
    this->DrawBitmap(0, 0, &backLeft->image, false);
    for (std::int64_t x = 0; x < this->geometry.width - backLeft->image.width - backRight->image.width; ++x) {
      this->DrawBitmap(static_cast<int>(x) + backLeft->image.width, 0, &backMiddle->image, false);
    }
    this->DrawBitmap(this->geometry.width - backRight->image.width, 0, &backRight->image, false);
  }

  if (maximum == 0) {
    return;
  }

  const double ratio = static_cast<double>(current) / static_cast<double>(maximum);
  if (ratio == 0.0) {
    return;
  }

  const std::string color = ratio < 0.25 ? "red" : "blue";
  const int fill = static_cast<int>(static_cast<double>(this->geometry.width - 8) * ratio);

  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
  this->DrawBitmap(0, 0, &left->image, true);

  const auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
  for (int x = 0; x < fill; ++x) {
    this->DrawBitmap(x + 4, 0, &middle->image, true);
  }

  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
  this->DrawBitmap(fill + 4, 0, &right->image, true);

  this->DrawCenteredText(0, -2, FormatOwnedString("%3I64d  /  %3I64d", current, maximum).c_str(), this->geometry.width, 1, 13, 0, 0);
}

// OFFSET: 0x008882C0
// FUNCTION: fn_11341
void UIBarGraphPlayer::Paint() {
  if (!this->m_current || !this->m_maximum) {
    return;
  }

  const std::int64_t current = this->m_current();
  const std::int64_t maximum = this->m_maximum();
  if (current > maximum) {
    return;
  }

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg1.bmp")))->image, false);
  if (maximum == 0) {
    return;
  }

  const double ratio = static_cast<double>(current) / static_cast<double>(maximum);
  const std::string color = ratio < 0.25 ? "red" : "blue";
  const int fill = static_cast<int>(static_cast<double>(this->geometry.width - 8) * ratio);

  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
  this->DrawBitmap(0, 0, &left->image, true);

  const auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
  for (int x = 0; x < fill; ++x) {
    this->DrawBitmap(x + 4, 0, &middle->image, true);
  }

  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
  this->DrawBitmap(fill + 4, 0, &right->image, true);

  this->DrawCenteredText(0, -2, FormatOwnedString("%3I64d  /  %3I64d", current, maximum).c_str(), this->geometry.width, 1, 13, 0, 0);
}

// OFFSET: 0x008887A0
// FUNCTION: fn_11342
void UIBarGraphPlayerAP::Paint() {
  if (!this->m_current || !this->m_maximum) {
    return;
  }

  const std::int64_t current = this->m_current();
  const std::int64_t maximum = this->m_maximum();
  if (current > maximum) {
    return;
  }

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg1.bmp")))->image, false);
  if (maximum > 0) {
    const double ratio = static_cast<double>(current) / static_cast<double>(maximum);
    if (ratio > 0.0) {
      std::string color = "blue";
      if (ratio == 1.0) {
        color = "red";
      }

      const int fill = static_cast<int>(static_cast<double>(this->geometry.width - 8) * ratio);

      const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
      this->DrawBitmap(0, 0, &left->image, true);

      const auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
      for (int x = 0; x < fill; ++x) {
        this->DrawBitmap(x + 4, 0, &middle->image, true);
      }

      const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
      this->DrawBitmap(fill + 4, 0, &right->image, true);
    }
  }

  this->DrawCenteredText(0, -2, FormatOwnedString("%3I64d  /  %3I64d", current, maximum).c_str(), this->geometry.width, 1, 13, 0, 0);
}

// OFFSET: 0x0089CDB0
// FUNCTION: fn_11593
void UIBarGraphHomun::SetValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}

// OFFSET: 0x0089CE60
// FUNCTION: fn_11594
void UIBarGraphPlayer::SetValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}
