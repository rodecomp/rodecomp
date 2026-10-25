#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemLink.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/TipBoxTitles.h"
#include "UI/Text/ItemLinkDisplay.h"
#include "UI/Text/TextFitting.h"

#include <mbstring.h>

// OFFSET: 0x008816E0
// FUNCTION: fn_11299
bool ItemLinkDisplay::Name(const std::string& encoded, std::string& output) {
  if (encoded.substr(0, 6) == "00000T") {
    const int id = ItemLinkMessageId(encoded);

    if (this->tips.maximumId >= id) {
      std::string abbreviated;
      output = this->tips.Title(id, &abbreviated, 17);

      if (!abbreviated.empty()) {
        output = abbreviated;
        return true;
      }
    }

    return false;
  }

  std::vector<std::int32_t> fields;
  DecodeItemLinkFields(encoded, fields);
  ItemInfo item;

  if (ItemFromLinkFields(fields, item, this->items.descriptions)) {
    output = this->items.DisplayName(item, 0);
    return true;
  }

  return false;
}

// OFFSET: 0x0087FD70
// FUNCTION: fn_11275
bool ItemLinkDisplay::Next(std::string& remaining, std::string& encoded, unsigned& position, const char* original) {
  constexpr std::size_t opening = 7;
  constexpr std::size_t closing = 8;
  auto close = remaining.find("</ITEML>");

  if (close == std::string::npos) {
    return false;
  }

  do {
    remaining.resize(close);
    auto begin = remaining.find("<ITEML>");

    if (begin == std::string::npos) {
      position += closing;
      remaining = std::string(original).substr(position);
      return true;
    }

    close = remaining.find("</ITEML>");

    do {
      position += static_cast<unsigned>(begin);
      remaining = remaining.substr(begin + opening);
      begin = remaining.find("<ITEML>");
      close = remaining.find("</ITEML>");

      if (begin != std::string::npos) {
        position += opening;
      }
    } while (begin != std::string::npos);

    encoded = remaining;
    remaining = original;
    std::string name;
    const bool valid = this->Name(encoded, name);
    const auto extent = static_cast<unsigned>(closing + opening + encoded.size());
    const auto next = position + extent;
    remaining = remaining.substr(next);

    if (!valid) {
      position = next;
      return true;
    }
  } while (close != std::string::npos);

  return true;
}

// OFFSET: 0x00879390
// FUNCTION: fn_11216
std::string ItemLinkDisplay::Convert(const std::string& text) {
  if (!_mbsstr(reinterpret_cast<const unsigned char*>(text.c_str()), reinterpret_cast<const unsigned char*>("<ITEML>"))) {
    return text;
  }

  std::string remaining = text;
  std::string result;
  unsigned position = 0;
  unsigned previous = 0;

  for (;;) {
    std::string encoded;

    if (!this->Next(remaining, encoded, position, text.c_str())) {
      break;
    }

    result.append(text, previous, position - previous);
    std::string name;

    if (this->Name(encoded, name)) {
      result += '<';
      result += FitText(this->surface, this->fonts, this->locale, name, this->width / 2);
      result += '>';
      position += static_cast<unsigned>(15 + encoded.size());
    }

    previous = position;
  }

  result += remaining;
  return result;
}
