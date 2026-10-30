#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <cstddef>
#include <list>
#include <map>
#include <new>
#include <string>
#include <utility>
#include <vector>

struct BarterTreeNode {
  /* 0000 */ BarterTreeNode* left;
  /* 0004 */ BarterTreeNode* parent;
  /* 0008 */ BarterTreeNode* right;
  /* 000C */ unsigned char color;
  /* 000D */ unsigned char isnil;
  /* 0010 */ int key;
  /* 0014 */ int value;
};

struct BarterTreeLookup {
  /* 0000 */ BarterTreeNode* node;
  /* 0004 */ int direction;
  /* 0008 */ BarterTreeNode* bound;
};

struct BarterTree {
  /* 0000 */ BarterTreeNode* head;
  /* 0004 */ std::size_t size;

  BarterTreeLookup FindBound(const int& key);
  void EraseSubtree(BarterTree* owner, BarterTreeNode* node);
  void Clear();
  ~BarterTree();
};

struct BarterTreeIterator {
  /* 0000 */ BarterTreeNode* node;

  BarterTreeIterator& operator++();
};

struct BarterMarketItem {
  /* 0000 */ std::string name;
  /* 0018 */ int value = 0;
  /* 001C */ int field_1c = 0;
  /* 0020 */ int field_20 = 0;
  /* 0024 */ bool flag_24 = true;
  /* 0028 */ int field_28 = 0;
  /* 002C */ int field_2c = 0;
  /* 0030 */ int amount = 0;
  /* 0034 */ int field_34 = 0;
  /* 0038 */ bool flag_38 = false;
  /* 003C */ int id = -1;
  /* 0040 */ int field_40 = 0;
  /* 0044 */ int field_44 = 0;

  BarterMarketItem() {}
  BarterMarketItem(const BarterMarketItem& other);
};

static_assert(sizeof(BarterMarketItem) == 0x48);

class CBarterMarketItemMgr {
public:
  CBarterMarketItemMgr();
  void RefreshTotals();
  void Clear();
  static bool CreateInstance();
  void AddItem(const BarterMarketItem& item);
  void RemoveItem(const BarterMarketItem& item);
  bool HasItemByName(std::string name);
  std::pair<bool, BarterMarketItem> FindItem(int id);
  std::pair<bool, BarterMarketItem> FindAllItem(int id);
  int GetAllItemValueByName(std::string name);
  int GetValueById(int id);
  int GetAmountById(int id);
  int GetAllItemValueById(int id);
  float GetTotalAmount();
  std::list<BarterMarketItem>* GetItems();
  std::list<BarterMarketItem>* GetAllItems();
  int GetAllItemCount();
  std::map<int, int>* GetTotals();
  static void DestroyInstance();

  virtual ~CBarterMarketItemMgr();

  /* 0004 */ bool initialized;
  /* 0008 */ std::list<BarterMarketItem> items;
  /* 0010 */ std::list<BarterMarketItem> allItems;
  /* 0018 */ std::map<int, int> totals;
};

static_assert(sizeof(CBarterMarketItemMgr) == 0x20);

// GLOBAL: gl_24814
CBarterMarketItemMgr* g_barterMarketItemMgr;

// OFFSET: 0x005D7020
// FUNCTION: fn_03719
BarterMarketItem::BarterMarketItem(const BarterMarketItem& other) = default;

// INLINED
CBarterMarketItemMgr::CBarterMarketItemMgr()
{
  this->initialized = true;
}

// INLINED
CBarterMarketItemMgr::~CBarterMarketItemMgr()
{
}

// OFFSET: 0x005D7840
// FUNCTION: fn_03723
void CBarterMarketItemMgr::RefreshTotals()
{
  this->totals.clear();
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    std::map<int, int>::iterator found = this->totals.find(it->field_2c);
    int total = it->field_28 * it->value;
    if (found != this->totals.end()) {
      found->second += total;
    } else {
      this->totals.insert(std::make_pair(it->field_2c, total));
    }
  }
}

// OFFSET: 0x005D7990
// FUNCTION: fn_03724
void CBarterMarketItemMgr::Clear()
{
  this->allItems.clear();
  this->items.clear();
  this->totals.clear();
}

