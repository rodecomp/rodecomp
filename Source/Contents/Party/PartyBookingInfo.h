#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYBOOKINGINFO_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYBOOKINGINFO_H

#include <ctime>

struct PARTY_BOOKING_DETAIL {
  /* 0000 */ short level;
  /* 0002 */ short mapId;
  /* 0004 */ short job[6];
};

struct PARTY_BOOKING_AD_INFO {
  /* 0000 */ int index;
  /* 0004 */ char charName[24];
  /* 001C */ __time32_t expireTime;
  /* 0020 */ PARTY_BOOKING_DETAIL detail;
};

static_assert(sizeof(PARTY_BOOKING_AD_INFO) == 0x30);

#endif
