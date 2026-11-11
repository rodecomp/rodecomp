#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Session/Session.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Party/UIRequestJoinPartyMgr.h"
#include "UI/Party/UIRequestJoinPartyWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Login/UINewSelectCharWnd.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>

// INLINED
static void SetViewerBackground(UITextViewer* viewer, std::uint32_t color) {
  viewer->backgroundColor = color;
  viewer->Invalidate();
}

// OFFSET: 0x007BB030
// FUNCTION: fn_09280
UIRequestJoinPartyWnd::UIRequestJoinPartyWnd()
    : UIFrameWnd(0) {
}

UIRequestJoinPartyWnd::~UIRequestJoinPartyWnd() = default;

// OFFSET: 0x007BEFD0
// FUNCTION: fn_09301
void UIRequestJoinPartyWnd::OnCreate(int, int) {
  this->icon = new UIBitmapButton();
  this->icon->SetSize(25, 25);

  this->icon->SetBitmap("유저인터페이스\\renewalparty\\icon_jobs_4046.bmp", 0, 0);
  this->icon->SetBitmap("유저인터페이스\\renewalparty\\icon_jobs_4046.bmp", 1, 0);
  this->icon->SetBitmap("유저인터페이스\\renewalparty\\icon_jobs_4046.bmp", 2, 0);

  this->icon->Move(12, 29);
  this->AddChild(this->icon);

  this->levelText = new UIStaticText();
  this->levelText->SetSize(25, 14);
  this->levelText->Move(16, 56);
  this->levelText->SetText("1", 0);
  this->levelText->SetColor(0, 0, 0);
  this->AddChild(this->levelText);
  const int ids[] = {541, 542};
  const int positions[][2] = {{122, 136}, {169, 136}};
  const std::string labels[] = {LookupMessage(MSI_CANNOT_FIND_REQUEST_USER), LookupMessage(MSI_STORE_ASSISTANT_TRADE_FORCED_REMOVAL)};

  for (int index = 0; index < 2; ++index) {
    auto* button = new UIBasicButton();
    button->Configure(this->TextWidth(labels[index].c_str(), 0, 0, 12, 0, 0) + 10, labels[index].c_str());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 541;
  this->details = new UITextViewer();
  this->details->Configure(43, 20, 167, 60, 0);
  SetViewerBackground(this->details, 0xFFFFFFFFu);
  this->AddChild(this->details);

  this->block = new UICheckBox();
  this->block->SetSize(24, 47);
  this->block->SetImages("유저인터페이스\\uiadventureragency\\btn_block_on.bmp", "유저인터페이스\\uiadventureragency\\btn_block_off.bmp", "");
  this->block->Move(7, 81);
  this->block->SetChecked(0);
  this->block->SetId(213);
  this->AddChild(this->block);
  std::vector<std::string> unused;
  WrapPlainText(LookupMessage(MSI_EXPAND_OBJALPHA), unused, 30);

  this->blockText = new UITextViewer();
  this->blockText->Configure(31, 81, 182, 47, 0);
  SetViewerBackground(this->blockText, 0xFFD7C6BCu);
  this->AddChild(this->blockText);
  this->blockText->lineHeight = 15;
  this->blockText->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXPAND_OBJALPHA)), 30, 0, 0);
  this->AddBigCloseButton("", 0);
}

// OFFSET: 0x007BFC20
// FUNCTION: fn_09306
void UIRequestJoinPartyWnd::Paint() {
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIadventurerAgency\\bg_agency_admit.bmp")));
  this->DrawBitmap(0, 0, background, false);
  this->DrawTitleBar(true, LookupMessage(MSI_ADVEN_ASSIST), 0);
}

// OFFSET: 0x007C1770
// FUNCTION: fn_09315
int UIRequestJoinPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      this->SendMsg(nullptr, UM_COMMAND, 542, 0, 0, 0);
      break;
    case 541:
      g_modeMgr.gameMode->SendMsg(MM_PARTY_REQ_ACK_MASTER_TO_JOIN, this->characterId, 1, reinterpret_cast<std::intptr_t>(&this->accountId), 0);
      g_requestJoinPartyMgr->Advance();
      this->block->SetChecked(0);
      break;
    case 542:
      if (this->block->geometry.state != 0) {
        g_requestJoinPartyMgr->Block(this->accountId);
      }

      g_modeMgr.gameMode->SendMsg(MM_PARTY_REQ_ACK_MASTER_TO_JOIN, this->characterId, 0, reinterpret_cast<std::intptr_t>(&this->accountId), 0);
      g_requestJoinPartyMgr->Advance();
      this->block->SetChecked(0);
      break;
    }

    return 0;
  case UM_SET_JOIN_PARTY_MSG: {
    const auto& request = *reinterpret_cast<const PartyJoinRequest*>(a);
    this->characterId = request.characterId;
    this->level = request.job;
    this->job = request.level;
    this->accountId = request.accountId;
    char path[500] = {};
    _snprintf_s(path, sizeof(path), _TRUNCATE, "유저인터페이스\\renewalparty\\icon_jobs_%lu.bmp", this->job);

    this->icon->SetBitmap(path, 0, 0);
    this->icon->SetBitmap(path, 1, 0);
    this->icon->SetBitmap(path, 2, 0);

    const std::string jobName = g_session.GetJobName(this->job, -1);
    this->icon->SetTooltip(jobName.c_str());
    char level[5] = {};
    _snprintf_s(level, sizeof(level), _TRUNCATE, "%d", this->level);
    this->levelText->SetText(level, 0);
    const int width = this->TextWidth(level, 0, 0, 12, 0, 0);
    this->levelText->Move(25 - width / 2, 56);
    this->details->Clear();
    char messageText[1024] = {};
    _snprintf_s(messageText, sizeof(messageText), _TRUNCATE, LookupMessage(MSI_EXIT), request.name);
    this->details->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(messageText), 0, 0, 1);
    if (this->details->lines.size() == 3) {
      this->details->Move(43, 20);
    } else {
      this->details->Move(43, 30);
    }
    this->InvalidateChildren();

    return 0;
  }
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}
