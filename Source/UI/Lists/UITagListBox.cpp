#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Lists/UITagListBox.h"

#include "Client/Input/InputCharset.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UITagButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/CreateLineInfo.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/TextTokens.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <windows.h>

// INLINED
UITagListBox::STagInfoV2::~STagInfoV2() {
}

// OFFSET: 0x009B7A10
// FUNCTION: fn_13869
void UITagListBox::AddLine(const char* text) {
  this->UIListBox::AddLine(text);
}

// OFFSET: 0x009B81C0
// FUNCTION: fn_13871
void UITagListBox::Clear() {
  this->UIListBox::Clear();
  for (auto it = this->m_tagWindows.begin(); it != this->m_tagWindows.end(); ++it) {
    this->RemoveChild(*it, 1);
  }
  this->m_tagWindows.clear();
  this->Invalidate();
}

// OFFSET: 0x009B9E80
// FUNCTION: fn_13879
void UITagListBox::OnLBtnDown(int, int) {
}

// OFFSET: 0x009B9CB0
// FUNCTION: fn_13877
void UITagListBox::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FF);

    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(
      (this->backgroundRed & 255) | ((this->backgroundGreen & 255) << 8) | ((this->backgroundBlue & 255) << 16)
    );
  }

  const int first = this->topLine;
  int divisor = this->lineHeight - this->bottomMargin;
  if (divisor == 0) {
    divisor = 1;
  }
  const int limit = (this->geometry.height / divisor + first);
  const int end =
    static_cast<int>(std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(limit)));
  std::uint32_t color = 0;

  for (int index = 0; index < first; ++index) {
    const std::string& line = this->lines[index];
    const char* text = line.c_str();
    const char* endText = text + line.size();

    while (text < endText) {
      text = ParseColorCode(SegmentEnd(text, endText), color);
    }
  }

  if (first < end) {
    if ((this->plainTextMode & 255) == 0) {
      for (int index = first; index < end; ++index) {
        const std::string& line = this->lines[index];
        const int y = (index - first) * this->lineHeight + 4;
        DrawColoredText(
          *this, 3, y, line.c_str(), static_cast<unsigned>(line.size()), color, 0, 12, false
        );
      }
    } else {
      for (int index = first; index < end; ++index) {
        const std::string& line = this->lines[index];
        const int y = (index - first) * this->lineHeight + 4;
        DrawOutlinedText(
          *this, 3, y, line.c_str(), static_cast<unsigned>(line.size()), 0, 0xFFFFFF, 0, 12, 0, 0
        );
      }
    }
  }
}

// OFFSET: 0x009B9E70
// FUNCTION: fn_13878
void UITagListBox::OnLBtnDblClk(int, int) {
}

// OFFSET: 0x009B9E90
// FUNCTION: fn_13880
void UITagListBox::OnLBtnUp(int, int) {
}

// OFFSET: 0x009B8210
// FUNCTION: fn_13872
int UITagListBox::DecodeNumber(std::string text) {
  char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const int size = static_cast<int>(text.size()) + 1;
  int result = 0;
  char* buffer = new char[size];
  strcpy_s(buffer, size, text.c_str());
  const int length = size - 1;

  for (int index = 0; index < length; ++index) {
    for (int digit = 0; digit < 62; ++digit) {
      if (digits[digit] == buffer[index]) {
        int place = 1;
        for (int exponent = 1; exponent < length - index; ++exponent) {
          place *= 62;
        }
        result += place * digit;
        break;
      }
    }
  }

  if (buffer) {
    delete[] buffer;
  }
  return result;
}

