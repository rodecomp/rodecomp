#include "Client/Application/Launcher.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Client/Stove/StoveMgr.h"
#include "Contents/AdventureGuide/AdventureGuideMgr.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Monster/ChampionMonsterMgr.h"
#include "Contents/Clan/ClanInfo.h"
#include "Contents/Configuration/ConfigMgr.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Items/EnchantMgr.h"
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Contents/Guild/GuildMgr.h"
#include "Contents/Items/ItemDBToIDMgr.h"
#include "Contents/Items/ItemReformMgr.h"
#include "Contents/Items/NeoPackageItemMgr.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/Items/JobCodeForItem.h"
#include "Contents/Map/MapPropertyMgr.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Repute/ReputeMgr.h"
#include "Contents/Rodex/RodexMailStore.h"
#include "Contents/Events/RouletteMgr.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "Contents/SignBoard/SignBoard.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Map/TownInfoMgr.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Network/Http/CURLMgr.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Security/ProcessSecurity.h"
#include "Security/ReassemblyPacketMgr.h"
#include "Security/ThemidaMgr.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "UI/Party/UIRequestJoinPartyMgr.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Async/NMessageMgr.h"
#include "Utility/Hash/GGHashMgr.h"
#include "Archive/GPakUpdate.h"
#include "Archive/PakRegistry.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/ClientInfo.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Application/UnhandledExceptionHandler.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Client/Application/Timer.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Contents/Skin/SkinMgr.h"
#include "Contents/Sound/SoundConfiguration.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Connection/ConnectionRuntime.h"
#include "Render/Device/DisplayMode.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Audio/BinkRuntime.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Utility/Math/AffineMatrix.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <float.h>
#include <imm.h>
#include <array>
#include <io.h>
#include <new>
#include <objbase.h>
#include <regex>
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <sys/stat.h>
#include <timeapi.h>
#include <utility>
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

// GLOBAL: gl_24398
RECT Rect;

// STATIC: 0x0173ad64
// GLOBAL: gl_24472
std::uint32_t g_clientVersion;

// GLOBAL: gl_24752
HINSTANCE g_instance;

// GLOBAL: gl_24584
char g_applicationDirectory[0x80];

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
  OBFUSCATE_ARRAY_BODY("ADTESTEXE.EXE", 0xD98F03A52D35636DULL, std::uint8_t);
}

// OFFSET: 0x00E55F90
// FUNCTION: fn_24332
EncryptedName<13, 0x73D1674B61DF7B0BULL>& RagexeMProcessName::operator()() const {
  OBFUSCATE_ARRAY_BODY("RAGEXE_M.EXE", 0x73D1674B61DF7B0BULL, std::uint8_t);
}

// OFFSET: 0x00E56060
// FUNCTION: fn_24333
EncryptedName<13, 0x91651D81F559F971ULL>& SakexeMProcessName::operator()() const {
  OBFUSCATE_ARRAY_BODY("SAKEXE_M.EXE", 0x91651D81F559F971ULL, std::uint8_t);
}

// OFFSET: 0x00E56130
// FUNCTION: fn_24334
EncryptedName<13, 0x17359BF953C367B3ULL>& Kernel32LibraryName::operator()() const {
  OBFUSCATE_ARRAY_BODY("kernel32.dll", 0x17359BF953C367B3ULL, std::uint8_t);
}

// OFFSET: 0x00E56200
// FUNCTION: fn_24335
EncryptedName<10, 0x490FFB43A12F0DB9ULL>& PsapiLibraryName::operator()() const {
  OBFUSCATE_ARRAY_BODY("psapi.dll", 0x490FFB43A12F0DB9ULL, std::uint8_t);
}

// OFFSET: 0x00E562D0
// FUNCTION: fn_24336
EncryptedName<25, 0x2FF3C5036D070127ULL>& CreateToolhelp32SnapshotName::operator()() const {
  OBFUSCATE_STRING_BODY("CreateToolhelp32Snapshot", 0x2FF3C5036D070127ULL);
}

// OFFSET: 0x00E563B0
// FUNCTION: fn_24337
EncryptedName<14, 0x3FAF85D995034FD7ULL>& EnumProcessesName::operator()() const {
  OBFUSCATE_ARRAY_BODY("EnumProcesses", 0x3FAF85D995034FD7ULL, std::uint8_t);
}

// OFFSET: 0x00E56480
// FUNCTION: fn_24338
EncryptedName<39, 0x6961FF056999A303ULL>& AdventurerAgencyCreateInstanceName::operator()() const {
  OBFUSCATE_STRING_BODY("CAdventurerAgencyMgr::CreateInstance()", 0x6961FF056999A303ULL);
}

// OFFSET: 0x00E56560
// FUNCTION: fn_24339
EncryptedName<40, 0x9DEFC105A1EB2F71ULL>& RequestJoinPartyCreateInstanceName::operator()() const {
  OBFUSCATE_STRING_BODY("UIRequestJoinPartyMgr::CreateInstance()", 0x9DEFC105A1EB2F71ULL);
}

// OFFSET: 0x00E56640
// FUNCTION: fn_24340
EncryptedName<37, 0x0DF5B33D9F21EB79ULL>& AdventureGuideCreateInstanceName::operator()() const {
  OBFUSCATE_STRING_BODY("CAdventureGuideMgr::CreateInstance()", 0x0DF5B33D9F21EB79ULL);
}

// OFFSET: 0x00E56720
// FUNCTION: fn_24341
EncryptedName<5, 0x01E7898D9D235DDFULL>& GoodResultName::operator()() const {
  OBFUSCATE_ARRAY_BODY("good", 0x01E7898D9D235DDFULL, std::uint8_t);
}

// OFFSET: 0x00E567E0
// FUNCTION: fn_24342
EncryptedName<15, 0x1D6D75EDE7CD311FULL>& Process32FirstName::operator()() const {
  OBFUSCATE_ARRAY_BODY("Process32First", 0x1D6D75EDE7CD311FULL, std::uint8_t);
}

// OFFSET: 0x00E568C0
// FUNCTION: fn_24343
EncryptedName<19, 0x151F77EBEBBB2147ULL>& EnumProcessModulesName::operator()() const {
  OBFUSCATE_STRING_BODY("EnumProcessModules", 0x151F77EBEBBB2147ULL);
}

// OFFSET: 0x00E56990
// FUNCTION: fn_24344
EncryptedName<14, 0x3375B1C7DBB5FD55ULL>& Process32NextName::operator()() const {
  OBFUSCATE_ARRAY_BODY("Process32Next", 0x3375B1C7DBB5FD55ULL, std::uint8_t);
}

// OFFSET: 0x00E56A60
// FUNCTION: fn_24345
EncryptedName<19, 0x2D394DBD0F332733ULL>& GetModuleBaseNameAName::operator()() const {
  OBFUSCATE_STRING_BODY("GetModuleBaseNameA", 0x2D394DBD0F332733ULL);
}

// OFFSET: 0x00E56B30
// FUNCTION: fn_24346
EncryptedName<11, 0x7D993F650D2785FFULL>& RagexeModuleName::operator()() const {
  OBFUSCATE_ARRAY_BODY("RAGEXE.EXE", 0x7D993F650D2785FFULL, std::uint8_t);
}

// OFFSET: 0x00E56C00
// FUNCTION: fn_24347
EncryptedName<11, 0x812BE72BCD5707B7ULL>& RagexeProcessName::operator()() const {
  OBFUSCATE_ARRAY_BODY("RAGEXE.EXE", 0x812BE72BCD5707B7ULL, std::uint8_t);
}

// OFFSET: 0x00E56CD0
// FUNCTION: fn_24348
EncryptedName<11, 0x61FF87ED7BF39BE1ULL>& SakexeModuleName::operator()() const {
  OBFUSCATE_ARRAY_BODY("SAKEXE.EXE", 0x61FF87ED7BF39BE1ULL, std::uint8_t);
}

// OFFSET: 0x00E56DA0
// FUNCTION: fn_24349
EncryptedName<11, 0x59278F63D79F3DA1ULL>& SakexeProcessName::operator()() const {
  OBFUSCATE_ARRAY_BODY("SAKEXE.EXE", 0x59278F63D79F3DA1ULL, std::uint8_t);
}

// OFFSET: 0x00E56E70
// FUNCTION: fn_24350
EncryptedName<13, 0x2D3B15774149D5DDULL>& RagexereModuleName::operator()() const {
  OBFUSCATE_ARRAY_BODY("RAGEXERE.EXE", 0x2D3B15774149D5DDULL, std::uint8_t);
}

// OFFSET: 0x00E56F40
// FUNCTION: fn_24351
EncryptedName<13, 0x31D7C1EDA345E36DULL>& RagexereProcessName::operator()() const {
  OBFUSCATE_ARRAY_BODY("RAGEXERE.EXE", 0x31D7C1EDA345E36DULL, std::uint8_t);
}

// OFFSET: 0x00E57010
// FUNCTION: fn_24352
EncryptedName<15, 0xD3755DD95F47A909ULL>& RagtestexeProcessName::operator()() const {
  OBFUSCATE_ARRAY_BODY("RAGTESTEXE.EXE", 0xD3755DD95F47A909ULL, std::uint8_t);
}

