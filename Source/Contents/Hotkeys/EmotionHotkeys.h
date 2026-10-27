#ifndef RAGNAROK_SOURCE_CONTENTS_HOTKEYS_EMOTIONHOTKEYS_H
#define RAGNAROK_SOURCE_CONTENTS_HOTKEYS_EMOTIONHOTKEYS_H

#include <cstddef>
#include <cstdint>
#include <json/value.h>
#include <string>
#include <vector>

class CRenderSystemBase;

struct EmotionHotkeys {
  /* 0FF8 */ std::vector<std::string> current;
  /* 1004 */ std::vector<std::string> saved;

  void Load(Json::Value& data);
  bool HasChanges() const;
  void Save(std::uint32_t accountId, CRenderSystemBase* renderSystem) const;
  bool LoadDefaults(std::uint32_t accountId, bool languagePrefix, int serviceType, std::uint16_t language);
  void Export(Json::Value& data) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EmotionHotkeys) == 24);
static_assert(offsetof(EmotionHotkeys, saved) == 12);
#endif

#endif
