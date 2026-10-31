#include "Client/Application/Launcher.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <io.h>
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <sys/stat.h>

// STATIC: 0x0173ad64
// GLOBAL: gl_24472
std::uint32_t g_clientVersion;

// GLOBAL: gl_24584
char g_applicationDirectory[0x80];

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
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

struct AdtestexeProcessName {
  EncryptedName<14, 0xD98F03A52D35636DULL>& operator()() const;
};

struct RagexeMProcessName {
  EncryptedName<13, 0x73D1674B61DF7B0BULL>& operator()() const;
};

struct SakexeMProcessName {
  EncryptedName<13, 0x91651D81F559F971ULL>& operator()() const;
};

struct Kernel32LibraryName {
  EncryptedName<13, 0x17359BF953C367B3ULL>& operator()() const;
};

struct PsapiLibraryName {
  EncryptedName<10, 0x490FFB43A12F0DB9ULL>& operator()() const;
};

struct CreateToolhelp32SnapshotName {
  EncryptedName<25, 0x2FF3C5036D070127ULL>& operator()() const;
};

struct EnumProcessesName {
  EncryptedName<14, 0x3FAF85D995034FD7ULL>& operator()() const;
};

struct AdventurerAgencyCreateInstanceName {
  EncryptedName<39, 0x6961FF056999A303ULL>& operator()() const;
};

struct RequestJoinPartyCreateInstanceName {
  EncryptedName<40, 0x9DEFC105A1EB2F71ULL>& operator()() const;
};

struct AdventureGuideCreateInstanceName {
  EncryptedName<37, 0x0DF5B33D9F21EB79ULL>& operator()() const;
};

struct GoodResultName {
  EncryptedName<5, 0x01E7898D9D235DDFULL>& operator()() const;
};

struct Process32FirstName {
  EncryptedName<15, 0x1D6D75EDE7CD311FULL>& operator()() const;
};

struct EnumProcessModulesName {
  EncryptedName<19, 0x151F77EBEBBB2147ULL>& operator()() const;
};

struct Process32NextName {
  EncryptedName<14, 0x3375B1C7DBB5FD55ULL>& operator()() const;
};

struct GetModuleBaseNameAName {
  EncryptedName<19, 0x2D394DBD0F332733ULL>& operator()() const;
};

struct RagexeModuleName {
  EncryptedName<11, 0x7D993F650D2785FFULL>& operator()() const;
};

struct RagexeProcessName {
  EncryptedName<11, 0x812BE72BCD5707B7ULL>& operator()() const;
};

struct SakexeModuleName {
  EncryptedName<11, 0x61FF87ED7BF39BE1ULL>& operator()() const;
};

struct SakexeProcessName {
  EncryptedName<11, 0x59278F63D79F3DA1ULL>& operator()() const;
};

struct RagexereModuleName {
  EncryptedName<13, 0x2D3B15774149D5DDULL>& operator()() const;
};

struct RagexereProcessName {
  EncryptedName<13, 0x31D7C1EDA345E36DULL>& operator()() const;
};

struct RagtestexeProcessName {
  EncryptedName<15, 0xD3755DD95F47A909ULL>& operator()() const;
};

struct SaktestexeProcessName {
  EncryptedName<15, 0x0F0F8B6B8F9D19D1ULL>& operator()() const;
};

