#include "Contents/Messenger/MessengerMember.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <vector>

namespace {
  struct MiniPartyIconPosition {
    /* 0000 */ int x;
    /* 0004 */ int y;
    /* 0008 */ int size;
  };

  struct FriendOptionPlacement {
    /* 0000 */ int x;
    /* 0004 */ int y;
    /* 0008 */ int width;
    /* 000C */ int height;
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

  /* 00B4 */ MessengerMember member;
  /* 0104 */ UIWindow* gauge;
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

  /* 00B4 */ FriendOptionPlacement* saved;
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
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;

  /* 0120 */ int unknown_0120;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MiniPartyIconPosition) == 12);
static_assert(sizeof(FriendOptionPlacement) == 16);
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
