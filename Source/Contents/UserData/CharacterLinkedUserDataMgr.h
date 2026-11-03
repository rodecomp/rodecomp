#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_CHARACTERLINKEDUSERDATAMGR_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_CHARACTERLINKEDUSERDATAMGR_H

#include <cstdint>
#include <string>

class LuaScript;
struct AsyncDiagnostic;
namespace Json {
  class Value;
}
struct CharacterConfigSave;
class CAsyncWorkMgr;

class CCharacterLinkedUserDataMgr {
public:
  CCharacterLinkedUserDataMgr();
  virtual ~CCharacterLinkedUserDataMgr();
  static bool Initialize();
  static void Shutdown();
  std::string LoadUrl() const;
  std::string SaveUrl() const;
  void CompleteSave();
  void CompleteLoad(AsyncDiagnostic diagnostic, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string response);
  void Load(std::uint32_t accountId, std::uint32_t characterId, std::string accountName);
  void LoadUseSkillLevels(Json::Value& data);
  void LoadHomunculusSkillLevels(Json::Value& data);
  void LoadNormalItemSlots(Json::Value& data);
  void Save(CharacterConfigSave& context, const char* token16, CAsyncWorkMgr& workManager);
  void RefreshUrls(LuaScript& script, const char* host, const char* port, const char* overrideKey);
  void Reset(LuaScript& script, const char* host, const char* port, const char* overrideKey);

protected:
  /* 0004 */ bool m_enabled;
  /* 0008 */ std::uint32_t m_accountId;
  /* 000C */ std::uint32_t m_characterId;
  /* 0010 */ std::string m_accountName;
  /* 0028 */ bool m_loaded;
  /* 002C */ std::uint32_t m_pendingSaves;
  /* 0030 */ std::string m_saveUrl;
  /* 0048 */ std::string m_loadUrl;
};

extern CCharacterLinkedUserDataMgr* g_characterLinkedUserDataMgr;
extern bool g_sortPrivateTab;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCharacterLinkedUserDataMgr) == 96);
#endif

#endif
