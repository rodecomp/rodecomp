#include "UI/Gauges/UIBarGraph.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <string>
#include <windows.h>

template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);

// OFFSET: 0x0086C190
// FUNCTION: fn_11022
UIBarGraph2::UIBarGraph2() : UIBarGraph() {
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
UIBarGraph3::UIBarGraph3() : UIWindow(0), m_current(nullptr), m_maximum(nullptr) {
}

// OFFSET: 0x0086C310
// FUNCTION: fn_11024
UIBarGraphHomun::UIBarGraphHomun() : UIWindow(0) {
}

// OFFSET: 0x0086C390
// FUNCTION: fn_11025
UIBarGraphPlayer::UIBarGraphPlayer() : UIWindow(0) {
}

// OFFSET: 0x0086FFA0
// FUNCTION: fn_11097
__declspec(noinline) UIBarGraph3::~UIBarGraph3() = default;

// OFFSET: 0x008700F0
// FUNCTION: fn_11098
UIBarGraphPlayer::~UIBarGraphPlayer() = default;

// OFFSET: 0x00887440
// FUNCTION: fn_11337
void UIBarGraph2::Paint() {
  if (this->m_vertical != 0) {
    this->ClearSurface(RGB(this->m_backgroundRed, this->m_backgroundGreen, this->m_backgroundBlue));
    if (this->m_value != 0.0f) {
      const int fillHeight = static_cast<int>(this->geometry.height * this->m_value / 100.0f);
      this->FillRegion(0, this->geometry.height - fillHeight, this->geometry.width, fillHeight,
        RGB(this->m_fillRed, this->m_fillGreen, this->m_fillBlue));
    }
    return;
  }

  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, 0x00FFFF00);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(this->m_backgroundRed, this->m_backgroundGreen, this->m_backgroundBlue));
  if (0.0f != this->m_value) {
    this->FillRegion(1, 1, static_cast<int>((this->geometry.width - 2) * this->m_value / 100.0f), this->geometry.height - 2,
      RGB(this->m_fillRed, this->m_fillGreen, this->m_fillBlue));
  }

  char text[12];
  sprintf(text, "%3ld%%", static_cast<long>(this->m_value));
  this->DrawTextAt((this->geometry.width >> 1) - 20, (this->geometry.height >> 1) - 7,
    text, 0, 1, 16, 0xFFFF, 0, 0
  );
}

// OFFSET: 0x00887600
// FUNCTION: fn_11338
void UIBarGraph4::Paint() {
  if (!this->m_current || !this->m_maximum || *this->m_maximum < *this->m_current) {
    return;
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(26, 2, red, green, blue);
  this->ClearSurface(RGB(red, green, green));
  g_skinInfo.PaletteChannels(30, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  if (*this->m_maximum == 0) {
    return;
  }

  const float ratio = static_cast<float>(*this->m_current) / static_cast<float>(*this->m_maximum);
  if (0.0f == ratio) {
    return;
  }

  g_skinInfo.PaletteChannels(2, 6, red, green, blue);
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
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg.bmp"))), false);
  if (this->m_value == 0.0f) {
    return;
  }

  std::string color;
  if (this->m_value < 25.0f) {
    color = "red";
  } else {
    color = "blue";
  }
  const int fill = (this->geometry.width - 8) * static_cast<int>(this->m_value) / 100;

  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
  this->DrawBitmap(0, 0, left, true);

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
  for (int x = 0; x < fill; ++x) {
    this->DrawBitmap(x + 4, 0, middle, true);
  }

  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
  this->DrawBitmap(fill + 4, 0, right, true);
}

// OFFSET: 0x00887CB0
// FUNCTION: fn_11340
void UIBarGraphHomun::Paint() {
  if (!this->m_current || !this->m_maximum) {
    return;
  }

  const std::int64_t current = this->m_current();
  const std::int64_t maximum = this->m_maximum();
  if (maximum < current) {
    return;
  }

  this->ClearSurface(0xFFFF00FF);
  auto* backLeft = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_left.bmp")));
  auto* backMiddle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_mid.bmp")));
  auto* backRight = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg_right.bmp")));
  if (backLeft && backMiddle && backRight) {
    this->DrawBitmap(0, 0, backLeft, false);
    for (std::int64_t x = 0; x < this->geometry.width - backLeft->image.width - backRight->image.width; ++x) {
      this->DrawBitmap(static_cast<int>(x) + backLeft->image.width, 0, backMiddle, false);
    }
    this->DrawBitmap(this->geometry.width - backRight->image.width, 0, backRight, false);
  }

  if (0 == maximum) {
    return;
  }

  const double ratio = static_cast<double>(current) / static_cast<double>(maximum);
  if (0.0 == ratio) {
    return;
  }

  std::string color = "blue";
  if (ratio < 0.25) {
    color = "red";
  }
  const int fill = static_cast<int>(static_cast<double>(this->geometry.width - 8) * ratio);

  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
  this->DrawBitmap(0, 0, left, true);

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
  for (int x = 0; x < fill; ++x) {
    this->DrawBitmap(x + 4, 0, middle, true);
  }

  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
  this->DrawBitmap(fill + 4, 0, right, true);

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
  if (maximum < current) {
    return;
  }

  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg1.bmp")));
  this->DrawBitmap(0, 0, background, false);
  if (0 == maximum) {
    return;
  }

  const double ratio = static_cast<double>(current) / static_cast<double>(maximum);
  std::string color = "blue";
  if (ratio < 0.25) {
    color = "red";
  }
  const int fill = static_cast<int>(static_cast<double>(this->geometry.width - 8) * ratio);

  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
  this->DrawBitmap(0, 0, left, true);

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
  for (int x = 0; x < fill; ++x) {
    this->DrawBitmap(x + 4, 0, middle, true);
  }

  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
  this->DrawBitmap(fill + 4, 0, right, true);

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
  if (maximum < current) {
    return;
  }

  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\gze_bg1.bmp")));
  this->DrawBitmap(0, 0, background, false);
  if (maximum > 0) {
    const double ratio = static_cast<double>(current) / static_cast<double>(maximum);
    if (ratio > 0.0) {
      std::string color = "blue";
      if (1.0 == ratio) {
        color = "red";
      }

      const int fill = static_cast<int>(static_cast<double>(this->geometry.width - 8) * ratio);

      auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_left.bmp").c_str())));
      this->DrawBitmap(0, 0, left, true);

      auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_mid.bmp").c_str())));
      for (int x = 0; x < fill; ++x) {
        this->DrawBitmap(x + 4, 0, middle, true);
      }

      auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\basic_interface\\gze" + color + "_right.bmp").c_str())));
      this->DrawBitmap(fill + 4, 0, right, true);
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
