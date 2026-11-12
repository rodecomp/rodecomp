#include "Contents/Actors/ActorNameCard.h"

#include <algorithm>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

// OFFSET: 0x005BF870
// FUNCTION: fn_03497
std::size_t CActorNameCardHolder::Erase(std::uint32_t id) {
  return this->cards.erase(id);
}

// OFFSET: 0x005BF890
// FUNCTION: fn_03498
std::uint32_t CActorNameCardHolder::FindId(std::string name) {
  auto it = std::find_if(this->cards.begin(), this->cards.end(), [name](std::pair<const std::uint32_t, CActorNameCard> entry) {
    return entry.second.name == name;
  });

  return it != this->cards.end() ? it->first : 0;
}

// OFFSET: 0x005C0190
// FUNCTION: fn_03503
bool CActorNameCardHolder::Contains(std::uint32_t id) const {
  return this->cards.end() != this->cards.find(id);
}
