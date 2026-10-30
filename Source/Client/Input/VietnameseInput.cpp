#include "Client/Input/VietnameseInput.h"

#include <cstring>

// GLOBAL: gl_24898
unsigned char g_telexToneKeys[6] = "sfrxj";

// GLOBAL: gl_24899
unsigned short g_vietLowerChars[74] = {
  0xEC61, 0xCC61, 0xD261, 0xDE61, 0xF261, 0x0061, 0xECE2, 0xCCE2, 0xD2E2, 0xDEE2, 0xF2E2, 0x00E2, 0xECE3, 0xCCE3, 0xD2E3,
  0xDEE3, 0xF2E3, 0x00E3, 0xEC65, 0xCC65, 0xD265, 0xDE65, 0xF265, 0x0065, 0xECEA, 0xCCEA, 0xD2EA, 0xDEEA, 0xF2EA, 0x00EA,
  0xEC69, 0xCC69, 0xD269, 0xDE69, 0xF269, 0x0069, 0xEC6F, 0xCC6F, 0xD26F, 0xDE6F, 0xF26F, 0x006F, 0xECF4, 0xCCF4, 0xD2F4,
  0xDEF4, 0xF2F4, 0x00F4, 0xECF5, 0xCCF5, 0xD2F5, 0xDEF5, 0xF2F5, 0x00F5, 0xEC75, 0xCC75, 0xD275, 0xDE75, 0xF275, 0x0075,
  0xECFD, 0xCCFD, 0xD2FD, 0xDEFD, 0xF2FD, 0x00FD, 0xEC79, 0xCC79, 0xD279, 0xDE79, 0xF279, 0x0079, 0x0064, 0x00F0,
};

// GLOBAL: gl_24900
unsigned short g_vietUpperChars[74] = {
  0xEC41, 0xCC41, 0xD241, 0xDE41, 0xF241, 0x0041, 0xECC2, 0xCCC2, 0xD2C2, 0xDEC2, 0xF2C2, 0x00C2, 0xECC3, 0xCCC3, 0xD2C3,
  0xDEC3, 0xF2C3, 0x00C3, 0xEC45, 0xCC45, 0xD245, 0xDE45, 0xF245, 0x0045, 0xECCA, 0xCCCA, 0xD2CA, 0xDECA, 0xF2CA, 0x00CA,
  0xEC49, 0xCC49, 0xD249, 0xDE49, 0xF249, 0x0049, 0xEC4F, 0xCC4F, 0xD24F, 0xDE4F, 0xF24F, 0x004F, 0xECD4, 0xCCD4, 0xD2D4,
  0xDED4, 0xF2D4, 0x00D4, 0xECD5, 0xCCD5, 0xD2D5, 0xDED5, 0xF2D5, 0x00D5, 0xEC55, 0xCC55, 0xD255, 0xDE55, 0xF255, 0x0055,
  0xECDD, 0xCCDD, 0xD2DD, 0xDEDD, 0xF2DD, 0x00DD, 0xEC59, 0xCC59, 0xD259, 0xDE59, 0xF259, 0x0059, 0x0044, 0x00D0,
};

// OFFSET: 0x00AFBB20
// FUNCTION: fn_18307
TELEXInputMode::TELEXInputMode() {
}

// OFFSET: 0x00AFBB40
// FUNCTION: fn_18308
int TELEXInputMode::Convert(char key, char vowel, int index) {
  if (key == this->m_lastKey) {
    this->Reset();
    return 1000;
  }
  int tone = 0;
  while (key != g_telexToneKeys[tone] && tone < 5) {
    ++tone;
  }
  if (tone < 5) {
    if (vowel != 'D' && vowel != 'd') {
      if (this->m_toneIndex > 0) {
        index = index - this->m_toneIndex + 6;
      }
      this->m_lastKey = key;
      this->m_toneIndex = tone + 1;
      return index - 5 + tone;
    }
  } else if (key == vowel) {
    if (vowel != 'U' && vowel != 'u' && vowel != 'Y' && vowel != 'y') {
      if (key == 'd' || key == 'D') {
        this->m_markKey = key;
        this->m_lastKey = key;
        return 73;
      }
      if (this->m_markKey > 0 && (this->m_markKey == 'w' || this->m_markKey == 'W')) {
        index -= 12;
      }
      this->m_markKey = key;
      this->m_lastKey = key;
      return index + 6;
    }
  } else if ((key == 'w' || key == 'W') && vowel != 'E' && vowel != 'e' && vowel != 'Y' && vowel != 'y' && vowel != 'D' && vowel != 'd') {
    if (this->m_markKey > 0 && this->m_markKey == vowel) {
      index -= 6;
    }
    this->m_markKey = key;
    this->m_lastKey = key;
    return index + 12;
  }
  this->Reset();
  return -1;
}

// OFFSET: 0x00AFBC80
// FUNCTION: fn_18309
int TELEXInputMode::IsModifierKey(char key, int count, char vowel) {
  if (key != vowel && key != 'w' && key != 'W') {
    int tone = 0;
    while (key != g_telexToneKeys[tone] && tone < 5) {
      ++tone;
    }
    if (tone >= 5) {
      this->Reset();
      return 0;
    }
  }
  return 1;
}

// OFFSET: 0x00AFBCE0
// FUNCTION: fn_18310
void CInputMode::Reset() {
  this->m_lastKey = 127;
  this->m_toneIndex = -1;
  this->m_markKey = -1;
}

// OFFSET: 0x00AFBD00
// FUNCTION: fn_18311
CVietnameseInput::CVietnameseInput() {
  std::memset(this->m_keys, 0, sizeof(this->m_keys) + sizeof(this->m_chars));
  this->m_state = 1001;
  this->m_count = 0;
  this->m_mode = nullptr;
}

