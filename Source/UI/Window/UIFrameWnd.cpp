#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIBank_NewWnd.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIReplayButton.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIChatRoomWnd.h"
#include "UI/Window/UICntWnd.h"
#include "UI/Window/UIHomunInfoWnd.h"
#include "UI/Window/UILoginOTPWnd.h"
#include "UI/Window/UILoginWnd.h"
#include "UI/Window/UIMonsterInfoWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Input/InputCharset.h"
#include "Client/Input/Mouse.h"
#include "UI/Lists/UIReplayListBox.h"
#include "UI/Window/UINoticeWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/TipOfTheDay.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Lists/UIMemberListBox.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UINewSelectCharWnd.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <intrin.h>
#include <mbstring.h>
#include <string>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24747
std::uint32_t g_monsterInfoActorId;

// OFFSET: 0x008A3700
// FUNCTION: fn_11657
UIFrameWnd::UIFrameWnd(unsigned char cached)
    : UIWindow(cached), m_dragX(0), m_dragY(0), m_message0(600), m_message1(600) {
}

// OFFSET: 0x008A4E90
// FUNCTION: fn_11678
UIFrameWnd::~UIFrameWnd() = default;

// OFFSET: 0x005C4640
// FUNCTION: fn_03570
bool UIFrameWnd::IsFrameWnd() {
  return true;
}

// OFFSET: 0x008AD150
// FUNCTION: fn_11735
void UIFrameWnd::Move(int x, int y) {
  this->geometry.Move(x, y);
  this->RefreshSnap();
}

// OFFSET: 0x008BB3F0
// FUNCTION: fn_11797
void UIFrameWnd::StoreMoveOrigin() {
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
}

// OFFSET: 0x008AD170
// FUNCTION: fn_11736
void UIFrameWnd::MoveFromOrigin(int dx, int dy) {
  this->geometry.x = this->m_originX + dx;
  this->geometry.y = this->m_originY + dy;
}

// OFFSET: 0x00A7D870
// FUNCTION: fn_16956
void UIWindow::ToScreen(int& x, int& y) const {
  x += this->geometry.x;
  y += this->geometry.y;

  for (const UIWindow* node = this->parent; node; node = node->parent) {
    x += node->geometry.x;
    y += node->geometry.y;
  }
}

// INLINED
static __forceinline void RaiseWindow(UIWindow* window) {
  if (window && g_windowMgr.lastWindowId != window->geometry.id) {
    g_windowMgr.BringToFront(window->geometry.id);
    g_windowMgr.lastWindowId = window->geometry.id;
  }
}

// OFFSET: 0x008BA1E0
// FUNCTION: fn_11779
void UIFrameWnd::OnLBtnDown(int x, int y) {
  RaiseWindow(this);
  g_windowMgr.SetCapture(this);
  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);
  this->m_dragX = screenX;
  this->m_dragY = screenY;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  this->geometry.targetAlpha = 128;
  this->geometry.alphaStart = timeGetTime();
  g_snapMgr.CaptureOrigins(this);
}

// OFFSET: 0x008BA510
// FUNCTION: fn_11786
void UIFrameWnd::OnMouseMove(int x, int y) {
  if (g_windowMgr.Capture() != this || x <= -g_configuredScreenWidth || y <= -g_configuredScreenHeight) {
    return;
  }

  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);
  const int dx = screenX - this->m_dragX;
  const int dy = screenY - this->m_dragY;

  if (!g_snapMgr.Contains(this)) {
    this->Move(this->m_originX + dx, this->m_originY + dy);
    return;
  }

  this->MoveFromOrigin(dx, dy);
  int ownX, ownY;
  g_windowMgr.ConstrainSnap(this, ownX, ownY, 0);
  int groupX = dx, groupY = dy;
  g_snapMgr.ConstrainGroup(this, groupX, groupY);

  if (std::abs(groupX) < std::abs(ownX)) {
    groupX = ownX;
  }

  if (std::abs(groupY) < std::abs(ownY)) {
    groupY = ownY;
  }

  this->MoveFromOrigin(dx + groupX, dy + groupY);
  g_windowMgr.StoreSnapRectangle(this);
  g_snapMgr.MoveConnected(this, dx + groupX, dy + groupY);
}

// OFFSET: 0x008BA3C0
// FUNCTION: fn_11783
void UIFrameWnd::OnMouseHover(int, int) {
}

