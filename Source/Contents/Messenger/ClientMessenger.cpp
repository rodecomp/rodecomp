#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Effects/PartyMemberEffects.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ItemInfo.h"
#include "Script/LuaInstance.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <list>
#include <map>
#include <new>
#include <unordered_map>
#include <vector>

void __fastcall RefreshItemWindows(UIWindowMgr* mgr);

struct SessionItemLists {
  /* 0000 */ unsigned char unknown_0000[0x1710];
  /* 1710 */ std::list<ItemInfo> inventory;
  /* 1718 */ std::list<ItemInfo> items_1718;
  /* 1720 */ std::list<ItemInfo> items_1720;
  /* 1728 */ std::list<ItemInfo> items_1728;
  /* 1730 */ std::list<ItemInfo> items_1730;
  /* 1738 */ unsigned char unknown_1738[0x48];
  /* 1780 */ std::list<ItemInfo> items_1780[11];

  void ClearItems1780();
  void KeepEquipment();
  void RemoveEquipment();
  void ClearTradeItems();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(SessionItemLists, inventory) == 0x1710);
static_assert(offsetof(SessionItemLists, items_1730) == 0x1730);
static_assert(offsetof(SessionItemLists, items_1780) == 0x1780);
#endif

struct EquipmentProperty {
  virtual ~EquipmentProperty() = default;
};

struct EquipmentProperty1 : EquipmentProperty {};

struct EquipmentProperty2 : EquipmentProperty {};

struct EquipmentProperty3 : EquipmentProperty {};

struct EquipmentValueA {
  /* 0000 */ int value;
};

struct EquipmentValueB {
  /* 0000 */ int value;
};

struct EquipmentValueC {
  /* 0000 */ int value;
};

struct EquipmentEntryA {
  /* 0000 */ int value;
};

struct EquipmentEntryB {
  /* 0000 */ int value;
};

struct EquipmentPropertyDefinition {
  /* 0000 */ unsigned char unknown_0000[8];
  /* 0008 */ std::string unknown_0008;
  /* 0020 */ std::vector<std::string> unknown_0020;
  /* 002C */ unsigned char unknown_002C[8];
  /* 0034 */ std::vector<std::string> unknown_0034;
  /* 0040 */ int unknown_0040;
  /* 0044 */ std::string unknown_0044;
  /* 005C */ std::map<int, int> unknown_005C;
  /* 0064 */ std::string unknown_0064;
  /* 007C */ int unknown_007C;
};

struct EquipmentPropertyGroup {
  /* 0000 */ std::string unknown_0000;
  /* 0018 */ std::map<std::int64_t, EquipmentPropertyDefinition*> definitions;
  ~EquipmentPropertyGroup();
};

struct EquipmentPropertyStore {
  /* 0000 */ std::map<int, std::map<int, EquipmentProperty*>> properties;
  /* 0008 */ std::unordered_map<std::string, std::string> names;

  EquipmentPropertyStore();
  ~EquipmentPropertyStore();
  void Clear();
};

struct EquipmentPropertyMaps {
  /* 0000 */ int unknown_0000;
  /* 0004 */ std::unordered_map<int, int> first;
  /* 0024 */ std::unordered_map<int, int> second;
};

namespace EquipmentProperties {
  class CEquipmentPropertiesMgr {
  public:
    /* 0004 */ CLuaInstance script;
    /* 000C */ std::uint16_t unknown_000C;
    /* 0010 */ int unknown_0010;
    /* 0014 */ int unknown_0014;
    /* 0018 */ EquipmentPropertyStore* store;
    /* 001C */ int unknown_001C;
    /* 0020 */ int unknown_0020;
    /* 0024 */ std::unordered_map<int, EquipmentProperty1*> properties1;
    /* 0044 */ std::unordered_map<int, EquipmentProperty2*> properties2;
    /* 0064 */ std::unordered_map<int, EquipmentProperty3*> properties3;
    /* 0084 */ std::set<int> unknown_0084;
    /* 008C */ std::map<int, EquipmentPropertyGroup> unknown_008C;
    /* 0094 */ std::unordered_map<int, EquipmentPropertyMaps> unknown_0094;
    /* 00B4 */ std::unordered_map<int, EquipmentValueA> unknown_00B4;
    /* 00D4 */ std::unordered_map<int, std::pair<int, int>> unknown_00D4;
    /* 00F4 */ std::unordered_map<int, std::pair<int, int>> unknown_00F4;
    /* 0114 */ std::unordered_map<int, std::pair<int, int>> unknown_0114;
    /* 0134 */ std::unordered_map<int, EquipmentValueA> unknown_0134;
    /* 0154 */ std::unordered_map<int, std::array<int, 3>> unknown_0154;
    /* 0174 */ std::unordered_map<int, EquipmentValueB> unknown_0174;
    /* 0194 */ std::map<int, int> unknown_0194;
    /* 019C */ std::unordered_map<int, EquipmentValueC> unknown_019C;
    /* 01BC */ std::unordered_map<int, std::unordered_map<int, EquipmentEntryA>> unknown_01BC;
    /* 01DC */ std::unordered_map<int, std::unordered_map<int, EquipmentEntryB>> unknown_01DC;

