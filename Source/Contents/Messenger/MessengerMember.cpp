#include "Contents/Messenger/MessengerMember.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Render/Renderer.h"
#include "Resource/TextureNames.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Gauges/UIPcGage.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <string>
#include <vector>

namespace {
  struct MiniPartyIconPosition {
    /* 0000 */ int x;
    /* 0004 */ int y;
    /* 0008 */ int size;
  };

  struct UIFriendOptionContext {
    UIButtonContext& buttons;
    int& option0;
    int& option1;
    int& option2;
  };
}

class UIDragMiniPartyWnd : public UIFrameWnd {
public:
  ~UIDragMiniPartyWnd() override;
  void MoveGauge(int x, int y);
  void RemoveGauge();
  void SetUnknown010C(int first, int second);
  void SetGaugeValues(int current, int maximum);

  /* 00B4 */ MessengerMember member;
  /* 0104 */ UIPcGage* gauge;
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ int unknown_0110;
  /* 0114 */ int iconIndex;
  /* 0118 */ std::vector<MiniPartyIconPosition> iconPositions;
};

class UIFriendOptionWnd : public UIFrameWnd {
public:
  ~UIFriendOptionWnd() override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ FriendOptionWndInfo* saved;
  /* 00B8 */ UICheckBox* option1Box;
  /* 00BC */ UICheckBox* option2Box;
  /* 00C0 */ UICheckBox* option0Box;
  /* 00C4 */ UIBitmapButton* closeButton;

private:
  UIFriendOptionContext* m_options;
};

class UIJobIconButton : public UIBitmapButton {
public:
  ~UIJobIconButton() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;

  /* 0120 */ int unknown_0120;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MiniPartyIconPosition) == 12);
#endif

// OFFSET: 0x00D03DC0
// FUNCTION: fn_21550
MessengerMember::MessengerMember() : valid(1), dead(0) {
}

// OFFSET: 0x00739B60
// FUNCTION: fn_07696
MessengerMember::MessengerMember(int) : valid(0), dead(0) {
}

// OFFSET: 0x00739AB0
// FUNCTION: fn_07695
MessengerMember::MessengerMember(const MessengerMember& other)
    : valid(other.valid), accountId(other.accountId), characterId(other.characterId), name(other.name), map(other.map), leader(other.leader),
      offline(other.offline), color(other.color), job(other.job), level(other.level), dead(other.dead) {
}

// OFFSET: 0x00DC6FA0
// FUNCTION: fn_22785
MessengerMember& MessengerMember::operator=(const MessengerMember& other) {
  this->valid = other.valid;
  this->accountId = other.accountId;
  this->characterId = other.characterId;
  this->name = other.name;
  this->map = other.map;
  this->leader = other.leader;
  this->offline = other.offline;
  this->color = other.color;
  this->job = other.job;
  this->level = other.level;
  this->dead = other.dead;

  return *this;
}

// OFFSET: 0x007384A0
// FUNCTION: fn_07666
MessengerMember::~MessengerMember() = default;

// OFFSET: 0x00738590
// FUNCTION: fn_07667
UIDragMiniPartyWnd::~UIDragMiniPartyWnd() = default;

// OFFSET: 0x00738630
// FUNCTION: fn_07668
void UIDragMiniPartyWnd::MoveGauge(int x, int y) {
  this->gauge->Move(x, y);
}

// OFFSET: 0x00738D20
// FUNCTION: fn_07672
void UIDragMiniPartyWnd::RemoveGauge() {
  this->RemoveChild(this->gauge, 1);
  this->gauge = nullptr;
}

// OFFSET: 0x00738D50
// FUNCTION: fn_07674
void UIDragMiniPartyWnd::SetUnknown010C(int first, int second) {
  this->unknown_010C = first;
  this->unknown_0110 = second;
}

// OFFSET: 0x00738D40
// FUNCTION: fn_07673
void UIDragMiniPartyWnd::SetGaugeValues(int current, int maximum) {
  this->gauge->SetValues(current, maximum);
}

// OFFSET: 0x0089BCC0
// FUNCTION: fn_11577
void UIPcGage::SetValues(int current, int maximum) {
  this->current = current;
  this->maximum = maximum;
  this->Invalidate();
}

// OFFSET: 0x00738EC0
// FUNCTION: fn_07680
UIFriendOptionWnd::~UIFriendOptionWnd() = default;

