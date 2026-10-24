#ifndef RAGNAROK_SOURCE_CONTENTS_STOREBACKUPDATA_H
#define RAGNAROK_SOURCE_CONTENTS_STOREBACKUPDATA_H

#include "Contents/ItemInfo.h"

#include <string>
#include <vector>

namespace Json {
  class Value;
}

// The title and vector belong to CMCStoreInfoBackup (+0x3C and +0x58).
// Serialization only; applying restored items to an open shop remains UI work.
struct StoreBackupData {
  std::string& title;
  std::vector<ItemInfo>& items;

  void Import(Json::Value& data);
  void Export(Json::Value& data, const std::list<ItemInfo>& storeItems) const;
};

#endif