// OFFSET: 0x005D79B0
// FUNCTION: fn_03725
bool CBarterMarketItemMgr::CreateInstance()
{
  if (g_barterMarketItemMgr == NULL) {
    g_barterMarketItemMgr = new (std::nothrow) CBarterMarketItemMgr();
    if (g_barterMarketItemMgr != NULL) {
      if (g_barterMarketItemMgr->initialized) {
        return true;
      }
      delete g_barterMarketItemMgr;
      g_barterMarketItemMgr = NULL;
    }
  }
  return false;
}

// OFFSET: 0x005D7BF0
// FUNCTION: fn_03726
void CBarterMarketItemMgr::DestroyInstance()
{
  if (g_barterMarketItemMgr != NULL) {
    delete g_barterMarketItemMgr;
    g_barterMarketItemMgr = NULL;
  }
}

// OFFSET: 0x005D7C10
// FUNCTION: fn_03727
void CBarterMarketItemMgr::AddItem(const BarterMarketItem& item)
{
  std::list<BarterMarketItem>::iterator found = std::find_if(this->allItems.begin(), this->allItems.end(), [item](const BarterMarketItem& entry) {
    return entry.id == item.id;
  });
  if (found != this->allItems.end()) {
    found->value += item.value;
  } else {
    this->allItems.push_back(item);
  }
  for (std::list<BarterMarketItem>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == item.id) {
      if (!it->flag_38) {
        it->value -= item.value;
        if (it->value <= 0) {
          this->items.erase(it);
          UIWindow* window = g_windowMgr.LookupWindow(334);
          if (window != NULL) {
            window->SendMsg(NULL, 23, 0, 0, 0, 0);
          }
        }
      }
      break;
    }
  }
  this->RefreshTotals();
}

// OFFSET: 0x005D7D10
// FUNCTION: fn_03728
void CBarterMarketItemMgr::RemoveItem(const BarterMarketItem& item)
{
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    if (it->id == item.id) {
      it->value -= item.value;
      if (it->value <= 0) {
        this->allItems.erase(it);
        UIWindow* window = g_windowMgr.LookupWindow(335);
        if (window != NULL) {
          window->SendMsg(NULL, 23, 0, 0, 0, 0);
        }
      }
      break;
    }
  }
  std::list<BarterMarketItem>::iterator found = std::find_if(this->items.begin(), this->items.end(), [item](const BarterMarketItem& entry) {
    return entry.id == item.id;
  });
  if (found != this->items.end()) {
    if (!found->flag_38) {
      found->value += item.value;
    }
  } else {
    this->items.push_back(item);
  }
  this->RefreshTotals();
}

// OFFSET: 0x005D7E40
// FUNCTION: fn_03729
bool CBarterMarketItemMgr::HasItemByName(std::string name)
{
  std::list<BarterMarketItem>::iterator found = std::find_if(this->allItems.begin(), this->allItems.end(), [name](const BarterMarketItem& item) {
    return item.name == name;
  });
  return found != this->allItems.end();
}

// OFFSET: 0x005D6F60
// FUNCTION: fn_03717
void BarterTree::EraseSubtree(BarterTree* owner, BarterTreeNode* root)
{
  BarterTreeNode* node = root;
  while (node->isnil == 0) {
    EraseSubtree(owner, node->right);
    BarterTreeNode* next = node->left;
    delete node;
    node = next;
  }
}

// OFFSET: 0x005D6FD0
// FUNCTION: fn_03718
BarterTreeLookup BarterTree::FindBound(const int& key)
{
  BarterTreeNode* head = this->head;
  BarterTreeLookup result;
  BarterTreeNode* node = head->parent;
  result.node = node;
  result.direction = 0;
  result.bound = head;
  while (node->isnil == 0) {
    result.node = node;
    int direction;
    if (node->key >= key) {
      result.bound = node;
      direction = 1;
      node = node->left;
    } else {
      direction = 0;
      node = node->right;
    }
    result.direction = direction;
  }
  return result;
}

