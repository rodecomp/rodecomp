#include "UI/Window/UIRoMapWnd.h"

#include "Contents/MapSkyMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x00931080
// FUNCTION: fn_12697
void UIRoMapWnd::Slot25(int, int) {
}

// OFFSET: 0x0091BE80
// FUNCTION: fn_12611
void SetRoMapMarker(std::string name, const WorldViewArea& area, RoMapMarker& marker) {
  const int width = area.right - area.left;
  const int height = area.bottom - area.top;
  const float centerX = static_cast<float>(width) * g_depthAverageHalf + static_cast<float>(area.left);
  const float centerY = static_cast<float>(height) * g_depthAverageHalf + static_cast<float>(area.top);

  marker.name = name;
  marker.area = area;
  marker.centerX = centerX;
  marker.centerY = centerY;
  marker.width = static_cast<float>(width);
  marker.height = static_cast<float>(height);
}

// OFFSET: 0x0091CF40
// FUNCTION: fn_12622
void UIRoMapWnd::ClearRecords() {
  this->m_records.clear();
  this->unknown_00D0 = 0;
  this->unknown_00C4 = 0;
  this->unknown_00C8 = 0;
  this->unknown_00C0 = 0;
  this->unknown_00BC = 0;
  this->unknown_00CC = 0;
}

// OFFSET: 0x009335B0
// FUNCTION: fn_12725
void UIRoMapWnd::Slot33(int, int) {
  if (g_windowMgr.unknown_0214 != 0) {
    this->m_frameContext->windows.CloseWindow(26);
  }

  this->unknown_0208 = 0;
}
