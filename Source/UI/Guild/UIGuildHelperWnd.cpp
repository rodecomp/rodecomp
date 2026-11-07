#include "UI/Guild/UIGuildHelperWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Text/UIStaticText.h"
#include "Contents/GuildTip.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Text/UITextViewer.h"

#include <mbstring.h>
#include <string>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24878
static const std::string s_itemLinkOpening = "<ITEML>";

// GLOBAL: gl_24879
static const std::string s_itemLinkClosing = "</ITEML>";

// GLOBAL: gl_25120
CGuildTip g_guildTip;

// OFFSET: 0x00953960
// FUNCTION: fn_12996
UIGuildHelperWnd::UIGuildHelperWnd() : UIFrameWnd(0) {
}

// OFFSET: 0x00953810
// FUNCTION: fn_12993
UICreateGuildWnd::UICreateGuildWnd(int mode) : m_mode(mode) {
}

// OFFSET: 0x00953A90
// FUNCTION: fn_12998
UIGuildLeaveReasonDescWnd::UIGuildLeaveReasonDescWnd()
    : UIFrameWnd(0), m_reasonEdit(nullptr), m_reasonText(nullptr) {
}

// OFFSET: 0x00953880
// FUNCTION: fn_12994
UIGuildBanReasonDescWnd::UIGuildBanReasonDescWnd() {
}

// OFFSET: 0x0095DB20
// FUNCTION: fn_13071
void UIGuildHelperWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(2074), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(15, 32, LookupMessage(2075), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0095FCB0
// FUNCTION: fn_13084
void UIGuildHelperWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
    if (UIWindow* window = g_windowMgr.unknown_0438) {
      window->geometry.targetAlpha = 128;
      window->geometry.alphaStart = timeGetTime();
    }
  }
}

// OFFSET: 0x00960130
// FUNCTION: fn_13095
void UIGuildHelperWnd::OnLBtnUp(int x, int y) {
  UIFrameWnd::OnLBtnUp(x, y);
  if (UIWindow* window = g_windowMgr.unknown_0438) {
    window->geometry.targetAlpha = 255;
    window->geometry.alphaStart = timeGetTime();
  }
}

// OFFSET: 0x009601D0
// FUNCTION: fn_13098
void UIGuildHelperWnd::OnMouseMove(int x, int y) {
  UIFrameWnd::OnMouseMove(x, y);
  if (g_windowMgr.unknown_0438) {
    x = 0;
    y = 0;
    this->ToScreen(x, y);
    g_windowMgr.unknown_0438->Move(x + 2 + this->geometry.width, y);
  }
}

// OFFSET: 0x0095D5C0
// FUNCTION: fn_13069
void UICreateGuildWnd::Paint() {
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  const int text = this->m_mode == 0 ? 2077 : 2089;
  this->DrawTitleBar(true, LookupMessage(text - 1), 0);
  this->DrawTextAt(16, 23, LookupMessage(text), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0095FBE0
// FUNCTION: fn_13082
void UICreateGuildWnd::OnLBtnDown(int, int) {
}

// OFFSET: 0x009601C0
// FUNCTION: fn_13097
void UICreateGuildWnd::OnMouseMove(int, int) {
}

// OFFSET: 0x00960120
// FUNCTION: fn_13094
void UICreateGuildWnd::OnLBtnUp(int, int) {
}

// OFFSET: 0x0095F420
// FUNCTION: fn_13078
void UIGuildTipWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(2078), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x0095E310
// FUNCTION: fn_13073
void UIGuildLeaveReasonDescWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x00965650
// FUNCTION: fn_13138
void UICreateGuildWnd::StoreInfo() {
}

// OFFSET: 0x00965660
// FUNCTION: fn_13139
void UIGuildHelperWnd::StoreInfo() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

// OFFSET: 0x009612E0
// FUNCTION: fn_13123
int UICreateGuildWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }

  switch (a1) {
  case 184: {
    CMode* mode = g_modeMgr.GetGameMode();
    std::string name = "";
    name = this->m_nameEdit->Text();
    this->m_nameEdit->SelectAll();

    if (name.empty()) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2080)), 0xFF, 0, 0, 0);
    } else if (_mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_itemLinkOpening.c_str())) ||
               _mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_itemLinkClosing.c_str()))) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2812)), 0xFF, 0, 0, 0);
    } else if (this->m_mode == 0) {
      mode->SendMsg(112, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
    } else {
      mode->SendMsg(106, reinterpret_cast<int>(name.c_str()), 0, 0, 0);
      g_windowMgr.CloseWindow(215);
    }
    break;
  }
  case 185:
    if (this->m_mode == 0) {
      g_windowMgr.CloseWindow(213);
    } else {
      g_windowMgr.CloseWindow(215);
    }
    break;
  }

  return 0;
}

