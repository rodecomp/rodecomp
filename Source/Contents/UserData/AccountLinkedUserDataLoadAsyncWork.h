#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATALOADASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTLINKEDUSERDATALOADASYNCWORK_H

#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class alignas(8) CAccountLinkedUserDataLoadAsyncWork : public NAsyncWork {
public:
  CAccountLinkedUserDataLoadAsyncWork();
  ~CAccountLinkedUserDataLoadAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(std::uint32_t accountId, std::string accountName, std::string url);

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::string m_accountName;
  /* 0044 */ std::string m_url;
  /* 005C */ std::string m_response;
  /* 0074 */ AsyncDiagnostic m_diagnostic;
  /* 00AC */ int padding_00AC;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAccountLinkedUserDataLoadAsyncWork) == 176);
#endif

#endif