// OFFSET: 0x005D7390
// FUNCTION: fn_03720
BarterTree::~BarterTree()
{
  BarterTreeNode* head = this->head;
  BarterTreeNode* node = head->parent;
  while (node->isnil == 0) {
    EraseSubtree(this, node->right);
    BarterTreeNode* next = node->left;
    delete node;
    node = next;
  }
  delete head;
}

// OFFSET: 0x005D84F0
// FUNCTION: fn_03743
void BarterTree::Clear()
{
  BarterTreeNode* head = this->head;
  BarterTreeNode* node = head->parent;
  while (node->isnil == 0) {
    EraseSubtree(this, node->right);
    BarterTreeNode* next = node->left;
    delete node;
    node = next;
  }
  head->parent = head;
  head->left = head;
  head->right = head;
  this->size = 0;
}

// OFFSET: 0x005D9630
// FUNCTION: fn_03756
BarterTreeIterator& BarterTreeIterator::operator++()
{
  BarterTreeNode* node = this->node;
  if (node->right->isnil != 0) {
    BarterTreeNode* parent = node->parent;
    while (parent->isnil == 0 && node == parent->right) {
      node = parent;
      parent = parent->parent;
    }
    this->node = parent;
  } else {
    BarterTreeNode* child = node->right;
    while (child->left->isnil == 0) {
      child = child->left;
    }
    this->node = child;
  }
  return *this;
}

// OFFSET: 0x005D7F10
// FUNCTION: fn_03730
int CBarterMarketItemMgr::GetValueById(int id)
{
  for (std::list<BarterMarketItem>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == id) {
      return it->value;
    }
  }
  return 0;
}

// OFFSET: 0x005D7F40
// FUNCTION: fn_03731
std::list<BarterMarketItem>* CBarterMarketItemMgr::GetItems()
{
  return &this->items;
}

// OFFSET: 0x005D7F50
// FUNCTION: fn_03732
std::pair<bool, BarterMarketItem> CBarterMarketItemMgr::FindItem(int id)
{
  for (std::list<BarterMarketItem>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == id) {
      return std::pair<bool, BarterMarketItem>(true, *it);
    }
  }
  return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
}

// OFFSET: 0x005D8050
// FUNCTION: fn_03733
std::pair<bool, BarterMarketItem> CBarterMarketItemMgr::FindAllItem(int id)
{
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    if (it->id == id) {
      return std::pair<bool, BarterMarketItem>(true, *it);
    }
  }
  return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
}

// OFFSET: 0x005D8150
// FUNCTION: fn_03734
std::list<BarterMarketItem>* CBarterMarketItemMgr::GetAllItems()
{
  return &this->allItems;
}

// OFFSET: 0x005D8160
// FUNCTION: fn_03735
int CBarterMarketItemMgr::GetAllItemCount()
{
  return (int)this->allItems.size();
}

// OFFSET: 0x005D8170
// FUNCTION: fn_03736
int CBarterMarketItemMgr::GetAllItemValueById(int id)
{
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    if (it->id == id) {
      return it->value;
    }
  }
  return 0;
}

// OFFSET: 0x005D81A0
// FUNCTION: fn_03737
int CBarterMarketItemMgr::GetAllItemValueByName(std::string name)
{
  int total = 0;
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    if (it->name == name) {
      total += it->value;
    }
  }
  return total;
}

// OFFSET: 0x005D8250
// FUNCTION: fn_03738
int CBarterMarketItemMgr::GetAmountById(int id)
{
  for (std::list<BarterMarketItem>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == id) {
      return it->amount;
    }
  }
  return -1;
}

// OFFSET: 0x005D8280
// FUNCTION: fn_03739
std::map<int, int>* CBarterMarketItemMgr::GetTotals()
{
  return &this->totals;
}

// OFFSET: 0x005D8290
// FUNCTION: fn_03740
float CBarterMarketItemMgr::GetTotalAmount()
{
  float total = 0.0f;
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    total += (float)(it->amount * it->value);
  }
  return total;
}

struct BarterChunk16 {
  /* 0000 */ int field_0;
  /* 0004 */ int field_4;
  /* 0008 */ int amount;
  /* 000C */ int field_c;
};

