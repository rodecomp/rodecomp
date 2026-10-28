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
  ItemInfo* GetItemAt(int x, int y);

  /* 00BC */ std::uint32_t unknown_00BC;
  /* 00C0 */ std::uint32_t unknown_00C0;
  /* 00C4 */ int m_resultX;
  /* 00C8 */ int m_resultY;
  /* 00D0 */ ItemInfo m_resultItem;
  /* 01C8 */ MaterialSlotPos m_materialPos[6];
  /* 01F8 */ std::vector<ItemInfo> m_materials;
};

class UIChangeMaterialWnd_List : public UIFrameWnd {
public:
  void Move(int x, int y) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIChangeMaterialWnd, m_resultX) == 0xC4);
static_assert(offsetof(UIChangeMaterialWnd, m_resultItem) == 0xD0);
static_assert(offsetof(UIChangeMaterialWnd, m_materialPos) == 0x1C8);
static_assert(offsetof(UIChangeMaterialWnd, m_materials) == 0x1F8);
#endif

#endif
