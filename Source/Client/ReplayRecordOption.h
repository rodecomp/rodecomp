#ifndef RAGNAROK_SOURCE_CLIENT_REPLAYRECORDOPTION_H
#define RAGNAROK_SOURCE_CLIENT_REPLAYRECORDOPTION_H

#include <cstdint>

enum class ReplayRecordOption : std::uint8_t {
  BasicInfo = 0,
  Equip = 1,
  Item = 2,
  Skill = 3,
  Guild = 4,
  Party = 5,
  Chatting = 6,
  Shortcut = 7,
  Status = 8,
};

enum : int {
  REPLAY_RECORD_BASICINFO = 0,
  REPLAY_RECORD_EQUIP = 1,
  REPLAY_RECORD_ITEM = 2,
  REPLAY_RECORD_SKILL = 3,
  REPLAY_RECORD_GUILD = 4,
  REPLAY_RECORD_PARTY = 5,
  REPLAY_RECORD_CHATTING = 6,
  REPLAY_RECORD_SHORTCUT = 7,
  REPLAY_RECORD_STATUS = 8,
};

#endif
