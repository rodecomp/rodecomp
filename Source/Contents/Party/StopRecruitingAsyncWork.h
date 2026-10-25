#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_STOPRECRUITINGASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_STOPRECRUITINGASYNCWORK_H

#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class WindowManagerWindows;
class ResourceMessages;

struct StopRecruitingContext {
  WindowManagerWindows& windows;
  ResourceMessages& messages;
  const std::uint32_t& accountId;
  const bool& languagePrefix;
  const int& service;
  const unsigned short& language;
};

class CStopRecruitingAsyncWork : public NAsyncWork {
public:
  CStopRecruitingAsyncWork();
  ~CStopRecruitingAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(std::uint32_t accountId, std::string world, std::string url, std::string token, std::uint32_t masterAccountId, StopRecruitingContext& context);

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_masterAccountId;
  /* 0030 */ std::string m_token;
  /* 0048 */ std::string m_world;
  /* 0060 */ std::string m_response;
  /* 0078 */ std::string m_url;
  /* 0090 */ AsyncDiagnostic m_diagnostic;
  StopRecruitingContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CStopRecruitingAsyncWork) == 204);
#endif

#endif
