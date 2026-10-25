#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENTURERAGENCYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENTURERAGENCYMGR_H

#include "Contents/Party/AdvenPartyInfo.h"

struct AsyncDiagnostic;
struct StopRecruitingContext;
struct RegisterPartyContext;
struct PartyInfoContext;
struct AgencyRequestContext;
class WindowManagerWindows;
class LuaScript;

class CAdventurerAgencyMgr {
public:
  CAdventurerAgencyMgr();
  virtual ~CAdventurerAgencyMgr();
  static bool Initialize();
  static void Shutdown();
  void LoadList(const Json::Value& value);
  const std::vector<CAdvenPartyInfo>& Entries() const;
  void RequestChangeMaster(std::uint32_t targetAccountId, std::uint32_t targetCharacterId, std::string targetName, AgencyRequestContext& context);
  void RequestMyParty(AgencyRequestContext& context);
  void RequestList(int page, AgencyRequestContext& context);
  void RequestInfo(std::uint32_t queryAccountId, AgencyRequestContext& context, PartyInfoContext& completion);
  void RequestSearch(const CAdvenPartyInfo& filter, int page, AgencyRequestContext& context);
  void RequestRegistration(const CAdvenPartyInfo& info, AgencyRequestContext& context, RegisterPartyContext& completion);
  void RequestStop(std::uint32_t masterAccountId, AgencyRequestContext& context, StopRecruitingContext& completion);
  void RequestStop(AgencyRequestContext& context, StopRecruitingContext& completion);
  void CompleteChangeMaster(AsyncDiagnostic diagnostic, std::string response, WindowManagerWindows& windows);
  void CompleteList(AsyncDiagnostic diagnostic, std::string response, WindowManagerWindows& windows);
  void CompleteMyParty(AsyncDiagnostic diagnostic, std::string response, WindowManagerWindows& windows);
  void CompletePartyInfo(AsyncDiagnostic diagnostic, std::string response, PartyInfoContext& context);
  void CompleteRegistration(AsyncDiagnostic diagnostic, std::string response, RegisterPartyContext& context);
  void CompleteStopRecruiting(AsyncDiagnostic diagnostic, std::string response, StopRecruitingContext& context);
  std::string ServiceUrl(int operation, LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey) const;

  /* 0004 */ bool enabled;
  /* 0005 */ bool registered;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::vector<CAdvenPartyInfo> entries;
  /* 0018 */ std::uint32_t pending;

private:
  static std::uint32_t Id(double value);
};

// STATIC: 0x01389848
// GLOBAL: gl_24352
extern CAdventurerAgencyMgr* g_adventurerAgencyMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAdventurerAgencyMgr) == 28);
#endif

#endif