// OFFSET: 0x00E55AD0
// FUNCTION: fn_24310
template <>
const char* EncryptedName<5, 0x01E7898D9D235DDFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 5, 0x01E7898D9D235DDFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55B00
// FUNCTION: fn_24311
template <>
const char* EncryptedName<19, 0x151F77EBEBBB2147ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 19, 0x151F77EBEBBB2147ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55B30
// FUNCTION: fn_24312
template <>
const char* EncryptedName<19, 0x2D394DBD0F332733ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 19, 0x2D394DBD0F332733ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55B60
// FUNCTION: fn_24313
template <>
const char* EncryptedName<25, 0x2FF3C5036D070127ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x2FF3C5036D070127ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55B90
// FUNCTION: fn_24314
template <>
const char* EncryptedName<37, 0x0DF5B33D9F21EB79ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 37, 0x0DF5B33D9F21EB79ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55BC0
// FUNCTION: fn_24315
template <>
const char* EncryptedName<39, 0x6961FF056999A303ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 39, 0x6961FF056999A303ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55BF0
// FUNCTION: fn_24316
template <>
const char* EncryptedName<40, 0x9DEFC105A1EB2F71ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 40, 0x9DEFC105A1EB2F71ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55C20
// FUNCTION: fn_24317
template <>
const char* EncryptedName<11, 0x812BE72BCD5707B7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 11, 0x812BE72BCD5707B7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55C50
// FUNCTION: fn_24318
template <>
const char* EncryptedName<11, 0x59278F63D79F3DA1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 11, 0x59278F63D79F3DA1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55C80
// FUNCTION: fn_24319
template <>
const char* EncryptedName<11, 0x61FF87ED7BF39BE1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 11, 0x61FF87ED7BF39BE1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55CB0
// FUNCTION: fn_24320
template <>
const char* EncryptedName<11, 0x7D993F650D2785FFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 11, 0x7D993F650D2785FFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55CE0
// FUNCTION: fn_24321
template <>
const char* EncryptedName<13, 0x91651D81F559F971ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 13, 0x91651D81F559F971ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55D10
// FUNCTION: fn_24322
template <>
const char* EncryptedName<13, 0x2D3B15774149D5DDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 13, 0x2D3B15774149D5DDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55D40
// FUNCTION: fn_24323
template <>
const char* EncryptedName<13, 0x31D7C1EDA345E36DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 13, 0x31D7C1EDA345E36DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55D70
// FUNCTION: fn_24324
template <>
const char* EncryptedName<13, 0x73D1674B61DF7B0BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 13, 0x73D1674B61DF7B0BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55DA0
// FUNCTION: fn_24325
template <>
const char* EncryptedName<14, 0xD98F03A52D35636DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xD98F03A52D35636DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55DD0
// FUNCTION: fn_24326
template <>
const char* EncryptedName<14, 0x3375B1C7DBB5FD55ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x3375B1C7DBB5FD55ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55E00
// FUNCTION: fn_24327
template <>
const char* EncryptedName<14, 0x3FAF85D995034FD7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x3FAF85D995034FD7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55E30
// FUNCTION: fn_24328
template <>
const char* EncryptedName<15, 0xD3755DD95F47A909ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 15, 0xD3755DD95F47A909ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55E60
// FUNCTION: fn_24329
template <>
const char* EncryptedName<15, 0x1D6D75EDE7CD311FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 15, 0x1D6D75EDE7CD311FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55E90
// FUNCTION: fn_24330
template <>
const char* EncryptedName<15, 0x0F0F8B6B8F9D19D1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 15, 0x0F0F8B6B8F9D19D1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E55EC0
// FUNCTION: fn_24331
EncryptedName<14, 0xD98F03A52D35636DULL>& AdtestexeProcessName::operator()() const {
  const std::uint8_t source[] = {0x2C, 0x27, 0x61, 0x68, 0xF6, 0x57, 0xCA, 0x81, 0x28, 0x4D, 0x70, 0x75, 0xE0, 0x03};
  thread_local EncryptedName<sizeof(source), 0xD98F03A52D35636DULL> name(source);
  return name;
}

// OFFSET: 0x00E55F90
// FUNCTION: fn_24332
EncryptedName<13, 0x73D1674B61DF7B0BULL>& RagexeMProcessName::operator()() const {
  const std::uint8_t source[] = {0x59, 0x3A, 0x98, 0x24, 0x13, 0x22, 0x8E, 0x3E, 0x25, 0x3E, 0x87, 0x24, 0x4B};
  thread_local EncryptedName<sizeof(source), 0x73D1674B61DF7B0BULL> name(source);
  return name;
}

// OFFSET: 0x00E56060
// FUNCTION: fn_24333
EncryptedName<13, 0x91651D81F559F971ULL>& SakexeMProcessName::operator()() const {
  const std::uint8_t source[] = {0x22, 0xB8, 0x12, 0xB0, 0xD9, 0x58, 0x3A, 0xDC, 0x5F, 0xBC, 0x01, 0xB0, 0x81};
  thread_local EncryptedName<sizeof(source), 0x91651D81F559F971ULL> name(source);
  return name;
}

