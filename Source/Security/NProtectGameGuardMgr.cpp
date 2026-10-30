#include "Security/NProtectGameGuardMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Threading/NSyncObject.h"

#include <cstdint>
#include <cstring>
#include <string>

// GLOBAL: gl_24848
CNProtectGameGuardMgr* g_nProtectGameGuardMgr;

// STRING: st_16836
static const char g_devClientTag[] = "Dev";

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
