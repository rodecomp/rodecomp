#include "Client/Session.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>

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

// OFFSET: 0x00E0B320
// FUNCTION: fn_23814
const char* CSession::GetPetTalkName(int job) {
  switch (job) {
  case 1002: {
    const std::uint8_t source[] = {0x67, 0x74, 0x5F, 0xCA, 0xD3, 0x8C, 0x41};
    thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> name(source);
    return name.Text();
  }
  case 1052: {
    const std::uint8_t source[] = {0xB3, 0x28, 0xA6, 0x7C, 0x84, 0xEB, 0xD5};
    thread_local EncryptedName<sizeof(source), 0xFBD599E117C547C1ULL> name(source);
    return name.Text();
  }
  case 1113: {
    const std::uint8_t source[] = {0x01, 0x81, 0xB8, 0x2D, 0xFE, 0x13};
    thread_local EncryptedName<sizeof(source), 0x176F138D5DD7F365ULL> name(source);
    return name.Text();
  }
  case 1031: {
    const std::uint8_t source[] = {0x15, 0x3A, 0xB9, 0x10, 0x7F, 0x86, 0x41, 0x22, 0x65};
    thread_local EncryptedName<sizeof(source), 0x452FEF0D7FC95565ULL> name(source);
    return name.Text();
  }
  case 1063: {
    const std::uint8_t source[] = {0x31, 0x78, 0x27, 0xE8, 0xD5, 0x14, 0x8E, 0x9F};
    thread_local EncryptedName<sizeof(source), 0x9FED7DA189490D5DULL> name(source);
    return name.Text();
  }
  case 1049: {
    const std::uint8_t source[] = {0xC3, 0x0E, 0x3A, 0xE4, 0xA6, 0x27};
    thread_local EncryptedName<sizeof(source), 0x839B27DF8F5967B3ULL> name(source);
    return name.Text();
  }
  case 1011: {
    const std::uint8_t source[] = {0x0E, 0x73, 0xC8, 0x7F, 0x14, 0x07, 0xA4, 0x9D, 0x6D};
    thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
    return name.Text();
  }
  case 1042: {
    const std::uint8_t source[] = {0x3C, 0xD5, 0xAA, 0x50, 0x0F, 0x88, 0x22, 0x11, 0x20, 0xCF, 0xAC, 0x5D, 0x0C, 0xB9, 0x41};
    thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> name(source);
    return name.Text();
  }
  case 1035: {
    const std::uint8_t source[] = {0x89, 0xEC, 0xE9, 0x3B, 0xE4, 0xB1, 0x7E, 0x15, 0x8D, 0xE0, 0x87};
    thread_local EncryptedName<sizeof(source), 0x7321C3814F8799E1ULL> name(source);
    return name.Text();
  }
  case 1107: {
    const std::uint8_t source[] = {0x47, 0x94, 0xA4, 0x72, 0xA5, 0x3D, 0xBC, 0x0A, 0x4C, 0x9D, 0xB1, 0x48, 0xB5, 0x28, 0x81, 0x18, 0x23};
    thread_local EncryptedName<sizeof(source), 0x7DE349D717D7F123ULL> name(source);
    return name.Text();
  }
  case 1014: {
    const std::uint8_t source[] = {0x0C, 0xA5, 0xC0, 0x29, 0x0E, 0xB1};
    thread_local EncryptedName<sizeof(source), 0x0F15B16B5BAFD57FULL> name(source);
    return name.Text();
  }
  case 1077: {
    const std::uint8_t source[] = {0x3F, 0x08, 0x84, 0xF6, 0x66, 0x59, 0x60, 0x0A, 0x3F, 0x08, 0x9F, 0xE0, 0x09};
    thread_local EncryptedName<sizeof(source), 0x793F370985ED674FULL> name(source);
    return name.Text();
  }
  case 1019: {
    const std::uint8_t source[] = {0x6D, 0x60, 0xF0, 0x46, 0x01, 0x46, 0x34, 0x8A, 0x1D};
    thread_local EncryptedName<sizeof(source), 0xE55723712993051DULL> name(source);
    return name.Text();
  }
  case 1056: {
    const std::uint8_t source[] = {0xBA, 0x7E, 0x96, 0x18, 0x90, 0xBC, 0x15};
    thread_local EncryptedName<sizeof(source), 0x7115D9F973F913C9ULL> name(source);
    return name.Text();
  }
  case 1057: {
    const std::uint8_t source[] = {0x1A, 0x9A, 0x98, 0x10, 0x3B};
    thread_local EncryptedName<sizeof(source), 0xCB1D253B7FE1F563ULL> name(source);
    return name.Text();
  }
  case 1023: {
    const std::uint8_t source[] = {0x18, 0xE5, 0xEC, 0x42, 0x54, 0x32, 0x93, 0x1D, 0x1E, 0xF8, 0xF5, 0x1D};
    thread_local EncryptedName<sizeof(source), 0x6FE153231D879777ULL> name(source);
    return name.Text();
  }
  case 1026: {
    const std::uint8_t source[] = {0x36, 0x26, 0x31, 0xBC, 0xEC, 0x9F};
    thread_local EncryptedName<sizeof(source), 0x39A39F87DD5F535BULL> name(source);
    return name.Text();
  }
  case 1110: {
    const std::uint8_t source[] = {0xA1, 0x42, 0x40, 0x5A, 0xEB, 0x74, 0x6D};
    thread_local EncryptedName<sizeof(source), 0xEF6D1D893F2B2DC5ULL> name(source);
    return name.Text();
  }
  case 1170: {
    const std::uint8_t source[] = {0x6E, 0xA8, 0x23, 0xD0, 0xB8, 0x77};
    thread_local EncryptedName<sizeof(source), 0x234977DDB54BC71DULL> name(source);
    return name.Text();
  }
  case 1029: {
    const std::uint8_t source[] = {0x46, 0xA4, 0x2C, 0x56, 0x39};
    thread_local EncryptedName<sizeof(source), 0xA12DAB392545D72FULL> name(source);
    return name.Text();
  }
  case 1155: {
    const std::uint8_t source[] = {0x05, 0x5E, 0x77, 0x46, 0x67, 0xC1};
    thread_local EncryptedName<sizeof(source), 0x6BA1C1132F033B75ULL> name(source);
    return name.Text();
  }
  case 1109: {
    const std::uint8_t source[] = {0xB5, 0x3E, 0xE1, 0x8C, 0x4B, 0xF2, 0x88, 0x47, 0xB8, 0x5B};
    thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
    return name.Text();
  }
  case 1101: {
    const std::uint8_t source[] = {0xAF, 0x62, 0xBD, 0x13, 0xF8, 0x56, 0x76, 0x47, 0x92, 0x69, 0xCD};
    thread_local EncryptedName<sizeof(source), 0x33133B977BCD03CDULL> name(source);
    return name.Text();
  }
  case 1167: {
    const std::uint8_t source[] = {0x84, 0x72, 0x5D, 0xCE, 0x96, 0x6A, 0x42, 0xEF, 0x96, 0x71, 0x4E, 0xAF};
    thread_local EncryptedName<sizeof(source), 0x8D1D0FF1AF2B13F7ULL> name(source);
    return name.Text();
  }
  case 1188: {
    const std::uint8_t source[] = {0xEF, 0xD4, 0xA1, 0x24, 0xF6, 0xD2, 0xF1, 0xB3};
    thread_local EncryptedName<sizeof(source), 0xB39FA7917BCFBB8DULL> name(source);
    return name.Text();
  }
  case 1879: {
    const std::uint8_t source[] = {0xEE, 0x82, 0x01, 0x70, 0x37, 0x26, 0x8E, 0x72, 0xFB, 0xC1};
    thread_local EncryptedName<sizeof(source), 0x2DCB7567394DC1ABULL> name(source);
    return name.Text();
  }
  case 1200: {
    const std::uint8_t source[] = {0x23, 0xDF, 0x62, 0x61, 0xA9, 0x09, 0x0B, 0x3C, 0x31, 0xB7};
    thread_local EncryptedName<sizeof(source), 0x4F637DC51307B759ULL> name(source);
    return name.Text();
  }
  case 1275: {
    const std::uint8_t source[] = {0x44, 0x1D, 0xC6, 0xA8, 0x68, 0xF3};
    thread_local EncryptedName<sizeof(source), 0xE52DF30DCBAF7125ULL> name(source);
    return name.Text();
  }
  case 1815: {
    const std::uint8_t source[] = {0x3A, 0x43, 0xCA, 0xFB, 0xE9, 0xD6, 0x81, 0xA6, 0x3C, 0x50, 0xCC, 0xF4, 0xF6, 0xEC, 0xF3};
    thread_local EncryptedName<sizeof(source), 0xCFF3899D95AF355FULL> name(source);
    return name.Text();
  }
  case 1245: {
    const std::uint8_t source[] = {0x90, 0xA6, 0x01, 0x05, 0x14, 0xF7, 0x14, 0xCE, 0x8F, 0xA4, 0x02, 0x1A, 0x7D};
    thread_local EncryptedName<sizeof(source), 0x9171997D6963C9F7ULL> name(source);
    return name.Text();
  }
  case 1519:
  case 1631: {
    const std::uint8_t source[] = {0xF4, 0x79, 0xBA, 0x8D, 0x44, 0x80, 0xDC, 0x4F};
    thread_local EncryptedName<sizeof(source), 0x4F99DF03C3EF31B7ULL> name(source);
    return name.Text();
  }
  case 1963: {
    const std::uint8_t source[] = {0x13, 0x76, 0x10, 0x6F, 0x80, 0xF1, 0x0E, 0xC6, 0x06, 0x29};
    thread_local EncryptedName<sizeof(source), 0x9949BFD527532943ULL> name(source);
    return name.Text();
  }
  case 1122: {
    const std::uint8_t source[] = {0x08, 0x12, 0x99, 0x4F, 0x18, 0x69, 0xD0, 0xFC, 0x6F};
    thread_local EncryptedName<sizeof(source), 0xCD8F077123FB7D6FULL> name(source);
    return name.Text();
  }
  case 1123: {
    const std::uint8_t source[] = {0x00, 0xC4, 0xF3, 0xC9, 0xC8, 0x51, 0xBC, 0x21, 0x67};
    thread_local EncryptedName<sizeof(source), 0x13E33FA1A591AB67ULL> name(source);
    return name.Text();
  }
  case 1125: {
    const std::uint8_t source[] = {0x50, 0x28, 0xCF, 0x55, 0xD6, 0x8B, 0xF6, 0x23, 0x37};
    thread_local EncryptedName<sizeof(source), 0x17A9E5BF39AD4737ULL> name(source);
    return name.Text();
  }
  case 1385: {
    const std::uint8_t source[] = {0xC1, 0xF8, 0x01, 0x9C, 0x17, 0x42, 0x05, 0x5C, 0xA5};
    thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> name(source);
    return name.Text();
  }
  case 1382: {
    const std::uint8_t source[] = {0xE3, 0x7A, 0x0A, 0x1D, 0x56, 0x65, 0x18, 0x68, 0x87};
    thread_local EncryptedName<sizeof(source), 0x0B7109397F6B1387ULL> name(source);
    return name.Text();
  }
  case 1208: {
    const std::uint8_t source[] = {0xBE, 0x94, 0x31, 0xEF, 0xEA, 0xA7, 0xE4, 0x4E, 0xA8, 0x9B, 0x5F};
    thread_local EncryptedName<sizeof(source), 0x23BBD58F8B5FF5C9ULL> name(source);
    return name.Text();
  }
  case 1630: {
    const std::uint8_t source[] = {0x2F, 0xFA, 0xD6, 0x7E, 0xDC, 0xB1, 0x5E, 0x8B, 0x32, 0xBB};
    thread_local EncryptedName<sizeof(source), 0xC517FB932D95BB6DULL> name(source);
    return name.Text();
  }
  case 1513: {
    const std::uint8_t source[] = {0xCC, 0xF4, 0xB9, 0xCE, 0x31, 0x9A, 0xFE, 0x30, 0xDD, 0xEB, 0xAE, 0xC9, 0x29, 0xC5};
    thread_local EncryptedName<sizeof(source), 0x75ADC57D87EFBD8FULL> name(source);
    return name.Text();
  }
  case 1586: {
    const std::uint8_t source[] = {0x95, 0xBC, 0xA0, 0xA5, 0x32, 0x56, 0xEC, 0x7F, 0xD9};
    thread_local EncryptedName<sizeof(source), 0x2BAD156DE3E1F9D9ULL> name(source);
    return name.Text();
  }
  case 1505: {
    const std::uint8_t source[] = {0xD9, 0xC0, 0xAD, 0x44, 0x02, 0x5B, 0x34, 0xA3, 0xDC, 0x8F};
    thread_local EncryptedName<sizeof(source), 0xF161095D0DE18F95ULL> name(source);
    return name.Text();
  }
  case 1143: {
    const std::uint8_t source[] = {0xA2, 0x30, 0x8D, 0x5A, 0x22, 0x01, 0x82, 0xB9, 0xBB, 0x34, 0xDF};
    thread_local EncryptedName<sizeof(source), 0xEDC74F6D13DF71EFULL> name(source);
    return name.Text();
  }
  case 1401: {
    const std::uint8_t source[] = {0xEA, 0x4D, 0x3E, 0x01, 0x44, 0x5F, 0xC6, 0x75};
    thread_local EncryptedName<sizeof(source), 0x758F1D0B4F7705B9ULL> name(source);
    return name.Text();
  }
  case 1179: {
    const std::uint8_t source[] = {0x32, 0x51, 0x0C, 0x5A, 0x7D, 0x66, 0x49, 0x89};
    thread_local EncryptedName<sizeof(source), 0x891B232D09451965ULL> name(source);
    return name.Text();
  }
  case 1504: {
    const std::uint8_t source[] = {0x2D, 0x4A, 0x3B, 0x19, 0xDC, 0xDD, 0x3A, 0x2D, 0x69};
    thread_local EncryptedName<sizeof(source), 0x637B959D55771F69ULL> name(source);
    return name.Text();
  }
  case 1299: {
    const std::uint8_t source[] = {0x16, 0xC4, 0x8F, 0x4D, 0xF2, 0x73, 0x1E, 0x9D, 0x14, 0xCA, 0x89, 0x44, 0xE9, 0x3D};
    thread_local EncryptedName<sizeof(source), 0xD1413DBB01CD8B51ULL> name(source);
    return name.Text();
  }
  case 1040: {
    const std::uint8_t source[] = {0x2C, 0xF6, 0x8B, 0x4A, 0x2A, 0x8F};
    thread_local EncryptedName<sizeof(source), 0xB7FD8F670FC7B96BULL> name(source);
    return name.Text();
  }
  case 1837: {
    const std::uint8_t source[] = {0xB4, 0x9A, 0x1D, 0xA9};
    thread_local EncryptedName<sizeof(source), 0xE7978FB5A94DD7FDULL> name(source);
    return name.Text();
  }
  case 1374: {
    const std::uint8_t source[] = {0xB4, 0xF7, 0xF8, 0x74, 0xDD, 0xA8, 0x12, 0x49};
    thread_local EncryptedName<sizeof(source), 0x4941FD9F21BBB9FDULL> name(source);
    return name.Text();
  }
  case 1148: {
    const std::uint8_t source[] = {0x38, 0x88, 0xA9, 0x08, 0x38, 0x24, 0xE5};
    thread_local EncryptedName<sizeof(source), 0x35E5656B5DEDCD75ULL> name(source);
    return name.Text();
  }
  case 1404: {
    const std::uint8_t source[] = {0x32, 0x5C, 0xD4, 0x50, 0x75, 0x46, 0xD4, 0x61, 0x36, 0x5B, 0xCA, 0x48, 0x78, 0x0F};
    thread_local EncryptedName<sizeof(source), 0x2F8B0F37118D157FULL> name(source);
    return name.Text();
  }
  case 1379: {
    const std::uint8_t source[] = {0x83, 0xB8, 0xDE, 0xFD, 0xD9, 0x16, 0x46, 0xFF, 0x88, 0xAE, 0xCD, 0xF0, 0xDF, 0x09, 0x48, 0xFF, 0xCD};
    thread_local EncryptedName<sizeof(source), 0xAD075B8DB599F1CDULL> name(source);
    return name.Text();
  }
  case 1495: {
    const std::uint8_t source[] = {0xC4, 0xA5, 0xAE, 0x93, 0x94, 0xCC, 0xBA, 0x67, 0xD8, 0xBE, 0xB5, 0x98, 0x83, 0x93};
    thread_local EncryptedName<sizeof(source), 0x2FE993D1DDE1F197ULL> name(source);
    return name.Text();
  }
  case 1370: {
    const std::uint8_t source[] = {0xDC, 0xCC, 0xA8, 0x4E, 0xE0, 0x3D, 0xAA, 0x6A, 0x8F};
    thread_local EncryptedName<sizeof(source), 0x39FF7FB50DEB998FULL> name(source);
    return name.Text();
  }
  case 1416: {
    const std::uint8_t source[] = {0xEA, 0xF4, 0xAE, 0x7C, 0xFE, 0xFB, 0xB0, 0xCD, 0xE4, 0xF0, 0xBD, 0x7F, 0xBB};
    thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
    return name.Text();
  }
  case 2200: {
    const std::uint8_t source[] = {0xED, 0xAA, 0x2B, 0x78, 0x78, 0x05, 0x62, 0xBF};
    thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
    return name.Text();
  }
  case 2210: {
    const std::uint8_t source[] = {0xB9, 0x36, 0xB8, 0x84, 0x8A, 0x5F, 0x6C, 0x93, 0xA0, 0x2F, 0xB0, 0x94, 0xD5};
    thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> name(source);
    return name.Text();
  }
  case 2313: {
    const std::uint8_t source[] = {0xCF, 0x1E, 0x48, 0xDB, 0x26, 0x49, 0x44, 0x4B, 0xDC, 0x57};
    thread_local EncryptedName<sizeof(source), 0x050505679903579BULL> name(source);
    return name.Text();
  }
  case 1242: {
    const std::uint8_t source[] = {0xBC, 0x86, 0x6B, 0x40, 0x59, 0x79};
    thread_local EncryptedName<sizeof(source), 0xF14F79170939C7F1ULL> name(source);
    return name.Text();
  }
  case 2336: {
    const std::uint8_t source[] = {0xDB, 0x8A, 0x20, 0xE4, 0x81, 0x66, 0xB4, 0xA5};
    thread_local EncryptedName<sizeof(source), 0xA5FD29D7AB6DC59FULL> name(source);
    return name.Text();
  }
  case 2398: {
    const std::uint8_t source[] = {0x45, 0x6E, 0x7B, 0xEF, 0xF1, 0x18, 0x22, 0x89, 0x46, 0x75, 0x66, 0xF5, 0xFA, 0x5D};
    thread_local EncryptedName<sizeof(source), 0xD97D5DBDBB2F2709ULL> name(source);
    return name.Text();
  }
  case 3257: {
    const std::uint8_t source[] = {0x12, 0x72, 0xB3, 0xB4, 0xDA, 0x17, 0x16, 0x9F, 0x16, 0x7D, 0xB8, 0xD1};
    thread_local EncryptedName<sizeof(source), 0xED796785D1DF137FULL> name(source);
    return name.Text();
  }
  case 3258: {
    const std::uint8_t source[] = {0xF5, 0x10, 0xB2, 0x8E, 0x21, 0x46, 0x50, 0xEB, 0xFC, 0x07, 0xB6, 0x81, 0x2A, 0x23};
    thread_local EncryptedName<sizeof(source), 0x9B0F234DEFDF7593ULL> name(source);
    return name.Text();
  }
  case 3261: {
    const std::uint8_t source[] = {0x4F, 0x2F, 0xD8, 0x4C, 0x1A, 0x06, 0xC1, 0x90, 0x4E, 0x2C, 0xDF, 0x47, 0x45};
    thread_local EncryptedName<sizeof(source), 0xD98F5345098D630DULL> name(source);
    return name.Text();
  }
  case 1090: {
    const std::uint8_t source[] = {0x46, 0x7E, 0x9A, 0xDF, 0xA4, 0x8D, 0x44, 0x11, 0x4C, 0x1F};
    thread_local EncryptedName<sizeof(source), 0x7F2DFFC1ABE91F2BULL> name(source);
    return name.Text();
  }
  case 1166: {
    const std::uint8_t source[] = {0xFE, 0xE0, 0x67, 0x92, 0xD4, 0xCC, 0x69};
    thread_local EncryptedName<sizeof(source), 0xFD69A9B3F311818DULL> name(source);
    return name.Text();
  }
  case 1369: {
    const std::uint8_t source[] = {0x76, 0x91, 0x32, 0xF7, 0xD5, 0x02, 0x05, 0x2E, 0x72, 0x8C, 0x53};
    thread_local EncryptedName<sizeof(source), 0x4B755DB19953E311ULL> name(source);
    return name.Text();
  }
  case 1213: {
    const std::uint8_t source[] = {0x53, 0x8E, 0x66, 0x73, 0x9E, 0xCE, 0x59, 0xDE, 0x3B};
    thread_local EncryptedName<sizeof(source), 0xBD2BA1C11B01E73BULL> name(source);
    return name.Text();
  }
  case 3636: {
    const std::uint8_t source[] = {0xF5, 0x64, 0x69, 0x67, 0xC1, 0xD6, 0x4A, 0xA4, 0xEA, 0x64, 0x6E, 0x33};
    thread_local EncryptedName<sizeof(source), 0xED15938D333D2DB9ULL> name(source);
    return name.Text();
  }
  case 1096: {
    const std::uint8_t source[] = {0x22, 0x57, 0x3C, 0x0E, 0x0B, 0x44, 0x1F, 0xAE, 0x43};
    thread_local EncryptedName<sizeof(source), 0xC9712D676B5B3943ULL> name(source);
    return name.Text();
  }
  case 3495: {
    const std::uint8_t source[] = {0xCB, 0x7B, 0x56, 0x82, 0xA0, 0xAE, 0x1B, 0xFC, 0xC1, 0x6E, 0x09};
    thread_local EncryptedName<sizeof(source), 0x9569C9C7E70909AFULL> name(source);
    return name.Text();
  }
  case 1214: {
    const std::uint8_t source[] = {0xB0, 0x51, 0x00, 0x72, 0xC6, 0x1F};
    thread_local EncryptedName<sizeof(source), 0xB19D1FA9116F39D3ULL> name(source);
    return name.Text();
  }
  case 1301: {
    const std::uint8_t source[] = {0xF8, 0x54, 0xA2, 0x5C, 0x20, 0xCF, 0x91};
    thread_local EncryptedName<sizeof(source), 0x4191BB5531FD3999ULL> name(source);
    return name.Text();
  }
  case 1512: {
    const std::uint8_t source[] = {0x33, 0xCE, 0x04, 0x3E, 0x3A, 0x3F, 0x15};
    thread_local EncryptedName<sizeof(source), 0xF715514F5961B75BULL> name(source);
    return name.Text();
  }
  case 3496: {
    const std::uint8_t source[] = {0xA5, 0xEB, 0x92, 0x89, 0xF4, 0x43, 0x24, 0xAD, 0xA8, 0xFA, 0xCD};
    thread_local EncryptedName<sizeof(source), 0xD9452D81E5CD99C1ULL> name(source);
    return name.Text();
  }
  case 1180: {
    const std::uint8_t source[] = {0x3B, 0x52, 0xEF, 0x92, 0x12, 0x9F, 0xCC, 0xC4, 0x39, 0x3B};
    thread_local EncryptedName<sizeof(source), 0xADADEB4DF7813B55ULL> name(source);
    return name.Text();
  }
  case 1307: {
    const std::uint8_t source[] = {0x3A, 0x6C, 0xBF, 0xF0, 0x12, 0x3C, 0xFB, 0xD8, 0x37, 0x68, 0x94, 0xDB, 0x1C, 0x0A, 0xF9, 0xB1};
    thread_local EncryptedName<sizeof(source), 0xB195637DAFCB0D59ULL> name(source);
    return name.Text();
  }
  case 3669: {
    const std::uint8_t source[] = {0x09, 0xD6, 0xB4, 0x45, 0x84, 0x25, 0xCE, 0xB4, 0x5F, 0xBF};
    thread_local EncryptedName<sizeof(source), 0xD7A749EB27D5BF6DULL> name(source);
    return name.Text();
  }
  case 3670: {
    const std::uint8_t source[] = {0x59, 0x18, 0x39, 0x44, 0xB5, 0x10, 0xEB, 0x3A, 0x0F, 0x7D};
    thread_local EncryptedName<sizeof(source), 0x659975C121557D3DULL> name(source);
    return name.Text();
  }
  case 1622: {
    const std::uint8_t source[] = {0x2D, 0x1A, 0xC3, 0xC5, 0x6E, 0x9A, 0x97, 0xDA, 0x38, 0x0D, 0xA7};
    thread_local EncryptedName<sizeof(source), 0xBFF5C517A1A77F59ULL> name(source);
    return name.Text();
  }
  case 1632: {
    const std::uint8_t source[] = {0x40, 0xD7, 0xF6, 0xF2, 0x6D, 0x3A, 0x0F, 0xAD};
    thread_local EncryptedName<sizeof(source), 0xAD6153019F93A527ULL> name(source);
    return name.Text();
  }
  case 3731: {
    const std::uint8_t source[] = {0x0C, 0x38, 0x14, 0xD1, 0xCB, 0x90, 0x95, 0xEA, 0x11, 0x5B};
    thread_local EncryptedName<sizeof(source), 0x85E1F5A7A5755B7FULL> name(source);
    return name.Text();
  }
  case 1041: {
    const std::uint8_t source[] = {0x30, 0x9E, 0x1E, 0x24, 0xE2, 0x49};
    thread_local EncryptedName<sizeof(source), 0x0F61499B4973EB5DULL> name(source);
    return name.Text();
  }
  case 1010: {
    const std::uint8_t source[] = {0x26, 0x9E, 0x37, 0x18, 0xEA, 0xA1};
    thread_local EncryptedName<sizeof(source), 0xB127A19D775BF751ULL> name(source);
    return name.Text();
  }
  case 1782: {
    const std::uint8_t source[] = {0xBB, 0x6E, 0x9A, 0x8A, 0x82, 0x07, 0x1F};
    thread_local EncryptedName<sizeof(source), 0xA31F69E7EFED01C9ULL> name(source);
    return name.Text();
  }
  case 1773: {
    const std::uint8_t source[] = {0x03, 0x24, 0xDF, 0x73, 0xE2, 0x02, 0xC5, 0xDE, 0x05, 0x4B};
    thread_local EncryptedName<sizeof(source), 0xB7A96F8701BB4B6BULL> name(source);
    return name.Text();
  }
  case 1058: {
    const std::uint8_t source[] = {0x2C, 0xB4, 0x95, 0x82, 0x07, 0x25, 0xEC, 0x45, 0x41};
    thread_local EncryptedName<sizeof(source), 0x3789496BE3E1D141ULL> name(source);
    return name.Text();
  }
  case 1297: {
    const std::uint8_t source[] = {0x76, 0xB7, 0x50, 0xC0, 0x1C, 0x89, 0xE3, 0x50, 0x7A, 0xAC, 0x5E, 0xC4, 0x00, 0xE7};
    thread_local EncryptedName<sizeof(source), 0x0F97E779A933D917ULL> name(source);
    return name.Text();
  }
  case 2995: {
    const std::uint8_t source[] = {0x0B, 0x4A, 0x12, 0x39, 0x9C, 0x0B, 0x61, 0xA8, 0x2C, 0x45, 0x28, 0x2C, 0x8B, 0x6F};
    thread_local EncryptedName<sizeof(source), 0xD1056FF94D4D2773ULL> name(source);
    return name.Text();
  }
  case 3790: {
    const std::uint8_t source[] = {0x98, 0x28, 0x6C, 0x56, 0x0B, 0xD2, 0x6E, 0x27, 0x99, 0x30, 0x79, 0x40, 0x7F};
    thread_local EncryptedName<sizeof(source), 0x4331A17F33095FEBULL> name(source);
    return name.Text();
  }
  case 1150: {
    const std::uint8_t source[] = {0xA6, 0x82, 0xDC, 0x45, 0xEB, 0x58, 0x2A, 0x2D, 0xBF, 0xED};
    thread_local EncryptedName<sizeof(source), 0x454D31872BB3EDCBULL> name(source);
    return name.Text();
  }
  case 1159: {
    const std::uint8_t source[] = {0x71, 0x65, 0x35, 0x38, 0x88, 0xE2, 0x9B, 0xE8, 0x01};
    thread_local EncryptedName<sizeof(source), 0x81F58DED5D470D01ULL> name(source);
    return name.Text();
  }
  case 3971: {
    const std::uint8_t source[] = {0x06, 0x54, 0x04, 0xC7, 0x46, 0x1E, 0x89, 0x5D};
    thread_local EncryptedName<sizeof(source), 0x5DE7712FAB613F75ULL> name(source);
    return name.Text();
  }
  case 1894: {
    const std::uint8_t source[] = {0xE1, 0x04, 0x1E, 0x6D, 0xEA, 0xA5, 0xA8, 0x41};
    thread_local EncryptedName<sizeof(source), 0x41CFCB831F6B6B91ULL> name(source);
    return name.Text();
  }
  case 20424: {
    const std::uint8_t source[] = {0x5E, 0x0C, 0x3A, 0x9F, 0x45, 0x5C, 0xB4, 0x21, 0x47, 0x63};
    thread_local EncryptedName<sizeof(source), 0x49D33529F1556333ULL> name(source);
    return name.Text();
  }
  case 20425: {
    const std::uint8_t source[] = {0xC5, 0xE7, 0x33, 0x40, 0x76, 0x92, 0x41, 0x10, 0xB5};
    thread_local EncryptedName<sizeof(source), 0x792FFD1325418FB5ULL> name(source);
    return name.Text();
  }
  case 20423: {
    const std::uint8_t source[] = {0xDD, 0x08, 0x80, 0xBE, 0xEA, 0xED, 0x36, 0x9F, 0x8D, 0x69};
    thread_local EncryptedName<sizeof(source), 0xF15F8785CDE369BFULL> name(source);
    return name.Text();
  }
  case 20571: {
    const std::uint8_t source[] = {0x16, 0xD7, 0xCC, 0x26, 0xBF, 0x6A, 0x6D, 0xAC, 0x6B, 0x85};
    thread_local EncryptedName<sizeof(source), 0xE33F2FF779878559ULL> name(source);
    return name.Text();
  }
  case 1737: {
    const std::uint8_t source[] = {0xCA, 0xD1, 0x3E, 0xD7, 0xA0, 0xDD};
    thread_local EncryptedName<sizeof(source), 0x1767DDE18D779D8BULL> name(source);
    return name.Text();
  }
  case 1735: {
    const std::uint8_t source[] = {0xA0, 0xCF, 0x98, 0xAE, 0x54, 0xCD, 0xBB};
    thread_local EncryptedName<sizeof(source), 0xC1BB8111EDD183E1ULL> name(source);
    return name.Text();
  }
  case 1736: {
    const std::uint8_t source[] = {0xD6, 0x85, 0x80, 0xD4, 0x99, 0xBB};
    thread_local EncryptedName<sizeof(source), 0xC5B1BBCD9BC9C997ULL> name(source);
    return name.Text();
  }
  case 20338:
  case 20373: {
    const std::uint8_t source[] = {0x5F, 0x90, 0xB4, 0x91, 0x69, 0x22, 0xAA, 0x73, 0x54, 0xA6, 0xA7, 0x9C, 0x6F, 0x3D, 0xA4, 0x73, 0x6E, 0x94, 0xB9, 0xF9};
    thread_local EncryptedName<sizeof(source), 0x01CB4F1DF9D3F931ULL> name(source);
    return name.Text();
  }
  case 20420: {
    const std::uint8_t source[] = {0xA0, 0xF4, 0x71, 0xFF, 0x34, 0x37, 0xD8, 0x66, 0xB6, 0xFB, 0x40, 0xF6, 0x3B, 0x45};
    thread_local EncryptedName<sizeof(source), 0x0B8745519B1F95D7ULL> name(source);
    return name.Text();
  }
  case 20619: {
    const std::uint8_t source[] = {0xE8, 0x35, 0x0E, 0x84, 0xD8, 0x6C, 0x6D, 0xA3, 0xEA, 0x2B, 0x0F, 0x82, 0xD2, 0x71, 0x77, 0xF5, 0x8F};
    thread_local EncryptedName<sizeof(source), 0xC70319B5EB61598FULL> name(source);
    return name.Text();
  }
  case 20696: {
    const std::uint8_t source[] = {0xFC, 0xB5, 0xEA, 0x73, 0x13, 0x1A, 0x7A, 0x43, 0xF2, 0xB4, 0xED, 0x40, 0x15, 0x45};
    thread_local EncryptedName<sizeof(source), 0x271B45771F83DD9FULL> name(source);
    return name.Text();
  }
  case 20697: {
    const std::uint8_t source[] = {0x74, 0x39, 0x88, 0x2D, 0x5D, 0x9C, 0xE2, 0xE3, 0x7A, 0x38, 0x8F, 0x1E, 0x58, 0xC3};
    thread_local EncryptedName<sizeof(source), 0x8783C33941E15117ULL> name(source);
    return name.Text();
  }
  case 21089: {
    const std::uint8_t source[] = {0xFC, 0xEE, 0xC3, 0xC9, 0xF6, 0xDF, 0xEE, 0x9B, 0xEC, 0xF0, 0xC9, 0xD8, 0xF0, 0xC6, 0xA7};
    thread_local EncryptedName<sizeof(source), 0xD5A78DB38D8DAFABULL> name(source);
    return name.Text();
  }
  case 21290: {
    const std::uint8_t source[] = {0x7F, 0x2A, 0x31, 0xF0, 0xF6, 0x2C, 0xFC, 0x6F, 0x0F};
    thread_local EncryptedName<sizeof(source), 0x0399459DAF454F0FULL> name(source);
    return name.Text();
  }
  case 21291: {
    const std::uint8_t source[] = {0x10, 0x8E, 0xCB, 0xBE, 0xBA, 0xBC, 0xDF, 0x88, 0x73};
    thread_local EncryptedName<sizeof(source), 0xFFABE3D5D1BBEF73ULL> name(source);
    return name.Text();
  }
  case 1106: {
    const std::uint8_t source[] = {0x91, 0x7C, 0x78, 0x0C, 0xB5, 0xF3, 0xC8, 0xF2, 0x9A, 0x75, 0x6D, 0x49};
    thread_local EncryptedName<sizeof(source), 0xA597A7E7492B39D5ULL> name(source);
    return name.Text();
  }
  case 1198: {
    const std::uint8_t source[] = {0xC9, 0xFC, 0xE9, 0x28, 0xE8, 0x69, 0xE9, 0x18, 0xC8, 0xEE, 0xEF, 0x63};
    thread_local EncryptedName<sizeof(source), 0x51BB39B763BBBD8DULL> name(source);
    return name.Text();
  }
  case 3023: {
    const std::uint8_t source[] = {0xA9, 0x64, 0x05, 0x48, 0x9A, 0x96, 0xAC, 0xCB, 0xAA, 0x60, 0x57};
    thread_local EncryptedName<sizeof(source), 0x87E3D1C50D572DEFULL> name(source);
    return name.Text();
  }
  case 20525: {
    const std::uint8_t source[] = {0xB6, 0xAF, 0x11, 0x5C, 0xE5, 0xEE, 0xF3, 0x77, 0xB0, 0xAE, 0x18, 0x57, 0xF8, 0xE5, 0xA3};
    thread_local EncryptedName<sizeof(source), 0x3FA3AFA7035DE3FFULL> name(source);
    return name.Text();
  }
  case 21502: {
    const std::uint8_t source[] = {0x1B, 0xC6, 0x53, 0x35, 0x1A, 0x86, 0x62, 0x8F, 0x6B, 0x87};
    thread_local EncryptedName<sizeof(source), 0xDB27CB557D038759ULL> name(source);
    return name.Text();
  }
  case 23000: {
    const std::uint8_t source[] = {0x35, 0xC2, 0x39, 0x40, 0x4E, 0x46, 0x11, 0x31, 0x24, 0xD0, 0x22, 0x44, 0x5F, 0x3D, 0x5D};
    thread_local EncryptedName<sizeof(source), 0x7D5D0F110B6B8371ULL> name(source);
    return name.Text();
  }
  case 21630: {
    const std::uint8_t source[] = {0x1E, 0x24, 0x33, 0xA7, 0x98, 0x53, 0x06, 0x42, 0x0C, 0x5E, 0x7F};
    thread_local EncryptedName<sizeof(source), 0x114901C7E37F6D49ULL> name(source);
    return name.Text();
  }
  case 21631: {
    const std::uint8_t source[] = {0x13, 0xB9, 0x9A, 0x0C, 0xB2, 0x58, 0xCE, 0x91, 0x0E, 0xB9, 0x80, 0x05, 0xB6, 0x48, 0xD3, 0x8A, 0x03, 0xBC, 0xFD, 0x49};
    thread_local EncryptedName<sizeof(source), 0xDF8117FF49CFF551ULL> name(source);
    return name.Text();
  }
  case 21632: {
    const std::uint8_t source[] = {0x7B, 0x00, 0xD9, 0x84, 0xB0, 0x57, 0x8C, 0xEC, 0x6F, 0x73, 0x8B};
    thread_local EncryptedName<sizeof(source), 0xADC51BF9C98B413DULL> name(source);
    return name.Text();
  }
  case 1005: {
    const std::uint8_t source[] = {0x07, 0x7C, 0x05, 0x38, 0x70, 0xD3, 0x04, 0x26, 0x13, 0x3D};
    thread_local EncryptedName<sizeof(source), 0x674D9F3975573D41ULL> name(source);
    return name.Text();
  }
  case 21650: {
    const std::uint8_t source[] = {0x12, 0xFA, 0xB5, 0x62, 0x1B, 0x76, 0x94, 0x25, 0x18, 0xEA, 0xB5, 0x7F, 0x4F};
    thread_local EncryptedName<sizeof(source), 0x6BDF334F2BFDAD51ULL> name(source);
    return name.Text();
  }
  case 21651: {
    const std::uint8_t source[] = {0x84, 0x95, 0x98, 0x52, 0xD1, 0x7B, 0x30, 0x5C, 0x89, 0x84, 0x90, 0x57, 0xC0, 0x7C, 0x3C, 0x21, 0xCF};
    thread_local EncryptedName<sizeof(source), 0x136F2F9915D1DBCFULL> name(source);
    return name.Text();
  }
  case 21652: {
    const std::uint8_t source[] = {0x6C, 0x9F, 0x16, 0x78, 0x47, 0x98, 0x77, 0x4E, 0x60, 0x99, 0x00, 0x68, 0x47, 0x85, 0x23};
    thread_local EncryptedName<sizeof(source), 0x0B23D10F2F49D72BULL> name(source);
    return name.Text();
  }
  case 21653: {
    const std::uint8_t source[] = {0xA6, 0x91, 0x17, 0xB0, 0xE4, 0x34, 0xE3};
    thread_local EncryptedName<sizeof(source), 0xB3E307A5F753D5E3ULL> name(source);
    return name.Text();
  }
  case 23009: {
    const std::uint8_t source[] = {0xB5, 0x32, 0x67, 0x68, 0x14, 0x24, 0x72, 0x4B, 0xBA, 0x2B, 0x76, 0x7D, 0x4D};
    thread_local EncryptedName<sizeof(source), 0x192D654D29337BE5ULL> name(source);
    return name.Text();
  }
  case 21919: {
    const std::uint8_t source[] = {0x4D, 0x34, 0xAF, 0x8F, 0x6E, 0xE2, 0xF9, 0xFE, 0x4E, 0x39, 0xB8, 0x8D, 0x2F};
    thread_local EncryptedName<sizeof(source), 0xB7AFBD2FD9FD7501ULL> name(source);
    return name.Text();
  }
  case 21965: {
    const std::uint8_t source[] = {0xB8, 0x25, 0x5D, 0x9F, 0xB6, 0x76, 0x86, 0x09, 0xB2, 0x55};
    thread_local EncryptedName<sizeof(source), 0x79E915E9AF6F55DDULL> name(source);
    return name.Text();
  }
  case 21966: {
    const std::uint8_t source[] = {0x40, 0xE1, 0x35, 0x93, 0xBE, 0x68, 0x50, 0x0E, 0x52, 0xF8, 0x69, 0xC7, 0xD0, 0x01};
    thread_local EncryptedName<sizeof(source), 0x6B3301E1A3079125ULL> name(source);
    return name.Text();
  }
  case 21967: {
    const std::uint8_t source[] = {0xEC, 0x4D, 0x5F, 0xD5, 0xF8, 0x68, 0x44, 0x4A, 0xFE, 0x54, 0x03, 0x81, 0x95, 0x01};
    thread_local EncryptedName<sizeof(source), 0x2F2701A7E56D3D89ULL> name(source);
    return name.Text();
  }
  case 22185: {
    const std::uint8_t source[] = {0x27, 0x78, 0x2F, 0xEA, 0x06, 0xF9, 0xD0, 0x91, 0x27, 0x0B, 0x7D};
    thread_local EncryptedName<sizeof(source), 0xC39FB559A17D3963ULL> name(source);
    return name.Text();
  }
  case 23215: {
    const std::uint8_t source[] = {0xFF, 0xE8, 0x91, 0x76, 0x4E, 0x28, 0xE8, 0x33, 0xF0, 0xF1, 0x80, 0x63, 0x17};
    thread_local EncryptedName<sizeof(source), 0x71B7691737C5A1AFULL> name(source);
    return name.Text();
  }
  case 23216: {
    const std::uint8_t source[] = {0xE3, 0x00, 0xCC, 0xE2, 0xCB, 0x68, 0x45, 0x48, 0xF4, 0x1F, 0xDA, 0xF7, 0x85};
    thread_local EncryptedName<sizeof(source), 0x07112F85A39F4FABULL> name(source);
    return name.Text();
  }
  case 23233: {
    const std::uint8_t source[] = {0x02, 0x82, 0xD4, 0xAC, 0x6A, 0xAE, 0xB3, 0x00, 0x03, 0xF1};
    thread_local EncryptedName<sizeof(source), 0x45E3F103CBB5F157ULL> name(source);
    return name.Text();
  }
  case 23234: {
    const std::uint8_t source[] = {0xA5, 0xEE, 0x8E, 0x96, 0x4A, 0x62, 0x3F, 0xD6, 0xA1, 0x87};
    thread_local EncryptedName<sizeof(source), 0x936F3D2FFDFD87F5ULL> name(source);
    return name.Text();
  }
  case 22299: {
    const std::uint8_t source[] = {0x89, 0x15, 0xF2, 0x24, 0x57, 0x38, 0x7C, 0x2D};
    thread_local EncryptedName<sizeof(source), 0x2D33791F67AD47DFULL> name(source);
    return name.Text();
  }
  case 22318: {
    const std::uint8_t source[] = {0xBA, 0xEB, 0x0D, 0xC8, 0xDA, 0x70, 0x24, 0xC2, 0xAC, 0xFE, 0x5E, 0x91, 0xEA, 0x6B, 0x34, 0xC2, 0xDF};
    thread_local EncryptedName<sizeof(source), 0xA7471985F93F9BDFULL> name(source);
    return name.Text();
  }
  case 22381: {
    const std::uint8_t source[] = {0x14, 0xA3, 0x93, 0xEA, 0xF8, 0x2C, 0xA2, 0x19, 0x14, 0xEF};
    thread_local EncryptedName<sizeof(source), 0x5FFD7FABB5D1EF57ULL> name(source);
    return name.Text();
  }
  case 22382: {
    const std::uint8_t source[] = {0x92, 0x2D, 0xB3, 0xD6, 0x3C, 0x02, 0xBA, 0x87, 0x8B, 0x61};
    thread_local EncryptedName<sizeof(source), 0xD3E5516F89F161D1ULL> name(source);
    return name.Text();
  }
  case 1004: {
    const std::uint8_t source[] = {0xC5, 0x7C, 0x0D, 0x7D, 0x04, 0x91, 0x35};
    thread_local EncryptedName<sizeof(source), 0xF535E561137F138DULL> name(source);
    return name.Text();
  }
  case 22406: {
    const std::uint8_t source[] = {0x4A, 0x12, 0x28, 0xCD, 0xEF, 0x4E, 0xD5, 0xC6, 0x43, 0x1E, 0x3D, 0x83};
    thread_local EncryptedName<sizeof(source), 0x899D11BB83695B0DULL> name(source);
    return name.Text();
  }
  case 22405: {
    const std::uint8_t source[] = {0xB8, 0x6C, 0x00, 0xBD, 0x85, 0x60, 0xD4, 0x78, 0xC7, 0x25};
    thread_local EncryptedName<sizeof(source), 0x2B8725D7E95325F5ULL> name(source);
    return name.Text();
  }
  case 1269: {
    const std::uint8_t source[] = {0xFE, 0xF1, 0x54, 0x84, 0x66, 0x83};
    thread_local EncryptedName<sizeof(source), 0x7989830DE73B9DBDULL> name(source);
    return name.Text();
  }
  case 22504: {
    const std::uint8_t source[] = {0x28, 0x54, 0xF1, 0xAA, 0xF0, 0xB8, 0xDF, 0xE2, 0x26, 0x6A, 0xCB, 0xA4, 0xE0, 0xB8, 0xDF, 0xBD};
    thread_local EncryptedName<sizeof(source), 0xBDADDD87C5A50B6BULL> name(source);
    return name.Text();
  }
  case 1754: {
    const std::uint8_t source[] = {0x18, 0x98, 0x32, 0xC2, 0xB6, 0x24, 0xB4, 0x8B, 0x0F, 0xD3};
    thread_local EncryptedName<sizeof(source), 0xC7FB6DF18577D34BULL> name(source);
    return name.Text();
  }
  case 22505: {
    const std::uint8_t source[] = {0xBC, 0x1F, 0xF2, 0x1C, 0x4D, 0x04, 0x70, 0xF1, 0xA9, 0x51};
    thread_local EncryptedName<sizeof(source), 0xBD3F430159B551FDULL> name(source);
    return name.Text();
  }
  case 23434: {
    const std::uint8_t source[] = {0x45, 0xFC, 0x6C, 0x1C, 0xD3, 0x34, 0x12, 0x65, 0x01};
    thread_local EncryptedName<sizeof(source), 0x2B477F9B5B39B301ULL> name(source);
    return name.Text();
  }
  case 23435: {
    const std::uint8_t source[] = {0xD1, 0x24, 0xEA, 0xF4, 0x3E, 0x6B, 0xC2, 0x0E, 0xC4, 0x24, 0xA7};
    thread_local EncryptedName<sizeof(source), 0x5B832361BDA77197ULL> name(source);
    return name.Text();
  }
  case 22506: {
    const std::uint8_t source[] = {0x2D, 0xA2, 0x6C, 0xDA, 0x75, 0xFC, 0xB9, 0x49, 0x3C, 0xB9, 0x13, 0x9F};
    thread_local EncryptedName<sizeof(source), 0x0DF5B33D9F21EB79ULL> name(source);
    return name.Text();
  }
  default: {
    const std::uint8_t source[] = {0xAF, 0x32, 0x51, 0xF4, 0xE3, 0xEE, 0xE7};
    thread_local EncryptedName<sizeof(source), 0x01E7898D9D235DDFULL> name(source);
    return name.Text();
  }
  }
}