// OFFSET: 0x008BA2C0
// FUNCTION: fn_11781
void UIFrameWnd::OnLBtnUp(int, int) {
  if (g_windowMgr.Capture() != this) {
    return;
  }

  g_windowMgr.lastWindowId = -1;
  this->geometry.targetAlpha = 255;
  this->geometry.alphaStart = timeGetTime();
  g_windowMgr.ReleaseCapture();
  g_snapMgr.Refresh(this);
}

// OFFSET: 0x008BB340
// FUNCTION: fn_11796
void UIFrameWnd::RefreshSnap() {
  int x, y;
  g_windowMgr.ConstrainSnap(this, x, y, 0);
  this->geometry.x += x;
  this->geometry.y += y;
  g_windowMgr.StoreSnapRectangle(this);
}

// OFFSET: 0x008BDC90
// FUNCTION: fn_11802
int UIFrameWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DEFPUSH:
    if (this->m_message0) {
      this->SendMsg(this, UM_COMMAND, this->m_message0, 0, 0, 0);
    }
    break;
  case UM_CANCELPUSH:
    if (this->m_message1) {
      this->SendMsg(this, UM_COMMAND, this->m_message1, 0, 0, 0);
    }
    break;
  case UM_TEXT_LBTN_UP:
    if (c > 0) {
      if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
        window->SendMsg(nullptr, UM_ITEMEXPLANATION, c, 0, 0, 0);
      }
    }
    break;
  default:
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x005C4420
// FUNCTION: fn_03559
std::vector<unsigned char> UIFrameWnd::GetReplayData() {
  return {};
}

// OFFSET: 0x005C44F0
// FUNCTION: fn_03562
int UIFrameWnd::GetWindowType() {
  return 1;
}

// OFFSET: 0x008CEF10
// FUNCTION: fn_12074
UIBitmapButton* UIFrameWnd::AddBaseButton() {
  UIBitmapButton* button = new UIBitmapButton();
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_base_on.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_base_on.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(3, 3);
  this->AddChild(button);

  return button;
}

// OFFSET: 0x008CEFD0
// FUNCTION: fn_12075
UIBitmapButton* UIFrameWnd::AddBigCloseButton(const char* tooltip, int width) {
  if (width == 0) {
    width = this->geometry.width;
  }

  UIBitmapButton* button = new UIBitmapButton();
  button->SetBitmap("유저인터페이스\\basic_interface\\big_sys_close_off.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\big_sys_close_on.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\big_sys_close_on.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - button->BitmapWidth(), 0);
  button->SetId(201);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  return button;
}

// OFFSET: 0x008CF0C0
// FUNCTION: fn_12076
UIBitmapButton* UIFrameWnd::AddCloseButton(const char* tooltip, int width) {
  if (width == 0) {
    width = this->geometry.width;
  }

  UIBitmapButton* button = new UIBitmapButton();
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 14, 3);
  button->SetId(201);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  return button;
}

// OFFSET: 0x008CF2A0
// FUNCTION: fn_12077
UIBitmapButton* UIFrameWnd::AddMinimizeButton(const char* tooltip, int width) {
  if (width == 0) {
    width = this->geometry.width;
  }

  UIBitmapButton* button = new UIBitmapButton();
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_mini_off.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_mini_on.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_mini_on.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 28, 3);
  button->SetId(202);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  return button;
}

