#include "Security/NProtectGameGuardMgr.h"
#include "Security/NProtectGameMon.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/MessageTable.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Threading/NSyncObject.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Resource/MSGSTRINGID.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>

// GLOBAL: gl_24848
CNProtectGameGuardMgr* g_nProtectGameGuardMgr;

// STRING: st_16836
static const char g_devClientTag[] = "Dev";

// STRING: st_02591
static const char g_nprotectSource[] =
    "C:\\repositories\\RagnarokClient\\RagnarokClient\\Security Solution\\nProtectGameGuard\\CNProtectGameGuardMgr.cpp";

class CNPGameLib {
public:
  // INLINED
  explicit CNPGameLib(const char* gameName) {
    PreInitNPGameMonA(gameName);
  }

  // INLINED
  int Init() {
    return InitNPGameMon();
  }
};

namespace {

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const char (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::array<char, Length>& source) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// OFFSET: 0x0076C3E0
// FUNCTION: fn_45544
CNProtectGameGuardMgr::~CNProtectGameGuardMgr() {
  void* gameLib = this->m_gameLib;
  if (gameLib) {
    CloseNPGameMon();
    ::operator delete(gameLib, 1);
    this->m_gameLib = nullptr;
  }

  DeleteCriticalSection(&this->m_lock);
}

// OFFSET: 0x0076C670
// FUNCTION: fn_08205
void CNProtectGameGuardMgr::ProcessAuth(int, int) {
}

// OFFSET: 0x0076C6E0
// FUNCTION: fn_08207
void CNProtectGameGuardMgr::Stop() {
}

// OFFSET: 0x0076C930
// FUNCTION: fn_08208
void CNProtectGameGuardMgr::Tick() {
}

// OFFSET: 0x0076CD60
// FUNCTION: fn_08210
int CNProtectGameGuardMgr::Create() {
  return 1;
}

// OFFSET: 0x0076CEA0
// FUNCTION: fn_08212
void CNProtectGameGuardMgr::Destroy() {
  if (g_nProtectGameGuardMgr) {
    delete g_nProtectGameGuardMgr;
    g_nProtectGameGuardMgr = nullptr;
  }
}

// OFFSET: 0x0076CEC0
// FUNCTION: fn_08213
int CNProtectGameGuardMgr::HasError() {
  return 0;
}

// OFFSET: 0x0076CF30
// FUNCTION: fn_08215
std::string CNProtectGameGuardMgr::ErrorMessage() {
  NSyncObject lock(&this->m_lock);
  return this->m_errorMessage;
}

// OFFSET: 0x0076CFB0
// FUNCTION: fn_08216
const char* CNProtectGameGuardMgr::GameName() {
  switch (g_serviceType) {
  case 3: {
    constexpr auto sourceText = CRYPT_NAME("ROCN", 0x7321C3814F8799E1ULL);
    const auto& source = sourceText.data;
    return DecryptName<0x7321C3814F8799E1ULL>(source);
  }
  case 0:
    if (std::strstr(g_session.clientVersion, g_devClientTag) != nullptr) {
      constexpr auto sourceText = CRYPT_NAME("RagnarokKR", 0x7941D76335CFA14FULL);
      const auto& source = sourceText.data;
      return DecryptName<0x7941D76335CFA14FULL>(source);
    } else {
      constexpr auto sourceText = CRYPT_NAME("RagnarokKR", 0x0F15B16B5BAFD57FULL);
      const auto& source = sourceText.data;
      return DecryptName<0x0F15B16B5BAFD57FULL>(source);
    }
  case 2: {
    constexpr auto sourceText = CRYPT_NAME("RagnarokJP", 0x7DE349D717D7F123ULL);
    const auto& source = sourceText.data;
    return DecryptName<0x7DE349D717D7F123ULL>(source);
  }
  case 12: {
    constexpr auto sourceText = CRYPT_NAME("RagnarokBR", 0x9FED7DA189490D5DULL);
    const auto& source = sourceText.data;
    return DecryptName<0x9FED7DA189490D5DULL>(source);
  }
  case 7: {
    constexpr auto sourceText = CRYPT_NAME("RagnarokPH", 0x452FEF0D7FC95565ULL);
    const auto& source = sourceText.data;
    return DecryptName<0x452FEF0D7FC95565ULL>(source);
  }
  case 4: {
    constexpr auto sourceText = CRYPT_NAME("ROTW", 0xF3CB6F7711A71B6DULL);
    const auto& source = sourceText.data;
    return DecryptName<0xF3CB6F7711A71B6DULL>(source);
  }
  default: {
    constexpr auto sourceText = CRYPT_NAME("Ragnarok", 0x839B27DF8F5967B3ULL);
    const auto& source = sourceText.data;
    return DecryptName<0x839B27DF8F5967B3ULL>(source);
  }
  }
}

// OFFSET: 0x0076D8A0
// FUNCTION: fn_08217
void CNProtectGameGuardMgr::SetHwnd(HWND) {
}

// OFFSET: 0x0076DF40
// FUNCTION: fn_08220
void CNProtectGameGuardMgr::IncrementCounter() {
}

// OFFSET: 0x0076DFA0
// FUNCTION: fn_08221
void CNProtectGameGuardMgr::SetError(const char*, int, std::string message) {
  NSyncObject lock(&this->m_lock);
  if (!this->m_hasError) {
    this->m_hasError = true;
    this->m_errorMessage = message;
  }
}

// OFFSET: 0x0076E040
// FUNCTION: fn_08222
void CNProtectGameGuardMgr::Send(const char*) {
}

// OFFSET: 0x0076D910
// FUNCTION: fn_08219
int __stdcall GameGuardCallback(int code, const int* packet) {
  switch (code) {
  case 0x3F3: {
    CNProtectGameGuardMgr* manager = g_nProtectGameGuardMgr;
    manager->SetError(g_nprotectSource, 0x136, std::string(FormatString(
      []() {
        constexpr auto sourceText = CRYPT_NAME("%s(%d)", 0x0F41EBBDA32D1B17ULL);
        return DecryptName<0x0F41EBBDA32D1B17ULL>(sourceText.data);
      }(), LookupMessage(MSI_SPEEDHACK_DETECT), code)));
    return 0;
  }
  case 0x3E9:
  case 0x3EA: {
    CNProtectGameGuardMgr* manager = g_nProtectGameGuardMgr;
    constexpr auto sourceText = CRYPT_NAME("unknown(%d)", 0x2D51E3B7236175EDULL);
    const auto& source = sourceText.data;
    manager->SetError(g_nprotectSource, 0x13B, std::string(FormatString(DecryptName<0x2D51E3B7236175EDULL>(source), code)));
    return 0;
  }
  case 0x3F6: {
    CNProtectGameGuardMgr* manager = g_nProtectGameGuardMgr;
    manager->SetError(g_nprotectSource, 0x13F, std::string(FormatString(
      []() {
        constexpr auto sourceText = CRYPT_NAME("%s(%d)", 0x176F138D5DD7F365ULL);
        return DecryptName<0x176F138D5DD7F365ULL>(sourceText.data);
      }(), LookupMessage(MSI_INIT_ERROR), code)));
    return 0;
  }
  case 0x3F4:
  case 0x3F5: {
    CNProtectGameGuardMgr* manager = g_nProtectGameGuardMgr;
    manager->SetError(g_nprotectSource, 0x14B, std::string(LookupMessage(MSI_USA_PERSONALINFO_DROP)));
    return 0;
  }
  case 0x3F7: {
    CNProtectGameGuardMgr* manager = g_nProtectGameGuardMgr;
    manager->SetError(g_nprotectSource, 0x151, std::string(FormatString(
      []() {
        constexpr auto sourceText = CRYPT_NAME("%s(%d)", 0xFBD599E117C547C1ULL);
        return DecryptName<0xFBD599E117C547C1ULL>(sourceText.data);
      }(), LookupMessage(MSI_GAMEHACK_DOUBT), code)));
    return 0;
  }
  case 0x3FB: {
    CNProtectGameGuardMgr* manager = g_nProtectGameGuardMgr;
    const unsigned char* data = reinterpret_cast<const unsigned char*>(packet + 2);
    CNProtectGameGuardMgr::lcCSAuthToken token(packet[0], std::vector<unsigned char>(data, data + packet[1]));
    NSyncObject lock(&manager->m_lock);
    manager->m_tokens.push_back(token);
    break;
  }
  default:
    break;
  }
  return 1;
}

// OFFSET: 0x0076C9D0
// FUNCTION: fn_08209
bool CNProtectGameGuardMgr::Initialize() {
  CNPGameLib* gameLib = new (std::nothrow) CNPGameLib(GameName());
  this->m_gameLib = gameLib;
  if (gameLib == nullptr) {
    return false;
  }

  const int status = gameLib->Init();
  if (status != 0x755) {
    char message[0x400] = {};
    constexpr std::array<char, 21> bodySource = CRYPT_NAME("GameGuard Error: %lu", 0xCFF3899D95AF355FULL);
    StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), DecryptName<0xCFF3899D95AF355FULL>(bodySource), status);

    char title[0x100] = {};
    constexpr std::array<char, 22> titleSource = CRYPT_NAME("GameGuard Error : %lu", 0xE52DF30DCBAF7125ULL);
    StringFormat(title, sizeof(title), static_cast<std::size_t>(-1), DecryptName<0xE52DF30DCBAF7125ULL>(titleSource), status);

    constexpr std::array<char, 10> tagSource = CRYPT_NAME("ApplyNPGE", 0x4F99DF03C3EF31B7ULL);
    if (std::strstr(g_commandLine, DecryptName<0x4F99DF03C3EF31B7ULL>(tagSource)) == nullptr) {
      MessageBoxA(hWnd, message, title, 0);
      return false;
    }
    WriteMyLog(g_nprotectSource, 0x284, title);
  }

  SetCallbackToGameMon(GameGuardCallback);
  return true;
}