// OFFSET: 0x00E56130
// FUNCTION: fn_24334
EncryptedName<13, 0x17359BF953C367B3ULL>& Kernel32LibraryName::operator()() const {
  const std::uint8_t source[] = {0xD8, 0x02, 0xB1, 0x3D, 0x9C, 0xF7, 0x06, 0x25, 0x9D, 0x03, 0xAF, 0x3F, 0xF9};
  thread_local EncryptedName<sizeof(source), 0x17359BF953C367B3ULL> name(source);
  return name;
}

// OFFSET: 0x00E56200
// FUNCTION: fn_24335
EncryptedName<10, 0x490FFB43A12F0DB9ULL>& PsapiLibraryName::operator()() const {
  const std::uint8_t source[] = {0xC9, 0x7E, 0x4E, 0xD1, 0x2A, 0xD5, 0x6B, 0x25, 0xD5, 0x0D};
  thread_local EncryptedName<sizeof(source), 0x490FFB43A12F0DB9ULL> name(source);
  return name;
}

// OFFSET: 0x00E562D0
// FUNCTION: fn_24336
EncryptedName<25, 0x2FF3C5036D070127ULL>& CreateToolhelp32SnapshotName::operator()() const {
  const std::uint8_t source[] = {0x64, 0x73, 0x62, 0x0C, 0x77, 0xA0, 0xA7, 0x40, 0x48, 0x6D, 0x6F, 0x08, 0x6F, 0xB5, 0xC0, 0x1D,
                                 0x74, 0x6F, 0x66, 0x1D, 0x70, 0xAD, 0x9C, 0x5B, 0x27};
  thread_local EncryptedName<sizeof(source), 0x2FF3C5036D070127ULL> name(source);
  return name;
}

// OFFSET: 0x00E563B0
// FUNCTION: fn_24337
EncryptedName<14, 0x3FAF85D995034FD7ULL>& EnumProcessesName::operator()() const {
  const std::uint8_t source[] = {0x92, 0x21, 0x76, 0xF8, 0x89, 0xF7, 0xC0, 0x5C, 0xB2, 0x3C, 0x70, 0xF0, 0xAA, 0x85};
  thread_local EncryptedName<sizeof(source), 0x3FAF85D995034FD7ULL> name(source);
  return name;
}

// OFFSET: 0x00E56480
// FUNCTION: fn_24338
EncryptedName<39, 0x6961FF056999A303ULL>& AdventurerAgencyCreateInstanceName::operator()() const {
  const std::uint8_t source[] = {0x40, 0xE2, 0xFD, 0x1F, 0x60, 0x91, 0x15, 0x1C, 0x71, 0xC6, 0xEB, 0x28, 0x62, 0x9A, 0x0F, 0x0A,
                                 0x7A, 0xEE, 0xFE, 0x1B, 0x3F, 0xC5, 0x22, 0x1B, 0x66, 0xC2, 0xED, 0x0C, 0x4C, 0x91, 0x12, 0x1D,
                                 0x62, 0xCD, 0xFA, 0x0C, 0x2D, 0xD6, 0x61};
  thread_local EncryptedName<sizeof(source), 0x6961FF056999A303ULL> name(source);
  return name;
}

// OFFSET: 0x00E56560
// FUNCTION: fn_24339
EncryptedName<40, 0x9DEFC105A1EB2F71ULL>& RequestJoinPartyCreateInstanceName::operator()() const {
  const std::uint8_t source[] = {0x24, 0x66, 0xB9, 0xC4, 0x74, 0xB4, 0x8A, 0xEE, 0x05, 0x65, 0x84, 0xC8, 0x6B, 0x91, 0x8E, 0xEF,
                                 0x05, 0x56, 0xA6, 0xC6, 0x77, 0xFB, 0xD5, 0xDE, 0x03, 0x4A, 0x8A, 0xD5, 0x60, 0x88, 0x81, 0xEE,
                                 0x05, 0x4E, 0x85, 0xC2, 0x60, 0xE9, 0xC6, 0x9D};
  thread_local EncryptedName<sizeof(source), 0x9DEFC105A1EB2F71ULL> name(source);
  return name;
}

