#include "Contents/ActorNameCard.h"

// FUNCTION: fn_03484
CActorNameCard::CActorNameCard() : unknown_0094(0), received(0) {
}

// FUNCTION: fn_03485
CActorNameCard::~CActorNameCard() = default;

// OFFSET: 0x005BEC80
// FUNCTION: fn_03487
void CActorNameCard::Reset() {
  this->name.clear();
  this->unknown_001C.clear();
  this->unknown_0034.clear();
  this->unknown_004C.clear();
  this->unknown_0064.clear();
  this->unknown_007C.clear();
  this->unknown_0094 = 0;
}

// OFFSET: 0x005BF080
// FUNCTION: fn_03491
CActorNameCardHolder::CActorNameCardHolder() = default;

// FUNCTION: fn_03493
CActorNameCardHolder::~CActorNameCardHolder() {
}

// OFFSET: 0x005BFEF0
// FUNCTION: fn_03501
std::string CActorNameCardHolder::Name(std::uint32_t id) {
  return this->cards[id].name;
}

// OFFSET: 0x005C0050
// FUNCTION: fn_03502
void CActorNameCardHolder::Clear() {
  this->cards.clear();
  this->pending.clear();
  this->requests.clear();
}

// OFFSET: 0x005C03B0
// FUNCTION: fn_03505
void CActorNameCardHolder::Set(std::uint32_t id, std::string name, std::string value1, std::string value2, std::string value3, std::string value4,
                             std::string value5, std::uint32_t value) {
  CActorNameCard& card = this->cards[id];
  card.SetName(name);
  card.SetValue1(value1);
  card.SetValue2(value2);
  card.SetValue3(value3);
  card.SetValue4(value4);
  card.SetValue5(value5, value);
  card.received = 1;
}

// OFFSET: 0x005C0E60
// FUNCTION: fn_03507
void CActorNameCardHolder::SetName(std::uint32_t id, std::string name) {
  this->cards[id].SetName(name);
}

// OFFSET: 0x005C0F00
// FUNCTION: fn_03508
void CActorNameCardHolder::SetValue3(std::uint32_t id, std::string value) {
  this->cards[id].SetValue3(value);
}

// OFFSET: 0x005C0FA0
// FUNCTION: fn_03509
void CActorNameCardHolder::SetValue4(std::uint32_t id, std::string value) {
  this->cards[id].SetValue4(value);
}
