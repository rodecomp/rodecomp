#ifndef RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H
#define RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H

#include "Contents/Items/ItemInfo.h"
#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"

class CUITextViewer;

class CUIEnchantUI : public CUIFrameWnd {
public:
  /* 00D8 */ CUIControl* background;
  /* 00DC */ CUIControl* titleBar;

  void Slot16() override;
  void Slot25(int x, int y) override;
};

namespace EnchantUI {
class CUIEnchantItem : public CUIListItem {
public:
  /* 00E8 */ CUIControl* itemIcon;

  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
};

class CUITargetItem : public CUIListItem {
public:
  /* 00E8 */ CUIControl* itemIcon;
  /* 00EC */ CUIControl* unknown_00EC;
  /* 00F0 */ CUIControl* unknown_00F0;

  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void SetSelected(bool selected) override;
};

class CEnchantItemData {
public:
  /* 0000 */ int itemId;
  /* 0004 */ bool frozen;
};

class CUISlot : public CUIControl {
public:
  /* 0080 */ CUIControl* radioButton;
  /* 0084 */ CUIControl* itemIcon;
  /* 0088 */ CUITextViewer* description;

  void Slot16() override;
  void Paint() override;
  void Slot32(int x, int y) override;
};

class CUISlot0 : public CUISlot {
public:
  void Slot16() override;
};

class CUISlot1 : public CUISlot {
public:
  void Slot16() override;
};

class CUISlot2 : public CUISlot {
public:
  void Slot16() override;
};

class CUISlot3 : public CUISlot {
public:
  void Slot16() override;
};
}

#endif
