#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Text/UIBalloonText.h"
#include "UI/Text/UINameBalloonText.h"

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

  // INLINED
  SIZE Measure(UIBalloonText& window, BalloonTextContext& owner, const std::string& text, int family, int height) {
    if (text.empty()) {
      return {0, 0};
    }

    TextContext context(*window.geometry.surface);
    context.SetFont(family, height, 0, 0);
    return context.Measure(text.c_str(), static_cast<int>(text.size()));
  }
}

// OFFSET: 0x0084E970
// FUNCTION: fn_10648
UIBalloonText::UIBalloonText(UIWindowContext& windows, BalloonTextContext& text)
    : UIWindow(windows, 0), unknown_007C(0), m_fontHeight(12), m_textColor(0xFFFFFF), m_outlineColor(0), m_richText(0), m_textContext(&text) {
  this->geometry.unknown_0034 = 0;
}

// OFFSET: 0x008520F0
UIBalloonText::~UIBalloonText() = default;

// OFFSET: 0x00851A00
// FUNCTION: fn_10695
UINameBalloonText::~UINameBalloonText() = default;

// OFFSET: 0x00869E20
// FUNCTION: fn_10974
bool UIBalloonText::CanHitTest() {
  return false;
}

// OFFSET: 0x0085FAE0
// FUNCTION: fn_10882
void UIBalloonText::Slot19() {
  ++this->geometry.unknown_0034;
}

// OFFSET: 0x00867F20
// FUNCTION: fn_10950
void UIBalloonText::SetText(const char* text, int width) {
  if (std::strlen(text) == 0) {
    return;
  }

  this->m_lines.clear();

  if (this->m_richText != 0) {
    WrapRichText(text, this->m_lines, width, false, this->m_textContext->codePage, this->m_textContext->items);
  } else {
    WrapPlainText(text, this->m_lines, width, this->m_textContext->codePage);
  }

  if (!this->m_lines.empty()) {
    this->FitText();
  }
}

// OFFSET: 0x00853530
// FUNCTION: fn_10740
void UIBalloonText::FitText() {
  int width = 0;
  int height = 4;
  const int family = Family(this->m_fontMode);

  for (const std::string& line : this->m_lines) {
    const SIZE extent = Measure(*this, *this->m_textContext, line, family, this->m_fontHeight);

    if (extent.cx > width) {
      width = extent.cx;
    }

    const int lineHeight = this->m_fontMode == 3 ? extent.cy : Measure(*this, *this->m_textContext, line, 0, this->m_fontHeight).cy;
    height = (height + (lineHeight + 4));
  }

  this->Resize((width + 12), height);
  this->Invalidate();
}

// OFFSET: 0x0085B140
// FUNCTION: fn_10793
void UIBalloonText::Paint() {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, 0xFFFFFF);
  }

  int y = 4;

  for (const std::string& line : this->m_lines) {
    this->DrawTextAt(this->m_textContext->fonts, this->m_textContext->locale, 6, y, line.c_str(), 0, 0, this->m_fontHeight, this->m_textColor, 0, 0);

    const int height = Measure(*this, *this->m_textContext, line, 0, this->m_fontHeight).cy;
    y = (y + (height + 4));
  }
}

// OFFSET: 0x0085FC90
// FUNCTION: fn_10884
void UINameBalloonText::Slot19() {
  CEmblemRes* emblem = dynamic_cast<CEmblemRes*>(ResourceMgr::Instance().Load(this->iconName.c_str()));
  if (emblem) {
    if (emblem->gif.frameChanged == 1) {
      this->Invalidate();
    }

    return;
  }

  CGifRes* gif = dynamic_cast<CGifRes*>(ResourceMgr::Instance().Load(this->iconName.c_str()));
  if (gif && gif->frameChanged == 1) {
    this->Invalidate();
  }
}
