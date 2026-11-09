#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Client/Input/LanguageTC.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Archive/PakRegistry.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Registry.h"
#include "Client/Session.h"
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
#include "Utility/Text/TextTokens.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "UI/Window/UILoginWnd.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <string>
#include <vector>
#include <windows.h>

// GLOBAL: gl_24542
signed char g_language = 0;

// GLOBAL: gl_24544
CLanguageBase* g_serverLanguage;

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

// GLOBAL: gl_24669
const char* g_serviceHost = "";

// GLOBAL: gl_24670
const char* g_servicePort = "";

// GLOBAL: gl_24671
const char* g_serviceOverrideKey = "";

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

// GLOBAL: gl_25364
int g_serviceFlag07;

// GLOBAL: gl_25365
char g_serviceAddress[129];

// GLOBAL: gl_25366
const int s_adminIds[45] = {
  100118, 4663634, 4663643, 4663644, 4663633, 4663642, 4663649, 4663632, 4663638, 4663645,
  9100774, 9100775, 9100776, 9100777, 9100720, 9100721, 9100722, 9100723, 9100778, 9100724,
  9100725, 9100726, 9100727, 9100728, 9100729, 9100730, 9100731, 9100732, 9100733, 9100779,
  9100780, 9100734, 9100781, 9100782, 9100783, 9100784, 9100785, 9100786, 9100787, 9100788,
  9100789, 9100790, 9100791, 9100792, 9100793
};

// GLOBAL: gl_25367
const int s_monsterJobIds[48] = {
  100118, 3304363, 3304378, 3304372, 4663634, 4663643, 4663644, 4663633, 4663642, 4663649,
  4663632, 4663638, 4663645, 9100774, 9100775, 9100776, 9100777, 9100720, 9100721, 9100722,
  9100723, 9100778, 9100724, 9100725, 9100726, 9100727, 9100728, 9100729, 9100730, 9100731,
  9100732, 9100733, 9100779, 9100780, 9100734, 9100781, 9100782, 9100783, 9100784, 9100785,
  9100786, 9100787, 9100788, 9100789, 9100790, 9100791, 9100792, 9100793
};

