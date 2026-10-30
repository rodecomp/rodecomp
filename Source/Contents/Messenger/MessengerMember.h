#ifndef RAGNAROK_SOURCE_CONTENTS_MESSENGER_MESSENGERMEMBER_H
#define RAGNAROK_SOURCE_CONTENTS_MESSENGER_MESSENGERMEMBER_H

#include <cstdint>
#include <list>
#include <string>
#include <vector>

struct MessengerMember {
  MessengerMember();
  explicit MessengerMember(int unused);
  MessengerMember(const MessengerMember& other);
  MessengerMember& operator=(const MessengerMember& other);
  ~MessengerMember();
  std::uint32_t Deserialize(const std::vector<unsigned char>& data);

  /* 0000 */ std::int32_t valid;
  /* 0004 */ std::uint32_t accountId;
  /* 0008 */ std::uint32_t characterId;
  /* 000C */ std::string name;
  /* 0024 */ std::string map;
  /* 003C */ std::int32_t leader;
  /* 0040 */ std::int32_t offline;
  /* 0044 */ std::int32_t color;
  /* 0048 */ std::uint16_t job;
  /* 004A */ std::uint16_t level;
  /* 004C */ std::int32_t dead;
};

int ReadMessengerMembers(std::list<MessengerMember>& members, int offset, int size, const std::vector<unsigned char>& data);

extern bool g_messengerGroupCollapsed;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MessengerMember) == 80);
#endif

#endif
