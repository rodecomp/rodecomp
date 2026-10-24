#ifndef RAGNAROK_SOURCE_UI_ITEMTAGRETIREMENT_H
#define RAGNAROK_SOURCE_UI_ITEMTAGRETIREMENT_H

#include <array>
#include <cstddef>
#include <list>

class UIWindow;
struct InputFocus;

// UIItemTagCtrl list-node payload is 456 bytes on x86; the child pointer is
// first. The remaining inline bytes are copied as 114 DWORDs during parsing.
// Their interpretation is not required by retirement and remains unrecovered.
struct ItemTagRecord {
  UIWindow* child;
  std::array<std::byte, 452> unknown_0004;
};

// UIItemTagCtrl virtual slot 60. Derived mail/chat variants choose a different
// removal owner; clearing must retain that dispatch and the current list node.
class ItemTagRemoval {
public:
  virtual void Remove(UIWindow* child) = 0;

protected:
  ~ItemTagRemoval() = default;
};

struct FocusedItemTagRemoval final : ItemTagRemoval {
  explicit FocusedItemTagRemoval(InputFocus& focus);
  void Remove(UIWindow* child) override;

private:
  InputFocus& m_focus;
};

struct ItemTagRetirement {
  std::list<ItemTagRecord>& records;
  ItemTagRemoval& removal;

  void Clear();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ItemTagRecord) == 456);
#endif

#endif
