#ifndef RAGNAROK_SOURCE_CONTENTS_MESSENGER_EQUIPMENTPROPERTYSTORE_H
#define RAGNAROK_SOURCE_CONTENTS_MESSENGER_EQUIPMENTPROPERTYSTORE_H

#include <cstddef>
#include <map>
#include <string>
#include <unordered_map>

namespace EquipmentProperties {
class CEquipmentProperties;
}

struct EquipmentPropertyStore {
  /* 0000 */ std::map<unsigned int, std::map<unsigned int, EquipmentProperties::CEquipmentProperties*>> properties;
  /* 0008 */ std::unordered_map<std::string, std::string> names;

  EquipmentPropertyStore();
  ~EquipmentPropertyStore();
  void Clear();
  std::map<unsigned int, std::map<unsigned int, EquipmentProperties::CEquipmentProperties*>>& GetProperties();
  int GetCount();
  std::unordered_map<std::string, std::string>& GetNames();
  EquipmentPropertyStore& operator+=(EquipmentPropertyStore& other);
  EquipmentPropertyStore& operator+=(EquipmentProperties::CEquipmentProperties* property);
  void RemoveDuplicateNames();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentPropertyStore) == 0x28);
static_assert(offsetof(EquipmentPropertyStore, names) == 0x8);
#endif

#endif
