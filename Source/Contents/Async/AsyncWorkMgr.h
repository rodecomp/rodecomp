#ifndef RAGNAROK_SOURCE_CONTENTS_ASYNC_ASYNCWORKMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ASYNC_ASYNCWORKMGR_H

#include "Contents/Party/AdvenPartyInfo.h"
#include "Utility/Async/NAsyncWorkMgr.h"

class CAsyncWorkRecycleObjectPoolMgr;
class CConnection;
class CEmblemDataMgr;
class CMCStoreInfoBackup;
class WindowManagerWindows;
struct AccountConfigLoad;
struct CharacterConfigLoad;
struct EmblemUploadContext;
struct StoreBackupContext;
struct RegisterPartyContext;
struct StopRecruitingContext;
struct PartyInfoContext;

class CAsyncWorkMgr : public NAsyncWorkMgr {
public:
  explicit CAsyncWorkMgr(std::uint32_t workers);
  ~CAsyncWorkMgr() override;

  static bool Initialize(std::uint32_t workers);
  static void Shutdown();

  void LoadAccount(std::uint32_t accountId, std::string world, std::string url, AccountConfigLoad& context);
  void SaveAccount(std::uint32_t accountId, std::string world, std::string url, std::string token, std::string data);
  void ChangePartyMaster(
    std::uint32_t accountId, std::uint32_t characterId, std::string characterName, std::uint32_t targetAccountId, std::uint32_t targetCharacterId,
    std::string targetName, std::string world, std::string url, std::string token, WindowManagerWindows& windows
  );
  void LoadCharacter(std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, CharacterConfigLoad& context);
  void SaveCharacter(std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::string data);
  void DownloadEmblem(
    std::string url, std::uint32_t accountId, std::string token, std::string world, std::uint32_t guildId, std::uint32_t version, CConnection& connection
  );
  void UploadEmblem(
    std::string url, std::uint32_t accountId, std::string token, std::string world, std::uint32_t guildId, std::string path, CEmblemDataMgr& owner,
    EmblemUploadContext& context
  );
  void LoadMyParty(std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, WindowManagerWindows& windows);
  void LoadPartyInfo(
    std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::uint32_t queryAccountId,
    PartyInfoContext& context
  );
  void LoadPartyList(
    std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, int search, CAdvenPartyInfo filter, int page,
    WindowManagerWindows& windows
  );
  void LoadStore(
    std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, int type, CMCStoreInfoBackup& owner,
    StoreBackupContext& context
  );
  void SaveStore(
    std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, std::string data, int type,
    CMCStoreInfoBackup& owner, StoreBackupContext& context
  );
  void RegisterParty(
    std::uint32_t accountId, std::uint32_t characterId, std::string world, std::string url, std::string token, CAdvenPartyInfo info,
    RegisterPartyContext& context
  );
  void
  StopRecruiting(std::uint32_t accountId, std::string world, std::string url, std::string token, std::uint32_t masterAccountId, StopRecruitingContext& context);

  // INLINED
  CAsyncWorkRecycleObjectPoolMgr& Pools() {
    return *this->m_pools;
  }

private:
  /* 0020 */ bool m_ready;
  /* 0024 */ CAsyncWorkRecycleObjectPoolMgr* m_pools;
};

extern CAsyncWorkMgr* g_asyncWorkMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAsyncWorkMgr) == 40);
#endif

#endif
