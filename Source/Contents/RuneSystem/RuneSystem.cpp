#include "Contents/RuneSystem/RuneSystem.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Messenger/EquipmentPropertiesMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaFileOperations.h"
#include "Script/LuaScript.h"
#include "Script/NLuaState.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <algorithm>
#include <format>
#include <new>
#include <string>
#include <windows.h>

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

#pragma pack(push, 1)
  struct RuneSelectPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint16_t tagId;
    /* 0004 */ int runeId;
    /* 0008 */ std::uint8_t value;
  };
#pragma pack(pop)

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
  for (std::vector<CTagData*>::iterator it = this->m_tags.begin(); it != this->m_tags.end(); ++it) {
    if (*it) {
      delete *it;
    }

    *it = nullptr;
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
  for (std::vector<RuneData*>::iterator it = this->m_runes[type].begin(); this->m_runes[type].end() != it; ++it) {
    if (*it) {
      this->m_activeCount[type] += (*it)->IsActive();
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

  if (!g_runeSystemMgr->m_initialized) {
    delete g_runeSystemMgr;
    g_runeSystemMgr = nullptr;
    return false;
  }

  return true;
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
  if (count > 7) {
    return -1;
  }

  for (int step = count - 1; step > 0; --step) {
    if (0 != this->m_reward.values[step]) {
      return step;
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

// OFFSET: 0x00766B20
// FUNCTION: fn_08120
CTagData* CRuneSystemMgr::GetTag(int tagId) {
  if (tagId == -1) {
    tagId = this->m_selectTag;
  }

  if (tagId > 0) {
    for (std::vector<CTagData*>::iterator it = this->m_tags.begin(); it != this->m_tags.end(); ++it) {
      if (tagId == (*it)->m_id) {
        return *it;
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00766B60
// FUNCTION: fn_08121
CTagData* CRuneSystemMgr::GetTagByIndex(int index) {
  if (index < 0 || this->m_tags.size() <= index) {
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
      count += rune->IsActive();
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
  if (this->m_decomItems.size() <= index) {
    return 0;
  }

  return this->m_decomItems[index];
}

// OFFSET: 0x00766CE0
// FUNCTION: fn_08126
int CRuneSystemMgr::GetOwnedDecomItem(unsigned int index) const {
  if (this->m_ownedDecomItems.size() <= index) {
    return 0;
  }

  return this->m_ownedDecomItems[index];
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
    if (this->m_gradeFailTypes.end() != found && found->second.size() > static_cast<unsigned int>(runeSet->m_grade)) {
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
int RuneData::IsActive() const {
  return this->m_active != 0;
}

// OFFSET: 0x00766B10
// FUNCTION: fn_08119
std::uint8_t RuneData::GetActiveState() const {
  return this->m_active;
}

// OFFSET: 0x00766E60
// FUNCTION: fn_08132
const std::pair<int, int>& CRuneSystemMgr::GetItemListEntry(int listId, unsigned int index) {
  const auto found = this->m_itemLists.find(listId);
  if (this->m_itemLists.end() == found || index >= found->second.size()) {
    return this->m_emptyItem;
  }

  return found->second[index];
}

// OFFSET: 0x00766EB0
// FUNCTION: fn_08133
int CRuneSystemMgr::GetItemListCount(int listId) {
  const auto found = this->m_itemLists.find(listId);
  if (this->m_itemLists.end() != found) {
    return static_cast<int>(found->second.size());
  }

  return 0;
}

// OFFSET: 0x00766F00
// FUNCTION: fn_08134
RuneData* CRuneSystemMgr::GetCurrentRune(int type, int index) {
  CTagData* tag = this->GetTag(-1);
  if (tag == nullptr) {
    return nullptr;
  }

  if (index < 0 || tag->m_runes[type].size() <= static_cast<unsigned int>(index)) {
    return nullptr;
  }

  return tag->m_runes[type][index];
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
    if (this->m_gradeTypes.end() != found && found->second.size() > static_cast<unsigned int>(runeSet->m_grade)) {
      const std::vector<int>& grades = found->second;
      return runeSet->m_gradeLevel * this->GetGradeFailValue(runeSet) + grades[runeSet->m_grade];
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
    for (unsigned int i = 0; i < this->m_runes[type].size(); ++i) {
      if (this->m_runes[type][i] && id == this->m_runes[type][i]->m_id) {
        if (index) {
          *index = i;
        }

        return this->m_runes[type][i];
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
  const auto decom = this->m_itemDecoms.find(static_cast<int>(itemId));
  const DecomTypes* types = this->m_itemDecoms.end() == decom ? nullptr : &decom->second;
  if (!types) {
    return nullptr;
  }

  const int typeId = which == 0 ? types->first : types->second;
  const auto found = this->m_decomTypes.find(typeId);
  return this->m_decomTypes.end() == found ? nullptr : &found->second;
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

// OFFSET: 0x00767250
// FUNCTION: fn_08146
bool CRuneSystemMgr::LoadScripts() {
  NLuaState lua;
  if (!lua.IsValid()) {
    return false;
  }

  luaL_openlibs(lua.GetState());
  lua_register(lua.GetState(), "Add_Decom_ItemNum", AddDecomItemNum);
  lua_register(lua.GetState(), "AdditemDecomType", AddItemDecomType);
  lua_register(lua.GetState(), "AdditemDecom", AddItemDecom);
  lua_register(lua.GetState(), "AddGradeType_Fail", AddGradeTypeFail);
  lua_register(lua.GetState(), "AddGradeType", AddGradeType);
  lua_register(lua.GetState(), "AddItemListTable_Type", AddItemListTableType);
  lua_register(lua.GetState(), "AddTag", AddTag);
  lua_register(lua.GetState(), "SetRuneSysteminfo", SetRuneSystemInfo);
  lua_register(lua.GetState(), "AddRune", AddRuneData);
  lua_register(lua.GetState(), "SetRune_UpGradeTable", SetRuneUpGradeTable);
  lua_register(lua.GetState(), "AddRune_SlotCollectionList", AddRuneSlotCollectionList);
  lua_register(lua.GetState(), "AddRune_GradeList", AddRuneGradeList);
  lua_register(lua.GetState(), "SetRune_DisplayName", SetRuneDisplayName);
  lua_register(lua.GetState(), "AddRune_Description", AddRuneDescription);
  lua_register(lua.GetState(), "AddRuneSetReward", AddRuneSetReward);

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\runeSystem_table.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\runeSystem_table.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\itemDecom.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\itemDecom.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\runesystemid.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\runesystemid.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\rune_info.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\collection_info.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\rune_desc.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\collection_desc.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\runeset_info.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\rune_info.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\runeset_desc.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\rune_desc.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\runesystem_f.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\runesystem_f.lub", 0);
    return false;
  }

  if (!LoadLuaResource(lua.GetState(), "System\\Rune\\runeset_reward.lub", 0) || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "System\\Rune\\runeset_reward.lub", 0);
    return false;
  }

  lua_getglobal(lua.GetState(), "main_rune");
  if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CRuneSystemMgr main_rune X", 0);
    return false;
  }

  lua_getglobal(lua.GetState(), "main_runeset_reward");
  if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CRuneSystemMgr main_runeset_reward X", 0);
    return false;
  }

  if (!lua_isstring(lua.GetState(), -1)) {
    MessageBoxA(nullptr, "second return value is not string...", "CRuneSystemMgr", 0);
    return false;
  }

  if (!lua_toboolean(lua.GetState(), -2)) {
    std::string message = std::string("error found :") + lua_tostring(lua.GetState(), -1);
    MessageBoxA(nullptr, message.c_str(), "CRuneSystemMgr", 0);
    return false;
  }

  std::sort(this->m_tags.begin(), this->m_tags.end(), [](const CTagData* left, const CTagData* right) { return left->m_id < right->m_id; });
  this->LoadDefaultTag();
  return true;
}

// OFFSET: 0x00767D20
// FUNCTION: fn_08147
void CRuneSystemMgr::Reset() {
  this->LoadDefaultTag();

  for (std::vector<CTagData*>::iterator tagIt = this->m_tags.begin(); tagIt != this->m_tags.end(); ++tagIt) {
    CTagData* tag = *tagIt;
    tag->m_locked = false;
    for (int type = 0; type < 2; ++type) {
      for (std::vector<RuneData*>::iterator runeIt = tag->m_runes[type].begin(); tag->m_runes[type].end() != runeIt; ++runeIt) {
        if (*runeIt) {
          (*runeIt)->m_active = false;
          if (type == 0) {
            RuneSetData* runeSet = dynamic_cast<RuneSetData*>(*runeIt);
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
    if (this->m_rewardStep == 0) {
      return true;
    }
    return false;
  }

  if (this->m_rewardStep == this->FindLastReward(step + 1)) {
    return true;
  }
  return false;
}

// OFFSET: 0x00767FF0
// FUNCTION: fn_08151
__declspec(noinline) bool CRuneSystemMgr::HasRequiredItems(int listId) {
  const auto found = this->m_itemLists.find(listId);
  if (this->m_itemLists.end() == found) {
    return false;
  }

  for (std::vector<std::pair<int, int>>::iterator it = found->second.begin(); it != found->second.end(); ++it) {
    const ITEM_INFO item = g_session.LookupClientItem(it->first, 0);
    if (it->second > item.quantity) {
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

  switch (packetId) {
  case ID_CZ_RUNESET_REQ_REWARD: {
    RuneSelectPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    packet.tagId = static_cast<std::uint16_t>(rune->m_tagId);
    packet.runeId = static_cast<int>(rune->m_id);
    packet.value = static_cast<std::uint8_t>(value);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), &packet);
    break;
  }
  case ID_CZ_RUNE_REQ_TAG_INFO: {
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
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), &packet);
    break;
  }
  case ID_CZ_RUNE_REQ_ACTIVATE:
  case ID_CZ_RUNESET_REQ_ACTIVATE: {
    if (!this->HasRequiredItems(rune->m_activeType)) {
      return 0;
    }

    RuneItemPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    packet.tagId = static_cast<std::uint16_t>(rune->m_tagId);
    packet.runeId = static_cast<int>(rune->m_id);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), &packet);
    this->m_requestPending = true;
    break;
  }
  case ID_CZ_RUNESET_REQ_UPGRADE: {
    if (!this->HasRequiredItems(static_cast<RuneSetData*>(rune)->GetGradeValue())) {
      return 0;
    }

    RuneItemPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    packet.tagId = static_cast<std::uint16_t>(rune->m_tagId);
    packet.runeId = static_cast<int>(rune->m_id);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), &packet);
    this->m_requestPending = true;
    break;
  }
  case ID_CZ_RUNE_REQ_LIST: {
    RuneHeaderPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), &packet);
    break;
  }
  case ID_CZ_RUNESET_REQ_EQUIP: {
    RuneIdPacket packet;
    packet.id = static_cast<std::uint16_t>(packetId);
    packet.runeId = static_cast<int>(rune->m_id);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packetId), &packet);
    break;
  }
  }
  return 1;
}

// OFFSET: 0x007684C0
// FUNCTION: fn_08153
int CRuneSystemMgr::UpdateOwnedDecomItems() {
  this->m_ownedDecomItems.clear();
  for (int i = 0; i < g_session.GetNumInventoryItems(); ++i) {
    ITEM_INFO item = g_session.GetInventoryItemAt(i);
    const auto decom = this->m_itemDecoms.find(std::atoi(item.itemId.c_str()));
    const DecomTypes* types = this->m_itemDecoms.end() == decom ? nullptr : &decom->second;
    if (types) {
      this->m_ownedDecomItems.emplace_back(static_cast<unsigned int>(std::atoi(item.itemId.c_str())));
    }
  }
  return static_cast<int>(this->m_ownedDecomItems.size());
}

// OFFSET: 0x00768780
// FUNCTION: fn_08154
void CRuneSystemMgr::SetCurSlotCount(const int& count) {
  this->m_curSlotCount = count;
}

// OFFSET: 0x007687B0
// FUNCTION: fn_08155
void CRuneSystemMgr::SetCurRuneSet(int tagId, int runeSetId, int grade, int slotCount) {
  int oldId = 0;
  if (this->m_curRuneSet != nullptr) {
    oldId = static_cast<int>(this->m_curRuneSet->m_id);
    if (this->m_curRuneSet->m_id != runeSetId) {
      EquipmentProperties::CEquipmentPropertiesMgr* mgr = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
      const int one = 1;
      mgr->RemoveExpansion(one, oldId);
    }
  }

  if (runeSetId == 0) {
    this->m_curRuneSet = nullptr;
    return;
  }

  RuneSetData* runeSet = static_cast<RuneSetData*>(this->FindRune(tagId, 0, runeSetId, nullptr));
  this->m_curRuneSet = runeSet;
  if (runeSet != nullptr) {
    this->m_curSlotCount = runeSet->CountActiveSlots();
    if (grade != -1) {
      runeSet->m_grade = grade;
    }
    if (slotCount != -1) {
      this->m_curSlotCount = slotCount;
    }
    if (oldId != runeSetId) {
      EquipmentProperties::CEquipmentPropertiesMgr* mgr = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
      const int one = 1;
      mgr->AddExpansion(one, runeSetId, false);
    }
  }
}

// OFFSET: 0x007688A0
// FUNCTION: fn_08156
void CRuneSystemMgr::SetSelectTag(int tagId) {
  if (this->m_tags.empty()) {
    return;
  }
  const auto hasTag = [&]() { return this->GetTag(tagId) != nullptr; };
  if (hasTag()) {
    this->m_selectTag = tagId;
  } else {
    this->m_selectTag = this->m_tags.front()->m_id;
  }
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

  RuneSetData* runeSet = static_cast<RuneSetData*>(tag->FindRune(0, id, nullptr));
  if (!runeSet) {
    return false;
  }

  runeSet->m_rewardStep = step;
  return true;
}

// INLINED
inline RuneSetData* CRuneSystemMgr::GetRuneSet(int tagId, int runeSetId) {
  return static_cast<RuneSetData*>(this->FindRune(tagId, 0, runeSetId, nullptr));
}

// OFFSET: 0x00765E40
// FUNCTION: fn_46506
inline void CRuneSystemMgr::InsertGradeType(int gradeId, int index, int value) {
  std::map<int, std::vector<int>>& types = this->m_gradeTypes;
  if (types[gradeId].size() < 15) {
    types[gradeId].resize(15, 0);
  }

  if (index < 15) {
    types[gradeId][index] = value;
  }
}

// OFFSET: 0x00765EE0
// FUNCTION: fn_46507
inline void CRuneSystemMgr::InsertGradeFailType(int gradeId, int index, int value) {
  std::map<int, std::vector<int>>& types = this->m_gradeFailTypes;
  if (types[gradeId].size() < 15) {
    types[gradeId].resize(15, 0);
  }

  if (index < 15) {
    types[gradeId][index] = value;
  }
}

// OFFSET: 0x00765F80
// FUNCTION: fn_46508
inline void CRuneSystemMgr::InsertItemList(int listId, int itemId, int count) {
  this->m_itemLists[listId].emplace_back(itemId, count);
}

// INLINED
inline void CRuneSystemMgr::InsertRuneDescription(int tagId, int runeSetId, int index, std::string description) {
  RuneSetData* runeSet = static_cast<RuneSetData*>(this->FindRune(tagId, 0, runeSetId, nullptr));
  if (runeSet) {
    if (index >= 0 && index <= 5) {
      runeSet->m_descriptions[index].push_back(description);
    } else {
      MessageBoxA(hWnd, FormatOwnedString("AddRune_Description 인댁스 오류 %d", index).c_str(), "LuaLoad", MB_OK);
    }
  }
}

// INLINED
inline void CRuneSystemMgr::InsertRuneGrade(int tagId, int runeSetId, int grade, int count) {
  RuneSetData* runeSet = static_cast<RuneSetData*>(this->FindRune(tagId, 0, runeSetId, nullptr));
  if (runeSet) {
    if (grade < 15) {
      runeSet->m_gradeList[grade] = count;
    }
  }
}

// INLINED
inline void CRuneSystemMgr::InsertRuneSlot(int tagId, int runeSetId, int runeId) {
  RuneSetData* runeSet = static_cast<RuneSetData*>(this->FindRune(tagId, 0, runeSetId, nullptr));
  if (runeSet) {
    RuneData* rune = this->FindRune(tagId, 1, runeId, nullptr);
    runeSet->m_slots.push_back(rune);
  }
}

// INLINED
inline void CRuneSystemMgr::SetRuneName(int tagId, int type, int runeId, const std::string& name) {
  RuneData* rune = this->FindRune(tagId, type, runeId, nullptr);
  if (rune) {
    rune->m_name = name;
  }
}

// INLINED
inline void CRuneSystemMgr::SetRuneUpGrade(int tagId, int runeSetId, int gradeType, int gradeFailType) {
  RuneSetData* runeSet = static_cast<RuneSetData*>(this->FindRune(tagId, 0, runeSetId, nullptr));
  if (runeSet) {
    runeSet->m_gradeType = gradeType;
    runeSet->m_gradeFailType = gradeFailType;
  }
}

// OFFSET: 0x00769240
// FUNCTION: fn_08173
int AddGradeType(lua_State* state) {
  const int gradeId = static_cast<int>(lua_tonumber(state, -3));
  int valueIndex = static_cast<int>(lua_tonumber(state, -2));
  const int value = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d", gradeId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ValIndex:%d", valueIndex).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d ValData %d", gradeId, value).c_str());
    return 2;
  }

  CRuneSystemMgr* mgr = g_runeSystemMgr;
  mgr->InsertGradeType(gradeId, --valueIndex, value);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x00769440
// FUNCTION: fn_08174
int AddGradeTypeFail(lua_State* state) {
  const int gradeId = static_cast<int>(lua_tonumber(state, -3));
  int valueIndex = static_cast<int>(lua_tonumber(state, -2));
  const int value = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d", gradeId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d", valueIndex).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable GradeID:%d ValData %d", gradeId, value).c_str());
    return 2;
  }

  CRuneSystemMgr* mgr = g_runeSystemMgr;
  mgr->InsertGradeFailType(gradeId, --valueIndex, value);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x00769640
// FUNCTION: fn_08175
int AddItemListTableType(lua_State* state) {
  const int listId = static_cast<int>(lua_tonumber(state, -4));
  const int listIndex = static_cast<int>(lua_tonumber(state, -3));
  const int itemValue = static_cast<int>(lua_tonumber(state, -2));
  const int countValue = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListID:%d", listId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListIndex:%d", listIndex).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListID:%d ItemID:%d", listId, itemValue).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("ListID:%d Num%d", listId, countValue).c_str());
    return 2;
  }

  g_runeSystemMgr->InsertItemList(listId, itemValue, countValue);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x007698F0
// FUNCTION: fn_08176
int AddRuneData(lua_State* state) {
  const int tagValue = static_cast<int>(lua_tonumber(state, -5));
  const int typeValue = static_cast<int>(lua_tonumber(state, -4));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -3));
  const std::string resName(lua_tostring(state, -2));
  const int activeType = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Tag:%d", tagValue).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneType:%d", typeValue).c_str());
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
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d ActiveType%d", runeSetId, activeType).c_str());
    return 2;
  }

  g_runeSystemMgr->AddRune(tagValue, typeValue - 1, runeSetId, resName, activeType);
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

  RuneSetData* runeSet = g_runeSystemMgr->GetRuneSet(tagId, runeSetId);
  RuneSetReward* reward = &runeSet->m_reward;
  if (!runeSet || !reward) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, std::format("ErrData TagID:{}RuneSetID:{}", tagId, runeSetId).c_str());
    return 2;
  }

  reward->values[0] = 1;
  for (int index = 0; index < 7; ++index) {
    const int itemId = static_cast<int>(lua_tonumber(state, index - 7));
    reward->values[index + 1] = itemId;
    if (!lua_isnumber(state, index - 7)) {
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
  const int indexValue = static_cast<int>(lua_tonumber(state, -2));
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
    lua_pushstring(state, FormatOwnedString("Description ID:%d itemID:%d", runeSetId, indexValue).c_str());
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("Description ID:%d Description%s", runeSetId, description.c_str()).c_str());
    return 2;
  }

  CRuneSystemMgr* mgr = g_runeSystemMgr;
  mgr->InsertRuneDescription(tagId, runeSetId, indexValue - 1, description);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076A240
