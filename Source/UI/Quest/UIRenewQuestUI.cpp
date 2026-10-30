#include "UI/Quest/UIRenewQuestUI.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>

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

// OFFSET: 0x00A59FE0
// FUNCTION: fn_16371
template <>
const char* EncryptedName<6, 0xE33F2FF779878559ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0xE33F2FF779878559ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A010
// FUNCTION: fn_16372
template <>
const char* EncryptedName<6, 0xC70319B5EB61598FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0xC70319B5EB61598FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A040
// FUNCTION: fn_16373
template <>
const char* EncryptedName<6, 0xB7AFBD2FD9FD7501ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0xB7AFBD2FD9FD7501ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A070
// FUNCTION: fn_16374
template <>
const char* EncryptedName<6, 0x114901C7E37F6D49ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0x114901C7E37F6D49ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A0A0
// FUNCTION: fn_16375
template <>
const char* EncryptedName<6, 0x1767DDE18D779D8BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0x1767DDE18D779D8BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A0D0
// FUNCTION: fn_16376
template <>
const char* EncryptedName<44, 0xB3E307A5F753D5E3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0xB3E307A5F753D5E3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A100
// FUNCTION: fn_16377
template <>
const char* EncryptedName<44, 0xA597A7E7492B39D5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0xA597A7E7492B39D5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A130
// FUNCTION: fn_16378
template <>
const char* EncryptedName<45, 0xD5A78DB38D8DAFABULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0xD5A78DB38D8DAFABULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A160
// FUNCTION: fn_16379
template <>
const char* EncryptedName<45, 0xFFABE3D5D1BBEF73ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0xFFABE3D5D1BBEF73ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A190
// FUNCTION: fn_16380
template <>
const char* EncryptedName<45, 0xF15F8785CDE369BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0xF15F8785CDE369BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A1C0
// FUNCTION: fn_16381
template <>
const char* EncryptedName<45, 0x192D654D29337BE5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0x192D654D29337BE5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A1F0
// FUNCTION: fn_16382
template <>
const char* EncryptedName<45, 0x0399459DAF454F0FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0x0399459DAF454F0FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A220
// FUNCTION: fn_16383
template <>
const char* EncryptedName<45, 0x79E915E9AF6F55DDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0x79E915E9AF6F55DDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A250
// FUNCTION: fn_16384
template <>
const char* EncryptedName<46, 0x271B45771F83DD9FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 46, 0x271B45771F83DD9FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A280
// FUNCTION: fn_16385
template <>
const char* EncryptedName<46, 0x792FFD1325418FB5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 46, 0x792FFD1325418FB5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A2B0
// FUNCTION: fn_16386
template <>
const char* EncryptedName<47, 0xC5B1BBCD9BC9C997ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0xC5B1BBCD9BC9C997ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A2E0
// FUNCTION: fn_16387
template <>
const char* EncryptedName<47, 0xC1BB8111EDD183E1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0xC1BB8111EDD183E1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A310
// FUNCTION: fn_16388
template <>
const char* EncryptedName<47, 0x8783C33941E15117ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x8783C33941E15117ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A340
// FUNCTION: fn_16389
template <>
const char* EncryptedName<47, 0x01CB4F1DF9D3F931ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x01CB4F1DF9D3F931ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A370
// FUNCTION: fn_16390
template <>
const char* EncryptedName<47, 0x49D33529F1556333ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x49D33529F1556333ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A3A0
// FUNCTION: fn_16391
template <>
const char* EncryptedName<47, 0x0B23D10F2F49D72BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x0B23D10F2F49D72BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A3D0
// FUNCTION: fn_16392
template <>
const char* EncryptedName<47, 0x0B8745519B1F95D7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x0B8745519B1F95D7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A400
// FUNCTION: fn_16393
template <>
const char* EncryptedName<48, 0x136F2F9915D1DBCFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 48, 0x136F2F9915D1DBCFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A430
// FUNCTION: fn_16394
template <>
const char* EncryptedName<48, 0x674D9F3975573D41ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 48, 0x674D9F3975573D41ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A460
// FUNCTION: fn_16395
template <>
const char* EncryptedName<48, 0x6BDF334F2BFDAD51ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 48, 0x6BDF334F2BFDAD51ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A490
// FUNCTION: fn_16396
template <>
const char* EncryptedName<49, 0x659975C121557D3DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 49, 0x659975C121557D3DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A4C0
// FUNCTION: fn_16397
template <>
const char* EncryptedName<50, 0x85E1F5A7A5755B7FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 50, 0x85E1F5A7A5755B7FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A4F0
// FUNCTION: fn_16398
template <>
const char* EncryptedName<51, 0xAD6153019F93A527ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 51, 0xAD6153019F93A527ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A520
// FUNCTION: fn_16399
template <>
const char* EncryptedName<52, 0xBFF5C517A1A77F59ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 52, 0xBFF5C517A1A77F59ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A550
// FUNCTION: fn_16400
EncryptedName<6, 0x114901C7E37F6D49ULL>& GulimFontName1() {
  const std::uint8_t source[] = {0x0E, 0x18, 0x13, 0x8A, 0xAA, 0x01};
  thread_local EncryptedName<sizeof(source), 0x114901C7E37F6D49ULL> name(source);
  return name;
}

