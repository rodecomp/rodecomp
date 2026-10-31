#include "UI/Quest/UIRenewQuestUI.h"
#include "Client/ReplayPacketRecorder.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Quest/UIOngoingQuestInfo.h"
#include "UI/Quest/UIRecommendedQuestInfo.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Cipher/XorBytes.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>

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

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const std::uint8_t (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

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

// OFFSET: 0x00A66C10
// FUNCTION: fn_16619
void RenewQuestUI::CUIQuestItem::Paint() {
  if (this->pressed) {
    this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bg_questList_press.bmp"))), false);
  } else {
    CUIListItem::Paint();
  }
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
// FUNCTION: fn_16444
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::OnScroll(int position) {
  auto it = this->items.begin();
  if (it == this->items.end()) {
    return;
  }
  if (this->data.size() == 0) {
    for (; it != this->items.end(); ++it) {
      (*it)->index = -1;
      (*it)->Show(0);
    }
    return;
  }

  const int first = (*it)->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      if (row - first < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      if (first - row < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - (first - row), this->items.end());
      }
    }
  }

  it = this->items.begin();
  for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
    if (it == this->items.end() || row >= this->data.size()) {
      break;
    }
    RenewQuestUI::CUIQuestItem* item = *it;
    if (item) {
      if (this->rebind || row != static_cast<unsigned int>(item->index)) {
        item->index = row;
        if (this->binder) {
          auto node = this->data.begin();
          for (int count = static_cast<int>(row); count > 0; --count) {
            ++node;
          }
          this->binder(item, *node);
        }
      }
      item->SetSelected(row == this->selected);
      item->Move(this->itemX, y);
      item->Show(1);
    }
  }

  for (; it != this->items.end(); ++it) {
    (*it)->index = -1;
    (*it)->Show(0);
  }
}

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

class CQuestItemSelector {
public:
  void SetHighlighted(CUIListItem* const& item);
  void Select(CUIListItem* const& item);

private:
  /* 0000 */ void* unknown_0000;
  /* 0004 */ CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox;
};

// OFFSET: 0x00A64DE0
// FUNCTION: fn_16497
void CQuestItemSelector::SetHighlighted(CUIListItem* const& item) {
  for (RenewQuestUI::CUIQuestItem* entry : this->listBox->items) {
    entry->SetHighlighted(static_cast<CUIListItem*>(entry) == item);
  }
}

// OFFSET: 0x00A64E80
// FUNCTION: fn_16501
void CQuestItemSelector::Select(CUIListItem* const& item) {
  CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = this->listBox;
  CUIListItem* selection = item;
  if (listBox->unknown_00C4) {
    return;
  }
  for (RenewQuestUI::CUIQuestItem* other : listBox->items) {
    if (selection != other) {
      other->SetSelected(0);
    }
  }
  selection->SetSelected(1);
  if (selection->index == static_cast<int>(listBox->selected) && listBox->unknown_00E0) {
    return;
  }
  RenewQuestUI::CUIQuestItem* selectedItem = dynamic_cast<RenewQuestUI::CUIQuestItem*>(selection);
  if (!selectedItem) {
    return;
  }
  listBox->selected = selectedItem->index;
  if (listBox->onSelect && static_cast<int>(listBox->selected) >= 0 && listBox->selected < listBox->data.size()) {
    auto node = listBox->data.begin();
    for (int count = static_cast<int>(listBox->selected); count > 0; --count) {
      ++node;
    }
    listBox->onSelect(selectedItem, *node);
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

// OFFSET: 0x00A66140
// FUNCTION: fn_16610
void RenewQuestUI::CUIQuestItem::OnActiveStateChanged(bool active) {
  if (this->questType == 2) {
    g_renewQuestMgr->SetRecommendedQuestActive(this->questId, active);
    if (auto* window = static_cast<CUIRenewQuestUI*>(g_windowMgr.LookupWindow(0x2718))) {
      window->sub_A67290(this->questType);
    }
    g_renewQuestMgr->SaveRecommendedActiveList();
    return;
  }

#pragma pack(push, 1)
  struct QuestActivePacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ int questId;
    /* 0006 */ bool active;
  };
#pragma pack(pop)

  QuestActivePacket packet;
  packet.questId = this->questId;
  packet.id = 0x02B6;
  packet.active = active;
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
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

// OFFSET: 0x00A67E10
// FUNCTION: fn_16633
void CUIRenewQuestUI::OnFilterButton(UIWindow* button) {
  if (button) {
    this->filter = button->geometry.id;
    this->sub_A67290(0xF);
  }
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

// OFFSET: 0x00A661F0
// FUNCTION: fn_16612
void WindowReplayRecorder::RecordWindowFlag(bool value) {
  this->RecordValues(6, 0xd5, value, 0, 0);
  if (UIWindow* window = g_windowMgr.LookupWindow(0xcc)) {
    window->Show(value);
  }
}

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

// FUNCTION: fn_16628
void CUIRenewQuestUI::sub_A66FF0(unsigned int questId) {
  for (RenewQuestUI::CUIQuestList* list : this->questLists) {
    CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list;
    auto node = listBox->data.begin();
    for (; node != listBox->data.end(); ++node) {
      if (*node == questId) {
        break;
      }
    }

    if (node == listBox->data.end()) {
      continue;
    }

    this->tab->SelectPage(list);
    CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* pageListBox = list->list;

    int index = 0;
    for (auto it = listBox->data.begin(); it != node; ++it) {
      ++index;
    }

    pageListBox->scrollBar->SetPosition(pageListBox->rowHeight * index);

    for (RenewQuestUI::CUIQuestItem* item : pageListBox->items) {
      if (item->index == index) {
        pageListBox->SelectItem(item);
        return;
      }
    }

    return;
  }

  std::string text = std::format("Not found Quest Info = %lu", questId);
  reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
}

// OFFSET: 0x00A59940
// FUNCTION: fn_16362
bool sub_A59940() {
  return true;
}

// OFFSET: 0x00A59950
// FUNCTION: fn_16363
bool sub_A59950() {
  return true;
}

// OFFSET: 0x00A59960
// FUNCTION: fn_16364
bool sub_A59960() {
  return true;
}

// OFFSET: 0x00A599F0
// FUNCTION: fn_16366
template <>
CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A59C80
// FUNCTION: fn_16368
template <>
CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::~CUIListBoxEx() {
}

// OFFSET: 0x00A59BB0
// FUNCTION: fn_16367
CUIEditBoxEx::CUIEditBoxEx(UIWindowContext& context)
    : CUIControl(context), backgroundColor(0xF7F7F7), borderColor(0xC6C6C6), borderWidth(1), editLeft(0), editTop(0), editRight(0), editBottom(0),
      edit(nullptr), onSubmit() {
}

// OFFSET: 0x00A67CA0
// FUNCTION: fn_16630
void RenewQuestUI::CUIQuestItem::SetSelected(bool selected) {
  this->unknown_00EC->SetColor(selected ? 0x00E47B2E : 0x00955322);
  CUIListItem::SetSelected(selected);
}

// OFFSET: 0x00A67D40
// FUNCTION: fn_16632
int CUIRenewQuestUI::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 0xD5) {
      const bool checked = b != 0;
      this->checkBox->checked = checked;
      this->checkBox->Invalidate();
      reinterpret_cast<WindowReplayRecorder*>(this)->RecordValues(6, 0xD5, checked, 0, 0);
      if (UIWindow* window = g_windowMgr.LookupWindow(0xCC)) {
        window->Show(checked);
      }
    }
  } else if (message == 0x16) {
    this->tab->SelectIndex(static_cast<int>(std::log2(static_cast<double>(a))));
  }

  return CUIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00A5C380
// FUNCTION: fn_16433
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A63B90
// FUNCTION: fn_16440
void CUIEditBoxEx::SetOnSubmit(std::function<void()> handler) {
  this->onSubmit = handler;
}

// OFFSET: 0x00A63C00
// FUNCTION: fn_16441
void CUITab::SetOnChange(std::function<void(CUITabButton*)> handler) {
  this->onChange = handler;
}

// OFFSET: 0x00A664C0
// FUNCTION: fn_16616
void RenewQuestUI::CUIQuestList::OnSelectItem(CUIQuestItem* item, const unsigned long& questId) {
  if (item->questId == 0) {
    return;
  }

  if (item->questType == 2) {
    if (CUIRecommendedQuestInfo* info = dynamic_cast<CUIRecommendedQuestInfo*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(0x271a)))) {
      info->SetQuest(item->questId);
      if (UIWindow* other = g_windowMgr.LookupWindow(0x2719)) {
        info->Move(other->geometry.x, other->geometry.y);
        g_windowMgr.CloseWindow(0x2719);
      }
    }
  } else {
    if (CUIOngoingQuestInfo* info = dynamic_cast<CUIOngoingQuestInfo*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(0x2719)))) {
      info->SetQuest(item->questId);
      if (UIWindow* other = g_windowMgr.LookupWindow(0x271a)) {
        info->Move(other->geometry.x, other->geometry.y);
        g_windowMgr.CloseWindow(0x271a);
      }
    }
  }
}

