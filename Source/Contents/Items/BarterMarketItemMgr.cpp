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
  /* 0010 */ void* key;
  /* 0014 */ void* value;
};

struct BarterTree {
  /* 0000 */ BarterTreeNode* head;
  /* 0004 */ std::size_t size;

  void EraseSubtree(BarterTree* owner, BarterTreeNode* node);
  void Clear();
  ~BarterTree();
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
  float GetTotalAmount();
  std::list<BarterMarketItem>* GetItems();
  std::list<BarterMarketItem>* GetAllItems();
  int GetAllItemCount();
  std::map<int, int>* GetTotals();

  /* 0000 */ void* vtable;
  /* 0004 */ void* field_4;
  /* 0008 */ std::list<BarterMarketItem> items;
  /* 0010 */ std::list<BarterMarketItem> allItems;
  /* 0018 */ std::map<int, int> totals;
};

// OFFSET: 0x005D6F60
// FUNCTION: fn_03717
void BarterTree::EraseSubtree(BarterTree* owner, BarterTreeNode* root)
{
  BarterTreeNode* node = root;
  while (node->isnil == 0) {
    EraseSubtree(owner, node->right);
    BarterTreeNode* next = node->left;
    ::operator delete(node);
    node = next;
  }
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
    ::operator delete(node);
    node = next;
  }
  ::operator delete(head);
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
    ::operator delete(node);
    node = next;
  }
  head->parent = head;
  head->left = head;
  head->right = head;
  this->size = 0;
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
  int GetEntryCount();
  int GetValueById(int id);
  int* GetField24();
  float GetWeightedTotal();

  /* 0000 */ int field_0[3];
  /* 000C */ std::vector<BarterMarketEntry> items;
  /* 0018 */ std::vector<BarterMarketEntry> entries;
  /* 0024 */ int field_24;
};

// OFFSET: 0x005DA3A0
// FUNCTION: fn_03764
std::vector<BarterMarketEntry>* CExpandedBarterMarketItemMgr::GetItems()
{
  return &this->items;
}

// OFFSET: 0x005DA620
// FUNCTION: fn_03770
int CExpandedBarterMarketItemMgr::GetEntryCount()
{
  return (int)this->entries.size();
}

// OFFSET: 0x005DA640

// OFFSET: 0x005DA670
// FUNCTION: fn_03772
int* CExpandedBarterMarketItemMgr::GetField24()
{
  return &this->field_24;
}

// OFFSET: 0x005DA680
