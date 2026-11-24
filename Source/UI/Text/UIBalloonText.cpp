#include "Contents/Items/ItemDescription.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/ActorNameCard.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
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
  SIZE Measure(UIBalloonText& window, const std::string& text, int family, int height) {
    if (text.empty()) {
      return {0, 0};
    }

    TextContext context(*window.geometry.surface);
    context.SetFont(family, height, 0, 0);
    SIZE size;
    context.Measure(text.c_str(), static_cast<int>(text.size()), &size);
    return size;
  }
}

// OFFSET: 0x0084E970
// FUNCTION: fn_10648
__declspec(noinline) UIBalloonText::UIBalloonText() : UIWindow(0) {
  this->m_richText = 0;
  this->balloonFlag = 0;
  this->m_fontHeight = 12;
  this->geometry.animationFrame = 0;
  this->m_textColor = 0xFFFFFF;
  this->m_outlineColor = 0;
}

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
void UIBalloonText::OnProcess() {
  ++this->geometry.animationFrame;
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

  for (unsigned int index = 0; index < this->m_lines.size(); ++index) {
    const std::int16_t mode = this->m_fontMode;
    if (mode == 3) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 22, this->m_fontHeight, 0, 0));
      height = (height + (this->TextHeight(this->m_lines[index].c_str(), 0, 22, this->m_fontHeight, 0, 0) + 4));
    } else if (mode == 2) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 20, this->m_fontHeight, 0, 0));
    } else if (mode == 1) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 21, this->m_fontHeight, 0, 0));
    } else if (mode == 4) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 23, this->m_fontHeight, 0, 0));
    } else if (mode == 5) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 24, this->m_fontHeight, 0, 0));
    } else if (mode == 6) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 25, this->m_fontHeight, 0, 0));
    } else if (mode == 7) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 26, this->m_fontHeight, 0, 0));
    } else if (mode == 8) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 27, this->m_fontHeight, 0, 0));
    } else if (mode == 9) {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 28, this->m_fontHeight, 0, 0));
    } else {
      width = std::max(width, this->TextWidth(this->m_lines[index].c_str(), 0, 0, this->m_fontHeight, 0, 0));
    }

    if (3 != this->m_fontMode) {
      height = (height + (this->TextHeight(this->m_lines[index].c_str(), 0, 0, this->m_fontHeight, 0, 0) + 4));
    }
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
      this->DrawBitmap(2, (this->geometry.height - frame->image.height) / 2, frame, true);
      x = frame->image.width + 4;
    }
  }

  if (this->iconName.size() != 0 && ResourceMgr::Instance().Exists(this->iconName.c_str())) {
    if (this->iconName.find(".bmp", 0, 4) != std::string::npos) {
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->iconName.c_str())));
      if (bitmap) {
        g_modeMgr.GetGameMode();
        this->DrawBitmap(4, (this->geometry.height - 24) / 2, bitmap, true);
        if (this->framed == 0) {
          x += 28;
        }
      }
    } else if (this->iconName.find(".gif", 0, 4) != std::string::npos) {
      CGifRes* gif = static_cast<CGifRes*>(ResourceMgr::Instance().Load(Resolve(this->iconName.c_str())));
      if (gif) {
        this->DrawBitmap(4, (this->geometry.height - 24) / 2, gif, true);
        if (this->framed == 0) {
          x += 28;
        }
      }
    } else {
      CEmblemRes* emblem = static_cast<CEmblemRes*>(ResourceMgr::Instance().Load(this->iconName.c_str()));
      if (emblem) {
        g_modeMgr.GetGameMode();
        this->DrawBitmap(4, (this->geometry.height - 24) / 2, &emblem->gif, true);
        if (this->framed == 0) {
          x += 28;
        }
      }
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
      this->DrawBitmap(0, 0, icon, true);
      x += icon->image.width;
    }
  }

