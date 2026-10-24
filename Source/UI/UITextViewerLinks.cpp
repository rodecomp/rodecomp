#include "Contents/ItemNaming.h"
#include "Render/TextContext.h"
#include "UI/ItemLinkDisplay.h"
#include "UI/NavigationLink.h"
#include "UI/PixelTextWrapping.h"
#include "UI/TextMarkup.h"
#include "UI/TextWrapping.h"
#include "UI/UIScrollBar.h"
#include "UI/UITextButton.h"
#include "UI/UITextViewer.h"

#include <cstring>

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
// OFFSET: 0x00878750
// FUNCTION: fn_11212
// OFFSET: 0x00876700
// FUNCTION: fn_11206
// OFFSET: 0x00877B00
// FUNCTION: fn_11211
// OFFSET: 0x00877020
// FUNCTION: fn_11209
void UITextViewer::AddLinks(const char* text, unsigned addedLines, int kind) {
  auto& owner = this->m_text->list;
  std::string parsed = RemoveTextColors(text, owner.codePage);

  if (kind != 1) {
    parsed = DisplayTextTag(std::move(parsed), "<URL>", "</URL>");
  }

  if (kind != 2) {
    parsed = DisplayTextTag(std::move(parsed), "<NAVI>", "</NAVI>");
  }

  if (kind == 1 || kind == 2 || kind == 3) {
    parsed = DisplayTextTag(std::move(parsed), "<ITEM>", "</ITEM>");
  }

  if (kind == 3) {
    parsed = DisplayTextTag(std::move(parsed), "<QUEST>", "</QUEST>");
  }

  if (this->m_text->chatConversions) {
    parsed = this->ConvertChat(std::move(parsed));
  }

  if (kind == 1 || kind == 2) {
    parsed = DisplayTextTag(std::move(parsed), "<TIPBOX>", "</TIPBOX>");
  }

  std::string navigation;
  DisplayNavigationLink(navigation, parsed);
  parsed = std::move(navigation);

  if (kind == 0 || kind == 4) {
    parsed = DisplayTextTag(std::move(parsed), "<TIPBOX>", "</TIPBOX>");
  }

  if (kind == 1 || kind == 2) {
    parsed = DisplayTextTag(std::move(parsed), "<QUEST>", "</QUEST>");
  }

  const char* begins[] = {"<ITEM>", "<URL>", "<NAVI>", "<TIPBOX>", "<QUEST>"};
  const char* ends[] = {"</ITEM>", "</URL>", "</NAVI>", "</TIPBOX>", "</QUEST>"};
  const int ids[] = {464, 437, 438, 517, 539};
  std::vector<TextLink> extracted;
  ExtractTextLinks(std::move(parsed), begins[kind], ends[kind], extracted);

  for (const auto& link : extracted) {
    std::vector<std::string> labels;

    if (kind == 3 && this->splitTipLinks == 1) {
      // Original first fragment gets the remaining byte budget of a 32-byte
      // row; subsequent fragments use 32, retaining its space/newline rules.
      const char* begin = link.display.c_str();
      const char* current = begin;
      const char* end = begin + link.display.size();
      const char* breakAt = begin;
      auto limit = reinterpret_cast<std::uintptr_t>(begin) + 32u - link.position;

      while (current < end) {
        current = CharNextExA(owner.codePage, current, 0);

        while (*current == ' ') {
          breakAt = ++current;
        }

        if (reinterpret_cast<std::uintptr_t>(current) >= limit || *current == '\n') {
          if (*current == '\n') {
            breakAt = current++;
          }

          const char* split = breakAt <= begin ? current : breakAt;
          labels.emplace_back(begin, split);
          begin = split;
          limit = reinterpret_cast<std::uintptr_t>(begin) + 32u;
        }
      }

      if (begin < current) {
        labels.emplace_back(begin, current);
      }
    } else {
      labels.push_back(link.display);
    }

    for (unsigned fragment = 0; fragment < labels.size(); ++fragment) {
      if (kind == 1) {
        labels[fragment] = RemoveTextLinkInfo(std::move(labels[fragment]));
      }

      auto* button = new UITextButton(*this->m_context, this->m_text->buttons);
      button->Configure(labels[fragment].c_str(), kind == 1 ? 0x00E6A300 : 0x00F4934A, 0, 12, 0, 1);
      const int height = button->geometry.height;
      const int width = this->TextWidth(owner.fonts, owner.locale, labels[fragment].c_str(), 0, 0, 12, 0, 0);
      button->Resize(width + 1, height);
      button->m_hoverColor = kind == 1 ? 0x00E6A300 : 0x00FF7864;
      button->SetId(ids[kind]);
      SplitTextLinkArguments(link.metadata, button->m_lines);
      const auto position = this->LinkPosition(fragment == 0 ? link.position : 0, addedLines - fragment);
      button->m_value = position.first;
      button->Move(position.second, -100);
      this->AddChild(button);
      this->links.push_back(button);
    }
  }

  if (!extracted.empty()) {
    this->SendMsg(nullptr, 7, 0, 0, 0, 0);
  }
}

void UITextViewer::AppendText(char* text, int width, bool carriageReturns, bool pixels) {
  auto& owner = this->m_text->list;
  std::vector<std::string> wrapped;

  if (width == 138) {
    WrapPlainText(text, wrapped, 38, owner.codePage, owner.locale.inlineCharset);

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
    WrapPixelText(
      *this->geometry.surface, owner.fonts, owner.locale, owner.codePage, display.c_str(), wrapped,
      this->geometry.width - scrollWidth - 4
    );
  } else {
    WrapRichText(
      display.c_str(), wrapped, width == 0 ? 35 : width, carriageReturns, owner.codePage, owner.locale.inlineCharset,
      this->m_text->items.descriptions
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
