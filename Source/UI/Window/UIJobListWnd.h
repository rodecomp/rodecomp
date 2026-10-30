#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>

struct UIButtonContext;
struct JobListWndInfo;

extern std::map<int, bool> g_partyBookingJobs;

class UIJobListWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
  /* 00B8 */ UIWindow* m_jobChecks[13];
  /* 00EC */ JobListWndInfo* m_saved;
};

#endif
