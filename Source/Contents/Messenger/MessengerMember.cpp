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
#include "UI/Lists/UIScrollBar.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/World/World.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Render/Text/TextContext.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Resource/SkinPalette.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Text/UIStaticText.h"
#include "File/ShortcutFilename.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Resource/SkinPalette.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <timeapi.h>
#include <string>
#include <vector>

namespace {
#pragma pack(push, 1)
  struct MessengerRecordHeader {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::uint32_t bytes;
  };
#pragma pack(pop)

  struct MiniPartyIconPosition {
    /* 0000 */ int x;
    /* 0004 */ int y;
    /* 0008 */ int size;
  };

  struct SessionHelper {
    const char* sub_DDE770(const char* mapName);
  };

}

class UIDragMiniPartyWnd : public UIFrameWnd {
public:
  UIDragMiniPartyWnd(UIWindowContext& windowContext, UIFrameContext& frameContext, MessengerMember member);
  ~UIDragMiniPartyWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot31(int argument1, int argument2) override;
  void MoveGauge(int x, int y);
  void RemoveGauge();
  void SetUnknown010C(int first, int second);
  void SetGaugeValues(int current, int maximum);
  std::string sub_A80880(int width, const char* text, int font, int height, int bold);
  void sub_A84360(int x, int y, const char* text, int length, int font, int height, unsigned int color, int bold, int shadow);

  /* 00B4 */ MessengerMember member;
  /* 0104 */ UIPcGage* gauge;
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ int unknown_0110;
  /* 0114 */ int iconIndex;
  /* 0118 */ std::vector<MiniPartyIconPosition> iconPositions;
};

struct UIFriendOptionContext {
  UIButtonContext& buttons;
  int& option0;
  int& option1;
  int& option2;
};

class UIFriendOptionWnd : public UIFrameWnd {
public:
  UIFriendOptionWnd(UIWindowContext& windows, UIFrameContext& frames);
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
  UIJobIconButton(UIWindowContext& windows, UIButtonContext& buttons);
  ~UIJobIconButton() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;

  /* 0120 */ int unknown_0120;
};

class UIMessengerGroupWnd : public UIFrameWnd {
public:
  UIMessengerGroupWnd();
  ~UIMessengerGroupWnd() override;
  void Slot11() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;
  MessengerMember MemberAt(int x, int y);
  void OpenMiniPartyWnd(int cell, const MessengerMember& member);
  void DropMember(int x, int y, int index);
  MessengerMember MemberAtIndex(int index);
  void UpdateScroll();
  void LayoutButtons();
  void UpdateGauges();
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  // INLINED
  UIWindow* CompanionWindow() const {
    if (this->mode == 0) {
      return reinterpret_cast<UIWindow*>(g_windowMgr.unknown_033C);
    }
    UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_042C);
    if (g_windowMgr.unknown_030C) {
      window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_030C);
    }
    return window;
  }

  /* 00B4 */ MessengerGroupWndInfo* saved;
  /* 00B8 */ UIScrollBar* scrollBar;
  /* 00BC */ UIWindow* tabs[2];
  /* 00C4 */ UIWindow* closeButton;
  /* 00C8 */ UIWindow* buttons[11];
  /* 00F4 */ UIWindow* resizeButton;
  /* 00F8 */ UIStaticText* titleText;
  /* 00FC */ std::list<MessengerMember> members;
  /* 0104 */ std::vector<MiniPartyInfoEntry> menuItems;
  /* 0110 */ int scrollable;
  /* 0114 */ int scrollTop;
  /* 0118 */ int columns;
  /* 011C */ int visibleRows;
  /* 0120 */ int selected;
  /* 0124 */ int rowHeight;
  /* 0128 */ UIPcGage* gauges[40];
  /* 01C8 */ UIJobIconButton* jobIcons[40];
  /* 0268 */ int iconIndex;
  /* 026C */ std::vector<MiniPartyIconPosition> iconPositions;
  /* 0278 */ UIButton* iconModeButtons[2];
  /* 0280 */ unsigned char hiddenButtons[11];
  /* 028C */ int mode;
};