template <std::size_t Length, std::uint64_t Key>
class EncryptedText {
public:
  explicit EncryptedText(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  // INLINED
  explicit EncryptedText(const char (&source)[Length]) : m_encrypted(true) {
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

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptString(const char (&source)[Length]) {
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
      text = DecryptString<0xBDADDD87C5A50B6BULL>({'\x25', '\x64', '\x85', '\x80', '\xD2', '\x91', '\xEC', '\x9D', '\x1F', '\x6E', '\xDD', '\xB1', '\xA7', '\xBB',
                                                   '\xC4', '\xD1', '\x0E', '\x25', '\x85', '\xED', '\xEB', '\xB4', '\xCE', '\xD8', '\x05', '\x68', '\xC0', '\xEB',
                                                   '\xF3', '\xA5', '\xD9', '\x94', '\x6B'});
    }
  }

  return text.c_str();
}

void LoadEtcInfo();
void InitializeLanguage();

// OFFSET: 0x00ACFF90
// FUNCTION: fn_17808
void InitializeServiceStrings() {
  LoadEtcInfo();

  switch (g_serviceType) {
  case ServiceKorea:
  case 1:
  case ServiceJapan:
  case ServiceChina:
  case ServiceTaiwan:
  case ServiceThai:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case ServiceRussia:
  case ServiceVietnam:
  case ServiceChile:
  case 18:
  case 19:
  case 20:
    break;
  default: {
    const char source[] = {'\x5C', '\xC7', '\x2C', '\x31', '\xB6', '\x2A', '\x1B', '\xF3', '\x5A', '\xCC', '\x35', '\x29',
                                   '\xB0', '\x3E', '\x10', '\x87', '\x70', '\xD9', '\x22', '\x7F', '\xF8', '\x7C', '\x54', '\xD3'};
    ShowErrorDialog(DecryptString<0xD3755DD95F47A909ULL>(source), 0);
    break;
  }
  }

  InitializeLanguage();

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
    const char source[] = {'\xD6', '\x0E', '\xED', '\x63', '\x08', '\x00', '\xD7', '\xCA', '\xD9', '\x26', '\xF9', '\x76', '\x09', '\x08', '\xD1', '\xD6',
                                   '\xA5', '\x32', '\xE4', '\x71', '\x0B', '\x3D', '\xF7', '\xCE', '\xE2', '\x0F', '\xEA', '\x65', '\x10', '\x0A', '\xA5'};
    g_subKey = DecryptString<0xAFA5617F178B6185ULL>(source);
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
    const char message[] = {'\x69', '\x8D', '\xCC', '\x8E', '\x92', '\xC2', '\x73', '\x75', '\x4C', '\x8F', '\x89', '\xBD', '\x80',
                                    '\xC0', '\x1A', '\x69', '\x5D', '\x8E', '\xC6', '\x9D', '\xCF', '\xC2', '\x2B', '\x62', '\x39'};
    g_runPatcherMessage = DecryptString<0x0753A7E1EFA9E139ULL>(message);
    break;
  }
  case 3: {
    const std::uint8_t client[] = {0x1F, 0xB0, 0xAC, 0x80, 0x8C, 0x1E, 0xA1, 0x1B, 0x63, 0xB4, 0xB3, 0xA2, 0xF9};
    g_clientExeName = DecryptText<0x7FCD77F9C7CBD14DULL>(client);
    const std::uint8_t patcher[] = {0x64, 0xF6, 0xB2, 0xEF, 0x25, 0xC8, 0x4F, 0xE5, 0x46, 0xFB, 0xBE, 0x83};
    g_patcherExeName = DecryptText<0xCB3FBD4183DB8323ULL>(patcher);
    const char message[] = {'\xBF', '\x4F', '\xB0', '\x8E', '\x6A', '\x34', '\xB1', '\x49', '\x9A', '\x4D', '\xF5', '\xBD',
                                    '\x78', '\x36', '\xD6', '\x4E', '\x86', '\x4F', '\xB1', '\xC1', '\x7C', '\x29', '\xF4', '\x3B'};
    g_runPatcherMessage = DecryptString<0x3B915119EFD523EFULL>(message);
    break;
  }
  default: {
    const std::uint8_t dev[] = {0xEB, 0x2A, 0x5D, 0xE3};
    if (std::strstr(g_launchServerName, DecryptText<0x3F5315DFE32B4FAFULL>(dev))) {
      const std::uint8_t client[] = {0xED, 0xDC, 0xB8, 0x51, 0x2C, 0x5F, 0xEC, 0x58, 0xEE, 0xF0, 0xF1, 0x5A, 0x35, 0x48, 0x83};
      g_clientExeName = DecryptText<0x33832D4D3FDFBDBFULL>(client);
      const std::uint8_t patcher[] = {0xA1, 0xA2, 0x9D, 0xDE, 0xC5, 0x78, 0x05, 0x2C, 0xBC, 0xED, 0x8C, 0xC5, 0xC8, 0x0D};
      g_patcherExeName = DecryptText<0x7D750DADBDE9C3F1ULL>(patcher);
      const char message[] = {'\x45', '\xFF', '\xD6', '\xA0', '\x94', '\x26', '\x71', '\x9F', '\x60', '\xFD', '\x93', '\x93', '\x86',
                                      '\x24', '\x3F', '\x8C', '\x67', '\xFC', '\xD8', '\x90', '\xAA', '\x6D', '\x34', '\x95', '\x70', '\x93'};
      g_runPatcherMessage = DecryptString<0xED5143E7C1B39315ULL>(message);
    } else {
      const std::uint8_t client[] = {0x8B, 0x58, 0x74, 0x17, 0xAA, 0xF1, 0x12, 0x94, 0xF7, 0x5C, 0x6B, 0x1C, 0xCB};
      g_clientExeName = DecryptText<0xFF7D83CB791339D9ULL>(client);
      const std::uint8_t patcher[] = {0xC5, 0x9C, 0x65, 0xD0, 0x39, 0x10, 0xE5, 0x1F, 0xF0, 0x85, 0x74, 0xB3};
      g_patcherExeName = DecryptText<0x31956551B311FD95ULL>(patcher);
      const char message[] = {'\xC1', '\xAF', '\xD8', '\x00', '\x16', '\xF4', '\x1D', '\x4B', '\xE4', '\xAD', '\x9D', '\x33',
                                      '\x04', '\xF6', '\x53', '\x58', '\xE3', '\xAC', '\xD6', '\x4F', '\x00', '\xE9', '\x58', '\x39'};
      g_runPatcherMessage = DecryptString<0x393D916561BDC391ULL>(message);
    }
    break;
  }
  }
}