struct BarterMarketEntry {
  /* 0000 */ int field_0 = 0;
  /* 0004 */ int value = 0;
  /* 0008 */ int field_8 = 0;
  /* 000C */ std::string name;
  /* 0024 */ bool flag_24 = true;
  /* 0028 */ int field_28 = 0;
  /* 002C */ float factor = 0.0f;
  /* 0030 */ bool flag_30 = false;
  /* 0034 */ int id = 0;
  /* 0038 */ int field_38 = 0;
  /* 003C */ int field_3c = 0;
  /* 0040 */ std::vector<BarterChunk16> chunks;

  BarterMarketEntry() {}

  // OFFSET: 0x005D8DA0
  // FUNCTION: fn_03752
  BarterMarketEntry(const BarterMarketEntry& other) = default;
};

static_assert(sizeof(BarterMarketEntry) == 0x4C);

class CExpandedBarterMarketItemMgr {
public:
  CExpandedBarterMarketItemMgr();
  static bool CreateInstance();
  void RemoveEntry(const BarterMarketEntry& entry);
  void AddEntry(const BarterMarketEntry& entry);
  bool HasEntryByName(std::string name);
  std::pair<bool, BarterMarketEntry> FindItem(int id);
  std::pair<bool, BarterMarketEntry> FindEntry(int id);
  void AddItem(const BarterMarketEntry& source);
  void Clear();
  std::vector<BarterMarketEntry>* GetItems();
  int GetItemValueById(int id);
  float GetItemFactorById(int id);
  std::vector<BarterMarketEntry>* GetEntries();
  int GetEntryCount();
  int GetValueById(int id);
  std::vector<BarterChunk16>* GetChunks();
  float GetWeightedTotal();
  static void DestroyInstance();

  virtual ~CExpandedBarterMarketItemMgr();

  /* 0008 */ bool initialized;
  /* 000C */ std::vector<BarterMarketEntry> items;
  /* 0018 */ std::vector<BarterMarketEntry> entries;
  /* 0024 */ std::vector<BarterChunk16> chunks;
  /* 0030 */ std::multimap<int, ITEM_INFO> itemInfos;
  /* 0038 */ __int64 total;
};

static_assert(sizeof(CExpandedBarterMarketItemMgr) == 0x40);

// GLOBAL: gl_24815
CExpandedBarterMarketItemMgr* g_expandedBarterMarketItemMgr;

// INLINED
CExpandedBarterMarketItemMgr::CExpandedBarterMarketItemMgr() : total(0)
{
  this->initialized = true;
}

// INLINED
CExpandedBarterMarketItemMgr::~CExpandedBarterMarketItemMgr()
{
}

// OFFSET: 0x005D9B00
// FUNCTION: fn_03759
bool CExpandedBarterMarketItemMgr::CreateInstance()
{
  if (g_expandedBarterMarketItemMgr == NULL) {
    g_expandedBarterMarketItemMgr = new (std::nothrow) CExpandedBarterMarketItemMgr();
    if (g_expandedBarterMarketItemMgr != NULL) {
      if (g_expandedBarterMarketItemMgr->initialized) {
        return true;
      }
      delete g_expandedBarterMarketItemMgr;
      g_expandedBarterMarketItemMgr = NULL;
    }
  }
  return false;
}

// OFFSET: 0x005D9E10
// FUNCTION: fn_03760
void CExpandedBarterMarketItemMgr::DestroyInstance()
{
  if (g_expandedBarterMarketItemMgr != NULL) {
    delete g_expandedBarterMarketItemMgr;
    g_expandedBarterMarketItemMgr = NULL;
  }
}

