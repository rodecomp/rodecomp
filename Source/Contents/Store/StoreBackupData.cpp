#include "Contents/Store/MCStoreInfoBackup.h"
#include "Client/Session/Session.h"
#include "Network/TextEncoding.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <json/value.h>

// OFFSET: 0x00605E80
// FUNCTION: fn_46720
void CMCStoreInfoBackup::AppendItem(const ITEM_INFO& item) {
  this->items.push_back(item);
}

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

    for (auto option = options.begin(); option != options.end() && index <= 5; ++option, ++index) {
      item.options[index].id = static_cast<std::int16_t>((*option).get("OptIndex", 0).asInt());
      item.options[index].value = static_cast<std::int16_t>((*option).get("Value", 0).asInt());
      item.options[index].parameter = static_cast<unsigned char>((*option).get("Parm1", 0).asInt());
    }

    item.prices[0] = static_cast<std::uint32_t>((*entry).get("Price", 0).asInt());
    this->AppendItem(item);
  }

  if (g_windowMgr.merchantShopMakeWnd) {
    g_windowMgr.merchantShopMakeWnd->Refresh();
  } else if (g_windowMgr.buyingShopMakeWnd) {
    g_windowMgr.buyingShopMakeWnd->Refresh();
  }
}

// OFFSET: 0x00607240
// FUNCTION: fn_04238
void CMCStoreInfoBackup::ExportData(Json::Value& data) const {
  data["Title"] = ToUtf8(this->title, g_textCodePage);
  Json::Value values;

  const int itemCount = g_session.GetNumMerchantPurchaseItems();
  for (int itemIndex = 0; itemIndex < itemCount; ++itemIndex) {
    const ITEM_INFO item = g_session.GetMerchantPurchaseItemInfoBySequence(itemIndex);
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

    value["RandomOptionCount"] = item.optionCount;
    Json::Value options;
    const int count = item.optionCount;

    for (int index = 0; index < count; ++index) {
      Json::Value option;
      option["OptIndex"] = item.options[index].id;
      option["Value"] = item.options[index].value;
      option["Parm1"] = item.options[index].parameter;
      options.append(option);
    }

    value["RandomOptionList"] = options;
    value["Price"] = static_cast<int>(item.prices[0]);
    values.append(value);
  }

  data["Item"] = values;
}
