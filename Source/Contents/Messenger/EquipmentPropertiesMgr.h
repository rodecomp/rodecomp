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

enum EquipmentPropertyAccumulation : int {
  EQUIP_PROPERTY_SUM = 0,
  EQUIP_PROPERTY_ONE = 1,
  EQUIP_PROPERTY_MAX = 2,
  EQUIP_PROPERTY_SUM_INDEX_AND_VALUE = 3,
  EQUIP_PROPERTY_NONNEGATIVE_MAX = 4,
};

enum EquipmentExpansionKind : int {
  EQUIP_EXPANSION_PET = 0,
  EQUIP_EXPANSION_RUNE = 1,
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
  enum EquipmentFormatFunctionHash : unsigned int {
    HASH_GET_NUMBER = 0x16716DCE,
    HASH_GET_SKILL_NAME = 0x78BA6385,
    HASH_GET_SKILL_GROUP = 0x8F918611,
    HASH_GET_ADD_DAMAGE_SKILL_NAME = 0xA2AF91ED,
  };

  enum EquipmentStatVariableHash : unsigned int {
    HASH_VAR_MAXHPAMOUNT = 0x617FECDF,
    HASH_VAR_MAXSPAMOUNT = 0x8854128A,
    HASH_VAR_STRAMOUNT = 0x4BA88EBA,
    HASH_VAR_AGIAMOUNT = 0x66C4AA72,
    HASH_VAR_VITAMOUNT = 0x1C3E3594,
    HASH_VAR_INTAMOUNT = 0xCF35A0EC,
    HASH_VAR_DEXAMOUNT = 0x6E5FE302,
    HASH_VAR_LUKAMOUNT = 0xEDABA34D,
    HASH_VAR_ATTPOWER = 0x35175B63,
    HASH_VAR_ITEMDEFPOWER = 0xBDC4C598,
    HASH_VAR_MDEFPOWER = 0xEFCBA0D6,
    HASH_VAR_ATTMPOWER = 0xD1114E90,
    HASH_VAR_POWAMOUNT = 0xD1CD4317,
    HASH_VAR_STAAMOUNT = 0x2F0A85E9,
    HASH_VAR_WISAMOUNT = 0x43C069D4,
    HASH_VAR_SPLAMOUNT = 0xCCC3CEB0,
    HASH_VAR_CONAMOUNT = 0x32AF6341,
    HASH_VAR_CRTAMOUNT = 0xC5524E6A,
  };

  enum EquipmentKindHash : unsigned int {
    HASH_NORMAL_WEAPON = 0xBDE09DB7,
    HASH_NORMAL_ARMOR = 0x78974D0E,
    HASH_SHADOW_WEAPON = 0x7A2F6FD4,
    HASH_SHADOW_ARMOR = 0xA515CF8B,
  };

  enum EquipmentGradeCategoryHash : unsigned int {
    HASH_GRADE_NORMAL = 0x95E1BB50,
    HASH_GRADE_MAGIC = 0x2B3A7568,
    HASH_GRADE_RARE = 0xBB80A3D1,
    HASH_GRADE_EPIC = 0xBB79C188,
    HASH_GRADE_UNIQUE = 0xA61EE35E,
    HASH_GRADE_LEGEND = 0x907C4C76,
    HASH_GRADE_ANCIENT = 0x65FD5769,
    HASH_GRADE_AWAKEN = 0x781614BE,
  };

  enum EquipmentRefineFieldHash : unsigned int {
    HASH_REFINING_WEAPON_ATK_MULTIPLE = 0x09B447CA,
    HASH_REFINING_WEAPON_MATK_MULTIPLE = 0x27A83F77,
  };

  enum EquipmentRefiningValueKindHash : unsigned int {
    HASH_WEAPON_ATK = 0xC3B9E8CE,
    HASH_WEAPON_ADV_ATK = 0x48A75AE8,
    HASH_ARMOR_DEF = 0x18585774,
    HASH_ARMOR_RES = 0x1858930F,
  };

  enum EquipmentInfoKindHash : unsigned int {
    HASH_ARMOR = 0x0F1AAAA6,
    HASH_MWEAPON = 0x47F48D1C,
    HASH_RWEAPON = 0xC8D7BCE1,
    HASH_AMMO = 0x7C941E6F,
    HASH_SPECIAL = 0xA4C66E86,
  };

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
    /* 0084 */ std::multiset<unsigned int> activeItems;
    /* 008C */ std::map<unsigned int, EquipmentPropertyGroup> propertyGroups;
    /* 0094 */ std::unordered_map<unsigned int, CEquipmentInfo> equipmentInfo;
    /* 00B4 */ std::unordered_map<unsigned int, unsigned int> statVariables;
    /* 00D4 */ std::unordered_map<unsigned int, std::list<unsigned int>> m_itemCombinations;
    /* 00F4 */ std::unordered_map<unsigned int, std::list<unsigned int>> m_combinationItems;
    /* 0114 */ std::unordered_map<unsigned int, std::list<unsigned int>> skillGroups;
    /* 0134 */ std::unordered_map<unsigned int, unsigned int> constantValues;
    /* 0154 */ std::unordered_map<std::uint64_t, EquipmentPropertyDefinition*> m_definitionsByName;
    /* 0174 */ std::unordered_map<int, EquipmentPropertyDefinition*> randomOptionDefinitions;
    /* 0194 */ std::set<std::uint64_t> functionPairs;
    /* 019C */ std::unordered_map<int, EquipmentValueC> displayValues;
    /* 01BC */ std::unordered_map<int, std::unordered_map<unsigned int, std::unordered_map<unsigned int, float>>, CEquipmentValueHash> levelRates;
    /* 01DC */ std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, EquipmentEntryB, CEquipmentValueHash>, CEquipmentValueHash>, CEquipmentValueHash> categoryValues;

    static CEquipmentPropertiesMgr* GetInstance();

    CEquipmentPropertiesMgr();
    virtual ~CEquipmentPropertiesMgr();
    void Clear();
    void SetWindow(UIWindow* window);
    const unsigned int& GetConstantValue(const unsigned int& key);
    CEquipment* GetCurrentEquipment();
    CEquipmentWearable* FindProperty1(const int& key);
    CEquipmentInfo* FindEquipmentInfo(const unsigned int& key);
    CLuaInstance& GetScript();
    EquipmentPropertyDefinition* FindRandomOptionDefinition(const int& key);
    std::list<unsigned int>* FindSkillGroup(const unsigned int& key);
    std::unordered_map<int, EquipmentValueC>& GetDisplayValues();
    bool HasFunctionPair(const std::uint64_t& key);
    EquipmentPropertyDefinition* FindDefinition(const std::uint64_t& key);
    int GetRefiningBonus(const int& first, const unsigned int& second, const unsigned int& third);
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
