#include "Contents/Items/ItemNaming.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/ItemLinkDisplay.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Lists/UIListBoxNavi.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"

#include <cstdint>
#include <cstring>

// INLINED
std::string UITextViewer::ConvertChat(std::string text) {
  std::string converted;
  DisplayNavigationLink(converted, text);
  const auto* owners = this->m_text->chatConversions;

  if (owners) {
    for (std::size_t index = 0; index < owners->size(); ++index) {
      converted = (*owners)[0]->Convert(converted);
    }
  }

  return converted;
}

// OFFSET: 0x0089C900
// FUNCTION: fn_11589
std::pair<int, int> UITextViewer::LinkPosition(unsigned position, unsigned addedLines) {
  unsigned accumulated = 0;
  const auto count = static_cast<unsigned>(this->lines.size());
  const unsigned first = count - addedLines;
  auto& owner = this->m_text->list;

  for (unsigned index = first; index < count; ++index) {
    const std::string plain = RemoveTextColors(this->lines[index].c_str(), owner.codePage);
    accumulated += static_cast<unsigned>(plain.size());

    if (accumulated > position) {
      const unsigned bytes = position + static_cast<unsigned>(plain.size()) - accumulated;
      const std::string prefix = plain.substr(0, bytes);
      return {static_cast<int>(index), this->TextWidth(owner.fonts, owner.locale, prefix.c_str(), 0, 0, 12, 0, 0) + 2};
    }
  }

  return {0, 0};
}

