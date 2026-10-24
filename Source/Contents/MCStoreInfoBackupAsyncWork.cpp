#include "Contents/MCStoreInfoBackup.h"
#include "Contents/MCStoreInfoBackupAsyncWork.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <mmsystem.h>

namespace {
  constexpr char loadSource[] =
    "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\MCStoreBackUpData\\CMCStoreInfoBackupLoadAsyncWork."
    "cpp";
  constexpr char saveSource[] =
    "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\MCStoreBackUpData\\CMCStoreInfoBackupSaveAsyncWork."
    "cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, const char* file, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, file, &line);
  }
}

// OFFSET: 0x00607B10
// FUNCTION: fn_04241
CMCStoreInfoBackupLoadAsyncWork::CMCStoreInfoBackupLoadAsyncWork()
    : NAsyncWork("MerchantStoreInformation_Load"), m_accountId(0), m_characterId(0),
      m_diagnostic(Diagnostic(false, "unknwon", loadSource, 28)), m_type(0), m_owner(nullptr), m_context(nullptr) {
}

// OFFSET: 0x00607EF0
// FUNCTION: fn_04242
CMCStoreInfoBackupLoadAsyncWork::~CMCStoreInfoBackupLoadAsyncWork() = default;

// OFFSET: 0x00608270
// FUNCTION: fn_04244
void CMCStoreInfoBackupLoadAsyncWork::Reset() {
}

// OFFSET: 0x00607FE0
// FUNCTION: fn_04243
void CMCStoreInfoBackupLoadAsyncWork::Bind(
  std::string url,
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string accountName,
  std::string token,
  int type,
  CMCStoreInfoBackup& owner,
  StoreBackupContext& context
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_response.clear();
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", loadSource, 55);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_type = type;
  this->m_owner = &owner;
  this->m_context = &context;
}

// OFFSET: 0x00608350
// FUNCTION: fn_04246
bool CMCStoreInfoBackupLoadAsyncWork::Execute() {
  char account[64] = {}, character[64] = {}, type[64] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  _snprintf_s(character, sizeof(character), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_characterId));
  std::string name = ToUtf8(this->m_accountName);
  _snprintf_s(type, sizeof(type), _TRUNCATE, "%d", this->m_type);

  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("GID", character, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("Type", type, 4);
  handler.AddField("WorldName", name, 4);

  if (handler.Perform() == 0) {
    this->m_response = handler.Response();
    AsyncDiagnostic diagnostic = Diagnostic(true, "good", loadSource, 115);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  return false;
}

// OFFSET: 0x00608280
// FUNCTION: fn_04245
void CMCStoreInfoBackupLoadAsyncWork::Complete() {
  this->m_owner->CompleteLoad(
    this->m_diagnostic, this->m_accountId, this->m_characterId, this->m_accountName, this->m_response, *this->m_context
  );
}

// OFFSET: 0x00608D60
// FUNCTION: fn_04247
CMCStoreInfoBackupSaveAsyncWork::CMCStoreInfoBackupSaveAsyncWork()
    : NAsyncWork("MerchantStoreInformation_Save"), m_accountId(0), m_characterId(0),
      m_diagnostic(Diagnostic(false, "unknwon", saveSource, 30)), m_type(0), m_owner(nullptr), m_context(nullptr) {
}

// OFFSET: 0x00609170
// FUNCTION: fn_04248
CMCStoreInfoBackupSaveAsyncWork::~CMCStoreInfoBackupSaveAsyncWork() = default;

// OFFSET: 0x00609520
// FUNCTION: fn_04250
void CMCStoreInfoBackupSaveAsyncWork::Reset() {
}

// OFFSET: 0x00609270
// FUNCTION: fn_04249
void CMCStoreInfoBackupSaveAsyncWork::Bind(
  std::string url,
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string accountName,
  std::string token,
  std::string data,
  int type,
  CMCStoreInfoBackup& owner,
  StoreBackupContext& context
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_accountName = accountName;
  this->m_url = url;
  this->m_token = token;
  this->m_data = data;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", saveSource, 57);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_type = type;
  this->m_owner = &owner;
  this->m_context = &context;
}

// OFFSET: 0x006095F0
// FUNCTION: fn_04252
bool CMCStoreInfoBackupSaveAsyncWork::Execute() {
  char account[64] = {}, character[64] = {}, type[64] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_accountId));
  _snprintf_s(character, sizeof(character), _TRUNCATE, "%d", static_cast<std::int32_t>(this->m_characterId));
  _snprintf_s(type, sizeof(type), _TRUNCATE, "%d", this->m_type);
  std::string name = ToUtf8(this->m_accountName);

  CCURLPostHander handler(this->m_url);
  handler.AddField("AID", account, 4);
  handler.AddField("GID", character, 4);
  handler.AddField("AuthToken", this->m_token, 4);
  handler.AddField("WorldName", name, 4);
  handler.AddField("Type", type, 4);
  handler.AddField("data", this->m_data, 4);

  const bool success = handler.Perform() == 0;
  AsyncDiagnostic diagnostic = Diagnostic(success, success ? "good" : "fail", saveSource, success ? 122 : 112);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_response = handler.Response();
  return false;
}

// OFFSET: 0x00609530
// FUNCTION: fn_04251
void CMCStoreInfoBackupSaveAsyncWork::Complete() {
  this->m_owner->CompleteSave(this->m_diagnostic, this->m_response, *this->m_context);
}
