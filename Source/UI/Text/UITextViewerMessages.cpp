#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Navigation/NavigationManager.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Items/ItemNaming.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/CreateLineInfo.h"

#include <cstring>
#include <bit>
#include <cstdlib>
#include <shellapi.h>

// INLINED
__forceinline void UITextViewer::Activate(UITextButton* button, int id) {
  switch (id) {
  case LINE_SEGMENT_URL:
    if (button) {
      const std::string& url = button->GetLine(0);

      if (!url.empty()) {
        ShellExecuteA(hWnd, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
      }
    }

    break;
  case LINE_SEGMENT_NAVI: {
    if (!button) {
      break;
    }

    const int count = button->GetLinesCount();

    if (count == 0) {
      break;
    }

    const std::string& map = button->GetLine(0);
    const int x = std::atoi(button->GetLine(1).c_str());
    const int y = std::atoi(button->GetLine(2).c_str());

    if (count > 3) {
      const int type = std::atoi(button->GetLine(3).c_str());
      int policy = std::atoi(button->GetLine(4).c_str());

      if (policy == -222) {
        std::string name = "$$DB ";

        if (type != 3) {
          name.clear();
        }

        name += map;
        g_navigationManager.SearchNavigation(name, type, 0);
        break;
      }

      const int option = std::atoi(button->GetLine(5).c_str());
      const int target = std::atoi(button->GetLine(6).c_str());
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

      g_navigationManager.StartNavigation(map.c_str(), type, flag, option == 0, x, y, target);
    } else {
      g_navigationManager.StartNavigation(map.c_str(), 2, 5, false, x, y, 1002);
    }

    break;
  }
  case LINE_SEGMENT_ITEM: {
    if (!button || button->GetLinesCount() == 0) {
      break;
    }

    int idValue = 0;

    if (!button->GetLine(1).empty()) {
      idValue = std::atoi(button->GetLine(1).c_str());
    }

    ITEM_INFO item;
    item.identified = 1;
    item.identity[0] = static_cast<unsigned>(idValue);
    item.refiningLevel = 0;
    item.itemId = button->GetLine(0);

    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }

    break;
  }
  case LINE_SEGMENT_TIPBOX:
    if (button && button->GetLinesCount() != 0) {
      const std::string& tip = button->GetLine(0);

      if (UIWindow* window = g_windowMgr.EnsureWindow(WID_TIPBOXWND)) {
        static_cast<UITipboxWnd*>(window)->ShowTip(std::atoi(tip.c_str()));
      }
    }

    break;
  case LINE_SEGMENT_QUEST:
    if (button) {
      const int value = std::atoi(button->GetLine(0).c_str());
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ReceiveServiceRequest(6, value);
    }

    break;
  default:
    break;
  }
}