// OFFSET: 0x00A665C0
// FUNCTION: fn_16617
void RenewQuestUI::CUIQuestList::BindItem(CUIQuestItem* item, const unsigned long& questId) {
  item->questType = this->type;
  item->questId = questId;
  if (this->type == 2) {
    auto found = g_renewQuestMgr->m_recommendedQuests.find(questId);
    if (found == g_renewQuestMgr->m_recommendedQuests.end()) {
      return;
    }

    const RenewQuestRecommendedInfo& info = found->second;
    item->icon->SetImage(std::format("유저인터페이스\\renew_questui\\{}", info.m_icon.empty() ? std::string("ico_nq.bmp") : info.m_icon));
    item->icon->tooltip = info.m_iconTooltip;
    item->unknown_00EC->SetText(info.m_title.c_str(), info.m_title.size());
    item->unknown_00F0->SetText(info.unknown_004C.c_str(), info.unknown_004C.size());
    item->unknown_00F4->Show(0);
    item->unknown_00F8->Show(1);
    item->unknown_00F8->checked = info.m_active;
    item->unknown_00F8->Invalidate();
    std::string tooltip = LookupMessage(3408);
    item->unknown_00F8->tooltip = tooltip;
  } else {
    auto found = g_renewQuestMgr->m_ongoingQuests.find(questId);
    if (found == g_renewQuestMgr->m_ongoingQuests.end()) {
      return;
    }

    const RenewQuestOngoingInfo& info = found->second;
    item->icon->SetImage(std::format("유저인터페이스\\renew_questui\\{}", info.m_icon.empty() ? std::string("ico_nq.bmp") : info.m_icon));
    item->icon->tooltip = info.m_iconTooltip;
    item->unknown_00EC->SetText(info.m_title.c_str(), info.m_title.size());
    item->unknown_00F0->SetText(info.unknown_004C.c_str(), info.unknown_004C.size());
    item->unknown_00F4->Show(this->type != 8);
    item->unknown_00F4->checked = info.m_simpleView;
    item->unknown_00F4->Invalidate();
    item->unknown_00F8->Show(this->type != 8);
    item->unknown_00F8->checked = info.m_active;
    item->unknown_00F8->Invalidate();
    std::string tooltip = LookupMessage(3407);
    item->unknown_00F8->tooltip = tooltip;
  }
}

// OFFSET: 0x00A67290
// FUNCTION: fn_16629
void CUIRenewQuestUI::sub_A67290(unsigned int flags) {
  if (flags & 1) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 1) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned int, RenewQuestOngoingInfo> entry : g_renewQuestMgr->m_ongoingQuests) {
          if (this->MatchesFilter(entry.second) && entry.second.unknown_01F8 != 1 && entry.second.m_state != 0) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }

  if (flags & 2) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 2) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned int, RenewQuestRecommendedInfo> entry : g_renewQuestMgr->m_recommendedQuests) {
          if (this->MatchesFilter(entry.second) && entry.second.m_active) {
            listBox->AddData(entry.first, false);
          }
        }
        for (std::pair<const unsigned int, RenewQuestRecommendedInfo> entry : g_renewQuestMgr->m_recommendedQuests) {
          if (this->MatchesFilter(entry.second) && !entry.second.m_active) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }

  if (flags & 4) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 4) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned int, RenewQuestOngoingInfo> entry : g_renewQuestMgr->m_ongoingQuests) {
          if (this->MatchesFilter(entry.second) && entry.second.unknown_01F8 != 1 && entry.second.m_state == 0) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }

  if (flags & 8) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 8) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned int, RenewQuestOngoingInfo> entry : g_renewQuestMgr->m_ongoingQuests) {
          if (this->MatchesFilter(entry.second) && entry.second.unknown_01F8 == 1) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }
}

// OFFSET: 0x00A5C0F0
// FUNCTION: fn_16430
CUIFrameWnd* CreateRenewQuestUI() {
  return new CUIRenewQuestUI();
}

// OFFSET: 0x00A5C590
// FUNCTION: fn_16434
void RenewQuestUI::CUIQuestItem::Slot15(int width, int height) {
  this->icon = new CUIStaticImage(*this->m_context);
  this->icon->SetSize(26, 29);
  this->icon->Move(8, 7);
  this->icon->SetImage(std::string());
  this->AddChild(this->icon);

  this->unknown_00EC = new CUIStaticText(*this->m_context);
  this->unknown_00EC->SetSize(260, 12);
  this->unknown_00EC->Move(42, 6);
  this->unknown_00EC->SetText("", 0);
  this->unknown_00EC->SetFitText(1);
  this->unknown_00EC->SetAlign(0, 0);
  const std::uint8_t titleFont[] = {0x8A, 0x84, 0xF5, 0xDC, 0xE0, 0x5B};
  this->unknown_00EC->SetFont(std::string(DecryptName<0xAD075B8DB599F1CDULL>(titleFont)));
  this->unknown_00EC->SetFontHeight(12);
  this->unknown_00EC->SetBold(1);
  this->unknown_00EC->SetColor(0x955322);
  this->unknown_00EC->SetShadow(0, 0xFFFFFF);
  this->unknown_00EC->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00EC);

  this->unknown_00F0 = new CUIStaticText(*this->m_context);
  this->unknown_00F0->SetSize(260, 12);
  this->unknown_00F0->Move(42, 24);
  this->unknown_00F0->SetText("", 0);
  this->unknown_00F0->SetFitText(1);
  this->unknown_00F0->SetAlign(0, 0);
  const std::uint8_t summaryFont[] = {0xA6, 0x0E, 0x95, 0xBE, 0xB8, 0x13};
  this->unknown_00F0->SetFont(std::string(DecryptName<0xDD3913D5D7F97BE1ULL>(summaryFont)));
  this->unknown_00F0->SetFontHeight(12);
  this->unknown_00F0->SetBold(0);
  this->unknown_00F0->SetColor(0x38302E);
  this->unknown_00F0->SetShadow(0, 0xFFFFFF);
  this->unknown_00F0->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00F0);

  this->unknown_00F4 = new CUICheckBox(*this->m_context);
  this->unknown_00F4->SetSize(14, 14);
  this->unknown_00F4->Move(312, 3);
  this->unknown_00F4->SetOnCheck([this](bool checked) { this->OnSimpleView(checked); });
  this->unknown_00F4->disabled = 0;
  this->unknown_00F4->Invalidate();
  const std::uint8_t checkInactive[] = {0xC9, 0x88, 0xEF, 0x41, 0x7D, 0x93, 0xB8, 0x14, 0xCF, 0xC3, 0xEF, 0x77, 0x00, 0xE7, 0x21, 0x8B,
                                        0x6C, 0x49, 0x4A, 0xCC, 0xE2, 0x0C, 0x08, 0xBC, 0x7A, 0x53, 0x7A, 0xF2, 0xE1, 0x3F, 0x09, 0x86,
                                        0x6A, 0x4F, 0x4A, 0xD8, 0xD6, 0x02, 0x12, 0xBF, 0x6F, 0x09, 0x4D, 0xD6, 0xCD, 0x5D};
  const std::uint8_t checkPressed[] = {0x67, 0x5A, 0xBF, 0xC3, 0xF1, 0x85, 0xEE, 0x72, 0x61, 0x11, 0xBF, 0xF5, 0x8C, 0xF1, 0x77, 0xED,
                                       0xC2, 0x9B, 0x1A, 0x4E, 0x6E, 0x1A, 0x5E, 0xDA, 0xD4, 0x81, 0x2A, 0x70, 0x6D, 0x29, 0x5F, 0xE0,
                                       0xC4, 0x9D, 0x1A, 0x5A, 0x5A, 0x14, 0x44, 0xD9, 0xC1, 0xDB, 0x1D, 0x54, 0x41, 0x4B};
  const std::uint8_t checkHover[] = {0x7D, 0x12, 0x2D, 0xCD, 0x7B, 0x71, 0x2A, 0x4E, 0x7B, 0x59, 0x2D, 0xFB, 0x06, 0x05, 0xB3, 0xD1,
                                     0xD8, 0xD3, 0x88, 0x40, 0xE4, 0xEE, 0x9A, 0xE6, 0xCE, 0xC9, 0xB8, 0x7E, 0xE7, 0xDD, 0x9B, 0xDC,
                                     0xDE, 0xD5, 0x88, 0x54, 0xD0, 0xE0, 0x80, 0xED, 0x93, 0xDF, 0x80, 0x47, 0xBB};
  const std::uint8_t checkNormal[] = {0x5B, 0xF8, 0xC3, 0x63, 0xA7, 0xCB, 0xC0, 0xC8, 0x5D, 0xB3, 0xC3, 0x55, 0xDA, 0xBF, 0x59, 0x57,
                                      0xFE, 0x39, 0x66, 0xEE, 0x38, 0x54, 0x70, 0x60, 0xE8, 0x23, 0x56, 0xD0, 0x3B, 0x67, 0x71, 0x5A,
                                      0xF8, 0x3F, 0x66, 0xFA, 0x0C, 0x5A, 0x6A, 0x63, 0xFD, 0x79, 0x61, 0xF4, 0x17, 0x05};
  this->unknown_00F4->SetImages(
    std::string(DecryptName<0x050505679903579BULL>(checkNormal)), std::string(DecryptName<0x83EFBFBB37EDBDBDULL>(checkHover)),
    std::string(DecryptName<0xBF2B4B31397FF5A7ULL>(checkPressed)), std::string(DecryptName<0xD97D5DBDBB2F2709ULL>(checkInactive))
  );
  this->unknown_00F4->checked = false;
  this->unknown_00F4->Invalidate();
  this->AddChild(this->unknown_00F4);

  this->unknown_00F8 = new CUICheckBox(*this->m_context);
  this->unknown_00F8->SetSize(20, 17);
  this->unknown_00F8->Move(306, 22);
  this->unknown_00F8->SetOnCheck([this](bool checked) { this->OnActiveStateChanged(checked); });
  this->unknown_00F8->disabled = 0;
  this->unknown_00F8->Invalidate();
  const std::uint8_t lockInactive[] = {0xA7, 0x04, 0x51, 0x5F, 0x61, 0xF1, 0x26, 0xDE, 0xA1, 0x4F, 0x51, 0x69, 0x1C, 0x85, 0xBF, 0x41,
                                       0x02, 0xC5, 0xF4, 0xD2, 0xFE, 0x6E, 0x96, 0x76, 0x14, 0xDF, 0xC4, 0xEC, 0xFD, 0x5D, 0x97, 0x4C,
                                       0x0B, 0xC4, 0xF2, 0xCE, 0x8F, 0x5D, 0x8E, 0x63, 0x67};
  const std::uint8_t lockPressed[] = {0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xF7,
                                      0xFA, 0xAB, 0x08, 0xDC, 0x88, 0x78, 0x88, 0xC0, 0xEC, 0xB1, 0x38, 0xE2, 0x8B, 0x4B, 0x89, 0xFA,
                                      0xF3, 0xAA, 0x0E, 0xC0, 0xF9, 0x4B, 0x90, 0xD5, 0x9F};
  const std::uint8_t lockHover[] = {0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0xA3,
                                    0x94, 0xA9, 0x5C, 0x7E, 0x48, 0x28, 0x3A, 0x94, 0x82, 0xB3, 0x6C, 0x40, 0x4B, 0x1B, 0x3B, 0xAE,
                                    0x9D, 0xA8, 0x5A, 0x62, 0x48, 0x16, 0x3F, 0x94, 0x9F, 0xE9, 0x5B, 0x64, 0x67, 0x79};
  const std::uint8_t lockNormal[] = {0xBF, 0xBC, 0x1F, 0x2B, 0x45, 0xA9, 0xBC, 0x20, 0xB9, 0xF7, 0x1F, 0x1D, 0x38, 0xDD, 0x25, 0xBF,
                                     0x1A, 0x7D, 0xBA, 0xA6, 0xDA, 0x36, 0x0C, 0x88, 0x0C, 0x67, 0x8A, 0x98, 0xD9, 0x05, 0x0D, 0xB2,
                                     0x13, 0x7C, 0xBC, 0xBA, 0xAB, 0x05, 0x14, 0x9D, 0x7F};
  this->unknown_00F8->SetImages(
    std::string(DecryptName<0xED796785D1DF137FULL>(lockNormal)), std::string(DecryptName<0xF14F79170939C7F1ULL>(lockHover)),
    std::string(DecryptName<0xA5FD29D7AB6DC59FULL>(lockPressed)), std::string(DecryptName<0x13E33FA1A591AB67ULL>(lockInactive))
  );
  this->unknown_00F8->checked = false;
  this->unknown_00F8->Invalidate();
  this->AddChild(this->unknown_00F8);
}

