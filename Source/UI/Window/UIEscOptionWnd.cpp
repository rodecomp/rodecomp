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
