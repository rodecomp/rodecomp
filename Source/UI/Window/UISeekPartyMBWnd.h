#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>

struct UIButtonContext;
struct SeekPartyMBWndInfo;
class UICheckBox;
class UIComboBoxDefWnd;

class UISeekPartyMBWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;
  void CreateJobChecks();
  void InitJobCategoryCombo();
  void ShowJobCategory(int category);

protected:
  /* 00BC */ unsigned char unknown_00BC[0x14];
  /* 00D0 */ UIComboBoxDefWnd* m_jobCategoryCombo;
  /* 00D4 */ unsigned char unknown_00D4[0x30];
  /* 0104 */ std::map<unsigned short, UICheckBox*> m_jobChecks[8];
  /* 0144 */ unsigned char unknown_0144[0x18];
  /* 015C */ int m_jobCategory;
  /* 0160 */ unsigned char unknown_0160[0x8];
  /* 0168 */ unsigned char m_minimized;
  /* 0169 */ unsigned char unknown_0169[0x13];
  /* 017C */ SeekPartyMBWndInfo* m_saved;
  UIButtonContext* m_controls;
};

#endif
