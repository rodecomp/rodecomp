#include "Contents/Tables/MessageTable.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Language.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "File/ShortcutFilename.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Window/InputFocus.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Party/UIJobSlotButton.h"
#include "UI/Party/UIPartyInfoWnd.h"
#include "UI/Party/UIRegisterPartyWnd.h"
#include "UI/Text/UIStaticText.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/WindowManagerWindows.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

template std::string std::operator+(std::string&&, const char* const);

namespace {
  // OFFSET: 0x007BBFC0
  // FUNCTION: fn_09295
  bool Digits(std::string text) {
    return !text.empty() && std::find_if(text.begin(), text.end(), [](char value) { return !std::isdigit(value); }) == text.end();
  }

  // INLINED
  std::string TextValue(UIEditCtrl* control) {
    return control->Text();
  }
}

// OFFSET: 0x007BAF30
// FUNCTION: fn_09278
UIRegisterPartyWnd::UIRegisterPartyWnd()
    : UIFrameWnd(0), selectedType(0) {
}

// OFFSET: 0x007BB420
// FUNCTION: fn_45802
__declspec(noinline) UIRegisterPartyWnd::~UIRegisterPartyWnd() = default;

// OFFSET: 0x007C2440
// FUNCTION: fn_09326
void UIRegisterPartyWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007BDD10
// FUNCTION: fn_09300
void UIRegisterPartyWnd::OnCreate(int, int) {
  auto* title = new UIStaticText();
  title->SetSize(40, 15);
  title->Move(10, 23);
  title->SetText(LookupMessage(MSI_PARTY_MASTER), 0);
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
  levelLabel->SetText(LookupMessage(MSI_LEVEL), 0);
  levelLabel->SetColor(0, 0, 0);
  this->AddChild(levelLabel);
  const int level = g_session.m_level;

  this->minimum = new UIEditCtrl2();
  this->minimum->SetSize(25, 16);
  this->minimum->Move(60, 47);
  this->minimum->invalidatesParent = 1;
  this->minimum->maximum = 3;
  this->minimum->Background(232, 232, 232);
  char number[10] = {};
  StringFormat(number, sizeof(number), _TRUNCATE, "%d", level - 5 > 1 ? level - 5 : 1);
  this->minimum->SetText(number);
  this->AddChild(this->minimum);

  this->maximum = new UIEditCtrl2();
  this->maximum->SetSize(25, 16);
  this->maximum->Move(96, 47);
  this->maximum->invalidatesParent = 1;
  this->maximum->maximum = 3;
  this->maximum->Background(232, 232, 232);
  char maximumNumber[10] = {};
  StringFormat(maximumNumber, sizeof(maximumNumber), _TRUNCATE, "%d", level + 5);
  this->maximum->SetText(maximumNumber);
  this->AddChild(this->maximum);

  auto* typeLabel = new UIStaticText();
  typeLabel->SetSize(this->geometry.width, 15);
  typeLabel->Move(10, 73);
  typeLabel->SetText(LookupMessage(MSI_ADVEN_PURPOSE), 0);
  typeLabel->SetColor(0, 0, 0);
  this->AddChild(typeLabel);
  auto* roleLabel = new UIStaticText();
  roleLabel->SetSize(this->geometry.width, 15);
  roleLabel->Move(10, 97);
  roleLabel->SetText(LookupMessage(MSI_JOB), 0);
  roleLabel->SetColor(0, 0, 0);
  roleLabel->shadow = 1;
  this->AddChild(roleLabel);

  const int positions[4][2] = {{60, 95}, {78, 95}, {96, 95}, {114, 95}};
  const char* images[] = {"btn_job_def_on", "btn_job_def_off", "btn_job_heal_on", "btn_job_heal_off",
                          "btn_job_atk_on", "btn_job_atk_off", "btn_job_buf_on", "btn_job_buf_off"};
  const std::string path = "유저인터페이스\\UIadventurerAgency\\";
  const char* tooltips[] = {LookupMessage(MSI_ADVEN_TANKER), LookupMessage(MSI_ADVEN_HEALER), LookupMessage(MSI_ADVEN_DEARER), LookupMessage(MSI_ADVEN_ASSIST)};

  for (int index = 0; index < 4; ++index) {
    this->jobs[index] = new UIJobSlotButton();
    this->jobs[index]->SetSize(16, 16);
    this->jobs[index]->Move(positions[index][0], positions[index][1]);
    this->jobs[index]->SetSelectedImage(path + images[index * 2] + ".bmp");
    this->jobs[index]->SetUnselectedImage(path + images[index * 2 + 1] + ".bmp");
    this->jobs[index]->SetSelected(1);
    this->jobs[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->jobs[index]);
  }

  auto* memoLabel = new UIStaticText();
  memoLabel->SetSize(this->geometry.width - 32, 15);
  memoLabel->Move(10, 121);
  memoLabel->SetText(LookupMessage(MSI_ADVEN_MEMO), 0);
  memoLabel->SetColor(0, 0, 0);
  this->AddChild(memoLabel);
  this->memo = new UIEditCtrl2();
  this->memo->SetSize(139, 14);
  this->memo->maximum = 24;
  this->memo->TextColor(0, 0, 0, 0xFFFFFFFFu);
  this->memo->Move(60, 119);
  this->memo->Background(232, 232, 232);
  this->AddChild(this->memo);

  if (g_adventurerAgencyMgr->registered == true) {
    this->change = new UIBasicButton();
    this->change->Configure(this->TextWidth(LookupMessage(MSI_ADVEN_REWRITE), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_ADVEN_REWRITE));
    this->change->Move(108, 165);
    this->change->SetId(ID_EDIT_OWN_PARTY_LISTING);
    this->m_message0 = ID_EDIT_OWN_PARTY_LISTING;
    this->AddChild(this->change);
  } else {
    this->recruit = new UIBasicButton();
    this->recruit->Configure(this->TextWidth(LookupMessage(MSI_ADVEN_REGISTER), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_ADVEN_REGISTER));
    this->recruit->Move(108, 165);
    this->recruit->SetId(ID_REGISTER_PARTY_LISTING);
    this->m_message0 = ID_REGISTER_PARTY_LISTING;
    this->AddChild(this->recruit);
  }

  std::vector<std::string> lines;
  WrapPlainText(LookupMessage(MSI_ADVEN_WARNING_TEXT), lines, 40);

  for (unsigned index = 0; index < lines.size(); ++index) {
    auto* value = new UIStaticText();
    value->SetSize(this->geometry.width - 18, 15);
    value->Move(9, 136 + index * 12);
    value->SetText(lines[index].c_str(), 0);
    value->SetFont(0, 11, 0, 0);
    value->SetColor(255, 0, 0);
    this->AddChild(value);
  }

  auto* close = new UIBitmapButton();
  close->SetBitmap("유저인터페이스\\basic_interface\\btn_close.bmp", 0, 0);
  close->SetBitmap("유저인터페이스\\basic_interface\\btn_close_a.bmp", 1, 0);
  close->SetBitmap("유저인터페이스\\basic_interface\\btn_close_b.bmp", 2, 0);
  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(173, 165);
  close->SetId(ID_CLOSE);
  this->AddChild(close);

  this->types = new UIComboBoxDef2Wnd();
  this->types->dropdownHeight = 126;
  this->types->SetSize(63, 18);
  this->types->Move(60, 70);
  this->AddChild(this->types);
  this->types->AddOption(LookupMessage(MSI_ALL_AREA), 0);

  this->types->AddOption(LookupMessage(MSI_ADVEN_QUEST), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_FIELD), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_DUNJUN), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_MD), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_EDEN_GROUP), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_ETC), 0);

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
  this->DrawTitleBar(true, LookupMessage(MSI_REGISTER_ADVENTURER_AGENCY), 0);
}

