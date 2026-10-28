#include "Contents/NavigationManager.h"

#include <cstring>

#pragma pack(push, 1)
struct NavigationStateRecord {
  /* 0000 */ int type;
  /* 0004 */ char map[0x14];
  /* 0018 */ int x;
  /* 001C */ int y;
  /* 0020 */ int nameLength;
};
#pragma pack(pop)

// OFFSET: 0x00B974D0
// FUNCTION: fn_19599
bool CNavigationManager::LoadStates(std::vector<char> packet) {
  if (packet.begin() == packet.end()) {
    return false;
  }

  const int count = *reinterpret_cast<const int*>(packet.data());
  this->states.clear();

  std::size_t offset = sizeof(int);
  for (int i = 0; i < count; ++i) {
    NavigationTarget target;
    NavigationStateRecord record;
    std::memcpy(&record, packet.data() + offset, sizeof(record));
    target.type = record.type;
    target.x = record.x;
    target.y = record.y;
    offset += sizeof(record);

    if (record.nameLength > 0) {
      target.name.resize(record.nameLength, '\0');
      std::memcpy(&target.name[0], packet.data() + offset, record.nameLength);
      offset += record.nameLength;
    }

    CNaviInfo* info = this->FindObject(record.map);
    if (info != nullptr) {
      target.object = info;
      this->states.push_back(target);
    }
  }

  return true;
}