// OFFSET: 0x008CF390
// FUNCTION: fn_12078
UIBitmapButton* UIFrameWnd::AddBitmapButton(int id, int x, int y, const char* name, const char* tooltip) {
  if (!name) {
    return nullptr;
  }

  UIBitmapButton* button = new UIBitmapButton();
  std::string path = "유저인터페이스\\";
  path += name;
  button->SetBitmap((path + "_out" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((path + "_over" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((path + "_press" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(id);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  return button;
}

// OFFSET: 0x008CF870
// FUNCTION: fn_12079
UIBitmapButton* UIFrameWnd::AddNumberedButton(int id, int x, int y, const char* name, const char* tooltip, bool prefix) {
  if (!name) {
    return nullptr;
  }

  UIBitmapButton* button = new UIBitmapButton();
  std::string path;
  if (prefix) {
    path += "유저인터페이스\\";
  }
  path += name;
  button->SetBitmap((path + "_1" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((path + "_2" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((path + "_3" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(id);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  return button;
}

// OFFSET: 0x008CFE50
// FUNCTION: fn_12080
UICheckBox* UIFrameWnd::AddStyledCheckBox(int id, int width, int height, const char* text, int x, int y, bool checked, const char* tooltip, int fontHeight,
  std::uint32_t color, const char* checkedImage, const char* uncheckedImage
) {
  UICheckBox* checkBox = new UICheckBox();
  checkBox->SetSize(width, height);
  checkBox->SetImages(
    checkedImage ? checkedImage : "유저인터페이스\\checkbox_1.bmp", uncheckedImage ? uncheckedImage : "유저인터페이스\\checkbox_0.bmp", text
  );
  checkBox->Move(x, y);
  checkBox->SetId(id);
  checkBox->SetChecked(checked);
  checkBox->SetTooltip(tooltip);
  checkBox->m_fontHeight = fontHeight;
  checkBox->m_color = color;
  this->AddChild(checkBox);

  return checkBox;
}

// OFFSET: 0x008CFF40
// FUNCTION: fn_12081
UICheckBox* UIFrameWnd::AddCheckBox(int id, int width, int height, const char* text, int x, int y, int checked, const char* tooltip) {
  UICheckBox* checkBox = new UICheckBox();
  checkBox->SetSize(width, height);
  checkBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", text);
  checkBox->Move(x, y);
  checkBox->SetId(id);
  checkBox->SetChecked(checked);
  checkBox->SetTooltip(tooltip);
  this->AddChild(checkBox);

  return checkBox;
}

// OFFSET: 0x008BA280
// FUNCTION: fn_11780
void UIReplayListBox::OnLBtnDown(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
  this->parent->SendMsg(nullptr, UM_SET_TARGET, 0, 0, 0, 0);
}

// OFFSET: 0x008BA500
// FUNCTION: fn_11785
void UINoticeWnd::OnMouseHover(int, int) {
}

// OFFSET: 0x008BA310
// FUNCTION: fn_11782
void UIReplayButton::OnLBtnUp(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;
    if (this->parent) {
      this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, this->m_argument, 0, 0);
      this->parent->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    } else {
      g_modeMgr.gameMode->SendMsg(MM_COMMAND, this->geometry.id, this->m_argument, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x008BA3D0
// FUNCTION: fn_11784
void UIMonsterInfoWnd::OnMouseHover(int x, int y) {
  if (x < 161 || x > 259 || y < 31 || y > 41) {
    return;
  }

  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  std::string name = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(g_monsterInfoActorId);
  const std::size_t mark = name.find("#", 0);
  if (mark != std::string::npos) {
    name.erase(mark, name.size());
  }

  if (this->TextWidth(name.c_str(), 0, 0, 12, 0, 0) > 100) {
    g_modeMgr.gameMode->ShowTooltip(name.c_str(), screenX + x, screenY + y, -1, false, true);
  }
}

// OFFSET: 0x008BB410
// FUNCTION: fn_11798
int UIAllyGuildAcceptWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_ALLYGUILDACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_ALLY_GUILD, this->identity, 1, 0, 0);
      break;
    case ID_CANCEL:
      g_windowMgr.CloseWindow(WID_ALLYGUILDACCEPTWND);
      g_modeMgr.gameMode->SendMsg(MM_ALLY_GUILD, this->identity, 0, 0, 0);
      break;
    }

    return 0;
  case UM_SET_ALLY_GUILD_MSG: {
    this->identity = static_cast<std::uint32_t>(a);
    std::string text = "(";
    text += reinterpret_cast<const char*>(b);
    text += ")";
    text += LookupMessage(MSI_SUGGEST_ALLY_GUILD);
    this->viewer->Clear();
    this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 38, 0, 0);
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

#pragma pack(push, 1)
struct PACKET_CZ_BANK_TRANSACTION {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ int amount;
};
#pragma pack(pop)

// OFFSET: 0x008BB5F0
// FUNCTION: fn_11799
int UIBank_NewWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  int step;
  switch (a) {
  case ID_CLOSE:
    g_windowMgr.CloseWindow(WID_BANK_NEWWND);
    return 0;
  case ID_UP:
    step = 1;
    break;
  case ID_DOWN:
    step = -1;
    break;
  case 0x185:
    if (!this->m_amountEdit) {
      return 0;
    }
    this->m_amountEdit->SetText(LookupMessage(MSI_BANK_1_MINI));
    g_windowMgr.SetFocus(this->m_amountEdit);
    return 0;
  case 0x1CE: {
    PACKET_CZ_BANK_TRANSACTION packet;
    if (g_modeMgr.GetGameMode() && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsMenuWindowOpen() == true) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_WRONG_GUN)), 255, 0, 0, 0);
      return 0;
    }
    const long long amount = this->TransactionAmount(1);
    if (amount == 0) {
      return 0;
    }
    if (g_session.m_zeny < amount) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BANK_OVER_INT_MAX)), 255, 0, 0, 0);
      this->m_status = LookupMessage(MSI_ITEMMOVEINFO_DROP);
    } else if (g_session.m_bankBalance + amount > 0x7FFFFFFFLL) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEMMOVEINFO_CART)), 255, 0, 0, 0);
      this->m_status = LookupMessage(MSI_BANK_6_MINI);
    } else {
      packet.amount = static_cast<int>(amount);
      packet.id = 0x9A7;
      packet.aid = g_session.m_aid;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      this->m_status.clear();
    }
    return 0;
  }
  case 0x1CF: {
    PACKET_CZ_BANK_TRANSACTION packet;
    if (g_modeMgr.GetGameMode() && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsMenuWindowOpen() == true) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_WRONG_GUN)), 255, 0, 0, 0);
      return 0;
    }
    const long long amount = this->TransactionAmount(0);
    if (amount == 0) {
      return 0;
    }
    if (g_session.m_bankBalance < amount) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BANK_WITHDRAW_ZERO)), 255, 0, 0, 0);
      this->m_status = LookupMessage(MSI_ITEMMOVEINFO_STORE);
    } else if (static_cast<unsigned int>(g_session.m_zeny + static_cast<int>(amount)) > 0x7FFFFFFFu) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_JOBPLUSEXP_VALUE)), 255, 0, 0, 0);
      this->m_status = LookupMessage(MSI_ITEMMOVEINFO_CART);
    } else {
      packet.amount = static_cast<int>(amount);
      packet.id = 0x9A9;
      packet.aid = g_session.m_aid;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      this->m_status.clear();
    }
    return 0;
  }
  case 0x186:
    step = 10000000;
    break;
  case 0x187:
    step = 1000000;
    break;
  case 0x188:
    step = 100000;
    break;
  default:
    return 0;
  }

  const long long amount = this->AdjustAmount(step);
  char text[260];
  std::memset(text, 0, sizeof(text));
  _i64toa(amount, text, 10);
  if (this->m_amountEdit) {
    this->m_amountEdit->SetText(text);
    g_windowMgr.SetFocus(this->m_amountEdit);
  }
  return 0;
}