// OFFSET: 0x00A5A620
// FUNCTION: fn_16401
EncryptedName<48, 0x136F2F9915D1DBCFULL>& NormalOffButtonName1() {
  const std::uint8_t source[] = {0x0F, 0x74, 0x11, 0xEF, 0x59, 0xE1, 0xAA, 0xDE, 0x09, 0x3F, 0x11, 0xD9, 0x24, 0x95, 0x33, 0x41, 0xAA, 0xB5, 0xB4, 0x62, 0xC6, 0x7E, 0x1A, 0x76, 0xBC, 0xAF, 0x84, 0x5C, 0xC5, 0x4D, 0x1B, 0x7D, 0x90, 0xB5, 0xBE, 0x67, 0xF4, 0x4E, 0x03, 0x4C, 0xA0, 0xBD, 0xB7, 0x3B, 0xFB, 0x42, 0x1F, 0x13};
  thread_local EncryptedName<sizeof(source), 0x136F2F9915D1DBCFULL> name(source);
  return name;
}

// OFFSET: 0x00A5A700
// FUNCTION: fn_16402
EncryptedName<47, 0x0B23D10F2F49D72BULL>& NormalOnButtonName() {
  const std::uint8_t source[] = {0xEB, 0x78, 0x89, 0xD5, 0xCF, 0x1F, 0xE6, 0xC6, 0xED, 0x33, 0x89, 0xE3, 0xB2, 0x6B, 0x7F, 0x59, 0x4E, 0xB9, 0x2C, 0x58, 0x50, 0x80, 0x56, 0x6E, 0x58, 0xA3, 0x1C, 0x66, 0x53, 0xB3, 0x57, 0x65, 0x74, 0xB9, 0x26, 0x5D, 0x62, 0xB0, 0x4F, 0x54, 0x44, 0xB9, 0x67, 0x4D, 0x62, 0xA1, 0x23};
  thread_local EncryptedName<sizeof(source), 0x0B23D10F2F49D72BULL> name(source);
  return name;
}

// OFFSET: 0x00A5A7F0
// FUNCTION: fn_16403
EncryptedName<48, 0x674D9F3975573D41ULL>& NormalOffButtonName2() {
  const std::uint8_t source[] = {0x81, 0x92, 0x97, 0x8F, 0xF9, 0x51, 0x88, 0xAA, 0x87, 0xD9, 0x97, 0xB9, 0x84, 0x25, 0x11, 0x35, 0x24, 0x53, 0x32, 0x02, 0x66, 0xCE, 0x38, 0x02, 0x32, 0x49, 0x02, 0x3C, 0x65, 0xFD, 0x39, 0x09, 0x1E, 0x53, 0x38, 0x07, 0x54, 0xFE, 0x21, 0x38, 0x2E, 0x5B, 0x31, 0x5B, 0x5B, 0xF2, 0x3D, 0x67};
  thread_local EncryptedName<sizeof(source), 0x674D9F3975573D41ULL> name(source);
  return name;
}

// OFFSET: 0x00A5A8D0
// FUNCTION: fn_16404
EncryptedName<48, 0x6BDF334F2BFDAD51ULL>& NormalOffButtonName3() {
  const std::uint8_t source[] = {0x91, 0x02, 0x3D, 0xD1, 0x8F, 0xFD, 0x1A, 0xA6, 0x97, 0x49, 0x3D, 0xE7, 0xF2, 0x89, 0x83, 0x39, 0x34, 0xC3, 0x98, 0x5C, 0x10, 0x62, 0xAA, 0x0E, 0x22, 0xD9, 0xA8, 0x62, 0x13, 0x51, 0xAB, 0x05, 0x0E, 0xC3, 0x92, 0x59, 0x22, 0x52, 0xB3, 0x34, 0x3E, 0xCB, 0x9B, 0x05, 0x2D, 0x5E, 0xAF, 0x6B};
  thread_local EncryptedName<sizeof(source), 0x6BDF334F2BFDAD51ULL> name(source);
  return name;
}

