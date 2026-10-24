#ifndef RAGNAROK_SOURCE_CONTENTS_LOADMYPARTYASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_LOADMYPARTYASYNCWORK_H

#include "Utility/AsyncDiagnostic.h"
#include "Utility/NAsyncWork.h"

class WindowManagerWindows;

// RTTI CLoadMyPartyAsyncWork, original x86 size 200.
// The borrowed window-manager binding is source-only, not original work state.
class CLoadMyPartyAsyncWork : public NAsyncWork {
public:
  CLoadMyPartyAsyncWork();
  ~CLoadMyPartyAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::uint32_t accountId,
    std::uint32_t characterId,
    std::string world,
    std::string url,
    std::string token,
    WindowManagerWindows& windows
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_token;
  /* 0048 */ std::string m_world;
  /* 0060 */ std::string m_response;
  /* 0078 */ std::string m_url;
  /* 0090 */ AsyncDiagnostic m_diagnostic;
  WindowManagerWindows* m_windows;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CLoadMyPartyAsyncWork) == 204);
#endif

#endif
