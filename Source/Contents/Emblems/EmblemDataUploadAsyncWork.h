#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMDATAUPLOADASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMDATAUPLOADASYNCWORK_H

#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class CEmblemDataMgr;
struct EmblemUploadContext;

class CEmblemDataUploadAsyncWork : public NAsyncWork {
public:
  CEmblemDataUploadAsyncWork();
  ~CEmblemDataUploadAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::string url, std::uint32_t accountId, std::string token, std::string accountName, std::uint32_t guildId, std::string path, CEmblemDataMgr& owner,
    EmblemUploadContext& context
  );

protected:
  /* 0028 */ std::string m_url;
  /* 0040 */ std::uint32_t m_accountId;
  /* 0044 */ std::string m_token;
  /* 005C */ std::string m_accountName;
  /* 0074 */ std::uint32_t m_guildId;
  /* 0078 */ std::string m_path;
  /* 0090 */ std::string m_response;
  /* 00A8 */ AsyncDiagnostic m_diagnostic;
  CEmblemDataMgr* m_owner;
  EmblemUploadContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEmblemDataUploadAsyncWork) == 232);
#endif

#endif
