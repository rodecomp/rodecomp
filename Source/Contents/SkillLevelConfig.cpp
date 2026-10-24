#include "Contents/SkillLevelConfig.h"

#include <cstdint>

// OFFSET: 0x005E6A10
// FUNCTION: fn_03930
void LoadUseSkillLevels(Json::Value& data, int* levels) {
  Json::Value values = data["UseSkillInfo"];
  if (!values.isNull()) {
    for (auto it = values.begin(); it != values.end(); ++it) {
      Json::Value& value = *it;
      if (value.isObject()) {
        int id = value.get("SKID", 0).asInt();
        int level = value.get("SKLv", 0).asInt();
        if (static_cast<std::uint32_t>(id) - 1 <= 0x1F3D) {
          levels[id] = level;
        }
      }
    }
  }
}

// OFFSET: 0x005E63B0
// FUNCTION: fn_03928
void LoadHomunculusSkillLevels(Json::Value& data, int* levels) {
  Json::Value values = data["HomunSkillInfo"];
  if (!values.isNull()) {
    for (auto it = values.begin(); it != values.end(); ++it) {
      Json::Value& value = *it;
      if (value.isObject()) {
        int id = value.get("HomunSKID", 0).asInt();
        int level = value.get("HomunSKLv", 0).asInt();
        // Original subtracts in 32 bits, then signed-compares against -1;
        // there is no upper bound check on the caller's storage.
        std::uint32_t index = static_cast<std::uint32_t>(id) - 8000;
        if (static_cast<std::int32_t>(index) > -1) {
          levels[index] = level;
        }
      }
    }
  }
}
