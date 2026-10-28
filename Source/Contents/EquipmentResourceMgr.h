#ifndef RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H

#include <string>

class EquipmentResourceMgr {
public:
  std::string GetBodyRacePrefix(int job);
  std::string GetBodyResourceName(int type, int job, int sex, int costumeJob, int costume);
  std::string GetRobeSprName(int robe, int);
  std::string GetHeadName(int type, int job, int sex, int* head);
  std::string GetHeadPalName(int head, int job, int sex, int palette);
  std::string GetWeaponResourceName(int type, int job, int sex, int weapon, int weaponItem, int costume);
  std::string GetAccessoryResourceName(int type, int job, int sex, int head, int accessory);
  std::string GetWeaponEffectResourceName(int type, int job, int sex, int weapon, int weaponItem, int effect, int costume);
};

extern EquipmentResourceMgr* g_equipmentResourceMgr;
extern const char* g_equipmentResourceExtensions[2];

#endif
