#include <cstddef>
#include <list>
#include <map>
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
  /* 0000 */ unsigned int header[6];
  /* 0018 */ int value;
  /* 001C */ int field_1c;
  /* 0020 */ int field_20;
  /* 0024 */ bool flag_24;
  /* 0028 */ int field_28;
  /* 002C */ int field_2c;
  /* 0030 */ int amount;
  /* 0034 */ int field_34;
  /* 0038 */ bool flag_38;
  /* 003C */ int id;
  /* 0040 */ int field_40;
  /* 0044 */ int field_44;
};

class CBarterMarketItemMgr {
public:
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

  /* 0004 */ void* field_4;
  /* 0008 */ std::list<BarterMarketItem> items;
  /* 0010 */ std::list<BarterMarketItem> allItems;
  /* 0018 */ std::map<int, int> totals;
};

// GLOBAL: gl_24814
CBarterMarketItemMgr* g_barterMarketItemMgr;

// OFFSET: 0x005D7BF0
// FUNCTION: fn_03726
void CBarterMarketItemMgr::DestroyInstance()
{
  if (g_barterMarketItemMgr != NULL) {
    delete g_barterMarketItemMgr;
    g_barterMarketItemMgr = NULL;
  }
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

struct BarterMarketEntry {
  /* 0000 */ int field_0;
  /* 0004 */ int value;
  /* 0008 */ int field_8[9];
  /* 002C */ float factor;
  /* 0030 */ int field_30;
  /* 0034 */ int id;
  /* 0038 */ int field_38[5];
};

class CExpandedBarterMarketItemMgr {
public:
  std::vector<BarterMarketEntry>* GetItems();
  int GetItemValueById(int id);
  float GetItemFactorById(int id);
  std::vector<BarterMarketEntry>* GetEntries();
  int GetEntryCount();
  int GetValueById(int id);
  int* GetField24();
  float GetWeightedTotal();
  static void DestroyInstance();

  virtual ~CExpandedBarterMarketItemMgr();

  /* 0004 */ int field_4;
  /* 0008 */ int field_8;
  /* 000C */ std::vector<BarterMarketEntry> items;
  /* 0018 */ std::vector<BarterMarketEntry> entries;
  /* 0024 */ int field_24;
};

// GLOBAL: gl_24815
CExpandedBarterMarketItemMgr* g_expandedBarterMarketItemMgr;

// OFFSET: 0x005D9E10
// FUNCTION: fn_03760
void CExpandedBarterMarketItemMgr::DestroyInstance()
{
  if (g_expandedBarterMarketItemMgr != NULL) {
    delete g_expandedBarterMarketItemMgr;
    g_expandedBarterMarketItemMgr = NULL;
  }
}

// OFFSET: 0x005DA3A0
// FUNCTION: fn_03764
std::vector<BarterMarketEntry>* CExpandedBarterMarketItemMgr::GetItems()
{
  return &this->items;
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
int* CExpandedBarterMarketItemMgr::GetField24()
{
  return &this->field_24;
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

struct BarterChunk16 {
  /* 0000 */ unsigned int data[4];
};

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
