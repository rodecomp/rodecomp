#ifndef RAGNAROK_SOURCE_UI_EDIT_ITEMTAGRETIREMENT_H
#define RAGNAROK_SOURCE_UI_EDIT_ITEMTAGRETIREMENT_H

#include <array>
#include <cstddef>
#include <list>

class UIWindow;
struct InputFocus;

struct ItemTagRecord {
  /* 0000 */ UIWindow* child;
  /* 0004 */ std::array<std::byte, 452> unknown_0004;
};

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
static_assert(offsetof(ItemTagRecord, child) == 0x0000);
static_assert(offsetof(ItemTagRecord, unknown_0004) == 0x0004);
#endif

#endif
