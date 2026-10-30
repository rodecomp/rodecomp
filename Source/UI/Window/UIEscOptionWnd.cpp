#include "UI/Window/UIEscOptionWnd.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIButton.h"
#include "Utility/Text/FormatString.h"

#include <string>

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
