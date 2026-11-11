#include "Contents/UserData/AccountService.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Client/Session/Session.h"
#include "Network/Connection/Connection.h"

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
void CEmblemDataMgr::RefreshUrls(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey) {
  const std::string base = ResolveAccountServiceBase(settings, tables, host, port, overrideKey);

  if (!base.empty()) {
    {
      const std::uint8_t source[] = {0x08, 0x64, 0x6A, 0x0F, 0x6F, 0xA0, 0x9E, 0x00, 0x52, 0x71, 0x6B, 0x02, 0x62, 0xA1, 0xF3};
      thread_local EncryptedName<sizeof(source), 0x2FF3C5036D070127ULL> name(source);
      this->m_uploadUrl = base + name.Text();
    }
    {
      const std::uint8_t source[] = {0xEA, 0xA8, 0xEA, 0x97, 0xCD, 0xC6, 0x8A, 0xBC, 0xA1, 0xA2, 0xF0, 0x9B, 0xCD, 0xCC, 0x86, 0xF7, 0xC5};
      thread_local EncryptedName<sizeof(source), 0x93E7A3A1F587CDC5ULL> name(source);
      this->m_downloadUrl = base + name.Text();
    }
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
