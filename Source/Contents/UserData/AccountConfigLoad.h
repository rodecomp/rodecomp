#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTCONFIGLOAD_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTCONFIGLOAD_H

#include <cstdint>

struct EmotionHotkeys;
class LuaScript;
class CRenderSystemBase;

struct AccountConfigLoad {
  EmotionHotkeys& emotions;
  LuaScript& script;
  const std::uint32_t& persistenceAccountId;
  CRenderSystemBase* const& renderSystem;
  const bool& languagePrefix;
  const int& serviceType;
  const std::uint16_t& language;
};

#endif
