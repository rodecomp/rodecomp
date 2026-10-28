#include "Contents/Items/ItemInfo.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Items/ItemNaming.h"
#include "UI/Text/ItemLinkDisplay.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "Render/Text/TextContext.h"

#include <cstring>
#include <bit>
#include <cstdlib>
#include <shellapi.h>

// INLINED
void UITextViewer::Activate(UITextButton* button, int id) {
  if (!button) {
    return;
  }

  auto& arguments = button->m_lines;
  const auto number = [&](unsigned index) { return std::atoi(arguments.at(index).c_str()); };

  switch (id) {
  case 437:
    if (!arguments.at(0).empty()) {
      ShellExecuteA(this->m_text->applicationWindow, "open", arguments[0].c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    }

    break;
  case 438: {
    if (arguments.empty()) {
      break;
    }

    const std::string map = arguments.at(0);
    const int x = number(1);
    const int y = number(2);

    if (arguments.size() <= 3) {
      this->m_text->actions.Navigate(map, 2, 5, false, x, y, 1002);
      break;
    }

    const int type = number(3);
    int policy = number(4);

    if (policy == -222) {
      this->m_text->actions.NavigateNamed((type == 3 ? "$$DB " : "") + map, type, 0);
      break;
    }

    const int option = number(5);
    const int target = number(6);
    unsigned char flag = 0;

    if (policy >= 100) {
      flag = 1;
      policy -= 100;
    }

    if (policy >= 10) {
      flag += 2;
      policy -= 10;
    }

    if (policy >= 1) {
      flag += 4;
    }

    this->m_text->actions.Navigate(map, type, flag, option == 0, x, y, target);
    break;
  }
  case 464: {
    if (arguments.empty()) {
      break;
    }

    const int idValue = !arguments.at(1).empty() ? number(1) : 0;
    ItemInfo item;
    item.identified = 1;
    item.unknown_0000[0] = static_cast<unsigned>(idValue);
    item.unknown_0070 = 0;
    item.itemId = arguments.at(0);

    if (UIWindow* window = this->m_text->list.windows.LookupWindow(12)) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }

    break;
  }
  case 517:
    if (!arguments.empty()) {
      if (UIWindow* window = this->m_text->list.windows.EnsureWindow(315)) {
        this->m_text->actions.OpenTip(*window, number(0));
      }
    }

    break;
  case 539:
    this->m_text->actions.OpenQuest(number(0));
    break;
  default:
    break;
  }
}

// INLINED
void UITextViewer::AppendText(char* text, int width, bool carriageReturns, bool pixels) {
  auto& owner = this->m_text->list;
  std::vector<std::string> wrapped;

  if (width == 138) {
    WrapPlainText(text, wrapped, 38, owner.codePage);

    for (const auto& line : wrapped) {
      this->AddLine(line.c_str());
    }

    this->unknown_00BC = (this->unknown_00BC & ~255) | 1;
    this->Invalidate();
    return;
  }

  const bool linked = HasTextLinks(text);
  std::string display;

  if (linked) {
    const std::string expanded = ExpandTextEscapes(text);
    std::strcpy(text, expanded.c_str());
    display = DisplayTextTag(expanded, "<URL>", "</URL>");
    display = DisplayTextTag(std::move(display), "<NAVI>", "</NAVI>");
    display = DisplayTextTag(std::move(display), "<TIPBOX>", "</TIPBOX>");
    display = DisplayTextTag(std::move(display), "<ITEM>", "</ITEM>");
    display = RemoveTextLinkInfo(std::move(display));

    if (this->m_text->chatConversions) {
      display = this->ConvertChat(std::move(display));
    }

    std::string navigation;
    DisplayNavigationLink(navigation, display);
    display = DisplayTextTag(std::move(navigation), "<QUEST>", "</QUEST>");
  } else {
    display = text;
  }

  if (pixels) {
    const int scrollWidth = carriageReturns ? this->verticalBar->geometry.width + 2 : 0;
    WrapPixelText(*this->geometry.surface, owner.codePage, display.c_str(), wrapped, this->geometry.width - scrollWidth - 4);
  } else {
    WrapRichText(
      display.c_str(), wrapped, width == 0 ? 35 : width, carriageReturns, owner.codePage, this->m_text->items.descriptions
    );
  }

  for (const auto& line : wrapped) {
    this->AddLine(line.c_str());
  }

  if (linked) {
    for (int kind = 0; kind < 5; ++kind) {
      this->AddLinks(text, static_cast<unsigned>(wrapped.size()), kind);
    }
  }

  this->unknown_00BC &= ~255;
  this->Invalidate();
}

// OFFSET: 0x00899E90
// FUNCTION: fn_11562
int UITextViewer::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12: {
    const int previousX = this->horizontalOffset;
    UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);

    for (UITextButton* button : this->links) {
      if (!button) {
        continue;
      }

      const int row = button->m_value;
      const int limit = this->topLine + this->geometry.height / this->lineHeight - this->bottomMargin;

      if (row < this->topLine || row >= limit) {
        button->Enable(0);
      } else {
        button->Enable(1);
        const int x = previousX + button->geometry.x - this->horizontalOffset;
        const int y = this->lineHeight * (row - this->topLine) + 3;
        button->Move(x, y);
      }
    }

    return 0;
  }
  case 18:
  case 19:
    if (this->verticalOverflow != 0) {
      this->SendMsg(this->verticalBar, 7, message == 18 ? -1 : 1, 0, 0, 0);
    }

    return 0;
  case 29:
    this->AppendText(reinterpret_cast<char*>(argument1), static_cast<int>(argument2), argument3 != 0, argument4 != 0);
    return 0;
  case 98:
    this->Activate(static_cast<UITextButton*>(sender), static_cast<int>(argument1));
    return 0;
  default:
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
