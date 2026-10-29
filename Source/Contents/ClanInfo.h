#ifndef RAGNAROK_SOURCE_CONTENTS_CLANINFO_H
#define RAGNAROK_SOURCE_CONTENTS_CLANINFO_H

#include <cstddef>
#include <list>
#include <string>
#include <utility>

class CClanInfo {
public:
  /* 0000 */ bool m_isInitialized;
  /* 0004 */ std::string m_clanName;
  /* 001C */ std::string m_masterName;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int m_connectedMembers;
  /* 003C */ int m_totalMembers;
  /* 0040 */ std::string m_mapName;
  /* 0058 */ int m_clanId;
  /* 005C */ bool m_hasClan;
  /* 0060 */ std::list<std::pair<int, std::string>> m_relations;

  CClanInfo();
  ~CClanInfo();

  static void Destroy();
  void Clear();
};

extern CClanInfo* g_clanInfo;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CClanInfo) == 0x68);
static_assert(offsetof(CClanInfo, m_clanName) == 0x04);
static_assert(offsetof(CClanInfo, m_masterName) == 0x1C);
static_assert(offsetof(CClanInfo, unknown_0034) == 0x34);
static_assert(offsetof(CClanInfo, m_mapName) == 0x40);
static_assert(offsetof(CClanInfo, m_clanId) == 0x58);
static_assert(offsetof(CClanInfo, m_hasClan) == 0x5C);
static_assert(offsetof(CClanInfo, m_relations) == 0x60);
#endif

#endif
