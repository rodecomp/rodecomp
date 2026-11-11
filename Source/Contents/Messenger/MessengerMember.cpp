#include "Contents/Messenger/MessengerMember.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Mouse.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/TextureNames.h"
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
#include "Client/Session/Session.h"
#include "Render/Text/TextContext.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Text/UIStaticText.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Client/Mode/CMode.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <timeapi.h>
#include <string>
#include <vector>

namespace {
#pragma pack(push, 1)
  struct MessengerRecordHeader {
    // INLINED
    MessengerRecordHeader() : id(0), bytes(0) {
    }

    // INLINED
    ~MessengerRecordHeader() {
      this->id = 0;
      this->bytes = 0;
    }

    // INLINED
    void Clear() {
      this->bytes = 0;
      this->id = 0;
    }

    /* 0000 */ std::int16_t id;
    /* 0002 */ std::uint32_t bytes;
  };
#pragma pack(pop)

  struct MiniPartyIconPosition {
    /* 0000 */ int x;
    /* 0004 */ int y;
    /* 0008 */ int size;
  };

  struct MessengerGroupIconPosition {
    /* 0000 */ int x;
    /* 0004 */ int y;
    /* 0008 */ int size;
  };

}

class UIDragMiniPartyWnd : public UIFrameWnd {
public:
  __declspec(noinline) UIDragMiniPartyWnd(MessengerMember member);
  ~UIDragMiniPartyWnd() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnUp(int argument1, int argument2) override;
  void MoveGauge(int x, int y);
  void RemoveGauge();
  void SetDragStartPosition(int first, int second);
  void SetGaugeValues(int current, int maximum);

  /* 00B4 */ MessengerMember member;
  /* 0104 */ UIPcGage* gauge;
  /* 0108 */ int memberIndex;
  /* 010C */ int dragStartX;
  /* 0110 */ int dragStartY;
  /* 0114 */ int iconIndex;
  /* 0118 */ std::vector<MiniPartyIconPosition> iconPositions;
};

class UIFriendOptionWnd : public UIFrameWnd {
public:
  UIFriendOptionWnd();
  ~UIFriendOptionWnd() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ FriendOptionWndInfo* saved;
  /* 00B8 */ UICheckBox* option1Box;
  /* 00BC */ UICheckBox* option2Box;
  /* 00C0 */ UICheckBox* option0Box;
  /* 00C4 */ UIBitmapButton* closeButton;
};

class UIJobIconButton : public UIBitmapButton {
public:
  __declspec(noinline) UIJobIconButton();
  ~UIJobIconButton() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnUp(int x, int y) override;

  /* 0120 */ int offline;
};

class UIMessengerGroupWnd : public UIFrameWnd {
public:
  UIMessengerGroupWnd();
  ~UIMessengerGroupWnd() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnWheel(int delta) override;
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
      return reinterpret_cast<UIWindow*>(g_windowMgr.friendOptionWnd);
    }
    UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.partyInvitationToWnd);
    if (g_windowMgr.partySettingWnd) {
      window = reinterpret_cast<UIWindow*>(g_windowMgr.partySettingWnd);
    }
    return window;
  }

  /* 00B4 */ MessengerGroupWndInfo* saved;
  /* 00B8 */ UIScrollBar* scrollBar;
  /* 00BC */ UIRadioBtn* tabs[2];
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
  /* 026C */ std::vector<MessengerGroupIconPosition> iconPositions;
  /* 0278 */ UIButton* iconModeButtons[2];
  /* 0280 */ unsigned char hiddenButtons[11];
  /* 028C */ int mode;
};

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

// OFFSET: 0x007382E0
// FUNCTION: fn_07664
UIDragMiniPartyWnd::UIDragMiniPartyWnd(MessengerMember member)
    : UIFrameWnd(0), member(member), gauge(nullptr), iconIndex(0) {
  this->geometry.state = 0;
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
  MessengerRecordHeader header;
  header.Clear();
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 251) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Clear();
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    switch (header.id) {
    case 252:
      return offset;
    case 253:
      std::memcpy(&this->valid, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 254:
      std::memcpy(&this->accountId, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 255:
      std::memcpy(&this->characterId, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 258:
      std::memcpy(&this->leader, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 259:
      std::memcpy(&this->offline, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 260:
      std::memcpy(&this->color, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 256: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->name = buffer;
      break;
    }
    case 257: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->map = buffer;
      break;
    }
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
void UIDragMiniPartyWnd::OnCreate(int, int) {
  this->iconPositions.resize(2);
  this->iconPositions[0] = {5, 5, 34};
  this->iconPositions[1] = {4, 1, 27};

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
      dead = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->isPlayerDead == 1;
    }

    int yOffset = this->member.leader ? 0 : 2;
    const char* jobFmt = dead ? "%sicon_jobs_%d_die.bmp" : "%sicon_jobs_%d.bmp";
    char path[256];
    sprintf(path, jobFmt, prefix.c_str(), this->member.job);
    this->DrawBitmap(
      this->iconPositions[this->iconIndex].x,
      this->iconPositions[this->iconIndex].y + yOffset,
      static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path))),
      true
    );

    if (this->member.leader == 0) {
      sprintf(path, "%sico_partyCrown.bmp", prefix.c_str());
      this->DrawBitmap(
        this->iconPositions[this->iconIndex].x + 2,
        0,
        static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path))),
        true
      );
    }

    const std::string mapName = g_session.GetMapDisplayName(this->member.map.c_str());
    char text[256];
    sprintf(text, "%d %s(%s)", this->member.level, this->member.name.c_str(), mapName.c_str());
    std::string clipped = this->FitDisplayText(this->geometry.width - 0x43, text, 0, 12, 1);
    this->DrawTextAt(
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
      this->DrawTextAt(
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
    CBitmapRes* statusBmp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
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
__declspec(noinline) void UIDragMiniPartyWnd::MoveGauge(int x, int y) {
  this->gauge->Move(x, y);
}

// OFFSET: 0x00738CE0
// FUNCTION: fn_07671
void UIDragMiniPartyWnd::OnLBtnUp(int argument1, int argument2) {
  auto* window = static_cast<UIMessengerGroupWnd*>(g_windowMgr.LookupWindow(WID_MESSENGERGROUPWND));
  if (window) {
    window->DropMember(argument1 + this->geometry.x, argument2 + this->geometry.y, this->memberIndex);
  }
}

// OFFSET: 0x00738D20
// FUNCTION: fn_07672
__declspec(noinline) void UIDragMiniPartyWnd::RemoveGauge() {
  this->RemoveChild(this->gauge, 1);
  this->gauge = nullptr;
}

// OFFSET: 0x00738D50
// FUNCTION: fn_07674
__declspec(noinline) void UIDragMiniPartyWnd::SetDragStartPosition(int first, int second) {
  this->dragStartX = first;
  this->dragStartY = second;
}

// OFFSET: 0x00738D40
// FUNCTION: fn_07673
__declspec(noinline) void UIDragMiniPartyWnd::SetGaugeValues(int current, int maximum) {
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
  this->secondaryValue = first;
  this->secondaryMaximum = second;
  this->Invalidate();
}

// OFFSET: 0x00738E10
// FUNCTION: fn_07679
UIFriendOptionWnd::UIFriendOptionWnd() : UIFrameWnd(0) {
  this->saved = nullptr;
}

// OFFSET: 0x00738EC0
// FUNCTION: fn_07680
UIFriendOptionWnd::~UIFriendOptionWnd() = default;

// OFFSET: 0x00738F30
// FUNCTION: fn_07681
void UIFriendOptionWnd::OnCreate(int, int) {

  int width = this->TextWidth(LookupMessage(MSI_AUTOOPEN_1ON1_WINDOW), 0, 0, 12, 0, 0) + 12;
  width = std::min(width, this->geometry.width - 15);
  this->option0Box = new UICheckBox();
  this->option0Box->SetSize(width, 12);
  this->option0Box->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_AUTOOPEN_1ON1_WINDOW));
  this->option0Box->Move(6, 21);
  this->option0Box->SetChecked(g_session.m_autoOpenWhisperWindow);
  this->AddChild(this->option0Box);

  width = this->TextWidth(LookupMessage(MSI_AUTOOPENWHISPERLISTWND_FRIEND), 0, 0, 12, 0, 0) + 12;
  width = std::min(width, this->geometry.width - 15);
  this->option2Box = new UICheckBox();
  this->option2Box->SetSize(width, 12);
  this->option2Box->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_AUTOOPENWHISPERLISTWND_FRIEND));
  this->option2Box->Move(6, 37);
  this->option2Box->SetChecked(g_session.m_autoOpenFriendWhisperWindow);
  this->AddChild(this->option2Box);

  width = this->TextWidth(LookupMessage(MSI_WHISPER_OPEN_SOUND), 0, 0, 12, 0, 0) + 12;
  width = std::min(width, this->geometry.width - 15);
  this->option1Box = new UICheckBox();
  this->option1Box->SetSize(width, 12);
  this->option1Box->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_WHISPER_OPEN_SOUND));
  this->option1Box->Move(6, 53);
  this->option1Box->SetChecked(g_session.m_whisperOpenSound);
  this->AddChild(this->option1Box);

  this->closeButton = this->AddCloseButton("Alt+I", 0);
}

