#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Utility/Text/FormatString.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"

#include <cstdlib>
#include <cstring>

namespace {
  // OFFSET: 0x006D8810
  // FUNCTION: fn_06597
  bool Equipment(const ItemInfo& item) {
    switch (item.unknown_0000[0]) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 21:
      return true;
    default:
      return false;
    }
  }

  // INLINED
  void CardIds(const ItemInfo& item, int (&ids)[4]) {
    std::memcpy(ids, item.unknown_001C, sizeof(ids));
  }

  // INLINED
  int Copy(char* output, int capacity, const char* text) {
    const int error = strncpy_s(output, capacity, text ? text : " ", _TRUNCATE);
    return error == 0 || error == 80 ? (output ? static_cast<int>(strnlen(output, capacity)) : 0) : 0;
  }
}

// INLINED
const char* ItemNaming::Message(int id) const {
  return LookupMessage(id, this->languagePrefix, this->service, this->language);
}

// OFFSET: 0x006D77E0
// FUNCTION: fn_06595
bool ItemNaming::HasCards(const ItemInfo& item) const {
  int ids[4];
  CardIds(item, ids);
  const bool first = this->cards.contains(ids[0]);
  const unsigned id = static_cast<unsigned>(std::atoi(item.itemId.c_str()));

  if (id - 9001u <= 498 || id - 10000u <= 999 || ids[0] == 254 || ids[0] == 255 || !Equipment(item)) {
    return first;
  }

  return first || this->cards.contains(ids[1]) || this->cards.contains(ids[2]) || this->cards.contains(ids[3]);
}

