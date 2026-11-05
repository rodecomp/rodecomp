#include "Security/NProtectGameGuardMgr.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/Launcher.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Threading/NSyncObject.h"

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

void sub_E92420();
void sub_E929B0(const char* name);
int sub_E92820();
int __stdcall sub_76D910(int code, const int* packet);
void sub_E92E90(int(__stdcall* callback)(int, const int*));
void sub_CF0BA0(const char* file, int line, const char* message);
void __fastcall sub_76C320(void* tokens);

namespace {
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    // INLINED
    explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const char (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
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
  typedef void(__thiscall * DeletingDestructor)(CNProtectGameGuardMgr*, unsigned char);

  if (g_nProtectGameGuardMgr) {
    (*reinterpret_cast<DeletingDestructor**>(g_nProtectGameGuardMgr))[0](g_nProtectGameGuardMgr, 1);
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
    const char source[] = {'\xB3', '\xD6', '\xC4', '\x01', '\x81'};
    return DecryptName<0x7321C3814F8799E1ULL>(source);
  }
  case 0:
    if (std::strstr(g_session.unknown_58FA, g_devClientTag) != nullptr) {
      const char source[] = {'\x1D', '\xC0', '\xA8', '\x5B', '\x02', '\xA5', '\x2E', '\x12', '\x04', '\xF3', '\xCF'};
      return DecryptName<0x7941D76335CFA14FULL>(source);
    } else {
      const char source[] = {'\x2D', '\xB4', '\xC8', '\x35', '\x0A', '\xC3', '\x7A', '\x64', '\x34', '\x87', '\xAF'};
      return DecryptName<0x0F15B16B5BAFD57FULL>(source);
    }
  case 2: {
    const char source[] = {'\x71', '\x90', '\xB0', '\x79', '\xB6', '\x3B', '\x8C', '\x16', '\x69', '\xA1', '\xD7'};
    return DecryptName<0x7DE349D717D7F123ULL>(source);
  }
  case 12: {
    const char source[] = {'\x0F', '\x6C', '\x2E', '\xE7', '\xC0', '\x0F', '\x82', '\xF4', '\x1F', '\x5F', '\x49'};
    return DecryptName<0x9FED7DA189490D5DULL>(source);
  }
  case 7: {
    const char source[] = {'\x37', '\x34', '\xAE', '\x11', '\x6C', '\x9D', '\x40', '\x2E', '\x35', '\x1D', '\xC9'};
    return DecryptName<0x452FEF0D7FC95565ULL>(source);
  }
  case 4: {
    const char source[] = {'\x3F', '\x54', '\xF3', '\x46', '\x77'};
    return DecryptName<0xF3CB6F7711A71B6DULL>(source);
  }
  default: {
    const char source[] = {'\xE1', '\x06', '\x3E', '\xE1', '\xBE', '\x55', '\xF4', '\xE8', '\xB3'};
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
int __stdcall sub_76D910(int code, const int* packet) {
  switch (code) {
  case 0x3E9:
  case 0x3EA: {
    const char source[] = {'\x98', '\x1B', '\x0A', '\x4D', '\xD8', '\x94', '\x3F', '\x05', '\xC8', '\x11', '\x48', '\x23'};
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x13B, std::string(FormatString(DecryptName<0x2D51E3B7236175EDULL>(source), code)));
    return 0;
  }
  case 0x3F3: {
    const char source[] = {'\x32', '\x68', '\x05', '\x86', '\xD9', '\xC2', '\x41'};
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x136, std::string(FormatString(DecryptName<0x0F41EBBDA32D1B17ULL>(source), LookupMessage(0x3A1), code)));
    return 0;
  }
  case 0x3F4:
  case 0x3F5:
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x14B, std::string(LookupMessage(0xAF7)));
    return 0;
  case 0x3F6: {
    const char source[] = {'\x40', '\x80', '\xFF', '\x78', '\xE9', '\x3A', '\x6F'};
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x13F, std::string(FormatString(DecryptName<0x176F138D5DD7F365ULL>(source), LookupMessage(0x3A0), code)));
    return 0;
  }
  case 0x3F7: {
    const char source[] = {'\xE4', '\x34', '\xED', '\x32', '\x85', '\xB0', '\xD5'};
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x151, std::string(FormatString(DecryptName<0xFBD599E117C547C1ULL>(source), LookupMessage(0x3A3), code)));
    return 0;
  }
  case 0x3FB: {
    const int id = packet[0];
    const int size = packet[1];
    const unsigned char* data = reinterpret_cast<const unsigned char*>(packet + 2);
    CNProtectGameGuardMgr::CSAuthToken token = {};
    token.id = id;
    if (size != 0) {
      token.bytes.assign(data, data + size);
    }
    NSyncObject lock(&g_nProtectGameGuardMgr->m_lock);
    g_nProtectGameGuardMgr->m_tokens.push_back(token);
    break;
  }
  default:
    break;
  }
  return 1;
}

// OFFSET: 0x0076C9D0
// FUNCTION: fn_08209
int CNProtectGameGuardMgr::sub_76C9D0() {
  void* created = ::operator new(1, std::nothrow);
  if (created != nullptr) {
    sub_E929B0(GameName());
  }
  this->m_gameLib = created;
  if (created == nullptr) {
    return 0;
  }

  const int status = sub_E92820();
  if (status != 0x755) {
    char message[0x400];
    char title[0x100];
    std::memset(message, 0, sizeof(message));
    std::memset(title, 0, sizeof(title));

    const char bodySource[] = {'\x18', '\x54', '\xC2', '\xF0', '\xDA', '\xFC', '\x92', '\xBD', '\x3B', '\x15', '\xEA', '\xE7', '\xEF', '\xE6', '\x81', '\xF5', '\x7F', '\x10', '\xC3', '\xE0', '\x9D'};
    StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), DecryptName<0xCFF3899D95AF355FULL>(bodySource), status);

    const char titleSource[] = {'\x62', '\x10', '\xC2', '\xAE', '\x4A', '\x86', '\x4C', '\x97', '\x41', '\x51', '\xEA', '\xB9', '\x7F', '\x9C', '\x5F', '\xC5', '\x1F', '\x51', '\x8A', '\xA7', '\x78', '\xF3'};
    StringFormat(title, sizeof(title), static_cast<std::size_t>(-1), DecryptName<0xE52DF30DCBAF7125ULL>(titleSource), status);

    const char tagSource[] = {'\xF6', '\x41', '\x9F', '\xAF', '\x7A', '\x91', '\xC9', '\x08', '\xF2', '\x31'};
    if (std::strstr(g_commandLine, DecryptName<0x4F99DF03C3EF31B7ULL>(tagSource)) == nullptr) {
      MessageBoxA(hWnd, message, title, 0);
      return 0;
    }
    sub_CF0BA0(g_nprotectSource, 0x284, title);
  }

  sub_E92E90(sub_76D910);
  return 1;
}
