#include "Contents/AdventurerAgencyMgr.h"
#include "Contents/AgencyRequestContext.h"
#include "Contents/AsyncWorkMgr.h"

// OFFSET: 0x005CB3B0
// FUNCTION: fn_03657
void CAdventurerAgencyMgr::RequestChangeMaster(
  std::uint32_t targetAccountId,
  std::uint32_t targetCharacterId,
  std::string targetName,
  AgencyRequestContext& context
) {
  const std::uint32_t account = context.accountId;
  const std::uint32_t character = context.characterId;
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(6, context.settings, context.tables, context.host, context.port, context.overrideKey);
  const std::string world(context.world.c_str());
  context.work.ChangePartyMaster(
    account, character, std::string(context.characterName.c_str()), targetAccountId, targetCharacterId, targetName,
    world, url, token, context.windows
  );
  ++this->pending;
}

// OFFSET: 0x005CB710
// FUNCTION: fn_03659
void CAdventurerAgencyMgr::RequestMyParty(AgencyRequestContext& context) {
  const std::uint32_t account = context.accountId;
  const std::uint32_t character = context.characterId;
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(5, context.settings, context.tables, context.host, context.port, context.overrideKey);
  context.work.LoadMyParty(account, character, std::string(context.world.c_str()), url, token, context.windows);
  ++this->pending;
}

// OFFSET: 0x005CBEE0
// FUNCTION: fn_03661
void CAdventurerAgencyMgr::RequestList(int page, AgencyRequestContext& context) {
  const std::uint32_t account = context.accountId;
  const std::uint32_t character = context.characterId;
  CAdvenPartyInfo filter;
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(1, context.settings, context.tables, context.host, context.port, context.overrideKey);
  context.work.LoadPartyList(
    account, character, std::string(context.world.c_str()), url, token, 0, filter, page, context.windows
  );
  ++this->pending;
}

// OFFSET: 0x005CC290
// FUNCTION: fn_03663
void CAdventurerAgencyMgr::RequestInfo(
  std::uint32_t queryAccountId,
  AgencyRequestContext& context,
  PartyInfoContext& completion
) {
  const std::uint32_t account = context.accountId;
  const std::uint32_t character = context.characterId;
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(7, context.settings, context.tables, context.host, context.port, context.overrideKey);
  context.work.LoadPartyInfo(
    account, character, std::string(context.world.c_str()), url, token, queryAccountId, completion
  );
  ++this->pending;
}

// OFFSET: 0x005CCC00
// FUNCTION: fn_03665
void CAdventurerAgencyMgr::RequestRegistration(
  const CAdvenPartyInfo& info,
  AgencyRequestContext& context,
  RegisterPartyContext& completion
) {
  const std::uint32_t account = context.accountId;
  const std::uint32_t character = context.characterId;
  const CAdvenPartyInfo registration(info);
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(0, context.settings, context.tables, context.host, context.port, context.overrideKey);
  context.work.RegisterParty(
    account, character, std::string(context.world.c_str()), url, token, registration, completion
  );
  ++this->pending;
}

// OFFSET: 0x005CCF80
// FUNCTION: fn_03667
void CAdventurerAgencyMgr::RequestSearch(const CAdvenPartyInfo& filter, int page, AgencyRequestContext& context) {
  const std::uint32_t account = context.accountId;
  const std::uint32_t character = context.characterId;
  const CAdvenPartyInfo search(filter);
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(3, context.settings, context.tables, context.host, context.port, context.overrideKey);
  context.work.LoadPartyList(
    account, character, std::string(context.world.c_str()), url, token, 1, search, page, context.windows
  );
  ++this->pending;
}

// OFFSET: 0x005CD340
// FUNCTION: fn_03669
void CAdventurerAgencyMgr::RequestStop(
  std::uint32_t masterAccountId,
  AgencyRequestContext& context,
  StopRecruitingContext& completion
) {
  this->unknown_0008 = masterAccountId;
  const std::uint32_t account = context.accountId;
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(4, context.settings, context.tables, context.host, context.port, context.overrideKey);
  context.work.StopRecruiting(account, std::string(context.world.c_str()), url, token, masterAccountId, completion);
  ++this->pending;
}

// OFFSET: 0x005CD440
// FUNCTION: fn_03670
void CAdventurerAgencyMgr::RequestStop(AgencyRequestContext& context, StopRecruitingContext& completion) {
  if (!this->registered) {
    return;
  }

  this->unknown_0008 = context.accountId;
  const std::uint32_t account = context.accountId;
  const std::string token(context.token16, 16);
  const std::string url =
    this->ServiceUrl(4, context.settings, context.tables, context.host, context.port, context.overrideKey);
  context.work.StopRecruiting(account, std::string(context.world.c_str()), url, token, account, completion);
  ++this->pending;
}
