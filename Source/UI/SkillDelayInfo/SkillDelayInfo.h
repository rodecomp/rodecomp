#ifndef RAGNAROK_SOURCE_UI_SKILLDELAYINFO_SKILLDELAYINFO_H
#define RAGNAROK_SOURCE_UI_SKILLDELAYINFO_SKILLDELAYINFO_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"

#include <cstddef>
#include <cstdint>
#include <map>

namespace SkillDelayInfo {
class CDelayItemData {
public:
  /* 0000 */ int unknown_0000;
  /* 0004 */ std::map<int, int> unknown_0004;
};

class CUIDelayItem : public CUIListItem {
public:
  void Slot16() override;
  void Paint() override;

  /* 00E8 */ unsigned char unknown_00E8[0x10];
  /* 00F8 */ std::uint32_t backgroundColor;
  /* 00FC */ std::uint32_t borderColor;
};

class CUITime : public CUIControl {
public:
  ~CUITime() override;
  void Slot16() override;
  void Paint() override;

  /* 0080 */ UIWindow* text;
};
}

class CUISkillDelayInfo : public CUIFrameWnd {
public:
  void Paint() override;
  virtual void Slot53();

  void OnButtonClick();
  void OnSelectItem(SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data);

  /* 00D8 */ unsigned char unknown_00D8[0x2C];
  /* 0104 */ std::uint32_t backgroundColor;
  /* 0108 */ std::uint32_t borderColor;
  /* 010C */ int skillId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SkillDelayInfo::CDelayItemData) == 0x0C);
#endif

#endif
