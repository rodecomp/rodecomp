#ifndef RAGNAROK_SOURCE_CONTENTS_ACCOUNTLINKEDUSERDATASAVEASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_ACCOUNTLINKEDUSERDATASAVEASYNCWORK_H

#include "Utility/AsyncDiagnostic.h"
#include "Utility/NAsyncWork.h"

// Original pool/deleting entries allocate/free 200 bytes; no extra tail field.
class alignas(8) CAccountLinkedUserDataSaveAsyncWork : public NAsyncWork {
public:
  CAccountLinkedUserDataSaveAsyncWork();
  ~CAccountLinkedUserDataSaveAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(std::uint32_t accountId, std::string accountName, std::string url, std::string token, std::string data);

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::string m_accountName;
  /* 0044 */ std::string m_url;
  /* 005C */ std::string m_data;
  /* 0074 */ std::string m_token;
  /* 008C */ AsyncDiagnostic m_diagnostic;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAccountLinkedUserDataSaveAsyncWork) == 200);
#endif

#endif