// OFFSET: 0x007C0250
// FUNCTION: fn_45798
__forceinline void UIRegisterPartyWnd::RequestRegistration() {
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
}

// OFFSET: 0x007C1410
// FUNCTION: fn_09314
int UIRegisterPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMBOBOX_SELECTED:
    if (sender == this->types) {
      this->selectedType = static_cast<int>(a);
    }

    return 0;
  case UM_SETINFO:
    this->saved = reinterpret_cast<RegisterPartyWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      break;
    case ID_EDIT_OWN_PARTY_LISTING:
    case ID_REGISTER_PARTY_LISTING:
      if (g_session.partyLeader == 0) {
        char text[256] = {};
        StringFormat(text, sizeof(text), _TRUNCATE, LookupMessage(MSI_NOT_PARTYMASTER_CANNOT_REGISTER_ADVENTURER_AGENCY));
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      } else {
        if (g_forbiddenWords.Contains(this->memo->Text()) == true) {
          char text[256] = {};
          StringFormat(text, sizeof(text), _TRUNCATE, LookupMessage(MSI_INCLUDE_BAD_WORD));
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
          return 0;
        }

        this->NormalizeLevels();

        if (std::atoi(this->minimum->Text()) > std::atoi(this->maximum->Text())) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CHECK_LEVEL_BEFORE_REGISTER_AGENCY)), 0xFF, 0, 0, 0);
          return 0;
        }

        this->RequestRegistration();
      }
      break;
    default:
      return 0;
    }

    g_windowMgr.CloseWindow(WID_REGISTERPARTYWND);
    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x007BAD00
