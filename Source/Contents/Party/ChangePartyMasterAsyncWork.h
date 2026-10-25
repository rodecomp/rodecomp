#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_CHANGEPARTYMASTERASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_CHANGEPARTYMASTERASYNCWORK_H

#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class WindowManagerWindows;

class CChangePartyMasterAsyncWork : public NAsyncWork {
public:
  CChangePartyMasterAsyncWork();
  ~CChangePartyMasterAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::uint32_t accountId, std::uint32_t characterId, std::string characterName, std::uint32_t targetAccountId, std::uint32_t targetCharacterId,
    std::string targetName, std::string worldName, std::string url, std::string token, WindowManagerWindows& windows
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::uint32_t m_targetAccountId;
  /* 0034 */ std::uint32_t m_targetCharacterId;
  /* 0038 */ std::string m_characterName;
  /* 0050 */ std::string m_targetName;
  /* 0068 */ std::string m_token;
  /* 0080 */ std::string m_worldName;
  /* 0098 */ std::string m_response;
  /* 00B0 */ std::string m_url;
  /* 00C8 */ AsyncDiagnostic m_diagnostic;
  WindowManagerWindows* m_windows;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CChangePartyMasterAsyncWork) == 260);
#endif

#endif
