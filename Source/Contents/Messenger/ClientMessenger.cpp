#include "Network/Packets/NoticeEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Items/ItemType.h"
#include "Contents/CombatTypes.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Jobs/JobId.h"
#include "Script/LuaInstance.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Buttons/UITextButton.h"
#include "Resource/World/TerrainResource.h"
#include "Contents/Actors/Pc.h"
#include "Client/Application/ErrorDialog.h"
#include "UI/CashShop/UIMileageWnd.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/World/World.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Events/SpecialPopupMgr.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Render/Text/EmbedFont.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <format>
#include <list>
#include <map>
#include <new>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

extern "C" {
#include <lualib.h>
}

namespace EquipmentProperties {
  class CEquipmentProperties;
}

using EquipmentProperty = EquipmentProperties::CEquipmentProperties;

struct EquipmentValueA {
  /* 0000 */ int value;
};

struct EquipmentValueC {
  /* 0000 */ int value;
};
template __declspec(noinline) std::unordered_map<int, EquipmentValueC>::unordered_map();
template __declspec(noinline) void std::_Tree_val<std::_Tree_simple_types<unsigned int>>::_Erase_tree<std::allocator<std::_Tree_node<unsigned int, void*>>>(
  std::allocator<std::_Tree_node<unsigned int, void*>>&, std::_Tree_node<unsigned int, void*>*
) noexcept;
template __declspec(noinline) void std::_Tree_val<std::_Tree_simple_types<std::pair<const int, int>>>::_Erase_tree<std::allocator<std::_Tree_node<std::pair<const int, int>, void*>>>(
  std::allocator<std::_Tree_node<std::pair<const int, int>, void*>>&, std::_Tree_node<std::pair<const int, int>, void*>*
) noexcept;

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

struct EquipmentPropertyStore {
  /* 0000 */ std::map<unsigned int, std::map<unsigned int, EquipmentProperty*>> properties;
  /* 0008 */ std::unordered_map<std::string, std::string> names;

  EquipmentPropertyStore();
  ~EquipmentPropertyStore();
  void Clear();
  std::map<unsigned int, std::map<unsigned int, EquipmentProperty*>>& GetProperties();
  int GetCount();
  std::unordered_map<std::string, std::string>& GetNames();
  EquipmentPropertyStore& operator+=(EquipmentPropertyStore& other);
  EquipmentPropertyStore& operator+=(EquipmentProperty* property);
  void RemoveDuplicateNames();
};

namespace EquipmentProperties {
  class CEquipmentProperties {
  public:
    explicit CEquipmentProperties(EquipmentPropertyDefinition* definition);
    CEquipmentProperties(const CEquipmentProperties& other);
    virtual ~CEquipmentProperties();
    virtual CEquipmentProperties* Accumulate(CEquipmentProperties* other);
    virtual CEquipmentProperties* Clone();
    virtual std::string Describe(bool detailed, bool negative);

    /* 0004 */ EquipmentPropertyDefinition* m_definition;
    /* 0008 */ std::set<unsigned int> m_keys;
  };

  class CEquipmentProperties1Dim : public CEquipmentProperties {
  public:
    explicit CEquipmentProperties1Dim(EquipmentPropertyDefinition* definition);
    CEquipmentProperties* Accumulate(CEquipmentProperties* other) override;
    CEquipmentProperties* Clone() override;
    std::string Describe(bool detailed, bool negative) override;
    void SetValue(const int& value, const unsigned int& key);

    /* 0010 */ int m_value;
  };

  class CEquipmentProperties2Dim : public CEquipmentProperties {
  public:
    explicit CEquipmentProperties2Dim(EquipmentPropertyDefinition* definition);
    CEquipmentProperties* Accumulate(CEquipmentProperties* other) override;
    ~CEquipmentProperties2Dim() override;
    CEquipmentProperties* Clone() override;
    std::string Describe(bool detailed, bool negative) override;
    void SetValue(const int& index, const int& value, const unsigned int& key);

    /* 0010 */ std::map<int, int> m_values;
  };
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentProperties::CEquipmentProperties) == 0x10);
static_assert(sizeof(EquipmentProperties::CEquipmentProperties1Dim) == 0x14);
static_assert(sizeof(EquipmentProperties::CEquipmentProperties2Dim) == 0x18);
#endif

// OFFSET: 0x00696120
// FUNCTION: fn_05739
EquipmentProperties::CEquipmentProperties::CEquipmentProperties(EquipmentPropertyDefinition* definition) : m_definition(definition) {
}

// OFFSET: 0x00695EC0
// FUNCTION: fn_05736
EquipmentProperties::CEquipmentProperties1Dim::CEquipmentProperties1Dim(EquipmentPropertyDefinition* definition)
    : CEquipmentProperties(definition), m_value(0) {
}

// OFFSET: 0x00695F60
// FUNCTION: fn_05737
EquipmentProperties::CEquipmentProperties2Dim::CEquipmentProperties2Dim(EquipmentPropertyDefinition* definition) : CEquipmentProperties(definition) {
}

// OFFSET: 0x0068B490
// FUNCTION: fn_05610
__declspec(noinline) EquipmentProperties::CEquipmentProperties::~CEquipmentProperties() {
}

// OFFSET: 0x0068B3F0
// FUNCTION: fn_05609
__declspec(noinline) EquipmentProperties::CEquipmentProperties2Dim::~CEquipmentProperties2Dim() {
}

// OFFSET: 0x00698550
// FUNCTION: fn_05768
EquipmentProperties::CEquipmentProperties* EquipmentProperties::CEquipmentProperties::Accumulate(CEquipmentProperties*) {
  return this;
}

// OFFSET: 0x00696020
// FUNCTION: fn_05738
EquipmentProperties::CEquipmentProperties::CEquipmentProperties(const CEquipmentProperties& other) = default;

// OFFSET: 0x00698FC0
// FUNCTION: fn_05774
EquipmentProperties::CEquipmentProperties* EquipmentProperties::CEquipmentProperties::Clone() {
  return new CEquipmentProperties(*this);
}

// OFFSET: 0x00698DE0
// FUNCTION: fn_05772
EquipmentProperties::CEquipmentProperties* EquipmentProperties::CEquipmentProperties1Dim::Clone() {
  return new CEquipmentProperties1Dim(*this);
}

// OFFSET: 0x00698E80
// FUNCTION: fn_05773
EquipmentProperties::CEquipmentProperties* EquipmentProperties::CEquipmentProperties2Dim::Clone() {
  return new CEquipmentProperties2Dim(*this);
}

// OFFSET: 0x00699070
// FUNCTION: fn_05778
__declspec(noinline) void EquipmentProperties::CEquipmentProperties1Dim::SetValue(const int& value, const unsigned int& key) {
  this->m_value = value;
  if (key != -1) {
    this->m_keys.insert(key);
  }
}

// OFFSET: 0x006990A0
// FUNCTION: fn_05779
__declspec(noinline) void EquipmentProperties::CEquipmentProperties2Dim::SetValue(const int& index, const int& value, const unsigned int& key) {
  this->m_values[index] = value;
  if (key != -1) {
    this->m_keys.insert(key);
  }
}

// OFFSET: 0x00698330
// FUNCTION: fn_05767
EquipmentProperties::CEquipmentProperties* EquipmentProperties::CEquipmentProperties2Dim::Accumulate(CEquipmentProperties* other) {
  if (this->m_definition == nullptr) {
    return this;
  }

  auto* source = dynamic_cast<CEquipmentProperties2Dim*>(other);
  if (source == nullptr) {
    return this;
  }

  if (this->m_definition->uniqueKeyMode != 0) {
    if (source->m_keys.size() != 1) {
      return this;
    }
    const unsigned int& key = *source->m_keys.begin();
    if (this->m_keys.contains(key)) {
      return this;
    }
    this->m_keys.insert(key);
  }

  for (auto& entry : source->m_values) {
    switch (this->m_definition->mode) {
    case 0:
      this->m_values[entry.first] += entry.second;
      break;
    case 1:
      ++this->m_values[entry.first];
      break;
    case 2:
      this->m_values[entry.first] = std::max(this->m_values[entry.first], entry.second);
      break;
    case 3:
      if (this->m_values.size() == 1) {
        int index = this->m_values.begin()->first;
        auto node = this->m_values.extract(index);
        node.key() = entry.first + index;
        this->m_values.insert(std::move(node));
        this->m_values.begin()->second += entry.second;
      }
      break;
    case 4:
      this->m_values[entry.first] = entry.second < 0 ? 0 : std::max(this->m_values[entry.first], entry.second);
      break;
    }
  }
  return this;
}

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

  struct CEquipmentInfo {
    /* 0000 */ int kind;
    /* 0004 */ std::unordered_map<int, int, CEquipmentValueHash> values;
    /* 0024 */ std::unordered_map<int, int> statProperties;
  };

  class CEquipmentPropertiesMgr {
  public:
    /* 0004 */ CLuaInstance script;
    /* 000C */ bool loadValid;
    /* 000D */ bool m_dirty = false;
    /* 0010 */ int padding_0010;
    /* 0014 */ int shopId;
    /* 0018 */ EquipmentPropertyStore* store;
    /* 001C */ int currentEquipment;
    /* 0020 */ UIWindow* notifyWindow;
    /* 0024 */ std::unordered_map<int, CEquipmentWearable*> properties1;
    /* 0044 */ std::unordered_map<unsigned int, CEquipmentExpansion*> properties2;
    /* 0064 */ std::unordered_map<unsigned int, CEquipmentCombiitem*> properties3;
    /* 0084 */ std::multiset<unsigned long> activeItems;
    /* 008C */ std::map<unsigned int, EquipmentPropertyGroup> propertyGroups;
    /* 0094 */ std::unordered_map<int, CEquipmentInfo> equipmentInfo;
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
    int GetCurrentEquipment();
    CEquipmentWearable* FindProperty1(const int& key);
    CEquipmentInfo* FindEquipmentInfo(const int& key);
    CLuaInstance& GetScript();
    EquipmentPropertyDefinition* FindRandomOptionDefinition(const int& key);
    std::list<unsigned int>* FindSkillGroup(const int& key);
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
    void RefreshWearable(const int& key);
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
namespace EquipmentProperties {
  class CEquipment {
  public:
    /* 0004 */ int m_id;
    /* 0008 */ CEquipmentInfo* m_info;
    /* 000C */ EquipmentPropertyStore m_properties;
    /* 0034 */ bool m_pretend;

    // INLINED
    CEquipment() : m_info(nullptr), m_pretend(false) {
    }
    explicit CEquipment(const int& id) : m_id(id), m_info(nullptr), m_pretend(false) {
    }
    virtual ~CEquipment();
    virtual void Load(bool force) = 0;
    virtual void Clear();
    virtual int GetType() = 0;
    virtual int GetValue(const int& key);

    int& GetId();
    EquipmentPropertyStore& GetProperties();
    bool IsPretend();
    void CallOnStartEquip(lua_State* state);
    void NotifyProperties(const COLORREF& color, bool negative, std::string_view suffix);
    void AddProperty(const std::string& name, const char* op, const int& index, const int& value, const int& key);
  };
}

namespace EquipmentProperties {
  class CEquipmentAttachable : public CEquipment {
  public:
    /* 0038 */ int m_slotIndex;
    /* 003C */ CEquipment* m_owner;

    // INLINED
    CEquipmentAttachable(const int& id, int index, CEquipment* owner) : CEquipment(id), m_slotIndex(index), m_owner(owner) {
    }

    void Load(bool force) override;
    int GetType() override;

    CEquipment* GetOwner();
  };

  class CEquipmentWearable : public CEquipment {
  public:
    /* 0038 */ bool m_active;
    /* 003C */ int m_type;
    /* 0040 */ int inventoryIndex;
    /* 0044 */ std::vector<CEquipmentAttachable> m_attachables;
    /* 0050 */ std::list<CEquipmentProperties*> m_children;

    explicit CEquipmentWearable(const ITEM_INFO* item);
    // INLINED
    ~CEquipmentWearable() override {
      for (CEquipmentProperties* property : this->m_children) {
        delete property;
      }
      this->m_children.clear();
    }
    void Load(bool force) override;
    void Clear() override;
    int GetType() override;

    std::vector<CEquipmentAttachable>& GetAttachables();
    __declspec(noinline) bool IsActive();
    __declspec(noinline) bool UpdateActive();
  };

  class CEquipmentExpansion : public CEquipment {
  public:
    /* 0038 */ int m_kind;

    CEquipmentExpansion(const int& id, const int& kind);
    void Load(bool force) override;
    void Clear() override;
    int GetType() override;

    std::string_view GetSuffix();
  };

  class CEquipmentCombiitem : public CEquipment {
  public:
    explicit CEquipmentCombiitem(const int& id);
    void Load(bool force) override;
    void Clear() override;
    int GetType() override;
  };
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentProperties::CEquipmentInfo) == 0x44);
static_assert(offsetof(EquipmentProperties::CEquipment, m_properties) == 0xC);
static_assert(offsetof(EquipmentProperties::CEquipment, m_pretend) == 0x34);
static_assert(sizeof(EquipmentProperties::CEquipment) == 0x38);
static_assert(sizeof(EquipmentProperties::CEquipmentAttachable) == 0x40);
static_assert(offsetof(EquipmentProperties::CEquipmentWearable, m_attachables) == 0x44);
static_assert(sizeof(EquipmentProperties::CEquipmentWearable) == 0x58);
static_assert(sizeof(EquipmentProperties::CEquipmentExpansion) == 0x3C);
#endif

// GLOBAL: gl_24723
int g_resetMessengerRandomStats = 1;

// GLOBAL: gl_24724
EquipmentProperties::CEquipmentPropertiesMgr* g_equipmentPropertiesMgr;

// GLOBAL: gl_24770
const COLORREF g_equipmentClearColor = RGB(255, 0, 0);

// GLOBAL: gl_24925
const COLORREF g_equipmentApplyColor = RGB(0, 255, 200);

// OFFSET: 0x006BF770
// FUNCTION: fn_06293
__declspec(noinline) bool EquipmentPropertyDefinition::IsTwoDimensional() const {
  return this->separatorMode != 0;
}

// OFFSET: 0x0068A340
// FUNCTION: fn_05603
EquipmentPropertyStore::EquipmentPropertyStore() = default;

// OFFSET: 0x00698CE0
// FUNCTION: fn_05771
void EquipmentPropertyStore::Clear() {
  for (auto& group : this->properties) {
    for (const auto& property : std::as_const(group.second)) {
      delete property.second;
    }
  }
  this->properties.clear();
  this->names.clear();
}

// OFFSET: 0x006965A0
// FUNCTION: fn_05740
EquipmentPropertyStore::~EquipmentPropertyStore() {
  for (auto& group : this->properties) {
    for (const auto& property : std::as_const(group.second)) {
      delete property.second;
    }
  }
  this->properties.clear();
}

// OFFSET: 0x00699040
// FUNCTION: fn_05775
__declspec(noinline) std::map<unsigned int, std::map<unsigned int, EquipmentProperty*>>& EquipmentPropertyStore::GetProperties() {
  return this->properties;
}

// OFFSET: 0x00699050
// FUNCTION: fn_05776
int EquipmentPropertyStore::GetCount() {
  return this->properties.size();
}

// OFFSET: 0x00699060
// FUNCTION: fn_05777
std::unordered_map<std::string, std::string>& EquipmentPropertyStore::GetNames() {
  return this->names;
}

// OFFSET: 0x0069BB80
// FUNCTION: fn_05807
EquipmentPropertyGroup::~EquipmentPropertyGroup() {
  for (auto& definition : this->definitions) {
    delete definition.second;
  }
}

// OFFSET: 0x0069BCF0
// FUNCTION: fn_05808
std::string& EquipmentPropertyGroup::GetName() {
  return this->name;
}

// OFFSET: 0x0069BD00
// FUNCTION: fn_05809
__declspec(noinline) std::map<std::int64_t, EquipmentPropertyDefinition*>& EquipmentPropertyGroup::GetDefinitions() {
  return this->definitions;
}

// OFFSET: 0x006A1AA0
// FUNCTION: fn_06013
EquipmentProperties::CEquipmentPropertiesMgr::CEquipmentPropertiesMgr()
    : loadValid(1), padding_0010(-1), shopId(-1), store(new EquipmentPropertyStore), currentEquipment(0), notifyWindow(0) {
}

// OFFSET: 0x006ABE90
// FUNCTION: fn_06170
void EquipmentProperties::CEquipmentPropertiesMgr::Clear() {
  this->currentEquipment = 0;
  this->activeItems.clear();
  for (auto& property : this->properties1) {
    delete property.second;
  }
  this->properties1.clear();
  for (auto& property : this->properties2) {
    delete property.second;
  }
  this->properties2.clear();
  for (auto& property : this->properties3) {
    delete property.second;
  }
  this->properties3.clear();
  this->store->Clear();
}

// OFFSET: 0x006A56C0
// FUNCTION: fn_06044
EquipmentProperties::CEquipmentPropertiesMgr::~CEquipmentPropertiesMgr() {
  this->Clear();
  this->store->Clear();
  delete this->store;
}

// OFFSET: 0x0068A0C0
// FUNCTION: fn_05600
__declspec(noinline) EquipmentProperties::CEquipmentCombiitem::CEquipmentCombiitem(const int& id) : CEquipment(id) {
}

// OFFSET: 0x0068A140
// FUNCTION: fn_05601
__declspec(noinline) EquipmentProperties::CEquipmentExpansion::CEquipmentExpansion(const int& id, const int& kind) : CEquipment(id), m_kind(kind) {
  this->m_pretend = true;
}

// OFFSET: 0x0068B6C0
// FUNCTION: fn_05612
EquipmentProperties::CEquipment::~CEquipment() {
  this->m_properties.Clear();
}

// OFFSET: 0x0068CE10
// FUNCTION: fn_05638
__declspec(noinline) void EquipmentProperties::CEquipment::Clear() {
  this->m_properties.Clear();
}

// OFFSET: 0x0068C020
// FUNCTION: fn_05624
int EquipmentProperties::CEquipment::GetValue(const int& key) {
  if (this->m_info == nullptr) {
    return -1;
  }

  auto found = this->m_info->values.find(key);
  if (this->m_info->values.end() != found) {
    return found->second;
  }
  return 0;
}

// OFFSET: 0x0068C0F0
// FUNCTION: fn_05628
__declspec(noinline) int& EquipmentProperties::CEquipment::GetId() {
  return this->m_id;
}

// OFFSET: 0x0068C140
// FUNCTION: fn_05633
__declspec(noinline) EquipmentPropertyStore& EquipmentProperties::CEquipment::GetProperties() {
  return this->m_properties;
}

// OFFSET: 0x0068C100
// FUNCTION: fn_05629
int EquipmentProperties::CEquipmentAttachable::GetType() {
  if (this->m_owner == nullptr) {
    return 0;
  }
  return this->m_owner->GetType();
}

// OFFSET: 0x0068C110
// FUNCTION: fn_05630
int EquipmentProperties::CEquipmentCombiitem::GetType() {
  return 0;
}

// OFFSET: 0x0068C120
// FUNCTION: fn_05631
int EquipmentProperties::CEquipmentExpansion::GetType() {
  return 0;
}

// OFFSET: 0x0068C130
// FUNCTION: fn_05632
int EquipmentProperties::CEquipmentWearable::GetType() {
  return this->m_type;
}

// OFFSET: 0x0068C010
// FUNCTION: fn_05623
__declspec(noinline) std::vector<EquipmentProperties::CEquipmentAttachable>& EquipmentProperties::CEquipmentWearable::GetAttachables() {
  return this->m_attachables;
}

// OFFSET: 0x0068C080
// FUNCTION: fn_05625
EquipmentProperties::CEquipment* EquipmentProperties::CEquipmentAttachable::GetOwner() {
  return this->m_owner;
}

// OFFSET: 0x0068C0E0
// FUNCTION: fn_05627
bool EquipmentProperties::CEquipmentWearable::IsActive() {
  return this->m_active;
}

// OFFSET: 0x0068C090
// FUNCTION: fn_05626
std::string_view EquipmentProperties::CEquipmentExpansion::GetSuffix() {
  switch (this->m_kind) {
  case 0:
    return "(P)";
  case 1:
    return "(R)";
  default:
    return "";
  }
}

// OFFSET: 0x0068CEF0
// FUNCTION: fn_05642
__declspec(noinline) bool EquipmentProperties::CEquipment::IsPretend() {
  if (this->m_pretend == true) {
    this->Load(false);
  }
  return this->m_pretend;
}

// OFFSET: 0x0068D9F0
// FUNCTION: fn_05654
EquipmentProperties::CEquipmentPropertiesMgr* EquipmentProperties::CEquipmentPropertiesMgr::GetInstance() {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) CEquipmentPropertiesMgr;
  }
  return g_equipmentPropertiesMgr;
}

// INLINED
inline unsigned int HashName(const char* text) {
  unsigned int hash = 5381;
  int character;
  while ((character = *text++) != 0) {
    hash = ((hash << 5) + hash) + character;
  }
  return hash;
}

// OFFSET: 0x0068BAD0
// FUNCTION: fn_05620
void EquipmentProperties::CEquipment::AddProperty(const std::string& name, const char* op, const int& index, const int& value, const int& key) {
  const std::uint64_t nameKey = HashName(name.c_str());

  EquipmentPropertyDefinition* definition = CEquipmentPropertiesMgr::GetInstance()->FindDefinition(nameKey);
  if (definition == nullptr) {
    CEquipmentPropertiesMgr::GetInstance()->HasFunctionPair(nameKey);
    return;
  }

  const int sign = (definition->functions[0] == op) * 2 - 1;
  if (value == 0x7FFFFFFF) {
    if (index == -1 && definition->mode == 1 && definition->functionMode == 0) {
      CEquipmentProperties property(definition);
      this->m_properties += &property;
    }
    return;
  }

  if (index == -1) {
    if (definition->mode == 1) {
      CEquipmentProperties property(definition);
      this->m_properties += &property;
      return;
    }

    CEquipmentProperties1Dim property(definition);
    property.SetValue(value * sign, key);
    this->m_properties += &property;
    return;
  }

  CEquipmentProperties2Dim property(definition);
  property.SetValue(index, value * sign, key);
  this->m_properties += &property;
}

// OFFSET: 0x00DDE720
// FUNCTION: fn_23398
__declspec(noinline) SessionShop* CSession::FindShop(const int& shopId) {
  const auto found = this->m_shops.find(shopId);
  if (found == this->m_shops.end()) {
    return nullptr;
  }
  return &found->second;
}

// OFFSET: 0x00DE3B10
// FUNCTION: fn_23485
int CSession::FindShopId(std::string name) {
  std::string mapName = name;
  if (mapName.empty()) {
    mapName = this->m_navigationPlayerMap;
  }

  const auto found = this->m_nameToId.find(mapName);
  if (found == this->m_nameToId.end()) {
    return -1;
  }
  return found->second;
}

// OFFSET: 0x0068BDB0
// FUNCTION: fn_05621
bool EquipmentProperties::CEquipmentWearable::UpdateActive() {
  SessionShop* shop = g_session.FindShop(g_session.FindShopId(""));
  bool active;
  if (shop != nullptr && shop->items.size() >= 1) {
    active = shop->items[0].find(this->m_id) != shop->items[0].end();
  } else {
    active = true;
  }
  const bool changed = this->m_active != active;
  this->m_active = active;
  return changed;
}

// OFFSET: 0x0068BEA0
// FUNCTION: fn_05622
void EquipmentProperties::CEquipment::CallOnStartEquip(lua_State* state) {
  lua_getfield(state, -1, "OnStartEquip");
  if (lua_type(state, -1) == LUA_TFUNCTION) {
    if (lua_pcall(state, 0, 0, 0) != 0) {
      std::string message = lua_tolstring(state, -1, nullptr);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0, 0, 0);
      lua_settop(state, -2);
    }
  } else {
    lua_settop(state, -2);
  }
}

// OFFSET: 0x0068C170
// FUNCTION: fn_05634
void EquipmentProperties::CEquipmentAttachable::Load(bool force) {
  CLuaInstance* script = &CEquipmentPropertiesMgr::GetInstance()->GetScript();
  if (script == nullptr) {
    return;
  }
  lua_State* state = script->GetState();
  if (state == nullptr) {
    return;
  }

  this->m_properties.Clear();
  CEquipmentPropertiesMgr::GetInstance()->SetCurrentEquipment(this);
  lua_getfield(state, LUA_GLOBALSINDEX, "Item");
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushinteger(state, this->m_id);
    lua_gettable(state, 1);
    if (lua_type(state, -1) == LUA_TTABLE) {
      this->CallOnStartEquip(state);
      lua_getfield(state, -1, "OnStartPretendEquip");
      this->m_owner->m_pretend |= lua_type(state, -1) == LUA_TBOOLEAN;
      lua_settop(state, -2);
    }
    lua_settop(state, -2);
  }
  lua_settop(state, -2);
}