// OFFSET: 0x009B8510
// FUNCTION: fn_13873
void UITagListBox::DecodeItemFields(const std::string& text, std::vector<int>& fields) {
  char delimiters[] = "!#$%&'()*+,-/";
  const int size = static_cast<int>(text.size()) + 1;
  if (size < 6) {
    return;
  }

  char* buffer = new char[size];
  strcpy_s(buffer, size, text.c_str());
  std::string value = "";
  int optionCount = 0;
  std::vector<int> options;
  int cardIndex = 0;

  fields.resize(260);
  fields[6] = -1;

  value += buffer[0];
  value += buffer[1];
  value += buffer[2];
  value += buffer[3];
  value += buffer[4];
  fields[0] = this->DecodeNumber(value);
  value.clear();

  value += buffer[5];
  fields[1] = this->DecodeNumber(value);
  value.clear();

  int index = 2;
  for (int position = 6; static_cast<unsigned int>(position) < text.size() + 1; ++position) {
    if (buffer[position] >= '0') {
      value += buffer[position];
      continue;
    }

    for (int type = 0; type < sizeof(delimiters); ++type) {
      if (delimiters[type] != buffer[position]) {
        continue;
      }

      switch (type) {
      case 7: {
        if (!value.empty()) {
          fields[index] = this->DecodeNumber(value);
        }
        int end = static_cast<int>(text.size());
        cardIndex = this->DecodeCards(fields, buffer, position, delimiters[7], end, 6);
        break;
      }
      case 9: {
        ++optionCount;
        if (!value.empty()) {
          fields[index] = this->DecodeNumber(value);
        }
        int end = static_cast<int>(text.size());
        this->DecodeValues(options, buffer, position, end, delimiters[10], delimiters[11]);
        break;
      }
      default:
        if (!value.empty()) {
          fields[index] = this->DecodeNumber(value);
        }
        break;
      }

      value.clear();
      index = type;
      break;
    }
  }

  if (optionCount != 0) {
    fields[cardIndex + 8] = optionCount;
    for (unsigned int option = 0; option < options.size(); ++option) {
      fields[cardIndex + 9 + option] = options[option];
    }
  }
}

// OFFSET: 0x009BA9E0
// FUNCTION: fn_13884
bool UITagListBox::ItemFromFields(const std::vector<int>& fields, ITEM_INFO& item) {
  if (fields.empty()) {
    return false;
  }

  ITEM_DESC description = FindItemDescription(fields[2]);
  item.SetName(description.identifiedName);
  std::string name = description.identifiedName;
  LowercaseText(name);
  if (name.find("unknown item") != std::string::npos) {
    return false;
  }

  item.identity[2] = fields[0];
  item.SetItemId(fields[2]);
  if (fields[1] != 0) {
    item.identity[0] = 4;
  }
  if (fields.size() > 3) {
    item.refiningLevel = fields[3];
  }
  item.sprite = fields[4];
  item.grade = static_cast<std::int16_t>(fields[5]);

  int cardCount = 0;
  if (fields.size() > 6) {
    cardCount = fields[6];
    for (int card = 1; card < cardCount + 1; ++card) {
      const int index = card - 1;
      item.SetCard(fields[6 + card], index);
    }
  }

  if (static_cast<unsigned int>(cardCount + 8) < fields.size()) {
    const int optionCount = fields[cardCount + 8];
    item.optionCount = optionCount;
    for (int option = 1; option < optionCount + 1; ++option) {
      const int* record = &fields[cardCount + 9 + (option - 1) * 3];
      ItemOption value;
      value.id = static_cast<std::int16_t>(record[0]);
      value.value = static_cast<std::int16_t>(record[2]);
      value.parameter = static_cast<unsigned char>(record[1]);
      const int index = option - 1;
      item.SetOption(value, index);
    }
  }

  item.identified = 1;
  return true;
}

// OFFSET: 0x009BACD0
// FUNCTION: fn_13885
void UITagListBox::DecodeValues(std::vector<int>& values, const char* text, int& position, const int& end, const char& separator, const char& alternate) {
  std::string value;

  while (++position < end + 1) {
    const char byte = text[position];
    if (byte >= '0') {
      value += byte;
    } else if (byte == separator || byte == alternate) {
      values.push_back(this->DecodeNumber(value));
      value.clear();
    } else {
      --position;
      values.push_back(this->DecodeNumber(value));
      break;
    }
  }
}

// OFFSET: 0x009BB090
// FUNCTION: fn_13886
int UITagListBox::DecodeCards(std::vector<int>& fields, const char* text, int& position, const char& separator, const int& end, int index) {
  std::string value = "";
  int count = 1;
  std::vector<int> cards;

  while (++position < end + 1) {
    const char byte = text[position];
    if (byte >= '0') {
      value += byte;
    } else if (byte == separator) {
      ++count;
      cards.push_back(this->DecodeNumber(value));
      value.clear();
    } else {
      --position;
      cards.push_back(this->DecodeNumber(value));
      fields[index] = count;
      for (unsigned int card = 0; card < cards.size(); ++card) {
        fields[index + 1 + card] = cards[card];
      }
      break;
    }
  }

  return count;
}

