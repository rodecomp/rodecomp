#ifndef RAGNAROK_SOURCE_UI_SKILLDELAYINFO_SKILLDELAYINFO_H
#define RAGNAROK_SOURCE_UI_SKILLDELAYINFO_SKILLDELAYINFO_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

class CUIStaticText;

namespace SkillDelayInfo {
class CUITime;

class CDelayItemData {
public:
  /* 0000 */ int unknown_0000;
  /* 0004 */ std::map<int, int> unknown_0004;
};

class CUIDelayItem : public CUIListItem {
public:
  CUIDelayItem();
  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;

  /* 00E8 */ CUIStaticText* level;
  /* 00EC */ std::vector<CUITime*> times;
  /* 00F8 */ std::uint32_t backgroundColor;
  /* 00FC */ std::uint32_t borderColor;
};

class CUITime : public CUIControl {
public:
  explicit CUITime(UIWindowContext& context);
  ~CUITime() override;
  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;

  __declspec(noinline) void SetTime(int milliseconds);

  /* 0080 */ CUIStaticText* text;
};
}

class CUITitleBar;
class CUITextViewer;

class CUISkillDelayInfo : public CUIFrameWnd {
public:
  __declspec(noinline) CUISkillDelayInfo();
  void Slot16() override;
  void Paint() override;
  virtual void Slot53();

  void OnButtonClick();
  void OnSelectItem(SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data);
  void OnUpdateItem(SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data);

  /* 00D8 */ CUITitleBar* titleBar;
  /* 00DC */ UIWindow* unknown_00DC;
  /* 00E0 */ CUIStaticText* labels[3];
  /* 00EC */ CUIStaticText* columns[4];
  /* 00FC */ CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* delayList;
  /* 0100 */ CUITextViewer* description;
  /* 0104 */ std::uint32_t backgroundColor;
  /* 0108 */ std::uint32_t borderColor;
  /* 010C */ int skillId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SkillDelayInfo::CDelayItemData) == 0x0C);
#endif

#endif
