#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMDATAMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMDATAMGR_H

#include <cstdint>
#include <string>

class LuaScript;
class CConnection;
class CAsyncWorkMgr;
struct AsyncDiagnostic;
struct EmblemUploadContext;

class CEmblemDataMgr {
public:
  CEmblemDataMgr();
  virtual ~CEmblemDataMgr();
  static bool Initialize();
  static void Shutdown();
  void RefreshUrls(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey);
  void Download(
    std::uint32_t guildId,
    std::uint32_t version,
    std::uint32_t accountId,
    const std::string& accountName,
    const char* token16,
    CConnection& connection,
    CAsyncWorkMgr& manager
  );
  bool Upload(
    std::uint32_t guildId,
    std::string path,
    std::uint32_t accountId,
    const std::string& accountName,
    const char* token16,
    EmblemUploadContext& context,
    CAsyncWorkMgr& manager
  );
  void
  CompleteUpload(AsyncDiagnostic diagnostic, std::string response, std::uint32_t guildId, EmblemUploadContext& context);

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::string m_uploadUrl;
  /* 0020 */ std::string m_downloadUrl;
  /* 0038 */ bool m_uploading;
};

// STATIC: 0x013898D4
// GLOBAL: gl_24355
extern CEmblemDataMgr* g_emblemDataMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEmblemDataMgr) == 60);
#endif

#endif
