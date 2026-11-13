#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Client/Session/Session.h"
#include "Network/Connection/Connection.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Utility/Cipher/EncryptedName.h"

#include <cstddef>
#include <cstdint>
#include <new>

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
void CEmblemDataMgr::RefreshUrls() {
  const std::string base = g_session.ResolveAccountServiceBase();

  if (!base.empty()) {
    this->m_uploadUrl = base + OBFUSCATE_TEXT("/emblem/upload", 0x2FF3C5036D070127ULL);
    this->m_downloadUrl = base + OBFUSCATE_TEXT("/emblem/download", 0x93E7A3A1F587CDC5ULL);
  }
}

// OFFSET: 0x005EB260
// FUNCTION: fn_03972
void CEmblemDataMgr::Download(std::uint32_t guildId, std::uint32_t version) {
  const std::string token(g_session.m_webAuthToken, 16);
  g_asyncWorkMgr->DownloadEmblem(this->m_downloadUrl, g_session.m_aid, token, g_session.m_accountName, guildId, version);
}

// OFFSET: 0x005EB410
// FUNCTION: fn_03973
bool CEmblemDataMgr::Upload(std::uint32_t guildId, std::string path) {
  if (this->m_uploading == true) {
    return false;
  }

  std::string token;
  token.assign(g_session.m_webAuthToken, 16);
  g_asyncWorkMgr->UploadEmblem(this->m_uploadUrl, g_session.m_aid, token, g_session.m_accountName, guildId, path);

  return true;
}
