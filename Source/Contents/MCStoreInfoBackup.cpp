#include "Contents/AccountService.h"
#include "Contents/AsyncWorkMgr.h"
#include "Contents/MCStoreInfoBackup.h"
#include "Contents/MessageTable.h"
#include "Contents/StoreBackupData.h"
#include "Utility/AsyncDiagnostic.h"

#include <algorithm>
#include <bit>
#include <cstring>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

// OFFSET: 0x00605BF0
// FUNCTION: fn_04226
CMCStoreInfoBackup::CMCStoreInfoBackup() : enabled(true), loaded(false), pendingSaves(0), type(0) {
}

// OFFSET: 0x00605D70
// FUNCTION: fn_04227
// OFFSET: 0x00605DF0
// FUNCTION: fn_04228
CMCStoreInfoBackup::~CMCStoreInfoBackup() = default;

// OFFSET: 0x00605EC0
// FUNCTION: fn_04229
void CMCStoreInfoBackup::Clear() {
  this->items.clear();
}

// OFFSET: 0x00606BB0
// FUNCTION: fn_04234
int CMCStoreInfoBackup::Restore(
  unsigned char wholeItem,
  int maximum,
  std::list<ItemInfo>& inventory,
  std::list<ItemInfo>& storeItems
) {
  auto saved = this->items.begin();
  int examined = 0;

  while (saved != this->items.end() && examined < maximum) {
    // Inventory match, including the strict <5 count.
    const auto found = std::find_if(inventory.begin(), inventory.end(), [&](const ItemInfo& item) {
      if (
        item.itemId != saved->itemId ||
        std::memcmp(item.unknown_001C, saved->unknown_001C, sizeof(item.unknown_001C)) != 0 ||
        item.identified != saved->identified || item.unknown_0060 != saved->unknown_0060 ||
        item.unknown_0088 != saved->unknown_0088 || item.unknown_0098 != saved->unknown_0098
      ) {
        return false;
      }

      const int count = std::bit_cast<std::int32_t>(item.unknown_0098);

      if (count >= 5) {
        return false;
      }

      return count <= 0 ||
             std::memcmp(item.unknown_009C, saved->unknown_009C, static_cast<std::size_t>(count) * 5) == 0;
    });

    if (found == inventory.end()) {
      saved = this->items.erase(saved);
    } else {
      ItemInfo item(*found);

      if (wholeItem == 0) {
        item.quantity = std::min(item.quantity, saved->quantity);
      }

      if (item.unknown_0000[0] != 0) {
        // Adds/merges the selected inventory value, then subtracts quantity or removes the complete identity.
        const auto offered = std::find_if(storeItems.begin(), storeItems.end(), [&](const ItemInfo& value) {
          return value.unknown_0000[1] == item.unknown_0000[1];
        });

        if (offered == storeItems.end()) {
          storeItems.push_back(item);
        } else if (wholeItem == 0) {
          offered->quantity = std::bit_cast<std::int32_t>(
            static_cast<std::uint32_t>(offered->quantity) + static_cast<std::uint32_t>(item.quantity)
          );
        }

        if (wholeItem != 0) {
          inventory.erase(found);
        } else {
          found->quantity = std::bit_cast<std::int32_t>(
            static_cast<std::uint32_t>(found->quantity) - static_cast<std::uint32_t>(item.quantity)
          );

          if (found->quantity <= 0) {
            inventory.erase(found);
          }
        }
      }

      ++saved;
    }

    ++examined;
  }

  return examined;
}

// OFFSET: 0x00605EE0
// FUNCTION: fn_04230
std::string CMCStoreInfoBackup::LoadUrl() const {
  return this->loadUrl;
}

// OFFSET: 0x00605F40
// FUNCTION: fn_04231
std::string CMCStoreInfoBackup::SaveUrl() const {
  return this->saveUrl;
}

// OFFSET: 0x006070E0
// FUNCTION: fn_04237
void CMCStoreInfoBackup::Reset(
  LuaScript& settings,
  LuaScript& tables,
  const char* host,
  const char* port,
  const char* overrideKey
) {
  this->loaded = false;
  this->title.clear();
  this->items.clear();
  const std::string base = ResolveAccountServiceBase(settings, tables, host, port, overrideKey);

  if (!base.empty()) {
    this->saveUrl = base + "/MerchantStore/save";
    this->loadUrl = base + "/MerchantStore/load";
  }
}

// OFFSET: 0x00605FA0
// FUNCTION: fn_04232
void CMCStoreInfoBackup::CompleteLoad(
  AsyncDiagnostic diagnostic,
  std::uint32_t,
  std::uint32_t,
  std::string,
  std::string response,
  StoreBackupContext& context
) {
  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;
  const std::string document(response.c_str());

  if (!reader.parse(document, root, true)) {
    return;
  }

  const int type = root["Type"].asInt();

  if (type == 1) {
    Json::Value& data = root["data"];
    StoreBackupData{this->title, this->items}.Import(data);

    if (!data["Item"].isNull() || !this->title.empty()) {
      context.RefreshShop();
    }
  }

  if (type == 1 || type == 4) {
    this->loaded = true;
  }
}

// OFFSET: 0x00607810
// FUNCTION: fn_04239
void CMCStoreInfoBackup::CompleteSave(AsyncDiagnostic diagnostic, std::string response, StoreBackupContext& context) {
  --this->pendingSaves;

  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;
  const std::string document(response.c_str());

  if (!reader.parse(document, root, true) || root["Type"].asInt() != 1) {
    return;
  }

  context.ui.Notify(3, nullptr, 0);

  if (this->type == 0 || this->type == 1) {
    const char* message =
      LookupMessage(this->type == 0 ? 3685 : 3686, context.languagePrefix, context.serviceType, context.language);
    context.ui.Notify(1, message, 0xFFFF);
  }
}

// OFFSET: 0x00606DE0
// FUNCTION: fn_04235
void CMCStoreInfoBackup::Load(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string accountName,
  const char* token16,
  int type,
  CAsyncWorkMgr& workManager,
  StoreBackupContext& context
) {
  if (accountId == 0 || characterId == 0) {
    return;
  }

  const std::string token(token16, 16);
  const std::string url = this->LoadUrl();
  workManager.LoadStore(accountId, characterId, accountName, url, token, type, *this, context);
}

// OFFSET: 0x00606EB0
// FUNCTION: fn_04236
void CMCStoreInfoBackup::Save(
  int type,
  std::uint32_t accountId,
  std::uint32_t characterId,
  const char* accountName,
  const char* token16,
  const std::list<ItemInfo>& storeItems,
  CAsyncWorkMgr& workManager,
  StoreBackupContext& context
) {
  this->type = type;
  Json::Value root;
  Json::Value data;
  StoreBackupData{this->title, this->items}.Export(data, storeItems);

  if (!data.isNull()) {
    root["data"] = data;
    Json::StyledWriter writer;
    const std::string response = writer.write(root);
    const std::string token(token16, 16);
    const std::string url = this->SaveUrl();
    const std::string name(accountName);
    workManager.SaveStore(accountId, characterId, name, url, token, response, this->type, *this, context);

    ++this->pendingSaves;
  }
}
