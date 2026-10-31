#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UINaviLinkButton.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/TextFitting.h"

// OFFSET: 0x00861AB0
// FUNCTION: fn_10902
void UIEditCtrl3::UpdateScrollBar() {
  const int visible = this->geometry.height / this->unknown_0138;
  const int total = this->unknown_013C;
  int maximum = total - visible;
  if (maximum == 1 && this->unknown_0158 == 0) {
    this->unknown_0134 = 0;
    static_cast<UIScrollBar*>(this->unknown_0130)->SetRange(0, visible, total);
    this->unknown_0158 = 0;
  } else {
    if (maximum < 0) {
      maximum = 0;
    }
    this->unknown_0134 = maximum > 0;
    if (maximum > 0) {
      static_cast<UIScrollBar*>(this->unknown_0130)->SetRange(maximum, visible, total);
      static_cast<UIScrollBar*>(this->unknown_0130)->SetPosition(this->unknown_0158);
    } else {
      static_cast<UIScrollBar*>(this->unknown_0130)->SetRange(0, visible, total);
      this->unknown_0158 = 0;
    }
  }

  this->unknown_0130->Invalidate();
}

// OFFSET: 0x00861B50
// FUNCTION: fn_10903
void UIEditCtrl3::PlaceLinks(const std::string& value, int y, int line, int firstLine, int endLine) {
  if (this->links.size() == 0) {
    return;
  }

  bool tagged = false;

  for (auto entry = this->links.begin(); entry != this->links.end(); ++entry) {
    UIWindow* window = *entry;

    if (window->geometry.id != 438) {
      continue;
    }

    tagged = true;

    if (static_cast<UINaviLinkButton*>(window)->line < firstLine || static_cast<UINaviLinkButton*>(window)->line > endLine - 1) {
      window->Move(-1000, -1000);
    }
  }

  if (!tagged) {
    return;
  }

  const std::string remaining = value;
  std::string prefix;
  std::string rest;

  if (remaining.find('<') == std::string::npos && remaining.find('>') == std::string::npos) {
    return;
  }

  for (auto entry = this->links.begin(); entry != this->links.end(); ++entry) {
    if (this->RemoveInvalidLink(entry)) {
      break;
    }

    auto* button = static_cast<UINaviLinkButton*>(*entry);
    const std::string name = FormatNavigationLink(button->link, false);
    const std::string fitted = FitText(*this->geometry.surface, name, this->geometry.width / 2);
    const std::string display = "<" + fitted + ">";
    const std::size_t found = remaining.find(display);

    if (found != std::string::npos) {
      this->pendingText[line].clear();
      prefix = remaining.substr(0, found);
      rest = remaining.substr(found + display.size());

      int x = 3;

      if (this->invalidatesParent == 0 && this->firstVisible != 0) {
        x = 3 - this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, this->text.c_str(), this->firstVisible, 0, 12, 0, 0);
      }

      button->line = line;
      button->Move(x - 1 + this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, prefix.c_str(), 0, 0, 12, 0, 0), y);
    }

    this->SnapLinkSelection(prefix.size(), fitted.size());
    this->pendingText[line] += prefix;
    this->pendingText[line] += button->link;
    this->pendingText[line] += rest;
  }
}
