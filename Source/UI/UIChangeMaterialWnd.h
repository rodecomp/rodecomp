#ifndef RAGNAROK_SOURCE_UI_UICHANGEMATERIALWND_H
#define RAGNAROK_SOURCE_UI_UICHANGEMATERIALWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <vector>

struct MaterialSlotPos {
  /* 0000 */ int x;
  /* 0004 */ int y;
};

class UIChangeMaterialWnd : public UIFrameWnd {
public:
  bool Slot02() override;
  ITEM_INFO* GetItemAt(int x, int y);

  /* 00BC */ std::uint32_t unknown_00BC;
  /* 00C0 */ std::uint32_t unknown_00C0;
  /* 00C4 */ int m_resultX;
  /* 00C8 */ int m_resultY;
  /* 00D0 */ ITEM_INFO m_resultItem;
  /* 01C8 */ MaterialSlotPos m_materialPos[6];
  /* 01F8 */ std::vector<ITEM_INFO> m_materials;
};

class UIChangeMaterialWnd_List : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot35(int argument) override;

  void sub_5E2A70();
  void sub_5E3B80(int index);
  void sub_5E1080(int delta);
  void ShiftSelectedIndex(int value);

  /* 00BC */ int m_pageSize;
  /* 00C0 */ int64_t m_selectedIndex;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int m_scrollOffset;
  /* 00D4 */ int m_previousScrollOffset;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ std::vector<int> m_items;
  /* 00E8 */ unsigned char unknown_00E8[0x10];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIChangeMaterialWnd, m_resultX) == 0xC4);
static_assert(offsetof(UIChangeMaterialWnd, m_resultItem) == 0xD0);
static_assert(offsetof(UIChangeMaterialWnd, m_materialPos) == 0x1C8);
static_assert(offsetof(UIChangeMaterialWnd, m_materials) == 0x1F8);
static_assert(sizeof(UIChangeMaterialWnd_List) == 0xF8);
static_assert(offsetof(UIChangeMaterialWnd_List, m_pageSize) == 0xBC);
static_assert(offsetof(UIChangeMaterialWnd_List, m_selectedIndex) == 0xC0);
static_assert(offsetof(UIChangeMaterialWnd_List, m_scrollOffset) == 0xD0);
static_assert(offsetof(UIChangeMaterialWnd_List, m_previousScrollOffset) == 0xD4);
static_assert(offsetof(UIChangeMaterialWnd_List, m_items) == 0xDC);
#endif

#endif