// OFFSET: 0x00E56640
// FUNCTION: fn_24340
EncryptedName<37, 0x0DF5B33D9F21EB79ULL>& AdventureGuideCreateInstanceName::operator()() const {
  const std::uint8_t source[] = {0x3A, 0xAA, 0x45, 0xE9, 0x58, 0xDD, 0x81, 0x78, 0x0B, 0x8E, 0x66, 0xEA, 0x54, 0xD7, 0x90, 0x40,
                                 0x1E, 0x99, 0x1B, 0xA5, 0x7E, 0xC1, 0x90, 0x6C, 0x0D, 0x8E, 0x68, 0xF1, 0x4E, 0xC7, 0x94, 0x63,
                                 0x1A, 0x8E, 0x09, 0xB6, 0x3D};
  thread_local EncryptedName<sizeof(source), 0x0DF5B33D9F21EB79ULL> name(source);
  return name;
}

// OFFSET: 0x00E56720
// FUNCTION: fn_24341
EncryptedName<5, 0x01E7898D9D235DDFULL>& GoodResultName::operator()() const {
  const std::uint8_t source[] = {0xB8, 0x32, 0x4C, 0xF9, 0x8D};
  thread_local EncryptedName<sizeof(source), 0x01E7898D9D235DDFULL> name(source);
  return name;
}

// OFFSET: 0x00E567E0
// FUNCTION: fn_24342
EncryptedName<15, 0x1D6D75EDE7CD311FULL>& Process32FirstName::operator()() const {
  const std::uint8_t source[] = {0x4F, 0x43, 0xA2, 0x84, 0x88, 0x06, 0x1E, 0x2E, 0x2D, 0x77, 0xA4, 0x95, 0x9E, 0x01, 0x6D};
  thread_local EncryptedName<sizeof(source), 0x1D6D75EDE7CD311FULL> name(source);
  return name;
}

// OFFSET: 0x00E568C0
// FUNCTION: fn_24343
EncryptedName<19, 0x151F77EBEBBB2147ULL>& EnumProcessModulesName::operator()() const {
  const std::uint8_t source[] = {0x02, 0x4F, 0xCE, 0x86, 0xBB, 0x05, 0x70, 0x76, 0x22, 0x52, 0xC8, 0xA6, 0x84, 0x13, 0x6A, 0x79, 0x22, 0x52, 0xBB};
  thread_local EncryptedName<sizeof(source), 0x151F77EBEBBB2147ULL> name(source);
  return name;
}

// OFFSET: 0x00E56990
// FUNCTION: fn_24344
EncryptedName<14, 0x3375B1C7DBB5FD55ULL>& Process32NextName::operator()() const {
  const std::uint8_t source[] = {0x05, 0x8F, 0xDA, 0xB8, 0xA2, 0xC2, 0x06, 0x00, 0x67, 0xB3, 0xD0, 0xA3, 0xB3, 0xB1};
  thread_local EncryptedName<sizeof(source), 0x3375B1C7DBB5FD55ULL> name(source);
  return name;
}

// OFFSET: 0x00E56A60
// FUNCTION: fn_24345
EncryptedName<19, 0x2D394DBD0F332733ULL>& GetModuleBaseNameAName::operator()() const {
  const std::uint8_t source[] = {0x74, 0x42, 0x47, 0x42, 0xD2, 0x29, 0x4C, 0x41, 0x56, 0x65, 0x52, 0x7C, 0xD8, 0x03, 0x58, 0x40, 0x56, 0x66, 0x33};
  thread_local EncryptedName<sizeof(source), 0x2D394DBD0F332733ULL> name(source);
  return name;
}

// OFFSET: 0x00E56B30
// FUNCTION: fn_24346
EncryptedName<11, 0x7D993F650D2785FFULL>& RagexeModuleName::operator()() const {
  const std::uint8_t source[] = {0xAD, 0xC4, 0x60, 0x48, 0x3D, 0x7A, 0xB7, 0x38, 0xA7, 0xC0, 0x27};
  thread_local EncryptedName<sizeof(source), 0x7D993F650D2785FFULL> name(source);
  return name;
}

// OFFSET: 0x00E56C00
// FUNCTION: fn_24347
EncryptedName<11, 0x812BE72BCD5707B7ULL>& RagexeProcessName::operator()() const {
  const std::uint8_t source[] = {0xE5, 0x46, 0x10, 0x88, 0x73, 0xA2, 0x05, 0xC4, 0xEF, 0x42, 0x57};
  thread_local EncryptedName<sizeof(source), 0x812BE72BCD5707B7ULL> name(source);
  return name;
}

