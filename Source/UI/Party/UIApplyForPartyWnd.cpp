#include "Client/Session/Session.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "File/ShortcutFilename.h"
#include "Render/Renderer.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Party/UIPartyInfoWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>

template __declspec(noinline) std::string::basic_string(const std::string&);

// OFFSET: 0x007BAD00
// FUNCTION: fn_09275
UIApplyForPartyWnd::UIApplyForPartyWnd()
    : UIFrameWnd(0) {
}

UIApplyForPartyWnd::~UIApplyForPartyWnd() = default;

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
  button->Configure(this->TextWidth(LookupMessage(MSI_ALL_AREA), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_ALL_AREA));
  button->Move(51, 165);
  button->SetId(381);
  this->AddChild(button);

  button = new UIBasicButton();
  button->Configure(this->TextWidth(LookupMessage(MSI_ADVEN_REWRITE), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_ADVEN_REWRITE));
  button->Move(108, 165);
  button->SetId(271);
  this->AddChild(button);

  button = new UIBasicButton();
  button->Configure(this->TextWidth(LookupMessage(MSI_ZENY_LOTTO_BUY_10), 0, 0, 12, 0, 0) + 10, LookupMessage(MSI_ZENY_LOTTO_BUY_10));
  button->Move(173, 165);
  button->SetId(201);
  this->AddChild(button);

  std::vector<std::string> lines;
  WrapPlainText(LookupMessage(MSI_ASK_JOIN), lines, 40);

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
  this->info = value;
  this->Invalidate();
}

// OFFSET: 0x007BF5A0
// FUNCTION: fn_09303
void UIApplyForPartyWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIadventurerAgency\\bg_agency_register.bmp"))), false);
  this->DrawTitleBar(true, LookupMessage(MSI_ADVEN_REGISTER), 0);
  this->DrawTextAt(10, 25, LookupMessage(MSI_ADVENTURER_AGENCY_REGIST_SUCCESSED), 0, 0, 12, 0, 0, 0);
  const std::string fitted = this->FitTextToWidth(this->info.characterName, 119);
  this->DrawTextAt(60, 25, fitted.c_str(), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(10, 49, LookupMessage(MSI_LEVEL), 0, 0, 12, 0, 0, 0);
  char levels[10] = {};
  StringFormat(levels, sizeof(levels), _TRUNCATE, "%d~%d", this->info.minimumLevel, this->info.maximumLevel);
  this->DrawTextAt(60, 49, levels, 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(10, 73, LookupMessage(MSI_INCLUDE_BAD_WORD), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(60, 73, UIPartyInfoWnd::TypeName(this->info.type), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(10, 97, LookupMessage(MSI_JOB), 0, 0, 12, 0, 0, 0);
  this->DrawBitmap(60, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.tanker == true ? "유저인터페이스\\UIadventurerAgency\\btn_job_def_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_def_off.bmp"))), false);
  this->DrawBitmap(78, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.healer == true ? "유저인터페이스\\UIadventurerAgency\\btn_job_heal_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_heal_off.bmp"))), false);
  this->DrawBitmap(96, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.dealer == true ? "유저인터페이스\\UIadventurerAgency\\btn_job_atk_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_atk_off.bmp"))), false);
  this->DrawBitmap(114, 95, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.assist == true ? "유저인터페이스\\UIadventurerAgency\\btn_job_buf_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_buf_off.bmp"))), false);
  this->DrawTextAt(10, 121, LookupMessage(MSI_ADVEN_DUNJUN), 0, 0, 12, 0, 0, 0);
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

  if (left > 60 && left < 76) {
    left = 60;
    label = LookupMessage(MSI_EXPLAIN_CHAT_UNBLOCK);
  } else if (left > 78 && left < 94) {
    left = 78;
    label = LookupMessage(MSI_CANNOT_FIND_PARTYMASTER);
  } else if (left > 96 && left < 112) {
    left = 96;
    label = LookupMessage(MSI_QUESTWND_LOWER);
  } else if (left > 114 && left < 130) {
    left = 114;
    label = LookupMessage(MSI_EXPAND_SIGHT_ON);
  }

  if (!label.empty()) {
    int screenX = static_cast<int>(left);
    int screenY = 75;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(label.c_str(), screenX, screenY, -1, false, true);
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
