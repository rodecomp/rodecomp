#ifndef RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H
#define RAGNAROK_SOURCE_UI_STYLINGSHOP_UISTYLINGSHOPWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"

#include <list>
#include <map>
#include <set>
#include <vector>

enum StylingShopId : int {
  STYLINGSHOP_BUTTON_HEAD_PREV = 450,
  STYLINGSHOP_BUTTON_HEAD_NEXT = 451,
  STYLINGSHOP_BUTTON_PALETTE = 495,
  STYLINGSHOP_TAB_FAVORITE = 496,
  STYLINGSHOP_TAB_ACCESSORY = 497,
  STYLINGSHOP_LAYOUT_2ND_COSTUME = 498,
  STYLINGSHOP_TAB_COSTUME_CHANGE = 583,
  STYLINGSHOP_BUTTON_JOB_PREV = 584,
  STYLINGSHOP_BUTTON_JOB_NEXT = 585,
};

enum StylingShopStyleTable : int {
  STYLE_TABLE_HEAD_PALETTE = 1,
  STYLE_TABLE_HEAD_STYLE = 2,
  STYLE_TABLE_BODY_PALETTE = 3,
  STYLE_TABLE_TOP_ACCESSORY = 4,
  STYLE_TABLE_MID_ACCESSORY = 5,
  STYLE_TABLE_BOTTOM_ACCESSORY = 6,
  STYLE_TABLE_BODY_STYLE = 7,
};

class UIDisplayCharacter : public UIWindow {
public:
  UIDisplayCharacter();
  explicit UIDisplayCharacter(bool hideFavoriteButton);
  __declspec(noinline) void LoadSessionStyle();
  void CopyStyle(const UIDisplayCharacter* other);
  void LoadCostumes();
  void SelectNextStyle(const char* table, int* goods);
  void SelectPreviousStyle(const char* table, int* goods);
  std::string GetStyleTable(const int& type);
  void CreateButtons();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ const char* shadowBitmapName;
  /* 0080 */ int slotNumber;
  /* 0084 */ int direction;
  /* 0088 */ int robe;
  /* 008C */ int head;
  /* 0090 */ int job;
  /* 0094 */ int headPalette;
  /* 0098 */ int bodyStyle;
  /* 009C */ int topAccessory;
  /* 00A0 */ int middleAccessory;
  /* 00A4 */ int bottomAccessory;
  /* 00A8 */ int animationStartTick;
  /* 00AC */ UIBitmapButton* m_favoriteButton;
  /* 00B0 */ bool hideFavoriteButton;
  /* 00B4 */ int shadowX;
  /* 00B8 */ int shadowY;
  /* 00BC */ int topAccessoryItemId;
  /* 00C0 */ int middleAccessoryItemId;
  /* 00C4 */ int bottomAccessoryItemId;
  /* 00C8 */ std::map<int, int> m_jobs;
};

class UIShowInfoWnd : public UIWindow {
public:
  UIShowInfoWnd();
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
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ UIDisplayCharacter* m_display;
  /* 0080 */ UICheckBox* m_checks[8];
  /* 00A0 */ unsigned char showPayments;
  /* 00A1 */ unsigned char showPaymentItems;
  /* 00A2 */ unsigned char showDottedLines;
  /* 00A3 */ unsigned char padding_00A3;
  /* 00A4 */ int m_checkTop;
  /* 00A8 */ int paymentFontSize;
  /* 00AC */ bool m_buyable;
  /* 00AD */ bool m_changed[6];
  /* 00B4 */ std::list<int> m_list;
  /* 00BC */ unsigned char costumeChanged;
  /* 00C0 */ int paymentIconX;
  /* 00C4 */ int paymentIconYOffset;
  /* 00C8 */ int paymentTextX;
  /* 00CC */ int paymentTextYOffset;
  /* 00D0 */ const char* m_offBitmap;
};

