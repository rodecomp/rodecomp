#include "Client/Replay.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Utility/Text/FormatString.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"

#include <cstdlib>
#include <cstring>
#include <vector>

// GLOBAL: gl_24725
std::map<int, ItemMoveInfo> g_itemMoveInfo;

// GLOBAL: gl_24726
std::map<int, std::vector<const char*>> g_metalProcessItems;

// GLOBAL: gl_24675
std::map<int, ItemCardName> g_itemCardNames;

// GLOBAL: gl_24727
std::map<int, std::vector<const char*>> g_bookItemNames;

namespace {
  // OFFSET: 0x006D8810
  // FUNCTION: fn_06597
  bool Equipment(const ITEM_INFO& item) {
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
  void CardIds(const ITEM_INFO& item, int (&ids)[4]) {
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
  return LookupMessage(id);
}

// OFFSET: 0x006D77E0
// FUNCTION: fn_06595
bool ItemNaming::HasCards(const ITEM_INFO& item) const {
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
int ItemNaming::Grade(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const {
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
int ItemNaming::Creator(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) {
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
int ItemNaming::Prefixes(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const {
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
int ItemNaming::Suffixes(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const {
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
int ItemNaming::Slots(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) {
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
std::string ItemNaming::DisplayName(const ITEM_INFO& item, unsigned char detailed) {
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

#include "Client/Input/InputCharset.h"
#include "Client/Session.h"
#include "Client/Application/ErrorDialog.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Resource/Bitmap/BitmapResource.h"

class CItemInfoMgr {
public:
  void Apply(std::map<int, ITEM_DESC>& values);
};

// GLOBAL: gl_24730
CCommonObject g_tableStorage;

// GLOBAL: gl_24729
CItemInfoMgr* g_itemInfoMgr;

// OFFSET: 0x005E11E0
// FUNCTION: fn_03873
std::string ItemDisplayName(int id) {
  ITEM_INFO item;
  item.SetItemId(id);
  item.identified = 1;
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  return naming.DisplayName(item, 0);
}

// OFFSET: 0x006D69C0
// FUNCTION: fn_06582
const std::vector<const char*>& FindMetalProcessItems(int id) {
  // GLOBAL: gl_24728
  static std::vector<const char*> missing;
  const auto entry = g_metalProcessItems.find(id);
  return entry == g_metalProcessItems.end() ? missing : entry->second;
}

// OFFSET: 0x006D70E0
// FUNCTION: fn_06590
std::vector<unsigned char> ITEM_INFO::Serialize() const {
  std::vector<unsigned char> result;
  const auto append = [&result](std::uint16_t id, const void* data, std::uint32_t bytes) {
    const auto record = BuildReplayTaggedRecord(id, data, bytes);
    result.insert(result.end(), record.begin(), record.end());
  };

  append(281, nullptr, 0);
  append(283, &this->unknown_0000[0], 4);
  append(285, &this->unknown_0000[1], 4);
  append(284, &this->unknown_0000[2], 4);
  append(286, &this->unknown_0000[3], 4);
  append(287, &this->quantity, 4);
  append(288, &this->unknown_0014[0], 4);
  append(289, &this->unknown_0014[1], 4);
  append(290, this->unknown_001C, 16);
  const int id = std::atoi(this->itemId.c_str());
  append(291, &id, 4);
  append(292, this->unknown_0044.c_str(), static_cast<std::uint32_t>(this->unknown_0044.size()));
  append(293, &this->identified, 1);
  append(294, &this->unknown_005D, 1);
  append(295, &this->unknown_0060, 4);
  append(296, &this->unknown_0064, 2);
  append(297, &this->unknown_0068, 4);
  append(298, &this->unknown_0070, 4);
  append(299, &this->unknown_0088, 2);
  append(300, &this->unknown_0098, 4);
  append(301, this->unknown_009C, 25);
  append(282, nullptr, 0);
  return result;
}

// OFFSET: 0x006D8850
// FUNCTION: fn_06598
bool IsBookItem(int id) {
  return g_bookItemNames.find(id) != g_bookItemNames.end();
}

// OFFSET: 0x006D8890
// FUNCTION: fn_06599
bool IsItemCard(int id) {
  return g_itemCardNames.find(id) != g_itemCardNames.end();
}

// OFFSET: 0x006D7660
// FUNCTION: fn_06591
const char* ItemResourcePath(const ITEM_INFO& item) {
  int id = std::atoi(item.itemId.c_str());
  ITEM_DESC& desc = FindItemDescription(id);
  return item.identified == 1 ? desc.identifiedResource : desc.unidentifiedResource;
}

// OFFSET: 0x006D76B0
// FUNCTION: fn_06592
int ItemSlotsCount(const ITEM_INFO& item) {
  int id = std::atoi(item.itemId.c_str());
  return FindItemDescription(id).slots;
}

// OFFSET: 0x006D7760
// FUNCTION: fn_06594
int ItemUnidentifiedName(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) {
  int id = std::atoi(item.itemId.c_str());
  int written = Copy(output, capacity, FindItemDescription(id).unidentifiedName);
  if (written > 0 && consume) {
    capacity -= written;
  }
  return written;
}

// OFFSET: 0x006D3770
// FUNCTION: fn_06544
ItemCardName& FindItemCardName(int id, std::map<int, ItemCardName>& values) {
  // GLOBAL: gl_24676
  static ItemCardName missing{};
  const auto entry = values.find(id);
  return entry == values.end() ? missing : entry->second;
}

// OFFSET: 0x006D3850
// FUNCTION: fn_45456
ItemMoveInfo& FindItemMoveInfo(int id, std::map<int, ItemMoveInfo>& values) {
  // GLOBAL: gl_24789
  static ItemMoveInfo missingMoveInfo{};
  const auto entry = values.find(id);
  return entry == values.end() ? missingMoveInfo : entry->second;
}

// OFFSET: 0x006D5630
// FUNCTION: fn_06574
ItemCardName& FindItemCardName(int id) {
  return FindItemCardName(id, g_itemCardNames);
}

// OFFSET: 0x006D58B0
// FUNCTION: fn_06580
ItemMoveInfo& FindItemMoveInfo(int id) {
  return FindItemMoveInfo(id, g_itemMoveInfo);
}

// OFFSET: 0x006D5150
// FUNCTION: fn_06567
void __fastcall sub_6D5150(std::uint16_t* value) {
  *value = 0;
  *reinterpret_cast<std::uint32_t*>(value + 1) = 0;
}

__declspec(noinline) bool LoadItemMoveInfoTable(std::vector<int>& values, const char* name) {
  values.clear();
  CFile file;
  if (g_serverLanguage != nullptr) {
    name = g_serverLanguage->ServiceName(name);
  }

  if (!file.Open(name, 0, true, false)) {
    MessageBoxA(nullptr, "Can't open file", "ErrorMessage", 0);
    return false;
  }

  const int size = static_cast<int>(file.GetSize());
  if (values.capacity() < static_cast<size_t>(size)) {
    values.reserve(static_cast<size_t>(size));
  }

  char* buffer = new char[static_cast<size_t>(size) + 1];
  file.Read(buffer, static_cast<std::uint32_t>(size));
  buffer[size] = '\0';
  file.Close();

  char* token = buffer;
  for (int index = 0; index < size; ++index) {
    char value = buffer[index];
    if (value == '/' && buffer[index + 1] == '/') {
      do {
        if (value == '\r' || index >= size) {
          break;
        }
        value = buffer[index + 1];
        ++index;
      } while (value != '\n');
      token = buffer + index + 1;
    }

    switch (value) {
    case '\t':
    case ' ':
      buffer[index] = '\0';
      values.push_back(std::atoi(token));
      token = buffer + index + 1;
      break;
    case '\n':
    case '\r':
      token = buffer + index + 1;
      break;
    }
  }

  delete[] buffer;
  return true;
}

namespace {
  class TableBytes : public ResourceBytes {
  public:
    CFile file;

    // INLINED
    bool Open(const char* name) override {
      return this->file.Open(name, 0, true, false);
    }

    // INLINED
    const void* Data() const override {
      return this->file.GetData();
    }

    // INLINED
    int Size() const override {
      return static_cast<int>(this->file.GetSize());
    }

    // INLINED
    void Close() override {
      this->file.Close();
    }
  };
}

// OFFSET: 0x006D78C0
// FUNCTION: fn_06596
__declspec(noinline) void LoadItemNameTables() {
  g_itemDescriptions.clear();
  g_itemCardNames.clear();
  g_metalProcessItems.clear();
  g_tableStorage.Clear();
  g_bookItemNames.clear();

  std::vector<char*> tokens;
  if (tokens.capacity() < 20000) {
    tokens.reserve(20000);
  }

  TableBytes file;
  unsigned char loaded = 0;
  const char* itemFiles[2] = {"item.txt", "eventItem.txt"};

  for (int index = 0; index < 2; ++index) {
    if (!ResourceExists(itemFiles[index], 0)) {
      continue;
    }

    g_tableStorage.LoadTokens(tokens, itemFiles[index], 2, file);
    char** cursor = tokens.empty() ? nullptr : &tokens[0];
    char** end = cursor + tokens.size();

    while (cursor < end) {
      ITEM_DESC& desc = g_itemDescriptions[std::atoi(*cursor)];
      cursor += 2;
      if (cursor >= end) {
        ShowErrorDialog("item.txt \xbd\xba\xc5\xa9\xb8\xb3\xc6\xae\xb8\xa6 \xc8\xae\xc0\xce\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", 0);
      }

      char* name = *cursor;
      if (name != nullptr) {
        g_session.ConvertTooltipText(name, name);
        name = *cursor;
      }
      desc.identifiedName = name;
      cursor += 2;
      if (cursor >= end) {
        ShowErrorDialog("item.txt \xbd\xba\xc5\xa9\xb8\xb3\xc6\xae\xb8\xa6 \xc8\xae\xc0\xce\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", 0);
      }

      while (*cursor != nullptr) {
        desc.identifiedDescription.push_back(*cursor);
        ++cursor;
      }

      ++cursor;
      if (cursor >= end) {
        ShowErrorDialog("item.txt \xbd\xba\xc5\xa9\xb8\xb3\xc6\xae\xb8\xa6 \xc8\xae\xc0\xce\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", 0);
      }

      char* unidentified = *cursor;
      if (unidentified == nullptr || std::strstr(unidentified, "idenitfied equal unidentified") != nullptr) {
        desc.unidentifiedName = desc.identifiedName;
        desc.unidentifiedDescription = desc.identifiedDescription;
        cursor += 2;
      } else {
        g_session.ConvertTooltipText(unidentified, unidentified);
        unidentified = *cursor;
        cursor += 2;
        desc.unidentifiedName = unidentified;
        if (cursor >= end) {
          ShowErrorDialog("item.txt \xbd\xba\xc5\xa9\xb8\xb3\xc6\xae\xb8\xa6 \xc8\xae\xc0\xce\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", 0);
        }

        if (*cursor == nullptr) {
          ++cursor;
        } else {
          do {
            desc.unidentifiedDescription.push_back(*cursor);
            ++cursor;
          } while (*cursor != nullptr);
          ++cursor;
        }
      }
    }

    loaded = 1;
  }

  if (loaded == 0) {
    if (ResourceExists("EventNum2ItemDisplayNameTable.txt", 0)) {
      g_tableStorage.LoadTokens(tokens, "EventNum2ItemDisplayNameTable.txt", 0, file);
      for (size_t index = 0; index + 1 < tokens.size(); index += 2) {
        g_session.ConvertTooltipText(tokens[index + 1], tokens[index + 1]);
        g_itemDescriptions[std::atoi(tokens[index])].unidentifiedName = tokens[index + 1];
      }
    }

    if (ResourceExists("EventNum2itemDescTable.txt", 0)) {
      int id = 0;
      g_tableStorage.LoadTokens(tokens, "EventNum2itemDescTable.txt", 1, file);
      for (size_t index = 0; index < tokens.size(); ++index) {
        char* text = tokens[index];
        if (text == nullptr || *text == '\0') {
          continue;
        }

        if (!IsNumericText(text)) {
          DecodeMessageRot(text);
          g_itemDescriptions[id].unidentifiedDescription.push_back(text);
        } else {
          id = std::atoi(text);
        }
      }
    }

    if (ResourceExists("EventIdNum2ItemDisplayNameTable.txt", 0)) {
      g_tableStorage.LoadTokens(tokens, "EventIdNum2ItemDisplayNameTable.txt", 0, file);
      for (size_t index = 0; index + 1 < tokens.size(); index += 2) {
        g_session.ConvertTooltipText(tokens[index + 1], tokens[index + 1]);
        g_itemDescriptions[std::atoi(tokens[index])].identifiedName = tokens[index + 1];
      }
    }

    if (ResourceExists("EventIdNum2itemDescTable.txt", 0)) {
      int id = 0;
      g_tableStorage.LoadTokens(tokens, "EventIdNum2itemDescTable.txt", 1, file);
      for (size_t index = 0; index < tokens.size(); ++index) {
        char* text = tokens[index];
        if (text == nullptr || *text == '\0') {
          continue;
        }

        if (!IsNumericText(text)) {
          DecodeMessageRot(text);
          g_itemDescriptions[id].identifiedDescription.push_back(text);
        } else {
          id = std::atoi(text);
        }
      }
    }

    g_itemInfoMgr->Apply(g_itemDescriptions);
  }

  if (ResourceExists("EventNum2ItemResNameTable.txt", 0)) {
    g_tableStorage.LoadTokens(tokens, "EventNum2ItemResNameTable.txt", 0, file);
    for (size_t index = 0; index + 1 < tokens.size(); index += 2) {
      g_itemDescriptions[std::atoi(tokens[index])].unidentifiedResource = tokens[index + 1];
    }
  }

  if (ResourceExists("EventIdNum2ItemResNameTable.txt", 0)) {
    g_tableStorage.LoadTokens(tokens, "EventIdNum2ItemResNameTable.txt", 0, file);
    for (size_t index = 0; index + 1 < tokens.size(); index += 2) {
      g_itemDescriptions[std::atoi(tokens[index])].identifiedResource = tokens[index + 1];
    }
  }

  g_tableStorage.LoadTokens(tokens, "itemSlotTable.txt", 0, file);
  for (size_t index = 0; index + 1 < tokens.size(); index += 2) {
    g_itemDescriptions[std::atoi(tokens[index])].unknown_002C = std::atoi(tokens[index + 1]);
  }

  g_tableStorage.LoadTokens(tokens, "itemParamTable.txt", 0, file);
  for (size_t index = 0; index + 2 < tokens.size(); index += 3) {
    ITEM_DESC& desc = g_itemDescriptions[std::atoi(tokens[index])];
    desc.unknown_0034.push_back(std::atoi(tokens[index + 1]));
    desc.unknown_0034.push_back(std::atoi(tokens[index + 2]));
  }

  g_tableStorage.LoadTokens(tokens, "CardItemNameTable.txt", 0, file);
  for (size_t index = 0; index < tokens.size(); ++index) {
    EnsureItemCardName(std::atoi(tokens[index]), g_itemCardNames);
  }

  g_tableStorage.LoadTokens(tokens, "Num2CardIllustNameTable.txt", 0, file);
  for (size_t index = 0; index + 1 < tokens.size(); index += 2) {
    EnsureItemCardName(std::atoi(tokens[index]), g_itemCardNames).name = tokens[index + 1];
  }

  g_tableStorage.LoadTokens(tokens, "CardPrefixNameTable.txt", 0, file);
  for (size_t index = 0; index + 1 < tokens.size(); index += 2) {
    EnsureItemCardName(std::atoi(tokens[index]), g_itemCardNames).prefix = tokens[index + 1];
  }

  g_tableStorage.LoadTokens(tokens, "CardPostfixNameTable.txt", 0, file);
  for (size_t index = 0; index < tokens.size(); ++index) {
    ItemCardName& card = EnsureItemCardName(std::atoi(tokens[index]), g_itemCardNames);
    card.suffix = card.prefix;
    card.prefix = "";
  }

  g_tableStorage.LoadTokens(tokens, "BookItemNameTable.txt", 0, file);
  for (size_t index = 0; index < tokens.size(); ++index) {
    EnsureItemDescriptionLines(std::atoi(tokens[index]), g_bookItemNames).push_back(tokens[index]);
  }

  int metalId = 0;
  g_tableStorage.LoadTokens(tokens, "MetalProcessItemList.txt", 1, file);
  for (size_t index = 0; index < tokens.size(); ++index) {
    char* text = tokens[index];
    if (text == nullptr || *text == '\0') {
      continue;
    }

    if (IsNumericText(text)) {
      metalId = std::atoi(text);
    } else if (std::strlen(text) != 0) {
      EnsureItemDescriptionLines(metalId, g_metalProcessItems).push_back(text);
    }
  }

  ITEM_DESC& unknown = g_itemDescriptions[0];
  unknown.identifiedName = "Unknown Item";
  unknown.identifiedResource = "\xbb\xe7\xb0\xfa";
  unknown.identifiedDescription.push_back("Unknown Item");
  unknown.unidentifiedName = "Unknown Item";
  unknown.unidentifiedResource = "\xbb\xe7\xb0\xfa";
  unknown.unidentifiedDescription.push_back("Unknown Item");
  g_itemDescriptions[-1] = unknown;

  ItemCardName& sorry = FindItemCardName(0, g_itemCardNames);
  sorry.name = "sorry";
  sorry.prefix = "";
  sorry.suffix = "";
  FindItemCardName(-1, g_itemCardNames) = sorry;

  std::vector<int> moveInfo;
  if (LoadItemMoveInfoTable(moveInfo, "ItemMoveInfoV5.txt") && !moveInfo.empty()) {
    for (size_t index = 0; index + 8 < moveInfo.size(); index += 9) {
      ItemMoveInfo flags;
      flags.itemId = moveInfo[index];
      flags.drop = moveInfo[index + 1] != 0;
      flags.trade = moveInfo[index + 2] != 0;
      flags.storage = moveInfo[index + 3] != 0;
      flags.cart = moveInfo[index + 4] != 0;
      flags.sellToNpc = moveInfo[index + 5] != 0;
      flags.mail = moveInfo[index + 6] != 0;
      flags.auction = moveInfo[index + 7] != 0;
      flags.guildStorage = moveInfo[index + 8] != 0;
      g_itemMoveInfo.insert(std::make_pair(flags.itemId, flags));
    }
  }
}

struct ItemNamingContext : public ItemNaming {
  int Element(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const;
  int Star(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const;
  int Apprentice(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const;
  std::uint32_t CardColor(const ITEM_INFO& item, int slot) const;
};

// OFFSET: 0x006D5650
// FUNCTION: fn_06575
std::uint32_t __fastcall ItemCardColor(const ITEM_INFO* item) {
  int ids[4];
  CardIds(*item, ids);
  const bool first = IsItemCard(ids[0]);
  const unsigned id = static_cast<unsigned>(std::atoi(item->itemId.c_str()));
  const bool hasCards = id - 9001u <= 498 || id - 10000u <= 999 || ids[0] == 254 || ids[0] == 255 || !Equipment(*item)
                            ? first
                            : first || IsItemCard(ids[1]) || IsItemCard(ids[2]) || IsItemCard(ids[3]);
  if (!hasCards) {
    return 0;
  }

  const std::uint32_t colors[5] = {0, 0x00ADFF2F, 0x0000BFFF, 0x00FFA500, 0x009400D3};
  const int slots = FindItemDescription(std::atoi(item->itemId.c_str())).slots;
  int count = 0;
  for (int index = 0; index < 4; ++index) {
    if (index >= slots && IsItemCard(ids[index])) {
      ++count;
    }
  }
  return colors[count];
}

// OFFSET: 0x006D6AA0
// FUNCTION: fn_06583
int ItemNamingContext::Element(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const {
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
int ItemNamingContext::Star(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const {
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
int ItemNamingContext::Apprentice(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume) const {
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
int ITEM_INFO::Refine(char* output, int& capacity, unsigned char consume) const {
  const int bytes = StringFormat(output, capacity, static_cast<std::size_t>(-1), "+%d ", this->unknown_0060);

  if (bytes <= 0) {
    return 0;
  }

  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}