lines:
  for (auto line = this->m_lines.begin(); line != this->m_lines.end(); ++line) {
    this->DrawOutlinedText(
      x, y, line->c_str(), static_cast<unsigned>(line->size()), this->m_textColor, this->m_outlineColor, 0, g_nameBalloonFontHeight,
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
      this->DrawBitmap((this->geometry.width - 24) / 2, 0, &emblem->gif, true);
      y = 28;
    }
  }

  for (auto line = this->m_lines.begin(); line != this->m_lines.end(); ++line) {
    this->DrawVerticalOutlinedText(
      x, y, line->c_str(), static_cast<int>(line->size()), this->m_textColor, this->m_outlineColor, 0, g_nameBalloonFontHeight, g_serviceFlag06
    );
    x += g_nameBalloonFontHeight + 2;
  }
}

// OFFSET: 0x0085FC90
// FUNCTION: fn_10884
void UINameBalloonText::OnProcess() {
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

  if (!ResourceMgr::Instance().Exists(path)) {
    if (!framed) {
      return;
    }
  } else {
    this->iconName = path;
    if (!framed) {
      this->Resize(this->nameWidth + 25, std::max(this->geometry.height, 24));
    }
  }

  if (framed == 1) {
    this->Resize(this->nameWidth + 29, std::max(this->geometry.height, 28));
  }

  this->Invalidate();
}

// OFFSET: 0x00865C40
// FUNCTION: fn_45617
void UIVerticalNameBalloonText::SetIcon(const char* name) {
  if (name == nullptr || std::strlen(name) == 0) {
    return;
  }

  this->iconName = name;
  if (this->iconName.size() != 0) {
    this->Resize(std::max(this->geometry.width, 24), this->textHeight + 25);
  }
  this->Invalidate();
}

// OFFSET: 0x0084FED0
// FUNCTION: fn_10668
UINameBalloonText::UINameBalloonText() : UIBalloonText(), monsterType(0), framed(0) {
  this->m_textColor = 0xFFFFFF;
  this->m_outlineColor = 0;
  this->nameWidth = 0;
  this->textFlags = 0;
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
    if (info.displayName[0] != '\0') {
      this->m_lines.push_back(info.displayName.c_str());
    }

    if (info.clanName[0] == '\0') {
      this->nameWidth = 0;
    } else {
      char position[260];
      std::memset(position, 0, sizeof(position));
      strncpy_s(position, sizeof(position), info.clanName.c_str(), info.clanName.size());
      if (g_forbiddenWords.Contains(position) == true) {
        std::string masked = "";
        g_forbiddenWords.Mask(position, &masked);
      }
      this->m_lines.push_back(position);
    }

    this->m_lines.push_back(name.c_str());
  } else {
    if (info.displayName[0] != '\0') {
      this->m_lines.push_back(info.displayName.c_str());
    }

    if (info.title[0] != '\0') {
      name = static_cast<const std::string&>(FormatOwnedString("[%s] %s", info.title.c_str(), name.c_str()));
    }

    if (info.partyName[0] == '\0') {
      this->m_lines.push_back(name.c_str());
    } else {
      char party[260];
      std::memset(party, 0, sizeof(party));
      strncpy_s(party, sizeof(party), info.partyName.c_str(), info.partyName.size());
      if (g_forbiddenWords.Contains(party) == true) {
        std::string masked = "";
        g_forbiddenWords.Mask(party, &masked);
      }

      char line[260];
      std::memset(line, 0, sizeof(line));
      if (g_serviceType == ServiceKorea) {
        sprintf_s(line, sizeof(line), "%s (%s) ", name.c_str(), party);
      } else {
        sprintf_s(line, sizeof(line), "%s (%s)", name.c_str(), party);
      }
      this->m_lines.push_back(line);
    }

    if (info.guildName[0] != '\0') {
      char guild[260];
      std::memset(guild, 0, sizeof(guild));
      strncpy_s(guild, sizeof(guild), info.guildName.c_str(), info.guildName.size());
      if (g_forbiddenWords.Contains(guild) == true) {
        std::string masked = "";
        g_forbiddenWords.Mask(guild, &masked);
      }

      char position[260];
      std::memset(position, 0, sizeof(position));
      strncpy_s(position, sizeof(position), info.clanName.c_str(), info.clanName.size());
      if (g_forbiddenWords.Contains(position) == true) {
        std::string masked = "";
        g_forbiddenWords.Mask(position, &masked);
      }

      char line[260];
      std::memset(line, 0, sizeof(line));
      if (g_serviceType == ServiceKorea) {
        sprintf_s(line, sizeof(line), "%s (%s) ", guild, position);
      } else {
        sprintf_s(line, sizeof(line), "%s [%s]", guild, position);
      }
      this->m_lines.push_back(line);
    }
  }

  int width = 32;
  for (auto line = this->m_lines.begin(); line != this->m_lines.end(); ++line) {
    const std::string text = *line;
    const int lineWidth = this->TextWidth(text.c_str(), 0, 0, g_nameBalloonFontHeight, g_serviceFlag06, 0) + 8;
    width = std::max(lineWidth, width);
  }

  this->nameWidth = width;
  this->monsterType = monsterType;
  if (monsterType == 0 || info.groupIconId != 0) {
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
      this->Resize(bitmap->image.width + this->nameWidth, static_cast<int>(height));
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
  this->m_lines.push_back(text);
  this->nameWidth = std::max(
    this->TextWidth(this->m_lines.back().c_str(), this->m_lines.back().size(), 0, g_nameBalloonFontHeight, g_serviceFlag06, 0) + 8, 32
  );
  this->Resize(this->nameWidth, g_nameBalloonFontHeight + 8);
  this->Invalidate();
}

