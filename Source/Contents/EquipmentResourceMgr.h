#ifndef RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H

#include <string>

class EquipmentResourceMgr {
public:
  std::string GetBodyRacePrefix(int job);
  std::string GetRobeSprName(int robe, int);
  std::string GetHeadName(int type, int job, int sex, int* head);
  std::string GetHeadPalName(int head, int job, int sex, int palette);
};

extern EquipmentResourceMgr* g_equipmentResourceMgr;
extern const char* g_equipmentResourceExtensions[2];

#endif
