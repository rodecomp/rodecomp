#include "Client/Session.h"
#include "File/ShortcutFilename.h"
#include "Render/Renderer.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Text/TextFitting.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Party/UIPartyInfoWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Mode/CMode.h"

#include <cstdio>

namespace {
  constexpr char prefix[] = "유저인터페이스\\";
}

// OFFSET: 0x007BAD00
// FUNCTION: fn_09275
UIApplyForPartyWnd::UIApplyForPartyWnd()
    : UIFrameWnd(0) {
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
  const int messages[] = {3374, 3373, 3045};
  const int ids[] = {381, 271, 201};
  const int positions[] = {51, 108, 173};

  for (int index = 0; index < 3; ++index) {
    auto* button = new UIBasicButton();
    const char* label = LookupMessage(messages[index]);
    const int width = this->TextWidth(label, 0, 0, 12, 0, 0);
    button->Configure(width + 10, label);
    button->Move(positions[index], 165);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  std::vector<std::string> lines;
  WrapPlainText(LookupMessage(3370), lines, 40);
  int y = 136;

  for (const auto& line : lines) {
    auto* label = new UIStaticText();
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

  auto* close = new UIBitmapButton();
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
  this->ClearSurface(0xFFFF00FF);
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIadventurerAgency\\bg_agency_register.bmp")));
  this->DrawBitmap(0, 0, background ? &background->image : nullptr, false);
  this->DrawTitleBar(true, LookupMessage(3368), 0);
  const auto text = [&](int x, int y, const char* value) { this->DrawTextAt(x, y, value, 0, 0, 12, 0, 0, 0); };
  text(10, 25, LookupMessage(3415));
  const std::string fitted = FitText(*this->geometry.surface, this->info.characterName, 119);
  text(60, 25, fitted.c_str());
  text(10, 49, LookupMessage(408));
  char levels[10] = {};
  _snprintf_s(levels, sizeof(levels), _TRUNCATE, "%d~%d", this->info.minimumLevel, this->info.maximumLevel);
  text(60, 49, levels);
  text(10, 73, LookupMessage(3369));
  text(60, 73, UIPartyInfoWnd::TypeName(this->info.type));
  text(10, 97, LookupMessage(504));
  const auto job = [&](int x, bool selected, const char* selectedImage, const char* unselectedImage) {
    const auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(selected == 1 ? selectedImage : unselectedImage)));
    this->DrawBitmap(x, 95, icon ? &icon->image : nullptr, false);
  };
  job(60, this->info.tanker, "유저인터페이스\\UIadventurerAgency\\btn_job_def_on.bmp", "유저인터페이스\\UIadventurerAgency\\btn_job_def_off.bmp");
  job(78, this->info.healer, "유저인터페이스\\UIadventurerAgency\\btn_job_heal_on.bmp", "유저인터페이스\\UIadventurerAgency\\btn_job_heal_off.bmp");
  job(96, this->info.dealer, "유저인터페이스\\UIadventurerAgency\\btn_job_atk_on.bmp", "유저인터페이스\\UIadventurerAgency\\btn_job_atk_off.bmp");
  job(114, this->info.assist, "유저인터페이스\\UIadventurerAgency\\btn_job_buf_on.bmp", "유저인터페이스\\UIadventurerAgency\\btn_job_buf_off.bmp");
  text(10, 121, LookupMessage(3359));
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
      const char* label = LookupMessage(messages[index]);

      if (*label != '\0') {
        int screenX = positions[index];
        int screenY = 75;
        this->ToScreen(screenX, screenY);
        static_cast<CMode*>(g_charViewMode)->ShowTooltip(label, screenX, screenY, -1, false, true);
      }

      break;
    }
  }
}

// OFFSET: 0x007C11E0
// FUNCTION: fn_09313
int UIApplyForPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    switch (static_cast<int>(a)) {
    case 201:
      g_windowMgr.CloseWindow(345);
      break;
    case 271:
      g_adventurerAgencyMgr->RequestInfo(this->info.accountId);
      break;
    case 381: {
      const std::string name = this->info.characterName;
      const char* relationship = g_windowMgr.IsFriend(name.c_str()) ? LookupMessage(102) : nullptr;

      if (g_session.IsGuildMemberAccount(this->info.accountId)) {
        relationship = "GuildMember";
      }

      const std::string code = ShortcutFilename(this->info.characterId);
      g_windowMgr.SendNotice(14, reinterpret_cast<std::intptr_t>(name.c_str()), reinterpret_cast<std::intptr_t>(relationship), reinterpret_cast<std::intptr_t>(code.c_str()), 0, 0);
      break;
    }
    default:
      break;
    }

    return 0;
  }

  if (message == 34) {
    this->saved = reinterpret_cast<PartyBoardPlacement*>(a);

    if (this->saved->width <= 0 || this->saved->height <= 0 || this->saved->x < 0 || this->saved->x > g_renderer->viewport.width - this->saved->width ||
        this->saved->y < 0 || this->saved->y > g_renderer->viewport.height - this->saved->height) {
      *this->saved = {200, 200, 220, 190};
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
