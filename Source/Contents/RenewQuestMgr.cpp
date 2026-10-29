#include "Contents/RenewQuestMgr.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <new>
#include <utility>

#pragma pack(push, 1)
struct RenewQuestOngoingRecord {
  /* 0000 */ unsigned int questId;
  /* 0004 */ unsigned short state;
  /* 0006 */ short huntCount;
  /* 0008 */ short year;
  /* 000A */ short month;
  /* 000C */ short day;
  /* 000E */ short hour;
  /* 0010 */ unsigned __int64 minute;
};

struct RenewQuestHuntRecord {
  /* 0000 */ int huntId;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ short unknown_000C;
  /* 000E */ short unknown_000E;
  /* 0010 */ short unknown_0010;
  /* 0012 */ short unknown_0012;
  /* 0014 */ char name[0x33];
};

struct PACKET_CZ_QUEST_STATUS_REQ {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::int16_t packetLength;
};

struct PACKET_CZ_QUEST_STATUS_REQ_BUFFER {
  /* 0000 */ PACKET_CZ_QUEST_STATUS_REQ header;
  /* 0004 */ unsigned int questIds[511];
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestOngoingRecord) == 0x18);
static_assert(sizeof(RenewQuestHuntRecord) == 0x47);
static_assert(sizeof(PACKET_CZ_QUEST_STATUS_REQ_BUFFER) == 0x800);
#endif

// GLOBAL: gl_24837
CRenewQuestMgr* g_renewQuestMgr;

// OFFSET: 0x00675990

// OFFSET: 0x00677C00
// FUNCTION: fn_05433
bool CRenewQuestMgr::Clear() {
  this->m_ongoingQuests.clear();
  this->m_recommendedQuests.clear();
  return true;
}

// OFFSET: 0x00677C50

// OFFSET: 0x00677D00

// OFFSET: 0x00677D20

// OFFSET: 0x00677D80

// OFFSET: 0x006779A0

// OFFSET: 0x00677F40

// OFFSET: 0x006792A0

// OFFSET: 0x00679A00

// OFFSET: 0x00679BD0

// OFFSET: 0x00679C20

// OFFSET: 0x00679E00

// OFFSET: 0x0067A500

// OFFSET: 0x0067AEB0

// OFFSET: 0x0067AF00

// OFFSET: 0x0067B030

// OFFSET: 0x0067B460

// OFFSET: 0x0067B6E0

// OFFSET: 0x0067B800

// OFFSET: 0x0067B850

// OFFSET: 0x0067B8A0

// OFFSET: 0x0067BF90

// OFFSET: 0x0067E9F0