// OFFSET: 0x007391C0
// FUNCTION: fn_07682
void UIFriendOptionWnd::Paint() {

  const int titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_FRIEND_SETTING), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  const int barHeight = this->DrawButtonBar(0, 0, 1);
  this->FillRegion(0, this->geometry.height - barHeight, this->geometry.width, 1, 0xC0C0C0);
}

// OFFSET: 0x00739220
// FUNCTION: fn_07683
int UIFriendOptionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND);
      break;
    case ID_CHECK:
      if (this->option0Box == sender) {
        g_session.m_autoOpenWhisperWindow = static_cast<int>(b);
      } else if (this->option2Box == sender) {
        g_session.m_autoOpenFriendWhisperWindow = static_cast<int>(b);
      } else if (this->option1Box == sender) {
        g_session.m_whisperOpenSound = static_cast<int>(b);
      }
      break;
    }
    return 0;
  case UM_SETINFO:
    this->saved = reinterpret_cast<FriendOptionWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->Move(this->saved->x, this->saved->y);
    return 0;
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x009A54B0
// FUNCTION: fn_13630
int FriendOptionWndInfo::IsValid() const {
  return this->width == 280 && this->height == 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - 120;
}

// OFFSET: 0x007392F0
// FUNCTION: fn_07684
void UIFriendOptionWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00739330
// FUNCTION: fn_07685
UIJobIconButton::UIJobIconButton() : UIBitmapButton() {
  this->offline = 0;
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
    switch (this->geometry.state) {
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
      if (this->offline == 1) {
        this->DrawAuxiliary(1, 0, 0, &bitmap->image, true, false);
      } else {
        this->DrawBitmap(0, 0, bitmap, true);
      }
    }
  }

  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x007395E0
