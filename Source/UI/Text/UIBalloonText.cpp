#include "Contents/Items/ItemDescription.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/ActorNameCard.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/ForbiddenWords.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Text/UIBalloonText.h"
#include "UI/Text/UINameBalloonText.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>

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
  SIZE Measure(UIBalloonText& window, const std::string& text, int family, int height) {
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
__declspec(noinline) UIBalloonText::UIBalloonText()
    : UIWindow(0), unknown_007C(0), m_fontHeight(12), m_textColor(0xFFFFFF), m_outlineColor(0), m_richText(0) {
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
    WrapRichText(text, this->m_lines, width, false);
  } else {
    WrapPlainText(text, this->m_lines, width);
  }

  if (this->m_lines.size() != 0) {
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
  WrapPlainText(text, this->m_lines, width);
}

// OFFSET: 0x00853530
// FUNCTION: fn_10740
void UIBalloonText::FitText() {
  int width = 0;
  int height = 4;
  const int family = Family(this->m_fontMode);

  for (const std::string& line : this->m_lines) {
    const SIZE extent = Measure(*this, line, family, this->m_fontHeight);

    if (extent.cx > width) {
      width = extent.cx;
    }

    const int lineHeight = this->m_fontMode == 3 ? extent.cy : Measure(*this, line, 0, this->m_fontHeight).cy;
    height = (height + (lineHeight + 4));
  }

  this->Resize((width + 12), height);
  this->Invalidate();
}

// OFFSET: 0x0085B140
// FUNCTION: fn_10793
void UIBalloonText::Paint() {
  this->ClearSurface(0xFFFFFF);

  int y = 4;
  unsigned int index = 0;

  while (index < this->m_lines.size()) {
    this->DrawTextAt(6, y, this->m_lines[index].c_str(), 0, 0, this->m_fontHeight, this->m_textColor, 0, 0);

    const int height = this->TextHeight(this->m_lines[index].c_str(), 0, 0, this->m_fontHeight, 0, 0);
    y += height + 4;
    ++index;
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
// OFFSET: 0x0084FED0
// FUNCTION: fn_10668
UINameBalloonText::UINameBalloonText() : UIBalloonText(), monsterType(0), framed(0) {
  this->m_textColor = 0xFFFFFF;
  this->m_outlineColor = 0;
  this->unknown_00B8 = 0;
  this->unknown_0098 = 0;
}

// OFFSET: 0x00CCDFF0
// FUNCTION: fn_21219
UIVerticalNameBalloonText::UIVerticalNameBalloonText() : UINameBalloonText() {
}

// OFFSET: 0x00865EB0
// FUNCTION: fn_10940
void UINameBalloonText::SetName(const CActorNameCard& info, bool stripHash, unsigned char monsterType) {
  this->m_lines.clear();

  std::string name = info.name;
  if (stripHash) {
    const std::size_t hash = name.find('#');
    if (hash != std::string::npos) {
      name.resize(hash);
    }
  }

  if (g_serviceFlag05) {
    if (std::strlen(info.unknown_007C.c_str()) != 0) {
      this->m_lines.push_back(info.unknown_007C.c_str());
    }

    if (std::strlen(info.unknown_004C.c_str()) == 0) {
      this->unknown_00B8 = 0;
    } else {
      char position[260];
      std::memset(position, 0, sizeof(position));
      strncpy_s(position, sizeof(position), info.unknown_004C.c_str(), info.unknown_004C.size());
      if (g_forbiddenWords.Contains(position)) {
        std::string masked;
        g_forbiddenWords.Mask(position, &masked);
      }
      this->m_lines.push_back(position);
    }

    this->m_lines.push_back(name.c_str());
  } else {
    if (std::strlen(info.unknown_007C.c_str()) != 0) {
      this->m_lines.push_back(info.unknown_007C.c_str());
    }

    if (std::strlen(info.unknown_0064.c_str()) != 0) {
      name = FormatOwnedString("[%s] %s", info.unknown_0064.c_str(), name.c_str());
    }

    if (std::strlen(info.unknown_001C.c_str()) == 0) {
      this->m_lines.push_back(name.c_str());
    } else {
      char party[260];
      std::memset(party, 0, sizeof(party));
      strncpy_s(party, sizeof(party), info.unknown_001C.c_str(), info.unknown_001C.size());
      if (g_forbiddenWords.Contains(party)) {
        std::string masked;
        g_forbiddenWords.Mask(party, &masked);
      }

      char line[260];
      std::memset(line, 0, sizeof(line));
      sprintf_s(line, sizeof(line), g_serviceType != 0 ? "%s (%s)" : "%s (%s) ", name.c_str(), party);
      this->m_lines.push_back(line);
    }

    if (std::strlen(info.unknown_0034.c_str()) != 0) {
      char guild[260];
      std::memset(guild, 0, sizeof(guild));
      strncpy_s(guild, sizeof(guild), info.unknown_0034.c_str(), info.unknown_0034.size());
      if (g_forbiddenWords.Contains(guild)) {
        std::string masked;
        g_forbiddenWords.Mask(guild, &masked);
      }

      char position[260];
      std::memset(position, 0, sizeof(position));
      strncpy_s(position, sizeof(position), info.unknown_004C.c_str(), info.unknown_004C.size());
      if (g_forbiddenWords.Contains(position)) {
        std::string masked;
        g_forbiddenWords.Mask(position, &masked);
      }

      char line[260];
      std::memset(line, 0, sizeof(line));
      sprintf_s(line, sizeof(line), g_serviceType != 0 ? "%s [%s]" : "%s (%s) ", guild, position);
      this->m_lines.push_back(line);
    }
  }

  int width = 32;
  for (const std::string& line : this->m_lines) {
    const int lineWidth = this->TextWidth(line.c_str(), 0, 0, g_nameBalloonFontHeight, g_serviceFlag06, 0) + 8;
    width = std::max(lineWidth, width);
  }

  this->unknown_00B8 = width;
  this->monsterType = monsterType;
  if (monsterType == 0 || info.unknown_0094 != 0) {
    this->Resize(width, static_cast<int>(this->m_lines.size()) * (g_nameBalloonFontHeight + 2) + 6);
  } else {
    const char* path;
    if (monsterType == 1) {
      path = "유저인터페이스\\montype_boss.bmp";
    } else if (monsterType == 2) {
      path = "유저인터페이스\\montype_mvp.bmp";
    } else {
      this->Invalidate();
      return;
    }

    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (bitmap) {
      std::size_t height = this->m_lines.size() * (g_nameBalloonFontHeight + 2) + 6;
      if (height < static_cast<std::size_t>(bitmap->image.height)) {
        height = bitmap->image.height;
      }
      this->Resize(bitmap->image.width + this->unknown_00B8, static_cast<int>(height));
    }
  }

  this->Invalidate();
}

// OFFSET: 0x00866DE0
// FUNCTION: fn_10941
void UINameBalloonText::AddNameLine(const char* text) {
  if (this->m_lines.size() != 0 && this->m_lines.front() == text) {
    return;
  }

  this->m_lines.clear();
  this->m_lines.push_back(std::string(text));
  const std::string& last = this->m_lines.back();
  this->unknown_00B8 =
    std::max(this->TextWidth(last.c_str(), last.size(), 0, g_nameBalloonFontHeight, g_serviceFlag06, 0) + 8, 32);
  this->Resize(this->unknown_00B8, g_nameBalloonFontHeight + 8);
  this->Invalidate();
}

// OFFSET: 0x00867740
// FUNCTION: fn_10942
void UIVerticalNameBalloonText::AddNameLine(const char* text) {
  if (this->m_lines.size() != 0 && this->m_lines.front() == text) {
    return;
  }

  this->m_lines.clear();
  this->m_lines.push_back(std::string(text));
  const int textHeight = this->VerticalTextHeight(this->m_lines.back().c_str(), 0, 0, g_nameBalloonFontHeight, g_serviceFlag06);
  this->textHeight = std::max(textHeight + 8, 32);
  this->Resize(g_nameBalloonFontHeight + 8, this->textHeight);
  this->Invalidate();
}
