#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMDATADOWNLOADASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMDATADOWNLOADASYNCWORK_H

#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class CConnection;

class CEmblemDataDownloadAsyncWork : public NAsyncWork {
public:
  CEmblemDataDownloadAsyncWork();
  ~CEmblemDataDownloadAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::string url, std::uint32_t accountId, std::string token, std::string accountName, std::uint32_t guildId, std::uint32_t version, CConnection& connection
  );

protected:
  /* 0028 */ std::string m_url;
  /* 0040 */ std::uint32_t m_accountId;
  /* 0044 */ std::string m_token;
  /* 005C */ std::string m_accountName;
  /* 0074 */ std::uint32_t m_guildId;
  /* 0078 */ std::uint32_t m_version;
  /* 007C */ std::string m_response;
  /* 0094 */ int m_responseCode;
  /* 0098 */ AsyncDiagnostic m_diagnostic;
  CConnection* m_connection;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEmblemDataDownloadAsyncWork) == 212);
#endif

#endif