// FUNCTION: fn_07688
void UIJobIconButton::OnLBtnDown(int x, int y) {
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MESSENGERGROUPWND)) {
    window->OnLBtnDown(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00739610
// FUNCTION: fn_07689
void UIJobIconButton::OnLBtnUp(int x, int y) {
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MESSENGERGROUPWND)) {
    window->OnLBtnUp(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00739640
// FUNCTION: fn_07690
void UIJobIconButton::OnRBtnUp(int x, int y) {
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MESSENGERGROUPWND)) {
    window->OnRBtnUp(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00739BF0
// FUNCTION: fn_07697
MiniPartyInfoEntry::MiniPartyInfoEntry(const char* text, int value) : text(text), value(value) {
}

// OFFSET: 0x00739C80
// FUNCTION: fn_07698
UIMessengerGroupWnd::UIMessengerGroupWnd() : UIFrameWnd(0) {
  this->saved = nullptr;
  this->scrollBar = nullptr;
  this->scrollTop = 0;
  this->columns = 1;
  this->visibleRows = 5;
  this->scrollable = 0;
  this->rowHeight = 23;
  this->iconModeButtons[1] = nullptr;
  this->iconModeButtons[0] = nullptr;
  this->iconIndex = 0;
  for (int i = 0; i < 11; ++i) {
    this->hiddenButtons[i] = 0;
  }
}

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
  SessionRecord0734 record = g_session.FindMiniPartyRecord(member.accountId);
  UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND);
  if (record.active != 1) {
    int top = static_cast<int>(static_cast<float>(g_renderer->viewport.height) * 0.3f);
    if (window) {
      top = window->geometry.y + window->geometry.height;
    }

    int cell;
    if (x < g_windowMgr.miniPartyWidth + 23 && y > top) {
      int row = (y - top) / (g_windowMgr.miniPartyHeight + 3);
      cell = row;
      if (g_windowMgr.FindSnapExcludedWindow(row)) {
        cell = g_windowMgr.FindMiniPartyCell();
      }
    } else {
      cell = g_windowMgr.FindMiniPartyCell();
    }

    if (x < 0 || x < g_windowMgr.screenWidth || y < 0 || y < g_windowMgr.screenHeight) {
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
  UIMiniPartyWnd* window = new UIMiniPartyWnd();
  window->SetSize(174, 52);
  window->m_cell = cell;
  g_windowMgr.AddMiniPartyWindow(window);
  window->SendMsg(nullptr, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&member), 0, 0, 0);
  g_session.AddMiniPartyRecord(member.accountId, cell);
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

// OFFSET: 0x0073A630
// FUNCTION: fn_07706
void UIMessengerGroupWnd::OnCreate(int, int) {
  this->iconPositions.resize(2);
  this->iconPositions[0] = {5, 5, 34};
  this->iconPositions[1] = {4, 1, 27};

  this->iconModeButtons[0] = this->AddMinimizeButton("", 0);
  this->iconModeButtons[1] = new UIBitmapButton();
  static_cast<UIBitmapButton*>(this->iconModeButtons[1])->SetBitmap("유저인터페이스\\renewalparty\\btn_titlebar_maximum_out.bmp", 0, 0);
  static_cast<UIBitmapButton*>(this->iconModeButtons[1])->SetBitmap("유저인터페이스\\renewalparty\\btn_titlebar_maximum_over.bmp", 1, 0);
  static_cast<UIBitmapButton*>(this->iconModeButtons[1])->SetBitmap("유저인터페이스\\renewalparty\\btn_titlebar_maximum_press.bmp", 2, 0);
  {
    UIBitmapButton* maximize = static_cast<UIBitmapButton*>(this->iconModeButtons[1]);
    maximize->SetSize(maximize->BitmapWidth(), maximize->BitmapHeight());
  }
  this->iconModeButtons[1]->Move(this->geometry.width - 28, 3);
  this->iconModeButtons[1]->SetId(523);
  this->AddChild(this->iconModeButtons[1]);

  for (int index = 0; index < 40; ++index) {
    this->gauges[index] = new UIPcGage();
    this->gauges[index]->SetSize(60, 5);
    this->gauges[index]->Move(-100, -100);
    this->AddChild(this->gauges[index]);
  }

  for (int index = 0; index < 40; ++index) {
    this->jobIcons[index] = new UIJobIconButton();
    this->jobIcons[index]->SetSize(50, 50);
    this->jobIcons[index]->SetBitmap("유저인터페이스\\renewalparty\\icon_jobs_4046.bmp", 0, 0);
    this->jobIcons[index]->SetBitmap("유저인터페이스\\renewalparty\\icon_jobs_4046.bmp", 1, 0);
    this->jobIcons[index]->SetBitmap("유저인터페이스\\renewalparty\\icon_jobs_4046.bmp", 2, 0);
    this->AddChild(this->jobIcons[index]);
  }

  this->scrollBar = new UIScrollBar();
  this->scrollBar->Configure(this->geometry.width - 15, 17, 1, this->geometry.height - 58);
  this->AddChild(this->scrollBar);
  this->mode = 1;

  {
    const char* tabLabels[2] = {LookupMessage(MSI_FRIEND), LookupMessage(MSI_PARTY)};
    int tabWidths[2] = {48, this->TextWidth(LookupMessage(MSI_PARTY), 0, 0, 12, 0, 0) + 13};
    int tabValues[2] = {0, 1};
    for (int index = 0; index < 2; ++index) {
      this->tabs[index] = new UIRadioBtn();
      this->tabs[index]->SetSize(tabWidths[index], 12);
      this->tabs[index]->Move(index * 60 + 4, this->geometry.height - 16);
      this->tabs[index]->SetText(tabLabels[index]);
      this->tabs[index]->SendMsg(this, UM_SETSTATE, tabValues[index], 0, 0, 0);
      this->AddChild(this->tabs[index]);
    }
  }

  char title[64];
  const char* titles[2] = {LookupMessage(MSI_FRIEND), LookupMessage(MSI_PARTY)};
  if (this->mode == 1) {
    if (g_session.partyName.size()) {
      sprintf(title, "%s(%s)", titles[1], g_session.partyName.c_str());
    } else {
      sprintf(title, "%s", titles[1]);
    }
  } else {
    sprintf(title, "%s(%d/%d)", titles[this->mode], this->members.size(), 40);
  }

  this->titleText = new UIStaticText();
  this->titleText->SetSize(this->geometry.width - 57, 15);
  this->titleText->SetTextFitted(title);
  this->titleText->Move(16, 2);
  this->titleText->shadow = 1;
  this->AddChild(this->titleText);
  this->closeButton = this->AddCloseButton("Alt+Z", 0);

  {
    std::string prefix = "유저인터페이스\\basic_interface\\mesbtn_0";
    int ids[11] = {544, 232, 233, 234, 235, 377, 377, 445, 446, 447, 364};
    const char* friendTooltips[10] = {
      LookupMessage(MSI_MSG),  LookupMessage(MSI_1ON1),  LookupMessage(MSI_CHARACTER_INFO),  LookupMessage(MSI_FRIEND_SETTING), LookupMessage(MSI_DELETE),
      LookupMessage(MSI_LOCK_MOUSE), LookupMessage(MSI_UNLOCK_MOUSE), LookupMessage(MSI_CREATE_PARTY), LookupMessage(MSI_WITHDRAW_PARTY), LookupMessage(MSI_INVITE_PARTY),
    };
    const char* partyTooltips[11] = {
      LookupMessage(MSI_CANNOT_MORE_SELECT_JOB), LookupMessage(MSI_1ON1),  LookupMessage(MSI_CHARACTER_INFO),  LookupMessage(MSI_PARTY_SETTING),  LookupMessage(MSI_EXPEL),  LookupMessage(MSI_LOCK_MOUSE),
      LookupMessage(MSI_UNLOCK_MOUSE), LookupMessage(MSI_CREATE_PARTY), LookupMessage(MSI_WITHDRAW_PARTY), LookupMessage(MSI_INVITE_PARTY), LookupMessage(MSI_FAILED_ALL_RODEX_LIST),
    };
    unsigned char disabled[11] = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    this->hiddenButtons[0] = 0;
    this->hiddenButtons[1] = 1;
    this->hiddenButtons[2] = 1;
    this->hiddenButtons[3] = 0;
    this->hiddenButtons[4] = 1;
    this->hiddenButtons[5] = 0;
    this->hiddenButtons[6] = 0;
    this->hiddenButtons[7] = 0;
    this->hiddenButtons[8] = 0;
    this->hiddenButtons[9] = 0;
    this->hiddenButtons[10] = 0;
    int disabledIndices[2] = {0, 2};

    int index = 0;
    for (int x = 5; x < 291; x += 26, ++index) {
      char number[8];
      _itoa(index + 1, number, 10);
      this->buttons[index] = new UIBitmapButton();

      bool useDisabledImage = false;
      for (int entry = 0; entry < 2; ++entry) {
        if (index == disabledIndices[entry]) {
          useDisabledImage = true;
          break;
        }
      }

      if (useDisabledImage == true) {
        static_cast<UIBitmapButton*>(this->buttons[index])->SetBitmap((prefix + number + "_disable.bmp").c_str(), 0, 0);
        static_cast<UIBitmapButton*>(this->buttons[index])->SetBitmap((prefix + number + "_disable.bmp").c_str(), 1, 0);
        static_cast<UIBitmapButton*>(this->buttons[index])->SetBitmap((prefix + number + "_disable.bmp").c_str(), 2, 0);
      } else {
        static_cast<UIBitmapButton*>(this->buttons[index])->SetBitmap((prefix + number + ".bmp").c_str(), 0, 0);
        static_cast<UIBitmapButton*>(this->buttons[index])->SetBitmap((prefix + number + "_a.bmp").c_str(), 1, 0);
        static_cast<UIBitmapButton*>(this->buttons[index])->SetBitmap((prefix + number + "_b.bmp").c_str(), 2, 0);
      }

      this->buttons[index]->SetSize(
        static_cast<UIBitmapButton*>(this->buttons[index])->BitmapWidth(),
        static_cast<UIBitmapButton*>(this->buttons[index])->BitmapHeight()
      );
      this->buttons[index]->Move(x, this->geometry.height - 40);
      this->buttons[index]->SetId(ids[index]);
      static_cast<UIBitmapButton*>(this->buttons[index])->SetTooltip(this->mode == 0 ? friendTooltips[index] : partyTooltips[index]);
      static_cast<UIBitmapButton*>(this->buttons[index])->SetDisabled(disabled[index]);
      this->buttons[index]->Invalidate();
      this->AddChild(this->buttons[index]);
    }
  }

  UIResizer* resizer = new UIResizer();
  this->resizeButton = resizer;
  resizer->SetSize(16, 16);
  resizer->SetDirection(7);
  resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizeButton);
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x0073B9D0
// FUNCTION: fn_07707
void UIMessengerGroupWnd::Paint() {
  this->DrawTitleBar(true, "", 0);
  {
    unsigned red, green, blue;
    SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));
  }
  DrawRectangleBorder(*this, 0, 17, this->geometry.width, this->geometry.height - 38, 0xC0C0C0);
  this->DrawButtonBar(0, 0, 1);

  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mesbtn_left.bmp")));
  this->DrawBitmap(0, this->geometry.height - 40, left, false);
  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mesbtn_right.bmp")));
  this->DrawBitmap(this->geometry.width - right->image.width, this->geometry.height - 40, right, false);
  const int count = (this->geometry.width - 14) / 7 + ((this->geometry.width - 14) % 7 ? 1 : 0);
  for (int index = 0; index < count; ++index) {
    auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mesbtn_mid.bmp")));
    this->DrawBitmap(index * 7 + 7, this->geometry.height - 40, middle, false);
  }

  std::string prefix = "유저인터페이스\\renewalparty\\";
  char path[256];
  if (this->mode == 0) {
    memset(path, 0, sizeof(path));
    StringFormat(path, sizeof(path), static_cast<std::size_t>(-1), "%simg_friend1.bmp", prefix.c_str());
    auto* friend1 = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (this->iconIndex == 0) {
      this->DrawBitmap(147, 36, friend1, true);
    }
    StringFormat(path, sizeof(path), static_cast<std::size_t>(-1), "%simg_friend2.bmp", prefix.c_str());
    auto* friend2 = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (this->iconIndex == 0) {
      this->DrawBitmap(99, 193, friend2, true);
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
    char crownPath[256];

    if (this->mode == 1) {
      MessengerMember entry = this->MemberAtIndex(index);
      SessionRecord0734 record = g_session.FindMiniPartyRecord(entry.accountId);
      if (record.active == 1) {
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
        sprintf(crownPath, "%sico_partyCrown.bmp", prefix.c_str());
        auto* crown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(crownPath)));
        this->DrawBitmap(x + 2, this->iconPositions[this->iconIndex].y + top - 8, crown, true);
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
      this->DrawTextAt(x + 28, this->iconPositions[this->iconIndex].y + top + 3, level,
        static_cast<int>(strlen(level)), 0, 12, levelColor, 1, 0
      );
      this->DrawTextAt(this->TextWidth(level, static_cast<unsigned>(strlen(level)), 0, 12, 1, 0) + x + 33,
        this->iconPositions[this->iconIndex].y + top + 3, clipped.c_str(), static_cast<int>(clipped.size()), 0, 12, color,
        1, 0
      );

      if (it->offline == 0) {
        UIPcGage* gauge = this->gauges[index];
        if (gauge->geometry.x != -100) {
          char hpText[128];
          sprintf(hpText, "%d/%d", gauge->current, gauge->maximum);
          this->DrawTextAt(this->gauges[index]->geometry.x + 66, this->gauges[index]->geometry.y - 2, hpText,
            static_cast<int>(strlen(hpText)), 0, 10, 0, 0, 0
          );
        }
      }

      memset(path, 0, sizeof(path));
      const char* statusIcon;
      if (it->offline == 0) {
        if (it->accountId == g_session.m_aid) {
          statusIcon = "%sicon_party_me.bmp";
        } else {
          statusIcon = "%sicon_party_on.bmp";
        }
      } else {
        statusIcon = "%sicon_party_off.bmp";
      }
      sprintf(path, statusIcon, prefix.c_str());
      auto* status = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
      this->DrawBitmap(
        static_cast<int>(static_cast<float>(this->geometry.width) * 0.8f),
        static_cast<int>(static_cast<float>(this->rowHeight) * 0.65f + static_cast<float>(top)), status, true
      );

      memset(path, 0, sizeof(path));
      sprintf(path, "%sbg_partymember.bmp", prefix.c_str());
      auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
      this->DrawTextAt(this->geometry.width - 96, this->geometry.height - 16, LookupMessage(MSI_CONNECT_SNS_FAIL_CUZ_NOT_CONNECTING), 0, 0, 12, 0, 0, 0
      );
      this->DrawBitmap(this->geometry.width - 58, this->geometry.height - 18, background, true);

      memset(path, 0, sizeof(path));
      sprintf(path, "%d/%d", g_session.PartyCount(), 12);
      this->DrawTextAt(this->geometry.width - 51, this->geometry.height - 16, path,
        static_cast<int>(strlen(path)), 0, 12, 0xFFFFFF, 0, 0
      );
    } else {
      MessengerMember entry = g_session.FriendAt(index);
      if (g_modeMgr.GetGameMode() && entry.valid) {
        entry.name = LookupPlayerName(entry.characterId);
      }

      if (entry.name.size() == 0) {
        entry.name = LookupMessage(MSI_NAMELESS_USER) + g_messengerNamelessTag;
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
          x + 17, top + 4, this->TextWidth(nameText, 0, 0, 12, 0, 0) + 3, 16,
          RGB(selectRed, selectGreen, selectBlue)
        );
      }

      this->DrawTextAt(x + 19, top + 6, nameText, static_cast<int>(strlen(nameText)), 0, 12, color, 0, 0
      );

      if (it->offline == 0) {
        char onlinePath[256];
        sprintf(onlinePath, "%s", "유저인터페이스\\basic_interface\\grp_online.bmp");
        auto* online = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(onlinePath)));
        this->DrawBitmap(x + 2, top + 4, online, true);
      }
    }

    if (++shown == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x0073CD30
// FUNCTION: fn_07708
void UIMessengerGroupWnd::OnLBtnDblClk(int, int) {
  if (!g_session.m_lockMouse) {
    this->SendMsg(nullptr, UM_COMMAND, ID_TALK, 0, 0, 0);
  }
}

// OFFSET: 0x0073D460
// FUNCTION: fn_07713
void UIMessengerGroupWnd::OnRBtnUp(int x, int y) {
  if (g_session.m_lockMouse) {
    return;
  }

  const int previous = this->selected;
  const MessengerMember member = this->MemberAt(x, y);
  if (previous != this->selected) {
    this->Invalidate();
  }

  if (this->selected != -1) {
    this->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0073CD60
// FUNCTION: fn_07709
void UIMessengerGroupWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnRBtnUp(x, y);
    if (UIWindow* window = this->CompanionWindow()) {
      window->geometry.targetAlpha = 128;
      window->geometry.alphaStart = timeGetTime();
    }
    return;
  }

  const int previous = this->selected;
  MessengerMember member = this->MemberAt(x, y);
  if (previous != this->selected) {
    this->Invalidate();
  }
  if (!member.valid || member.offline != 0) {
    return;
  }

  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!gameMode) {
    return;
  }

  gameMode->TargetActorByAid(member.accountId);
  MessengerMember selectedMember = this->MemberAtIndex(this->selected);
  if (this->mode == 1 && !g_session.m_lockMouse && !g_windowMgr.HasMiniPartyMember(selectedMember.accountId)) {
    gameMode->SetDragIds(29, this->selected);
    if (!g_windowMgr.transientWnd) {
      UIDragMiniPartyWnd* window = new (std::nothrow) UIDragMiniPartyWnd(selectedMember);
      {
        window->SetSize(this->geometry.width, this->rowHeight);
        window->Move(-1000, -1000);
        g_windowMgr.ShowTransient(window);
        window->memberIndex = this->selected;
        window->iconIndex = this->iconIndex;
        window->MoveGauge(47, static_cast<int>(this->rowHeight * 0.76f));
        const int itemY = ((this->selected - this->scrollTop * this->columns) / this->columns) * this->rowHeight + 17;
        window->SetDragStartPosition(x, y - itemY);

        int index = 0;
        for (const MessengerMember& entry : this->members) {
          if (this->scrollTop * this->columns <= index && entry.valid && entry.accountId == selectedMember.accountId) {
            CGameActor* actor = entry.accountId == g_session.m_aid
                                  ? gameMode->scene->player
                                  : static_cast<CGameActor*>(gameMode->scene->GetGameActorByAID(entry.accountId));
            if (actor) {
              window->SetGaugeValues(this->gauges[index]->current, this->gauges[index]->maximum);
            } else {
              window->RemoveGauge();
            }
            break;
          }
          ++index;
        }
      }
    }
  }
}

// OFFSET: 0x0073D080
// FUNCTION: fn_07710
void UIMessengerGroupWnd::OnLBtnUp(int x, int y) {
  UIFrameWnd::OnLBtnUp(x, y);
  if (UIWindow* window = this->CompanionWindow()) {
    window->geometry.targetAlpha = 255;
    window->geometry.alphaStart = timeGetTime();
  }

  g_windowMgr.RetireTransientWindow();
}

// OFFSET: 0x0073D0E0
// FUNCTION: fn_07711
void UIMessengerGroupWnd::OnMouseHover(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnMouseHover(x, y);
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
    int tipX = this->TextWidth(level, std::strlen(level), 0, 12, 1, 0) +
               this->iconPositions[this->iconIndex].x + 33;
    int tipY = this->selected / this->columns * this->rowHeight + 22;
    this->ToScreen(tipX, tipY);
    mode->ShowTooltip(text, tipX, tipY, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x0073D3F0
// FUNCTION: fn_07712
void UIMessengerGroupWnd::OnMouseMove(int x, int y) {
  UIFrameWnd::OnMouseMove(x, y);
  if (UIWindow* window = this->CompanionWindow()) {
    x = 0;
    y = 0;
    this->ToScreen(x, y);
    window->Move(x + 2 + this->geometry.width, y);
  }
}

// OFFSET: 0x0073D530
// FUNCTION: fn_07714
void UIMessengerGroupWnd::OnWheel(int delta) {
  if (this->scrollable) {
    int step = this->visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, UM_SCROLLVERT, delta < 0 ? step : -step, 0, 0, 0);
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
    if (g_session.PartyCount()) {
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

  if (g_session.m_lockMouse) {
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
// FUNCTION: fn_07718
int UIMessengerGroupWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<MessengerGroupWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->iconIndex = this->saved->iconIndex;
    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_REFRESHITEM: {
    this->members.clear();
    for (int index = 0; index < 40; ++index) {
      this->gauges[index]->Move(-100, -100);
    }
    switch (this->mode) {
    case 0: {
      const int count = g_session.FriendCount();
      for (int index = 0; index < count; ++index) {
        MessengerMember member = g_session.FriendAt(index);
        if (member.valid) {
          this->members.push_back(member);
        }
      }
      this->rowHeight = 23;
      break;
    }
    case 1: {
      const int count = g_session.PartyCount();
      for (int index = 0; index < count; ++index) {
        MessengerMember member = g_session.PartyAt(index);
        if (member.valid) {
          this->members.push_back(member);
        }
        this->rowHeight = this->iconPositions[this->iconIndex].size;
      }
      break;
    }
    }

    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (gameMode && this->mode != 0) {
      int index = 0;
      for (auto it = this->members.begin(); it != this->members.end(); ++it, ++index) {
        if (it->valid && !it->offline) {
          if (it->accountId == g_session.m_aid) {
            if (gameMode->scene->player) {
              this->gauges[index]->SetValues(g_session.hp, g_session.maxHp);
            }
          } else if (CRenderObject* actor = gameMode->scene->GetGameActorByAID(it->accountId)) {
            UIPcGage* gauge = static_cast<UIPcGage*>(static_cast<CPc*>(static_cast<CGameActor*>(actor))->gaugeWindow);
            int current;
            int maximum;
            if (gauge) {
              current = gauge->current;
              maximum = gauge->maximum;
            } else {
              current = 0;
              maximum = 0;
            }
            this->gauges[index]->SetValues(current, maximum);
          }
          this->jobIcons[index]->offline = 0;
        } else {
          this->jobIcons[index]->offline = 1;
        }

        char path[256];
        memset(path, 0, sizeof(path));
        int dead = 0;
        if (it->dead == 1) {
          dead = 1;
        } else if (g_session.m_aid == it->accountId && gameMode->isPlayerDead == 1) {
          dead = 1;
        }
        std::string prefix = "유저인터페이스\\renewalparty\\";
        sprintf(path, dead ? "%sicon_jobs_%d_die.bmp" : "%sicon_jobs_%d.bmp", prefix.c_str(), it->job);
        this->jobIcons[index]->SetBitmap(path, 0, 0);
        this->jobIcons[index]->SetBitmap(path, 1, 0);
        this->jobIcons[index]->SetBitmap(path, 2, 0);
        this->jobIcons[index]->Invalidate();
      }
    }

    this->UpdateScroll();
    this->Invalidate();
    char title[64];
    const char* titles[2] = {LookupMessage(MSI_FRIEND), LookupMessage(MSI_PARTY)};
    if (this->mode == 1) {
      if (g_session.partyName.size()) {
        sprintf(title, "%s(%s)", titles[1], g_session.partyName.c_str());
      } else {
        sprintf(title, "%s", titles[1]);
      }
    } else {
      sprintf(title, "%s(%d/%d)", titles[this->mode], this->members.size(), 40);
    }
    this->titleText->SetTextFitted(title);
    if (g_windowMgr.modal) {
      g_windowMgr.modal->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    }

    int tooltip;
    if (this->mode == 0) {
      static_cast<UIBitmapButton*>(this->buttons[0])->SetDisabled(1);
      static_cast<UIBitmapButton*>(this->buttons[0])->Invalidate();
      static_cast<UIBitmapButton*>(this->buttons[0])->SetBitmap("유저인터페이스\\basic_interface\\mesbtn_01_disable.bmp", 0, 0);
      static_cast<UIBitmapButton*>(this->buttons[0])->SetBitmap("유저인터페이스\\basic_interface\\mesbtn_01_disable.bmp", 1, 0);
      static_cast<UIBitmapButton*>(this->buttons[0])->SetBitmap("유저인터페이스\\basic_interface\\mesbtn_01_disable.bmp", 2, 0);
      static_cast<UIBitmapButton*>(this->buttons[0])->SetBitmap("유저인터페이스\\basic_interface\\mesbtn_01_disable.bmp", 3, 0);
      tooltip = 0x62;
    } else {
      static_cast<UIBitmapButton*>(this->buttons[0])->SetDisabled(0);
      static_cast<UIBitmapButton*>(this->buttons[0])->Invalidate();
      static_cast<UIBitmapButton*>(this->buttons[0])->SetBitmap("유저인터페이스\\basic_interface\\mesbtn_partymaster_01.bmp", 0, 0);
      static_cast<UIBitmapButton*>(this->buttons[0])->SetBitmap("유저인터페이스\\basic_interface\\mesbtn_partymaster_01_a.bmp", 1, 0);
      static_cast<UIBitmapButton*>(this->buttons[0])->SetBitmap("유저인터페이스\\basic_interface\\mesbtn_partymaster_01_b.bmp", 2, 0);
      tooltip = 0xDBA;
    }
    static_cast<UIBitmapButton*>(this->buttons[0])->SetTooltip(LookupMessage(tooltip));
    this->LayoutButtons();
    return 0;
  }
  case UM_MAKEMENU:
    this->menuItems.clear();
    if (g_windowMgr.menuWnd) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }
    if (this->selected != -1) {
      if (this->mode == 0) {
        this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_OPEN_1ON1_WINDOW)).c_str(), 7));
        this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_USA_PERSONALINFO_DEATH)).c_str(), 0x3A));
        this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_DELETE)).c_str(), 0x13));
        MessengerMember member = this->MemberAtIndex(this->selected);
        if (member.valid && !member.offline && g_session.partyLeader
            && !g_session.PartyByName(member.name.c_str()).valid) {
          char text[200];
          sprintf(text, LookupMessage(MSI_REQ_JOIN_PARTY3), member.name.c_str());
          this->menuItems.push_back(MiniPartyInfoEntry(text, 5));
        }
      } else {
        MessengerMember member = this->MemberAtIndex(this->selected);
        if (member.valid) {
          this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_USA_PERSONALINFO_DEATH)).c_str(), 0x3A));
          if (member.name == g_session.DecodePlayerName()) {
            this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_LEAVE_PARTY)).c_str(), 9));
          } else {
            this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_OPEN_1ON1_WINDOW)).c_str(), 7));
            if (g_session.partyLeader) {
              this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_EXPEL)).c_str(), 0));
              this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_YIELD_PARTYMASTER)).c_str(), 0x2B));
            }
          }
          if (!member.offline) {
            this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_DATE_MM_DD)).c_str(), 0x3C));
            this->menuItems.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_ACHIEVEMENT_NEXT_GRADE)).c_str(), 0x3D));
          }
        }
      }
    }
    if (this->menuItems.size()) {
      UIWindow* menu = g_windowMgr.EnsureWindow(WID_MENUWND);
      for (unsigned index = 0; index < this->menuItems.size(); ++index) {
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->menuItems[index].text.c_str()), 0, 0, 0);
      }
      menu->SendMsg(nullptr, UM_SET_CALLER_WID, 0x22, 0, 0, 0);
      menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
      menu->Move(
        std::min(std::max(g_mouse.x + 1, 1), g_renderer->viewport.width - menu->geometry.width),
        std::min(std::max(g_mouse.y + 1, 1), g_renderer->viewport.height - menu->geometry.height)
      );
    }
    return 0;
  case UM_MENU_SELECTED: {
    if (this->menuItems.begin() == this->menuItems.end()) {
      return 0;
    }
    MessengerMember member = this->MemberAtIndex(this->selected);
    if (member.name.find(g_messengerNamelessTag, 0) != std::string::npos) {
      g_windowMgr.Confirm(LookupMessage(MSI_CHR_DELETE_FAIL_CHN_TAREN_PERSONALNUMBER), 0, true, false, 0, "Message", 0x118, 0x78, 0);
      return 0;
    }
    if (!member.valid) {
      return 0;
    }
    switch (this->menuItems[a].value) {
    case MENUID_EXPEL:
    case MENUID_DELETE_WHISPER_NAME:
      this->SendMsg(nullptr, UM_COMMAND, ID_DELETE_FRIEND, 0, 0, 0);
      return 0;
    case MENUID_QUERYINFO:
      this->SendMsg(nullptr, UM_COMMAND, ID_FRIEND_INFO, 0, 0, 0);
      return 0;
    case MENUID_INVITE_IN_PARTY:
      if (this->selected != -1) {
        MessengerMember target = this->MemberAtIndex(this->selected);
        if (target.valid) {
          g_modeMgr.gameMode->SendMsg(MM_REQ_JOIN_GROUP, reinterpret_cast<int>(target.name.c_str()), 0, 0, 0);
        }
      }
      return 0;
    case MENUID_SEND_MSG:
      this->SendMsg(nullptr, UM_COMMAND, ID_SENDMSG, 0, 0, 0);
      return 0;
    case MENUID_TALKONLYTWO:
      this->SendMsg(nullptr, UM_COMMAND, ID_TALK, 0, 0, 0);
      return 0;
    case MENUID_LEAVE:
      this->SendMsg(nullptr, UM_COMMAND, ID_LEAVE, 0, 0, 0);
      return 0;
    case MENUID_MAKE_WHISPER_WINDOW:
      this->SendMsg(nullptr, 0x47, 0, 0, 0, 0);
      return 0;
    case 0x2B:
      if (this->selected != -1) {
        MessengerMember target = this->MemberAtIndex(this->selected);
        if (target.valid && !target.offline) {
          if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_YIELD_PARTYMASTER), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
            g_modeMgr.gameMode->SendMsg(MM_GROUP_CHANGE_MASTER, target.accountId, 0, 0, 0);
          }
        } else {
          g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_YIELD_PARTYMASTER)), 0xFF, 3, 0, 0);
        }
      }
      return 0;
    case 0x3A: {
      MessengerMember target = this->MemberAtIndex(this->selected);
      if (target.valid) {
        g_modeMgr.gameMode->SendMsg(MM_OPEN_WRITE_RODEX, reinterpret_cast<int>(target.name.c_str()), 0, 0, 0);
      }
      return 0;
    }
    case 0x3C: {
      MessengerMember target = this->MemberAtIndex(this->selected);
      if (target.valid && g_session.FindMiniPartyRecord(target.accountId).active != 1) {
        this->OpenMiniPartyWnd(g_windowMgr.FindMiniPartyCell(), target);
      }
      return 0;
    }
    case 0x3D: {
      MessengerMember target = this->MemberAtIndex(this->selected);
      if (target.valid) {
        g_windowMgr.CloseMiniPartyWindow(target.accountId);
      }
      return 0;
    }
    }
    return 0;
  }
  case UM_COMMAND:
    switch (a) {
    case ID_SENDMSG:
      if (!g_session.m_lockMouse && this->selected != -1) {
        MessengerMember member = this->MemberAtIndex(this->selected);
      }
      return 0;
    case ID_TALK:
      if (!g_session.m_lockMouse && this->selected != -1) {
        MessengerMember member = this->MemberAtIndex(this->selected);
        if (member.valid && !(member.name == g_session.DecodePlayerName())) {
          const char* tag;
          if (g_windowMgr.IsFriend(member.name.c_str())) {
            tag = LookupMessage(MSI_FRIEND);
          } else {
            tag = g_session.IsGuildMemberCharacter(member.characterId) ? "GuildMember" : nullptr;
          }
          g_windowMgr.SendNotice(
            14, reinterpret_cast<std::intptr_t>(member.name.c_str()), reinterpret_cast<std::intptr_t>(tag),
            reinterpret_cast<std::intptr_t>(ShortcutFilename(member.characterId).c_str()), 0, 0
          );
        }
      }
      return 0;
    case ID_FRIEND_INFO:
      if (!g_session.m_lockMouse && this->selected != -1) {
        MessengerMember member = this->MemberAtIndex(this->selected);
        if (member.valid) {
          g_windowMgr.Confirm(LookupMessage(MSI_NOTSUPPORTEDYET), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        }
      }
      return 0;
    case ID_PARTY_SETTING:
      if (!g_session.m_lockMouse) {
        if (g_windowMgr.partyInvitationToWnd) {
          g_windowMgr.CloseWindow(WID_PARTYINVITATIONTOWND);
        }
        if (this->mode == 0) {
          if (!g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND)) {
            g_windowMgr.EnsureWindow(WID_FRIENDOPTIONWND);
          }
        } else if (!g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT) && g_session.PartyCount() && this->mode == 1) {
          g_windowMgr.EnsureWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
        }

        UIWindow* window;
        if (this->mode == 0) {
          g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
          window = g_windowMgr.friendOptionWnd;
        } else {
          g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND);
          window = g_windowMgr.partySettingWnd;
        }
        if (window) {
          int x = 0;
          int y = 0;
          this->ToScreen(x, y);
          window->Move(x + 2 + this->geometry.width, y);
        }
      }
      return 0;
    case ID_DELETE_FRIEND:
      if (!g_session.m_lockMouse && this->selected != -1) {
        MessengerMember member = this->MemberAtIndex(this->selected);
        if (member.valid) {
          if (this->mode == 0) {
            if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_DELETE), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
              g_modeMgr.gameMode->SendMsg(MM_DELETE_FRIEND, member.accountId, member.characterId, 0, 0);
            }
          } else if (!(member.name == g_session.DecodePlayerName())) {
            if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_EXPEL), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
              g_modeMgr.gameMode->SendMsg(MM_REQ_EXPEL_GROUP_MEMBER, reinterpret_cast<int>(member.name.c_str()), 0, 0, 0);
            }
          }
        }
      }
      return 0;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_MESSENGERWND);
      return 0;
    case ID_LEAVE:
      if (!g_session.m_lockMouse && this->selected != -1) {
        MessengerMember member = this->MemberAtIndex(this->selected);
        if (member.valid && member.name == g_session.DecodePlayerName()) {
          if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_LEAVE), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
            g_modeMgr.gameMode->SendMsg(MM_REQ_LEAVE_GROUP, 0, 0, 0, 0);
          }
        }
      }
      return 0;
    case ID_RADIOBTN: {
      UIWindow* clicked = sender;
      if (b) {
        clicked = this->tabs[b - 1];
      }
      const int previous = this->mode;
      for (int index = 0; index < 2; ++index) {
        this->tabs[index]->Invalidate();
        if (clicked == this->tabs[index]) {
          this->mode = index;
          this->tabs[index]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
        } else {
          this->tabs[index]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        }
      }
      int secondTip;
      int firstTip;
      if (this->mode == 0) {
        secondTip = 0x15F;
        firstTip = 0x163;
      } else {
        secondTip = 0x61;
        firstTip = 0x65;
      }
      static_cast<UIButton*>(this->buttons[3])->SetTooltip(LookupMessage(firstTip));
      static_cast<UIButton*>(this->buttons[4])->SetTooltip(LookupMessage(secondTip));
      if (this->mode != previous) {
        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      this->SendMsg(nullptr, UM_RESIZE, 7, this->geometry.width, this->geometry.height, 0);
      return 0;
    }
    case 0x22: {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (!gameMode || this->mode == 0) {
        return 0;
      }
      int index = 0;
      for (auto it = this->members.begin(); it != this->members.end(); ++it, ++index) {
        if (it->valid && !it->offline && it->accountId == static_cast<std::uint32_t>(b)) {
          CRenderObject* actor;
          if (it->accountId == g_session.m_aid) {
            actor = gameMode->scene->player;
            c = g_session.hp;
            d = g_session.maxHp;
          } else {
            actor = gameMode->scene->GetGameActorByAID(it->accountId);
          }
          if (actor) {
            this->gauges[index]->SetValues(c, d);
          }
          this->UpdateGauges();
          this->Invalidate();
          return 1;
        }
      }
      return 0;
    }
    case ID_MINI:
      this->iconIndex = 1;
      this->iconModeButtons[1]->Enable(1);
      this->iconModeButtons[0]->Enable(0);
      this->SendMsg(sender, UM_RESIZE, 7, 0xF3, 0xB3, 0);
      return 0;
    case 0x16C:
      if (g_session.m_lockMouse != true) {
        g_windowMgr.RearrangeMiniPartyWindows();
      }
      return 0;
    case ID_TOGGLE_PARTY_BOARD:
      if (!g_windowMgr.CloseWindow(WID_ADVENPARTYBOARDWND)) {
        g_windowMgr.EnsureWindow(WID_ADVENPARTYBOARDWND);
      }
      return 0;
    case ID_TOGGLE_PARTY_MOUSE_LOCK:
      if (sender == this->buttons[5]) {
        g_session.m_lockMouse = false;
        this->buttons[6]->Move(sender->geometry.x, sender->geometry.y);
      } else if (sender == this->buttons[6]) {
        g_session.m_lockMouse = true;
        this->buttons[5]->Move(sender->geometry.x, sender->geometry.y);
      } else {
        return 0;
      }
      sender->Move(-100, -100);
      return 0;
    case ID_OPEN_PARTY_SETTINGS:
      if (!g_session.m_lockMouse) {
        if (g_windowMgr.friendOptionWnd) {
          g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND);
        }
        if (!g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT) && !g_session.PartyCount()) {
          if (this->mode != 1) {
            return 0;
          }
          g_windowMgr.EnsureWindow(WID_PARTYSETTINGWND_HEIGHT_220);
        }
        if (this->mode == 1) {
          if (UIWindow* window = g_windowMgr.partySettingWnd) {
            int x = 0;
            int y = 0;
            this->ToScreen(x, y);
            window->Move(x + 2 + this->geometry.width, y);
          }
        }
      }
      return 0;
    case ID_LEAVE_PARTY:
      if (!g_session.m_lockMouse) {
        if (g_windowMgr.Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_LEAVE), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
          g_modeMgr.gameMode->SendMsg(MM_REQ_LEAVE_GROUP, 0, 0, 0, 0);
          if (g_windowMgr.partyInvitationToWnd) {
            g_windowMgr.CloseWindow(WID_PARTYINVITATIONTOWND);
          }
        }
      }
      return 0;
    case ID_OPEN_PARTY_INVITATION:
      if (!g_session.m_lockMouse) {
        if (g_windowMgr.friendOptionWnd) {
          g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND);
        }
        if (g_windowMgr.partySettingWnd) {
          g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
        }
        if (!g_windowMgr.CloseWindow(WID_PARTYINVITATIONTOWND)) {
          if (g_session.partyLeader) {
            g_windowMgr.EnsureWindow(WID_PARTYINVITATIONTOWND);
          } else {
            g_windowMgr.Confirm(LookupMessage(MSI_YOU_ARE_NOT_PARTYMASTER), 4, true, false, 0, "Message", 0x118, 0x78, 0);
          }
        }
        if (this->mode == 1) {
          if (UIWindow* window = g_windowMgr.partyInvitationToWnd) {
            int x = 0;
            int y = 0;
            this->ToScreen(x, y);
            window->Move(x + 2 + this->geometry.width, y);
          }
        }
      }
      return 0;
    case ID_RESTORE_PARTY_LIST:
      this->iconIndex = 0;
      this->iconModeButtons[1]->Enable(0);
      this->iconModeButtons[0]->Enable(1);
      this->SendMsg(sender, UM_RESIZE, 7, 0x111, 0x146, 0);
      return 0;
    }
    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->scrollTop;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->scrollTop = previous - this->visibleRows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->scrollTop = this->visibleRows - 1 + previous;
    } else {
      this->scrollTop = a + previous;
    }
    if (previous != this->scrollTop) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->scrollTop - previous;
  }
  case UM_RESIZE:
    if (a == 7) {
      int width = static_cast<int>(b);
      int height = static_cast<int>(c);
      if (this->iconIndex == 0) {
        width = 0x111;
        height = std::max(height, 0x146);
      } else if (this->iconIndex == 1) {
        width = 0xF3;
        if (height < 0xB3) {
          height = 0xB3;
        }
      }
      this->rowHeight = this->mode ? this->iconPositions[this->iconIndex].size : 23;
      int body = height - 58;
      if (body / this->rowHeight > 12) {
        body = this->rowHeight * 12;
        height = body + 58;
      }
      height -= body % this->rowHeight;
      if (width != this->geometry.width || height != this->geometry.height) {
        this->scrollTop = 0;
        this->Resize(width, height);
      }
      this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 58);
      if (this->mode) {
        this->visibleRows = this->scrollBar->geometry.height / this->rowHeight;
      } else {
        this->visibleRows = (this->geometry.height - 180) / this->rowHeight + 5;
      }
      this->closeButton->Move(this->geometry.width - 14, 3);
      this->scrollBar->Move(this->geometry.width - 15, 17);
      this->resizeButton->Move(this->geometry.width - 15, this->geometry.height - 15);
      this->iconModeButtons[0]->Move(this->geometry.width - 28, 3);
      this->iconModeButtons[1]->Move(this->geometry.width - 28, 3);
      if (this->iconIndex == 0) {
        this->iconModeButtons[0]->Enable(1);
        this->iconModeButtons[1]->Enable(0);
      } else {
        this->iconModeButtons[1]->Enable(1);
        this->iconModeButtons[0]->Enable(0);
      }
      UIRadioBtn** tab = this->tabs;
      for (int x = 4; x < 124; x += 60, ++tab) {
        (*tab)->Move(x, this->geometry.height - 16);
      }
      this->LayoutButtons();
      switch (g_serviceType) {
      case 10:
      case 12:
      case 15:
      case 18:
        this->visibleRows = this->scrollBar->geometry.height / this->rowHeight;
        break;
      }

      char title[64];
      memset(title, 0, sizeof(title));
      const char* titles[2] = {LookupMessage(MSI_FRIEND), LookupMessage(MSI_PARTY)};
      if (this->mode == 1) {
        if (g_session.partyName.size()) {
          sprintf(title, "%s(%s)", titles[1], g_session.partyName.c_str());
        } else {
          sprintf(title, "%s", titles[1]);
        }
      } else {
        sprintf(title, "%s(%d/%d)", titles[this->mode], this->members.size(), 40);
      }
      this->titleText->Resize(this->geometry.width - 57, 12);
      this->titleText->SetTextFitted(title);
      this->titleText->Move(16, 3);
      this->UpdateScroll();
      UIWindow* window = g_windowMgr.partySettingWnd;
      if (this->mode == 0) {
        window = g_windowMgr.friendOptionWnd;
      }
      if (window) {
        int x = 0;
        int y = 0;
        this->ToScreen(x, y);
        window->Move(x + 2 + this->geometry.width, y);
      }
      g_windowMgr.StoreSnapRectangle(this);
    }
    return 0;
  }
  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x0089CFD0
