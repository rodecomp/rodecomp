#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Archive/PakRegistry.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Registry.h"
#include "Contents/MapSkyMgr.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/XmlNode.h"
#include "UI/Text/UIBalloonText.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
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

// GLOBAL: gl_25024
std::vector<std::string> g_loadingScreens;

// GLOBAL: gl_25276
bool g_hideAccountList;

// GLOBAL: gl_25277
bool g_extendedSlot;

// GLOBAL: gl_25278
bool g_readFolder;

// GLOBAL: gl_25279
bool g_clientInfoLoaded;

// GLOBAL: gl_25280
char g_homepageUrl[256];

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

namespace {
  class EtcInfoBytes : public ResourceBytes {
  public:
    CFile file;

    // INLINED
    bool Open(const char* name) override {
      return this->file.Open(name, 0, true, false);
    }

    // INLINED
    const void* Data() const override {
      return this->file.GetData();
    }

    // INLINED
    int Size() const override {
      return static_cast<int>(this->file.GetSize());
    }

    // INLINED
    void Close() override {
      this->file.Close();
    }
  };
}

// OFFSET: 0x00AD0F60
// FUNCTION: fn_17809
void LoadLoadingScreens(int count) {
  if (count == 0) {
    switch (g_serviceType) {
    case 0:
      count = 7;
      break;
    case 1:
    case 15:
      count = 12;
      break;
    case 3:
      count = 4;
      break;
    case 5:
    case 7:
      count = 8;
      break;
    case 6:
      count = 9;
      break;
    default:
      count = 1;
      break;
    }
  }

  for (int index = 0; index < count; ++index) {
    const std::uint8_t source[] = {0x7D, 0x32, 0xFB, 0x1D, 0xCD, 0x4D, 0x4C, 0xB4, 0x7B, 0x79, 0xFB, 0x2B, 0xB0, 0x39, 0xD5, 0x15,
                                   0xD2, 0xFC, 0x5F, 0x8E, 0x63, 0xE4, 0xAC, 0x49, 0x8F, 0xF9, 0x15, 0x8D, 0x7D, 0xE4, 0x89};
    char name[256];
    std::sprintf(name, DecryptText<0x7989830DE73B9DBDULL>(source), index);
    if (ResourceMgr::Instance().Exists(name)) {
      g_loadingScreens.push_back(std::string(name));
    }
  }
}

// OFFSET: 0x00AD1270
// FUNCTION: fn_17810
void sub_AD1270() {
  if (ResourceExists("etcinfo.txt", 0)) {
    CCommonObject storage;
    g_loadingScreens.clear();

    std::vector<char*> tokens;
    EtcInfoBytes file;
    storage.LoadTokens(tokens, "etcinfo.txt", 0, file);

    for (auto token = tokens.begin(); token != tokens.end(); ++token) {
      if (std::strcmp(*token, "loadingscreen") == 0) {
        const int count = std::atoi(token[1]);
        if (count > 0) {
          LoadLoadingScreens(count);
        }
      } else if (std::strcmp(*token, "event") == 0) {
        if (std::atoi(token[1]) > 0) {
          g_pakRegistry.Add("event.grf");
        }
      } else if (std::strcmp(*token, "weather") == 0) {
        std::string mapName(token[1]);
        token += 2;
        std::string weather(*token);
        LowercaseText(mapName);
        LowercaseText(weather);
        g_weatherEffect.AddMapWeather(mapName, weather);
      }
    }
  }

  if (g_loadingScreens.empty()) {
    LoadLoadingScreens(0);
  }
}

