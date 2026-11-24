#include "Contents/Items/BarterMarketItemMgr.h"

#include "Client/Session/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
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

struct BarterTree {
  /* 0000 */ BarterTreeNode* head;
  /* 0004 */ std::size_t size;

  void EraseSubtree(BarterTree* owner, BarterTreeNode* node) noexcept;
  void Clear() noexcept;
  ~BarterTree();
};


struct BarterTreeIterator {
  /* 0000 */ BarterTreeNode* node;

  BarterTreeIterator& operator++();
};

// GLOBAL: gl_24814
CBarterMarketItemMgr* g_barterMarketItemMgr;

// OFFSET: 0x005D7020
// FUNCTION: fn_03719
BarterMarketItem::BarterMarketItem(const BarterMarketItem& other) = default;

// OFFSET: 0x005D70C0
// FUNCTION: fn_46485
CBarterMarketItemMgr::CBarterMarketItemMgr()
{
  this->initialized = true;
}

// OFFSET: 0x005D75D0
// FUNCTION: fn_46450
__declspec(noinline) CBarterMarketItemMgr::~CBarterMarketItemMgr()
{
}

// OFFSET: 0x005D7840
// FUNCTION: fn_03723
void CBarterMarketItemMgr::RefreshTotals()
{
  this->totals.clear();
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    std::map<int, int>::iterator found = this->totals.find(it->currencyItemId);
    int total = it->currencyAmount * it->value;
    if (found != this->totals.end()) {
      found->second += total;
    } else {
      this->totals.insert(std::make_pair(it->currencyItemId, total));
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
      if (!g_barterMarketItemMgr->initialized) {
        delete g_barterMarketItemMgr;
        g_barterMarketItemMgr = NULL;
        return false;
      }
      return true;
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

// OFFSET: 0x005D7790
// FUNCTION: fn_46622
void CBarterMarketItemMgr::AddPurchaseQuantity(const BarterMarketItem& item)
{
  std::list<BarterMarketItem>::iterator found = std::find_if(this->allItems.begin(), this->allItems.end(), [item](const BarterMarketItem& entry) {
    return entry.id == item.id;
  });
  if (found != this->allItems.end()) {
    found->value += item.value;
  } else {
    this->allItems.push_back(item);
  }
}

// OFFSET: 0x005D7AE0
// FUNCTION: fn_46623
void CBarterMarketItemMgr::TakeStockQuantity(const BarterMarketItem& item)
{
  const auto itemsEnd = this->items.end();
  std::list<BarterMarketItem>::iterator it = std::find_if(this->items.begin(), itemsEnd, [&item](const BarterMarketItem& entry) {
    return entry.id == item.id;
  });
  if (it != itemsEnd && !it->flag_38) {
    it->value -= item.value;
    if (it->value <= 0) {
      this->items.erase(it);
      UIWindow* window = g_windowMgr.LookupWindow(WID_BARTERMARKETWND);
      if (window != NULL) {
        window->SendMsg(NULL, UM_REFRESHITEM, 0, 0, 0, 0);
      }
    }
  }
}

// OFFSET: 0x005D7B60
// FUNCTION: fn_46624
void CBarterMarketItemMgr::RemovePurchaseQuantity(const BarterMarketItem& item)
{
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    if (it->id == item.id) {
      it->value -= item.value;
      if (it->value <= 0) {
        this->allItems.erase(it);
        UIWindow* window = g_windowMgr.LookupWindow(WID_BARTERITEMPURCHASEWND);
        if (window != NULL) {
          window->SendMsg(NULL, UM_REFRESHITEM, 0, 0, 0, 0);
        }
      }
      break;
    }
  }
}

// OFFSET: 0x005D76E0
// FUNCTION: fn_46621
void CBarterMarketItemMgr::RestoreStockQuantity(const BarterMarketItem& item)
{
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
}

// OFFSET: 0x005D7C10
// FUNCTION: fn_03727
void CBarterMarketItemMgr::AddItem(const BarterMarketItem& item)
{
  AddPurchaseQuantity(item);
  TakeStockQuantity(item);
  this->RefreshTotals();
}

// OFFSET: 0x005D7D10
// FUNCTION: fn_03728
void CBarterMarketItemMgr::RemoveItem(const BarterMarketItem& item)
{
  RemovePurchaseQuantity(item);
  RestoreStockQuantity(item);
  this->RefreshTotals();
}

// OFFSET: 0x005D7E40
// FUNCTION: fn_03729
bool CBarterMarketItemMgr::HasItemByName(std::string name)
{
  std::list<BarterMarketItem>::iterator found = std::find_if(this->allItems.begin(), this->allItems.end(), [name](const BarterMarketItem& item) {
    return item.name == name;
  });
  if (found != this->allItems.end()) {
    return true;
  }
  return false;
}

// OFFSET: 0x005D6F60
// FUNCTION: fn_03717
void BarterTree::EraseSubtree(BarterTree* owner, BarterTreeNode* root) noexcept
{
  BarterTreeNode* node = root;
  while (node->isnil == 0) {
    EraseSubtree(owner, node->right);
    BarterTreeNode* erased = node;
    node = node->left;
    std::allocator<BarterTreeNode>().deallocate(erased, 1);
  }
}

// OFFSET: 0x005D7390
// FUNCTION: fn_03720
BarterTree::~BarterTree()
{
  EraseSubtree(this, this->head->parent);
  std::allocator<BarterTreeNode>().deallocate(this->head, 1);
}

// OFFSET: 0x005D84F0
// FUNCTION: fn_03743
void BarterTree::Clear() noexcept
{
  BarterTreeNode* head = this->head;
  EraseSubtree(this, head->parent);
  head->parent = head;
  head->left = head;
  head->right = head;
  this->size = 0;
}

// OFFSET: 0x005D9630
// FUNCTION: fn_03756
BarterTreeIterator& BarterTreeIterator::operator++()
{
  if (this->node->right->isnil != 0) {
    BarterTreeNode* parent;
    while ((parent = this->node->parent)->isnil == 0 && this->node == parent->right) {
      this->node = parent;
    }
    this->node = parent;
  } else {
    BarterTreeNode* child = this->node->right;
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
      return std::make_pair(true, *it);
    }
  }
  return std::make_pair(false, BarterMarketItem());
}

