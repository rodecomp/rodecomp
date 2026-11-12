#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Emblems/EmblemDataUploadAsyncWork.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <cctype>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\Emblem\\CEmblemDataUploadAsyncWork.cpp";

  // INLINED
  }

// GLOBAL: gl_25610
const char* g_emblemDataUploadPoolName = OBFUSCATE_BYTES_TEXT("Emblem_Upload", 0x2FF3C5036D070127ULL);

// OFFSET: 0x005EBF50
// FUNCTION: fn_03978
CEmblemDataUploadAsyncWork::CEmblemDataUploadAsyncWork()
    : NAsyncWork(g_emblemDataUploadPoolName), m_accountId(0), m_guildId(0),
      m_diagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0x93E7A3A1F587CDC5ULL)), sourceFile, 27) {
}

// INLINED
CEmblemDataUploadAsyncWork::~CEmblemDataUploadAsyncWork() = default;

// OFFSET: 0x005EC6F0
// FUNCTION: fn_03981
void CEmblemDataUploadAsyncWork::Reset() {
}

// OFFSET: 0x005EC450
// FUNCTION: fn_03980
void CEmblemDataUploadAsyncWork::Bind(std::string url, std::uint32_t accountId, std::string token, std::string accountName, std::uint32_t guildId, std::string path) {
  this->m_group = timeGetTime();
  this->m_key = 0;
  this->m_url = url;
  this->m_accountId = accountId;
  this->m_token = token;
  this->m_accountName = accountName;
  this->m_guildId = guildId;
  this->m_path = path;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("unknwon", 0xC52FB5FBB94FF917ULL)), sourceFile, 54);
}

// OFFSET: 0x005EC7C0
// FUNCTION: fn_03983
bool CEmblemDataUploadAsyncWork::Execute() {
  char account[64] = {};
  StringFormat(account, sizeof(account), _TRUNCATE, "%lu", this->m_accountId);
  char guild[64] = {};
  StringFormat(guild, sizeof(guild), _TRUNCATE, "%lu", this->m_guildId);
  std::string name = g_curlMgr->ToUtf8(this->m_accountName);
  std::string path = g_curlMgr->ToUtf8(this->m_path);
  std::string extension = this->m_path.substr(this->m_path.size() - 3, this->m_path.size());

  for (char& value : extension) {
    value = static_cast<char>(std::toupper(static_cast<unsigned char>(value)));
  }

  std::string imageType = g_curlMgr->ToUtf8(extension);
  CCURLPostHander handler(this->m_url);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AID", 0x812BE72BCD5707B7ULL), account, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("AuthToken", 0x59278F63D79F3DA1ULL), this->m_token, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("WorldName", 0x1D6D75EDE7CD311FULL), name, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("GDID", 0x3375B1C7DBB5FD55ULL), guild, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("ImgType", 0x0F0F8B6B8F9D19D1ULL), imageType, 4);
  handler.AddField(OBFUSCATE_BYTES_TEXT("Img", 0xD98F03A52D35636DULL), path, 10);

  if (handler.Perform() == 0) {
    this->m_response = g_curlMgr->FromUtf8(handler.Response());
    this->m_diagnostic = AsyncDiagnostic(true, std::string(OBFUSCATE_BYTES_TEXT("good", 0x31D7C1EDA345E36DULL)), sourceFile, 100);
  } else {
    this->m_diagnostic = AsyncDiagnostic(false, std::string(OBFUSCATE_BYTES_TEXT("fail", 0xD3755DD95F47A909ULL)), sourceFile, 109);
  }

  return false;
}

// OFFSET: 0x005EC700
// FUNCTION: fn_03982
void CEmblemDataUploadAsyncWork::Complete() {
  CEmblemDataMgr* emblems = g_emblemDataMgr;
  emblems->CompleteUpload(this->m_diagnostic, this->m_response, this->m_guildId);
}

template void std::deque<CEmblemDataUploadAsyncWork*>::push_back(CEmblemDataUploadAsyncWork*&&);