// OFFSET: 0x009BB560
// FUNCTION: fn_13887
int UITagListBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_UPPUSH:
    if (this->verticalOverflow) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, -1, 0, 0, 0);
      return 0;
    }
    break;
  case UM_DOWNPUSH:
    if (this->verticalOverflow) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, 1, 0, 0, 0);
      return 0;
    }
    break;
  case UM_SETTEXT:
    this->AppendText(argument1, argument2, argument3, argument4);
    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLHORZ:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN:
  case UM_SCROLLHORZPAGEUP:
  case UM_SCROLLHORZPAGEDOWN: {
    UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    const int offset = this->horizontalOffset;
    for (unsigned int index = 0; index < this->m_tagWindows.size(); ++index) {
      UITagButton* tag = this->m_tagWindows[index];
      if (tag) {
        const int line = tag->m_value;
        if (line < this->topLine || line >= this->geometry.height / this->lineHeight - this->bottomMargin + this->topLine) {
          tag->Enable(0);
        } else {
          tag->Enable(1);
          tag->Move(tag->geometry.x - this->horizontalOffset + offset, (line - this->topLine) * this->lineHeight + 3);
        }
      }
    }
    break;
  }
  case UM_TEXT_LBTN_UP:
    this->OnTagClick(argument1, sender);
    break;
  default:
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x009B9EA0
// FUNCTION: fn_13881
void UITagListBox::AppendText(std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  char* text = reinterpret_cast<char*>(argument1);
  const int width = static_cast<int>(argument2);
  std::vector<std::string> lines;

  if (width == 138) {
    WrapPlainText(text, lines, 38);

    for (std::size_t index = 0; index < lines.size(); ++index) {
      this->AddLine(lines[index].c_str());
    }

    this->plainTextMode = (this->plainTextMode & ~255) | 1;
  } else {
    if (argument4 != 0) {
      int extra = 0;
      if (argument3 != 0) {
        extra = this->verticalBar->geometry.width + 2;
      }
      this->WrapPixelText(text, lines, this->geometry.width - extra - 4);

      for (std::size_t index = 0; index < lines.size(); ++index) {
        this->AddLine(lines[index].c_str());
      }
    } else {
      std::vector<STagInfoV2> tags;
      const std::string parsed = this->ParseTaggedText(std::string(text), tags);
      this->AppendTaggedLines(parsed, tags, width == 0 ? 35 : width);
    }

    this->plainTextMode &= ~255;
  }

  this->Invalidate();
}

// OFFSET: 0x009BA1D0
// FUNCTION: fn_13882
std::vector<STagString> ExtractTextTags(std::string text) {
  std::vector<STagString> tags;
  int index = 0;

  for (auto prototype = g_textTagRegistry.begin(); prototype != g_textTagRegistry.end(); ++prototype, ++index) {
    std::string remaining = text;

    for (std::size_t offset = 0; offset < text.size();) {
      const std::size_t begin = remaining.find(prototype->opening);

      if (begin == std::string::npos) {
        break;
      }

      std::size_t end = remaining.find(prototype->closing);
      std::size_t advance;

      if (end != std::string::npos) {
        end += prototype->closing.size();
        STagString tag;
        tag.tag = index;
        tag.position = offset + begin;
        tag.text = remaining.substr(begin, end - begin);
        tags.push_back(tag);
        remaining.erase(0, end);
        advance = end;
      } else {
        advance = prototype->opening.size();
        remaining.erase(0, advance);
      }

      offset += advance;
    }
  }

  return tags;
}

// GLOBAL: gl_25151
std::string infoTagOpening = "<INFO>";

// GLOBAL: gl_25152
std::string infoTagClosing = "</INFO>";

namespace {
// INLINED
std::string ItemName(int id) {
  ITEM_DESC description = FindItemDescription(id);
  return description.identifiedName;
}
}

