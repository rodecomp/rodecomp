#include "Client/Session.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Script/LuaScript.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/AccountService.h"
#include "Contents/UserData/AccountUserData.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "Contents/Hotkeys/UserHotkeyMgr.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Cipher/XorBytes.h"

#include <json/reader.h>
#include <json/writer.h>
#include <cstddef>
#include <new>

namespace {
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedText {
  public:
    // INLINED
    explicit EncryptedText(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    // INLINED
    ~EncryptedText() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    // INLINED
    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    /* 0000 */ std::uint8_t m_text[Length];
    bool m_encrypted;
  };

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptText(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedText<Length, Key> text(source);
    return text.Text();
  }

  // INLINED
  const char* UnknownAccountName() {
    const std::uint8_t source[] = {0x6A, 0x5F, 0xA6, 0x89, 0x82, 0x02, 0x03, 0x1D};
    return DecryptText<0x1D6D75EDE7CD311FULL>(source);
  }
}

// STATIC: 0x01389760
// GLOBAL: gl_24351
CAccountLinkedUserDataMgr* g_accountLinkedUserDataMgr;

// INLINED
CAccountLinkedUserDataMgr::CAccountLinkedUserDataMgr()
    : m_accountId(0), m_accountName(UnknownAccountName()), m_loaded(false), unknown_0028(0), m_saveUrl(""), m_loadUrl("") {
  this->m_enabled = true;
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
  AsyncDiagnostic diagnostic, std::uint32_t accountId, std::string accountName, std::string response
) {
  if (diagnostic.success && this->m_accountId == 0) {
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(response, root, true)) {
      const int type = root["Type"].asInt();
      if (type == 1) {
        LoadWhisperBlockList(root["data"]);
        g_session.m_emotionHotkeys.Load(root["data"]);
        g_userHotkeyMgr->LoadV2(*g_session.m_hotkeyScript, root["data"]);
      }

      if (type == 1 || type == 2) {
        g_whisperBlockMgr->m_tracking = true;
        g_userHotkeyMgr->m_loaded = true;
        this->m_accountId = accountId;
        this->m_accountName = accountName;
        this->m_loaded = true;
        g_session.m_emotionHotkeys.Save(g_session.m_aid, GetRenderSystemHolder()->system);
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
void CAccountLinkedUserDataMgr::Reset(LuaScript& script, const char* host, const char* port, const char* overrideKey) {
  this->m_accountId = 0;
  this->m_accountName = "unknown";
  this->m_loaded = false;
  g_whisperBlockMgr->Reset();
  g_session.m_emotionHotkeys.saved.clear();
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
void CAccountLinkedUserDataMgr::Save() {
  EmotionHotkeys& emotions = g_session.m_emotionHotkeys;
  emotions.Save(g_session.m_aid, GetRenderSystemHolder()->system);
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

  if (g_userHotkeyMgr->HasChanges(*g_session.m_hotkeyScript)) {
    g_userHotkeyMgr->Export(*g_session.m_hotkeyScript, data);
  }

  if (!data.isNull()) {
    root["data"] = data;
    Json::StyledWriter writer;
    const std::string response = writer.write(root);
    g_asyncWorkMgr->SaveAccount(this->m_accountId, this->m_accountName, this->SaveUrl(), std::string(reinterpret_cast<const char*>(&g_session.unknown_807C[0x08]), 16), response);
    ++this->unknown_0028;
  }
}

// OFFSET: 0x005BCD80
// FUNCTION: fn_03468
void CAccountLinkedUserDataMgr::Load(std::uint32_t accountId, std::string accountName) {
  g_session.m_emotionHotkeys.LoadDefaults(g_session.m_aid);

  if (this->m_accountId == accountId && this->m_accountName == accountName) {
    return;
  }

  const std::string url = this->LoadUrl();
  g_asyncWorkMgr->LoadAccount(accountId, accountName, url);
}
