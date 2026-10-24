#ifndef RAGNAROK_SOURCE_CONTENTS_EMOTIONHOTKEYS_H
#define RAGNAROK_SOURCE_CONTENTS_EMOTIONHOTKEYS_H

#include <cstdint>
#include <json/value.h>
#include <string>
#include <vector>

class CRenderSystemBase;

// Recovered contiguous subobject of the original client state, not a second
// global owner: current at owner+0xFF8, saved at owner+0x1004 on x86.
struct EmotionHotkeys {
  std::vector<std::string> current;
  std::vector<std::string> saved;

  void Load(Json::Value& data);
  bool HasChanges() const;
  void Save(std::uint32_t accountId, CRenderSystemBase* renderSystem) const;
  bool LoadDefaults(std::uint32_t accountId, bool languagePrefix, int serviceType, std::uint16_t language);
  void Export(Json::Value& data) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EmotionHotkeys) == 24);
#endif

#endif