// OFFSET: 0x00A5D850
// FUNCTION: fn_16435
void RenewQuestUI::CUIQuestList::Slot15(int width, int height) {
  this->list = new CUIListBoxEx<CUIQuestItem, unsigned long>(*this->m_context);
  this->list->itemWidth = 330;
  this->list->rowHeight = 46;
  this->list->SetSize(343, 394);
  this->list->Move(0, 0);
  this->list->SetOnSelect([this](CUIQuestItem* item, const unsigned long& questId) { this->OnSelectItem(item, questId); });
  this->list->SetBinder([this](CUIQuestItem* item, const unsigned long& questId) { this->BindItem(item, questId); });
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    const std::uint8_t disabledSource[] = {
      0x09, 0x5A, 0x9F, 0x71, 0x4F, 0x1B, 0x7E, 0xEE, 0x0F, 0x11, 0x9F, 0x47, 0x32, 0x6F, 0xE7, 0x71,
      0xAC, 0x9B, 0x3A, 0xFC, 0xD0, 0x84, 0xCE, 0x46, 0xBA, 0x81, 0x0A, 0xC2, 0xD3, 0xB7, 0xDC, 0x7C,
      0xB8, 0x80, 0x3A, 0xF8, 0xFB, 0x99, 0xD2, 0x50, 0xBD, 0xDB, 0x3D, 0xE6, 0xFF, 0xD5
    };
    const std::string disabled(DecryptName<0x23BBD58F8B5FF5C9ULL>(disabledSource));
    const std::uint8_t pressedSource[] = {
      0xAF, 0xD2, 0x3B, 0xD9, 0xB1, 0xC9, 0x4A, 0x00, 0xA9, 0x99, 0x3B, 0xEF, 0xCC, 0xBD, 0xD3, 0x9F,
      0x0A, 0x13, 0x9E, 0x54, 0x2E, 0x56, 0xFA, 0xA8, 0x1C, 0x09, 0xAE, 0x6A, 0x2D, 0x65, 0xE8, 0x92,
      0x1E, 0x08, 0x9E, 0x50, 0x05, 0x4B, 0xE6, 0xBE, 0x1B, 0x53, 0x99, 0x4E, 0x01, 0x07
    };
    const std::string pressed(DecryptName<0xCD8F077123FB7D6FULL>(pressedSource));
    const std::uint8_t hoverSource[] = {
      0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xCB,
      0x26, 0x47, 0x36, 0x50, 0x8A, 0xEE, 0x3C, 0xFC, 0x30, 0x5D, 0x06, 0x6E, 0x89, 0xDD, 0x2E, 0xC6,
      0x32, 0x5C, 0x36, 0x54, 0xA1, 0xF3, 0x20, 0xEA, 0x37, 0x76, 0x30, 0x4F, 0xB0, 0xDC, 0x22, 0xB7,
      0x21, 0x44, 0x23, 0x27
    };
    const std::string hover(DecryptName<0x9949BFD527532943ULL>(hoverSource));
    const std::uint8_t normalSource[] = {
      0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x45,
      0x52, 0x29, 0xC8, 0x4E, 0xE0, 0xB4, 0xDC, 0x72, 0x44, 0x33, 0xF8, 0x70, 0xE3, 0x87, 0xCE, 0x48,
      0x46, 0x32, 0xC8, 0x4A, 0xCB, 0xA9, 0xC0, 0x64, 0x43, 0x69, 0xCF, 0x54, 0xCF, 0xE5
    };
    const std::string normal(DecryptName<0x17A9E5BF39AD4737ULL>(normalSource));
    list->SetImages(normal, hover, pressed, disabled);
  }
  this->list->itemX = 0;
  this->list->itemY = 7;
  this->list->scrollBar->SetSize(7, 394);
  this->list->scrollBar->Move(336, 0);
  this->list->scrollBar->step = 138;
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    const std::uint8_t downSource[] = {
      0x65, 0x32, 0xAD, 0x03, 0xA3, 0xE9, 0xB2, 0xCE, 0x63, 0x79, 0xAD, 0x35, 0xDE, 0x9D, 0x2B, 0x51,
      0xC0, 0xF3, 0x08, 0x8E, 0x3C, 0x76, 0x02, 0x66, 0xD6, 0xE9, 0x38, 0xB0, 0x3F, 0x45, 0x03, 0x5C,
      0xD6, 0xFE, 0x1F, 0x96, 0x0F, 0x4B, 0x15, 0x62, 0xD7, 0xC2, 0x0C, 0x8B, 0x11, 0x48, 0x00, 0x5C,
      0xC1, 0xF2, 0x1A, 0x97, 0x4D, 0x45, 0x1A, 0x73, 0xA5
    };
    const std::string down(DecryptName<0x03772763F96D9DA5ULL>(downSource));
    const std::uint8_t upSource[] = {
      0xAD, 0x14, 0x55, 0xD7, 0x53, 0x35, 0xD2, 0x08, 0xAB, 0x5F, 0x55, 0xE1, 0x2E, 0x41, 0x4B, 0x97,
      0x08, 0xD5, 0xF0, 0x5A, 0xCC, 0xAA, 0x62, 0xA0, 0x1E, 0xCF, 0xC0, 0x64, 0xCF, 0x99, 0x63, 0x9A,
      0x1E, 0xD8, 0xE7, 0x42, 0xFF, 0x97, 0x75, 0xA4, 0x1F, 0xE4, 0xF4, 0x5F, 0xE1, 0x94, 0x60, 0x9A,
      0x18, 0xCB, 0xE5, 0x48, 0xE1, 0xD5, 0x75, 0xA8, 0x1D, 0xBB
    };
    const std::string up(DecryptName<0xC517FB932D95BB6DULL>(upSource));
    list->scrollBar->SetButtonImages(up, down);
  }
  this->list->scrollBar->SetBackground(std::string());
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    const std::uint8_t bottomSource[] = {
      0x2F, 0xDE, 0x1F, 0xE9, 0xAD, 0x81, 0x02, 0x20, 0x29, 0x95, 0x1F, 0xDF, 0xD0, 0xF5, 0x9B, 0xBF,
      0x8A, 0x1F, 0xBA, 0x64, 0x32, 0x1E, 0xB2, 0x88, 0x9C, 0x05, 0x8A, 0x5A, 0x31, 0x2D, 0xB3, 0xB2,
      0x9C, 0x12, 0xAD, 0x7C, 0x01, 0x23, 0x85, 0x8C, 0x9D, 0x2E, 0xBB, 0x7C, 0x1A, 0x21, 0xE9, 0x8F,
      0x82, 0x01, 0xDF
    };
    const std::string bottom(DecryptName<0xEDC74F6D13DF71EFULL>(bottomSource));
    const std::uint8_t middleSource[] = {
      0x55, 0x20, 0x21, 0xF7, 0x9D, 0xC7, 0xA4, 0x3C, 0x53, 0x6B, 0x21, 0xC1, 0xE0, 0xB3, 0x3D, 0xA3,
      0xF0, 0xE1, 0x84, 0x7A, 0x02, 0x58, 0x14, 0x94, 0xE6, 0xFB, 0xB4, 0x44, 0x01, 0x6B, 0x15, 0xAE,
      0xE6, 0xEC, 0x93, 0x62, 0x31, 0x65, 0x23, 0x90, 0xE7, 0xD0, 0x8C, 0x64, 0x39, 0x27, 0x03, 0x9C,
      0xE5, 0x8F
    };
    const std::string middle(DecryptName<0xF161095D0DE18F95ULL>(middleSource));
    const std::uint8_t topSource[] = {
      0x47, 0xBC, 0xAB, 0x85, 0xF9, 0xC7, 0xB4, 0xC6, 0x41, 0xF7, 0xAB, 0xB3, 0x84, 0xB3, 0x2D, 0x59,
      0xE2, 0x7D, 0x0E, 0x08, 0x66, 0x58, 0x04, 0x6E, 0xF4, 0x67, 0x3E, 0x36, 0x65, 0x6B, 0x05, 0x54,
      0xF4, 0x70, 0x19, 0x10, 0x55, 0x65, 0x33, 0x6A, 0xF5, 0x4C, 0x1E, 0x0F, 0x17, 0x6B, 0x1C, 0x7B,
      0x87
    };
    const std::string top(DecryptName<0x0B7109397F6B1387ULL>(topSource));
    list->scrollBar->SetThumbImages(top, middle, bottom);
  }
  this->list->AddNewItem();
  this->AddChild(this->list);
}

