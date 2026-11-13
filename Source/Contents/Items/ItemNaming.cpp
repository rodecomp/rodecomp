#include "Client/Replay/Replay.h"
#include "Contents/Items/ItemType.h"
#include "Contents/CombatTypes.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"

#include <cstdlib>
#include <cstring>
#include <vector>
#include "Resource/MSGSTRINGID.h"

// GLOBAL: gl_24725
std::map<int, ItemMoveInfo> g_itemMoveInfo;

// GLOBAL: gl_24726
std::map<int, std::vector<const char*>> g_metalProcessItems;

// GLOBAL: gl_24675
std::map<int, ItemCardName> g_itemCardNames;

// GLOBAL: gl_24727
std::map<int, std::vector<const char*>> g_bookItemNames;

// OFFSET: 0x006D8810
// FUNCTION: fn_06597
bool Equipment(const ITEM_INFO& item) {
  switch (item.identity[0]) {
  case TYPE_ARMOR:
  case TYPE_WEAPON:
  case TYPE_BOW:
  case TYPE_BOTHHAND:
  case TYPE_ARMORTM:
  case TYPE_ARMORTB:
  case TYPE_ARMORMB:
  case TYPE_ARMORTMB:
  case TYPE_GUN:
  case 21:
    return true;
  default:
    return false;
  }
}

namespace {
  // INLINED
  int CardId(const ITEM_INFO& item, int index) {
    int id;
    std::memcpy(&id, &item.cards[index], sizeof(id));
    return id;
  }

  // INLINED
  ItemCardName MakeEmptyItemCardName() {
    return ItemCardName{nullptr, nullptr, nullptr};
  }

  // INLINED
  ItemMoveInfo MakeEmptyItemMoveInfo() {
    return ItemMoveInfo{};
  }

  // INLINED
  __forceinline int CopyText(char* output, int& capacity, unsigned char consume, const char* text) {
    const int error = strncpy_s(output, capacity, text, _TRUNCATE);
    if (error != 0 && error != 80) {
      return 0;
    }

    const int written = !output ? 0 : static_cast<int>(strnlen(output, capacity));
    if (consume) {
      capacity -= written;
    }
    return written;
  }
}

// OFFSET: 0x006D77E0
// FUNCTION: fn_06595
bool ITEM_INFO::HasCards() const {
  const bool first = g_itemCardNames.contains(CardId(*this, 0));
  const unsigned id = static_cast<unsigned>(std::atoi(this->itemId.c_str()));

  if (id - 9001u <= 498 || id - 10000u <= 999 || CardId(*this, 0) == 254 || CardId(*this, 0) == 255 || Equipment(*this) != true) {
    return first;
  }

  return first || IsItemCard(CardId(*this, 1)) || IsItemCard(CardId(*this, 2)) || IsItemCard(CardId(*this, 3));
}

// OFFSET: 0x006D5780
// FUNCTION: fn_06577
__declspec(noinline) int ITEM_INFO::Grade(char* output, int& capacity, unsigned char consume) const {
  char grade[2] = {};
  if (1 == this->grade) {
    grade[0] = 'D';
  } else if (2 == this->grade) {
    grade[0] = 'C';
  } else if (3 == this->grade) {
    grade[0] = 'B';
  } else if (4 == this->grade) {
    grade[0] = 'A';
  }

  const int bytes = StringFormat(output, capacity, _TRUNCATE, "[%s] ", grade);
  if (bytes > 0) {
    if (consume) {
      capacity -= bytes;
    }
    return bytes;
  }

  return 0;
}

// OFFSET: 0x006D6C10
// FUNCTION: fn_06585
int ITEM_INFO::Creator(char* output, int& capacity, unsigned char consume) const {
  std::string name = LookupPlayerName(static_cast<unsigned>(CardId(*this, 2)) + (static_cast<unsigned>(CardId(*this, 3)) << 16));

  if (name.empty()) {
    name = LookupMessage(MSI_NAMELESS_USER);
  }

  return CopyText(output, capacity, consume, name.c_str());
}

