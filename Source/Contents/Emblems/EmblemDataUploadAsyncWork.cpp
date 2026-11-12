#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Emblems/EmblemDataUploadAsyncWork.h"
#include "Network/Http/CURLMgr.h"
#include "Network/Http/CURLPostHander.h"
#include "Network/TextEncoding.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <cctype>
#include <windows.h>
#include <timeapi.h>

namespace {
  constexpr char sourceFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\Emblem\\CEmblemDataUploadAsyncWork.cpp";

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// GLOBAL: gl_25610
const char* g_emblemDataUploadPoolName = DecryptName<0x2FF3C5036D070127ULL>(CRYPT_BYTES(14, "Emblem_Upload", 0x2FF3C5036D070127ULL));

// OFFSET: 0x005EBF50
// FUNCTION: fn_03978
CEmblemDataUploadAsyncWork::CEmblemDataUploadAsyncWork()
    : NAsyncWork(g_emblemDataUploadPoolName), m_accountId(0), m_guildId(0),
      m_diagnostic(false, std::string(DecryptName<0x93E7A3A1F587CDC5ULL>({0xB0, 0xA3, 0xEC, 0x9B, 0xD6, 0xCC, 0x89, 0x93})), sourceFile, 27) {
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
  constexpr std::array<std::uint8_t, 8> diagnosticSourceArray = CRYPT_NAME("unknwon", 0xC52FB5FBB94FF917ULL);
  const std::uint8_t (&diagnosticSource)[8] = diagnosticSourceArray._Elems;
  this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0xC52FB5FBB94FF917ULL>(diagnosticSource)), sourceFile, 54);
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
  constexpr std::array<std::uint8_t, 4> aidArray = CRYPT_NAME("AID", 0x812BE72BCD5707B7ULL);
  const std::uint8_t (&aid)[4] = aidArray._Elems;
  handler.AddField(DecryptName<0x812BE72BCD5707B7ULL>(aid), account, 4);
  constexpr std::array<std::uint8_t, 10> authTokenArray = CRYPT_NAME("AuthToken", 0x59278F63D79F3DA1ULL);
  const std::uint8_t (&authToken)[10] = authTokenArray._Elems;
  handler.AddField(DecryptName<0x59278F63D79F3DA1ULL>(authToken), this->m_token, 4);
  const std::uint8_t worldName[] = CRYPT_BYTES(10, "WorldName", 0x1D6D75EDE7CD311FULL);
  handler.AddField(DecryptName<0x1D6D75EDE7CD311FULL>(worldName), name, 4);
  constexpr std::array<std::uint8_t, 5> gdidArray = CRYPT_NAME("GDID", 0x3375B1C7DBB5FD55ULL);
  const std::uint8_t (&gdid)[5] = gdidArray._Elems;
  handler.AddField(DecryptName<0x3375B1C7DBB5FD55ULL>(gdid), guild, 4);
  const std::uint8_t imgType[] = CRYPT_BYTES(8, "ImgType", 0x0F0F8B6B8F9D19D1ULL);
  handler.AddField(DecryptName<0x0F0F8B6B8F9D19D1ULL>(imgType), imageType, 4);
  const std::uint8_t img[] = CRYPT_BYTES(4, "Img", 0xD98F03A52D35636DULL);
  handler.AddField(DecryptName<0xD98F03A52D35636DULL>(img), path, 10);

  if (handler.Perform() == 0) {
    this->m_response = g_curlMgr->FromUtf8(handler.Response());
    const std::uint8_t good[] = CRYPT_BYTES(5, "good", 0x31D7C1EDA345E36DULL);
    this->m_diagnostic = AsyncDiagnostic(true, std::string(DecryptName<0x31D7C1EDA345E36DULL>(good)), sourceFile, 100);
  } else {
    const std::uint8_t fail[] = CRYPT_BYTES(5, "fail", 0xD3755DD95F47A909ULL);
    this->m_diagnostic = AsyncDiagnostic(false, std::string(DecryptName<0xD3755DD95F47A909ULL>(fail)), sourceFile, 109);
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
