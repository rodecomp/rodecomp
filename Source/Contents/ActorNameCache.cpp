#include "Contents/ActorNameCard.h"

#include <cstring>

// OFFSET: 0x005BFA60
// FUNCTION: fn_03499
std::size_t CActorNameCardHolder::Save(std::vector<char>& output) {
  if (this->cards.empty()) {
    return 0;
  }

  std::vector<char> buffer(0x400);
  std::size_t offset = 4;
  *reinterpret_cast<std::size_t*>(&buffer[0]) = this->cards.size();

  for (const auto& entry : this->cards) {
    const CActorNameCard& card = entry.second;
    const std::size_t needed = offset + card.name.size() + card.unknown_001C.size() + card.unknown_0034.size() + card.unknown_004C.size() + 0x14;
    if (buffer.size() < needed) {
      buffer.resize(needed + 1);
    }

    *reinterpret_cast<std::uint32_t*>(&buffer[offset]) = entry.first;

    int length = static_cast<int>(card.name.size());
    *reinterpret_cast<int*>(&buffer[offset + 4]) = length;
    offset += 8;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.name.c_str(), length);
      offset += length;
    }

    length = static_cast<int>(card.unknown_001C.size());
    *reinterpret_cast<int*>(&buffer[offset]) = length;
    offset += 4;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.unknown_001C.c_str(), length);
      offset += length;
    }

    length = static_cast<int>(card.unknown_0034.size());
    *reinterpret_cast<int*>(&buffer[offset]) = length;
    offset += 4;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.unknown_0034.c_str(), length);
      offset += length;
    }

    length = static_cast<int>(card.unknown_004C.size());
    *reinterpret_cast<int*>(&buffer[offset]) = length;
    offset += 4;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.unknown_004C.c_str(), length);
      offset += length;
    }
  }

  output.assign(buffer.begin(), buffer.begin() + offset);
  return this->cards.size();
}

// OFFSET: 0x005C05B0
// FUNCTION: fn_03506
std::size_t CActorNameCardHolder::Load(const std::vector<char>& input) {
  if (input.empty()) {
    return 0;
  }

  this->cards.clear();
  const int count = *reinterpret_cast<const int*>(input.data());
  std::string text;
  std::size_t offset = 4;
  for (int index = 0; index < count; ++index) {
    const std::uint32_t id = *reinterpret_cast<const std::uint32_t*>(&input[offset]);
    this->cards[id].Reset();

    int length = *reinterpret_cast<const int*>(&input[offset + 4]);
    offset += 8;
    if (length > 0) {
      this->cards[id].name.assign(&input[offset], length);
      offset += length;
    }

    length = *reinterpret_cast<const int*>(&input[offset]);
    offset += 4;
    if (length > 0) {
      this->cards[id].unknown_001C.assign(&input[offset], length);
      offset += length;
    }

    length = *reinterpret_cast<const int*>(&input[offset]);
    offset += 4;
    if (length > 0) {
      this->cards[id].unknown_0034.assign(&input[offset], length);
      offset += length;
    }

    length = *reinterpret_cast<const int*>(&input[offset]);
    offset += 4;
    if (length > 0) {
      this->cards[id].unknown_004C.assign(&input[offset], length);
      offset += length;
    }
  }

  return this->cards.size();
}
