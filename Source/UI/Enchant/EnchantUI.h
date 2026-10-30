#ifndef RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H
#define RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H

#include "Contents/Items/ItemInfo.h"
#include "UI/FrameSkin.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <optional>

class CUITextViewer;
struct EnchantInfo;
struct EnchantOptionInfo;

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
  /* 00E8 */ CUIGameItem* itemIcon;
  /* 00EC */ CUIControl* unknown_00EC;
  /* 00F0 */ CUIControl* unknown_00F0;

  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void SetSelected(bool selected) override;

  ITEM_INFO* GetItemInfo();
};

class CUIIngredient : public CUIControl {
public:
  explicit CUIIngredient(UIWindowContext& context)
      : CUIControl(context), unknown_0080(nullptr), unknown_0084(nullptr), unknown_0088(nullptr), unknown_008C(nullptr), unknown_0090(nullptr) {
  }

  /* 0080 */ CUIControl* unknown_0080;
  /* 0084 */ CUIControl* unknown_0084;
  /* 0088 */ CUIControl* unknown_0088;
  /* 008C */ CUIControl* unknown_008C;
  /* 0090 */ CUIControl* unknown_0090;
};

class CUIEnchantPage : public CUIControl {
public:
  /* 0080 */ unsigned char unknown_0080[0x4];
  /* 0084 */ CUIGameItem* gameItem;
  /* 0088 */ unsigned char unknown_0088[0x1C];
  /* 00A4 */ EnchantInfo* enchantInfo;
  /* 00A8 */ unsigned char unknown_00A8[0x8];
  /* 00B0 */ int unknown_00B0;

  virtual void Slot54(int argument1, int argument2);
  virtual void Slot55() = 0;
  virtual void Slot56() = 0;
  virtual void Slot57() = 0;
  virtual void Slot58(int value) {
  }
  virtual void Slot59(int value);

  CUIIngredient* CreateIngredient();
  int FindEmptySlot();
  std::optional<EnchantOptionInfo*> FindOption(uint32_t key);
};

class CUIUpgradeEnchantPage : public CUIEnchantPage {
public:
  void Slot58(int value) override;
};

class CUIMainPage : public CUIControl {
public:
  /* 0080 */ CUIEnchantPage* firstPage;
  /* 0084 */ CUIEnchantPage* secondPage;
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

  void SelectSlot(UIWindow* button);
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
