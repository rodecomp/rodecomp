#include "Contents/ActorNameCard.h"

#include <cstring>

// OFFSET: 0x005BFA60
// FUNCTION: fn_03499
std::size_t CActorNameCardHolder::Save(std::vector<char>& output) {
  if (this->cards.size() == 0) {
    return 0;
  }

  std::vector<char> buffer(0x400);
  std::size_t offset = 4;
  *reinterpret_cast<std::size_t*>(&buffer[0]) = this->cards.size();

  for (auto it = this->cards.begin(); it != this->cards.end(); ++it) {
    const CActorNameCard& card = it->second;
    const std::size_t needed = card.name.size() + card.partyName.size() + card.guildName.size() + card.clanName.size() + 0x14 + offset;
    if (buffer.size() < needed) {
      buffer.resize(needed + 1, 0);
    }

    *reinterpret_cast<std::uint32_t*>(&buffer[offset]) = it->first;

    int length = static_cast<int>(card.name.size());
    *reinterpret_cast<int*>(&buffer[offset + 4]) = length;
    offset += 8;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.name.c_str(), length);
      offset += length;
    }

    length = static_cast<int>(card.partyName.size());
    *reinterpret_cast<int*>(&buffer[offset]) = length;
    offset += 4;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.partyName.c_str(), length);
      offset += length;
    }

    length = static_cast<int>(card.guildName.size());
    *reinterpret_cast<int*>(&buffer[offset]) = length;
    offset += 4;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.guildName.c_str(), length);
      offset += length;
    }

    length = static_cast<int>(card.clanName.size());
    *reinterpret_cast<int*>(&buffer[offset]) = length;
    offset += 4;
    if (length > 0) {
      std::memcpy(&buffer[offset], card.clanName.c_str(), length);
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
  int count;
  std::memcpy(&count, input.data(), sizeof(count));
  std::string text;
  std::size_t offset = 4;
  for (int index = 0; index < count; ++index) {
    std::uint32_t id;
    std::memcpy(&id, input.data() + offset, sizeof(id));
    this->cards[id].Reset();

    int length;
    std::memcpy(&length, input.data() + offset + 4, sizeof(length));
    offset += 8;
    if (length > 0) {
      this->cards[id].name.assign(&input[offset], length);
      offset += length;
    }

    std::memcpy(&length, input.data() + offset, sizeof(length));
    offset += 4;
    if (length > 0) {
      this->cards[id].partyName.assign(&input[offset], length);
      offset += length;
    }

    std::memcpy(&length, input.data() + offset, sizeof(length));
    offset += 4;
    if (length > 0) {
      this->cards[id].guildName.assign(&input[offset], length);
      offset += length;
    }

    std::memcpy(&length, input.data() + offset, sizeof(length));
    offset += 4;
    if (length > 0) {
      this->cards[id].clanName.assign(&input[offset], length);
      offset += length;
    }
  }

  return this->cards.size();
}