// OFFSET: 0x008BDB00
// FUNCTION: fn_11801
int UICntWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETTEXT:
    this->m_text->SetText(reinterpret_cast<const char*>(a), 0);
    return 0;
  case UM_SET_COUNT: {
    char buffer[16];
    this->m_edit->SetText(_itoa(static_cast<int>(a), buffer, 10));
    return 0;
  }
  case UM_SET_TARGET:
    this->m_event = static_cast<int>(a);
    return 0;
  case UM_COMMAND:
    if (a == ID_OK) {
      CMode* mode = g_modeMgr.gameMode;
      mode->SendMsg(this->m_event, std::atoi(this->m_edit->Text()), 0, 0, 0);
    }
    g_windowMgr.Schedule(this);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008A34B0
// FUNCTION: fn_11653
UIAllyGuildAcceptWnd::UIAllyGuildAcceptWnd()
    : UIExchangeAcceptWnd() {
}

// OFFSET: 0x008A3510
// FUNCTION: fn_11654
UIBank_NewWnd::UIBank_NewWnd()
    : UIFrameWnd(0), m_amountEdit(nullptr), m_status() {
}

// OFFSET: 0x008A35A0
// FUNCTION: fn_11655
UIChatRoomWnd::UIChatRoomWnd()
    : UIFrameWnd(0), title(), unknown_00CC(), unknown_010C(), unknown_0114(), saved(nullptr) {
}

#pragma pack(push, 1)
struct ChatRoomRecordHeader {
  // INLINED
  ChatRoomRecordHeader() : tag(0), size(0) {}
  // INLINED
  ~ChatRoomRecordHeader() {
    this->size = 0;
    this->tag = 0;
  }

  /* 0000 */ std::int16_t tag;
  /* 0002 */ std::uint32_t size;
};
#pragma pack(pop)