class UIStylingshopTabParentWnd : public UIWindow {
public:
  // INLINED
  explicit UIStylingshopTabParentWnd() : UIWindow(0), m_display(nullptr), m_showInfo(nullptr) {
  }
  void UpdateFavoriteButton(const bool& favorite);
  bool IsBuyable() const;

  /* 007C */ UIDisplayCharacter* m_display;
  /* 0080 */ UIShowInfoWnd* m_showInfo;
  /* 0084 */ bool primaryTabVisible;
  /* 0085 */ bool padding_0085;
  /* 0086 */ bool accessoryTabVisible;

  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UICostumeChangeWnd : public UIStylingshopTabParentWnd {
public:
  UICostumeChangeWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIStylingShopButton;

void ToggleStylingAccessory(int itemId, int slot);

class UIStylingShopWnd : public UIFrameWnd {
public:
  UIStylingShopWnd();

  /* 00B4 */ UIBitmapButton* m_buyButton;
  /* 00B8 */ UIDisplayCharacter* m_preview;
  /* 00BC */ std::map<UIWindow*, UIStylingshopTabParentWnd*> m_tabs;

  void OnCreate(int width, int height) override;
  void OnProcess() override;
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
  UI2ndCostumeWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 0088 */ UI2ndCostumeCheckBox* m_hideCheckBox;
  /* 008C */ UI2ndCostumeCheckBox* m_showCheckBox;
};

class UIAccessoryCusWnd : public UIStylingshopTabParentWnd {
public:
  // INLINED
  explicit UIAccessoryCusWnd() : UIStylingshopTabParentWnd() {
  }
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIStaticBitmap;

class UIAccessoryShowCase : public UIScrollWnd {
public:
  // INLINED
  UIAccessoryShowCase(int width, int height) : UIScrollWnd(width, height, 0), m_itemFrames() {
    this->LoadAccessories(width, height);
  }
  void LoadAccessories(int width, int height);
  void AddAccessories(int& loaded, int& skipped, const char* table, const int& count, const int& slot);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B0 */ unsigned char padding_00B0[12];
  /* 00BC */ std::vector<UIStaticBitmap*> m_itemFrames;
};

class UIBasicStylingWnd : public UIStylingshopTabParentWnd {
public:
  UIBasicStylingWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0088 */ std::vector<UIButton*> m_paletteButtons;
  /* 0094 */ UIButton* m_defaultPaletteButton;
};

class UISlotOfMyStyleWnd;

class UIMyStyleWnd : public UIStylingshopTabParentWnd {
public:
  UIMyStyleWnd();
  void LoadLikeList();
  void SaveLikeList();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0088 */ int m_selectedSlot;
  /* 008C */ UISlotOfMyStyleWnd* m_slots[2];
  /* 0094 */ UIBitmapButton* deleteStyleButtons[2];
  /* 009C */ UIBitmapButton* rotateLeftButtons[2];
  /* 00A4 */ UIBitmapButton* rotateRightButtons[2];
};

class UISlotOfMyStyleWnd : public UIStylingshopTabParentWnd {
public:
  UISlotOfMyStyleWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UISlotForSelectWnd : public UIWindow {
public:
  // INLINED
  explicit UISlotForSelectWnd() : UIWindow(0) {
  }
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
};

class UI2ndCostumeCheckBox : public UICheckBox {
public:
  // INLINED
  UI2ndCostumeCheckBox() : UICheckBox() {
  }
  void OnLBtnDown(int x, int y) override;
};

class UIStylingShopButton : public UIBitmapButton {
public:
  // INLINED
  UIStylingShopButton() : UIBitmapButton() {
  }
  // INLINED
  void SetArgument(int argument) {
    this->m_argument = argument;
  }
  // INLINED
  int Argument() const {
    return this->m_argument;
  }
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
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
  void OnRBtnUp(int x, int y) override;

  /* 0120 */ int m_slot;
  /* 0124 */ int m_itemId;
};

#endif
