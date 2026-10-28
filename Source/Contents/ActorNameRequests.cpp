#include "Contents/ActorNameCard.h"

#include <algorithm>

// OFFSET: 0x005BFD10
// FUNCTION: fn_03500
const CActorNameCard& CActorNameCardHolder::Lookup(std::uint32_t id) {
  const auto found = this->cards.find(id);
  if (found != this->cards.end() && found->second.received == 1) {
    return found->second;
  }

  if (std::find(this->pending.begin(), this->pending.end(), id) == this->pending.end()) {
    this->pending.push_back(id);
  }

  // GLOBAL: gl_24526
  static CActorNameCard empty;
  return empty;
}
