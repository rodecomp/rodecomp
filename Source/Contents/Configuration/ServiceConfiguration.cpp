#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Registry.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Text/UIBalloonText.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <windows.h>

// GLOBAL: gl_24542
signed char g_language = 0;

// GLOBAL: gl_24544
InputCharset* g_serverLanguage;

// GLOBAL: gl_24972
unsigned char g_startupSecondPhase;

// GLOBAL: gl_24760
bool g_serviceFlag05;

// GLOBAL: gl_24761
bool g_serviceFlag06;

// GLOBAL: gl_24529
int g_serviceType = 0;

// GLOBAL: gl_24968
const char* g_licenceFileName = "";

// GLOBAL: gl_24969
const char* g_clientExeName = "";

// GLOBAL: gl_24970
const char* g_patcherExeName = "";

// GLOBAL: gl_24971
const char* g_runPatcherMessage = "";

// GLOBAL: gl_21921
const char* g_windowName = "";

// GLOBAL: gl_24974
char g_launchServerName[24];

template <std::size_t Length, std::uint64_t Key>
class EncryptedText {
public:
  explicit EncryptedText(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedText() {
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
const char* DecryptText(const std::uint8_t (&source)[Length]) {
  thread_local EncryptedText<Length, Key> text(source);
  return text.Text();
}

// OFFSET: 0x00ACFCF0
// FUNCTION: fn_17807
const char* GetLicenceText() {
  // GLOBAL: gl_24973
  static std::string text;

  if (text.empty()) {
    CFile file;
    if (file.Open(g_licenceFileName, 0, true, false)) {
      text.assign(static_cast<const char*>(file.GetData()), file.GetSize());
      file.Close();
    } else {
      const std::uint8_t source[] = {0x25, 0x64, 0x85, 0x80, 0xD2, 0x91, 0xEC, 0x9D, 0x1F, 0x6E, 0xDD, 0xB1, 0xA7, 0xBB, 0xC4, 0xD1, 0x0E,
                                     0x25, 0x85, 0xED, 0xEB, 0xB4, 0xCE, 0xD8, 0x05, 0x68, 0xC0, 0xEB, 0xF3, 0xA5, 0xD9, 0x94, 0x6B};
      text = DecryptText<0xBDADDD87C5A50B6BULL>(source);
    }
  }

  return text.c_str();
}

void sub_AD1270();
void sub_AD1780();

// OFFSET: 0x00ACFF90
// FUNCTION: fn_17808
void InitializeServiceStrings() {
  sub_AD1270();

  switch (g_serviceType) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 17:
  case 18:
  case 19:
  case 20:
    break;
  default: {
    const std::uint8_t source[] = {0x5C, 0xC7, 0x2C, 0x31, 0xB6, 0x2A, 0x1B, 0xF3, 0x5A, 0xCC, 0x35, 0x29,
                                   0xB0, 0x3E, 0x10, 0x87, 0x70, 0xD9, 0x22, 0x7F, 0xF8, 0x7C, 0x54, 0xD3};
    ShowErrorDialog(GetRenderSystemHolder()->system, DecryptText<0xD3755DD95F47A909ULL>(source), 0);
    break;
  }
  }

  sub_AD1780();

  if (g_serviceType == 0 || g_serviceType == 6) {
    g_nameBalloonFontHeight = 13;
    g_serviceFlag05 = true;
  } else {
    g_nameBalloonFontHeight = 12;
    g_serviceFlag05 = false;
  }
  g_serviceFlag06 = g_serviceFlag05;

  SYSTEMTIME time;
  GetLocalTime(&time);

  {
    const std::uint8_t source[] = {0x63, 0x0B, 0xC5, 0x25, 0x3A, 0x20, 0xE4, 0x69, 0x2E, 0x40, 0xB7, 0x3D, 0x2B, 0x37, 0x81};
    g_licenceFileName = DecryptText<0x078143534999254DULL>(source);
  }
  g_startupSecondPhase = static_cast<unsigned char>(time.wSecond) & 3;

  {
    const std::uint8_t source[] = {0xD6, 0x0E, 0xED, 0x63, 0x08, 0x00, 0xD7, 0xCA, 0xD9, 0x26, 0xF9, 0x76, 0x09, 0x08, 0xD1, 0xD6,
                                   0xA5, 0x32, 0xE4, 0x71, 0x0B, 0x3D, 0xF7, 0xCE, 0xE2, 0x0F, 0xEA, 0x65, 0x10, 0x0A, 0xA5};
    g_subKey = DecryptText<0xAFA5617F178B6185ULL>(source);
  }

  {
    const std::uint8_t source[] = {0xBB, 0xB2, 0x5A, 0x69, 0x54, 0x5B, 0x64, 0x8E, 0xE9};
    g_windowName = DecryptText<0xE50B2935073DD3E9ULL>(source);
  }

  switch (g_serviceMode) {
  case 2: {
    const std::uint8_t client[] = {0x59, 0xD4, 0xEE, 0x5C, 0x67, 0xD3, 0xF6, 0xAC, 0x79, 0x9B, 0xEC, 0x6D, 0x6C, 0xB7};
    g_clientExeName = DecryptText<0xC399B7091589B50BULL>(client);
    const std::uint8_t patcher[] = {0x50, 0x03, 0xAB, 0x90, 0x32, 0x73, 0xB4, 0x0F, 0x37, 0x08, 0xB7, 0x9A, 0x5D};
    g_patcherExeName = DecryptText<0x7FC1015DFFCF6D19ULL>(patcher);
    const std::uint8_t message[] = {0x69, 0x8D, 0xCC, 0x8E, 0x92, 0xC2, 0x73, 0x75, 0x4C, 0x8F, 0x89, 0xBD, 0x80,
                                    0xC0, 0x1A, 0x69, 0x5D, 0x8E, 0xC6, 0x9D, 0xCF, 0xC2, 0x2B, 0x62, 0x39};
    g_runPatcherMessage = DecryptText<0x0753A7E1EFA9E139ULL>(message);
    break;
  }
  case 3: {
    const std::uint8_t client[] = {0x1F, 0xB0, 0xAC, 0x80, 0x8C, 0x1E, 0xA1, 0x1B, 0x63, 0xB4, 0xB3, 0xA2, 0xF9};
    g_clientExeName = DecryptText<0x7FCD77F9C7CBD14DULL>(client);
    const std::uint8_t patcher[] = {0x64, 0xF6, 0xB2, 0xEF, 0x25, 0xC8, 0x4F, 0xE5, 0x46, 0xFB, 0xBE, 0x83};
    g_patcherExeName = DecryptText<0xCB3FBD4183DB8323ULL>(patcher);
    const std::uint8_t message[] = {0xBF, 0x4F, 0xB0, 0x8E, 0x6A, 0x34, 0xB1, 0x49, 0x9A, 0x4D, 0xF5, 0xBD,
                                    0x78, 0x36, 0xD6, 0x4E, 0x86, 0x4F, 0xB1, 0xC1, 0x7C, 0x29, 0xF4, 0x3B};
    g_runPatcherMessage = DecryptText<0x3B915119EFD523EFULL>(message);
    break;
  }
  default: {
    const std::uint8_t dev[] = {0xEB, 0x2A, 0x5D, 0xE3};
    if (std::strstr(g_launchServerName, DecryptText<0x3F5315DFE32B4FAFULL>(dev))) {
      const std::uint8_t client[] = {0xED, 0xDC, 0xB8, 0x51, 0x2C, 0x5F, 0xEC, 0x58, 0xEE, 0xF0, 0xF1, 0x5A, 0x35, 0x48, 0x83};
      g_clientExeName = DecryptText<0x33832D4D3FDFBDBFULL>(client);
      const std::uint8_t patcher[] = {0xA1, 0xA2, 0x9D, 0xDE, 0xC5, 0x78, 0x05, 0x2C, 0xBC, 0xED, 0x8C, 0xC5, 0xC8, 0x0D};
      g_patcherExeName = DecryptText<0x7D750DADBDE9C3F1ULL>(patcher);
      const std::uint8_t message[] = {0x45, 0xFF, 0xD6, 0xA0, 0x94, 0x26, 0x71, 0x9F, 0x60, 0xFD, 0x93, 0x93, 0x86,
                                      0x24, 0x3F, 0x8C, 0x67, 0xFC, 0xD8, 0x90, 0xAA, 0x6D, 0x34, 0x95, 0x70, 0x93};
      g_runPatcherMessage = DecryptText<0xED5143E7C1B39315ULL>(message);
    } else {
      const std::uint8_t client[] = {0x8B, 0x58, 0x74, 0x17, 0xAA, 0xF1, 0x12, 0x94, 0xF7, 0x5C, 0x6B, 0x1C, 0xCB};
      g_clientExeName = DecryptText<0xFF7D83CB791339D9ULL>(client);
      const std::uint8_t patcher[] = {0xC5, 0x9C, 0x65, 0xD0, 0x39, 0x10, 0xE5, 0x1F, 0xF0, 0x85, 0x74, 0xB3};
      g_patcherExeName = DecryptText<0x31956551B311FD95ULL>(patcher);
      const std::uint8_t message[] = {0xC1, 0xAF, 0xD8, 0x00, 0x16, 0xF4, 0x1D, 0x4B, 0xE4, 0xAD, 0x9D, 0x33,
                                      0x04, 0xF6, 0x53, 0x58, 0xE3, 0xAC, 0xD6, 0x4F, 0x00, 0xE9, 0x58, 0x39};
      g_runPatcherMessage = DecryptText<0x393D916561BDC391ULL>(message);
    }
    break;
  }
  }
}
