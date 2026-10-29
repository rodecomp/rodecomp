#ifndef RAGNAROK_SOURCE_CONTENTS_STORE_MCSTOREINFOBACKUP_H
#define RAGNAROK_SOURCE_CONTENTS_STORE_MCSTOREINFOBACKUP_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/WindowNotifications.h"

#include <vector>

class LuaScript;
class CAsyncWorkMgr;
struct AsyncDiagnostic;
namespace Json {
  class Value;
}
struct MerchantShopRefresh;

struct StoreBackupContext {
  WindowNotifications& ui;
  MerchantShopRefresh* const& shop;
  MerchantShopRefresh* const& otherShop;
  const bool& languagePrefix;
  const int& serviceType;
  const std::uint16_t& language;

  void RefreshShop();
};

class CMCStoreInfoBackup {
public:
  CMCStoreInfoBackup();
  virtual ~CMCStoreInfoBackup();
  void Clear();
  int Restore(unsigned char wholeItem, int maximum, std::list<ITEM_INFO>& inventory, std::list<ITEM_INFO>& storeItems);
  std::string LoadUrl() const;
  std::string SaveUrl() const;
  void Reset(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey);
  void CompleteLoad(
    AsyncDiagnostic diagnostic, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string response, StoreBackupContext& context
  );
  void CompleteSave(AsyncDiagnostic diagnostic, std::string response, StoreBackupContext& context);
  void Load(
    std::uint32_t accountId, std::uint32_t characterId, std::string accountName, const char* token16, int type, CAsyncWorkMgr& workManager,
    StoreBackupContext& context
  );
  void Save(
    int type, std::uint32_t accountId, std::uint32_t characterId, const char* accountName, const char* token16, const std::list<ITEM_INFO>& storeItems,
    CAsyncWorkMgr& workManager, StoreBackupContext& context
  );

  /* 0004 */ bool enabled;
  /* 0005 */ bool loaded;
  /* 0008 */ std::uint32_t pendingSaves;
  /* 000C */ std::string saveUrl;
  /* 0024 */ std::string loadUrl;
  /* 003C */ std::string title;
  /* 0054 */ int type;
  /* 0058 */ std::vector<ITEM_INFO> items;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMCStoreInfoBackup) == 100);
#endif

#endif