// OFFSET: 0x009B94E0
// FUNCTION: fn_13876
std::pair<std::string, std::string> UITagListBox::ParseTag(std::string text, int tagIndex) {
  std::pair<std::string, std::string> result;
  if (text.size() == 0) {
    return result;
  }

  if (text.find(g_textTagRegistry[tagIndex].opening) == std::string::npos) {
    return result;
  }

  std::size_t closing = text.find(g_textTagRegistry[tagIndex].closing);
  if (closing == std::string::npos) {
    return result;
  }

  switch (g_textTagRegistry[tagIndex].type) {
  case LINE_SEGMENT_NAVI: {
    const std::size_t opening = text.find(infoTagOpening, 0);
    if (opening == std::string::npos) {
      return result;
    }
    closing = text.find(infoTagClosing, 0);
    if (closing == std::string::npos) {
      return result;
    }
    std::string marker = infoTagOpening;
    const std::size_t start = g_textTagRegistry[tagIndex].opening.size();
    result.first += text.substr(start, opening - start);
    result.second += text.substr(opening + marker.size(), closing - marker.size() - opening);
    break;
  }
  case LINE_SEGMENT_ITEM: {
    const std::size_t opening = text.find(infoTagOpening, 0);
    if (opening == std::string::npos) {
      return result;
    }
    closing = text.find(infoTagClosing, 0);
    if (closing == std::string::npos) {
      return result;
    }
    std::string marker = infoTagOpening;
    const std::size_t start = g_textTagRegistry[tagIndex].opening.size();
    result.first += text.substr(start, opening - start);
    result.second += text.substr(opening + marker.size(), closing - marker.size() - opening);
    break;
  }
  case LINE_SEGMENT_ITEM_LINK: {
    result.first = text.substr(0, 1);
    const std::size_t start = g_textTagRegistry[tagIndex].opening.size();
    result.second = text.substr(start, closing - start);
    std::vector<int> fields;
    this->DecodeItemFields(result.second, fields);
    result.first += ItemName(fields[2]);
    result.first += text.substr(text.size() - 1, 1);
    break;
  }
  }

  return result;
}

// OFFSET: 0x009BC0A0
// FUNCTION: fn_13889
void UITagListBox::OnTagClick(std::intptr_t type, UIWindow* sender) {
  UITagButton* button = static_cast<UITagButton*>(sender);
  if (!button) {
    return;
  }

  switch (type) {
  case LINE_SEGMENT_NAVI: {
    const int count = button->GetStringCount();
    if (count == 0) {
      return;
    }

    const std::string name = button->GetString(0);
    const int x = std::atoi(button->GetString(1).c_str());
    const int y = std::atoi(button->GetString(2).c_str());
    if (count > 3) {
      const int mode = std::atoi(button->GetString(3).c_str());
      int code = std::atoi(button->GetString(4).c_str());
      if (code == -222) {
        std::string tag = "$$DB ";
        if (mode != 3) {
          tag.clear();
        }

        tag += name;
        g_navigationManager.SearchNavigation(tag, mode, false);
      } else {
        const int flag = std::atoi(button->GetString(5).c_str());
        const int id = std::atoi(button->GetString(6).c_str());
        unsigned char hundreds = 0;
        if (code >= 100) {
          hundreds = 1;
          code -= 100;
        }

        const unsigned char group = code < 10 ? hundreds : static_cast<unsigned char>(hundreds + 2);
        const int remainder = code < 10 ? code : code - 10;
        const unsigned char kind = remainder < 1 ? group : static_cast<unsigned char>(group + 4);
        g_navigationManager.StartNavigation(name.c_str(), mode, kind, flag == 0, x, y, id);
      }
    } else {
      g_navigationManager.StartNavigation(name.c_str(), 2, 5, false, x, y, 1002);
    }
    break;
  }
  case LINE_SEGMENT_ITEM:
  case LINE_SEGMENT_ITEM_LINK:
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&button->item), 0, 0, 0);
    }
    break;
  }
}

namespace {
struct TagPositionLess {
  // INLINED
  bool operator()(const STagString& left, const STagString& right) const {
    return static_cast<int>(left.position) < static_cast<int>(right.position);
  }
};
}

// OFFSET: 0x009B8AA0
// FUNCTION: fn_13874
std::string UITagListBox::ParseTaggedText(std::string text, std::vector<STagInfoV2>& tags) {
  std::vector<STagString> extracted = ExtractTextTags(text);
  if (extracted.empty()) {
    return std::string(text);
  }

  tags.clear();
  std::sort(extracted.begin(), extracted.end(), TagPositionLess{});

  std::string result;
  std::size_t position = 0;
  for (std::size_t index = 0; index < extracted.size(); ++index) {
    STagInfoV2 tag;
    result += text.substr(position, extracted[index].position - position);
    tag.startOffset = static_cast<int>(result.size());
    const std::pair<std::string, std::string> parsed = this->ParseTag(extracted[index].text, extracted[index].tag);
    result += parsed.first;
    tag.endOffset = static_cast<int>(result.size());
    tag.label = parsed.first;
    tag.data = parsed.second;
    tag.type = g_textTagRegistry[extracted[index].tag].type;
    tags.push_back(tag);
    position = extracted[index].position + extracted[index].text.size();
  }

  if (position < text.size() - 1) {
    result += text.substr(position, text.size() - position);
  }
  return result;
}

