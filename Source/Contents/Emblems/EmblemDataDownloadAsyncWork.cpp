#include "Contents/Emblems/EmblemDataDownloadAsyncWork.h"
#include "Contents/Emblems/EmblemDownload.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\Emblem\\CEmblemDataDownloadAsyncWork.cpp";

  // INLINED
  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005E9C00
// FUNCTION: fn_03963
CEmblemDataDownloadAsyncWork::CEmblemDataDownloadAsyncWork()
    : NAsyncWork("Emblem_Download"), m_accountId(0), m_guildId(0), m_version(0), m_responseCode(500), m_diagnostic(Diagnostic(false, "unknwon", 29)),
      m_connection(nullptr) {
}

// INLINED
CEmblemDataDownloadAsyncWork::~CEmblemDataDownloadAsyncWork() = default;

// OFFSET: 0x005EA350
// FUNCTION: fn_03966
void CEmblemDataDownloadAsyncWork::Reset() {
}

// OFFSET: 0x005EA0D0
// FUNCTION: fn_03965
void CEmblemDataDownloadAsyncWork::Bind(
  std::string url, std::uint32_t accountId, std::string token, std::string accountName, std::uint32_t guildId, std::uint32_t version, CConnection& connection
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_url = url;
  this->m_accountId = accountId;
  this->m_token = token;
  this->m_accountName = accountName;
  this->m_guildId = guildId;
  this->m_version = version;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 57);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_connection = &connection;
}

// OFFSET: 0x005EA430
// FUNCTION: fn_03968
bool CEmblemDataDownloadAsyncWork::Execute() {
  char account[64] = {}, guild[64] = {}, version[10] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  _snprintf_s(guild, sizeof(guild), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_guildId));
  _snprintf_s(version, sizeof(version), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_version));
  std::string name = ToUtf8(this->m_accountName);

  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", name, 4);
  handler.AddField("GDID", guild, 4);
  handler.AddField("Version", version, 4);

  if (handler.Perform() == 0) {
    this->m_responseCode = static_cast<int>(handler.ResponseCode());
    this->m_response = handler.Response();
    AsyncDiagnostic diagnostic = Diagnostic(true, "good", 96);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    AsyncDiagnostic diagnostic = Diagnostic(false, "fail", 105);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005EA360
// FUNCTION: fn_03967
void CEmblemDataDownloadAsyncWork::Complete() {
  CompleteEmblemDownload(this->m_diagnostic, this->m_responseCode, this->m_response, this->m_guildId, this->m_version, *this->m_connection);
}
