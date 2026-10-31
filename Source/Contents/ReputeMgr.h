#ifndef RAGNAROK_SOURCE_CONTENTS_REPUTEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_REPUTEMGR_H

#include "Contents/ReputeRecords.h"
#include "Script/LuaInstance.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

enum ReputeVisibility : std::int64_t {
  REPUTE_VISIBLE = 0,
  REPUTE_HIDDEN = 1,
  REPUTE_VISIBLE_WITH_POINT = 2,
};

struct ReputeInfo {
  /* 0000 */ std::string name;
  /* 0018 */ std::int64_t unknown_0018;
  /* 0020 */ std::int64_t unknown_0020;
  /* 0028 */ ReputeVisibility visibility;
  /* 0030 */ std::int64_t point;
  /* 0038 */ bool updated;
};

struct ReputeGroup {
  /* 0000 */ std::string name;
  /* 0018 */ std::vector<std::uint64_t> reputes;
};

class CReputeMgr : public Singleton<CReputeMgr> {
public:
  CReputeMgr();
  std::vector<std::string> GetGroupNames() const;
  std::vector<const ReputeInfo*> GetGroupReputes(const std::string& groupName) const;
  std::vector<const ReputeInfo*> GetAllReputes() const;
  bool sub_60DBF0();
  bool sub_60DC70(std::map<std::uint64_t, ReputeGroup>& groups);
  bool sub_60E090(std::map<std::uint64_t, ReputeInfo>& infos);
  void OpenRepute(std::uint64_t groupId, std::uint64_t reputeId);
  void SetPoint(std::uint64_t id, std::int64_t point);
  std::vector<const ReputeInfo*> FindReputes(const std::string& keyword) const;
  void SetPointsReceived(bool received);

  struct GroupInserter {
    /* 0000 */ CReputeMgr* manager;
    /* 0004 */ std::map<std::uint64_t, ReputeGroup>* dest;

    void operator()(const std::pair<const int, ReputeGroupRecord>& entry) const;
  };

  struct InfoInserter {
    /* 0000 */ CReputeMgr* manager;
    /* 0004 */ std::map<std::uint64_t, ReputeInfo>* dest;

    void operator()(const std::pair<const int, ReputeInfoRecord>& entry) const;
  };

protected:
  /* 0004 */ std::map<std::uint64_t, ReputeInfo> m_infos;
  /* 000C */ std::map<std::uint64_t, ReputeGroup> m_groups;
  /* 0014 */ bool m_waitingForPoints = true;
  /* 0018 */ CLuaInstance m_lua;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ReputeInfo) == 0x40);
static_assert(offsetof(ReputeInfo, visibility) == 0x28);
static_assert(offsetof(ReputeInfo, point) == 0x30);
static_assert(offsetof(ReputeInfo, updated) == 0x38);
static_assert(sizeof(ReputeGroup) == 0x24);
static_assert(sizeof(CReputeMgr) == 0x20);
#endif

#endif
