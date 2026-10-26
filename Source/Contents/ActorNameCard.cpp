#include "Contents/ActorNameCard.h"



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
  {
    const std::string copy(name);
    card.name = copy;
  }
  {
    const std::string copy(value1);
    card.unknown_001C = copy;
  }
  {
    const std::string copy(value2);
    card.unknown_0034 = copy;
  }
  {
    const std::string copy(value3);
    card.unknown_004C = copy;
  }
  {
    const std::string copy(value4);
    card.unknown_0064 = copy;
  }
  {
    const std::string copy(value5);
    card.unknown_007C = copy;
    card.unknown_0094 = value;
  }
  card.received = 1;
}

// OFFSET: 0x005C0E60
// FUNCTION: fn_03507
void CActorNameCardHolder::SetName(std::uint32_t id, std::string name) {
  CActorNameCard& card = this->cards[id];
  const std::string copy(name);
  card.name = copy;
}