// OFFSET: 0x00E56CD0
// FUNCTION: fn_24348
EncryptedName<11, 0x61FF87ED7BF39BE1ULL>& SakexeModuleName::operator()() const {
  const std::uint8_t source[] = {0xB2, 0xDA, 0xB8, 0x3E, 0xB5, 0xC2, 0xD1, 0x24, 0xB9, 0xDE, 0xF3};
  thread_local EncryptedName<sizeof(source), 0x61FF87ED7BF39BE1ULL> name(source);
  return name;
}

// OFFSET: 0x00E56DA0
// FUNCTION: fn_24349
EncryptedName<11, 0x59278F63D79F3DA1ULL>& SakexeProcessName::operator()() const {
  const std::uint8_t source[] = {0xF2, 0x7C, 0xD4, 0x92, 0x3B, 0xCA, 0x09, 0x1C, 0xF9, 0x78, 0x9F};
  thread_local EncryptedName<sizeof(source), 0x59278F63D79F3DA1ULL> name(source);
  return name;
}

// OFFSET: 0x00E56E70
// FUNCTION: fn_24350
EncryptedName<13, 0x2D3B15774149D5DDULL>& RagexereModuleName::operator()() const {
  const std::uint8_t source[] = {0x8F, 0x94, 0x0E, 0x04, 0x2F, 0x50, 0x69, 0x68, 0xF3, 0x90, 0x11, 0x04, 0x77};
  thread_local EncryptedName<sizeof(source), 0x2D3B15774149D5DDULL> name(source);
  return name;
}

// OFFSET: 0x00E56F40
// FUNCTION: fn_24351
EncryptedName<13, 0x31D7C1EDA345E36DULL>& RagexereProcessName::operator()() const {
  const std::uint8_t source[] = {0x3F, 0xA2, 0x02, 0xE6, 0xB5, 0x84, 0x85, 0x74, 0x43, 0xA6, 0x1D, 0xE6, 0xED};
  thread_local EncryptedName<sizeof(source), 0x31D7C1EDA345E36DULL> name(source);
  return name;
}

// OFFSET: 0x00E57010
// FUNCTION: fn_24352
EncryptedName<15, 0xD3755DD95F47A909ULL>& RagtestexeProcessName::operator()() const {
  const std::uint8_t source[] = {0x5B, 0xE8, 0x00, 0x0B, 0x9C, 0x0E, 0x21, 0x96, 0x51, 0xEC, 0x69, 0x1A, 0x81, 0x18, 0x75};
  thread_local EncryptedName<sizeof(source), 0xD3755DD95F47A909ULL> name(source);
  return name;
}

// OFFSET: 0x00E570F0
// FUNCTION: fn_24353
EncryptedName<15, 0x0F0F8B6B8F9D19D1ULL>& SaktestexeProcessName::operator()() const {
  const std::uint8_t source[] = {0x82, 0x58, 0xD6, 0xDB, 0x2E, 0xD8, 0x5B, 0x4A, 0x89, 0x5C, 0xB3, 0xCA, 0x33, 0xCE, 0x0F};
  thread_local EncryptedName<sizeof(source), 0x0F0F8B6B8F9D19D1ULL> name(source);
  return name;
}

// OFFSET: 0x00E57B50
// FUNCTION: fn_24359
int RemovePath(std::string path, int depth, bool file) {
  std::string pattern = path;

  if (file == true) {
    DeleteFileA(pattern.c_str());
    return 0;
  }

  if (pattern.compare(pattern.size() - 4, 4, "\\*.*") != 0) {
    pattern += "\\*.*";
  }

  WIN32_FIND_DATAA data;
  HANDLE find = FindFirstFileA(pattern.c_str(), &data);
  if (find == INVALID_HANDLE_VALUE) {
    if (depth > 0) {
      RemoveDirectoryA(path.c_str());
    }

    return -1;
  }

  BOOL found;
  do {
    std::string child = FormatOwnedString("%s\\%s", path.c_str(), data.cFileName);

    if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
      SetFileAttributesA(child.c_str(), FILE_ATTRIBUTE_NORMAL);
    }

    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if (std::strcmp(data.cFileName, ".") != 0 && std::strcmp(data.cFileName, "..") != 0) {
        RemovePath(child, depth + 1, false);
      }
    } else {
      DeleteFileA(child.c_str());
    }

    found = FindNextFileA(find, &data);
  } while (found);

  FindClose(find);

  if (depth > 0) {
    RemoveDirectoryA(path.c_str());
  }

  return 0;
}