// OFFSET: 0x008760D0
// FUNCTION: fn_11205
bool UITextViewer::AddItemLinks(std::string text, unsigned addedLines) {
  auto& owner = this->m_text->list;
  std::string parsed = RemoveTextColors(text.c_str(), owner.codePage);
  parsed = DisplayTextTag(std::move(parsed), "<URL>", "</URL>");
  parsed = DisplayTextTag(std::move(parsed), "<NAVI>", "</NAVI>");

  if (this->m_text->chatConversions) {
    parsed = this->ConvertChat(std::move(parsed));
  }

  std::string navigation;
  DisplayNavigationLink(navigation, parsed);
  parsed = std::move(navigation);
  parsed = DisplayTextTag(std::move(parsed), "<TIPBOX>", "</TIPBOX>");

  std::vector<TextLink> extracted;
  ExtractTextLinks(parsed, "<ITEM>", "</ITEM>", extracted);

  if (extracted.empty()) {
    return false;
  }

  for (const auto& link : extracted) {
    auto* button = new UITextButton(*this->m_context, this->m_text->buttons);
    button->Configure(link.display.c_str(), 0x00F4934A, 0, 12, 0, 1);
    const int height = button->geometry.height;
    const int width = this->TextWidth(owner.fonts, owner.locale, link.display.c_str(), 0, 0, 12, 0, 0);
    button->Resize(width + 1, height);
    button->m_hoverColor = 0x00FF7864;
    button->SetId(464);
    SplitTextLinkArguments(link.metadata, button->m_lines);
    const auto position = this->LinkPosition(link.position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, 7, 0, 0, 0, 0);
  return true;
}

// OFFSET: 0x00876700
// FUNCTION: fn_11206
bool UITextViewer::AddNaviLinks(std::string text, unsigned addedLines) {
  auto& owner = this->m_text->list;
  std::string parsed = RemoveTextColors(text.c_str(), owner.codePage);
  parsed = DisplayTextTag(std::move(parsed), "<URL>", "</URL>");
  parsed = DisplayTextTag(std::move(parsed), "<ITEM>", "</ITEM>");

  if (this->m_text->chatConversions) {
    parsed = this->ConvertChat(std::move(parsed));
  }

  parsed = DisplayTextTag(std::move(parsed), "<TIPBOX>", "</TIPBOX>");
  std::string navigation;
  DisplayNavigationLink(navigation, parsed);
  parsed = std::move(navigation);
  parsed = DisplayTextTag(std::move(parsed), "<QUEST>", "</QUEST>");

  std::vector<TextLink> extracted;
  ExtractTextLinks(parsed, "<NAVI>", "</NAVI>", extracted);

  if (extracted.empty()) {
    return false;
  }

  for (const auto& link : extracted) {
    auto* button = new UITextButton(*this->m_context, this->m_text->buttons);
    button->Configure(link.display.c_str(), 0x00F4934A, 0, 12, 0, 1);
    const int height = button->geometry.height;
    const int width = this->TextWidth(owner.fonts, owner.locale, link.display.c_str(), 0, 0, 12, 0, 0);
    button->Resize(width + 1, height);
    button->m_hoverColor = 0x00FF7864;
    button->SetId(438);
    SplitTextLinkArguments(link.metadata, button->m_lines);
    const auto position = this->LinkPosition(link.position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, 7, 0, 0, 0, 0);
  return true;
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

// OFFSET: 0x00877020
// FUNCTION: fn_11209
bool UITextViewer::AddQuestLinks(std::string text, unsigned addedLines) {
  auto& owner = this->m_text->list;
  std::string parsed = RemoveTextColors(text.c_str(), owner.codePage);
  parsed = DisplayTextTag(std::move(parsed), "<URL>", "</URL>");
  parsed = DisplayTextTag(std::move(parsed), "<NAVI>", "</NAVI>");

  if (this->m_text->chatConversions) {
    parsed = this->ConvertChat(std::move(parsed));
  }

  std::string navigation;
  DisplayNavigationLink(navigation, parsed);
  parsed = std::move(navigation);
  parsed = DisplayTextTag(std::move(parsed), "<TIPBOX>", "</TIPBOX>");

  std::vector<TextLink> extracted;
  ExtractTextLinks(parsed, "<QUEST>", "</QUEST>", extracted);

  if (extracted.empty()) {
    return false;
  }

  for (const auto& link : extracted) {
    auto* button = new UITextButton(*this->m_context, this->m_text->buttons);
    button->Configure(link.display.c_str(), 0x00F4934A, 0, 12, 0, 1);
    const int height = button->geometry.height;
    const int width = this->TextWidth(owner.fonts, owner.locale, link.display.c_str(), 0, 0, 12, 0, 0);
    button->Resize(width + 1, height);
    button->m_hoverColor = 0x00FF7864;
    button->SetId(539);
    SplitTextLinkArguments(link.metadata, button->m_lines);
    const auto position = this->LinkPosition(link.position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, 7, 0, 0, 0, 0);
  return true;
}

// OFFSET: 0x00877B00

// OFFSET: 0x00878750
// OFFSET: 0x00878750
// FUNCTION: fn_11212
bool UITextViewer::AddUrlLinks(std::string text, unsigned addedLines) {
  auto& owner = this->m_text->list;
  std::string parsed = RemoveTextColors(text.c_str(), owner.codePage);
  parsed = DisplayTextTag(std::move(parsed), "<NAVI>", "</NAVI>");
  parsed = DisplayTextTag(std::move(parsed), "<ITEM>", "</ITEM>");

  if (this->m_text->chatConversions) {
    parsed = this->ConvertChat(std::move(parsed));
  }

  parsed = DisplayTextTag(std::move(parsed), "<TIPBOX>", "</TIPBOX>");
  std::string navigation;
  DisplayNavigationLink(navigation, parsed);
  parsed = std::move(navigation);
  parsed = DisplayTextTag(std::move(parsed), "<QUEST>", "</QUEST>");

  std::vector<TextLink> extracted;
  ExtractTextLinks(parsed, "<URL>", "</URL>", extracted);

  if (extracted.empty()) {
    return false;
  }

  for (const auto& link : extracted) {
    auto* button = new UITextButton(*this->m_context, this->m_text->buttons);
    button->Configure(link.display.c_str(), 0x00E6A300, 0, 12, 0, 1);
    button->m_hoverColor = 0x00EFB700;
    button->SetId(437);
    const int height = button->geometry.height;
    const int width = this->TextWidth(owner.fonts, owner.locale, link.display.c_str(), 0, 0, 12, 0, 0);
    button->Resize(width + 1, height);
    parsed = RemoveTextLinkInfo(parsed);
    const auto position = this->LinkPosition(link.position, addedLines);
    button->m_value = position.first;
    button->Move(position.second, -100);
    this->AddChild(button);
    SplitTextLinkArguments(link.metadata, button->m_lines);
    this->links.push_back(button);
  }

  this->SendMsg(nullptr, 7, 0, 0, 0, 0);
  return true;
}

// INLINED
void UITextViewer::AddLinks(const char* text, unsigned addedLines, int kind) {
  switch (kind) {
  case 0:
    this->AddItemLinks(text, addedLines);
    break;
  case 1:
    this->AddUrlLinks(text, addedLines);
    break;
  case 2:
    this->AddNaviLinks(text, addedLines);
    break;
  case 3:
    this->AddTipLinks(text, addedLines);
    break;
  case 4:
    this->AddQuestLinks(text, addedLines);
    break;
  default:
    break;
  }
}
