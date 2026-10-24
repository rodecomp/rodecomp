#include "Client/TextInputBuffer.h"
#include "Contents/AdventurerAgencyMgr.h"
#include "Contents/AgencyRequestContext.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/RegisterPartyAsyncWork.h"
#include "Render/DeviceContext.h"
#include "Render/TextContext.h"
#include "Resource/BitmapResource.h"
#include "Resource/ResourceFailures.h"
#include "Resource/ResourceMgr.h"
#include "UI/InputFocus.h"
#include "UI/TextFitting.h"
#include "UI/TextWrapping.h"
#include "UI/UIBasicButton.h"
#include "UI/UIBitmapButton.h"
#include "UI/UIComboBoxDefWnd.h"
#include "UI/UIEditCtrl.h"
#include "UI/UIJobSlotButton.h"
#include "UI/UIRegisterPartyWnd.h"
#include "UI/UIStaticText.h"
#include "UI/WindowManagerWindows.h"

#include <algorithm>
#include <bit>
#include <cctype>
#include <cstdio>
#include <cstdlib>

namespace {
  constexpr char prefix[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\";
  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }

  // OFFSET: 0x007BBFC0
  // FUNCTION: fn_09295
  bool Digits(std::string text) {
    if (text.empty()) {
      return false;
    }

    for (unsigned char value : text) {
      if (!std::isdigit(value)) {
        return false;
      }
    }

    return true;
  }
}

// OFFSET: 0x007BAF30
UIRegisterPartyWnd::UIRegisterPartyWnd(
  UIWindowContext& windows,
  UIFrameContext& frames,
  UIRegisterPartyContext& registration
)
    : UIFrameWnd(windows, frames, 0), selectedType(0), m_registration(&registration) {
}

// OFFSET: 0x007BB810
// FUNCTION: fn_09286
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
  auto& owner = *this->m_registration;
  auto& controls = owner.buttons;
  const auto label = [&](const char* text, int x, int y, int width) {
    auto* value = new UIStaticText(*this->m_context, controls);
    value->SetSize(width, 15);
    value->Move(x, y);
    value->SetText(text, 0);
    value->red = 0;
    value->green = 0;
    value->blue = 0;
    this->AddChild(value);
    return value;
  };
  label(controls.services.MessageText(3415), 10, 23, 40);
  this->character = label(owner.characterName.c_str(), 60, 23, 119);
  this->character->text =
    FitText(*this->character->geometry.surface, controls.fonts, controls.locale, owner.characterName, 119);
  this->character->tooltipEnabled = this->character->text != owner.characterName;
  label(controls.services.MessageText(408), 10, 49, 40);
  const int level = owner.playerLevel;
  const auto edit = [&](int x, int value) {
    auto* control = new UIEditCtrl(*this->m_context, owner.edits);
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
  this->minimum = edit(60, std::max(Add(level, -5), 1));
  this->maximum = edit(96, Add(level, 5));
  label(controls.services.MessageText(3369), 10, 73, this->geometry.width);
  auto* roleLabel = label(controls.services.MessageText(504), 10, 97, this->geometry.width);
  roleLabel->outline = 1;
  const char* roles[] = {"def", "heal", "atk", "buf"};
  const int tooltips[] = {3420, 3491, 3444, 3508};

  for (int index = 0; index < 4; ++index) {
    auto* job = new UIJobSlotButton(*this->m_context, controls);
    this->jobs[index] = job;
    job->SetSize(16, 16);
    job->Move(60 + index * 18, 95);
    job->selectedImage = std::string(prefix) + "UIadventurerAgency\\btn_job_" + roles[index] + "_on.bmp";
    job->unselectedImage = std::string(prefix) + "UIadventurerAgency\\btn_job_" + roles[index] + "_off.bmp";
    job->SetSelected(1);
    job->SetTooltip(controls.services.MessageText(tooltips[index]));
    this->AddChild(job);
  }

  label(controls.services.MessageText(3359), 10, 121, this->geometry.width - 32);
  this->memo = new UIEditCtrl(*this->m_context, owner.edits);
  this->memo->SetSize(139, 14);
  this->memo->maximum = 24;
  this->memo->TextColor(0, 0xFFFFFFFFu);
  this->memo->Move(60, 119);
  this->memo->Background(232, 232, 232);
  this->AddChild(this->memo);
  auto* submit = new UIBasicButton(*this->m_context, controls);
  const bool registered = g_adventurerAgencyMgr->registered;
  const char* title = controls.services.MessageText(registered ? 3376 : 3371);
  submit->Configure(this->TextWidth(controls.fonts, controls.locale, title, 0, 0, 12, 0, 0) + 10, title);
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
  WrapPlainText(controls.services.MessageText(3370), lines, 40, owner.codePage, controls.locale.inlineCharset);
  int y = 136;

  for (const auto& line : lines) {
    auto* value = label(line.c_str(), 9, y, this->geometry.width - 18);
    value->fontHeight = 11;
    value->red = 255;
    y += 12;
  }

  auto* close = new UIBitmapButton(*this->m_context, controls);
  const char* suffixes[] = {"", "_a", "_b"};

  for (int state = 0; state < 3; ++state) {
    const std::string path = std::string(prefix) + "basic_interface\\btn_close" + suffixes[state] + ".bmp";
    close->SetBitmap(path.c_str(), state, 0);
  }

  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(173, 165);
  close->SetId(201);
  this->AddChild(close);
  this->types = new UIComboBoxDef2Wnd(*this->m_context, owner.combos);
  this->types->dropdownHeight = 126;
  this->types->SetSize(63, 18);
  this->types->Move(60, 70);
  this->AddChild(this->types);
  this->types->AddOption(controls.services.MessageText(3377), 0);

  for (int id = 3360; id <= 3365; ++id) {
    this->types->AddOption(controls.services.MessageText(id), 0);
  }

  this->types->Select(0);
  auto* titleClose = new UIBitmapButton(*this->m_context, controls);
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
  _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", value.minimumLevel);
  this->minimum->SetText(number);
  _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", value.maximumLevel);
  this->maximum->SetText(number);
  this->selectedType = value.type;
  this->types->Select(value.type);
  this->memo->SetText(value.memo.c_str());
  this->character->SetText(value.characterName.c_str(), 0);
  this->Invalidate();
}

