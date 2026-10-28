#include "Client/Mode/ModeEvents.h"
#include "Contents/Jobs/JobNames.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Party/UIRequestJoinPartyMgr.h"
#include "UI/Party/UIRequestJoinPartyWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"

#include <cstdio>

namespace {
  constexpr char prefix[] = "유저인터페이스\\";
}

// OFFSET: 0x007BB030
// FUNCTION: fn_09280
UIRequestJoinPartyWnd::UIRequestJoinPartyWnd(UIWindowContext& windows, UIFrameContext& frames, UIRequestJoinPartyContext& request)
    : UIFrameWnd(windows, frames, 0), m_request(&request) {
}

// OFFSET: 0x007BB960
UIRequestJoinPartyWnd::~UIRequestJoinPartyWnd() = default;

// OFFSET: 0x007BEFD0
// FUNCTION: fn_09301
void UIRequestJoinPartyWnd::Slot15(int, int) {
  auto& text = this->m_request->text;
  auto& controls = text.buttons;

  this->icon = new UIBitmapButton(*this->m_context, controls);
  this->icon->SetSize(25, 25);
  const std::string initial = std::string(prefix) + "renewalparty\\icon_jobs_4046.bmp";

  for (int state = 0; state < 3; ++state) {
    this->icon->SetBitmap(initial.c_str(), state, 0);
  }

  this->icon->Move(12, 29);
  this->AddChild(this->icon);

  this->levelText = new UIStaticText(*this->m_context, controls);
  this->levelText->SetSize(25, 14);
  this->levelText->Move(16, 56);
  this->levelText->SetText("1", 0);
  this->levelText->red = 0;
  this->levelText->green = 0;
  this->levelText->blue = 0;
  this->AddChild(this->levelText);
  const int messages[] = {3489, 3490};
  const int ids[] = {541, 542};
  const int x[] = {122, 169};

  for (int index = 0; index < 2; ++index) {
    auto* button = new UIBasicButton(*this->m_context, controls);
    const char* label = controls.services.MessageText(messages[index]);
    button->Configure(this->TextWidth(controls.fonts, controls.locale, label, 0, 0, 12, 0, 0) + 10, label);
    button->Move(x[index], 136);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 541;
  this->details = new UITextViewer(*this->m_context, text);
  this->details->Configure(43, 20, 167, 60, 0);
  this->details->textColor = 0xFFFFFFFFu;
  this->details->Invalidate();
  this->AddChild(this->details);

  this->block = new UICheckBox(*this->m_context, controls);
  this->block->SetSize(24, 47);
  const std::string checked = std::string(prefix) + "uiadventureragency\\btn_block_on.bmp";
  const std::string unchecked = std::string(prefix) + "uiadventureragency\\btn_block_off.bmp";
  this->block->SetImages(checked.c_str(), unchecked.c_str(), "");
  this->block->Move(7, 81);
  this->block->SetChecked(0);
  this->block->SetId(213);
  this->AddChild(this->block);
  std::vector<std::string> unused;
  WrapPlainText(controls.services.MessageText(3518), unused, 30, text.list.codePage);

  this->blockText = new UITextViewer(*this->m_context, text);
  this->blockText->Configure(31, 81, 182, 47, 0);
  this->blockText->textColor = 0xFFD7C53Cu;
  this->blockText->Invalidate();
  this->AddChild(this->blockText);
  this->blockText->lineHeight = 15;
  this->blockText->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(controls.services.MessageText(3518)), 30, 0, 0);

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

// OFFSET: 0x007BFC20
// FUNCTION: fn_09306
void UIRequestJoinPartyWnd::Paint() {
  auto& controls = this->m_request->text.buttons;
  const auto load = [&](const char* name) { return static_cast<CBitmapRes*>(controls.resources.Load((std::string(prefix) + name).c_str())); };
  const auto* background = load("UIadventurerAgency\\bg_agency_admit.bmp");
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

  const char* caption = controls.services.MessageText(3505);
  this->DrawTextAt(controls.fonts, controls.locale, 19, titleHeight - 13, caption, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 18, titleHeight - 14, caption, 0, 0, 12, 0, 0, 0);
  const auto* base = load("basic_interface\\sys_base_off.bmp");
  this->DrawBitmap(3, 3, base ? &base->image : nullptr, true);
}

// OFFSET: 0x007C1770
// FUNCTION: fn_09315
int UIRequestJoinPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    const int value = static_cast<int>(a);

    if (value == 201) {
      this->SendMsg(nullptr, 6, 542, 0, 0, 0);
      return 0;
    }

    if (value != 541 && value != 542) {
      return 0;
    }

    if (value == 542 && this->block->geometry.unknown_0030 != 0) {
      this->m_request->requests.Block(this->accountId);
    }

    this->m_context->mode->SendModeMessage(264, this->characterId, value == 541, reinterpret_cast<std::intptr_t>(&this->accountId), 0);
    this->m_request->requests.Advance();
    this->block->SetChecked(0);

    return 0;
  }

  if (message != 56) {
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  const auto& request = *reinterpret_cast<const PartyJoinRequest*>(a);
  this->characterId = request.characterId;
  this->accountId = request.accountId;
  this->job = request.job;
  this->level = request.level;
  char path[500] = {};
  _snprintf_s(path, sizeof(path), _TRUNCATE, "%srenewalparty\\icon_jobs_%lu.bmp", prefix, static_cast<unsigned long>(static_cast<int>(this->job)));

  for (int state = 0; state < 3; ++state) {
    this->icon->SetBitmap(path, state, 0);
  }

  this->icon->SetTooltip(this->m_request->jobs.Lookup(static_cast<unsigned>(static_cast<int>(this->job)), -1));
  char level[5] = {};
  _snprintf_s(level, sizeof(level), _TRUNCATE, "%d", this->level);
  this->levelText->SetText(level, 0);
  auto& controls = this->m_request->text.buttons;
  const int width = this->TextWidth(controls.fonts, controls.locale, level, 0, 0, 12, 0, 0);
  this->levelText->Move(25 - width / 2, 56);
  this->details->Clear();
  char messageText[1024] = {};
  _snprintf_s(messageText, sizeof(messageText), _TRUNCATE, controls.services.MessageText(3378), request.name);
  this->details->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(messageText), 0, 0, 1);
  this->details->Move(43, static_cast<unsigned>(this->details->lines.size()) - 3u >= 1u ? 30 : 20);
  this->Invalidate();

  return 0;
}
