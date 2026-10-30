#ifndef RAGNAROK_SOURCE_CONTENTS_GUILDINFO_H
#define RAGNAROK_SOURCE_CONTENTS_GUILDINFO_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <vector>

class GuildInfo {
public:
  struct Member {
    /* 0000 */ int accountId;
    /* 0004 */ int characterId;
    /* 0008 */ std::string name;
    /* 0020 */ int unknown_0020;
    /* 0024 */ int unknown_0024;
    /* 0028 */ unsigned char unknown_0028[0x04];
    /* 002C */ int unknown_002C;
    /* 0030 */ int positionId;
    /* 0034 */ std::string positionName;
    /* 004C */ unsigned char unknown_004C[0x04];
    /* 0050 */ std::string unknown_0050;
    /* 0068 */ float expRatio;
    /* 006C */ int exp;
    /* 0070 */ int online;
    /* 0074 */ unsigned char unknown_0074[0x04];
  };

  struct Unknown00D4Entry {
    /* 0000 */ int unknown_0000;
    /* 0004 */ int unknown_0004;
    /* 0008 */ std::string unknown_0008;
    /* 0020 */ int unknown_0020;
    /* 0024 */ int unknown_0024;
    /* 0028 */ int unknown_0028;
    /* 002C */ int unknown_002C;
  };

  struct RelatedGuild {
    /* 0000 */ int unknown_0000;
    /* 0004 */ int unknown_0004;
    /* 0008 */ std::string unknown_0008;
  };

  /* 0000 */ std::string unknown_0000;
  /* 0018 */ std::string unknown_0018;
  /* 0030 */ std::string unknown_0030;
  /* 0048 */ std::string unknown_0048;
  /* 0060 */ int unknown_0060;
  /* 0064 */ int unknown_0064;
  /* 0068 */ int unknown_0068;
  /* 006C */ int unknown_006C;
  /* 0070 */ int unknown_0070;
  /* 0074 */ std::string unknown_0074;
  /* 008C */ int unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ int unknown_0094;
  /* 0098 */ int unknown_0098;
  /* 009C */ int unknown_009C;
  /* 00A0 */ int unknown_00A0;
  /* 00A4 */ int unknown_00A4;
  /* 00A8 */ int m_guildId;
  /* 00AC */ int unknown_00AC;
  /* 00B0 */ int unknown_00B0;
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ std::list<Unknown00D4Entry> unknown_00D4;
  /* 00DC */ std::list<Member> m_members;
  /* 00E4 */ std::list<RelatedGuild> unknown_00E4;
  /* 00EC */ std::list<int> unknown_00EC;
  /* 00F4 */ std::list<int> unknown_00F4;
  /* 00FC */ std::map<int, std::string> m_positions;
  /* 0104 */ std::map<int, int> unknown_0104;
  /* 010C */ std::map<int, int> unknown_010C;
  /* 0114 */ std::vector<int> unknown_0114;

  void ChangeMemberPosition(int aid, int cid, int position);
  void ClearUnknown0114();
  void ClearMembers();
  void RemoveUnknown0114(int value);
  void RemoveRelatedGuild(int unknown_0000, int unknown_0004);
  int GetMemberAccountId(const char* name);
  int GetUnknown0114(int index);
  int CountRelatedGuilds(int unknown_0004);
  int GetUnknown0114Count();
  void Init();
  bool HasRelatedGuild(int unknown_0000);
  void UpdateMemberExpRatios();
  int DeleteMember(const char* name);
  void UpdateMemberStatus(int accountId, int online, int characterId, short unknown_002C, short unknown_0020, short unknown_0024);
  void SetMemberOnline(int accountId, int online);
  void SetMasterCharacterId(int characterId);
  void UpdateUnknown00D4(Unknown00D4Entry entry);
};

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

static_assert(sizeof(GuildInfo) == 0x120);
static_assert(offsetof(GuildInfo, m_guildId) == 0xA8);
static_assert(offsetof(GuildInfo, m_members) == 0xDC);
static_assert(offsetof(GuildInfo, m_positions) == 0xFC);
#endif

#endif
