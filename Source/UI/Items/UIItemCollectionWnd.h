#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H

#include "UI/Buttons/UIItemMoveInfo.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIResizer.h"

#include <cstdint>
#include <list>
#include <map>
#include <string>

class UIBitmapButton;
class UIBasicButton;
class UISpriteButton;
struct WindowPlacement;
struct CombinedCardItemCollectionWndInfo;

class UIItemCollectionWnd : public UIFrameWnd {
public:
  UIItemCollectionWnd();
  ~UIItemCollectionWnd() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;

  // INLINED
  std::string& ItemIdText() {
    return this->m_item.itemId;
  }

  __declspec(property(get = ItemIdText)) std::string& m_itemIdText;

  /* 00B4 */ unsigned char unknown_00B4[0x4];
  /* 00B8 */ ITEM_INFO m_item;
  /* 01B0 */ UITextViewer* m_viewer;
  /* 01B4 */ UIBitmapButton* m_viewButton;
  /* 01B8 */ UISpriteButton* m_readButtons[2];
  /* 01C0 */ std::uint32_t m_previewTick;
  /* 01C4 */ std::string unknown_01C4;
  /* 01DC */ std::string unknown_01DC;
  /* 01F4 */ unsigned char unknown_01F4[0x4];
  /* 01F8 */ int m_kind;
  /* 01FC */ int unknown_01FC;
  /* 0200 */ WindowPlacement* m_placement;
  /* 0204 */ UIResizer* m_resizer;
  /* 0208 */ UIBitmapButton* m_closeButton;
  /* 020C */ unsigned char m_compare;
  /* 0210 */ UIItemMoveInfo* m_moveInfo;
  /* 0214 */ UIBitmapButton* m_helpButton;
  /* 0218 */ std::int16_t unknown_0218;
  /* 021A */ std::int16_t unknown_021A;
  /* 021C */ std::int16_t unknown_021C;
  /* 021E */ unsigned char unknown_021E[0x18];
  /* 0236 */ std::uint16_t unknown_0236;
  /* 0238 */ int unknown_0238;
  /* 023C */ UIBasicButton* unknown_023C;
  /* 0240 */ std::map<int, std::list<std::string>> m_cardTexts;
  /* 0248 */ int m_extraHeight;
  /* 024C */ unsigned char unknown_024C[0x4];

  void Move(int x, int y) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetHeight(int height);
  int CardSlotAt(int x, int y, int* slotX, int* slotY);
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  bool WrapText(std::string text, int width, std::list<std::string>& lines);
};

class UICombinedCardItemCollectionWnd : public UIItemCollectionWnd {
public:
  UICombinedCardItemCollectionWnd();
  void OnCreate(int width, int height) override;
  void Move(int x, int y) override;
  void StoreInfo() override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0250 */ CombinedCardItemCollectionWndInfo* m_cardPlacement;
};

class UIItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  UIItemCollection_ComparisonWnd();
  void OnCreate(int width, int height) override;
  void Move(int x, int y) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void OnRBtnDown(int x, int y) override;
};

class UICombinedCardItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  UICombinedCardItemCollection_ComparisonWnd();
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif
