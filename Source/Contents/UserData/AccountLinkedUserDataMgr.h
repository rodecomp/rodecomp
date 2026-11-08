#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATAMGR_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATAMGR_H

#include <cstdint>
#include <string>

struct AsyncDiagnostic;
class LuaScript;
class CRenderSystemBase;
class CAsyncWorkMgr;

class CAccountLinkedUserDataMgr {
  friend class CGameMode;
  friend void ShutdownManagers();

public:
  CAccountLinkedUserDataMgr();
  virtual ~CAccountLinkedUserDataMgr();
  static bool Initialize();
  static void Shutdown();
  std::string SaveUrl() const;
  __declspec(noinline) std::string LoadUrl() const;
  __declspec(noinline) void RefreshUrls(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey);

  void CompleteLoad(
    AsyncDiagnostic diagnostic, std::uint32_t accountId, std::string accountName, std::string response
  );
  void Load(std::uint32_t accountId, std::string accountName);
  void Reset(LuaScript& script, const char* host, const char* port, const char* overrideKey);
  void CompleteSave();
  void Save();

protected:
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
