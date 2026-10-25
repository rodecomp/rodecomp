#include "Contents/UserData/AccountConfigLoad.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/AccountService.h"
#include "Contents/UserData/AccountUserData.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "Contents/Hotkeys/UserHotkeyMgr.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"

#include <json/reader.h>
#include <json/writer.h>
#include <new>

// STATIC: 0x01389760
// GLOBAL: gl_24351
CAccountLinkedUserDataMgr* g_accountLinkedUserDataMgr;

// INLINED
CAccountLinkedUserDataMgr::CAccountLinkedUserDataMgr() : m_enabled(true), m_accountId(0), m_accountName("unknown"), m_loaded(false), unknown_0028(0) {
}

// INLINED
CAccountLinkedUserDataMgr::~CAccountLinkedUserDataMgr() = default;

// OFFSET: 0x005BBF50
// FUNCTION: fn_03459
bool CAccountLinkedUserDataMgr::Initialize() {
  if (g_accountLinkedUserDataMgr) {
    return false;
  }

  g_accountLinkedUserDataMgr = new (std::nothrow) CAccountLinkedUserDataMgr;
  if (!g_accountLinkedUserDataMgr) {
    return false;
  }

  if (!g_accountLinkedUserDataMgr->m_enabled) {
    delete g_accountLinkedUserDataMgr;
    g_accountLinkedUserDataMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x005BC150
// FUNCTION: fn_03460
void CAccountLinkedUserDataMgr::Shutdown() {
  if (g_accountLinkedUserDataMgr) {
    delete g_accountLinkedUserDataMgr;
    g_accountLinkedUserDataMgr = nullptr;
  }
}

// OFFSET: 0x005BC170
// FUNCTION: fn_03461
std::string CAccountLinkedUserDataMgr::LoadUrl() const {
  return this->m_loadUrl;
}

// OFFSET: 0x005BC1D0
// FUNCTION: fn_03462
std::string CAccountLinkedUserDataMgr::SaveUrl() const {
  return this->m_saveUrl;
}

// OFFSET: 0x005BC520
// FUNCTION: fn_03465
void CAccountLinkedUserDataMgr::CompleteLoad(
  AsyncDiagnostic diagnostic, std::uint32_t accountId, std::string accountName, std::string response, EmotionHotkeys& emotions, LuaScript& script,
  std::uint32_t persistenceAccountId, CRenderSystemBase* renderSystem
) {
  if (diagnostic.success && this->m_accountId == 0) {
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(response, root, true)) {
      const int type = root["Type"].asInt();
      if (type == 1) {
        LoadWhisperBlockList(root["data"]);
        emotions.Load(root["data"]);
        g_userHotkeyMgr->LoadV2(script, root["data"]);
      }

      if (type == 1 || type == 2) {
        g_whisperBlockMgr->m_tracking = true;
        g_userHotkeyMgr->m_loaded = true;
        this->m_accountId = accountId;
        this->m_accountName = accountName;
        this->m_loaded = true;
        emotions.Save(persistenceAccountId, renderSystem);
      }
    }
  }
}

// OFFSET: 0x005BCB10
// FUNCTION: fn_03467
void CAccountLinkedUserDataMgr::RefreshUrls(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey) {
  const std::string base = ResolveAccountServiceBase(settings, tables, host, port, overrideKey);
  if (!base.empty()) {
    this->m_saveUrl = base + "/userconfig/save";
    this->m_loadUrl = base + "/userconfig/load";
  }
}

// OFFSET: 0x005BD170
// FUNCTION: fn_03470
void CAccountLinkedUserDataMgr::Reset(EmotionHotkeys& emotions, LuaScript& script, const char* host, const char* port, const char* overrideKey) {
  this->m_accountId = 0;
  this->m_accountName = "unknown";
  this->m_loaded = false;
  g_whisperBlockMgr->Reset();
  emotions.saved.clear();
  g_userHotkeyMgr->Reset();
  this->RefreshUrls(script, script, host, port, overrideKey);
}

// OFFSET: 0x005BD650
// FUNCTION: fn_03472
void CAccountLinkedUserDataMgr::CompleteSave() {
  --this->unknown_0028;
}

// OFFSET: 0x005BCE50
// FUNCTION: fn_03469
void CAccountLinkedUserDataMgr::Save(
  EmotionHotkeys& emotions, LuaScript& script, std::uint32_t persistenceAccountId, CRenderSystemBase* renderSystem, const char* token16,
  CAsyncWorkMgr& workManager
) {
  emotions.Save(persistenceAccountId, renderSystem);
  if (!this->m_loaded) {
    return;
  }

  Json::Value root;
  Json::Value data;
  if (static_cast<std::int64_t>(g_whisperBlockMgr->m_changes) > 0) {
    Json::Value values;
    g_whisperBlockMgr->Export(values);
    data["WhisperBlockList"] = values;
  }

  if (emotions.HasChanges()) {
    emotions.Export(data);
  }

  if (g_userHotkeyMgr->HasChanges(script)) {
    g_userHotkeyMgr->Export(script, data);
  }

  if (!data.isNull()) {
    root["data"] = data;
    Json::StyledWriter writer;
    const std::string response = writer.write(root);
    workManager.SaveAccount(this->m_accountId, this->m_accountName, this->SaveUrl(), std::string(token16, 16), response);
    ++this->unknown_0028;
  }
}

// OFFSET: 0x005BCD80
// FUNCTION: fn_03468
void CAccountLinkedUserDataMgr::Load(std::uint32_t accountId, std::string accountName, CAsyncWorkMgr& workManager, AccountConfigLoad& context) {
  context.emotions.LoadDefaults(context.persistenceAccountId, context.languagePrefix, context.serviceType, context.language);

  if (this->m_accountId == accountId && this->m_accountName == accountName) {
    return;
  }

  const std::string url = this->LoadUrl();
  workManager.LoadAccount(accountId, accountName, url, context);
}
