#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_LOADPARTYLISTASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_LOADPARTYLISTASYNCWORK_H

#include "Contents/Party/AdvenPartyInfo.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class WindowManagerWindows;

class CLoadPartyListAsyncWork : public NAsyncWork {
public:
  CLoadPartyListAsyncWork();
  ~CLoadPartyListAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, CAdvenPartyInfo filter, int search, int page,
    WindowManagerWindows& windows
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_world;
  /* 0048 */ std::string m_url;
  /* 0060 */ int m_page;
  /* 0064 */ std::string m_response;
  /* 007C */ std::string m_token;
  /* 0094 */ CAdvenPartyInfo m_filter;
  /* 00E0 */ AsyncDiagnostic m_diagnostic;
  /* 0118 */ int m_search;
  WindowManagerWindows* m_windows;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CLoadPartyListAsyncWork) == 288);
#endif

#endif
