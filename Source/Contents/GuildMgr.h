#ifndef RAGNAROK_SOURCE_CONTENTS_GUILDMGR_H
#define RAGNAROK_SOURCE_CONTENTS_GUILDMGR_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

class CGuildEmblemReqBundle;
class CEmblemDataMgr;
class CConnection;
class CAsyncWorkMgr;

class CGuild {
public:
  CGuild();
  virtual ~CGuild();

  /* 0004 */ int m_emblemVersion;
};

class CGuildMgr {
public:
  CGuildMgr();
  virtual ~CGuildMgr();

  static bool Create();
  static void Destroy();

  int GetEmblemVersion(std::uint32_t guildId) const;
  std::string GetEmblemFilename(std::uint32_t guildId);
  void SetEmblemVersion(std::uint32_t guildId, int version);
  void UpdateEmblemRequests(
    CEmblemDataMgr& emblems, std::uint32_t accountId, const std::string& accountName, const char* token16, CConnection& connection, CAsyncWorkMgr& manager
  );

  struct MemberData {
    /* 0000 */ int unknown_0000[5];
  };
  /* 0004 */ bool m_initialized;
  /* 0008 */ std::map<std::uint32_t, CGuild> m_guilds;
  /* 0010 */ CGuildEmblemReqBundle* m_reqBundle;
  /* 0014 */ std::map<int, MemberData> m_members;
  /* 001C */ int unknown_001C[2];
  /* 0024 */ std::vector<int> m_unknownVector;
  /* 0030 */ int m_timeout;
};

extern CGuildMgr* g_guildMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGuild) == 8);
static_assert(offsetof(CGuild, m_emblemVersion) == 0x04);
static_assert(sizeof(CGuildMgr) == 52);
static_assert(sizeof(CGuildMgr::MemberData) == 20);
static_assert(offsetof(CGuildMgr, m_initialized) == 0x04);
static_assert(offsetof(CGuildMgr, m_guilds) == 0x08);
static_assert(offsetof(CGuildMgr, m_reqBundle) == 0x10);
static_assert(offsetof(CGuildMgr, m_members) == 0x14);
static_assert(offsetof(CGuildMgr, unknown_001C) == 0x1C);
static_assert(offsetof(CGuildMgr, m_unknownVector) == 0x24);
static_assert(offsetof(CGuildMgr, m_timeout) == 0x30);
#endif

#endif
