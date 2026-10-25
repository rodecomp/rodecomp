#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMS_GUILDEMBLEMREQBUNDLE_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMS_GUILDEMBLEMREQBUNDLE_H

#include <cstdint>
#include <deque>
#include <string>
#include <utility>

class CEmblemDataMgr;
class CConnection;
class CAsyncWorkMgr;

class CGuildEmblemReqBundle {
public:
  CGuildEmblemReqBundle();
  virtual ~CGuildEmblemReqBundle();
  void Add(std::uint32_t guildId, std::uint32_t version);
  void Update(
    CEmblemDataMgr& emblems, std::uint32_t accountId, const std::string& accountName, const char* token16, CConnection& connection, CAsyncWorkMgr& manager
  );

protected:
  struct Recent {
    std::uint32_t guildId;
    std::uint32_t version;
    std::uint32_t tick;
  };
  /* 0004 */ std::deque<std::pair<std::uint32_t, std::uint32_t>> m_pending;
  /* 0018 */ std::deque<Recent> m_recent;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGuildEmblemReqBundle) == 44);
#endif

#endif
