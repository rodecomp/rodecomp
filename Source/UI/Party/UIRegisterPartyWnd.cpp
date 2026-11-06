#include "UI/Window/UIWindowMgr.h"
#include "Client/Input/InputCharset.h"
#include "Contents/MessageTable.h"
#include "Resource/TextureNames.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Client/Session.h"
#include "Contents/ForbiddenWords.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/TextFitting.h"
#include "Utility/Text/FormatString.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Buttons/UIJobSlotButton.h"
#include "UI/Party/UIRegisterPartyWnd.h"
#include "UI/Text/UIStaticText.h"
#include "Render/Renderer.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>

namespace {
  constexpr char prefix[] = "유저인터페이스\\";

  // OFFSET: 0x007BBFC0
  // FUNCTION: fn_09295
  bool Digits(std::string text) {
    if (text.empty()) {
      return false;
    }

    for (char value : text) {
      if (!std::isdigit(value)) {
        return false;
      }
    }

    return true;
  }
}

// OFFSET: 0x007BAF30
// FUNCTION: fn_09278
UIRegisterPartyWnd::UIRegisterPartyWnd()
    : UIFrameWnd(0), selectedType(0) {
}

UIRegisterPartyWnd::~UIRegisterPartyWnd() = default;

// OFFSET: 0x007C2440
// FUNCTION: fn_09326
void UIRegisterPartyWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007BDD10
// FUNCTION: fn_09300
void UIRegisterPartyWnd::Slot15(int, int) {
  const auto label = [&](const char* text, int x, int y, int width) {
    auto* value = new UIStaticText();
    value->SetSize(width, 15);
    value->Move(x, y);
    value->SetText(text, 0);
    value->red = 0;
    value->green = 0;
    value->blue = 0;
    this->AddChild(value);

    return value;
  };
  label(LookupMessage(3415), 10, 23, 40);
  this->character = label(g_session.DecodePlayerName(), 60, 23, 119);
  this->character->text = FitText(*this->character->geometry.surface, std::string(g_session.DecodePlayerName()), 119);
  this->character->tooltipEnabled = this->character->text != g_session.DecodePlayerName();
  label(LookupMessage(408), 10, 49, 40);
  const int level = g_session.m_level;

  const auto edit = [&](int x, int value) {
    auto* control = new UIEditCtrl();
    control->SetSize(25, 16);
    control->Move(x, 47);
    control->invalidatesParent = 1;
    control->maximum = 3;
    control->Background(232, 232, 232);
    char number[10] = {};
    _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", value);
    control->SetText(number);
    this->AddChild(control);

    return control;
  };
  this->minimum = edit(60, std::max((level - 5), 1));
  this->maximum = edit(96, (level + 5));
  label(LookupMessage(3369), 10, 73, this->geometry.width);
  auto* roleLabel = label(LookupMessage(504), 10, 97, this->geometry.width);
  roleLabel->outline = 1;
  const char* roles[] = {"def", "heal", "atk", "buf"};
  const int tooltips[] = {3420, 3491, 3444, 3508};

  for (int index = 0; index < 4; ++index) {
    auto* job = new UIJobSlotButton();
    this->jobs[index] = job;
    job->SetSize(16, 16);
    job->Move(60 + index * 18, 95);
    job->selectedImage = std::string(prefix) + "UIadventurerAgency\\btn_job_" + roles[index] + "_on.bmp";
    job->unselectedImage = std::string(prefix) + "UIadventurerAgency\\btn_job_" + roles[index] + "_off.bmp";
    job->SetSelected(1);
    job->SetTooltip(LookupMessage(tooltips[index]));
    this->AddChild(job);
  }

  label(LookupMessage(3359), 10, 121, this->geometry.width - 32);
  this->memo = new UIEditCtrl();
  this->memo->SetSize(139, 14);
  this->memo->maximum = 24;
  this->memo->TextColor(0, 0xFFFFFFFFu);
  this->memo->Move(60, 119);
  this->memo->Background(232, 232, 232);
  this->AddChild(this->memo);

  auto* submit = new UIBasicButton();
  const bool registered = g_adventurerAgencyMgr->registered;
  const char* title = LookupMessage(registered ? 3376 : 3371);
  submit->Configure(this->TextWidth(title, 0, 0, 12, 0, 0) + 10, title);
  submit->Move(108, 165);
  submit->SetId(registered ? 564 : 565);

  if (registered) {
    this->change = submit;
  } else {
    this->recruit = submit;
  }

  this->m_message0 = registered ? 564 : 565;
  this->AddChild(submit);
  std::vector<std::string> lines;
  WrapPlainText(LookupMessage(3370), lines, 40);
  int y = 136;

  for (const auto& line : lines) {
    auto* value = label(line.c_str(), 9, y, this->geometry.width - 18);
    value->fontHeight = 11;
    value->red = 255;
    y += 12;
  }

  auto* close = new UIBitmapButton();
  const char* suffixes[] = {"", "_a", "_b"};

  for (int state = 0; state < 3; ++state) {
    const std::string path = std::string(prefix) + "basic_interface\\btn_close" + suffixes[state] + ".bmp";
    close->SetBitmap(path.c_str(), state, 0);
  }

  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(173, 165);
  close->SetId(201);
  this->AddChild(close);

  this->types = new UIComboBoxDef2Wnd();
  this->types->dropdownHeight = 126;
  this->types->SetSize(63, 18);
  this->types->Move(60, 70);
  this->AddChild(this->types);
  this->types->AddOption(LookupMessage(3377), 0);

  for (int id = 3360; id <= 3365; ++id) {
    this->types->AddOption(LookupMessage(id), 0);
  }

  this->types->Select(0);
  auto* titleClose = new UIBitmapButton();
  const std::string off = std::string(prefix) + "basic_interface\\big_sys_close_off.bmp";
  const std::string on = std::string(prefix) + "basic_interface\\big_sys_close_on.bmp";
  titleClose->SetBitmap(off.c_str(), 0, 0);
  titleClose->SetBitmap(on.c_str(), 1, 0);
  titleClose->SetBitmap(on.c_str(), 2, 0);
  titleClose->SetSize(titleClose->BitmapWidth(), titleClose->BitmapHeight());
  titleClose->Move(this->geometry.width - titleClose->BitmapWidth(), 0);
  titleClose->SetId(201);
  titleClose->SetTooltip("");
  this->AddChild(titleClose);
}