// OFFSET: 0x006D6F50
// FUNCTION: fn_06588
__declspec(noinline) int ITEM_INFO::Prefixes(char* output, int& capacity, unsigned char consume) const {
  int ids[4] = {CardId(*this, 0), CardId(*this, 1), CardId(*this, 2), CardId(*this, 3)};
  int remaining = capacity;
  char* cursor = output;

  for (int index = 0; index < 4; ++index) {
    if (ids[index] <= 0) {
      continue;
    }

    const char* prefix = FindItemCardName(ids[index], g_itemCardNames).prefix;
    if (!prefix || !*prefix) {
      continue;
    }

    int count = 1;

    for (int next = index + 1; next < 4; ++next) {
      if (ids[next] == ids[index]) {
        ids[next] = 0;
        ++count;
      }
    }

    int bytes = 0;
    switch (count) {
    case 2:
      bytes = StringFormat(cursor, remaining, _TRUNCATE, "%s ", LookupMessage(MSI_DOUBLE));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
      break;
    case 3:
      bytes = StringFormat(cursor, remaining, _TRUNCATE, "%s ", LookupMessage(MSI_TRIPLE));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
      break;
    case 4:
      bytes = StringFormat(cursor, remaining, _TRUNCATE, "%s ", LookupMessage(MSI_QUADROPLE));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
      break;
    }

    bytes = StringFormat(cursor, remaining, _TRUNCATE, "%s ", FindItemCardName(ids[index], g_itemCardNames).prefix);
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
int ITEM_INFO::Suffixes(char* output, int& capacity, unsigned char consume) const {
  int ids[4] = {CardId(*this, 0), CardId(*this, 1), CardId(*this, 2), CardId(*this, 3)};
  int remaining = capacity;
  char* cursor = output;

  for (int index = 0; index < 4; ++index) {
    if (ids[index] <= 0) {
      continue;
    }

    const char* suffix = FindItemCardName(ids[index], g_itemCardNames).suffix;
    if (!suffix || !*suffix) {
      continue;
    }

    int count = 1;

    for (int next = index + 1; next < 4; ++next) {
      if (ids[next] == ids[index]) {
        ids[next] = 0;
        ++count;
      }
    }

    int bytes = StringFormat(cursor, remaining, _TRUNCATE, " %s", FindItemCardName(ids[index], g_itemCardNames).suffix);
    if (bytes > 0) {
      cursor += bytes;
      remaining -= bytes;
    }

    switch (count) {
    case 2:
      bytes = StringFormat(cursor, remaining, _TRUNCATE, " %s", LookupMessage(MSI_DOUBLE));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
      break;
    case 3:
      bytes = StringFormat(cursor, remaining, _TRUNCATE, " %s", LookupMessage(MSI_TRIPLE));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
      break;
    case 4:
      bytes = StringFormat(cursor, remaining, _TRUNCATE, " %s", LookupMessage(MSI_QUADROPLE));
      if (bytes > 0) {
        cursor += bytes;
        remaining -= bytes;
      }
      break;
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
int ITEM_INFO::Slots(char* output, int& capacity, unsigned char consume) const {
  const int id = std::atoi(this->itemId.c_str());
  const int slots = FindItemDescription(id, g_itemDescriptions).slots;
  if (slots == 0) {
    return 0;
  }

  const int bytes = StringFormat(output, capacity, _TRUNCATE, " [%d]", slots);
  if (bytes <= 0) {
    return 0;
  }

  if (consume) {
    capacity -= bytes;
  }

  return bytes;
}

namespace {
  // INLINED
  __forceinline void AppendText(std::string& result, const char* text, int bytes) {
    result += bytes > 0 ? text : "";
  }

  // INLINED
  __forceinline void AppendName(const ITEM_INFO& item, std::string& result, char (&text)[128], int& capacity) {
    const int id = std::atoi(item.itemId.c_str());
    const char* name = FindItemDescription(id, g_itemDescriptions).identifiedName;
    AppendText(result, text, CopyText(text, capacity, 0, name ? name : " "));
  }
}

// OFFSET: 0x006D59A0
// FUNCTION: fn_06581
std::string ITEM_INFO::DisplayName(unsigned char detailed) const {
  char text[128] = {};
  std::string result;
  int capacity = 128;
  result.reserve(150);

  if (this->identified == 1) {
    const int id = std::atoi(this->itemId.c_str());
    if ((static_cast<unsigned>(id) - 9001u <= 498 || static_cast<unsigned>(id) - 10000u <= 999) && CardId(*this, 3) == 1) {
      AppendText(result, text, this->Apprentice(text, capacity, 0));
      AppendName(*this, result, text, capacity);
    } else if (CardId(*this, 0) == 254 && CardId(*this, 1) == 0) {
      if (this->refiningLevel > 0) {
        AppendText(result, text, this->Refine(text, capacity, 0));
      }

      if (this->grade > 0) {
        AppendText(result, text, this->Grade(text, capacity, 0));
      }

      if (g_serviceType == ServiceBrazil || g_serviceType == ServiceRussia) {
        AppendName(*this, result, text, capacity);
        AppendText(result, text, this->Element(text, capacity, 0));
        AppendText(result, text, this->Creator(text, capacity, 0));
      } else {
        AppendText(result, text, this->Creator(text, capacity, 0));
        AppendText(result, text, this->Element(text, capacity, 0));
        AppendName(*this, result, text, capacity);
      }
    } else {
      if (!Equipment(*this) && detailed == 0) {
        AppendName(*this, result, text, capacity);
      } else {
        if (this->refiningLevel > 0) {
          AppendText(result, text, this->Refine(text, capacity, 0));
        }

        if (this->grade > 0) {
          AppendText(result, text, this->Grade(text, capacity, 0));
        }

        if (this->HasCards() == true) {
          AppendText(result, text, this->Prefixes(text, capacity, 0));
          AppendName(*this, result, text, capacity);
          AppendText(result, text, this->Suffixes(text, capacity, 0));
          AppendText(result, text, this->Slots(text, capacity, 0));
        } else if (CardId(*this, 0) != 254 && CardId(*this, 0) != 255) {
          AppendName(*this, result, text, capacity);
          AppendText(result, text, this->Slots(text, capacity, 0));
        } else if (g_serviceType == ServiceBrazil) {
          AppendName(*this, result, text, capacity);
          AppendText(result, text, this->Element(text, capacity, 0));
          AppendText(result, text, this->Star(text, capacity, 0));
          AppendText(result, text, this->Creator(text, capacity, 0));
        } else if (g_serviceType == ServiceRussia) {
          AppendText(result, text, this->Star(text, capacity, 0));
          AppendName(*this, result, text, capacity);
          AppendText(result, text, this->Element(text, capacity, 0));
          AppendText(result, text, this->Creator(text, capacity, 0));
        } else {
          AppendText(result, text, this->Star(text, capacity, 0));
          AppendText(result, text, this->Creator(text, capacity, 0));
          AppendText(result, text, this->Element(text, capacity, 0));
          AppendName(*this, result, text, capacity);
        }
      }
    }
  } else {
    AppendText(result, text, this->UnidentifiedName(text, capacity, 0));
  }

  return result;
}

#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Client/Application/ErrorDialog.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Contents/Items/ItemInfoMgr.h"

// GLOBAL: gl_24730
CCommonObject g_tableStorage;

// OFFSET: 0x005E11E0
// FUNCTION: fn_03873
std::string ItemDisplayName(int id) {
  ITEM_INFO item;
  item.SetItemId(id);
  item.identified = 1;
  return item.DisplayName(0);
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
  append(283, &this->identity[0], 4);
  append(285, &this->identity[1], 4);
  append(284, &this->identity[2], 4);
  append(286, &this->identity[3], 4);
  append(287, &this->quantity, 4);
  append(288, &this->prices[0], 4);
  append(289, &this->prices[1], 4);
  append(290, this->cards, 16);
  const int id = std::atoi(this->itemId.c_str());
  append(291, &id, 4);
  append(292, this->displayName.c_str(), static_cast<std::uint32_t>(this->displayName.size()));
  append(293, &this->identified, 1);
  append(294, &this->damaged, 1);
  append(295, &this->refiningLevel, 4);
  append(296, &this->bindOnEquip, 2);
  append(297, &this->expireTime, 4);
  append(298, &this->sprite, 4);
  append(299, &this->grade, 2);
  append(300, &this->optionCount, 4);
  append(301, this->options, 25);
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
  if (item.identified == 1) {
    return FindItemDescription(std::atoi(item.itemId.c_str())).identifiedResource;
  }

  return FindItemDescription(std::atoi(item.itemId.c_str())).unidentifiedResource;
}

// OFFSET: 0x006D76B0
// FUNCTION: fn_06592
int ItemSlotsCount(const ITEM_INFO& item) {
  int id = std::atoi(item.itemId.c_str());
  return FindItemDescription(id).slots;
}

// OFFSET: 0x006D7760
// FUNCTION: fn_06594
int ITEM_INFO::UnidentifiedName(char* output, int& capacity, unsigned char consume) const {
  const int id = std::atoi(this->itemId.c_str());
  const char* name = FindItemDescription(id, g_itemDescriptions).unidentifiedName;
  return CopyText(output, capacity, consume, name ? name : " ");
}

// OFFSET: 0x006D3770
// FUNCTION: fn_06544
__declspec(noinline) ItemCardName& FindItemCardName(int id, std::map<int, ItemCardName>& values) {
  // GLOBAL: gl_24676
  static ItemCardName missing = MakeEmptyItemCardName();
  const auto entry = values.find(id);
  return entry == values.end() ? missing : entry->second;
}

// OFFSET: 0x006D3850
// FUNCTION: fn_45456
ItemMoveInfo& FindItemMoveInfo(int id, std::map<int, ItemMoveInfo>& values) {
  // GLOBAL: gl_24789
  static ItemMoveInfo missingMoveInfo = MakeEmptyItemMoveInfo();
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
void __fastcall DestroyReplayTaggedRecordHeader(std::uint16_t* value) {
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

  unsigned char loaded = 0;
  const char* itemFiles[2] = {"item.txt", "eventItem.txt"};

  for (int index = 0; index < 2; ++index) {
    if (!ResourceExists(itemFiles[index], 0)) {
      continue;
    }

    g_tableStorage.LoadTokens(tokens, itemFiles[index], 2);
    std::vector<char*>::iterator cursor = tokens.begin();

    while (cursor < tokens.end()) {
      ITEM_DESC& desc = EnsureItemDescription(std::atoi(*cursor), g_itemDescriptions);
      cursor += 2;
      if (cursor >= tokens.end()) {
        ShowErrorDialog("item.txt \xbd\xba\xc5\xa9\xb8\xb3\xc6\xae\xb8\xa6 \xc8\xae\xc0\xce\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", 0);
      }

      char* name = *cursor;
      if (name != nullptr) {
        g_session.ConvertTooltipText(name, name);
        name = *cursor;
      }
      desc.identifiedName = name;
      cursor += 2;
      if (cursor >= tokens.end()) {
        ShowErrorDialog("item.txt \xbd\xba\xc5\xa9\xb8\xb3\xc6\xae\xb8\xa6 \xc8\xae\xc0\xce\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.", 0);
      }

      while (*cursor != nullptr) {
        desc.identifiedDescription.push_back(*cursor);
        ++cursor;
      }

      ++cursor;
      if (cursor >= tokens.end()) {
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
        if (cursor >= tokens.end()) {
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
      g_tableStorage.LoadTokens(tokens, "EventNum2ItemDisplayNameTable.txt", 0);
      for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 2) {
        g_session.ConvertTooltipText(token[1], token[1]);
        EnsureItemDescription(std::atoi(*token), g_itemDescriptions).unidentifiedName = token[1];
      }
    }

    if (ResourceExists("EventNum2itemDescTable.txt", 0)) {
      int id = 0;
      g_tableStorage.LoadTokens(tokens, "EventNum2itemDescTable.txt", 1);
      for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
        char* text = *token;
        if (text == nullptr || *text == '\0') {
          continue;
        }

        if (!IsNumericText(text)) {
          DecodeMessageRot(text);
          EnsureItemDescription(id, g_itemDescriptions).unidentifiedDescription.push_back(text);
        } else {
          id = std::atoi(text);
        }
      }
    }

    if (ResourceExists("EventIdNum2ItemDisplayNameTable.txt", 0)) {
      g_tableStorage.LoadTokens(tokens, "EventIdNum2ItemDisplayNameTable.txt", 0);
      for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 2) {
        g_session.ConvertTooltipText(token[1], token[1]);
        EnsureItemDescription(std::atoi(*token), g_itemDescriptions).identifiedName = token[1];
      }
    }

    if (ResourceExists("EventIdNum2itemDescTable.txt", 0)) {
      int id = 0;
      g_tableStorage.LoadTokens(tokens, "EventIdNum2itemDescTable.txt", 1);
      for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
        char* text = *token;
        if (text == nullptr || *text == '\0') {
          continue;
        }

        if (!IsNumericText(text)) {
          DecodeMessageRot(text);
          EnsureItemDescription(id, g_itemDescriptions).identifiedDescription.push_back(text);
        } else {
          id = std::atoi(text);
        }
      }
    }

    g_itemInfoMgr->SetItemInfo(g_itemDescriptions);
  }

  if (ResourceExists("EventNum2ItemResNameTable.txt", 0)) {
    g_tableStorage.LoadTokens(tokens, "EventNum2ItemResNameTable.txt", 0);
    for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 2) {
      EnsureItemDescription(std::atoi(*token), g_itemDescriptions).unidentifiedResource = token[1];
    }
  }

  if (ResourceExists("EventIdNum2ItemResNameTable.txt", 0)) {
    g_tableStorage.LoadTokens(tokens, "EventIdNum2ItemResNameTable.txt", 0);
    for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 2) {
      EnsureItemDescription(std::atoi(*token), g_itemDescriptions).identifiedResource = token[1];
    }
  }

  g_tableStorage.LoadTokens(tokens, "itemSlotTable.txt", 0);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 2) {
    EnsureItemDescription(std::atoi(*token), g_itemDescriptions).descriptionFlags = std::atoi(token[1]);
  }

  g_tableStorage.LoadTokens(tokens, "itemParamTable.txt", 0);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 3) {
    ITEM_DESC& desc = EnsureItemDescription(std::atoi(*token), g_itemDescriptions);
    desc.descriptionValues.push_back(std::atoi(token[1]));
    desc.descriptionValues.push_back(std::atoi(token[2]));
  }

  g_tableStorage.LoadTokens(tokens, "CardItemNameTable.txt", 0);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
    EnsureItemCardName(std::atoi(*token), g_itemCardNames);
  }

  g_tableStorage.LoadTokens(tokens, "Num2CardIllustNameTable.txt", 0);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 2) {
    EnsureItemCardName(std::atoi(*token), g_itemCardNames).name = token[1];
  }

  g_tableStorage.LoadTokens(tokens, "CardPrefixNameTable.txt", 0);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); token += 2) {
    EnsureItemCardName(std::atoi(*token), g_itemCardNames).prefix = token[1];
  }

  g_tableStorage.LoadTokens(tokens, "CardPostfixNameTable.txt", 0);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
    ItemCardName& card = EnsureItemCardName(std::atoi(*token), g_itemCardNames);
    card.suffix = card.prefix;
    card.prefix = "";
  }

  g_tableStorage.LoadTokens(tokens, "BookItemNameTable.txt", 0);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
    EnsureItemDescriptionLines(std::atoi(*token), g_bookItemNames).push_back(*token);
  }

  int metalId = 0;
  g_tableStorage.LoadTokens(tokens, "MetalProcessItemList.txt", 1);
  for (std::vector<char*>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
    char* text = *token;
    if (text == nullptr || *text == '\0') {
      continue;
    }

    if (IsNumericText(text)) {
      metalId = std::atoi(text);
    } else if (std::strlen(text) != 0) {
      EnsureItemDescriptionLines(metalId, g_metalProcessItems).push_back(text);
    }
  }

  ITEM_DESC& unknownItemDesc = FindItemDescription(0, g_itemDescriptions);
  unknownItemDesc.identifiedName = "Unknown Item";
  unknownItemDesc.identifiedResource = "\xbb\xe7\xb0\xfa";
  unknownItemDesc.identifiedDescription.push_back("Unknown Item");
  unknownItemDesc.unidentifiedName = "Unknown Item";
  unknownItemDesc.unidentifiedResource = "\xbb\xe7\xb0\xfa";
  unknownItemDesc.unidentifiedDescription.push_back("Unknown Item");
  FindItemDescription(-1, g_itemDescriptions) = unknownItemDesc;

  ItemCardName& sorry = FindItemCardName(0, g_itemCardNames);
  sorry.name = "sorry";
  sorry.prefix = "";
  sorry.suffix = "";
  FindItemCardName(-1, g_itemCardNames) = sorry;

  std::vector<int> moveInfo;
  if (LoadItemMoveInfoTable(moveInfo, "ItemMoveInfoV5.txt") && !moveInfo.empty()) {
    for (std::vector<int>::iterator value = moveInfo.begin(); value != moveInfo.end(); value += 9) {
      ItemMoveInfo flags;
      flags.itemId = value[0];
      flags.drop = value[1] != 0;
      flags.trade = value[2] != 0;
      flags.storage = value[3] != 0;
      flags.cart = value[4] != 0;
      flags.sellToNpc = value[5] != 0;
      flags.mail = value[6] != 0;
      flags.auction = value[7] != 0;
      flags.guildStorage = value[8] != 0;
      g_itemMoveInfo.insert(std::make_pair(flags.itemId, flags));
    }
  }
}

