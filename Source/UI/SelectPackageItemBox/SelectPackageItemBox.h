#ifndef RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_SELECTPACKAGEITEMBOX_H
#define RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_SELECTPACKAGEITEMBOX_H

#include "Contents/Items/ItemInfo.h"
#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <cstddef>
#include <string>

class CUIGameItem;
class CUIStaticImage;
class CUITextButton;

namespace SelectPackageItemBox {
class CTabInfoData {
public:
  /* 0000 */ int packageId;
  /* 0004 */ int tabIndex;
  /* 0008 */ std::string name;
};

class CUIItemInfo : public CUIListItem {
public:
  void Slot16() override;
  void Paint() override;

  /* 00E8 */ CUIStaticImage* image;
  /* 00EC */ CUIGameItem* gameItem;
  /* 00F0 */ UIWindow* nameText;
};

class CUITabInfo : public CUIListItem {
public:
  void Slot16() override;

  /* 00E8 */ UIWindow* nameText;
};
}

class CUISelectPackageItemBox : public CUIFrameWnd {
public:
  virtual void Slot53();

  void OnCancel();
  void OnOk();
  void OnSelectItem(SelectPackageItemBox::CUIItemInfo* item, const ITEM_INFO& data);
  void OnSelectTab(SelectPackageItemBox::CUITabInfo* tab, const SelectPackageItemBox::CTabInfoData& data);

  /* 00D8 */ UIWindow* title;
  /* 00DC */ CUIListBoxEx<SelectPackageItemBox::CUITabInfo, SelectPackageItemBox::CTabInfoData>* tabList;
  /* 00E0 */ CUIListBoxEx<SelectPackageItemBox::CUIItemInfo, ITEM_INFO>* itemList;
  /* 00E4 */ CUITextButton* okButton;
  /* 00E8 */ CUITextButton* cancelButton;
  /* 00EC */ int sourceIndex;
  /* 00F0 */ int packageId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SelectPackageItemBox::CTabInfoData) == 0x20);
#endif

#endif
