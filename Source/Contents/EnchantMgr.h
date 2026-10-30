#ifndef RAGNAROK_SOURCE_CONTENTS_ENCHANTMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ENCHANTMGR_H

#include "Script/LuaInstance.h"
#include "Utility/Singleton.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct EnchantRandomUpgradeInfo {
  /* 0000 */ uint64_t unknown_0000;
  /* 0008 */ std::map<int, int> unknown_0008;
  /* 0010 */ std::map<int, uint64_t> rates;
};

struct EnchantOptionInfo {
  /* 0000 */ int64_t unknown_0000;
  /* 0008 */ std::map<int, int> unknown_0008;
  /* 0010 */ uint64_t successRate;
  /* 0018 */ std::map<uint32_t, uint64_t> gradeBonus;
  /* 0020 */ std::map<int, std::array<uint8_t, 0x8>> unknown_0020;
  /* 0028 */ std::map<int, std::array<uint8_t, 0x14>> unknown_0028;
  /* 0030 */ std::map<int, EnchantRandomUpgradeInfo> randomUpgrades;
  /* 0038 */ std::map<int, std::array<uint8_t, 0x8>> unknown_0038;
  /* 0040 */ bool unknown_0040 = false;
};

struct EnchantInfo {
  /* 0000 */ std::vector<int> targetItems;
  /* 000C */ uint32_t condition1;
  /* 0010 */ uint32_t condition2;
  /* 0014 */ bool approveRandomOption;
  /* 0018 */ std::vector<int> slots;
  /* 0024 */ uint8_t unknown_0024[0x1C];
  /* 0040 */ std::map<int, int> unknown_0040;
  /* 0048 */ std::string caution;
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
  /* 0004 */ std::map<uint64_t, EnchantInfo> m_enchants;
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

int L_AddRandomUpgradeEnchant(lua_State* L);
int L_AddTargetItem(lua_State* L);
int L_ApproveRandomOption(lua_State* L);
int L_GetSlotCount(lua_State* L);
int L_SetCaution(lua_State* L);
int L_SetCondition(lua_State* L);
int L_SetGradeBonus(lua_State* L);
int L_SetSlotOrder(lua_State* L);
int L_SetSuccessRate(lua_State* L);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EnchantRandomUpgradeInfo) == 0x18);
static_assert(offsetof(EnchantRandomUpgradeInfo, unknown_0008) == 0x08);
static_assert(offsetof(EnchantRandomUpgradeInfo, rates) == 0x10);
static_assert(sizeof(EnchantOptionInfo) == 0x48);
static_assert(offsetof(EnchantOptionInfo, unknown_0008) == 0x08);
static_assert(offsetof(EnchantOptionInfo, successRate) == 0x10);
static_assert(offsetof(EnchantOptionInfo, gradeBonus) == 0x18);
static_assert(offsetof(EnchantOptionInfo, unknown_0020) == 0x20);
static_assert(offsetof(EnchantOptionInfo, unknown_0028) == 0x28);
static_assert(offsetof(EnchantOptionInfo, randomUpgrades) == 0x30);
static_assert(offsetof(EnchantOptionInfo, unknown_0038) == 0x38);
static_assert(offsetof(EnchantOptionInfo, unknown_0040) == 0x40);
static_assert(sizeof(EnchantInfo) == 0x68);
static_assert(offsetof(EnchantInfo, condition1) == 0x0C);
static_assert(offsetof(EnchantInfo, condition2) == 0x10);
static_assert(offsetof(EnchantInfo, approveRandomOption) == 0x14);
static_assert(offsetof(EnchantInfo, slots) == 0x18);
static_assert(offsetof(EnchantInfo, unknown_0040) == 0x40);
static_assert(offsetof(EnchantInfo, caution) == 0x48);
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
