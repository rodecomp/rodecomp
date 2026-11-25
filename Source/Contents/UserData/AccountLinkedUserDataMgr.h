#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATAMGR_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATAMGR_H

#include <cstdint>
#include <string>

#include "Utility/Async/AsyncDiagnostic.h"
class LuaScript;
class CRenderSystemBase;
class CAsyncWorkMgr;

namespace Json {
class Value;
}

class CAccountLinkedUserDataMgr {
  friend class CGameMode;
  friend void ShutdownManagers();

public:
  CAccountLinkedUserDataMgr();
  virtual ~CAccountLinkedUserDataMgr();
  static bool Initialize();
  static void Shutdown();
  std::string SaveUrl() const;
  std::string LoadUrl() const;
  __declspec(noinline) void RefreshUrls();

  void CompleteLoad(
    AsyncDiagnostic diagnostic, std::uint32_t accountId, std::string accountName, std::string response
  );
  void Load(std::uint32_t accountId, std::string accountName);
  void Reset();
  // INLINED
  __declspec(noinline) void Reset(LuaScript&, const char*, const char*, const char*) {
    this->Reset();
  }
  void CompleteSave();
  void Save();
  void ExportUserHotkey(Json::Value& data);

protected:
  void LoadEmotionHotkeys(Json::Value& data);
  void LoadUserHotkeysV2(Json::Value& data);
  void LoadUserHotkeyTab(Json::Value& values, int tab);

  /* 0004 */ bool m_enabled;
  /* 0008 */ std::uint32_t m_accountId;
  /* 000C */ std::string m_accountName;
  /* 0024 */ bool m_loaded;
  /* 0028 */ std::uint32_t m_pendingLoads;
  /* 002C */ std::string m_saveUrl;
  /* 0044 */ std::string m_loadUrl;
};

extern CAccountLinkedUserDataMgr* g_accountLinkedUserDataMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAccountLinkedUserDataMgr) == 92);
#endif

#endif
