#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIROMAPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIROMAPWND_H

#include "Contents/WorldView/WorldViewData.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <string>

struct RoMapMarker {
  /* 0000 */ std::string name;
  /* 0018 */ WorldViewArea area;
  /* 002C */ float centerX;
  /* 0030 */ float centerY;
  /* 0034 */ float width;
  /* 0038 */ float height;
};

void SetRoMapMarker(std::string name, const WorldViewArea& area, RoMapMarker& marker);

class UIRoMapWnd : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;
  void ClearRecords();

private:
  /* 00B4 */ std::list<WorldViewRecord> m_records;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ unsigned char unknown_00CC;
  /* 00D0 */ int unknown_00D0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RoMapMarker) == 0x3C);
static_assert(offsetof(RoMapMarker, area) == 0x18);
static_assert(offsetof(RoMapMarker, centerX) == 0x2C);
#endif

#endif
