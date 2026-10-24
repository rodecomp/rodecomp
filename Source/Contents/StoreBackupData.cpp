#include "Contents/StoreBackupData.h"
#include "Network/TextEncoding.h"

#include <bit>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <json/value.h>

// OFFSET: 0x00606190
// FUNCTION: fn_04233
void StoreBackupData::Import(Json::Value& data) {
  this->items.clear();
  this->title = FromUtf8(data.get("Title", "").asString(), g_textCodePage);
  Json::Value values = data["Item"];

  for (auto entry = values.begin(); entry != values.end(); ++entry) {
    Json::Value& value = *entry;

    if (!value.isObject()) {
      continue;
    }

    ItemInfo item;
    item.itemId = std::to_string(value.get("ID", 0).asInt());
    item.quantity = value.get("Count", 0).asInt();
    item.identified = value.get("Identified", 0).asBool();
    item.unknown_0060 = static_cast<std::uint32_t>(value.get("RefiningLevel", 0).asInt());
    item.unknown_0088 = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(value.get("GradeLevel", 0).asInt()));

    const char* slotKeys[] = {"Slot_1", "Slot_2", "Slot_3", "Slot_4"};

    for (unsigned index = 0; index < 4; ++index) {
      const auto slot = static_cast<std::uint32_t>(value.get(slotKeys[index], 0).asInt());
      std::memcpy(item.unknown_001C + 4 * index, &slot, sizeof(slot));
    }

    item.unknown_0098 = static_cast<std::uint32_t>(value.get("RandomOptionCount", 0).asInt());
    Json::Value& options = value["RandomOptionList"];
    unsigned index = 0;

    for (auto option = options.begin(); option != options.end() && index <= 5; ++option, ++index) {
      const auto id = static_cast<std::uint16_t>((*option).get("OptIndex", 0).asInt());
      const auto amount = static_cast<std::uint16_t>((*option).get("Value", 0).asInt());
      const auto parameter = static_cast<unsigned char>((*option).get("Parm1", 0).asInt());
      auto* record = reinterpret_cast<unsigned char*>(&item) + offsetof(ItemInfo, unknown_009C) + index * 5;
      // Original accepts six records, spilling the sixth into +0xB5..+0xB9.
      std::memcpy(record, &id, sizeof(id));
      std::memcpy(record + 2, &amount, sizeof(amount));
      record[4] = parameter;
    }

    item.unknown_0014[0] = static_cast<std::uint32_t>(value.get("Price", 0).asInt());
    this->items.push_back(item);
  }
}

// OFFSET: 0x00607240
// FUNCTION: fn_04238
void StoreBackupData::Export(Json::Value& data, const std::list<ItemInfo>& storeItems) const {
  data["Title"] = ToUtf8(this->title, g_textCodePage);
  Json::Value values;

  for (const ItemInfo& item : storeItems) {
    Json::Value value;
    value["ID"] = std::atoi(item.itemId.c_str());
    value["Count"] = item.quantity;
    value["Identified"] = item.identified != 0;
    value["RefiningLevel"] = std::bit_cast<std::int32_t>(item.unknown_0060);
    value["GradeLevel"] = item.unknown_0088;
    const char* slotKeys[] = {"Slot_1", "Slot_2", "Slot_3", "Slot_4"};

    for (unsigned index = 0; index < 4; ++index) {
      std::int32_t slot;
      std::memcpy(&slot, item.unknown_001C + 4 * index, sizeof(slot));
      value[slotKeys[index]] = slot;
    }

    const int count = std::bit_cast<std::int32_t>(item.unknown_0098);
    value["RandomOptionCount"] = count;
    Json::Value options;

    for (int index = 0; index < count; ++index) {
      const auto* record = reinterpret_cast<const unsigned char*>(&item) + offsetof(ItemInfo, unknown_009C) + index * 5;
      std::int16_t id, amount;
      std::memcpy(&id, record, sizeof(id));
      std::memcpy(&amount, record + 2, sizeof(amount));
      Json::Value option;
      option["OptIndex"] = id;
      option["Value"] = amount;
      option["Parm1"] = record[4];
      options.append(option);
    }

    value["RandomOptionList"] = options;
    value["Price"] = std::bit_cast<std::int32_t>(item.unknown_0014[0]);
    values.append(value);
  }

  data["Item"] = values;
}
