#include "UI/Lists/UITagListBox.h"

#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/NavigationManager.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UITagButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <windows.h>

namespace {
// INLINED
const char* SegmentEnd(const char* text, const char* end, unsigned short codePage) {
  const char* current = text;

  while (current < end) {
    if (*current == '^' && end - current >= 6) {
      bool color = true;

      for (int index = 1; index <= 6; ++index) {
        const unsigned char digit = static_cast<unsigned char>(current[index]);
        if (!((digit >= '0' && digit <= '9') || (digit >= 'a' && digit <= 'f') || (digit >= 'A' && digit <= 'F'))) {
          color = false;
          break;
        }
      }

      if (color) {
        return current;
      }
    }

    current = CharNextExA(codePage, current, 0);
  }

  return current;
}
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
  for (UITagButton* window : this->m_tagWindows) {
    this->RemoveChild(window, 1);
  }
  this->m_tagWindows.clear();
  this->Invalidate();
}

// OFFSET: 0x009B9E80
// FUNCTION: fn_13879
void UITagListBox::Slot25(int, int) {
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
  auto& owner = *this->m_listContext;

  for (int index = 0; index < first; ++index) {
    const std::string& line = this->lines[index];
    const char* text = line.c_str();
    const char* endText = text + line.size();

    while (text < endText) {
      text = ParseColorCode(SegmentEnd(text, endText, owner.codePage), color);
    }
  }

  if (first < end) {
    if ((this->unknown_00BC & 255) == 0) {
      for (int index = first; index < end; ++index) {
        const std::string& line = this->lines[index];
        const int y = (index - first) * this->lineHeight + 4;
        DrawColoredText(
          *this, owner.codePage, 3, y, line.c_str(), static_cast<unsigned>(line.size()), color, 0, 12, false
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
void UITagListBox::Slot26(int, int) {
}

// OFFSET: 0x009B9E90
// FUNCTION: fn_13880
void UITagListBox::Slot31(int, int) {
}

// OFFSET: 0x009B8210
// FUNCTION: fn_13872
int UITagListBox::DecodeNumber(std::string text) {
  char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const int length = static_cast<int>(text.size());
  int result = 0;
  char* buffer = new char[length + 1];
  strcpy_s(buffer, length + 1, text.c_str());

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
  std::string value;
  std::vector<int> options;
  int optionCount = 0;
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

    for (int type = 0; type < 14; ++type) {
      if (delimiters[type] != buffer[position]) {
        continue;
      }

      if (type == 7) {
        if (!value.empty()) {
          fields[index] = this->DecodeNumber(value);
        }
        int end = static_cast<int>(text.size());
        cardIndex = this->DecodeCards(fields, buffer, position, delimiters[7], end, 6);
      } else if (type == 9) {
        ++optionCount;
        if (!value.empty()) {
          fields[index] = this->DecodeNumber(value);
        }
        int end = static_cast<int>(text.size());
        this->DecodeValues(options, buffer, position, end, delimiters[10], delimiters[11]);
      } else if (!value.empty()) {
        fields[index] = this->DecodeNumber(value);
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

  item.unknown_0000[2] = fields[0];
  item.SetItemId(fields[2]);
  if (fields[1] != 0) {
    item.unknown_0000[0] = 4;
  }
  if (fields.size() > 3) {
    item.unknown_0060 = fields[3];
  }
  item.unknown_0070 = fields[4];
  item.unknown_0088 = static_cast<std::int16_t>(fields[5]);

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
    item.unknown_0098 = optionCount;
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
  std::vector<int> cards;
  std::string value;
  int count = 1;

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
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12: {
    UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    const int offset = this->horizontalOffset;
    for (unsigned int index = 0; index < this->m_tagWindows.size(); ++index) {
      UITagButton* tag = this->m_tagWindows[index];
      if (tag) {
        const int line = tag->m_value;
        if (line < this->topLine || this->geometry.height / this->lineHeight - this->bottomMargin + this->topLine <= line) {
          tag->Enable(0);
        } else {
          tag->Enable(1);
          tag->Move(tag->geometry.x - this->horizontalOffset + offset, (line - this->topLine) * this->lineHeight + 3);
        }
      }
    }
    break;
  }
  case 18:
    if (this->verticalOverflow) {
      this->SendMsg(this->verticalBar, 7, -1, 0, 0, 0);
      return 0;
    }
    break;
  case 19:
    if (this->verticalOverflow) {
      this->SendMsg(this->verticalBar, 7, 1, 0, 0, 0);
      return 0;
    }
    break;
  case 29:
    this->sub_9B9EA0(argument1, argument2, argument3, argument4);
    return 0;
  case 98:
    this->sub_9BC0A0(argument1, sender);
    break;
  default:
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x009B9EA0
// FUNCTION: fn_13881
void UITagListBox::sub_9B9EA0(std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  char* text = reinterpret_cast<char*>(argument1);
  const int width = static_cast<int>(argument2);
  auto& owner = *this->m_listContext;
  std::vector<std::string> lines;

  if (width == 138) {
    WrapPlainText(text, lines, 38, owner.codePage);

    for (const std::string& line : lines) {
      this->AddLine(line.c_str());
    }

    this->unknown_00BC = (this->unknown_00BC & ~255) | 1;
  } else {
    if (argument4 != 0) {
      int extra = 0;
      if (argument3 != 0) {
        extra = this->verticalBar->geometry.width + 2;
      }
      WrapPixelText(*this->geometry.surface, owner.codePage, text, lines, this->geometry.width - extra - 4);

      for (const std::string& line : lines) {
        this->AddLine(line.c_str());
      }
    } else {
      std::vector<STagInfoV2> tags;
      const std::string parsed = this->sub_9B8AA0(std::string(text));
      this->sub_9BB8E0(parsed, tags, width == 0 ? 35 : width);
    }

    this->unknown_00BC &= ~255;
  }

  this->Invalidate();
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
  case 438: {
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
  case 464: {
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
  case 501: {
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
void UITagListBox::sub_9BC0A0(std::intptr_t type, UIWindow* sender) {
  UITagButton* button = static_cast<UITagButton*>(sender);
  if (!button) {
    return;
  }

  switch (type) {
  case 438: {
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
        g_navigationManager.sub_B995D0(tag, mode, false);
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
        g_navigationManager.sub_B99030(name.c_str(), mode, kind, flag == 0, x, y, id);
      }
    } else {
      g_navigationManager.sub_B99030(name.c_str(), 2, 5, false, x, y, 1002);
    }
    break;
  }
  case 464:
  case 501:
    if (UIWindow* window = g_windowMgr.EnsureWindow(12)) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&button->item), 0, 0, 0);
    }
    break;
  }
}