// OFFSET: 0x00A5A9B0
// FUNCTION: fn_16405
EncryptedName<6, 0xB7AFBD2FD9FD7501ULL>& GulimFontName2() {
  const std::uint8_t source[] = {0x46, 0x00, 0x91, 0xB0, 0x42, 0xBD};
  thread_local EncryptedName<sizeof(source), 0xB7AFBD2FD9FD7501ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AA80
// FUNCTION: fn_16406
EncryptedName<45, 0x79E915E9AF6F55DDULL>& SubOffButtonName1() {
  const std::uint8_t source[] = {0x1D, 0xFA, 0xAF, 0x55, 0x29, 0xDB, 0x2C, 0xB4, 0x1B, 0xB1, 0xAF, 0x63, 0x54, 0xAF, 0xB5, 0x2B, 0xB8, 0x3B, 0x0A, 0xD8, 0xB6, 0x44, 0x9C, 0x1C, 0xAE, 0x21, 0x3A, 0xE6, 0xB5, 0x77, 0x9D, 0x17, 0x82, 0x26, 0x1A, 0xCD, 0xB6, 0x7A, 0x8F, 0x1F, 0xF3, 0x37, 0x02, 0xDF, 0xE9};
  thread_local EncryptedName<sizeof(source), 0x79E915E9AF6F55DDULL> name(source);
  return name;
}

// OFFSET: 0x00A5AB70
// FUNCTION: fn_16407
EncryptedName<44, 0xB3E307A5F753D5E3ULL>& SubOnButtonName() {
  const std::uint8_t source[] = {0x23, 0x7A, 0x93, 0x0D, 0x65, 0xC9, 0x26, 0x7E, 0x25, 0x31, 0x93, 0x3B, 0x18, 0xBD, 0xBF, 0xE1, 0x86, 0xBB, 0x36, 0x80, 0xFA, 0x56, 0x96, 0xD6, 0x90, 0xA1, 0x06, 0xBE, 0xF9, 0x65, 0x97, 0xDD, 0xBC, 0xA6, 0x26, 0x95, 0xFA, 0x68, 0x8D, 0x9D, 0x81, 0xB8, 0x23, 0xF7};
  thread_local EncryptedName<sizeof(source), 0xB3E307A5F753D5E3ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AC50
// FUNCTION: fn_16408
EncryptedName<45, 0x192D654D29337BE5ULL>& SubOffButtonName2() {
  const std::uint8_t source[] = {0x25, 0xD4, 0xF3, 0xD3, 0x8D, 0xAB, 0xE8, 0xD4, 0x23, 0x9F, 0xF3, 0xE5, 0xF0, 0xDF, 0x71, 0x4B, 0x80, 0x15, 0x56, 0x5E, 0x12, 0x34, 0x58, 0x7C, 0x96, 0x0F, 0x66, 0x60, 0x11, 0x07, 0x59, 0x77, 0xBA, 0x08, 0x46, 0x4B, 0x12, 0x0A, 0x4B, 0x7F, 0xCB, 0x19, 0x5E, 0x59, 0x4D};
  thread_local EncryptedName<sizeof(source), 0x192D654D29337BE5ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AD40
// FUNCTION: fn_16409
EncryptedName<45, 0xF15F8785CDE369BFULL>& SubOffButtonName3() {
  const std::uint8_t source[] = {0x7F, 0xC6, 0x23, 0x37, 0x45, 0x49, 0x9A, 0x3C, 0x79, 0x8D, 0x23, 0x01, 0x38, 0x3D, 0x03, 0xA3, 0xDA, 0x07, 0x86, 0xBA, 0xDA, 0xD6, 0x2A, 0x94, 0xCC, 0x1D, 0xB6, 0x84, 0xD9, 0xE5, 0x2B, 0x9F, 0xE0, 0x1A, 0x96, 0xAF, 0xDA, 0xE8, 0x39, 0x97, 0x91, 0x0B, 0x8E, 0xBD, 0x85};
  thread_local EncryptedName<sizeof(source), 0xF15F8785CDE369BFULL> name(source);
  return name;
}

// OFFSET: 0x00A5AE30
// FUNCTION: fn_16410
EncryptedName<6, 0xE33F2FF779878559ULL>& GulimFontName3() {
  const std::uint8_t source[] = {0x1E, 0xF0, 0xEB, 0x10, 0x9A, 0x2F};
  thread_local EncryptedName<sizeof(source), 0xE33F2FF779878559ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AF00
// FUNCTION: fn_16411
EncryptedName<47, 0x49D33529F1556333ULL>& DailyOffButtonName1() {
  const std::uint8_t source[] = {0xF3, 0xCC, 0x95, 0x0B, 0xE9, 0xFB, 0x16, 0x84, 0xF5, 0x87, 0x95, 0x3D, 0x94, 0x8F, 0x8F, 0x1B, 0x56, 0x0D, 0x30, 0x86, 0x76, 0x64, 0xA6, 0x2C, 0x40, 0x17, 0x00, 0xB8, 0x75, 0x57, 0xA7, 0x27, 0x6C, 0x07, 0x34, 0x98, 0x45, 0x4C, 0x8C, 0x26, 0x55, 0x05, 0x7B, 0x93, 0x44, 0x45, 0xD3};
  thread_local EncryptedName<sizeof(source), 0x49D33529F1556333ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AFF0
// FUNCTION: fn_16412
EncryptedName<46, 0x792FFD1325418FB5ULL>& DailyOnButtonName() {
  const std::uint8_t source[] = {0x75, 0x20, 0x81, 0xDF, 0xD3, 0x33, 0xEA, 0xB4, 0x73, 0x6B, 0x81, 0xE9, 0xAE, 0x47, 0x73, 0x2B, 0xD0, 0xE1, 0x24, 0x52, 0x4C, 0xAC, 0x5A, 0x1C, 0xC6, 0xFB, 0x14, 0x6C, 0x4F, 0x9F, 0x5B, 0x17, 0xEA, 0xEB, 0x20, 0x4C, 0x7F, 0x84, 0x70, 0x16, 0xDB, 0xA1, 0x23, 0x48, 0x63, 0xFD};
  thread_local EncryptedName<sizeof(source), 0x792FFD1325418FB5ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B0E0
// FUNCTION: fn_16413
EncryptedName<47, 0xC5B1BBCD9BC9C997ULL>& DailyOffButtonName2() {
  const std::uint8_t source[] = {0x57, 0x66, 0x09, 0x61, 0x0D, 0x75, 0x74, 0x08, 0x51, 0x2D, 0x09, 0x57, 0x70, 0x01, 0xED, 0x97, 0xF2, 0xA7, 0xAC, 0xEC, 0x92, 0xEA, 0xC4, 0xA0, 0xE4, 0xBD, 0x9C, 0xD2, 0x91, 0xD9, 0xC5, 0xAB, 0xC8, 0xAD, 0xA8, 0xF2, 0xA1, 0xC2, 0xEE, 0xAA, 0xF1, 0xAF, 0xE7, 0xF9, 0xA0, 0xCB, 0xB1};
  thread_local EncryptedName<sizeof(source), 0xC5B1BBCD9BC9C997ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B1D0
// FUNCTION: fn_16414
EncryptedName<47, 0x01CB4F1DF9D3F931ULL>& DailyOffButtonName3() {
  const std::uint8_t source[] = {0xF1, 0x56, 0x13, 0x03, 0xDD, 0x81, 0x0E, 0xCC, 0xF7, 0x1D, 0x13, 0x35, 0xA0, 0xF5, 0x97, 0x53, 0x54, 0x97, 0xB6, 0x8E, 0x42, 0x1E, 0xBE, 0x64, 0x42, 0x8D, 0x86, 0xB0, 0x41, 0x2D, 0xBF, 0x6F, 0x6E, 0x9D, 0xB2, 0x90, 0x71, 0x36, 0x94, 0x6E, 0x57, 0x9F, 0xFD, 0x9B, 0x70, 0x3F, 0xCB};
  thread_local EncryptedName<sizeof(source), 0x01CB4F1DF9D3F931ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B2C0
// FUNCTION: fn_16415
EncryptedName<6, 0x1767DDE18D779D8BULL>& GulimFontName4() {
  const std::uint8_t source[] = {0xCC, 0xE8, 0x1B, 0xE4, 0x8C, 0xDD};
  thread_local EncryptedName<sizeof(source), 0x1767DDE18D779D8BULL> name(source);
  return name;
}

// OFFSET: 0x00A5B390
// FUNCTION: fn_16416
EncryptedName<47, 0xC1BB8111EDD183E1ULL>& EventOffButtonName1() {
  const std::uint8_t source[] = {0x21, 0x2C, 0x11, 0x17, 0xD1, 0x4F, 0x7E, 0x0C, 0x27, 0x67, 0x11, 0x21, 0xAC, 0x3B, 0xE7, 0x93, 0x84, 0xED, 0xB4, 0x9A, 0x4E, 0xD0, 0xCE, 0xA4, 0x92, 0xF7, 0x84, 0xA4, 0x4D, 0xE3, 0xCF, 0xAF, 0xBE, 0xE6, 0xA7, 0x88, 0x7F, 0xF5, 0xE4, 0xAE, 0x87, 0xE5, 0xFF, 0x8F, 0x7C, 0xF1, 0xBB};
  thread_local EncryptedName<sizeof(source), 0xC1BB8111EDD183E1ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B480
// FUNCTION: fn_16417
EncryptedName<46, 0x271B45771F83DD9FULL>& EventOnButtonName() {
  const std::uint8_t source[] = {0x5F, 0x72, 0x43, 0xE5, 0xB7, 0x8B, 0xDE, 0xEA, 0x59, 0x39, 0x43, 0xD3, 0xCA, 0xFF, 0x47, 0x75, 0xFA, 0xB3, 0xE6, 0x68, 0x28, 0x14, 0x6E, 0x42, 0xEC, 0xA9, 0xD6, 0x56, 0x2B, 0x27, 0x6F, 0x49, 0xC0, 0xB8, 0xF5, 0x7A, 0x19, 0x31, 0x44, 0x48, 0xF1, 0xF3, 0xE1, 0x72, 0x07, 0x45};
  thread_local EncryptedName<sizeof(source), 0x271B45771F83DD9FULL> name(source);
  return name;
}

// OFFSET: 0x00A5B570
// FUNCTION: fn_16418
EncryptedName<47, 0x8783C33941E15117ULL>& EventOffButtonName2() {
  const std::uint8_t source[] = {0xD7, 0xFE, 0x21, 0xBB, 0xF9, 0x0D, 0x46, 0x4A, 0xD1, 0xB5, 0x21, 0x8D, 0x84, 0x79, 0xDF, 0xD5, 0x72, 0x3F, 0x84, 0x36, 0x66, 0x92, 0xF6, 0xE2, 0x64, 0x25, 0xB4, 0x08, 0x65, 0xA1, 0xF7, 0xE9, 0x48, 0x34, 0x97, 0x24, 0x57, 0xB7, 0xDC, 0xE8, 0x71, 0x37, 0xCF, 0x23, 0x54, 0xB3, 0x83};
  thread_local EncryptedName<sizeof(source), 0x8783C33941E15117ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B660
// FUNCTION: fn_16419
EncryptedName<47, 0x0B8745519B1F95D7ULL>& EventOffButtonName3() {
  const std::uint8_t source[] = {0x17, 0x3A, 0xDF, 0x61, 0x91, 0x8B, 0x42, 0xC6, 0x11, 0x71, 0xDF, 0x57, 0xEC, 0xFF, 0xDB, 0x59, 0xB2, 0xFB, 0x7A, 0xEC, 0x0E, 0x14, 0xF2, 0x6E, 0xA4, 0xE1, 0x4A, 0xD2, 0x0D, 0x27, 0xF3, 0x65, 0x88, 0xF0, 0x69, 0xFE, 0x3F, 0x31, 0xD8, 0x64, 0xB1, 0xF3, 0x31, 0xF9, 0x3C, 0x35, 0x87};
  thread_local EncryptedName<sizeof(source), 0x0B8745519B1F95D7ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B750
// FUNCTION: fn_16420
EncryptedName<6, 0xC70319B5EB61598FULL>& GulimFontName5() {
  const std::uint8_t source[] = {0xC8, 0x2C, 0x0D, 0x82, 0xD8, 0x19};
  thread_local EncryptedName<sizeof(source), 0xC70319B5EB61598FULL> name(source);
  return name;
}

// OFFSET: 0x00A5B820
// FUNCTION: fn_16421
EncryptedName<45, 0xFFABE3D5D1BBEF73ULL>& EtcOffButtonName1() {
  const std::uint8_t source[] = {0xB3, 0x40, 0x7B, 0x2B, 0x15, 0x2D, 0x6E, 0x32, 0xB5, 0x0B, 0x7B, 0x1D, 0x68, 0x59, 0xF7, 0xAD, 0x16, 0x81, 0xDE, 0xA6, 0x8A, 0xB2, 0xDE, 0x9A, 0x00, 0x9B, 0xEE, 0x98, 0x89, 0x81, 0xDF, 0x91, 0x2C, 0x8A, 0xCF, 0xB2, 0x8A, 0x8C, 0xCD, 0x99, 0x5D, 0x8D, 0xD6, 0xA1, 0xD5};
  thread_local EncryptedName<sizeof(source), 0xFFABE3D5D1BBEF73ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B910
// FUNCTION: fn_16422
EncryptedName<44, 0xA597A7E7492B39D5ULL>& EtcOnButtonName() {
  const std::uint8_t source[] = {0x15, 0x96, 0xEB, 0xB3, 0x27, 0x69, 0x52, 0x68, 0x13, 0xDD, 0xEB, 0x85, 0x5A, 0x1D, 0xCB, 0xF7, 0xB0, 0x57, 0x4E, 0x3E, 0xB8, 0xF6, 0xE2, 0xC0, 0xA6, 0x4D, 0x7E, 0x00, 0xBB, 0xC5, 0xE3, 0xCB, 0x8A, 0x5C, 0x5F, 0x2A, 0xB8, 0xC8, 0xF9, 0x8B, 0xB7, 0x54, 0x5B, 0x49};
  thread_local EncryptedName<sizeof(source), 0xA597A7E7492B39D5ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B9F0
// FUNCTION: fn_16423
EncryptedName<45, 0xD5A78DB38D8DAFABULL>& EtcOffButtonName2() {
  const std::uint8_t source[] = {0x6B, 0x00, 0x4D, 0x77, 0x73, 0x43, 0x62, 0x18, 0x6D, 0x4B, 0x4D, 0x41, 0x0E, 0x37, 0xFB, 0x87, 0xCE, 0xC1, 0xE8, 0xFA, 0xEC, 0xDC, 0xD2, 0xB0, 0xD8, 0xDB, 0xD8, 0xC4, 0xEF, 0xEF, 0xD3, 0xBB, 0xF4, 0xCA, 0xF9, 0xEE, 0xEC, 0xE2, 0xC1, 0xB3, 0x85, 0xCD, 0xE0, 0xFD, 0xB3};
  thread_local EncryptedName<sizeof(source), 0xD5A78DB38D8DAFABULL> name(source);
  return name;
}

// OFFSET: 0x00A5BAE0
// FUNCTION: fn_16424
EncryptedName<45, 0x0399459DAF454F0FULL>& EtcOffButtonName3() {
  const std::uint8_t source[] = {0xCF, 0xE0, 0x85, 0x55, 0x5D, 0x8B, 0x5C, 0xCE, 0xC9, 0xAB, 0x85, 0x63, 0x20, 0xFF, 0xC5, 0x51, 0x6A, 0x21, 0x20, 0xD8, 0xC2, 0x14, 0xEC, 0x66, 0x7C, 0x3B, 0x10, 0xE6, 0xC1, 0x27, 0xED, 0x6D, 0x50, 0x2A, 0x31, 0xCC, 0xC2, 0x2A, 0xFF, 0x65, 0x21, 0x2D, 0x28, 0xDF, 0x9D};
  thread_local EncryptedName<sizeof(source), 0x0399459DAF454F0FULL> name(source);
  return name;
}

// OFFSET: 0x00A5BBD0
// FUNCTION: fn_16425
EncryptedName<51, 0xAD6153019F93A527ULL>& SearchNormalButtonName() {
  const std::uint8_t source[] = {0xE7, 0x0A, 0x53, 0x65, 0xC1, 0x9D, 0xA4, 0x60, 0xE1, 0x41, 0x53, 0x53, 0xBC, 0xE9, 0x3D, 0xFF, 0x42, 0xCB, 0xF6, 0xE8, 0x5E, 0x02, 0x14, 0xC8, 0x54, 0xD1, 0xC6, 0xD6, 0x5D, 0x31, 0x15, 0xC3, 0x78, 0xD6, 0xF6, 0xFE, 0x73, 0x30, 0x09, 0xF2, 0x49, 0xCA, 0xE1, 0xF2, 0x60, 0x3F, 0x4F, 0xCF, 0x4A, 0xD5, 0x93};
  thread_local EncryptedName<sizeof(source), 0xAD6153019F93A527ULL> name(source);
  return name;
}

// OFFSET: 0x00A5BCB0
// FUNCTION: fn_16426
EncryptedName<50, 0x85E1F5A7A5755B7FULL>& SearchPressButtonName() {
  const std::uint8_t source[] = {0xBF, 0xF4, 0xB5, 0x5F, 0x67, 0x3B, 0x24, 0x48, 0xB9, 0xBF, 0xB5, 0x69, 0x1A, 0x4F, 0xBD, 0xD7, 0x1A, 0x35, 0x10, 0xD2, 0xF8, 0xA4, 0x94, 0xE0, 0x0C, 0x2F, 0x20, 0xEC, 0xFB, 0x97, 0x95, 0xEB, 0x20, 0x28, 0x10, 0xC4, 0xD5, 0x96, 0x89, 0xDA, 0x0F, 0x29, 0x10, 0xD6, 0xD4, 0xDB, 0x83, 0xE8, 0x0F, 0x5B};
  thread_local EncryptedName<sizeof(source), 0x85E1F5A7A5755B7FULL> name(source);
  return name;
}

// OFFSET: 0x00A5BD90
// FUNCTION: fn_16427
EncryptedName<49, 0x659975C121557D3DULL>& SearchOverButtonName() {
  const std::uint8_t source[] = {0xFD, 0xD2, 0x95, 0xDB, 0x01, 0xBB, 0x5C, 0xA8, 0xFB, 0x99, 0x95, 0xED, 0x7C, 0xCF, 0xC5, 0x37, 0x58, 0x13, 0x30, 0x56, 0x9E, 0x24, 0xEC, 0x00, 0x4E, 0x09, 0x00, 0x68, 0x9D, 0x17, 0xED, 0x0B, 0x62, 0x0E, 0x30, 0x40, 0xB3, 0x16, 0xF1, 0x3A, 0x52, 0x0B, 0x30, 0x53, 0xEF, 0x17, 0xF4, 0x15, 0x3D};
  thread_local EncryptedName<sizeof(source), 0x659975C121557D3DULL> name(source);
  return name;
}

// OFFSET: 0x00A5BE70
// FUNCTION: fn_16428
EncryptedName<52, 0xBFF5C517A1A77F59ULL>& SearchDisableButtonName() {
  const std::uint8_t source[] = {0x99, 0xD0, 0x67, 0x5B, 0xD7, 0x0B, 0x30, 0x72, 0x9F, 0x9B, 0x67, 0x6D, 0xAA, 0x7F, 0xA9, 0xED, 0x3C, 0x11, 0xC2, 0xD6, 0x48, 0x94, 0x80, 0xDA, 0x2A, 0x0B, 0xF2, 0xE8, 0x4B, 0xA7, 0x81, 0xD1, 0x06, 0x0C, 0xC2, 0xC0, 0x65, 0xA6, 0x9D, 0xE0, 0x3D, 0x16, 0xD4, 0xC0, 0x75, 0xA9, 0x90, 0x91, 0x3B, 0x12, 0xD7, 0xA1};
  thread_local EncryptedName<sizeof(source), 0xBFF5C517A1A77F59ULL> name(source);
  return name;
}

// OFFSET: 0x00A66BE0
// FUNCTION: fn_16618
void RenewQuestUI::CUIQuestItem::Move(int x, int y) {
  UIWindow::Move(x, y);
  this->baseY = y;
  this->baseX = x;
}

// OFFSET: 0x00A66C50
// FUNCTION: fn_16620
void RenewQuestUI::CUIQuestList::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A66C60
// FUNCTION: fn_16621
void CUIRenewQuestUI::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A66F40
// FUNCTION: fn_16625
void RenewQuestUI::CUIQuestItem::Slot25(int x, int y) {
  CUIListItem::Slot25(x, y);
  this->pressed = 1;
  UIWindow::Move(this->baseX, this->baseY + 1);
  this->Invalidate();
}

// OFFSET: 0x00A66F80
// FUNCTION: fn_16626
void RenewQuestUI::CUIQuestItem::Slot31(int x, int y) {
  CUIListItem::Slot31(x, y);
  this->pressed = 0;
  UIWindow::Move(this->baseX, this->baseY);
  this->Invalidate();
}

// OFFSET: 0x00A66FC0
// FUNCTION: fn_16627
void RenewQuestUI::CUIQuestItem::Slot48() {
  this->pressed = 0;
  UIWindow::Move(this->baseX, this->baseY);
  this->Invalidate();
}

// OFFSET: 0x00A5C170
// FUNCTION: fn_16431
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::ClearItems() {
  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A63F60

// OFFSET: 0x00A64160
// FUNCTION: fn_16445
template <>
__declspec(noinline) void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::SelectItem(CUIListItem* item) {
  if (this->unknown_00C4) {
    return;
  }
  for (RenewQuestUI::CUIQuestItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->unknown_00E0) {
    return;
  }
  RenewQuestUI::CUIQuestItem* selectedItem = dynamic_cast<RenewQuestUI::CUIQuestItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    std::advance(node, static_cast<int>(this->selected));
    this->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A67CE0
// FUNCTION: fn_16631
template <>
bool CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::SelectIndex(int index) {
  this->scrollBar->SetPosition(this->rowHeight * index);
  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    if (index == item->index) {
      this->SelectItem(item);
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00A65FD0
// FUNCTION: fn_16604
template <>
int CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::AddData(const unsigned long& value, bool refresh) {
  this->data.push_back(value);
  if (refresh) {
    this->UpdateScrollBar();
  }
  return this->data.size() - 1;
}

// OFFSET: 0x00A66280
// FUNCTION: fn_16613
void RenewQuestUI::CUIQuestItem::OnSimpleView(bool simpleView) {
  g_renewQuestMgr->SetOngoingSimpleView(this->questId, simpleView);
  g_renewQuestMgr->SaveOngoingSimpleViewList();
  if (UIWindow* window = g_windowMgr.LookupWindow(204)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00A66C70
// FUNCTION: fn_16622
void RenewQuestUI::CUIQuestItem::Slot16() {
  CUIButton* button = this->unknown_00F4;
  std::string tooltip = LookupMessage(3409);
  button->tooltip = tooltip;
}

// OFFSET: 0x00A66D90
// FUNCTION: fn_16623
void RenewQuestUI::CUIQuestList::Slot16() {
  this->list->unknown_00E0 = 0;
}

// OFFSET: 0x00A5C200
// FUNCTION: fn_16432
void CUIRenewQuestUI::Slot53(int value) {
  this->unknown_00B4 = 1;
  this->m_resolveSkin = true;
  this->Resize(381, 466);
  this->Move(
    g_renderer->viewport.centerX - this->geometry.width / 2, g_renderer->viewport.centerY - this->geometry.height / 2
  );
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A67FA0
// FUNCTION: fn_16634
void CUIRenewQuestUI::OnTabButton(CUITabButton* button) {
}

// OFFSET: 0x00A67FB0
// FUNCTION: fn_16635
void CUIRenewQuestUI::OnPanelButton() {
}

template void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::ClearData();

// OFFSET: 0x00A66410
// FUNCTION: fn_16615
bool CUIRenewQuestUI::MatchesFilter(const RenewQuestInfo& info) const {
  if (!g_renewQuestMgr->IsInFilter(this->filter, info.m_icon)) {
    return false;
  }

  if (!this->searchText.empty()) {
    if (FindSubstring(info.unknown_004C.c_str(), static_cast<int>(info.unknown_004C.size()), this->searchText.c_str()) == -1 &&
        FindSubstring(info.m_title.c_str(), static_cast<int>(info.m_title.size()), this->searchText.c_str()) == -1) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00A660E0
// FUNCTION: fn_16605
void CUIRenewQuestUI::OnCallback6() {
}

// OFFSET: 0x00A660F0
// FUNCTION: fn_16606
void CUIRenewQuestUI::OnCallback50() {
}

// OFFSET: 0x00A66110
// FUNCTION: fn_16607
void CUIRenewQuestUI::OnCallback43() {
}

// OFFSET: 0x00A66120
// FUNCTION: fn_16608
void CUIRenewQuestUI::OnCallback80() {
}

// OFFSET: 0x00A66130
// FUNCTION: fn_16609
void CUIRenewQuestUI::OnCallback74() {
}

// OFFSET: 0x00A661E0
// FUNCTION: fn_16611
void CUIRenewQuestUI::OnCallback56() {
}
