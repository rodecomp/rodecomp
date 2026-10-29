#ifndef RAGNAROK_SOURCE_CONTENTS_STORE_STOREBACKUPDATA_H
#define RAGNAROK_SOURCE_CONTENTS_STORE_STOREBACKUPDATA_H

#include "Contents/Items/ItemInfo.h"

#include <string>
#include <vector>

namespace Json {
  class Value;
}

struct StoreBackupData {
  std::string& title;
  std::vector<ITEM_INFO>& items;

  void Import(Json::Value& data);
  void Export(Json::Value& data, const std::list<ITEM_INFO>& storeItems) const;
};

#endif
