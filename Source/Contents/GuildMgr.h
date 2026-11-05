#ifndef RAGNAROK_SOURCE_CONTENTS_GUILDMGR_H
#define RAGNAROK_SOURCE_CONTENTS_GUILDMGR_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

class CGuildEmblemReqBundle;
class CEmblemDataMgr;
class CConnection;
class CAsyncWorkMgr;

class CGuild {
public:
  __declspec(noinline) CGuild();
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
  std::string EmblemFilename(std::uint32_t guild, std::uint32_t revision) const;
  bool IsInvalidEmblem(std::string path) const;
  void MarkInvalidEmblem(std::string path);
  static void StoreEmblem(const void* bytes, std::uint32_t size, std::uint32_t guild, std::uint32_t revision);
  static void __stdcall ReceiveEmblem(std::uint16_t state, std::uint32_t guild, std::uint32_t revision, const unsigned char* bytes, int size);
  static void RefreshEmblem(std::uint32_t guild);
  void UpdateEmblemRequests(
    CEmblemDataMgr& emblems, std::uint32_t accountId, const std::string& accountName, const char* token16, CConnection& connection, CAsyncWorkMgr& manager
  );

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::map<std::uint32_t, CGuild> m_guilds;
  /* 0010 */ CGuildEmblemReqBundle* m_reqBundle;
  /* 0014 */ std::set<std::string> m_invalidEmblems;
  /* 001C */ std::uint32_t m_receiveGuildId;
  /* 0020 */ std::uint32_t m_receiveVersion;
  /* 0024 */ std::vector<unsigned char> m_receiveData;
  /* 0030 */ int m_timeout;
};

extern CGuildMgr* g_guildMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGuild) == 8);
static_assert(offsetof(CGuild, m_emblemVersion) == 0x04);
static_assert(sizeof(CGuildMgr) == 52);
static_assert(offsetof(CGuildMgr, m_initialized) == 0x04);
static_assert(offsetof(CGuildMgr, m_guilds) == 0x08);
static_assert(offsetof(CGuildMgr, m_reqBundle) == 0x10);
static_assert(offsetof(CGuildMgr, m_invalidEmblems) == 0x14);
static_assert(offsetof(CGuildMgr, m_receiveGuildId) == 0x1C);
static_assert(offsetof(CGuildMgr, m_receiveData) == 0x24);
static_assert(offsetof(CGuildMgr, m_timeout) == 0x30);
#endif

#endif
