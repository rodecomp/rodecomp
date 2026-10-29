#ifndef RAGNAROK_SOURCE_CONTENTS_ENCHANTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ENCHANTMGR_H

#include "Script/LuaInstance.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <cstdint>
#include <map>

struct EnchantKey {
  /* 0000 */ int32_t first;
  /* 0004 */ int32_t second;

  // INLINED
  bool operator<(const EnchantKey& other) const {
    if (second != other.second) {
      return second < other.second;
    }
    return first < other.first;
  }
};

struct EnchantOptionInfo {
  /* 0000 */ int64_t unknown_0000;
  /* 0008 */ uint8_t unknown_0008[0x28];
};

struct EnchantInfo {
  /* 0000 */ uint8_t unknown_0000[0x60];
  /* 0060 */ std::map<uint32_t, EnchantOptionInfo> options;
};

class CEnchantMgr : public Singleton<CEnchantMgr> {
public:
  /* 0004 */ std::map<EnchantKey, EnchantInfo> m_enchants;
  /* 000C */ CLuaInstance m_lua;
  /* 0014 */ int32_t m_unknown_0014;
  /* 0018 */ int16_t m_unknown_0018;
  /* 001A */ int16_t m_unknown_001A;
  /* 001C */ int32_t m_unknown_001C;
  /* 0020 */ int32_t m_unknown_0020;
  /* 0024 */ int32_t m_unknown_0024;
  /* 0028 */ int16_t m_unknown_0028;
  /* 002A */ int16_t m_unknown_002A;
  /* 002C */ int32_t m_unknown_002C;
  /* 0030 */ bool m_unknown_0030;
  /* 0031 */ uint8_t m_unknown_0031[7];

  CEnchantMgr();
  virtual ~CEnchantMgr();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EnchantKey) == 8);
static_assert(sizeof(EnchantInfo) == 0x68);
static_assert(offsetof(EnchantInfo, options) == 0x60);
static_assert(sizeof(CEnchantMgr) == 0x38);
static_assert(offsetof(CEnchantMgr, m_enchants) == 0x04);
static_assert(offsetof(CEnchantMgr, m_lua) == 0x0C);
static_assert(offsetof(CEnchantMgr, m_unknown_0014) == 0x14);
static_assert(offsetof(CEnchantMgr, m_unknown_0018) == 0x18);
static_assert(offsetof(CEnchantMgr, m_unknown_0020) == 0x20);
static_assert(offsetof(CEnchantMgr, m_unknown_0024) == 0x24);
static_assert(offsetof(CEnchantMgr, m_unknown_0028) == 0x28);
static_assert(offsetof(CEnchantMgr, m_unknown_002A) == 0x2A);
static_assert(offsetof(CEnchantMgr, m_unknown_0030) == 0x30);
#endif

#endif