// INLINED
__forceinline void UITextViewer::AppendText(char* text, int width, int carriageReturns, int pixels) {
  const int lineWidth = width == 0 ? 35 : width;
  std::vector<std::string> wrapped;

  if (width == 138) {
    WrapPlainText(text, wrapped, 38);

    for (unsigned index = 0; index < wrapped.size(); ++index) {
      this->AddLine(wrapped[index].c_str());
    }

    this->plainTextMode = (this->plainTextMode & ~255) | 1;
    return;
  }

  if (HasTextLinks(text)) {
    const std::string expanded = ExpandTextEscapes(text);
    strcpy_s(text, std::strlen(text) + 1, expanded.c_str());
    std::string display = DisplayUrlTextTag(text);
    display = DisplayNavigationTextTag(display);
    display = DisplayTipBoxTextTag(display);
    display = DisplayItemTextTag(display);
    display = RemoveTextLinkInfo(display);

    if (g_windowMgr.chatWnd) {
      display = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(display);
    }

    std::string navigation;
    DisplayNavigationLink(navigation, display);
    display = DisplayQuestTextTag(navigation);

    if (pixels) {
      const int scrollWidth = carriageReturns ? this->verticalBar->geometry.width + 2 : 0;
      this->WrapPixelText(display.c_str(), wrapped, this->geometry.width - scrollWidth - 4);
    } else {
      WrapRichText(display.c_str(), wrapped, lineWidth, carriageReturns != 0);
    }

    for (unsigned index = 0; index < wrapped.size(); ++index) {
      this->AddLine(wrapped[index].c_str());
    }

    this->AddItemLinks(text, static_cast<unsigned>(wrapped.size()));
    this->AddUrlLinks(text, static_cast<unsigned>(wrapped.size()));
    this->AddNaviLinks(text, static_cast<unsigned>(wrapped.size()));
    this->AddTipLinks(text, static_cast<unsigned>(wrapped.size()));
    this->AddQuestLinks(text, static_cast<unsigned>(wrapped.size()));
  } else {
    if (pixels) {
      const int scrollWidth = carriageReturns ? this->verticalBar->geometry.width + 2 : 0;
      this->WrapPixelText(text, wrapped, this->geometry.width - scrollWidth - 4);
    } else {
      WrapRichText(text, wrapped, lineWidth, carriageReturns != 0);
    }

    for (unsigned index = 0; index < wrapped.size(); ++index) {
      this->AddLine(wrapped[index].c_str());
    }
  }

  this->plainTextMode &= ~255;
}

