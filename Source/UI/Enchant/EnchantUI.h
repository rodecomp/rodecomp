#ifndef RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H
#define RAGNAROK_SOURCE_UI_ENCHANT_ENCHANTUI_H

#include "Contents/Items/ItemInfo.h"
#include "UI/FrameSkin.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIWindowMgr.h"

#include <list>
#include <map>
#include <optional>

class CUIRadioButton;
class CUIStaticText;
class CUITextViewer;
class CUITab;
struct EnchantInfo;
struct EnchantOptionInfo;
struct EnchantPerfectInfo;

namespace EnchantUI {
class CUITargetItem;
class CUIMainPage;
class CUIReset;
}

class CUIEnchantUI : public CUIFrameWnd {
public:
  /* 00D8 */ CUIStaticImage* background;
  /* 00DC */ CUITitleBar* titleBar;
  /* 00E0 */ CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>* targetList;
  /* 00E4 */ CUITab* tab;
  /* 00E8 */ EnchantUI::CUIMainPage* normalPage;
  /* 00EC */ EnchantUI::CUIMainPage* upgradePage;
  /* 00F0 */ EnchantUI::CUIReset* resetPage;
  /* 00F4 */ CUITextViewer* textViewer;
  /* 00F8 */ uint64_t group;
  /* 0100 */ EnchantInfo* enchantInfo;
  /* 0104 */ int padding_0104;

  // INLINED
  CUIEnchantUI()
      : background(nullptr), titleBar(nullptr), targetList(nullptr), tab(nullptr), normalPage(nullptr), upgradePage(nullptr), resetPage(nullptr),
        textViewer(nullptr), group(0), enchantInfo(nullptr) {
  }
  ~CUIEnchantUI() override;
  bool CanHitTest() override;
  __declspec(noinline) void OnCreate(int width, int height) override;
  void OnInit() override;
  void OnLBtnDown(int x, int y) override;
  virtual void Initialize();
  void SetEnchantGroup(uint64_t group);
  void OnSelectTarget(EnchantUI::CUITargetItem* item, const ITEM_INFO& info);
  void OnClickClose();
};

namespace EnchantUI {
class CEnchantItemData;
class CUISlot;

class CUIEnchantItem : public CUIListItem {
public:
  // INLINED
  CUIEnchantItem() : itemIcon(nullptr), name(nullptr) {
  }

  /* 00E8 */ CUIGameItem* itemIcon;
  /* 00EC */ CUIStaticText* name;

  void OnCreate(int width, int height) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void SetFrozen(bool frozen) override;
};

class CUITargetItem : public CUIListItem {
public:
  // INLINED
  CUITargetItem() : itemIcon(nullptr), targetIcon(nullptr), targetText(nullptr) {
  }

  /* 00E8 */ CUIGameItem* itemIcon;
  /* 00EC */ CUIStaticImage* targetIcon;
  /* 00F0 */ CUIStaticText* targetText;

  void OnCreate(int width, int height) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void SetSelected(bool selected) override;

  ITEM_INFO* GetItemInfo();
};

class CUIIngredient : public CUIControl {
public:
  explicit CUIIngredient()
      : CUIControl(), backgroundImage(nullptr), sufficientMaterialImage(nullptr), itemIcon(nullptr), ownedText(nullptr), requiredText(nullptr) {
  }

  /* 0080 */ CUIStaticImage* backgroundImage;
  /* 0084 */ CUIStaticImage* sufficientMaterialImage;
  /* 0088 */ CUIGameItem* itemIcon;
  /* 008C */ CUIStaticText* ownedText;
  /* 0090 */ CUIStaticText* requiredText;

  void OnCreate(int width, int height) override;
  __declspec(noinline) void SetMaterial(int itemId, int required);
};

class CUIEnchantPage : public CUIControl {
public:
  CUIEnchantPage();

  union {
    /* 0080 */ unsigned char backgroundStorage[0x4];
    /* 0080 */ CUIStaticImage* background;
  };
  /* 0084 */ CUIGameItem* gameItem;
  /* 0088 */ std::map<int, CUISlot*> slots;
  /* 0090 */ std::list<CUIIngredient*> ingredients;
  /* 0098 */ CUIListBoxEx<CUIEnchantItem, CEnchantItemData>* enchantList;
  /* 009C */ CUIStaticText* costText;
  /* 00A0 */ CUIImageButton* enchantButton = nullptr;
  /* 00A4 */ EnchantInfo* enchantInfo;
  /* 00A8 */ uint64_t group;
  /* 00B0 */ int selectedSlot;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  virtual void OnSelectEnchant(CUIEnchantItem* item, const CEnchantItemData& data);
  virtual void RequestEnchant() = 0;
  virtual bool LoadEnchantList() = 0;
  virtual void UpdateEnchantInfo();
  virtual void OnSelectSlot(int value);
  virtual void SetSlot(int value);

