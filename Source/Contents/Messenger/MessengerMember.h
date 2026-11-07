#ifndef RAGNAROK_SOURCE_CONTENTS_MESSENGER_MESSENGERMEMBER_H
#define RAGNAROK_SOURCE_CONTENTS_MESSENGER_MESSENGERMEMBER_H

#include <cstdint>
#include <list>
#include <string>
#include <vector>

struct MessengerMember {
  MessengerMember();
  // OFFSET: 0x00739B60
  // FUNCTION: fn_07696
  explicit MessengerMember(int) : valid(0), dead(0) {
  }


  // OFFSET: 0x00739AB0
  // FUNCTION: fn_07695
  MessengerMember(const MessengerMember& other) = default;


  // OFFSET: 0x00DC6FA0
  // FUNCTION: fn_22785
  MessengerMember& operator=(const MessengerMember& other) = default;

  ~MessengerMember();
  std::uint32_t Deserialize(const std::vector<unsigned char>& data);
  std::vector<unsigned char> Serialize() const;

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


#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MessengerMember) == 80);
#endif

#endif
