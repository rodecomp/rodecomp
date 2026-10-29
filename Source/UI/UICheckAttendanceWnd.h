#ifndef RAGNAROK_SOURCE_UI_UICHECKATTENDANCEWND_H
#define RAGNAROK_SOURCE_UI_UICHECKATTENDANCEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

struct AttendanceBanner {
  /* 0000 */ std::string image;
  /* 0018 */ std::string url;
  /* 0030 */ int action;
  /* 0034 */ int argument;
};

class UICheckAttendanceWnd_Banner : public UIWindow {
public:
  ~UICheckAttendanceWnd_Banner() override;
  void Slot19() override;
  void ResetCursor(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void AddBanner(std::string image, std::string url, int action, int argument);

  /* 007C */ std::vector<AttendanceBanner> m_entries;
  /* 0088 */ std::vector<UIWindow*> m_buttons;
  /* 0094 */ unsigned int m_current;
  /* 0098 */ std::uint32_t m_lastSwitch;
};

class UICheckAttendanceWnd : public UIFrameWnd {
public:
  ~UICheckAttendanceWnd() override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void VisitChildren() override;
  int SlotAt(int x, int y);

  // INLINED
  int BannerHeight() const {
    return this->m_banner->m_entries.size() != 0 ? 42 : 0;
  }

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ unsigned char m_blinking;
  /* 00BC */ int m_checkedDays;
  /* 00C0 */ int m_daysLeft;
  /* 00C4 */ int m_highlight;
  /* 00C8 */ UICheckAttendanceWnd_Banner* m_banner;
  /* 00CC */ UIWindow* m_notice;
};

#endif
