#include "Contents/Navigation/NavigationManager.h"

#include <cstring>

extern template std::string::basic_string(const std::string&);

// OFFSET: 0x00B974D0
// FUNCTION: fn_19599
bool CNavigationManager::LoadStates(std::vector<char> packet) {
  if (packet.begin() == packet.end()) {
    return false;
  }

  const int count = *reinterpret_cast<const int*>(packet.data());
  std::size_t offset = sizeof(int);
  char map[0x14] = {};
  this->states.clear();

  for (int i = 0; i < count; ++i) {
    NavigationTarget target;
    std::memcpy(&target.type, packet.data() + offset, sizeof(target.type));
    offset += sizeof(target.type);
    std::memset(map, 0, sizeof(map));
    std::memcpy(map, packet.data() + offset, sizeof(map));
    offset += sizeof(map);
    std::memcpy(&target.x, packet.data() + offset, sizeof(target.x));
    offset += sizeof(target.x);
    std::memcpy(&target.y, packet.data() + offset, sizeof(target.y));
    offset += sizeof(target.y);
    int nameLength;
    std::memcpy(&nameLength, packet.data() + offset, sizeof(nameLength));
    offset += sizeof(nameLength);

    if (nameLength > 0) {
      target.name.resize(nameLength, '\0');
      std::memcpy(&target.name[0], packet.data() + offset, nameLength);
      offset += nameLength;
    }

    CNaviInfo* info = this->FindObject(map);
    if (info != nullptr) {
      target.object = info;
      this->states.push_back(target);
    }
  }

  return true;
}