// FUNCTION: fn_09275
UIApplyForPartyWnd::UIApplyForPartyWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x007BB280
// FUNCTION: fn_45801
__declspec(noinline) UIApplyForPartyWnd::~UIApplyForPartyWnd() = default;

// OFFSET: 0x007C2400
// FUNCTION: fn_09325
void UIApplyForPartyWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007BD970
// FUNCTION: fn_09299
void UIApplyForPartyWnd::OnCreate(int, int) {
  UIBasicButton* button = new UIBasicButton();
  button->Configure(this->TextWidth(LookupMessage(MSI_CHAT_WITH_PARTYMASTER), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_CHAT_WITH_PARTYMASTER));
  button->Move(51, 165);
  button->SetId(ID_WHISPER_PARTY_LEADER);
  this->AddChild(button);

  button = new UIBasicButton();
  button->Configure(this->TextWidth(LookupMessage(MSI_ASK_JOIN), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_ASK_JOIN));
  button->Move(108, 165);
  button->SetId(ID_APPLY);
  this->AddChild(button);

  button = new UIBasicButton();
  button->Configure(this->TextWidth(LookupMessage(MSI_CLOSE), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_CLOSE));
  button->Move(173, 165);
  button->SetId(ID_CLOSE);
  this->AddChild(button);

  std::vector<std::string> lines;
  WrapPlainText(LookupMessage(MSI_ADVEN_WARNING_TEXT), lines, 40);

  for (unsigned index = 0; index < lines.size(); ++index) {
    auto* label = new UIStaticText();
    label->SetSize(this->geometry.width - 18, 15);
    label->Move(9, 136 + index * 12);
    label->SetText(lines[index].c_str(), 0);
    label->SetFont(0, 11, 0, 0);
    label->SetColor(255, 0, 0);
    this->AddChild(label);
  }

  this->AddBigCloseButton("", 0);
}

// OFFSET: 0x007C1D10
// FUNCTION: fn_09319
void UIApplyForPartyWnd::SetInfo(CAdvenPartyInfo value) {
  this->info.tanker = value.tanker;
  this->info.healer = value.healer;
  this->info.dealer = value.dealer;
  this->info.assist = value.assist;
  this->info.minimumLevel = value.minimumLevel;
  this->info.maximumLevel = value.maximumLevel;
  this->info.type = value.type;
  this->info.memo = value.memo;
  this->info.characterName = value.characterName;
  this->info.accountId = value.accountId;
  this->info.characterId = value.characterId;
  this->Invalidate();
}