// OFFSET: 0x005D9E30
// FUNCTION: fn_03761
void CExpandedBarterMarketItemMgr::RemoveEntry(const BarterMarketEntry& entry)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    if (it->id == entry.id) {
      it->value -= entry.value;
      if (it->value <= 0) {
        this->entries.erase(it);
      }
      break;
    }
  }
  std::vector<BarterMarketEntry>::iterator found = std::find_if(this->items.begin(), this->items.end(), [&entry](const BarterMarketEntry& item) {
    return item.id == entry.id;
  });
  if (found != this->items.end()) {
    if (!found->flag_30) {
      found->value += entry.value;
    }
  } else {
    this->items.push_back(entry);
  }
  for (std::vector<BarterChunk16>::const_iterator it = entry.chunks.begin(); it != entry.chunks.end(); ++it) {
    int count = entry.value;
    BarterChunk16 chunk = *it;
    for (std::vector<BarterChunk16>::iterator target = this->chunks.begin(); target != this->chunks.end(); ++target) {
      if (target->field_0 == chunk.field_0 && target->field_4 == chunk.field_4) {
        target->amount -= chunk.amount * count;
        if (target->amount <= 0) {
          this->chunks.erase(target);
        }
        break;
      }
    }
  }
  this->total = 0;
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    this->total += it->field_8 * it->value;
  }
}

// OFFSET: 0x005DA040
// FUNCTION: fn_03762
void CExpandedBarterMarketItemMgr::AddEntry(const BarterMarketEntry& entry)
{
  std::vector<BarterMarketEntry>::iterator found = std::find_if(this->entries.begin(), this->entries.end(), [&entry](const BarterMarketEntry& item) {
    return item.id == entry.id;
  });
  if (found != this->entries.end()) {
    found->value += entry.value;
  } else {
    this->entries.push_back(entry);
  }
  for (std::vector<BarterMarketEntry>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == entry.id && !it->flag_30) {
      it->value -= entry.value;
      if (it->value < 0) {
        it->value = 0;
      }
      break;
    }
  }
  for (std::vector<BarterChunk16>::const_iterator it = entry.chunks.begin(); it != entry.chunks.end(); ++it) {
    int count = entry.value;
    BarterChunk16 chunk = *it;
    std::vector<BarterChunk16>::iterator target = std::find_if(this->chunks.begin(), this->chunks.end(), [&chunk](const BarterChunk16& other) {
      return other.field_0 == chunk.field_0 && other.field_4 == chunk.field_4;
    });
    if (target != this->chunks.end()) {
      target->amount += chunk.amount * count;
    } else {
      BarterChunk16 added;
      added.field_0 = chunk.field_0;
      added.field_4 = chunk.field_4;
      added.amount = chunk.amount * count;
      added.field_c = chunk.field_c;
      this->chunks.push_back(added);
    }
  }
  this->total = 0;
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    this->total += it->field_8 * it->value;
  }
}

// OFFSET: 0x005DA1C0
// FUNCTION: fn_03763
bool CExpandedBarterMarketItemMgr::HasEntryByName(std::string name)
{
  bool found = false;
  std::find_if(this->entries.begin(), this->entries.end(), [name, &found](BarterMarketEntry entry) {
    found = name == entry.name;
    return found;
  });
  return found;
}

// OFFSET: 0x005DA3A0
// FUNCTION: fn_03764
std::vector<BarterMarketEntry>* CExpandedBarterMarketItemMgr::GetItems()
{
  return &this->items;
}

// OFFSET: 0x005DA3E0
// FUNCTION: fn_03766
std::pair<bool, BarterMarketEntry> CExpandedBarterMarketItemMgr::FindItem(int id)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == id) {
      return std::pair<bool, BarterMarketEntry>(true, *it);
    }
  }
  return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
}

// OFFSET: 0x005DA4E0
// FUNCTION: fn_03767
std::pair<bool, BarterMarketEntry> CExpandedBarterMarketItemMgr::FindEntry(int id)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    if (it->id == id) {
      return std::pair<bool, BarterMarketEntry>(true, *it);
    }
  }
  return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
}

// OFFSET: 0x005DA3B0
// FUNCTION: fn_03765
int CExpandedBarterMarketItemMgr::GetItemValueById(int id)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == id) {
      return it->value;
    }
  }
  return 0;
}

// OFFSET: 0x005DA5E0
// FUNCTION: fn_03768
float CExpandedBarterMarketItemMgr::GetItemFactorById(int id)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (it->id == id) {
      return it->factor;
    }
  }
  return -1.0f;
}