    CEquipmentPropertiesMgr();
    virtual ~CEquipmentPropertiesMgr();
    void Clear();
  };
}

// GLOBAL: gl_24723
int g_resetMessengerRandomStats = 1;

// GLOBAL: gl_24724
EquipmentProperties::CEquipmentPropertiesMgr* g_equipmentPropertiesMgr;

// OFFSET: 0x0068A340
// FUNCTION: fn_05603
EquipmentPropertyStore::EquipmentPropertyStore() = default;

// OFFSET: 0x00698CE0
// FUNCTION: fn_05771
void EquipmentPropertyStore::Clear() {
  for (auto& group : this->properties) {
    for (auto& property : group.second) {
      delete property.second;
    }
  }
  this->properties.clear();
  this->names.clear();
}

// OFFSET: 0x006965A0
// FUNCTION: fn_05740
EquipmentPropertyStore::~EquipmentPropertyStore() {
  this->Clear();
}

// OFFSET: 0x0069BB80
// FUNCTION: fn_05807
EquipmentPropertyGroup::~EquipmentPropertyGroup() {
  for (auto& definition : this->definitions) {
    delete definition.second;
  }
}

// OFFSET: 0x006A1AA0
// FUNCTION: fn_06013
EquipmentProperties::CEquipmentPropertiesMgr::CEquipmentPropertiesMgr()
    : unknown_000C(1), unknown_0010(-1), unknown_0014(-1), store(new EquipmentPropertyStore), unknown_001C(0), unknown_0020(0) {
}

// OFFSET: 0x006ABE90
// FUNCTION: fn_06170
void EquipmentProperties::CEquipmentPropertiesMgr::Clear() {
  this->unknown_001C = 0;
  this->unknown_0084.clear();
  for (auto& property : this->properties1) {
    delete property.second;
  }
  this->properties1.clear();
  for (auto& property : this->properties2) {
    delete property.second;
  }
  this->properties2.clear();
  for (auto& property : this->properties3) {
    delete property.second;
  }
  this->properties3.clear();
  this->store->Clear();
}

// OFFSET: 0x006A56C0

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentPropertyDefinition) == 0x80);
static_assert(sizeof(EquipmentPropertyGroup) == 0x20);
static_assert(sizeof(EquipmentPropertyStore) == 0x28);
static_assert(sizeof(EquipmentProperties::CEquipmentPropertiesMgr) == 0x1FC);
#endif

// OFFSET: 0x00DDFFE0
// FUNCTION: fn_23451
std::int32_t ClientMessenger::MemberColor(int index) {
  switch (index) {
  case 0:
    return -16108936;
  case 1:
    return -16746376;
  case 2:
    return -16777216;
  case 3:
  case 15:
    return -8912896;
  case 4:
    return -16746496;
  case 5:
    return -8882176;
  case 6:
    return -8912776;
  case 7:
    return -8900096;
  case 8:
    return -8882056;
  case 9:
    return -16777096;
  case 10:
    return -16764366;
  case 11:
    return -13500416;
  case 12:
    return -16764416;
  case 13:
    return -13487616;
  case 14:
    return -13500366;
  default:
    return -13487566;
  }
}