// GLOBAL: gl_24961
bool g_messengerGroupCollapsed;
// GLOBAL: gl_25219
int g_messengerDropColumnWidth;
// GLOBAL: gl_25220
int g_messengerDropRowHeight;
// GLOBAL: gl_25250
std::string g_messengerNamelessTag = "<NAMELESS>";

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MiniPartyIconPosition) == 12);
static_assert(sizeof(MessengerRecordHeader) == 6);
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

// OFFSET: 0x007382E0
// FUNCTION: fn_07664
UIDragMiniPartyWnd::UIDragMiniPartyWnd(UIWindowContext& windowContext, UIFrameContext& frameContext, MessengerMember member)
    : UIFrameWnd(windowContext, frameContext, 0), member(member), gauge(nullptr), iconIndex(0) {
  this->geometry.unknown_0030 = 0;
}

// OFFSET: 0x007384A0
// FUNCTION: fn_07666
MessengerMember::~MessengerMember() = default;

// OFFSET: 0x00E48C00
// FUNCTION: fn_24228
int ReadMessengerMembers(std::list<MessengerMember>& members, int offset, int size, const std::vector<unsigned char>& data) {
  if (size <= 0) {
    return offset;
  }

  std::vector<unsigned char> buffer(size, 0);
  std::memcpy(buffer.data(), data.data() + offset, size);
  int end = offset + size;
  std::uint32_t consumed = 0;
  std::vector<unsigned char> remaining(buffer);
  while (consumed < buffer.size()) {
    MessengerMember member;
    consumed += member.Deserialize(remaining);
    members.push_back(member);
    int left = static_cast<int>(buffer.size()) - static_cast<int>(consumed);
    if (left > 0) {
      std::memcpy(remaining.data(), buffer.data() + consumed, left);
    }
  }

  return end;
}

// OFFSET: 0x00E494F0
// FUNCTION: fn_24236
std::uint32_t MessengerMember::Deserialize(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  MessengerRecordHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 251) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    header = {};
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 252:
      return offset;
    case 253:
      std::memcpy(&this->valid, source, header.bytes);
      offset += header.bytes;
      break;
    case 254:
      std::memcpy(&this->accountId, source, header.bytes);
      offset += header.bytes;
      break;
    case 255:
      std::memcpy(&this->characterId, source, header.bytes);
      offset += header.bytes;
      break;
    case 256: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, source, header.bytes);
      offset += header.bytes;
      this->name = buffer;
      break;
    }
    case 257: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, source, header.bytes);
      offset += header.bytes;
      this->map = buffer;
      break;
    }
    case 258:
      std::memcpy(&this->leader, source, header.bytes);
      offset += header.bytes;
      break;
    case 259:
      std::memcpy(&this->offline, source, header.bytes);
      offset += header.bytes;
      break;
    case 260:
      std::memcpy(&this->color, source, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00738590
// FUNCTION: fn_07667
UIDragMiniPartyWnd::~UIDragMiniPartyWnd() = default;

// OFFSET: 0x00738640
// FUNCTION: fn_07669
void UIDragMiniPartyWnd::Slot15(int, int) {
  this->iconPositions.resize(2);
  this->iconPositions[0].x = 5;
  this->iconPositions[0].y = 5;
  this->iconPositions[0].size = 34;
  this->iconPositions[1].x = 4;
  this->iconPositions[1].y = 1;
  this->iconPositions[1].size = 27;

  if (!this->gauge) {
    this->gauge = new UIPcGage();
    this->gauge->SetSize(60, 5);
    this->gauge->Move(54, 25);
    this->AddChild(this->gauge);
  }
}

// OFFSET: 0x007387A0
// FUNCTION: fn_07670
void UIDragMiniPartyWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  std::string prefix = "유저인터페이스\\renewalparty\\";
  if (this->member.valid) {
    bool dead = false;
    if (this->member.dead == 1) {
      dead = true;
    } else if (this->member.accountId == g_session.m_aid) {
      dead = (*reinterpret_cast<const int*>(reinterpret_cast<const char*>(g_modeMgr.GetGameMode()) + 0x250) == 1);
    }

    int yOffset = this->member.leader ? 0 : 2;
    const char* jobFmt = dead ? "%sicon_jobs_%d_die.bmp" : "%sicon_jobs_%d.bmp";
    char path[256];
    sprintf(path, jobFmt, prefix.c_str(), this->member.job);
    this->DrawBitmap(
      this->iconPositions[this->iconIndex].x,
      this->iconPositions[this->iconIndex].y + yOffset,
      reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(path))),
      true
    );

    if (this->member.leader == 0) {
      sprintf(path, "%sico_partyCrown.bmp", prefix.c_str());
      this->DrawBitmap(
        this->iconPositions[this->iconIndex].x + 2,
        0,
        reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(path))),
        true
      );
    }

    const char* mapName = reinterpret_cast<SessionHelper&>(g_session).sub_DDE770(this->member.map.c_str());
    char text[256];
    sprintf(text, "%d %s(%s)", this->member.level, this->member.name.c_str(), mapName);
    std::string clipped = this->sub_A80880(this->geometry.width - 0x43, text, 0, 12, 1);
    this->sub_A84360(
      this->iconPositions[this->iconIndex].x + 0x1C,
      this->iconPositions[this->iconIndex].y + 3,
      clipped.c_str(),
      0,
      0,
      12,
      0x00B07F72,
      1,
      0
    );

    if (this->gauge) {
      char hpText[128];
      sprintf(hpText, "%d/%d", this->gauge->current, this->gauge->maximum);
      int len = static_cast<int>(strlen(hpText));
      this->sub_A84360(
        this->gauge->geometry.x + 0x42,
        this->gauge->geometry.y - 4,
        hpText,
        len,
        0,
        10,
        0,
        0,
        0
      );
    }

    memset(path, 0, 0x100);
    const char* statusIcon;
    if (this->member.offline == 0) {
      if (this->member.accountId == g_session.m_aid) {
        statusIcon = "%sicon_party_me.bmp";
      } else {
        statusIcon = "%sicon_party_on.bmp";
      }
    } else {
      statusIcon = "%sicon_party_off.bmp";
    }

    sprintf(path, statusIcon, prefix.c_str());
    const BitmapImage* statusBmp = reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (this->gauge) {
      this->DrawBitmap(
        static_cast<int>(this->geometry.width * 0.8f),
        this->gauge->geometry.y - 5,
        statusBmp,
        true
      );
    }
  }
}

