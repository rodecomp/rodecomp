#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMDATAMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMDATAMGR_H

#include <cstdint>
#include <string>

class LuaScript;
class CConnection;
class CAsyncWorkMgr;
struct AsyncDiagnostic;

class CEmblemDataMgr {
public:
  CEmblemDataMgr();
  virtual ~CEmblemDataMgr();
  static bool Initialize();
  static void Shutdown();
  void RefreshUrls();
  // INLINED
  __declspec(noinline) void RefreshUrls(LuaScript&, LuaScript&, const char*, const char*, const char*) {
    this->RefreshUrls();
  }
  void Download(std::uint32_t guildId, std::uint32_t version);
  bool Upload(std::uint32_t guildId, std::string path);
  void CompleteUpload(AsyncDiagnostic diagnostic, std::string response, std::uint32_t guildId);
  void CompleteDownload(AsyncDiagnostic diagnostic, int responseCode, std::string response, std::uint32_t guildId, std::uint32_t version);

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::string m_uploadUrl;
  /* 0020 */ std::string m_downloadUrl;
  /* 0038 */ bool m_uploading;
};

extern CEmblemDataMgr* g_emblemDataMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEmblemDataMgr) == 60);
#endif

#endif
