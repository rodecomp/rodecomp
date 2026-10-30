#include "UI/SkillDelayInfo/SkillDelayInfo.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Window/WindowBorder.h"

// OFFSET: 0x00A71950
// FUNCTION: fn_16806
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::ClearItems() {
  for (SkillDelayInfo::CUIDelayItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A719E0
// FUNCTION: fn_16807
void CUISkillDelayInfo::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(345, 379);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
  this->SetSkinColors(0, 0, 0, 0);
}

namespace SkillDelayInfo {
// OFFSET: 0x00A778C0
// FUNCTION: fn_16877
CUITime::~CUITime() = default;

// OFFSET: 0x00A77CE0
// FUNCTION: fn_16882
void CUIDelayItem::Paint() {
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);
  DrawRectangleBorder(*this, 0, 0, 325, 22, this->borderColor);
  this->FillRegion(1, 0, 43, 21, this->backgroundColor);
  this->FillRegion(44, 0, 280, 21, 0xFFFFFF);
  DrawRectangleBorder(*this, 44, 0, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 114, 0, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 184, 0, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 254, 0, 1, 21, this->borderColor);
}

// OFFSET: 0x00A77F00
// FUNCTION: fn_16884
void CUITime::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A77F10
// FUNCTION: fn_16885
void CUIDelayItem::Slot16() {
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);
}

// OFFSET: 0x00A78180
// FUNCTION: fn_16887
void CUITime::Slot16() {
}
}

// OFFSET: 0x00A77DD0
// FUNCTION: fn_16883
void CUISkillDelayInfo::Paint() {
  CUIFrameWnd::Paint();
  this->backgroundColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{0, 2}, true);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), SkinPaletteCell{6, 2}, true);
  this->FillRegion(11, 28, 323, 23, this->backgroundColor);
  DrawRectangleBorder(*this, 10, 27, 325, 25, this->borderColor);
  DrawRectangleBorder(*this, 10, 58, 325, 46, this->borderColor);
  this->FillRegion(11, 59, 323, 44, this->backgroundColor);
  DrawRectangleBorder(*this, 11, 81, 323, 1, this->borderColor);
  DrawRectangleBorder(*this, 54, 59, 1, 44, this->borderColor);
  DrawRectangleBorder(*this, 194, 59, 1, 44, this->borderColor);
  DrawRectangleBorder(*this, 124, 83, 1, 21, this->borderColor);
  DrawRectangleBorder(*this, 264, 83, 1, 21, this->borderColor);
}
