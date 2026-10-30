#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Effects/PartyMemberEffects.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Jobs/JobId.h"
#include "Script/LuaInstance.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIMileageWnd.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/World/World.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <list>
#include <map>
#include <new>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

void __fastcall RefreshItemWindows(UIWindowMgr* mgr);

struct SessionItemLists {
  /* 0000 */ unsigned char unknown_0000[0x1710];
  /* 1710 */ std::list<ITEM_INFO> inventory;
  /* 1718 */ std::list<ITEM_INFO> items_1718;
  /* 1720 */ std::list<ITEM_INFO> items_1720;
  /* 1728 */ std::list<ITEM_INFO> items_1728;
  /* 1730 */ std::list<ITEM_INFO> items_1730;
  /* 1738 */ unsigned char unknown_1738[0x48];
  /* 1780 */ std::list<ITEM_INFO> items_1780[11];

  void ClearItems1780();
  void KeepEquipment();
  void RemoveEquipment();
  void ClearTradeItems();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(SessionItemLists, inventory) == 0x1710);
static_assert(offsetof(SessionItemLists, items_1730) == 0x1730);
static_assert(offsetof(SessionItemLists, items_1780) == 0x1780);
#endif

namespace EquipmentProperties {
  class CEquipmentProperties;
}

using EquipmentProperty = EquipmentProperties::CEquipmentProperties;

struct EquipmentValueA {
  /* 0000 */ int value;
};

struct EquipmentValueB {
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
  /* 0000 */ unsigned int group;
  /* 0004 */ unsigned int id;
  /* 0008 */ std::string unknown_0008;
  /* 0020 */ std::vector<std::string> unknown_0020;
  /* 002C */ int unknown_002C;
  /* 0030 */ int mode;
  /* 0034 */ std::vector<std::string> unknown_0034;
  /* 0040 */ int unknown_0040;
  /* 0044 */ std::string unknown_0044;
  /* 005C */ std::map<int, int> unknown_005C;
  /* 0064 */ std::string unknown_0064;
  /* 007C */ int unknown_007C;
};

struct EquipmentPropertyGroup {
  /* 0000 */ std::string unknown_0000;
  /* 0018 */ std::map<std::int64_t, EquipmentPropertyDefinition*> definitions;
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

struct EquipmentPropertyMaps {
  /* 0000 */ int unknown_0000;
  /* 0004 */ std::unordered_map<int, int> first;
  /* 0024 */ std::unordered_map<int, int> second;
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
    void SetValue(const int& value, const unsigned int& key);

    /* 0010 */ int m_value;
  };