// OFFSET: 0x00A5E8C0
// FUNCTION: fn_16436
void CUIRenewQuestUI::Slot15(int width, int height) {
  this->titleBar = new CUITitleBar(*this->m_context);
  this->titleBar->SetSize(352, 17);
  this->titleBar->Move(29, 0);
  const std::uint8_t titleRight[] = {
    0xA9, 0xB0, 0xB7, 0xAF, 0x5D, 0x5B, 0xBE, 0xAE, 0xAF, 0xFB, 0xB7, 0x99, 0x20, 0x2F, 0x27, 0x01,
    0x08, 0x6C, 0x1E, 0x36, 0xC2, 0xFC, 0x15, 0x17, 0x0C, 0x6D, 0x11, 0x34, 0xFE, 0xF0, 0x27, 0x17,
    0x00, 0x6B, 0x1B, 0x30, 0xFF, 0xF4, 0x09, 0x3C, 0x1B, 0x76, 0x10, 0x3D, 0xE9, 0xBB, 0x19, 0x0E,
    0x19, 0x1F
  };
  const std::uint8_t titleMiddle[] = {
    0x19, 0x56, 0x21, 0x19, 0xAD, 0xDB, 0x68, 0xE6, 0x1F, 0x1D, 0x21, 0x2F, 0xD0, 0xAF, 0xF1, 0x49,
    0xB8, 0x8A, 0x88, 0x80, 0x32, 0x7C, 0xC3, 0x5F, 0xBC, 0x8B, 0x87, 0x82, 0x0E, 0x70, 0xF1, 0x5F,
    0xB0, 0x8D, 0x8D, 0x86, 0x0F, 0x74, 0xDF, 0x74, 0xB4, 0x90, 0x85, 0xCD, 0x0F, 0x78, 0xDD, 0x2B
  };
  const std::uint8_t titleLeft[] = {
    0x4F, 0x12, 0x2F, 0x7D, 0xBD, 0x0B, 0x68, 0xB8, 0x49, 0x59, 0x2F, 0x4B, 0xC0, 0x7F, 0xF1, 0x17,
    0xEE, 0xCE, 0x86, 0xE4, 0x22, 0xAC, 0xC3, 0x01, 0xEA, 0xCF, 0x89, 0xE6, 0x1E, 0xA0, 0xF1, 0x01,
    0xE6, 0xC9, 0x83, 0xE2, 0x1F, 0xA4, 0xDF, 0x2A, 0xE3, 0xD8, 0x89, 0xF3, 0x53, 0xA7, 0xC0, 0x05,
    0x8F
  };
  this->titleBar->SetImages(std::string(DecryptName<0x75ADC57D87EFBD8FULL>(titleLeft)), std::string(DecryptName<0x2BAD156DE3E1F9D9ULL>(titleMiddle)), std::string(DecryptName<0x637B959D55771F69ULL>(titleRight)));
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(100, 17);
  title->Move(18, 0);
  const std::uint8_t questTitle[] = {0x00, 0xDE, 0x88, 0x52, 0xEF, 0x3D};
  const char* questText = DecryptName<0xD1413DBB01CD8B51ULL>(questTitle);
  title->SetText(questText, std::strlen(questText));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  const std::uint8_t titleFont[] = {0xFE, 0x70, 0x1B, 0x26, 0x66, 0x1D};
  title->SetFont(std::string(DecryptName<0x758F1D0B4F7705B9ULL>(titleFont)));
  title->SetFontHeight(13);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(0, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* systemButton = new CUIImageButton(*this->m_context);
  systemButton->SetSize(11, 11);
  systemButton->Move(3, 3);
  systemButton->SetOnClick([this]() { this->OnCallback6(); });
  systemButton->disabled = 0;
  systemButton->Invalidate();
  const std::uint8_t systemInactive[] = {
    0xB7, 0x38, 0x47, 0xE7, 0xE3, 0x9D, 0x24, 0xA2, 0xB1, 0x73, 0x47, 0xD1, 0x9E, 0xE9, 0xBD, 0x0D,
    0x16, 0xE4, 0xEE, 0x7E, 0x7C, 0x3A, 0x8F, 0x1B, 0x12, 0xE5, 0xE1, 0x7C, 0x40, 0x36, 0xBD, 0x1C,
    0x0E, 0xE4, 0xD8, 0x7F, 0x42, 0x20, 0x84, 0x30, 0x18, 0xF1, 0xE1, 0x33, 0x41, 0x3E, 0x91, 0x6F
  };
  const std::uint8_t systemPressed[] = {
    0xDD, 0x68, 0x8B, 0x4F, 0x1D, 0xB9, 0x8C, 0xEE, 0xDB, 0x23, 0x8B, 0x79, 0x60, 0xCD, 0x15, 0x41,
    0x7C, 0xB4, 0x22, 0xD6, 0x82, 0x1E, 0x27, 0x57, 0x78, 0xB5, 0x2D, 0xD4, 0xBE, 0x12, 0x15, 0x50,
    0x64, 0xB4, 0x14, 0xD7, 0xBC, 0x04, 0x2C, 0x7C, 0x72, 0xA9, 0x65, 0xD7, 0xB0, 0x07, 0x49
  };
  const std::uint8_t systemHover[] = {
    0x05, 0x82, 0xEB, 0xC5, 0x49, 0xD3, 0xA8, 0x22, 0x03, 0xC9, 0xEB, 0xF3, 0x34, 0xA7, 0x31, 0x8D,
    0xA4, 0x5E, 0x42, 0x5C, 0xD6, 0x74, 0x03, 0x9B, 0xA0, 0x5F, 0x4D, 0x5E, 0xEA, 0x78, 0x31, 0x9C,
    0xBC, 0x5E, 0x74, 0x5D, 0xE8, 0x6E, 0x08, 0xB0, 0xAA, 0x43, 0x05, 0x5D, 0xE4, 0x6D, 0x6D
  };
  const std::uint8_t systemNormal[] = {
    0xA5, 0xB6, 0x85, 0xF3, 0xED, 0xED, 0xDE, 0x44, 0xA3, 0xFD, 0x85, 0xC5, 0x90, 0x99, 0x47, 0xEB,
    0x04, 0x6A, 0x2C, 0x6A, 0x72, 0x4A, 0x75, 0xFD, 0x00, 0x6B, 0x23, 0x68, 0x4E, 0x46, 0x47, 0xFA,
    0x1C, 0x6A, 0x1A, 0x6B, 0x4C, 0x50, 0x7E, 0xD6, 0x0A, 0x7F, 0x23, 0x27, 0x4F, 0x4E, 0x6B, 0x89
  };
  systemButton->SetImages(
    std::string(DecryptName<0x891B232D09451965ULL>(systemNormal)), std::string(DecryptName<0xEF6D1D893F2B2DC5ULL>(systemHover)),
    std::string(DecryptName<0x234977DDB54BC71DULL>(systemPressed)), std::string(DecryptName<0x6FE153231D879777ULL>(systemInactive))
  );
  this->titleBar->AddChild(systemButton);

  this->background = new CUIStaticImage(*this->m_context);
  this->background->SetSize(352, 449);
  this->background->Move(29, 17);
  const std::uint8_t backgroundImage[] = {
    0x9B, 0xFC, 0x9F, 0x27, 0x47, 0x51, 0x66, 0xF4, 0x9D, 0xB7, 0x9F, 0x11, 0x3A, 0x25, 0xFF, 0x6B,
    0x3E, 0x3D, 0x3A, 0xAA, 0xD8, 0xCE, 0xD6, 0x5C, 0x28, 0x27, 0x0A, 0x94, 0xDB, 0xFD, 0xC4, 0x66,
    0x2A, 0x26, 0x3A, 0xAE, 0xF3, 0xB1, 0xC1, 0x54, 0x2B, 0x53
  };
  this->background->SetImage(std::string(DecryptName<0x39A39F87DD5F535BULL>(backgroundImage)));
  this->AddChild(this->background);

  this->tab = new CUITab(*this->m_context);
  this->tab->SetSize(30, 403);
  this->tab->Move(0, 17);
  this->tab->SetOnChange([this](CUITabButton* button) { this->OnTabButton(button); });
  this->AddChild(this->tab);

  CUITabButton* processButton = new CUITabButton(*this->m_context);
  processButton->SetSize(30, 100);
  processButton->Move(0, 0);
  processButton->SetOnClick([this]() {
    reinterpret_cast<WindowReplayRecorder*>(this)->RecordValues(0x16, 1, 0, 0, 0);
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type == 1) {
        if (list->list) {
          list->list->scrollBar->SetPosition(0);
        }
        return;
      }
    }
  });
  processButton->disabled = 0;
  processButton->Invalidate();
  const std::uint8_t processFont[] = {0x96, 0x2E, 0xFB, 0x8C, 0x54, 0x87};
  processButton->SetText(std::string(""));
  processButton->SetFitText(0);
  processButton->SetAlign(0, 0);
  processButton->SetFont(std::string(DecryptName<0x2FEB8739E5975BD1ULL>(processFont)));
  processButton->SetFontHeight(12);
  processButton->SetBold(0);
  processButton->SetTextColors(0, 0, 0, 0);
  processButton->SetShadow(0, 0);
  processButton->SetOutline(0, 0);
  const std::uint8_t processInactive[] = {
    0x6B, 0x6E, 0x8D, 0xC3, 0xA7, 0xBB, 0x0E, 0xE0, 0x6D, 0x25, 0x8D, 0xF5, 0xDA, 0xCF, 0x97, 0x7F,
    0xCE, 0xAF, 0x28, 0x4E, 0x38, 0x24, 0xBE, 0x48, 0xD8, 0xB5, 0x18, 0x70, 0x3B, 0x01, 0xAA, 0x4F,
    0xF4, 0xB1, 0x3F, 0x56, 0x04, 0x10, 0xB8, 0x5E, 0xF4, 0xAE, 0x2B, 0x5F, 0x49, 0x17, 0xA6, 0x5D,
    0xAB
  };
  const std::uint8_t processPressed[] = {
    0xB5, 0x94, 0xC3, 0xD5, 0xD3, 0x0F, 0x64, 0xA6, 0xB3, 0xDF, 0xC3, 0xE3, 0xAE, 0x7B, 0xFD, 0x39,
    0x10, 0x55, 0x66, 0x58, 0x4C, 0x90, 0xD4, 0x0E, 0x06, 0x4F, 0x56, 0x66, 0x4F, 0xB5, 0xC0, 0x09,
    0x2A, 0x4B, 0x71, 0x40, 0x70, 0xA4, 0xD2, 0x18, 0x2A, 0x54, 0x65, 0x49, 0x3D, 0xA3, 0xCC, 0x1B,
    0x75
  };
  const std::uint8_t processHover[] = {
    0xEF, 0x78, 0x85, 0xDF, 0xF9, 0x65, 0xE8, 0x6C, 0xE9, 0x33, 0x85, 0xE9, 0x84, 0x11, 0x71, 0xF3,
    0x4A, 0xB9, 0x20, 0x52, 0x66, 0xFA, 0x58, 0xC4, 0x5C, 0xA3, 0x10, 0x6C, 0x65, 0xDF, 0x4C, 0xC3,
    0x70, 0xA7, 0x37, 0x4A, 0x5A, 0xCE, 0x5E, 0xD2, 0x70, 0xB8, 0x2B, 0x0B, 0x5B, 0xC6, 0x5D, 0xA1
  };
  const std::uint8_t processNormal[] = {
    0x0D, 0xAC, 0x0D, 0x81, 0x57, 0xF5, 0xD6, 0xFE, 0x0B, 0xE7, 0x0D, 0xB7, 0x2A, 0x81, 0x4F, 0x61,
    0xA8, 0x6D, 0xA8, 0x0C, 0xC8, 0x6A, 0x66, 0x56, 0xBE, 0x77, 0x98, 0x32, 0xCB, 0x4F, 0x72, 0x51,
    0x92, 0x73, 0xBF, 0x14, 0xF4, 0x5E, 0x60, 0x40, 0x92, 0x6C, 0xAB, 0x1D, 0xB9, 0x59, 0x7E, 0x43,
    0xCD
  };
  processButton->SetImages(
    std::string(DecryptName<0x33133B977BCD03CDULL>(processNormal)), std::string(DecryptName<0xA12DAB392545D72FULL>(processHover)),
    std::string(DecryptName<0x6BA1C1132F033B75ULL>(processPressed)), std::string(DecryptName<0x2DCB7567394DC1ABULL>(processInactive))
  );
  this->tab->AddChild(processButton);

  CUITabButton* recommendedButton = new CUITabButton(*this->m_context);
  recommendedButton->SetSize(30, 100);
  recommendedButton->Move(0, 101);
  recommendedButton->SetOnClick([this]() {
    reinterpret_cast<WindowReplayRecorder*>(this)->RecordValues(0x16, 2, 0, 0, 0);
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type == 2) {
        if (list->list) {
          // GLOBAL: gl_25249
          static bool firstSelect = true;
          if (firstSelect) {
            list->list->SelectIndex(0);
            firstSelect = false;
            return;
          }
          list->list->scrollBar->SetPosition(0);
        }
        return;
      }
    }
  });
  recommendedButton->disabled = 0;
  recommendedButton->Invalidate();
  const std::uint8_t recommendedFont[] = {0x1E, 0xC2, 0x6B, 0x7A, 0xA8, 0x7D};
  recommendedButton->SetText(std::string(""));
  recommendedButton->SetFitText(0);
  recommendedButton->SetAlign(0, 0);
  recommendedButton->SetFont(std::string(DecryptName<0x4F637DC51307B759ULL>(recommendedFont)));
  recommendedButton->SetFontHeight(12);
  recommendedButton->SetBold(0);
  recommendedButton->SetTextColors(0, 0, 0, 0);
  recommendedButton->SetShadow(0, 0);
  recommendedButton->SetOutline(0, 0);
  const std::uint8_t recommendedInactive[] = {
    0x77, 0x9E, 0x2F, 0x39, 0xC3, 0x11, 0x5C, 0x82, 0x71, 0xD5, 0x2F, 0x0F, 0xBE, 0x65, 0xC5, 0x1D,
    0xD2, 0x5F, 0x8A, 0xB4, 0x5C, 0x8E, 0xEC, 0x2A, 0xC4, 0x45, 0xBA, 0x8A, 0x5F, 0xAB, 0xF8, 0x2D,
    0xE8, 0x43, 0x8A, 0xA0, 0x6C, 0xB2, 0xF4, 0x2A, 0xD9, 0x55, 0x8A, 0xA7, 0x5C, 0xB0, 0xFF, 0x29,
    0x99, 0x53, 0x82, 0xB3, 0x03
  };
  const std::uint8_t recommendedPressed[] = {
    0x37, 0x66, 0xA3, 0x93, 0xBD, 0x57, 0xB4, 0x5C, 0x31, 0x2D, 0xA3, 0xA5, 0xC0, 0x23, 0x2D, 0xC3,
    0x92, 0xA7, 0x06, 0x1E, 0x22, 0xC8, 0x04, 0xF4, 0x84, 0xBD, 0x36, 0x20, 0x21, 0xED, 0x10, 0xF3,
    0xA8, 0xBB, 0x06, 0x0A, 0x12, 0xF4, 0x1C, 0xF4, 0x99, 0xAD, 0x06, 0x0D, 0x22, 0xF6, 0x17, 0xF7,
    0xD9, 0xAB, 0x0E, 0x19, 0x7D
  };
  const std::uint8_t recommendedHover[] = {
    0x4D, 0x14, 0x0F, 0x81, 0x51, 0x69, 0x5A, 0x7E, 0x4B, 0x5F, 0x0F, 0xB7, 0x2C, 0x1D, 0xC3, 0xE1,
    0xE8, 0xD5, 0xAA, 0x0C, 0xCE, 0xF6, 0xEA, 0xD6, 0xFE, 0xCF, 0x9A, 0x32, 0xCD, 0xD3, 0xFE, 0xD1,
    0xD2, 0xC9, 0xAA, 0x18, 0xFE, 0xCA, 0xF2, 0xD6, 0xE3, 0xDF, 0xAA, 0x1F, 0xCE, 0xC8, 0xF1, 0x9D,
    0xEF, 0xD6, 0xBF, 0x7B
  };
  const std::uint8_t recommendedNormal[] = {
    0x37, 0xBC, 0xEB, 0x55, 0x31, 0xC1, 0xD8, 0x40, 0x31, 0xF7, 0xEB, 0x63, 0x4C, 0xB5, 0x41, 0xDF,
    0x92, 0x7D, 0x4E, 0xD8, 0xAE, 0x5E, 0x68, 0xE8, 0x84, 0x67, 0x7E, 0xE6, 0xAD, 0x7B, 0x7C, 0xEF,
    0xA8, 0x61, 0x4E, 0xCC, 0x9E, 0x62, 0x70, 0xE8, 0x99, 0x77, 0x4E, 0xCB, 0xAE, 0x60, 0x7B, 0xEB,
    0xD9, 0x71, 0x46, 0xDF, 0xF1
  };
  recommendedButton->SetImages(
    std::string(DecryptName<0x8D1D0FF1AF2B13F7ULL>(recommendedNormal)), std::string(DecryptName<0xB39FA7917BCFBB8DULL>(recommendedHover)),
    std::string(DecryptName<0x9171997D6963C9F7ULL>(recommendedPressed)), std::string(DecryptName<0x4F99DF03C3EF31B7ULL>(recommendedInactive))
  );
  this->tab->AddChild(recommendedButton);

  CUITabButton* holdButton = new CUITabButton(*this->m_context);
  holdButton->SetSize(30, 100);
  holdButton->Move(0, 202);
  holdButton->SetOnClick([this]() {
    reinterpret_cast<WindowReplayRecorder*>(this)->RecordValues(0x16, 4, 0, 0, 0);
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type == 4) {
        if (list->list) {
          list->list->scrollBar->SetPosition(0);
        }
        return;
      }
    }
  });
  holdButton->disabled = 0;
  holdButton->Invalidate();
  const std::uint8_t holdFont[] = {0x62, 0x04, 0xC3, 0xA2, 0x60, 0xF3};
  holdButton->SetText(std::string(""));
  holdButton->SetFitText(0);
  holdButton->SetAlign(0, 0);
  holdButton->SetFont(std::string(DecryptName<0xE52DF30DCBAF7125ULL>(holdFont)));
  holdButton->SetFontHeight(12);
  holdButton->SetBold(0);
  holdButton->SetTextColors(0, 0, 0, 0);
  holdButton->SetShadow(0, 0);
  holdButton->SetOutline(0, 0);
  const std::uint8_t holdInactive[] = {
    0x2D, 0xDA, 0xA1, 0xD9, 0x77, 0x2D, 0x94, 0xE0, 0x2B, 0x91, 0xA1, 0xEF, 0x0A, 0x59, 0x0D, 0x7F,
    0x88, 0x1B, 0x04, 0x54, 0xE8, 0xB2, 0x24, 0x48, 0x9E, 0x01, 0x34, 0x6A, 0xEB, 0x97, 0x30, 0x4F,
    0xB2, 0x1D, 0x0E, 0x4F, 0xD3, 0xBC, 0x3E, 0x4B, 0x8B, 0x5B, 0x03, 0x4E, 0xC7, 0xE3
  };
  const std::uint8_t holdPressed[] = {
    0xA5, 0x5C, 0x17, 0xA7, 0x4D, 0xDD, 0xAA, 0xDA, 0xA3, 0x17, 0x17, 0x91, 0x30, 0xA9, 0x33, 0x45,
    0x00, 0x9D, 0xB2, 0x2A, 0xD2, 0x42, 0x1A, 0x72, 0x16, 0x87, 0x82, 0x14, 0xD1, 0x67, 0x0E, 0x75,
    0x3A, 0x9B, 0xB8, 0x31, 0xE9, 0x4C, 0x00, 0x71, 0x03, 0xDD, 0xB5, 0x30, 0xFD, 0x13
  };
  const std::uint8_t holdHover[] = {
    0x01, 0xE8, 0x05, 0xED, 0x21, 0x57, 0x10, 0x36, 0x07, 0xA3, 0x05, 0xDB, 0x5C, 0x23, 0x89, 0xA9,
    0xA4, 0x29, 0xA0, 0x60, 0xBE, 0xC8, 0xA0, 0x9E, 0xB2, 0x33, 0x90, 0x5E, 0xBD, 0xED, 0xB4, 0x99,
    0x9E, 0x2F, 0xAA, 0x7B, 0x85, 0xC6, 0xBA, 0x95, 0xEF, 0x25, 0xA8, 0x67, 0xE1
  };
  const std::uint8_t holdNormal[] = {
    0x9F, 0x9A, 0x6F, 0x6F, 0x5D, 0x47, 0x36, 0x02, 0x99, 0xD1, 0x6F, 0x59, 0x20, 0x33, 0xAF, 0x9D,
    0x3A, 0x5B, 0xCA, 0xE2, 0xC2, 0xD8, 0x86, 0xAA, 0x2C, 0x41, 0xFA, 0xDC, 0xC1, 0xFD, 0x92, 0xAD,
    0x00, 0x5D, 0xC0, 0xF9, 0xF9, 0xD6, 0x9C, 0xA9, 0x39, 0x1B, 0xCD, 0xF8, 0xED, 0x89
  };
  holdButton->SetImages(
    std::string(DecryptName<0xCFF3899D95AF355FULL>(holdNormal)), std::string(DecryptName<0xFBD599E117C547C1ULL>(holdHover)),
    std::string(DecryptName<0x176F138D5DD7F365ULL>(holdPressed)), std::string(DecryptName<0x2D51E3B7236175EDULL>(holdInactive))
  );
  this->tab->AddChild(holdButton);

  CUITabButton* waitButton = new CUITabButton(*this->m_context);
  waitButton->SetSize(30, 100);
  waitButton->Move(0, 303);
  waitButton->SetOnClick([this]() {
    reinterpret_cast<WindowReplayRecorder*>(this)->RecordValues(0x16, 8, 0, 0, 0);
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type == 8) {
        if (list->list) {
          list->list->scrollBar->SetPosition(0);
        }
        return;
      }
    }
  });
  waitButton->disabled = 0;
  waitButton->Invalidate();
  const std::uint8_t waitFont[] = {0x50, 0x6E, 0x41, 0xCA, 0xD0, 0xEB};
  waitButton->SetText(std::string(""));
  waitButton->SetFitText(0);
  waitButton->SetAlign(0, 0);
  waitButton->SetFont(std::string(DecryptName<0x0F41EBBDA32D1B17ULL>(waitFont)));
  waitButton->SetFontHeight(12);
  waitButton->SetBold(0);
  waitButton->SetTextColors(0, 0, 0, 0);
  waitButton->SetShadow(0, 0);
  waitButton->SetOutline(0, 0);
  const std::uint8_t waitInactive[] = {
    0x9D, 0xA2, 0x89, 0x73, 0x61, 0xB3, 0x28, 0x52, 0x9B, 0xE9, 0x89, 0x45, 0x1C, 0xC7, 0xB1, 0xCD,
    0x38, 0x63, 0x2C, 0xFE, 0xFE, 0x2C, 0x98, 0xFA, 0x2E, 0x79, 0x1C, 0xC0, 0xFD, 0x09, 0x8C, 0xFD,
    0x02, 0x7A, 0x28, 0xE0, 0xD5, 0x22, 0x82, 0xF9, 0x3B, 0x23, 0x2B, 0xE4, 0xD1, 0x7D
  };
  const std::uint8_t waitPressed[] = {
    0xA5, 0xFA, 0x09, 0x85, 0xCD, 0x21, 0xEA, 0x88, 0xA3, 0xB1, 0x09, 0xB3, 0xB0, 0x55, 0x73, 0x17,
    0x00, 0x3B, 0xAC, 0x08, 0x52, 0xBE, 0x5A, 0x20, 0x16, 0x21, 0x9C, 0x36, 0x51, 0x9B, 0x4E, 0x27,
    0x3A, 0x22, 0xA8, 0x16, 0x79, 0xB0, 0x40, 0x23, 0x03, 0x7B, 0xAB, 0x12, 0x7D, 0xEF
  };
  const std::uint8_t waitHover[] = {
    0xAD, 0xB4, 0x67, 0xEB, 0xB7, 0xA1, 0x0E, 0x3E, 0xAB, 0xFF, 0x67, 0xDD, 0xCA, 0xD5, 0x97, 0xA1,
    0x08, 0x75, 0xC2, 0x66, 0x28, 0x3E, 0xBE, 0x96, 0x1E, 0x6F, 0xF2, 0x58, 0x2B, 0x1B, 0xAA, 0x91,
    0x32, 0x6C, 0xC6, 0x78, 0x03, 0x30, 0xA4, 0x9D, 0x43, 0x79, 0xCA, 0x61, 0x77
  };
  const std::uint8_t waitNormal[] = {
    0x73, 0xC8, 0x99, 0x75, 0x1F, 0xE9, 0x5E, 0x4E, 0x75, 0x83, 0x99, 0x43, 0x62, 0x9D, 0xC7, 0xD1,
    0xD6, 0x09, 0x3C, 0xF8, 0x80, 0x76, 0xEE, 0xE6, 0xC0, 0x13, 0x0C, 0xC6, 0x83, 0x53, 0xFA, 0xE1,
    0xEC, 0x10, 0x38, 0xE6, 0xAB, 0x78, 0xF4, 0xE5, 0xD5, 0x49, 0x3B, 0xE2, 0xAF, 0x27
  };
  waitButton->SetImages(
    std::string(DecryptName<0x839B27DF8F5967B3ULL>(waitNormal)), std::string(DecryptName<0xF3CB6F7711A71B6DULL>(waitHover)),
    std::string(DecryptName<0x452FEF0D7FC95565ULL>(waitPressed)), std::string(DecryptName<0x9FED7DA189490D5DULL>(waitInactive))
  );
  this->tab->AddChild(waitButton);

  this->checkBox = new CUICheckBox(*this->m_context);
  this->checkBox->SetSize(10, 10);
  this->checkBox->Move(38, 448);
  this->checkBox->SetOnCheck([this](bool checked) { reinterpret_cast<WindowReplayRecorder*>(this)->RecordWindowFlag(checked); });
  this->checkBox->disabled = 0;
  this->checkBox->Invalidate();
  const std::uint8_t checkInactive[] = {
    0x21, 0x36, 0x47, 0xB5, 0x41, 0x0D, 0xE4, 0xBE, 0x27, 0x7D, 0x47, 0x83, 0x3C, 0x79, 0x7D, 0x21,
    0x84, 0xF7, 0xE2, 0x38, 0xDE, 0x92, 0x54, 0x16, 0x92, 0xED, 0xD2, 0x06, 0xDD, 0xA0, 0x49, 0x16,
    0x82, 0xF2, 0xE5, 0x20, 0xF9, 0x9C, 0x4E, 0x15, 0x87, 0xB7, 0xE5, 0x22, 0xF1, 0xC3
  };
  const std::uint8_t checkPressed[] = {
    0x8F, 0x0E, 0x0F, 0xCF, 0xA3, 0x19, 0x84, 0xB4, 0x89, 0x45, 0x0F, 0xF9, 0xDE, 0x6D, 0x1D, 0x2B,
    0x2A, 0xCF, 0xAA, 0x42, 0x3C, 0x86, 0x34, 0x1C, 0x3C, 0xD5, 0x9A, 0x7C, 0x3F, 0xB4, 0x29, 0x1C,
    0x2C, 0xCA, 0xAD, 0x5A, 0x1B, 0x88, 0x2E, 0x1F, 0x29, 0x8F, 0xAD, 0x58, 0x13, 0xD7
  };
  const std::uint8_t checkHover[] = {
    0xBF, 0x7A, 0x6F, 0xA1, 0xAB, 0x7F, 0xD0, 0xC2, 0xB9, 0x31, 0x6F, 0x97, 0xD6, 0x0B, 0x49, 0x5D,
    0x1A, 0xBB, 0xCA, 0x2C, 0x34, 0xE0, 0x60, 0x6A, 0x0C, 0xA1, 0xFA, 0x12, 0x37, 0xD2, 0x7D, 0x6A,
    0x1C, 0xBE, 0xCD, 0x34, 0x13, 0xEE, 0x7A, 0x61, 0x51, 0xB7, 0xC2, 0x2B, 0x6B
  };
  const std::uint8_t checkNormal[] = {
    0xE3, 0x5E, 0x17, 0xED, 0x17, 0x87, 0x26, 0xB0, 0xE5, 0x15, 0x17, 0xDB, 0x6A, 0xF3, 0xBF, 0x2F,
    0x46, 0x9F, 0xB2, 0x60, 0x88, 0x18, 0x96, 0x18, 0x50, 0x85, 0x82, 0x5E, 0x8B, 0x2A, 0x8B, 0x18,
    0x40, 0x9A, 0xB5, 0x78, 0xAF, 0x16, 0x8C, 0x1B, 0x45, 0xDF, 0xB5, 0x7A, 0xA7, 0x49
  };
  this->checkBox->SetImages(
    std::string(DecryptName<0x7DE349D717D7F123ULL>(checkNormal)), std::string(DecryptName<0x0F15B16B5BAFD57FULL>(checkHover)),
    std::string(DecryptName<0x7941D76335CFA14FULL>(checkPressed)), std::string(DecryptName<0x7321C3814F8799E1ULL>(checkInactive))
  );
  this->checkBox->checked = false;
  this->checkBox->Invalidate();
  this->AddChild(this->checkBox);

  this->filterLabel = new CUIStaticText(*this->m_context);
  this->filterLabel->SetSize(100, 12);
  this->filterLabel->Move(52, 447);
  this->filterLabel->SetText("", 0);
  this->filterLabel->SetFitText(0);
  this->filterLabel->SetAlign(0, 0);
  const std::uint8_t labelFont[] = {0x8E, 0x66, 0x95, 0x1A, 0x94, 0xD9};
  this->filterLabel->SetFont(std::string(DecryptName<0x7115D9F973F913C9ULL>(labelFont)));
  this->filterLabel->SetFontHeight(12);
  this->filterLabel->SetBold(0);
  this->filterLabel->SetColor(0);
  this->filterLabel->SetShadow(0, 0xFFFFFF);
  this->filterLabel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->filterLabel);

  this->closeButton = new CUIImageButton(*this->m_context);
  this->closeButton->SetSize(42, 20);
  this->closeButton->Move(327, 442);
  this->closeButton->SetOnClick([this]() { g_windowMgr.CloseWindow(0x2718); });
  this->closeButton->disabled = 0;
  this->closeButton->Invalidate();
  const std::uint8_t closeInactive[] = {
    0x3F, 0x4C, 0x9D, 0xF9, 0x67, 0x61, 0x66, 0xF2, 0x39, 0x07, 0x9D, 0xCF, 0x1A, 0x15, 0xFF, 0x5D,
    0x9E, 0x90, 0x34, 0x60, 0xF8, 0xC6, 0xCD, 0x4B, 0x9A, 0x91, 0x3B, 0x62, 0xC4, 0xCA, 0xFF, 0x5D,
    0x8B, 0x8D, 0x02, 0x60, 0xCB, 0xC0, 0xD0, 0x5A, 0xD1, 0x81, 0x30, 0x73, 0xA7
  };
  const std::uint8_t closePressed[] = {
    0xDD, 0xAA, 0x53, 0xD3, 0xB1, 0xED, 0x92, 0x28, 0xDB, 0xE1, 0x53, 0xE5, 0xCC, 0x99, 0x0B, 0x87,
    0x7C, 0x76, 0xFA, 0x4A, 0x2E, 0x4A, 0x39, 0x91, 0x78, 0x77, 0xF5, 0x48, 0x12, 0x46, 0x0B, 0x87,
    0x69, 0x6B, 0xCC, 0x4A, 0x1D, 0x4C, 0x24, 0x80, 0x42, 0x64, 0xBD, 0x4B, 0x1C, 0x53, 0x57
  };
  const std::uint8_t closeHover[] = {
    0x8F, 0xC8, 0x2D, 0x7F, 0xC9, 0xF9, 0xFA, 0xB4, 0x89, 0x83, 0x2D, 0x49, 0xB4, 0x8D, 0x63, 0x1B,
    0x2E, 0x14, 0x84, 0xE6, 0x56, 0x5E, 0x51, 0x0D, 0x2A, 0x15, 0x8B, 0xE4, 0x6A, 0x52, 0x63, 0x1B,
    0x3B, 0x09, 0xB2, 0xE6, 0x65, 0x58, 0x4C, 0x1C, 0x10, 0x05, 0xC3, 0xE7, 0x64, 0x47, 0x3F
  };
  const std::uint8_t closeNormal[] = {
    0xA3, 0x5A, 0x21, 0x85, 0xFB, 0xEB, 0xD8, 0x06, 0xA5, 0x11, 0x21, 0xB3, 0x86, 0x9F, 0x41, 0xA9,
    0x02, 0x86, 0x88, 0x1C, 0x64, 0x4C, 0x73, 0xBF, 0x06, 0x87, 0x87, 0x1E, 0x58, 0x40, 0x41, 0xA9,
    0x17, 0x9B, 0xBE, 0x1C, 0x57, 0x4A, 0x6E, 0xAE, 0x4D, 0x97, 0x8C, 0x0F, 0x3B
  };
  this->closeButton->SetImages(
    std::string(DecryptName<0xCB1D253B7FE1F563ULL>(closeNormal)), std::string(DecryptName<0x793F370985ED674FULL>(closeHover)),
    std::string(DecryptName<0xE55723712993051DULL>(closePressed)), std::string(DecryptName<0x3FA3AFA7035DE3FFULL>(closeInactive))
  );
  this->AddChild(this->closeButton);

  for (int index = 0; index < 4; ++index) {
    RenewQuestUI::CUIQuestList* list = new RenewQuestUI::CUIQuestList(*this->m_context);
    list->SetSize(343, 394);
    this->AddChild(list);
    this->questLists.push_back(list);
  }
  this->questLists[0]->Move(38, 17);
  this->questLists[1]->Move(38, 17);
  this->questLists[2]->Move(38, 17);
  this->questLists[3]->Move(38, 17);

  this->searchBackground = new CUIStaticImage(*this->m_context);
  this->searchBackground->SetSize(352, 28);
  this->searchBackground->Move(29, 411);
  const std::uint8_t searchImage[] = {
    0x99, 0x28, 0xC3, 0x87, 0x95, 0x05, 0xE2, 0x16, 0x9F, 0x63, 0xC3, 0xB1, 0xE8, 0x71, 0x7B, 0x89,
    0x3C, 0xE9, 0x66, 0x0A, 0x0A, 0x9A, 0x52, 0xBE, 0x2A, 0xF3, 0x56, 0x34, 0x09, 0xA9, 0x40, 0x84,
    0x2A, 0xE2, 0x62, 0x0F, 0x36, 0xA3, 0x09, 0xB9, 0x34, 0xF7, 0x03
  };
  this->searchBackground->SetImage(std::string(DecryptName<0xDB27CB557D038759ULL>(searchImage)));
  this->AddChild(this->searchBackground);

  this->filterPanel = new CUITab(*this->m_context);
  this->filterPanel->SetSize(181, 22);
  this->filterPanel->Move(35, 414);
  this->filterPanel->SetOnChange([this](CUITabButton* button) { this->OnFilterButton(button); });
  this->AddChild(this->filterPanel);

  CUITabButton* allButton = new CUITabButton(*this->m_context);
  allButton->SetSize(26, 22);
  allButton->Move(0, 0);
  allButton->SetOnClick([this]() { this->OnCallback50(); });
  allButton->disabled = 0;
  allButton->Invalidate();
  allButton->SetId(0);
  const std::uint8_t allFont[] = {0xCA, 0xC8, 0xD7, 0x0A, 0xDA, 0x39};
  allButton->SetText(std::string(""));
  allButton->SetFitText(0);
  allButton->SetAlign(0, 0);
  allButton->SetFont(std::string(DecryptName<0x51BB39B763BBBD8DULL>(allFont)));
  allButton->SetFontHeight(12);
  allButton->SetBold(0);
  allButton->SetTextColors(0, 0, 0, 0);
  allButton->SetShadow(0, 0);
  allButton->SetOutline(0, 0);
  const std::uint8_t allInactive[] = {
    0xB1, 0x2C, 0xAB, 0xF1, 0xD1, 0xC1, 0x98, 0xB0, 0xB7, 0x67, 0xAB, 0xC7, 0xAC, 0xB5, 0x01, 0x2F,
    0x14, 0xED, 0x0E, 0x7C, 0x4E, 0x5E, 0x28, 0x18, 0x02, 0xF7, 0x3E, 0x42, 0x4D, 0x6D, 0x29, 0x13,
    0x2E, 0xE2, 0x07, 0x67, 0x4E, 0x60, 0x3B, 0x1B, 0x5F, 0xE1, 0x06, 0x7B, 0x11
  };
  const std::uint8_t allPressed[] = {
    0xFD, 0xEE, 0x4B, 0x33, 0x39, 0xD5, 0x00, 0x60, 0xFB, 0xA5, 0x4B, 0x05, 0x44, 0xA1, 0x99, 0xFF,
    0x58, 0x2F, 0xEE, 0xBE, 0xA6, 0x4A, 0xB0, 0xC8, 0x4E, 0x35, 0xDE, 0x80, 0xA5, 0x79, 0xB1, 0xC3,
    0x62, 0x20, 0xE7, 0xA5, 0xA6, 0x74, 0xA3, 0xCB, 0x13, 0x23, 0xE6, 0xB9, 0xF9
  };
  const std::uint8_t allHover[] = {
    0x91, 0x5A, 0x0F, 0xB3, 0x3F, 0xD9, 0x44, 0x12, 0x97, 0x11, 0x0F, 0x85, 0x42, 0xAD, 0xDD, 0x8D,
    0x34, 0x9B, 0xAA, 0x3E, 0xA0, 0x46, 0xF4, 0xBA, 0x22, 0x81, 0x9A, 0x00, 0xA3, 0x75, 0xF5, 0xB1,
    0x0E, 0x94, 0xA3, 0x25, 0xA0, 0x78, 0xEF, 0xF1, 0x33, 0x98, 0xBF, 0x49
  };
  const std::uint8_t allNormal[] = {
    0x2F, 0x82, 0x97, 0xF7, 0x05, 0x1F, 0x26, 0x4A, 0x29, 0xC9, 0x97, 0xC1, 0x78, 0x6B, 0xBF, 0xD5,
    0x8A, 0x43, 0x32, 0x7A, 0x9A, 0x80, 0x96, 0xE2, 0x9C, 0x59, 0x02, 0x44, 0x99, 0xB3, 0x97, 0xE9,
    0xB0, 0x4C, 0x3B, 0x61, 0x9A, 0xBE, 0x85, 0xE1, 0xC1, 0x4F, 0x3A, 0x7D, 0xC5
  };
  allButton->SetImages(
    std::string(DecryptName<0x87E3D1C50D572DEFULL>(allNormal)), std::string(DecryptName<0xDF8117FF49CFF551ULL>(allHover)),
    std::string(DecryptName<0xADC51BF9C98B413DULL>(allPressed)), std::string(DecryptName<0x7D5D0F110B6B8371ULL>(allInactive))
  );
  this->filterPanel->AddChild(allButton);

  CUITabButton* normalButton = new CUITabButton(*this->m_context);
  normalButton->SetSize(26, 22);
  normalButton->Move(31, 0);
  normalButton->SetOnClick([this]() { this->OnCallback56(); });
  normalButton->disabled = 0;
  normalButton->Invalidate();
  normalButton->SetId(2);
  normalButton->SetText(std::string(""));
  normalButton->SetFitText(0);
  normalButton->SetAlign(0, 0);
  normalButton->SetFont(std::string(GulimFontName1().Text()));
  normalButton->SetFontHeight(12);
  normalButton->SetBold(0);
  normalButton->SetTextColors(0, 0, 0, 0);
  normalButton->SetShadow(0, 0);
  normalButton->SetOutline(0, 0);
  normalButton->SetImages(
    std::string(NormalOffButtonName1().Text()), std::string(NormalOnButtonName().Text()),
    std::string(NormalOffButtonName2().Text()), std::string(NormalOffButtonName3().Text())
  );
  this->filterPanel->AddChild(normalButton);

  CUITabButton* subButton = new CUITabButton(*this->m_context);
  subButton->SetSize(26, 22);
  subButton->Move(62, 0);
  subButton->SetOnClick([this]() { this->OnPanelButton(); });
  subButton->disabled = 0;
  subButton->Invalidate();
  subButton->SetId(3);
  subButton->SetText(std::string(""));
  subButton->SetFitText(0);
  subButton->SetAlign(0, 0);
  subButton->SetFont(std::string(GulimFontName2().Text()));
  subButton->SetFontHeight(12);
  subButton->SetBold(0);
  subButton->SetTextColors(0, 0, 0, 0);
  subButton->SetShadow(0, 0);
  subButton->SetOutline(0, 0);
  subButton->SetImages(
    std::string(SubOffButtonName1().Text()), std::string(SubOnButtonName().Text()),
    std::string(SubOffButtonName2().Text()), std::string(SubOffButtonName3().Text())
  );
  this->filterPanel->AddChild(subButton);

  CUITabButton* dailyButton = new CUITabButton(*this->m_context);
  dailyButton->SetSize(26, 22);
  dailyButton->Move(93, 0);
  dailyButton->SetOnClick([this]() { this->OnCallback43(); });
  dailyButton->disabled = 0;
  dailyButton->Invalidate();
  dailyButton->SetId(4);
  dailyButton->SetText(std::string(""));
  dailyButton->SetFitText(0);
  dailyButton->SetAlign(0, 0);
  dailyButton->SetFont(std::string(GulimFontName3().Text()));
  dailyButton->SetFontHeight(12);
  dailyButton->SetBold(0);
  dailyButton->SetTextColors(0, 0, 0, 0);
  dailyButton->SetShadow(0, 0);
  dailyButton->SetOutline(0, 0);
  dailyButton->SetImages(
    std::string(DailyOffButtonName1().Text()), std::string(DailyOnButtonName().Text()),
    std::string(DailyOffButtonName2().Text()), std::string(DailyOffButtonName3().Text())
  );
  this->filterPanel->AddChild(dailyButton);

  CUITabButton* eventButton = new CUITabButton(*this->m_context);
  eventButton->SetSize(26, 22);
  eventButton->Move(124, 0);
  eventButton->SetOnClick([this]() { this->OnCallback74(); });
  eventButton->disabled = 0;
  eventButton->Invalidate();
  eventButton->SetId(5);
  eventButton->SetText(std::string(""));
  eventButton->SetFitText(0);
  eventButton->SetAlign(0, 0);
  eventButton->SetFont(std::string(GulimFontName4().Text()));
  eventButton->SetFontHeight(12);
  eventButton->SetBold(0);
  eventButton->SetTextColors(0, 0, 0, 0);
  eventButton->SetShadow(0, 0);
  eventButton->SetOutline(0, 0);
  eventButton->SetImages(
    std::string(EventOffButtonName1().Text()), std::string(EventOnButtonName().Text()),
    std::string(EventOffButtonName2().Text()), std::string(EventOffButtonName3().Text())
  );
  this->filterPanel->AddChild(eventButton);

  CUITabButton* etcButton = new CUITabButton(*this->m_context);
  etcButton->SetSize(26, 22);
  etcButton->Move(155, 0);
  etcButton->SetOnClick([this]() { this->OnCallback80(); });
  etcButton->disabled = 0;
  etcButton->Invalidate();
  etcButton->SetId(1);
  etcButton->SetText(std::string(""));
  etcButton->SetFitText(0);
  etcButton->SetAlign(0, 0);
  etcButton->SetFont(std::string(GulimFontName5().Text()));
  etcButton->SetFontHeight(12);
  etcButton->SetBold(0);
  etcButton->SetTextColors(0, 0, 0, 0);
  etcButton->SetShadow(0, 0);
  etcButton->SetOutline(0, 0);
  etcButton->SetImages(
    std::string(EtcOffButtonName1().Text()), std::string(EtcOnButtonName().Text()),
    std::string(EtcOffButtonName2().Text()), std::string(EtcOffButtonName3().Text())
  );
  this->filterPanel->AddChild(etcButton);

  this->searchEdit = new CUIEditBoxEx(*this->m_context);
  this->searchEdit->Create(119, 16, false);
  this->searchEdit->Move(223, 417);
  this->searchEdit->SetText(std::string(""));
  this->searchEdit->SetTextColor(0);
  this->searchEdit->SetBackgroundColor(0xFFFF00FF);
  this->searchEdit->SetBorderColor(0xC6C6C6);
  this->searchEdit->SetBorderWidth(1);
  this->searchEdit->SetMaximum(20);
  this->searchEdit->SetEditBounds(0, 0, 0, 0);
  this->searchEdit->SetMask(0);
  this->searchEdit->SetAlignment(0);
  this->searchEdit->SetOnSubmit([this]() {
    this->searchText = this->searchEdit->edit->Text();
    this->sub_A67290(0xF);
  });
  this->AddChild(this->searchEdit);

  this->searchButton = new CUIImageButton(*this->m_context);
  this->searchButton->SetSize(23, 18);
  this->searchButton->Move(342, 416);
  this->searchButton->SetOnClick([this]() {
    this->searchText = this->searchEdit->edit->Text();
    this->sub_A67290(0xF);
  });
  this->searchButton->disabled = 0;
  this->searchButton->Invalidate();
  this->searchButton->SetImages(
    std::string(SearchNormalButtonName().Text()), std::string(SearchPressButtonName().Text()),
    std::string(SearchOverButtonName().Text()), std::string(SearchDisableButtonName().Text())
  );
  this->AddChild(this->searchButton);

  processButton->page = this->questLists[0];
  recommendedButton->page = this->questLists[1];
  holdButton->page = this->questLists[2];
  waitButton->page = this->questLists[3];
  this->tab->Select(processButton);
  this->filterPanel->Select(allButton);
}

