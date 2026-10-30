#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_RUNESYSTEM_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_RUNESYSTEM_H

#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct lua_State;

// VTABLE: vt_00501
class Decom_ItemInfo {
public:
  Decom_ItemInfo(int itemId, int minCount, int maxCount, int percent) : m_itemId(itemId), m_minCount(minCount), m_maxCount(maxCount), m_percent(percent) {
  }

  /* 0004 */ int m_itemId;
  /* 0008 */ int m_minCount;
  /* 000C */ int m_maxCount;
  /* 0010 */ int m_percent;

  virtual ~Decom_ItemInfo();
};

// VTABLE: vt_00499
class RuneData {
public:
  /* 0008 */ int m_kind;
  /* 000C */ bool m_active;
  /* 0010 */ std::int64_t m_id;
  /* 0018 */ std::string m_name;
  /* 0030 */ std::string m_resName;
  /* 0048 */ int m_tagId;
  /* 004C */ int m_activeType;

  RuneData();

  virtual ~RuneData() {
  }

  bool IsSame(const RuneData& other) const;
  std::int64_t GetId() const;
  bool IsActive() const;
  const char* GetName() const;
  const char* GetResName() const;
  int GetTagId() const;
  int GetActiveType() const;
};

#pragma pack(push, 1)
struct RuneSetReward {
  /* 0000 */ std::int64_t values[7];
  /* 0038 */ bool received;
};
#pragma pack(pop)

// VTABLE: vt_00500
class RuneSetData : public RuneData {
public:
  /* 0050 */ std::vector<RuneData*> m_slots;
  /* 005C */ std::vector<std::string> m_descriptions[6];
  /* 00A4 */ int unknown_00A4;
  /* 00A8 */ int m_gradeList[15];
  /* 00E4 */ int m_grade;
  /* 00E8 */ int m_gradeLevel;
  /* 00EC */ int m_gradeType;
  /* 00F0 */ int m_gradeFailType;
  /* 00F4 */ int m_rewardStep;
  /* 00F8 */ RuneSetReward m_reward;

  RuneSetData() : m_grade(0), m_gradeLevel(0), m_gradeType(0), m_gradeFailType(0), m_rewardStep(0) {
    this->m_kind = 0;
    std::memset(this->m_gradeList, 0, sizeof(this->m_gradeList));
    std::memset(&this->m_reward, 0, sizeof(this->m_reward));
  }

  ~RuneSetData() override {
  }

  int FindLastReward(int count) const;
  int CountSlots() const;
  int CountActiveSlots() const;
  int GetGradeValue() const;
  bool IsRewardStep(int step) const;
};

// VTABLE: vt_00502
class CTagData {
public:
  /* 0004 */ int m_id;
  /* 0008 */ std::string m_name;
  /* 0020 */ bool m_locked;
  /* 0024 */ int m_activeCount[2];
  /* 002C */ std::vector<RuneData*> m_runes[2];

  CTagData(int id, std::string name) {
    this->m_id = id;
    this->m_name = name;
    this->m_locked = false;
    this->m_activeCount[1] = 0;
    this->m_activeCount[0] = 0;
  }

  virtual ~CTagData() {
    for (int type = 0; type < 2; ++type) {
      for (RuneData* rune : this->m_runes[type]) {
        if (rune) {
          delete rune;
        }
      }
    }
  }

  void UpdateActiveCount(int type);
  int GetRuneCount(int type) const;
  RuneData* FindRune(int type, std::int64_t id, unsigned int* index);
};

// VTABLE: vt_00503
class CRuneSystemMgr {
public:
  static bool Create();
  static void Destroy();

  /* 0004 */ std::pair<int, int> m_emptyItem;
  /* 000C */ bool m_initialized;
  /* 000D */ bool m_requestPending;
  /* 0010 */ int m_selectTag;
  /* 0014 */ RuneSetData* m_curRuneSet;
  /* 0018 */ int m_curSlotCount;
  /* 001C */ std::vector<CTagData*> m_tags;
  /* 0028 */ std::map<int, std::vector<int>> m_gradeTypes;
  /* 0030 */ std::map<int, std::vector<int>> m_gradeFailTypes;
  /* 0038 */ std::map<int, std::vector<std::pair<int, int>>> m_itemLists;
  /* 0040 */ std::vector<int> m_decomItems;
  /* 004C */ std::map<int, std::vector<Decom_ItemInfo>> m_decomTypes;
  /* 0054 */ std::map<int, std::pair<int, int>> m_itemDecoms;
  /* 005C */ std::vector<int> m_ownedDecomItems;

  CRuneSystemMgr();

  virtual ~CRuneSystemMgr();

  void AddRune(int tagId, int type, std::int64_t id, const std::string& resName, int activeType);
  int GetTagCount() const;
  int GetOwnedDecomItemCount() const;
  bool IsInitialized() const;
  CTagData* GetTag(int tagId);
  CTagData* GetTagByIndex(int index);
  int GetDecomItem(unsigned int index) const;
  int GetOwnedDecomItem(unsigned int index) const;
  RuneSetData* GetCurRuneSet() const;
  int GetCurSlotCount() const;
  int GetGradeFailValue(RuneSetData* runeSet);
  const std::pair<int, int>& GetItemListEntry(int listId, unsigned int index);
  int GetItemListCount(int listId);
  RuneData* GetCurrentRune(int type, int index);
  int GetGradeValue(RuneSetData* runeSet);
  RuneData* FindRune(int tagId, int type, std::int64_t id, unsigned int* index);
  int GetSelectTag() const;
  std::vector<Decom_ItemInfo>* GetDecomList(int itemId, int which);
  bool IsRequestPending() const;
  bool LoadScripts();
  void Reset();
  void ClearCurRuneSet();
  void LoadDefaultTag();
  bool HasRequiredItems(int listId);
  int SendRequest(int packetId, RuneData* rune, int value);
  void SetCurSlotCount(const int& count);
  void SetSelectTag(int tagId);
  void SetRequestPending(bool pending);
  int SetRuneGrade(int tagId, int type, std::int64_t id, int grade, int gradeLevel);
  bool SetRewardStep(int tagId, std::int64_t id, int step);
};

extern CRuneSystemMgr* g_runeSystemMgr;

int AddGradeType(lua_State* state);
int AddGradeTypeFail(lua_State* state);
int AddItemListTableType(lua_State* state);
int AddRuneData(lua_State* state);
int AddRuneSetReward(lua_State* state);
int AddRuneDescription(lua_State* state);
int AddRuneGradeList(lua_State* state);
int AddRuneSlotCollectionList(lua_State* state);
int AddTag(lua_State* state);
int AddDecomItemNum(lua_State* state);
int AddItemDecom(lua_State* state);
int AddItemDecomType(lua_State* state);
int SetRuneDisplayName(lua_State* state);
int SetRuneUpGradeTable(lua_State* state);
int SetRuneSystemInfo(lua_State* state);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(Decom_ItemInfo) == 0x14);
static_assert(sizeof(RuneData) == 0x50);
static_assert(sizeof(RuneSetReward) == 0x39);
static_assert(sizeof(RuneSetData) == 0x138);
static_assert(sizeof(CTagData) == 0x44);
static_assert(sizeof(CRuneSystemMgr) == 0x68);
#endif

#endif