const char* UIRegisterPartyWnd::Text(UIEditCtrl& control) const {
  const auto& edit = this->m_registration->edits;
  return edit.focus.window == &control ? edit.input.text.c_str() : control.text.c_str();
}

// OFFSET: 0x007BBA80
// FUNCTION: fn_09290
void UIRegisterPartyWnd::NormalizeLevels() {
  const int level = this->m_registration->playerLevel;
  char number[10] = {};

  if (!Digits(this->Text(*this->minimum))) {
    _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", std::max(Add(level, -5), 1));
    this->minimum->SetText(number);
  }

  if (!Digits(this->Text(*this->maximum))) {
    _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", Add(level, 5));
    this->maximum->SetText(number);
  }
}

// OFFSET: 0x007BFBB0
// FUNCTION: fn_09305
void UIRegisterPartyWnd::Paint() {
  auto& controls = this->m_registration->buttons;
  this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
  const auto load = [&](const char* name) {
    return static_cast<CBitmapRes*>(controls.resources.Load((std::string(prefix) + name).c_str()));
  };
  const auto* background = load("UIadventurerAgency\\bg_agency_register.bmp");
  this->DrawBitmap(0, 0, background ? &background->image : nullptr, false);
  this->DrawTextAt(controls.fonts, controls.locale, 86, 50, "~", 0, 0, 12, 0, 0, 0);
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

  const char* caption = controls.services.MessageText(3356);
  this->DrawTextAt(controls.fonts, controls.locale, 19, titleHeight - 13, caption, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 18, titleHeight - 14, caption, 0, 0, 12, 0, 0, 0);
  const auto* base = load("basic_interface\\sys_base_off.bmp");
  this->DrawBitmap(3, 3, base ? &base->image : nullptr, true);
}

// OFFSET: 0x007C1410
// FUNCTION: fn_09314
int UIRegisterPartyWnd::SendMsg(
  UIWindow* sender,
  int message,
  std::intptr_t a,
  std::intptr_t b,
  std::intptr_t c,
  std::intptr_t d
) {
  auto& owner = *this->m_registration;

  if (message == 6) {
    const int value = std::bit_cast<int>(static_cast<unsigned>(a));

    if (value != 201) {
      if (value != 564 && value != 565) {
        return 0;
      }

      const auto notify = [&](int id) {
        char text[256] = {};
        _snprintf_s(text, sizeof(text), _TRUNCATE, owner.buttons.services.MessageText(id));
        owner.completion.messages.Message(text, 0xFF);
      };

      if (owner.partyMember != 0) {
        if (owner.forbidden.Contains(this->Text(*this->memo))) {
          notify(3372);
          return 0;
        }

        this->NormalizeLevels();

        if (std::atoi(this->Text(*this->minimum)) > std::atoi(this->Text(*this->maximum))) {
          notify(3516);
          return 0;
        }

        CAdvenPartyInfo info;
        info.tanker = this->jobs[0]->selected;
        info.healer = this->jobs[1]->selected;
        info.dealer = this->jobs[2]->selected;
        info.assist = this->jobs[3]->selected;
        info.minimumLevel = std::atoi(this->Text(*this->minimum));
        info.maximumLevel = std::atoi(this->Text(*this->maximum));
        info.type = this->selectedType;
        info.memo = this->Text(*this->memo);
        info.characterName = owner.characterName;
        g_adventurerAgencyMgr->RequestRegistration(info, owner.requests, owner.completion);
      } else {
        notify(3506);
      }
    }

    owner.requests.windows.CloseWindow(323);
    return 0;
  }

  if (message == 34) {
    this->saved = reinterpret_cast<PartyBoardPlacement*>(a);

    if (
      this->saved->width <= 0 || this->saved->height <= 0 || this->saved->x < 0 ||
      this->saved->x > owner.screenWidth - this->saved->width || this->saved->y < 0 ||
      this->saved->y > owner.screenHeight - this->saved->height
    ) {
      *this->saved = {200, 200, 220, 197};
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  if (message == 39) {
    if (sender == this->types) {
      this->selectedType = std::bit_cast<int>(static_cast<unsigned>(a));
    }

    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}
