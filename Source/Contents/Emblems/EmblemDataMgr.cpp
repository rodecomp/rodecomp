#include "Contents/UserData/AccountService.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"

#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <new>

namespace {
template <std::size_t Length>
class EncryptedSource {
public:
  // INLINED
  constexpr explicit EncryptedSource(const std::uint8_t (&source)[Length]) : data{} {
    for (std::size_t index = 0; index < Length; ++index) {
      this->data[index] = static_cast<char>(source[index]);
    }
  }

  char data[Length];
};

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const EncryptedSource<Length>& source) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = static_cast<std::uint8_t>(source.data[index]);
    }
  }

  ~EncryptedName() {
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
  std::uint8_t m_text[Length];
  bool m_encrypted;
};
}

// STATIC: 0x013898D4
// GLOBAL: gl_24355
CEmblemDataMgr* g_emblemDataMgr;

// INLINED
CEmblemDataMgr::CEmblemDataMgr() : m_enabled(true), m_uploading(false) {
}

// INLINED
CEmblemDataMgr::~CEmblemDataMgr() = default;

// OFFSET: 0x005EB150
// FUNCTION: fn_03970
bool CEmblemDataMgr::Initialize() {
  if (g_emblemDataMgr) {
    return false;
  }

  g_emblemDataMgr = new (std::nothrow) CEmblemDataMgr;

  if (!g_emblemDataMgr) {
    return false;
  }

  if (!g_emblemDataMgr->m_enabled) {
    delete g_emblemDataMgr;
    g_emblemDataMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x005EB240
// FUNCTION: fn_03971
void CEmblemDataMgr::Shutdown() {
  if (g_emblemDataMgr) {
    delete g_emblemDataMgr;
    g_emblemDataMgr = nullptr;
  }
}

// OFFSET: 0x005EB580
// FUNCTION: fn_03974
void CEmblemDataMgr::RefreshUrls(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey) {
  const std::string base = ResolveAccountServiceBase(settings, tables, host, port, overrideKey);

  if (!base.empty()) {
    this->m_uploadUrl = base + "/emblem/upload";
    this->m_downloadUrl = base + "/emblem/download";
  }
}

// OFFSET: 0x005EB260
// FUNCTION: fn_03972
void CEmblemDataMgr::Download(std::uint32_t guildId, std::uint32_t version) {
  const std::string token(reinterpret_cast<const char*>(&g_session.webServiceLoginData[0x08]), 16);
  g_asyncWorkMgr->DownloadEmblem(
    this->m_downloadUrl, g_session.m_aid, token, g_session.m_accountName, guildId, version, GetRagConnection()
  );
}

// OFFSET: 0x005EB410
// FUNCTION: fn_03973
bool CEmblemDataMgr::Upload(std::uint32_t guildId, std::string path) {
  if (this->m_uploading == true) {
    return false;
  }

  std::string token;
  token.assign(reinterpret_cast<const char*>(&g_session.webServiceLoginData[0x08]), 16);
  g_asyncWorkMgr->UploadEmblem(this->m_uploadUrl, g_session.m_aid, token, g_session.m_accountName, guildId, path);

  return true;
}
