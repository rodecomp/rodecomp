#include "Contents/Store/MCStoreInfoBackup.h"
#include "Client/Session/Session.h"
#include "Network/TextEncoding.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <json/value.h>

// OFFSET: 0x00606190
// FUNCTION: fn_04233
void CMCStoreInfoBackup::ImportData(Json::Value& data) {
  this->items.clear();
  this->title = static_cast<const std::string&>(FromUtf8(data.get("Title", "").asString(), g_textCodePage));
  Json::Value values = data["Item"];
  const std::string styledText = values.toStyledString();

  if (values.isNull() && this->title.empty()) {
    return;
  }

  for (auto entry = values.begin(); entry != values.end(); ++entry) {
    if (!(*entry).isObject()) {
      continue;
    }

    ITEM_INFO item;
    item.SetItemId((*entry).get("ID", 0).asInt());
    item.quantity = (*entry).get("Count", 0).asInt();
    item.identified = (*entry).get("Identified", 0).asBool();
    item.refiningLevel = (*entry).get("RefiningLevel", 0).asInt();
    item.grade = static_cast<std::int16_t>((*entry).get("GradeLevel", 0).asInt());

    item.cards[0] = (*entry).get("Slot_1", 0).asInt();
    item.cards[1] = (*entry).get("Slot_2", 0).asInt();
    item.cards[2] = (*entry).get("Slot_3", 0).asInt();
    item.cards[3] = (*entry).get("Slot_4", 0).asInt();
    item.optionCount = (*entry).get("RandomOptionCount", 0).asInt();
    Json::Value options = (*entry)["RandomOptionList"];
    int index = 0;
    ItemOptionRecord* record = item.options;

    for (auto option = options.begin(); option != options.end() && index <= 5; ++option, ++index, ++record) {
      record->id = static_cast<std::int16_t>((*option).get("OptIndex", 0).asInt());
      record->value = static_cast<std::int16_t>((*option).get("Value", 0).asInt());
      record->parameter = static_cast<unsigned char>((*option).get("Parm1", 0).asInt());
    }

    item.prices[0] = static_cast<std::uint32_t>((*entry).get("Price", 0).asInt());
    this->items.push_back(item);
  }

  UIWindow* window = g_windowMgr.merchantShopMakeWnd ? g_windowMgr.merchantShopMakeWnd : g_windowMgr.buyingShopMakeWnd;

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
    value["RefiningLevel"] = item.refiningLevel;
    value["GradeLevel"] = item.grade;
    value["Slot_1"] = static_cast<std::int32_t>(item.cards[0]);
    value["Slot_2"] = static_cast<std::int32_t>(item.cards[1]);
    value["Slot_3"] = static_cast<std::int32_t>(item.cards[2]);
    value["Slot_4"] = static_cast<std::int32_t>(item.cards[3]);

    const int count = item.optionCount;
    value["RandomOptionCount"] = count;
    Json::Value options;

    for (int index = 0; index < count; ++index) {
      const ItemOptionRecord& record = item.options[index];
      const std::int16_t id = record.id;
      const std::int16_t amount = record.value;
      Json::Value option;
      option["OptIndex"] = id;
      option["Value"] = amount;
      option["Parm1"] = record.parameter;
      options.append(option);
    }

    value["RandomOptionList"] = options;
    value["Price"] = static_cast<int>(item.prices[0]);
    values.append(value);
  }

  data["Item"] = values;
}
