#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>

class UIBitmapButton;
class UICheckBox;
class UIStaticText;
struct UIButtonContext;
struct JobListWndInfo;

extern std::map<int, bool> g_partyBookingJobs;

class UIJobListWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void RefreshJobs();

protected:
  /* 00B4 */ UIStaticText* m_title;
  /* 00B8 */ UICheckBox* m_jobChecks[6];
  /* 00D0 */ UIBitmapButton* m_okButton;
  /* 00D4 */ unsigned char unknown_00D4[0x18];
  /* 00EC */ JobListWndInfo* m_saved;
  UIButtonContext* m_controls;
};

#endif
