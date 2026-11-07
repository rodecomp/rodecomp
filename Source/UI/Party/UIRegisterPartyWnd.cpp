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
  // OFFSET: 0x007BBFC0
  // FUNCTION: fn_09295
  bool Digits(std::string text) {
    return !text.empty() && std::find_if(text.begin(), text.end(), [](char value) { return !std::isdigit(value); }) == text.end();
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
  auto* title = new UIStaticText();
  title->SetSize(40, 15);
  title->Move(10, 23);
  title->SetText(LookupMessage(3415), 0);
  title->SetColor(0, 0, 0);
  this->AddChild(title);

  this->character = new UIStaticText();
  this->character->SetSize(119, 15);
  this->character->Move(60, 23);
  this->character->SetTextFitted(g_session.DecodePlayerName());
  this->character->SetColor(0, 0, 0);
  this->AddChild(this->character);

  auto* levelLabel = new UIStaticText();
  levelLabel->SetSize(40, 15);
  levelLabel->Move(10, 49);
  levelLabel->SetText(LookupMessage(408), 0);
  levelLabel->SetColor(0, 0, 0);
  this->AddChild(levelLabel);
  const int level = g_session.m_level;

  this->minimum = new UIEditCtrl();
  this->minimum->SetSize(25, 16);
  this->minimum->Move(60, 47);
  this->minimum->invalidatesParent = 1;
  this->minimum->maximum = 3;
  this->minimum->Background(232, 232, 232);
  char number[10] = {};
  StringFormat(number, sizeof(number), _TRUNCATE, "%d", std::max((level - 5), 1));
  this->minimum->SetText(number);
  this->AddChild(this->minimum);

  this->maximum = new UIEditCtrl();
  this->maximum->SetSize(25, 16);
  this->maximum->Move(96, 47);
  this->maximum->invalidatesParent = 1;
  this->maximum->maximum = 3;
  this->maximum->Background(232, 232, 232);
  StringFormat(number, sizeof(number), _TRUNCATE, "%d", level + 5);
  this->maximum->SetText(number);
  this->AddChild(this->maximum);

  auto* typeLabel = new UIStaticText();
  typeLabel->SetSize(this->geometry.width, 15);
  typeLabel->Move(10, 73);
  typeLabel->SetText(LookupMessage(3369), 0);
  typeLabel->SetColor(0, 0, 0);
  this->AddChild(typeLabel);
  auto* roleLabel = new UIStaticText();
  roleLabel->SetSize(this->geometry.width, 15);
  roleLabel->Move(10, 97);
  roleLabel->SetText(LookupMessage(504), 0);
  roleLabel->SetColor(0, 0, 0);
  roleLabel->shadow = 1;
  this->AddChild(roleLabel);

  const int positions[4][2] = {{60, 95}, {78, 95}, {96, 95}, {114, 95}};
  const char* images[] = {"btn_job_def_on", "btn_job_def_off", "btn_job_heal_on", "btn_job_heal_off",
                          "btn_job_atk_on", "btn_job_atk_off", "btn_job_buf_on", "btn_job_buf_off"};
  const std::string path = "유저인터페이스\\UIadventurerAgency\\";
  const char* tooltips[] = {LookupMessage(3420), LookupMessage(3491), LookupMessage(3444), LookupMessage(3508)};

  for (int index = 0; index < 4; ++index) {
    this->jobs[index] = new UIJobSlotButton();
    this->jobs[index]->SetSize(16, 16);
    this->jobs[index]->Move(positions[index][0], positions[index][1]);
    this->jobs[index]->selectedImage = static_cast<const std::string&>(path + images[index * 2] + ".bmp");
    this->jobs[index]->unselectedImage = static_cast<const std::string&>(path + images[index * 2 + 1] + ".bmp");
    this->jobs[index]->SetSelected(1);
    this->jobs[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->jobs[index]);
  }

  auto* memoLabel = new UIStaticText();
  memoLabel->SetSize(this->geometry.width - 32, 15);
  memoLabel->Move(10, 121);
  memoLabel->SetText(LookupMessage(3359), 0);
  memoLabel->SetColor(0, 0, 0);
  this->AddChild(memoLabel);
  this->memo = new UIEditCtrl();
  this->memo->SetSize(139, 14);
  this->memo->maximum = 24;
  this->memo->TextColor(0, 0, 0, 0xFFFFFFFFu);
  this->memo->Move(60, 119);
  this->memo->Background(232, 232, 232);
  this->AddChild(this->memo);

  if (g_adventurerAgencyMgr->registered == true) {
    this->change = new UIBasicButton();
    this->change->Configure(this->TextWidth(LookupMessage(3376), 0, 0, 12, 0, 0) + 10, LookupMessage(3376));
    this->change->Move(108, 165);
    this->change->SetId(564);
    this->m_message0 = 564;
    this->AddChild(this->change);
  } else {
    this->recruit = new UIBasicButton();
    this->recruit->Configure(this->TextWidth(LookupMessage(3371), 0, 0, 12, 0, 0) + 10, LookupMessage(3371));
    this->recruit->Move(108, 165);
    this->recruit->SetId(565);
    this->m_message0 = 565;
    this->AddChild(this->recruit);
  }

  std::vector<std::string> lines;
  WrapPlainText(LookupMessage(3370), lines, 40);
  int y = 136;

  for (const auto& line : lines) {
    auto* value = new UIStaticText();
    value->SetSize(this->geometry.width - 18, 15);
    value->Move(9, y);
    value->SetText(line.c_str(), 0);
    value->SetFont(0, 11, 0, 0);
    value->SetColor(255, 0, 0);
    this->AddChild(value);
    y += 12;
  }

  auto* close = new UIBitmapButton();
  close->SetBitmap("유저인터페이스\\basic_interface\\btn_close.bmp", 0, 0);
  close->SetBitmap("유저인터페이스\\basic_interface\\btn_close_a.bmp", 1, 0);
  close->SetBitmap("유저인터페이스\\basic_interface\\btn_close_b.bmp", 2, 0);
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

  this->types->AddOption(LookupMessage(3360), 0);
  this->types->AddOption(LookupMessage(3361), 0);
  this->types->AddOption(LookupMessage(3362), 0);
  this->types->AddOption(LookupMessage(3363), 0);
  this->types->AddOption(LookupMessage(3364), 0);
  this->types->AddOption(LookupMessage(3365), 0);

  this->types->Select(0);
  this->AddBigCloseButton("", 0);
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
  switch (message) {
  case 39:
    if (sender == this->types) {
      this->selectedType = static_cast<int>(a);
    }

    return 0;
  case 34:
    this->saved = reinterpret_cast<RegisterPartyWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  case 6:
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

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
