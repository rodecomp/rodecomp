#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Text/UIBalloonText.h"
#include "UI/Text/UINameBalloonText.h"

#include <algorithm>
#include <cstring>
#include <string>

// GLOBAL: gl_24926
int g_nameBalloonFontHeight = 12;

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

// OFFSET: 0x00867EC0
// FUNCTION: fn_10949
void UIBalloonText::WrapText(const char* text, int width) {
  if (std::strlen(text) == 0) {
    return;
  }

  this->m_lines.clear();
  WrapPlainText(text, this->m_lines, width, this->m_textContext->codePage);
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

// OFFSET: 0x0085D1F0
// FUNCTION: fn_10812
void UINameBalloonText::Paint() {
  this->ClearSurface(0xFFFF00FF);
  int x = 4;
  int y = 4;
  if (this->framed == 1) {
    CBitmapRes* frame = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\emblem_frame.bmp")));
    if (frame) {
      this->DrawBitmap(2, (this->geometry.height - frame->image.height) / 2, &frame->image, true);
      x = frame->image.width + 4;
    }
  }

  if (this->iconName.size() != 0 && ResourceMgr::Instance().Exists(this->iconName.c_str())) {
    const BitmapImage* image;
    if (this->iconName.find(".bmp", 0, 4) != std::string::npos) {
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->iconName.c_str())));
      if (!bitmap) {
        goto lines;
      }

      g_modeMgr.GetGameMode();
      image = &bitmap->image;
    } else if (this->iconName.find(".gif", 0, 4) != std::string::npos) {
      CGifRes* gif = static_cast<CGifRes*>(ResourceMgr::Instance().Load(Resolve(this->iconName.c_str())));
      if (!gif) {
        goto lines;
      }

      image = &gif->image;
    } else {
      CEmblemRes* emblem = static_cast<CEmblemRes*>(ResourceMgr::Instance().Load(this->iconName.c_str()));
      if (!emblem) {
        goto lines;
      }

      g_modeMgr.GetGameMode();
      image = &emblem->gif.image;
    }

    this->DrawBitmap(4, (this->geometry.height - 24) / 2, image, true);
    if (this->framed == 0) {
      x += 28;
    }
  } else if (this->monsterType != 0) {
    const char* name;
    if (this->monsterType == 1) {
      name = "유저인터페이스\\montype_boss.bmp";
    } else if (this->monsterType == 2) {
      name = "유저인터페이스\\montype_mvp.bmp";
    } else {
      goto lines;
    }

    CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
    if (icon) {
      this->DrawBitmap(0, 0, &icon->image, true);
      x += icon->image.width;
    }
  }

lines:
  for (const std::string& line : this->m_lines) {
    DrawOutlinedText(
      *this, x, y, line.c_str(), static_cast<unsigned>(line.size()), this->m_textColor, this->m_outlineColor, 0, g_nameBalloonFontHeight,
      g_serviceFlag06, 0
    );
    y += g_nameBalloonFontHeight + 2;
  }
}

// OFFSET: 0x0085E280
// FUNCTION: fn_10824
void UIVerticalNameBalloonText::Paint() {
  this->ClearSurface(0xFFFF00FF);
  int x = 4;
  int y = 4;
  if (this->iconName.size() != 0 && ResourceMgr::Instance().Exists(this->iconName.c_str())) {
    CEmblemRes* emblem = static_cast<CEmblemRes*>(ResourceMgr::Instance().Load(this->iconName.c_str()));
    if (emblem) {
      this->DrawBitmap((this->geometry.width - 24) / 2, 0, &emblem->gif.image, true);
      y = 28;
    }
  }

  for (const std::string& line : this->m_lines) {
    this->DrawVerticalOutlinedText(
      x, y, line.c_str(), static_cast<int>(line.size()), this->m_textColor, this->m_outlineColor, 0, g_nameBalloonFontHeight, g_serviceFlag06
    );
    x += g_nameBalloonFontHeight + 2;
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

// OFFSET: 0x00865CC0
// FUNCTION: fn_10935
void UIBalloonText::SetFontHeight(int height) {
  this->m_fontHeight = height;
  this->FitText();
}

// OFFSET: 0x008659A0
// FUNCTION: fn_10934
void UINameBalloonText::SetIcon(const char* name, unsigned char framed) {
  this->iconName = "";
  this->framed = framed;
  std::string path = name;
  if (path.empty() && !framed) {
    return;
  }

  const std::size_t extension = path.find(".bmp");
  if (extension != std::string::npos) {
    path.replace(extension, path.size() - extension, ".gif");
    if (!ResourceMgr::Instance().Exists(path)) {
      path = name;
    }
  }

  if (ResourceMgr::Instance().Exists(path)) {
    this->iconName = path;
    if (!framed) {
      this->Resize(this->unknown_00B8 + 25, std::max(this->geometry.height, 24));
      this->Invalidate();
      return;
    }
  } else if (!framed) {
    return;
  }

  if (framed == 1) {
    this->Resize(this->unknown_00B8 + 29, std::max(this->geometry.height, 28));
  }

  this->Invalidate();
}