  class CEquipmentProperties2Dim : public CEquipmentProperties {
  public:
    explicit CEquipmentProperties2Dim(EquipmentPropertyDefinition* definition);
    CEquipmentProperties* Accumulate(CEquipmentProperties* other) override;
    ~CEquipmentProperties2Dim() override;
    CEquipmentProperties* Clone() override;
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
EquipmentProperties::CEquipmentProperties::~CEquipmentProperties() {
}

// OFFSET: 0x0068B3F0
// FUNCTION: fn_05609
EquipmentProperties::CEquipmentProperties2Dim::~CEquipmentProperties2Dim() {
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
void EquipmentProperties::CEquipmentProperties1Dim::SetValue(const int& value, const unsigned int& key) {
  this->m_value = value;
  if (key != -1) {
    this->m_keys.insert(key);
  }
}

// OFFSET: 0x006990A0
// FUNCTION: fn_05779
void EquipmentProperties::CEquipmentProperties2Dim::SetValue(const int& index, const int& value, const unsigned int& key) {
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

  if (this->m_definition->unknown_007C != 0) {
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

  class CEquipmentPropertiesMgr {
  public:
    /* 0004 */ CLuaInstance script;
    /* 000C */ bool unknown_000C;
    /* 000D */ bool m_dirty = false;
    /* 0010 */ int unknown_0010;
    /* 0014 */ int unknown_0014;
    /* 0018 */ EquipmentPropertyStore* store;
    /* 001C */ int unknown_001C;
    /* 0020 */ UIWindow* unknown_0020;
    /* 0024 */ std::unordered_map<int, CEquipmentWearable*> properties1;
    /* 0044 */ std::unordered_map<int, CEquipmentExpansion*> properties2;
    /* 0064 */ std::unordered_map<unsigned int, CEquipmentCombiitem*> properties3;
    /* 0084 */ std::multiset<unsigned long> unknown_0084;
    /* 008C */ std::map<int, EquipmentPropertyGroup> unknown_008C;
    /* 0094 */ std::unordered_map<int, EquipmentPropertyMaps> unknown_0094;
    /* 00B4 */ std::unordered_map<int, EquipmentValueA> unknown_00B4;
    /* 00D4 */ std::unordered_map<unsigned int, std::list<unsigned int>> m_itemCombinations;
    /* 00F4 */ std::unordered_map<unsigned int, std::list<unsigned int>> m_combinationItems;
    /* 0114 */ std::unordered_map<int, std::pair<int, int>> unknown_0114;
    /* 0134 */ std::unordered_map<int, EquipmentValueA> unknown_0134;
    /* 0154 */ std::unordered_map<std::uint64_t, EquipmentPropertyDefinition*> m_definitionsByName;
    /* 0174 */ std::unordered_map<int, EquipmentValueB> unknown_0174;
    /* 0194 */ std::set<std::uint64_t> unknown_0194;
    /* 019C */ std::unordered_map<int, EquipmentValueC> unknown_019C;
    /* 01BC */ std::unordered_map<int, std::unordered_map<int, std::unordered_map<unsigned int, float>>, CEquipmentValueHash> unknown_01BC;
    /* 01DC */ std::unordered_map<int, std::unordered_map<int, EquipmentEntryB>> unknown_01DC;

    static CEquipmentPropertiesMgr* GetInstance();

    CEquipmentPropertiesMgr();
    virtual ~CEquipmentPropertiesMgr();
    void Clear();
    void SetWindow(UIWindow* window);
    const int& GetUnknown0134(const int& key);
    int GetUnknown001C();
    CEquipmentWearable* FindProperty1(const int& key);
    EquipmentPropertyMaps* FindUnknown0094(const int& key);
    CLuaInstance& GetScript();
    int GetUnknown0174(const int& key);
    std::pair<int, int>* FindUnknown0114(const int& key);
    std::unordered_map<int, EquipmentValueC>& GetUnknown019C();
    bool HasUnknown0194(const std::uint64_t& key);
    EquipmentPropertyDefinition* FindDefinition(const std::uint64_t& key);
    int GetUnknown01BC(const int& first, const int& second, const unsigned int& third);
    void SetCurrentEquipment(CEquipment* equipment);
    void RefreshPretend();
    void Rebuild();
    void GetDescriptions(const int& group, std::list<std::string>& lines);
    void ApplyProperty(const char* name, lua_State* state, const int& count, const int& valuePosition, const int& indexPosition, const int& keyPosition);
    void EquipItem(const int& itemId, bool notify);
    void UnequipItem(const int& itemId, bool notify);
    void AddExpansion(const int& kind, const int& itemId, bool notify);
    void RemoveExpansion(const int& kind, const int& itemId);
    void RefreshExpansion(const int& kind, const int& itemId);
  };
}

namespace EquipmentProperties {
  struct CEquipmentInfo {
    /* 0000 */ int unknown_0000;
    /* 0004 */ std::unordered_map<int, int, CEquipmentValueHash> values;
  };

  class CEquipment {
  public:
    /* 0004 */ int m_id;
    /* 0008 */ CEquipmentInfo* m_info;
    /* 000C */ EquipmentPropertyStore m_properties;
    /* 0034 */ bool m_pretend;

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
    /* 0038 */ int unknown_0038;
    /* 003C */ CEquipment* m_owner;

    void Load(bool force) override;
    int GetType() override;

    CEquipment* GetOwner();
  };

  class CEquipmentWearable : public CEquipment {
  public:
    /* 0038 */ bool m_active;
    /* 003C */ int m_type;
    /* 0040 */ int unknown_0040;
    /* 0044 */ std::vector<CEquipmentAttachable> m_attachables;
    /* 0050 */ std::list<CEquipment*> m_children;

    void Clear() override;
    int GetType() override;

    std::vector<CEquipmentAttachable>& GetAttachables();
    bool IsActive();
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
static_assert(sizeof(EquipmentProperties::CEquipmentInfo) == 0x24);
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
  this->Clear();
}

// OFFSET: 0x00699040
// FUNCTION: fn_05775
std::map<unsigned int, std::map<unsigned int, EquipmentProperty*>>& EquipmentPropertyStore::GetProperties() {
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
  return this->unknown_0000;
}

// OFFSET: 0x0069BD00
// FUNCTION: fn_05809
std::map<std::int64_t, EquipmentPropertyDefinition*>& EquipmentPropertyGroup::GetDefinitions() {
  return this->definitions;
}

// OFFSET: 0x006A1AA0
// FUNCTION: fn_06013
EquipmentProperties::CEquipmentPropertiesMgr::CEquipmentPropertiesMgr()
    : unknown_000C(1), unknown_0010(-1), unknown_0014(-1), store(new EquipmentPropertyStore), unknown_001C(0), unknown_0020(0) {
}

// OFFSET: 0x006ABE90
// FUNCTION: fn_06170
void EquipmentProperties::CEquipmentPropertiesMgr::Clear() {
  this->unknown_001C = 0;
  this->unknown_0084.clear();
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
EquipmentProperties::CEquipmentCombiitem::CEquipmentCombiitem(const int& id) : CEquipment(id) {
}

// OFFSET: 0x0068A140
// FUNCTION: fn_05601
EquipmentProperties::CEquipmentExpansion::CEquipmentExpansion(const int& id, const int& kind) : CEquipment(id), m_kind(kind) {
  this->m_pretend = true;
}

// OFFSET: 0x0068B6C0
// FUNCTION: fn_05612
EquipmentProperties::CEquipment::~CEquipment() {
  this->m_properties.Clear();
}

// OFFSET: 0x0068CE10
// FUNCTION: fn_05638
void EquipmentProperties::CEquipment::Clear() {
  this->m_properties.Clear();
}

// OFFSET: 0x0068C020
// FUNCTION: fn_05624
int EquipmentProperties::CEquipment::GetValue(const int& key) {
  if (this->m_info == nullptr) {
    return -1;
  }

  auto found = this->m_info->values.find(key);
  if (found == this->m_info->values.end()) {
    return 0;
  }
  return found->second;
}

// OFFSET: 0x0068C0F0
// FUNCTION: fn_05628
int& EquipmentProperties::CEquipment::GetId() {
  return this->m_id;
}

// OFFSET: 0x0068C140
// FUNCTION: fn_05633
EquipmentPropertyStore& EquipmentProperties::CEquipment::GetProperties() {
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
std::vector<EquipmentProperties::CEquipmentAttachable>& EquipmentProperties::CEquipmentWearable::GetAttachables() {
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
bool EquipmentProperties::CEquipment::IsPretend() {
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

// OFFSET: 0x0068BAD0
void EquipmentProperties::CEquipment::AddProperty(const std::string& name, const char* op, const int& index, const int& value, const int& key) {
  unsigned int hash = 5381;
  for (const char* character = name.c_str(); *character != '\0'; ++character) {
    hash = hash * 33 + *character;
  }
  const std::uint64_t nameKey = hash;

  EquipmentPropertyDefinition* definition = CEquipmentPropertiesMgr::GetInstance()->FindDefinition(nameKey);
  if (definition == nullptr) {
    CEquipmentPropertiesMgr::GetInstance()->HasUnknown0194(nameKey);
    return;
  }

  const int sign = (definition->unknown_0020[0] == op) * 2 - 1;
  if (value == 0x7FFFFFFF) {
    if (index == -1 && definition->mode == 1 && definition->unknown_002C == 0) {
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

// OFFSET: 0x0068BEA0
// FUNCTION: fn_05622
void EquipmentProperties::CEquipment::CallOnStartEquip(lua_State* state) {
  lua_getfield(state, -1, "OnStartEquip");
  if (lua_type(state, -1) != LUA_TFUNCTION) {
    lua_settop(state, -2);
    return;
  }

  if (lua_pcall(state, 0, 0, 0) != 0) {
    std::string message = lua_tolstring(state, -1, nullptr);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF, 0, 0, 0);
    lua_settop(state, -2);
  }
}

// OFFSET: 0x0068C170
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

// OFFSET: 0x0068CE20
void EquipmentProperties::CEquipmentCombiitem::Clear() {
  this->NotifyProperties(g_equipmentClearColor, true, "(C)");
  this->m_properties.Clear();
}

// OFFSET: 0x0068CE50
void EquipmentProperties::CEquipmentExpansion::Clear() {
  this->NotifyProperties(g_equipmentClearColor, true, this->GetSuffix());
  this->m_properties.Clear();
}

// OFFSET: 0x0068CEA0
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
void EquipmentProperties::CEquipment::NotifyProperties(const COLORREF& color, bool negative, std::string_view suffix) {
  for (auto& group : this->m_properties.GetProperties()) {
    for (auto& entry : group.second) {
      std::vector<std::string> lines = Split(entry.second->Describe(true, negative), "\n");
      for (std::string& line : lines) {
        ReplaceAll(line, "#", " ");
        RemoveAll(line, "{~}");
        if (line.size() > 0) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(std::format("{0}{1}", suffix, line).c_str()), color, 0x18, 0, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00698560
EquipmentPropertyStore& EquipmentPropertyStore::operator+=(EquipmentPropertyStore& other) {
  for (auto& group : other.properties) {
    for (auto& entry : group.second) {
      EquipmentPropertyDefinition* definition = entry.second->m_definition;
      if (definition == nullptr) {
        continue;
      }

      if (this->properties[definition->group].find(definition->id) == this->properties[definition->group].end()) {
        this->properties[definition->group][definition->id] = entry.second->Clone();
      } else {
        this->properties[definition->group][definition->id]->Accumulate(entry.second);
      }
    }
  }
  return *this;
}

// OFFSET: 0x00698C30
EquipmentPropertyStore& EquipmentPropertyStore::operator+=(EquipmentProperty* property) {
  EquipmentPropertyDefinition* definition = property->m_definition;
  if (definition == nullptr) {
    return *this;
  }

  if (this->properties[definition->group].find(definition->id) == this->properties[definition->group].end()) {
    this->properties[definition->group][definition->id] = property->Clone();
  } else {
    this->properties[definition->group][definition->id]->Accumulate(property);
  }
  return *this;
}

// OFFSET: 0x0069A2A0
std::string EquipmentProperties::CEquipmentProperties::Describe(bool detailed, bool negative) {
  std::string text;
  if (this->m_definition == nullptr) {
    return text;
  }

  switch (this->m_definition->unknown_0034.size()) {
  case 0:
    text = this->m_definition->unknown_0008;
    break;
  case 2:
    text += EncodeText(g_textCodePage, std::vformat(DecodeText(g_textCodePage, this->m_definition->unknown_0008),
                                                    std::make_wformat_args(DecodeText(g_textCodePage, this->m_definition->unknown_0034[negative]))));
    break;
  }
  return text;
}

// OFFSET: 0x0069A4F0
void EquipmentPropertyStore::RemoveDuplicateNames() {
  std::set<std::string> names;
  for (auto& group : this->properties) {
    for (auto it = group.second.begin(); it != group.second.end();) {
      EquipmentProperty* property = it->second;
      EquipmentPropertyDefinition* definition = property->m_definition;
      if (definition == nullptr || definition->unknown_0064.size() == 0) {
        ++it;
      } else if (names.find(definition->unknown_0064) == names.end()) {
        names.insert(definition->unknown_0064);
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
const int& EquipmentProperties::CEquipmentPropertiesMgr::GetUnknown0134(const int& key) {
  auto found = this->unknown_0134.find(key);
  if (found == this->unknown_0134.end()) {
    return key;
  }
  return found->second.value;
}

// OFFSET: 0x006AC3B0
// FUNCTION: fn_06174
int EquipmentProperties::CEquipmentPropertiesMgr::GetUnknown001C() {
  return this->unknown_001C;
}

// OFFSET: 0x006AC3C0
// FUNCTION: fn_06175
EquipmentProperties::CEquipmentWearable* EquipmentProperties::CEquipmentPropertiesMgr::FindProperty1(const int& key) {
  auto found = this->properties1.find(key);
  if (found == this->properties1.end()) {
    return nullptr;
  }
  return found->second;
}

// OFFSET: 0x006AC440
// FUNCTION: fn_06176
EquipmentPropertyMaps* EquipmentProperties::CEquipmentPropertiesMgr::FindUnknown0094(const int& key) {
  auto found = this->unknown_0094.find(key);
  if (found == this->unknown_0094.end()) {
    return nullptr;
  }
  return &found->second;
}

// OFFSET: 0x006AC4C0
// FUNCTION: fn_06177
int GetEquipLocationMask(const int& location) {
  switch (location) {
  case 1:
    return 0x1;
  case 2:
    return 0x10;
  case 3:
    return 0x20;
  case 4:
    return 0x2;
  case 5:
    return 0x4;
  case 6:
    return 0x40;
  case 7:
    return 0x8;
  case 8:
    return 0x80;
  case 10:
    return 0x100;
  case 11:
    return 0x200;
  case 20:
    return 0x400;
  case 21:
    return 0x800;
  case 22:
    return 0x1000;
  case 23:
    return 0x2000;
  case 30:
    return 0x10000;
  case 31:
    return 0x20000;
  case 32:
    return 0x40000;
  case 33:
    return 0x80000;
  case 34:
    return 0x100000;
  case 35:
    return 0x200000;
  default:
    return 0;
  }
}

// OFFSET: 0x006AC610
// FUNCTION: fn_06178
CLuaInstance& EquipmentProperties::CEquipmentPropertiesMgr::GetScript() {
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
  case JT_GUNSLINGER:
  case JT_REBELLION:
  case JT_GUNSLINGER_B:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    return JT_GUNSLINGER;
  case JT_NINJA:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_NINJA_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
    return JT_NINJA;
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
  case JT_GUNSLINGER:
  case JT_GUNSLINGER_B:
    return JT_GUNSLINGER;
  case JT_NINJA:
  case JT_NINJA_B:
    return JT_NINJA;
  case JT_TAEKWON:
  case JT_TAEKWON_B:
    return JT_TAEKWON;
  case JT_STAR:
  case JT_STAR_B:
    return JT_STAR;
  case JT_LINKER:
  case JT_LINKER_B:
    return JT_LINKER;
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_KNIGHT_B:
  case JT_DRAGON_KNIGHT:
    return JT_RUNE_KNIGHT;
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
  case JT_ARCH_MAGE:
    return JT_WARLOCK;
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_RANGER_B:
  case JT_WINDHAWK:
    return JT_RANGER;
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
  case JT_CARDINAL:
    return JT_ARCHBISHOP;
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MECHANIC_B:
  case JT_MEISTER:
    return JT_MECHANIC;
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
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
  case JT_ELEMENTAL_MASTER:
    return JT_SORCERER;
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
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
  case JT_INQUISITOR:
    return JT_SURA;
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
  case JT_BIOLO:
    return JT_GENETIC;
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
  case JT_ABYSS_CHASER:
    return JT_SHADOW_CHASER;
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_HYPER_NOVICE:
    return JT_SUPERNOVICE2;
  case JT_KAGEROU:
  case JT_KAGEROU_B:
  case JT_SHINKIRO:
    return JT_KAGEROU;
  case JT_OBORO:
  case JT_OBORO_B:
  case JT_SHIRANUI:
    return JT_OBORO;
  case JT_REBELLION:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    return JT_REBELLION;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
    return JT_DO_SUMMONER;
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
  case JT_SKY_EMPEROR:
    return JT_STAR_EMPEROR;
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
  case JT_SOUL_ASCETIC:
    return JT_SOUL_REAPER;
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
  case JT_GUNSLINGER:
  case JT_GUNSLINGER_B:
    return JT_GUNSLINGER;
  case JT_NINJA:
  case JT_NINJA_B:
    return JT_NINJA;
  case JT_TAEKWON:
  case JT_TAEKWON_B:
    return JT_TAEKWON;
  case JT_STAR:
  case JT_STAR_B:
    return JT_STAR;
  case JT_LINKER:
  case JT_LINKER_B:
    return JT_LINKER;
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_KNIGHT_B:
    return JT_RUNE_KNIGHT;
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
    return JT_WARLOCK;
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_RANGER_B:
    return JT_RANGER;
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
    return JT_ARCHBISHOP;
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MECHANIC_B:
    return JT_MECHANIC;
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
    return JT_GUILLOTINE_CROSS;
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_ROYAL_GUARD_B:
    return JT_ROYAL_GUARD;
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
    return JT_SORCERER;
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
    return JT_MINSTREL;
  case JT_WANDERER:
  case JT_WANDERER_H:
  case JT_WANDERER_B:
    return JT_WANDERER;
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
    return JT_SURA;
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
    return JT_GENETIC;
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
    return JT_SHADOW_CHASER;
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
    return JT_SUPERNOVICE2;
  case JT_KAGEROU:
  case JT_KAGEROU_B:
    return JT_KAGEROU;
  case JT_OBORO:
  case JT_OBORO_B:
    return JT_OBORO;
  case JT_REBELLION:
  case JT_REBELLION_B:
    return JT_REBELLION;
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
    return JT_DO_SUMMONER;
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
    return JT_STAR_EMPEROR;
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
    return JT_SOUL_REAPER;
  case JT_DRUID:
  case JT_DRUID_B:
    return JT_DRUID;
  case JT_KARNOS:
  case JT_KARNOS_B:
    return JT_KARNOS;
  default:
    return job;
  }
}

// OFFSET: 0x006AD500
// FUNCTION: fn_06184
int EquipmentProperties::CEquipmentPropertiesMgr::GetUnknown0174(const int& key) {
  auto found = this->unknown_0174.find(key);
  if (found == this->unknown_0174.end()) {
    return 0;
  }
  return found->second.value;
}

// OFFSET: 0x006AD690
// FUNCTION: fn_06186
int GetEquipLocationIndex(const int& mask) {
  switch (mask) {
  case 0x1:
    return 1;
  case 0x2:
  case 0x22:
    return 4;
  case 0x4:
    return 5;
  case 0x8:
    return 7;
  case 0x10:
    return 2;
  case 0x20:
    return 3;
  case 0x40:
    return 6;
  case 0x80:
    return 8;
  case 0x100:
    return 10;
  case 0x101:
    return 1;
  case 0x200:
    return 11;
  case 0x201:
    return 1;
  case 0x300:
    return 10;
  case 0x301:
    return 1;
  case 0x400:
  case 0xC00:
  case 0x1400:
  case 0x1C00:
    return 20;
  case 0x800:
  case 0x1800:
    return 21;
  case 0x1000:
    return 22;
  case 0x2000:
    return 23;
  case 0x8000:
    return 0;
  case 0x10000:
    return 30;
  case 0x20000:
    return 31;
  case 0x40000:
    return 32;
  case 0x80000:
    return 33;
  case 0x100000:
    return 34;
  case 0x200000:
    return 35;
  default:
    return 0;
  }
}

// OFFSET: 0x006AD880
// FUNCTION: fn_06187
std::pair<int, int>* EquipmentProperties::CEquipmentPropertiesMgr::FindUnknown0114(const int& key) {
  auto found = this->unknown_0114.find(key);
  if (found == this->unknown_0114.end()) {
    return nullptr;
  }
  return &found->second;
}

// OFFSET: 0x006AD900
// FUNCTION: fn_06188
std::unordered_map<int, EquipmentValueC>& EquipmentProperties::CEquipmentPropertiesMgr::GetUnknown019C() {
  return this->unknown_019C;
}

// OFFSET: 0x006B0120
// FUNCTION: fn_06190
bool EquipmentProperties::CEquipmentPropertiesMgr::HasUnknown0194(const std::uint64_t& key) {
  return this->unknown_0194.find(key) != this->unknown_0194.end();
}

// OFFSET: 0x006B7760
// FUNCTION: fn_06210
void EquipmentProperties::CEquipmentPropertiesMgr::SetWindow(UIWindow* window) {
  this->unknown_0020 = window;
}

// OFFSET: 0x006AB520
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

  if (this->unknown_001C != 0) {
    reinterpret_cast<CEquipment*>(this->unknown_001C)->AddProperty(text, name, index, value, key);
  }
}

// OFFSET: 0x006AB810
void EquipmentProperties::CEquipmentPropertiesMgr::EquipItem(const int& itemId, bool notify) {
  this->unknown_0084.insert(itemId);

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
      if (this->unknown_0084.count(item) == 0) {
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
void EquipmentProperties::CEquipmentPropertiesMgr::UnequipItem(const int& itemId, bool notify) {
  this->unknown_0084.extract(itemId);

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
      if (this->unknown_0084.count(item) != 0) {
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
void EquipmentProperties::CEquipmentPropertiesMgr::GetDescriptions(const int& group, std::list<std::string>& lines) {
  if (this->m_dirty == true) {
    this->Rebuild();
  }

  lines.clear();
  for (auto& entry : this->store->GetProperties()) {
    if (group != 0 && entry.first != group) {
      continue;
    }
    for (auto& property : entry.second) {
      std::vector<std::string> parts = Split(property.second->Describe(false, false), "\n");
      for (const std::string& part : parts) {
        lines.push_back(part);
      }
    }
  }
}

// OFFSET: 0x006AD4A0
EquipmentPropertyDefinition* EquipmentProperties::CEquipmentPropertiesMgr::FindDefinition(const std::uint64_t& key) {
  auto found = this->m_definitionsByName.find(key);
  if (found == this->m_definitionsByName.end()) {
    return nullptr;
  }
  return found->second;
}

// OFFSET: 0x006AD580
int EquipmentProperties::CEquipmentPropertiesMgr::GetUnknown01BC(const int& first, const int& second, const unsigned int& third) {
  auto outer = this->unknown_01BC.find(first);
  if (outer == this->unknown_01BC.end()) {
    return 0;
  }
  auto middle = outer->second.find(second);
  if (middle == outer->second.end()) {
    return 0;
  }
  if (third == 0 || third > middle->second.size()) {
    return 0;
  }
  auto inner = middle->second.find(third);
  if (inner == middle->second.end()) {
    return 0;
  }
  return static_cast<int>(inner->second);
}

// OFFSET: 0x006B6AF0
void EquipmentProperties::CEquipmentPropertiesMgr::AddExpansion(const int& kind, const int& itemId, bool notify) {
  this->m_dirty = true;

  int key = itemId;
  if (kind == 0 || kind == 1) {
    key = kind;
  }

  auto found = this->properties2.find(key);
  if (found != this->properties2.end()) {
    delete found->second;
    this->properties2.erase(found);
  }

  CEquipmentExpansion* expansion = new CEquipmentExpansion(itemId, kind);
  CEquipment* equipment = this->properties2.insert({key, expansion}).first->second;
  if (kind == 1) {
    notify = g_windowMgr.LookupWindow(0x169) != nullptr;
  }
  equipment->Load(notify);
  this->EquipItem(equipment->GetId(), notify);

  if (this->unknown_0020 != nullptr) {
    this->unknown_0020->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B6E10
void EquipmentProperties::CEquipmentPropertiesMgr::RemoveExpansion(const int& kind, const int& itemId) {
  this->m_dirty = true;

  int key = itemId;
  if (kind == 0 || kind == 1) {
    key = kind;
  }

  auto found = this->properties2.find(key);
  if (found != this->properties2.end()) {
    this->UnequipItem(found->second->GetId(), true);
    found->second->Clear();
    delete found->second;
    this->properties2.erase(found);
  }

  if (this->unknown_0020 != nullptr) {
    this->unknown_0020->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B7020
void EquipmentProperties::CEquipmentPropertiesMgr::RefreshExpansion(const int& kind, const int& itemId) {
  int key = itemId;
  if (kind == 0 || kind == 1) {
    key = kind;
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

  if (this->unknown_0020 != nullptr) {
    this->unknown_0020->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x006B7530
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
void EquipmentProperties::CEquipmentPropertiesMgr::SetCurrentEquipment(CEquipment* equipment) {
  this->unknown_019C.clear();
  this->unknown_001C = reinterpret_cast<int>(equipment);
}

// OFFSET: 0x006B7770
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

  if (this->unknown_0020 != nullptr) {
    this->unknown_0020->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
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
  case JT_NOVICE_H:
    group = 10;
    break;
  case JT_SWORDMAN_H:
  case JT_MAGICIAN_H:
  case JT_ARCHER_H:
  case JT_ACOLYTE_H:
  case JT_MERCHANT_H:
  case JT_THIEF_H:
    group = 11;
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
  case JT_STAR:
  case JT_STAR2:
  case JT_LINKER:
  case JT_STAR_B:
  case JT_LINKER_B:
    group = 19;
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
    case JT_NOVICE_H:
    case JT_NOVICE_B:
      job = 0;
      break;
    case JT_SWORDMAN_H:
    case JT_SWORDMAN_B:
      job = 1;
      break;
    case JT_MAGICIAN_H:
    case JT_MAGICIAN_B:
      job = 2;
      break;
    case JT_ARCHER_H:
    case JT_ARCHER_B:
      job = 3;
      break;
    case JT_ACOLYTE_H:
    case JT_ACOLYTE_B:
      job = 4;
      break;
    case JT_MERCHANT_H:
    case JT_MERCHANT_B:
      job = 5;
      break;
    case JT_THIEF_H:
    case JT_THIEF_B:
      job = 6;
      break;
    case JT_KNIGHT_H:
    case JT_KNIGHT_B:
    case JT_RUNE_KNIGHT:
    case JT_RUNE_KNIGHT_H:
    case JT_RUNE_KNIGHT_B:
    case JT_DRAGON_KNIGHT:
      job = 7;
      break;
    case JT_PRIEST_H:
    case JT_PRIEST_B:
    case JT_ARCHBISHOP:
    case JT_ARCHBISHOP_H:
    case JT_ARCHBISHOP_B:
    case JT_CARDINAL:
      job = 8;
      break;
    case JT_WIZARD_H:
    case JT_WIZARD_B:
    case JT_WARLOCK:
    case JT_WARLOCK_H:
    case JT_WARLOCK_B:
    case JT_ARCH_MAGE:
      job = 9;
      break;
    case JT_BLACKSMITH_H:
    case JT_BLACKSMITH_B:
    case JT_MECHANIC:
    case JT_MECHANIC_H:
    case JT_MECHANIC_B:
    case JT_MEISTER:
      job = 10;
      break;
    case JT_HUNTER_H:
    case JT_HUNTER_B:
    case JT_RANGER:
    case JT_RANGER_H:
    case JT_RANGER_B:
    case JT_WINDHAWK:
      job = 11;
      break;
    case JT_ASSASSIN_H:
    case JT_ASSASSIN_B:
    case JT_GUILLOTINE_CROSS:
    case JT_GUILLOTINE_CROSS_H:
    case JT_GUILLOTINE_CROSS_B:
    case JT_SHADOW_CROSS:
      job = 12;
      break;
    case JT_CRUSADER_H:
    case JT_CRUSADER_B:
    case JT_ROYAL_GUARD:
    case JT_ROYAL_GUARD_H:
    case JT_ROYAL_GUARD_B:
    case JT_IMPERIAL_GUARD:
      job = 14;
      break;
    case JT_MONK_H:
    case JT_MONK_B:
    case JT_SURA:
    case JT_SURA_H:
    case JT_SURA_B:
    case JT_INQUISITOR:
      job = 15;
      break;
    case JT_SAGE_H:
    case JT_SAGE_B:
    case JT_SORCERER:
    case JT_SORCERER_H:
    case JT_SORCERER_B:
    case JT_ELEMENTAL_MASTER:
      job = 16;
      break;
    case JT_ROGUE_H:
    case JT_ROGUE_B:
    case JT_SHADOW_CHASER:
    case JT_SHADOW_CHASER_H:
    case JT_SHADOW_CHASER_B:
    case JT_ABYSS_CHASER:
      job = 17;
      break;
    case JT_ALCHEMIST_H:
    case JT_ALCHEMIST_B:
    case JT_GENETIC:
    case JT_GENETIC_H:
    case JT_GENETIC_B:
    case JT_BIOLO:
      job = 18;
      break;
    case JT_BARD_H:
    case JT_BARD_B:
    case JT_MINSTREL:
    case JT_MINSTREL_H:
    case JT_MINSTREL_B:
    case JT_TROUBADOUR:
      job = 19;
      break;
    case JT_DANCER_H:
    case JT_DANCER_B:
    case JT_WANDERER:
    case JT_WANDERER_H:
    case JT_WANDERER_B:
    case JT_TROUVERE:
      job = 20;
      break;
    case JT_SUPERNOVICE_B:
    case JT_SUPERNOVICE2:
    case JT_SUPERNOVICE2_B:
    case JT_HYPER_NOVICE:
      job = 23;
      break;
    case JT_KAGEROU:
    case JT_OBORO:
    case JT_NINJA_B:
    case JT_KAGEROU_B:
    case JT_OBORO_B:
    case JT_SHINKIRO:
    case JT_SHIRANUI:
      job = 25;
      break;
    case JT_REBELLION:
    case JT_GUNSLINGER_B:
    case JT_REBELLION_B:
    case JT_NIGHT_WATCH:
      job = 24;
      break;
    case JT_DO_SUMMONER_B:
    case JT_SPIRIT_HANDLER:
      job = 4218;
      break;
    case JT_TAEKWON_B:
      job = 4046;
      break;
    case JT_STAR_B:
    case JT_STAR_EMPEROR:
    case JT_STAR_EMPEROR_B:
    case JT_SKY_EMPEROR:
      job = 4047;
      break;
    case JT_LINKER_B:
    case JT_SOUL_REAPER:
    case JT_SOUL_REAPER_B:
    case JT_SOUL_ASCETIC:
      job = 4049;
      break;
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

// OFFSET: 0x006BD3C0

// OFFSET: 0x006BD9D0

// OFFSET: 0x006BDCE0

struct EquipLocationSource {
  virtual void Slot00();
  virtual void Slot01();
  virtual void Slot02();
  virtual int GetLocation();
};

// OFFSET: 0x006BD3C0
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
  int value = 0;
  if (wearable != nullptr) {
    value = wearable->GetValue(2);
  }
  lua_pushnumber(state, value);
  return 1;
}

// OFFSET: 0x006BDCE0
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
  int value = 0;
  if (wearable != nullptr) {
    value = wearable->GetValue(2);
  }
  lua_pushnumber(state, value);
  return 1;
}

// OFFSET: 0x006BE2F0
// FUNCTION: fn_06282
int GetLocation(lua_State* state) {
  int index = 0;
  EquipmentProperties::CEquipmentPropertiesMgr* mgr = EquipmentProperties::CEquipmentPropertiesMgr::GetInstance();
  if (mgr->unknown_001C != 0) {
    const int location = reinterpret_cast<EquipLocationSource*>(mgr->unknown_001C)->GetLocation();
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

// OFFSET: 0x006BE6C0
// FUNCTION: fn_06285
int GetPureJob(lua_State* state) {
  lua_pushnumber(state, GetFirstJob(g_session.m_job));
  return 1;
}

// OFFSET: 0x006BEFA0
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
  lua_pushnumber(state, g_mileageWndActive == 1 ? 2 : 0);
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

  const std::string text = lua_tostring(state, -1);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FromUtf8(text, g_textCodePage).c_str()), 0xFFFF, 0, 0, 0);
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
std::int32_t ClientMessenger::MemberColor(int index) {
  switch (index) {
  case 0:
    return -16108936;
  case 1:
    return -16746376;
  case 2:
    return -16777216;
  case 3:
  case 15:
    return -8912896;
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
  default:
    return -13487566;
  }
}

// OFFSET: 0x00DD6F10
// FUNCTION: fn_23281
void ClientMessenger::AddFriend(MessengerMember& member) {
  int index = 0;

  for (MessengerMember& existing : this->friends) {
    if (existing.name == member.name) {
      member.color = MemberColor(index);
      existing = member;
    }

    ++index;
  }

  member.color = MemberColor(index);
  this->friends.push_back(member);
}

// OFFSET: 0x00DD70B0
// FUNCTION: fn_23283
void ClientMessenger::AddPartyMember(MessengerMember& member, PartyMemberEffects& effects) {
  if (member.map.size() > 4) {
    member.map.resize(member.map.size() - 3);
    member.map.append("rsw", 3);
  }

  int index = 0;

  for (MessengerMember& existing : this->party) {
    if (existing.name == member.name) {
      member.color = MemberColor(index);
      existing = member;
      return;
    }

    ++index;
  }

  member.color = MemberColor(index);
  this->party.push_back(member);

  if (effects.Enabled()) {
    effects.EnsurePartyEffect(member.accountId);
  }
}

// OFFSET: 0x00DD87E0
// FUNCTION: fn_23312
void ClientMessenger::ClearFriends() {
  this->friends.clear();
}

// OFFSET: 0x00DDF8A0
// FUNCTION: fn_23427
int ClientMessenger::FriendCount() const {
  return static_cast<int>(this->friends.size());
}

// OFFSET: 0x00DDF9D0
// FUNCTION: fn_23436
int ClientMessenger::PartyCount() const {
  return static_cast<int>(this->party.size());
}

// OFFSET: 0x00DDFA20
// FUNCTION: fn_23441
int ClientMessenger::OnlinePartyCount() const {
  int count = 0;

  for (const MessengerMember& member : this->party) {
    if (member.offline == 0) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00DDCC30
// FUNCTION: fn_23372
MessengerMember ClientMessenger::FriendAt(int index) const {
  int position = 0;

  for (const MessengerMember& member : this->friends) {
    if (position == index) {
      return member;
    }

    ++position;
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE0500
// FUNCTION: fn_23455
MessengerMember ClientMessenger::PartyAt(int index) const {
  int position = 0;

  for (const MessengerMember& member : this->party) {
    if (position == index) {
      return member;
    }

    ++position;
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE00D0
// FUNCTION: fn_23452
MessengerMember ClientMessenger::PartyByAccount(std::uint32_t accountId) const {
  for (const MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      return member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE01C0
// FUNCTION: fn_23453
MessengerMember ClientMessenger::PartyByCharacter(std::uint32_t characterId) const {
  for (const MessengerMember& member : this->party) {
    if (member.characterId == characterId) {
      return member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DE03E0
// FUNCTION: fn_23454
MessengerMember ClientMessenger::PartyByName(const char* name) const {
  for (const MessengerMember& member : this->party) {
    if (member.name == name) {
      return member;
    }
  }

  return MessengerMember(0);
}

// OFFSET: 0x00DD8000
// FUNCTION: fn_23301
void ClientMessenger::SetPartyDead(std::uint32_t accountId, int dead) {
  for (MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      member.dead = dead;
      return;
    }
  }
}

// OFFSET: 0x00DD8030
// FUNCTION: fn_23302
void ClientMessenger::SetPartyJobLevel(std::uint32_t accountId, std::uint16_t job, std::uint16_t level) {
  for (MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      member.job = job;
      member.level = level;
      return;
    }
  }
}

// OFFSET: 0x00DD8070
// FUNCTION: fn_23303
void ClientMessenger::SetPartyLeader(std::uint32_t accountId, int leader) {
  for (MessengerMember& member : this->party) {
    if (member.accountId == accountId) {
      member.leader = leader;
      return;
    }
  }
}

struct SessionMessengerContext {
  unsigned char pad_0000[0x1710];
  /* 1710 */ std::list<MessengerMember> lists[12];
  /* 1770 */ unsigned char pad_1770[0x18];
  /* 1788 */ int listData[11][2];
  /* 17E0 */ unsigned char pad_17E0[0x26E0];
  /* 3EC0 */ int slots[10][62];
  /* 4870 */ unsigned char pad_4870[0x9A0];
  /* 5210 */ int slot5210;
  /* 5214 */ unsigned char pad_5214[0x118];
  /* 532C */ int stat2C;
  /* 5330 */ int stat30;
  /* 5334 */ int stat34;
  /* 5338 */ int stat38;
  /* 533C */ int stat3C;
  /* 5340 */ int stats[12];
  /* 5370 */ unsigned char pad_5370[0x1F8];
  /* 5568 */ int bound68;
  /* 556C */ int bound6C;
  /* 5570 */ int bound70;
  /* 5574 */ int bound74;

  void UpdateRandomStats();
  void ClearMemberLists();
  void PurgeMatchingMembers1();
  void ClearLists();
  void InitSlots();
  void PurgeMatchingMembers2();
  void ResetSlots();

  int GetList0Size() const;
  int GetList11Size() const;
  int GetList6Size() const;
  int GetList8Size() const;
  int GetList9Size() const;
  int GetList10Size() const;
  int GetList7Size() const;
  int GetList1788(int index) const;
  int GetList2Size() const;
  int GetList4Size() const;
  int GetList1Size() const;
  int GetList5Size() const;
};

struct ItemNotificationWindow : UIWindow {
  /* 0080 */ unsigned char unknown_0080[0x34];
  /* 00B4 */ MessengerMember member;
};

// OFFSET: 0x00AAAF10
// FUNCTION: fn_17276
void UIWindowMgr::NotifyItemWindows(int accountId, int lower, int upper) {
  for (UIWindow* window : this->snapExcluded) {
    bool matches;
    {
      MessengerMember member = static_cast<ItemNotificationWindow*>(window)->member;
      matches = member.accountId == accountId;
    }
    if (matches) {
      window->SendMsg(nullptr, 6, 34, accountId, lower, upper);
      break;
    }
  }
}

// OFFSET: 0x00CD6E40
// FUNCTION: fn_21290
void CGameMode::NotifyItemBounds() {
  if (this->itemWindow) {
    this->itemWindow->Invalidate();
  }
  if (g_windowMgr.window_02C8) {
    g_windowMgr.window_02C8->SendMsg(nullptr, 6, 34, g_session.m_aid, 0, 0);
  }
  g_windowMgr.NotifyItemWindows(g_session.m_aid, g_session.unknown_5568, g_session.unknown_556C);
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
    result = (type != LUA_TNIL) + 1;
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
  int result = 0;
  if (type == LUA_TTABLE) {
    lua_pushnil(this->m_state);
    while (lua_next(this->m_state, -2) != 0) {
      if (lua_type(this->m_state, -1) != LUA_TNUMBER) {
        result = 2;
        break;
      }
      values.push_back(static_cast<unsigned int>(lua_tonumber(this->m_state, -1)));
      lua_settop(this->m_state, -2);
    }
  } else {
    result = (type != LUA_TNIL) + 1;
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
    result = (type != LUA_TNIL) + 1;
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
  switch (kind) {
  case 8:
    return value == 6 || value == 9;
  case 0x16:
    if (g_windowMgr.unknown_0064 != 0) {
      return false;
    }
  case 0xA0:
    return value == 4 || value == 5;
  case 0x18:
    if (g_windowMgr.unknown_0064 != 0) {
      return false;
    }
  case 0x17:
  case 0xA1:
    return value == 3;
  case 0x1F:
  case 0x271B:
    return value == 1;
  case 0x21:
    return value == 9 || value == 1;
  case 0x28:
    return value == 1 || value == 6;
  case 0x29:
    return value == 0xB;
  case 0x2A:
    return value == 0xA;
  case 0x2B:
    return value == 0xD;
  case 0x2C:
  case 0xB2:
    return value == 0xC || value == 0x16;
  case 0xAF:
    return value == 0x15;
  case 0xB8:
  case 0x13E:
    return value == 6 || value == 4;
  }
  return false;
}

// OFFSET: 0x00CD73B0
// FUNCTION: fn_21292
bool CGameMode::IsWithinRange(const int& x1, const int& y1, const int& x2, const int& y2, int range) {
  return std::abs(x1 - x2) <= range && std::abs(y1 - y2) <= range;
}

// OFFSET: 0x00CD74B0
// FUNCTION: fn_21293
bool CGameMode::IsRegisterCell(int x, int y) {
  for (auto& entry : this->unknown_019C) {
    if (entry.second.first == x && entry.second.second == y) {
      CGameActor* actor = static_cast<CGameActor*>(this->scene->GetGameActorByAID(entry.first));
      if (actor == nullptr) {
        return true;
      }
      if (!StateFlag01(actor->Slot13())) {
        return true;
      }
    }
  }
  return false;
}

// OFFSET: 0x00CD7560
// FUNCTION: fn_21294
bool CGameMode::HasUnknown01A4(int value) {
  for (int entry : this->unknown_01A4) {
    if (entry == value) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00DD80A0
// FUNCTION: fn_23304
void SessionMessengerContext::UpdateRandomStats() {
  if (g_resetMessengerRandomStats != 0) {
    g_resetMessengerRandomStats = 0;
    this->stat34 = std::max(std::rand() % 12, 0);
    this->stat30 = std::max(std::rand() % 12, 0);
    this->stat3C = std::max(std::rand() % 12, 0);
    this->stat2C = std::max(std::rand() % 12, 0);
    this->stat38 = std::max(std::rand() % 12, 0);
  }

  const int bound70 = this->bound70;
  const int bound74 = this->bound74;
  const int bound68 = this->bound68;
  const int bound6C = this->bound6C;
  this->stat34 = std::max(std::rand() % 12, 0);
  this->stat2C = std::max(std::rand() % 12, 0);
  this->stat38 = std::max(std::rand() % 12, 0);
  this->stat30 = std::max(std::rand() % 12, 0);
  this->stat3C = std::max(std::rand() % 12, 0);
  for (int& value : this->stats) {
    value = std::max(std::abs(std::rand()) % 1, 0);
  }

  const auto notify = [this]() {
    if (g_modeMgr.GetGameMode()) {
      if (g_windowMgr.unknown_01DC) {
        if (this->bound6C < this->bound68 || this->bound74 < this->bound70) {
          return;
        }
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01DC)->Invalidate();
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01DC)->InvalidateChildren();
      }
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->NotifyItemBounds();
    }
  };
  if (this->bound68 != bound68) {
    this->bound68 = bound68;
    notify();
  }
  if (this->bound6C != bound6C) {
    this->bound6C = bound6C;
    notify();
  }
  if (this->bound70 != bound70) {
    this->bound70 = bound70;
    notify();
  }
  if (this->bound74 != bound74) {
    this->bound74 = bound74;
    notify();
  }
}

// OFFSET: 0x00DD8380

// OFFSET: 0x00DD8450

// OFFSET: 0x00DD8540

// OFFSET: 0x00DD85A0
// FUNCTION: fn_23308
void ClearMemberFlags(std::uint32_t* flags) {
  *flags &= ~0x40u;
}

// OFFSET: 0x00DD85B0

// OFFSET: 0x00DD8660

// OFFSET: 0x00DD8750
// FUNCTION: fn_23311
void SessionMessengerContext::ResetSlots() {
  this->slot5210 = 0;
  this->slots[0][0] = 0;
  this->slots[1][0] = 0;
  this->slots[2][0] = 0;
  this->slots[3][0] = 0;
  this->slots[4][0] = 0;
  this->slots[5][0] = 0;
  this->slots[6][0] = 0;
  this->slots[7][0] = 0;
  this->slots[8][0] = 0;
  this->slots[9][0] = 0;
}

// OFFSET: 0x00DDF8B0
// FUNCTION: fn_23428
int SessionMessengerContext::GetList0Size() const {
  return static_cast<int>(this->lists[0].size());
}

// OFFSET: 0x00DDF8C0
// FUNCTION: fn_23429
int SessionMessengerContext::GetList11Size() const {
  return static_cast<int>(this->lists[11].size());
}

// OFFSET: 0x00DDF8D0
// FUNCTION: fn_23430
int SessionMessengerContext::GetList6Size() const {
  return static_cast<int>(this->lists[6].size());
}

// OFFSET: 0x00DDF8E0
// FUNCTION: fn_23431
int SessionMessengerContext::GetList8Size() const {
  return static_cast<int>(this->lists[8].size());
}

// OFFSET: 0x00DDF8F0
// FUNCTION: fn_23432
int SessionMessengerContext::GetList9Size() const {
  return static_cast<int>(this->lists[9].size());
}

// OFFSET: 0x00DDF900
// FUNCTION: fn_23433
int SessionMessengerContext::GetList10Size() const {
  return static_cast<int>(this->lists[10].size());
}

// OFFSET: 0x00DDF910
// FUNCTION: fn_23434
int SessionMessengerContext::GetList7Size() const {
  return static_cast<int>(this->lists[7].size());
}

// OFFSET: 0x00DDF920
// FUNCTION: fn_23435
int SessionMessengerContext::GetList1788(int index) const {
  if (index < 0) {
    return 0;
  }
  if (index >= 11) {
    return 0;
  }
  return this->listData[index][0];
}

// OFFSET: 0x00DDF9E0
// FUNCTION: fn_23437
int SessionMessengerContext::GetList2Size() const {
  return static_cast<int>(this->lists[2].size());
}

// OFFSET: 0x00DDF9F0
// FUNCTION: fn_23438
int SessionMessengerContext::GetList4Size() const {
  return static_cast<int>(this->lists[4].size());
}

// OFFSET: 0x00DDFA00
// FUNCTION: fn_23439
int SessionMessengerContext::GetList1Size() const {
  return static_cast<int>(this->lists[1].size());
}

// OFFSET: 0x00DDFA10
// FUNCTION: fn_23440
int SessionMessengerContext::GetList5Size() const {
  return static_cast<int>(this->lists[5].size());
}

// OFFSET: 0x00DD8380
// FUNCTION: fn_23305
void SessionItemLists::ClearItems1780() {
  for (std::list<ITEM_INFO>& items : this->items_1780) {
    items.clear();
  }
}

// OFFSET: 0x00DD8450
// FUNCTION: fn_23306
void SessionItemLists::KeepEquipment() {
  for (auto entry = this->inventory.begin(); entry != this->inventory.end();) {
    switch (entry->unknown_0000[0]) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      ++entry;
      break;
    default:
      entry = this->inventory.erase(entry);
      break;
    }
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00DD8540
// FUNCTION: fn_23307
void SessionItemLists::ClearTradeItems() {
  this->items_1720.erase(this->items_1720.begin(), this->items_1720.end());
  this->items_1730.erase(this->items_1730.begin(), this->items_1730.end());
  this->items_1718.erase(this->items_1718.begin(), this->items_1718.end());
}

// OFFSET: 0x00DD8660
// FUNCTION: fn_23310
void SessionItemLists::RemoveEquipment() {
  for (auto entry = this->inventory.begin(); entry != this->inventory.end();) {
    switch (entry->unknown_0000[0]) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      entry = this->inventory.erase(entry);
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

  if (this->m_definition->unknown_007C != 0) {
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
    if (source->m_value < 0) {
      this->m_value = 0;
    } else {
      this->m_value = std::max(this->m_value, source->m_value);
    }
    break;
  }
  return this;
}

struct MessengerLinkNode {
  /* 0000 */ MessengerLinkNode* m_next;
  /* 0004 */ MessengerLinkNode* m_prev;
};

struct MessengerIntrusiveLink {
  /* 0000 */ unsigned int unknown_0000;
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