// OFFSET: 0x00E570F0
// FUNCTION: fn_24353
EncryptedName<15, 0x0F0F8B6B8F9D19D1ULL>& SaktestexeProcessName::operator()() const {
  OBFUSCATE_ARRAY_BODY("SAKTESTEXE.EXE", 0x0F0F8B6B8F9D19D1ULL, std::uint8_t);
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
bool RestartFromCommandLine() {
  const std::array markerSource = {'\x2E', '\xDF', '\x58', '\x2E', '\xF5', '\x1C', '\x2F', '\x4E', '\x73', '\xE1', '\x7C', '\x08', '\xCF', '\x3E', '\x15', '\x7F', '\x3B', '\xB3'};
  EncryptedName<sizeof(markerSource), 0x2B477F9B5B39B301ULL>& markerName = OBFUSCATE_DECRYPT_NAME(0x2B477F9B5B39B301ULL, markerSource);
  char* marker = std::strstr(g_commandLine, markerName.Text());
  if (!marker) {
    return false;
  }
  const std::array prefixSource = {'\xB8', '\x1D', '\xC6', '\xC8', '\x0F', '\x40', '\xEB', '\x3E', '\xE5', '\x23', '\xE2', '\xEE', '\x35', '\x62', '\xD1', '\x0F', '\xAD', '\x71'};
  EncryptedName<sizeof(prefixSource), 0x5B832361BDA77197ULL>& prefixName = OBFUSCATE_DECRYPT_NAME(0x5B832361BDA77197ULL, prefixSource);
  char* commandLine = marker + std::strlen(prefixName.Text());
  STARTUPINFOA startup = {};
  startup.cb = sizeof(startup);
  startup.dwFlags = STARTF_USESHOWWINDOW;
  startup.wShowWindow = SW_SHOWNORMAL;
  PROCESS_INFORMATION process;
  CreateProcessA(nullptr, commandLine, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &process);
  CloseHandle(process.hProcess);
  CloseHandle(process.hThread);
  return true;
}

// OFFSET: 0x00E58A60
// FUNCTION: fn_24365
void ApplyAiPatch() {
  if (_access(OBFUSCATE_BYTES_TEXT("AI_patch", 0x3B915119EFD523EFULL), 0) != 0) {
    return;
  }

  if (_access(OBFUSCATE_BYTES_TEXT("AI", 0x5B8F49AF09390D1BULL), 0) != 0) {
    return;
  }

  if (_access(OBFUSCATE_BYTES_TEXT("AI_backup", 0x49C3759B77DBD3C1ULL), 0) == 0) {
    constexpr std::array<std::uint8_t, 10> deleteSource = OBFUSCATE_SOURCE("AI_backup", 0x6DED3DD1CF75AD9BULL);
    char path[16] = {};
    EncryptedName<sizeof(deleteSource), 0x6DED3DD1CF75AD9BULL>& deleteName = OBFUSCATE_DECRYPT_NAME(0x6DED3DD1CF75AD9BULL, deleteSource);
    std::strcpy(path, deleteName.Text());

    SHFILEOPSTRUCTA operation = {};
    operation.hwnd = nullptr;
    operation.wFunc = FO_DELETE;
    operation.pFrom = path;
    operation.pTo = nullptr;
    operation.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
    SHFileOperationA(&operation);
  }

  const char* backup = OBFUSCATE_BYTES_TEXT("AI_backup", 0x1BF915E1F37DFDFDULL);
  std::rename(OBFUSCATE_BYTES_TEXT("AI", 0x01FF9313695DD9DFULL), backup);

  const char* install = OBFUSCATE_BYTES_TEXT("AI", 0x4F1791FB1503C713ULL);
  std::rename(OBFUSCATE_BYTES_TEXT("AI_patch", 0xA1A55B4D1597EDD3ULL), install);
}

// INLINED
void ExitForProtectionCheck(const char* commandLine) {
  if (std::strstr(commandLine, OBFUSCATE_BYTES_TEXT("/check_themida", 0xC7FB6DF18577D34BULL))) {
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



// GLOBAL: gl_25076
char g_launchAccountPassword[24];

// GLOBAL: gl_25077
char g_launchAccountSex;

// GLOBAL: gl_25078
bool g_launchPassCommandLine;

// GLOBAL: gl_25446
char g_launchServerKind;

// GLOBAL: gl_25447
bool g_launchFromPc;

// GLOBAL: gl_25388
bool g_windowDragging;

// GLOBAL: gl_25389
RECT g_windowDragRect;

// GLOBAL: gl_24400
POINT g_windowDragStart;

// OFFSET: 0x00E57E60
// FUNCTION: fn_24361
HANDLE StartClientProcess(char* program) {
  char commandLine[1024];
  std::memset(commandLine, 0, sizeof(commandLine));

  if (g_launchPassCommandLine) {
    StringFormat(commandLine, sizeof(commandLine), static_cast<std::size_t>(-1), "%s %s", program, g_commandLine);
  } else if (g_session.m_launchAccountEnabled) {
    StringFormat(commandLine, sizeof(commandLine), static_cast<std::size_t>(-1), "%s %s %s %c %s", program, g_session.m_loginErrorText,
                 g_launchAccountPassword, g_launchAccountSex, g_launchServerName);
  } else {
    StringFormat(commandLine, sizeof(commandLine), static_cast<std::size_t>(-1), program);
  }

  STARTUPINFOA startup = {};
  startup.cb = sizeof(startup);
  PROCESS_INFORMATION process = {};
  startup.dwFlags = STARTF_USESHOWWINDOW;
  startup.wShowWindow = SW_SHOWNORMAL;

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
    const std::array messageSource = {'\xBA', '\xB0', '\x67', '\xDB', '\x84', '\x2E', '\xC1', '\xFC', '\xF0', '\xA7', '\x2E', '\xDB', '\x82', '\x24', '\x80', '\xF5',
                                  '\xFE', '\xED', '\x67', '\xFE', '\x87', '\x2E', '\x90', '\xE4', '\xFA', '\xE3', '\x22', '\xD0', '\xCB', '\x08', '\x8A', '\xB1',
                                  '\xEF', '\xA2', '\x35', '\xDC', '\xCB', '\x24', '\x8E', '\xFF', '\xEB', '\xAA', '\x29', '\xC8', '\x8A', '\x35', '\xCF', '\x91'};
    EncryptedName<sizeof(messageSource), 0x91E147EBBD47C39FULL>& message = OBFUSCATE_DECRYPT_NAME(0x91E147EBBD47C39FULL, messageSource);
    format = message.Text();
  } else {
    const std::array messageSource = {'\x24', '\x24', '\xB7', '\xB6', '\x60', '\xA5', '\x30', '\xB9', '\x60', '\x39', '\xF0', '\xBA', '\x77', '\xAB', '\x73', '\x81', '\x73', '\x32',
                                  '\xE4', '\xAC', '\x33', '\xEA', '\x38', '\xF1', '\x75', '\x38', '\xB7', '\xAD', '\x76', '\xF6', '\x27', '\xB0', '\x73', '\x23', '\x97'};
    EncryptedName<sizeof(messageSource), 0xD1538513DF975701ULL>& message = OBFUSCATE_DECRYPT_NAME(0xD1538513DF975701ULL, messageSource);
    format = message.Text();
  }

  char text[256];
  std::sprintf(text, format, executable);

  const std::array titleSource = {'\x0C', '\x49', '\xFC', '\x3B', '\x27', '\x43'};
  EncryptedName<sizeof(titleSource), 0x078143534999254DULL>& title = OBFUSCATE_DECRYPT_NAME(0x078143534999254DULL, titleSource);
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

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  virtual ~CEquipmentPropertiesMgr();

  static CEquipmentPropertiesMgr* GetInstance();
  bool InitializeLua();
};
}

// OFFSET: 0x00E58100
// FUNCTION: fn_24362
int InitApp(HINSTANCE instance, int showCommand) {
  g_instance = instance;

  WNDCLASSA windowClass;
  windowClass.style = 0;
  windowClass.lpfnWndProc = WindowProc;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = instance;
  windowClass.hIcon = LoadIconA(instance, MAKEINTRESOURCEA(123));
  windowClass.hCursor = LoadCursorA(nullptr, IDC_ARROW);
  windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
  windowClass.lpszMenuName = nullptr;
  windowClass.lpszClassName = g_windowName;
  RegisterClassA(&windowClass);

  DWORD style = g_configuredFullscreen ? WS_POPUP : WS_CLIPCHILDREN | WS_CAPTION | WS_MINIMIZEBOX;
  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = g_viewportWidth;
  rect.bottom = g_viewportHeight;
  AdjustWindowRect(&rect, style, FALSE);

  int x = 0;
  int y = 0;
  if (!g_configuredFullscreen) {
    GetSystemMetrics(SM_CXSCREEN);
    int posX;
    int posY;
    if (g_configuredOldWidth == g_viewportWidth && g_configuredOldHeight == g_viewportHeight) {
      int defaultX = (GetSystemMetrics(SM_CXSCREEN) - g_viewportWidth) / 2;
      posX = g_session.GetOptionInteger(OBFUSCATE_BYTES_TEXT("Window_XPos", 0x9B17C9EFDBB39F2FULL), defaultX);

      int defaultY = (GetSystemMetrics(SM_CYSCREEN) - g_viewportHeight) / 2;
      posY = g_session.GetOptionInteger(OBFUSCATE_BYTES_TEXT("Window_YPos", 0x69D345DB950F336DULL), defaultY);
    } else {
      posX = (GetSystemMetrics(SM_CXSCREEN) - g_viewportWidth) / 2;
      posY = (GetSystemMetrics(SM_CYSCREEN) - g_viewportHeight) / 2;
    }

    if (posX > GetSystemMetrics(SM_CXSCREEN) - g_viewportWidth) {
      posX = (GetSystemMetrics(SM_CXSCREEN) - g_viewportWidth) / 2;
    }
    x = posX < 0 ? 0 : posX;
    if (posY > GetSystemMetrics(SM_CYSCREEN) - g_viewportHeight) {
      posY = (GetSystemMetrics(SM_CYSCREEN) - g_viewportHeight) / 2;
    }
    if (posY < 0) {
      posY = 0;
    }
    y = posY;
  }

  const __int64 width = rect.right - rect.left;
  const __int64 height = rect.bottom - rect.top;
  int windowWidth = static_cast<int>(width);
  int windowHeight = static_cast<int>(height);
  if (!g_configuredFullscreen) {
    RECT trayRect;
    HWND tray = FindWindowA(OBFUSCATE_BYTES_TEXT("Shell_TrayWnd", 0xC7912587FFF31B71ULL), nullptr);
    if (tray) {
      GetWindowRect(tray, &trayRect);
    }

    const __int64 screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const __int64 screenHeight = GetSystemMetrics(SM_CYSCREEN);
    if (screenWidth <= width && screenHeight <= height) {
      const __int64 trayWidth = trayRect.right - trayRect.left;
      const __int64 trayHeight = trayRect.bottom - trayRect.top;
      if (trayWidth < trayHeight) {
        windowWidth = static_cast<int>(screenWidth - trayWidth);
      } else if (trayHeight < trayWidth) {
        windowHeight = static_cast<int>(screenHeight - trayHeight);
      }
    }
  }

  hWnd = CreateApplicationWindow(0, windowClass.lpszClassName, g_windowName, style, x, y, windowWidth, windowHeight, nullptr, nullptr, instance, nullptr);
  if (!hWnd) {
    const char* caption = OBFUSCATE_BYTES_TEXT("ERROR", 0x078333BB9725B7C9ULL);
    MessageBoxA(nullptr, OBFUSCATE_BYTES_TEXT("Window Creation Failed", 0x8F1B5BFD5F6175B5ULL), caption, MB_OK);
    return 0;
  }

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  GetWindowRect(hWnd, &Rect);
  GetCurrentDirectoryA(sizeof(g_applicationDirectory), g_applicationDirectory);
  return 1;
}


// INLINED
template <typename T>
static void DestroyManager(T*& instance) {
  if (instance == nullptr) {
    instance = new (std::nothrow) T;
  }

  if (instance != nullptr) {
    delete instance;
    instance = nullptr;
  }
}

// OFFSET: 0x00E574A0
// FUNCTION: fn_24358
void ShutdownManagers() {
  DestroyManager(Singleton<CGameSettingsMgr>::s_instance);
  DestroyManager(Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::s_instance);
  DestroyManager(Singleton<NEO_PACKAGEITEM::CPaidPackageItemMgr>::s_instance);

  Singleton<CJobCodeForItem>::GetInstance();
  if (Singleton<CJobCodeForItem>::s_instance != nullptr) {
    delete Singleton<CJobCodeForItem>::s_instance;
    Singleton<CJobCodeForItem>::s_instance = nullptr;
  }

  DestroyManager(Singleton<CUIRegisterMgr>::s_instance);
  DestroyManager(g_skillActionMgr);

  Singleton<CEZ2STREffectMgr>::GetInstance();
  if (Singleton<CEZ2STREffectMgr>::s_instance != nullptr) {
    delete Singleton<CEZ2STREffectMgr>::s_instance;
    Singleton<CEZ2STREffectMgr>::s_instance = nullptr;
  }

  DestroyManager(g_mapSkyMgr);
  DestroyManager(Singleton<CEnchantMgr>::s_instance);
  DestroyManager(g_itemReformMgr);
  DestroyManager(g_itemDbToIdMgr);
  DestroyManager(Singleton<CReputeMgr>::s_instance);

  DestroySignBoardMgr();
  DestroyNavigationLinkParser();
  CEmblemDataMgr::Shutdown();
  CGuildMgr::Destroy();
  DestroyPlayerNameDictionary();

  for (unsigned int attempt = 0; attempt < 30; ++attempt) {
    if (g_adventurerAgencyMgr->pending + g_accountLinkedUserDataMgr->m_pendingLoads == 0) {
      break;
    }

    Sleep(100);
    g_asyncWorkMgr->Update(1000);
  }

  g_asyncWorkMgr->Stop();
  CWhisperBlockMgr::Shutdown();
  CAsyncWorkMgr::Shutdown();
  NMessageMgr::Shutdown();
  CAccountLinkedUserDataMgr::Shutdown();
  CCharacterLinkedUserDataMgr::Shutdown();
  CCURLMgr::Shutdown();
  RouletteMgr::Destroy();
  RodexMailStore::Destroy();
  CMapPropertyMgr::Destroy();
  CChampionMonsterMgr::Destroy();
  CClanInfo::Destroy();
  CGGHashMgr::Destroy();
  CNProtectGameGuardMgr::Destroy();
  CCashEmotionMgr::Shutdown();
  CSimpleStringMgr::Destroy();
  CRenewQuestMgr::Destroy();
  DestroyCooldownOwner();
  CTownInfoMgr::Destroy();
  EquipmentResourceMgr::Destroy();
  CTimeScheduler::Destroy();
  DestroyEntryQueueMgr();
  DestroyEffectMemoryMgr();
  DestroyEffectMgr();
  CReassemblyPacketMgr::DestroyInstance();
  CThemidaMgr::Destroy();

  if (g_launchPassCommandLine && g_stoveMgr != nullptr) {
    delete g_stoveMgr;
    g_stoveMgr = nullptr;
  }

  DestroyManager(Singleton<CSelectPackageItemMgr>::s_instance);

  CConfigMgr::Destroy();
  CAdventurerAgencyMgr::Shutdown();
  UIRequestJoinPartyMgr::Shutdown();

  if (g_equipmentPropertiesMgr != nullptr) {
    delete g_equipmentPropertiesMgr;
    g_equipmentPropertiesMgr = nullptr;
  }

  CAdventureGuideMgr::Destroy();
}

// OFFSET: 0x00E5A330
// FUNCTION: fn_24370
std::pair<bool, std::string> InitializeManagers() {
  if (!CConfigMgr::Create()) {
    const std::array nameSource = {'\xC6', '\x22', '\xE4', '\x79', '\x19', '\x08', '\xC2', '\xE2', '\xE2', '\x13', '\xB1', '\x2D', '\x3C', '\x13', '\xC0', '\xCE',
                               '\xF1', '\x04', '\xC2', '\x79', '\x0C', '\x15', '\xC4', '\xC1', '\xE6', '\x04', '\xA3', '\x3E', '\x7F'};
    EncryptedName<sizeof(nameSource), 0xAFA5617F178B6185ULL>& name = OBFUSCATE_DECRYPT_NAME(0xAFA5617F178B6185ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (std::strstr(g_commandLine, "/stove_sandbox") || std::strstr(g_commandLine, "/stove_live")) {
    g_launchPassCommandLine = true;
    g_stoveMgr = new (std::nothrow) CStoveMgr;
    if (!g_stoveMgr->m_initialized) {
      const std::array nameSource = {'\x70', '\xC0', '\x39', '\xFA', '\xED', '\xF0', '\x16', '\xFC', '\x41', '\xA9', '\x77', '\xD6', '\xE9', '\xF0', '\x3A', '\xEF',
                                 '\x56', '\xDA', '\x23', '\xE6', '\xEF', '\xF4', '\x35', '\xF8', '\x56', '\xBB', '\x64', '\x95'};
      EncryptedName<sizeof(nameSource), 0x9B5B959B954D9333ULL>& name = OBFUSCATE_DECRYPT_NAME(0x9B5B959B954D9333ULL, nameSource);
      return std::pair<bool, std::string>(false, std::string(name.Text()));
    }
  }

  if (!CThemidaMgr::Create()) {
    const std::array nameSource = {'\x22', '\xFB', '\xE7', '\x5C', '\xE6', '\xA0', '\x9B', '\x76', '\x2C', '\xC8', '\xFD', '\x03', '\xB1', '\x8A', '\x8D', '\x72',
                               '\x00', '\xDB', '\xEA', '\x70', '\xE5', '\xBA', '\x8B', '\x76', '\x0F', '\xCC', '\xEA', '\x11', '\xA2', '\xC9'};
    EncryptedName<sizeof(nameSource), 0x17FFC98B398FAF61ULL>& name = OBFUSCATE_DECRYPT_NAME(0x17FFC98B398FAF61ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CReassemblyPacketMgr::CreateInstance()) {
    const std::array nameSource = {'\xFC', '\xEF', '\xBA', '\x5E', '\x3E', '\x5E', '\xE6', '\x5E', '\xDD', '\xD1', '\xA6', '\x6F', '\x2C', '\x4E', '\xE8', '\x56',
                               '\xCB', '\xF0', '\xB8', '\x4D', '\x77', '\x17', '\xC0', '\x41', '\xDA', '\xDC', '\xAB', '\x5A', '\x04', '\x43', '\xF0', '\x47',
                               '\xDE', '\xD3', '\xBC', '\x5A', '\x65', '\x04', '\x83'};
    EncryptedName<sizeof(nameSource), 0x33832D4D3FDFBDBFULL>& name = OBFUSCATE_DECRYPT_NAME(0x33832D4D3FDFBDBFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  Singleton<CEZ2STREffectMgr>::GetInstance()->Load();
  if (!CreateEffectMgr()) {
    const std::array nameSource = {'\xB2', '\x86', '\x8F', '\xDB', '\xC8', '\x6E', '\x01', '\x30', '\x96', '\xB1', '\xD3', '\x87', '\xEE', '\x7F', '\x10', '\x1C',
                               '\x85', '\xA6', '\xA0', '\xD3', '\xDE', '\x79', '\x14', '\x13', '\x92', '\xA6', '\xC1', '\x94', '\xAD'};
    EncryptedName<sizeof(nameSource), 0x7D750DADBDE9C3F1ULL>& name = OBFUSCATE_DECRYPT_NAME(0x7D750DADBDE9C3F1ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateEffectMemoryMgr()) {
    const std::array nameSource = {'\xAA', '\x96', '\x5B', '\x61', '\x50', '\x4A', '\x7F', '\xA8', '\x8C', '\xBE', '\x52', '\x75', '\x4C', '\x64', '\x6C', '\x97',
                               '\xD3', '\xE9', '\x7E', '\x75', '\x50', '\x48', '\x7F', '\x80', '\xA0', '\xBD', '\x4E', '\x73', '\x54', '\x47', '\x68', '\x80',
                               '\xC1', '\xFA', '\x3D'};
    EncryptedName<sizeof(nameSource), 0xE50B2935073DD3E9ULL>& name = OBFUSCATE_DECRYPT_NAME(0xE50B2935073DD3E9ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateEntryQueueMgr()) {
    const std::array nameSource = {'\xEC', '\x0A', '\x45', '\x97', '\xAD', '\x6C', '\x02', '\x4A', '\xCA', '\x3A', '\x4E', '\xAE', '\xB8', '\x67', '\x69', '\x05',
                               '\xEC', '\x3D', '\x4E', '\x82', '\xAB', '\x70', '\x1A', '\x51', '\xDC', '\x3B', '\x4A', '\x8D', '\xBC', '\x70', '\x7B', '\x16',
                               '\xAF'};
    EncryptedName<sizeof(nameSource), 0x3F5315DFE32B4FAFULL>& name = OBFUSCATE_DECRYPT_NAME(0x3F5315DFE32B4FAFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CTimeScheduler::Create()) {
    const std::array nameSource = {'\xD6', '\xA9', '\x78', '\xDE', '\x34', '\x36', '\xF6', '\x59', '\xF0', '\x99', '\x64', '\xDF', '\x34', '\x17', '\xAF', '\x0B',
                               '\xD6', '\x8F', '\x74', '\xD2', '\x25', '\x00', '\xDC', '\x5F', '\xE6', '\x89', '\x70', '\xDD', '\x32', '\x00', '\xBD', '\x18',
                               '\x95'};
    EncryptedName<sizeof(nameSource), 0x31956551B311FD95ULL>& name = OBFUSCATE_DECRYPT_NAME(0x31956551B311FD95ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!EquipmentResourceMgr::Create()) {
    const std::array nameSource = {'\xD2', '\x96', '\xF4', '\x33', '\x00', '\xE2', '\x52', '\x4C', '\xE3', '\xA0', '\xD8', '\x2C', '\x02', '\xE3', '\x07', '\x03',
                               '\xD2', '\xB1', '\xD8', '\x00', '\x11', '\xF4', '\x74', '\x57', '\xE2', '\xB7', '\xDC', '\x0F', '\x06', '\xF4', '\x15', '\x10',
                               '\x91'};
    EncryptedName<sizeof(nameSource), 0x393D916561BDC391ULL>& name = OBFUSCATE_DECRYPT_NAME(0x393D916561BDC391ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  CTownInfoMgr::Create();
  if (!g_townInfoMgr->LoadScript()) {
    const std::array titleSource = {'\x9C', '\x4B', '\x61', '\x16', '\xB9', '\x83'};
    EncryptedName<sizeof(titleSource), 0xFF7D83CB791339D9ULL>& title = OBFUSCATE_DECRYPT_NAME(0xFF7D83CB791339D9ULL, titleSource);
    const char* caption = title.Text();
    const std::array textSource = {'\x41', '\xFC', '\xC4', '\xAF', '\xAE', '\x2D', '\x37', '\x82', '\x35', '\xF5', '\xDA', '\xAD', '\x82', '\x63', '\x18', '\x83',
                               '\x7C', '\xE7', '\xB3'};
    EncryptedName<sizeof(textSource), 0xED5143E7C1B39315ULL>& text = OBFUSCATE_DECRYPT_NAME(0xED5143E7C1B39315ULL, textSource);
    MessageBoxA(nullptr, text.Text(), caption, MB_OK);
  }

  if (!CreateCooldownOwner()) {
    const std::array nameSource = {'\x2C', '\xEE', '\x4E', '\x86', '\x53', '\x73', '\x50', '\xDA', '\x0A', '\xDF', '\x6C', '\x8E', '\x4D', '\x3D', '\x03', '\xF4',
                               '\x1D', '\xC8', '\x40', '\x9D', '\x5A', '\x4E', '\x57', '\xC4', '\x1B', '\xCC', '\x4F', '\x8A', '\x5A', '\x2F', '\x10', '\xB7'};
    EncryptedName<sizeof(nameSource), 0xB739073FE921AD6FULL>& name = OBFUSCATE_DECRYPT_NAME(0xB739073FE921AD6FULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CRenewQuestMgr::Create()) {
    const std::array nameSource = {'\x48', '\xE7', '\xEC', '\x7B', '\x6C', '\xC0', '\xC8', '\xB6', '\x6E', '\xC6', '\xFD', '\x58', '\x6E', '\xC5', '\xA3', '\xF9',
                               '\x48', '\xC7', '\xEC', '\x74', '\x7D', '\xD2', '\xD0', '\xAD', '\x78', '\xC1', '\xE8', '\x7B', '\x6A', '\xD2', '\xB1', '\xEA',
                               '\x0B'};
    EncryptedName<sizeof(nameSource), 0xC399B7091589B50BULL>& name = OBFUSCATE_DECRYPT_NAME(0xC399B7091589B50BULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  g_renewQuestMgr->LoadQuestInfo();
  if (!CClanInfo::Create()) {
    const std::array nameSource = {'\x44', '\xF8', '\x85', '\x2C', '\x97', '\x04', '\x33', '\xF3', '\x68', '\xF6', '\x8E', '\x3F', '\xC3', '\x77', '\x1E', '\xE7',
                               '\x62', '\xDA', '\x9D', '\x28', '\xB0', '\x23', '\x2E', '\xE1', '\x66', '\xD5', '\x8A', '\x28', '\xD1', '\x64', '\x5D'};
    EncryptedName<sizeof(nameSource), 0x955D4DF94DE9BB07ULL>& name = OBFUSCATE_DECRYPT_NAME(0x955D4DF94DE9BB07ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  CChampionMonsterMgr::Create();
  if (!g_championMonsterMgr->Load()) {
    const std::array titleSource = {'\xB0', '\xF5', '\x8F', '\x92', '\x5D', '\x3D'};
    EncryptedName<sizeof(titleSource), 0x936F3D2FFDFD87F5ULL>& title = OBFUSCATE_DECRYPT_NAME(0x936F3D2FFDFD87F5ULL, titleSource);
    const char* caption = title.Text();
    const std::array textSource = {'\x0C', '\x23', '\xD2', '\xE2', '\x5D', '\x7C', '\x2E', '\xE9', '\x02', '\x24', '\xDD', '\xFC', '\x59', '\x70', '\x33', '\xA7',
                               '\x29', '\x22', '\xDF', '\xEA', '\x0D', '\x5C', '\x2F', '\xEE', '\x3B', '\x4B'};
    EncryptedName<sizeof(textSource), 0x8741152D8FB34B4FULL>& text = OBFUSCATE_DECRYPT_NAME(0x8741152D8FB34B4FULL, textSource);
    MessageBoxA(nullptr, text.Text(), caption, MB_OK);
  }

  if (!CMapPropertyMgr::Create()) {
    const std::array nameSource = {'\x9C', '\x0A', '\xCC', '\x17', '\x4F', '\x0B', '\x5C', '\x5D', '\xBA', '\x35', '\xD9', '\x1E', '\x52', '\x1E', '\x41', '\x17',
                               '\xE5', '\x04', '\xDF', '\x02', '\x7E', '\x0D', '\x56', '\x64', '\xB1', '\x34', '\xD9', '\x06', '\x71', '\x1A', '\x56', '\x05',
                               '\xF6', '\x47'};
    EncryptedName<sizeof(nameSource), 0x2D33791F67AD47DFULL>& name = OBFUSCATE_DECRYPT_NAME(0x2D33791F67AD47DFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  InitMapTypeInfoTable();
  if (!RodexMailStore::Create()) {
    const std::array nameSource = {'\xE8', '\x1D', '\xF0', '\xC7', '\xE0', '\x57', '\x42', '\x7E', '\xD8', '\x3B', '\xFA', '\xCE', '\xC8', '\x48', '\x63', '\x3D',
                               '\x91', '\x0C', '\xED', '\xC6', '\xE4', '\x5B', '\x74', '\x4E', '\xC5', '\x3C', '\xEB', '\xC2', '\xEB', '\x4C', '\x74', '\x2F',
                               '\x82', '\x4F'};
    EncryptedName<sizeof(nameSource), 0x07112F85A39F4FABULL>& name = OBFUSCATE_DECRYPT_NAME(0x07112F85A39F4FABULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!RouletteMgr::Create()) {
    const std::array nameSource = {'\x14', '\xA3', '\xDA', '\xBE', '\x6F', '\x9D', '\x86', '\x31', '\x23', '\x94', '\xF8', '\xAC', '\x71', '\xCB', '\xD9', '\x06',
                               '\x25', '\x94', '\xD4', '\xBF', '\x66', '\xB8', '\x8D', '\x36', '\x23', '\x90', '\xDB', '\xA8', '\x66', '\xD9', '\xCA', '\x45'};
    EncryptedName<sizeof(nameSource), 0x45E3F103CBB5F157ULL>& name = OBFUSCATE_DECRYPT_NAME(0x45E3F103CBB5F157ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CCURLMgr::Initialize()) {
    const std::array nameSource = {'\x20', '\x7A', '\x28', '\xF3', '\x15', '\xF8', '\xF8', '\xB1', '\x59', '\x03', '\x3E', '\xD3', '\x3C', '\xD4', '\xEB', '\xA6',
                               '\x2A', '\x57', '\x0E', '\xD5', '\x38', '\xDB', '\xFC', '\xA6', '\x4B', '\x10', '\x7D'};
    EncryptedName<sizeof(nameSource), 0xC39FB559A17D3963ULL>& name = OBFUSCATE_DECRYPT_NAME(0xC39FB559A17D3963ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CAccountLinkedUserDataMgr::Initialize()) {
    const std::array nameSource = {'\xEC', '\xE0', '\xA6', '\x54', '\x78', '\x1C', '\xD9', '\x05', '\xE3', '\xC8', '\xAB', '\x5C', '\x72', '\x0D', '\xE2', '\x02',
                               '\xCA', '\xD3', '\x81', '\x56', '\x63', '\x08', '\xFA', '\x16', '\xDD', '\x9B', '\xFF', '\x74', '\x65', '\x0C', '\xD6', '\x05',
                               '\xCA', '\xE8', '\xAB', '\x44', '\x63', '\x08', '\xD9', '\x12', '\xCA', '\x89', '\xEC', '\x37'};
    EncryptedName<sizeof(nameSource), 0x71B7691737C5A1AFULL>& name = OBFUSCATE_DECRYPT_NAME(0x71B7691737C5A1AFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CCharacterLinkedUserDataMgr::Initialize()) {
    const std::array nameSource = {'\x66', '\xD2', '\x6F', '\xC2', '\x93', '\x60', '\x50', '\x1F', '\x40', '\xE3', '\x4B', '\xCA', '\x8F', '\x6A', '\x56', '\x0F',
                               '\x70', '\xE2', '\x62', '\xD1', '\xA5', '\x60', '\x47', '\x0A', '\x68', '\xF6', '\x75', '\x99', '\xDB', '\x42', '\x41', '\x0E',
                               '\x44', '\xE5', '\x62', '\xEA', '\x8F', '\x72', '\x47', '\x0A', '\x4B', '\xF2', '\x62', '\x8B', '\xC8', '\x01'};
    EncryptedName<sizeof(nameSource), 0x6B3301E1A3079125ULL>& name = OBFUSCATE_DECRYPT_NAME(0x6B3301E1A3079125ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!NMessageMgr::Initialize()) {
    const std::array nameSource = {'\xC7', '\x70', '\x08', '\x96', '\xD4', '\x60', '\x40', '\x4A', '\xC4', '\x5A', '\x1F', '\xDF', '\x9D', '\x42', '\x55', '\x4A',
                               '\xE8', '\x49', '\x08', '\xAC', '\xC9', '\x72', '\x53', '\x4E', '\xE7', '\x5E', '\x08', '\xCD', '\x8E', '\x01'};
    EncryptedName<sizeof(nameSource), 0x2F2701A7E56D3D89ULL>& name = OBFUSCATE_DECRYPT_NAME(0x2F2701A7E56D3D89ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CAsyncWorkMgr::Initialize(2)) {
    const std::array nameSource = {'\xFE', '\xDC', '\x48', '\x9E', '\x63', '\xE0', '\xDE', '\x16', '\xCF', '\xF6', '\x76', '\x80', '\x7F', '\xB9', '\xB3', '\x3A',
                               '\xCF', '\xF8', '\x5A', '\x93', '\x68', '\xCA', '\xE7', '\x0A', '\xC9', '\xFC', '\x55', '\x84', '\x68', '\xAB', '\xA0', '\x79'};
    EncryptedName<sizeof(nameSource), 0x7989830DE73B9DBDULL>& name = OBFUSCATE_DECRYPT_NAME(0x7989830DE73B9DBDULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!g_asyncWorkMgr->Start()) {
    const std::array nameSource = {'\x22', '\x65', '\xD6', '\xB1', '\xE6', '\xB3', '\xCE', '\xD8', '\x28', '\x4A', '\xD6', '\xBC', '\xE9', '\xBE', '\xFA', '\xD2',
                               '\x19', '\x60', '\xE8', '\xA2', '\xF5', '\xF5', '\x84', '\x90', '\x55', '\x58', '\xD1', '\xA4', '\xF5', '\xA9', '\x85', '\x94',
                               '\x6B'};
    EncryptedName<sizeof(nameSource), 0xBDADDD87C5A50B6BULL>& name = OBFUSCATE_DECRYPT_NAME(0xBDADDD87C5A50B6BULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CWhisperBlockMgr::Initialize()) {
    const std::array nameSource = {'\x4E', '\x0C', '\x01', '\xEA', '\xC8', '\x61', '\xF8', '\xFB', '\x4F', '\x37', '\x06', '\xE0', '\xD0', '\x5C', '\xFA', '\xFB',
                               '\x37', '\x61', '\x2A', '\xF1', '\xDE', '\x70', '\xE9', '\xEC', '\x44', '\x35', '\x1A', '\xF7', '\xDA', '\x7F', '\xFE', '\xEC',
                               '\x25', '\x72', '\x69'};
    EncryptedName<sizeof(nameSource), 0x899D11BB83695B0DULL>& name = OBFUSCATE_DECRYPT_NAME(0x899D11BB83695B0DULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreatePlayerNameDictionary()) {
    const std::array nameSource = {'\xB6', '\x75', '\x3F', '\x88', '\xAE', '\x40', '\xF5', '\x65', '\x94', '\x48', '\x36', '\xAD', '\xBE', '\x46', '\xF3', '\x42',
                               '\x9A', '\x4B', '\x32', '\x9B', '\xAE', '\x1F', '\xBD', '\x68', '\x87', '\x40', '\x32', '\x9D', '\xB2', '\x6C', '\xE9', '\x58',
                               '\x81', '\x44', '\x3D', '\x8A', '\xB2', '\x0D', '\xAE', '\x2B'};
    EncryptedName<sizeof(nameSource), 0x2B8725D7E95325F5ULL>& name = OBFUSCATE_DECRYPT_NAME(0x2B8725D7E95325F5ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CGuildMgr::Create()) {
    const std::array nameSource = {'\x92', '\x26', '\x84', '\xE0', '\x03', '\x35', '\xA8', '\xB4', '\xA3', '\x5B', '\xCB', '\xCA', '\x1D', '\x34', '\x84', '\xA7',
                               '\xB4', '\x28', '\x9F', '\xFA', '\x1B', '\x30', '\x8B', '\xB0', '\xB4', '\x49', '\xD8', '\x89'};
    EncryptedName<sizeof(nameSource), 0xD3E5516F89F161D1ULL>& name = OBFUSCATE_DECRYPT_NAME(0xD3E5516F89F161D1ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CEmblemDataMgr::Initialize()) {
    const std::array nameSource = {'\xCE', '\x56', '\x12', '\x71', '\x0D', '\x80', '\x58', '\xB1', '\xEC', '\x67', '\x1E', '\x5E', '\x06', '\x97', '\x0F', '\xCF',
                               '\xCE', '\x61', '\x1A', '\x72', '\x15', '\x80', '\x7C', '\x9B', '\xFE', '\x67', '\x1E', '\x7D', '\x02', '\x80', '\x1D', '\xDC',
                               '\x8D'};
    EncryptedName<sizeof(nameSource), 0xF535E561137F138DULL>& name = OBFUSCATE_DECRYPT_NAME(0xF535E561137F138DULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateNavigationLinkParser()) {
    const std::array nameSource = {'\x9C', '\xCF', '\x5E', '\x9E', '\xC8', '\x7E', '\x35', '\x9D', '\xE5', '\xD8', '\x4D', '\x9C', '\xE4', '\x6D', '\x22', '\xEE',
                               '\xB1', '\xE8', '\x4B', '\x98', '\xEB', '\x7A', '\x22', '\x8F', '\xF6', '\x9B'};
    EncryptedName<sizeof(nameSource), 0xA7471985F93F9BDFULL>& name = OBFUSCATE_DECRYPT_NAME(0xA7471985F93F9BDFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateSignBoardMgr()) {
    const std::array nameSource = {'\x14', '\xBC', '\xB8', '\xD2', '\xC5', '\x3D', '\x92', '\x3E', '\x25', '\x8B', '\x9C', '\xD2', '\xD9', '\x45', '\xC7', '\x1C',
                               '\x25', '\x8A', '\xB0', '\xC1', '\xCE', '\x36', '\x93', '\x2C', '\x23', '\x8E', '\xBF', '\xD6', '\xCE', '\x57', '\xD4', '\x5F'};
    EncryptedName<sizeof(nameSource), 0x5FFD7FABB5D1EF57ULL>& name = OBFUSCATE_DECRYPT_NAME(0x5FFD7FABB5D1EF57ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  InitializeSignBoardManager(s_signBoardMgr);
  if (!CAdventurerAgencyMgr::Initialize()) {
    return std::pair<bool, std::string>(false, std::string(AdventurerAgencyCreateInstanceName()().Text()));
  }

  if (!UIRequestJoinPartyMgr::Initialize()) {
    return std::pair<bool, std::string>(false, std::string(RequestJoinPartyCreateInstanceName()().Text()));
  }

  Singleton<CReputeMgr>::GetInstance()->Load();
  CItemDBToIDMgr::GetInstance()->Load();
  CItemReformMgr::Instance()->Load();
  Singleton<CEnchantMgr>::GetInstance()->Init();
  GetMapSkyMgr()->LoadData();
  Singleton<CSelectPackageItemMgr>::GetInstance()->LoadScript();
  Singleton<CUIRegisterMgr>::GetInstance()->Load();
  Singleton<CJobCodeForItem>::GetInstance()->Init();
  EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->InitializeLua();
  if (!CAdventureGuideMgr::Create()) {
    return std::pair<bool, std::string>(false, std::string(AdventureGuideCreateInstanceName()().Text()));
  }

  Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::GetInstance()->LoadScript();
  Singleton<NEO_PACKAGEITEM::CPaidPackageItemMgr>::GetInstance()->LoadScript();
  return std::pair<bool, std::string>(true, std::string(GoodResultName()().Text()));
}

// OFFSET: 0x00E5D390
// FUNCTION: fn_24380
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR commandLine, int showCommand) {
  ExitForProtectionCheck(commandLine);

  HANDLE mutex;
  if (!AcquireInstanceMutex(&mutex)) {
    return 0;
  }

  g_commandLine = commandLine;
  if (RestartFromCommandLine() == true) {
    return 0;
  }

  {
    myGPack::CGPakUpdate update;
    if (update.Open("data.grf ")) {
      update.Commit(nullptr);
    }

    GetCurrentDirectoryA(sizeof(g_applicationDirectory), g_applicationDirectory);
    RemovePath(FormatOwnedString("%s%s", g_applicationDirectory, "\\skin\\default"), 1, false);
  }

  BinkSetSoundSystem(BinkOpenDirectSound, 0);
  if (!InitUnhandledExceptionHandler(reinterpret_cast<int (*)()>(&IgnoreNull))) {
    const std::array failureSource = {'\xB6', '\xEA', '\xAF', '\x66', '\xCF', '\x3A', '\x50', '\x9B', '\xFB', '\xEA', '\xAF', '\x7C', '\xDF', '\x2E', '\x51', '\x87',
                                          '\xFB', '\xE5', '\xA0', '\x66', '\xC7', '\x4F'};
    EncryptedName<sizeof(failureSource), 0xEB3D4FAB0FC183DBULL>& failure = OBFUSCATE_DECRYPT_NAME(0xEB3D4FAB0FC183DBULL, failureSource);
    ShowErrorDialog(failure.Text(), MB_OK);
    return 0;
  }

  const std::array replaySource = {'\x1B', '\x9A', '\xA5', '\x93', '\xB0', '\x3A', '\xBF'};
  EncryptedName<sizeof(replaySource), 0xFFBF43D1FFD5FF49ULL>& replay = OBFUSCATE_DECRYPT_NAME(0xFFBF43D1FFD5FF49ULL, replaySource);
  g_replayMode = std::strstr(commandLine, replay.Text()) != nullptr;
  Replay::Instance().SetState(1);

  const std::array qmSource = {'\xD6', '\x3C', '\x6E', '\x48', '\x07'};
  EncryptedName<sizeof(qmSource), 0x7DB1050779034DE7ULL>& qm = OBFUSCATE_DECRYPT_NAME(0x7DB1050779034DE7ULL, qmSource);
  if (std::strstr(commandLine, qm.Text())) {
    const char* server;
    const std::array sakSource = {'\x80', '\xE4', '\x42', '\x80', '\x2C', '\x3B'};
    EncryptedName<sizeof(sakSource), 0x5D6B3B1DEB2397B1ULL>& sak = OBFUSCATE_DECRYPT_NAME(0x5D6B3B1DEB2397B1ULL, sakSource);
    if (std::strstr(commandLine, sak.Text())) {
      const std::array sakraySource = {'\x74', '\x94', '\xE4', '\x5E', '\xD6', '\x11', '\x84', '\x06', '\x25'};
      EncryptedName<sizeof(sakraySource), 0x7FE563BD3FB7F925ULL>& sakray = OBFUSCATE_DECRYPT_NAME(0x7FE563BD3FB7F925ULL, sakraySource);
      server = sakray.Text();
    } else {
      const std::array devSource = {'\xCD', '\xBA', '\xAB', '\x5B'};
      EncryptedName<sizeof(devSource), 0x8B3FA35B5BDDDF89ULL>& dev = OBFUSCATE_DECRYPT_NAME(0x8B3FA35B5BDDDF89ULL, devSource);
      server = dev.Text();
    }

    StringFormat(g_launchServerName, sizeof(g_launchServerName), static_cast<std::size_t>(-1), server);
  }

  const std::array rocSource = {'\x8E', '\xF7', '\x48', '\x1E', '\x62', '\x23'};
  EncryptedName<sizeof(rocSource), 0xE3FB23537D2785BFULL>& roc = OBFUSCATE_DECRYPT_NAME(0xE3FB23537D2785BFULL, rocSource);
  g_session.SetOption(18, std::strstr(commandLine, roc.Text()) != nullptr, 0);

  const std::array stoveSandboxSource = {'\xCC', '\x24', '\x61', '\x32', '\x61', '\xE4', '\x84', '\x6C', '\x82', '\x39', '\x71', '\x3F', '\x78', '\xF9', '\xDB'};
  EncryptedName<sizeof(stoveSandboxSource), 0x1FDB81175D1557E3ULL>& stoveSandbox = OBFUSCATE_DECRYPT_NAME(0x1FDB81175D1557E3ULL, stoveSandboxSource);
  if (std::strstr(commandLine, stoveSandbox.Text())) {
    g_launchPassCommandLine = true;
  } else {
    const std::array stoveLiveSource = {'\xF8', '\x34', '\x89', '\x0C', '\x7B', '\x5C', '\x74', '\x1F', '\xBE', '\x31', '\x98', '\x63'};
    EncryptedName<sizeof(stoveLiveSource), 0x732B390D63FD47D7ULL>& stoveLive = OBFUSCATE_DECRYPT_NAME(0x732B390D63FD47D7ULL, stoveLiveSource);
    if (std::strstr(commandLine, stoveLive.Text())) {
      g_launchPassCommandLine = true;
    }
  }

  const std::array manifestSource = {'\xFC', '\x96', '\x2C', '\xAF', '\x90', '\x05', '\xC2', '\xD3', '\xFE', '\x8D', '\x20', '\x92', '\x94', '\x0A', '\xC9', '\xDE',
                                         '\xF9', '\x9C', '\x32', '\xB9', '\xF9'};
  EncryptedName<sizeof(manifestSource), 0xB7A76BF9CD41F99FULL>& manifest = OBFUSCATE_DECRYPT_NAME(0xB7A76BF9CD41F99FULL, manifestSource);
  WIN32_FIND_DATAA manifestData;
  HANDLE manifestFind = FindFirstFileA(manifest.Text(), &manifestData);
  if ((g_launchPassCommandLine && manifestFind == INVALID_HANDLE_VALUE) || (!g_launchPassCommandLine && manifestFind != INVALID_HANDLE_VALUE)) {
    FindClose(manifestFind);
    const std::array captionSource = {'\xE2', '\x5D', '\x16', '\x35', '\xBE', '\x29', '\xC0', '\x03', '\xE3', '\x46', '\x0B', '\x43'};
    EncryptedName<sizeof(captionSource), 0x71A509DB43792991ULL>& caption = OBFUSCATE_DECRYPT_NAME(0x71A509DB43792991ULL, captionSource);
    const char* title = caption.Text();
    const std::array messageSource = {'\x2E', '\x27', '\xE0', '\xD7', '\x9A', '\xCD', '\xAC', '\x71', '\xCB', '\xA0', '\x4F', '\x6D', '\x41', '\x1D', '\xAF', '\x73',
                                          '\xC5', '\xF5', '\xAF', '\x64', '\x14', '\x2A', '\xB1', '\xFD', '\xC0', '\x94', '\x48', '\x41', '\x38', '\x35', '\x49', '\x1C',
                                          '\xAB', '\xEF', '\x33', '\x1E', '\x1B', '\xED'};
    EncryptedName<sizeof(messageSource), 0xDD69EDFFA18F537DULL>& message = OBFUSCATE_DECRYPT_NAME(0xDD69EDFFA18F537DULL, messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message.Text()).c_str(), title, MB_OK);
    return 0;
  }

  FindClose(manifestFind);
  if (!g_launchPassCommandLine) {
    const std::array accountMarkerSource = {'\xA4', '\xAF', '\x59', '\x79'};
    EncryptedName<sizeof(accountMarkerSource), 0xB3CF9BEB7963DB89ULL>& accountMarker = OBFUSCATE_DECRYPT_NAME(0xB3CF9BEB7963DB89ULL, accountMarkerSource);
    if (std::strstr(commandLine, accountMarker.Text()) && !g_replayMode) {
      g_session.m_launchAccountEnabled = true;
      char* marker = std::strstr(commandLine, "-t:");
      char* account = marker + 3;
      char* accountEnd = std::strstr(account, " ");
      if (!accountEnd) {
        accountEnd = account + std::strlen(account);
      }

      int length = accountEnd - account;
      std::strncpy(g_session.m_loginErrorText, account, length > 500 ? 500 : length);
      g_session.m_loginErrorText[length > 499 ? 499 : length] = 0;

      char* password = accountEnd + 1;
      char* passwordEnd = std::strstr(password, " ");
      length = passwordEnd - password;
      std::strncpy(g_launchAccountPassword, password, length > 24 ? 24 : length);
      g_launchAccountPassword[length > 23 ? 23 : length] = 0;

      g_launchAccountSex = passwordEnd[1];
      char* server = passwordEnd + 3;
      length = commandLine + std::strlen(commandLine) - server;
      std::strncpy(g_launchServerName, server, length > 24 ? 24 : length);
      g_launchServerName[length > 23 ? 23 : length] = 0;

      const std::array nonPaySource = {'\x3D', '\x4E', '\x89', '\x17', '\x4A', '\x00', '\xED', '\xEA', '\x14', '\x21'};
      EncryptedName<sizeof(nonPaySource), 0x8BBF792B47E72173ULL>& nonPay = OBFUSCATE_DECRYPT_NAME(0x8BBF792B47E72173ULL, nonPaySource);
      if (std::strstr(g_launchServerName, nonPay.Text())) {
        g_launchServerKind = 2;
      } else {
        const std::array eventSource = {'\x3C', '\x81', '\x70', '\x5B', '\xD9', '\x51'};
        EncryptedName<sizeof(eventSource), 0x9B9D51AD3515F779ULL>& event = OBFUSCATE_DECRYPT_NAME(0x9B9D51AD3515F779ULL, eventSource);
        if (std::strstr(g_launchServerName, event.Text())) {
          g_launchServerKind = 2;
        }
      }
    } else {
      const std::array secondManifestSource = {'\x34', '\x90', '\x80', '\x2D', '\xC6', '\xDB', '\xB2', '\xD1', '\x36', '\x8B', '\x8C', '\x10', '\xC2', '\xD4', '\xB9', '\xDC',
                                                   '\x31', '\x9A', '\x9E', '\x3B', '\xAF'};
      EncryptedName<sizeof(secondManifestSource), 0xB5D7B5AF4FEDFF57ULL>& secondManifest = OBFUSCATE_DECRYPT_NAME(0xB5D7B5AF4FEDFF57ULL, secondManifestSource);
      WIN32_FIND_DATAA secondManifestData;
      HANDLE secondManifestFind = FindFirstFileA(secondManifest.Text(), &secondManifestData);
      if (secondManifestFind != INVALID_HANDLE_VALUE) {
        FindClose(secondManifestFind);
        const std::array secondCaptionSource = {'\xC4', '\x47', '\x4A', '\x7F', '\xDE', '\x15', '\xE0', '\x71', '\xC5', '\x5C', '\x57', '\x09'};
        EncryptedName<sizeof(secondCaptionSource), 0x038535BB092533B7ULL>& secondCaption = OBFUSCATE_DECRYPT_NAME(0x038535BB092533B7ULL, secondCaptionSource);
        const char* title = secondCaption.Text();
        const std::array secondMessageSource = {'\x80', '\x03', '\x88', '\x1F', '\x32', '\xE7', '\xEE', '\x57', '\x65', '\x84', '\x27', '\xA5', '\xE9', '\x37', '\xED', '\x55',
                                                    '\x6B', '\xD1', '\xC7', '\xAC', '\xBC', '\x00', '\xF3', '\xDB', '\x6E', '\xB0', '\x20', '\x89', '\x90', '\x1F', '\x0B', '\x3A',
                                                    '\x05', '\xCB', '\x5B', '\xD6', '\xB3', '\xC7'};
        EncryptedName<sizeof(secondMessageSource), 0xFB2BC75769E777D3ULL>& secondMessage = OBFUSCATE_DECRYPT_NAME(0xFB2BC75769E777D3ULL, secondMessageSource);
        MessageBoxA(hWnd, FormatOwnedString(secondMessage.Text()).c_str(), title, MB_OK);
        return 0;
      }

      g_session.m_launchAccountEnabled = false;
      for (int index = 0; index < 500; ++index) {
        g_session.m_loginErrorText[index] = 0;
      }

      for (int index = 0; index < 24; ++index) {
        g_launchAccountPassword[index] = 0;
      }
    }
  }

  const std::array ragSource = {'\x32', '\x47', '\xF8', '\xD2', '\x82', '\x77'};
  EncryptedName<sizeof(ragSource), 0x6BC977B3B5993503ULL>& rag = OBFUSCATE_DECRYPT_NAME(0x6BC977B3B5993503ULL, ragSource);
  std::strstr(commandLine, rag.Text());

  {
    std::string arguments(commandLine);
    std::smatch match;
    std::regex pattern("-inf:(\\d+)");
    if (std::regex_search(arguments, match, pattern)) {
      g_clientVersion = std::stoi(match[1].str());
    }
  }

  const std::array accountSource = {'\x74', '\xFC', '\x46', '\x4E', '\xE8', '\x58', '\x43', '\x73', '\x61', '\x9D'};
  EncryptedName<sizeof(accountSource), 0x072D2D872D259D5BULL>& account = OBFUSCATE_DECRYPT_NAME(0x072D2D872D259D5BULL, accountSource);
  account.Text();
  g_launchFromPc = std::strstr(commandLine, "pc") != nullptr;

  char protection[64];
  std::memset(protection, 0, sizeof(protection));
  const std::array dataGrfSource = {'\x3B', '\x18', '\xD5', '\x86', '\xDF', '\xAA', '\x2D', '\xE1', '\x5F'};
  EncryptedName<sizeof(dataGrfSource), 0x875FCDF1E7A1795FULL>& dataGrf = OBFUSCATE_DECRYPT_NAME(0x875FCDF1E7A1795FULL, dataGrfSource);
  dataGrf.Text();
  HMODULE kernel = GetModuleHandleA("kernel32.dll");
  if (kernel) {
    typedef DWORD(WINAPI * ReadProfileString)(LPCSTR, LPCSTR, LPCSTR, LPSTR, DWORD, LPCSTR);
    typedef BOOL(WINAPI * WriteProfileString)(LPCSTR, LPCSTR, LPCSTR, LPCSTR);
    auto readString = reinterpret_cast<ReadProfileString>(GetProcAddress(kernel, "GetPrivateProfileStringA"));
    if (readString) {
      auto writeString = reinterpret_cast<WriteProfileString>(GetProcAddress(kernel, "WritePrivateProfileStringA"));
      if (writeString) {
        bool loading = true;
        while (loading) {
          int added = 0;
          char key[2] = {'9', 0};
          do {
            char file[0x74];
            readString("Data", key, key, file, sizeof(file), ".\\DATA.INI");
            if (file[0] != key[0] || file[1] != key[1]) {
              g_pakRegistry.Add(file);
              ++added;
            }

            --key[0];
          } while (key[0] >= '0');

          key[0] = '2';
          key[1] = 0;
          loading = added == 0 && writeString("Data", key, "data.grf", ".\\DATA.INI");
        }
      }
    }
  }
  LoadClientInfo(protection);

  const std::array patchSandboxSource = {'\x5A', '\x16', '\x15', '\xB6', '\x3B', '\x6C', '\x80', '\x0A', '\x14', '\x0B', '\x05', '\xBB', '\x22', '\x71', '\xDF'};
  EncryptedName<sizeof(patchSandboxSource), 0x79DF094DD9616575ULL>& patchSandbox = OBFUSCATE_DECRYPT_NAME(0x79DF094DD9616575ULL, patchSandboxSource);
  if (!std::strstr(g_commandLine, patchSandbox.Text())) {
    const std::array patchLiveSource = {'\x98', '\x12', '\x89', '\xD0', '\x41', '\x48', '\xD0', '\xF1', '\xDE', '\x17', '\x98', '\xBF'};
    EncryptedName<sizeof(patchLiveSource), 0x9D8F2D37BFFD61B7ULL>& patchLive = OBFUSCATE_DECRYPT_NAME(0x9D8F2D37BFFD61B7ULL, patchLiveSource);
    if (!std::strstr(g_commandLine, patchLive.Text()) &&
        ReplaceClientExecutable(const_cast<char*>(g_clientExeName), g_patcherExeName)) {
      return 0;
    }
  }

  g_session.Init();

  const std::array setupSource = {'\x64', '\x98', '\x01', '\x20', '\x45', '\xEF', '\x30', '\x3F', '\x52', '\xFD'};
  EncryptedName<sizeof(setupSource), 0x4755C1355575FD37ULL>& setup = OBFUSCATE_DECRYPT_NAME(0x4755C1355575FD37ULL, setupSource);
  while (ReadSetup() == 0) {
    if (_access(setup.Text(), 0) == 0) {
      WaitForSingleObject(StartClientProcess(const_cast<char*>(setup.Text())), INFINITE);
    }
  }

  const std::array saveDataSource = {'\x74', '\xBC', '\x01', '\x06', '\x07', '\x54', '\xBF', '\x70', '\x07'};
  EncryptedName<sizeof(saveDataSource), 0x11CB35636377DD07ULL>& saveData = OBFUSCATE_DECRYPT_NAME(0x11CB35636377DD07ULL, saveDataSource);
  WIN32_FIND_DATAA saveDataData;
  HANDLE saveDataFind = FindFirstFileA(saveData.Text(), &saveDataData);
  if (saveDataFind == INVALID_HANDLE_VALUE) {
    const std::array saveDataDirectorySource = {'\x54', '\xF2', '\x09', '\x6A', '\x3F', '\x7E', '\x43', '\x5C', '\x27'};
    EncryptedName<sizeof(saveDataDirectorySource), 0x3D371F5B0F7F9327ULL>& saveDataDirectory = OBFUSCATE_DECRYPT_NAME(0x3D371F5B0F7F9327ULL, saveDataDirectorySource);
    CreateDirectoryA(saveDataDirectory.Text(), nullptr);
  } else {
    FindClose(saveDataFind);
  }

  _controlfp(_PC_24, _MCW_PC);

  UINT period = 0;
  TIMECAPS caps;
  if (timeGetDevCaps(&caps, sizeof(caps)) == TIMERR_NOERROR) {
    period = caps.wPeriodMin < 1 ? 1 : caps.wPeriodMin;
    if (caps.wPeriodMax < period) {
      period = caps.wPeriodMax;
    }

    timeBeginPeriod(period);
  }

  ResetTimer();
  CoInitialize(nullptr);

  if (!CSoundMgr::Create()) {
    const std::array audioFailureSource = {'\x68', '\xBA', '\x4D', '\x6E', '\x8C', '\x09', '\xA4', '\x12', '\x5A', '\xBB', '\x4C', '\x6A', '\xC3', '\x60', '\x99', '\x02',
                                               '\x5D', '\xEF', '\x6F', '\x66', '\x8A', '\x45', '\x92', '\x0F', '\x29'};
    EncryptedName<sizeof(audioFailureSource), 0x6BF729E30729CF29ULL>& audioFailure = OBFUSCATE_DECRYPT_NAME(0x6BF729E30729CF29ULL, audioFailureSource);
    if (audioFailure.Encrypted()) {
      audioFailure.Text();
    } else {
      ShowErrorDialog(audioFailure.Text(), MB_OK);
      return 0;
    }
  }

  g_session.InitClientData();

  ResourceMgr& resources = ResourceMgr::Instance();
  const std::array resourceNamesSource = {'\x8F', '\xA4', '\x00', '\x0D', '\x46', '\x3C', '\x78', '\x4B', '\x9C', '\xA3', '\x1F', '\x26', '\x09', '\x25', '\x65', '\x6B',
                                              '\xFD'};
  EncryptedName<sizeof(resourceNamesSource), 0x1F1D51274373C1FDULL>& resourceNames = OBFUSCATE_DECRYPT_NAME(0x1F1D51274373C1FDULL, resourceNamesSource);
  resources.LoadAliases(reinterpret_cast<int>(resourceNames.Text()));

  if (!InitApp(instance, showCommand)) {
    return 1;
  }

  std::pair<bool, std::string> managers = InitializeManagers();
  if (!managers.first) {
    const std::array errorSource = {'\x72', '\x27', '\xFF', '\xE8', '\x91', '\xA1'};
    EncryptedName<sizeof(errorSource), 0x5F93A1C3A7AD7537ULL>& error = OBFUSCATE_DECRYPT_NAME(0x5F93A1C3A7AD7537ULL, errorSource);
    MessageBoxA(nullptr, managers.second.c_str(), error.Text(), MB_OK);
    return 1;
  }

  if (GetRenderSystemHolder()->Initialize(g_configuredRenderBackend, hWnd) < 0) {
    const std::array deviceFailureSource = {'\x14', '\xE2', '\x87', '\xBB', '\xEE', '\xB1', '\x6D', '\x2E', '\x39', '\xEA', '\x9D', '\xF5', '\xE5', '\xF6', '\x29', '\x67',
                                                '\x18', '\xD1', '\xC9', '\xB2', '\xF3', '\xA3', '\x6D', '\x21', '\x3E', '\xEF', '\x8C', '\xF5', '\xE9', '\xA4', '\x3E', '\x67',
                                                '\x27', '\xF1', '\x86', '\xB7', '\xED', '\xA0', '\x20', '\x69', '\x57'};
    EncryptedName<sizeof(deviceFailureSource), 0x474DC581D5E98357ULL>& deviceFailure = OBFUSCATE_DECRYPT_NAME(0x474DC581D5E98357ULL, deviceFailureSource);
    ShowErrorDialog(deviceFailure.Text(), MB_OK);
    return 1;
  }

  g_renderer = GetRenderSystemHolder()->CreateRenderer();
  if (!g_replayMode && !InitializeConnections()) {
    return 1;
  }

  InitTimer(60);
  SetFocus(hWnd);
  if (GetRenderSystemHolder()->IsWindowed()) {
    ShowCursor(FALSE);
  }

  g_nProtectGameGuardMgr->SetHwnd(hWnd);
  g_windowMgr.SetScreenSize(g_renderer->viewport.width, g_renderer->viewport.height);
  g_windowMgr.LoadUIRectInfo();
  g_grannyAnimationLibrary.Initialize();
  const std::array guildFlagSource = {'\xEA', '\xC8', '\x21', '\x16', '\x79', '\xD5', '\x40', '\x6D', '\xEA', '\xC8', '\x27', '\x2F', '\x72', '\xFC', '\x1A', '\x65',
                                          '\xE3', '\xC1', '\x29', '\x12', '\x72', '\xB0', '\x43', '\x56', '\xB6', '\x89', '\x22', '\x01', '\x27', '\x89'};
  EncryptedName<sizeof(guildFlagSource), 0x097389157345A787ULL>& guildFlag = OBFUSCATE_DECRYPT_NAME(0x097389157345A787ULL, guildFlagSource);
  g_grannyAnimationLibrary.Load(guildFlag.Text(), 1, 0);
  g_skinInfo.Refresh();
  CreateTrigonometricTable();

  if (g_serviceType == 4 || g_serviceType == 8 || g_serviceType == 9 || (g_serviceType == 6 && g_serviceMode == 0) || g_serviceType == 12) {
    g_loginStartMode = 1;
  } else if (g_serviceType != 0 && ((g_serviceMode == 0 && g_serviceType != 6) || g_serviceType == 2 || g_serviceMode == 5 || g_serviceType == 3 ||
                                      g_serviceType == 19)) {
    g_loginStartMode = 2;
  } else {
    g_loginStartMode = 0;
  }

  const std::array memoSource = {'\xE4', '\x0E', '\x84', '\xF0', '\x4B'};
  EncryptedName<sizeof(memoSource), 0xC5995F4B9FE96B89ULL>& memo = OBFUSCATE_DECRYPT_NAME(0xC5995F4B9FE96B89ULL, memoSource);
  WIN32_FIND_DATAA memoData;
  HANDLE memoFind = FindFirstFileA(memo.Text(), &memoData);
  if (memoFind == INVALID_HANDLE_VALUE) {
    CreateDirectoryA("memo", nullptr);
  } else {
    FindClose(memoFind);
  }

  g_navigationManager.LoadSettings();

  const std::array replayFolderSource = {'\x49', '\xAC', '\xB1', '\x83', '\xB0', '\x04', '\x0F'};
  EncryptedName<sizeof(replayFolderSource), 0x610F7DD1EFC1C91BULL>& replayFolder = OBFUSCATE_DECRYPT_NAME(0x610F7DD1EFC1C91BULL, replayFolderSource);
  WIN32_FIND_DATAA replayData;
  HANDLE replayFind = FindFirstFileA(replayFolder.Text(), &replayData);
  if (replayFind == INVALID_HANDLE_VALUE) {
    const std::array replayDirectorySource = {'\xEB', '\x48', '\x4D', '\x5F', '\xEC', '\xEA', '\x15'};
    EncryptedName<sizeof(replayDirectorySource), 0xED15938D333D2DB9ULL>& replayDirectory = OBFUSCATE_DECRYPT_NAME(0xED15938D333D2DB9ULL, replayDirectorySource);
    CreateDirectoryA(replayDirectory.Text(), nullptr);
  } else {
    FindClose(replayFind);
  }

  WIN32_FIND_DATAA navigationData;
  HANDLE navigationFind = FindFirstFileA("", &navigationData);
  if (replayFind == INVALID_HANDLE_VALUE) {
    const std::array navigationDirectorySource = {'\x0D', '\x58', '\x2D', '\x02', '\x00', '\x4C', '\x05', '\xA0', '\x2C', '\x57', '\x3F', '\x0A', '\x13', '\x4C', '\x71'};
    EncryptedName<sizeof(navigationDirectorySource), 0xC9712D676B5B3943ULL>& navigationDirectory = OBFUSCATE_DECRYPT_NAME(0xC9712D676B5B3943ULL, navigationDirectorySource);
    CreateDirectoryA(navigationDirectory.Text(), nullptr);
  } else {
    FindClose(navigationFind);
  }

  ReleaseMutex(mutex);
  CloseHandle(mutex);
  SetPriorityClass(GetCurrentProcess(), g_session.m_priorityClass);

  g_modeMgr.Run(0, "login.rsw");

  g_windowMgr.DestroyBackgroundSurface();
  GetRagConnection().Close();
  Cleanup();
  g_windowMgr.Shutdown();
  g_windowMgr.SaveUIRectInfo();
  g_navigationManager.SaveSettings();
  g_session.SaveOptionInfo();
  g_windowMgr.DestroyPersistentWindows();
  CMapInfoMgr::Destroy();
  ResourceMgr::Instance().UnloadAll();
  g_texMgr.Clear();
  SaveDisplayDeviceSettings();
  GetRenderSystemHolder()->DestroySurface(reinterpret_cast<CSurface*>(g_renderer));
  GetRenderSystemHolder()->Close();

  MSG message;
  while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
    if (message.message == WM_KEYDOWN && message.wParam == VK_PROCESSKEY) {
      g_serverLanguage->ProcessKey(ImmGetVirtualKey(message.hwnd));
    }

    TranslateMessage(&message);
    DispatchMessageA(&message);
  }

  if (g_windowDragging == true) {
    POINT cursor;
    GetCursorPos(&cursor);
    const int deltaX = cursor.x - g_windowDragStart.x;
    const int deltaY = cursor.y - g_windowDragStart.y;
    if (deltaX != 0 || deltaY != 0) {
      MoveWindow(hWnd, g_windowDragRect.left + deltaX, g_windowDragRect.top + deltaY, g_windowDragRect.right - g_windowDragRect.left,
                 g_windowDragRect.bottom - g_windowDragRect.top, TRUE);
    }

    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0) {
      g_windowDragging = false;
      if (g_serviceOptions.Get(ServiceOption::ConfineCursor)) {
        ClipApplicationCursor(hWnd);
      }
    }
  }

  CoUninitialize();
  DestroyWindow(hWnd);
  RemoveProtectionTools();
  ShutdownManagers();
  CSoundMgr::Destroy();
  CoUninitialize();
  if (period) {
    timeEndPeriod(period);
  }

  CleanupUnhandledExceptionHandler();

  if (g_restartClient) {
    const std::array devServerSource = {'\x55', '\x86', '\x25', '\x99'};
    EncryptedName<sizeof(devServerSource), 0x4B755DB19953E311ULL>& devServer = OBFUSCATE_DECRYPT_NAME(0x4B755DB19953E311ULL, devServerSource);
    if (!std::strstr(g_launchServerName, devServer.Text())) {
      const char* parameters = LookupMessage(MSI_FRIEND_ALREADY_FRIEND);
      const char* file = LookupMessage(MSI_FRIEND_ALREADY_FRIEND);
      const std::array openSource = {'\x22', '\xEF', '\x90', '\x69', '\xEB'};
      EncryptedName<sizeof(openSource), 0xD7A749EB27D5BF6DULL>& open = OBFUSCATE_DECRYPT_NAME(0xD7A749EB27D5BF6DULL, openSource);
      ShellExecuteA(nullptr, open.Text(), file, parameters, nullptr, SW_SHOWNORMAL);
    } else if (g_serviceMode == 0) {
      const std::array parametersSource = {'\xE5', '\xF5', '\x65', '\x83', '\x89', '\x86', '\x46', '\x9C', '\xE9', '\xEC', '\x65', '\x9C', '\xDC', '\xC5', '\x1A', '\xD3',
                                               '\xEA', '\xEF', '\x7B', '\x9C', '\xCA', '\x87', '\x0A', '\x92', '\xE0', '\xAE', '\x76', '\x92', '\xDE', '\xCC', '\x1A', '\x89',
                                               '\xEC', '\xF3', '\x65', '\x96', '\xC1', '\x86', '\x0D', '\x98', '\xFB', '\xF3', '\x7E', '\xDD', '\xD2', '\xDA', '\x19', '\xC2',
                                               '\xCA', '\xE0', '\x7C', '\x96', '\xF0', '\xC6', '\x0D', '\x98', '\xB0', '\xB5', '\x21', '\xC2', '\x82', '\xA9'};
      EncryptedName<sizeof(parametersSource), 0xFD69A9B3F311818DULL>& parameters = OBFUSCATE_DECRYPT_NAME(0xFD69A9B3F311818DULL, parametersSource);
      const char* arguments = parameters.Text();
      const std::array fileSource = {'\x43', '\x6B', '\x9D', '\xDB', '\xFB', '\xD0', '\x02', '\x1E', '\x4F', '\x72', '\x9D', '\xC4', '\xAE', '\x93', '\x5E', '\x51',
                                         '\x4C', '\x71', '\x83', '\xC4', '\xB8', '\xD1', '\x4E', '\x10', '\x46', '\x30', '\x8E', '\xCA', '\xAC', '\x9A', '\x5E', '\x0B',
                                         '\x4A', '\x6D', '\x9D', '\xCE', '\xB3', '\xD0', '\x49', '\x1A', '\x5D', '\x6D', '\x86', '\x85', '\xA0', '\x8C', '\x5D', '\x40',
                                         '\x6C', '\x7E', '\x84', '\xCE', '\x82', '\x90', '\x49', '\x1A', '\x16', '\x2B', '\xD9', '\x9A', '\xF0', '\xFF'};
      EncryptedName<sizeof(fileSource), 0x7F2DFFC1ABE91F2BULL>& file = OBFUSCATE_DECRYPT_NAME(0x7F2DFFC1ABE91F2BULL, fileSource);
      const char* target = file.Text();
      const std::array openSource = {'\x74', '\xB7', '\x44', '\x55', '\xC1'};
      EncryptedName<sizeof(openSource), 0xBD2BA1C11B01E73BULL>& open = OBFUSCATE_DECRYPT_NAME(0xBD2BA1C11B01E73BULL, openSource);
      ShellExecuteA(nullptr, open.Text(), target, arguments, nullptr, SW_SHOWNORMAL);
    } else {
      const std::array parametersSource = {'\x31', '\x79', '\xBF', '\xDF', '\x47', '\x4C', '\xBA', '\xD0', '\x3D', '\x60', '\xBF', '\xC0', '\x12', '\x0F', '\xE6', '\x9F',
                                               '\x3E', '\x63', '\xA1', '\xC0', '\x04', '\x4D', '\xF6', '\xDE', '\x34', '\x22', '\xAC', '\xCE', '\x10', '\x06', '\xE6', '\xC5',
                                               '\x38', '\x7F', '\xBF', '\xCA', '\x0F', '\x4C', '\xF1', '\xD4', '\x2F', '\x7F', '\xA4', '\x81', '\x1C', '\x10', '\xE5', '\x8E',
                                               '\x1E', '\x6C', '\xA6', '\xCA', '\x3E', '\x0C', '\xF1', '\xD4', '\x64', '\x3B', '\xFB', '\x9E', '\x4C', '\x63'};
      EncryptedName<sizeof(parametersSource), 0xB195637DAFCB0D59ULL>& parameters = OBFUSCATE_DECRYPT_NAME(0xB195637DAFCB0D59ULL, parametersSource);
      const char* arguments = parameters.Text();
      const std::array fileSource = {'\x65', '\x17', '\xF9', '\x79', '\x7F', '\x7C', '\xA0', '\xB8', '\x69', '\x0E', '\xF9', '\x66', '\x2A', '\x3F', '\xFC', '\xF7',
                                         '\x6A', '\x0D', '\xE7', '\x66', '\x3C', '\x7D', '\xEC', '\xB6', '\x60', '\x4C', '\xEA', '\x68', '\x28', '\x36', '\xFC', '\xAD',
                                         '\x6C', '\x11', '\xF9', '\x6C', '\x37', '\x7C', '\xEB', '\xBC', '\x7B', '\x11', '\xE2', '\x27', '\x24', '\x20', '\xFF', '\xE6',
                                         '\x4A', '\x02', '\xE0', '\x6C', '\x06', '\x3C', '\xEB', '\xBC', '\x30', '\x55', '\xBD', '\x38', '\x74', '\x53'};
      EncryptedName<sizeof(fileSource), 0xD98F5345098D630DULL>& file = OBFUSCATE_DECRYPT_NAME(0xD98F5345098D630DULL, fileSource);
      const char* target = file.Text();
      const std::array openSource = {'\xDC', '\x25', '\x9A', '\xA1', '\x4D'};
      EncryptedName<sizeof(openSource), 0x9B0F234DEFDF7593ULL>& open = OBFUSCATE_DECRYPT_NAME(0x9B0F234DEFDF7593ULL, openSource);
      ShellExecuteA(nullptr, open.Text(), target, arguments, nullptr, SW_SHOWNORMAL);
    }
  }

  CThemidaMgr::StopThread();
  return 0;
}
