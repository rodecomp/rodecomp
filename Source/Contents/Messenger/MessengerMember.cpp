#include "Contents/Messenger/MessengerMember.h"

// OFFSET: 0x00D03DC0
// FUNCTION: fn_21550
MessengerMember::MessengerMember() : valid(1), dead(0) {
}

// OFFSET: 0x00739B60
// FUNCTION: fn_07696
MessengerMember::MessengerMember(int) : valid(0), dead(0) {
}

// OFFSET: 0x00739AB0
// FUNCTION: fn_07695
MessengerMember::MessengerMember(const MessengerMember& other)
    : valid(other.valid), accountId(other.accountId), characterId(other.characterId), name(other.name), map(other.map), leader(other.leader),
      offline(other.offline), color(other.color), job(other.job), level(other.level), dead(other.dead) {
}

// OFFSET: 0x00DC6FA0
// FUNCTION: fn_22785
MessengerMember& MessengerMember::operator=(const MessengerMember& other) {
  this->valid = other.valid;
  this->accountId = other.accountId;
  this->characterId = other.characterId;
  this->name = other.name;
  this->map = other.map;
  this->leader = other.leader;
  this->offline = other.offline;
  this->color = other.color;
  this->job = other.job;
  this->level = other.level;
  this->dead = other.dead;

  return *this;
}

// OFFSET: 0x007384A0
// FUNCTION: fn_07666
MessengerMember::~MessengerMember() = default;