// OFFSET: 0x006D5780
// FUNCTION: fn_06577
int ItemNaming::Grade(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const {
  const char grade[2] = {item.unknown_0088 >= 1 && item.unknown_0088 <= 4 ? "DCBA"[item.unknown_0088 - 1] : '\0', '\0'};
  const int bytes = _snprintf_s(output, capacity, _TRUNCATE, "[%s] ", grade);
  if (bytes <= 0) {
    return 0;
  }

  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}

// OFFSET: 0x006D6C10
// FUNCTION: fn_06585
int ItemNaming::Creator(const ItemInfo& item, char* output, int& capacity, unsigned char consume) {
  int ids[4];
  CardIds(item, ids);
  std::string name = this->players.Lookup(static_cast<unsigned>(ids[2]) + (static_cast<unsigned>(ids[3]) << 16));

  if (name.empty()) {
    name = this->Message(581);
  }

  const int bytes = Copy(output, capacity, name.c_str());
  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}

// OFFSET: 0x006D6F50
// FUNCTION: fn_06588
int ItemNaming::Prefixes(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const {
  int ids[4];
  CardIds(item, ids);
  int remaining = capacity;
  char* cursor = output;

  for (int index = 0; index < 4; ++index) {
    if (ids[index] <= 0) {
      continue;
    }

    const auto card = this->cards.find(ids[index]);
    if (card == this->cards.end() || !card->second.prefix || !*card->second.prefix) {
      continue;
    }

    int count = 1;

    for (int next = index + 1; next < 4; ++next) {
      if (ids[next] == ids[index]) {
        ids[next] = 0;
        ++count;
      }
    }

    if (count >= 2) {
      const int bytes = _snprintf_s(cursor, remaining, _TRUNCATE, "%s ", this->Message(444 + count));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
    }

    const int bytes = _snprintf_s(cursor, remaining, _TRUNCATE, "%s ", card->second.prefix);
    if (bytes > 0) {
      cursor += bytes;
      remaining -= bytes;
    }
  }

  const int bytes = static_cast<int>(cursor - output);
  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}

// OFFSET: 0x006D6DB0
// FUNCTION: fn_06587
int ItemNaming::Suffixes(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const {
  int ids[4];
  CardIds(item, ids);
  int remaining = capacity;
  char* cursor = output;

  for (int index = 0; index < 4; ++index) {
    if (ids[index] <= 0) {
      continue;
    }

    const auto card = this->cards.find(ids[index]);
    if (card == this->cards.end() || !card->second.suffix || !*card->second.suffix) {
      continue;
    }

    int count = 1;

    for (int next = index + 1; next < 4; ++next) {
      if (ids[next] == ids[index]) {
        ids[next] = 0;
        ++count;
      }
    }

    int bytes = _snprintf_s(cursor, remaining, _TRUNCATE, " %s", card->second.suffix);
    if (bytes > 0) {
      cursor += bytes;
      remaining -= bytes;
    }

    if (count >= 2) {
      bytes = _snprintf_s(cursor, remaining, _TRUNCATE, " %s", this->Message(444 + count));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
    }
  }

  const int bytes = static_cast<int>(cursor - output);
  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}

// OFFSET: 0x006D76E0
// FUNCTION: fn_06593
int ItemNaming::Slots(const ItemInfo& item, char* output, int& capacity, unsigned char consume) {
  const int slots = this->descriptions.Find(std::atoi(item.itemId.c_str())).slots;
  if (slots == 0) {
    return 0;
  }

  const int bytes = _snprintf_s(output, capacity, _TRUNCATE, " [%d]", slots);
  if (bytes <= 0) {
    return 0;
  }

  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}

// OFFSET: 0x006D59A0
// FUNCTION: fn_06581
std::string ItemNaming::DisplayName(const ItemInfo& item, unsigned char detailed) {
  char text[128] = {};
  int capacity = 128;
  std::string result;
  result.reserve(150);
  const auto append = [&](int bytes) {
    if (bytes > 0) {
      result += text;
    }
  };
  const int id = std::atoi(item.itemId.c_str());
  ITEM_DESC& descriptor = this->descriptions.Find(id);

  if (item.identified != 1) {
    append(Copy(text, capacity, descriptor.unidentifiedName));
    return result;
  }

  int cards[4];
  CardIds(item, cards);
  if ((static_cast<unsigned>(id) - 9001u <= 498 || static_cast<unsigned>(id) - 10000u <= 999) && cards[3] == 1) {
    append(Copy(text, capacity, this->Message(756)));
    append(Copy(text, capacity, descriptor.identifiedName));
    return result;
  }

  const auto refine = [&]() {
    const int value = item.unknown_0060;
    if (value > 0) {
      append(_snprintf_s(text, sizeof(text), _TRUNCATE, "+%d ", value));
    }

    if (item.unknown_0088 > 0) {
      append(this->Grade(item, text, capacity, 0));
    }
  };
  const auto name = [&]() { append(Copy(text, capacity, descriptor.identifiedName)); };
  const auto element = [&]() {
    const unsigned value = item.unknown_001C[4];
    int message = 0;

    switch (value) {
    case 0:
      message = id == 2634 || id == 2635 ? 2976 : 450;
      break;
    case 1:
      message = 452;
      break;
    case 2:
      message = 454;
      break;
    case 3:
      message = 451;
      break;
    case 4:
      message = 453;
      break;
    }

    if (message) {
      append(Copy(text, capacity, this->Message(message)));
    }
  };
  const auto star = [&]() {
    int message = 0;

    switch (item.unknown_001C[5]) {
    case 5:
      message = 459;
      break;
    case 10:
      message = 460;
      break;
    case 15:
      message = 461;
      break;
    }

    if (message) {
      append(Copy(text, capacity, this->Message(message)));
    }
  };

  if (cards[0] == 254 && cards[1] == 0) {
    refine();
    if (this->service != 12 && this->service != 14) {
      append(this->Creator(item, text, capacity, 0));
      element();
      name();
    } else {
      name();
      element();
      append(this->Creator(item, text, capacity, 0));
    }

    return result;
  }

  if (!Equipment(item) && detailed == 0) {
    name();
    return result;
  }

  refine();
  if (this->HasCards(item)) {
    append(this->Prefixes(item, text, capacity, 0));
    name();
    append(this->Suffixes(item, text, capacity, 0));
    append(this->Slots(item, text, capacity, 0));

    return result;
  }

  if (cards[0] != 254 && cards[0] != 255) {
    name();
    append(this->Slots(item, text, capacity, 0));
    return result;
  }

  if (this->service == 12) {
    name();
    element();
    star();
    append(this->Creator(item, text, capacity, 0));
  } else if (this->service == 14) {
    star();
    name();
    element();
    append(this->Creator(item, text, capacity, 0));
  } else {
    star();
    append(this->Creator(item, text, capacity, 0));
    element();
    name();
  }

  return result;
}


// OFFSET: 0x005E11E0

// OFFSET: 0x006D69C0

// OFFSET: 0x006D70E0

// OFFSET: 0x006D7660
// FUNCTION: fn_06591
const char* ItemResourcePath(const ItemInfo& item) {
  int id = std::atoi(item.itemId.c_str());
  ITEM_DESC& desc = FindItemDescription(id);
  return item.identified == 1 ? desc.identifiedResource : desc.unidentifiedResource;
}

// OFFSET: 0x006D76B0
// FUNCTION: fn_06592
int ItemSlotsCount(const ItemInfo& item) {
  int id = std::atoi(item.itemId.c_str());
  return FindItemDescription(id).slots;
}

// OFFSET: 0x006D7760
// FUNCTION: fn_06594
int ItemUnidentifiedName(const ItemInfo& item, char* output, int& capacity, unsigned char consume) {
  int id = std::atoi(item.itemId.c_str());
  int written = Copy(output, capacity, FindItemDescription(id).unidentifiedName);
  if (written > 0 && consume) {
    capacity -= written;
  }
  return written;
}

// OFFSET: 0x006D78C0

struct ItemNamingContext : public ItemNaming {
  int Element(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const;
  int Star(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const;
  int Apprentice(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const;
  std::uint32_t CardColor(const ItemInfo& item, int slot) const;
};

// OFFSET: 0x006D5650

// OFFSET: 0x006D6AA0
// FUNCTION: fn_06583
int ItemNamingContext::Element(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const {
  const unsigned value = item.unknown_001C[4];
  int message = 0;
  int id = std::atoi(item.itemId.c_str());

  switch (value) {
  case 0:
    message = id == 2634 || id == 2635 ? 2976 : 450;
    break;
  case 1:
    message = 452;
    break;
  case 2:
    message = 454;
    break;
  case 3:
    message = 451;
    break;
  case 4:
    message = 453;
    break;
  }

  if (message) {
    const char* text = this->Message(message);
    const int error = strncpy_s(output, capacity, text ? text : " ", _TRUNCATE);
    int written = (error == 0 || error == 80) && output ? static_cast<int>(strnlen(output, capacity)) : 0;
    if (written > 0 && consume) {
      capacity -= written;
    }
    return written;
  }
  return 0;
}

// OFFSET: 0x006D6B90
// FUNCTION: fn_06584
int ItemNamingContext::Star(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const {
  int message = 0;
  switch (item.unknown_001C[5]) {
  case 5:
    message = 459;
    break;
  case 10:
    message = 460;
    break;
  case 15:
    message = 461;
    break;
  }

  if (message) {
    const char* text = this->Message(message);
    const int error = strncpy_s(output, capacity, text ? text : " ", _TRUNCATE);
    int written = (error == 0 || error == 80) && output ? static_cast<int>(strnlen(output, capacity)) : 0;
    if (written > 0 && consume) {
      capacity -= written;
    }
    return written;
  }
  return 0;
}

// OFFSET: 0x006D6D50
// FUNCTION: fn_06586
int ItemNamingContext::Apprentice(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const {
  const char* text = this->Message(756);
  const int error = strncpy_s(output, capacity, text ? text : " ", _TRUNCATE);
  int written = (error == 0 || error == 80) && output ? static_cast<int>(strnlen(output, capacity)) : 0;
  if (written > 0 && consume) {
    capacity -= written;
  }
  return written;
}

// OFFSET: 0x006D70A0

// OFFSET: 0x006D70A0
// FUNCTION: fn_06589
int ItemInfo::Refine(char* output, int& capacity, unsigned char consume) const {
  const int bytes = StringFormat(output, capacity, static_cast<std::size_t>(-1), "+%d ", this->unknown_0060);

  if (bytes <= 0) {
    return 0;
  }

  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}