// FUNCTION: fn_11596
void UIGage::SetColor(std::uint32_t color) {
  this->colors[1] = color;
}

// OFFSET: 0x0073FD90
// FUNCTION: fn_07719
void UIMessengerGroupWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->mode = this->mode;
    this->saved->iconIndex = this->iconIndex;
  }
}

// OFFSET: 0x0073FFC0
// FUNCTION: fn_07726
UIMiniPartyInfoWnd::UIMiniPartyInfoWnd()
    : UIFrameWnd(0), m_gauge(nullptr), m_owner(nullptr) {
  this->m_entries.clear();
}

// OFFSET: 0x00740170
// FUNCTION: fn_07728
MessengerMember UIMiniPartyWnd::GetMember() {
  MessengerMember member = this->m_member;
  return member;
}

// OFFSET: 0x00740250
// FUNCTION: fn_07729
void UIMiniPartyInfoWnd::OnCreate(int width, int) {
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
  UIDragMiniPartyWnd* transient = static_cast<UIDragMiniPartyWnd*>(this->transientWnd);
  if (transient && transient->member.valid) {
    transient->Move(x - transient->dragStartX, y - transient->dragStartY);
  }
}

// OFFSET: 0x00AA9E90
// FUNCTION: fn_17262
__declspec(noinline) void UIWindowMgr::RetireTransientWindow() {
  UIDragMiniPartyWnd* transient = static_cast<UIDragMiniPartyWnd*>(this->transientWnd);
  if (transient && transient->member.valid) {
    transient->geometry.state = 1;
    this->Schedule(this->transientWnd);
    this->transientWnd = nullptr;
  }
}
