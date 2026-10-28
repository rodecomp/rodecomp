#include "Contents/ActorNameCard.h"

// OFFSET: 0x005BF870
// FUNCTION: fn_03497
std::size_t CActorNameCardHolder::Erase(std::uint32_t id) {
  return this->cards.erase(id);
}

// OFFSET: 0x005BF890

// OFFSET: 0x005C0190