// FUNCTION: fn_08179
int AddRuneGradeList(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -4));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -3));
  const int gradeValue = static_cast<int>(lua_tonumber(state, -2));
  const int countValue = static_cast<int>(lua_tonumber(state, -1));

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
    lua_pushstring(state, FormatOwnedString("GradeList ID:%d itemID:%d", runeSetId, gradeValue).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("GradeList ID:%d itemNum:%d]", runeSetId, countValue).c_str());
    return 2;
  }

  g_runeSystemMgr->InsertRuneGrade(tagId, runeSetId, gradeValue - 1, countValue);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076A470
// FUNCTION: fn_08180
int AddRuneSlotCollectionList(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -3));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -2));
  const int runeValue = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SlotCollectionList Tag:%d", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SlotCollectionList ID:%d", runeSetId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SlotCollectionList ID:%d RuneID%d", runeSetId, runeValue).c_str());
    return 2;
  }

  g_runeSystemMgr->InsertRuneSlot(tagId, runeSetId, runeValue);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076A690
// FUNCTION: fn_08181
int AddTag(lua_State* state) {
  const int tagValue = static_cast<int>(lua_tonumber(state, -2));
  const std::string name(lua_tostring(state, -1));

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d", tagValue).c_str());
    return 2;
  }

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d ResName%s", tagValue, name.c_str()).c_str());
    return 2;
  }

  CRuneSystemMgr* mgr = g_runeSystemMgr;
  mgr->InsertTag(tagValue, name);
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

  g_runeSystemMgr->InsertDecomItem(itemId);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076AA40
