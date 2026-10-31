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
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
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
    const std::uint8_t source[] = {0xB3, 0xD6, 0xC4, 0x01, 0x81};
    thread_local EncryptedName<sizeof(source), 0x7321C3814F8799E1ULL> name(source);
    return name.Text();
  }
  case 0:
    if (std::strstr(g_session.unknown_58FA, g_devClientTag) != nullptr) {
      const std::uint8_t source[] = {0x1D, 0xC0, 0xA8, 0x5B, 0x02, 0xA5, 0x2E, 0x12, 0x04, 0xF3, 0xCF};
      thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> name(source);
      return name.Text();
    } else {
      const std::uint8_t source[] = {0x2D, 0xB4, 0xC8, 0x35, 0x0A, 0xC3, 0x7A, 0x64, 0x34, 0x87, 0xAF};
      thread_local EncryptedName<sizeof(source), 0x0F15B16B5BAFD57FULL> name(source);
      return name.Text();
    }
  case 2: {
    const std::uint8_t source[] = {0x71, 0x90, 0xB0, 0x79, 0xB6, 0x3B, 0x8C, 0x16, 0x69, 0xA1, 0xD7};
    thread_local EncryptedName<sizeof(source), 0x7DE349D717D7F123ULL> name(source);
    return name.Text();
  }
  case 12: {
    const std::uint8_t source[] = {0x0F, 0x6C, 0x2E, 0xE7, 0xC0, 0x0F, 0x82, 0xF4, 0x1F, 0x5F, 0x49};
    thread_local EncryptedName<sizeof(source), 0x9FED7DA189490D5DULL> name(source);
    return name.Text();
  }
  case 7: {
    const std::uint8_t source[] = {0x37, 0x34, 0xAE, 0x11, 0x6C, 0x9D, 0x40, 0x2E, 0x35, 0x1D, 0xC9};
    thread_local EncryptedName<sizeof(source), 0x452FEF0D7FC95565ULL> name(source);
    return name.Text();
  }
  case 4: {
    const std::uint8_t source[] = {0x3F, 0x54, 0xF3, 0x46, 0x77};
    thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
    return name.Text();
  }
  default: {
    const std::uint8_t source[] = {0xE1, 0x06, 0x3E, 0xE1, 0xBE, 0x55, 0xF4, 0xE8, 0xB3};
    thread_local EncryptedName<sizeof(source), 0x839B27DF8F5967B3ULL> name(source);
    return name.Text();
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

// OFFSET: 0x0076C4C0

// OFFSET: 0x0076D910
// FUNCTION: fn_08219
int __stdcall sub_76D910(int code, const int* packet) {
  switch (code) {
  case 0x3E9:
  case 0x3EA: {
    const std::uint8_t source[] = {0x98, 0x1B, 0x0A, 0x4D, 0xD8, 0x94, 0x3F, 0x05, 0xC8, 0x11, 0x48, 0x23};
    thread_local EncryptedName<sizeof(source), 0x2D51E3B7236175EDULL> name(source);
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x13B, std::string(FormatString(name.Text(), code)));
    return 0;
  }
  case 0x3F3: {
    const std::uint8_t source[] = {0x32, 0x68, 0x05, 0x86, 0xD9, 0xC2, 0x41};
    thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> name(source);
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x136, std::string(FormatString(name.Text(), LookupMessage(0x3A1), code)));
    return 0;
  }
  case 0x3F4:
  case 0x3F5:
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x14B, std::string(LookupMessage(0xAF7)));
    return 0;
  case 0x3F6: {
    const std::uint8_t source[] = {0x40, 0x80, 0xFF, 0x78, 0xE9, 0x3A, 0x6F};
    thread_local EncryptedName<sizeof(source), 0x176F138D5DD7F365ULL> name(source);
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x13F, std::string(FormatString(name.Text(), LookupMessage(0x3A0), code)));
    return 0;
  }
  case 0x3F7: {
    const std::uint8_t source[] = {0xE4, 0x34, 0xED, 0x32, 0x85, 0xB0, 0xD5};
    thread_local EncryptedName<sizeof(source), 0xFBD599E117C547C1ULL> name(source);
    g_nProtectGameGuardMgr->SetError(g_nprotectSource, 0x151, std::string(FormatString(name.Text(), LookupMessage(0x3A3), code)));
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

    const std::uint8_t bodySource[] = {
        0x18, 0x54, 0xC2, 0xF0, 0xDA, 0xFC, 0x92, 0xBD, 0x3B, 0x15, 0xEA, 0xE7, 0xEF, 0xE6, 0x81, 0xF5, 0x7F, 0x10, 0xC3, 0xE0, 0x9D};
    thread_local EncryptedName<sizeof(bodySource), 0xCFF3899D95AF355FULL> body(bodySource);
    StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), body.Text(), status);

    const std::uint8_t titleSource[] = {
        0x62, 0x10, 0xC2, 0xAE, 0x4A, 0x86, 0x4C, 0x97, 0x41, 0x51, 0xEA, 0xB9, 0x7F, 0x9C, 0x5F, 0xC5, 0x1F, 0x51, 0x8A, 0xA7, 0x78, 0xF3};
    thread_local EncryptedName<sizeof(titleSource), 0xE52DF30DCBAF7125ULL> titleName(titleSource);
    StringFormat(title, sizeof(title), static_cast<std::size_t>(-1), titleName.Text(), status);

    const std::uint8_t tagSource[] = {0xF6, 0x41, 0x9F, 0xAF, 0x7A, 0x91, 0xC9, 0x08, 0xF2, 0x31};
    thread_local EncryptedName<sizeof(tagSource), 0x4F99DF03C3EF31B7ULL> tag(tagSource);
    if (std::strstr(g_commandLine, tag.Text()) == nullptr) {
      MessageBoxA(hWnd, message, title, 0);
      return 0;
    }
    sub_CF0BA0(g_nprotectSource, 0x284, title);
  }

  sub_E92E90(sub_76D910);
  return 1;
}
