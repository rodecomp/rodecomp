#include "Client/Input/LanguageTC.h"
#include "Network/TextEncoding.h"
#include <cstring>
#include <windows.h>

// OFFSET: 0x00ACCD30
// FUNCTION: fn_17752
CLanguageTC::CLanguageTC() : keys() {
  // STATIC: 0x011594E0
  // GLOBAL: gl_13427
  static constexpr std::array<std::uint8_t, 83> zhuyinBig5 = {0xA3, 0x74, 0xA3, 0x78, 0xA3, 0xBE, 0xA3, 0xBF, 0xA3, 0xA4, 0xA3, 0xBD, 0xA3, 0xBB, 0xA3, 0xAB, 0xA3,
                                                0xAF, 0xA3, 0xB3, 0xA3, 0x76, 0xA3, 0xA7, 0xA3, 0x74, 0xA3, 0x74, 0xA3, 0x74, 0xA3, 0xA2, 0xA3, 0xA6,
                                                0xA3, 0xA9, 0xA3, 0xAC, 0xA3, 0xB9, 0xA3, 0xAD, 0xA3, 0xB1, 0xA3, 0xAA, 0xA3, 0xBA, 0xA3, 0xB0, 0xA3,
                                                0xB4, 0xA3, 0x75, 0xA3, 0xA1, 0xA3, 0x7A, 0xA3, 0xA5, 0xA3, 0x74, 0xA3, 0xA3, 0xA3, 0x79, 0xA3, 0x74,
                                                0xA3, 0xA8, 0xA3, 0x77, 0xA3, 0xB7, 0xA3, 0xB5, 0xA3, 0xAE, 0xA3, 0xB2, 0xA3, 0xB6, 0x00};
  const auto zhuyin = zhuyinBig5;
  const char categories[] = "11441443331111111132331233111121111133333";
  const unsigned char keys[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                           'I', 'J', 'K', 'L', 'N', 'M', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                           VK_OEM_MINUS, VK_OEM_1, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2};

  this->keyStroke = 0;

  for (int index = 0; index < 256; ++index) {
    this->mappings[index].category = 4;
  }

  for (unsigned index = 0; index < 41; ++index) {
    auto& mapping = this->mappings[keys[index]];
    mapping.bytes[0] = zhuyin[2 * index];
    mapping.bytes[1] = zhuyin[2 * index + 1];
    mapping.bytes[2] = 0;
    mapping.category = static_cast<std::uint8_t>(categories[index] - '1');
  }

  this->mappings['C'].bytes[1] = 0x7E;
  this->mappings['D'].bytes[1] = 0x7D;
  this->mappings['E'].bytes[1] = 0x7C;
  this->mappings['U'].bytes[1] = 0xB8;
  this->mappings['X'].bytes[1] = 0x7B;
}

// OFFSET: 0x00ACD260
// FUNCTION: fn_17758
CLanguageTC::~CLanguageTC() = default;

// OFFSET: 0x00ACF330
// FUNCTION: fn_17795
void CLanguageBase::ProcessKey(unsigned char) {
}

// OFFSET: 0x00ACF340
// FUNCTION: fn_17796
void CLanguageTC::ProcessKey(unsigned char key) {
  if (key != 8) {
    for (auto entry = this->keys.begin(); entry != this->keys.end(); ++entry) {
      if (this->mappings[*entry].category == this->mappings[key].category) {
        this->keys.erase(entry);
        break;
      }
    }

    this->keys.push_back(key);
  } else if (!this->keys.empty()) {
    this->keys.resize(this->keys.size() - 1);
  }
}

// OFFSET: 0x00ACE270
// FUNCTION: fn_17777
const char* CLanguageBase::DisplayText() const {
  return this->composition.c_str();
}

// OFFSET: 0x00ACE280
// FUNCTION: fn_17778
const char* CLanguageTC::DisplayText() const {
  char bytes[9] = {};

  for (std::uint8_t key : this->keys) {
    const auto& mapping = this->mappings[key];

    if (mapping.category < 4) {
      std::memcpy(&bytes[2 * mapping.category], mapping.bytes, 2);
    }
  }

  const_cast<std::string&>(this->composition).assign(bytes, 8);
  return this->composition.c_str();
}

// OFFSET: 0x00ACE320
// FUNCTION: fn_17779
int CLanguageBase::DisplayLength() const {
  return static_cast<int>(this->composition.size());
}

// OFFSET: 0x00ACE330
// FUNCTION: fn_17780
int CLanguageTC::DisplayLength() const {
  if (this->keys.size() == 1 && this->mappings[this->keys.back()].category == 4) {
    return 0;
  }

  return static_cast<int>(this->keys.size());
}

// OFFSET: 0x00ACE470
// FUNCTION: fn_17784
const char* CLanguageBase::NextChar(const char* current, int) {
  return CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
}

// OFFSET: 0x00ACE490
// FUNCTION: fn_17785
const char* CLanguageTH::NextChar(const char* current, int) {
  return CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
}

// OFFSET: 0x00ACD7D0
// FUNCTION: fn_17768
void CLanguageBase::EndComposition() {
}

// OFFSET: 0x00ACD7E0
// FUNCTION: fn_17769
void CLanguageTC::EndComposition() {
  this->keys.clear();
}

// OFFSET: 0x00ACD4A0
// FUNCTION: fn_17761
void CLanguageBase::ResetState() {
}

// OFFSET: 0x00ACD4E0
// FUNCTION: fn_17762
void CLanguageBase::ResetCandidate() {
}

// OFFSET: 0x00ACD4F0
// FUNCTION: fn_17763
void CLanguageTC::ResetCandidate() {
  if (!this->keys.empty()) {
    const auto key = this->keys.back();
    this->keys.clear();
    if (this->mappings[key].category < 4) {
      this->keys.push_back(key);
    }
  }
}

// OFFSET: 0x00ACE000
// FUNCTION: fn_17776
void CLanguageTC::RefreshComposition() {
  CLanguageBase::RefreshComposition();
  if (this->candidate.size() > 2 && std::strcmp(this->candidate.c_str() + this->candidate.size() - 2, "?") == 0) {
    this->candidate.resize(this->candidate.size() - 2);
  }
}
