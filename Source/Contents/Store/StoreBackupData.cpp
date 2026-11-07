#include "Contents/Store/MCStoreInfoBackup.h"
#include "Client/Session.h"
#include "Network/TextEncoding.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <json/value.h>

// INLINED
template <typename T>
static __forceinline void StoreField(unsigned char* field, T value) {
  std::memcpy(field, &value, sizeof(value));
}

// INLINED
template <typename T>
static __forceinline T LoadField(const unsigned char* field) {
  T value;
  std::memcpy(&value, field, sizeof(value));
  return value;
}

// OFFSET: 0x00606190
// FUNCTION: fn_04233
void CMCStoreInfoBackup::ImportData(Json::Value& data) {
  this->items.clear();
  this->title = FromUtf8(data.get("Title", "").asString(), g_textCodePage);
  Json::Value values = data["Item"];
  const std::string styledText = values.toStyledString();

  if (values.isNull() && this->title.empty()) {
    return;
  }

  for (auto entry = values.begin(); entry != values.end(); ++entry) {
    Json::Value& value = *entry;

    if (!value.isObject()) {
      continue;
    }

    ITEM_INFO item;
    item.SetItemId(value.get("ID", 0).asInt());
    item.quantity = value.get("Count", 0).asInt();
    item.identified = value.get("Identified", 0).asBool();
    item.unknown_0060 = value.get("RefiningLevel", 0).asInt();
    item.unknown_0088 = static_cast<std::int16_t>(value.get("GradeLevel", 0).asInt());

    StoreField<std::int32_t>(item.unknown_001C + 0, value.get("Slot_1", 0).asInt());
    StoreField<std::int32_t>(item.unknown_001C + 4, value.get("Slot_2", 0).asInt());
    StoreField<std::int32_t>(item.unknown_001C + 8, value.get("Slot_3", 0).asInt());
    StoreField<std::int32_t>(item.unknown_001C + 12, value.get("Slot_4", 0).asInt());
    item.unknown_0098 = value.get("RandomOptionCount", 0).asInt();
    Json::Value options = value["RandomOptionList"];
    int index = 0;
    unsigned char* record = item.unknown_009C;

    for (auto option = options.begin(); option != options.end() && index <= 5; ++option, ++index, record += 5) {
      StoreField<std::int16_t>(record, (*option).get("OptIndex", 0).asInt());
      StoreField<std::int16_t>(record + 2, (*option).get("Value", 0).asInt());
      record[4] = static_cast<unsigned char>((*option).get("Parm1", 0).asInt());
    }

    item.unknown_0014[0] = static_cast<std::uint32_t>(value.get("Price", 0).asInt());
    this->items.push_back(item);
  }

  UIWindow* window = g_windowMgr.unknown_02FC ? g_windowMgr.unknown_02FC : g_windowMgr.unknown_040C;

  if (window) {
    static_cast<UIMerchantShopMakeWnd*>(window)->Refresh();
  }
}

// OFFSET: 0x00607240
// FUNCTION: fn_04238
void CMCStoreInfoBackup::ExportData(Json::Value& data) const {
  data["Title"] = ToUtf8(this->title, g_textCodePage);
  Json::Value values;

  const int itemCount = g_session.GetList9Size();
  for (int itemIndex = 0; itemIndex < itemCount; ++itemIndex) {
    const ITEM_INFO item = g_session.GetItem1758At(itemIndex);
    Json::Value value;
    value["ID"] = std::atoi(item.itemId.c_str());
    value["Count"] = item.quantity;
    value["Identified"] = item.identified != 0;
    value["RefiningLevel"] = item.unknown_0060;
    value["GradeLevel"] = item.unknown_0088;
    const char* slotKeys[] = {"Slot_1", "Slot_2", "Slot_3", "Slot_4"};

    for (unsigned index = 0; index < 4; ++index) {
      std::int32_t slot;
      std::memcpy(&slot, item.unknown_001C + 4 * index, sizeof(slot));
      value[slotKeys[index]] = slot;
    }

    const int count = item.unknown_0098;
    value["RandomOptionCount"] = count;
    Json::Value options;

    for (int index = 0; index < count; ++index) {
      const auto* record = reinterpret_cast<const unsigned char*>(&item) + offsetof(ITEM_INFO, unknown_009C) + index * 5;
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
    value["Price"] = static_cast<int>(item.unknown_0014[0]);
    values.append(value);
  }

  data["Item"] = values;
}
