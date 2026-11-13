#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Items/ItemNaming.h"
#include "Render/Text/TextContext.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Navigation/UIListBoxNavi.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

// GLOBAL: gl_24664
static const std::string g_viewerItemTagBegin = "<ITEM>";

// GLOBAL: gl_24665
static const std::string g_viewerItemTagEnd = "</ITEM>";

// GLOBAL: gl_24666
static const std::string g_viewerQuestTagBegin = "<QUEST>";

// GLOBAL: gl_24667
static const std::string g_viewerQuestTagEnd = "</QUEST>";

// OFFSET: 0x0091B800
// FUNCTION: fn_12605
std::string UINewChatWnd::ConvertChat(std::string text) {
  std::string converted;
  DisplayNavigationLink(converted, text);
  for (std::size_t index = 0; index < this->m_tabs.size(); ++index) {
    converted = this->m_tabs[0]->Convert(converted);
  }

  return converted;
}

// OFFSET: 0x0089C900
// FUNCTION: fn_11589
std::pair<int, int> UITextViewer::LinkPosition(unsigned position, unsigned addedLines) {
  std::pair<int, int> result(0, 0);
  int accumulated = 0;

  for (unsigned index = this->lines.size() - addedLines; index < this->lines.size(); ++index) {
    const std::string plain = RemoveTextColors(this->lines[index].c_str());
    accumulated += plain.size();

    if (accumulated > static_cast<int>(position)) {
      result.first = index;
      const std::string prefix = plain.substr(0, plain.size() - accumulated + position);
      result.second = this->TextWidth(prefix.c_str(), 0, 0, 12, 0, 0) + 2;
      break;
    }
  }

  return result;
}