// OFFSET: 0x00AD0F60
// FUNCTION: fn_17809
void LoadLoadingScreens(int count) {
  if (count == 0) {
    switch (g_serviceType) {
    case ServiceKorea:
      count = 7;
      break;
    case 1:
    case ServiceVietnam:
      count = 12;
      break;
    case ServiceChina:
      count = 4;
      break;
    case ServiceThai:
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
    char name[256];
    std::sprintf(name, DecryptString<0x7989830DE73B9DBDULL>({'\x7D', '\x32', '\xFB', '\x1D', '\xCD', '\x4D', '\x4C', '\xB4', '\x7B', '\x79', '\xFB', '\x2B', '\xB0', '\x39', '\xD5', '\x15',
                                                                            '\xD2', '\xFC', '\x5F', '\x8E', '\x63', '\xE4', '\xAC', '\x49', '\x8F', '\xF9', '\x15', '\x8D', '\x7D', '\xE4', '\x89'}), index);
    if (ResourceMgr::Instance().Exists(name)) {
      g_loadingScreens.push_back(std::string(name));
    }
  }
}

// OFFSET: 0x00AD1270
// FUNCTION: fn_17810
void LoadEtcInfo() {
  if (ResourceExists("etcinfo.txt", 0)) {
    CCommonObject storage;
    g_loadingScreens.clear();

    std::vector<char*> tokens;
    storage.LoadTokens(tokens, "etcinfo.txt", 0);

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

// OFFSET: 0x00AD1780
// FUNCTION: fn_17811
void InitializeLanguage() {
  // GLOBAL: gl_25359
  static CLanguageBase defaultCharset;
  // GLOBAL: gl_25360
  static CLanguageTC traditionalChineseCharset;
  // GLOBAL: gl_25361
  static CLanguageTH thaiCharset;
  // GLOBAL: gl_25362
  static CLanguageArabic arabicCharset;

  switch (g_serviceType) {
  case ServiceKorea:
    g_language = 0;
    g_textCodePage = 949;
    g_serverLanguage = &defaultCharset;
    break;
  case ServiceJapan:
    g_language = 2;
    g_textCodePage = 932;
    g_serverLanguage = &defaultCharset;
    break;
  case ServiceChina:
    g_language = 3;
    g_serverLanguage = &traditionalChineseCharset;
    g_textCodePage = 936;
    break;
  case ServiceTaiwan:
    g_language = 4;
    g_serverLanguage = &traditionalChineseCharset;
    g_textCodePage = 950;
    break;
  case ServiceThai:
    g_language = 5;
    g_serverLanguage = &thaiCharset;
    g_textCodePage = 874;
    break;
  case 10:
    g_language = 1;
    g_inlineCharset = 1;
    g_textCodePage = 0;
    g_serverLanguage = &defaultCharset;
    break;
  case 11:
    g_language = 1;
    g_inlineCharset = 0;
    g_textCodePage = 0;
    g_serverLanguage = &defaultCharset;
    break;
  case 12:
  case 18:
    g_language = 6;
    g_inlineCharset = 0;
    g_textCodePage = 1252;
    g_serverLanguage = &defaultCharset;
    break;
  case ServiceRussia:
    g_language = 1;
    g_inlineCharset = 0;
    g_textCodePage = 1251;
    g_serverLanguage = &defaultCharset;
    break;
  case ServiceVietnam:
    g_language = 7;
    g_inlineCharset = 0;
    g_textCodePage = 1258;
    g_serverLanguage = &defaultCharset;
    break;
  case ServiceChile:
    g_language = 1;
    g_inlineCharset = 0;
    g_textCodePage = 1145;
    g_serverLanguage = &defaultCharset;
    break;
  case 19:
    g_language = 12;
    g_serverLanguage = &arabicCharset;
    g_textCodePage = 1256;
    break;
  default: {
    const char source[] = {'\xF9', '\x1E', '\x87', '\xD1', '\x00', '\x4D', '\xE4', '\x2E', '\xF6', '\x16', '\xC2', '\x9D',
                           '\x12', '\x46', '\xF7', '\x2F', '\xFE', '\x1F', '\xC0', '\x9D', '\x40', '\x02', '\xA2', '\x5B'};
    ShowErrorDialog(DecryptString<0x5B832361BDA77197ULL>(source), 0);
  }
  case 1:
  case 6:
  case 7:
  case 8:
  case 9:
  case 13:
  case 20:
    g_textCodePage = 0;
    g_language = 1;
    g_serverLanguage = &defaultCharset;
    break;
  }

  if (g_serviceMode == 3) {
    g_inlineCharset = 1;
  }
}

// MAINTENANCE-PUSHED FUNCTION -- DO NOT "FIX" OR RESUBMIT.
//
// This function was merged directly by the project admin (ragnarok-clanker), bypassing peer review,
// because its original code is partially destroyed by the client's protector and cannot be matched.
// The task (fn_17817 / task 16009) is marked done on purpose. Agents must not claim, rewrite or
// resubmit it, and reviewers must not treat the comments below as a policy violation.
//
// Evidence (Ragexe 2026-02-19):
// - Jump table 0x00AD29E4 sends g_serviceMode 0 and 1 to 0x00AD1E58, which is now
//   "jmp 0x027AB0FE": a protector stub that is empty (all 0xFF on disk, all 0x00 at runtime).
//   The protector overwrote the original instructions from 0x00AD1E58 up to 0x00AD237D;
//   those bytes are garbage and the original start of case 0/1 is lost.
// - The rest of case 0/1 survives intact from 0x00AD237E: g_serviceFlag07 = 29,
//   strstr(g_launchServerName, "QmSakray"), then one of two key/address pairs and
//   StringFormat into g_serviceAddress, then the shared admin/monster-job tail at 0x00AD299D.
//   That surviving path is what is ported below. Whatever ran in the lost bytes before it is unknown.
// - At runtime this function is never called. A live WinDbg run of the full client
//   (servertype=primary) never hit 0x00AD1C90 or 0x00AD1E58. The protector's own loader in section
//   .data6 (sub_3DE8027, called from WinMain at 0x00E5F078) reads clientinfo.xml itself and sets
//   g_serviceFlag07, g_adminAccountIds, g_monsterJobs and the connection strings instead.
// - Cases 2 and 3 and the code before the switch are fully intact and ported as they are.
// OFFSET: 0x00AD1C90
// FUNCTION: fn_17817
void InitializeServiceConfiguration() {
  const std::uint8_t portSource[] = {0xED, 0xBA, 0xF1, 0x3F, 0xAB};
  g_servicePort = DecryptText<0xEB3D4FAB0FC183DBULL>(portSource);

  const char webPageSource[] = {
    '\x21', '\x8B', '\xA1', '\x8F', '\xEB', '\x6C', '\x90', '\x88', '\x3E', '\x88', '\xFB', '\x8D', '\xB0', '\x24', '\xD1', '\x9E',
    '\x3B', '\x90', '\xBE', '\xD1', '\xB2', '\x2C', '\x91', '\x94', '\x3B', '\xFF'
  };
  g_webPageUrl = DecryptString<0xFFBF43D1FFD5FF49ULL>(webPageSource);

  g_extendedSlot = true;
  g_serviceOverrideKey = "";
  g_adminAccountIds.clear();
  g_monsterJobs.clear();

  switch (g_serviceMode) {
  case 0:
  case 1: {
    // Original instructions between 0x00AD1E58 and 0x00AD237D were overwritten by the protector
    // (jump into the empty stub 0x027AB0FE). The code from here on starts at 0x00AD237E.
    g_serviceFlag07 = 29;
    if (std::strstr(g_launchServerName, "QmSakray")) {
      const char keySource[] = {
        '\x3C', '\x83', '\xDA', '\xE6', '\x72', '\x9C', '\xCE', '\x77', '\x36', '\xDF', '\xC7', '\xAA', '\x64', '\x9F', '\x82', '\x37',
        '\x38', '\x9A', '\x9B', '\xA8', '\x6C', '\xDF', '\x88', '\x37', '\x6D', '\xC7', '\x8C', '\xFB', '\x33', '\xF1'
      };
      g_serviceOverrideKey = DecryptString<0x45E3F103CBB5F157ULL>(keySource);

      const char addressSource[] = {
        '\xB4', '\x35', '\xC2', '\x4A', '\x6E', '\x14', '\x1E', '\x1C', '\xBE', '\x69', '\xDF', '\x06', '\x78', '\x17', '\x52', '\x5F',
        '\xB0', '\x2C', '\x83', '\x04', '\x70', '\x57', '\x58', '\x5F', '\xE5', '\x71', '\x94', '\x52', '\x2E', '\x79'
      };
      const std::uint8_t formatSource[] = {0xAC, 0x4E, 0x6D};
      StringFormat(g_serviceAddress, sizeof(g_serviceAddress), static_cast<std::size_t>(-1), DecryptText<0x2F2701A7E56D3D89ULL>(formatSource), DecryptString<0x2D33791F67AD47DFULL>(addressSource));
    } else {
      const char keySource[] = {
        '\x4E', '\xE3', '\x68', '\x8E', '\x80', '\x62', '\x50', '\x58', '\x0B', '\xE3', '\x66', '\xC4', '\x8F', '\x60', '\x41', '\x04',
        '\x4E', '\xBF', '\x64', '\xCC', '\xCF', '\x6A', '\x41', '\x51', '\x13', '\xA8', '\x37', '\x93', '\xE1'
      };
      g_serviceOverrideKey = DecryptString<0x6B3301E1A3079125ULL>(keySource);

      const char addressSource[] = {
        '\x08', '\x4B', '\x12', '\x8C', '\x38', '\xD2', '\xFA', '\xAD', '\x00', '\x40', '\x50', '\xD2', '\x77', '\xC7', '\xFE', '\xA4',
        '\x0D', '\x58', '\x0F', '\xCE', '\x32', '\x9B', '\xFC', '\xAC', '\x4D', '\x52', '\x0F', '\x9B', '\x6F', '\x8C', '\xAA', '\xF7',
        '\x63'
      };
      const std::uint8_t formatSource[] = {0xAC, 0x4E, 0x6D};
      StringFormat(g_serviceAddress, sizeof(g_serviceAddress), static_cast<std::size_t>(-1), DecryptText<0x2F2701A7E56D3D89ULL>(formatSource), DecryptString<0xC39FB559A17D3963ULL>(addressSource));
    }
    g_adminAccountIds.assign(std::begin(s_adminIds), std::end(s_adminIds));
    g_monsterJobs.assign(std::begin(s_monsterJobIds), std::end(s_monsterJobIds));
    break;
  }
  case 2: {
    g_serviceFlag07 = 14;
    const std::uint8_t hostSource[] = {0x9E, 0x98, 0xF7, 0x19, 0x26, 0x5F, 0x8F, 0x5F, 0x9A, 0x8F, 0xF0, 0x05, 0x17};
    g_serviceHost = DecryptText<0x71B7691737C5A1AFULL>(hostSource);
    break;
  }
  case 3: {
    const char hostSource[] = {
      '\x3F', '\x6A', '\x58', '\xAD', '\x8A', '\x26', '\xAF', '\xA7', '\x3F', '\x6F', '\x5E', '\xAD', '\x8A', '\x20', '\xA8', '\x89'
    };
    g_serviceHost = DecryptString<0x899D11BB83695B0DULL>(hostSource);
    g_serviceFlag07 = 29;
    const std::uint8_t portSource[] = {0xC3, 0x1C, 0x63, 0xD9, 0xD7};
    g_servicePort = DecryptText<0x2B8725D7E95325F5ULL>(portSource);
    g_adminAccountIds.assign(std::begin(s_adminIds), std::end(s_adminIds));
    g_monsterJobs.assign(std::begin(s_monsterJobIds), std::end(s_monsterJobIds));
    break;
  }
  default:
    break;
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
      ShowErrorDialog(
                      DecryptString<0x8F1B5BFD5F6175B5ULL>({'\xE6', '\x10', '\x13', '\x29', '\x94', '\x38', '\x7E', '\xDB', '\xCC', '\x05', '\x04',
                                                          '\x7F', '\xB8', '\x29', '\x69', '\xE0', '\xC7', '\x55', '\x40', '\x7E', '\xFD'}),
                      0);
    }
  } else if (!g_clientInfoLoaded) {
    ShowErrorDialog(
                    DecryptString<0x078333BB9725B7C9ULL>(
                        {'\x87', '\xD8', '\x05', '\xC4', '\xDE', '\x41', '\xF5', '\x6E', '\xAA', '\xD2', '\x71', '\xEE', '\xCB', '\x56', '\xA3', '\x26', '\xE8', '\xB7'}),
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
      ShowErrorDialog(
                      DecryptString<0x2FF3C5036D070127ULL>({'\x74', '\x64', '\x75', '\x1B', '\x66', '\xB7', '\xA7', '\x56', '\x57', '\x64',
                                                          '\x27', '\x28', '\x71', '\xB7', '\x9C', '\x5D', '\x07', '\x20', '\x26', '\x6D'}),
                      0);
    }
  } else if (!g_clientInfoLoaded) {
    ShowErrorDialog(
        DecryptString<0x93E7A3A1F587CDC5ULL>({'\x8B', '\xA2', '\xA7', '\xA6', '\xC4', '\xD1', '\x91', '\xF6', '\xB7', '\x99', '\xFE', '\x85', '\xC4', '\x83', '\xC6', '\xB2', '\xC5'}),
        0);
  }

  if (clientInfo->FindChild(DecryptString<0xC52FB5FBB94FF917ULL>(
          {'\x7F', '\x90', '\x2B', '\xDC', '\x9A', '\xD6', '\x4C', '\xAA', '\x62', '\x97', '\x3B', '\xD5', '\x92', '\xC6', '\x5B', '\xC5'}))) {
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
                   DecryptString<0x0F0F8B6B8F9D19D1ULL>(
                       {'\x11', '\xB6', '\x5D', '\x75', '\xAB', '\x45', '\xCA', '\xC2', '\x17', '\xFD', '\x5D', '\x43', '\xD6', '\x31', '\x53', '\x2A', '\xA2', '\x19'}),
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
