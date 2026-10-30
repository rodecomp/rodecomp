#ifndef RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_SELECTPACKAGEITEMBOX_H
#define RAGNAROK_SOURCE_UI_SELECTPACKAGEITEMBOX_SELECTPACKAGEITEMBOX_H

#include "Contents/Items/ItemInfo.h"
#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <string>

namespace SelectPackageItemBox {
class CTabInfoData {
public:
  /* 0000 */ unsigned char unknown_0000[0x08];
  /* 0008 */ std::string unknown_0008;
};

class CUIItemInfo : public CUIListItem {};

class CUITabInfo : public CUIListItem {};
}

class CUISelectPackageItemBox : public CUIFrameWnd {
public:
  virtual void Slot53();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SelectPackageItemBox::CTabInfoData) == 0x20);
#endif

#endif
