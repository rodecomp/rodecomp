#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_LOADPARTYINFOASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_LOADPARTYINFOASYNCWORK_H

#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class WindowManagerWindows;
class ResourceMessages;
class ModeEvents;
struct WindowOrdering;

struct PartyInfoContext {
  WindowManagerWindows& windows;
  WindowOrdering& ordering;
  ResourceMessages& messages;
  ModeEvents*& mode;
  const int& playerLevel;
  const int& partyCount;
  const bool& languagePrefix;
  const int& service;
  const unsigned short& language;
};

class alignas(8) CLoadPartyInfoAsyncWork : public NAsyncWork {
public:
  CLoadPartyInfoAsyncWork();
  ~CLoadPartyInfoAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::uint32_t queryAccountId,
    PartyInfoContext& context
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_world;
  /* 0048 */ std::string m_url;
  /* 0060 */ std::string m_response;
  /* 0078 */ std::string m_token;
  /* 0090 */ std::uint32_t m_queryAccountId;
  /* 0094 */ AsyncDiagnostic m_diagnostic;
  PartyInfoContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CLoadPartyInfoAsyncWork) == 208);
#endif

#endif
