#ifndef RAGNAROK_SOURCE_UI_UICHANGEMATERIALWND_H
#define RAGNAROK_SOURCE_UI_UICHANGEMATERIALWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

class UIButton;
struct UIButtonContext;
struct UIEditContext;

struct MaterialSlotPos {
  /* 0000 */ int x;
  /* 0004 */ int y;
};

struct MaterialReq {
  /* 0000 */ int id;
  /* 0004 */ int quantity;
};

struct MaterialResult {
  /* 0000 */ int id;
  /* 0004 */ int quantity;
  /* 0008 */ int amount;
};

struct MaterialListEntry {
  /* 0000 */ unsigned char state;
  /* 0004 */ std::vector<MaterialReq> materials;
  /* 0010 */ std::vector<MaterialResult> results;
};

class UIChangeMaterialWnd_ScrollBar : public UIScrollBar {
public:
  // VTABLE: vt_00211
  // INLINED
  explicit UIChangeMaterialWnd_ScrollBar(UIWindowContext& windows) : UIScrollBar(windows) {}

  void Paint() override;
};

class UIChangeMaterialWnd_CheckBox : public UICheckBox {
public:
  // INLINED
  UIChangeMaterialWnd_CheckBox(UIWindowContext& windows, UIButtonContext& controls) : UICheckBox(windows, controls) {}

  /* 010C */ std::string ascendingText;
  /* 0124 */ std::string descendingText;
};

class UIChangeMaterialWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  bool Slot02() override;
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  ITEM_INFO* GetItemAt(int x, int y);
  void sub_5E39A0();
  void UpdateSearchPlaceholder();
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4)
    override;

  /* 00BC */ UIChangeMaterialWnd_CheckBox* m_sortBox;
  /* 00C0 */ UIButton* m_button;
  /* 00C4 */ int m_resultX;
  /* 00C8 */ int m_resultY;
  /* 00D0 */ ITEM_INFO m_resultItem;
  /* 01C8 */ MaterialSlotPos m_materialPos[6];
  /* 01F8 */ std::vector<ITEM_INFO> m_materials;
  /* 0204 */ int m_resultAmount;
  /* 0208 */ std::string m_resultText;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
};

class UIChangeMaterialWnd_List : public UIFrameWnd {
public:
  // INLINED
  UIChangeMaterialWnd_List(UIWindowContext& windows, UIFrameContext& frames)
      : UIFrameWnd(windows, frames, 0), m_selectedIndex(0), m_hoverIndex(0), m_scrollOffset(0), m_previousScrollOffset(0), m_scrollBar(nullptr), m_items(), m_entries() {
    *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0xB4) = 27;
    *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0xB8) = 16;
  }

  void Paint() override;
  void Move(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot35(int argument) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4)
    override;

  void ApplyFilter(std::string text);
  void SortItems(bool ascending);
  void RefreshVisibleEntries();
  void sub_5E2A70();
  void sub_5E3B80(int index);
  void sub_5E2590(int unknown, int position);
  void sub_5E1080(int delta);
  void ShiftSelectedIndex(int value);
  void CreateScrollBar(int width, int height);

  /* 00BC */ int m_pageSize;
  /* 00C0 */ int64_t m_selectedIndex;
  /* 00C8 */ int64_t m_hoverIndex;
  /* 00D0 */ int m_scrollOffset;
  /* 00D4 */ int m_previousScrollOffset;
  /* 00D8 */ UIScrollBar* m_scrollBar;
  /* 00DC */ std::vector<int> m_items;
  /* 00E8 */ std::vector<MaterialListEntry> m_entries;
  /* 00F4 */ std::uint32_t unknown_00F4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIChangeMaterialWnd, m_resultX) == 0xC4);
static_assert(offsetof(UIChangeMaterialWnd, m_resultItem) == 0xD0);
static_assert(offsetof(UIChangeMaterialWnd, m_materialPos) == 0x1C8);
static_assert(offsetof(UIChangeMaterialWnd, m_materials) == 0x1F8);
static_assert(offsetof(UIChangeMaterialWnd, m_resultAmount) == 0x204);
static_assert(offsetof(UIChangeMaterialWnd, m_resultText) == 0x208);
static_assert(sizeof(UIChangeMaterialWnd_List) == 0xF8);
static_assert(offsetof(UIChangeMaterialWnd_List, m_pageSize) == 0xBC);
static_assert(offsetof(UIChangeMaterialWnd_List, m_selectedIndex) == 0xC0);
static_assert(offsetof(UIChangeMaterialWnd_List, m_hoverIndex) == 0xC8);
static_assert(offsetof(UIChangeMaterialWnd_List, m_scrollOffset) == 0xD0);
static_assert(offsetof(UIChangeMaterialWnd_List, m_previousScrollOffset) == 0xD4);
static_assert(offsetof(UIChangeMaterialWnd_List, m_items) == 0xDC);
static_assert(offsetof(UIChangeMaterialWnd_List, m_entries) == 0xE8);
static_assert(sizeof(MaterialListEntry) == 0x1C);
static_assert(offsetof(MaterialListEntry, materials) == 0x4);
static_assert(offsetof(MaterialListEntry, results) == 0x10);
static_assert(sizeof(MaterialResult) == 0xC);
static_assert(sizeof(MaterialReq) == 0x8);
#endif

#endif
