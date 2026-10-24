#include "Contents/EmblemDataMgr.h"
#include "Contents/EmblemDataUploadAsyncWork.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cctype>
#include <cstdio>
#include <mmsystem.h>

namespace {
  constexpr char sourceFile[] =
    "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\Emblem\\CEmblemDataUploadAsyncWork.cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005EBF50
// FUNCTION: fn_03978
CEmblemDataUploadAsyncWork::CEmblemDataUploadAsyncWork()
    : NAsyncWork("Emblem_Upload"), m_accountId(0), m_guildId(0), m_diagnostic(Diagnostic(false, "unknwon", 27)),
      m_owner(nullptr), m_context(nullptr) {
}

// OFFSET: 0x005EC350
// FUNCTION: fn_03979
CEmblemDataUploadAsyncWork::~CEmblemDataUploadAsyncWork() = default;

// OFFSET: 0x005EC6F0
// FUNCTION: fn_03981
void CEmblemDataUploadAsyncWork::Reset() {
}

// OFFSET: 0x005EC450
// FUNCTION: fn_03980
void CEmblemDataUploadAsyncWork::Bind(
  std::string url,
  std::uint32_t accountId,
  std::string token,
  std::string accountName,
  std::uint32_t guildId,
  std::string path,
  CEmblemDataMgr& owner,
  EmblemUploadContext& context
) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_url = url;
  this->m_accountId = accountId;
  this->m_token = token;
  this->m_accountName = accountName;
  this->m_guildId = guildId;
  this->m_path = path;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 54);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_owner = &owner;
  this->m_context = &context;
}

// OFFSET: 0x005EC7C0
// FUNCTION: fn_03983
bool CEmblemDataUploadAsyncWork::Execute() {
  char account[64] = {}, guild[64] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  _snprintf_s(guild, sizeof(guild), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_guildId));
  std::string name = ToUtf8(this->m_accountName);
  std::string path = ToUtf8(this->m_path);
  std::string extension = this->m_path.substr(this->m_path.size() - 3, this->m_path.size());

  for (char& value : extension) {
    value = static_cast<char>(std::toupper(static_cast<unsigned char>(value)));
  }

  extension = ToUtf8(extension);
  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", name, 4);
  handler.AddField("GDID", guild, 4);
  handler.AddField("ImgType", extension, 4);
  handler.AddField("Img", path, 10);

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, "good", 100);
    this->m_diagnostic.MoveFrom(diagnostic);
  } else {
    AsyncDiagnostic diagnostic = Diagnostic(false, "fail", 109);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x005EC700
// FUNCTION: fn_03982
void CEmblemDataUploadAsyncWork::Complete() {
  this->m_owner->CompleteUpload(this->m_diagnostic, this->m_response, this->m_guildId, *this->m_context);
}