  __declspec(noinline) CUIIngredient* CreateIngredient();
  void ResetSlots();
  bool SetItem(const ITEM_INFO& item);

  // INLINED
  void LoadTarget(EnchantInfo* info, uint64_t targetGroup, const ITEM_INFO& item) {
    this->ResetSlots();
    if (info && this->SetItem(item)) {
      this->group = targetGroup;
      this->enchantInfo = info;
      this->UpdateEnchantInfo();
    }
  }

  __declspec(noinline) int FindEmptySlot();
  __declspec(noinline) std::optional<EnchantOptionInfo*> FindOption(uint32_t key);
  __declspec(noinline) void SetCost(const EnchantPerfectInfo& info);
  void UpdateEnchantItem(CUIEnchantItem* item, const CEnchantItemData& data);
};

class CUINormalRandom : public CUIEnchantPage {
public:
  using CUIEnchantPage::CUIEnchantPage;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void RequestEnchant() override;
  bool LoadEnchantList() override;
};

class CUINormalPerfect : public CUIEnchantPage {
public:
  using CUIEnchantPage::CUIEnchantPage;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void OnSelectEnchant(CUIEnchantItem* item, const CEnchantItemData& data) override;
  void RequestEnchant() override;
  bool LoadEnchantList() override;
};

class CUIReset : public CUIEnchantPage {
public:
  using CUIEnchantPage::CUIEnchantPage;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void RequestEnchant() override;
  bool LoadEnchantList() override;
  void UpdateEnchantInfo() override;
};

class CUIUpgradeEnchantPage : public CUIEnchantPage {
public:
  using CUIEnchantPage::CUIEnchantPage;

  void OnSelectSlot(int value) override;
  void SetSlot(int value) override;
};

class CUIUpgradeRandom : public CUIUpgradeEnchantPage {
public:
  using CUIUpgradeEnchantPage::CUIUpgradeEnchantPage;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void RequestEnchant() override;
  bool LoadEnchantList() override;
};

class CUIUpgradePerfect : public CUIUpgradeEnchantPage {
public:
  using CUIUpgradeEnchantPage::CUIUpgradeEnchantPage;

  /* 00B8 */ bool isUpgrade = false;

  void OnCreate(int width, int height) override;
  void OnSelectEnchant(CUIEnchantItem* item, const CEnchantItemData& data) override;
  void RequestEnchant() override;
  bool LoadEnchantList() override;
};

class CUIMainPage : public CUIControl {
public:
  __declspec(noinline) CUIMainPage();

  /* 0080 */ CUIEnchantPage* firstPage;
  /* 0084 */ CUIEnchantPage* secondPage;
  /* 0088 */ CUITab* tab;

  void OnInit() override;

  // INLINED
  void LoadTarget(EnchantInfo* info, uint64_t targetGroup, const ITEM_INFO& item) {
    this->firstPage->LoadTarget(info, targetGroup, item);
    this->secondPage->LoadTarget(info, targetGroup, item);
  }
};

class CUINormal : public CUIMainPage {
public:
  using CUIMainPage::CUIMainPage;

  void OnCreate(int width, int height) override;
};

class CUIUpgrade : public CUIMainPage {
public:
  using CUIMainPage::CUIMainPage;

  void OnCreate(int width, int height) override;
};

class CEnchantItemData {
public:
  /* 0000 */ int itemId;
  /* 0004 */ bool frozen;
};

class CUISlot : public CUIControl {
public:
  // INLINED
  explicit CUISlot() : CUIControl(), radioButton(nullptr), itemIcon(nullptr), description(nullptr) {
  }

  /* 0080 */ CUIRadioButton* radioButton;
  /* 0084 */ CUIGameItem* itemIcon;
  /* 0088 */ CUITextViewer* description;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void OnRBtnDown(int x, int y) override;

  void SelectSlot(UIWindow* button);
};

class CUISlot0 : public CUISlot {
public:
  using CUISlot::CUISlot;

  void OnCreate(int width, int height) override;
  void OnInit() override;
};

class CUISlot1 : public CUISlot {
public:
  using CUISlot::CUISlot;

  void OnCreate(int width, int height) override;
  void OnInit() override;
};

class CUISlot2 : public CUISlot {
public:
  using CUISlot::CUISlot;

  void OnCreate(int width, int height) override;
  void OnInit() override;
};

class CUISlot3 : public CUISlot {
public:
  using CUISlot::CUISlot;

  void OnCreate(int width, int height) override;
  void OnInit() override;
};
}

#endif