// OFFSET: 0x008760D0
// FUNCTION: fn_11205
bool UITextViewer::AddItemLinks(std::string text, unsigned addedLines) {
  std::string parsed = RemoveTextColors(text.c_str());
  parsed = static_cast<const std::string&>(DisplayUrlTextTag(parsed));
  parsed = static_cast<const std::string&>(DisplayNavigationTextTag(parsed));

  if (g_windowMgr.chatWnd) {
    parsed = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(std::move(parsed));
  }

  {
    std::string navigation;
    DisplayNavigationLink(navigation, parsed);
    parsed = std::move(navigation);
  }
  parsed = static_cast<const std::string&>(DisplayTipBoxTextTag(parsed));

  std::vector<TextLink> extracted;
  ExtractTextLinks(parsed, g_viewerItemTagBegin, g_viewerItemTagEnd, extracted);

  if (extracted.empty()) {
    return false;
  }

  for (unsigned index = 0; index < extracted.size(); ++index) {
    auto* button = new UITextButton();
    button->Configure(extracted[index].display.c_str(), 0x00F4934A, 0, 12, 0, 1);
    const int height = button->geometry.height;
    const int width = this->TextWidth(extracted[index].display.c_str(), 0, 0, 12, 0, 0);
    button->ResizeToTextClipped(width + 1, height);
    button->m_hoverColor = 0x00FF7864;
    button->SetId(LINK_BUTTON_ITEM);
    this->SplitTextLinkArguments(extracted[index].metadata, button);
    const auto position = this->LinkPosition(extracted[index].position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, UM_SCROLLVERT, 0, 0, 0, 0);
  return true;
}

// OFFSET: 0x00876700
// FUNCTION: fn_11206
bool UITextViewer::AddNaviLinks(std::string text, unsigned addedLines) {
  std::string parsed = RemoveTextColors(text.c_str());
  parsed = static_cast<const std::string&>(DisplayUrlTextTag(parsed));
  parsed = static_cast<const std::string&>(DisplayItemTextTag(parsed));

  if (g_windowMgr.chatWnd) {
    parsed = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(std::move(parsed));
  }

  parsed = static_cast<const std::string&>(DisplayTipBoxTextTag(parsed));
  {
    std::string navigation;
    DisplayNavigationLink(navigation, parsed);
    parsed = std::move(navigation);
  }
  parsed = static_cast<const std::string&>(DisplayQuestTextTag(parsed));

  std::vector<TextLink> extracted;
  ExtractNavigationTextLinks(parsed, extracted);

  if (extracted.empty()) {
    return false;
  }

  for (unsigned index = 0; index < extracted.size(); ++index) {
    const TextLink& link = extracted[index];
    auto* button = new UITextButton();
    button->Configure(link.display.c_str(), 0x00F4934A, 0, 12, 0, 1);
    const int height = button->geometry.height;
    const int width = this->TextWidth(link.display.c_str(), 0, 0, 12, 0, 0);
    button->ResizeToTextClipped(width + 1, height);
    button->m_hoverColor = 0x00FF7864;
    button->SetId(LINK_BUTTON_NAVI);
    this->SplitTextLinkArguments(link.metadata, button);
    const auto position = this->LinkPosition(link.position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, UM_SCROLLVERT, 0, 0, 0, 0);
  return true;
}

// OFFSET: 0x00876D50
// FUNCTION: fn_11207
void UIChatHisBox::AddLinkRecord(const char* name, int value, const char* detail) {
  if (this->historyTexts.size() >= static_cast<unsigned int>(this->maxLines)) {
    this->DropOldestRecords();
  }

  this->historyTexts.push_back(std::string(name));
  this->historyColors.push_back(value);
  this->historyItemIds.push_back(std::string(detail));
}

// OFFSET: 0x0089BA70
// FUNCTION: fn_11575
void UIListBoxNavi::SetDefaultLineColor(std::uint32_t color) {
  this->defaultLineColor = color;
  this->Invalidate();
}

// OFFSET: 0x00876FD0
// FUNCTION: fn_11208
void UIListBoxNavi::SetLineColor(int line, std::uint32_t color) {
  const int index = this->lineOffset + line;

  if (index < 0 || this->lines.size() < static_cast<unsigned>(index)) {
    return;
  }

  this->lineColors[index] = color;
  this->Invalidate();
}

// OFFSET: 0x0087A0F0
// FUNCTION: fn_11226
void UIListBoxNavi::Clear() {
  this->selected = -1;
  this->contentWidth = 0;
  this->horizontalOffset = 0;
  this->topLine = 0;
  this->verticalOverflow = 0;
  this->bottomMargin = 0;
  this->lines.clear();
  this->lineColors.clear();
  this->Refresh();
  this->lineOffset = -1;
}

// OFFSET: 0x0087A390
// FUNCTION: fn_11232
void UIListBoxNavi::ResetLineColors() {
  for (std::vector<std::uint32_t>::iterator color = this->lineColors.begin(); color != this->lineColors.end(); ++color) {
    *color = 0;
  }
  this->Invalidate();
}

// OFFSET: 0x00877020
// FUNCTION: fn_11209
bool UITextViewer::AddQuestLinks(std::string text, unsigned addedLines) {
  std::string parsed = RemoveTextColors(text.c_str());
  parsed = static_cast<const std::string&>(DisplayUrlTextTag(parsed));
  parsed = static_cast<const std::string&>(DisplayNavigationTextTag(parsed));

  if (g_windowMgr.chatWnd) {
    parsed = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(std::move(parsed));
  }

  {
    std::string navigation;
    DisplayNavigationLink(navigation, parsed);
    parsed = std::move(navigation);
  }
  parsed = static_cast<const std::string&>(DisplayTipBoxTextTag(parsed));

  std::vector<TextLink> extracted;
  ExtractTextLinks(parsed, g_viewerQuestTagBegin, g_viewerQuestTagEnd, extracted);

  if (extracted.empty()) {
    return false;
  }

  for (unsigned index = 0; index < extracted.size(); ++index) {
    auto* button = new UITextButton();
    button->Configure(extracted[index].display.c_str(), 0x00F4934A, 0, 12, 0, 1);
    const int height = button->geometry.height;
    const int width = this->TextWidth(extracted[index].display.c_str(), 0, 0, 12, 0, 0);
    button->ResizeToTextClipped(width + 1, height);
    button->m_hoverColor = 0x00FF7864;
    button->SetId(LINK_BUTTON_QUEST);
    this->SplitTextLinkArguments(extracted[index].metadata, button);
    const auto position = this->LinkPosition(extracted[index].position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, UM_SCROLLVERT, 0, 0, 0, 0);
  return true;
}

// OFFSET: 0x00877B00
// FUNCTION: fn_11211
void UITextViewer::AddTipLinks(std::string text, unsigned addedLines) {
  std::string parsed = RemoveTextColors(text.c_str());
  parsed = static_cast<const std::string&>(DisplayUrlTextTag(parsed));
  parsed = static_cast<const std::string&>(DisplayNavigationTextTag(parsed));
  parsed = static_cast<const std::string&>(DisplayItemTextTag(parsed));
  parsed = static_cast<const std::string&>(DisplayQuestTextTag(parsed));

  if (g_windowMgr.chatWnd) {
    parsed = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(std::move(parsed));
  }

  {
    std::string navigation;
    DisplayNavigationLink(navigation, parsed);
    parsed = std::move(navigation);
  }

  std::vector<TextLink> extracted;
  ExtractTipBoxTextLinks(parsed, extracted);

  if (!extracted.empty()) {
    for (unsigned index = 0; index < extracted.size(); ++index) {

      if (this->splitTipLinks == 1) {
        std::vector<std::string> labels;
        const char* begin = extracted[index].display.c_str();
        const char* const end = begin + std::strlen(begin);
        std::uintptr_t limit = reinterpret_cast<std::uintptr_t>(begin) + 32u - extracted[index].position;
        const char* current = begin;
        const char* breakAt = begin;

        while (current < end) {
          current = g_serverLanguage->NextChar(current, static_cast<int>(end - current));

          while (*current == ' ') {
            breakAt = ++current;
          }

          if (reinterpret_cast<std::uintptr_t>(current) >= limit || *current == '\n') {
            if (*current == '\n') {
              breakAt = current++;
            }

            if (breakAt > begin) {
              labels.push_back(std::string(begin, breakAt));
              begin = breakAt;
            } else {
              labels.push_back(std::string(begin, current));
              begin = current;
            }

            limit = reinterpret_cast<std::uintptr_t>(begin) + 32u;
          }
        }

        if (begin < current) {
          labels.push_back(std::string(begin, current));
        }

        for (unsigned fragment = 0; fragment < labels.size(); ++fragment) {
          auto* button = new UITextButton();
          button->Configure(labels[fragment].c_str(), 0x00F4934A, 0, 12, 0, 1);
          const int height = button->geometry.height;
          const int width = this->TextWidth(labels[fragment].c_str(), 0, 0, 12, 0, 0);
          button->ResizeToTextClipped(width + 1, height);
          button->m_hoverColor = 0x00FF7864;
          button->SetId(LINK_BUTTON_TIP);
          this->SplitTextLinkArguments(extracted[index].metadata, button);
          const auto position = this->LinkPosition(fragment == 0 ? extracted[index].position : 0, addedLines - fragment);
          button->m_value = position.first;
          button->Move(position.second, -100);
          this->AddChild(button);
          this->links.push_back(button);
        }
      } else {
        auto* button = new UITextButton();
        button->Configure(extracted[index].display.c_str(), 0x00F4934A, 0, 12, 0, 1);
        const int height = button->geometry.height;
        const int width = this->TextWidth(extracted[index].display.c_str(), 0, 0, 12, 0, 0);
        button->ResizeToTextClipped(width + 1, height);
        button->m_hoverColor = 0x00FF7864;
        button->SetId(LINK_BUTTON_TIP);
        this->SplitTextLinkArguments(extracted[index].metadata, button);
        const auto position = this->LinkPosition(extracted[index].position, addedLines);
        button->m_value = position.first;
        button->Move(position.second, -100);
        this->AddChild(button);
        this->links.push_back(button);
      }
    }

    this->SendMsg(nullptr, UM_SCROLLVERT, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00878750
// FUNCTION: fn_11212
bool UITextViewer::AddUrlLinks(std::string text, unsigned addedLines) {
  std::string parsed = RemoveTextColors(text.c_str());
  parsed = static_cast<const std::string&>(DisplayNavigationTextTag(parsed));
  parsed = static_cast<const std::string&>(DisplayItemTextTag(parsed));

  if (g_windowMgr.chatWnd) {
    parsed = static_cast<UINewChatWnd*>(g_windowMgr.chatWnd)->ConvertChat(std::move(parsed));
  }

  parsed = static_cast<const std::string&>(DisplayTipBoxTextTag(parsed));
  std::string navigation;
  DisplayNavigationLink(navigation, parsed);
  parsed = std::move(navigation);
  parsed = static_cast<const std::string&>(DisplayQuestTextTag(parsed));

  std::vector<TextLink> extracted;
  ExtractUrlTextLinks(parsed, extracted);

  if (extracted.empty()) {
    return false;
  }

  for (unsigned index = 0; index < extracted.size(); ++index) {
    auto* button = new UITextButton();
    button->Configure(extracted[index].display.c_str(), 0x00E6A300, 0, 12, 0, 1);
    button->m_hoverColor = 0x00EFB700;
    button->SetId(LINK_BUTTON_URL);
    const int height = button->geometry.height;
    const int width = this->TextWidth(extracted[index].display.c_str(), 0, 0, 12, 0, 0);
    button->ResizeToTextClipped(width + 1, height);
    parsed = RemoveTextLinkInfo(parsed);
    const auto position = this->LinkPosition(extracted[index].position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    this->SplitTextLinkArguments(extracted[index].metadata, button);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, UM_SCROLLVERT, 0, 0, 0, 0);
  return true;
}

// INLINED
void UITextViewer::AddLinks(const char* text, unsigned addedLines, int kind) {
  switch (kind) {
  case TEXT_LINK_ITEM:
    this->AddItemLinks(text, addedLines);
    break;
  case TEXT_LINK_URL:
    this->AddUrlLinks(text, addedLines);
    break;
  case TEXT_LINK_NAVI:
    this->AddNaviLinks(text, addedLines);
    break;
  case TEXT_LINK_TIP:
    this->AddTipLinks(text, addedLines);
    break;
  case TEXT_LINK_QUEST:
    this->AddQuestLinks(text, addedLines);
    break;
  default:
    break;
  }
}
