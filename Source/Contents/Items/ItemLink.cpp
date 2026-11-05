#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemLink.h"
#include "Network/TextEncoding.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/UITextViewer.h"

#include <cstring>

namespace {
  // OFFSET: 0x0087A700
  // FUNCTION: fn_11236
  int Number(std::string text) {
    char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int result = 0;
    const int size = static_cast<int>(text.size()) + 1;
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

  // OFFSET: 0x00893AE0
  // FUNCTION: fn_11507
  int DecodeCards(std::vector<std::int32_t>& fields, const char* text, int& position, const char& delimiter, const int& length, int field) {
    std::string value;
    std::vector<std::int32_t> cards;
    int count = 1;

    while (++position < length + 1) {
      const char byte = text[position];

      if (byte >= '0') {
        value += byte;
      } else if (byte == delimiter) {
        ++count;
        cards.push_back(Number(value));
        value.clear();
      } else {
        --position;
        cards.push_back(Number(value));
        fields[field] = count;

        for (std::size_t index = 0; index < cards.size(); ++index) {
          fields[field + 1 + index] = cards[index];
        }

        break;
      }
    }

    return count;
  }

  // OFFSET: 0x00893720
  // FUNCTION: fn_11506
  void DecodeOptions(std::vector<std::int32_t>& options, const char* text, int& position, const int& length, const char& separator,
                     const char& recordSeparator) {
    std::string value;

    while (++position < length + 1) {
      const char byte = text[position];

      if (byte >= '0') {
        value += byte;
      } else if (byte == separator || byte == recordSeparator) {
        options.push_back(Number(value));
        value.clear();
      } else {
        --position;
        options.push_back(Number(value));
        break;
      }
    }
  }
}

// OFFSET: 0x0087A810
// FUNCTION: fn_11237
std::string UITextViewer::ItemLinkNumberText(int value) {
  char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int base = 62;
  std::vector<char> reversed;

  do {
    reversed.push_back(digits[value % base]);
    value /= base;
  } while (value >= base);

  reversed.push_back(digits[value]);
  std::string text = "";
  const std::size_t count = reversed.size();

  for (std::size_t index = 0; index < count; ++index) {
    text += reversed[count - 1 - index];
  }

  return text;
}

// OFFSET: 0x0087F7D0
// FUNCTION: fn_11274
void DecodeItemLinkFields(const std::string& text, std::vector<std::int32_t>& fields) {
  char delimiters[] = "!#$%&'()*+,-/";
  const int size = static_cast<int>(text.size()) + 1;

  if (size < 6) {
    return;
  }

  char* buffer = new char[size];
  strcpy_s(buffer, size, text.c_str());
  std::string value;
  std::vector<std::int32_t> options;
  int optionCount = 0;
  int cardCount = 0;
  fields.resize(260);
  fields[6] = -1;

  value += buffer[0];
  value += buffer[1];
  value += buffer[2];
  value += buffer[3];
  value += buffer[4];

  fields[0] = Number(value);
  value.clear();
  value += buffer[5];
  fields[1] = Number(value);
  value.clear();
  int field = 2;

  for (int position = 6; position < text.size() + 1; ++position) {
    if (buffer[position] >= '0') {
      value += buffer[position];
      continue;
    }

    for (unsigned int delimiter = 0; delimiter < sizeof(delimiters); ++delimiter) {
      if (delimiters[delimiter] != buffer[position]) {
        continue;
      }

      switch (delimiter) {
      case 7:
        if (!value.empty()) {
          fields[field] = Number(value);
        }

        cardCount = DecodeCards(fields, buffer, position, delimiters[7], static_cast<int>(text.size()), 6);
        break;
      case 9:
        ++optionCount;

        if (!value.empty()) {
          fields[field] = Number(value);
        }

        DecodeOptions(options, buffer, position, static_cast<int>(text.size()), delimiters[10], delimiters[11]);
        break;
      default:
        if (!value.empty()) {
          fields[field] = Number(value);
        }

        break;
      }

      value.clear();
      field = delimiter;
      break;
    }
  }

  if (optionCount != 0) {
    fields[cardCount + 8] = optionCount;

    for (std::size_t index = 0; index < options.size(); ++index) {
      fields[cardCount + 9 + index] = options[index];
    }
  }

  delete[] buffer;
}

// OFFSET: 0x008933E0
// FUNCTION: fn_11505
bool ItemFromLinkFields(const std::vector<std::int32_t>& fields, ITEM_INFO& item) {
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

  item.unknown_0060 = fields[3];
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

// OFFSET: 0x008345F0
// FUNCTION: fn_10416
int ItemLinkMessageId(const std::string& text) {
  char delimiters[] = "!#$%&'()*+,-/";
  char* buffer = new char[text.size() + 1];
  strcpy_s(buffer, text.size() + 1, text.c_str());
  std::string value;

  for (std::size_t position = 6; position < text.size() + 1; ++position) {
    if (buffer[position] >= '0') {
      value += buffer[position];
      continue;
    }

    for (unsigned int delimiter = 0; delimiter < sizeof(delimiters); ++delimiter) {
      if (delimiters[delimiter] == buffer[position] && !value.empty()) {
        return DecodeNavigationNumber(value);
      }
    }
  }

  if (buffer) {
    delete[] buffer;
  }

  return -1;
}