// OFFSET: 0x0068C2B0
// FUNCTION: fn_05635
void EquipmentProperties::CEquipmentCombiitem::Load(bool force) {
  CLuaInstance* script = &CEquipmentPropertiesMgr::GetInstance()->GetScript();
  if (script == nullptr) {
    return;
  }
  lua_State* state = script->GetState();
  if (state == nullptr) {
    return;
  }

  this->m_properties.Clear();
  CEquipmentPropertiesMgr::GetInstance()->SetCurrentEquipment(this);
  lua_getfield(state, LUA_GLOBALSINDEX, "Combiitem");
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushinteger(state, this->m_id);
    lua_gettable(state, 1);
    if (lua_type(state, -1) == LUA_TTABLE) {
      this->CallOnStartEquip(state);
      lua_getfield(state, -1, "OnStartPretendEquip");
      this->m_pretend = lua_type(state, -1) == LUA_TBOOLEAN;
      lua_settop(state, -2);
    }
    lua_settop(state, -2);
  }
  lua_settop(state, -2);

  this->m_properties.RemoveDuplicateNames();
  if (force == true) {
    this->NotifyProperties(g_equipmentApplyColor, false, "(C)");
  }
}

// OFFSET: 0x0068C420
// FUNCTION: fn_05636
void EquipmentProperties::CEquipmentExpansion::Load(bool force) {
  CLuaInstance* script = &CEquipmentPropertiesMgr::GetInstance()->GetScript();
  if (script == nullptr) {
    return;
  }
  lua_State* state = script->GetState();
  if (state == nullptr) {
    return;
  }

  this->m_properties.Clear();
  CEquipmentPropertiesMgr::GetInstance()->SetCurrentEquipment(this);
  lua_getfield(state, LUA_GLOBALSINDEX, "Item");
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushinteger(state, this->m_id);
    lua_gettable(state, 1);
    if (lua_type(state, -1) == LUA_TTABLE) {
      this->CallOnStartEquip(state);
    }
    lua_settop(state, -2);
  }
  lua_settop(state, -2);

  this->m_properties.RemoveDuplicateNames();
  if (force == true) {
    this->NotifyProperties(g_equipmentApplyColor, false, this->GetSuffix());
  }
}

// OFFSET: 0x0068C560
// FUNCTION: fn_05637
void EquipmentProperties::CEquipmentWearable::Load(bool force) {
  CLuaInstance* script = &CEquipmentPropertiesMgr::GetInstance()->GetScript();
  if (script == nullptr) {
    return;
  }
  lua_State* state = script->GetState();
  if (state == nullptr) {
    return;
  }

  this->m_properties.Clear();
  CEquipmentPropertiesMgr::GetInstance()->SetCurrentEquipment(this);
  lua_getfield(state, LUA_GLOBALSINDEX, "Item");
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushinteger(state, this->m_id);
    lua_gettable(state, 1);
    if (lua_type(state, -1) == LUA_TTABLE) {
      for (auto& entry : this->m_info->statProperties) {
        this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", entry.first), "AddExtParam", -1, entry.second, -1);
      }

      if (!g_session.IsCostumeLocation(this->m_type) && this->m_info != nullptr) {
        const ITEM_INFO equipped = g_session.GetEquippedItemAt(this->inventoryIndex);
        if (equipped.identity[1] > 0) {
          const int key = this->GetValue(2);
          const int level = equipped.refiningLevel;
          if (level > 0) {
            if (this->m_info->kind == 1) {
              const int attack = CEquipmentPropertiesMgr::GetInstance()->GetRefiningBonus(3, key, level);
              if (attack != 0) {
                this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", 0x2D), "AddExtParam", -1, attack, -1);
              }

              const int resist = CEquipmentPropertiesMgr::GetInstance()->GetRefiningBonus(4, key, level);
              if (resist != 0) {
                this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", 0xF4), "AddExtParam", -1, resist, -1);
                this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", 0xF5), "AddExtParam", -1, resist, -1);
              }
            } else if (this->m_info->kind == 2 || this->m_info->kind == 3) {
              const int bonus = CEquipmentPropertiesMgr::GetInstance()->GetRefiningBonus(1, key, level);
              if (bonus != 0) {
                const int rate = CEquipmentPropertiesMgr::GetInstance()->GetGradeBonus(1, equipped.grade, 1);
                this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", 0x29), "AddExtParam", -1,
                                  static_cast<int>(static_cast<float>(rate * bonus) / 10.0f + static_cast<float>(bonus)), -1);
                if (this->m_info->kind == 2) {
                  const int magicRate = CEquipmentPropertiesMgr::GetInstance()->GetGradeBonus(1, equipped.grade, 2);
                  this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", 0xC8), "AddExtParam", -1,
                                    static_cast<int>(static_cast<float>(magicRate * bonus) / 10.0f + static_cast<float>(bonus)), -1);
                }
              }

              const int defense = CEquipmentPropertiesMgr::GetInstance()->GetRefiningBonus(2, key, level);
              if (defense != 0) {
                this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", 0xF2), "AddExtParam", -1, defense, -1);
                this->AddProperty(std::format("AddExtParam arg1:0 arg2:{0}", 0xF3), "AddExtParam", -1, defense, -1);
              }
            }
          }
        }
      }

      this->CallOnStartEquip(state);
      lua_getfield(state, -1, "OnStartPretendEquip");
      this->m_pretend = lua_type(state, -1) == LUA_TBOOLEAN || g_session.IsPetEgg(this->m_id) != 0;
      lua_settop(state, -2);
    }
    lua_settop(state, -2);
  }
  lua_settop(state, -2);

  for (CEquipmentProperties* child : this->m_children) {
    this->m_properties += child;
  }
  for (CEquipmentAttachable& attachable : this->m_attachables) {
    attachable.Load(force);
    this->m_properties += attachable.m_properties;
  }

  this->m_properties.RemoveDuplicateNames();
  if (force == true) {
    this->NotifyProperties(g_equipmentApplyColor, false, "");
  }
}

// OFFSET: 0x0068CE20
// FUNCTION: fn_05639
void EquipmentProperties::CEquipmentCombiitem::Clear() {
  this->NotifyProperties(g_equipmentClearColor, true, "(C)");
  this->m_properties.Clear();
}

// OFFSET: 0x0068CE50
// FUNCTION: fn_05640
void EquipmentProperties::CEquipmentExpansion::Clear() {
  this->NotifyProperties(g_equipmentClearColor, true, this->GetSuffix());
  this->m_properties.Clear();
}

// OFFSET: 0x0068CEA0
// FUNCTION: fn_05641
void EquipmentProperties::CEquipmentWearable::Clear() {
  if (this->m_active == true) {
    this->NotifyProperties(g_equipmentClearColor, true, "");
  }
  for (CEquipmentAttachable& attachable : this->m_attachables) {
    attachable.Clear();
  }
  this->m_properties.Clear();
}

