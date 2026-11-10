#include "UI/Window/UIEnums.h"
#include "Contents/UserData/AccountService.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Store/MCStoreInfoBackup.h"
#include "Contents/Tables/MessageTable.h"
#include "Client/Session.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

// OFFSET: 0x00605BF0
// FUNCTION: fn_04226
CMCStoreInfoBackup::CMCStoreInfoBackup() : loaded(false), pendingSaves(0), saveUrl(""), loadUrl(""), title(""), type(0) {
  this->enabled = true;
}

// OFFSET: 0x00605D70
// FUNCTION: fn_04227
CMCStoreInfoBackup::~CMCStoreInfoBackup() = default;

// OFFSET: 0x00605EC0
// FUNCTION: fn_04229
void CMCStoreInfoBackup::Clear() {
  this->items.clear();
}

// OFFSET: 0x00606BB0
// FUNCTION: fn_04234
int CMCStoreInfoBackup::Restore(unsigned char wholeItem, int maximum) {
  CSession& lists = g_session;
  int examined = 0;
  auto saved = this->items.begin();

  while (saved != this->items.end() && examined < maximum) {
    std::pair<bool, ITEM_INFO> found = lists.FindMerchantMyShopItem(*saved, wholeItem);

    if (!found.first) {
      saved = this->items.erase(saved);
    } else {
      if (wholeItem == 0) {
        found.second.quantity = std::min(found.second.quantity, saved->quantity);
      }

      if (found.second.identity[1] != 0) {
        lists.AddMerchantPurchaseSavedItem(found.second, wholeItem, 0);
        lists.RemoveMerchantMyShopItem(found.second, wholeItem, 0);
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
void CMCStoreInfoBackup::Reset(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey) {
  this->loaded = false;
  this->title = "";
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
  AsyncDiagnostic diagnostic, std::uint32_t, std::uint32_t, std::string, std::string response
) {
  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;

  if (!reader.parse(std::string(response.c_str()), root, true)) {
    return;
  }

  switch (root["Type"].asInt()) {
  case 1:
    this->ImportData(root["data"]);
    this->loaded = true;
    break;
  case 4:
    this->loaded = true;
    break;
  }
}

// OFFSET: 0x00607810
// FUNCTION: fn_04239
void CMCStoreInfoBackup::CompleteSave(AsyncDiagnostic diagnostic, std::string response) {
  --this->pendingSaves;

  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;

  if (!reader.parse(std::string(response.c_str()), root, true)) {
    return;
  }

  if (root["Type"].asInt() != 1) {
    return;
  }

  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);

  if (this->type == 0) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EQUIP_FAIL_DURING_BARTER)), 0xFFFF, 0, 0, 0);
  } else if (this->type == 1) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BARTER_ITEMLIST)), 0xFFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00606DE0
// FUNCTION: fn_04235
void CMCStoreInfoBackup::Load(
  std::uint32_t accountId, std::uint32_t characterId, std::string accountName, int type
) {
  if (accountId == 0 || characterId == 0) {
    return;
  }

  g_asyncWorkMgr->LoadStore(accountId, characterId, accountName, this->LoadUrl(),
                            std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[0x08]), 16), type);
}

// OFFSET: 0x00606EB0
// FUNCTION: fn_04236
void CMCStoreInfoBackup::Save(int type) {
  this->type = type;
  Json::Value root;
  Json::Value data;
  this->ExportData(data);

  if (!data.isNull()) {
    root["data"] = data;
    Json::StyledWriter writer;
    const std::string response = writer.write(root);
    g_asyncWorkMgr->SaveStore(g_session.m_aid, g_session.m_gid, std::string(g_session.m_accountName.c_str()), this->SaveUrl(),
                              std::string(reinterpret_cast<const char*>(&g_session.webServiceLoginData[8]), 16), response, this->type);

    ++this->pendingSaves;
  }
}
