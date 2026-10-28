#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/AgencyRequestContext.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/TextFitting.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Party/UIPartyInfoWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdio>

namespace {
  constexpr char prefix[] = "유저인터페이스\\";
}

// OFFSET: 0x007BAD00
// FUNCTION: fn_09275
UIApplyForPartyWnd::UIApplyForPartyWnd(UIWindowContext& windows, UIFrameContext& frames, UIApplyForPartyContext& application)
    : UIFrameWnd(windows, frames, 0), m_application(&application) {
}

// OFFSET: 0x007BB610
UIApplyForPartyWnd::~UIApplyForPartyWnd() = default;

// OFFSET: 0x007C2400
// FUNCTION: fn_09325
void UIApplyForPartyWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007BD970
// FUNCTION: fn_09299
void UIApplyForPartyWnd::Slot15(int, int) {
  auto& controls = this->m_application->buttons;
  const int messages[] = {3374, 3373, 3045};
  const int ids[] = {381, 271, 201};
  const int positions[] = {51, 108, 173};

  for (int index = 0; index < 3; ++index) {
    auto* button = new UIBasicButton(*this->m_context, controls);
    const char* label = controls.services.MessageText(messages[index]);
    const int width = this->TextWidth(controls.fonts, controls.locale, label, 0, 0, 12, 0, 0);
    button->Configure(width + 10, label);
    button->Move(positions[index], 165);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  std::vector<std::string> lines;
  WrapPlainText(controls.services.MessageText(3370), lines, 40, this->m_application->codePage);
  int y = 136;

  for (const auto& line : lines) {
    auto* label = new UIStaticText(*this->m_context, controls);
    label->SetSize(this->geometry.width - 18, 15);
    label->Move(9, y);
    label->SetText(line.c_str(), 0);
    label->fontHeight = 11;
    label->red = 255;
    label->green = 0;
    label->blue = 0;
    this->AddChild(label);
    y += 12;
  }

  auto* close = new UIBitmapButton(*this->m_context, controls);
  const std::string off = std::string(prefix) + "basic_interface\\big_sys_close_off.bmp";
  const std::string on = std::string(prefix) + "basic_interface\\big_sys_close_on.bmp";
  close->SetBitmap(off.c_str(), 0, 0);
  close->SetBitmap(on.c_str(), 1, 0);
  close->SetBitmap(on.c_str(), 2, 0);
  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(this->geometry.width - close->BitmapWidth(), 0);
  close->SetId(201);
  close->SetTooltip("");
  this->AddChild(close);
}

// OFFSET: 0x007C1D10
// FUNCTION: fn_09319
void UIApplyForPartyWnd::SetInfo(CAdvenPartyInfo value) {
  this->info = value;
  this->Invalidate();
}

// OFFSET: 0x007BF5A0
// FUNCTION: fn_09303
void UIApplyForPartyWnd::Paint() {
  auto& controls = this->m_application->buttons;
  this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
  const auto load = [&](const char* name) { return static_cast<CBitmapRes*>(controls.resources.Load((std::string(prefix) + name).c_str())); };
  const auto* background = load("UIadventurerAgency\\bg_agency_register.bmp");
  this->DrawBitmap(0, 0, background ? &background->image : nullptr, false);
  int leftWidth = 0;
  int rightWidth = 0;
  int titleHeight = 0;

  if (const auto* left = load("basic_interface\\titlebar_left.bmp")) {
    this->DrawBitmap(0, 0, &left->image, false);
    leftWidth = left->image.width;
  }

  if (const auto* right = load("basic_interface\\titlebar_right.bmp")) {
    rightWidth = right->image.width;
    this->DrawBitmap(this->geometry.width - rightWidth, 0, &right->image, false);
  }

  if (const auto* middle = load("basic_interface\\titlebar_mid.bmp")) {
    titleHeight = middle->image.height;
    const int extent = this->geometry.width - rightWidth - leftWidth;
    const int tiles = extent / middle->image.width + (extent % middle->image.width != 0);

    for (int index = 0; index < tiles; ++index) {
      this->DrawBitmap(leftWidth, 0, &middle->image, false);
      leftWidth += middle->image.width;
    }
  }

  const char* caption = controls.services.MessageText(3368);
  this->DrawTextAt(controls.fonts, controls.locale, 19, titleHeight - 13, caption, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 18, titleHeight - 14, caption, 0, 0, 12, 0, 0, 0);
  const auto* base = load("basic_interface\\sys_base_off.bmp");
  this->DrawBitmap(3, 3, base ? &base->image : nullptr, true);
  const auto text = [&](int x, int y, const char* value) { this->DrawTextAt(controls.fonts, controls.locale, x, y, value, 0, 0, 12, 0, 0, 0); };
  text(10, 25, controls.services.MessageText(3415));
  const std::string fitted = FitText(*this->geometry.surface, this->info.characterName, 119);
  text(60, 25, fitted.c_str());
  text(10, 49, controls.services.MessageText(408));
  char levels[10] = {};
  _snprintf_s(levels, sizeof(levels), _TRUNCATE, "%d~%d", this->info.minimumLevel, this->info.maximumLevel);
  text(60, 49, levels);
  text(10, 73, controls.services.MessageText(3369));
  text(60, 73, UIPartyInfoWnd::TypeName(this->info.type, controls.services));
  text(10, 97, controls.services.MessageText(504));
  const char* jobs[] = {"def", "heal", "atk", "buf"};
  const bool selected[] = {this->info.tanker, this->info.healer, this->info.dealer, this->info.assist};

  for (int index = 0; index < 4; ++index) {
    const std::string name = std::string("UIadventurerAgency\\btn_job_") + jobs[index] + (selected[index] ? "_on.bmp" : "_off.bmp");
    const auto* icon = load(name.c_str());
    this->DrawBitmap(60 + index * 18, 95, icon ? &icon->image : nullptr, false);
  }

  text(10, 121, controls.services.MessageText(3359));
  text(60, 121, this->info.memo.c_str());
}

// OFFSET: 0x007BFCC0
// FUNCTION: fn_09309
void UIApplyForPartyWnd::Slot28(int x, int y) {
  if (static_cast<unsigned>(y) - 95u > 16u) {
    return;
  }

  const int positions[] = {60, 78, 96, 114};
  const int messages[] = {3420, 3491, 3444, 3508};

  for (int index = 0; index < 4; ++index) {
    if (x > positions[index] && x < positions[index] + 16) {
      const char* label = this->m_application->buttons.services.MessageText(messages[index]);

      if (*label != '\0') {
        int screenX = positions[index];
        int screenY = 75;
        this->ToScreen(screenX, screenY);
        this->m_application->buttons.services.Tooltip(label, screenX, screenY, -1, false, true);
      }

      break;
    }
  }
}

// OFFSET: 0x007C11E0
// FUNCTION: fn_09313
int UIApplyForPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    auto& owner = *this->m_application;

    switch (static_cast<int>(a)) {
    case 201:
      this->m_frameContext->windows.CloseWindow(345);
      break;
    case 271:
      g_adventurerAgencyMgr->RequestInfo(this->info.accountId, owner.requests, owner.completion);
      break;
    case 381: {
      const std::string name = this->info.characterName;
      const char* relationship = owner.rows.services.IsFriend(name) ? owner.buttons.services.MessageText(102) : nullptr;

      if (owner.rows.services.IsGuildMember(this->info.accountId)) {
        relationship = "GuildMember";
      }

      const std::string code = owner.rows.services.CharacterCode(this->info.characterId);
      owner.rows.services.ShowCharacter(name.c_str(), relationship, code.c_str());
      break;
    }
    default:
      break;
    }

    return 0;
  }

  if (message == 34) {
    this->saved = reinterpret_cast<PartyBoardPlacement*>(a);
    const auto& owner = *this->m_application;

    if (this->saved->width <= 0 || this->saved->height <= 0 || this->saved->x < 0 || this->saved->x > owner.screenWidth - this->saved->width ||
        this->saved->y < 0 || this->saved->y > owner.screenHeight - this->saved->height) {
      *this->saved = {200, 200, 220, 190};
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
