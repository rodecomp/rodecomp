#include "Contents/ItemDescription.h"
#include "Contents/ItemInfo.h"
#include "Contents/ItemLink.h"

#include <bit>
#include <cctype>
#include <cstdio>
#include <cstring>

namespace {
  // OFFSET: 0x0087A700
  // FUNCTION: fn_11236
  std::int32_t Number(const std::string& text) {
    constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::uint32_t result = 0;

    for (std::size_t index = 0; index < text.size(); ++index) {
      const char* digit = std::strchr(digits, text[index]);

      if (!digit || *digit == '\0') {
        continue;
      }

      std::uint32_t place = 1;

      for (std::size_t exponent = index + 1; exponent < text.size(); ++exponent) {
        place *= 62;
      }

      result += static_cast<std::uint32_t>(digit - digits) * place;
    }

    return std::bit_cast<std::int32_t>(result);
  }

  // OFFSET: 0x00893AE0
  // FUNCTION: fn_11507
  int DecodeCards(const std::string& text, std::size_t& position, std::vector<std::int32_t>& fields) {
    std::vector<std::int32_t> cards;
    std::string value;
    int count = 1;

    while (++position <= text.size()) {
      const char byte = text.c_str()[position];

      if (static_cast<signed char>(byte) >= '0') {
        value += byte;
      } else if (byte == '(') {
        ++count;
        cards.push_back(Number(value));
        value.clear();
      } else {
        --position;
        cards.push_back(Number(value));
        fields[6] = count;

        for (std::size_t index = 0; index < cards.size(); ++index) {
          fields[7 + index] = cards[index];
        }

        break;
      }
    }

    return count;
  }

  // OFFSET: 0x00893720
  // FUNCTION: fn_11506
  void DecodeOptions(const std::string& text, std::size_t& position, std::vector<std::int32_t>& options) {
    std::string value;

    while (++position <= text.size()) {
      const char byte = text.c_str()[position];

      if (static_cast<signed char>(byte) >= '0') {
        value += byte;
      } else if (byte == '-' || byte == '/') {
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

// OFFSET: 0x0087F7D0
// FUNCTION: fn_11274
void DecodeItemLinkFields(const std::string& text, std::vector<std::int32_t>& fields) {
  if (text.size() + 1 < 6) {
    return;
  }

  fields.resize(260);
  fields[6] = -1;
  fields[0] = Number(text.substr(0, 5));
  fields[1] = Number(text.substr(5, 1));
  std::string value;
  int field = 2;
  int cardCount = 0;
  int optionCount = 0;
  std::vector<std::int32_t> options;
  constexpr char delimiters[] = "!#$%&'()*+,-/";

  for (std::size_t position = 6; position <= text.size(); ++position) {
    const char byte = text.c_str()[position];

    if (static_cast<signed char>(byte) >= '0') {
      value += byte;
      continue;
    }

    const char* delimiter = std::strchr(delimiters, byte);

    if (!delimiter) {
      continue;
    }

    if (!value.empty()) {
      fields[field] = Number(value);
    }

    const int nextField = static_cast<int>(delimiter - delimiters);

    if (nextField == 7) {
      cardCount = DecodeCards(text, position, fields);
    } else if (nextField == 9) {
      ++optionCount;
      DecodeOptions(text, position, options);
    }

    value.clear();
    field = nextField;
  }

  if (optionCount != 0) {
    fields[cardCount + 8] = optionCount;

    for (std::size_t index = 0; index < options.size(); ++index) {
      fields[cardCount + 9 + index] = options[index];
    }
  }
}

// OFFSET: 0x008933E0
// FUNCTION: fn_11505
bool ItemFromLinkFields(const std::vector<std::int32_t>& fields, ItemInfo& item, ItemDescriptions& descriptions) {
  if (fields.empty()) {
    return false;
  }

  const char* identified = descriptions.Find(fields[2]).identifiedName;
  item.unknown_0044 = identified;
  std::string name = identified;

  for (std::size_t index = 0; index < name.size(); ++index) {
    if (static_cast<signed char>(name[index]) <= 0) {
      ++index;
    } else if (std::isupper(static_cast<unsigned char>(name[index]))) {
      name[index] += 'a' - 'A';
    }
  }

  if (name.find("unknown item") != std::string::npos) {
    return false;
  }

  item.unknown_0000[2] = std::bit_cast<std::uint32_t>(fields[0]);
  char id[16];
  std::sprintf(id, "%d", fields[2]);
  item.itemId = id;

  if (fields[1] != 0) {
    item.unknown_0000[0] = 4;
  }

  item.unknown_0060 = std::bit_cast<std::uint32_t>(fields[3]);
  item.unknown_0070 = std::bit_cast<std::uint32_t>(fields[4]);
  item.unknown_0088 = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(fields[5]));
  int cardCount = 0;

  if (fields.size() > 6) {
    cardCount = fields[6];

    for (int index = 0; index < cardCount; ++index) {
      if (index < 4) {
        std::memcpy(item.unknown_001C + index * 4, &fields[index + 7], 4);
      }
    }
  }

  const auto optionIndex = static_cast<std::uint32_t>(cardCount) + 8u;

  if (optionIndex < fields.size()) {
    const int count = fields[optionIndex];
    item.unknown_0098 = std::bit_cast<std::uint32_t>(count);

    for (int index = 0; index < count; ++index) {
      if (index < 5) {
        const auto* record = &fields[optionIndex + 1 + 3 * index];
        const std::uint16_t idValue = static_cast<std::uint16_t>(record[0]);
        const std::uint16_t amount = static_cast<std::uint16_t>(record[2]);
        std::memcpy(item.unknown_009C + 5 * index, &idValue, 2);
        std::memcpy(item.unknown_009C + 5 * index + 2, &amount, 2);
        item.unknown_009C[5 * index + 4] = static_cast<unsigned char>(record[1]);
      }
    }
  }

  item.identified = 1;
  return true;
}

// OFFSET: 0x008345F0
// FUNCTION: fn_10416
int ItemLinkMessageId(const std::string& text) {
  constexpr char delimiters[] = "!#$%&'()*+,-/";
  std::string value;

  for (std::size_t position = 6; position <= text.size(); ++position) {
    const char byte = text.c_str()[position];

    if (static_cast<signed char>(byte) >= '0') {
      value += byte;
    } else if (std::strchr(delimiters, byte) && !value.empty()) {
      return Number(value);
    }
  }

  return -1;
}