// OFFSET: 0x007C1FB0
// FUNCTION: fn_09321
void UIRegisterPartyWnd::SetInfo(CAdvenPartyInfo value) {
  this->jobs[0]->SetSelected(value.tanker);
  this->jobs[1]->SetSelected(value.healer);
  this->jobs[2]->SetSelected(value.dealer);
  this->jobs[3]->SetSelected(value.assist);
  char number[10] = {};
  StringFormat(number, sizeof(number), _TRUNCATE, "%d", value.minimumLevel);
  this->minimum->SetText(number);
  StringFormat(number, sizeof(number), _TRUNCATE, "%d", value.maximumLevel);
  this->maximum->SetText(number);
  this->selectedType = value.type;
  this->types->Select(value.type);
  this->memo->SetText(value.memo.c_str());
  this->character->SetText(value.characterName.c_str(), 0);
  this->Invalidate();
}

// OFFSET: 0x007BBA80
// FUNCTION: fn_09290
void UIRegisterPartyWnd::NormalizeLevels() {
  const int level = g_session.m_level;
  std::string text = this->minimum->Text();

  if (!Digits(text)) {
    char number[10] = {};
    StringFormat(number, sizeof(number), _TRUNCATE, "%d", level - 5 > 1 ? level - 5 : 1);
    this->minimum->SetText(number);
  }

  text = this->maximum->Text();

  if (!Digits(text)) {
    char number[10] = {};
    StringFormat(number, sizeof(number), _TRUNCATE, "%d", level + 5);
    this->maximum->SetText(number);
  }
}

// OFFSET: 0x007BFBB0
// FUNCTION: fn_09305
void UIRegisterPartyWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIadventurerAgency\\bg_agency_register.bmp"))), false);
  this->DrawTextAt(86, 50, "~", 0, 0, 12, 0, 0, 0);
  this->DrawTitleBar(true, LookupMessage(3356), 0);
}

// OFFSET: 0x007C1410
// FUNCTION: fn_09314
int UIRegisterPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    switch (a) {
    case 201:
      break;
    case 564:
    case 565:
      if (g_session.unknown_5464 != 0) {
        if (g_forbiddenWords.Contains(this->memo->Text()) == true) {
          char text[256] = {};
          StringFormat(text, sizeof(text), _TRUNCATE, LookupMessage(3372));
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
          return 0;
        }

        this->NormalizeLevels();

        if (std::atoi(this->minimum->Text()) > std::atoi(this->maximum->Text())) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3516)), 0xFF, 0, 0, 0);
          return 0;
        }

        CAdvenPartyInfo info;
        info.tanker = this->jobs[0]->selected;
        info.healer = this->jobs[1]->selected;
        info.dealer = this->jobs[2]->selected;
        info.assist = this->jobs[3]->selected;
        info.minimumLevel = std::atoi(this->minimum->Text());
        info.maximumLevel = std::atoi(this->maximum->Text());
        info.type = this->selectedType;
        info.memo = this->memo->Text();
        info.characterName = g_session.DecodePlayerName();
        g_adventurerAgencyMgr->RequestRegistration(info);
      } else {
        char text[256] = {};
        StringFormat(text, sizeof(text), _TRUNCATE, LookupMessage(3506));
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      }
      break;
    default:
      return 0;
    }

    g_windowMgr.CloseWindow(323);
    return 0;
  }

  if (message == 34) {
    this->saved = reinterpret_cast<RegisterPartyWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  if (message == 39) {
    if (sender == this->types) {
      this->selectedType = static_cast<int>(a);
    }

    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
