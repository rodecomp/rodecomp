#ifndef RAGNAROK_SOURCE_CONTENTS_REGISTERPARTYASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_REGISTERPARTYASYNCWORK_H

#include "Contents/AdvenPartyInfo.h"
#include "Utility/AsyncDiagnostic.h"
#include "Utility/NAsyncWork.h"

class WindowManagerWindows;
class ResourceMessages;

// Borrowed client name/locale and manager operations used by the original work.
struct RegisterPartyContext {
  WindowManagerWindows& windows;
  ResourceMessages& messages;
  const std::string& characterName;
  const bool& languagePrefix;
  const int& service;
  const unsigned short& language;
};

// RTTI CRegisterPartyAsyncWork, original allocation 280.
// The source context pointer occupies the original alignment tail on x86.
class alignas(8) CRegisterPartyAsyncWork : public NAsyncWork {
public:
  CRegisterPartyAsyncWork();
  ~CRegisterPartyAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::uint32_t accountId,
    std::uint32_t characterId,
    std::string world,
    std::string url,
    std::string token,
    const CAdvenPartyInfo& info,
    RegisterPartyContext& context
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_token;
  /* 0048 */ std::string m_world;
  /* 0060 */ std::string m_url;
  /* 0078 */ CAdvenPartyInfo m_info;
  /* 00C4 */ AsyncDiagnostic m_diagnostic;
  /* 00FC */ std::string m_response;
  RegisterPartyContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRegisterPartyAsyncWork) == 280);

#endif

#endif
