#include "Contents/AdvenPartyInfo.h"

// OFFSET: 0x005CA660
// FUNCTION: fn_03647
CAdvenPartyInfo::CAdvenPartyInfo()
    : tanker(false), healer(false), dealer(false), assist(false), minimumLevel(0), maximumLevel(999), type(0),
      accountId(0), characterId(0) {
}

// OFFSET: 0x005CA5B0
// FUNCTION: fn_03646
CAdvenPartyInfo::CAdvenPartyInfo(const CAdvenPartyInfo& other) = default;

// OFFSET: 0x005739D0
// FUNCTION: fn_02494
// OFFSET: 0x00573AA0
// FUNCTION: fn_02495
CAdvenPartyInfo::~CAdvenPartyInfo() = default;