// OFFSET: 0x00961540
// FUNCTION: fn_13124
int UIGuildBanReasonDescWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  switch (message) {
  case 6:
    switch (a1) {
    case 184: {
      g_windowMgr.CloseWindow(76);
      CMode* mode = g_modeMgr.gameMode;
      mode->SendMsg(111, reinterpret_cast<int>(&this->m_target), reinterpret_cast<int>(this->m_reasonEdit->Text()), 0, 0);
      break;
    }
    case 185:
      g_windowMgr.CloseWindow(76);
      break;
    }
    return 0;
  case 34: {
    const GuildBanTarget* target = reinterpret_cast<const GuildBanTarget*>(a1);
    this->m_target.unknown_0000 = target->unknown_0000;
    this->m_target.unknown_0004 = target->unknown_0004;
    this->m_target.unknown_0008 = target->unknown_0008;
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }
}

// OFFSET: 0x00961710
// FUNCTION: fn_13126
int UIGuildHelperWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  switch (message) {
  case 6:
    switch (a1) {
    case 184:
      g_windowMgr.CloseWindow(214);
      g_windowMgr.CloseWindow(213);
      g_windowMgr.CloseWindow(212);
      break;
    case 343:
      if (!g_windowMgr.CloseWindow(214)) {
        g_windowMgr.EnsureWindow(214);
      }
      break;
    case 448:
      if (!g_windowMgr.CloseWindow(213)) {
        g_windowMgr.EnsureWindow(213);
        if (g_windowMgr.unknown_0438) {
          int x = 0;
          int y = 0;
          this->ToScreen(x, y);
          g_windowMgr.unknown_0438->Move(x + 2 + this->geometry.width, y);
        }
      }
      break;
    }
    return 0;
  case 34:
    this->m_saved = reinterpret_cast<GuildHelperWndInfo*>(a1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
  }
}

// OFFSET: 0x00964C60
// FUNCTION: fn_13133
int UIGuildTipWnd::SendMsg(UIWindow*, int message, std::intptr_t a1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && a1 == 201) {
    g_windowMgr.CloseWindow(214);
  }

  return 0;
}

// OFFSET: 0x00958070
// FUNCTION: fn_13055
void UICreateGuildWnd::OnCreate(int, int) {
  this->m_nameEdit = new UIEditCtrl();
  this->m_nameEdit->SetSize(124, 20);
  this->m_nameEdit->Move(13, 42);
  this->m_nameEdit->maximum = 23;
  this->m_nameEdit->Background(230, 230, 230);
  this->AddChild(this->m_nameEdit);

  {
    const std::string prefix("유저인터페이스\\");
    int positions[2][2] = {{this->geometry.width - 92, this->geometry.height - 24}, {this->geometry.width - 46, this->geometry.height - 24}};
    const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    int ids[2] = {184, 185};
    std::string name;

    for (unsigned int i = 0; i < 2; ++i) {
      auto* button = new UIBitmapButton();
      name.assign(prefix);
      name += names[i][0];
      name += ".bmp";
      button->SetBitmap(name.c_str(), 0, 0);
      name.assign(prefix);
      name += names[i][1];
      name += ".bmp";
      button->SetBitmap(name.c_str(), 1, 0);
      name.assign(prefix);
      name += names[i][2];
      name += ".bmp";
      button->SetBitmap(name.c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[i][0], positions[i][1]);
      button->SetId(ids[i]);
      this->AddChild(button);
    }
  }

  g_windowMgr.SetFocus(this->m_nameEdit);
  this->m_message0 = 184;
}

// OFFSET: 0x00958990
// FUNCTION: fn_13058
void UIGuildHelperWnd::OnCreate(int width, int height) {
  const std::string prefix("유저인터페이스\\guild_helper\\");
  const char* names[3][3] = {{"btn_help", "btn_help_a", "btn_help_b"}, {"create_guild", "create_guild_a", "create_guild_b"}, {"btn_ok", "btn_ok_a", "btn_ok_b"}};
  int ids[3] = {343, 448, 184};

  int x = 8;
  std::string name;
  for (int i = 0; i < 3; ++i) {
    this->m_button = new UIBitmapButton();
    name.assign(prefix);
    name += names[i][0];
    name += ".bmp";
    this->m_button->SetBitmap(name.c_str(), 0, 0);
    name.assign(prefix);
    name += names[i][1];
    name += ".bmp";
    this->m_button->SetBitmap(name.c_str(), 1, 0);
    name.assign(prefix);
    name += names[i][2];
    name += ".bmp";
    this->m_button->SetBitmap(name.c_str(), 2, 0);
    this->m_button->SetSize(this->m_button->BitmapWidth(), this->m_button->BitmapHeight());
    this->m_button->Move(i < 2 ? x : width - 52, height - 24);
    this->m_button->SetId(ids[i]);
    this->AddChild(this->m_button);
    x += 62;
  }
}