// FUNCTION: fn_08183
int AddItemDecom(lua_State* state) {
  const int itemId = static_cast<int>(lua_tonumber(state, -3));
  const int firstType = static_cast<int>(lua_tonumber(state, -2));
  const int secondType = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ItemID:%d", itemId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ItemID:%d TypeID1 %d", itemId, firstType).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("gradetable ItemID:%d TypeID2 %d", itemId, secondType).c_str());
    return 2;
  }

  DecomTypes& types = g_runeSystemMgr->m_itemDecoms[static_cast<int>(itemId)];
  types.first = firstType;
  types.second = secondType;
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// INLINED
static void AddDecomTypeItem(int type, int itemId, int minCount, int maxCount, int percent) {
  g_runeSystemMgr->m_decomTypes[type].emplace_back(itemId, minCount, maxCount, percent);
}

// OFFSET: 0x0076AC70
// FUNCTION: fn_08184
int AddItemDecomType(lua_State* state) {
  const int typeValue = static_cast<int>(lua_tonumber(state, -6));
  const int subTypeValue = static_cast<int>(lua_tonumber(state, -5));
  int itemId = static_cast<int>(lua_tonumber(state, -4));
  int minCount = static_cast<int>(lua_tonumber(state, -3));
  int maxCount = static_cast<int>(lua_tonumber(state, -2));
  int percent = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("DecomTypeID:%d", typeValue).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("DecomTypeID:%d", subTypeValue).c_str());
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

  AddDecomTypeItem(typeValue, itemId, minCount, maxCount, percent);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076AFE0
