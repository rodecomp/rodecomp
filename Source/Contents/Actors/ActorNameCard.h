#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORNAMECARD_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORNAMECARD_H

#include <cstdint>
#include <deque>
#include <map>
#include <string>
#include <vector>

class CActorNameCard {
public:
  CActorNameCard();
  virtual ~CActorNameCard();
  void Reset();
  // INLINED
  void SetName(std::string value) {
    this->name = value;
  }
  // INLINED
  void SetValue1(std::string value) {
    this->partyName = value;
  }
  // INLINED
  void SetValue2(std::string value) {
    this->guildName = value;
  }
  // INLINED
  __forceinline void SetValue3(std::string value) {
    this->clanName = value;
  }
  // INLINED
  __forceinline void SetValue4(std::string value) {
    this->title = value;
  }
  // INLINED
  void SetValue5(std::string value, std::uint32_t number) {
    this->displayName = value;
    this->groupIconId = number;
  }

  /* 0004 */ std::string name;
  /* 001C */ std::string partyName;
  /* 0034 */ std::string guildName;
  /* 004C */ std::string clanName;
  /* 0064 */ std::string title;
  /* 007C */ std::string displayName;
  /* 0094 */ std::uint32_t groupIconId;
  /* 0098 */ unsigned char received;
};

class CActorNameCardHolder {
public:
  CActorNameCardHolder();
  virtual ~CActorNameCardHolder();
  std::string Name(std::uint32_t id);
  void SetName(std::uint32_t id, std::string name);
  void SetValue3(std::uint32_t id, std::string value);
  void SetValue4(std::uint32_t id, std::string value);
  const CActorNameCard& Lookup(std::uint32_t id);
  std::size_t Save(std::vector<char>& output);
  std::size_t Load(const std::vector<char>& input);
  void Set(std::uint32_t id, std::string name, std::string value1, std::string value2, std::string value3, std::string value4, std::string value5,
           std::uint32_t value);
  std::uint32_t FindId(std::string name);
  std::size_t Erase(std::uint32_t id);
  bool Contains(std::uint32_t id) const;
  void Clear();
  void ProcessRequests();

  struct Request {
    /* 0000 */ std::uint32_t id;
    /* 0004 */ std::uint32_t tick;
  };

  /* 0004 */ std::map<std::uint32_t, CActorNameCard> cards;
  /* 000C */ std::deque<std::uint32_t> pending;
  /* 0020 */ std::deque<Request> requests;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CActorNameCard) == 156);
static_assert(sizeof(CActorNameCardHolder) == 52);
#endif

#endif