// OFFSET: 0x00899E90
// FUNCTION: fn_11562
int UITextViewer::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_UPPUSH:
    if (this->verticalOverflow != 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, -1, 0, 0, 0);
    }

    return 0;

  case UM_DOWNPUSH:
    if (this->verticalOverflow != 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, 1, 0, 0, 0);
    }

    return 0;

  case UM_SETTEXT:
  {
    char* text = reinterpret_cast<char*>(argument1);
    const int width = static_cast<int>(argument2);
    const bool carriageReturns = argument3 != 0;
    const bool pixels = argument4 != 0;
    const int lineWidth = width == 0 ? 35 : width;
    std::vector<std::string> wrapped;

    if (width == 138) {
      WrapPlainText(text, wrapped, 38);
      for (const auto& line : wrapped) {
        this->AddLine(line.c_str());
      }
      this->plainTextMode = (this->plainTextMode & ~255) | 1;
      this->Invalidate();
      return 0;
    }

    if (HasTextLinks(text)) {
      const std::string expanded = ExpandTextEscapes(text);
      strcpy_s(text, std::strlen(text) + 1, expanded.c_str());
      std::string display = DisplayUrlTextTag(text);
      display = DisplayNavigationTextTag(display);
      display = DisplayTipBoxTextTag(display);
      display = DisplayItemTextTag(display);
      display = RemoveTextLinkInfo(display);

      if (g_windowMgr.chatWnd) {
        display = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(display);
      }

      std::string navigation;
      DisplayNavigationLink(navigation, display);
      display = DisplayQuestTextTag(navigation);

      if (pixels) {
        const int scrollWidth = carriageReturns ? this->verticalBar->geometry.width + 2 : 0;
        this->WrapPixelText(display.c_str(), wrapped, this->geometry.width - scrollWidth - 4);
      } else {
        WrapRichText(display.c_str(), wrapped, lineWidth, carriageReturns);
      }

      for (const auto& line : wrapped) {
        this->AddLine(line.c_str());
      }
      this->AddItemLinks(text, static_cast<unsigned>(wrapped.size()));
      this->AddUrlLinks(text, static_cast<unsigned>(wrapped.size()));
      this->AddNaviLinks(text, static_cast<unsigned>(wrapped.size()));
      this->AddTipLinks(text, static_cast<unsigned>(wrapped.size()));
      this->AddQuestLinks(text, static_cast<unsigned>(wrapped.size()));
    } else {
      if (pixels) {
        const int scrollWidth = carriageReturns ? this->verticalBar->geometry.width + 2 : 0;
        this->WrapPixelText(text, wrapped, this->geometry.width - scrollWidth - 4);
      } else {
        WrapRichText(text, wrapped, lineWidth, carriageReturns);
      }

      for (const auto& line : wrapped) {
        this->AddLine(line.c_str());
      }
    }

    this->plainTextMode &= ~255;
    this->Invalidate();
    return 0;
  }

  case UM_SCROLLVERT:
  case UM_SCROLLHORZ:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN:
  case UM_SCROLLHORZPAGEUP:
  case UM_SCROLLHORZPAGEDOWN: {
    const int previousX = this->horizontalOffset;
    UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);

    for (unsigned index = 0; index < this->links.size(); ++index) {
      UITextButton* button = this->links[index];

      if (!button) {
        continue;
      }

      const int row = button->m_value;

      if (row >= this->topLine && row < this->geometry.height / this->lineHeight - this->bottomMargin + this->topLine) {
        button->Enable(1);
        button->Move(button->geometry.x - this->horizontalOffset + previousX, (row - this->topLine) * this->lineHeight + 3);
      } else {
        button->Enable(0);
      }
    }

    return 0;
  }

  case UM_TEXT_LBTN_UP:
  {
    auto* button = static_cast<UITextButton*>(sender);
    const int id = static_cast<int>(argument1);
    switch (id) {
    case LINE_SEGMENT_URL:
      if (button && !button->GetLine(0).empty()) {
        ShellExecuteA(hWnd, "open", button->GetLine(0).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
      }
      break;
    case LINE_SEGMENT_NAVI:
      if (button && button->GetLinesCount() != 0) {
        const int count = button->GetLinesCount();
        const std::string& map = button->GetLine(0);
        const int x = std::atoi(button->GetLine(1).c_str());
        const int y = std::atoi(button->GetLine(2).c_str());
        if (count > 3) {
          const int type = std::atoi(button->GetLine(3).c_str());
          int policy = std::atoi(button->GetLine(4).c_str());
          if (policy == -222) {
            std::string name = "$$DB ";
            if (type != 3) {
              name = "";
            }
            name += map;
            g_navigationManager.SearchNavigation(name, type, 0);
            break;
          }
          const int option = std::atoi(button->GetLine(5).c_str());
          const int target = std::atoi(button->GetLine(6).c_str());
          unsigned char flag = 0;
          if (policy >= 100) { flag = 1; policy -= 100; }
          if (policy >= 10) { flag += 2; policy -= 10; }
          if (policy >= 1) { flag += 4; }
          g_navigationManager.StartNavigation(map.c_str(), type, flag, option == 0, x, y, target);
        } else {
          g_navigationManager.StartNavigation(map.c_str(), 2, 5, false, x, y, 1002);
        }
      }
      break;
    case LINE_SEGMENT_ITEM:
      if (button && button->GetLinesCount() != 0) {
        int idValue = 0;
        if (!button->GetLine(1).empty()) {
          idValue = std::atoi(button->GetLine(1).c_str());
        }
        ITEM_INFO item;
        item.identified = 1;
        item.identity[0] = static_cast<unsigned>(idValue);
        item.refiningLevel = 0;
        item.itemId = button->GetLine(0);
        if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
          window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
        }
      }
      break;
    case LINE_SEGMENT_TIPBOX:
      if (button && button->GetLinesCount() != 0) {
        const std::string& tip = button->GetLine(0);
        if (UIWindow* window = g_windowMgr.EnsureWindow(WID_TIPBOXWND)) {
          static_cast<UITipboxWnd*>(window)->ShowTip(std::atoi(tip.c_str()));
        }
      }
      break;
    case LINE_SEGMENT_QUEST:
      if (button) {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ReceiveServiceRequest(6, std::atoi(button->GetLine(0).c_str()));
      }
      break;
    default:
      break;
    }
    return 0;
  }

  default:
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
