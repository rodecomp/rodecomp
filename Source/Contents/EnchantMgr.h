#ifndef RAGNAROK_SOURCE_CONTENTS_ENCHANTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ENCHANTMGR_H

#include "Script/LuaInstance.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

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
  /* 0000 */ uint8_t unknown_0000[0x18];
  /* 0018 */ std::vector<int> slots;
  /* 0024 */ uint8_t unknown_0024[0x3C];
  /* 0060 */ std::map<uint32_t, EnchantOptionInfo> options;
};

struct EnchantRequest {
  /* 0000 */ int16_t packetType;
  /* 0008 */ uint64_t group;
  /* 0010 */ int16_t index;
  /* 0012 */ int16_t slot;
};

class CEnchantMgr : public Singleton<CEnchantMgr> {
public:
  /* 0004 */ std::map<EnchantKey, EnchantInfo> m_enchants;
  /* 000C */ CLuaInstance m_lua;
  /* 0014 */ int32_t m_unknown_0014;
  /* 0018 */ EnchantRequest m_request;
  /* 0030 */ bool m_unknown_0030;
  /* 0031 */ uint8_t m_unknown_0031[7];

  CEnchantMgr();
  virtual ~CEnchantMgr();

  void SendCloseEnchant();
  void RequestPerfectEnchant(uint64_t group, int16_t index, int16_t slot, int32_t itemId);
  void RequestRandomEnchant(uint64_t group, int16_t index, int16_t slot);
  void RequestResetEnchant(uint64_t group, int16_t index);
  void RequestUpgradePerfectEnchant(uint64_t group, int16_t index, int16_t slot, int32_t itemId);
  void RequestUpgradeEnchant(uint64_t group, int16_t index, int16_t slot);
  void RequestUpgradeRandomEnchant(uint64_t group, int16_t index, int16_t slot);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EnchantKey) == 8);
static_assert(sizeof(EnchantInfo) == 0x68);
static_assert(offsetof(EnchantInfo, options) == 0x60);
static_assert(sizeof(CEnchantMgr) == 0x38);
static_assert(offsetof(CEnchantMgr, m_enchants) == 0x04);
static_assert(offsetof(CEnchantMgr, m_lua) == 0x0C);
static_assert(offsetof(CEnchantMgr, m_unknown_0014) == 0x14);
static_assert(sizeof(EnchantRequest) == 0x18);
static_assert(offsetof(EnchantRequest, group) == 0x08);
static_assert(offsetof(EnchantRequest, index) == 0x10);
static_assert(offsetof(EnchantRequest, slot) == 0x12);
static_assert(offsetof(CEnchantMgr, m_request) == 0x18);
static_assert(offsetof(CEnchantMgr, m_unknown_0030) == 0x30);
#endif

#endif
