#ifndef RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIITEMSTOREWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <string>

class UIBitmapEditCtrl;
class UICheckBox;
class UIItemStoreComboBoxWnd;
class UIStaticText;
class UITab;

class UIItemStoreWnd : public UIItemBaseWnd {
public:
  std::vector<unsigned char> GetReplayData() override;
  UIItemStoreWnd();
  __declspec(noinline) ~UIItemStoreWnd() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void StoreInfo() override;
  void OnLBtnDown(int x, int y) override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  __declspec(noinline) ITEM_INFO GetItemAt(int x, int y);
  void SetTabChecked(int index, bool checked);
  void SetTitle(std::string title);

  /* 00F0 */ UIBitmapEditCtrl* searchEdit;
  /* 00F4 */ UIItemStoreComboBoxWnd* sortCombo;
  /* 00F8 */ unsigned char m_tabFlags[0x08];
  /* 0100 */ UICheckBox* tabs[8];
  /* 0120 */ UIWindow* m_tabWindows[8];
  /* 0140 */ UIWindow* m_searchWindow;
  /* 0144 */ std::string m_capacityText;
  /* 015C */ UIStaticText* title;
  /* 0160 */ unsigned char padding_0160[0x04];
  /* 0164 */ ItemStoreFindListInfo* saved;
  /* 0168 */ UITab* tabBar;
  /* 016C */ int m_sortOptionA;
  /* 0170 */ int m_sortOptionB;
  /* 0174 */ int m_sortOptionC;
  /* 0178 */ int m_sortOptionD;
  /* 017C */ int m_sortOptionE;
  /* 0180 */ int m_sortMode;
  /* 0184 */ unsigned char m_searchFlags[0x04];
  /* 0188 */ int m_itemCount;
  /* 018C */ int m_capacity;
  /* 0190 */ unsigned char m_hideCapacity;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemStoreWnd, tabs) == 0x100);
static_assert(offsetof(UIItemStoreWnd, saved) == 0x164);
#endif

#endif
