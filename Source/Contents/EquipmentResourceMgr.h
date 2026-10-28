#ifndef RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H

#include <string>

class EquipmentResourceMgr {
public:
  std::string GetBodyRacePrefix(int job);
  std::string GetRobeSprName(int robe, int);
};

#endif