// OFFSET: 0x007BF5A0
// FUNCTION: fn_09303
void UIApplyForPartyWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIadventurerAgency\\bg_agency_register.bmp"))), false);
  this->DrawTitleBar(true, LookupMessage(MSI_ADVEN_JOIN_PARTY), 0);
  this->DrawTextAt(10, 25, LookupMessage(MSI_PARTY_MASTER), 0, 0, 12, 0, 0, 0);
  const std::string fitted = this->FitTextToWidth(this->info.characterName, 119);
  this->DrawTextAt(60, 25, fitted.c_str(), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(10, 49, LookupMessage(MSI_LEVEL), 0, 0, 12, 0, 0, 0);
  char levels[10] = {};
  StringFormat(levels, sizeof(levels), _TRUNCATE, "%d~%d", this->info.minimumLevel, this->info.maximumLevel);
  this->DrawTextAt(60, 49, levels, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(10, 73, LookupMessage(MSI_ADVEN_PURPOSE), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(60, 73, UIPartyInfoWnd::TypeName(this->info.type), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(10, 97, LookupMessage(MSI_JOB), 0, 0, 12, 0, 0, 0);
  this->DrawBitmap(60, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.tanker != true ? "유저인터페이스\\UIadventurerAgency\\btn_job_def_off.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_def_on.bmp"))), false);
  this->DrawBitmap(78, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.healer != true ? "유저인터페이스\\UIadventurerAgency\\btn_job_heal_off.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_heal_on.bmp"))), false);
  this->DrawBitmap(96, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.dealer != true ? "유저인터페이스\\UIadventurerAgency\\btn_job_atk_off.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_atk_on.bmp"))), false);
  this->DrawBitmap(114, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.assist != true ? "유저인터페이스\\UIadventurerAgency\\btn_job_buf_off.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_buf_on.bmp"))), false);
  this->DrawTextAt(10, 121, LookupMessage(MSI_ADVEN_MEMO), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(60, 121, this->info.memo.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x007BFCC0
// FUNCTION: fn_09309
void UIApplyForPartyWnd::OnMouseHover(int x, int y) {
  if (y < 95 || y > 111) {
    return;
  }

  std::string label;
  unsigned int left = static_cast<unsigned int>(x);
  const auto between = [&](unsigned int first, unsigned int last) {
    return left > first && left < last;
  };

  if (between(60, 76)) {
    left = 60;
    label = LookupMessage(MSI_ADVEN_TANKER);
  } else if (between(78, 94)) {
    left = 78;
    label = LookupMessage(MSI_ADVEN_HEALER);
  } else if (between(96, 112)) {
    left = 96;
    label = LookupMessage(MSI_ADVEN_DEARER);
  } else if (between(114, 130)) {
    left = 114;
    label = LookupMessage(MSI_ADVEN_ASSIST);
  }

  if (!label.empty()) {
    int screenX = static_cast<int>(left);
    int screenY = 75;
    this->ToScreen(screenX, screenY);
    g_modeMgr.currentMode->ShowTooltip(label.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x007C11E0
// FUNCTION: fn_09313
int UIApplyForPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (static_cast<int>(a)) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
      break;
    case ID_APPLY:
      g_adventurerAgencyMgr->RequestInfo(this->info.accountId);
      break;
    case ID_WHISPER_PARTY_LEADER: {
      const std::string name = this->info.characterName;
      const char* relationship = nullptr;

      if (g_windowMgr.IsFriend(name.c_str())) {
        relationship = LookupMessage(MSI_FRIEND);
      }

      const char* group = g_session.IsGuildMemberAccount(this->info.accountId) ? "GuildMember" : relationship;
      std::string code("");
      code = g_session.ShortcutFilename(this->info.characterId);
      g_windowMgr.SendNotice(UIM_MAKE_WHISPER_WINDOW, reinterpret_cast<std::intptr_t>(name.c_str()), reinterpret_cast<std::intptr_t>(group), reinterpret_cast<std::intptr_t>(code.c_str()), 0, 0);
      break;
    }
    }

    return 0;
  case UM_SETINFO:
    this->saved = reinterpret_cast<ApplyForPartyWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

namespace {
  // INLINED
  void DisableButton(UIButton* button, unsigned char value) {
    button->SetDisabled(value);
    button->Invalidate();
  }
}

// OFFSET: 0x007BAC40
// FUNCTION: fn_09274
UIAdvenPartyBoardWnd::UIAdvenPartyBoardWnd()
    : UIFrameWnd(0), unknown_0104(0), unknown_0108(0), page(1), totalPages(1), unknown_0114(1), unknown_0118(0), selectedType(0) {
}

// OFFSET: 0x007BB220
// FUNCTION: fn_45800
__declspec(noinline) UIAdvenPartyBoardWnd::~UIAdvenPartyBoardWnd() = default;

// OFFSET: 0x007C23C0
// FUNCTION: fn_09324
void UIAdvenPartyBoardWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007BC220
// FUNCTION: fn_09298
void UIAdvenPartyBoardWnd::OnCreate(int, int) {
  const int level = g_session.m_level;
  this->minimum = new UIEditCtrl2();
  this->minimum->SetSize(25, 14);
  this->minimum->invalidatesParent = 1;
  this->minimum->maximum = 3;
  this->minimum->TextColor(0, 0, 0, 0xFFFFFFFFu);
  char minimumText[10] = {};
  StringFormat(minimumText, sizeof(minimumText), _TRUNCATE, "%d", level - 5 > 1 ? level - 5 : 1);
  this->minimum->SetText(minimumText);
  this->minimum->Move(10, 24);
  this->minimum->Background(242, 242, 242);
  this->minimum->Show(WINDOW_VISIBLE);
  this->AddChild(this->minimum);

  this->maximum = new UIEditCtrl2();
  this->maximum->SetSize(25, 14);
  this->maximum->invalidatesParent = 1;
  this->maximum->maximum = 3;
  this->maximum->TextColor(0, 0, 0, 0xFFFFFFFFu);
  char maximumText[10] = {};
  StringFormat(maximumText, sizeof(maximumText), _TRUNCATE, "%d", level + 5);
  this->maximum->SetText(maximumText);
  this->maximum->Move(46, 24);
  this->maximum->Background(242, 242, 242);
  this->maximum->Show(WINDOW_VISIBLE);
  this->AddChild(this->maximum);

  const int positions[][2] = {{160, 23}, {178, 23}, {196, 23}, {214, 23}};
  const char* images[] = {"btn_job_def_on", "btn_job_def_off", "btn_job_heal_on", "btn_job_heal_off", "btn_job_atk_on", "btn_job_atk_off", "btn_job_buf_on", "btn_job_buf_off"};
  const std::string path = "유저인터페이스\\UIadventurerAgency\\";
  const char* tooltips[] = {LookupMessage(MSI_ADVEN_TANKER), LookupMessage(MSI_ADVEN_HEALER), LookupMessage(MSI_ADVEN_DEARER), LookupMessage(MSI_ADVEN_ASSIST)};

  for (int index = 0; index < 4; ++index) {
    this->jobs[index] = new UIJobSlotButton();
    this->jobs[index]->SetSize(16, 16);
    this->jobs[index]->Move(positions[index][0], positions[index][1]);
    this->jobs[index]->SetSelectedImage(path + images[index * 2] + ".bmp");
    this->jobs[index]->SetUnselectedImage(path + images[index * 2 + 1] + ".bmp");
    this->jobs[index]->SetSelected(0);
    this->jobs[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->jobs[index]);
  }

  this->memo = new UIEditCtrl5();
  this->memo->SetSize(116, 14);
  this->memo->maximum = 150;
  this->memo->TextColor(0, 0, 0, 0xFFFFFFFFu);
  this->memo->SetPlaceholderEnabled(true);
  this->memo->SetPlaceholder(LookupMessage(MSI_INPUT_SEARCH_TEXT));
  this->memo->SetPlaceholderColor(0x646464);
  this->memo->Move(238, 24);
  this->memo->Background(255, 255, 255);
  this->AddChild(this->memo);

  auto* search = new UIBmpTextButton();
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search.bmp", 0, 0);
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search_over.bmp", 1, 0);
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search_press.bmp", 2, 0);
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search_disable.bmp", 3, 0);
  search->SetSize(search->BitmapWidth(), search->BitmapHeight());
  search->Move(354, 22);
  search->SetId(ID_FIND);
  search->SetText(LookupMessage(MSI_ADVEN_SEARCH));
  this->AddChild(search);
  auto* reset = new UIBmpTextButton();
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize.bmp", 0, 0);
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize_over.bmp", 1, 0);
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize_press.bmp", 2, 0);
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize_disable.bmp", 3, 0);
  reset->SetSize(reset->BitmapWidth(), reset->BitmapHeight());
  reset->Move(401, 22);
  reset->SetId(ID_RESET);
  reset->SetText(LookupMessage(MSI_ADVEN_RESET));
  this->AddChild(reset);
  this->refresh = new UIBitmapButton();
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh.bmp", 0, 0);
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh_over.bmp", 1, 0);
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh_press.bmp", 2, 0);
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh_disable.bmp", 3, 0);
  this->refresh->SetSize(this->refresh->BitmapWidth(), this->refresh->BitmapHeight());
  this->refresh->Move(452, 22);
  this->refresh->SetId(ID_REFRESH);
  this->AddChild(this->refresh);

  this->previous = new UIBitmapButton();
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left.bmp", 0, 0);
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left.bmp", 1, 0);
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left.bmp", 2, 0);
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left_disable.bmp", 3, 0);
  this->previous->SetSize(this->previous->BitmapWidth(), this->previous->BitmapHeight());
  this->previous->Move(199, 319);
  this->previous->SetId(ID_PREV);
  this->AddChild(this->previous);
  this->next = new UIBitmapButton();
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right.bmp", 0, 0);
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right.bmp", 1, 0);
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right.bmp", 2, 0);
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right_disable.bmp", 3, 0);
  this->next->SetSize(this->next->BitmapWidth(), this->next->BitmapHeight());
  this->next->Move(265, 319);
  this->next->SetId(ID_NEXT);
  this->AddChild(this->next);

  const std::vector<CAdvenPartyInfo> parties = g_adventurerAgencyMgr->Entries();
  for (int index = 0; index < 10; ++index) {
    this->rows[index] = new UIPartyInfoWnd();
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list.bmp", 0, 0);
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_over.bmp", 1, 0);
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_press.bmp", 2, 0);
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_disable.bmp", 3, 0);
    this->rows[index]->SetSize(this->rows[index]->BitmapWidth(), this->rows[index]->BitmapHeight());
    this->rows[index]->Move(1, 73 + index * 24);
    DisableButton(this->rows[index], 1);
    this->rows[index]->SetId(ID_SELECT_PARTY_LISTING);
    this->rows[index]->m_argument = index;
    this->rows[index]->m_invalidateParent = 1;
    this->AddChild(this->rows[index]);
  }

  char pages[1024] = {};
  StringFormat(pages, sizeof(pages), _TRUNCATE, "%d / %d", this->page, this->totalPages);

  this->ownParty = new UIPartyInfoWnd();
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_on.bmp", 0, 0);
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_on.bmp", 1, 0);
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruiting_press.bmp", 2, 0);
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruiting_disable.bmp", 3, 0);
  this->ownParty->SetSize(this->ownParty->BitmapWidth(), this->ownParty->BitmapHeight());
  this->ownParty->Move(1, 49);
  this->ownParty->SetId(ID_EDIT_OWN_PARTY_LISTING);
  this->ownParty->Enable(0);
  this->AddChild(this->ownParty);
  this->recruit = new UIBmpTextButton();
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_normal.bmp", 0, 0);
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_over.bmp", 1, 0);
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_press.bmp", 2, 0);
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_disable.bmp", 3, 0);
  this->recruit->SetSize(this->recruit->BitmapWidth(), this->recruit->BitmapHeight());
  this->recruit->Move(1, 49);
  this->recruit->SetId(ID_REGISTER_PARTY_LISTING);
  this->recruit->Enable(0);
  this->AddChild(this->recruit);

  this->types = new UIComboBoxDef2Wnd();
  this->types->dropdownHeight = 126;
  this->types->SetSize(76, 18);
  this->types->Move(76, 22);
  this->AddChild(this->types);
  this->types->AddOption(LookupMessage(MSI_ALL_AREA), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_QUEST), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_FIELD), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_DUNJUN), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_MD), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_EDEN_GROUP), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_ETC), 0);

  this->types->Select(0);
  this->pageText = new UIStaticText();
  this->pageText->SetSize(this->geometry.width, 18);
  const int textWidth = this->TextWidth("1/1", 0, 0, 12, 0, 0);
  this->pageText->Resize((textWidth + 1), 16);
  this->pageText->SetText("1/1", 0);
  const int halfWidth = textWidth / 2;
  this->pageText->Move(this->geometry.width / 2 - halfWidth, 319);
  this->AddChild(this->pageText);
  this->LoadPage(1);
  this->m_message0 = ID_FIND;

  auto* close = new UIBasicButton();
  close->Configure(42, LookupMessage(MSI_CLOSE));
  close->Move(this->geometry.width - close->geometry.width - 10, 317);
  close->SetId(ID_CLOSE);
  this->AddChild(close);
  this->stop = new UIBasicButton();
  this->stop->Configure((this->TextWidth(LookupMessage(MSI_STOP_RECRUITING_PARTY), 0, 0, 12, 0, 0) + 10), LookupMessage(MSI_STOP_RECRUITING_PARTY));
  this->stop->Move(close->geometry.x - this->stop->geometry.width - 5, 317);
  this->stop->SetId(ADVENPARTY_BUTTON_STOP_RECRUIT);
  this->AddChild(this->stop);
  this->stop->Enable(0);
  this->AddBigCloseButton("", 0);
}

