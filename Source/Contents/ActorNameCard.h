#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORNAMECARD_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORNAMECARD_H

#include <cstdint>
#include <deque>
#include <map>
#include <string>

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
    this->unknown_001C = value;
  }
  // INLINED
  void SetValue2(std::string value) {
    this->unknown_0034 = value;
  }
  // INLINED
  void SetValue3(std::string value) {
    this->unknown_004C = value;
  }
  // INLINED
  void SetValue4(std::string value) {
    this->unknown_0064 = value;
  }
  // INLINED
  void SetValue5(std::string value, std::uint32_t number) {
    this->unknown_007C = value;
    this->unknown_0094 = number;
  }

  /* 0004 */ std::string name;
  /* 001C */ std::string unknown_001C;
  /* 0034 */ std::string unknown_0034;
  /* 004C */ std::string unknown_004C;
  /* 0064 */ std::string unknown_0064;
  /* 007C */ std::string unknown_007C;
  /* 0094 */ std::uint32_t unknown_0094;
  /* 0098 */ unsigned char received;
};

class CActorNameCardHolder {
public:
  CActorNameCardHolder();
  virtual ~CActorNameCardHolder();
  std::string Name(std::uint32_t id);
  void SetName(std::uint32_t id, std::string name);
  const CActorNameCard& Lookup(std::uint32_t id);
  void Set(std::uint32_t id, std::string name, std::string value1, std::string value2, std::string value3, std::string value4, std::string value5,
           std::uint32_t value);
  std::uint32_t FindId(std::string name);
  std::size_t Erase(std::uint32_t id);
  bool Contains(std::uint32_t id) const;
  void Clear();

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
