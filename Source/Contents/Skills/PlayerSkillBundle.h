#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_PLAYERSKILLBUNDLE_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_PLAYERSKILLBUNDLE_H

#include "Contents/Skills/SkillInfo.h"

#include <cstddef>
#include <functional>
#include <list>
#include <map>
#include <unordered_map>

#include "UI/Window/WindowManagerWindows.h"

struct SkillIdHash {
  std::size_t operator()(std::uint32_t id) const;
};

class CPlayerSkillBundle {
public:
  struct lcEXCInfo {
    lcEXCInfo();
    virtual ~lcEXCInfo();
    /* 0004 */ std::int32_t previous;
    /* 0008 */ std::int32_t target;
  };

  CPlayerSkillBundle();
  virtual ~CPlayerSkillBundle();
  void Add(const CSkillInfo& value);
  void Remove(std::uint32_t id);
  CSkillInfo* Find(std::uint32_t id);
  CSkillInfo Lookup(std::uint32_t id);
  bool FindIndexed(std::uint32_t id, CSkillInfo*& output);
  bool UpdatePending(const CSkillInfo& value);
  CSkillInfo Pending(std::uint32_t id);
  CSkillInfo* FindExtra(std::uint32_t id);
  void ClearExtra();
  bool RemoveExtra(std::uint32_t id);
  void AddExtra(const CSkillInfo& value);
  void NumberExtra();
  void AddGrouped(int group, const CSkillInfo& value);
  void ClearGroups();
  CSkillInfo* FindGrouped(std::uint32_t id);
  std::list<CSkillInfo>& Group(int group);
  void ForEachGrouped(std::function<bool(CSkillInfo*)> callback);
  std::size_t GroupSize(int group) const;
  int GroupedLevel(std::uint32_t id);
  bool HasHigherLevel(std::uint32_t id);
  void RecordLevelChange(const CSkillInfo& value);
  std::pair<bool, int> PreviousLevel(int id) const;
  void ApplyLevelChange(CSkillInfo& value);
  void RevertLevelChange(const CSkillInfo& current, CSkillInfo& value);
  void TakePreviousLevel(int& level, int id, int key);
  void ClearLevelChanges();
  void MarkLevelChanges();
  void ResetFirstLevelChange();
  void SetSkillValue(int id, int value);

  /* 0004 */ std::list<CSkillInfo> skills;
  /* 000C */ std::unordered_map<std::uint32_t, CSkillInfo*, SkillIdHash> index;
  /* 002C */ std::map<int, std::list<CSkillInfo>> groups;
  /* 0034 */ std::list<CSkillInfo> extra;
  /* 003C */ std::vector<std::uint32_t> extraIds;
  /* 0048 */ std::map<int, lcEXCInfo> levelChanges;
  /* 0050 */ std::map<int, bool> changed;
  /* 0058 */ std::vector<int> removals;
  /* 0064 */ std::vector<int> skillValues;
  /* 0070 */ std::list<CSkillInfo> pending;

private:
  CPlayerSkillBundle(const CPlayerSkillBundle&) = delete;
  CPlayerSkillBundle& operator=(const CPlayerSkillBundle&) = delete;
};

bool IsIndexedSkill(int id);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPlayerSkillBundle::lcEXCInfo) == 12);
static_assert(offsetof(CPlayerSkillBundle::lcEXCInfo, previous) == 0x4);
static_assert(offsetof(CPlayerSkillBundle::lcEXCInfo, target) == 0x8);
static_assert(sizeof(CPlayerSkillBundle) == 120);
#endif

#endif
