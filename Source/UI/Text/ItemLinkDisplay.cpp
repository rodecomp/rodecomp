#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemLink.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Text/TextFitting.h"

#include <mbstring.h>

extern template std::size_t std::_Traits_find<std::char_traits<char>>(const char* const, const std::size_t, const std::size_t, const char* const, const std::size_t) noexcept;

// OFFSET: 0x008816E0
// FUNCTION: fn_11299
bool UIChatHisBox::Name(const std::string& encoded, std::string& output) {
  if (encoded.substr(0, 6) == "00000T") {
    const int id = ItemLinkMessageId(encoded);

    if (g_itemTagOwner->maxTipId >= id) {
      std::string abbreviated;
      output = g_itemTagOwner->Title(id, &abbreviated, 17);

      if (!abbreviated.empty()) {
        output = abbreviated;
        return true;
      }
    }

    return false;
  }

  std::vector<std::int32_t> fields;
  DecodeItemLinkFields(encoded, fields);
  ITEM_INFO item;

  if (ItemFromLinkFields(fields, item)) {
    output = item.DisplayName(0);
    return true;
  }

  return false;
}

// OFFSET: 0x0087FD70
// FUNCTION: fn_11275
bool UIChatHisBox::Next(std::string& remaining, std::string& encoded, unsigned& position, const char* original) {
  auto close = remaining.find(g_itemLinkTagEnd);

  if (close == std::string::npos) {
    return false;
  }

  do {
    remaining = remaining.substr(0, close);
    auto begin = remaining.find(g_itemLinkTagBegin);

    if (close != std::string::npos && begin == std::string::npos) {
      position += static_cast<unsigned>(g_itemLinkTagEnd.size());
      remaining = original;
      remaining = remaining.substr(position);
      return true;
    }

    close = remaining.find(g_itemLinkTagEnd);

    if (begin != std::string::npos) {
      do {
        position += static_cast<unsigned>(begin);
        remaining = remaining.substr(begin + g_itemLinkTagBegin.size());
        begin = remaining.find(g_itemLinkTagBegin);
        close = remaining.find(g_itemLinkTagEnd);

        if (begin != std::string::npos) {
          position += static_cast<unsigned>(g_itemLinkTagBegin.size());
        }
      } while (begin != std::string::npos);

      encoded = remaining;
      remaining = original;
      std::string name = "";

      if (this->Name(encoded, name)) {
        remaining = remaining.substr(position + encoded.size() + g_itemLinkTagBegin.size() + g_itemLinkTagEnd.size());
      } else {
        position += static_cast<unsigned>(encoded.size() + g_itemLinkTagBegin.size() + g_itemLinkTagEnd.size());
        remaining = remaining.substr(position);
        return true;
      }
    }
  } while (close != std::string::npos);

  return true;
}

// OFFSET: 0x00879390
// FUNCTION: fn_11216
std::string UIChatHisBox::Convert(const std::string& text) {
  if (!_mbsstr(reinterpret_cast<const unsigned char*>(text.c_str()), reinterpret_cast<const unsigned char*>(g_itemLinkTagBegin.c_str()))) {
    return text;
  }

  std::string remaining = text;
  std::string result = "";
  unsigned position = 0;
  unsigned previous = 0;

  for (;;) {
    std::string encoded = "";

    if (!this->Next(remaining, encoded, position, text.c_str())) {
      break;
    }

    result += text.substr(previous, position - previous);
    std::string name = "";

    if (this->Name(encoded, name)) {
      name = this->BracketName(name);
      result += name;
      position += static_cast<unsigned>(encoded.size() + g_itemLinkTagBegin.size() + g_itemLinkTagEnd.size());
    }

    previous = position;
  }

  result += remaining;
  return result;
}

// OFFSET: 0x00884460
// FUNCTION: fn_11307
const std::string UIChatHisBox::BracketName(const std::string& name) {
  std::string result;
  result += '<';
  result += FitText(*this->geometry.surface, name, this->geometry.width / 2);
  result += '>';
  return result;
}