// OFFSET: 0x00738630
// FUNCTION: fn_07668
void UIDragMiniPartyWnd::MoveGauge(int x, int y) {
  this->gauge->Move(x, y);
}

// OFFSET: 0x00738CE0
// FUNCTION: fn_07671
void UIDragMiniPartyWnd::Slot31(int argument1, int argument2) {
  auto* window = static_cast<UIMessengerGroupWnd*>(g_windowMgr.LookupWindow(34));
  if (window) {
    window->DropMember(argument1 + this->geometry.x, argument2 + this->geometry.y, this->unknown_0108);
  }
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

// OFFSET: 0x0089C360
// FUNCTION: fn_11586
void UIPcGage::SetLimits(int first, int second) {
  this->unknown_00A8 = first;
  this->unknown_00AC = second;
  this->Invalidate();
}

// OFFSET: 0x00738E10
// FUNCTION: fn_07679
UIFriendOptionWnd::UIFriendOptionWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->saved = nullptr;
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

// OFFSET: 0x00739330
// FUNCTION: fn_07685
UIJobIconButton::UIJobIconButton(UIWindowContext& windows, UIButtonContext& buttons) : UIBitmapButton(windows, buttons) {
  this->unknown_0120 = 0;
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

// OFFSET: 0x00739BF0
// FUNCTION: fn_07697
MiniPartyInfoEntry::MiniPartyInfoEntry(const char* text, int value) : text(text), value(value) {
}

// OFFSET: 0x00739C80

// OFFSET: 0x0073A080
// FUNCTION: fn_07700
UIMessengerGroupWnd::~UIMessengerGroupWnd() = default;

// OFFSET: 0x0073A1C0
// FUNCTION: fn_07702
__declspec(noinline) void UIMessengerGroupWnd::DropMember(int x, int y, int index) {
  g_windowMgr.RetireTransientWindow();
  if (x > this->geometry.x && x < this->geometry.x + this->geometry.width && y > this->geometry.y && y < this->geometry.y + this->geometry.height) {
    return;
  }

  MessengerMember member = this->MemberAtIndex(index);
  SessionRecord0734 record = g_session.FindRecord0734(member.accountId);
  UIWindow* window = g_windowMgr.LookupWindow(307);
  if (record.unknown_0000 != 1) {
    int top = static_cast<int>(static_cast<float>(g_renderer->viewport.width) * 0.3f);
    if (window) {
      top = window->geometry.y + window->geometry.height;
    }

    int cell;
    if (x < g_messengerDropColumnWidth + 23 && y > top) {
      int row = (y - top) / (g_messengerDropRowHeight + 3);
      if (g_windowMgr.FindSnapExcludedWindow(row)) {
        cell = g_windowMgr.FindMiniPartyCell();
      } else {
        cell = row;
      }
    } else {
      cell = g_windowMgr.FindMiniPartyCell();
    }

    if (x < 0 || x < g_windowMgr.unknown_047C || y < 0 || y < g_windowMgr.unknown_0480) {
      cell = g_windowMgr.FindMiniPartyCell();
    }

    this->OpenMiniPartyWnd(cell, member);
  }
}

// OFFSET: 0x0073A3F0
// FUNCTION: fn_07704
__declspec(noinline) MessengerMember UIMessengerGroupWnd::MemberAt(int x, int y) {
  this->selected = -1;
  if (x < 2 || x > this->geometry.width - 2) {
    return MessengerMember(0);
  }

  if (y >= 17 && y < this->visibleRows * this->rowHeight + 17) {
    this->selected = (y - 17) / this->rowHeight + this->scrollTop;
    int position = 0;
    for (auto it = this->members.begin(); it != this->members.end(); ++it, ++position) {
      if (position == this->selected) {
        return *it;
      }
    }

    this->selected = -1;
  }

  return MessengerMember(0);
}

// OFFSET: 0x0073A330
// FUNCTION: fn_07703
__declspec(noinline) void UIMessengerGroupWnd::OpenMiniPartyWnd(int cell, const MessengerMember& member) {
  UIMiniPartyWnd* window = new UIMiniPartyWnd(*this->m_context, *this->m_frameContext);
  window->SetSize(174, 52);
  window->m_cell = cell;
  g_windowMgr.AddMiniPartyWindow(window);
  window->SendMsg(nullptr, 0x17, reinterpret_cast<std::intptr_t>(&member), 0, 0, 0);
  g_session.AddRecord0734(member.accountId, cell);
}

// OFFSET: 0x0073A540
// FUNCTION: fn_07705
__declspec(noinline) MessengerMember UIMessengerGroupWnd::MemberAtIndex(int index) {
  int position = 0;
  for (auto it = this->members.begin(); it != this->members.end(); ++it, ++position) {
    if (index == position) {
      return *it;
    }
  }
  return MessengerMember(0);
}

// OFFSET: 0x0073B9D0
// FUNCTION: fn_07707
void UIMessengerGroupWnd::Paint() {
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, "", 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));
  DrawRectangleBorder(*this, 0, 17, this->geometry.width, this->geometry.height - 38, 0xC0C0C0);
  this->DrawButtonBar(0, 0, 1);

  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mesbtn_left.bmp")));
  this->DrawBitmap(0, this->geometry.height - 40, &left->image, false);
  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mesbtn_right.bmp")));
  this->DrawBitmap(this->geometry.width - right->image.width, this->geometry.height - 40, &right->image, false);
  const int span = this->geometry.width - 14;
  int count = span / 7 + (span % 7 != 0);
  for (int x = 7; count > 0; x += 7, --count) {
    const auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mesbtn_mid.bmp")));
    this->DrawBitmap(x, this->geometry.height - 40, &middle->image, false);
  }

  std::string prefix = "유저인터페이스\\renewalparty\\";
  char path[256];
  if (this->mode == 0) {
    memset(path, 0, sizeof(path));
    StringFormat(path, sizeof(path), static_cast<std::size_t>(-1), "%simg_friend1.bmp", prefix.c_str());
    const auto* friend1 = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (this->iconIndex == 0) {
      this->DrawBitmap(147, 36, &friend1->image, true);
    }
    StringFormat(path, sizeof(path), static_cast<std::size_t>(-1), "%simg_friend2.bmp", prefix.c_str());
    const auto* friend2 = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (this->iconIndex == 0) {
      this->DrawBitmap(99, 193, &friend2->image, true);
    }
  }

  for (int index = 0; index < 40; ++index) {
    this->jobIcons[index]->Move(-100, -100);
  }

  int shown = 0;
  int index = 0;
  for (auto it = this->members.begin(); it != this->members.end(); ++it, ++index) {
    if (index < this->scrollTop * this->columns) {
      continue;
    }

    const int x = shown % this->columns * 32 + this->iconPositions[this->iconIndex].x;
    const int top = this->rowHeight * (shown / this->columns) + 17;
    const std::uint32_t source = it->color;
    std::uint32_t color = ((source >> 16) & 0xFF) | ((source & 0xFF) << 16) | (source & 0xFF00);

    if (this->mode == 1) {
      MessengerMember entry = this->MemberAtIndex(index);
      SessionRecord0734 record = g_session.FindRecord0734(entry.accountId);
      if (record.unknown_0000 == 1) {
        this->FillRegion(0, top + 2, this->geometry.width, this->rowHeight, 0xFFE8DD);
      }
      if (this->selected == index) {
        unsigned selectRed, selectGreen, selectBlue;
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, selectRed, selectGreen, selectBlue);
        this->FillRegion(0, top + 2, this->geometry.width, this->rowHeight, RGB(selectRed, selectGreen, selectBlue));
      }

      std::string jobName = g_session.GetJobName(it->job, 99);
      this->jobIcons[index]->SetTooltip(jobName.c_str());
      this->jobIcons[index]->Move(x, this->iconPositions[this->iconIndex].y + top);

      if (it->leader == 0) {
        char crownPath[256];
        sprintf(crownPath, "%sico_partyCrown.bmp", prefix.c_str());
        const auto* crown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(crownPath)));
        this->DrawBitmap(x + 2, this->iconPositions[this->iconIndex].y + top - 8, &crown->image, true);
      }

      char level[64];
      sprintf(level, "Lv.%d", it->level);
      if (it->map.size() != 0 && it->offline == 0) {
        std::string mapName = g_session.GetMapDisplayName(it->map.c_str());
        sprintf(path, "%s(%s)", it->name.c_str(), mapName.c_str());
      } else {
        sprintf(path, "%s", it->name.c_str());
      }

      std::string clipped = this->FitDisplayText(static_cast<int>(this->geometry.width * 0.8 - 80.0), path, 0, 12, 1);
      std::uint32_t levelColor;
      if (it->offline == 1) {
        color = 0xA38A86;
        levelColor = 0xA38A86;
      } else {
        levelColor = 0x493A37;
      }
      this->DrawTextAt(
        this->m_context->fonts, this->m_context->locale, x + 28, this->iconPositions[this->iconIndex].y + top + 3, level,
        static_cast<int>(strlen(level)), 0, 12, levelColor, 1, 0
      );
      const int textY = this->iconPositions[this->iconIndex].y + top + 3;
      const int levelWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, level, static_cast<unsigned>(strlen(level)), 0, 12, 1, 0);
      this->DrawTextAt(
        this->m_context->fonts, this->m_context->locale, levelWidth + x + 33, textY, clipped.c_str(), static_cast<int>(clipped.size()), 0, 12, color,
        1, 0
      );

      if (it->offline == 0) {
        UIPcGage* gauge = this->gauges[index];
        if (gauge->geometry.x != -100) {
          char hpText[128];
          sprintf(hpText, "%d/%d", gauge->current, gauge->maximum);
          this->DrawTextAt(
            this->m_context->fonts, this->m_context->locale, this->gauges[index]->geometry.x + 66, this->gauges[index]->geometry.y - 2, hpText,
            static_cast<int>(strlen(hpText)), 0, 10, 0, 0, 0
          );
        }
      }

      memset(path, 0, sizeof(path));
      const char* statusIcon;
      if (it->offline) {
        statusIcon = "%sicon_party_off.bmp";
      } else if (it->accountId == g_session.m_aid) {
        statusIcon = "%sicon_party_me.bmp";
      } else {
        statusIcon = "%sicon_party_on.bmp";
      }
      sprintf(path, statusIcon, prefix.c_str());
      const auto* status = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
      this->DrawBitmap(
        static_cast<int>(static_cast<float>(this->geometry.width) * 0.8f),
        static_cast<int>(static_cast<float>(this->rowHeight) * 0.65f + static_cast<float>(top)), &status->image, true
      );

      memset(path, 0, sizeof(path));
      sprintf(path, "%sbg_partymember.bmp", prefix.c_str());
      const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
      this->DrawTextAt(
        this->m_context->fonts, this->m_context->locale, this->geometry.width - 96, this->geometry.height - 16, LookupMessage(3231), 0, 0, 12, 0, 0, 0
      );
      this->DrawBitmap(this->geometry.width - 58, this->geometry.height - 18, &background->image, true);

      memset(path, 0, sizeof(path));
      sprintf(path, "%d/%d", ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount(), 12);
      this->DrawTextAt(
        this->m_context->fonts, this->m_context->locale, this->geometry.width - 51, this->geometry.height - 16, path,
        static_cast<int>(strlen(path)), 0, 12, 0xFFFFFF, 0, 0
      );
    } else {
      MessengerMember entry = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.FriendAt(index);
      if (g_modeMgr.GetGameMode() && entry.valid) {
        entry.name = LookupPlayerName(entry.characterId);
      }

      if (entry.name.size() == 0) {
        entry.name = LookupMessage(581) + g_messengerNamelessTag;
      } else if (!g_session.IsFriendName(entry.name.c_str())) {
        g_session.UpdateFriendName(entry.characterId, entry.name.c_str());
      }

      char nameText[80];
      memset(nameText, 0, sizeof(nameText));
      std::string display = entry.name;
      const std::size_t position = entry.name.find(g_messengerNamelessTag);
      if (position != std::string::npos) {
        display.clear();
        display = entry.name.substr(0, position);
      }
      sprintf(nameText, "%s", display.c_str());

      if (this->selected == index) {
        unsigned selectRed, selectGreen, selectBlue;
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, selectRed, selectGreen, selectBlue);
        this->FillRegion(
          x + 17, top + 4, this->TextWidth(this->m_context->fonts, this->m_context->locale, nameText, 0, 0, 12, 0, 0) + 3, 16,
          RGB(selectRed, selectGreen, selectBlue)
        );
      }

      this->DrawTextAt(
        this->m_context->fonts, this->m_context->locale, x + 19, top + 6, nameText, static_cast<int>(strlen(nameText)), 0, 12, color, 0, 0
      );

      if (it->offline == 0) {
        char onlinePath[256];
        sprintf(onlinePath, "%s", "유저인터페이스\\basic_interface\\grp_online.bmp");
        const auto* online = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(onlinePath)));
        this->DrawBitmap(x + 2, top + 4, &online->image, true);
      }
    }

    if (++shown == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x0073CD30
// FUNCTION: fn_07708
void UIMessengerGroupWnd::Slot26(int, int) {
  if (!g_messengerGroupCollapsed) {
    this->SendMsg(nullptr, 6, 232, 0, 0, 0);
  }
}

// OFFSET: 0x0073D460
// FUNCTION: fn_07713
void UIMessengerGroupWnd::Slot25(int x, int y) {
  if (g_messengerGroupCollapsed) {
    return;
  }

  const int previous = this->selected;
  this->selected = -1;

  if (x >= 2 && x <= this->geometry.width - 2 && y > 16 && y < this->visibleRows * this->rowHeight + 17) {
    const int index = (y - 17) / this->rowHeight + this->scrollTop;
    int position = 0;
    for (auto member = this->members.begin(); member != this->members.end(); ++member, ++position) {
      if (position == index) {
        this->selected = index;
        break;
      }
    }
  }

  if (previous != this->selected) {
    this->Invalidate();
  }

  if (this->selected != -1) {
    this->SendMsg(nullptr, 49, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0073D080
// FUNCTION: fn_07710
void UIMessengerGroupWnd::Slot31(int x, int y) {
  UIFrameWnd::Slot31(x, y);
  if (UIWindow* window = this->CompanionWindow()) {
    window->geometry.targetAlpha = 255;
    window->geometry.alphaStart = timeGetTime();
  }

  UIWindow* transient = g_windowMgr.window_4F250;
  if (transient && static_cast<UIDragMiniPartyWnd*>(transient)->member.valid) {
    transient->geometry.unknown_0030 = 1;
    g_windowMgr.Schedule(g_windowMgr.window_4F250);
    g_windowMgr.window_4F250 = nullptr;
  }
}

// OFFSET: 0x0073D0E0
// FUNCTION: fn_07711
void UIMessengerGroupWnd::Slot28(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot28(x, y);
  }

  if (this->mode == 0) {
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (!mode) {
    return;
  }

  const int previous = this->selected;
  const MessengerMember member = this->MemberAt(x, y);
  if (previous != this->selected) {
    this->Invalidate();
  }

  if (!member.valid) {
    return;
  }

  char text[256];
  std::memset(text, 0, sizeof(text));
  if (member.map.size() != 0 && member.offline == 0) {
    const std::string mapName = g_session.GetMapDisplayName(member.map.c_str());
    std::sprintf(text, "%s(%s)", member.name.c_str(), mapName.c_str());
  } else {
    std::sprintf(text, "%s", member.name.c_str());
  }

  const std::string fitted = this->FitDisplayText(static_cast<int>(this->geometry.width * 0.8 - 80.0), text, 0, 12, 1);
  if (fitted.find(text, 0, std::strlen(text)) == std::string::npos) {
    char level[64];
    std::sprintf(level, "Lv.%d", member.level);
    int tipX = this->TextWidth(this->m_context->fonts, this->m_context->locale, level, std::strlen(level), 0, 12, 1, 0) +
               this->iconPositions[this->iconIndex].x + 33;
    int tipY = this->selected / this->columns * this->rowHeight + 22;
    this->ToScreen(tipX, tipY);
    reinterpret_cast<ModeTooltip*>(mode)->Show(text, tipX, tipY, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x0073D3F0
// FUNCTION: fn_07712
void UIMessengerGroupWnd::Slot27(int x, int y) {
  UIFrameWnd::Slot27(x, y);
  if (UIWindow* window = this->CompanionWindow()) {
    x = 0;
    y = 0;
    this->ToScreen(x, y);
    window->Move(x + 2 + this->geometry.width, y);
  }
}

// OFFSET: 0x0073D530
// FUNCTION: fn_07714
void UIMessengerGroupWnd::Slot35(int delta) {
  if (this->scrollable) {
    int step = this->visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, 7, delta < 0 ? step : -step, 0, 0, 0);
  }
}

// OFFSET: 0x0073D580
// FUNCTION: fn_07715
void UIMessengerGroupWnd::UpdateScroll() {
  if (!this->scrollBar) {
    return;
  }

  const int rows = this->members.size() / this->columns + (this->members.size() % this->columns != 0);
  const int visible = this->visibleRows;
  int range = 0;
  if (rows - visible >= 0) {
    range = rows - visible;
  }
  this->scrollable = range != 0;
  if (range) {
    this->scrollBar->SetRange(range, visible, rows);
    int top = this->scrollTop;
    if (top < 0) {
      this->scrollTop = 0;
      top = 0;
    }
    const int limit = rows - this->visibleRows;
    if (top > limit) {
      this->scrollTop = limit;
      top = limit;
    }
    this->scrollBar->SetPosition(top);
  } else {
    this->scrollTop = 0;
    this->scrollBar->SetRange(0, visible, rows);
  }
  this->scrollBar->Invalidate();
  this->UpdateGauges();
}

// OFFSET: 0x0073D640
// FUNCTION: fn_07716
void UIMessengerGroupWnd::LayoutButtons() {
  int hidden = 0;
  for (int index = 0; index < 11; ++index) {
    if (this->mode == 1 && this->hiddenButtons[index]) {
      this->buttons[index]->Move(-100, -100);
      ++hidden;
    } else {
      this->buttons[index]->Move((index - hidden) * 26 + 5, this->geometry.height - 40);
    }
  }

  int x;
  if (this->mode == 1) {
    const int offset = hidden * -26;
    if (ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount()) {
      this->buttons[10]->Move(offset + 213, this->geometry.height - 40);
      this->buttons[9]->Move(offset + 161, this->geometry.height - 40);
      this->buttons[8]->Move(offset + 187, this->geometry.height - 40);
      this->buttons[7]->Move(-100, -100);
    } else {
      this->buttons[10]->Move(-100, -100);
      this->buttons[7]->Move(offset + 161, this->geometry.height - 40);
      this->buttons[8]->Move(-100, -100);
      this->buttons[9]->Move(-100, -100);
    }
    x = offset + 135;
  } else {
    this->buttons[7]->Move(-100, -100);
    this->buttons[8]->Move(-100, -100);
    this->buttons[9]->Move(-100, -100);
    this->buttons[10]->Move(-100, -100);
    x = 135;
  }

  if (g_messengerGroupCollapsed) {
    this->buttons[5]->Move(x, this->geometry.height - 40);
    this->buttons[6]->Move(-100, -100);
  } else {
    this->buttons[6]->Move(x, this->geometry.height - 40);
    this->buttons[5]->Move(-100, -100);
  }
}

// OFFSET: 0x0073D800
// FUNCTION: fn_07717
void UIMessengerGroupWnd::UpdateGauges() {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!gameMode || !this->mode) {
    return;
  }

  int shown = 0;
  int index = 0;
  UIPcGage** gauge = this->gauges;
  for (auto it = this->members.begin(); it != this->members.end(); ++it, ++index, ++gauge) {
    if (index < this->scrollTop * this->columns) {
      if ((*gauge)->geometry.x != -100) {
        (*gauge)->Move(-100, -100);
      }
      continue;
    }

    if (it->valid) {
      CPc* pc;
      if (it->accountId == g_session.m_aid) {
        pc = gameMode->scene->player;
      } else {
        pc = dynamic_cast<CPc*>(static_cast<CGameActor*>(gameMode->scene->GetGameActorByAID(it->accountId)));
      }

      const int y = shown / this->columns * this->rowHeight + 17;
      if ((*gauge)->geometry.y != y) {
        (*gauge)->Move(47, static_cast<int>(static_cast<float>(this->rowHeight) * 0.76f + static_cast<float>(y)));
      }
      if (pc && !it->offline) {
        (*gauge)->SetColor(0x444444);
      } else if ((*gauge)->geometry.x != -100) {
        (*gauge)->Move(-100, -100);
      }
    }

    if (++shown > this->visibleRows * this->columns && (*gauge)->geometry.x != -100) {
      (*gauge)->Move(-100, -100);
    }
  }
}

// OFFSET: 0x0073D980

// OFFSET: 0x0089CFD0
// FUNCTION: fn_11596
void UIGage::SetColor(std::uint32_t color) {
  this->colors[0] = color;
}

// OFFSET: 0x0073FD90
// FUNCTION: fn_07719
void UIMessengerGroupWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->unknown_0014 = this->mode;
    this->saved->unknown_0018 = this->iconIndex;
  }
}

// OFFSET: 0x0073FFC0
// FUNCTION: fn_07726
UIMiniPartyInfoWnd::UIMiniPartyInfoWnd(UIWindowContext& windowContext, UIFrameContext& frameContext)
    : UIFrameWnd(windowContext, frameContext, 0), m_gauge(nullptr), m_owner(nullptr) {
  this->m_entries.clear();
}

// OFFSET: 0x00740170
// FUNCTION: fn_07728
MessengerMember UIMiniPartyWnd::GetMember() {
  return this->m_member;
}

// OFFSET: 0x00740250
// FUNCTION: fn_07729
void UIMiniPartyInfoWnd::Slot15(int width, int) {
  if (!this->m_gauge) {
    this->m_gauge = new UIPcGage();
    this->m_gauge->SetSize(static_cast<int>(static_cast<float>(width) * 0.38f), 5);
    this->m_gauge->Move(35, 23);
    this->AddChild(this->m_gauge);
  }
}

// OFFSET: 0x00AA24D0
// FUNCTION: fn_17213
void UIWindowMgr::MoveTransient(int x, int y) {
  UIDragMiniPartyWnd* transient = static_cast<UIDragMiniPartyWnd*>(this->window_4F250);
  if (transient && transient->member.valid) {
    transient->Move(x - transient->unknown_010C, y - transient->unknown_0110);
  }
}

// OFFSET: 0x00AA9E90
// FUNCTION: fn_17262
__declspec(noinline) void UIWindowMgr::RetireTransientWindow() {
  UIDragMiniPartyWnd* transient = static_cast<UIDragMiniPartyWnd*>(this->window_4F250);
  if (transient && transient->member.valid) {
    transient->geometry.unknown_0030 = 1;
    this->Schedule(transient);
    this->window_4F250 = nullptr;
  }
}
