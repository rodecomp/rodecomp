#include "UI/Edit/UICandidateWnd.h"

#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstring>
#include <windows.h>

// OFFSET: 0x0096D6F0
// FUNCTION: fn_13200
void UICandidateWnd::Slot15(int, int) {}

// OFFSET: 0x0096D700
// FUNCTION: fn_13201
void UICompositionWnd::Slot15(int, int) {}

// OFFSET: 0x0096D710
// FUNCTION: fn_13202
void UIKeyStrokeWnd::Slot15(int, int) {}

// OFFSET: 0x0096D720
// FUNCTION: fn_13203
void UICandidateWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->DrawBorder(0, 0, 0, 0);
  char text[65];
  switch (g_language) {
  case 0: {
    std::memset(text, 0, sizeof(text));
    int x = 14;
    for (int i = 0; i < this->m_count;) {
      ++i;
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d %s", i, this->m_candidates[i - 1].c_str());
      this->DrawTextAt(controls.fonts, controls.locale, x, 12, text, 0, 0, 12, 0, 0, 0);
      x += 8 + this->TextWidth(controls.fonts, controls.locale, text, static_cast<unsigned>(std::strlen(text)), 0, 12, 0, 0);
    }
    break;
  }
  case 2: {
    std::memset(text, 0, sizeof(text));
    int y = 12;
    for (int i = 0; i < this->m_count;) {
      int number = i + 1;
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d %s", number, this->m_candidates[i].c_str());
      if (this->m_selection == i) {
        unsigned red, green, blue;
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
        this->FillRegion(8, y - 2, this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0) + 4, 16, RGB(red, green, blue));
      }
      this->DrawTextAt(controls.fonts, controls.locale, 10, y, text, 0, 0, 12, 0, 0, 0);
      y += 20;
      i = number;
    }
    std::sprintf(text, "%d / %d", this->m_page + 1, this->m_lastPage + 1);
    this->DrawTextAt(controls.fonts, controls.locale, 110 - this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0), 192, text, 0, 0, 12, 0, 0, 0);
    return;
  }
  case 3:
  case 4: {
    std::memset(text, 0, sizeof(text));
    int x = 14;
    for (int i = 0; i < this->m_count;) {
      int number = i + 1;
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d %s", number, this->m_candidates[i].c_str());
      if (i == this->m_selection) {
        this->DrawTextAt(controls.fonts, controls.locale, x, 12, text, static_cast<int>(std::strlen(text)), 0, 12, 0xFF, 0, 0);
      } else {
        this->DrawTextAt(controls.fonts, controls.locale, x, 12, text, 0, 0, 12, 0, 0, 0);
      }
      x += this->TextWidth(controls.fonts, controls.locale, text, static_cast<unsigned>(std::strlen(text)), 0, 12, 0, 0) + 8;
      i = number;
    }
    break;
  }
  default:
    return;
  }
  FillTriangle(*this, 8, 18, 14, 10, 14, 26, this->m_page > 0 ? 0xFF9F70 : 0xD2D2D2);
  int width = this->geometry.width;
  FillTriangle(*this, width - 8, 18, width - 14, 10, width - 14, 26, this->m_page < this->m_lastPage ? 0xFF9F70 : 0xD2D2D2);
}

// OFFSET: 0x0096DB20
// FUNCTION: fn_13204
void UICompositionWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 10, 12, this->m_text.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0096DB60
// FUNCTION: fn_13205
void UIKeyStrokeWnd::Paint() {
  if (g_language != 4) {
    return;
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 22, 2, red, green, blue);
  const std::uint32_t color = RGB(red, green, blue);
  this->FillRegion(0, 0, this->geometry.width, 1, color);
  this->FillRegion(0, 0, 1, this->geometry.height, color);
  this->FillRegion(0, this->geometry.height - 1, this->geometry.width, 1, color);
  this->FillRegion(this->geometry.width - 1, 0, 1, this->geometry.height, color);

  using IndicatorText = const char* (__thiscall*)(InputCharset*);
  for (int index = 0; index < 4; ++index) {
    auto table = *reinterpret_cast<IndicatorText**>(g_serverLanguage);
    const char* source = table[1](g_serverLanguage) + index * 2;
    char text[3] = {source[0], source[1], 0};
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 2 + index * 14, 2, text, 2, 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x0096DD40
// FUNCTION: fn_13206
int UICandidateWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 65) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  UIButtonContext& controls = *this->m_controls;
  const std::string* candidates = reinterpret_cast<const std::string*>(argument1);
  for (int i = 0; i < 9; ++i) {
    this->m_candidates[i] = candidates[i];
  }
  const int* info = reinterpret_cast<const int*>(argument2);
  this->m_selection = info[0];
  this->m_page = info[1];
  this->m_lastPage = info[2];
  this->m_count = info[3];
  char text[65];
  std::memset(text, 0, sizeof(text));
  int width = 0;
  for (int i = 0; i < this->m_count;) {
    ++i;
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%d %s", i, this->m_candidates[i - 1].c_str());
    if (g_language == 2) {
      int textWidth = this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0) + 8;
      if (textWidth < width) {
        textWidth = width;
      }
      width = textWidth;
    } else {
      width += 8 + this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0);
    }
  }
  int height;
  if (g_language == 2) {
    height = this->m_count * 20 + 14;
  } else {
    height = this->geometry.height;
  }
  this->Resize(width + 22, height);
  this->Invalidate();
  return 0;
}

// OFFSET: 0x0096DED0
// FUNCTION: fn_13207
int UICompositionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 66) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  const char* text = reinterpret_cast<const char*>(argument1);
  if (text != nullptr) {
    this->m_text = text;
    int padded = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, text, 0, 0, 12, 0, 0) + 20;
    int width = padded / 28 * 28;
    width += padded != width ? 28 : 0;
    if (width < 34) {
      width = 34;
    }
    if (width != this->geometry.width) {
      this->Resize(width, 34);
    }
    this->Invalidate();
  }
  return 0;
}

// OFFSET: 0x0096DFA0
// FUNCTION: fn_13208
int UIKeyStrokeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
