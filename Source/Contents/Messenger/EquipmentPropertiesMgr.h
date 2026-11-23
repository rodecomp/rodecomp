#ifndef RAGNAROK_SOURCE_CONTENTS_MESSENGER_EQUIPMENTPROPERTIESMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MESSENGER_EQUIPMENTPROPERTIESMGR_H

#include "Contents/Messenger/EquipmentPropertyStore.h"
#include "Script/LuaInstance.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class UIWindow;
struct ITEM_INFO;

struct EquipmentValueA {
  /* 0000 */ int value;
};

struct EquipmentValueC {
  /* 0000 */ int value;
};

struct EquipmentEntryA {
  /* 0000 */ int value;
};

struct EquipmentEntryB {
  /* 0000 */ int value;
};

struct EquipmentPropertyDefinition {
  /* 0000 */ unsigned int group = 0;
  /* 0004 */ unsigned int id = 0;
  /* 0008 */ std::string formatText;
  /* 0020 */ std::vector<std::string> functions;
  /* 002C */ int functionMode = 0;
  /* 0030 */ int mode;
  /* 0034 */ std::vector<std::string> signLabels;
  /* 0040 */ int separatorMode = 0;
  /* 0044 */ std::string separator;
  /* 005C */ std::map<int, int> formatOptions;
  /* 0064 */ std::string uniqueGroup;
  /* 007C */ int uniqueKeyMode = 0;

  bool IsTwoDimensional() const;
};

struct EquipmentPropertyGroup {
  /* 0000 */ std::string name;
  /* 0018 */ std::map<std::int64_t, EquipmentPropertyDefinition*> definitions;
  // INLINED
  explicit EquipmentPropertyGroup(const char* name) : name(name) {
  }
  ~EquipmentPropertyGroup();
  std::string& GetName();
  std::map<std::int64_t, EquipmentPropertyDefinition*>& GetDefinitions();
};

namespace EquipmentProperties {
  class CEquipment;
  class CEquipmentWearable;
  class CEquipmentExpansion;
  class CEquipmentCombiitem;

  struct CEquipmentValueHash {
    std::size_t operator()(int key) const {
      return key;
    }
  };

  struct CEquipmentInfo;

  class CEquipmentPropertiesMgr {
  public:
    /* 0004 */ CLuaInstance script;
    /* 000C */ bool loadValid;
    /* 000D */ bool m_dirty = false;
    /* 0010 */ int unknown_0010;
    /* 0014 */ int shopId;
    /* 0018 */ EquipmentPropertyStore* store;
    /* 001C */ CEquipment* currentEquipment;
    /* 0020 */ UIWindow* notifyWindow;
    /* 0024 */ std::unordered_map<int, CEquipmentWearable*> properties1;
    /* 0044 */ std::unordered_map<unsigned int, CEquipmentExpansion*> properties2;
    /* 0064 */ std::unordered_map<unsigned int, CEquipmentCombiitem*> properties3;
    /* 0084 */ std::multiset<unsigned long> activeItems;
    /* 008C */ std::map<unsigned int, EquipmentPropertyGroup> propertyGroups;
    /* 0094 */ std::unordered_map<unsigned int, CEquipmentInfo> equipmentInfo;
    /* 00B4 */ std::unordered_map<int, EquipmentValueA> statVariables;
    /* 00D4 */ std::unordered_map<unsigned int, std::list<unsigned int>> m_itemCombinations;
    /* 00F4 */ std::unordered_map<unsigned int, std::list<unsigned int>> m_combinationItems;
    /* 0114 */ std::unordered_map<unsigned int, std::list<unsigned int>> skillGroups;
    /* 0134 */ std::unordered_map<int, EquipmentValueA> constantValues;
    /* 0154 */ std::unordered_map<std::uint64_t, EquipmentPropertyDefinition*> m_definitionsByName;
    /* 0174 */ std::unordered_map<int, EquipmentPropertyDefinition*> randomOptionDefinitions;
    /* 0194 */ std::set<std::uint64_t> functionPairs;
    /* 019C */ std::unordered_map<int, EquipmentValueC> displayValues;
    /* 01BC */ std::unordered_map<int, std::unordered_map<int, std::unordered_map<unsigned int, float>>, CEquipmentValueHash> levelRates;
    /* 01DC */ std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, EquipmentEntryB, CEquipmentValueHash>, CEquipmentValueHash>, CEquipmentValueHash> categoryValues;

    static CEquipmentPropertiesMgr* GetInstance();

    CEquipmentPropertiesMgr();
    virtual ~CEquipmentPropertiesMgr();
    void Clear();
    void SetWindow(UIWindow* window);
    const int& GetConstantValue(const int& key);
    CEquipment* GetCurrentEquipment();
    CEquipmentWearable* FindProperty1(const int& key);
    CEquipmentInfo* FindEquipmentInfo(const unsigned int& key);
    CLuaInstance& GetScript();
    EquipmentPropertyDefinition* FindRandomOptionDefinition(const int& key);
    std::list<unsigned int>* FindSkillGroup(const unsigned int& key);
    std::unordered_map<int, EquipmentValueC>& GetDisplayValues();
    bool HasFunctionPair(const std::uint64_t& key);
    EquipmentPropertyDefinition* FindDefinition(const std::uint64_t& key);
    int GetRefiningBonus(const int& first, const int& second, const unsigned int& third);
    __declspec(noinline) void SetCurrentEquipment(CEquipment* equipment);
    void RefreshPretend();
    __declspec(noinline) void RefreshShop();
    void Rebuild();
    void GetDescriptions(const int& group, std::list<std::string>& lines);
    void ApplyProperty(const char* name, lua_State* state, const int& count, const int& valuePosition, const int& indexPosition, const int& keyPosition);
    void EquipItem(const int& itemId, bool notify);
    void UnequipItem(const int& itemId, bool notify);
    void UnequipWearable(const int& mask);
    __declspec(noinline) void EquipWearable(const ITEM_INFO* item, bool notify);
    void AddExpansion(const int& kind, const int& itemId, bool notify);
    void RemoveExpansion(const int& kind, const int& itemId);
    void RefreshExpansion(const int& kind, const int& itemId);
    void RefreshWearable(const std::uint32_t& key);
    int GetGradeBonus(const int& first, const int& second, const int& third);
    void LoadADSkill();
    void LoadCombiitem();
    void LoadEnumVar();
    void LoadRefiningBonus();
    void LoadGradeBonus();
    void LoadIgnoreEquipmentProperties();
    void LoadSkillGroup();
    void LoadEquipmentPropertiesOrder();
    bool InitializeLua();
    void LoadItems();
  };
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentPropertyDefinition) == 0x80);
static_assert(sizeof(EquipmentPropertyGroup) == 0x20);
static_assert(offsetof(EquipmentProperties::CEquipmentPropertiesMgr, properties1) == 0x24);
static_assert(sizeof(EquipmentProperties::CEquipmentPropertiesMgr) == 0x1FC);
#endif

#endif
