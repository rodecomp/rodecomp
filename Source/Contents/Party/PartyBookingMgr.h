#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYBOOKINGMGR_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYBOOKINGMGR_H

#include "Contents/Party/PartyBookingInfo.h"

#include <cstddef>
#include <ctime>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct PartyBookingMapInfo {
  /* 0000 */ std::string name;
  /* 0018 */ int id;
};

static_assert(sizeof(PartyBookingMapInfo) == 0x1C);

struct PartyBookingEntry {
  /* 0000 */ std::string time;
  /* 0018 */ std::string charName;
  /* 0030 */ std::string level;
  /* 0048 */ std::string location;
  /* 0060 */ std::string jobs;
  /* 0078 */ std::string leaderName;
  /* 0090 */ bool isEnd;
};

static_assert(sizeof(PartyBookingEntry) == 0x94);

class PartyBookingMgr {
public:
  PartyBookingMgr();

  void Reset();
  void Clear();
  void SetFilter(int level, int mapId, int job);
  void Remove(int index);
  void EraseEntry(int index);
  void SetJobs(int index, const short* jobs);
  bool FlushPending();
  void AddEntries(const PARTY_BOOKING_AD_INFO* infos, int count, bool more);
  void OnAdvertisement(PARTY_BOOKING_AD_INFO info);
  std::pair<bool, PartyBookingEntry> MakeEntry(PARTY_BOOKING_AD_INFO info);

  /* 0000 */ int unknown_0000;
  /* 0004 */ bool m_alarm;
  /* 0005 */ bool m_filterEnabled;
  /* 0008 */ std::vector<std::string> m_regionNames;
  /* 0014 */ std::map<int, std::vector<PartyBookingMapInfo>> m_maps;
  /* 001C */ std::map<int, PartyBookingEntry> m_entries;
  /* 0024 */ std::map<int, PartyBookingEntry> m_pending;
  /* 002C */ bool m_lastPage;
  /* 0030 */ int m_lastIndex;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int m_filterLevel;
  /* 003C */ int m_filterMap;
  /* 0040 */ int m_filterJob;
};

static_assert(offsetof(PartyBookingMgr, m_regionNames) == 0x08);
static_assert(offsetof(PartyBookingMgr, m_maps) == 0x14);
static_assert(offsetof(PartyBookingMgr, m_entries) == 0x1C);
static_assert(offsetof(PartyBookingMgr, m_pending) == 0x24);
static_assert(offsetof(PartyBookingMgr, m_lastPage) == 0x2C);
static_assert(offsetof(PartyBookingMgr, m_filterJob) == 0x40);

#endif
