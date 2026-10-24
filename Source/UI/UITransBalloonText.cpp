#include "Render/DeviceContext.h"
#include "Render/TextContext.h"
#include "UI/ColoredText.h"
#include "UI/UITransBalloonText.h"
#include "UI/WindowBorder.h"

#include <bit>
#include <cstring>

namespace {

  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }

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
UITransBalloonText::UITransBalloonText(UIWindowContext& windows, BalloonTextContext& text)
    : UIBalloonText(windows, text), m_borderColor(6052956) {
}

// OFFSET: 0x00853210
// FUNCTION: fn_10735
UITransBalloonText::~UITransBalloonText() = default;

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
  this->m_rectangle = {
    this->geometry.x, this->geometry.y, this->geometry.width, this->geometry.height, 0, 1711276032u, 0
  };
  this->m_fontMode = 0;
}

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
    std::uint32_t color = index < this->m_lineColors.size() && this->m_lineColors[index] != 0
                            ? this->m_lineColors[index]
                            : this->m_textColor;

    if (this->m_richText != 0) {
      DrawColoredText(
        *this, this->m_textContext->fonts, this->m_textContext->locale, this->m_textContext->codePage, 6, y,
        line.c_str(), 0, color, family, height, true
      );
    } else {
      TextContext context(*this->geometry.surface, this->m_textContext->fonts, this->m_textContext->locale);
      context.SetFont(family, height, 0, 0);
      context.SetColor(this->m_outlineColor);
      context.Draw(7, Add(y, 1), line.c_str(), static_cast<int>(line.size()));
      context.SetColor(color);
      context.Draw(6, y, line.c_str(), static_cast<int>(line.size()));
    }

    int lineHeight = 0;

    if (!line.empty()) {
      TextContext context(*this->geometry.surface, this->m_textContext->fonts, this->m_textContext->locale);
      context.SetFont(0, this->m_fontHeight, 0, 0);
      lineHeight = context.Measure(line.c_str(), static_cast<int>(line.size())).cy;
    }

    y = Add(y, Add(lineHeight, 4));
  }
}
