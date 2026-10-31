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
#include <string>
#include <vector>

// OFFSET: 0x0091B800
// FUNCTION: fn_12605
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

struct TextLinkHistory {
  /* 0000 */ unsigned char unknown_0000[0xF0];
  /* 00F0 */ unsigned int limit;
  /* 00F4 */ unsigned char unknown_00F4[0xC];
  /* 0100 */ std::vector<std::string> names;
  /* 010C */ std::vector<int> values;
  /* 0118 */ std::vector<std::string> details;

  void sub_876D50(const char* name, int value, const char* detail);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(TextLinkHistory, limit) == 0xF0);
static_assert(offsetof(TextLinkHistory, names) == 0x100);
static_assert(offsetof(TextLinkHistory, values) == 0x10C);
static_assert(offsetof(TextLinkHistory, details) == 0x118);
#endif

// OFFSET: 0x00876D50
// FUNCTION: fn_11207
void TextLinkHistory::sub_876D50(const char* name, int value, const char* detail) {
  if (this->names.size() >= this->limit) {
    reinterpret_cast<void(__thiscall*)(TextLinkHistory*)>(0x0087CC30)(this);
  }

  this->names.push_back(std::string(name));
  this->values.push_back(value);
  this->details.push_back(std::string(detail));
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
// FUNCTION: fn_11211
void UITextViewer::AddTipLinks(std::string text, unsigned addedLines) {
  auto& owner = this->m_text->list;
  std::string parsed = RemoveTextColors(text.c_str(), owner.codePage);
  parsed = DisplayTextTag(std::move(parsed), "<URL>", "</URL>");
  parsed = DisplayTextTag(std::move(parsed), "<NAVI>", "</NAVI>");
  parsed = DisplayTextTag(std::move(parsed), "<ITEM>", "</ITEM>");
  parsed = DisplayTextTag(std::move(parsed), "<QUEST>", "</QUEST>");

  if (this->m_text->chatConversions) {
    parsed = this->ConvertChat(std::move(parsed));
  }

  std::string navigation;
  DisplayNavigationLink(navigation, parsed);
  parsed = std::move(navigation);

  std::vector<TextLink> extracted;
  ExtractTextLinks(parsed, "<TIPBOX>", "</TIPBOX>", extracted);

  if (extracted.empty()) {
    return;
  }

  for (unsigned index = 0; index < extracted.size(); ++index) {
    const TextLink& link = extracted[index];

    if (this->splitTipLinks == 1) {
      std::vector<std::string> labels;
      const char* begin = link.display.c_str();
      const char* const end = begin + std::strlen(begin);
      std::uintptr_t limit = reinterpret_cast<std::uintptr_t>(begin) + 32u - link.position;
      const char* current = begin;
      const char* breakAt = begin;

      while (current < end) {
        current = CharNextExA(owner.codePage, current, 0);

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
        auto* button = new UITextButton(*this->m_context, this->m_text->buttons);
        button->Configure(labels[fragment].c_str(), 0x00F4934A, 0, 12, 0, 1);
        const int height = button->geometry.height;
        const int width = this->TextWidth(owner.fonts, owner.locale, labels[fragment].c_str(), 0, 0, 12, 0, 0);
        button->Resize(width + 1, height);
        button->m_hoverColor = 0x00FF7864;
        button->SetId(517);
        SplitTextLinkArguments(link.metadata, button->m_lines);
        const auto position = this->LinkPosition(fragment == 0 ? link.position : 0, addedLines - fragment);
        button->m_value = position.first;
        button->Move(position.second, -100);
        this->AddChild(button);
        this->links.push_back(button);
      }
    } else {
      auto* button = new UITextButton(*this->m_context, this->m_text->buttons);
      button->Configure(link.display.c_str(), 0x00F4934A, 0, 12, 0, 1);
      const int height = button->geometry.height;
      const int width = this->TextWidth(owner.fonts, owner.locale, link.display.c_str(), 0, 0, 12, 0, 0);
      button->Resize(width + 1, height);
      button->m_hoverColor = 0x00FF7864;
      button->SetId(517);
      SplitTextLinkArguments(link.metadata, button->m_lines);
      const auto position = this->LinkPosition(link.position, addedLines);
      button->m_value = position.first;
      button->Move(position.second, -100);
      this->AddChild(button);
      this->links.push_back(button);
    }
  }

  this->SendMsg(nullptr, 7, 0, 0, 0, 0);
}

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
