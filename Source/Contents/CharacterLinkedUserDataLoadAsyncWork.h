#ifndef RAGNAROK_SOURCE_CONTENTS_CHARACTERLINKEDUSERDATALOADASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_CHARACTERLINKEDUSERDATALOADASYNCWORK_H

#include "Utility/AsyncDiagnostic.h"
#include "Utility/NAsyncWork.h"

struct CharacterConfigLoad;

class CCharacterLinkedUserDataLoadAsyncWork : public NAsyncWork {
public:
  CCharacterLinkedUserDataLoadAsyncWork();
  ~CCharacterLinkedUserDataLoadAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::uint32_t accountId,
    std::uint32_t characterId,
    std::string accountName,
    std::string url,
    std::string token,
    CharacterConfigLoad& context
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_accountName;
  /* 0048 */ std::string m_url;
  /* 0060 */ std::string m_token;
  /* 0078 */ std::string m_response;
  /* 0090 */ AsyncDiagnostic m_diagnostic;

  // Host binding for original global owner state; outlives queued completion.
  // Not part of the original 200-byte x86 object.
  CharacterConfigLoad* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCharacterLinkedUserDataLoadAsyncWork) == 204);
#endif

#endif
