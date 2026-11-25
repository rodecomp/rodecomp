#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENTURERAGENCYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENTURERAGENCYMGR_H

#include "Contents/Party/AdvenPartyInfo.h"

#include <vector>

#include "Utility/Async/AsyncDiagnostic.h"
#include "UI/Window/WindowManagerWindows.h"

enum AgencyServiceOperation : int {
  AGENCY_SERVICE_REGISTER = 0,
  AGENCY_SERVICE_LIST = 1,
  AGENCY_SERVICE_ADD = 2,
  AGENCY_SERVICE_SEARCH = 3,
  AGENCY_SERVICE_DELETE = 4,
  AGENCY_SERVICE_MY_PARTY = 5,
  AGENCY_SERVICE_CHANGE_MASTER = 6,
  AGENCY_SERVICE_INFO = 7,
};

class CAdventurerAgencyMgr {
public:
  CAdventurerAgencyMgr();
  virtual ~CAdventurerAgencyMgr();
  static bool Initialize();
  static void Shutdown();
  void LoadList(Json::Value& value);
  const std::vector<CAdvenPartyInfo>& Entries() const;
  void RequestChangeMaster(std::uint32_t targetAccountId, std::uint32_t targetCharacterId, std::string targetName);
  void RequestMyParty();
  void RequestList(int page);
  void RequestInfo(std::uint32_t queryAccountId);
  void RequestSearch(const CAdvenPartyInfo& filter, int page);
  void RequestRegistration(const CAdvenPartyInfo& info);
  void RequestStop(std::uint32_t masterAccountId);
  void RequestStop();
  void CompleteChangeMaster(AsyncDiagnostic diagnostic, std::string response);
  void CompleteList(AsyncDiagnostic diagnostic, std::string response);
  void CompleteMyParty(AsyncDiagnostic diagnostic, std::string response);
  void CompletePartyInfo(AsyncDiagnostic diagnostic, std::string response);
  void CompleteRegistration(AsyncDiagnostic diagnostic, std::string response);
  void CompleteStopRecruiting(AsyncDiagnostic diagnostic, std::string response);
  std::string ServiceUrl(int operation) const;

  /* 0004 */ bool enabled;
  /* 0005 */ bool registered;
  /* 0008 */ std::uint32_t masterAccountId;
  /* 000C */ std::vector<CAdvenPartyInfo> entries;
  /* 0018 */ std::uint32_t pending;

private:
  static std::uint32_t Id(double value);
};

extern CAdventurerAgencyMgr* g_adventurerAgencyMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAdventurerAgencyMgr) == 28);
#endif

#endif
