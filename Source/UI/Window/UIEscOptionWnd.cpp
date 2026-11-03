#include "UI/Window/UIEscOptionWnd.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "Utility/Text/FormatString.h"

#include <string>

// OFFSET: 0x0091F4F0
// FUNCTION: fn_12638
void UIEscOptionWnd::Slot15(int, int) {
  const std::string prefix("유저인터페이스\\");
  int ids[7] = {371, 458, 370, 251, 185, 353, 372};
  const char* names[7] = {"esc_01", "esc_001", "esc_08", "esc_09", "esc_02", "esc_05", "esc_10"};

  for (int i = 0; i < 7; ++i) {
    UIBitmapButton* button = new UIBitmapButton();
    this->m_buttons[i] = button;
    button->SetBitmap((prefix + names[i] + "a.bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i] + "b.bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i] + "c.bmp").c_str(), 2, 0);
    this->m_buttons[i]->SetSize(button->BitmapWidth(), button->BitmapHeight());
    this->m_buttons[i]->Move(-100, -100);
    this->m_buttons[i]->SetId(ids[i]);
    this->AddChild(this->m_buttons[i]);
  }

  this->LayoutButtons();
}

// OFFSET: 0x0092BDE0
// FUNCTION: fn_12662
void UIEscOptionWnd::Paint() {
  const std::string name = ShortcutFilename(g_session.m_gid);
  const std::string title = FormatOwnedString("%s (%s)", LookupMessage(1483), name.c_str());
  this->m_titleHeight = this->DrawTitleBar(true, title.c_str(), 0);
  this->DrawPanelBackground(this->m_titleHeight, 0, "");
}

// OFFSET: 0x00933B90
// FUNCTION: fn_12735
void UIEscOptionWnd::LayoutButtons() {
  for (int i = 0; i < 7; ++i) {
    this->m_buttons[i]->Move(-100, -100);
  }

  if (this->m_mode == 0) {
    this->Resize(this->geometry.width, this->m_titleHeight + 122);
    for (int i = 0; i < 5; ++i) {
      this->m_buttons[i]->Move(29, this->m_titleHeight + 5 + i * 23);
    }
  } else if (this->m_mode == 1) {
    this->Resize(this->geometry.width, this->m_titleHeight + 53);
    this->m_buttons[6]->Move(29, this->m_titleHeight + 5);
    this->m_buttons[4]->Move(29, this->m_titleHeight + 28);
  } else if (this->m_mode == 2) {
    this->Resize(this->geometry.width, this->m_titleHeight + 76);
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
  if (mode->unknown_0250 == 0) {
    escMode = 0;
  } else if (!mode->HasResurrectionItem() && !g_session.HasSkillEffect(580)) {
    escMode = 1;
  } else {
    escMode = 2;
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
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  const int id = static_cast<int>(argument1);
  if (id > 0x173) {
    if (id == 0x174) {
      const int result = g_windowMgr.Confirm(
        LookupMessage(0x60C), 2, false, false, 0, "Message", 0x118, 0x78, 0
      );
      if (result != 0xBB) {
        return 0;
      }
      mode->SendMsg(0x19, 0, 0, 0, 0);
      g_windowMgr.CloseWindow(0x9B);
      return 0;
    }
    if (id == 0x1C9) {
      if (!g_windowMgr.CloseWindow(0xEC)) {
        g_windowMgr.EnsureWindow(0xEC);
      }
      return 0;
    }
    if (id == 0x1CA) {
      if (!g_windowMgr.CloseWindow(0x271E)) {
        g_windowMgr.EnsureWindow(0x271E);
      }
      return 0;
    }
    return 0;
  }

  if (id == 0x173) {
    union SaveSlot {
      void (CCharacterLinkedUserDataMgr::*member)(CharacterConfigSave&, const char*, CAsyncWorkMgr&);
      void (__thiscall* function)(CCharacterLinkedUserDataMgr*);
    };
    SaveSlot saveSlot;
    saveSlot.member = &CCharacterLinkedUserDataMgr::Save;
    saveSlot.function(g_characterLinkedUserDataMgr);
    union StopSlot {
      void (CAdventurerAgencyMgr::*member)();
      void (__thiscall* function)(CAdventurerAgencyMgr*);
    };
    StopSlot stopSlot;
    stopSlot.member = static_cast<void (CAdventurerAgencyMgr::*)()>(&CAdventurerAgencyMgr::RequestStop);
    stopSlot.function(g_adventurerAgencyMgr);
    mode->SendMsg(0x19, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(0x9B);
    g_windowMgr.CloseWindow(0xA4);
    g_windowMgr.CloseWindow(0x10D);
    return 0;
  }

  if (id == 0xB9) {
    g_windowMgr.CloseWindow(0x9B);
    return 0;
  }
  if (id == 0xFB) {
    g_windowMgr.CloseWindow(0x9B);
    mode->SendMsg(0x58, 0, 0, 0, 0);
    return 0;
  }
  if (id == 0x161) {
    mode->SendMsg(0xFA, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(0x9B);
    return 0;
  }
  if (id == 0x172) {
    if (!g_windowMgr.CloseWindow(0x9C)) {
      g_windowMgr.EnsureWindow(0x9C);
    }
    return 0;
  }
  return 0;
}
