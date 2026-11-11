#ifndef RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_SELECTPACKAGEITEMBOX_H
#define RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_SELECTPACKAGEITEMBOX_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <cstddef>
#include <string>

class CUIBasicButton;
class CUIGameItem;
class CUIStaticImage;
class CUIStaticText;

namespace SelectPackageItemBox {
class CTabInfoData {
public:
  /* 0000 */ int packageId;
  /* 0004 */ int tabIndex;
  /* 0008 */ std::string name;
};

class CUIItemInfo : public CUIListItem {
public:
  CUIItemInfo();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;

  /* 00E8 */ CUIStaticImage* image;
  /* 00EC */ CUIGameItem* gameItem;
  /* 00F0 */ CUIStaticText* nameText;
};

class CUITabInfo : public CUIListItem {
public:
  CUITabInfo();
  void OnCreate(int width, int height) override;
  void OnInit() override;

  /* 00E8 */ CUIStaticText* nameText;
};
}

class CUISelectPackageItemBox : public CUIFrameWnd {
public:
  __declspec(noinline) CUISelectPackageItemBox();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  virtual void Initialize();

  void OnCancel();
  void OnClickClose();
  void OnClose();
  void OnOk();
  void OnSelectItem(SelectPackageItemBox::CUIItemInfo* item, const ITEM_INFO& data);
  void OnSelectTab(SelectPackageItemBox::CUITabInfo* tab, const SelectPackageItemBox::CTabInfoData& data);
  void BindItem(SelectPackageItemBox::CUIItemInfo* item, const ITEM_INFO& data);
  void BindTab(SelectPackageItemBox::CUITabInfo* tab, const SelectPackageItemBox::CTabInfoData& data);
  void Open(int sourceIndex);

  /* 00D8 */ CUITitleBar* title;
  /* 00DC */ CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>* tabList;
  /* 00E0 */ CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>* itemList;
  /* 00E4 */ CUIBasicButton* okButton;
  /* 00E8 */ CUIBasicButton* cancelButton;
  /* 00EC */ int sourceIndex;
  /* 00F0 */ int packageId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SelectPackageItemBox::CTabInfoData) == 0x20);
#endif

#endif
