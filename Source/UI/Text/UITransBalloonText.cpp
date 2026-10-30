#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/WindowBorder.h"

#include <cstring>

namespace {
  // INLINED
  int Family(unsigned mode) {
    switch (mode) {
    case 1:
      return 21;
    case 2:
      return 20;
    case 3:
      return 22;
    case 4:
      return 23;
    case 5:
      return 24;
    case 6:
      return 25;
    case 7:
      return 26;
    case 8:
      return 27;
    case 9:
      return 28;
    default:
      return 0;
    }
  }
}

// OFFSET: 0x00850810
// FUNCTION: fn_10680
UITransBalloonText::UITransBalloonText(UIWindowContext& windows, BalloonTextContext& text) : UIBalloonText(windows, text), m_borderColor(6052956) {
}

// OFFSET: 0x00853210
// OFFSET: 0x00851F40
// FUNCTION: fn_10700
UITransBalloonText::~UITransBalloonText() = default;

// OFFSET: 0x008533B0
// FUNCTION: fn_10738
void UITransBalloonText::AddText(const char* text) {
  this->m_lines.push_back(text);
  this->FitText();
}

// OFFSET: 0x008534F0
// FUNCTION: fn_10739
void UITransBalloonText::AddLine(const char* text, std::uint32_t color) {
  this->AddText(text);
  this->m_lineColors.push_back(color);
}

// OFFSET: 0x00868280
// FUNCTION: fn_10957
void UITransBalloonText::SetLine(const char* text, std::uint32_t color) {
  this->m_lines.clear();
  this->m_lineColors.clear();
  this->AddText(text);
  this->m_lineColors.push_back(color);
}

// OFFSET: 0x00853EA0
// FUNCTION: fn_10744
void UITransBalloonText::ChangeText(const char* text, int line) {
  if (this->m_lines.empty()) {
    return;
  }

  if (line == -1) {
    line = static_cast<int>(this->m_lines.size()) - 1;
  }

  std::string& target = this->m_lines[line];
  if (!target.empty()) {
    target = text;
    this->Invalidate();
  }
}

// OFFSET: 0x008632D0
// FUNCTION: fn_10907
void UITransBalloonText::Resize(int width, int height) {
  this->UIWindow::Resize(width, height);
  this->m_rectangle.width = this->geometry.width;
  this->m_rectangle.height = this->geometry.height;
}

// OFFSET: 0x0085AA10
// FUNCTION: fn_10781
void UITransBalloonText::Move(int x, int y) {
  this->UIWindow::Move(x, y);
  this->m_rectangle.x = this->geometry.x;
  this->m_rectangle.y = this->geometry.y;
}

// OFFSET: 0x00858F00
// FUNCTION: fn_10770
bool UITransBalloonText::Slot07(void* output) {
  std::memcpy(output, &this->m_rectangle, sizeof(this->m_rectangle));
  return true;
}

// OFFSET: 0x0085AFB0
// FUNCTION: fn_10790
void UITransBalloonText::Slot15(int, int) {
  this->m_rectangle = {this->geometry.x, this->geometry.y, this->geometry.width, this->geometry.height, 0, 1711276032u, 0};
  this->m_fontMode = 0;
}

// INLINED
void UITransBalloonText::SetColors(std::uint32_t text, std::uint32_t outline) {
  this->m_textColor = text;
  this->m_outlineColor = outline;
}

// OFFSET: 0x008697E0
// FUNCTION: fn_10970
void UITransBalloonText::SetBackground(std::uint32_t color) {
  this->m_rectangle.color = color;
}

// OFFSET: 0x0085E080
// FUNCTION: fn_10823
void UITransBalloonText::Paint() {
  if (this->m_lines.empty()) {
    return;
  }

  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
  }

  DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, this->m_borderColor);
  const int family = Family(this->m_fontMode);
  const int height = this->m_fontMode == 0 ? this->m_fontHeight : 12;
  int y = 4;

  for (std::size_t index = 0; index < this->m_lines.size(); ++index) {
    const std::string& line = this->m_lines[index];
    std::uint32_t color = index < this->m_lineColors.size() && this->m_lineColors[index] != 0 ? this->m_lineColors[index] : this->m_textColor;

    if (this->m_richText != 0) {
      DrawColoredText(
        *this, this->m_textContext->codePage, 6, y, line.c_str(), 0, color, family, height, true
      );
    } else {
      TextContext context(*this->geometry.surface);
      context.SetFont(family, height, 0, 0);
      context.SetColor(this->m_outlineColor);
      context.Draw(7, (y + 1), line.c_str(), static_cast<int>(line.size()));
      context.SetColor(color);
      context.Draw(6, y, line.c_str(), static_cast<int>(line.size()));
    }

    int lineHeight = 0;

    if (!line.empty()) {
      TextContext context(*this->geometry.surface);
      context.SetFont(0, this->m_fontHeight, 0, 0);
      lineHeight = context.Measure(line.c_str(), static_cast<int>(line.size())).cy;
    }

    y = (y + (lineHeight + 4));
  }
}

// OFFSET: 0x0085CBA0
// FUNCTION: fn_10805
void UIColorTransBalloonText::Paint() {
  this->ClearSurface(0xFFFF00FF);
  std::uint32_t color = 0xFFFFFF;
  DrawColoredText(*this, this->m_textContext->codePage, 6, 4, this->m_lines[0].c_str(), 0, color, 0, 12, false);
}

// OFFSET: 0x0085B010
// FUNCTION: fn_10792
void UIAchBalloonText::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->m_lines.empty()) {
    return;
  }

  const int count = static_cast<int>(this->m_lines.size());
  const std::string& first = this->m_lines.front();
  const int lineHeight =
    this->TextHeight(
      this->m_textContext->fonts, this->m_textContext->locale, first.c_str(), static_cast<unsigned>(first.size()), this->m_fontFamily, this->m_fontHeight, 1, 0
    ) +
    4;
  const int top = (this->geometry.height - lineHeight * count + 4) / 2;

  for (int index = 0; index < count; ++index) {
    const std::string& line = this->m_lines[index];
    const int width = this->TextWidth(
      this->m_textContext->fonts, this->m_textContext->locale, line.c_str(), static_cast<unsigned>(line.size()), this->m_fontFamily, this->m_fontHeight, 1, 0
    );
    int y = top;
    if (index != 0) {
      y = top - 4 + lineHeight * index;
    }
    this->DrawShadowedText(
      (this->geometry.width - width) / 2, y, this->m_lines[index].c_str(), 0, this->m_textColor, this->m_outlineColor, this->m_fontFamily, this->m_fontHeight, 1
    );
  }
}