// OFFSET: 0x005DA610
// FUNCTION: fn_03769
std::vector<BarterMarketEntry>* CExpandedBarterMarketItemMgr::GetEntries()
{
  return &this->entries;
}

// OFFSET: 0x005DA620
// FUNCTION: fn_03770
int CExpandedBarterMarketItemMgr::GetEntryCount()
{
  return (int)this->entries.size();
}

// OFFSET: 0x005DA640
// FUNCTION: fn_03771
int CExpandedBarterMarketItemMgr::GetValueById(int id)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    if (it->id == id) {
      return it->value;
    }
  }
  return 0;
}

// OFFSET: 0x005DA670
// FUNCTION: fn_03772
std::vector<BarterChunk16>* CExpandedBarterMarketItemMgr::GetChunks()
{
  return &this->chunks;
}

// OFFSET: 0x005DA680
// FUNCTION: fn_03773
float CExpandedBarterMarketItemMgr::GetWeightedTotal()
{
  float total = 0.0f;
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    total += (float)it->value * it->factor;
  }
  return total;
}

// OFFSET: 0x005DA8E0
// FUNCTION: fn_03775
void CExpandedBarterMarketItemMgr::AddItem(const BarterMarketEntry& source)
{
  BarterMarketEntry entry;
  entry.value = source.value;
  entry.field_0 = source.field_0;
  entry.name = source.name;
  entry.field_38 = source.field_38;
  entry.field_3c = source.field_3c;
  entry.flag_24 = true;
  entry.field_28 = source.field_28;
  for (std::vector<BarterChunk16>::const_iterator it = source.chunks.begin(); it != source.chunks.end(); ++it) {
    BarterChunk16 chunk;
    chunk.field_0 = it->field_0;
    chunk.field_4 = it->field_4;
    chunk.amount = it->amount;
    chunk.field_c = it->field_c;
    entry.chunks.push_back(chunk);
  }
  entry.id = source.id;
  entry.flag_30 = source.value == -1;
  entry.factor = source.factor;
  entry.field_8 = source.field_8;
  this->items.push_back(entry);
}

// OFFSET: 0x005DAA30
// FUNCTION: fn_03776
void CExpandedBarterMarketItemMgr::Clear()
{
  this->total = 0;
  this->entries.clear();
  this->items.clear();
  this->chunks.clear();
}

struct BarterEntryPair {
  /* 0000 */ int id;
  /* 0004 */ int value;
};

// OFFSET: 0x005DBA50
// FUNCTION: fn_03798
void SortMedianOfThree(BarterEntryPair* first, BarterEntryPair* mid, BarterEntryPair* last)
{
  if (mid->id < first->id) {
    BarterEntryPair saved = *mid;
    *mid = *first;
    *first = saved;
  }
  if (last->id < mid->id) {
    BarterEntryPair saved = *last;
    *last = *mid;
    *mid = saved;
    if (mid->id < first->id) {
      saved = *mid;
      *mid = *first;
      *first = saved;
    }
  }
}

struct BarterEntryAllocator {
  void Deallocate(BarterMarketEntry* entries, std::size_t count);
};

// OFFSET: 0x005DB2C0
// FUNCTION: fn_03790
void BarterEntryAllocator::Deallocate(BarterMarketEntry* entries, std::size_t count)
{
  std::size_t bytes = count * sizeof(BarterMarketEntry);
  if (bytes >= 0x1000) {
    unsigned int block = *(unsigned int*)((char*)entries - sizeof(unsigned int));
    bytes += 0x23;
    if ((std::size_t)entries - block - sizeof(unsigned int) > 0x1f) {
      _invalid_parameter_noinfo_noreturn();
    }
    entries = (BarterMarketEntry*)block;
  }
  ::operator delete(entries, bytes);
}

// OFFSET: 0x005DB160
// FUNCTION: fn_03784
BarterChunk16* CopyChunks16(BarterChunk16* first, BarterChunk16* last, BarterChunk16* dest)
{
  while (first != last) {
    *dest = *first;
    ++first;
    ++dest;
  }
  return dest;
}

