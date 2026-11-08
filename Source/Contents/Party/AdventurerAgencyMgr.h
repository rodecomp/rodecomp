#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENTURERAGENCYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENTURERAGENCYMGR_H

#include "Contents/Party/AdvenPartyInfo.h"

#include <vector>

struct AsyncDiagnostic;
#include "UI/Window/WindowManagerWindows.h"

class CAdventurerAgencyMgr {
public:
  CAdventurerAgencyMgr();
  virtual ~CAdventurerAgencyMgr();
  static bool Initialize();
  static void Shutdown();
  void LoadList(const Json::Value& value);
  const std::vector<CAdvenPartyInfo>& Entries() const;
  void RequestChangeMaster(std::uint32_t targetAccountId, std::uint32_t targetCharacterId, std::string targetName);
  void RequestMyParty();
  void RequestList(int page);
  void RequestInfo(std::uint32_t queryAccountId);
  void RequestSearch(const CAdvenPartyInfo& filter, int page);
  void RequestRegistration(const CAdvenPartyInfo& info);
  void RequestStop(std::uint32_t masterAccountId);
  void RequestStop();
  void CompleteChangeMaster(AsyncDiagnostic diagnostic, std::string response, WindowManagerWindows& windows);
  void CompleteList(AsyncDiagnostic diagnostic, std::string response, WindowManagerWindows& windows);
  void CompleteMyParty(AsyncDiagnostic diagnostic, std::string response, WindowManagerWindows& windows);
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