// OFFSET: 0x00A63930
// FUNCTION: fn_16439
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->rowHeight <= this->geometry.height) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight > 1) {
      ++count;
    }
    this->items.resize(count, nullptr);
    for (RenewQuestUI::CUIQuestItem*& slot : this->items) {
      RenewQuestUI::CUIQuestItem* item = new RenewQuestUI::CUIQuestItem(*this->m_context);
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (RenewQuestUI::CUIQuestItem* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        this->SelectItem(listItem);
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A63CD0
// FUNCTION: fn_16442
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::Slot53(int resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A63EE0
// FUNCTION: fn_16443
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A659B0
// FUNCTION: fn_16597
RenewQuestUI::CUIQuestItem::CUIQuestItem(UIWindowContext& context)
    : CUIListItem(context), icon(nullptr), unknown_00EC(nullptr), unknown_00F0(nullptr), unknown_00F4(nullptr), unknown_00F8(nullptr), questType(1),
      questId(0), pressed(0), baseX(0), baseY(0) {
}

// OFFSET: 0x00A65A70
// FUNCTION: fn_16598
RenewQuestUI::CUIQuestList::CUIQuestList(UIWindowContext& context) : CUIControl(context), list(nullptr), type(1) {
}

// OFFSET: 0x00A65AF0
// FUNCTION: fn_16599
CUIRenewQuestUI::CUIRenewQuestUI()
    : tab(nullptr), checkBox(nullptr), filterLabel(nullptr), closeButton(nullptr), questLists(), searchBackground(nullptr), filterPanel(nullptr), searchEdit(nullptr), searchButton(nullptr), filter(0), searchText(),
      titleBar(nullptr), background(nullptr) {
}

// OFFSET: 0x00A66DA0
// FUNCTION: fn_16624
void CUIRenewQuestUI::Slot16() {
  g_windowMgr.RecordWindowPacket(0x2718);
  g_windowMgr.CloseWindow(0x8F);

  const char* title = LookupMessage(0xD48);
  this->titleBar->SetTitle(title, std::strlen(title));
  const char* label = LookupMessage(0x7E3);
  this->filterLabel->SetText(label, std::strlen(label));

  bool visible = false;
  if (UIWindow* window = g_windowMgr.LookupWindow(0xCC)) {
    visible = window->geometry.visible != 0;
  }
  this->checkBox->checked = visible;
  this->checkBox->Invalidate();

  for (unsigned int index = 0; index < this->questLists.size(); ++index) {
    this->questLists[index]->type = 1 << index;
  }
  this->sub_A67290(0xF);

  for (UIWindow* child : this->filterPanel->m_children) {
    const std::string name = g_renewQuestMgr->GetFilterName(child->geometry.id);
    static_cast<CUITabButton*>(child)->tooltipText = name;
  }
}
