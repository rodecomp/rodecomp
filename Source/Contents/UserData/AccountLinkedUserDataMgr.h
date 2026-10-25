#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATAMGR_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATAMGR_H

#include <cstdint>
#include <string>

struct AsyncDiagnostic;
struct EmotionHotkeys;
struct AccountConfigLoad;
class LuaScript;
class CRenderSystemBase;
class CAsyncWorkMgr;

class CAccountLinkedUserDataMgr {
public:
  CAccountLinkedUserDataMgr();
  virtual ~CAccountLinkedUserDataMgr();
  static bool Initialize();
  static void Shutdown();
  std::string SaveUrl() const;
  std::string LoadUrl() const;
  void RefreshUrls(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey);

  void CompleteLoad(
    AsyncDiagnostic diagnostic, std::uint32_t accountId, std::string accountName, std::string response, EmotionHotkeys& emotions, LuaScript& script,
    std::uint32_t persistenceAccountId, CRenderSystemBase* renderSystem
  );
  void Load(std::uint32_t accountId, std::string accountName, CAsyncWorkMgr& workManager, AccountConfigLoad& context);
  void Reset(EmotionHotkeys& emotions, LuaScript& script, const char* host, const char* port, const char* overrideKey);
  void CompleteSave();
  void Save(
    EmotionHotkeys& emotions, LuaScript& script, std::uint32_t persistenceAccountId, CRenderSystemBase* renderSystem, const char* token16,
    CAsyncWorkMgr& workManager
  );

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::uint32_t m_accountId;
  /* 000C */ std::string m_accountName;
  /* 0024 */ bool m_loaded;
  /* 0028 */ std::uint32_t unknown_0028;
  /* 002C */ std::string m_saveUrl;
  /* 0044 */ std::string m_loadUrl;
};

// STATIC: 0x01389760
// GLOBAL: gl_24351
extern CAccountLinkedUserDataMgr* g_accountLinkedUserDataMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAccountLinkedUserDataMgr) == 92);
#endif

#endif
