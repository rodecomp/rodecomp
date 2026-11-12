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
  const std::array<char, 18> markerSource = OBFUSCATE_SOURCE_ARRAY_CHAR("/launcherRESTART:", 0x2B477F9B5B39B301ULL);
  EncryptedName<sizeof(markerSource), 0x2B477F9B5B39B301ULL>& markerName = OBFUSCATE_DECRYPT_NAME(0x2B477F9B5B39B301ULL, markerSource);
  char* marker = std::strstr(g_commandLine, markerName.Text());
  if (!marker) {
    return false;
  }
  const std::array<char, 18> prefixSource = OBFUSCATE_SOURCE_ARRAY_CHAR("/launcherRESTART:", 0x5B832361BDA77197ULL);
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
    const std::array<char, 48> messageSource = OBFUSCATE_SOURCE_ARRAY_CHAR("%s foi modificada. Clique em Ok para continuar.", 0x91E147EBBD47C39FULL);
    EncryptedName<sizeof(messageSource), 0x91E147EBBD47C39FULL>& message = OBFUSCATE_DECRYPT_NAME(0x91E147EBBD47C39FULL, messageSource);
    format = message.Text();
  } else {
    const std::array<char, 35> messageSource = OBFUSCATE_SOURCE_ARRAY_CHAR("%s is changed. Press ok to restart", 0xD1538513DF975701ULL);
    EncryptedName<sizeof(messageSource), 0xD1538513DF975701ULL>& message = OBFUSCATE_DECRYPT_NAME(0xD1538513DF975701ULL, messageSource);
    format = message.Text();
  }

  char text[256];
  std::sprintf(text, format, executable);

  const std::array<char, 6> titleSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Alert", 0x078143534999254DULL);
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
    const std::array<char, 29> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CConfigMgr::CreateInstance()", 0xAFA5617F178B6185ULL);
    EncryptedName<sizeof(nameSource), 0xAFA5617F178B6185ULL>& name = OBFUSCATE_DECRYPT_NAME(0xAFA5617F178B6185ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (std::strstr(g_commandLine, "/stove_sandbox") || std::strstr(g_commandLine, "/stove_live")) {
    g_launchPassCommandLine = true;
    g_stoveMgr = new (std::nothrow) CStoveMgr;
    if (!g_stoveMgr->m_initialized) {
      const std::array<char, 28> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CStoveMgr::CreateInstance()", 0x9B5B959B954D9333ULL);
      EncryptedName<sizeof(nameSource), 0x9B5B959B954D9333ULL>& name = OBFUSCATE_DECRYPT_NAME(0x9B5B959B954D9333ULL, nameSource);
      return std::pair<bool, std::string>(false, std::string(name.Text()));
    }
  }

  if (!CThemidaMgr::Create()) {
    const std::array<char, 30> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CThemidaMgr::CreateInstance()", 0x17FFC98B398FAF61ULL);
    EncryptedName<sizeof(nameSource), 0x17FFC98B398FAF61ULL>& name = OBFUSCATE_DECRYPT_NAME(0x17FFC98B398FAF61ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CReassemblyPacketMgr::CreateInstance()) {
    const std::array<char, 39> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CReassemblyPacketMgr::CreateInstance()", 0x33832D4D3FDFBDBFULL);
    EncryptedName<sizeof(nameSource), 0x33832D4D3FDFBDBFULL>& name = OBFUSCATE_DECRYPT_NAME(0x33832D4D3FDFBDBFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  Singleton<CEZ2STREffectMgr>::GetInstance()->Load();
  if (!CreateEffectMgr()) {
    const std::array<char, 29> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CEffectMgr::CreateInstance()", 0x7D750DADBDE9C3F1ULL);
    EncryptedName<sizeof(nameSource), 0x7D750DADBDE9C3F1ULL>& name = OBFUSCATE_DECRYPT_NAME(0x7D750DADBDE9C3F1ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateEffectMemoryMgr()) {
    const std::array<char, 35> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CEffectMemoryMgr::CreateInstance()", 0xE50B2935073DD3E9ULL);
    EncryptedName<sizeof(nameSource), 0xE50B2935073DD3E9ULL>& name = OBFUSCATE_DECRYPT_NAME(0xE50B2935073DD3E9ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateEntryQueueMgr()) {
    const std::array<char, 33> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CEntryQueueMgr::CreateInstance()", 0x3F5315DFE32B4FAFULL);
    EncryptedName<sizeof(nameSource), 0x3F5315DFE32B4FAFULL>& name = OBFUSCATE_DECRYPT_NAME(0x3F5315DFE32B4FAFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CTimeScheduler::Create()) {
    const std::array<char, 33> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CTimeScheduler::CreateInstance()", 0x31956551B311FD95ULL);
    EncryptedName<sizeof(nameSource), 0x31956551B311FD95ULL>& name = OBFUSCATE_DECRYPT_NAME(0x31956551B311FD95ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!EquipmentResourceMgr::Create()) {
    const std::array<char, 33> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CUIResourceMgr::CreateInstance()", 0x393D916561BDC391ULL);
    EncryptedName<sizeof(nameSource), 0x393D916561BDC391ULL>& name = OBFUSCATE_DECRYPT_NAME(0x393D916561BDC391ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  CTownInfoMgr::Create();
  if (!g_townInfoMgr->LoadScript()) {
    const std::array<char, 6> titleSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Error", 0xFF7D83CB791339D9ULL);
    EncryptedName<sizeof(titleSource), 0xFF7D83CB791339D9ULL>& title = OBFUSCATE_DECRYPT_NAME(0xFF7D83CB791339D9ULL, titleSource);
    const char* caption = title.Text();
    const std::array<char, 19> textSource = OBFUSCATE_SOURCE_ARRAY_CHAR("TownInfo file Init", 0xED5143E7C1B39315ULL);
    EncryptedName<sizeof(textSource), 0xED5143E7C1B39315ULL>& text = OBFUSCATE_DECRYPT_NAME(0xED5143E7C1B39315ULL, textSource);
    MessageBoxA(nullptr, text.Text(), caption, MB_OK);
  }

  if (!CreateCooldownOwner()) {
    const std::array<char, 32> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CCooltimerMgr::CreateInstance()", 0xB739073FE921AD6FULL);
    EncryptedName<sizeof(nameSource), 0xB739073FE921AD6FULL>& name = OBFUSCATE_DECRYPT_NAME(0xB739073FE921AD6FULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CRenewQuestMgr::Create()) {
    const std::array<char, 33> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CRenewQuestMgr::CreateInstance()", 0xC399B7091589B50BULL);
    EncryptedName<sizeof(nameSource), 0xC399B7091589B50BULL>& name = OBFUSCATE_DECRYPT_NAME(0xC399B7091589B50BULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  g_renewQuestMgr->LoadQuestInfo();
  if (!CClanInfo::Create()) {
    const std::array<char, 31> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CClanInfoMgr::CreateInstance()", 0x955D4DF94DE9BB07ULL);
    EncryptedName<sizeof(nameSource), 0x955D4DF94DE9BB07ULL>& name = OBFUSCATE_DECRYPT_NAME(0x955D4DF94DE9BB07ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  CChampionMonsterMgr::Create();
  if (!g_championMonsterMgr->Load()) {
    const std::array<char, 6> titleSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Error", 0x936F3D2FFDFD87F5ULL);
    EncryptedName<sizeof(titleSource), 0x936F3D2FFDFD87F5ULL>& title = OBFUSCATE_DECRYPT_NAME(0x936F3D2FFDFD87F5ULL, titleSource);
    const char* caption = title.Text();
    const std::array<char, 26> textSource = OBFUSCATE_SOURCE_ARRAY_CHAR("ChampionMonster file Init", 0x8741152D8FB34B4FULL);
    EncryptedName<sizeof(textSource), 0x8741152D8FB34B4FULL>& text = OBFUSCATE_DECRYPT_NAME(0x8741152D8FB34B4FULL, textSource);
    MessageBoxA(nullptr, text.Text(), caption, MB_OK);
  }

  if (!CMapPropertyMgr::Create()) {
    const std::array<char, 34> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CMapPropertyMgr::CreateInstance()", 0x2D33791F67AD47DFULL);
    EncryptedName<sizeof(nameSource), 0x2D33791F67AD47DFULL>& name = OBFUSCATE_DECRYPT_NAME(0x2D33791F67AD47DFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  InitMapTypeInfoTable();
  if (!RodexMailStore::Create()) {
    const std::array<char, 34> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CRodexSystemMgr::CreateInstance()", 0x07112F85A39F4FABULL);
    EncryptedName<sizeof(nameSource), 0x07112F85A39F4FABULL>& name = OBFUSCATE_DECRYPT_NAME(0x07112F85A39F4FABULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!RouletteMgr::Create()) {
    const std::array<char, 32> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CRoulletteMgr::CreateInstance()", 0x45E3F103CBB5F157ULL);
    EncryptedName<sizeof(nameSource), 0x45E3F103CBB5F157ULL>& name = OBFUSCATE_DECRYPT_NAME(0x45E3F103CBB5F157ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CCURLMgr::Initialize()) {
    const std::array<char, 27> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CCURLMgr::CreateInstance()", 0xC39FB559A17D3963ULL);
    EncryptedName<sizeof(nameSource), 0xC39FB559A17D3963ULL>& name = OBFUSCATE_DECRYPT_NAME(0xC39FB559A17D3963ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CAccountLinkedUserDataMgr::Initialize()) {
    const std::array<char, 44> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CAccountLinkedUserDataMgr::CreateInstance()", 0x71B7691737C5A1AFULL);
    EncryptedName<sizeof(nameSource), 0x71B7691737C5A1AFULL>& name = OBFUSCATE_DECRYPT_NAME(0x71B7691737C5A1AFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CCharacterLinkedUserDataMgr::Initialize()) {
    const std::array<char, 46> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CCharacterLinkedUserDataMgr::CreateInstance()", 0x6B3301E1A3079125ULL);
    EncryptedName<sizeof(nameSource), 0x6B3301E1A3079125ULL>& name = OBFUSCATE_DECRYPT_NAME(0x6B3301E1A3079125ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!NMessageMgr::Initialize()) {
    const std::array<char, 30> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("NMessageMgr::CreateInstance()", 0x2F2701A7E56D3D89ULL);
    EncryptedName<sizeof(nameSource), 0x2F2701A7E56D3D89ULL>& name = OBFUSCATE_DECRYPT_NAME(0x2F2701A7E56D3D89ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CAsyncWorkMgr::Initialize(2)) {
    const std::array<char, 32> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CAsyncWorkMgr::CreateInstance()", 0x7989830DE73B9DBDULL);
    EncryptedName<sizeof(nameSource), 0x7989830DE73B9DBDULL>& name = OBFUSCATE_DECRYPT_NAME(0x7989830DE73B9DBDULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!g_asyncWorkMgr->Start()) {
    const std::array<char, 33> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("InstanceCAsyncWorkMgr()->Start()", 0xBDADDD87C5A50B6BULL);
    EncryptedName<sizeof(nameSource), 0xBDADDD87C5A50B6BULL>& name = OBFUSCATE_DECRYPT_NAME(0xBDADDD87C5A50B6BULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CWhisperBlockMgr::Initialize()) {
    const std::array<char, 35> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CWhisperBlockMgr::CreateInstance()", 0x899D11BB83695B0DULL);
    EncryptedName<sizeof(nameSource), 0x899D11BB83695B0DULL>& name = OBFUSCATE_DECRYPT_NAME(0x899D11BB83695B0DULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreatePlayerNameDictionary()) {
    const std::array<char, 40> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CPlayerNameDictionary::CreateInstance()", 0x2B8725D7E95325F5ULL);
    EncryptedName<sizeof(nameSource), 0x2B8725D7E95325F5ULL>& name = OBFUSCATE_DECRYPT_NAME(0x2B8725D7E95325F5ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CGuildMgr::Create()) {
    const std::array<char, 28> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CGuildMgr::CreateInstance()", 0xD3E5516F89F161D1ULL);
    EncryptedName<sizeof(nameSource), 0xD3E5516F89F161D1ULL>& name = OBFUSCATE_DECRYPT_NAME(0xD3E5516F89F161D1ULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CEmblemDataMgr::Initialize()) {
    const std::array<char, 33> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CEmblemDataMgr::CreateInstance()", 0xF535E561137F138DULL);
    EncryptedName<sizeof(nameSource), 0xF535E561137F138DULL>& name = OBFUSCATE_DECRYPT_NAME(0xF535E561137F138DULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateNavigationLinkParser()) {
    const std::array<char, 26> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CTagMgr::CreateInstance()", 0xA7471985F93F9BDFULL);
    EncryptedName<sizeof(nameSource), 0xA7471985F93F9BDFULL>& name = OBFUSCATE_DECRYPT_NAME(0xA7471985F93F9BDFULL, nameSource);
    return std::pair<bool, std::string>(false, std::string(name.Text()));
  }

  if (!CreateSignBoardMgr()) {
    const std::array<char, 32> nameSource = OBFUSCATE_SOURCE_ARRAY_CHAR("CSignBoardMgr::CreateInstance()", 0x5FFD7FABB5D1EF57ULL);
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
    const std::array<char, 22> failureSource = OBFUSCATE_SOURCE_ARRAY_CHAR("minidump install fail", 0xEB3D4FAB0FC183DBULL);
    EncryptedName<sizeof(failureSource), 0xEB3D4FAB0FC183DBULL>& failure = OBFUSCATE_DECRYPT_NAME(0xEB3D4FAB0FC183DBULL, failureSource);
    ShowErrorDialog(failure.Text(), MB_OK);
    return 0;
  }

  const std::array<char, 7> replaySource = OBFUSCATE_SOURCE_ARRAY_CHAR("Replay", 0xFFBF43D1FFD5FF49ULL);
  EncryptedName<sizeof(replaySource), 0xFFBF43D1FFD5FF49ULL>& replay = OBFUSCATE_DECRYPT_NAME(0xFFBF43D1FFD5FF49ULL, replaySource);
  g_replayMode = std::strstr(commandLine, replay.Text()) != nullptr;
  Replay::Instance().SetState(1);

  const std::array<char, 5> qmSource = OBFUSCATE_SOURCE_ARRAY_CHAR("1qm1", 0x7DB1050779034DE7ULL);
  EncryptedName<sizeof(qmSource), 0x7DB1050779034DE7ULL>& qm = OBFUSCATE_DECRYPT_NAME(0x7DB1050779034DE7ULL, qmSource);
  if (std::strstr(commandLine, qm.Text())) {
    const char* server;
    const std::array<char, 6> sakSource = OBFUSCATE_SOURCE_ARRAY_CHAR("1sak1", 0x5D6B3B1DEB2397B1ULL);
    EncryptedName<sizeof(sakSource), 0x5D6B3B1DEB2397B1ULL>& sak = OBFUSCATE_DECRYPT_NAME(0x5D6B3B1DEB2397B1ULL, sakSource);
    if (std::strstr(commandLine, sak.Text())) {
      const std::array<char, 9> sakraySource = OBFUSCATE_SOURCE_ARRAY_CHAR("QmSakray", 0x7FE563BD3FB7F925ULL);
      EncryptedName<sizeof(sakraySource), 0x7FE563BD3FB7F925ULL>& sakray = OBFUSCATE_DECRYPT_NAME(0x7FE563BD3FB7F925ULL, sakraySource);
      server = sakray.Text();
    } else {
      const std::array<char, 4> devSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Dev", 0x8B3FA35B5BDDDF89ULL);
      EncryptedName<sizeof(devSource), 0x8B3FA35B5BDDDF89ULL>& dev = OBFUSCATE_DECRYPT_NAME(0x8B3FA35B5BDDDF89ULL, devSource);
      server = dev.Text();
    }

    StringFormat(g_launchServerName, sizeof(g_launchServerName), static_cast<std::size_t>(-1), server);
  }

  const std::array<char, 6> rocSource = OBFUSCATE_SOURCE_ARRAY_CHAR("1roc1", 0xE3FB23537D2785BFULL);
  EncryptedName<sizeof(rocSource), 0xE3FB23537D2785BFULL>& roc = OBFUSCATE_DECRYPT_NAME(0xE3FB23537D2785BFULL, rocSource);
  g_session.SetOption(18, std::strstr(commandLine, roc.Text()) != nullptr, 0);

  const std::array<char, 15> stoveSandboxSource = OBFUSCATE_SOURCE_ARRAY_CHAR("/stove_sandbox", 0x1FDB81175D1557E3ULL);
  EncryptedName<sizeof(stoveSandboxSource), 0x1FDB81175D1557E3ULL>& stoveSandbox = OBFUSCATE_DECRYPT_NAME(0x1FDB81175D1557E3ULL, stoveSandboxSource);
  if (std::strstr(commandLine, stoveSandbox.Text())) {
    g_launchPassCommandLine = true;
  } else {
    const std::array<char, 12> stoveLiveSource = OBFUSCATE_SOURCE_ARRAY_CHAR("/stove_live", 0x732B390D63FD47D7ULL);
    EncryptedName<sizeof(stoveLiveSource), 0x732B390D63FD47D7ULL>& stoveLive = OBFUSCATE_DECRYPT_NAME(0x732B390D63FD47D7ULL, stoveLiveSource);
    if (std::strstr(commandLine, stoveLive.Text())) {
      g_launchPassCommandLine = true;
    }
  }

  const std::array<char, 21> manifestSource = OBFUSCATE_SOURCE_ARRAY_CHAR("combinedata_manifest", 0xB7A76BF9CD41F99FULL);
  EncryptedName<sizeof(manifestSource), 0xB7A76BF9CD41F99FULL>& manifest = OBFUSCATE_DECRYPT_NAME(0xB7A76BF9CD41F99FULL, manifestSource);
  WIN32_FIND_DATAA manifestData;
  HANDLE manifestFind = FindFirstFileA(manifest.Text(), &manifestData);
  if ((g_launchPassCommandLine && manifestFind == INVALID_HANDLE_VALUE) || (!g_launchPassCommandLine && manifestFind != INVALID_HANDLE_VALUE)) {
    FindClose(manifestFind);
    const std::array<char, 12> captionSource = OBFUSCATE_SOURCE_ARRAY_CHAR("stove error", 0x71A509DB43792991ULL);
    EncryptedName<sizeof(captionSource), 0x71A509DB43792991ULL>& caption = OBFUSCATE_DECRYPT_NAME(0x71A509DB43792991ULL, captionSource);
    const char* title = caption.Text();
    const std::array<char, 38> messageSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Stove 클라이언트를 통해 실행해 주세요", 0xDD69EDFFA18F537DULL);
    EncryptedName<sizeof(messageSource), 0xDD69EDFFA18F537DULL>& message = OBFUSCATE_DECRYPT_NAME(0xDD69EDFFA18F537DULL, messageSource);
    MessageBoxA(hWnd, FormatOwnedString(message.Text()).c_str(), title, MB_OK);
    return 0;
  }

  FindClose(manifestFind);
  if (!g_launchPassCommandLine) {
    const std::array<char, 4> accountMarkerSource = OBFUSCATE_SOURCE_ARRAY_CHAR("-t:", 0xB3CF9BEB7963DB89ULL);
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

      const std::array<char, 10> nonPaySource = OBFUSCATE_SOURCE_ARRAY_CHAR("NonPayRag", 0x8BBF792B47E72173ULL);
      EncryptedName<sizeof(nonPaySource), 0x8BBF792B47E72173ULL>& nonPay = OBFUSCATE_DECRYPT_NAME(0x8BBF792B47E72173ULL, nonPaySource);
      if (std::strstr(g_launchServerName, nonPay.Text())) {
        g_launchServerKind = 2;
      } else {
        const std::array<char, 6> eventSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Event", 0x9B9D51AD3515F779ULL);
        EncryptedName<sizeof(eventSource), 0x9B9D51AD3515F779ULL>& event = OBFUSCATE_DECRYPT_NAME(0x9B9D51AD3515F779ULL, eventSource);
        if (std::strstr(g_launchServerName, event.Text())) {
          g_launchServerKind = 2;
        }
      }
    } else {
      const std::array<char, 21> secondManifestSource = OBFUSCATE_SOURCE_ARRAY_CHAR("combinedata_manifest", 0xB5D7B5AF4FEDFF57ULL);
      EncryptedName<sizeof(secondManifestSource), 0xB5D7B5AF4FEDFF57ULL>& secondManifest = OBFUSCATE_DECRYPT_NAME(0xB5D7B5AF4FEDFF57ULL, secondManifestSource);
      WIN32_FIND_DATAA secondManifestData;
      HANDLE secondManifestFind = FindFirstFileA(secondManifest.Text(), &secondManifestData);
      if (secondManifestFind != INVALID_HANDLE_VALUE) {
        FindClose(secondManifestFind);
        const std::array<char, 12> secondCaptionSource = OBFUSCATE_SOURCE_ARRAY_CHAR("stove error", 0x038535BB092533B7ULL);
        EncryptedName<sizeof(secondCaptionSource), 0x038535BB092533B7ULL>& secondCaption = OBFUSCATE_DECRYPT_NAME(0x038535BB092533B7ULL, secondCaptionSource);
        const char* title = secondCaption.Text();
        const std::array<char, 38> secondMessageSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Stove 클라이언트를 통해 실행해 주세요", 0xFB2BC75769E777D3ULL);
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

  const std::array<char, 6> ragSource = OBFUSCATE_SOURCE_ARRAY_CHAR("1rag1", 0x6BC977B3B5993503ULL);
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

  const std::array<char, 10> accountSource = OBFUSCATE_SOURCE_ARRAY_CHAR("/account:", 0x072D2D872D259D5BULL);
  EncryptedName<sizeof(accountSource), 0x072D2D872D259D5BULL>& account = OBFUSCATE_DECRYPT_NAME(0x072D2D872D259D5BULL, accountSource);
  account.Text();
  g_launchFromPc = std::strstr(commandLine, "pc") != nullptr;

  char protection[64];
  std::memset(protection, 0, sizeof(protection));
  const std::array<char, 9> dataGrfSource = OBFUSCATE_SOURCE_ARRAY_CHAR("data.grf", 0x875FCDF1E7A1795FULL);
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

  const std::array<char, 15> patchSandboxSource = OBFUSCATE_SOURCE_ARRAY_CHAR("/stove_sandbox", 0x79DF094DD9616575ULL);
  EncryptedName<sizeof(patchSandboxSource), 0x79DF094DD9616575ULL>& patchSandbox = OBFUSCATE_DECRYPT_NAME(0x79DF094DD9616575ULL, patchSandboxSource);
  if (!std::strstr(g_commandLine, patchSandbox.Text())) {
    const std::array<char, 12> patchLiveSource = OBFUSCATE_SOURCE_ARRAY_CHAR("/stove_live", 0x9D8F2D37BFFD61B7ULL);
    EncryptedName<sizeof(patchLiveSource), 0x9D8F2D37BFFD61B7ULL>& patchLive = OBFUSCATE_DECRYPT_NAME(0x9D8F2D37BFFD61B7ULL, patchLiveSource);
    if (!std::strstr(g_commandLine, patchLive.Text()) &&
        ReplaceClientExecutable(const_cast<char*>(g_clientExeName), g_patcherExeName)) {
      return 0;
    }
  }

  g_session.Init();

  const std::array<char, 10> setupSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Setup.exe", 0x4755C1355575FD37ULL);
  EncryptedName<sizeof(setupSource), 0x4755C1355575FD37ULL>& setup = OBFUSCATE_DECRYPT_NAME(0x4755C1355575FD37ULL, setupSource);
  while (ReadSetup() == 0) {
    if (_access(setup.Text(), 0) == 0) {
      WaitForSingleObject(StartClientProcess(const_cast<char*>(setup.Text())), INFINITE);
    }
  }

  const std::array<char, 9> saveDataSource = OBFUSCATE_SOURCE_ARRAY_CHAR("savedata", 0x11CB35636377DD07ULL);
  EncryptedName<sizeof(saveDataSource), 0x11CB35636377DD07ULL>& saveData = OBFUSCATE_DECRYPT_NAME(0x11CB35636377DD07ULL, saveDataSource);
  WIN32_FIND_DATAA saveDataData;
  HANDLE saveDataFind = FindFirstFileA(saveData.Text(), &saveDataData);
  if (saveDataFind == INVALID_HANDLE_VALUE) {
    const std::array<char, 9> saveDataDirectorySource = OBFUSCATE_SOURCE_ARRAY_CHAR("savedata", 0x3D371F5B0F7F9327ULL);
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
    const std::array<char, 25> audioFailureSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Audio System Init Failed", 0x6BF729E30729CF29ULL);
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
  const std::array<char, 17> resourceNamesSource = OBFUSCATE_SOURCE_ARRAY_CHAR("resNameTable.txt", 0x1F1D51274373C1FDULL);
  EncryptedName<sizeof(resourceNamesSource), 0x1F1D51274373C1FDULL>& resourceNames = OBFUSCATE_DECRYPT_NAME(0x1F1D51274373C1FDULL, resourceNamesSource);
  resources.LoadAliases(reinterpret_cast<int>(resourceNames.Text()));

  if (!InitApp(instance, showCommand)) {
    return 1;
  }

  std::pair<bool, std::string> managers = InitializeManagers();
  if (!managers.first) {
    const std::array<char, 6> errorSource = OBFUSCATE_SOURCE_ARRAY_CHAR("ERROR", 0x5F93A1C3A7AD7537ULL);
    EncryptedName<sizeof(errorSource), 0x5F93A1C3A7AD7537ULL>& error = OBFUSCATE_DECRYPT_NAME(0x5F93A1C3A7AD7537ULL, errorSource);
    MessageBoxA(nullptr, managers.second.c_str(), error.Text(), MB_OK);
    return 1;
  }

  if (GetRenderSystemHolder()->Initialize(g_configuredRenderBackend, hWnd) < 0) {
    const std::array<char, 41> deviceFailureSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Cannot init d3d OR grf file has problem.", 0x474DC581D5E98357ULL);
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
  const std::array<char, 30> guildFlagSource = OBFUSCATE_SOURCE_ARRAY_CHAR("model\\3dmob\\guildflag90_1.gr2", 0x097389157345A787ULL);
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

  const std::array<char, 5> memoSource = OBFUSCATE_SOURCE_ARRAY_CHAR("memo", 0xC5995F4B9FE96B89ULL);
  EncryptedName<sizeof(memoSource), 0xC5995F4B9FE96B89ULL>& memo = OBFUSCATE_DECRYPT_NAME(0xC5995F4B9FE96B89ULL, memoSource);
  WIN32_FIND_DATAA memoData;
  HANDLE memoFind = FindFirstFileA(memo.Text(), &memoData);
  if (memoFind == INVALID_HANDLE_VALUE) {
    CreateDirectoryA("memo", nullptr);
  } else {
    FindClose(memoFind);
  }

  g_navigationManager.LoadSettings();

  const std::array<char, 7> replayFolderSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Replay", 0x610F7DD1EFC1C91BULL);
  EncryptedName<sizeof(replayFolderSource), 0x610F7DD1EFC1C91BULL>& replayFolder = OBFUSCATE_DECRYPT_NAME(0x610F7DD1EFC1C91BULL, replayFolderSource);
  WIN32_FIND_DATAA replayData;
  HANDLE replayFind = FindFirstFileA(replayFolder.Text(), &replayData);
  if (replayFind == INVALID_HANDLE_VALUE) {
    const std::array<char, 7> replayDirectorySource = OBFUSCATE_SOURCE_ARRAY_CHAR("Replay", 0xED15938D333D2DB9ULL);
    EncryptedName<sizeof(replayDirectorySource), 0xED15938D333D2DB9ULL>& replayDirectory = OBFUSCATE_DECRYPT_NAME(0xED15938D333D2DB9ULL, replayDirectorySource);
    CreateDirectoryA(replayDirectory.Text(), nullptr);
  } else {
    FindClose(replayFind);
  }

  WIN32_FIND_DATAA navigationData;
  HANDLE navigationFind = FindFirstFileA("", &navigationData);
  if (replayFind == INVALID_HANDLE_VALUE) {
    const std::array<char, 15> navigationDirectorySource = OBFUSCATE_SOURCE_ARRAY_CHAR("Navigationdata", 0xC9712D676B5B3943ULL);
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
    const std::array<char, 4> devServerSource = OBFUSCATE_SOURCE_ARRAY_CHAR("Dev", 0x4B755DB19953E311ULL);
    EncryptedName<sizeof(devServerSource), 0x4B755DB19953E311ULL>& devServer = OBFUSCATE_DECRYPT_NAME(0x4B755DB19953E311ULL, devServerSource);
    if (!std::strstr(g_launchServerName, devServer.Text())) {
      const char* parameters = LookupMessage(MSI_FRIEND_ALREADY_FRIEND);
      const char* file = LookupMessage(MSI_FRIEND_ALREADY_FRIEND);
      const std::array<char, 5> openSource = OBFUSCATE_SOURCE_ARRAY_CHAR("OPEN", 0xD7A749EB27D5BF6DULL);
      EncryptedName<sizeof(openSource), 0xD7A749EB27D5BF6DULL>& open = OBFUSCATE_DECRYPT_NAME(0xD7A749EB27D5BF6DULL, openSource);
      ShellExecuteA(nullptr, open.Text(), file, parameters, nullptr, SW_SHOWNORMAL);
    } else if (g_serviceMode == 0) {
      const std::array<char, 62> parametersSource = OBFUSCATE_SOURCE_ARRAY_CHAR("http://admtools.gnjoy.com/gamestarter/devro.asp?GameCode=4011", 0xFD69A9B3F311818DULL);
      EncryptedName<sizeof(parametersSource), 0xFD69A9B3F311818DULL>& parameters = OBFUSCATE_DECRYPT_NAME(0xFD69A9B3F311818DULL, parametersSource);
      const char* arguments = parameters.Text();
      const std::array<char, 62> fileSource = OBFUSCATE_SOURCE_ARRAY_CHAR("http://admtools.gnjoy.com/gamestarter/devro.asp?GameCode=4011", 0x7F2DFFC1ABE91F2BULL);
      EncryptedName<sizeof(fileSource), 0x7F2DFFC1ABE91F2BULL>& file = OBFUSCATE_DECRYPT_NAME(0x7F2DFFC1ABE91F2BULL, fileSource);
      const char* target = file.Text();
      const std::array<char, 5> openSource = OBFUSCATE_SOURCE_ARRAY_CHAR("OPEN", 0xBD2BA1C11B01E73BULL);
      EncryptedName<sizeof(openSource), 0xBD2BA1C11B01E73BULL>& open = OBFUSCATE_DECRYPT_NAME(0xBD2BA1C11B01E73BULL, openSource);
      ShellExecuteA(nullptr, open.Text(), target, arguments, nullptr, SW_SHOWNORMAL);
    } else {
      const std::array<char, 62> parametersSource = OBFUSCATE_SOURCE_ARRAY_CHAR("http://admtools.gnjoy.com/gamestarter/devro.asp?GameCode=6011", 0xB195637DAFCB0D59ULL);
      EncryptedName<sizeof(parametersSource), 0xB195637DAFCB0D59ULL>& parameters = OBFUSCATE_DECRYPT_NAME(0xB195637DAFCB0D59ULL, parametersSource);
      const char* arguments = parameters.Text();
      const std::array<char, 62> fileSource = OBFUSCATE_SOURCE_ARRAY_CHAR("http://admtools.gnjoy.com/gamestarter/devro.asp?GameCode=6011", 0xD98F5345098D630DULL);
      EncryptedName<sizeof(fileSource), 0xD98F5345098D630DULL>& file = OBFUSCATE_DECRYPT_NAME(0xD98F5345098D630DULL, fileSource);
      const char* target = file.Text();
      const std::array<char, 5> openSource = OBFUSCATE_SOURCE_ARRAY_CHAR("OPEN", 0x9B0F234DEFDF7593ULL);
      EncryptedName<sizeof(openSource), 0x9B0F234DEFDF7593ULL>& open = OBFUSCATE_DECRYPT_NAME(0x9B0F234DEFDF7593ULL, openSource);
      ShellExecuteA(nullptr, open.Text(), target, arguments, nullptr, SW_SHOWNORMAL);
    }
  }

  CThemidaMgr::StopThread();
  return 0;
}
