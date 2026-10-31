#ifndef RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H
#define RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"

#include <list>
#include <map>
#include <set>
#include <vector>

class UIDisplayCharacter : public UIWindow {
public:
  UIDisplayCharacter(UIWindowContext& context);
  void LoadSessionStyle();
  void CopyStyle(const UIDisplayCharacter* other);
  void LoadCostumes();
  void SelectNextStyle(const char* table, int* goods);
  void SelectPreviousStyle(const char* table, int* goods);
  std::string GetStyleTable(const int& type);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ const char* unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int job;
  /* 0094 */ int unknown_0094;
  /* 0098 */ int unknown_0098;
  /* 009C */ int unknown_009C;
  /* 00A0 */ int unknown_00A0;
  /* 00A4 */ int unknown_00A4;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ UIBitmapButton* m_favoriteButton;
  /* 00B0 */ int unknown_00B0;
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ std::map<int, int> m_jobs;
};

class UIShowInfoWnd : public UIWindow {
public:
  explicit UIShowInfoWnd(UIWindowContext& context);
  ~UIShowInfoWnd() override;
  void ResetCheckBoxes();
  void ArrangeCheckBoxes(const int& layout);
  void CheckColoringGoods(bool* coloring);
  bool CheckPayment(const int& index, const int& zeny, const int& itemId, const int& costumeItemId);
  void GetPayment(const int& type, int* zeny, int* itemId, int* costumeItemId);
  void UpdateChangedStyles();
  bool SendStyleChange();
  void DrawPaymentItem(const int& type, const int& zeny, const int& itemId, bool enabled, const int& row);
  void DrawPayments();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ UIDisplayCharacter* m_display;
  /* 0080 */ UICheckBox* m_checks[8];
  /* 00A0 */ unsigned char unknown_00A0;
  /* 00A1 */ unsigned char unknown_00A1;
  /* 00A2 */ unsigned char unknown_00A2;
  /* 00A3 */ unsigned char unknown_00A3;
  /* 00A4 */ int m_checkTop;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ bool m_buyable;
  /* 00AD */ unsigned char unknown_00AD[4];
  /* 00B1 */ unsigned short unknown_00B1;
  /* 00B3 */ unsigned char unknown_00B3;
  /* 00B4 */ std::list<int> m_list;
  /* 00BC */ unsigned char unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ const char* m_offBitmap;
};

class UIStylingshopTabParentWnd : public UIWindow {
public:
  // INLINED
  explicit UIStylingshopTabParentWnd(UIWindowContext& context) : UIWindow(context, 0), m_display(nullptr), m_showInfo(nullptr) {
  }
  void UpdateFavoriteButton(const bool& favorite);
  bool IsBuyable() const;

  /* 007C */ UIDisplayCharacter* m_display;
  /* 0080 */ UIShowInfoWnd* m_showInfo;
  /* 0084 */ bool unknown_0084;
  /* 0085 */ bool unknown_0085;
  /* 0086 */ bool unknown_0086;

  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UICostumeChangeWnd : public UIStylingshopTabParentWnd {
public:
  explicit UICostumeChangeWnd(UIWindowContext& context);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIStylingShopButton;

void ToggleStylingAccessory(int itemId, int slot);

class UIStylingShopWnd : public UIFrameWnd {
public:
  UIStylingShopWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);

  /* 00B4 */ UIStylingShopButton* m_buyButton;
  /* 00B8 */ UIDisplayCharacter* m_preview;
  /* 00BC */ std::map<UIWindow*, UIStylingshopTabParentWnd*> m_tabs;
  UIButtonContext* m_controls;

  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void AddTab(const char* normal, const char* hover, const char* pressed, const int& id, UIStylingshopTabParentWnd* tab, const char* tooltip);
  void RefreshFavoriteButton();
  bool AddToMyStyle();
  void SetBuyEnabled(bool enabled);
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UI2ndCostumeCheckBox;

class UI2ndCostumeWnd : public UIStylingshopTabParentWnd {
  friend class UIStylingShopWnd;

public:
  explicit UI2ndCostumeWnd(UIWindowContext& context);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 0088 */ UI2ndCostumeCheckBox* m_hideCheckBox;
  /* 008C */ UI2ndCostumeCheckBox* m_showCheckBox;
};

class UIAccessoryCusWnd : public UIStylingshopTabParentWnd {
public:
  // INLINED
  explicit UIAccessoryCusWnd(UIWindowContext& context) : UIStylingshopTabParentWnd(context) {
  }
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIStaticBitmap;

class UIAccessoryShowCase : public UIScrollWnd {
public:
  // INLINED
  UIAccessoryShowCase(UIWindowContext& windows, int width, int height) : UIScrollWnd(windows, width, height, 0), m_itemFrames() {
    this->Slot15(width, height);
  }
  void Slot15(int width, int height) override;
  void AddAccessories(int& loaded, int& skipped, const char* table, const int& count, const int& slot);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B0 */ unsigned char unknown_00B0[12];
  /* 00BC */ std::vector<UIStaticBitmap*> m_itemFrames;
};

class UIBasicStylingWnd : public UIStylingshopTabParentWnd {
public:
  explicit UIBasicStylingWnd(UIWindowContext& context);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0088 */ std::vector<UIButton*> m_paletteButtons;
  /* 0094 */ UIButton* m_defaultPaletteButton;
};

class UISlotOfMyStyleWnd;

class UIMyStyleWnd : public UIStylingshopTabParentWnd {
public:
  explicit UIMyStyleWnd(UIWindowContext& context);
  void SaveLikeList();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0088 */ int m_selectedSlot;
  /* 008C */ UISlotOfMyStyleWnd* m_slots[2];
  /* 0094 */ UIButton* unknown_0094[2];
  /* 009C */ UIButton* unknown_009C[2];
  /* 00A4 */ UIButton* unknown_00A4[2];
};

class UISlotOfMyStyleWnd : public UIStylingshopTabParentWnd {
public:
  explicit UISlotOfMyStyleWnd(UIWindowContext& context);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UISlotForSelectWnd : public UIWindow {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
};

class UI2ndCostumeCheckBox : public UICheckBox {
public:
  void Slot25(int x, int y) override;
};

class UIStylingShopButton : public UIBitmapButton {
public:
  // INLINED
  UIStylingShopButton(UIWindowContext& windowContext, UIButtonContext& buttonContext) : UIBitmapButton(windowContext, buttonContext) {
  }
  // INLINED
  void SetArgument(int argument) {
    this->m_argument = argument;
  }
  // INLINED
  int Argument() const {
    return this->m_argument;
  }
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UIAccBitmapButton : public UIBitmapButton {
public:
  // INLINED
  UIAccBitmapButton() : UIBitmapButton() {
  }
  // INLINED
  void SetArgument(int argument) {
    this->m_argument = argument;
  }
  // INLINED
  int Argument() const {
    return this->m_argument;
  }
  void Slot33(int x, int y) override;

  /* 0120 */ int m_slot;
  /* 0124 */ int m_itemId;
};

#endif