// OFFSET: 0x00958700
// FUNCTION: fn_13056
void UIGuildBanReasonDescWnd::OnCreate(int width, int height) {
  UIGuildLeaveReasonDescWnd::OnCreate(width, height);

  if (this->m_reasonText != nullptr) {
    this->m_reasonText->SetText(LookupMessage(0x20C), 0);
  }
}

// OFFSET: 0x009597E0
// FUNCTION: fn_13060
void UIGuildLeaveReasonDescWnd::OnCreate(int, int) {
  this->m_reasonEdit = new UIEditCtrl();
  this->m_reasonEdit->SetSize(this->geometry.width - 32, 16);
  this->m_reasonEdit->Move(15, 22);
  this->m_reasonEdit->maximum = 39;
  this->m_reasonEdit->Background(232, 232, 232);
  this->m_reasonEdit->SetText("");
  this->AddChild(this->m_reasonEdit);

  g_windowMgr.SetFocus(this->m_reasonEdit);

  this->m_reasonText = new UIStaticText();
  this->m_reasonText->SetSize(this->geometry.width - 30, 15);
  this->m_reasonText->Move(15, 6);
  this->m_reasonText->SetText(LookupMessage(523), 0);
  this->AddChild(this->m_reasonText);

  {
    const std::string prefix("유저인터페이스\\");
    int positions[2][2] = {{this->geometry.width - 101, this->geometry.height - 32}, {this->geometry.width - 55, this->geometry.height - 32}};
    const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    int ids[2] = {184, 185};
    std::string name;

    for (int i = 0; i < 2; ++i) {
      auto* button = new UIBitmapButton();
      name.assign(prefix);
      name += names[i][0];
      name += ".bmp";
      button->SetBitmap(name.c_str(), 0, 0);
      name.assign(prefix);
      name += names[i][1];
      name += ".bmp";
      button->SetBitmap(name.c_str(), 1, 0);
      name.assign(prefix);
      name += names[i][2];
      name += ".bmp";
      button->SetBitmap(name.c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(positions[i][0], positions[i][1]);
      button->SetId(ids[i]);
      this->AddChild(button);
    }
  }

  this->m_message0 = 184;
}

// OFFSET: 0x0095CCD0
// FUNCTION: fn_13065
void UIGuildTipWnd::OnCreate(int width, int height) {
  this->m_textViewer = new UITextViewer();
  this->m_textViewer->Configure(10, 22, width - 15, height - 55, 0);

  unsigned int red = 0;
  unsigned int green = 0;
  unsigned int blue = 0;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_textViewer->backgroundColor = 0xFF000000 | (blue << 16) | (green << 8) | red;
  this->m_textViewer->Invalidate();
  this->AddChild(this->m_textViewer);

  if (!g_guildTip.m_bTryLoading) {
    g_guildTip.Load();
  }

  if (g_guildTip.m_tipList.size() != 0) {
    std::vector<StringList> tipList = g_guildTip.GetTipList();
    for (const auto& tip : tipList) {
      for (unsigned int i = 0; i < tip.size(); ++i) {
        this->m_textViewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(tip[i].c_str()), 50, 0, 0);
      }
    }
  }

  const std::string prefix("유저인터페이스\\");
  this->m_closeButton = new UIBitmapButton();
  this->m_closeButton->SetBitmap((prefix + "btn_close" + ".bmp").c_str(), 0, 0);
  this->m_closeButton->SetBitmap((prefix + "btn_close_a" + ".bmp").c_str(), 1, 0);
  this->m_closeButton->SetBitmap((prefix + "btn_close_b" + ".bmp").c_str(), 2, 0);
  this->m_closeButton->SetSize(this->m_closeButton->BitmapWidth(), this->m_closeButton->BitmapHeight());
  this->m_closeButton->Move(width - 50, height - 24);
  this->m_closeButton->SetId(201);
  this->AddChild(this->m_closeButton);
}
