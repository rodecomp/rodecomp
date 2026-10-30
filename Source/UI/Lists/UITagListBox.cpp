#include "UI/Lists/UITagListBox.h"

#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UITagButton.h"
#include "UI/Lists/UIScrollBar.h"

#include <cstdint>
#include <cstring>

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