// OFFSET: 0x005D8050
// FUNCTION: fn_03733
std::pair<bool, BarterMarketItem> CBarterMarketItemMgr::FindAllItem(int id)
{
  for (std::list<BarterMarketItem>::iterator it = this->allItems.begin(); it != this->allItems.end(); ++it) {
    if (it->id == id) {
      return std::make_pair(true, *it);
    }
  }
  return std::make_pair(false, BarterMarketItem());
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
  const auto addValue = [&total, &name](const BarterMarketItem& item) {
    if (item.name == name) {
      total += item.value;
    }
  };
  for (const BarterMarketItem& item : this->allItems) {
    addValue(item);
  }
  return total;
}

// OFFSET: 0x005D8250
// FUNCTION: fn_03738
int CBarterMarketItemMgr::GetAmountById(int id)
{
  for (std::list<BarterMarketItem>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (id == it->id) {
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

// GLOBAL: gl_24815
CExpandedBarterMarketItemMgr* g_expandedBarterMarketItemMgr;

// OFFSET: 0x005D8EB0
// FUNCTION: fn_46453
CExpandedBarterMarketItemMgr::CExpandedBarterMarketItemMgr() : total(0)
{
  this->initialized = true;
}

// OFFSET: 0x005D9590
// FUNCTION: fn_46454
__declspec(noinline) CExpandedBarterMarketItemMgr::~CExpandedBarterMarketItemMgr()
{
}

// OFFSET: 0x005D9B00
// FUNCTION: fn_03759
bool CExpandedBarterMarketItemMgr::CreateInstance()
{
  if (g_expandedBarterMarketItemMgr == NULL) {
    g_expandedBarterMarketItemMgr = new (std::nothrow) CExpandedBarterMarketItemMgr();
    if (g_expandedBarterMarketItemMgr != NULL) {
      if (!g_expandedBarterMarketItemMgr->initialized) {
        delete g_expandedBarterMarketItemMgr;
        g_expandedBarterMarketItemMgr = NULL;
        return false;
      }
      return true;
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

// OFFSET: 0x005D9CA0
// FUNCTION: fn_46628
void CExpandedBarterMarketItemMgr::RemoveEntryQuantity(const BarterMarketEntry& entry)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    if (entry.id == it->id) {
      it->value -= entry.value;
      if (it->value <= 0) {
        this->entries.erase(it);
      }
      break;
    }
  }
}

// OFFSET: 0x005D9C50
// FUNCTION: fn_46627
void CExpandedBarterMarketItemMgr::TakeItemQuantity(const BarterMarketEntry& entry)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (entry.id == it->id && !it->flag_30) {
      it->value -= entry.value;
      if (it->value < 0) {
        it->value = 0;
      }
      break;
    }
  }
}

// OFFSET: 0x005D9DA0
// FUNCTION: fn_46629
void CExpandedBarterMarketItemMgr::RemoveChunkQuantity(BarterChunk16 chunk, int count)
{
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

// OFFSET: 0x005D9810
// FUNCTION: fn_46625
void CExpandedBarterMarketItemMgr::AddChunkQuantity(BarterChunk16 chunk, int count)
{
  for (std::vector<BarterChunk16>::iterator target = this->chunks.begin();; ++target) {
    if (target == this->chunks.end()) {
      BarterChunk16 added;
      added.field_0 = chunk.field_0;
      added.field_4 = chunk.field_4;
      added.amount = chunk.amount * count;
      added.field_c = chunk.field_c;
      this->chunks.push_back(added);
      break;
    }
    if (target->field_0 == chunk.field_0 && target->field_4 == chunk.field_4) {
      target->amount += chunk.amount * count;
      break;
    }
  }
}

// OFFSET: 0x005D98A0
// FUNCTION: fn_46626
void CExpandedBarterMarketItemMgr::RecalculateTotal()
{
  this->total = 0;
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    this->total += it->field_8 * it->value;
  }
}

// OFFSET: 0x005D9E30
// FUNCTION: fn_03761
void CExpandedBarterMarketItemMgr::RemoveEntry(const BarterMarketEntry& entry)
{
  RemoveEntryQuantity(entry);
  std::vector<BarterMarketEntry>::iterator found = this->items.begin();
  for (; found != this->items.end(); ++found) {
    if (entry.id == found->id) {
      break;
    }
  }
  if (found != this->items.end()) {
    if (!found->flag_30) {
      found->value += entry.value;
    }
  } else {
    this->items.push_back(entry);
  }
  for (auto it = entry.chunks.begin(); it != entry.chunks.end(); ++it) {
    int count = entry.value;
    alignas(16) BarterChunk16 chunk = *it;
    RemoveChunkQuantity(chunk, count);
  }
  RecalculateTotal();
}

// OFFSET: 0x005DA040
// FUNCTION: fn_03762
void CExpandedBarterMarketItemMgr::AddEntry(const BarterMarketEntry& entry)
{
  std::vector<BarterMarketEntry>::iterator found = this->entries.begin();
  for (; found != this->entries.end(); ++found) {
    if (entry.id == found->id) {
      break;
    }
  }
  if (found == this->entries.end()) {
    this->entries.push_back(entry);
  } else {
    found->value += entry.value;
  }
  TakeItemQuantity(entry);
  for (auto it = entry.chunks.begin(); it != entry.chunks.end(); ++it) {
    int count = entry.value;
    alignas(16) BarterChunk16 chunk = *it;
    AddChunkQuantity(chunk, count);
  }
  RecalculateTotal();
}

// OFFSET: 0x005DA1C0
// FUNCTION: fn_03763
bool CExpandedBarterMarketItemMgr::HasEntryByName(std::string name)
{
  bool found = false;
  std::find_if(this->entries.begin(), this->entries.end(), [name, &found](BarterMarketEntry entry) {
    const bool equal = name == entry.name;
    found = equal;
    return equal;
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
    if (id == it->id) {
      return std::make_pair(true, *it);
    }
  }
  return std::make_pair(false, BarterMarketEntry());
}

// OFFSET: 0x005DA4E0
// FUNCTION: fn_03767
std::pair<bool, BarterMarketEntry> CExpandedBarterMarketItemMgr::FindEntry(int id)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it) {
    if (id == it->id) {
      return std::make_pair(true, *it);
    }
  }
  return std::make_pair(false, BarterMarketEntry());
}

// OFFSET: 0x005DA3B0
// FUNCTION: fn_03765
int CExpandedBarterMarketItemMgr::GetItemValueById(int id)
{
  for (std::vector<BarterMarketEntry>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    if (id == it->id) {
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
    if (id == it->id) {
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
    if (id == it->id) {
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
  entry.location = source.location;
  entry.viewSprite = source.viewSprite;
  entry.flag_24 = true;
  entry.refine = source.refine;
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
    BarterEntryPair saved;
    saved.id = mid->id;
    saved.value = mid->value;
    *mid = *first;
    *first = saved;
  }
  int lastId = last->id;
  if (lastId < mid->id) {
    BarterEntryPair saved;
    saved.id = last->id;
    saved.value = last->value;
    *last = *mid;
    *mid = saved;
    if (mid->id < first->id) {
      BarterEntryPair saved2;
      saved2.id = mid->id;
      saved2.value = mid->value;
      *mid = *first;
      *first = saved2;
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
  std::allocator<BarterMarketEntry>().deallocate(entries, count);
}

struct BarterChunkVector {
  BarterChunk16* Umove(BarterChunk16* first, BarterChunk16* last, BarterChunk16* dest);
  BarterChunk16* MoveChunks16(BarterChunk16* first, BarterChunk16* last, BarterChunk16* dest);
};

// OFFSET: 0x005DB160
// FUNCTION: fn_03784
BarterChunk16* BarterChunkVector::Umove(BarterChunk16* first, BarterChunk16* last, BarterChunk16* dest)
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
BarterChunk16* BarterChunkVector::MoveChunks16(BarterChunk16* first, BarterChunk16* last, BarterChunk16* dest)
{
  BarterChunk16* src = first;
  BarterChunk16* dst = dest;
  while (src != last) {
    *dst = *src;
    ++src;
    ++dst;
  }
  return src;
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
  BarterTriple* src = first;
  BarterTriple* dst = dest;
  while (src != last) {
    const int secondValue = src->second;
    const int firstValue = src->first;
    dst->first = firstValue;
    dst->second = secondValue;
    dst->third = src->third;
    ++src;
    ++dst;
  }
  return dst;
}

struct BarterChunkAllocator {
  void Deallocate(BarterChunk16* chunks, std::size_t count);
};

// OFFSET: 0x005DB300
// FUNCTION: fn_03791
void BarterChunkAllocator::Deallocate(BarterChunk16* chunks, std::size_t count)
{
  std::allocator<BarterChunk16>().deallocate(chunks, count);
}

// OFFSET: 0x005DBD40
// FUNCTION: fn_03800
void AdjustHeap(BarterEntryPair* first, int hole, int bottom, BarterEntryPair* value)
{
  const int top = hole;
  int idx = hole;
  const int maxNonLeaf = (bottom - 1) >> 1;
  while (idx < maxNonLeaf) {
    idx = 2 * idx + 2;
    if (first[idx].id < first[idx - 1].id) {
      --idx;
    }
    first[hole] = first[idx];
    hole = idx;
  }
  if (idx == maxNonLeaf && bottom % 2 == 0) {
    first[hole] = first[bottom - 1];
    hole = bottom - 1;
  }
  for (idx = (hole - 1) >> 1; top < hole && first[idx].id < value->id; idx = (hole - 1) >> 1) {
    first[hole] = first[idx];
    hole = idx;
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
  BarterKeyNode* node = this->head->parent;
  BarterKeyLookup result;
  result.node = node;
  result.direction = 0;
  result.bound = this->head;
  while (node->isnil == 0) {
    result.node = node;
    int i = 0;
    int nodeCount = (int)node->key.size();
    int keyCount = (int)key.size();
    int count = std::max(nodeCount, keyCount);
    int relation = 0;
    for (; i < count; ++i) {
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

// OFFSET: 0x005D98E0
// FUNCTION: fn_03758
void CExpandedBarterMarketItemMgr::LoadInventoryItems()
{
  this->itemInfos.clear();
  const int count = g_session.GetNumInventoryItems();
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = g_session.GetInventoryItemAt(i);
    if (item.identified) {
      this->itemInfos.insert({item.refiningLevel, item});
    }
  }
}

// OFFSET: 0x005DA6D0
// FUNCTION: fn_03774
bool CExpandedBarterMarketItemMgr::HasRequiredItems()
{
  this->LoadInventoryItems();
  for (std::vector<BarterChunk16>::iterator chunk = this->chunks.begin(); chunk != this->chunks.end(); ++chunk) {
    int required = chunk->amount;
    std::multimap<int, ITEM_INFO>::iterator it = this->itemInfos.begin();
    bool found = false;
    while (it != this->itemInfos.end()) {
      if (std::atoi(it->second.itemId.c_str()) == chunk->field_0 && (chunk->field_4 <= 0 || it->second.refiningLevel >= chunk->field_4)) {
        const int available = it->second.quantity;
        if (available > required) {
          it->second.quantity = available - required;
          found = true;
          break;
        }
        if (available == required) {
          const std::multimap<int, ITEM_INFO>::const_iterator used = it;
          this->itemInfos.erase(used);
          found = true;
          break;
        }
        required -= available;
        it = this->itemInfos.erase(it);
      } else {
        ++it;
      }
    }

    if (!found) {
      return false;
    }
  }
  return true;
}
