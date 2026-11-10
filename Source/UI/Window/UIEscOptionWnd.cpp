#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEscOptionWnd.h"
#include "UI/Window/UIEnums.h"

#include "Client/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "Utility/Text/FormatString.h"

#include <string>
#include "Resource/MSGSTRINGID.h"

// OFFSET: 0x0091F4F0
// FUNCTION: fn_12638
void UIEscOptionWnd::OnCreate(int, int) {
  const std::string prefix("유저인터페이스\\");
  const char* names[7] = {"esc_01", "esc_001", "esc_08", "esc_09", "esc_02", "esc_05", "esc_10"};
  int ids[7] = {371, 458, 370, 251, 185, 353, 372};

  for (int i = 0; i < 7; ++i) {
    this->m_buttons[i] = new UIBitmapButton();

    this->m_buttons[i]->SetBitmap((prefix + names[i] + "a.bmp").c_str(), 0, 0);
    this->m_buttons[i]->SetBitmap((prefix + names[i] + "b.bmp").c_str(), 1, 0);
    this->m_buttons[i]->SetBitmap((prefix + names[i] + "c.bmp").c_str(), 2, 0);
    this->m_buttons[i]->SetSize(this->m_buttons[i]->BitmapWidth(), this->m_buttons[i]->BitmapHeight());
    this->m_buttons[i]->Move(-100, -100);
    this->m_buttons[i]->SetId(ids[i]);
    this->AddChild(this->m_buttons[i]);
  }

  this->LayoutButtons();
}

// OFFSET: 0x0092BDE0
// FUNCTION: fn_12662
void UIEscOptionWnd::Paint() {
  this->m_titleHeight = this->DrawTitleBar(true, FormatOwnedString("%s (%s)", LookupMessage(MSI_ESC_OPTIONWND), ShortcutFilename(g_session.m_gid).c_str()).c_str(), 0);

  this->DrawPanelBackground(this->m_titleHeight, 0, "");
}

// OFFSET: 0x00933B90
// FUNCTION: fn_12735
void UIEscOptionWnd::LayoutButtons() {
  for (int i = 0; i < 7; ++i) {
    this->m_buttons[i]->Move(-100, -100);
  }

  int titleHeight = this->m_titleHeight;
  if (this->m_mode == 0) {
    this->Resize(this->geometry.width, titleHeight + 122);
    for (int i = 0; i < 5; ++i) {
      this->m_buttons[i]->Move(29, this->m_titleHeight + 5 + i * 23);
    }
  } else if (this->m_mode == 1) {
    this->Resize(this->geometry.width, titleHeight + 53);
    this->m_buttons[6]->Move(29, this->m_titleHeight + 5);
    this->m_buttons[4]->Move(29, this->m_titleHeight + 28);
  } else if (this->m_mode == 2) {
    this->Resize(this->geometry.width, titleHeight + 76);
    this->m_buttons[5]->Move(29, this->m_titleHeight + 5);
    this->m_buttons[6]->Move(29, this->m_titleHeight + 28);
    this->m_buttons[4]->Move(29, this->m_titleHeight + 51);
  }
}

// OFFSET: 0x00911250
// FUNCTION: fn_12512
UIEscOptionWnd::UIEscOptionWnd()
    : UIFrameWnd(0), m_titleHeight(17) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int escMode;
  if (mode->isPlayerDead != 0) {
    if (!mode->HasResurrectionItem() && !g_session.HasSkillEffect(580)) {
      escMode = 1;
    } else {
      escMode = 2;
    }
  } else {
    escMode = 0;
  }
  this->m_mode = escMode;
}

#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00934FA0
// FUNCTION: fn_12744
int UIEscOptionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_OPEN_HOTKEY:
    if (!g_windowMgr.CloseWindow(WID_HOTKEYWND)) {
      g_windowMgr.EnsureWindow(WID_HOTKEYWND);
    }
    break;
  case ID_QUIT:
    g_windowMgr.CloseWindow(WID_ESCOPTIONWND);
    mode->SendMsg(MM_QUIT_GAME, 0, 0, 0, 0);
    break;
  case ID_REBIRTH:
    mode->SendMsg(MM_REBIRTH, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_ESCOPTIONWND);
    break;
  case ID_RETURN_TO_CHARACTER_SELECT:
    g_characterLinkedUserDataMgr->Save();
    g_adventurerAgencyMgr->RequestStop();
    mode->SendMsg(MM_RESTART, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_ESCOPTIONWND);
    g_windowMgr.CloseWindow(WID_SEEKPARTYWND);
    g_windowMgr.CloseWindow(WID_ROULLETTEWND);
    break;
  case ID_OPEN_GAME_SETTINGS:
    if (!g_windowMgr.CloseWindow(WID_GAMESETTINGSUI)) {
      g_windowMgr.EnsureWindow(WID_GAMESETTINGSUI);
    }
    break;
  case 0x1C9:
    if (!g_windowMgr.CloseWindow(0xEC)) {
      g_windowMgr.EnsureWindow(0xEC);
    }
    break;
  case ID_RETURN_TO_LOGIN:
    if (g_windowMgr.Confirm(LookupMessage(MSI_MOVETO_SAVEPOINT), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
      mode->SendMsg(MM_RESTART, 0, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_ESCOPTIONWND);
    }
    break;
  case ID_CANCEL:
    g_windowMgr.CloseWindow(WID_ESCOPTIONWND);
    break;
  }
  return 0;
}