// OFFSET: 0x00E58820
// FUNCTION: fn_24363
bool RestartFromCommandLine(char* commandLine) {
  char* marker = std::strstr(commandLine, "/launcherRESTART:");
  if (!marker) {
    return false;
  }

  STARTUPINFOA startup = {};
  startup.cb = sizeof(startup);
  startup.dwFlags = STARTF_USESHOWWINDOW;
  startup.wShowWindow = SW_SHOWNORMAL;
  PROCESS_INFORMATION process;

  CreateProcessA(nullptr, marker + std::strlen("/launcherRESTART:"), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &process);
  CloseHandle(process.hProcess);
  CloseHandle(process.hThread);

  return true;
}

// OFFSET: 0x00E58A60
// FUNCTION: fn_24365
void ApplyAiPatch() {
  const std::uint8_t patchSource[] = {0xAE, 0x6A, 0x8A, 0x9F, 0x78, 0x25, 0xF2, 0x53, 0xEF};
  thread_local EncryptedName<sizeof(patchSource), 0x3B915119EFD523EFULL> patchName(patchSource);
  if (_access(patchName.Text(), 0) != 0) {
    return;
  }

  const std::uint8_t aiSource[] = {0x5A, 0x44, 0x39};
  thread_local EncryptedName<sizeof(aiSource), 0x5B8F49AF09390D1BULL> aiName(aiSource);
  if (_access(aiName.Text(), 0) != 0) {
    return;
  }

  const std::uint8_t backupSource[] = {0x80, 0x9A, 0x84, 0x15, 0xFA, 0x16, 0xA8, 0x3C, 0xB1, 0xD3};
  thread_local EncryptedName<sizeof(backupSource), 0x49C3759B77DBD3C1ULL> backupName(backupSource);
  if (_access(backupName.Text(), 0) == 0) {
    const std::uint8_t deleteSource[] = {0xDA, 0xE4, 0x2A, 0xAD, 0xB0, 0x5E, 0x86, 0x18, 0xEB, 0xAD};
    char path[16] = {};
    thread_local EncryptedName<sizeof(deleteSource), 0x6DED3DD1CF75AD9BULL> deleteName(deleteSource);
    std::strcpy(path, deleteName.Text());

    SHFILEOPSTRUCTA operation;
    operation.hwnd = nullptr;
    operation.wFunc = FO_DELETE;
    operation.pFrom = path;
    operation.pTo = nullptr;
    operation.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
    operation.fAnyOperationsAborted = FALSE;
    operation.hNameMappings = nullptr;
    operation.lpszProgressTitle = nullptr;
    SHFileOperationA(&operation);
  }

  const std::uint8_t backupTargetSource[] = {0xBC, 0xB4, 0x22, 0x91, 0x80, 0x76, 0x92, 0x6E, 0x8D, 0xFD};
  thread_local EncryptedName<sizeof(backupTargetSource), 0x1BF915E1F37DFDFDULL> backupTarget(backupTargetSource);
  const char* backup = backupTarget.Text();
  const std::uint8_t currentSource[] = {0x9E, 0x90, 0x5D};
  thread_local EncryptedName<sizeof(currentSource), 0x01FF9313695DD9DFULL> current(currentSource);
  std::rename(current.Text(), backup);

  const std::uint8_t installTargetSource[] = {0x52, 0x8E, 0x03};
  thread_local EncryptedName<sizeof(installTargetSource), 0x4F1791FB1503C713ULL> installTarget(installTargetSource);
  const char* install = installTarget.Text();
  const std::uint8_t patchedSource[] = {0x92, 0xA4, 0xC8, 0x65, 0x2C, 0x2F, 0xC6, 0xC9, 0xD3};
  thread_local EncryptedName<sizeof(patchedSource), 0xA1A55B4D1597EDD3ULL> patched(patchedSource);
  std::rename(patched.Text(), install);
}

// INLINED
void ExitForProtectionCheck(const char* commandLine) {
  if (std::strstr(commandLine, "/check_themida")) {
    ExitProcess(0);
  }
}

