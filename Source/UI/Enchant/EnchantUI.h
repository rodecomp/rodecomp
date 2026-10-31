#ifndef RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H
#define RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H

#include "Contents/Items/ItemInfo.h"
#include "UI/FrameSkin.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <list>
#include <map>
#include <optional>

class CUIRadioButton;
class CUIStaticText;
class CUITextViewer;
class CUITab;
struct EnchantInfo;
struct EnchantOptionInfo;

namespace EnchantUI {
class CUITargetItem;
class CUIMainPage;
class CUIReset;
}

class CUIEnchantUI : public CUIFrameWnd {
public:
  /* 00D8 */ CUIControl* background;
  /* 00DC */ CUIControl* titleBar;
  /* 00E0 */ CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>* targetList;
  /* 00E4 */ CUITab* tab;
  /* 00E8 */ EnchantUI::CUIMainPage* normalPage;
  /* 00EC */ EnchantUI::CUIMainPage* upgradePage;
  /* 00F0 */ EnchantUI::CUIReset* resetPage;
  /* 00F4 */ CUITextViewer* textViewer;
  /* 00F8 */ uint64_t group;
  /* 0100 */ EnchantInfo* enchantInfo;
  /* 0104 */ int unknown_0104;

  // INLINED
  CUIEnchantUI()
      : background(nullptr), titleBar(nullptr), targetList(nullptr), tab(nullptr), normalPage(nullptr), upgradePage(nullptr), resetPage(nullptr),
        textViewer(nullptr), group(0), enchantInfo(nullptr) {
  }
  ~CUIEnchantUI() override;
  bool CanHitTest() override;
  void Slot16() override;
  void Slot25(int x, int y) override;
  virtual void Slot53();
  void sub_A06C40(uint64_t group);
};

namespace EnchantUI {
class CEnchantItemData;
class CUISlot;

class CUIEnchantItem : public CUIListItem {
public:
  /* 00E8 */ CUIGameItem* itemIcon;
  /* 00EC */ CUIStaticText* name;

  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void SetFrozen(bool frozen) override;
};

class CUITargetItem : public CUIListItem {
public:
  /* 00E8 */ CUIGameItem* itemIcon;
  /* 00EC */ CUIStaticImage* unknown_00EC;
  /* 00F0 */ CUIStaticText* unknown_00F0;

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

#pragma pack(push, 4)
class CUIEnchantPage : public CUIControl {
public:
  explicit CUIEnchantPage(UIWindowContext& context);

  /* 0080 */ unsigned char unknown_0080[0x4];
  /* 0084 */ CUIGameItem* gameItem;
  /* 0088 */ std::map<int, CUISlot*> unknown_0088;
  /* 0090 */ std::list<CUIIngredient*> unknown_0090;
  /* 0098 */ CUIListBoxEx<CUIEnchantItem, CEnchantItemData>* enchantList;
  /* 009C */ CUIStaticText* unknown_009C;
  /* 00A0 */ CUIButton* unknown_00A0 = nullptr;
  /* 00A4 */ EnchantInfo* enchantInfo;
  /* 00A8 */ uint64_t group;
  /* 00B0 */ int unknown_00B0;

  void Slot16() override;
  virtual void Slot54(int argument1, int argument2);
  virtual void Slot55() = 0;
  virtual bool Slot56() = 0;
  virtual void Slot57();
  virtual void Slot58(int value);
  virtual void Slot59(int value);

  __declspec(noinline) CUIIngredient* CreateIngredient();
  void ResetSlots();
  __declspec(noinline) int FindEmptySlot();
  std::optional<EnchantOptionInfo*> FindOption(uint32_t key);
};
#pragma pack(pop)

class CUINormalRandom : public CUIEnchantPage {
public:
  void Slot16() override;
  void Slot55() override;
};

class CUINormalPerfect : public CUIEnchantPage {
public:
  void Slot16() override;
  void Slot55() override;
  bool Slot56() override;
};

class CUIReset : public CUIEnchantPage {
public:
  void Slot16() override;
  void Slot55() override;
  void Slot57() override;
};

class CUIUpgradeEnchantPage : public CUIEnchantPage {
public:
  void Slot58(int value) override;
};

class CUIUpgradeRandom : public CUIUpgradeEnchantPage {
public:
  void Slot16() override;
  void Slot55() override;
};

class CUIUpgradePerfect : public CUIUpgradeEnchantPage {
public:
  /* 00B8 */ bool unknown_00B8;

  void Slot55() override;
};

class CUIMainPage : public CUIControl {
public:
  explicit CUIMainPage(UIWindowContext& context);

  /* 0080 */ CUIEnchantPage* firstPage;
  /* 0084 */ CUIEnchantPage* secondPage;
  /* 0088 */ CUITab* tab;

  void Slot16() override;
};

class CEnchantItemData {
public:
  /* 0000 */ int itemId;
  /* 0004 */ bool frozen;
};

class CUISlot : public CUIControl {
public:
  /* 0080 */ CUIRadioButton* radioButton;
  /* 0084 */ CUIGameItem* itemIcon;
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
