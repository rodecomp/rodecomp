#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Effects/PartyMemberEffects.h"

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

// OFFSET: 0x00DD80A0

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