// OFFSET: 0x00867740
// FUNCTION: fn_10942
void UIVerticalNameBalloonText::AddNameLine(const char* text) {
  if (this->m_lines.size() != 0 && this->m_lines.front() == text) {
    return;
  }

  this->m_lines.clear();
  this->m_lines.push_back(text);
  const int textHeight = this->VerticalTextHeight(this->m_lines.back().c_str(), 0, 0, g_nameBalloonFontHeight, g_serviceFlag06);
  this->textHeight = std::max(textHeight + 8, 32);
  this->Resize(g_nameBalloonFontHeight + 8, this->textHeight);
  this->Invalidate();
}

// OFFSET: 0x00867000
// FUNCTION: fn_45651
void UIVerticalNameBalloonText::SetNameLines(const CActorNameCard& info, bool stripHash) {
  this->m_lines.clear();

  std::string name = info.name;
  if (stripHash) {
    const std::size_t hash = name.find('#');
    if (hash != std::string::npos) {
      name.resize(hash);
    }
  }

  char line[256];
  if (g_serviceFlag05) {
    if (info.clanName[0] == '\0') {
      this->textHeight = 0;
    } else {
      this->m_lines.push_back(info.clanName.c_str());
      const int height = this->VerticalTextHeight(this->m_lines.back().c_str(), this->m_lines.back().size(), 0, g_nameBalloonFontHeight, g_serviceFlag06);
      this->textHeight = std::max(height + 8, 32);
    }
    this->m_lines.push_back(name.c_str());
    const int nameHeight = this->VerticalTextHeight(this->m_lines.back().c_str(), 0, 0, g_nameBalloonFontHeight, g_serviceFlag06);
    this->textHeight = std::max(nameHeight + 8, this->textHeight);
  } else {
    if (info.partyName[0] == '\0') {
      this->m_lines.push_back(name.c_str());
    } else {
      std::sprintf(line, "%s (%s)", name.c_str(), info.partyName.c_str());
      this->m_lines.push_back(line);
    }
    const int firstHeight = this->VerticalTextHeight(this->m_lines.back().c_str(), this->m_lines.back().size(), 0, g_nameBalloonFontHeight, g_serviceFlag06);
    this->textHeight = std::max(firstHeight + 8, 32);

    if (info.guildName[0] != '\0') {
      std::sprintf(line, "%s [%s]", info.guildName.c_str(), info.clanName.c_str());
      this->m_lines.push_back(line);
      const int guildHeight = this->VerticalTextHeight(this->m_lines.back().c_str(), 0, 0, g_nameBalloonFontHeight, g_serviceFlag06);
      this->textHeight = std::max(guildHeight + 8, this->textHeight);
    }
  }

  this->Resize(this->textHeight, static_cast<int>(this->m_lines.size()) * (g_nameBalloonFontHeight + 2) + 6);
  this->Invalidate();
}

// FOLDED: fn_21415
std::string UIBalloonText::GetLine(unsigned int index) {
  if (!this->m_lines.empty() && index <= this->m_lines.size()) {
    return this->m_lines[index];
  }

  return "";
}