// OFFSET: 0x0068CF10
// FUNCTION: fn_05643
void EquipmentProperties::CEquipment::NotifyProperties(const COLORREF& color, bool negative, std::string_view suffix) {
  for (const auto& group : std::as_const(this->m_properties.GetProperties())) {
    for (const auto& entry : group.second) {
      std::vector<std::string> lines = Split(entry.second->Describe(true, negative), "\n");
      for (std::string& line : lines) {
        ReplaceAll(line, "#", " ");
        RemoveAll(line, "{~}");
        if (line.size() > 0) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(std::format("{0}{1}", suffix, line).c_str()), color, 0x18, 0, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00698560
// FUNCTION: fn_05769
__declspec(noinline) EquipmentPropertyStore& EquipmentPropertyStore::operator+=(EquipmentPropertyStore& other) {
  for (const auto& group : std::as_const(other.properties)) {
    for (const auto& entry : group.second) {
      EquipmentPropertyDefinition* definition = entry.second->m_definition;
      if (definition == nullptr) {
        continue;
      }

      auto found = this->properties[definition->group].find(definition->id);
      if (this->properties[definition->group].end() == found) {
        this->properties[definition->group][definition->id] = entry.second->Clone();
      } else {
        EquipmentProperty* property = entry.second;
        this->properties[definition->group][definition->id]->Accumulate(property);
      }
    }
  }
  return *this;
}

// OFFSET: 0x00698C30
// FUNCTION: fn_05770
__declspec(noinline) EquipmentPropertyStore& EquipmentPropertyStore::operator+=(EquipmentProperty* property) {
  EquipmentPropertyDefinition* definition = property->m_definition;
  if (definition == nullptr) {
    return *this;
  }

  auto found = this->properties[definition->group].find(definition->id);
  if (this->properties[definition->group].end() == found) {
    this->properties[definition->group][definition->id] = property->Clone();
  } else {
    this->properties[definition->group][definition->id]->Accumulate(property);
  }
  return *this;
}

// OFFSET: 0x006990E0
// FUNCTION: fn_05780
std::string EquipmentProperties::CEquipmentProperties1Dim::Describe(bool detailed, bool negative) {
  std::string text;
  if (this->m_definition == nullptr) {
    return text;
  }

  int value = this->m_value;
  if (value == 0) {
    return text;
  }

  if (detailed == false) {
    auto minimum = this->m_definition->formatOptions.find(0);
    if (minimum != this->m_definition->formatOptions.end()) {
      value = std::max(value, minimum->second);
    }
    auto maximum = this->m_definition->formatOptions.find(1);
    if (maximum != this->m_definition->formatOptions.end()) {
      value = std::min(value, maximum->second);
    }
  }

  double result = value;
  auto divisor = this->m_definition->formatOptions.find(2);
  if (divisor != this->m_definition->formatOptions.end()) {
    result /= divisor->second;
  }
  int precision = 0;
  auto digits = this->m_definition->formatOptions.find(3);
  if (digits != this->m_definition->formatOptions.end()) {
    precision = digits->second;
  }

  const double sign = (result < 0.0 && this->m_definition->signLabels.size() == 2) ? -1.0 : 1.0;
  const double scale = std::pow(10.0, precision);
  const double rounded = std::floor(result * scale) / scale;
  std::string number = std::vformat("{:.{}f}", std::make_format_args(sign * rounded, precision));
  if (rounded != 0.0) {
    const int nonPositive = rounded <= 0.0;
    switch (this->m_definition->signLabels.size()) {
    case 0:
      text = EncodeText(g_textCodePage,
                        std::vformat(DecodeText(g_textCodePage, this->m_definition->formatText), std::make_wformat_args(DecodeText(g_textCodePage, number))));
      break;
    case 2: {
      const int index = negative == true ? nonPositive ^ 1 : nonPositive;
      if (index == 1) {
        text += "{~}";
      }
      text += EncodeText(g_textCodePage, std::vformat(DecodeText(g_textCodePage, this->m_definition->formatText),
                                                      std::make_wformat_args(DecodeText(g_textCodePage, number),
                                                                             DecodeText(g_textCodePage, this->m_definition->signLabels[index]))));
      break;
    }
    }
  }
  return text;
}

// OFFSET: 0x006996F0
// FUNCTION: fn_05781
std::string EquipmentProperties::CEquipmentProperties2Dim::Describe(bool detailed, bool negative) {
  std::string text;
  if (this->m_definition == nullptr) {
    return text;
  }

  bool failed = false;
  for (auto entry = this->m_values.rbegin(); entry != this->m_values.rend(); ++entry) {
    std::string argument;
    unsigned int hash = 5381;
    for (const char* character = this->m_definition->separator.c_str(); *character != '\0'; ++character) {
      hash = hash * 33 + *character;
    }
    switch (hash) {
    case 0xA2AF91ED: {
      const int& skillId = CEquipmentPropertiesMgr::GetInstance()->GetConstantValue(entry->first);
      if (skillId == 0) {
        continue;
      }
      argument = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, skillId);
      if (argument.size() == 0) {
        continue;
      }
      break;
    }
    case 0x8F918611:
      if (entry->first != 0) {
        std::list<unsigned int>* skills = CEquipmentPropertiesMgr::GetInstance()->FindSkillGroup(entry->first);
        if (skills == nullptr) {
          failed = true;
          continue;
        }
        for (const unsigned int& skillId : *skills) {
          if (argument.size() != 0) {
            argument += ", ";
          }
          argument += CSkillInfo::DisplayName(*g_session.m_hotkeyScript, skillId);
        }
        if (argument.size() != 0) {
          argument += " ";
        }
      }
      break;
    case 0x78BA6385:
      argument = CSkillInfo::DisplayName(*g_session.m_hotkeyScript, entry->first);
      if (argument.size() == 0) {
        continue;
      }
      break;
    case 0x16716DCE:
      argument = std::format("{0}", entry->first);
      break;
    default:
      failed = true;
      break;
    }

    int value = entry->second;
    if (detailed == false) {
      auto minimum = this->m_definition->formatOptions.find(0);
      if (minimum != this->m_definition->formatOptions.end()) {
        value = std::max(value, minimum->second);
      }
      auto maximum = this->m_definition->formatOptions.find(1);
      if (maximum != this->m_definition->formatOptions.end()) {
        value = std::min(value, maximum->second);
      }
    }

    double result = value;
    auto divisor = this->m_definition->formatOptions.find(2);
    if (divisor != this->m_definition->formatOptions.end()) {
      result /= divisor->second;
    }
    int precision = 0;
    auto digits = this->m_definition->formatOptions.find(3);
    if (digits != this->m_definition->formatOptions.end()) {
      precision = digits->second;
    }

    const double sign = (result < 0.0 && this->m_definition->signLabels.size() == 2) ? -1.0 : 1.0;
    const double scale = std::pow(10.0, precision);
    const double rounded = std::floor(result * scale) / scale;
    std::string number = std::vformat("{:.{}f}", std::make_format_args(sign * rounded, precision));
    if (rounded != 0.0) {
      if (text.size() != 0) {
        text += "\n";
      }
      const int nonPositive = rounded <= 0.0;
      switch (this->m_definition->signLabels.size()) {
      case 0:
        text += EncodeText(g_textCodePage, std::vformat(DecodeText(g_textCodePage, this->m_definition->formatText),
                                                        std::make_wformat_args(DecodeText(g_textCodePage, number), DecodeText(g_textCodePage, argument))));
        break;
      case 2: {
        const int index = negative == true ? nonPositive ^ 1 : nonPositive;
        if (index == 1) {
          text += "{~}";
        }
        text += EncodeText(g_textCodePage,
                           std::vformat(DecodeText(g_textCodePage, this->m_definition->formatText),
                                        std::make_wformat_args(DecodeText(g_textCodePage, number), DecodeText(g_textCodePage, argument),
                                                               DecodeText(g_textCodePage, this->m_definition->signLabels[index]))));
        break;
      }
      }
    }
  }

  if (failed == true) {
    return g_emptyString;
  }
  return text;
}

// OFFSET: 0x0069A2A0
// FUNCTION: fn_05782
std::string EquipmentProperties::CEquipmentProperties::Describe(bool detailed, bool negative) {
  std::string text;
  if (this->m_definition == nullptr) {
    return text;
  }

  switch (this->m_definition->signLabels.size()) {
  case 0:
    text = this->m_definition->formatText;
    break;
  case 2:
    text += EncodeText(g_textCodePage, std::vformat(DecodeText(g_textCodePage, this->m_definition->formatText),
                                                    std::make_wformat_args(DecodeText(g_textCodePage, this->m_definition->signLabels[negative]))));
    break;
  }
  return text;
}

// OFFSET: 0x0069A4F0
// FUNCTION: fn_05783
void EquipmentPropertyStore::RemoveDuplicateNames() {
  std::set<std::string> names;
  for (auto& group : this->properties) {
    for (auto it = group.second.begin(); it != group.second.end();) {
      EquipmentProperty* property = it->second;
      EquipmentPropertyDefinition* definition = property->m_definition;
      if (definition == nullptr || definition->uniqueGroup.size() == 0) {
        ++it;
      } else if (names.find(definition->uniqueGroup) == names.end()) {
        names.insert(definition->uniqueGroup);
        ++it;
      } else {
        delete property;
        it = group.second.erase(it);
      }
    }
  }
}

// OFFSET: 0x006AC2A0
// FUNCTION: fn_06172
__declspec(noinline) const int& EquipmentProperties::CEquipmentPropertiesMgr::GetConstantValue(const int& key) {
  auto found = this->constantValues.find(key);
  if (this->constantValues.end() == found) {
    return key;
  }
  return found->second.value;
}

// OFFSET: 0x006AC3B0
// FUNCTION: fn_06174
int EquipmentProperties::CEquipmentPropertiesMgr::GetCurrentEquipment() {
  return this->currentEquipment;
}

// OFFSET: 0x006AC3C0
// FUNCTION: fn_06175
EquipmentProperties::CEquipmentWearable* EquipmentProperties::CEquipmentPropertiesMgr::FindProperty1(const int& key) {
  auto found = this->properties1.find(key);
  if (this->properties1.end() == found) {
    return nullptr;
  }
  return found->second;
}

// OFFSET: 0x006AC440
// FUNCTION: fn_06176
__declspec(noinline) EquipmentProperties::CEquipmentInfo* EquipmentProperties::CEquipmentPropertiesMgr::FindEquipmentInfo(const int& key) {
  auto found = this->equipmentInfo.find(key);
  if (this->equipmentInfo.end() == found) {
    return nullptr;
  }
  return &found->second;
}

// OFFSET: 0x006AC4C0
// FUNCTION: fn_06177
int GetEquipLocationMask(const int& location) {
  int mask = 0;
  switch (location) {
  case 1:
    mask = 0x1;
    break;
  case 2:
    mask = 0x10;
    break;
  case 3:
    mask = 0x20;
    break;
  case 4:
    mask = 0x2;
    break;
  case 5:
    mask = 0x4;
    break;
  case 6:
    mask = 0x40;
    break;
  case 7:
    mask = 0x8;
    break;
  case 8:
    mask = 0x80;
    break;
  case 10:
    mask = 0x100;
    break;
  case 11:
    mask = 0x200;
    break;
  case 20:
    mask = 0x400;
    break;
  case 21:
    mask = 0x800;
    break;
  case 22:
    mask = 0x1000;
    break;
  case 23:
    mask = 0x2000;
    break;
  case 30:
    mask = 0x10000;
    break;
  case 31:
    mask = 0x20000;
    break;
  case 32:
    mask = 0x40000;
    break;
  case 33:
    mask = 0x80000;
    break;
  case 34:
    mask = 0x100000;
    break;
  case 35:
    mask = 0x200000;
    break;
  }
  return mask;
}

// OFFSET: 0x006AC610
// FUNCTION: fn_06178
__declspec(noinline) CLuaInstance& EquipmentProperties::CEquipmentPropertiesMgr::GetScript() {
  return this->script;
}

// OFFSET: 0x006AC620
// FUNCTION: fn_06179
int GetFirstJob(int job) {
  switch (job) {
  case JT_SWORDMAN:
  case JT_KNIGHT:
  case JT_CRUSADER:
  case JT_SWORDMAN_H:
  case JT_KNIGHT_H:
  case JT_CRUSADER_H:
  case JT_SWORDMAN_B:
  case JT_KNIGHT_B:
  case JT_CRUSADER_B:
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_RUNE_KNIGHT_B:
  case JT_ROYAL_GUARD_B:
  case JT_DRAGON_KNIGHT:
  case JT_IMPERIAL_GUARD:
    return JT_SWORDMAN;
  case JT_MAGICIAN:
  case JT_WIZARD:
  case JT_SAGE:
  case JT_MAGICIAN_H:
  case JT_WIZARD_H:
  case JT_SAGE_H:
  case JT_MAGICIAN_B:
  case JT_WIZARD_B:
  case JT_SAGE_B:
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_WARLOCK_B:
  case JT_SORCERER_B:
  case JT_ARCH_MAGE:
  case JT_ELEMENTAL_MASTER:
    return JT_MAGICIAN;
  case JT_ARCHER:
  case JT_HUNTER:
  case JT_BARD:
  case JT_DANCER:
  case JT_ARCHER_H:
  case JT_HUNTER_H:
  case JT_BARD_H:
  case JT_DANCER_H:
  case JT_ARCHER_B:
  case JT_HUNTER_B:
  case JT_BARD_B:
  case JT_DANCER_B:
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_MINSTREL:
  case JT_WANDERER:
  case JT_MINSTREL_H:
  case JT_WANDERER_H:
  case JT_RANGER_B:
  case JT_MINSTREL_B:
  case JT_WANDERER_B:
  case JT_WINDHAWK:
  case JT_TROUBADOUR:
  case JT_TROUVERE:
    return JT_ARCHER;
  case JT_ACOLYTE:
  case JT_PRIEST:
  case JT_MONK:
  case JT_ACOLYTE_H:
  case JT_PRIEST_H:
  case JT_MONK_H:
  case JT_ACOLYTE_B:
  case JT_PRIEST_B:
  case JT_MONK_B:
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_SURA:
  case JT_SURA_H:
  case JT_ARCHBISHOP_B:
  case JT_SURA_B:
  case JT_CARDINAL:
  case JT_INQUISITOR:
    return JT_ACOLYTE;
  case JT_MERCHANT:
  case JT_BLACKSMITH:
  case JT_ALCHEMIST:
  case JT_MERCHANT_H:
  case JT_BLACKSMITH_H:
  case JT_ALCHEMIST_H:
  case JT_MERCHANT_B:
  case JT_BLACKSMITH_B:
  case JT_ALCHEMIST_B:
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_MECHANIC_B:
  case JT_GENETIC_B:
  case JT_MEISTER:
  case JT_BIOLO:
    return JT_MERCHANT;
  case JT_THIEF:
  case JT_ASSASSIN:
  case JT_ROGUE:
  case JT_THIEF_H:
  case JT_ASSASSIN_H:
  case JT_ROGUE_H:
  case JT_THIEF_B:
  case JT_ASSASSIN_B:
  case JT_ROGUE_B:
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CHASER_B:
  case JT_SHADOW_CROSS:
  case JT_ABYSS_CHASER:
    return JT_THIEF;
  case JT_TAEKWON:
  case JT_STAR:
  case JT_STAR2:
  case JT_LINKER:
  case JT_TAEKWON_B:
  case JT_STAR_B:
  case JT_LINKER_B:
  case JT_STAR_EMPEROR:
  case JT_SOUL_REAPER:
  case JT_STAR_EMPEROR_B:
  case JT_SOUL_REAPER_B:
  case JT_SKY_EMPEROR:
  case JT_SOUL_ASCETIC:
    return JT_TAEKWON;
  case JT_NINJA:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_NINJA_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
    return JT_NINJA;
  case JT_GUNSLINGER:
  case JT_REBELLION:
  case JT_GUNSLINGER_B:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    return JT_GUNSLINGER;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
    return JT_DO_SUMMONER;
  case JT_DRUID:
  case JT_DRUID_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
  case JT_ALITEA:
    return JT_DRUID;
  default:
    return 0;
  }
}

// OFFSET: 0x006AC890
// FUNCTION: fn_06180
int GetNormalJob(int job) {
  switch (job) {
  case JT_NOVICE:
  case JT_NOVICE_H:
  case JT_NOVICE_B:
    return JT_NOVICE;
  case JT_SWORDMAN:
  case JT_SWORDMAN_H:
  case JT_SWORDMAN_B:
    return JT_SWORDMAN;
  case JT_MAGICIAN:
  case JT_MAGICIAN_H:
  case JT_MAGICIAN_B:
    return JT_MAGICIAN;
  case JT_ARCHER:
  case JT_ARCHER_H:
  case JT_ARCHER_B:
    return JT_ARCHER;
  case JT_ACOLYTE:
  case JT_ACOLYTE_H:
  case JT_ACOLYTE_B:
    return JT_ACOLYTE;
  case JT_MERCHANT:
  case JT_MERCHANT_H:
  case JT_MERCHANT_B:
    return JT_MERCHANT;
  case JT_THIEF:
  case JT_THIEF_H:
  case JT_THIEF_B:
    return JT_THIEF;
  case JT_KNIGHT:
  case JT_KNIGHT_H:
  case JT_KNIGHT_B:
    return JT_KNIGHT;
  case JT_PRIEST:
  case JT_PRIEST_H:
  case JT_PRIEST_B:
    return JT_PRIEST;
  case JT_WIZARD:
  case JT_WIZARD_H:
  case JT_WIZARD_B:
    return JT_WIZARD;
  case JT_BLACKSMITH:
  case JT_BLACKSMITH_H:
  case JT_BLACKSMITH_B:
    return JT_BLACKSMITH;
  case JT_HUNTER:
  case JT_HUNTER_H:
  case JT_HUNTER_B:
    return JT_HUNTER;
  case JT_ASSASSIN:
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
    return JT_ASSASSIN;
  case JT_CRUSADER:
  case JT_CRUSADER_H:
  case JT_CRUSADER_B:
    return JT_CRUSADER;
  case JT_MONK:
  case JT_MONK_H:
  case JT_MONK_B:
    return JT_MONK;
  case JT_SAGE:
  case JT_SAGE_H:
  case JT_SAGE_B:
    return JT_SAGE;
  case JT_ROGUE:
  case JT_ROGUE_H:
  case JT_ROGUE_B:
    return JT_ROGUE;
  case JT_ALCHEMIST:
  case JT_ALCHEMIST_H:
  case JT_ALCHEMIST_B:
    return JT_ALCHEMIST;
  case JT_BARD:
  case JT_BARD_H:
  case JT_BARD_B:
    return JT_BARD;
  case JT_DANCER:
  case JT_DANCER_H:
  case JT_DANCER_B:
    return JT_DANCER;
  case JT_SUPERNOVICE:
  case JT_SUPERNOVICE_B:
    return JT_SUPERNOVICE;
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_HYPER_NOVICE:
    return JT_SUPERNOVICE2;
  case JT_NINJA:
  case JT_NINJA_B:
    return JT_NINJA;
  case JT_OBORO:
  case JT_OBORO_B:
  case JT_SHIRANUI:
    return JT_OBORO;
  case JT_KAGEROU:
  case JT_KAGEROU_B:
  case JT_SHINKIRO:
    return JT_KAGEROU;
  case JT_GUNSLINGER:
  case JT_GUNSLINGER_B:
    return JT_GUNSLINGER;
  case JT_REBELLION:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    return JT_REBELLION;
  case JT_TAEKWON:
  case JT_TAEKWON_B:
    return JT_TAEKWON;
  case JT_STAR:
  case JT_STAR_B:
    return JT_STAR;
  case JT_LINKER:
  case JT_LINKER_B:
    return JT_LINKER;
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
  case JT_SKY_EMPEROR:
    return JT_STAR_EMPEROR;
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
  case JT_SOUL_ASCETIC:
    return JT_SOUL_REAPER;
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_KNIGHT_B:
  case JT_DRAGON_KNIGHT:
    return JT_RUNE_KNIGHT;
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
  case JT_CARDINAL:
    return JT_ARCHBISHOP;
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
  case JT_ARCH_MAGE:
    return JT_WARLOCK;
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MECHANIC_B:
  case JT_MEISTER:
    return JT_MECHANIC;
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_RANGER_B:
  case JT_WINDHAWK:
    return JT_RANGER;
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CROSS:
    return JT_GUILLOTINE_CROSS;
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_ROYAL_GUARD_B:
  case JT_IMPERIAL_GUARD:
    return JT_ROYAL_GUARD;
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
  case JT_INQUISITOR:
    return JT_SURA;
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
  case JT_ELEMENTAL_MASTER:
    return JT_SORCERER;
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
  case JT_ABYSS_CHASER:
    return JT_SHADOW_CHASER;
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
  case JT_BIOLO:
    return JT_GENETIC;
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
  case JT_TROUBADOUR:
    return JT_MINSTREL;
  case JT_WANDERER:
  case JT_WANDERER_H:
  case JT_WANDERER_B:
  case JT_TROUVERE:
    return JT_WANDERER;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
    return JT_DO_SUMMONER;
  case JT_DRUID:
  case JT_DRUID_B:
    return JT_DRUID;
  case JT_KARNOS:
  case JT_KARNOS_B:
  case JT_ALITEA:
    return JT_KARNOS;
  default:
    return job;
  }
}

// OFFSET: 0x006ACCB0
// FUNCTION: fn_06181
int GetNormalThirdJob(int job) {
  switch (job) {
  case JT_NOVICE:
  case JT_NOVICE_H:
  case JT_NOVICE_B:
    return JT_NOVICE;
  case JT_SWORDMAN:
  case JT_SWORDMAN_H:
  case JT_SWORDMAN_B:
    return JT_SWORDMAN;
  case JT_MAGICIAN:
  case JT_MAGICIAN_H:
  case JT_MAGICIAN_B:
    return JT_MAGICIAN;
  case JT_ARCHER:
  case JT_ARCHER_H:
  case JT_ARCHER_B:
    return JT_ARCHER;
  case JT_ACOLYTE:
  case JT_ACOLYTE_H:
  case JT_ACOLYTE_B:
    return JT_ACOLYTE;
  case JT_MERCHANT:
  case JT_MERCHANT_H:
  case JT_MERCHANT_B:
    return JT_MERCHANT;
  case JT_THIEF:
  case JT_THIEF_H:
  case JT_THIEF_B:
    return JT_THIEF;
  case JT_KNIGHT:
  case JT_KNIGHT_H:
  case JT_KNIGHT_B:
    return JT_KNIGHT;
  case JT_PRIEST:
  case JT_PRIEST_H:
  case JT_PRIEST_B:
    return JT_PRIEST;
  case JT_WIZARD:
  case JT_WIZARD_H:
  case JT_WIZARD_B:
    return JT_WIZARD;
  case JT_BLACKSMITH:
  case JT_BLACKSMITH_H:
  case JT_BLACKSMITH_B:
    return JT_BLACKSMITH;
  case JT_HUNTER:
  case JT_HUNTER_H:
  case JT_HUNTER_B:
    return JT_HUNTER;
  case JT_ASSASSIN:
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
    return JT_ASSASSIN;
  case JT_CRUSADER:
  case JT_CRUSADER_H:
  case JT_CRUSADER_B:
    return JT_CRUSADER;
  case JT_MONK:
  case JT_MONK_H:
  case JT_MONK_B:
    return JT_MONK;
  case JT_SAGE:
  case JT_SAGE_H:
  case JT_SAGE_B:
    return JT_SAGE;
  case JT_ROGUE:
  case JT_ROGUE_H:
  case JT_ROGUE_B:
    return JT_ROGUE;
  case JT_ALCHEMIST:
  case JT_ALCHEMIST_H:
  case JT_ALCHEMIST_B:
    return JT_ALCHEMIST;
  case JT_BARD:
  case JT_BARD_H:
  case JT_BARD_B:
    return JT_BARD;
  case JT_DANCER:
  case JT_DANCER_H:
  case JT_DANCER_B:
    return JT_DANCER;
  case JT_SUPERNOVICE:
  case JT_SUPERNOVICE_B:
    return JT_SUPERNOVICE;
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
    return JT_SUPERNOVICE2;
  case JT_NINJA:
  case JT_NINJA_B:
    return JT_NINJA;
  case JT_OBORO:
  case JT_OBORO_B:
    return JT_OBORO;
  case JT_KAGEROU:
  case JT_KAGEROU_B:
    return JT_KAGEROU;
  case JT_GUNSLINGER:
  case JT_GUNSLINGER_B:
    return JT_GUNSLINGER;
  case JT_REBELLION:
  case JT_REBELLION_B:
    return JT_REBELLION;
  case JT_TAEKWON:
  case JT_TAEKWON_B:
    return JT_TAEKWON;
  case JT_STAR:
  case JT_STAR_B:
    return JT_STAR;
  case JT_LINKER:
  case JT_LINKER_B:
    return JT_LINKER;
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
    return JT_STAR_EMPEROR;
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
    return JT_SOUL_REAPER;
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_KNIGHT_B:
    return JT_RUNE_KNIGHT;
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
    return JT_ARCHBISHOP;
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
    return JT_WARLOCK;
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MECHANIC_B:
    return JT_MECHANIC;
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_RANGER_B:
    return JT_RANGER;
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
    return JT_GUILLOTINE_CROSS;
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_ROYAL_GUARD_B:
    return JT_ROYAL_GUARD;
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
    return JT_SURA;
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
    return JT_SORCERER;
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
    return JT_SHADOW_CHASER;
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
    return JT_GENETIC;
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
    return JT_MINSTREL;
  case JT_WANDERER:
  case JT_WANDERER_H:
  case JT_WANDERER_B:
    return JT_WANDERER;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
    return JT_DO_SUMMONER;
  case JT_DRUID:
  case JT_DRUID_B:
    return JT_DRUID;
  case JT_KARNOS:
  case JT_KARNOS_B:
    return JT_KARNOS;
  case JT_DRAGON_KNIGHT:
  case JT_MEISTER:
  case JT_SHADOW_CROSS:
  case JT_ARCH_MAGE:
  case JT_CARDINAL:
  case JT_WINDHAWK:
  case JT_IMPERIAL_GUARD:
  case JT_BIOLO:
  case JT_ABYSS_CHASER:
  case JT_ELEMENTAL_MASTER:
  case JT_INQUISITOR:
  case JT_TROUBADOUR:
  case JT_TROUVERE:
  case JT_SKY_EMPEROR:
  case JT_SOUL_ASCETIC:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
  case JT_NIGHT_WATCH:
  case JT_HYPER_NOVICE:
  case JT_SPIRIT_HANDLER:
  case JT_ALITEA:
    return job;
  default:
    return job;
  }
}

// OFFSET: 0x006AD500
// FUNCTION: fn_06184
__declspec(noinline) EquipmentPropertyDefinition* EquipmentProperties::CEquipmentPropertiesMgr::FindRandomOptionDefinition(const int& key) {
  auto found = this->randomOptionDefinitions.find(key);
  if (this->randomOptionDefinitions.end() == found) {
    return nullptr;
  }
  return found->second;
}

// OFFSET: 0x006AD690
// FUNCTION: fn_06186
int GetEquipLocationIndex(const int& mask) {
  int index = 0;
  switch (mask) {
  case EQP_HEAD_LOW:
    index = 1;
    break;
  case EQP_ARMOR:
    index = 2;
    break;
  case EQP_HAND_L:
    index = 3;
    break;
  case EQP_HAND_R:
  case (EQP_HAND_R | EQP_HAND_L):
    index = 4;
    break;
  case EQP_GARMENT:
    index = 5;
    break;
  case EQP_ACC_R:
    index = 7;
    break;
  case EQP_SHOES:
    index = 6;
    break;
  case (EQP_HEAD_TOP | EQP_HEAD_LOW):
    index = 1;
    break;
  case EQP_HEAD_TOP:
    index = 10;
    break;
  case EQP_ACC_L:
    index = 8;
    break;
  case EQP_HEAD_MID:
    index = 11;
    break;
  case (EQP_HEAD_MID | EQP_HEAD_LOW):
    index = 1;
    break;
  case (EQP_HEAD_TOP | EQP_HEAD_MID):
    index = 10;
    break;
  case (EQP_HEAD_TOP | EQP_HEAD_MID | EQP_HEAD_LOW):
    index = 1;
    break;
  case EQP_COSTUME_HEAD_TOP:
  case (EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_MID):
  case (EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_LOW):
  case (EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_MID | EQP_COSTUME_HEAD_LOW):
    index = 20;
    break;
  case EQP_COSTUME_HEAD_LOW:
    index = 22;
    break;
  case EQP_COSTUME_HEAD_MID:
  case (EQP_COSTUME_HEAD_MID | EQP_COSTUME_HEAD_LOW):
    index = 21;
    break;
  case EQP_COSTUME_GARMENT:
    index = 23;
    break;
  case EQP_AMMO:
    index = 0;
    break;
  case EQP_SHADOW_SHIELD:
    index = 32;
    break;
  case EQP_SHADOW_WEAPON:
    index = 31;
    break;
  case EQP_SHADOW_ARMOR:
    index = 30;
    break;
  case EQP_SHADOW_SHOES:
    index = 33;
    break;
  case EQP_SHADOW_ACC_L:
    index = 35;
    break;
  case EQP_SHADOW_ACC_R:
    index = 34;
    break;
  }
  return index;
}

// OFFSET: 0x006AD880
// FUNCTION: fn_06187
__declspec(noinline) std::list<unsigned int>* EquipmentProperties::CEquipmentPropertiesMgr::FindSkillGroup(const int& key) {
  auto found = this->skillGroups.find(key);
  if (found == this->skillGroups.end()) {
    return nullptr;
  }
  return &found->second;
}

// OFFSET: 0x006AD900
// FUNCTION: fn_06188
std::unordered_map<int, EquipmentValueC>& EquipmentProperties::CEquipmentPropertiesMgr::GetDisplayValues() {
  return this->displayValues;
}

// OFFSET: 0x006B0120
// FUNCTION: fn_06190
__declspec(noinline) bool EquipmentProperties::CEquipmentPropertiesMgr::HasFunctionPair(const std::uint64_t& key) {
  return this->functionPairs.contains(key);
}

// OFFSET: 0x006B0170
// FUNCTION: fn_06191
void EquipmentProperties::CEquipmentPropertiesMgr::LoadADSkill() {
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "ADSkill";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      unsigned int key = 0;
      int result;
      int type = lua_type(this->script.GetState(), -2);
      if (type == LUA_TNUMBER) {
        key = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
        result = 0;
      } else {
        result = (type != LUA_TNIL) + 1;
      }
      this->loadValid &= result == 0;
      if (result != 0) {
        ShowErrorDialog(std::format("{0} table has invalid key({1})", name, result), 0);
      }
      if (this->loadValid) {
        unsigned int value = 0;
        type = lua_type(this->script.GetState(), -1);
        if (type == LUA_TNUMBER) {
          value = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -1));
          result = 0;
        } else {
          result = (type != LUA_TNIL) + 1;
        }
        this->loadValid &= result == 0;
        if (result != 0) {
          ShowErrorDialog(std::format("{0} table has invalid value({1})", name, result), 0);
        }
        if (this->loadValid) {
          this->constantValues.try_emplace(key, EquipmentValueA{static_cast<int>(value)});
        }
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}

// OFFSET: 0x006B05A0
// FUNCTION: fn_06192
void EquipmentProperties::CEquipmentPropertiesMgr::LoadCombiitem() {
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "Combiitem";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      unsigned int key = 0;
      int result;
      const int type = lua_type(this->script.GetState(), -2);
      if (type == LUA_TNUMBER) {
        key = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
        result = 0;
      } else {
        result = (type != LUA_TNIL) + 1;
      }
      this->loadValid &= result == 0;
      if (result != 0) {
        ShowErrorDialog(std::format("{0} table has invalid entry key({1})", name, result), 0);
      }
      if (this->loadValid) {
        std::vector<unsigned int> items;
        result = this->script.GetFieldUIntArray(items, "Item");
        this->loadValid &= result == 0;
        if (result != 0) {
          ShowErrorDialog(std::format("Combiitem[{0}] has invalid 'Item' table({1})", key, result), 0);
        }
        if (this->loadValid) {
          for (unsigned int item : items) {
            this->m_combinationItems[key].push_back(item);
          }
        }
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}

// OFFSET: 0x006B09F0
// FUNCTION: fn_06193
void EquipmentProperties::CEquipmentPropertiesMgr::LoadEnumVar() {
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "EnumVAR";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      std::string key;
      int result = 0;
      int type = lua_type(this->script.GetState(), -2);
      if (type == LUA_TSTRING) {
        key = lua_tolstring(this->script.GetState(), -2, nullptr);
      } else {
        result = (type != LUA_TNIL) + 1;
      }
      this->loadValid &= result == 0;
      if (result != 0) {
        ShowErrorDialog(std::format("{0} table has invalid entry key({1})", name, result), 0);
      }
      if (this->loadValid) {
        std::vector<int> values;
        if (lua_type(state, -1) == LUA_TTABLE) {
          lua_pushnil(state);
          while (lua_next(state, -2) != 0) {
            values.push_back(static_cast<int>(lua_tonumber(state, -1)));
            lua_settop(state, -2);
          }
        }
        if (values.size() > 1) {
          unsigned int hash = HashName(key.c_str());
          int variable;
          switch (hash) {
          case 0x617FECDF:
            variable = 6;
            break;
          case 0x8854128A:
            variable = 8;
            break;
          case 0x4BA88EBA:
            variable = 13;
            break;
          case 0x66C4AA72:
            variable = 14;
            break;
          case 0x1C3E3594:
            variable = 15;
            break;
          case 0xCF35A0EC:
            variable = 16;
            break;
          case 0x6E5FE302:
            variable = 17;
            break;
          case 0xEDABA34D:
            variable = 18;
            break;
          case 0x35175B63:
            variable = 41;
            break;
          case 0xBDC4C598:
            variable = 45;
            break;
          case 0xEFCBA0D6:
            variable = 47;
            break;
          case 0xD1114E90:
            variable = 200;
            break;
          case 0xD1CD4317:
            variable = 219;
            break;
          case 0x2F0A85E9:
            variable = 220;
            break;
          case 0x43C069D4:
            variable = 221;
            break;
          case 0xCCC3CEB0:
            variable = 222;
            break;
          case 0x32AF6341:
            variable = 223;
            break;
          case 0xC5524E6A:
            variable = 224;
            break;
          default:
            variable = 0;
            break;
          }
          if (variable != 0) {
            this->statVariables.emplace(variable, values[1]);
          }
        }
      }
      lua_settop(state, -2);
    }
  }
  lua_settop(state, -2);
}

// OFFSET: 0x006B3C00
// FUNCTION: fn_06196
void EquipmentProperties::CEquipmentPropertiesMgr::LoadIgnoreEquipmentProperties() {
  if (!this->loadValid) {
    return;
  }
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "IgnoreEquipmentProperties";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    int index = 0;
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      ++index;
      std::vector<std::string> functions;
      int result = this->script.GetFieldStringArray(functions, "func");
      this->loadValid &= result == 0;
      if (result != 0) {
        ShowErrorDialog(std::format("{0}'s sub table[{1}] has invaild 'func' field({2})", name, index, result), 0);
      }
      if (!this->loadValid) {
        break;
      }

      this->loadValid &= functions.size() != 0;
      if (functions.size() == 0) {
        ShowErrorDialog(std::format("{0}'s sub table[{1}]'s 'func' table has 0 value", name, index), 0);
      }
      if (!this->loadValid) {
        break;
      }

      this->loadValid &= functions.size() <= 2;
      if (functions.size() > 2) {
        ShowErrorDialog(std::format("{0}'s sub table[{1}]'s 'func' table has more than 2 values", name, index), 0);
      }
      if (!this->loadValid) {
        break;
      }

      std::map<unsigned int, unsigned int> conditions;
      result = this->script.GetFieldUIntMap(conditions, "cond");
      const bool valid = result == 0 || result == 1;
      this->loadValid &= valid;
      if (!valid) {
        ShowErrorDialog(std::format("{0}'s sub table[{1}]'s has invalid 'cond' table({2})", name, index, result), 0);
      }
      if (!this->loadValid) {
        break;
      }

      for (const std::string& function : functions) {
        std::string text = std::format("{0}", function);
        for (const auto& condition : conditions) {
          text = std::format("{0} arg{1}:{2}", text, condition.first, condition.second);
        }
        unsigned int hash = 5381;
        for (const char* character = text.c_str(); *character != '\0'; ++character) {
          hash = hash * 33 + *character;
        }
        this->functionPairs.insert(hash);
      }

      this->loadValid &= valid;
      if (!valid) {
        ShowErrorDialog(std::format("{0}'s sub table[{1}]'s 'cond' table has some error", name, index), 0);
      }
      if (!this->loadValid) {
        break;
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}

// OFFSET: 0x006B2EE0
// FUNCTION: fn_06195
void EquipmentProperties::CEquipmentPropertiesMgr::LoadGradeBonus() {
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "GradeBonus";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      if (this->loadValid) {
        std::string key;
        int result = 0;
        int type = lua_type(this->script.GetState(), -2);
        if (type == LUA_TSTRING) {
          key = lua_tolstring(this->script.GetState(), -2, nullptr);
        } else {
          result = (type != LUA_TNIL) + 1;
        }
        this->loadValid &= result == 0;
        if (result != 0) {
          ShowErrorDialog(std::format("invalid key({0}): {1}", result, name), 0);
        }
        if (this->loadValid) {
          bool valid = lua_type(state, -1) == LUA_TTABLE;
          this->loadValid &= valid;
          if (!valid) {
            ShowErrorDialog(std::format("invalid data type: {0}.{1}", name, key), 0);
          }
          if (this->loadValid) {
            int kind = 0;
            unsigned int hash = 5381;
            for (const char* character = key.c_str(); *character != '\0'; ++character) {
              hash = hash * 33 + *character;
            }
            switch (hash) {
            case 0xBDE09DB7:
              kind = 1;
              break;
            case 0x78974D0E:
              kind = 2;
              break;
            case 0x7A2F6FD4:
              kind = 3;
              break;
            case 0xA515CF8B:
              kind = 4;
              break;
            default:
              valid = false;
              break;
            }
            this->loadValid &= valid;
            if (!valid) {
              ShowErrorDialog(std::format("invalid value: {0}.{1}", name, key), 0);
            }
            if (this->loadValid) {
              lua_pushnil(state);
              while (lua_next(state, -2) != 0) {
                if (this->loadValid) {
                  std::string category;
                  type = lua_type(this->script.GetState(), -2);
                  if (type == LUA_TSTRING) {
                    category = lua_tolstring(this->script.GetState(), -2, nullptr);
                    result = 0;
                  } else {
                    result = (type != LUA_TNIL) + 1;
                  }
                  this->loadValid &= result == 0;
                  if (result != 0) {
                    ShowErrorDialog(std::format("invalid key({0}): {1}.{2}", result, name, key), 0);
                  }
                  if (this->loadValid) {
                    valid = lua_type(state, -1) == LUA_TTABLE;
                    this->loadValid &= valid;
                    if (!valid) {
                      ShowErrorDialog(std::format("invalid data type: {0}.{1}.{2}", name, key, category), 0);
                    }
                    if (this->loadValid) {
                      int categoryIndex = 0;
                      hash = 5381;
                      for (const char* character = category.c_str(); *character != '\0'; ++character) {
                        hash = hash * 33 + *character;
                      }
                      switch (hash) {
                      case 0x95E1BB50:
                        break;
                      case 0x2B3A7568:
                        categoryIndex = 1;
                        break;
                      case 0xBB80A3D1:
                        categoryIndex = 2;
                        break;
                      case 0xBB79C188:
                        categoryIndex = 3;
                        break;
                      case 0xA61EE35E:
                        categoryIndex = 4;
                        break;
                      case 0x907C4C76:
                        categoryIndex = 5;
                        break;
                      case 0x65FD5769:
                        categoryIndex = 6;
                        break;
                      case 0x781614BE:
                        categoryIndex = 7;
                        break;
                      default:
                        valid = false;
                        break;
                      }
                      this->loadValid &= valid;
                      if (!valid) {
                        ShowErrorDialog(std::format("invalid value: {0}.{1}.{2}", name, key, category), 0);
                      }
                      if (this->loadValid) {
                        lua_pushnil(state);
                        while (lua_next(state, -2) != 0) {
                          if (this->loadValid) {
                            std::string field;
                            type = lua_type(this->script.GetState(), -2);
                            if (type == LUA_TSTRING) {
                              field = lua_tolstring(this->script.GetState(), -2, nullptr);
                              result = 0;
                            } else {
                              result = (type != LUA_TNIL) + 1;
                            }
                            valid = result == 0;
                            this->loadValid &= valid;
                            if (result != 0) {
                              ShowErrorDialog(std::format("invalid key({0}): {1}.{2}.{3}", result, name, key, category), 0);
                            }
                            if (this->loadValid) {
                              int fieldIndex = 0;
                              hash = 5381;
                              for (const char* character = field.c_str(); *character != '\0'; ++character) {
                                hash = hash * 33 + *character;
                              }
                              switch (hash) {
                              case 0x09B447CA:
                                fieldIndex = 1;
                                break;
                              case 0x27A83F77:
                                fieldIndex = 2;
                                break;
                              default:
                                valid = false;
                                break;
                              }
                              this->loadValid &= valid;
                              if (!valid) {
                                ShowErrorDialog(std::format("invalid key: {0}.{1}.{2}", name, key, category), 0);
                              }
                              if (this->loadValid) {
                                int value;
                                type = lua_type(this->script.GetState(), -1);
                                if (type == LUA_TNUMBER) {
                                  value = static_cast<int>(lua_tonumber(this->script.GetState(), -1));
                                  result = 0;
                                } else {
                                  result = (type != LUA_TNIL) + 1;
                                }
                                this->loadValid &= result == 0;
                                if (result != 0) {
                                  ShowErrorDialog(std::format("invalid value: {0}.{1}.{2}.{3}", name, key, category, field), 0);
                                }
                                if (this->loadValid) {
                                  this->categoryValues[kind][categoryIndex][fieldIndex].value = value;
                                }
                              }
                            }
                          }
                          lua_settop(state, -2);
                        }
                      }
                    }
                  }
                }
                lua_settop(state, -2);
              }
            }
          }
        }
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}

// OFFSET: 0x006B5DA0
// FUNCTION: fn_06198
void EquipmentProperties::CEquipmentPropertiesMgr::LoadRefiningBonus() {
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "RefiningBonus";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      if (this->loadValid) {
        std::string key;
        int result = 0;
        int type = lua_type(this->script.GetState(), -2);
        if (type == LUA_TSTRING) {
          key = lua_tolstring(this->script.GetState(), -2, nullptr);
        } else {
          result = (type != LUA_TNIL) + 1;
        }
        this->loadValid &= result == 0;
        if (result != 0) {
          ShowErrorDialog(std::format("invalid key({0}): {1}", result, name), 0);
        }
        if (this->loadValid) {
          bool valid = lua_type(state, -1) == LUA_TTABLE;
          this->loadValid &= valid;
          if (!valid) {
            ShowErrorDialog(std::format("invalid data type: {0}.{1}", name, key), 0);
          }
          if (this->loadValid) {
            int kind = 0;
            unsigned int hash = 5381;
            for (const char* character = key.c_str(); *character != '\0'; ++character) {
              hash = hash * 33 + *character;
            }
            switch (hash) {
            case 0xC3B9E8CE:
              kind = 1;
              break;
            case 0x48A75AE8:
              kind = 2;
              break;
            case 0x18585774:
              kind = 3;
              break;
            case 0x1858930F:
              kind = 4;
              break;
            default:
              valid = false;
              break;
            }
            this->loadValid &= valid;
            if (!valid) {
              ShowErrorDialog(std::format("invalid value: {0}.{1}", name, key), 0);
            }
            if (this->loadValid) {
              lua_pushnil(state);
              while (lua_next(state, -2) != 0) {
                if (this->loadValid) {
                  unsigned int level;
                  type = lua_type(this->script.GetState(), -2);
                  if (type == LUA_TNUMBER) {
                    level = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
                    result = 0;
                  } else {
                    result = (type != LUA_TNIL) + 1;
                  }
                  this->loadValid &= result == 0;
                  if (result != 0) {
                    ShowErrorDialog(std::format("invalid key({0}): {1}.{2}", result, name, key), 0);
                  }
                  if (this->loadValid) {
                    valid = lua_type(state, -1) == LUA_TTABLE;
                    this->loadValid &= valid;
                    if (!valid) {
                      ShowErrorDialog(std::format("invalid data type: {0}.{1}.{2}", name, key, level), 0);
                    }
                    if (this->loadValid) {
                      lua_pushnil(state);
                      while (lua_next(state, -2) != 0) {
                        if (this->loadValid) {
                          unsigned int index = -1;
                          type = lua_type(this->script.GetState(), -2);
                          if (type == LUA_TNUMBER) {
                            index = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
                            result = 0;
                          } else {
                            result = (type != LUA_TNIL) + 1;
                          }
                          this->loadValid &= result == 0;
                          if (result != 0) {
                            ShowErrorDialog(std::format("invalid key({0}): {1}.{2}.{3}", result, name, key, level), 0);
                          }
                          if (this->loadValid) {
                            int value = -1;
                            type = lua_type(this->script.GetState(), -1);
                            if (type == LUA_TNUMBER) {
                              value = static_cast<int>(lua_tonumber(this->script.GetState(), -1));
                              result = 0;
                            } else {
                              result = (type != LUA_TNIL) + 1;
                            }
                            this->loadValid &= result == 0;
                            if (result != 0) {
                              ShowErrorDialog(std::format("invalid value({0}): {1}.{2}.{3}.{4}", result, name, key, level, index), 0);
                            }
                            if (this->loadValid) {
                              this->levelRates[kind][level][index] = static_cast<float>(value);
                            }
                          }
                        }
                        lua_settop(state, -2);
                      }
                    }
                  }
                }
                lua_settop(state, -2);
              }
            }
          }
        }
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}

// OFFSET: 0x006B6720
// FUNCTION: fn_06199
void EquipmentProperties::CEquipmentPropertiesMgr::LoadSkillGroup() {
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "SkillGroup";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      unsigned int key = 0;
      int keyResult;
      int type = lua_type(this->script.GetState(), -2);
      if (type == LUA_TNUMBER) {
        key = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
        keyResult = 0;
      } else {
        keyResult = (type != LUA_TNIL) + 1;
      }
      const bool keyValid = keyResult == 0;
      this->loadValid &= keyValid;
      if (keyResult != 0) {
        ShowErrorDialog(std::format("{0} table has invalid entry key({1})", name, keyResult), 0);
      }
      if (this->loadValid && lua_type(state, -1) == LUA_TTABLE) {
        lua_pushnil(state);
        while (lua_next(state, -2) != 0) {
          unsigned int skill = 0;
          int result;
          type = lua_type(this->script.GetState(), -1);
          if (type == LUA_TNUMBER) {
            skill = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -1));
            result = 0;
          } else {
            result = (type != LUA_TNIL) + 1;
          }
          this->loadValid &= keyValid;
          if (keyResult != 0) {
            ShowErrorDialog(std::format("SkillGroup[{0}] has invalid 'Skill' table({1})", key, result), 0);
          }
          if (this->loadValid) {
            this->skillGroups[key].push_back(skill);
          }
          lua_settop(state, -2);
        }
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}

// OFFSET: 0x006B7760
// FUNCTION: fn_06210
void EquipmentProperties::CEquipmentPropertiesMgr::SetWindow(UIWindow* window) {
  this->notifyWindow = window;
}

// OFFSET: 0x006A6490
// FUNCTION: fn_06059
int __stdcall subspconsumption(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("subspconsumption", state, 2, 1, 2, -1);
  return 0;
}

// OFFSET: 0x006A6550
// FUNCTION: fn_06060
int __stdcall SubSPdrain(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSPdrain", state, 2, 1, 2, -1);
  return 0;
}

// OFFSET: 0x006A6610
// FUNCTION: fn_06061
int __stdcall SubSkillDelay(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSkillDelay", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A66D0
// FUNCTION: fn_06062
int __stdcall SubSkillMDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSkillMDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A6790
// FUNCTION: fn_06063
int __stdcall SubSkillSP(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSkillSP", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A6850
// FUNCTION: fn_06064
int __stdcall SubSpecificSpellCastTime(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSpecificSpellCastTime", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A6910
// FUNCTION: fn_06065
int __stdcall SubSpellCastTime(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSpellCastTime", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A69D0
// FUNCTION: fn_06066
int __stdcall SubSpellDelay(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSpellDelay", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A6A90
// FUNCTION: fn_06067
int __stdcall AddEXPPercent_KillRace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddEXPPercent_KillRace", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A6B50
// FUNCTION: fn_06068
int __stdcall AddExtParam(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddExtParam", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006A6C10
// FUNCTION: fn_06069
int __stdcall AddGuideAttack(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddGuideAttack", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A6CD0
// FUNCTION: fn_06070
int __stdcall AddHPdrain(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddHPdrain", state, 2, 1, 2, -1);
  return 0;
}

// OFFSET: 0x006A6D90
// FUNCTION: fn_06071
int __stdcall AddHealModifyPercent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddHealModifyPercent", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A6E50
// FUNCTION: fn_06072
int __stdcall AddHealValue(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddHealValue", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A6F10
// FUNCTION: fn_06073
int __stdcall AddIgnore_MRES_RacePercent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddIgnore_MRES_RacePercent", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A6FD0
// FUNCTION: fn_06074
int __stdcall AddIgnore_RES_RacePercent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddIgnore_RES_RacePercent", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A7090
// FUNCTION: fn_06075
int __stdcall AddMDamage_Property(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddMDamage_Property", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006A7150
// FUNCTION: fn_06076
int __stdcall AddMDamage_Size(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddMDamage_Size", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006A7210
// FUNCTION: fn_06077
int __stdcall AddAttrTolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddAttrTolerace", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A72D0
// FUNCTION: fn_06078
int __stdcall AddMdamage_Class(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddMdamage_Class", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A7390
// FUNCTION: fn_06079
int __stdcall AddMdamage_Race(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddMdamage_Race", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A7450
// FUNCTION: fn_06080
int __stdcall AddMeleeAttackDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddMeleeAttackDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A7510
// FUNCTION: fn_06081
int __stdcall AddMeleeAttackReflect(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddMeleeAttackReflect", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A75D0
// FUNCTION: fn_06082
int __stdcall AddNeverknockback(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddNeverknockback", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A7690
// FUNCTION: fn_06083
int __stdcall AddRaceTolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddRaceTolerace", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A7750
// FUNCTION: fn_06084
int __stdcall AddRangeAttackDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddRangeAttackDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A7810
// FUNCTION: fn_06085
int __stdcall AddReceiveItem_Equip(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddReceiveItem_Equip", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A78D0
// FUNCTION: fn_06086
int __stdcall AddReflectMagic(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddReflectMagic", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A7990
// FUNCTION: fn_06087
int __stdcall AddReflectTolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddReflectTolerace", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A7A50
// FUNCTION: fn_06088
int __stdcall addattrtolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("addattrtolerace", state, 3, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A7B10
// FUNCTION: fn_06089
int __stdcall AddSFCTEquipAmount(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSFCTEquipAmount", state, 3, 2, 3, 1);
  return 0;
}

// OFFSET: 0x006A7BD0
// FUNCTION: fn_06090
int __stdcall AddSFCTEquipPermill(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSFCTEquipPermill", state, 3, 3, 3, 1);
  return 0;
}

// OFFSET: 0x006A7C90
// FUNCTION: fn_06091
int __stdcall AddSPconsumption(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSPconsumption", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A7D50
// FUNCTION: fn_06092
int __stdcall addspconsumption(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("addspconsumption", state, 2, 1, 2, -1);
  return 0;
}

// OFFSET: 0x006A7E10
// FUNCTION: fn_06093
int __stdcall AddSPdrain(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSPdrain", state, 2, 1, 2, -1);
  return 0;
}

// OFFSET: 0x006A7ED0
// FUNCTION: fn_06094
int __stdcall AddSkillDelay(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSkillDelay", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A7F90
// FUNCTION: fn_06095
int __stdcall AddSkillMDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSkillMDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A8050
// FUNCTION: fn_06096
int __stdcall AddSkillSP(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSkillSP", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A8110
// FUNCTION: fn_06097
int __stdcall AddSpecificSpellCastTime(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSpecificSpellCastTime", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A81D0
// FUNCTION: fn_06098
int __stdcall AddSpellCastTime(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSpellCastTime", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8290
// FUNCTION: fn_06099
int __stdcall AddBowAttackDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddBowAttackDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A8350
// FUNCTION: fn_06100
int __stdcall AddSpellDelay(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddSpellDelay", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A84D0
// FUNCTION: fn_06102
int __stdcall Clairvoyance(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("Clairvoyance", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8590
// FUNCTION: fn_06103
int __stdcall ClassAddDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("ClassAddDamage", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006A8650
// FUNCTION: fn_06104
int __stdcall ClassSubDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("ClassSubDamage", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006A87D0
// FUNCTION: fn_06106
int __stdcall DisableSkill(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("DisableSkill", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A8890
// FUNCTION: fn_06107
int __stdcall EnableSkill(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("EnableSkill", state, 2, 2, 1, -1);
  return 0;
}

// OFFSET: 0x006A8950
// FUNCTION: fn_06108
int __stdcall IndestructibleArmor(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("IndestructibleArmor", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8A10
// FUNCTION: fn_06109
int __stdcall IndestructibleWeapon(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("IndestructibleWeapon", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8AD0
// FUNCTION: fn_06110
int __stdcall AddCRIPercent_Race(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddCRIPercent_Race", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A8B90
// FUNCTION: fn_06111
int __stdcall Magicimmune(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("Magicimmune", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8C50
// FUNCTION: fn_06112
int __stdcall NoDispell(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("NoDispell", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8D10
// FUNCTION: fn_06113
int __stdcall NoJamstone(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("NoJamstone", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8DD0
// FUNCTION: fn_06114
int __stdcall NoMadogearfuel(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("NoMadogearfuel", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8E90
// FUNCTION: fn_06115
int __stdcall PerfectDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("PerfectDamage", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8F50
// FUNCTION: fn_06116
int __stdcall RaceAddDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("RaceAddDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A9010
// FUNCTION: fn_06117
int __stdcall RaceAddDamageSelf(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("RaceAddDamageSelf", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A90D0
// FUNCTION: fn_06118
int __stdcall RaceSubDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("RaceSubDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A9190
// FUNCTION: fn_06119
int __stdcall RaceSubDamageSelf(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("RaceSubDamageSelf", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A9250
// FUNCTION: fn_06120
int __stdcall Reincarnation(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("Reincarnation", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006A9310
// FUNCTION: fn_06121
int __stdcall AddDamage_CRI(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddDamage_CRI", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A93D0
// FUNCTION: fn_06122
int __stdcall SetIgnoreDEFClass(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SetIgnoreDEFClass", state, 1, -1, -1, -1);
  return 0;
}

// OFFSET: 0x006A9490
// FUNCTION: fn_06123
int __stdcall SetIgnoreDEFRace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SetIgnoreDEFRace", state, 1, -1, -1, -1);
  return 0;
}

// OFFSET: 0x006A9550
// FUNCTION: fn_06124
int __stdcall SetIgnoreDefClass_Percent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SetIgnoreDefClass_Percent", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A9610
// FUNCTION: fn_06125
int __stdcall SetIgnoreDefRace_Percent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SetIgnoreDefRace_Percent", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A96D0
// FUNCTION: fn_06126
int __stdcall SetIgnoreMdefClass(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SetIgnoreMdefClass", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A9790
// FUNCTION: fn_06127
int __stdcall SetIgnoreMdefRace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SetIgnoreMdefRace", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006A9850
// FUNCTION: fn_06128
int __stdcall SetInvestigate(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SetInvestigate", state, 0, -1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8410
// FUNCTION: fn_06101
int __stdcall BodyAttribute(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("BodyAttribute", state, 1, -1, -1, -1);
  return 0;
}

// OFFSET: 0x006A8710
// FUNCTION: fn_06105
int __stdcall Condition(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("Condition", state, 3, -1, -1, -1);
  return 0;
}

// OFFSET: 0x006A9910
// FUNCTION: fn_06129
int __stdcall SplashAttack(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int count;
  int valuePosition;
  int indexPosition;
  int keyPosition;
  keyPosition = -1;
  indexPosition = -1;
  valuePosition = 1;
  count = 1;
  g_equipmentPropertiesMgr->ApplyProperty("SplashAttack", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A99D0
// FUNCTION: fn_06130
int __stdcall SubAttrTolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubAttrTolerace", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9A90
// FUNCTION: fn_06131
int __stdcall subattrtolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 3;
  g_equipmentPropertiesMgr->ApplyProperty("subattrtolerace", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9B50
// FUNCTION: fn_06132
int __stdcall AddDamage_HIT(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("AddDamage_HIT", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9C10
// FUNCTION: fn_06133
int __stdcall SubBowAttackDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubBowAttackDamage", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9CD0
// FUNCTION: fn_06134
int __stdcall SubCRIPercent_Race(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubCRIPercent_Race", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9D90
// FUNCTION: fn_06135
int __stdcall SubDamage_CRI(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubDamage_CRI", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9E50
// FUNCTION: fn_06136
int __stdcall SubDamage_HIT(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubDamage_HIT", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9F10
// FUNCTION: fn_06137
int __stdcall SubDamage_Property(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 3;
  int count = 3;
  g_equipmentPropertiesMgr->ApplyProperty("SubDamage_Property", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006A9FD0
// FUNCTION: fn_06138
int __stdcall SubDamage_SKID(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = 2;
  int valuePosition = 3;
  int count = 3;
  g_equipmentPropertiesMgr->ApplyProperty("SubDamage_SKID", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA090
// FUNCTION: fn_06139
int __stdcall SubDamage_Size(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 3;
  int count = 3;
  g_equipmentPropertiesMgr->ApplyProperty("SubDamage_Size", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA150
// FUNCTION: fn_06140
int __stdcall SubEXPPercent_KillRace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubEXPPercent_KillRace", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA210
// FUNCTION: fn_06141
int __stdcall SubExtParam(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 3;
  int count = 3;
  g_equipmentPropertiesMgr->ApplyProperty("SubExtParam", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA2D0
// FUNCTION: fn_06142
int __stdcall SubGuideAttack(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 1;
  int count = 1;
  g_equipmentPropertiesMgr->ApplyProperty("SubGuideAttack", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA390
// FUNCTION: fn_06143
int __stdcall AddDamage_Property(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 3;
  int count = 3;
  g_equipmentPropertiesMgr->ApplyProperty("AddDamage_Property", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA450
// FUNCTION: fn_06144
int __stdcall SubHPdrain(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = 2;
  int valuePosition = 1;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubHPdrain", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA510
// FUNCTION: fn_06145
int __stdcall SubHealModifyPercent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 1;
  int count = 1;
  g_equipmentPropertiesMgr->ApplyProperty("SubHealModifyPercent", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA5D0
// FUNCTION: fn_06146
int __stdcall SubHealValue(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 1;
  int count = 1;
  g_equipmentPropertiesMgr->ApplyProperty("SubHealValue", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA690
// FUNCTION: fn_06147
int __stdcall SubIgnore_MRES_RacePercent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubIgnore_MRES_RacePercent", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA750
// FUNCTION: fn_06148
int __stdcall SubIgnore_RES_RacePercent(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  int keyPosition = -1;
  int indexPosition = -1;
  int valuePosition = 2;
  int count = 2;
  g_equipmentPropertiesMgr->ApplyProperty("SubIgnore_RES_RacePercent", state, count, valuePosition, indexPosition, keyPosition);
  return 0;
}

// OFFSET: 0x006AA810
// FUNCTION: fn_06149
int __stdcall SubMDamage_Property(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubMDamage_Property", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006AA8D0
// FUNCTION: fn_06150
int __stdcall SubMDamage_Size(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubMDamage_Size", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006AA990
// FUNCTION: fn_06151
int __stdcall SubMdamage_Class(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubMdamage_Class", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006AAA50
// FUNCTION: fn_06152
int __stdcall SubMdamage_Race(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubMdamage_Race", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006AAB10
// FUNCTION: fn_06153
int __stdcall SubMeleeAttackDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubMeleeAttackDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006AABD0
// FUNCTION: fn_06154
int __stdcall AddDamage_SKID(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddDamage_SKID", state, 3, 3, 2, -1);
  return 0;
}

// OFFSET: 0x006AAC90
// FUNCTION: fn_06155
int __stdcall SubMeleeAttackReflect(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubMeleeAttackReflect", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006AAD50
// FUNCTION: fn_06156
int __stdcall SubNeverknockback(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubNeverknockback", state, 0, -1, -1, -1);
  return 0;
}

// OFFSET: 0x006AAE10
// FUNCTION: fn_06157
int __stdcall SubRaceTolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubRaceTolerace", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006AAED0
// FUNCTION: fn_06158
int __stdcall SubRangeAttackDamage(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubRangeAttackDamage", state, 2, 2, -1, -1);
  return 0;
}

// OFFSET: 0x006AAF90
// FUNCTION: fn_06159
int __stdcall SubReceiveItem_Equip(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubReceiveItem_Equip", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006AB050
// FUNCTION: fn_06160
int __stdcall SubReflectMagic(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubReflectMagic", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006AB110
// FUNCTION: fn_06161
int __stdcall SubReflectTolerace(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubReflectTolerace", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006AB1D0
// FUNCTION: fn_06162
int __stdcall SubSFCTEquipAmount(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSFCTEquipAmount", state, 3, 2, 3, 1);
  return 0;
}

// OFFSET: 0x006AB290
// FUNCTION: fn_06163
int __stdcall SubSFCTEquipPermill(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSFCTEquipPermill", state, 3, 2, 3, 1);
  return 0;
}

// OFFSET: 0x006AB350
// FUNCTION: fn_06164
int __stdcall SubSPconsumption(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("SubSPconsumption", state, 1, 1, -1, -1);
  return 0;
}

// OFFSET: 0x006AB410
// FUNCTION: fn_06165
int __stdcall AddDamage_Size(lua_State* state) {
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->ApplyProperty("AddDamage_Size", state, 3, 3, -1, -1);
  return 0;
}

// OFFSET: 0x006AB520
// FUNCTION: fn_06168
void EquipmentProperties::CEquipmentPropertiesMgr::ApplyProperty(const char* name, lua_State* state, const int& count, const int& valuePosition,
                                                               const int& indexPosition, const int& keyPosition) {
  int index = -1;
  int value = 0x7FFFFFFF;
  int key = -1;
  std::string text = std::format("{0}", name);
  for (int position = 1; position <= count; ++position) {
    const int stackIndex = position - count - 1;
    if (!lua_isnumber(state, stackIndex)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", position).c_str());
      return;
    }

    const int number = static_cast<int>(lua_tonumber(state, stackIndex));
    if (valuePosition == position) {
      value = number;
    } else if (indexPosition == position) {
      index = number;
    } else if (keyPosition == position) {
      key = number;
    } else {
      text = std::format("{0} arg{1}:{2}", text, position, number);
    }
  }

  if (this->currentEquipment != 0) {
    reinterpret_cast<CEquipment*>(this->currentEquipment)->AddProperty(text, name, index, value, key);
  }
}

// OFFSET: 0x006AB810
// FUNCTION: fn_06169
void EquipmentProperties::CEquipmentPropertiesMgr::EquipItem(const int& itemId, bool notify) {
  this->activeItems.insert(itemId);

  auto combinations = this->m_itemCombinations.find(itemId);
  if (combinations == this->m_itemCombinations.end()) {
    return;
  }

  for (const unsigned int& combinationId : combinations->second) {
    auto items = this->m_combinationItems.find(combinationId);
    if (items == this->m_combinationItems.end()) {
      continue;
    }

    bool complete = true;
    for (const unsigned int& item : items->second) {
      if (this->activeItems.count(item) == 0) {
        complete = false;
        break;
      }
    }
    if (!complete) {
      continue;
    }

    if (this->properties3.find(combinationId) == this->properties3.end()) {
      this->properties3.try_emplace(combinationId, new CEquipmentCombiitem(combinationId));
      this->properties3[combinationId]->Load(notify);
    }
  }
}

// OFFSET: 0x006ABF80
// FUNCTION: fn_06171
void EquipmentProperties::CEquipmentPropertiesMgr::UnequipItem(const int& itemId, bool notify) {
  this->activeItems.extract(itemId);

  auto combinations = this->m_itemCombinations.find(itemId);
  if (combinations == this->m_itemCombinations.end()) {
    return;
  }

  for (const unsigned int& combinationId : combinations->second) {
    auto items = this->m_combinationItems.find(combinationId);
    if (items == this->m_combinationItems.end()) {
      continue;
    }

    for (const unsigned int& item : items->second) {
      if (this->activeItems.count(item) != 0) {
        continue;
      }

      auto combination = this->properties3.find(combinationId);
      if (combination != this->properties3.end()) {
        if (notify == true) {
          combination->second->Clear();
        } else {
          combination->second->CEquipment::Clear();
        }
        delete combination->second;
        this->properties3.erase(combination);
      }
      break;
    }
  }
}

// OFFSET: 0x006AD0E0
// FUNCTION: fn_06182
void EquipmentProperties::CEquipmentPropertiesMgr::GetDescriptions(const int& group, std::list<std::string>& lines) {
  if (this->m_dirty == true) {
    this->Rebuild();
  }

  lines.clear();
  for (const auto& entry : std::as_const(this->store->GetProperties())) {
    if (group != 0 && entry.first != group) {
      continue;
    }
    for (const auto& property : entry.second) {
      std::vector<std::string> parts = Split(property.second->Describe(false, false), "\n");
      for (const std::string& part : parts) {
        lines.push_back(part);
      }
    }
  }
}

// OFFSET: 0x006AD4A0
// FUNCTION: fn_06183
__declspec(noinline) EquipmentPropertyDefinition* EquipmentProperties::CEquipmentPropertiesMgr::FindDefinition(const std::uint64_t& key) {
  auto found = this->m_definitionsByName.find(key);
  if (this->m_definitionsByName.end() == found) {
    return nullptr;
  }
  return found->second;
}

// OFFSET: 0x006AD580
// FUNCTION: fn_06185
int EquipmentProperties::CEquipmentPropertiesMgr::GetRefiningBonus(const int& first, const int& second, const unsigned int& third) {
  auto outer = this->levelRates.find(first);
  if (this->levelRates.end() == outer) {
    return 0;
  }
  auto middle = outer->second.find(second);
  if (outer->second.end() == middle) {
    return 0;
  }
  if (third == 0 || third > middle->second.size()) {
    return 0;
  }
  auto inner = middle->second.find(third);
  if (middle->second.end() == inner) {
    return 0;
  }
  return static_cast<int>(inner->second);
}

// OFFSET: 0x006B6AF0
// FUNCTION: fn_06200
void EquipmentProperties::CEquipmentPropertiesMgr::AddExpansion(const int& kind, const int& itemId, bool notify) {
  this->m_dirty = true;

  unsigned int key = itemId;
  switch (kind) {
  case 0:
  case 1:
    key = kind;
    break;
  }

  auto found = this->properties2.find(key);
  if (found != this->properties2.end()) {
    delete found->second;
    this->properties2.erase(found);
  }

  CEquipmentExpansion* expansion = new CEquipmentExpansion(itemId, kind);
  CEquipment* equipment = this->properties2.insert({key, expansion}).first->second;
  if (kind == 1) {
    notify = g_windowMgr.LookupWindow(WID_RUNESYSTEMWND) != nullptr;
  }
  equipment->Load(notify);
  this->EquipItem(equipment->GetId(), notify);

  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B6E10
// FUNCTION: fn_06202
void EquipmentProperties::CEquipmentPropertiesMgr::RemoveExpansion(const int& kind, const int& itemId) {
  this->m_dirty = true;

  unsigned int key = itemId;
  switch (kind) {
  case 0:
  case 1:
    key = kind;
    break;
  }

  auto found = this->properties2.find(key);
  if (this->properties2.end() != found) {
    this->UnequipItem(found->second->GetId(), true);
    found->second->Clear();
    delete found->second;
    this->properties2.erase(found);
  }

  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B7020
// FUNCTION: fn_06204
void EquipmentProperties::CEquipmentPropertiesMgr::RefreshExpansion(const int& kind, const int& itemId) {
  unsigned int key = itemId;
  switch (kind) {
  case 0:
  case 1:
    key = kind;
    break;
  }

  auto found = this->properties2.find(key);
  if (found == this->properties2.end()) {
    return;
  }

  this->m_dirty |= found->second->IsPretend();
  auto combinations = this->m_itemCombinations.find(found->second->GetId());
  if (combinations != this->m_itemCombinations.end()) {
    for (const unsigned int& combinationId : combinations->second) {
      if (this->properties3.find(combinationId) != this->properties3.end()) {
        this->properties3[combinationId]->IsPretend();
      }
    }
  }

  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B7390
// FUNCTION: fn_06206
void EquipmentProperties::CEquipmentPropertiesMgr::RefreshShop() {
  const int shopId = g_session.FindShopId("");
  if (this->shopId != shopId) {
    for (auto& entry : this->properties1) {
      const bool previous = entry.second->IsActive();
      this->m_dirty |= entry.second->UpdateActive();
      if (previous != entry.second->IsActive()) {
        if (!entry.second->IsActive()) {
          for (CEquipmentAttachable& attachable : entry.second->GetAttachables()) {
            this->UnequipItem(attachable.GetId(), false);
          }
          this->UnequipItem(entry.second->GetId(), false);
        } else {
          this->EquipItem(entry.second->GetId(), false);
          for (CEquipmentAttachable& attachable : entry.second->GetAttachables()) {
            this->EquipItem(attachable.GetId(), false);
          }
        }
      }
    }
  }
  this->shopId = shopId;
  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B7530
// FUNCTION: fn_06207
void EquipmentProperties::CEquipmentPropertiesMgr::Rebuild() {
  this->store->Clear();
  for (auto& entry : this->properties1) {
    if (entry.second->IsActive() == true) {
      *this->store += entry.second->GetProperties();
    }
  }
  for (auto& entry : this->properties2) {
    *this->store += entry.second->GetProperties();
  }
  for (auto& entry : this->properties3) {
    *this->store += entry.second->GetProperties();
  }
  this->store->RemoveDuplicateNames();
  this->m_dirty = false;
}

// OFFSET: 0x006B75C0
// FUNCTION: fn_06208
void EquipmentProperties::CEquipmentPropertiesMgr::SetCurrentEquipment(CEquipment* equipment) {
  this->displayValues.clear();
  this->currentEquipment = reinterpret_cast<int>(equipment);
}

// OFFSET: 0x006B7770
// FUNCTION: fn_06211
void EquipmentProperties::CEquipmentPropertiesMgr::RefreshPretend() {
  for (auto& entry : this->properties1) {
    this->m_dirty |= entry.second->IsPretend();
  }
  for (auto& entry : this->properties2) {
    this->m_dirty |= entry.second->IsPretend();
  }
  for (auto& entry : this->properties3) {
    this->m_dirty |= entry.second->IsPretend();
  }

  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006BCB90
// FUNCTION: fn_06275
int CheckJobGroup2(lua_State* state) {
  int group;
  switch (g_session.m_job) {
  case JT_NOVICE:
  case JT_NOVICE_B:
    group = 0;
    break;
  case JT_NOVICE_H:
    group = 10;
    break;
  case JT_SWORDMAN:
  case JT_MAGICIAN:
  case JT_ARCHER:
  case JT_ACOLYTE:
  case JT_MERCHANT:
  case JT_THIEF:
  case JT_SWORDMAN_B:
  case JT_MAGICIAN_B:
  case JT_ARCHER_B:
  case JT_ACOLYTE_B:
  case JT_MERCHANT_B:
  case JT_THIEF_B:
    group = 1;
    break;
  case JT_SWORDMAN_H:
  case JT_MAGICIAN_H:
  case JT_ARCHER_H:
  case JT_ACOLYTE_H:
  case JT_MERCHANT_H:
  case JT_THIEF_H:
    group = 11;
    break;
  case JT_KNIGHT:
  case JT_PRIEST:
  case JT_WIZARD:
  case JT_BLACKSMITH:
  case JT_HUNTER:
  case JT_ASSASSIN:
  case JT_CRUSADER:
  case JT_MONK:
  case JT_SAGE:
  case JT_ROGUE:
  case JT_ALCHEMIST:
  case JT_BARD:
  case JT_DANCER:
  case JT_KNIGHT_B:
  case JT_PRIEST_B:
  case JT_WIZARD_B:
  case JT_BLACKSMITH_B:
  case JT_HUNTER_B:
  case JT_ASSASSIN_B:
  case JT_CRUSADER_B:
  case JT_MONK_B:
  case JT_SAGE_B:
  case JT_ROGUE_B:
  case JT_ALCHEMIST_B:
  case JT_BARD_B:
  case JT_DANCER_B:
    group = 2;
    break;
  case JT_KNIGHT_H:
  case JT_PRIEST_H:
  case JT_WIZARD_H:
  case JT_BLACKSMITH_H:
  case JT_HUNTER_H:
  case JT_ASSASSIN_H:
  case JT_CRUSADER_H:
  case JT_MONK_H:
  case JT_SAGE_H:
  case JT_ROGUE_H:
  case JT_ALCHEMIST_H:
  case JT_BARD_H:
  case JT_DANCER_H:
    group = 12;
    break;
  case JT_RUNE_KNIGHT:
  case JT_WARLOCK:
  case JT_RANGER:
  case JT_ARCHBISHOP:
  case JT_MECHANIC:
  case JT_GUILLOTINE_CROSS:
  case JT_ROYAL_GUARD:
  case JT_SORCERER:
  case JT_MINSTREL:
  case JT_WANDERER:
  case JT_SURA:
  case JT_GENETIC:
  case JT_SHADOW_CHASER:
  case JT_RUNE_KNIGHT_B:
  case JT_WARLOCK_B:
  case JT_RANGER_B:
  case JT_ARCHBISHOP_B:
  case JT_MECHANIC_B:
  case JT_GUILLOTINE_CROSS_B:
  case JT_ROYAL_GUARD_B:
  case JT_SORCERER_B:
  case JT_MINSTREL_B:
  case JT_WANDERER_B:
  case JT_SURA_B:
  case JT_GENETIC_B:
  case JT_SHADOW_CHASER_B:
    group = 3;
    break;
  case JT_RUNE_KNIGHT_H:
  case JT_WARLOCK_H:
  case JT_RANGER_H:
  case JT_ARCHBISHOP_H:
  case JT_MECHANIC_H:
  case JT_GUILLOTINE_CROSS_H:
  case JT_ROYAL_GUARD_H:
  case JT_SORCERER_H:
  case JT_MINSTREL_H:
  case JT_WANDERER_H:
  case JT_SURA_H:
  case JT_GENETIC_H:
  case JT_SHADOW_CHASER_H:
    group = 13;
    break;
  case JT_SUPERNOVICE:
  case JT_GUNSLINGER:
  case JT_NINJA:
  case JT_SUPERNOVICE_B:
  case JT_TAEKWON:
  case JT_NINJA_B:
  case JT_TAEKWON_B:
  case JT_GUNSLINGER_B:
  case JT_DRUID:
  case JT_DRUID_B:
    group = 9;
    break;
  case JT_STAR:
  case JT_STAR2:
  case JT_LINKER:
  case JT_STAR_B:
  case JT_LINKER_B:
    group = 19;
    break;
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_REBELLION:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_REBELLION_B:
  case JT_STAR_EMPEROR:
  case JT_SOUL_REAPER:
  case JT_STAR_EMPEROR_B:
  case JT_SOUL_REAPER_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
    group = 29;
    break;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
    group = 30;
    break;
  case JT_DRAGON_KNIGHT:
  case JT_MEISTER:
  case JT_SHADOW_CROSS:
  case JT_ARCH_MAGE:
  case JT_CARDINAL:
  case JT_WINDHAWK:
  case JT_IMPERIAL_GUARD:
  case JT_BIOLO:
  case JT_ABYSS_CHASER:
  case JT_ELEMENTAL_MASTER:
  case JT_INQUISITOR:
  case JT_TROUBADOUR:
  case JT_TROUVERE:
    group = 14;
    break;
  case JT_SKY_EMPEROR:
  case JT_SOUL_ASCETIC:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
  case JT_NIGHT_WATCH:
  case JT_HYPER_NOVICE:
  case JT_SPIRIT_HANDLER:
  case JT_ALITEA:
    group = 39;
    break;
  default:
    group = 99;
    break;
  }

  lua_pushnumber(state, group);
  return 1;
}

// OFFSET: 0x006BCE30
// FUNCTION: fn_06276
int GetCategoryJob(lua_State* state) {
  std::vector<int> args;
  int count = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", count + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++count;
  }

  int job = 0;
  switch (args[0]) {
  case 1:
    job = GetFirstJob(g_session.m_job);
    break;
  case 2:
    switch (g_session.m_job) {
    case JT_NOVICE:
    case JT_NOVICE_H:
    case JT_NOVICE_B:
      job = 0;
      break;
    case JT_SWORDMAN:
    case JT_SWORDMAN_H:
    case JT_SWORDMAN_B:
      job = 1;
      break;
    case JT_MAGICIAN:
    case JT_MAGICIAN_H:
    case JT_MAGICIAN_B:
      job = 2;
      break;
    case JT_ARCHER:
    case JT_ARCHER_H:
    case JT_ARCHER_B:
      job = 3;
      break;
    case JT_ACOLYTE:
    case JT_ACOLYTE_H:
    case JT_ACOLYTE_B:
      job = 4;
      break;
    case JT_MERCHANT:
    case JT_MERCHANT_H:
    case JT_MERCHANT_B:
      job = 5;
      break;
    case JT_THIEF:
    case JT_THIEF_H:
    case JT_THIEF_B:
      job = 6;
      break;
    case JT_KNIGHT:
    case JT_KNIGHT_H:
    case JT_KNIGHT_B:
    case JT_RUNE_KNIGHT:
    case JT_RUNE_KNIGHT_H:
    case JT_RUNE_KNIGHT_B:
    case JT_DRAGON_KNIGHT:
      job = 7;
      break;
    case JT_PRIEST:
    case JT_PRIEST_H:
    case JT_PRIEST_B:
    case JT_ARCHBISHOP:
    case JT_ARCHBISHOP_H:
    case JT_ARCHBISHOP_B:
    case JT_CARDINAL:
      job = 8;
      break;
    case JT_WIZARD:
    case JT_WIZARD_H:
    case JT_WIZARD_B:
    case JT_WARLOCK:
    case JT_WARLOCK_H:
    case JT_WARLOCK_B:
    case JT_ARCH_MAGE:
      job = 9;
      break;
    case JT_BLACKSMITH:
    case JT_BLACKSMITH_H:
    case JT_BLACKSMITH_B:
    case JT_MECHANIC:
    case JT_MECHANIC_H:
    case JT_MECHANIC_B:
    case JT_MEISTER:
      job = 10;
      break;
    case JT_HUNTER:
    case JT_HUNTER_H:
    case JT_HUNTER_B:
    case JT_RANGER:
    case JT_RANGER_H:
    case JT_RANGER_B:
    case JT_WINDHAWK:
      job = 11;
      break;
    case JT_ASSASSIN:
    case JT_ASSASSIN_H:
    case JT_ASSASSIN_B:
    case JT_GUILLOTINE_CROSS:
    case JT_GUILLOTINE_CROSS_H:
    case JT_GUILLOTINE_CROSS_B:
    case JT_SHADOW_CROSS:
      job = 12;
      break;
    case JT_CRUSADER:
    case JT_CRUSADER_H:
    case JT_CRUSADER_B:
    case JT_ROYAL_GUARD:
    case JT_ROYAL_GUARD_H:
    case JT_ROYAL_GUARD_B:
    case JT_IMPERIAL_GUARD:
      job = 14;
      break;
    case JT_MONK:
    case JT_MONK_H:
    case JT_MONK_B:
    case JT_SURA:
    case JT_SURA_H:
    case JT_SURA_B:
    case JT_INQUISITOR:
      job = 15;
      break;
    case JT_SAGE:
    case JT_SAGE_H:
    case JT_SAGE_B:
    case JT_SORCERER:
    case JT_SORCERER_H:
    case JT_SORCERER_B:
    case JT_ELEMENTAL_MASTER:
      job = 16;
      break;
    case JT_ROGUE:
    case JT_ROGUE_H:
    case JT_ROGUE_B:
    case JT_SHADOW_CHASER:
    case JT_SHADOW_CHASER_H:
    case JT_SHADOW_CHASER_B:
    case JT_ABYSS_CHASER:
      job = 17;
      break;
    case JT_ALCHEMIST:
    case JT_ALCHEMIST_H:
    case JT_ALCHEMIST_B:
    case JT_GENETIC:
    case JT_GENETIC_H:
    case JT_GENETIC_B:
    case JT_BIOLO:
      job = 18;
      break;
    case JT_BARD:
    case JT_BARD_H:
    case JT_BARD_B:
    case JT_MINSTREL:
    case JT_MINSTREL_H:
    case JT_MINSTREL_B:
    case JT_TROUBADOUR:
      job = 19;
      break;
    case JT_DANCER:
    case JT_DANCER_H:
    case JT_DANCER_B:
    case JT_WANDERER:
    case JT_WANDERER_H:
    case JT_WANDERER_B:
    case JT_TROUVERE:
      job = 20;
      break;
    case JT_SUPERNOVICE:
    case JT_SUPERNOVICE_B:
    case JT_SUPERNOVICE2:
    case JT_SUPERNOVICE2_B:
    case JT_HYPER_NOVICE:
      job = 23;
      break;
    case JT_NINJA:
    case JT_KAGEROU:
    case JT_OBORO:
    case JT_NINJA_B:
    case JT_KAGEROU_B:
    case JT_OBORO_B:
    case JT_SHINKIRO:
    case JT_SHIRANUI:
      job = 25;
      break;
    case JT_GUNSLINGER:
    case JT_REBELLION:
    case JT_GUNSLINGER_B:
    case JT_REBELLION_B:
    case JT_NIGHT_WATCH:
      job = 24;
      break;
    case JT_TAEKWON:
    case JT_TAEKWON_B:
      job = 4046;
      break;
    case JT_STAR:
    case JT_STAR_B:
    case JT_STAR_EMPEROR:
    case JT_STAR_EMPEROR_B:
    case JT_SKY_EMPEROR:
      job = 4047;
      break;
    case JT_LINKER:
    case JT_LINKER_B:
    case JT_SOUL_REAPER:
    case JT_SOUL_REAPER_B:
    case JT_SOUL_ASCETIC:
      job = 4049;
      break;
    case JT_DO_SUMMONER:
    case JT_DO_SUMMONER_B:
    case JT_SPIRIT_HANDLER:
      job = 4218;
      break;
    case JT_DRUID:
    case JT_DRUID_B:
    case JT_KARNOS:
    case JT_KARNOS_B:
    case JT_ALITEA:
      job = 4351;
      break;
    default:
      job = g_session.m_job;
      break;
    }
    break;
  case 3:
    job = GetNormalJob(g_session.m_job);
    break;
  case 4:
    job = GetNormalThirdJob(g_session.m_job);
    break;
  }

  lua_pushnumber(state, job);
  return 1;
}

struct EquipLocationSource {
  virtual void Destroy();
  virtual void Load();
  virtual void Clear();
  virtual int GetLocation();
};

// OFFSET: 0x006BD3C0
// FUNCTION: fn_06277
int GetEquipArmorLv(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  EquipmentProperties::CEquipmentPropertiesMgr* mgr = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
  EquipmentProperties::CEquipmentWearable* wearable = mgr->FindProperty1(args[0]);
  const int value = wearable != nullptr ? wearable->GetValue(2) : 0;
  lua_pushnumber(state, value);
  return 1;
}

// OFFSET: 0x006BDCE0
// FUNCTION: fn_06280
int GetEquipWeaponLv(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  EquipmentProperties::CEquipmentPropertiesMgr* mgr = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
  EquipmentProperties::CEquipmentWearable* wearable = mgr->FindProperty1(args[0]);
  const int value = wearable != nullptr ? wearable->GetValue(2) : 0;
  lua_pushnumber(state, value);
  return 1;
}

// OFFSET: 0x006BE2F0
// FUNCTION: fn_06282
int GetLocation(lua_State* state) {
  int index = 0;
  EquipmentProperties::CEquipmentPropertiesMgr* mgr = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
  if (mgr->currentEquipment != 0) {
    const int location = reinterpret_cast<EquipLocationSource*>(mgr->currentEquipment)->GetLocation();
    index = GetEquipLocationIndex(location);
  }
  lua_pushnumber(state, index);
  return 1;
}

// OFFSET: 0x006BE3A0
// FUNCTION: fn_06283
int GetMapName(lua_State* state) {
  std::string mapName;
  if (g_modeMgr.GetGameMode() != nullptr) {
    mapName = std::string(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->mapName);
  }
  mapName = mapName.substr(0, mapName.find(".rsw"));
  lua_pushstring(state, mapName.c_str());
  return 1;
}

// OFFSET: 0x006BE680
// FUNCTION: fn_06284
int GetPetRelationship(lua_State* state) {
  lua_pushnumber(state, g_session.GetPetFriendlyState(g_session.m_petFriendly));
  return 1;
}

// OFFSET: 0x006BE6C0
// FUNCTION: fn_06285
int GetPureJob(lua_State* state) {
  lua_pushnumber(state, GetFirstJob(g_session.m_job));
  return 1;
}

// OFFSET: 0x00E16510
// FUNCTION: fn_23832
__declspec(noinline) CSkillInfo* CSession::FindGroupedSkill(std::uint32_t id) {
  return this->m_skillBundle.FindGrouped(id);
}

// OFFSET: 0x006BEA40
// FUNCTION: fn_06287
int GetSkillLevel(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  CSkillInfo* skill = g_session.FindGroupedSkill(args[0]);
  lua_pushnumber(state, skill != nullptr ? skill->level : 0);
  return 1;
}

// OFFSET: 0x006BEC30
// FUNCTION: fn_06288
int GetPlayerValue(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  std::map<int, int>& traitStats = g_session.m_traits.stats;
  int value = 0;
  switch (args[0]) {
  case SP_SP:
    value = g_session.m_sp;
    break;
  case SP_MAXSP:
    value = g_session.m_maxSp;
    break;
  case SP_BASELEVEL:
    value = g_session.m_level;
    break;
  case SP_CLASS:
    value = g_session.m_job;
    break;
  case SP_USTR:
    value = g_session.m_standardStats[0];
    break;
  case SP_UAGI:
    value = g_session.m_standardStats[1];
    break;
  case SP_UVIT:
    value = g_session.m_standardStats[2];
    break;
  case SP_UINT:
    value = g_session.m_standardStats[3];
    break;
  case SP_UDEX:
    value = g_session.m_standardStats[4];
    break;
  case SP_ULUK:
    value = g_session.m_standardStats[5];
    break;
  case SP_JOBLEVEL:
    value = g_session.m_jobLevel;
    break;
  case 255:
    value = traitStats[219];
    break;
  case 256:
    value = traitStats[220];
    break;
  case 257:
    value = traitStats[221];
    break;
  case 258:
    value = traitStats[222];
    break;
  case 259:
    value = traitStats[223];
    break;
  case 260:
    value = traitStats[224];
    break;
  case 263:
    value = g_runeSystemMgr->GetCurSlotCount();
    break;
  case 264: {
    RuneSetData* runeSet = g_runeSystemMgr->GetCurRuneSet();
    if (runeSet != nullptr) {
      value = runeSet->m_grade;
    }
    break;
  }
  }
  lua_pushnumber(state, value);
  return 1;
}

// OFFSET: 0x006BEFA0
// FUNCTION: fn_06289
int GetWeaponClass(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  EquipmentProperties::CEquipmentPropertiesMgr* mgr = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
  EquipmentProperties::CEquipmentWearable* wearable = mgr->FindProperty1(args[0]);
  int value = 0;
  if (wearable != nullptr) {
    const int result = wearable->GetValue(1);
    if (result != -1) {
      value = result;
    }
  }
  lua_pushnumber(state, value);
  return 1;
}

// OFFSET: 0x006BF240
// FUNCTION: fn_06290
int IsPremiumPcCafe(lua_State* state) {
  lua_pushnumber(state, g_session.m_mileageWndActive == 1 ? 2 : 0);
  return 1;
}

// OFFSET: 0x006BF280
// FUNCTION: fn_06291
int C_Logger(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "Logger arg1 Error");
    return 2;
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FromUtf8(lua_tostring(state, -1), g_textCodePage).c_str()), 0xFFFF, 0, 0, 0);
  return 0;
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentPropertyDefinition) == 0x80);
static_assert(sizeof(EquipmentPropertyGroup) == 0x20);
static_assert(sizeof(EquipmentPropertyStore) == 0x28);
static_assert(sizeof(EquipmentProperties::CEquipmentPropertiesMgr) == 0x1FC);
#endif

// OFFSET: 0x00DDFFE0
// FUNCTION: fn_23451
std::int32_t CSession::MemberColor(int index) {
  switch (index) {
  case 0:
    return -16108936;
  case 1:
    return -16746376;
  case 2:
    return -16777216;
  case 4:
    return -16746496;
  case 5:
    return -8882176;
  case 6:
    return -8912776;
  case 7:
    return -8900096;
  case 8:
    return -8882056;
  case 9:
    return -16777096;
  case 10:
    return -16764366;
  case 11:
    return -13500416;
  case 12:
    return -16764416;
  case 13:
    return -13487616;
  case 14:
    return -13500366;
  case 3:
  case 15:
    return -8912896;
  default:
    return -13487566;
  }
}

// OFFSET: 0x00DD6F10
// FUNCTION: fn_23281
void CSession::AddFriend(MessengerMember& member) {
  int index = 0;

  for (std::list<MessengerMember>::iterator it = this->m_friends.begin(); it != this->m_friends.end(); ++it) {
    if (it->name == member.name) {
      member.color = MemberColor(index);
      *it = member;
    }

    ++index;
  }

  member.color = MemberColor(index);
  this->m_friends.push_back(member);
}

// OFFSET: 0x00DD70B0
// FUNCTION: fn_23283
void CSession::AddPartyMember(MessengerMember& member) {
  if (member.map.size() > 4) {
    member.map.resize(member.map.size() - 3);
    member.map.append("rsw", 3);
  }

  int index = 0;

  for (std::list<MessengerMember>::iterator it = this->m_partyMembers.begin(); it != this->m_partyMembers.end(); ++it) {
    if (it->name == member.name) {
      member.color = MemberColor(index);
      *it = member;
      return;
    }

    ++index;
  }

  member.color = MemberColor(index);
  this->m_partyMembers.push_back(member);

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode && (mode->scene->isPKZone != 0 || mode->scene->isEventPVPMode != 0)) {
    EffectActor* actor = static_cast<EffectActor*>(mode->scene->GetGameActorByAID(member.accountId));
    if (actor && !actor->HasId(EF_PARTY)) {
      actor->LaunchEffect(EF_PARTY, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
  }
}

// OFFSET: 0x00DD87E0
// FUNCTION: fn_23312
void CSession::ClearFriends() {
  this->m_friends.erase(this->m_friends.begin(), this->m_friends.end());
}

// OFFSET: 0x00DDF8A0
// FUNCTION: fn_23427
int CSession::FriendCount() const {
  return static_cast<int>(this->m_friends.size());
}

// OFFSET: 0x00DDF9D0
// FUNCTION: fn_23436
int CSession::PartyCount() const {
  return static_cast<int>(this->m_partyMembers.size());
}

// OFFSET: 0x00DDFA20
// FUNCTION: fn_23441
int CSession::OnlinePartyCount() const {
  int count = 0;

  for (const MessengerMember& member : this->m_partyMembers) {
    if (member.offline == 0) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00DDCC30
// FUNCTION: fn_23372
MessengerMember CSession::FriendAt(int index) const {
  auto member = this->m_friends.begin();
  int position = 0;

  for (; member != this->m_friends.end(); ++member) {
    if (position == index) {
      return *member;
    }

    ++position;
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE0500
// FUNCTION: fn_23455
MessengerMember CSession::PartyAt(int index) const {
  auto member = this->m_partyMembers.begin();
  int position = 0;

  for (; member != this->m_partyMembers.end(); ++member) {
    if (position == index) {
      return *member;
    }

    ++position;
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE00D0
// FUNCTION: fn_23452
MessengerMember CSession::PartyByAccount(std::uint32_t accountId) const {
  for (auto member = this->m_partyMembers.begin(); member != this->m_partyMembers.end(); ++member) {
    if (member->accountId == accountId) {
      return *member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE01C0
// FUNCTION: fn_23453
MessengerMember CSession::PartyByCharacter(std::uint32_t characterId) const {
  std::list<MessengerMember>::const_iterator it = std::find_if(this->m_partyMembers.begin(), this->m_partyMembers.end(),
                                                               [characterId](const MessengerMember& member) { return member.characterId == characterId; });
  return it != this->m_partyMembers.end() ? *it : MessengerMember(0);
}

// OFFSET: 0x00DE03E0
// FUNCTION: fn_23454
MessengerMember CSession::PartyByName(const char* name) const {
  for (auto member = this->m_partyMembers.begin(); member != this->m_partyMembers.end(); ++member) {
    if (member->name == name) {
      return *member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DD8000
// FUNCTION: fn_23301
void CSession::SetPartyDead(std::uint32_t accountId, int dead) {
  for (MessengerMember& member : this->m_partyMembers) {
    if (member.accountId == accountId) {
      member.dead = dead;
      return;
    }
  }
}

// OFFSET: 0x00DD8030
// FUNCTION: fn_23302
void CSession::SetPartyJobLevel(std::uint32_t accountId, std::uint16_t job, std::uint16_t level) {
  for (MessengerMember& member : this->m_partyMembers) {
    if (member.accountId == accountId) {
      member.job = job;
      member.level = level;
      return;
    }
  }
}

// OFFSET: 0x00DD8070
// FUNCTION: fn_23303
void CSession::SetPartyLeader(std::uint32_t accountId, int leader) {
  for (MessengerMember& member : this->m_partyMembers) {
    if (member.accountId == accountId) {
      member.leader = leader;
      return;
    }
  }
}

struct ItemNotificationWindow : UIWindow {
  /* 007C */ unsigned char padding_007C[0x38];
  /* 00B4 */ MessengerMember member;
};

// OFFSET: 0x00AAAF10
// FUNCTION: fn_17276
__declspec(noinline) void UIWindowMgr::NotifyItemWindows(int accountId, int lower, int upper) {
  for (UIWindow* window : this->snapExcluded) {
    bool matches;
    {
      MessengerMember member = static_cast<ItemNotificationWindow*>(window)->member;
      matches = member.accountId == accountId;
    }
    if (matches) {
      window->SendMsg(nullptr, UM_COMMAND, 34, accountId, lower, upper);
      break;
    }
  }
}

// OFFSET: 0x00CD3780
// FUNCTION: fn_21271
bool CGameMode::GetClosestValidCell(int* x, int* y) {
  CPlayer* player = this->scene->player;
  int column;
  int row;
  int subColumn;
  int subRow;
  this->WorldSubcell(player->render.position.x, player->render.position.z, column, row, subColumn, subRow);
  const int startY = *y;
  const int startX = *x;
  const int deltaY = row - startY;
  const int deltaX = column - startX;
  const int offsets[9][2] = {{0, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
  if (std::abs(deltaX) >= std::abs(deltaY)) {
    if (deltaX == 0) {
      return false;
    }
    const int step = deltaX > 0 ? 1 : -1;
    for (int cellX = startX; cellX != column; cellX += step) {
      const int cellY = (cellX - *x) * deltaY / deltaX + *y;
      for (int i = 0; i < 9; ++i) {
        const int candidateX = cellX + offsets[i][0];
        const int candidateY = cellY + offsets[i][1];
        if (this->scene->terrain->Walkable(candidateX, candidateY) && !this->IsRegisterCell(candidateX, candidateY)) {
          if (!this->CanFindPath(column, row, subColumn, subRow, candidateX, candidateY)) {
            return false;
          }
          *x = candidateX;
          *y = candidateY;
          return true;
        }
      }
    }
  } else {
    if (deltaY == 0) {
      return false;
    }
    const int step = deltaY > 0 ? 1 : -1;
    for (int cellY = startY; cellY != row; cellY += step) {
      const int cellX = (cellY - *y) * deltaX / deltaY + *x;
      for (int i = 0; i < 9; ++i) {
        const int candidateX = cellX + offsets[i][0];
        const int candidateY = cellY + offsets[i][1];
        if (this->scene->terrain->Walkable(candidateX, candidateY) && !this->IsRegisterCell(candidateX, candidateY)) {
          if (!this->CanFindPath(column, row, subColumn, subRow, candidateX, candidateY)) {
            return false;
          }
          *x = candidateX;
          *y = candidateY;
          return true;
        }
      }
    }
  }
  return false;
}

// OFFSET: 0x00CD6E40
// FUNCTION: fn_21290
__declspec(noinline) void CGameMode::NotifyItemBounds() {
  if (this->itemWindow) {
    this->itemWindow->Invalidate();
  }
  if (g_windowMgr.messengerGroupWnd) {
    g_windowMgr.messengerGroupWnd->SendMsg(nullptr, UM_COMMAND, 34, g_session.m_aid, 0, 0);
  }
  g_windowMgr.NotifyItemWindows(g_session.m_aid, g_session.hp, g_session.maxHp);
}

// OFFSET: 0x0069C980
// FUNCTION: fn_05924
int CLuaInstance::GetFieldInt(int& value, const char* name) {
  if (lua_type(this->m_state, -1) != LUA_TTABLE) {
    return 3;
  }

  lua_getfield(this->m_state, -1, name);
  const int type = lua_type(this->m_state, -1);
  int result = 0;
  if (type == LUA_TNUMBER) {
    value = static_cast<int>(lua_tonumber(this->m_state, -1));
  } else {
    result = type == LUA_TNIL ? 1 : 2;
  }
  lua_settop(this->m_state, -2);
  return result;
}

// OFFSET: 0x0069CA00
// FUNCTION: fn_05925
int CLuaInstance::GetFieldUIntArray(std::vector<unsigned int>& values, const char* name) {
  if (lua_type(this->m_state, -1) != LUA_TTABLE) {
    return 3;
  }

  lua_getfield(this->m_state, -1, name);
  int type = lua_type(this->m_state, -1);
  int result;
  if (type == LUA_TTABLE) {
    lua_pushnil(this->m_state);
    result = 0;
    while (lua_next(this->m_state, -2) != 0) {
      const int valueType = lua_type(this->m_state, -1);
      result = valueType == LUA_TNUMBER ? 0 : 2;
      if (valueType != LUA_TNUMBER) {
        break;
      }
      values.push_back(static_cast<unsigned int>(lua_tonumber(this->m_state, -1)));
      lua_settop(this->m_state, -2);
    }
  } else {
    result = type == LUA_TNIL ? 1 : 2;
  }
  lua_settop(this->m_state, -2);
  return result;
}

// OFFSET: 0x0069CB10
// FUNCTION: fn_05926
int CLuaInstance::GetFieldString(std::string& value, const char* name) {
  if (lua_type(this->m_state, -1) != LUA_TTABLE) {
    return 3;
  }

  lua_getfield(this->m_state, -1, name);
  const int type = lua_type(this->m_state, -1);
  int result = 0;
  if (type == LUA_TSTRING) {
    value = lua_tostring(this->m_state, -1);
  } else {
    result = type == LUA_TNIL ? 1 : 2;
  }
  lua_settop(this->m_state, -2);
  return result;
}

// OFFSET: 0x0069CBA0
// FUNCTION: fn_05927
int CLuaInstance::GetFieldStringArray(std::vector<std::string>& values, const char* name) {
  if (lua_type(this->m_state, -1) != LUA_TTABLE) {
    return 3;
  }

  lua_getfield(this->m_state, -1, name);
  int type = lua_type(this->m_state, -1);
  int result = 0;
  if (type == LUA_TTABLE) {
    lua_pushnil(this->m_state);
    while (lua_next(this->m_state, -2) != 0) {
      if (lua_type(this->m_state, -1) != LUA_TSTRING) {
        result = 2;
        break;
      }
      values.push_back(std::string(lua_tostring(this->m_state, -1)));
      lua_settop(this->m_state, -2);
    }
  } else {
    result = (type != LUA_TNIL) + 1;
  }
  lua_settop(this->m_state, -2);
  return result;
}

// OFFSET: 0x0069CE90
// FUNCTION: fn_05928
int CLuaInstance::GetFieldUIntMap(std::map<unsigned int, unsigned int>& values, const char* name) {
  lua_pushstring(this->m_state, name);
  lua_gettable(this->m_state, -2);
  int type = lua_type(this->m_state, -1);
  int result = 0;
  if (type == LUA_TTABLE) {
    lua_pushnil(this->m_state);
    while (lua_next(this->m_state, -2) != 0) {
      type = lua_type(this->m_state, -2);
      if (type != LUA_TNUMBER) {
        result = (type != LUA_TNIL) + 1;
        break;
      }
      const unsigned int key = static_cast<unsigned int>(lua_tonumber(this->m_state, -2));
      type = lua_type(this->m_state, -1);
      if (type != LUA_TNUMBER) {
        result = (type != LUA_TNIL) + 1;
        break;
      }
      values[key] = static_cast<unsigned int>(lua_tonumber(this->m_state, -1));
      lua_settop(this->m_state, -2);
    }
  } else {
    result = (type != LUA_TNIL) + 1;
  }
  lua_settop(this->m_state, -2);
  return result;
}

// OFFSET: 0x0069D0A0
// FUNCTION: fn_05929
int CLuaInstance::GetFieldStringMap(std::map<unsigned int, std::string>& values, const char* name) {
  lua_pushstring(this->m_state, name);
  lua_gettable(this->m_state, -2);
  int type = lua_type(this->m_state, -1);
  int result = 0;
  if (type == LUA_TTABLE) {
    lua_pushnil(this->m_state);
    while (lua_next(this->m_state, -2) != 0) {
      type = lua_type(this->m_state, -2);
      if (type != LUA_TNUMBER) {
        result = (type != LUA_TNIL) + 1;
        break;
      }
      const unsigned int key = static_cast<unsigned int>(lua_tonumber(this->m_state, -2));
      std::string value;
      type = lua_type(this->m_state, -1);
      if (type != LUA_TSTRING) {
        result = (type != LUA_TNIL) + 1;
        break;
      }
      value = lua_tostring(this->m_state, -1);
      values[key] = value;
      lua_settop(this->m_state, -2);
    }
  } else {
    result = (type != LUA_TNIL) + 1;
  }
  lua_settop(this->m_state, -2);
  return result;
}

// OFFSET: 0x0069D380
// FUNCTION: fn_05930
int CLuaInstance::GetInt(int& value, int index) {
  const int type = lua_type(this->m_state, index);
  if (type == LUA_TNUMBER) {
    value = static_cast<int>(lua_tonumber(this->m_state, index));
    return 0;
  }
  return (type != LUA_TNIL) + 1;
}

// OFFSET: 0x00CD7180
// FUNCTION: fn_21291
bool CGameMode::IsAllowedPair(int kind, int value) {
  bool result = false;
  switch (kind) {
  case 8:
    if (value == 6 || value == 9) {
      result = true;
    }
    break;
  case 0x16:
    if (g_windowMgr.dragAllItems != 0) {
      break;
    }
  case 0xA0:
    if (value == 4 || value == 5) {
      result = true;
    }
    break;
  case 0x18:
    if (g_windowMgr.dragAllItems != 0) {
      break;
    }
  case 0x17:
  case 0xA1:
    result = value == 3;
    break;
  case 0x21:
    if (value == 9 || value == 1) {
      result = true;
    }
    break;
  case 0x28:
    if (value == 1 || value == 6) {
      result = true;
    }
    break;
  case 0x29:
    result = value == 0xB;
    break;
  case 0x2A:
    result = value == 0xA;
    break;
  case 0x2B:
    result = value == 0xD;
    break;
  case 0x2C:
  case 0xB2:
    if (value == 0xC || value == 0x16) {
      result = true;
    }
    break;
  case 0xAF:
    result = value == 0x15;
    break;
  case 0xB8:
  case 0x13E:
    if (value == 6 || value == 4) {
      result = true;
    }
    break;
  case 0x1F:
  case 0x271B:
    if (value == 1) {
      result = true;
    }
    break;
  }
  return result;
}

// OFFSET: 0x00CD73B0
// FUNCTION: fn_21292
bool CGameMode::IsWithinRange(const int& x1, const int& y1, const int& x2, const int& y2, int range) {
  return std::abs(x1 - x2) <= range && std::abs(y1 - y2) <= range;
}

// OFFSET: 0x00CD74B0
// FUNCTION: fn_21293
bool CGameMode::IsRegisterCell(int x, int y) {
  for (auto it = this->actorPosList.begin(); it != this->actorPosList.end(); ++it) {
    if (it->second.first == x && it->second.second == y) {
      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(it->first));
      if (actor == nullptr) {
        return true;
      }
      if (!StateFlag01(actor->GetStateFlags())) {
        return true;
      }
    }
  }
  return false;
}

// OFFSET: 0x00CD7560
// FUNCTION: fn_21294
bool CGameMode::HasPendingItemPickup(int value) {
  for (int entry : this->pickupReqItemNaidList) {
    if (entry == value) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00CD75B0
// FUNCTION: fn_21295
bool CGameMode::IsValidCell(int x, int y) {
  if (!this->scene->terrain->Walkable(x, y)) {
    return false;
  }
  CPlayer* player = this->scene->player;
  if (x == player->destCellX && y == player->destCellZ) {
    return false;
  }
  int column;
  int row;
  int subColumn;
  int subRow;
  this->WorldSubcell(player->render.position.x, player->render.position.z, column, row, subColumn, subRow);
  return this->CanFindPath(column, row, subColumn, subRow, x, y) && !this->IsRegisterCell(x, y);
}

// OFFSET: 0x00DD80A0
// FUNCTION: fn_23304
__declspec(noinline) void CSession::UpdateRandomStats() {
  if (g_resetMessengerRandomStats != 0) {
    g_resetMessengerRandomStats = 0;
    this->randomStatIndexC = std::ranges::max(std::rand() % 12, 0);
    this->randomStatIndexB = std::ranges::max(std::rand() % 12, 0);
    this->randomStatIndexE = std::ranges::max(std::rand() % 12, 0);
    this->randomStatIndexA = std::ranges::max(std::rand() % 12, 0);
    this->randomStatIndexD = std::ranges::max(std::rand() % 12, 0);
  }

  const int bound70 = this->m_sp;
  const int bound74 = this->m_maxSp;
  const int bound68 = this->hp;
  const int bound6C = this->maxHp;
  this->randomStatIndexC = std::ranges::max(std::rand() % 12, 0);
  this->randomStatIndexA = std::ranges::max(std::rand() % 12, 0);
  this->randomStatIndexD = std::ranges::max(std::rand() % 12, 0);
  this->randomStatIndexB = std::ranges::max(std::rand() % 12, 0);
  this->randomStatIndexE = std::ranges::max(std::rand() % 12, 0);
  for (int& value : this->randomStatValues) {
    value = std::max(std::abs(std::rand()) % 1, 0);
  }

  if (this->hp != bound68) {
    this->hp = bound68;
    this->RefreshHpSpWindows();
  }
  if (this->maxHp != bound6C) {
    this->maxHp = bound6C;
    this->RefreshHpSpWindows();
  }
  if (this->m_sp != bound70) {
    this->m_sp = bound70;
    this->RefreshHpSpWindows();
  }
  if (this->m_maxSp != bound74) {
    this->m_maxSp = bound74;
    this->RefreshHpSpWindows();
  }
}

// OFFSET: 0x00DD85A0
// FUNCTION: fn_23308
void ClearMemberFlags(std::uint32_t* flags) {
  *flags &= ~0x40u;
}

// OFFSET: 0x00DD85B0
// FUNCTION: fn_23309
void CSession::ResetEquipment() {
  for (int index = 0; index < 10; ++index) {
    this->equippedItems[index].Reset();
  }
  RequestBasicInfoRefresh();
  EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->Clear();
  g_windowMgr.InvalidateFrames();
}

// OFFSET: 0x00DD8750
// FUNCTION: fn_23311
void CSession::ResetSlots() {
  this->exchangeMoney = 0;
  this->m_exchangeItems[0].quantity = 0;
  this->m_exchangeItems[1].quantity = 0;
  this->m_exchangeItems[2].quantity = 0;
  this->m_exchangeItems[3].quantity = 0;
  this->m_exchangeItems[4].quantity = 0;
  this->m_exchangeItems[5].quantity = 0;
  this->m_exchangeItems[6].quantity = 0;
  this->m_exchangeItems[7].quantity = 0;
  this->m_exchangeItems[8].quantity = 0;
  this->m_exchangeItems[9].quantity = 0;
}

// OFFSET: 0x00DDF8B0
// FUNCTION: fn_23428
int CSession::GetList0Size() const {
  return static_cast<int>(this->m_inventoryItems.size());
}

// OFFSET: 0x00DDF8C0
// FUNCTION: fn_23429
int CSession::GetList11Size() const {
  return static_cast<int>(this->merchantPurchaseSavedItems.size());
}

// OFFSET: 0x00DDF8D0
// FUNCTION: fn_23430
int CSession::GetList6Size() const {
  return static_cast<int>(this->merchantShopItems.size());
}

// OFFSET: 0x00DDF8E0
// FUNCTION: fn_23431
int CSession::GetList8Size() const {
  return static_cast<int>(this->merchantMyShopItems.size());
}

// OFFSET: 0x00DDF8F0
// FUNCTION: fn_23432
int CSession::GetList9Size() const {
  return static_cast<int>(this->merchantPurchaseItems.size());
}

// OFFSET: 0x00DDF900
// FUNCTION: fn_23433
int CSession::GetList10Size() const {
  return static_cast<int>(this->merchantHopeToSellItems.size());
}

// OFFSET: 0x00DDF910
// FUNCTION: fn_23434
int CSession::GetList7Size() const {
  return static_cast<int>(this->merchantMirrorItems.size());
}

// OFFSET: 0x00DDF920
// FUNCTION: fn_23435
int CSession::GetInventoryCategorySize(int index) const {
  if (index < 0) {
    return 0;
  }
  if (index >= std::size(this->inventoryCategories)) {
    return 0;
  }
  return static_cast<int>(this->inventoryCategories[index].size());
}

// OFFSET: 0x00DDF9E0
// FUNCTION: fn_23437
int CSession::GetList2Size() const {
  return static_cast<int>(this->purchaseItems.size());
}

// OFFSET: 0x00DDF9F0
// FUNCTION: fn_23438
int CSession::GetList4Size() const {
  return static_cast<int>(this->storageItems.size());
}

// OFFSET: 0x00DDFA00
// FUNCTION: fn_23439
int CSession::GetList1Size() const {
  return static_cast<int>(this->shopItems.size());
}

// OFFSET: 0x00DDFA10
// FUNCTION: fn_23440
int CSession::GetList5Size() const {
  return static_cast<int>(this->cartItems.size());
}

// OFFSET: 0x00DD8380
// FUNCTION: fn_23305
void CSession::ClearAllInventoryCategories() {
  for (int index = 0; index < 11; ++index) {
    this->inventoryCategories[index].erase(this->inventoryCategories[index].begin(), this->inventoryCategories[index].end());
  }
}

// OFFSET: 0x00DD8450
// FUNCTION: fn_23306
void CSession::KeepEquipment() {
  for (auto entry = this->m_inventoryItems.begin(); entry != this->m_inventoryItems.end();) {
    switch (entry->identity[0]) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      ++entry;
      break;
    default:
      entry = this->m_inventoryItems.erase(entry);
      break;
    }
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00DD8540
// FUNCTION: fn_23307
void CSession::ClearTradeItems() {
  this->purchaseItems.erase(this->purchaseItems.begin(), this->purchaseItems.end());
  this->storageItems.erase(this->storageItems.begin(), this->storageItems.end());
  this->shopItems.erase(this->shopItems.begin(), this->shopItems.end());
}

// OFFSET: 0x00DD8660
// FUNCTION: fn_23310
void CSession::RemoveEquipment() {
  for (auto entry = this->m_inventoryItems.begin(); entry != this->m_inventoryItems.end();) {
    switch (entry->identity[0]) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      entry = this->m_inventoryItems.erase(entry);
      break;
    default:
      ++entry;
      break;
    }
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00698230
// FUNCTION: fn_05766
EquipmentProperties::CEquipmentProperties* EquipmentProperties::CEquipmentProperties1Dim::Accumulate(CEquipmentProperties* other) {
  if (this->m_definition == nullptr) {
    return this;
  }

  auto* source = dynamic_cast<CEquipmentProperties1Dim*>(other);
  if (source == nullptr) {
    return this;
  }

  if (this->m_definition->uniqueKeyMode != 0) {
    if (source->m_keys.size() != 1) {
      return this;
    }
    const unsigned int& key = *source->m_keys.begin();
    if (this->m_keys.contains(key)) {
      return this;
    }
    this->m_keys.insert(key);
  }

  switch (this->m_definition->mode) {
  case 0:
    this->m_value += source->m_value;
    break;
  case 1:
    ++this->m_value;
    break;
  case 2:
    this->m_value = std::max(this->m_value, source->m_value);
    break;
  case 4:
    this->m_value = source->m_value < 0 ? 0 : std::max(this->m_value, source->m_value);
    break;
  }
  return this;
}

struct MessengerLinkNode {
  /* 0000 */ MessengerLinkNode* m_next;
  /* 0004 */ MessengerLinkNode* m_prev;
};

struct MessengerIntrusiveLink {
  /* 0000 */ unsigned int padding_0000;
  /* 0004 */ MessengerLinkNode* m_prev;
  /* 0008 */ MessengerLinkNode* m_next;

  void Unlink68B6A0();
  void Unlink6A6100();
  void Unlink6A6120();
  void Unlink6A6140();
  void Unlink6A6160();
  void Unlink6A6180();
  void Unlink6A61A0();
  void Unlink6A61C0();
  void Unlink6A61E0();
  void Unlink6A6200();
  void Unlink6A6220();
  void Unlink6A6240();
};

// OFFSET: 0x0068B6A0
// FUNCTION: fn_05611
void MessengerIntrusiveLink::Unlink68B6A0() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6100
// FUNCTION: fn_06045
void MessengerIntrusiveLink::Unlink6A6100() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6120
// FUNCTION: fn_06046
void MessengerIntrusiveLink::Unlink6A6120() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6140
// FUNCTION: fn_06047
void MessengerIntrusiveLink::Unlink6A6140() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6160
// FUNCTION: fn_06048
void MessengerIntrusiveLink::Unlink6A6160() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6180
// FUNCTION: fn_06049
void MessengerIntrusiveLink::Unlink6A6180() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A61A0
// FUNCTION: fn_06050
void MessengerIntrusiveLink::Unlink6A61A0() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A61C0
// FUNCTION: fn_06051
void MessengerIntrusiveLink::Unlink6A61C0() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A61E0
// FUNCTION: fn_06052
void MessengerIntrusiveLink::Unlink6A61E0() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6200
// FUNCTION: fn_06053
void MessengerIntrusiveLink::Unlink6A6200() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6220
// FUNCTION: fn_06054
void MessengerIntrusiveLink::Unlink6A6220() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

// OFFSET: 0x006A6240
// FUNCTION: fn_06055
void MessengerIntrusiveLink::Unlink6A6240() {
  this->m_prev->m_next = this->m_next;
  this->m_next->m_prev = this->m_prev;
}

struct ClientMessengerSelf {
  ClientMessengerSelf* Self68D5B0();
  ClientMessengerSelf* Self68D5C0();
  ClientMessengerSelf* Self69ACB0();
  ClientMessengerSelf* Self69ACC0();
};

// OFFSET: 0x0068D5B0
// FUNCTION: fn_05647
ClientMessengerSelf* ClientMessengerSelf::Self68D5B0() {
  return this;
}

// OFFSET: 0x0068D5C0
// FUNCTION: fn_05648
ClientMessengerSelf* ClientMessengerSelf::Self68D5C0() {
  return this;
}

// OFFSET: 0x0069ACB0
// FUNCTION: fn_05786
ClientMessengerSelf* ClientMessengerSelf::Self69ACB0() {
  return this;
}

// OFFSET: 0x0069ACC0
// FUNCTION: fn_05787
ClientMessengerSelf* ClientMessengerSelf::Self69ACC0() {
  return this;
}

// OFFSET: 0x0068DAF0
// FUNCTION: fn_05656
void sub_68DAF0() {
}

// OFFSET: 0x006A2170
// FUNCTION: fn_06015
void sub_6A2170() {
}

// OFFSET: 0x00A91730
// FUNCTION: fn_17164
UIWindow* UIWindowMgr::FindItemWindow(int accountId) {
  for (UIWindow* window : this->snapExcluded) {
    bool matches;
    {
      MessengerMember member = static_cast<ItemNotificationWindow*>(window)->member;
      matches = member.accountId == accountId;
    }
    if (matches) {
      return window;
    }
  }
  return nullptr;
}

// OFFSET: 0x00AA64D0
// FUNCTION: fn_17230
void UIWindowMgr::NotifyMemberWindows(const MessengerMember* target) {
  if (this->messengerGroupWnd) {
    this->messengerGroupWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  for (UIWindow* window : this->snapExcluded) {
    bool matches;
    {
      MessengerMember member = static_cast<ItemNotificationWindow*>(window)->member;
      matches = target->accountId == member.accountId;
    }
    if (matches) {
      if (window) {
        window->SendMsg(nullptr, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(target), 0, 0, 0);
      }
      break;
    }
  }
}

// OFFSET: 0x006BD660
// FUNCTION: fn_06278
int GetEquipGrade(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  int grade = 0;
  if (args[0] != 0) {
    const int mask = GetEquipLocationMask(args[0]);
    const EquipSlots slots = g_session.GetEquipSlots(mask);
    const ITEM_INFO item = g_session.IsCostumeLocation(mask) ? g_session.GetCostumeItemAt(slots.values[2]) : g_session.GetEquippedItemAt(slots.values[2]);
    if (item.identity[1] > 0) {
      grade = item.grade;
    }
  }

  lua_pushnumber(state, grade);
  return 1;
}

// OFFSET: 0x006BDF80
// FUNCTION: fn_06281
int GetEquipItemId(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  const int mask = GetEquipLocationMask(args[0]);
  const EquipSlots slots = g_session.GetEquipSlots(mask);
  const ITEM_INFO item = g_session.IsCostumeLocation(mask) ? g_session.GetCostumeItemAt(slots.values[2]) : g_session.GetEquippedItemAt(slots.values[2]);
  int itemId = 0;
  if (item.identity[1] > 0) {
    itemId = std::atoi(item.itemId.c_str());
  }

  lua_pushnumber(state, itemId);
  return 1;
}

// OFFSET: 0x006BE6F0
// FUNCTION: fn_06286
int GetEquipRefine(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  const int mask = GetEquipLocationMask(args[0]);
  const EquipSlots slots = g_session.GetEquipSlots(mask);
  const ITEM_INFO item = g_session.IsCostumeLocation(mask) ? g_session.GetCostumeItemAt(slots.values[2]) : g_session.GetEquippedItemAt(slots.values[2]);
  int refine = 0;
  if (item.identity[1] > 0) {
    refine = item.refiningLevel;
  }

  lua_pushnumber(state, refine);
  return 1;
}

#pragma pack(push, 1)
struct ShopItemPacketEntry {
  /* 0000 */ std::uint8_t index;
  /* 0001 */ int itemId;
  /* 0005 */ int first;
  /* 0009 */ int second;
};

struct ShopItemListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ std::uint8_t flag;
  /* 0005 */ ShopItemPacketEntry entries[1];
};
#pragma pack(pop)

static_assert(sizeof(ShopItemPacketEntry) == 13);
static_assert(offsetof(ShopItemListPacket, entries) == 5);

// OFFSET: 0x00D75840
// FUNCTION: fn_22148
void CGameMode::ReceiveShopItemList(const unsigned char* data) {
  const ShopItemListPacket* packet = reinterpret_cast<const ShopItemListPacket*>(data);
  const int count = (packet->length - 5) / sizeof(ShopItemPacketEntry);
  const int shopId = g_session.FindShopId("");
  if (shopId != -1) {
    g_session.SetShopFlag(shopId, packet->flag != 0);
    for (int i = 0; i < count; ++i) {
      const ShopItemPacketEntry& entry = packet->entries[i];
      g_session.SetShopItem(shopId, entry.index, entry.itemId, entry.first, entry.second);
    }
    if (g_equipmentPropertiesMgr == nullptr) {
      g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
    }
    g_equipmentPropertiesMgr->RefreshShop();
  }
}

// OFFSET: 0x006AC320
// FUNCTION: fn_06173
int EquipmentProperties::CEquipmentPropertiesMgr::GetGradeBonus(const int& first, const int& second, const int& third) {
  auto outer = this->categoryValues.find(first);
  if (this->categoryValues.end() != outer) {
    auto middle = outer->second.find(second);
    if (outer->second.end() != middle) {
      auto inner = middle->second.find(third);
      if (middle->second.end() != inner) {
        return inner->second.value;
      }
    }
  }
  return 0;
}

// OFFSET: 0x006BD9D0
// FUNCTION: fn_06279
int GetEquipTempValue(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -1; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  std::unordered_map<int, EquipmentValueC> values = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->GetDisplayValues();
  auto found = values.find(args[0]);
  lua_pushnumber(state, found != values.end() ? found->second.value : 0);
  return 1;
}

// OFFSET: 0x006BF3F0
// FUNCTION: fn_06292
int SetEquipTempValue(lua_State* state) {
  std::vector<int> args;
  int number = 0;
  for (int index = -2; index < 0; ++index) {
    if (!lua_isnumber(state, index)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, std::format("arg{0} is must number", number + 1).c_str());
      return 2;
    }
    args.push_back(static_cast<int>(lua_tonumber(state, index)));
    ++number;
  }

  EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->GetDisplayValues()[args[0]].value = args[1];
  return 0;
}

#pragma pack(push, 1)
struct MercenaryInitPacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint32_t aid;
  /* 0006 */ std::int16_t stats[8];
  /* 0016 */ char name[24];
  /* 002E */ std::int16_t level;
  /* 0030 */ int hp;
  /* 0034 */ int maxHp;
  /* 0038 */ int sp;
  /* 003C */ int maxSp;
  /* 0040 */ __time32_t expiration;
  /* 0044 */ std::int16_t loyalty;
  /* 0046 */ int summons;
  /* 004A */ int kills;
  /* 004E */ std::int16_t attackRange;
};
#pragma pack(pop)

static_assert(offsetof(MercenaryInitPacket, name) == 0x16);
static_assert(offsetof(MercenaryInitPacket, attackRange) == 0x4E);

// OFFSET: 0x00D322A0
// FUNCTION: fn_21741
void CGameMode::Zc_Mer_Init(const unsigned char* data) {
  const MercenaryInitPacket* packet = reinterpret_cast<const MercenaryInitPacket*>(data);
  g_session.m_mercSkills.clear();
  CRenderObject* actor = this->scene->GetGameActorByAID(packet->aid);
  g_session.m_mercenaryAid = packet->aid;
  g_session.mercenaryAlive = 1;
  if (actor != nullptr) {
    g_session.m_mercenaryClass = static_cast<CGameActor*>(actor)->GetEffectiveJob(-1);
  }
  sprintf(g_session.m_mercenaryName, packet->name);
  g_session.m_mercenaryStats[0] = packet->stats[0];
  g_session.m_mercenaryStats[1] = packet->stats[1];
  g_session.m_mercenaryStats[2] = packet->stats[2];
  g_session.m_mercenaryStats[3] = packet->stats[3];
  g_session.m_mercenaryStats[4] = packet->stats[4];
  g_session.m_mercenaryStats[5] = packet->stats[5];
  g_session.m_mercenaryStats[6] = packet->stats[6];
  g_session.m_mercenaryStats[7] = packet->stats[7];
  g_session.m_mercenaryLevel = packet->level;
  g_session.m_mercenaryHp = packet->hp;
  g_session.RefreshMercenaryInfo();
  g_session.m_mercenarySp = packet->sp;
  g_session.RefreshMercenaryInfo();
  g_session.m_mercenaryMaxHp = packet->maxHp;
  g_session.RefreshMercenaryInfo();
  g_session.m_mercenaryMaxSp = packet->maxSp;
  g_session.RefreshMercenaryInfo();
  g_session.m_mercenaryLoyalty = packet->loyalty;
  g_session.m_mercenarySummons = packet->summons;
  g_session.m_mercenaryKills = packet->kills;
  g_session.m_mercenaryAttackRange = packet->attackRange;
  g_session.m_mercenaryExpiration = packet->expiration;
}

#pragma pack(push, 1)
struct MercenaryParChangePacket {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ int value;
};
#pragma pack(pop)

// OFFSET: 0x00D323D0
// FUNCTION: fn_21742
void CGameMode::Zc_Mer_Par_Change(const unsigned char* data) {
  if (g_session.mercenaryAlive == 0) {
    return;
  }

  const MercenaryParChangePacket* packet = reinterpret_cast<const MercenaryParChangePacket*>(data);
  switch (packet->type) {
  case SP_HP:
    g_session.m_mercenaryHp = packet->value;
    g_session.RefreshMercenaryInfo();
    break;
  case SP_SP:
    g_session.m_mercenarySp = packet->value;
    g_session.RefreshMercenaryInfo();
    break;
  case SP_MAXHP:
    g_session.m_mercenaryMaxHp = packet->value;
    g_session.RefreshMercenaryInfo();
    break;
  case SP_MAXSP:
    g_session.m_mercenaryMaxSp = packet->value;
    g_session.RefreshMercenaryInfo();
    break;
  case SP_ATK1:
    g_session.m_mercenaryStats[0] = packet->value;
    break;
  case SP_HIT:
    g_session.m_mercenaryStats[2] = packet->value;
    break;
  case SP_MERCFLEE:
    g_session.m_mercenaryStats[6] = packet->value;
    break;
  case SP_ASPD:
    g_session.m_mercenaryStats[7] = packet->value;
    break;
  case SP_MERCKILLS:
    g_session.m_mercenaryKills = packet->value;
    break;
  case SP_MERCFAITH:
    g_session.m_mercenaryLoyalty = packet->value;
    break;
  }

  g_session.RefreshMercenaryInfo();
}

// GLOBAL: gl_25251
HANDLE g_embedFontFiles;

// OFFSET: 0x00DD9D50
// FUNCTION: fn_23337
void CSession::InitClientData() {
  if (!g_itemInfoMgr->Load()) {
    const char* caption = OBFUSCATE_TEXT("Error", 0xCD8F077123FB7D6FULL);
    const char* message = OBFUSCATE_TEXT("ItemInfo file Init", 0x23BBD58F8B5FF5C9ULL);
    MessageBoxA(nullptr, message, caption, 0);
  }

  if (!g_specialPopupMgr->Init()) {
    const char* caption = OBFUSCATE_TEXT("Error", 0x17A9E5BF39AD4737ULL);
    const char* message = OBFUSCATE_TEXT("SPECIALPOPUP file Init", 0x9949BFD527532943ULL);
    MessageBoxA(nullptr, message, caption, 0);
  }

  if (!g_runeSystemMgr->LoadScripts()) {
    const char* caption = OBFUSCATE_TEXT("Error", 0xCFF3899D95AF355FULL);
    const char* message = OBFUSCATE_TEXT("CRuneSystemMgr file Init", 0x13E33FA1A591AB67ULL);
    MessageBoxA(nullptr, message, caption, 0);
  }

  static_cast<void>(CTipBoxMgr::Create());
  if (!g_itemTagOwner->RegisterLuaFunctions()) {
    const char* caption = OBFUSCATE_TEXT("Error", 0x4F99DF03C3EF31B7ULL);
    const char* message = OBFUSCATE_TEXT("TipBox file Init", 0xE52DF30DCBAF7125ULL);
    MessageBoxA(nullptr, message, caption, 0);
  }

  static_cast<void>(CSimpleStringMgr::Create());
  static_cast<void>(CCashEmotionMgr::Initialize());
  srand(static_cast<unsigned int>(_time32(nullptr)));
  this->UpdateRandomStats();
  this->LoadSessionTables();
  static_cast<void>(g_mapInfoMgr->LoadScript());

  CGameSettingsMgr* settings = Singleton<CGameSettingsMgr>::GetInstance();
  static_cast<void>(settings->LoadSettings());
  this->LoadOptionInfo();
  this->LoadDamageSkinSettings();

  if (g_serviceType == 0) {
    InitEmbedFont();
    SetEmbedFontRootUrl("http://www.ragnarok.co.kr");
    LoadEmbedFont(&g_embedFontFiles, "RixSquirrel_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixLoveangel_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixNHCgogo_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixDiary_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixMiniHeart_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixFreshman_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixKid_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixMagic_10.eot", 0);
    LoadEmbedFont(&g_embedFontFiles, "RixJJangu_10.eot", 0);
  }
}

// OFFSET: 0x006B6F10
// FUNCTION: fn_06203
void EquipmentProperties::CEquipmentPropertiesMgr::UnequipWearable(const int& mask) {
  this->m_dirty = true;
  const int key = GetEquipLocationIndex(mask);
  auto found = this->properties1.find(key);
  if (found != this->properties1.end()) {
    for (CEquipmentAttachable& attachable : found->second->GetAttachables()) {
      this->UnequipItem(attachable.GetId(), true);
    }
    this->UnequipItem(found->second->GetId(), true);
    found->second->Clear();
    delete found->second;
    this->properties1.erase(found);
  }
  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B71F0
// FUNCTION: fn_06205
void EquipmentProperties::CEquipmentPropertiesMgr::RefreshWearable(const int& key) {
  const int index = key;
  auto found = this->properties1.find(index);
  if (found == this->properties1.end()) {
    return;
  }

  found->second->IsPretend();
  auto combinations = this->m_itemCombinations.find(found->second->GetId());
  if (combinations != this->m_itemCombinations.end()) {
    for (const unsigned int& combinationId : combinations->second) {
      if (this->properties3.find(combinationId) != this->properties3.end()) {
        this->properties3[combinationId]->IsPretend();
      }
    }
  }

  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B1000
// FUNCTION: fn_06194
void EquipmentProperties::CEquipmentPropertiesMgr::LoadEquipmentPropertiesOrder() {
  if (!this->loadValid) {
    return;
  }
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }
  const char* name = "EquipmentPropertiesOrder";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      unsigned int category;
      int result;
      int type = lua_type(this->script.GetState(), -2);
      if (type == LUA_TNUMBER) {
        category = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
        result = 0;
      } else {
        result = (type != LUA_TNIL) + 1;
      }
      this->loadValid &= result == 0;
      if (result != 0) {
        ShowErrorDialog(std::format("{0} table has invalid entry key({1})", name, result), 0);
      }
      if (this->loadValid) {
        std::string categoryName;
        result = this->script.GetFieldString(categoryName, "name");
        this->loadValid &= result == 0;
        if (result != 0) {
          ShowErrorDialog(std::format("Category[{0}] has invalid 'name' field({1})", category, result), 0);
        }
        if (this->loadValid) {
          categoryName = FromUtf8(categoryName, g_textCodePage);
          std::map<std::int64_t, EquipmentPropertyDefinition*>& definitions =
              this->propertyGroups.try_emplace(category, EquipmentPropertyGroup(categoryName.c_str())).first->second.GetDefinitions();
          lua_pushstring(state, "order");
          lua_gettable(state, -2);
          if (lua_type(state, -1) == LUA_TTABLE) {
            lua_pushnil(state);
            while (lua_next(state, -2) != 0) {
              unsigned int order;
              type = lua_type(this->script.GetState(), -2);
              if (type == LUA_TNUMBER) {
                order = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
                result = 0;
              } else {
                result = (type != LUA_TNIL) + 1;
              }
              this->loadValid &= result == 0;
              if (result != 0) {
                ShowErrorDialog(std::format("Category table has invalid entry key({0})", result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              EquipmentPropertyDefinition* definition = new EquipmentPropertyDefinition;
              definition->group = category;
              definition->id = order;
              definitions.emplace(order, definition);

              std::string orderName;
              result = this->script.GetFieldString(orderName, "name");
              this->loadValid &= result == 0;
              if (result != 0) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}] has invaild 'name' field({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              orderName = FromUtf8(orderName, g_textCodePage);
              definition->formatText = orderName;
              const bool hasSeparator = ContainsText(definition->formatText, "{sep}");
              const bool hasValue = ContainsText(definition->formatText, "{val}");
              std::string_view symbol;
              if (hasValue == true) {
                ReplaceAll(definition->formatText, "{val}", "{0}");
                ReplaceAll(definition->formatText, "{sep}", "{1}");
                symbol = hasSeparator ? "{2}" : "{1}";
              } else {
                symbol = "{0}";
              }
              ReplaceAll(definition->formatText, "{sym}", symbol);

              result = this->script.GetFieldStringArray(definition->functions, "func");
              this->loadValid &= result == 0;
              if (result != 0) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}] has invaild 'func' field({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              this->loadValid &= definition->functions.size() != 0;
              if (definition->functions.size() == 0) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s 'func' table has 0 value", categoryName, order), 0);
              }
              if (!this->loadValid) {
                break;
              }

              this->loadValid &= definition->functions.size() <= 2;
              if (definition->functions.size() > 2) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s 'func' table has more than 2 values", categoryName, order), 0);
              }
              if (!this->loadValid) {
                break;
              }

              std::map<unsigned int, unsigned int> values;
              result = this->script.GetFieldUIntMap(values, "val");
              this->loadValid &= result == 0;
              if (result != 0) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s has invalid 'val' table({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              this->loadValid &= values.size() == 1;
              if (values.size() != 1) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s 'val' table has too many key-value pair", categoryName, order), 0);
              }
              if (!this->loadValid) {
                break;
              }

              for (const auto& value : values) {
                definition->functionMode = value.first;
                definition->mode = value.second;
                if (!hasValue) {
                  this->loadValid &= value.second == 1;
                  if (definition->mode != 1) {
                    ShowErrorDialog(std::format("Category({0})'s order[{1}]'s 'val' table must have 'ONE' type", categoryName, order), 0);
                  }
                  if (!this->loadValid) {
                    break;
                  }
                }
              }

              std::map<unsigned int, unsigned int> conditions;
              result = this->script.GetFieldUIntMap(conditions, "cond");
              bool valid = result == 0 || result == 1;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s has invalid 'cond' table({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              for (const std::string& function : definition->functions) {
                std::string text = std::format("{0}", function);
                for (const auto& condition : conditions) {
                  text = std::format("{0} arg{1}:{2}", text, condition.first, condition.second);
                }
                unsigned int hash = 5381;
                for (const char* character = text.c_str(); *character != '\0'; ++character) {
                  hash = hash * 33 + *character;
                }
                const std::uint64_t nameKey = hash;
                valid = this->m_definitionsByName.find(nameKey) == this->m_definitionsByName.end();
                this->loadValid &= valid;
                if (!valid) {
                  ShowErrorDialog(std::format("string:{0} cause hash collision", text), 0);
                }
                if (!this->loadValid) {
                  break;
                }
                this->m_definitionsByName.insert({nameKey, definition});
              }

              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s 'cond' table has some error", categoryName, order), 0);
              }
              if (!this->loadValid) {
                break;
              }

              result = this->script.GetFieldStringArray(definition->signLabels, "sym");
              valid = result == 0 || result == 1;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s value table has invaild 'sym' field", categoryName, order), 0);
              }
              valid = definition->signLabels.size() == 0 || definition->signLabels.size() == 2;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s value table has invaild 'sym' field", categoryName, order), 0);
              }
              for (std::string& symbolText : definition->signLabels) {
                symbolText = FromUtf8(symbolText, g_textCodePage);
              }

              std::map<unsigned int, std::string> separators;
              result = this->script.GetFieldStringMap(separators, "sep");
              valid = result == 0 || result == 1;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}]'s has invalid 'sep' table({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              if (result == 0) {
                this->loadValid &= separators.size() == 1;
                if (separators.size() != 1) {
                  ShowErrorDialog(std::format("Category({0})'s order[{1}]'s 'sep' table has too many key-value pair", categoryName, order), 0);
                }
                if (!this->loadValid) {
                  break;
                }
                for (const auto& separator : separators) {
                  definition->separatorMode = separator.first;
                  definition->separator = separator.second;
                }
              }

              int randomOption;
              result = this->script.GetFieldInt(randomOption, "rndOpt");
              valid = result == 0 || result == 1;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}] has invaild 'rndOpt' field({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              if (result == 0) {
                valid = this->randomOptionDefinitions.find(randomOption) == this->randomOptionDefinitions.end();
                this->loadValid &= valid;
                if (!valid) {
                  ShowErrorDialog(std::format("Category({0})'s order[{1}]'s rndOpt value is already defined: {2}", category, order,
                                              static_cast<unsigned int>(randomOption)),
                                  0);
                }
                if (!this->loadValid) {
                  break;
                }
                this->randomOptionDefinitions[randomOption] = definition;
              }

              result = this->script.GetFieldString(definition->uniqueGroup, "uniq1");
              valid = result == 0 || result == 1;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}] has invaild 'uniq1' field({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              result = this->script.GetFieldInt(definition->uniqueKeyMode, "uniq2");
              valid = result == 0 || result == 1;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}] has invaild 'uniq2' field({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }

              lua_pushstring(this->script.GetState(), "valOpt");
              lua_gettable(this->script.GetState(), -2);
              type = lua_type(this->script.GetState(), -1);
              result = 0;
              if (type == LUA_TTABLE) {
                lua_pushnil(this->script.GetState());
                while (lua_next(this->script.GetState(), -2) != 0) {
                  int optionKey;
                  int optionValue;
                  result = this->script.GetInt(optionKey, -2);
                  if (result != 0) {
                    break;
                  }
                  result = this->script.GetInt(optionValue, -1);
                  if (result != 0) {
                    break;
                  }
                  definition->formatOptions[optionKey] = optionValue;
                  lua_settop(this->script.GetState(), -2);
                }
              } else {
                result = (type != LUA_TNIL) + 1;
              }
              lua_settop(this->script.GetState(), -2);
              valid = result == 0 || result == 1;
              this->loadValid &= valid;
              if (!valid) {
                ShowErrorDialog(std::format("Category({0})'s order[{1}] has invaild 'rndOpt' field({2})", categoryName, order, result), 0);
              }
              if (!this->loadValid) {
                break;
              }
              lua_settop(state, -2);
            }
          } else {
            ShowErrorDialog(std::format("Category({0})'s has invaild 'order' table", categoryName), 0);
          }
          lua_settop(state, -2);
        }
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}

#pragma pack(push, 1)
struct EquipmentItemOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};
#pragma pack(pop)

static_assert(sizeof(EquipmentItemOption) == 5);

// OFFSET: 0x0068A420
// FUNCTION: fn_05604
EquipmentProperties::CEquipmentWearable::CEquipmentWearable(const ITEM_INFO* item) : CEquipment(), m_active(true) {
  this->m_id = std::atoi(item->itemId.c_str());
  this->m_type = item->identity[3];
  this->m_info = CEquipmentPropertiesMgr::GetInstance()->FindEquipmentInfo(this->m_id);
  this->inventoryIndex = g_session.GetEquipSlots(this->m_type).values[2];

  for (int index = 0; index < 4; ++index) {
    int card;
    std::memcpy(&card, &item->cards[index], sizeof(card));
    if (card != 0) {
      const CEquipmentAttachable attachable(card, index, this);
      this->m_attachables.push_back(attachable);
    }
  }

  for (int index = 0; index < 5; ++index) {
    EquipmentItemOption option;
    std::memcpy(&option, item->options + index * sizeof(option), sizeof(option));
    const int optionId = option.id;
    EquipmentPropertyDefinition* definition = CEquipmentPropertiesMgr::GetInstance()->FindRandomOptionDefinition(optionId);
    if (definition == nullptr || definition->functions.front() == "SetWeaponAttr") {
      continue;
    }

    int multiplier = 1;
    const auto found = definition->formatOptions.find(2);
    if (found != definition->formatOptions.end()) {
      multiplier = found->second;
    }
    if (definition->IsTwoDimensional() == true) {
      CEquipmentProperties2Dim* property = new CEquipmentProperties2Dim(definition);
      const int parameter = option.parameter;
      const int value = option.value * multiplier;
      const unsigned int key = -1;
      property->SetValue(parameter, value, key);
      this->m_children.push_back(property);
    } else if (definition->mode == 1) {
      this->m_children.push_back(new CEquipmentProperties(definition));
    } else {
      CEquipmentProperties1Dim* property = new CEquipmentProperties1Dim(definition);
      const int value = option.value * multiplier;
      const unsigned int key = -1;
      property->SetValue(value, key);
      this->m_children.push_back(property);
      if (optionId == 35) {
        const int additionalId = 25;
        definition = CEquipmentPropertiesMgr::GetInstance()->FindRandomOptionDefinition(additionalId);
        if (definition != nullptr) {
          CEquipmentProperties1Dim* additional = new CEquipmentProperties1Dim(definition);
          const int negative = -option.value;
          additional->SetValue(negative, key);
          this->m_children.push_back(additional);
        }
      }
    }
  }
}

// OFFSET: 0x006B6C70
// FUNCTION: fn_06201
void EquipmentProperties::CEquipmentPropertiesMgr::EquipWearable(const ITEM_INFO* item, bool notify) {
  if (item == nullptr) {
    return;
  }

  this->m_dirty = true;
  const int key = GetEquipLocationIndex(item->identity[3]);
  const auto found = this->properties1.find(static_cast<int>(key));
  if (found != this->properties1.end()) {
    delete found->second;
    this->properties1.erase(found);
  }
  CEquipmentWearable* replacement = new CEquipmentWearable(item);
  CEquipmentWearable* wearable = this->properties1.insert({key, replacement}).first->second;
  wearable->UpdateActive();
  if (!wearable->IsActive()) {
    wearable->Load(false);
  } else {
    wearable->Load(notify);
    this->EquipItem(wearable->GetId(), notify);
    for (CEquipmentAttachable& attachable : wearable->GetAttachables()) {
      this->EquipItem(attachable.GetId(), notify);
    }
  }
  if (this->notifyWindow != nullptr) {
    this->notifyWindow->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006AD910
// FUNCTION: fn_06189
bool EquipmentProperties::CEquipmentPropertiesMgr::InitializeLua() {
  luaL_openlibs(this->script.GetState());

  this->script.SetGlobal("C_Logger", ::C_Logger);
  this->script.SetGlobal("get", ::GetPlayerValue);
  this->script.SetGlobal("GetEquipArmorLv", ::GetEquipArmorLv);
  this->script.SetGlobal("GetEquipGradeLevel", ::GetEquipGrade);
  this->script.SetGlobal("GetEquipWeaponLv", ::GetEquipWeaponLv);
  this->script.SetGlobal("GetItemIDLocation", ::GetEquipItemId);
  this->script.SetGlobal("GetLocation", ::GetLocation);
  this->script.SetGlobal("GetMapName", ::GetMapName);
  this->script.SetGlobal("GetPetRelationship", ::GetPetRelationship);
  this->script.SetGlobal("GetPureJob", ::GetPureJob);
  this->script.SetGlobal("GetRefineLevel", ::GetEquipRefine);
  this->script.SetGlobal("GetSkillLevel", ::GetSkillLevel);
  this->script.SetGlobal("GetWeaponClass", ::GetWeaponClass);
  this->script.SetGlobal("GetEquipTempValue", ::GetEquipTempValue);
  this->script.SetGlobal("IsPremiumPcCafe", ::IsPremiumPcCafe);
  this->script.SetGlobal("CheckJobGroup2", ::CheckJobGroup2);
  this->script.SetGlobal("GetCategoryJob", ::GetCategoryJob);
  this->script.SetGlobal("AddAttrTolerace", +[](lua_State* state) { return ::AddAttrTolerace(state); });
  this->script.SetGlobal("addattrtolerace", +[](lua_State* state) { return ::addattrtolerace(state); });
  this->script.SetGlobal("AddBowAttackDamage", +[](lua_State* state) { return ::AddBowAttackDamage(state); });
  this->script.SetGlobal("AddCRIPercent_Race", +[](lua_State* state) { return ::AddCRIPercent_Race(state); });
  this->script.SetGlobal("AddDamage_CRI", +[](lua_State* state) { return ::AddDamage_CRI(state); });
  this->script.SetGlobal("AddDamage_HIT", +[](lua_State* state) { return ::AddDamage_HIT(state); });
  this->script.SetGlobal("AddDamage_Property", +[](lua_State* state) { return ::AddDamage_Property(state); });
  this->script.SetGlobal("AddDamage_SKID", +[](lua_State* state) { return ::AddDamage_SKID(state); });
  this->script.SetGlobal("AddDamage_Size", +[](lua_State* state) { return ::AddDamage_Size(state); });
  this->script.SetGlobal("AddEXPPercent_KillRace", +[](lua_State* state) { return ::AddEXPPercent_KillRace(state); });
  this->script.SetGlobal("AddExtParam", +[](lua_State* state) { return ::AddExtParam(state); });
  this->script.SetGlobal("AddGuideAttack", +[](lua_State* state) { return ::AddGuideAttack(state); });
  this->script.SetGlobal("AddHPdrain", +[](lua_State* state) { return ::AddHPdrain(state); });
  this->script.SetGlobal("AddHealModifyPercent", +[](lua_State* state) { return ::AddHealModifyPercent(state); });
  this->script.SetGlobal("AddHealValue", +[](lua_State* state) { return ::AddHealValue(state); });
  this->script.SetGlobal("AddIgnore_MRES_RacePercent", +[](lua_State* state) { return ::AddIgnore_MRES_RacePercent(state); });
  this->script.SetGlobal("AddIgnore_RES_RacePercent", +[](lua_State* state) { return ::AddIgnore_RES_RacePercent(state); });
  this->script.SetGlobal("AddMDamage_Property", +[](lua_State* state) { return ::AddMDamage_Property(state); });
  this->script.SetGlobal("AddMDamage_Size", +[](lua_State* state) { return ::AddMDamage_Size(state); });
  this->script.SetGlobal("AddMdamage_Class", +[](lua_State* state) { return ::AddMdamage_Class(state); });
  this->script.SetGlobal("AddMdamage_Race", +[](lua_State* state) { return ::AddMdamage_Race(state); });
  this->script.SetGlobal("AddMeleeAttackDamage", +[](lua_State* state) { return ::AddMeleeAttackDamage(state); });
  this->script.SetGlobal("AddMeleeAttackReflect", +[](lua_State* state) { return ::AddMeleeAttackReflect(state); });
  this->script.SetGlobal("AddNeverknockback", +[](lua_State* state) { return ::AddNeverknockback(state); });
  this->script.SetGlobal("AddRaceTolerace", +[](lua_State* state) { return ::AddRaceTolerace(state); });
  this->script.SetGlobal("AddRangeAttackDamage", +[](lua_State* state) { return ::AddRangeAttackDamage(state); });
  this->script.SetGlobal("AddReceiveItem_Equip", +[](lua_State* state) { return ::AddReceiveItem_Equip(state); });
  this->script.SetGlobal("AddReflectMagic", +[](lua_State* state) { return ::AddReflectMagic(state); });
  this->script.SetGlobal("AddReflectTolerace", +[](lua_State* state) { return ::AddReflectTolerace(state); });
  this->script.SetGlobal("AddSFCTEquipAmount", +[](lua_State* state) { return ::AddSFCTEquipAmount(state); });
  this->script.SetGlobal("AddSFCTEquipPermill", +[](lua_State* state) { return ::AddSFCTEquipPermill(state); });
  this->script.SetGlobal("AddSPconsumption", +[](lua_State* state) { return ::AddSPconsumption(state); });
  this->script.SetGlobal("addspconsumption", +[](lua_State* state) { return ::addspconsumption(state); });
  this->script.SetGlobal("AddSPdrain", +[](lua_State* state) { return ::AddSPdrain(state); });
  this->script.SetGlobal("AddSkillDelay", +[](lua_State* state) { return ::AddSkillDelay(state); });
  this->script.SetGlobal("AddSkillMDamage", +[](lua_State* state) { return ::AddSkillMDamage(state); });
  this->script.SetGlobal("AddSkillSP", +[](lua_State* state) { return ::AddSkillSP(state); });
  this->script.SetGlobal("AddSpecificSpellCastTime", +[](lua_State* state) { return ::AddSpecificSpellCastTime(state); });
  this->script.SetGlobal("AddSpellCastTime", +[](lua_State* state) { return ::AddSpellCastTime(state); });
  this->script.SetGlobal("AddSpellDelay", +[](lua_State* state) { return ::AddSpellDelay(state); });
  this->script.SetGlobal("BodyAttribute", +[](lua_State* state) { return ::BodyAttribute(state); });
  this->script.SetGlobal("Clairvoyance", +[](lua_State* state) { return ::Clairvoyance(state); });
  this->script.SetGlobal("ClassAddDamage", +[](lua_State* state) { return ::ClassAddDamage(state); });
  this->script.SetGlobal("ClassSubDamage", +[](lua_State* state) { return ::ClassSubDamage(state); });
  this->script.SetGlobal("Condition", +[](lua_State* state) { return ::Condition(state); });
  this->script.SetGlobal("DisableSkill", +[](lua_State* state) { return ::DisableSkill(state); });
  this->script.SetGlobal("EnableSkill", +[](lua_State* state) { return ::EnableSkill(state); });
  this->script.SetGlobal("IndestructibleArmor", +[](lua_State* state) { return ::IndestructibleArmor(state); });
  this->script.SetGlobal("IndestructibleWeapon", +[](lua_State* state) { return ::IndestructibleWeapon(state); });
  this->script.SetGlobal("Magicimmune", +[](lua_State* state) { return ::Magicimmune(state); });
  this->script.SetGlobal("NoDispell", +[](lua_State* state) { return ::NoDispell(state); });
  this->script.SetGlobal("NoJamstone", +[](lua_State* state) { return ::NoJamstone(state); });
  this->script.SetGlobal("NoMadogearfuel", +[](lua_State* state) { return ::NoMadogearfuel(state); });
  this->script.SetGlobal("PerfectDamage", +[](lua_State* state) { return ::PerfectDamage(state); });
  this->script.SetGlobal("RaceAddDamage", +[](lua_State* state) { return ::RaceAddDamage(state); });
  this->script.SetGlobal("RaceAddDamageSelf", +[](lua_State* state) { return ::RaceAddDamageSelf(state); });
  this->script.SetGlobal("RaceSubDamage", +[](lua_State* state) { return ::RaceSubDamage(state); });
  this->script.SetGlobal("RaceSubDamageSelf", +[](lua_State* state) { return ::RaceSubDamageSelf(state); });
  this->script.SetGlobal("Reincarnation", +[](lua_State* state) { return ::Reincarnation(state); });
  this->script.SetGlobal("SetIgnoreDEFClass", +[](lua_State* state) { return ::SetIgnoreDEFClass(state); });
  this->script.SetGlobal("SetIgnoreDEFRace", +[](lua_State* state) { return ::SetIgnoreDEFRace(state); });
  this->script.SetGlobal("SetIgnoreDefClass_Percent", +[](lua_State* state) { return ::SetIgnoreDefClass_Percent(state); });
  this->script.SetGlobal("SetIgnoreDefRace_Percent", +[](lua_State* state) { return ::SetIgnoreDefRace_Percent(state); });
  this->script.SetGlobal("SetIgnoreMdefClass", +[](lua_State* state) { return ::SetIgnoreMdefClass(state); });
  this->script.SetGlobal("SetIgnoreMdefRace", +[](lua_State* state) { return ::SetIgnoreMdefRace(state); });
  this->script.SetGlobal("SetInvestigate", +[](lua_State* state) { return ::SetInvestigate(state); });
  this->script.SetGlobal("SplashAttack", +[](lua_State* state) { return ::SplashAttack(state); });
  this->script.SetGlobal("SubAttrTolerace", +[](lua_State* state) { return ::SubAttrTolerace(state); });
  this->script.SetGlobal("subattrtolerace", +[](lua_State* state) { return ::subattrtolerace(state); });
  this->script.SetGlobal("SubBowAttackDamage", +[](lua_State* state) { return ::SubBowAttackDamage(state); });
  this->script.SetGlobal("SubCRIPercent_Race", +[](lua_State* state) { return ::SubCRIPercent_Race(state); });
  this->script.SetGlobal("SubDamage_CRI", +[](lua_State* state) { return ::SubDamage_CRI(state); });
  this->script.SetGlobal("SubDamage_HIT", +[](lua_State* state) { return ::SubDamage_HIT(state); });
  this->script.SetGlobal("SubDamage_Property", +[](lua_State* state) { return ::SubDamage_Property(state); });
  this->script.SetGlobal("SubDamage_SKID", +[](lua_State* state) { return ::SubDamage_SKID(state); });
  this->script.SetGlobal("SubDamage_Size", +[](lua_State* state) { return ::SubDamage_Size(state); });
  this->script.SetGlobal("SubEXPPercent_KillRace", +[](lua_State* state) { return ::SubEXPPercent_KillRace(state); });
  this->script.SetGlobal("SubExtParam", +[](lua_State* state) { return ::SubExtParam(state); });
  this->script.SetGlobal("SubGuideAttack", +[](lua_State* state) { return ::SubGuideAttack(state); });
  this->script.SetGlobal("SubHPdrain", +[](lua_State* state) { return ::SubHPdrain(state); });
  this->script.SetGlobal("SubHealModifyPercent", +[](lua_State* state) { return ::SubHealModifyPercent(state); });
  this->script.SetGlobal("SubHealValue", +[](lua_State* state) { return ::SubHealValue(state); });
  this->script.SetGlobal("SubIgnore_MRES_RacePercent", +[](lua_State* state) { return ::SubIgnore_MRES_RacePercent(state); });
  this->script.SetGlobal("SubIgnore_RES_RacePercent", +[](lua_State* state) { return ::SubIgnore_RES_RacePercent(state); });
  this->script.SetGlobal("SubMDamage_Property", +[](lua_State* state) { return ::SubMDamage_Property(state); });
  this->script.SetGlobal("SubMDamage_Size", +[](lua_State* state) { return ::SubMDamage_Size(state); });
  this->script.SetGlobal("SubMdamage_Class", +[](lua_State* state) { return ::SubMdamage_Class(state); });
  this->script.SetGlobal("SubMdamage_Race", +[](lua_State* state) { return ::SubMdamage_Race(state); });
  this->script.SetGlobal("SubMeleeAttackDamage", +[](lua_State* state) { return ::SubMeleeAttackDamage(state); });
  this->script.SetGlobal("SubMeleeAttackReflect", +[](lua_State* state) { return ::SubMeleeAttackReflect(state); });
  this->script.SetGlobal("SubNeverknockback", +[](lua_State* state) { return ::SubNeverknockback(state); });
  this->script.SetGlobal("SubRaceTolerace", +[](lua_State* state) { return ::SubRaceTolerace(state); });
  this->script.SetGlobal("SubRangeAttackDamage", +[](lua_State* state) { return ::SubRangeAttackDamage(state); });
  this->script.SetGlobal("SubReceiveItem_Equip", +[](lua_State* state) { return ::SubReceiveItem_Equip(state); });
  this->script.SetGlobal("SubReflectMagic", +[](lua_State* state) { return ::SubReflectMagic(state); });
  this->script.SetGlobal("SubReflectTolerace", +[](lua_State* state) { return ::SubReflectTolerace(state); });
  this->script.SetGlobal("SubSFCTEquipAmount", +[](lua_State* state) { return ::SubSFCTEquipAmount(state); });
  this->script.SetGlobal("SubSFCTEquipPermill", +[](lua_State* state) { return ::SubSFCTEquipPermill(state); });
  this->script.SetGlobal("SubSPconsumption", +[](lua_State* state) { return ::SubSPconsumption(state); });
  this->script.SetGlobal("subspconsumption", +[](lua_State* state) { return ::subspconsumption(state); });
  this->script.SetGlobal("SubSPdrain", +[](lua_State* state) { return ::SubSPdrain(state); });
  this->script.SetGlobal("SubSkillDelay", +[](lua_State* state) { return ::SubSkillDelay(state); });
  this->script.SetGlobal("SubSkillMDamage", +[](lua_State* state) { return ::SubSkillMDamage(state); });
  this->script.SetGlobal("SubSkillSP", +[](lua_State* state) { return ::SubSkillSP(state); });
  this->script.SetGlobal("SubSpecificSpellCastTime", +[](lua_State* state) { return ::SubSpecificSpellCastTime(state); });
  this->script.SetGlobal("SubSpellCastTime", +[](lua_State* state) { return ::SubSpellCastTime(state); });
  this->script.SetGlobal("SubSpellDelay", +[](lua_State* state) { return ::SubSpellDelay(state); });
  this->script.SetGlobal("SetEquipTempValue", ::SetEquipTempValue);

  if (!this->script.LoadFile("LuaFiles514\\Lua Files\\EquipmentProperties\\EquipmentProperties.lub")) {
    return false;
  }
  if (!this->script.LoadFile("LuaFiles514\\Lua Files\\DataInfo\\EnumVar.lub")) {
    return false;
  }
  if (!this->script.LoadFile("LuaFiles514\\Lua Files\\DataInfo\\AddRandomOptionNameTable.lub")) {
    return false;
  }
  if (!this->script.LoadFile("LuaFiles514\\Lua Files\\EquipmentProperties\\EquipmentPropertiesOrder.lub")) {
    return false;
  }

  this->LoadEnumVar();
  this->LoadItems();
  this->LoadCombiitem();
  this->LoadSkillGroup();
  this->LoadGradeBonus();
  this->LoadADSkill();
  this->LoadRefiningBonus();
  this->LoadEquipmentPropertiesOrder();
  this->LoadIgnoreEquipmentProperties();
  return this->loadValid;
}

// OFFSET: 0x006B4600
// FUNCTION: fn_06197
void EquipmentProperties::CEquipmentPropertiesMgr::LoadItems() {
  lua_State* state = this->script.GetState();
  if (state == nullptr) {
    return;
  }

  const char* name = "Item";
  lua_getfield(state, LUA_GLOBALSINDEX, name);
  if (lua_type(state, -1) == LUA_TTABLE) {
    lua_pushnil(state);
    while (lua_next(state, -2) != 0) {
      {
        CEquipmentInfo info{0};
        do {
          unsigned int key = 0;
          int result;
          int type = lua_type(this->script.GetState(), -2);
          if (type == LUA_TNUMBER) {
            key = static_cast<unsigned int>(lua_tonumber(this->script.GetState(), -2));
            result = 0;
          } else {
            result = (type != LUA_TNIL) + 1;
          }
          this->loadValid &= result == 0;
          if (result != 0) {
            ShowErrorDialog(std::format("{0} table has invalid entry key({1})", name, result), 0);
          }
          if (!this->loadValid) {
            break;
          }

          std::vector<int> stats;
          if (lua_type(this->script.GetState(), -1) == LUA_TTABLE) {
            lua_getfield(this->script.GetState(), -1, "Stat");
            type = lua_type(this->script.GetState(), -1);
            if (type == LUA_TTABLE) {
              lua_pushnil(this->script.GetState());
              result = 0;
              while (lua_next(this->script.GetState(), -2) != 0) {
                if (lua_type(this->script.GetState(), -1) != LUA_TNUMBER) {
                  result = 2;
                  break;
                }
                stats.push_back(static_cast<int>(lua_tonumber(this->script.GetState(), -1)));
                lua_settop(this->script.GetState(), -2);
              }
            } else {
              result = (type != LUA_TNIL) + 1;
            }
            lua_settop(this->script.GetState(), -2);
          } else {
            result = 3;
          }
          bool valid = result == 0 || result == 1;
          this->loadValid &= valid;
          if (!valid) {
            ShowErrorDialog(std::format("Item[{0}] has invalid 'Stat' table({1})", key, result), 0);
          }
          if (!this->loadValid) {
            break;
          }

          std::string itemType;
          result = this->script.GetFieldString(itemType, "Type");
          const bool hasType = result == 0;
          this->loadValid &= hasType;
          if (result != 0) {
            ShowErrorDialog(std::format("Item[{0}] has invalid 'Type' field({1})", key, result), 0);
          }
          if (!this->loadValid) {
            break;
          }

          int count = 0;
          if (hasType) {
            unsigned int hash = HashName(itemType.c_str());
            switch (hash) {
            case 0x0F1AAAA6:
              info.kind = 1;
              count = 17;
              break;
            case 0x47F48D1C:
              info.kind = 2;
              count = 17;
              break;
            case 0xC8D7BCE1:
              info.kind = 3;
              count = 15;
              break;
            case 0x7C941E6F:
              info.kind = 4;
              count = 2;
              break;
            case 0xA4C66E86:
              info.kind = 5;
              break;
            }
          }
          valid = count == stats.size();
          this->loadValid &= valid;
          if (!valid) {
            ShowErrorDialog(std::format("Item[{0}] has invalid 'Stat' table(count: {1})", key, count), 0);
          }
          if (!this->loadValid) {
            break;
          }

          unsigned int hash = HashName(itemType.c_str());
          switch (hash) {
          case 0x0F1AAAA6:
            for (int index = 0; index < count; ++index) {
              if (stats[index] == 0) {
                continue;
              }
              int variable;
              switch (index) {
              case 0:
                variable = 45;
                break;
              case 1:
                variable = 13;
                break;
              case 2:
                variable = 16;
                break;
              case 3:
                variable = 15;
                break;
              case 4:
                variable = 17;
                break;
              case 5:
                variable = 14;
                break;
              case 6:
                variable = 18;
                break;
              case 7:
                variable = 6;
                break;
              case 8:
                variable = 8;
                break;
              case 9:
                variable = 47;
                break;
              case 10:
                info.values.try_emplace(2, stats[index]);
                continue;
              case 11:
                variable = 219;
                break;
              case 12:
                variable = 222;
                break;
              case 13:
                variable = 220;
                break;
              case 14:
                variable = 221;
                break;
              case 15:
                variable = 223;
                break;
              case 16:
                variable = 224;
                break;
              default:
                continue;
              }
              auto found = this->statVariables.find(variable);
              if (found == this->statVariables.end()) {
                ShowErrorDialog(std::format("Not available stat type: {0}", variable), 0);
              } else {
                info.statProperties.try_emplace(found->second.value, stats[index]);
              }
            }
            break;
          case 0x47F48D1C:
            for (int index = 0; index < count; ++index) {
              if (stats[index] == 0) {
                continue;
              }
              int variable;
              switch (index) {
              case 0:
                info.values.try_emplace(0, stats[index]);
                continue;
              case 1:
                info.values.try_emplace(1, stats[index]);
                continue;
              case 2:
                variable = 41;
                break;
              case 3:
                variable = 200;
                break;
              case 4:
                variable = 13;
                break;
              case 5:
                variable = 16;
                break;
              case 6:
                variable = 15;
                break;
              case 7:
                variable = 17;
                break;
              case 8:
                variable = 14;
                break;
              case 9:
                variable = 18;
                break;
              case 10:
                info.values.try_emplace(2, stats[index]);
                continue;
              case 11:
                variable = 219;
                break;
              case 12:
                variable = 222;
                break;
              case 13:
                variable = 220;
                break;
              case 14:
                variable = 221;
                break;
              case 15:
                variable = 223;
                break;
              case 16:
                variable = 224;
                break;
              default:
                continue;
              }
              auto found = this->statVariables.find(variable);
              if (found == this->statVariables.end()) {
                ShowErrorDialog(std::format("Not available stat type: {0}", variable), 0);
              } else {
                info.statProperties.try_emplace(found->second.value, stats[index]);
              }
            }
            break;
          case 0x7C941E6F:
            for (int index = 0; index < count; ++index) {
              if (stats[index] == 0) {
                continue;
              }
              if (index == 0) {
                info.values.try_emplace(0, stats[index]);
              } else if (index == 1) {
                const int variable = 41;
                auto found = this->statVariables.find(variable);
                if (found == this->statVariables.end()) {
                  ShowErrorDialog(std::format("Not available stat type: {0}", variable), 0);
                } else {
                  info.statProperties.try_emplace(found->second.value, stats[index]);
                }
              }
            }
            break;
          case 0xC8D7BCE1:
            for (int index = 0; index < count; ++index) {
              if (stats[index] == 0) {
                continue;
              }
              int variable;
              switch (index) {
              case 0:
                info.values.try_emplace(1, stats[index]);
                continue;
              case 1:
                variable = 41;
                break;
              case 2:
                variable = 13;
                break;
              case 3:
                variable = 16;
                break;
              case 4:
                variable = 15;
                break;
              case 5:
                variable = 17;
                break;
              case 6:
                variable = 14;
                break;
              case 7:
                variable = 18;
                break;
              case 8:
                info.values.try_emplace(2, stats[index]);
                continue;
              case 9:
                variable = 219;
                break;
              case 10:
                variable = 222;
                break;
              case 11:
                variable = 220;
                break;
              case 12:
                variable = 221;
                break;
              case 13:
                variable = 223;
                break;
              case 14:
                variable = 224;
                break;
              default:
                continue;
              }
              auto found = this->statVariables.find(variable);
              if (found == this->statVariables.end()) {
                ShowErrorDialog(std::format("Not available stat type: {0}", variable), 0);
              } else {
                info.statProperties.try_emplace(found->second.value, stats[index]);
              }
            }
            break;
          }

          if (info.kind != 0) {
            this->equipmentInfo.try_emplace(key, std::move(info));
          }

          std::vector<unsigned int> combinations;
          result = this->script.GetFieldUIntArray(combinations, "Combiitem");
          valid = result == 0 || result == 1;
          this->loadValid &= valid;
          if (!valid) {
            ShowErrorDialog(std::format("Item[{0}] has invalid 'Combiitem' table({1})", key, result), 0);
          }
          if (!this->loadValid) {
            break;
          }
          if (result == 0) {
            for (const unsigned int& combination : combinations) {
              this->m_itemCombinations[key].push_back(combination);
            }
          }
        } while (false);
      }
      lua_settop(state, -2);
    }
  } else {
    ShowErrorDialog(std::format("Can't find global table '{0}'", name), 0);
  }
  lua_settop(state, -2);
  if (!this->loadValid) {
    ShowErrorDialog(std::format("Check error in '{0}' table and restart game ", name), 0);
  }
}