// OFFSET: 0x00AD2A00
// FUNCTION: fn_17818
void ReadClientInfo(XmlNode* clientInfo) {
  XmlNode* serviceType =
      clientInfo->FindChild(DecryptText<0x5B8F49AF09390D1BULL>({0x68, 0x68, 0x4B, 0x7F, 0xC6, 0x2A, 0xEA, 0x2F, 0x62, 0x7D, 0x5C, 0x09}));
  if (serviceType) {
    if (serviceType->Text() == std::string(DecryptText<0x3F557BE333F95F6DULL>({0x06, 0x30, 0x8B, 0x56, 0x82, 0x7B}))) {
      g_serviceType = 0;
    } else if (serviceType->Text() == std::string(DecryptText<0x474DC581D5E98357ULL>({0x36, 0xEE, 0x8C, 0xA7, 0xE8, 0xA6, 0x2C, 0x47}))) {
      g_serviceType = 1;
    } else if (serviceType->Text() == std::string(DecryptText<0x01FF9313695DD9DFULL>({0xB5, 0xB8, 0x2D, 0x08, 0x7D, 0x93}))) {
      g_serviceType = 2;
    } else if (serviceType->Text() == std::string(DecryptText<0x1BF915E1F37DFDFDULL>({0x9E, 0x95, 0x14, 0x9D, 0x80, 0x15}))) {
      g_serviceType = 3;
    } else if (serviceType->Text() == std::string(DecryptText<0x49C3759B77DBD3C1ULL>({0xB5, 0xB2, 0xB2, 0x00, 0xFA, 0x1B, 0xC3}))) {
      g_serviceType = 4;
    } else if (serviceType->Text() == std::string(DecryptText<0x6DED3DD1CF75AD9BULL>({0xEF, 0xC5, 0x14, 0xA6, 0xD1}))) {
      g_serviceType = 5;
    } else if (serviceType->Text() ==
               std::string(DecryptText<0x91E147EBBD47C39FULL>({0xF6, 0xAD, 0x23, 0xD2, 0x85, 0x22, 0x92, 0xF8, 0xFE, 0xC3}))) {
      g_serviceType = 6;
    } else if (serviceType->Text() ==
               std::string(DecryptText<0xD1538513DF975701ULL>({0x71, 0x3F, 0xFE, 0xB3, 0x7A, 0xF5, 0x23, 0xB8, 0x6F, 0x32, 0x97}))) {
      g_serviceType = 7;
    } else if (serviceType->Text() ==
               std::string(DecryptText<0xA1A55B4D1597EDD3ULL>({0xBE, 0x8C, 0xFB, 0x74, 0x34, 0x28, 0xCC, 0xC0, 0xD3}))) {
      g_serviceType = 8;
    } else if (serviceType->Text() ==
               std::string(DecryptText<0x4F1791FB1503C713ULL>({0x60, 0xAE, 0x6D, 0x72, 0x9A, 0xE1, 0x78, 0x3D, 0x76, 0xC7}))) {
      g_serviceType = 9;
    } else if (serviceType->Text() ==
               std::string(DecryptText<0x7D993F650D2785FFULL>({0x98, 0xE0, 0x55, 0x60, 0x04, 0x51, 0xE0, 0x7D}))) {
      g_serviceType = 10;
    } else if (serviceType->Text() == std::string(DecryptText<0x61FF87ED7BF39BE1ULL>({0x88, 0xF5, 0x97, 0x12, 0x8C, 0x87}))) {
      g_serviceType = 11;
    } else if (serviceType->Text() == std::string(DecryptText<0x151F77EBEBBB2147ULL>({0x25, 0x53, 0xDA, 0x91, 0x82, 0x1B, 0x1F}))) {
      g_serviceType = 12;
    } else if (serviceType->Text() ==
               std::string(DecryptText<0x2D394DBD0F332733ULL>({0x52, 0x52, 0x40, 0x7B, 0xCF, 0x2C, 0x55, 0x44, 0x52, 0x27}))) {
      g_serviceType = 13;
    } else if (serviceType->Text() == std::string(DecryptText<0x69D345DB950F336DULL>({0x1F, 0x46, 0x7C, 0xE6, 0xB2, 0x24, 0xD3}))) {
      g_serviceType = 14;
    } else if (serviceType->Text() ==
               std::string(DecryptText<0xC7912587FFF31B71ULL>({0x07, 0x72, 0x96, 0x8B, 0xE9, 0x44, 0xFC, 0xC7}))) {
      g_serviceType = 15;
    } else if (serviceType->Text() == std::string(DecryptText<0x2D3B15774149D5DDULL>({0xBE, 0xBD, 0x20, 0x2D, 0x12, 0x15}))) {
      g_serviceType = 17;
    } else if (serviceType->Text() == std::string(DecryptText<0x9B17C9EFDBB39F2FULL>({0x49, 0xED, 0xD2, 0xB5, 0x8C, 0xAC, 0x17}))) {
      g_serviceType = 18;
    } else if (serviceType->Text() == std::string(DecryptText<0x0BEB63A5C905B3A7ULL>({0xD2, 0xD2, 0x60, 0xC9}))) {
      g_serviceType = 19;
    } else if (serviceType->Text() == std::string(DecryptText<0x8B3FEB5B8BD5C3C3ULL>({0xA6, 0xB6, 0xD5}))) {
      g_serviceType = 20;
    } else {
      ShowErrorDialog(GetRenderSystemHolder()->system,
                      DecryptText<0x8F1B5BFD5F6175B5ULL>({0xE6, 0x10, 0x13, 0x29, 0x94, 0x38, 0x7E, 0xDB, 0xCC, 0x05, 0x04,
                                                          0x7F, 0xB8, 0x29, 0x69, 0xE0, 0xC7, 0x55, 0x40, 0x7E, 0xFD}),
                      0);
    }
  } else if (!g_clientInfoLoaded) {
    ShowErrorDialog(GetRenderSystemHolder()->system,
                    DecryptText<0x078333BB9725B7C9ULL>(
                        {0x87, 0xD8, 0x05, 0xC4, 0xDE, 0x41, 0xF5, 0x6E, 0xAA, 0xD2, 0x71, 0xEE, 0xCB, 0x56, 0xA3, 0x26, 0xE8, 0xB7}),
                    0);
  }

  XmlNode* serverType =
      clientInfo->FindChild(DecryptText<0xF5657D1F3DE5CDC7ULL>({0xB4, 0xA8, 0x97, 0x4B, 0x7A, 0x0F, 0x11, 0x8C, 0xB7, 0xA8, 0xE5}));
  if (serverType) {
    if (serverType->Text() == std::string(DecryptText<0x69FD1D9733D3CFC1ULL>({0xB1, 0xBD, 0xBA, 0x5E, 0xF6, 0x6F, 0x84, 0x69}))) {
      g_serviceMode = g_serviceMode == 13 ? 13 : 0;
    } else if (serverType->Text() == std::string(DecryptText<0xF7530137154D8FE5ULL>({0x96, 0xEE, 0x26, 0x67, 0x56, 0x78, 0x53}))) {
      g_serviceMode = 1;
    } else if (serverType->Text() == std::string(DecryptText<0x8B65CD61012B5931ULL>({0x5D, 0x36, 0x48, 0x60, 0x0D, 0xCD}))) {
      g_serviceMode = 2;
    } else if (serverType->Text() == std::string(DecryptText<0x17359BF953C367B3ULL>({0xC3, 0x0C, 0xC3}))) {
      g_serviceMode = 5;
    } else {
      ShowErrorDialog(GetRenderSystemHolder()->system,
                      DecryptText<0x2FF3C5036D070127ULL>({0x74, 0x64, 0x75, 0x1B, 0x66, 0xB7, 0xA7, 0x56, 0x57, 0x64,
                                                          0x27, 0x28, 0x71, 0xB7, 0x9C, 0x5D, 0x07, 0x20, 0x26, 0x6D}),
                      0);
    }
  } else if (!g_clientInfoLoaded) {
    ShowErrorDialog(
        GetRenderSystemHolder()->system,
        DecryptText<0x93E7A3A1F587CDC5ULL>({0x8B, 0xA2, 0xA7, 0xA6, 0xC4, 0xD1, 0x91, 0xF6, 0xB7, 0x99, 0xFE, 0x85, 0xC4, 0x83, 0xC6, 0xB2, 0xC5}),
        0);
  }

  if (clientInfo->FindChild(DecryptText<0xC52FB5FBB94FF917ULL>(
          {0x7F, 0x90, 0x2B, 0xDC, 0x9A, 0xD6, 0x4C, 0xAA, 0x62, 0x97, 0x3B, 0xD5, 0x92, 0xC6, 0x5B, 0xC5}))) {
    g_hideAccountList = true;
  }

  if (clientInfo->FindChild(DecryptText<0x812BE72BCD5707B7ULL>({0xD2, 0x7F, 0x23, 0xA8, 0x45, 0x83, 0x4E, 0xE5, 0xC4, 0x6B, 0x38, 0xB9, 0x2B}))) {
    g_extendedSlot = true;
  }

  if (clientInfo->FindChild(DecryptText<0x59278F63D79F3DA1ULL>({0xD3, 0x58, 0xFE, 0xB3, 0x05, 0xE0, 0x4B, 0x3D, 0xC4, 0x4F, 0x9F}))) {
    g_readFolder = true;
  }

  XmlNode* loading = clientInfo->FindChild(DecryptText<0x1D6D75EDE7CD311FULL>({0x73, 0x5E, 0xAC, 0x83, 0x84, 0x1B, 0x0A, 0x1D}));
  if (loading) {
    for (XmlNode* image = loading->FindChild(DecryptText<0x3375B1C7DBB5FD55ULL>({0x3C, 0x90, 0xD4, 0xBC, 0xA2, 0xB1})); image;
         image = image->FindSibling(DecryptText<0xD98F03A52D35636DULL>({0x04, 0x0E, 0x54, 0x4A, 0xC0, 0x03}))) {
      char path[256];
      std::sprintf(path,
                   DecryptText<0x0F0F8B6B8F9D19D1ULL>(
                       {0x11, 0xB6, 0x5D, 0x75, 0xAB, 0x45, 0xCA, 0xC2, 0x17, 0xFD, 0x5D, 0x43, 0xD6, 0x31, 0x53, 0x2A, 0xA2, 0x19}),
                   image->Text().c_str());
      g_loadingScreens.push_back(std::string(path));
    }
  }

  XmlNode* homepage =
      clientInfo->FindChild(DecryptText<0x31D7C1EDA345E36DULL>({0x1B, 0x8A, 0x20, 0xD4, 0x85, 0xAE, 0xBA, 0x54, 0x1D, 0x82, 0x22, 0xC6, 0xED}));
  if (homepage) {
    sprintf_s(g_homepageUrl, sizeof(g_homepageUrl), "%s", homepage->Text().c_str());
  }

  g_clientInfoLoaded = true;
}