// OFFSET: 0x00DD6F10
// FUNCTION: fn_23281
void ClientMessenger::AddFriend(MessengerMember& member) {
  int index = 0;

  for (MessengerMember& existing : this->friends) {
    if (existing.name == member.name) {
      member.color = MemberColor(index);
      existing = member;
    }

    ++index;
  }

  member.color = MemberColor(index);
  this->friends.push_back(member);
}

// OFFSET: 0x00DD70B0
// FUNCTION: fn_23283
void ClientMessenger::AddPartyMember(MessengerMember& member, PartyMemberEffects& effects) {
  if (member.map.size() > 4) {
    member.map.resize(member.map.size() - 3);
    member.map.append("rsw", 3);
  }

  int index = 0;

  for (MessengerMember& existing : this->party) {
    if (existing.name == member.name) {
      member.color = MemberColor(index);
      existing = member;
      return;
    }

    ++index;
  }

  member.color = MemberColor(index);
  this->party.push_back(member);

  if (effects.Enabled()) {
    effects.EnsurePartyEffect(member.accountId);
  }
}

// OFFSET: 0x00DD87E0
// FUNCTION: fn_23312
void ClientMessenger::ClearFriends() {
  this->friends.clear();
}

// OFFSET: 0x00DDF8A0
// FUNCTION: fn_23427
int ClientMessenger::FriendCount() const {
  return static_cast<int>(this->friends.size());
}

// OFFSET: 0x00DDF9D0
// FUNCTION: fn_23436
int ClientMessenger::PartyCount() const {
  return static_cast<int>(this->party.size());
}

