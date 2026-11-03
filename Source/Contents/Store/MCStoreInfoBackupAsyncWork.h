#ifndef RAGNAROK_SOURCE_CONTENTS_STORE_MCSTOREINFOBACKUPASYNCWORK_H
#define RAGNAROK_SOURCE_CONTENTS_STORE_MCSTOREINFOBACKUPASYNCWORK_H

#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Async/NAsyncWork.h"

class CMCStoreInfoBackupLoadAsyncWork : public NAsyncWork {
public:
  CMCStoreInfoBackupLoadAsyncWork();
  ~CMCStoreInfoBackupLoadAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::string url, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string token, int type
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_accountName;
  /* 0048 */ std::string m_url;
  /* 0060 */ std::string m_token;
  /* 0078 */ std::string m_response;
  /* 0090 */ AsyncDiagnostic m_diagnostic;
  /* 00C8 */ int m_type;

  /* 00CC */ int unknown_00CC;
};

class CMCStoreInfoBackupSaveAsyncWork : public NAsyncWork {
public:
  CMCStoreInfoBackupSaveAsyncWork();
  ~CMCStoreInfoBackupSaveAsyncWork() override;
  void Reset() override;
  bool Execute() override;
  void Complete() override;
  void Bind(
    std::string url, std::uint32_t accountId, std::uint32_t characterId, std::string accountName, std::string token, std::string data, int type
  );

protected:
  /* 0028 */ std::uint32_t m_accountId;
  /* 002C */ std::uint32_t m_characterId;
  /* 0030 */ std::string m_accountName;
  /* 0048 */ std::string m_url;
  /* 0060 */ std::string m_data;
  /* 0078 */ std::string m_token;
  /* 0090 */ AsyncDiagnostic m_diagnostic;
  /* 00C8 */ std::string m_response;
  /* 00E0 */ int m_type;
  /* 00E4 */ unsigned char unknown_00E4[4];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMCStoreInfoBackupLoadAsyncWork) == 208);
static_assert(sizeof(CMCStoreInfoBackupSaveAsyncWork) == 232);
#endif

#endif
