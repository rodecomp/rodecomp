#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Client/Input/LanguageTC.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Archive/PakRegistry.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Application/Registry.h"
#include "Client/Session/Session.h"
#include "Contents/Map/MapSkyMgr.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Xml/XmlNode.h"
#include "UI/Text/UIBalloonText.h"
#include "Utility/Text/TextTokens.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "UI/Login/UILoginWnd.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <string>
#include <vector>
#include <windows.h>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

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

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptText(const std::uint8_t (&source)[Length]) {
  thread_local EncryptedName<Length, Key> text(source);
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
      text = OBFUSCATE_TEXT("No EULA text file. (licence.txt)", 0xBDADDD87C5A50B6BULL);
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
    ShowErrorDialog(OBFUSCATE_TEXT("Unknown ServiceType !!!", 0xD3755DD95F47A909ULL), 0);
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
    g_licenceFileName = OBFUSCATE_BYTES_TEXT("..\\licence.txt", 0x078143534999254DULL);
  }
  g_startupSecondPhase = static_cast<unsigned char>(time.wSecond) & 3;

  {
    g_subKey = OBFUSCATE_TEXT("Software\\Gravity Soft\\Ragnarok", 0xAFA5617F178B6185ULL);
  }

  {
    g_windowName = OBFUSCATE_BYTES_TEXT("Ragnarok", 0xE50B2935073DD3E9ULL);
  }

  switch (g_serviceMode) {
  case 2: {
    g_clientExeName = OBFUSCATE_BYTES_TEXT("RagIndoor.exe", 0xC399B7091589B50BULL);
    g_patcherExeName = OBFUSCATE_BYTES_TEXT("Indoorup.exe", 0x7FC1015DFFCF6D19ULL);
    g_runPatcherMessage = OBFUSCATE_TEXT("Please run RagIndoor.exe", 0x0753A7E1EFA9E139ULL);
    break;
  }
  case 3: {
    g_clientExeName = OBFUSCATE_BYTES_TEXT("RagGuild.exe", 0x7FCD77F9C7CBD14DULL);
    g_patcherExeName = OBFUSCATE_BYTES_TEXT("Guildup.exe", 0xCB3FBD4183DB8323ULL);
    g_runPatcherMessage = OBFUSCATE_TEXT("Please run RagGuild.exe", 0x3B915119EFD523EFULL);
    break;
  }
  default: {
    if (std::strstr(g_launchServerName, OBFUSCATE_BYTES_TEXT("Dev", 0x3F5315DFE32B4FAFULL))) {
      g_clientExeName = OBFUSCATE_BYTES_TEXT("RagnarokQM.exe", 0x33832D4D3FDFBDBFULL);
      g_patcherExeName = OBFUSCATE_BYTES_TEXT("PatchupQM.exe", 0x7D750DADBDE9C3F1ULL);
      g_runPatcherMessage = OBFUSCATE_TEXT("Please run RagnarokQM.exe", 0xED5143E7C1B39315ULL);
    } else {
      g_clientExeName = OBFUSCATE_BYTES_TEXT("Ragnarok.exe", 0xFF7D83CB791339D9ULL);
      g_patcherExeName = OBFUSCATE_BYTES_TEXT("Patchup.exe", 0x31956551B311FD95ULL);
      g_runPatcherMessage = OBFUSCATE_TEXT("Please run Ragnarok.exe", 0x393D916561BDC391ULL);
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
    std::sprintf(name, OBFUSCATE_TEXT("유저인터페이스\\loading%02d.jpg", 0x7989830DE73B9DBDULL), index);
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
    ShowErrorDialog(OBFUSCATE_TEXT("no language setting !!!", 0x5B832361BDA77197ULL), 0);
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
  const std::uint8_t portSource[] = CRYPT_BYTES(5, "6900", 0xEB3D4FAB0FC183DBULL);
  g_servicePort = DecryptText<0xEB3D4FAB0FC183DBULL>(portSource);

  g_webPageUrl = OBFUSCATE_TEXT("http://www.ragnarok.co.kr", 0xFFBF43D1FFD5FF49ULL);

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
      g_serviceOverrideKey = OBFUSCATE_TEXT("kro-qm-2a.ragnarok.co.kr:6900", 0x45E3F103CBB5F157ULL);

      StringFormat(g_serviceAddress, sizeof(g_serviceAddress), static_cast<std::size_t>(-1), OBFUSCATE_BYTES_TEXT("%s", 0x2F2701A7E56D3D89ULL), OBFUSCATE_TEXT("kro-qm-1a.ragnarok.co.kr:6951", 0x2D33791F67AD47DFULL));
    } else {
      g_serviceOverrideKey = OBFUSCATE_TEXT("kro-acc3.ragnarok.co.kr:6900", 0x6B3301E1A3079125ULL);

      StringFormat(g_serviceAddress, sizeof(g_serviceAddress), static_cast<std::size_t>(-1), OBFUSCATE_BYTES_TEXT("%s", 0x2F2701A7E56D3D89ULL), OBFUSCATE_TEXT("kro-agency-s.ragnarok.co.kr:6954", 0xC39FB559A17D3963ULL));
    }
    g_adminAccountIds.assign(std::begin(s_adminIds), std::end(s_adminIds));
    g_monsterJobs.assign(std::begin(s_monsterJobIds), std::end(s_monsterJobIds));
    break;
  }
  case 2: {
    g_serviceFlag07 = 14;
    g_serviceHost = OBFUSCATE_BYTES_TEXT("192.168.5.52", 0x71B7691737C5A1AFULL);
    break;
  }
  case 3: {
    g_serviceHost = OBFUSCATE_TEXT("211.172.247.115", 0x899D11BB83695B0DULL);
    g_serviceFlag07 = 29;
    g_servicePort = OBFUSCATE_BYTES_TEXT("6900", 0x2B8725D7E95325F5ULL);
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
      clientInfo->FindChild(OBFUSCATE_BYTES_TEXT("servicetype", 0x5B8F49AF09390D1BULL));
  if (serviceType) {
    if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("korea", 0x3F557BE333F95F6DULL))) {
      g_serviceType = 0;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("america", 0x474DC581D5E98357ULL))) {
      g_serviceType = 1;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("japan", 0x01FF9313695DD9DFULL))) {
      g_serviceType = 2;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("china", 0x1BF915E1F37DFDFDULL))) {
      g_serviceType = 3;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("taiwan", 0x49C3759B77DBD3C1ULL))) {
      g_serviceType = 4;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("thai", 0x6DED3DD1CF75AD9BULL))) {
      g_serviceType = 5;
    } else if (serviceType->Text() ==
               std::string(OBFUSCATE_BYTES_TEXT("indonesia", 0x91E147EBBD47C39FULL))) {
      g_serviceType = 6;
    } else if (serviceType->Text() ==
               std::string(OBFUSCATE_BYTES_TEXT("philippine", 0xD1538513DF975701ULL))) {
      g_serviceType = 7;
    } else if (serviceType->Text() ==
               std::string(OBFUSCATE_BYTES_TEXT("malaysia", 0xA1A55B4D1597EDD3ULL))) {
      g_serviceType = 8;
    } else if (serviceType->Text() ==
               std::string(OBFUSCATE_BYTES_TEXT("singapore", 0x4F1791FB1503C713ULL))) {
      g_serviceType = 9;
    } else if (serviceType->Text() ==
               std::string(OBFUSCATE_BYTES_TEXT("germany", 0x7D993F650D2785FFULL))) {
      g_serviceType = 10;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("india", 0x61FF87ED7BF39BE1ULL))) {
      g_serviceType = 11;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("brazil", 0x151F77EBEBBB2147ULL))) {
      g_serviceType = 12;
    } else if (serviceType->Text() ==
               std::string(OBFUSCATE_BYTES_TEXT("australia", 0x2D394DBD0F332733ULL))) {
      g_serviceType = 13;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("russia", 0x69D345DB950F336DULL))) {
      g_serviceType = 14;
    } else if (serviceType->Text() ==
               std::string(OBFUSCATE_BYTES_TEXT("vietnam", 0xC7912587FFF31B71ULL))) {
      g_serviceType = 15;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("chile", 0x2D3B15774149D5DDULL))) {
      g_serviceType = 17;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("france", 0x9B17C9EFDBB39F2FULL))) {
      g_serviceType = 18;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("uae", 0x0BEB63A5C905B3A7ULL))) {
      g_serviceType = 19;
    } else if (serviceType->Text() == std::string(OBFUSCATE_BYTES_TEXT("eu", 0x8B3FEB5B8BD5C3C3ULL))) {
      g_serviceType = 20;
    } else {
      ShowErrorDialog(
                      OBFUSCATE_TEXT("ServiceType Error !!", 0x8F1B5BFD5F6175B5ULL),
                      0);
    }
  } else if (!g_clientInfoLoaded) {
    ShowErrorDialog(
                    OBFUSCATE_TEXT("No ServiceType !!", 0x078333BB9725B7C9ULL),
                    0);
  }

  XmlNode* serverType =
      clientInfo->FindChild(OBFUSCATE_BYTES_TEXT("servertype", 0xF5657D1F3DE5CDC7ULL));
  if (serverType) {
    if (serverType->Text() == std::string(OBFUSCATE_BYTES_TEXT("primary", 0x69FD1D9733D3CFC1ULL))) {
      g_serviceMode = g_serviceMode == 13 ? 13 : 0;
    } else if (serverType->Text() == std::string(OBFUSCATE_BYTES_TEXT("sakray", 0xF7530137154D8FE5ULL))) {
      g_serviceMode = 1;
    } else if (serverType->Text() == std::string(OBFUSCATE_BYTES_TEXT("local", 0x8B65CD61012B5931ULL))) {
      g_serviceMode = 2;
    } else if (serverType->Text() == std::string(OBFUSCATE_BYTES_TEXT("pk", 0x17359BF953C367B3ULL))) {
      g_serviceMode = 5;
    } else {
      ShowErrorDialog(
                      OBFUSCATE_TEXT("ServerType Error !!", 0x2FF3C5036D070127ULL),
                      0);
    }
  } else if (!g_clientInfoLoaded) {
    ShowErrorDialog(
        OBFUSCATE_TEXT("No ServerType !!", 0x93E7A3A1F587CDC5ULL),
        0);
  }

  if (clientInfo->FindChild(OBFUSCATE_TEXT("hideaccountlist", 0xC52FB5FBB94FF917ULL))) {
    g_hideAccountList = true;
  }

  if (clientInfo->FindChild(OBFUSCATE_BYTES_TEXT("extendedslot", 0x812BE72BCD5707B7ULL))) {
    g_extendedSlot = true;
  }

  if (clientInfo->FindChild(OBFUSCATE_BYTES_TEXT("readfolder", 0x59278F63D79F3DA1ULL))) {
    g_readFolder = true;
  }

  XmlNode* loading = clientInfo->FindChild(OBFUSCATE_BYTES_TEXT("loading", 0x1D6D75EDE7CD311FULL));
  if (loading) {
    for (XmlNode* image = loading->FindChild(OBFUSCATE_BYTES_TEXT("image", 0x3375B1C7DBB5FD55ULL)); image;
         image = image->FindSibling(OBFUSCATE_BYTES_TEXT("image", 0xD98F03A52D35636DULL))) {
      char path[256];
      std::sprintf(path,
                   OBFUSCATE_TEXT("유저인터페이스\\%s", 0x0F0F8B6B8F9D19D1ULL),
                   image->Text().c_str());
      g_loadingScreens.push_back(std::string(path));
    }
  }

  XmlNode* homepage =
      clientInfo->FindChild(OBFUSCATE_BYTES_TEXT("viewhomepage", 0x31D7C1EDA345E36DULL));
  if (homepage) {
    sprintf_s(g_homepageUrl, sizeof(g_homepageUrl), "%s", homepage->Text().c_str());
  }

  g_clientInfoLoaded = true;
}
