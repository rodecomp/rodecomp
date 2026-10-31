#ifndef RAGNAROK_SOURCE_CONTENTS_MESSENGER_CLIENTMESSENGER_H
#define RAGNAROK_SOURCE_CONTENTS_MESSENGER_CLIENTMESSENGER_H

#include "Contents/Messenger/MessengerMember.h"

#include <list>

struct PartyMemberEffects;

struct ClientMessenger {
  std::list<MessengerMember>& party;
  std::list<MessengerMember>& friends;

  static std::int32_t MemberColor(int index);
  void AddFriend(MessengerMember& member);
  void AddPartyMember(MessengerMember& member, PartyMemberEffects& effects);
  void ClearFriends();
  int FriendCount() const;
  int PartyCount() const;
  int OnlinePartyCount() const;
  MessengerMember FriendAt(int index) const;
  MessengerMember PartyAt(int index) const;
  MessengerMember PartyByAccount(std::uint32_t accountId) const;
  MessengerMember PartyByCharacter(std::uint32_t characterId) const;
  MessengerMember PartyByName(const char* name) const;
  void SetPartyDead(std::uint32_t accountId, int dead);
  void SetPartyJobLevel(std::uint32_t accountId, std::uint16_t job, std::uint16_t level);
  void SetPartyLeader(std::uint32_t accountId, int leader);
};

namespace EquipmentProperties {
class CEquipmentPropertiesMgr;
}

extern EquipmentProperties::CEquipmentPropertiesMgr* g_equipmentPropertiesMgr;

#endif