// OFFSET: 0x00738F30
// FUNCTION: fn_07681
void UIFriendOptionWnd::Slot15(int, int) {
  auto& controls = this->m_options->buttons;

  const char* label = controls.services.MessageText(361);
  int width = std::min(this->TextWidth(controls.fonts, controls.locale, label, 0, 0, 12, 0, 0) + 12, this->geometry.width - 15);
  this->option0Box = new UICheckBox(*this->m_context, controls);
  this->option0Box->SetSize(width, 12);
  this->option0Box->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", controls.services.MessageText(361));
  this->option0Box->Move(6, 21);
  this->option0Box->SetChecked(this->m_options->option0);
  this->AddChild(this->option0Box);

  label = controls.services.MessageText(359);
  width = std::min(this->TextWidth(controls.fonts, controls.locale, label, 0, 0, 12, 0, 0) + 12, this->geometry.width - 15);
  this->option2Box = new UICheckBox(*this->m_context, controls);
  this->option2Box->SetSize(width, 12);
  this->option2Box->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", controls.services.MessageText(359));
  this->option2Box->Move(6, 37);
  this->option2Box->SetChecked(this->m_options->option2);
  this->AddChild(this->option2Box);

  label = controls.services.MessageText(362);
  width = std::min(this->TextWidth(controls.fonts, controls.locale, label, 0, 0, 12, 0, 0) + 12, this->geometry.width - 15);
  this->option1Box = new UICheckBox(*this->m_context, controls);
  this->option1Box->SetSize(width, 12);
  this->option1Box->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", controls.services.MessageText(362));
  this->option1Box->Move(6, 53);
  this->option1Box->SetChecked(this->m_options->option1);
  this->AddChild(this->option1Box);

  this->closeButton = this->AddCloseButton(controls, "Alt+I", 0);
}

// OFFSET: 0x007391C0
// FUNCTION: fn_07682
void UIFriendOptionWnd::Paint() {
  auto& controls = this->m_options->buttons;

  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(355), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  const int barHeight = this->DrawButtonBar(0, 0, 1);
  this->FillRegion(0, this->geometry.height - barHeight, this->geometry.width, 1, 0xC0C0C0);
}

// OFFSET: 0x00739220
// FUNCTION: fn_07683
int UIFriendOptionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 201) {
      this->m_frameContext->windows.CloseWindow(68);
    } else if (a == 213) {
      if (sender == this->option0Box) {
        this->m_options->option0 = static_cast<int>(b);
      } else if (sender == this->option2Box) {
        this->m_options->option2 = static_cast<int>(b);
      } else if (sender == this->option1Box) {
        this->m_options->option1 = static_cast<int>(b);
      }
    }
    return 0;
  }

  if (message == 34) {
    this->saved = reinterpret_cast<FriendOptionWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009A54B0
// FUNCTION: fn_13630
int FriendOptionWndInfo::IsValid() const {
  return this->width == 280 && this->height == 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - 120;
}

// OFFSET: 0x007392F0
// FUNCTION: fn_07684
void UIFriendOptionWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007393E0
// FUNCTION: fn_07686
UIJobIconButton::~UIJobIconButton() = default;

// OFFSET: 0x00739450
// FUNCTION: fn_07687
void UIJobIconButton::Paint() {
  std::string name;
  if (this->m_disabled == 1) {
    name = this->m_names[3];
  } else {
    switch (this->geometry.unknown_0030) {
    case 0:
      name = this->m_names[0];
      break;
    case 1:
      name = this->m_names[1];
      break;
    case 2:
      name = this->m_names[2];
      break;
    }
  }

  if (!name.empty()) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
    if (bitmap) {
      if (this->unknown_0120 == 1) {
        this->DrawAuxiliary(1, 0, 0, &bitmap->image, true, false);
      } else {
        this->DrawBitmap(0, 0, &bitmap->image, true);
      }
    }
  }

  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x007395E0
// FUNCTION: fn_07688
void UIJobIconButton::Slot25(int x, int y) {
  if (UIWindow* window = this->m_context->windowMgr.LookupWindow(34)) {
    window->Slot25(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00739610
// FUNCTION: fn_07689
void UIJobIconButton::Slot31(int x, int y) {
  if (UIWindow* window = this->m_context->windowMgr.LookupWindow(34)) {
    window->Slot31(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00739640
// FUNCTION: fn_07690
void UIJobIconButton::Slot33(int x, int y) {
  if (UIWindow* window = this->m_context->windowMgr.LookupWindow(34)) {
    window->Slot33(x + this->geometry.x, y + this->geometry.y);
  }
}