// OFFSET: 0x007C0050
// FUNCTION: fn_45797
__forceinline void UIAdvenPartyBoardWnd::SetPageButtonEnabled(PageButton button, int enabled) {
  switch (button) {
  case PageButton::Previous:
    if (enabled == 1) {
      DisableButton(this->previous, 0);
    } else {
      DisableButton(this->previous, 1);
    }
    this->previous->Invalidate();
    break;
  case PageButton::Next:
    if (enabled == 1) {
      DisableButton(this->next, 0);
    } else {
      DisableButton(this->next, 1);
    }
    this->next->Invalidate();
    break;
  }
}

// OFFSET: 0x007BC070
// FUNCTION: fn_09296
void UIAdvenPartyBoardWnd::LoadPage(int pageValue) {
  this->SetPageButtonEnabled(PageButton::Previous, 0);
  this->SetPageButtonEnabled(PageButton::Next, 0);
  DisableButton(this->refresh, 1);
  this->refresh->Invalidate();
  g_adventurerAgencyMgr->RequestList(pageValue);

  if (g_session.partyLeader == 1) {
    g_adventurerAgencyMgr->RequestMyParty();
  }
}

// OFFSET: 0x007C1C50
// FUNCTION: fn_09318
void UIJobSlotButton::SetSelected(bool value) {
  this->selected = value;
  this->SetState(0);

  if (value == 1) {
    this->SetBitmap(this->selectedImage.c_str(), 0, 0);
    this->SetBitmap(this->selectedImage.c_str(), 1, 0);
    this->SetBitmap(this->unselectedImage.c_str(), 2, 0);
  } else {
    this->SetBitmap(this->unselectedImage.c_str(), 0, 0);
    this->SetBitmap(this->unselectedImage.c_str(), 1, 0);
    this->SetBitmap(this->selectedImage.c_str(), 2, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x007C2120
// FUNCTION: fn_09322
void UIAdvenPartyBoardWnd::Registration(bool value) {

  if (g_session.partyLeader != 0) {
    this->recruit->SetTooltip("");

    if (value == true) {
      this->ownParty->Enable(1);
      DisableButton(this->ownParty, 0);
      this->recruit->Enable(0);
      DisableButton(this->recruit, 1);
      this->stop->Enable(1);
      DisableButton(this->stop, 0);
    } else {
      this->ownParty->Enable(0);
      DisableButton(this->ownParty, 1);
      this->recruit->Enable(1);
      DisableButton(this->recruit, 0);
      this->stop->Enable(0);
      DisableButton(this->stop, 1);
    }
  } else {
    this->recruit->SetTooltip(LookupMessage(MSI_PARTY_RECRUIT_ERROR));
    this->ownParty->Enable(0);
    DisableButton(this->ownParty, 1);
    this->recruit->Enable(1);
    DisableButton(this->recruit, 1);
    this->recruit->SetText("");
  }

  this->InvalidateChildren();
}

// OFFSET: 0x007C1BC0
// FUNCTION: fn_09317
void UIAdvenPartyBoardWnd::SetOwnParty(CAdvenPartyInfo value) {
  this->ownParty->SetInfo(value);
  this->Registration(true);
}

// OFFSET: 0x007C03A0
// FUNCTION: fn_45799
__forceinline void UIPartyInfoWnd::Reset() {
  this->m_drawLabel = 0;
  this->memo = "";
  this->levels = "";
  this->tanker = 1;
  this->dealer = 1;
  this->assist = 1;
  this->type = 0;
  this->name = "";
  this->m_disabled = 1;
  this->Invalidate();
}

// INLINED
__forceinline void UIAdvenPartyBoardWnd::RefreshRows() {
  for (int index = 0; index < 10; ++index) {
    this->rows[index]->Reset();
  }

  const auto entries = g_adventurerAgencyMgr->Entries();

  int index = 0;

  for (auto entry = entries.begin(); entry != entries.end(); ++entry) {
    if (index > 9) {
      break;
    }

    this->rows[index]->SetInfo(*entry);
    this->rows[index]->Invalidate();
    ++index;
  }

  char text[1024] = {};
  StringFormat(text, sizeof(text), _TRUNCATE, "%d / %d", this->page, this->totalPages);
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
  this->pageText->Resize((width + 1), 16);
  this->pageText->SetText(text, 0);
  const int halfWidth = width / 2;
  this->pageText->Move(this->geometry.width / 2 - halfWidth, 319);
  this->previous->Move(this->geometry.width / 2 - this->previous->geometry.width - halfWidth - 3, 319);
  this->next->Move(halfWidth + this->geometry.width / 2 + 3, 319);
  this->Registration(g_adventurerAgencyMgr->registered);
  this->SendMsg(nullptr, UM_UPDATE_PARTY_PAGINATION, 0, 0, 0, 0);
  this->Invalidate();
  this->InvalidateChildren();
}

// INLINED
void UIAdvenPartyBoardWnd::UpdatePagination() {
  if (this->page > this->totalPages) {
    this->page = this->totalPages;
  } else if (this->page != this->totalPages) {
    this->SetPageButtonEnabled(PageButton::Next, 1);
  }

  if (this->page == 1) {
    this->SetPageButtonEnabled(PageButton::Previous, 0);
  } else {
    this->SetPageButtonEnabled(PageButton::Previous, 1);
  }
  this->Invalidate();

  if (g_session.partyLeader == 0) {
    this->ownParty->SetState(3);
  }

  DisableButton(this->refresh, 0);
  this->refresh->Invalidate();
}

// INLINED
void UIAdvenPartyBoardWnd::ResetSearch() {
  const int level = g_session.m_level;
  char minimumText[10] = {};
  StringFormat(minimumText, sizeof(minimumText), _TRUNCATE, "%d", std::max(1, level - 5));
  this->minimum->SetText(minimumText);
  char maximumText[10] = {};
  StringFormat(maximumText, sizeof(maximumText), _TRUNCATE, "%d", level + 5);
  this->maximum->SetText(maximumText);
  this->types->Select(0);

  for (int index = 0; index < 4; ++index) {
    this->jobs[index]->SetState(0);
    this->jobs[index]->SetSelected(0);
  }

  this->searching = 0;
  this->memo->SetText("");
  this->InvalidateChildren();
}

// INLINED
__forceinline void UIAdvenPartyBoardWnd::Search(int trigger) {
  this->searching = 1;

  if (!Digits(TextValue(this->minimum))) {
    char number[10] = {};
    StringFormat(number, sizeof(number), _TRUNCATE, "%d", std::max(1, (g_session.m_level - 5)));
    this->minimum->SetText(number);
  }

  if (!Digits(TextValue(this->maximum))) {
    char number[10] = {};
    StringFormat(number, sizeof(number), _TRUNCATE, "%d", (g_session.m_level + 5));
    this->maximum->SetText(number);
  }

  CAdvenPartyInfo filter;
  filter.tanker = this->jobs[0]->selected;
  filter.healer = this->jobs[1]->selected;
  filter.dealer = this->jobs[2]->selected;
  filter.assist = this->jobs[3]->selected;

  if (this->minimum->Length() > 0) {
    filter.minimumLevel = std::atoi(this->minimum->Text());
  }

  if (this->minimum->Length() > 0) {
    filter.maximumLevel = std::atoi(this->maximum->Text());
  }

  if (this->selectedType != 0) {
    filter.type = this->selectedType;
  }

  filter.memo = this->memo->Text();
  filter.characterName = g_session.DecodePlayerName();

  this->SetPageButtonEnabled(PageButton::Previous, 0);
  this->SetPageButtonEnabled(PageButton::Next, 0);
  DisableButton(this->refresh, 1);
  this->refresh->Invalidate();

  if (trigger != 246 && trigger != 218) {
    this->page = 1;
  }

  g_adventurerAgencyMgr->RequestSearch(filter, this->page);
}

// OFFSET: 0x007C0420
// FUNCTION: fn_09312
int UIAdvenPartyBoardWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<AdvenPartyBoardWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_REFRESH:
    this->RefreshRows();
    return 0;
  case UM_UPDATE_PARTY_PAGINATION:
    this->UpdatePagination();
    return 0;
  case UM_COMMAND:
    switch (value) {
    case ID_RESET:
      this->ResetSearch();
      break;
    case ID_PREV:
      this->page = (this->page - 1);

      if (this->searching == 1) {
        this->SendMsg(nullptr, UM_COMMAND, ID_FIND, this->page, 246, 0);
      } else {
        this->LoadPage(this->page);
      }

      break;
    case ID_NEXT:
      this->page = (this->page + 1);
      this->SetPageButtonEnabled(PageButton::Previous, 0);
      this->SetPageButtonEnabled(PageButton::Next, 0);

      if (this->searching == 1) {
        this->SendMsg(nullptr, UM_COMMAND, ID_FIND, this->page, 218, 0);
      } else {
        this->LoadPage(this->page);
      }

      break;
    case ID_FIND:
      this->Search(static_cast<int>(c));
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_ADVENPARTYBOARDWND);
      break;
    case ID_SELECT_PARTY_LISTING: {
      const auto& info = this->rows[static_cast<int>(b)]->info;

      if (info.accountId == g_session.m_aid) {
        g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
        auto* window = static_cast<UIRegisterPartyWnd*>(g_windowMgr.EnsureWindow(WID_REGISTERPARTYWND));

        if (window) {
          window->SetInfo(info);
          g_windowMgr.Promote(window);
        }
      } else {
        g_windowMgr.CloseWindow(WID_REGISTERPARTYWND);
        auto* window = static_cast<UIApplyForPartyWnd*>(g_windowMgr.EnsureWindow(WID_APPLYFORPARTYWND));

        if (window) {
          window->SetInfo(info);
          g_windowMgr.Promote(window);
        }
      }

      break;
    }
    case ID_EDIT_OWN_PARTY_LISTING: {
      g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
      auto* window = static_cast<UIRegisterPartyWnd*>(g_windowMgr.EnsureWindow(WID_REGISTERPARTYWND));

      if (window) {
        window->SetInfo(this->ownParty->info);
        g_windowMgr.Promote(window);
      }

      break;
    }
    case ID_REGISTER_PARTY_LISTING: {
      g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
      UIWindow* window = g_windowMgr.EnsureWindow(WID_REGISTERPARTYWND);

      if (window) {
        g_windowMgr.Promote(window);
      }

      break;
    }
    case ADVENPARTY_BUTTON_STOP_RECRUIT:
      g_adventurerAgencyMgr->RequestStop();
      break;
    case ID_REFRESH:
      this->page = 1;
      this->searching = 0;
      this->LoadPage(1);
      break;
    default:
      break;
    }

    return 0;
  case UM_COMBOBOX_SELECTED:
    if (sender == this->types) {
      this->selectedType = value;
    }

    return 0;
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x007BF520
// FUNCTION: fn_09302
void UIAdvenPartyBoardWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIadventurerAgency\\bg_agencyList.bmp"))), false);
  this->DrawTitleBar(true, LookupMessage(MSI_ADVENTURER_AGENCY_LIST), 0);
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(36, 25, "~", 0, 0, 12, 0, 0, 0);
}
