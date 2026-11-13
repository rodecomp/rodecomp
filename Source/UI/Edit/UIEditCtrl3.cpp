#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UINaviLinkButton.h"
#include "UI/Navigation/NavigationLink.h"

// OFFSET: 0x00861AB0
// FUNCTION: fn_10902
void UIEditCtrl3::UpdateScrollBar() {
  const int visible = this->geometry.height / this->lineHeight;
  const int total = this->lineCount;
  int maximum = total - visible;
  if (maximum == 1 && this->firstVisibleLine == 0) {
    maximum = 0;
  }
  if (maximum < 0) {
    maximum = 0;
  }
  this->scrollEnabled = maximum > 0;
  if (maximum > 0) {
    static_cast<UIScrollBar*>(this->scrollBar)->SetRange(maximum, visible, total);
    static_cast<UIScrollBar*>(this->scrollBar)->SetPosition(this->firstVisibleLine);
  } else {
    static_cast<UIScrollBar*>(this->scrollBar)->SetRange(0, visible, total);
    this->firstVisibleLine = 0;
  }

  this->scrollBar->Invalidate();
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

  if (value.find("<") == std::string::npos && value.find(">") == std::string::npos) {
    return;
  }

  for (auto entry = this->links.begin(); entry != this->links.end(); ++entry) {
    if (this->RemoveInvalidLink(438, entry)) {
      break;
    }

    auto* button = static_cast<UINaviLinkButton*>(*entry);
    const std::string name = g_navigationLinkParser->FormatNavigationLink(button->Link(), false);
    const std::string fitted = this->FitTextToWidth(name, this->geometry.width / 2);
    std::string display = "<";
    display += fitted;
    display += ">";
    const std::size_t found = remaining.find(display);

    if (found != std::string::npos) {
      this->pendingText[line].clear();
      prefix = remaining.substr(0, found);
      rest = remaining.substr(found + std::strlen(display.c_str()));

      int x = 3;

      if (this->invalidatesParent == 0 && this->firstVisible != 0) {
        x = 3 - this->TextWidth(this->text.c_str(), this->firstVisible, 0, 12, 0, 0);
      }

      button->line = line;
      (*entry)->Move(x - 1 + this->TextWidth(prefix.c_str(), 0, 0, 12, 0, 0), y);
    }

    const std::string visible = this->FitTextToWidth(name, this->geometry.width / 2);
    this->SnapLinkSelection(prefix, static_cast<int>(std::strlen(visible.c_str())));
    this->pendingText[line] += prefix;
    this->pendingText[line] += button->Link();
    this->pendingText[line] += rest;
  }
}