// OFFSET: 0x005DB210
// FUNCTION: fn_03786
BarterChunk16* MoveChunks16(BarterChunk16* first, BarterChunk16* last, BarterChunk16* dest)
{
  while (first != last) {
    *dest = *first;
    ++first;
    ++dest;
  }
  return first;
}

struct BarterTriple {
  /* 0000 */ int first;
  /* 0004 */ int second;
  /* 0008 */ int third;
};

// OFFSET: 0x005DC060
// FUNCTION: fn_03802
BarterTriple* CopyTriples(BarterTriple* first, BarterTriple* last, BarterTriple* dest)
{
  while (first != last) {
    dest->first = first->first;
    dest->second = first->second;
    dest->third = first->third;
    ++first;
    ++dest;
  }
  return dest;
}

struct BarterChunkAllocator {
  void Deallocate(BarterChunk16* chunks, std::size_t count);
};

// OFFSET: 0x005DB300
// FUNCTION: fn_03791
void BarterChunkAllocator::Deallocate(BarterChunk16* chunks, std::size_t count)
{
  std::size_t bytes = count * sizeof(BarterChunk16);
  if (bytes >= 0x1000) {
    unsigned int block = *(unsigned int*)((char*)chunks - sizeof(unsigned int));
    bytes += 0x23;
    if ((std::size_t)chunks - block - sizeof(unsigned int) > 0x1f) {
      _invalid_parameter_noinfo_noreturn();
    }
    chunks = (BarterChunk16*)block;
  }
  ::operator delete(chunks, bytes);
}

// OFFSET: 0x005DBD40
// FUNCTION: fn_03800
void AdjustHeap(BarterEntryPair* first, int hole, int top, BarterEntryPair* value)
{
  const int bottom = hole;
  int idx = hole;
  for (; idx < (top - 1) / 2; ) {
    idx = 2 * idx + 2;
    if (first[idx].id < first[idx - 1].id) {
      --idx;
    }
    first[hole] = first[idx];
    hole = idx;
  }
  if (idx == (top - 1) / 2 && top % 2 == 0) {
    first[hole] = first[top - 1];
    hole = top - 1;
  }
  for (; bottom < hole; hole = idx) {
    idx = (hole - 1) >> 1;
    if (!(first[idx].id < value->id)) {
      break;
    }
    first[hole] = first[idx];
  }
  first[hole] = *value;
}

struct BarterKeyNode {
  /* 0000 */ BarterKeyNode* left;
  /* 0004 */ BarterKeyNode* parent;
  /* 0008 */ BarterKeyNode* right;
  /* 000C */ unsigned char color;
  /* 000D */ unsigned char isnil;
  /* 0010 */ std::vector<int> key;
};

struct BarterKeyLookup {
  /* 0000 */ BarterKeyNode* node;
  /* 0004 */ int direction;
  /* 0008 */ BarterKeyNode* bound;
};

struct BarterKeyTree {
  /* 0000 */ BarterKeyNode* head;

  BarterKeyLookup FindLowerBound(const std::vector<int>& key);
};

// OFFSET: 0x005DB9A0
// FUNCTION: fn_03797
BarterKeyLookup BarterKeyTree::FindLowerBound(const std::vector<int>& key)
{
  BarterKeyLookup result;
  BarterKeyNode* node = this->head->parent;
  result.node = node;
  result.direction = 0;
  result.bound = this->head;
  while (node->isnil == 0) {
    result.node = node;
    int nodeCount = (int)node->key.size();
    int keyCount = (int)key.size();
    int count = nodeCount < keyCount ? keyCount : nodeCount;
    int relation = 0;
    for (int i = 0; i < count; ++i) {
      int left = i < nodeCount ? node->key[i] : 0;
      int right = i < keyCount ? key[i] : 0;
      if (left < right) {
        relation = -1;
        break;
      }
      if (left > right) {
        relation = 1;
        break;
      }
    }
    if (relation < 0) {
      result.direction = 0;
      node = node->right;
    } else {
      result.bound = node;
      result.direction = 1;
      node = node->left;
    }
  }
  return result;
}