// OFFSET: 0x006D5650
// FUNCTION: fn_06575
std::uint32_t __fastcall ItemCardColor(const ITEM_INFO* item) {
  const std::uint32_t colors[5] = {0, 0x00ADFF2F, 0x0000BFFF, 0x00FFA500, 0x009400D3};
  if (item->HasCards() == true) {
    int count = 0;
    const int id = std::atoi(item->itemId.c_str());
    const int slots = FindItemDescription(id, g_itemDescriptions).slots;
    for (int index = 0; index < 4; ++index) {
      if (index >= slots && IsItemCard(CardId(*item, index))) {
        ++count;
      }
    }
    return colors[count];
  }

  return 0;
}

// OFFSET: 0x006D6AA0
// FUNCTION: fn_06583
int ITEM_INFO::Element(char* output, int& capacity, unsigned char consume) const {
  switch (reinterpret_cast<const unsigned char*>(this->cards)[4]) {
  case ELE_NEUTRAL: {
    const int id = std::atoi(this->itemId.c_str());
    if (id == 2634 || id == 2635) {
      return CopyText(output, capacity, consume, LookupMessage(2976));
    }
    return CopyText(output, capacity, consume, LookupMessage(450));
  }
  case ELE_WATER:
    return CopyText(output, capacity, consume, LookupMessage(MSI_MANUFACTURED_ICE_ITEM));
  case ELE_EARTH:
    return CopyText(output, capacity, consume, LookupMessage(MSI_MANUFACTURED_EARTH_ITEM));
  case ELE_FIRE:
    return CopyText(output, capacity, consume, LookupMessage(MSI_MANUFACTURED_FIRE_ITEM));
  case ELE_WIND:
    return CopyText(output, capacity, consume, LookupMessage(MSI_MANUFACTURED_WIND_ITEM));
  default:
    return 0;
  }
}