// INLINED
bool AcquireInstanceMutex(HANDLE* mutex) {
  *mutex = CreateMutexA(nullptr, TRUE, "MutexInit");
  return GetLastError() != ERROR_ALREADY_EXISTS;
}

// GLOBAL: gl_24515
char* g_commandLine;

// GLOBAL: gl_25074
bool g_launchAccountEnabled;

// GLOBAL: gl_25075
char g_launchAccountName[0x1F4];

// GLOBAL: gl_25076
char g_launchAccountPassword[24];

// GLOBAL: gl_25077
char g_launchAccountSex;

// GLOBAL: gl_25078
bool g_launchPassCommandLine;

// OFFSET: 0x00E57E60
// FUNCTION: fn_24361
HANDLE StartClientProcess(char* program) {
  char commandLine[1024];
  std::memset(commandLine, 0, sizeof(commandLine));

  if (g_launchPassCommandLine) {
    StringFormat(commandLine, sizeof(commandLine), static_cast<std::size_t>(-1), "%s %s", program, g_commandLine);
  } else if (g_launchAccountEnabled) {
    StringFormat(commandLine, sizeof(commandLine), static_cast<std::size_t>(-1), "%s %s %s %c %s", program, g_launchAccountName,
                 g_launchAccountPassword, g_launchAccountSex, g_launchServerName);
  } else {
    StringFormat(commandLine, sizeof(commandLine), static_cast<std::size_t>(-1), program);
  }

  STARTUPINFOA startup = {};
  startup.cb = sizeof(startup);
  startup.dwFlags = STARTF_USESHOWWINDOW;
  startup.wShowWindow = SW_SHOWNORMAL;
  PROCESS_INFORMATION process = {};

  CreateProcessA(nullptr, commandLine, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &process);
  return process.hProcess;
}

// OFFSET: 0x00E5C100
// FUNCTION: fn_24371
bool ReplaceClientExecutable(char* executable, const char* update) {
  ApplyAiPatch();
  if (_access(update, 0) != 0) {
    return false;
  }

  const char* format;
  if (g_serviceType == 12) {
    const std::uint8_t messageSource[] = {0xBA, 0xB0, 0x67, 0xDB, 0x84, 0x2E, 0xC1, 0xFC, 0xF0, 0xA7, 0x2E, 0xDB, 0x82, 0x24, 0x80, 0xF5,
                                          0xFE, 0xED, 0x67, 0xFE, 0x87, 0x2E, 0x90, 0xE4, 0xFA, 0xE3, 0x22, 0xD0, 0xCB, 0x08, 0x8A, 0xB1,
                                          0xEF, 0xA2, 0x35, 0xDC, 0xCB, 0x24, 0x8E, 0xFF, 0xEB, 0xAA, 0x29, 0xC8, 0x8A, 0x35, 0xCF, 0x91};
    thread_local EncryptedName<sizeof(messageSource), 0x91E147EBBD47C39FULL> message(messageSource);
    format = message.Text();
  } else {
    const std::uint8_t messageSource[] = {0x24, 0x24, 0xB7, 0xB6, 0x60, 0xA5, 0x30, 0xB9, 0x60, 0x39, 0xF0, 0xBA, 0x77, 0xAB, 0x73, 0x81, 0x73, 0x32,
                                          0xE4, 0xAC, 0x33, 0xEA, 0x38, 0xF1, 0x75, 0x38, 0xB7, 0xAD, 0x76, 0xF6, 0x27, 0xB0, 0x73, 0x23, 0x97};
    thread_local EncryptedName<sizeof(messageSource), 0xD1538513DF975701ULL> message(messageSource);
    format = message.Text();
  }

  char text[256];
  std::sprintf(text, format, executable);

  const std::uint8_t titleSource[] = {0x0C, 0x49, 0xFC, 0x3B, 0x27, 0x43};
  thread_local EncryptedName<sizeof(titleSource), 0x078143534999254DULL> title(titleSource);
  if (MessageBoxA(nullptr, text, title.Text(), MB_OKCANCEL | MB_ICONINFORMATION) != IDOK) {
    return false;
  }

  _chmod(executable, _S_IWRITE);
  _unlink(executable);
  if (std::rename(update, executable) != 0) {
    return false;
  }

  StartClientProcess(executable);
  return true;
}
