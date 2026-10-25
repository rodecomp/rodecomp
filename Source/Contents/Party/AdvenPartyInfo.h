#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENPARTYINFO_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_ADVENPARTYINFO_H

#include <cstdint>
#include <string>

namespace Json {
  class Value;
}

class CAdvenPartyInfo {
public:
  CAdvenPartyInfo();
  CAdvenPartyInfo(const CAdvenPartyInfo& other);
  virtual ~CAdvenPartyInfo();

  /* 0004 */ bool tanker;
  /* 0005 */ bool healer;
  /* 0006 */ bool dealer;
  /* 0007 */ bool assist;
  /* 0008 */ int minimumLevel;
  /* 000C */ int maximumLevel;
  /* 0010 */ int type;
  /* 0014 */ std::string memo;
  /* 002C */ std::uint32_t accountId;
  /* 0030 */ std::uint32_t characterId;
  /* 0034 */ std::string characterName;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAdvenPartyInfo) == 76);
#endif

#endif
