#ifndef RAGNAROK_SOURCE_UI_EDIT_ITEMTAGRETIREMENT_H
#define RAGNAROK_SOURCE_UI_EDIT_ITEMTAGRETIREMENT_H

#include <array>
#include <cstddef>
#include <list>

class UIWindow;
struct InputFocus;

struct ItemTagRecord {
  /* 0000 */ UIWindow* child;
  /* 0004 */ std::array<std::byte, 64> unknown_0004;
  /* 0044 */ char name[384];
  /* 01C4 */ int index;
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

struct MailItemTagRemoval final : ItemTagRemoval {
  explicit MailItemTagRemoval(UIWindow* const& owner);
  void Remove(UIWindow* child) override;

private:
  UIWindow* const& m_owner;
};

struct MegaPhoneItemTagRemoval final : ItemTagRemoval {
  explicit MegaPhoneItemTagRemoval(UIWindow* const& owner);
  void Remove(UIWindow* child) override;

private:
  UIWindow* const& m_owner;
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
static_assert(offsetof(ItemTagRecord, name) == 0x0044);
static_assert(offsetof(ItemTagRecord, index) == 0x01C4);
#endif

#endif
