#ifndef RAGNAROK_SOURCE_CONTENTS_GUILD_GUILDMGR_H
#define RAGNAROK_SOURCE_CONTENTS_GUILD_GUILDMGR_H

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
  struct ReceivedEmblem {
    /* 0000 */ std::uint32_t guildId;
    /* 0004 */ std::uint32_t version;
    /* 0008 */ std::vector<unsigned char> data;
  };

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
  static __declspec(noinline) void RefreshEmblem(std::uint32_t guild);
  void UpdateEmblemRequests();

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::map<std::uint32_t, CGuild> m_guilds;
  /* 0010 */ CGuildEmblemReqBundle* m_reqBundle;
  /* 0014 */ std::set<std::string> m_invalidEmblems;
  /* 001C */ ReceivedEmblem m_receivedEmblem;
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
static_assert(offsetof(CGuildMgr, m_receivedEmblem) == 0x1C);
static_assert(offsetof(CGuildMgr, m_receivedEmblem) + offsetof(CGuildMgr::ReceivedEmblem, data) == 0x24);
static_assert(offsetof(CGuildMgr, m_timeout) == 0x30);
#endif

#endif
