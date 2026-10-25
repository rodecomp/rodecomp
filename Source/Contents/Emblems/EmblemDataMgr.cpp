#include "Contents/UserData/AccountService.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"

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
    this->m_uploadUrl = base + "/emblem/upload";
    this->m_downloadUrl = base + "/emblem/download";
  }
}

// OFFSET: 0x005EB260
// FUNCTION: fn_03972
void CEmblemDataMgr::Download(
  std::uint32_t guildId, std::uint32_t version, std::uint32_t accountId, const std::string& accountName, const char* token16, CConnection& connection,
  CAsyncWorkMgr& manager
) {
  const std::string token(token16, 16);
  manager.DownloadEmblem(this->m_downloadUrl, accountId, token, accountName, guildId, version, connection);
}

// OFFSET: 0x005EB410
// FUNCTION: fn_03973
bool CEmblemDataMgr::Upload(
  std::uint32_t guildId, std::string path, std::uint32_t accountId, const std::string& accountName, const char* token16, EmblemUploadContext& context,
  CAsyncWorkMgr& manager
) {
  if (this->m_uploading) {
    return false;
  }

  const std::string token(token16, 16);
  manager.UploadEmblem(this->m_uploadUrl, accountId, token, accountName, guildId, path, *this, context);

  return true;
}