// OFFSET: 0x00AFBE20
// FUNCTION: fn_18312
int CVietnameseInput::Release() {
  if (this->m_mode) {
    delete this->m_mode;
  }
  return 1;
}

// OFFSET: 0x00AFBE40
// FUNCTION: fn_18313
int CVietnameseInput::Compose(char key, unsigned short* output, int* replaced) {
  int index;
  if (this->m_state == 1002) {
    this->m_vowel = key;
    this->m_state = 1003;
    switch (key) {
    case 'A':
    case 'a':
      index = 5;
      break;
    case 'E':
    case 'e':
      index = 23;
      break;
    case 'I':
    case 'i':
      index = 35;
      break;
    case 'O':
    case 'o':
      index = 41;
      break;
    case 'U':
    case 'u':
      index = 59;
      break;
    case 'Y':
    case 'y':
      index = 71;
      break;
    case 'D':
    case 'd':
      index = 72;
      break;
    default:
      this->m_state = 1001;
      index = -1;
      break;
    }
  } else if (this->m_state == 1003) {
    index = this->m_mode->Convert(key, this->m_vowel, this->m_index);
    if (index == 1000) {
      *output = this->m_chars[this->m_count - 2];
      this->Reset(0);
      return 1001;
    }
    if (index < 0) {
      *output = this->m_chars[this->m_count - 1];
      this->Reset(0);
      return 1001;
    }
    *replaced = 0;
  } else {
    index = reinterpret_cast<int>(output);
  }
  this->m_index = index;
  if (this->m_vowel >= 'a') {
    *output = g_vietLowerChars[index];
  } else {
    *output = g_vietUpperChars[index];
  }
  if (this->m_count > 9) {
    this->m_keys[1] = this->m_keys[8];
    this->m_keys[2] = this->m_keys[9];
    this->m_chars[1] = this->m_chars[8];
    this->m_chars[2] = this->m_chars[9];
    this->m_count = 3;
  }
  this->m_keys[this->m_count] = key;
  this->m_chars[this->m_count] = *output;
  ++this->m_count;
  return this->m_state;
}

// OFFSET: 0x00AFC010
// FUNCTION: fn_18314
bool CVietnameseInput::SetMode(int type) {
  this->m_type = type;
  if (this->m_mode) {
    delete this->m_mode;
  }
  if (type == 0) {
    this->m_mode = new VNIInputMode;
  } else if (type == 1) {
    this->m_mode = new TELEXInputMode;
  }
  return this->m_mode != nullptr;
}

// OFFSET: 0x00AFC0D0
// FUNCTION: fn_18315
int CVietnameseInput::IsComposeKey(char key) {
  if (this->m_state == 1003) {
    if (this->m_mode->IsModifierKey(key, this->m_count, this->m_vowel)) {
      return 1;
    }
    this->Reset(0);
  }
  if (key == 'A' || key == 'a' || key == 'E' || key == 'e' || key == 'I' || key == 'i' || key == 'O' || key == 'o' || key == 'U' || key == 'u' ||
      key == 'Y' || key == 'y' || key == 'D' || key == 'd') {
    if (this->m_state != 1002) {
      this->m_state = 1002;
    }
    return 1;
  }
  return 0;
}

// OFFSET: 0x00AFC1A0
// FUNCTION: fn_18316
void CVietnameseInput::Reset(int resetMode) {
  if (resetMode && this->m_mode) {
    this->m_mode->Reset();
  }
  std::memset(this->m_keys, 0, sizeof(this->m_keys) + sizeof(this->m_chars));
  this->m_state = 1001;
  this->m_count = 0;
  this->m_vowel = 0;
}

// OFFSET: 0x00AFC1F0
// FUNCTION: fn_18317
VNIInputMode::VNIInputMode() {
}

// OFFSET: 0x00AFC210
// FUNCTION: fn_18318
int VNIInputMode::Convert(char key, char vowel, int index) {
  if (key == this->m_lastKey) {
    this->Reset();
    return 1000;
  }
  switch (key) {
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
    if (vowel != 'D' && vowel != 'd') {
      if (this->m_toneIndex > 0) {
        index = index - this->m_toneIndex + 6;
      }
      this->m_lastKey = key;
      this->m_toneIndex = key - '0';
      return index - '6' + key;
    }
    break;
  case '6':
    if (vowel == 'A' || vowel == 'a' || vowel == 'E' || vowel == 'e' || vowel == 'O' || vowel == 'o') {
      this->m_lastKey = key;
      if (this->m_markKey > 0) {
        index -= this->m_markKey;
      }
      this->m_markKey = 6;
      return index + 6;
    }
    break;
  case '7':
    if (vowel == 'O' || vowel == 'o') {
      this->m_lastKey = key;
      if (this->m_markKey > 6) {
        index -= this->m_markKey;
      }
      this->m_markKey = 12;
      return index + 12;
    }
    if (vowel == 'U' || vowel == 'u') {
      this->m_lastKey = key;
      this->m_markKey = 6;
      return index + 6;
    }
    break;
  case '8':
    if (vowel == 'A' || vowel == 'a') {
      this->m_lastKey = key;
      if (this->m_markKey > 0) {
        index -= this->m_markKey;
      }
      this->m_markKey = 12;
      return index + 12;
    }
    break;
  case '9':
    if (vowel == 'D' || vowel == 'd') {
      index = 73;
      this->m_lastKey = key;
    }
    return index;
  default:
    return index;
  }
  this->Reset();
  return -1;
}

// OFFSET: 0x00AFC380
// FUNCTION: fn_18319
int VNIInputMode::IsModifierKey(char key, int count, char vowel) {
  if (key >= '1' && key <= '9') {
    return 1;
  }
  this->Reset();
  return 0;
}