// FUNCTION: fn_08185
int SetRuneDisplayName(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -4));
  const int typeValue = static_cast<int>(lua_tonumber(state, -3) - 1.0);
  const int runeId = static_cast<int>(lua_tonumber(state, -2));
  const std::string name(lua_tostring(state, -1));

  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SetRune_DisplayName Tag:%d", tagId).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("SetRune_DisplayName Type:%d", typeValue).c_str());
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

  g_runeSystemMgr->SetRuneName(tagId, typeValue, runeId, name);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076B280
// FUNCTION: fn_08186
int SetRuneUpGradeTable(lua_State* state) {
  const int tagId = static_cast<int>(lua_tonumber(state, -4));
  const int runeSetId = static_cast<int>(lua_tonumber(state, -3));
  const int gradeType = static_cast<int>(lua_tonumber(state, -2));
  const int gradeFailType = static_cast<int>(lua_tonumber(state, -1));

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
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d RuneGradeType%d", runeSetId, gradeType).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("RuneSetID:%d RuneGradeFailType%d", runeSetId, gradeFailType).c_str());
    return 2;
  }

  g_runeSystemMgr->SetRuneUpGrade(tagId, runeSetId, gradeType, gradeFailType);

  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}

// OFFSET: 0x0076B4B0
// FUNCTION: fn_08187
int SetRuneSystemInfo(lua_State* state) {
  const int tagValue = static_cast<int>(lua_tonumber(state, -1));

  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatOwnedString("defult Select Tag:%d", tagValue).c_str());
    return 2;
  }

  g_runeSystemMgr->SetSelectTag(tagValue);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatOwnedString("good").c_str());
  return 2;
}
