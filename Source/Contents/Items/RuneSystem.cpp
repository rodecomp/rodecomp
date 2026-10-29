#include "Contents/Items/RuneSystem.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lua.h>
}

#include <format>
#include <new>

// GLOBAL: gl_24845
CRuneSystemMgr* g_runeSystemMgr;

namespace {
  struct RuneTagPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint16_t tagId;
  };

  struct RuneItemPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint16_t tagId;
    /* 0004 */ int runeId;
  };

  struct RuneSelectPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint16_t tagId;
    /* 0004 */ int runeId;
    /* 0008 */ std::uint8_t value;
  };

  struct RuneIdPacket {
    /* 0000 */ std::uint16_t id;
    /* 0004 */ int runeId;
  };

  struct RuneHeaderPacket {
    /* 0000 */ std::uint16_t id;
  };
}

// OFFSET: 0x00764C30
// FUNCTION: fn_08098
CRuneSystemMgr::CRuneSystemMgr() : m_emptyItem(), m_requestPending(false), m_selectTag(0), m_curRuneSet(nullptr), m_curSlotCount(0) {
  this->m_initialized = true;
}

// OFFSET: 0x00764DE0
// FUNCTION: fn_08099
RuneData::RuneData() : m_kind(1), m_active(false), m_id(0), m_name("NoName"), m_resName("NoData"), m_tagId(0), m_activeType(1) {
}

// OFFSET: 0x007657D0
// FUNCTION: fn_08104
CRuneSystemMgr::~CRuneSystemMgr() {
  for (CTagData*& tag : this->m_tags) {
    if (tag) {
      delete tag;
    }

    tag = nullptr;
  }

  this->m_tags.clear();
}

// OFFSET: 0x007659C0
// FUNCTION: fn_08105
Decom_ItemInfo::~Decom_ItemInfo() {
}

// OFFSET: 0x00765B10
// FUNCTION: fn_08106
bool RuneData::IsSame(const RuneData& other) const {
  return this->m_id == other.m_id && this->m_tagId == other.m_tagId;
}

// OFFSET: 0x007660C0
// FUNCTION: fn_08112
void CRuneSystemMgr::AddRune(int tagId, int type, std::int64_t id, const std::string& resName, int activeType) {
  CTagData* tag = this->GetTag(tagId);
  if (!tag) {
    return;
  }

  RuneData* rune;
  if (type == 1) {
    rune = new RuneData();
  } else {
    rune = new RuneSetData();
  }

  rune->m_id = id;
  rune->m_tagId = tagId;
  rune->m_resName = resName;
  rune->m_activeType = activeType;
  tag->m_runes[type].push_back(rune);
}

// OFFSET: 0x00766920
// FUNCTION: fn_08113
void CTagData::UpdateActiveCount(int type) {
  this->m_activeCount[type] = 0;
  for (RuneData* rune : this->m_runes[type]) {
    if (rune) {
      this->m_activeCount[type] += rune->m_active;
    }
  }
}

// OFFSET: 0x00766970
// FUNCTION: fn_08114
bool CRuneSystemMgr::Create() {
  if (g_runeSystemMgr) {
    return false;
  }

  g_runeSystemMgr = new (std::nothrow) CRuneSystemMgr();
  if (!g_runeSystemMgr) {
    return false;
  }

  if (g_runeSystemMgr->m_initialized) {
    return true;
  }

  delete g_runeSystemMgr;
  g_runeSystemMgr = nullptr;
  return false;
}

// OFFSET: 0x00766A10
// FUNCTION: fn_08115
void CRuneSystemMgr::Destroy() {
  if (g_runeSystemMgr) {
    delete g_runeSystemMgr;
    g_runeSystemMgr = nullptr;
  }
}

// OFFSET: 0x00766A30
// FUNCTION: fn_08116
int RuneSetData::FindLastReward(int count) const {
  if (count < 8) {
    for (int step = count - 1; step > 0; --step) {
      if (this->m_reward.values[step] != 0) {
        return step;
      }
    }
  }

  return -1;
}

// OFFSET: 0x00766A70
// FUNCTION: fn_08117
int CRuneSystemMgr::GetTagCount() const {
  return static_cast<int>(this->m_tags.size());
}

