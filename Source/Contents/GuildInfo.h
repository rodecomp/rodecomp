#ifndef RAGNAROK_SOURCE_CONTENTS_GUILDINFO_H
#define RAGNAROK_SOURCE_CONTENTS_GUILDINFO_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <vector>

struct GuildMemberManageEntry {
  /* 0000 */ int accountId;
  /* 0004 */ int characterId;
  /* 0008 */ int positionId;
};

class GuildInfo {
public:
  struct Member {
    /* 0000 */ int accountId;
    /* 0004 */ int characterId;
    /* 0008 */ std::string name;
    /* 0020 */ int head;
    /* 0024 */ int headPalette;
    /* 0028 */ int job;
    /* 002C */ int sex;
    /* 0030 */ int positionId;
    /* 0034 */ std::string positionName;
    /* 004C */ int level;
    /* 0050 */ std::string memo;
    /* 0068 */ float expRatio;
    /* 006C */ int exp;
    /* 0070 */ int online;
    /* 0074 */ unsigned char lastOfflineTime[0x04];
  };

  struct Unknown00D4Entry {
    /* 0000 */ int ranking;
    /* 0004 */ int positionId;
    /* 0008 */ std::string name;
    /* 0020 */ int canInvite;
    /* 0024 */ int canExpel;
    /* 0028 */ int tax;
    /* 002C */ int canStorage;
  };

  struct RelatedGuild {
    /* 0000 */ int guildId;
    /* 0004 */ int relation;
    /* 0008 */ std::string name;
  };

  struct Unknown00ECEntry {
    /* 0000 */ int characterId;
    /* 0004 */ std::string name;
    /* 001C */ std::string accountName;
    /* 0034 */ std::string reason;
  };

  /* 0000 */ std::string guildName;
  /* 0018 */ std::string masterName;
  /* 0030 */ std::string subject;
  /* 0048 */ std::string notice;
  /* 0060 */ int guildLevel;
  /* 0064 */ int memberCount;
  /* 0068 */ int onlineMemberCount;
  /* 006C */ int maxMemberCount;
  /* 0070 */ int memberAverageLevel;
  /* 0074 */ std::string manageLand;
  /* 008C */ int exp;
  /* 0090 */ int nextExp;
  /* 0094 */ int emblem;
  /* 0098 */ int points;
  /* 009C */ int honor;
  /* 00A0 */ int virtue;
  /* 00A4 */ int guildTabBitmask;
  /* 00A8 */ std::uint32_t m_guildId;
  /* 00AC */ int canInvite;
  /* 00B0 */ int canExpel;
  /* 00B4 */ int isMaster;
  /* 00B8 */ int requestState;
  /* 00BC */ int infoRequested;
  /* 00C0 */ int membersRequested;
  /* 00C4 */ int positionsRequested;
  /* 00C8 */ int skillsRequested;
  /* 00CC */ int bansRequested;
  /* 00D0 */ std::uint32_t masterCharacterId;
  /* 00D4 */ std::list<Unknown00D4Entry> positions;
  /* 00DC */ std::list<Member> m_members;
  /* 00E4 */ std::list<RelatedGuild> relatedGuilds;
  /* 00EC */ std::list<Unknown00ECEntry> bannedMembers;
  /* 00F4 */ std::list<int> guildList;
  /* 00FC */ std::map<int, std::string> m_positions;
  /* 0104 */ std::map<int, GuildMemberManageEntry> pendingMemberPositions;
  /* 010C */ std::map<int, int> pendingPositionChanges;
  /* 0114 */ std::vector<int> agitIds;

  GuildInfo();
  void ChangeMemberPosition(int aid, int cid, int position);
  void ClearAgitIds();
  void ClearMembers();
  void RemoveAgitId(int value);
  void RemoveRelatedGuild(int padding_0000, int padding_0004);
  int GetMemberAccountId(const char* name);
  int GetAgitId(int index);
  int CountRelatedGuilds(int padding_0004);
  int GetAgitCount();
  void Init();
  bool HasRelatedGuild(int padding_0000);
  void UpdateMemberExpRatios();
  int DeleteMember(const char* name);
  void UpdateMemberStatus(int accountId, int online, int characterId, short padding_002C, short padding_0020, short padding_0024);
  void SetMemberOnline(int accountId, int online);
  void SetMasterCharacterId(int characterId);
  void UpdatePosition(Unknown00D4Entry entry);
  void AddAgitId(int value);
  void AddMember(const Member& member);
  void AddBannedMember(const Unknown00ECEntry& entry);
  int DeleteMemberByCharacterId(int characterId);
  void UpdateCharacterName(int characterId, const char* name);
};

extern GuildInfo g_guildInfo;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GuildInfo::Member) == 0x78);
static_assert(offsetof(GuildInfo::Member, accountId) == 0x00);
static_assert(offsetof(GuildInfo::Member, characterId) == 0x04);
static_assert(offsetof(GuildInfo::Member, positionId) == 0x30);
static_assert(offsetof(GuildInfo::Member, positionName) == 0x34);

static_assert(offsetof(GuildInfo::Member, expRatio) == 0x68);
static_assert(offsetof(GuildInfo::Member, online) == 0x70);

static_assert(sizeof(GuildInfo::Unknown00D4Entry) == 0x30);

static_assert(sizeof(GuildInfo::RelatedGuild) == 0x20);

static_assert(sizeof(GuildInfo::Unknown00ECEntry) == 0x4C);
static_assert(offsetof(GuildInfo::Unknown00ECEntry, characterId) == 0x00);
static_assert(offsetof(GuildInfo::Unknown00ECEntry, name) == 0x04);
static_assert(offsetof(GuildInfo::Unknown00ECEntry, accountName) == 0x1C);
static_assert(offsetof(GuildInfo::Unknown00ECEntry, reason) == 0x34);

static_assert(sizeof(GuildInfo) == 0x120);
static_assert(offsetof(GuildInfo, m_guildId) == 0xA8);
static_assert(offsetof(GuildInfo, m_members) == 0xDC);
static_assert(offsetof(GuildInfo, m_positions) == 0xFC);
#endif

#endif