namespace {
// INLINED
int WrappedLineLength(const char* text, int length, int width) {
  if (text == nullptr) {
    return -1;
  }

  const char* end = text + length;
  const char* current = text;
  const char* limit = text + width;
  while (current < limit && current < end) {
    if (*current == '\n') {
      return static_cast<int>(current - text) + 1;
    }

    if (IsHexColorToken(current, end)) {
      current += 7;
      limit += 7;
    } else if (IsPipeColorToken(current)) {
      current += 3;
      limit += 3;
    } else {
      current = g_serverLanguage->NextChar(current, static_cast<int>(limit - current));
    }
  }

  return static_cast<int>(current - text);
}
}

// OFFSET: 0x009BB8E0
// FUNCTION: fn_13888
void UITagListBox::AppendTaggedLines(std::string text, std::vector<STagInfoV2>& tags, int width) {
  const int firstLine = static_cast<int>(this->lines.size());
  const char* buffer = text.c_str();
  std::size_t position = 0;
  while (position < text.size()) {
    const char* start = buffer + position;
    int next = WrappedLineLength(start, static_cast<int>(text.size() - position), width) + static_cast<int>(position);
    for (std::size_t index = 0; index < tags.size(); ++index) {
      if (next > tags[index].startOffset && next < tags[index].endOffset) {
        next = tags[index].startOffset;
        break;
      }
    }

    const std::string line(start, buffer + next);
    this->AddLine(line.c_str());
    position = next;
  }

  for (std::vector<STagInfoV2>::iterator tag = tags.begin(); tag != tags.end(); ++tag) {
    this->AddTagButton(*tag, static_cast<int>(this->lines.size()) - firstLine);
  }
  this->SendMsg(nullptr, UM_SCROLLVERT, 0, 0, 0, 0);
}

// OFFSET: 0x009B7A20
// FUNCTION: fn_13870
void UITagListBox::AddTagButton(STagInfoV2 tag, int lineCount) {
  if (tag.label.empty()) {
    return;
  }

  UITagButton* button = new UITagButton();
  button->Configure(tag.label.c_str(), 0x00F4934A, 0, 12, 0, 1);
  const int height = button->geometry.height;
  button->ResizeToTextClipped(
    this->TextWidth(tag.label.c_str(), 0, 0, 12, 0, 0) + 1, height
  );
  button->m_hoverColor = 0x00FF7864;
  button->SetId(tag.type);

  std::string data = tag.data;
  if (!data.empty()) {
    switch (tag.type) {
    case LINE_SEGMENT_NAVI: {
      std::string remaining = data;
      std::size_t separator = remaining.find(",");
      while (separator != std::string::npos) {
        const std::string field = remaining.substr(0, separator);
        button->AddString(field);
        remaining = remaining.substr(separator + 1);
        separator = remaining.find(",");
      }
      button->AddString(remaining);
      break;
    }
    case LINE_SEGMENT_ITEM: {
      std::string remaining = data;
      std::size_t separator = remaining.find(",");
      while (separator != std::string::npos) {
        const std::string field = remaining.substr(0, separator);
        button->AddString(field);
        remaining = remaining.substr(separator + 1);
        separator = remaining.find(",");
      }
      button->AddString(remaining);
      int id = 0;
      if (!button->GetString(1).empty()) {
        id = std::atoi(button->GetString(1).c_str());
      }
      button->item.identity[0] = id;
      button->item.identified = 1;
      button->item.refiningLevel = 0;
      button->item.itemId = button->GetString(0);
      break;
    }
    case LINE_SEGMENT_ITEM_LINK: {
      std::vector<int> fields;
      this->DecodeItemFields(data, fields);
      this->ItemFromFields(fields, button->item);
      break;
    }
    }
  }

  int x = 0;
  unsigned int line = 0;
  int bytes = 0;
  const unsigned int count = static_cast<unsigned int>(this->lines.size());
  for (unsigned int index = count - lineCount; index < count; ++index) {
    bytes += static_cast<int>(this->lines[index].size());
    if (tag.startOffset < bytes) {
      const int prefixLength = static_cast<int>(this->lines[index].size()) - bytes + tag.startOffset;
      const std::string stripped = StripColorTokens(this->lines[index].c_str(), prefixLength);
      const std::string prefix = stripped.substr(0, prefixLength);
      x = this->TextWidth(prefix.c_str(), 0, 0, 12, 0, 0) + 2;
      line = index;
      break;
    }
  }

  button->m_value = line;
  button->Move(x, -100);
  this->AddChild(button);
  this->m_tagWindows.push_back(button);
}