// OFFSET: 0x00766A80
// FUNCTION: fn_08118
int CRuneSystemMgr::GetOwnedDecomItemCount() const {
  return static_cast<int>(this->m_ownedDecomItems.size());
}

// OFFSET: 0x00766B10
// FUNCTION: fn_08119
bool CRuneSystemMgr::IsInitialized() const {
  return this->m_initialized;
}

// OFFSET: 0x00766B20
// FUNCTION: fn_08120
CTagData* CRuneSystemMgr::GetTag(int tagId) {
  if (tagId == -1) {
    tagId = this->m_selectTag;
  }

  if (tagId > 0) {
    for (CTagData* tag : this->m_tags) {
      if (tagId == tag->m_id) {
        return tag;
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00766B60
// FUNCTION: fn_08121
CTagData* CRuneSystemMgr::GetTagByIndex(int index) {
  if (index < 0 || static_cast<unsigned int>(index) >= this->m_tags.size()) {
    return nullptr;
  }

  return this->m_tags[index];
}

// OFFSET: 0x00766B90
// FUNCTION: fn_08122
int RuneSetData::CountSlots() const {
  int count = 0;
  for (RuneData* rune : this->m_slots) {
    if (rune) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00766C70
// FUNCTION: fn_08123
int RuneSetData::CountActiveSlots() const {
  int count = 0;
  for (RuneData* rune : this->m_slots) {
    if (rune) {
      count += rune->m_active;
    }
  }

  return count;
}

// OFFSET: 0x00766CA0
// FUNCTION: fn_08124
const char* RuneData::GetName() const {
  return this->m_name.c_str();
}

// OFFSET: 0x00766CB0
// FUNCTION: fn_08125
int CRuneSystemMgr::GetDecomItem(unsigned int index) const {
  if (index < this->m_decomItems.size()) {
    return this->m_decomItems[index];
  }

  return 0;
}

// OFFSET: 0x00766CE0
// FUNCTION: fn_08126
int CRuneSystemMgr::GetOwnedDecomItem(unsigned int index) const {
  if (index < this->m_ownedDecomItems.size()) {
    return this->m_ownedDecomItems[index];
  }

  return 0;
}

// OFFSET: 0x00766DB0
// FUNCTION: fn_08127
RuneSetData* CRuneSystemMgr::GetCurRuneSet() const {
  return this->m_curRuneSet;
}

// OFFSET: 0x00766DC0
// FUNCTION: fn_08128
int CRuneSystemMgr::GetCurSlotCount() const {
  return this->m_curSlotCount;
}

// OFFSET: 0x00766DD0
// FUNCTION: fn_08129
int CRuneSystemMgr::GetGradeFailValue(RuneSetData* runeSet) {
  if (runeSet) {
    const auto found = this->m_gradeFailTypes.find(runeSet->m_gradeFailType);
    if (found != this->m_gradeFailTypes.end() && static_cast<unsigned int>(runeSet->m_grade) < found->second.size()) {
      return found->second[runeSet->m_grade];
    }
  }

  return 0;
}

// OFFSET: 0x00766E40
// FUNCTION: fn_08130
std::int64_t RuneData::GetId() const {
  return this->m_id;
}

// OFFSET: 0x00766E50
// FUNCTION: fn_08131
bool RuneData::IsActive() const {
  return this->m_active;
}

// OFFSET: 0x00766E60
// FUNCTION: fn_08132
const std::pair<int, int>& CRuneSystemMgr::GetItemListEntry(int listId, unsigned int index) {
  const auto found = this->m_itemLists.find(listId);
  if (found == this->m_itemLists.end() || index >= found->second.size()) {
    return this->m_emptyItem;
  }

  return found->second[index];
}

// OFFSET: 0x00766EB0
// FUNCTION: fn_08133
int CRuneSystemMgr::GetItemListCount(int listId) {
  const auto found = this->m_itemLists.find(listId);
  if (found != this->m_itemLists.end()) {
    return static_cast<int>(found->second.size());
  }

  return 0;
}

// OFFSET: 0x00766F00
// FUNCTION: fn_08134
RuneData* CRuneSystemMgr::GetCurrentRune(int type, int index) {
  CTagData* tag = this->GetTag(-1);
  if (!tag || index < 0) {
    return nullptr;
  }

  if (static_cast<unsigned int>(index) < tag->m_runes[type].size()) {
    return tag->m_runes[type][index];
  }

  return nullptr;
}

// OFFSET: 0x00766F60
// FUNCTION: fn_08135
int CTagData::GetRuneCount(int type) const {
  return static_cast<int>(this->m_runes[type].size());
}

// OFFSET: 0x00766F80
// FUNCTION: fn_08136
int CRuneSystemMgr::GetGradeValue(RuneSetData* runeSet) {
  if (runeSet) {
    const auto found = this->m_gradeTypes.find(runeSet->m_gradeType);
    if (found != this->m_gradeTypes.end()) {
      const unsigned int grade = runeSet->m_grade;
      if (grade < found->second.size()) {
        const auto fail = this->m_gradeFailTypes.find(runeSet->m_gradeFailType);
        if (fail != this->m_gradeFailTypes.end() && grade < fail->second.size()) {
          return runeSet->m_gradeLevel * fail->second[grade] + found->second[grade];
        }

        return found->second[grade];
      }
    }
  }

  return 0;
}

// OFFSET: 0x00767070
// FUNCTION: fn_08137
RuneData* CRuneSystemMgr::FindRune(int tagId, int type, std::int64_t id, unsigned int* index) {
  CTagData* tag = this->GetTag(tagId);
  if (!tag) {
    return nullptr;
  }

  return tag->FindRune(type, id, index);
}

// OFFSET: 0x007670C0
// FUNCTION: fn_08138
RuneData* CTagData::FindRune(int type, std::int64_t id, unsigned int* index) {
  if (type == 0 || type == 1) {
    std::vector<RuneData*>& runes = this->m_runes[type];
    const unsigned int count = static_cast<unsigned int>(runes.size());
    for (unsigned int i = 0; i < count; ++i) {
      RuneData* rune = runes[i];
      if (rune && id == rune->m_id) {
        if (index) {
          *index = i;
        }

        return runes[i];
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00767130
// FUNCTION: fn_08139
int CRuneSystemMgr::GetSelectTag() const {
  return this->m_selectTag;
}

// OFFSET: 0x00767140
// FUNCTION: fn_08140
int RuneData::GetTagId() const {
  return this->m_tagId;
}

// OFFSET: 0x00767150
// FUNCTION: fn_08141
int RuneData::GetActiveType() const {
  return this->m_activeType;
}

// OFFSET: 0x00767160
// FUNCTION: fn_08142
int RuneSetData::GetGradeValue() const {
  if (this->m_grade < 15) {
    return this->m_gradeList[this->m_grade];
  }

  return 0;
}

// OFFSET: 0x00767180
// FUNCTION: fn_08143
std::vector<Decom_ItemInfo>* CRuneSystemMgr::GetDecomList(int itemId, int which) {
  const auto decom = this->m_itemDecoms.find(itemId);
  const std::pair<int, int>* types = decom != this->m_itemDecoms.end() ? &decom->second : nullptr;
  if (!types) {
    return nullptr;
  }

  const int typeId = which == 0 ? types->first : types->second;
  const auto found = this->m_decomTypes.find(typeId);
  if (found == this->m_decomTypes.end()) {
    return nullptr;
  }

  return &found->second;
}

// OFFSET: 0x00767230
// FUNCTION: fn_08144
bool CRuneSystemMgr::IsRequestPending() const {
  return this->m_requestPending;
}

// OFFSET: 0x00767240
// FUNCTION: fn_08145
const char* RuneData::GetResName() const {
  return this->m_resName.c_str();
}

// OFFSET: 0x00767D20
// FUNCTION: fn_08147
void CRuneSystemMgr::Reset() {
  this->LoadDefaultTag();

  for (CTagData* tag : this->m_tags) {
    tag->m_locked = false;
    for (int type = 0; type < 2; ++type) {
      for (RuneData* rune : tag->m_runes[type]) {
        if (rune) {
          rune->m_active = false;
          if (type == 0) {
            RuneSetData* runeSet = dynamic_cast<RuneSetData*>(rune);
            runeSet->m_grade = 0;
            runeSet->m_gradeLevel = 0;
            runeSet->m_rewardStep = 0;
          }
        }
      }
    }
  }

  this->m_curRuneSet = nullptr;
}

// OFFSET: 0x00767E60
// FUNCTION: fn_08148
void CRuneSystemMgr::ClearCurRuneSet() {
  this->m_curRuneSet = nullptr;
}

// OFFSET: 0x00767E70
// FUNCTION: fn_08149
void CRuneSystemMgr::LoadDefaultTag() {
  if (!this->m_tags.empty()) {
    this->m_selectTag = this->m_tags.front()->m_id;
  }

  LuaScript script;
  script.Open();
  if (script.Load("System\\Rune\\runesystemInfo", false, true)) {
    int tagId = 0;
    if (script.Call("GetDefultTag", ">d", &tagId) && !this->m_tags.empty()) {
      if (this->GetTag(tagId)) {
        this->m_selectTag = tagId;
      } else {
        this->m_selectTag = this->m_tags.front()->m_id;
      }
    }
  }

  script.Close();
}

// OFFSET: 0x00767FA0
// FUNCTION: fn_08150
bool RuneSetData::IsRewardStep(int step) const {
  if (step == 0) {
    return this->m_rewardStep == 0;
  }

  return this->m_rewardStep == this->FindLastReward(step + 1);
}

// OFFSET: 0x00767FF0
// FUNCTION: fn_08151
bool CRuneSystemMgr::HasRequiredItems(int listId) {
  const auto found = this->m_itemLists.find(listId);
  if (found == this->m_itemLists.end()) {
    return false;
  }

  for (const std::pair<int, int>& entry : found->second) {
    const ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, entry.first, 0);
    if (item.quantity < entry.second) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00768240
// FUNCTION: fn_08152
int CRuneSystemMgr::SendRequest(int packetId, RuneData* rune, int value) {
  if (this->m_requestPending) {
    return 0;
  }

  int listId;
  switch (packetId) {
  case 0x0BCB: {
    CTagData* tag = this->GetTag(value);
    if (!tag) {
      return 4;
    }

    if (tag->m_locked) {
      return 0;
    }

    RuneTagPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    packet.tagId = static_cast<std::uint16_t>(tag->m_id);
    GetRagConnection().Send(g_packetLengths.FixedLength(packetId), &packet);
    return 1;
  }
  case 0x0BCE:
  case 0x0BD0:
    listId = rune->m_activeType;
    break;
  case 0x0BD2:
    listId = static_cast<RuneSetData*>(rune)->GetGradeValue();
    break;
  case 0x0BD4: {
    RuneHeaderPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    GetRagConnection().Send(g_packetLengths.FixedLength(packetId), &packet);
    return 1;
  }
  case 0x0BD6: {
    RuneIdPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    packet.runeId = static_cast<int>(rune->m_id);
    GetRagConnection().Send(g_packetLengths.FixedLength(packetId), &packet);
    return 1;
  }
  case 0x0C16: {
    RuneSelectPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    packet.tagId = static_cast<std::uint16_t>(rune->m_tagId);
    packet.runeId = static_cast<int>(rune->m_id);
    packet.value = static_cast<std::uint8_t>(value);
    GetRagConnection().Send(g_packetLengths.FixedLength(packetId), &packet);
    return 1;
  }
  default:
    return 1;
  }

  if (!this->HasRequiredItems(listId)) {
    return 0;
  }

  RuneItemPacket packet;
  packet.id = static_cast<std::uint16_t>(packetId);
  packet.tagId = static_cast<std::uint16_t>(rune->m_tagId);
  packet.runeId = static_cast<int>(rune->m_id);
  GetRagConnection().Send(g_packetLengths.FixedLength(packetId), &packet);
  this->m_requestPending = true;
  return 1;
}

// OFFSET: 0x00768780
// FUNCTION: fn_08154
void CRuneSystemMgr::SetCurSlotCount(const int& count) {
  this->m_curSlotCount = count;
}

// OFFSET: 0x007688A0
// FUNCTION: fn_08156
void CRuneSystemMgr::SetSelectTag(int tagId) {
  if (this->m_tags.empty()) {
    return;
  }

  if (this->GetTag(tagId)) {
    this->m_selectTag = tagId;
    return;
  }

  this->m_selectTag = this->m_tags.front()->m_id;
}

// OFFSET: 0x00768920
// FUNCTION: fn_08159
void CRuneSystemMgr::SetRequestPending(bool pending) {
  this->m_requestPending = pending;
}

// OFFSET: 0x00768960
// FUNCTION: fn_08160
int CRuneSystemMgr::SetRuneGrade(int tagId, int type, std::int64_t id, int grade, int gradeLevel) {
  CTagData* tag = this->GetTag(tagId);
  if (!tag) {
    return -1;
  }

  RuneData* rune = tag->FindRune(type, id, nullptr);
  if (!rune) {
    return -1;
  }

  rune->m_active = true;
  if (type == 0) {
    RuneSetData* runeSet = static_cast<RuneSetData*>(rune);
    const int oldGrade = runeSet->m_grade;
    const int oldLevel = runeSet->m_gradeLevel;
    runeSet->m_grade = grade;
    runeSet->m_gradeLevel = gradeLevel;
    if (oldGrade < grade) {
      return 2;
    }

    if (oldLevel < gradeLevel) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00768A00
// FUNCTION: fn_08161
bool CRuneSystemMgr::SetRewardStep(int tagId, std::int64_t id, int step) {
  CTagData* tag = this->GetTag(tagId);
  if (!tag) {
    return false;
  }

  std::vector<RuneData*>& runes = tag->m_runes[0];
  const unsigned int count = static_cast<unsigned int>(runes.size());
  for (unsigned int i = 0; i < count; ++i) {
    RuneData* rune = runes[i];
    if (rune && id == rune->m_id) {
      static_cast<RuneSetData*>(rune)->m_rewardStep = step;
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00769240
// FUNCTION: fn_08173
int AddGradeType(lua_State* state) {
  const int gradeId = static_cast<int>(lua_tonumber(state, -3));
  const double valueIndex = lua_tonumber(state, -2);
  const double value = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d", gradeId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ValIndex:%d", static_cast<int>(valueIndex)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d ValData %d", gradeId, static_cast<int>(value)).c_str());
    return 2;
  }

  const int index = static_cast<int>(valueIndex) - 1;
  std::map<int, std::vector<int>>& gradeTypes = g_runeSystemMgr->m_gradeTypes;
  if (gradeTypes[gradeId].size() < 15) {
    gradeTypes[gradeId].resize(15);
  }

  if (index < 15) {
    gradeTypes[gradeId][index] = static_cast<int>(value);
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x00769440
// FUNCTION: fn_08174
int AddGradeTypeFail(lua_State* state) {
  const int gradeId = static_cast<int>(lua_tonumber(state, -3));
  const double valueIndex = lua_tonumber(state, -2);
  const double value = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d", gradeId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d", static_cast<int>(valueIndex)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d ValData %d", gradeId, static_cast<int>(value)).c_str());
    return 2;
  }

  const int index = static_cast<int>(valueIndex) - 1;
  std::map<int, std::vector<int>>& gradeFailTypes = g_runeSystemMgr->m_gradeFailTypes;
  if (gradeFailTypes[gradeId].size() < 15) {
    gradeFailTypes[gradeId].resize(15);
  }

  if (index < 15) {
    gradeFailTypes[gradeId][index] = static_cast<int>(value);
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x00769640
// FUNCTION: fn_08175
int AddItemListTableType(lua_State* state) {
  const int listId = static_cast<int>(lua_tonumber(state, -4));
  const double listIndex = lua_tonumber(state, -3);
  const double itemValue = lua_tonumber(state, -2);
  const double countValue = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListID:%d", listId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListIndex:%d", static_cast<int>(listIndex)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListID:%d ItemID:%d", listId, static_cast<int>(itemValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListID:%d Num%d", listId, static_cast<int>(countValue)).c_str());
    return 2;
  }

  int itemId = static_cast<int>(itemValue);
  int count = static_cast<int>(countValue);
  g_runeSystemMgr->m_itemLists[listId].emplace_back(itemId, count);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x007698F0
// FUNCTION: fn_08176
int AddRuneData(lua_State* state) {
  const double tagValue = lua_tonumber(state, -5);
  const double typeValue = lua_tonumber(state, -4);
  const int runeSetId = static_cast<int>(lua_tonumber(state, -3));
  const std::string resName(lua_tostring(state, -2));
  const double activeType = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Tag:%d", static_cast<int>(tagValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneType:%d", static_cast<int>(typeValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d", runeSetId).c_str());
    return 2;
  }

  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d ResName%s", runeSetId, resName.c_str()).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d ActiveType%d", runeSetId, static_cast<int>(activeType)).c_str());
    return 2;
  }

  g_runeSystemMgr->AddRune(static_cast<int>(tagValue), static_cast<int>(typeValue) - 1, runeSetId, resName, static_cast<int>(activeType));
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x00769B80
// FUNCTION: fn_08177
int AddRuneSetReward(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -9));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -8));

  if (!lua_isnumber(state, -9)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, std::format("TagID:{}", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -8)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, std::format("RuneSetID:{}", runeSetId).c_str());
    return 2;
  }

  RuneData* rune = g_runeSystemMgr->FindRune(tagId, 0, runeSetId, nullptr);
  RuneSetReward* reward = rune ? &static_cast<RuneSetData*>(rune)->m_reward : nullptr;
  if (!reward) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, std::format("ErrData TagID:{}RuneSetID:{}", tagId, runeSetId).c_str());
    return 2;
  }

  reward->values[0] = 1;
  for (int index = -7; index < -1; ++index) {
    const int itemId = static_cast<int>(lua_tonumber(state, index));
    reward->values[index + 8] = itemId;
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("ErrData TagID:{} RuneSetID:{} itemID:{}", tagId, runeSetId, itemId).c_str());
      return 2;
    }
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x00769EE0
// FUNCTION: fn_08178
int AddRuneDescription(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -4));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -3));
  const double indexValue = lua_tonumber(state, -2);
  const std::string description(lua_tostring(state, -1));

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Description Tag:%d", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Description ID:%d", runeSetId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Description ID:%d itemID:%d", runeSetId, static_cast<int>(indexValue)).c_str());
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Description ID:%d Description%s", runeSetId, description.c_str()).c_str());
    return 2;
  }

  const std::string text = description;
  RuneData* rune = g_runeSystemMgr->FindRune(tagId, 0, runeSetId, nullptr);
  if (rune) {
    const unsigned int index = static_cast<int>(indexValue) - 1;
    if (index < 6) {
      static_cast<RuneSetData*>(rune)->m_descriptions[index].push_back(text);
    } else {
      MessageBoxA(hWnd, FormatOwnedString("AddRune_Description 인댁스 오류 %d", index).c_str(), "LuaLoad", MB_OK);
    }
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076A240
// FUNCTION: fn_08179
int AddRuneGradeList(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -4));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -3));
  const double gradeValue = lua_tonumber(state, -2);
  const double countValue = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("GradeList Tag:%d", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("GradeList ID:%d", runeSetId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("GradeList ID:%d itemID:%d", runeSetId, static_cast<int>(gradeValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("GradeList ID:%d itemNum:%d]", runeSetId, static_cast<int>(countValue)).c_str());
    return 2;
  }

  RuneData* rune = g_runeSystemMgr->FindRune(tagId, 0, runeSetId, nullptr);
  if (rune) {
    const int grade = static_cast<int>(gradeValue) - 1;
    if (grade < 15) {
      static_cast<RuneSetData*>(rune)->m_gradeList[grade] = static_cast<int>(countValue);
    }
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076A470
// FUNCTION: fn_08180
int AddRuneSlotCollectionList(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -3));
  const double runeSetValue = lua_tonumber(state, -2);
  const double runeValue = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SlotCollectionList Tag:%d", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SlotCollectionList ID:%d", static_cast<int>(runeSetValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SlotCollectionList ID:%d RuneID%d", static_cast<int>(runeSetValue), static_cast<int>(runeValue)).c_str());
    return 2;
  }

  CTagData* tag = g_runeSystemMgr->GetTag(tagId);
  if (tag) {
    RuneData* runeSet = tag->FindRune(0, static_cast<int>(runeSetValue), nullptr);
    if (runeSet) {
      RuneData* rune = g_runeSystemMgr->FindRune(tagId, 1, static_cast<int>(runeValue), nullptr);
      static_cast<RuneSetData*>(runeSet)->m_slots.push_back(rune);
    }
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076A690
// FUNCTION: fn_08181
int AddTag(lua_State* state) {
  const double tagValue = lua_tonumber(state, -2);
  const std::string name(lua_tostring(state, -1));

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d", static_cast<int>(tagValue)).c_str());
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d ResName%s", static_cast<int>(tagValue), name.c_str()).c_str());
    return 2;
  }

  g_runeSystemMgr->m_tags.push_back(new CTagData(static_cast<int>(tagValue), name));
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076A940
// FUNCTION: fn_08182
int AddDecomItemNum(lua_State* state) {
  const int itemId = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ItemID:%d persent:%d", itemId).c_str());
    return 2;
  }

  g_runeSystemMgr->m_decomItems.push_back(itemId);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076AA40
// FUNCTION: fn_08183
int AddItemDecom(lua_State* state) {
  const int itemId = static_cast<int>(lua_tonumber(state, -3));
  const double firstType = lua_tonumber(state, -2);
  const double secondType = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ItemID:%d", itemId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ItemID:%d TypeID1 %d", itemId, static_cast<int>(firstType)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ItemID:%d TypeID2 %d", itemId, static_cast<int>(secondType)).c_str());
    return 2;
  }

  std::pair<int, int>& types = g_runeSystemMgr->m_itemDecoms[itemId];
  types.first = static_cast<int>(firstType);
  types.second = static_cast<int>(secondType);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076AC70
// FUNCTION: fn_08184
int AddItemDecomType(lua_State* state) {
  const double typeValue = lua_tonumber(state, -6);
  const double subTypeValue = lua_tonumber(state, -5);
  int itemId = static_cast<int>(lua_tonumber(state, -4));
  int minCount = static_cast<int>(lua_tonumber(state, -3));
  int maxCount = static_cast<int>(lua_tonumber(state, -2));
  int percent = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("DecomTypeID:%d", static_cast<int>(typeValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("DecomTypeID:%d", static_cast<int>(subTypeValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ItemID:%d", itemId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ItemID:%d itemMin:%d", itemId, minCount).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ItemID:%d ItemMax:%d", itemId, maxCount).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ItemID:%d persent:%d", itemId, percent).c_str());
    return 2;
  }

  g_runeSystemMgr->m_decomTypes[static_cast<int>(typeValue)].emplace_back(itemId, minCount, maxCount, percent);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076AFE0
// FUNCTION: fn_08185
int SetRuneDisplayName(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -4));
  const double typeValue = lua_tonumber(state, -3) - 1.0;
  const int runeId = static_cast<int>(lua_tonumber(state, -2));
  const std::string name(lua_tostring(state, -1));

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SetRune_DisplayName Tag:%d", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SetRune_DisplayName Type:%d", static_cast<int>(typeValue)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SetRune_DisplayName ID:%d", runeId).c_str());
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SetRune_DisplayName ID:%d ResName%s", runeId, name.c_str()).c_str());
    return 2;
  }

  CTagData* tag = g_runeSystemMgr->GetTag(tagId);
  if (tag) {
    RuneData* rune = tag->FindRune(static_cast<int>(typeValue), runeId, nullptr);
    if (rune) {
      rune->m_name = name;
    }
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076B280
// FUNCTION: fn_08186
int SetRuneUpGradeTable(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -4));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -3));
  const double gradeType = lua_tonumber(state, -2);
  const double gradeFailType = lua_tonumber(state, -1);

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Tag:%d", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d", runeSetId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d RuneGradeType%d", runeSetId, static_cast<int>(gradeType)).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d RuneGradeFailType%d", runeSetId, static_cast<int>(gradeFailType)).c_str());
    return 2;
  }

  RuneData* rune = g_runeSystemMgr->FindRune(tagId, 0, runeSetId, nullptr);
  if (rune) {
    RuneSetData* runeSet = static_cast<RuneSetData*>(rune);
    runeSet->m_gradeType = static_cast<int>(gradeType);
    runeSet->m_gradeFailType = static_cast<int>(gradeFailType);
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076B4B0
// FUNCTION: fn_08187
int SetRuneSystemInfo(lua_State* state) {
  const double tagValue = lua_tonumber(state, -1);

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("defult Select Tag:%d", static_cast<int>(tagValue)).c_str());
    return 2;
  }

  g_runeSystemMgr->SetSelectTag(static_cast<int>(tagValue));
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}