// OFFSET: 0x008BB9E0
// FUNCTION: fn_11800
int UIChatRoomWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO2:
    this->saved = reinterpret_cast<ChatRoomWndInfo*>(a);
    if (this->saved->IsValid() == 0) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      if (this->edit->Length() == 0) {
        return 0;
      }
      if (g_navigationLinkParser->HasLink(std::string(this->edit->Text()))) {
        this->edit->SetText("");
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CLOSE)), 255, 0, 0, 0);
        return 0;
      }

      std::string text = this->edit->Text();
      if (!this->edit->links.empty()) {
        text = this->edit->LinkedText();
      } else if (_mbsstr(reinterpret_cast<const unsigned char*>(this->edit->Text()), reinterpret_cast<const unsigned char*>("<"))) {
        UIItemTagOnChat* tags = this->edit->unknown_0144;
        if (tags != nullptr && !tags->unknown_0140.empty()) {
          text = tags->unknown_0140;
        }
      }

      char buffer[256];
      if (!text.empty()) {
        std::strcpy(buffer, text.c_str());
      } else {
        std::strcpy(buffer, this->edit->Text());
      }

      if (g_serviceType == 1 && !ValidateCharacterName(buffer)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ENGLISHONLY)), 255, 0, 0, 0);
        return 0;
      }

      bool self = false;
      if (this->memberList->selected != -1) {
        const std::string name = this->memberList->lines[this->memberList->selected];
        self = name == g_session.DecodePlayerName();
      }

      if (this->memberList->selected == -1 || buffer[0] == '/' || self) {
        if (g_session.ProcessEmotionShortcut(buffer) == 0) {
          std::string parts[3];
          int talkType;
          const int offset = g_session.GetTalkType(buffer, &talkType, parts);
          if (offset != -1) {
            g_windowMgr.SetUnknown04DC(buffer + offset);
          }
          if (talkType == 0) {
            talkType = 50;
          } else if (talkType == 51) {
            talkType = 52;
          }
          g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, talkType, reinterpret_cast<int>(parts), 0, 0);
        }
      } else {
        g_windowMgr.unknown_04DC = buffer;
        const std::string name = this->memberList->lines[this->memberList->selected];
        g_modeMgr.gameMode->SendMsg(MM_WHISPERMSG, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
      }

      this->edit->PushHistory();
      this->edit->SetText("");
      g_windowMgr.SetFocus(this->edit);
      return 0;
    }
    case ID_CLOSE:
      g_modeMgr.gameMode->SendMsg(MM_REQ_EXIT_ROOM, 0, 0, 0, 0);
      return 0;
    case ID_NAME:
      g_windowMgr.SelectWhisperName(reinterpret_cast<const char*>(b));
      return 0;
    default:
      return 0;
    }
  case UM_MENU_SELECTED: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    switch (this->unknown_0114[a].second) {
    case 0: {
      if (this->memberList->selected == -1) {
        return 0;
      }
      const std::string name = this->memberList->lines[this->memberList->selected];
      mode->SendMsg(MM_REQ_EXPEL_MEMBER, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
      return 0;
    }
    case 1: {
      if (this->memberList->selected == -1) {
        return 0;
      }
      const std::string name = this->memberList->lines[this->memberList->selected];
      mode->SendMsg(MM_REQ_ROLE_CHANGE, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
      return 0;
    }
    case 2:
      g_windowMgr.EnsureWindow(WID_CHATROOMCHANGEWND)->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&mode->unknown_03C8), 0, 0, 0);
      return 0;
    case 3:
      g_windowMgr.Confirm(LookupMessage(MSI_NOTSUPPORTEDYET), 0, true, false, 0, "Message", 280, 120, 0);
      return 0;
    case 11:
      this->SaveChatLog();
      return 0;
    case 21: {
      if (this->memberList->selected == -1) {
        return 0;
      }
      const std::string name = this->memberList->lines[this->memberList->selected];
      g_windowMgr.RequestAddFriend(name.c_str());
      return 0;
    }
    default:
      return 0;
    }
  }
  case UM_MAKEMENU: {
    this->unknown_0114.clear();
    if (g_windowMgr.unknown_0210 != nullptr) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }

    {
      const std::string self = g_session.DecodePlayerName();
      if (this->unknown_010C.find(self) == this->unknown_010C.end()) {
        return 0;
      }
      const int rank = this->unknown_010C[self];
      if (rank == -1) {
        return 0;
      }

      if (this->memberList->selected != -1) {
        const std::string target = this->memberList->lines[this->memberList->selected];
        if (this->unknown_010C.find(target) == this->unknown_010C.end() || this->unknown_010C[target] == -1) {
          return 0;
        }
        switch (rank) {
        case 0:
          if (!(target == self)) {
            this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_BAN)), 0));
            this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_GIVE_GIVE_ROOM_POWER)), 1));
          }
          this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_SEE_INFORMATION_OF_CHARACTER)), 3));
          if (g_serviceMode != 3 && !g_windowMgr.IsFriend(target.c_str()) && !(target == self)) {
            this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_ADD_TO_FRIEND_CHAR_LIST)), 21));
          }
          this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_CHANGE_ROOM_SETTING)), 2));
          break;
        case 1:
          this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_SEE_INFORMATION_OF_CHARACTER)), 3));
          if (!g_windowMgr.IsFriend(target.c_str()) && !(target == self) && g_serviceMode != 3) {
            this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_ADD_TO_FRIEND_CHAR_LIST)), 21));
          }
          break;
        }
      } else {
        if (rank == 0) {
          this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_CHANGE_ROOM_SETTING)), 2));
        }
        this->unknown_0114.push_back(std::pair<std::string, int>(std::string(LookupMessage(MSI_WRITE_CHAT_TO_FILE)), 11));
      }
    }

    if (!this->unknown_0114.empty()) {
      UIWindow* menu = g_windowMgr.EnsureWindow(WID_MENUWND);
      for (unsigned index = 0; index < this->unknown_0114.size(); ++index) {
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->unknown_0114[index].first.c_str()), 0, 0, 0);
      }
      menu->SendMsg(nullptr, UM_SET_CALLER_WID, 28, 0, 0, 0);
      menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);

      int x = g_mouse.x + 1;
      int y = g_mouse.y + 1;
      if (x < 1) {
        x = 1;
      }
      int maxX = g_renderer->viewport.width - menu->geometry.width;
      if (x <= maxX) {
        maxX = x;
      }
      if (y < 1) {
        y = 1;
      }
      int maxY = g_renderer->viewport.height - menu->geometry.height;
      if (y <= maxY) {
        maxY = y;
      }
      menu->Move(maxX, maxY);
    }
    return 0;
  }
  case UM_SETINFO: {
    const ChatRoomTitleInfo* settings = reinterpret_cast<const ChatRoomTitleInfo*>(a);
    this->title = settings->title;
    this->unknown_00CC = settings->unknown_0018;
    this->roomStatus.roomType = settings->type;
    this->roomStatus.memberCount = settings->memberCount;
    this->roomStatus.memberLimit = settings->memberLimit;
    this->roomStatus.roomId = settings->id;

    const std::size_t end = this->title.find('\r');
    if (end != std::string::npos) {
      this->title = this->title.substr(0, end);
    }

    std::string prefix;
    prefix = LookupMessage((this->roomStatus.roomType != 0) + 89);
    char text[256];
    std::sprintf(text, "%s%s (%d/%d)", prefix.c_str(), this->title.c_str(), this->roomStatus.memberCount, this->roomStatus.memberLimit);
    if (g_forbiddenWords.Contains(text)) {
      std::string masked;
      g_forbiddenWords.Mask(text, &masked);
    }
    this->titleText->SetTextFitted(text);
    this->Invalidate();
    return 0;
  }
  case UM_ROLE_CHANGE: {
    if (this->unknown_010C.find(std::string(reinterpret_cast<const char*>(a))) != this->unknown_010C.end()) {
      this->unknown_010C[std::string(reinterpret_cast<const char*>(a))] = static_cast<int>(b);
    }
    this->memberList->Clear();
    for (std::map<std::string, int>::iterator it = this->unknown_010C.begin(); it != this->unknown_010C.end(); ++it) {
      this->memberList->AddText(it->first.c_str(), it->second != 0 ? 0 : 0x808000, nullptr);
    }
    return 0;
  }
  case UM_MEMBER_EXIT: {
    const std::map<std::string, int>::iterator found = this->unknown_010C.find(std::string(reinterpret_cast<const char*>(a)));
    if (found == this->unknown_010C.end()) {
      return 0;
    }
    this->unknown_010C.erase(found);
    this->memberList->Clear();
    for (std::map<std::string, int>::iterator it = this->unknown_010C.begin(); it != this->unknown_010C.end(); ++it) {
      this->memberList->AddText(it->first.c_str(), it->second != 0 ? 0 : 0x808000, nullptr);
    }
    return 0;
  }
  case UM_CLEARMEMBER:
    this->memberList->Clear();
    this->unknown_010C.erase(this->unknown_010C.begin(), this->unknown_010C.end());
    return 0;
  case UM_ADDMEMBER: {
    this->unknown_010C[std::string(reinterpret_cast<const char*>(a))] = static_cast<int>(b);
    int color = 0;
    if (b != 0) {
      if (g_serviceOptions.Get(11) && g_soundMgr->unknown != 0) {
        g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
    } else {
      color = 0x808000;
    }
    this->memberList->AddText(reinterpret_cast<const char*>(a), color, nullptr);
    return 0;
  }
  case UM_ADDCHAT: {
    char* text = reinterpret_cast<char*>(a);
    if (g_forbiddenWords.Contains(text)) {
      std::string masked("");
      g_forbiddenWords.Mask(text, &masked);
    }

    std::vector<std::string> lines;
    char name[25] = {};
    const int wrapWidth = this->geometry.width * 80 / 640;
    {
      const std::string message(text);
      const int length = static_cast<int>(std::strlen(text));
      const int enter = static_cast<int>(message.find(")님이 입장 하였습니다.", 0));
      const int leave = static_cast<int>(message.find(")님이 퇴장 하였습니다.", 0));
      const int colon = static_cast<int>(message.find(" :", 0));
      if (enter > 0 && enter < length) {
        if (g_serviceType == 19 && g_tipOfTheDayEnglish) {
          std::memcpy(name, text + enter + 1, length - enter);
          name[length - enter] = '\0';
        } else {
          std::memcpy(name, text + 1, enter - 1);
          name[enter - 1] = '\0';
        }
      } else if (leave > 0 && leave < length) {
        if (g_serviceType == 19 && g_tipOfTheDayEnglish) {
          std::memcpy(name, text + leave + 1, length - leave);
          name[length - leave] = '\0';
        } else {
          std::memcpy(name, text + 1, leave - 1);
          name[leave - 1] = '\0';
        }
      } else if (colon > 0 && colon < length && colon < 24) {
        if (g_serviceType == 19 && g_tipOfTheDayEnglish) {
          std::memcpy(name, text + colon, length - colon);
          name[length - colon] = '\0';
        } else {
          std::memcpy(name, text, colon);
          name[colon] = '\0';
        }
      }
      if (std::strcmp(name, g_session.DecodePlayerName()) == 0) {
        name[0] = '\0';
      }
    }

    if (_mbsstr(reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>("<ITEML>")) ||
        _mbsstr(reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>("</ITEML>"))) {
      lines.push_back(std::string(text));
    } else {
      WrapPlainText(text, lines, wrapWidth);
    }

    for (unsigned index = 0; index < lines.size(); ++index) {
      this->history->AddText(lines[index].c_str(), static_cast<int>(b), name);
    }
    return 0;
  }
  case UM_PAGEUPPUSH:
    if (this->history->verticalOverflow != 0) {
      this->history->SendMsg(nullptr, UM_SCROLLVERTPAGEUP, 0, 0, 0, 0);
    }
    return 0;
  case UM_PAGEDOWNPUSH:
    if (this->history->verticalOverflow != 0) {
      this->history->SendMsg(nullptr, UM_SCROLLVERTPAGEDOWN, 0, 0, 0, 0);
    }
    return 0;
  case UM_RESIZE: {
    if (a != 7) {
      return 0;
    }
    int width = b > 512 ? 512 : b;
    int height = c > 384 ? 384 : c;
    if (width < 280) {
      width = 280;
    }
    if (height < 120) {
      height = 120;
    }
    width = (width - 280) / 32 * 32 + 280;
    height = (height - 120) / 32 * 32 + 120;
    if (width != this->geometry.width || height != this->geometry.height) {
      this->Resize(width, height);
      this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
      this->edit->Resize(this->geometry.width - 30, 16);
      this->edit->Move(3, this->geometry.height - 18);
      this->history->Resize(this->geometry.width - this->geometry.width * 56 / 280 - 3, this->geometry.height - 38);
      this->history->Move(1, 17);
      this->memberList->Resize(this->geometry.width * 56 / 280, this->geometry.height - 38, false);
      this->memberList->Move(this->geometry.width - this->geometry.width * 56 / 280 - 1, 17);
      this->closeButton->Move(this->geometry.width - 14, 3);
      const std::string title = this->titleText->text;
      this->titleText->Resize(this->geometry.width - 30, 12);
      this->titleText->SetTextFitted(title.c_str());
    }
    g_windowMgr.StoreSnapRectangle(this);
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* data = reinterpret_cast<const unsigned char*>(a);
    const unsigned size = static_cast<unsigned>(b);
    ChatRoomRecordHeader header;
    std::memcpy(&header, data, sizeof(header));
    if (header.tag == 0x5014) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ROOM_IS_MADE)), 0xFFFF00, 0, 0, 0);
      ChatRoomTitleInfo* settings = reinterpret_cast<ChatRoomTitleInfo*>(&static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_03C8);
      unsigned offset = 6;
      while (offset < size) {
        std::memcpy(&header, data + offset, sizeof(header));
        offset += 6;
        switch (header.tag) {
        case 0x5015:
          return 0;
        case 0x5016:
          std::memcpy(this->saved, data + offset, header.size);
          offset += header.size;
          break;
        case 0x5017: {
          char value[260];
          std::memset(value, 0, sizeof(value));
          std::memcpy(value, data + offset, header.size);
          offset += header.size;
          settings->title = this->title = value;
          break;
        }
        case 0x5018: {
          char value[260];
          std::memset(value, 0, sizeof(value));
          std::memcpy(value, data + offset, header.size);
          offset += header.size;
          settings->unknown_0018 = this->unknown_00CC = value;
          break;
        }
        case 0x5019:
        std::memcpy(&this->roomStatus.roomType, data + offset, header.size);
          offset += header.size;
        settings->type = this->roomStatus.roomType;
          break;
        case 0x501A:
          std::memcpy(&this->roomStatus.memberCount, data + offset, header.size);
          offset += header.size;
          settings->memberCount = this->roomStatus.memberCount;
          break;
        case 0x501B:
          std::memcpy(&this->roomStatus.memberLimit, data + offset, header.size);
          offset += header.size;
          settings->memberLimit = this->roomStatus.memberLimit;
          break;
        case 0x501C:
          std::memcpy(&this->roomStatus.roomId, data + offset, header.size);
          offset += header.size;
          settings->id = this->roomStatus.roomId;
          this->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(settings), 0, 0, 0);
          break;
        case 0x501E: {
          char value[260];
          std::memset(value, 0, sizeof(value));
          std::memcpy(value, data + offset, header.size);
          offset += header.size;
          this->SendMsg(nullptr, UM_CLEARMEMBER, 0, 0, 0, 0);
          this->SendMsg(nullptr, UM_ADDMEMBER, reinterpret_cast<std::intptr_t>(value), 0, 0, 0);
          break;
        }
        case 0x501F: {
          char value[260];
          std::memset(value, 0, sizeof(value));
          std::memcpy(value, data + offset, header.size);
          offset += header.size;
          this->SendMsg(nullptr, UM_ADDMEMBER, reinterpret_cast<std::intptr_t>(value), 1, 0, 0);
          break;
        }
        default:
          offset += header.size;
          break;
        }
      }
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x008A36A0
// FUNCTION: fn_11656
UICntWnd::UICntWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x008A37F0
// FUNCTION: fn_11658
UIHomunInfoWnd::UIHomunInfoWnd() : UIFrameWnd(0) {
  this->unknown_00D4 = nullptr;
  this->unknown_00D0 = nullptr;
}

// OFFSET: 0x008A3860
// FUNCTION: fn_11659
UILoginOTPWnd::UILoginOTPWnd()
    : UIFrameWnd(0), unknown_00B4(0), unknown_00B8(0), timerText(nullptr), unknown_00C0(0) {
}

// OFFSET: 0x008A38E0
// FUNCTION: fn_11660
UILoginWnd::UILoginWnd() : UIFrameWnd(0), unknown_00D8(), unknown_00E8(0), unknown_00EC("G000") {
  this->idEdit = nullptr;
  this->passwordEdit = nullptr;
  this->unknown_00C4 = 0;
  this->saveId = 0;
  this->unknown_00CC = 1;
  this->unknown_00D0 = 0;
  this->unknown_00D4 = 0;
  this->unknown_00BC = nullptr;
}
