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
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
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
  this->m_titleHeight = this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, title.c_str(), 0);
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
UIEscOptionWnd::UIEscOptionWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_titleHeight(17), m_controls(&controls) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int escMode;
  if (mode->unknown_0250 == 0) {
    escMode = 0;
  } else if (!mode->HasResurrectionItem() && !reinterpret_cast<const ClientSkills&>(g_session).HasEffect(580)) {
    escMode = 1;
  } else {
    escMode = 2;
  }
  this->m_mode = escMode;
}