// OFFSET: 0x00DDFA20
// FUNCTION: fn_23441
int ClientMessenger::OnlinePartyCount() const {
  int count = 0;

  for (const MessengerMember& member : this->party) {
    if (member.offline == 0) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00DDCC30
// FUNCTION: fn_23372
MessengerMember ClientMessenger::FriendAt(int index) const {
  int position = 0;

  for (const MessengerMember& member : this->friends) {
    if (position == index) {
      return member;
    }

    ++position;
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE0500
// FUNCTION: fn_23455
MessengerMember ClientMessenger::PartyAt(int index) const {
  int position = 0;

  for (const MessengerMember& member : this->party) {
    if (position == index) {
      return member;
    }

    ++position;
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE00D0
// FUNCTION: fn_23452
MessengerMember ClientMessenger::PartyByAccount(std::uint32_t accountId) const {
  for (const MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      return member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE01C0
// FUNCTION: fn_23453
MessengerMember ClientMessenger::PartyByCharacter(std::uint32_t characterId) const {
  for (const MessengerMember& member : this->party) {
    if (member.characterId == characterId) {
      return member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE03E0
// FUNCTION: fn_23454
MessengerMember ClientMessenger::PartyByName(const char* name) const {
  for (const MessengerMember& member : this->party) {
    if (member.name == name) {
      return member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DD8000
// FUNCTION: fn_23301
void ClientMessenger::SetPartyDead(std::uint32_t accountId, int dead) {
  for (MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      member.dead = dead;
      return;
    }
  }
}

// OFFSET: 0x00DD8030
// FUNCTION: fn_23302
void ClientMessenger::SetPartyJobLevel(std::uint32_t accountId, std::uint16_t job, std::uint16_t level) {
  for (MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      member.job = job;
      member.level = level;
      return;
    }
  }
}

// OFFSET: 0x00DD8070
// FUNCTION: fn_23303
void ClientMessenger::SetPartyLeader(std::uint32_t accountId, int leader) {
  for (MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      member.leader = leader;
      return;
    }
  }
}

struct SessionMessengerContext {
  unsigned char pad_0000[0x1710];
  /* 1710 */ std::list<MessengerMember> lists[12];
  /* 1770 */ unsigned char pad_1770[0x18];
  /* 1788 */ int listData[11][2];
  /* 17E0 */ unsigned char pad_17E0[0x26E0];
  /* 3EC0 */ int slots[10][62];
  /* 4870 */ unsigned char pad_4870[0x9A0];
  /* 5210 */ int slot5210;
  /* 5214 */ unsigned char pad_5214[0x118];
  /* 532C */ int stat2C;
  /* 5330 */ int stat30;
  /* 5334 */ int stat34;
  /* 5338 */ int stat38;
  /* 533C */ int stat3C;
  /* 5340 */ int stats[12];
  /* 5370 */ unsigned char pad_5370[0x1F8];
  /* 5568 */ int bound68;
  /* 556C */ int bound6C;
  /* 5570 */ int bound70;
  /* 5574 */ int bound74;

  void UpdateRandomStats();
  void ClearMemberLists();
  void PurgeMatchingMembers1();
  void ClearLists();
  void InitSlots();
  void PurgeMatchingMembers2();
  void ResetSlots();

  int GetList0Size() const;
  int GetList11Size() const;
  int GetList6Size() const;
  int GetList8Size() const;
  int GetList9Size() const;
  int GetList10Size() const;
  int GetList7Size() const;
  int GetList1788(int index) const;
  int GetList2Size() const;
  int GetList4Size() const;
  int GetList1Size() const;
  int GetList5Size() const;
};

struct ItemNotificationWindow : UIWindow {
  /* 0080 */ unsigned char unknown_0080[0x34];
  /* 00B4 */ MessengerMember member;
};

// OFFSET: 0x00AAAF10
// FUNCTION: fn_17276
void UIWindowMgr::NotifyItemWindows(int accountId, int lower, int upper) {
  for (UIWindow* window : this->snapExcluded) {
    bool matches;
    {
      MessengerMember member = static_cast<ItemNotificationWindow*>(window)->member;
      matches = member.accountId == accountId;
    }
    if (matches) {
      window->SendMsg(nullptr, 6, 34, accountId, lower, upper);
      break;
    }
  }
}

// OFFSET: 0x00CD6E40
// FUNCTION: fn_21290
void CGameMode::NotifyItemBounds() {
  if (this->itemWindow) {
    this->itemWindow->Invalidate();
  }
  if (g_windowMgr.window_02C8) {
    g_windowMgr.window_02C8->SendMsg(nullptr, 6, 34, g_session.unknown_1604, 0, 0);
  }
  g_windowMgr.NotifyItemWindows(g_session.unknown_1604, g_session.unknown_5568, g_session.unknown_556C);
}

// OFFSET: 0x00DD80A0
// FUNCTION: fn_23304
void SessionMessengerContext::UpdateRandomStats() {
  if (g_resetMessengerRandomStats != 0) {
    g_resetMessengerRandomStats = 0;
    this->stat34 = std::max(std::rand() % 12, 0);
    this->stat30 = std::max(std::rand() % 12, 0);
    this->stat3C = std::max(std::rand() % 12, 0);
    this->stat2C = std::max(std::rand() % 12, 0);
    this->stat38 = std::max(std::rand() % 12, 0);
  }

  const int bound70 = this->bound70;
  const int bound74 = this->bound74;
  const int bound68 = this->bound68;
  const int bound6C = this->bound6C;
  this->stat34 = std::max(std::rand() % 12, 0);
  this->stat2C = std::max(std::rand() % 12, 0);
  this->stat38 = std::max(std::rand() % 12, 0);
  this->stat30 = std::max(std::rand() % 12, 0);
  this->stat3C = std::max(std::rand() % 12, 0);
  for (int& value : this->stats) {
    value = std::max(std::abs(std::rand()) % 1, 0);
  }

  const auto notify = [this]() {
    if (g_world.ActiveGameMode()) {
      if (g_windowMgr.unknown_01DC) {
        if (this->bound6C < this->bound68 || this->bound74 < this->bound70) {
          return;
        }
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01DC)->Invalidate();
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01DC)->InvalidateChildren();
      }
      static_cast<CGameMode*>(g_world.ActiveGameMode())->NotifyItemBounds();
    }
  };
  if (this->bound68 != bound68) {
    this->bound68 = bound68;
    notify();
  }
  if (this->bound6C != bound6C) {
    this->bound6C = bound6C;
    notify();
  }
  if (this->bound70 != bound70) {
    this->bound70 = bound70;
    notify();
  }
  if (this->bound74 != bound74) {
    this->bound74 = bound74;
    notify();
  }
}

// OFFSET: 0x00DD8380

// OFFSET: 0x00DD8450

// OFFSET: 0x00DD8540

// OFFSET: 0x00DD85A0
// FUNCTION: fn_23308
void ClearMemberFlags(std::uint32_t* flags) {
  *flags &= ~0x40u;
}

// OFFSET: 0x00DD85B0

// OFFSET: 0x00DD8660

// OFFSET: 0x00DD8750
// FUNCTION: fn_23311
void SessionMessengerContext::ResetSlots() {
  this->slot5210 = 0;
  this->slots[0][0] = 0;
  this->slots[1][0] = 0;
  this->slots[2][0] = 0;
  this->slots[3][0] = 0;
  this->slots[4][0] = 0;
  this->slots[5][0] = 0;
  this->slots[6][0] = 0;
  this->slots[7][0] = 0;
  this->slots[8][0] = 0;
  this->slots[9][0] = 0;
}

// OFFSET: 0x00DDF8B0
// FUNCTION: fn_23428
int SessionMessengerContext::GetList0Size() const {
  return static_cast<int>(this->lists[0].size());
}

// OFFSET: 0x00DDF8C0
// FUNCTION: fn_23429
int SessionMessengerContext::GetList11Size() const {
  return static_cast<int>(this->lists[11].size());
}

// OFFSET: 0x00DDF8D0
// FUNCTION: fn_23430
int SessionMessengerContext::GetList6Size() const {
  return static_cast<int>(this->lists[6].size());
}

// OFFSET: 0x00DDF8E0
// FUNCTION: fn_23431
int SessionMessengerContext::GetList8Size() const {
  return static_cast<int>(this->lists[8].size());
}

// OFFSET: 0x00DDF8F0
// FUNCTION: fn_23432
int SessionMessengerContext::GetList9Size() const {
  return static_cast<int>(this->lists[9].size());
}

// OFFSET: 0x00DDF900
// FUNCTION: fn_23433
int SessionMessengerContext::GetList10Size() const {
  return static_cast<int>(this->lists[10].size());
}

// OFFSET: 0x00DDF910
// FUNCTION: fn_23434
int SessionMessengerContext::GetList7Size() const {
  return static_cast<int>(this->lists[7].size());
}

// OFFSET: 0x00DDF920
// FUNCTION: fn_23435
int SessionMessengerContext::GetList1788(int index) const {
  if (index < 0) {
    return 0;
  }
  if (index >= 11) {
    return 0;
  }
  return this->listData[index][0];
}

// OFFSET: 0x00DDF9E0
// FUNCTION: fn_23437
int SessionMessengerContext::GetList2Size() const {
  return static_cast<int>(this->lists[2].size());
}

// OFFSET: 0x00DDF9F0
// FUNCTION: fn_23438
int SessionMessengerContext::GetList4Size() const {
  return static_cast<int>(this->lists[4].size());
}

// OFFSET: 0x00DDFA00
// FUNCTION: fn_23439
int SessionMessengerContext::GetList1Size() const {
  return static_cast<int>(this->lists[1].size());
}

// OFFSET: 0x00DDFA10
// FUNCTION: fn_23440
int SessionMessengerContext::GetList5Size() const {
  return static_cast<int>(this->lists[5].size());
}

// OFFSET: 0x00DD8380
// FUNCTION: fn_23305
void SessionItemLists::ClearItems1780() {
  for (std::list<ItemInfo>& items : this->items_1780) {
    items.clear();
  }
}

// OFFSET: 0x00DD8450
// FUNCTION: fn_23306
void SessionItemLists::KeepEquipment() {
  for (auto entry = this->inventory.begin(); entry != this->inventory.end();) {
    switch (entry->unknown_0000[0]) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      ++entry;
      break;
    default:
      entry = this->inventory.erase(entry);
      break;
    }
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00DD8540
// FUNCTION: fn_23307
void SessionItemLists::ClearTradeItems() {
  this->items_1720.erase(this->items_1720.begin(), this->items_1720.end());
  this->items_1730.erase(this->items_1730.begin(), this->items_1730.end());
  this->items_1718.erase(this->items_1718.begin(), this->items_1718.end());
}

// OFFSET: 0x00DD8660
// FUNCTION: fn_23310
void SessionItemLists::RemoveEquipment() {
  for (auto entry = this->inventory.begin(); entry != this->inventory.end();) {
    switch (entry->unknown_0000[0]) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      entry = this->inventory.erase(entry);
      break;
    default:
      ++entry;
      break;
    }
  }

  RefreshItemWindows(&g_windowMgr);
}