// OFFSET: 0x006D6B90
// FUNCTION: fn_06584
int ITEM_INFO::Star(char* output, int& capacity, unsigned char consume) const {
  switch (reinterpret_cast<const unsigned char*>(this->cards)[5]) {
  case 5:
    return CopyText(output, capacity, consume, LookupMessage(MSI_ONE_STARPIECE));
  case 10:
    return CopyText(output, capacity, consume, LookupMessage(MSI_TWO_STARPIECE));
  case 15:
    return CopyText(output, capacity, consume, LookupMessage(MSI_THREE_STARPIECE));
  default:
    return 0;
  }
}

// OFFSET: 0x006D6D50
// FUNCTION: fn_06586
int ITEM_INFO::Apprentice(char* output, int& capacity, unsigned char consume) const {
  const char* text = LookupMessage(MSI_NAMED_PET);
  const int error = strncpy_s(output, capacity, text, _TRUNCATE);
  if (error != 0 && error != 80) {
    return 0;
  }

  const int written = !output ? 0 : static_cast<int>(strnlen(output, capacity));
  if (consume) {
    capacity -= written;
  }
  return written;
}

// OFFSET: 0x006D70A0
// FUNCTION: fn_06589
int ITEM_INFO::Refine(char* output, int& capacity, unsigned char consume) const {
  const int bytes = StringFormat(output, capacity, static_cast<std::size_t>(-1), "+%d ", this->refiningLevel);

  if (bytes > 0) {
    if (consume) {
      capacity -= bytes;
    }

    return bytes;
  }
  return 0;
}
