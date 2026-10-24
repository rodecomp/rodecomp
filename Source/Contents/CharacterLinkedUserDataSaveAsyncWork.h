#ifndef RAGNAROK_SOURCE_CONTENTS_CHARACTERLINKEDUSERDATASAVEASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_CHARACTERLINKEDUSERDATASAVEASYNCWORK_H

#include "Utility/AsyncDiagnostic.h"
#include "Utility/NAsyncWork.h"

class CCharacterLinkedUserDataSaveAsyncWork : public NAsyncWork {
public:
  CCharacterLinkedUserDataSaveAsyncWork();
  ~CCharacterLinkedUserDataSaveAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::uint32_t accountId,
    std::uint32_t characterId,
    std::string accountName,
    std::string url,
    std::string token,
    std::string data
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_accountName;
  /* 0048 */ std::string m_url;
  /* 0060 */ std::string m_data;
  /* 0078 */ std::string m_token;
  /* 0090 */ AsyncDiagnostic m_diagnostic;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCharacterLinkedUserDataSaveAsyncWork) == 200);
#endif

#endif
