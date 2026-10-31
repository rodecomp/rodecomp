#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIROMAPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIROMAPWND_H

#include "Contents/WorldView/WorldViewData.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>
#include <string>
#include <vector>

class CTexture;

struct RoMapMarker {
  /* 0000 */ std::string name;
  /* 0018 */ WorldViewArea area = {};
  /* 002C */ float centerX;
  /* 0030 */ float centerY;
  /* 0034 */ float width;
  /* 0038 */ float height;
};

struct RoMapTooltipBox {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ int unknown_0010;
};

void SetRoMapMarker(std::string name, const WorldViewArea& area, RoMapMarker& marker);

extern CWorldViewMgr g_worldViewMgr;

class UIRoMapWnd;

struct RoMapKafraMarkers {
  void Build(std::string mapName);
  void Update(UIRoMapWnd* wnd, float elapsed);

  /* 0000 */ int unknown_0000;
  /* 0004 */ std::vector<RoMapMarker> markers;
  /* 0010 */ RoMapMarker current;
  /* 004C */ std::string mapName;
  /* 0064 */ float blend;
};

class UIRoMapWnd : public UIFrameWnd {
  friend struct RoMapKafraMarkers;

public:
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot33(int x, int y) override;
  void ClearRecords();
  void ResetKafraBounds();
  void UpdateNavigationButtons();
  void DrawHoverTooltip();
  void DrawMapNames();
  void DrawFieldLabels();
  void DrawRecordBorders();
  void DrawDungeonLinks();
  void DrawMarks();
  void DrawRecordFaces(int unused);
  void DrawRecordAreas();
  void DrawKafraIcon(float x, float y);
  void DrawArrow(
    float x0, float y0, float x1, float y1, float startLeft, float startTop, float startRight, float startBottom, float endLeft, float endTop, float endRight,
    float endBottom, int bodyWidth, CTexture* body, int headWidth, CTexture* head, float progress, std::uint32_t color
  );
  void RefreshPartyRecords();
  bool LocateRecord(WorldViewRecord& record);

private:
  /* 00B4 */ std::list<WorldViewRecord> m_records;
  /* 00BC */ WorldViewRecord* unknown_00BC;
  /* 00C0 */ WorldViewRecord* unknown_00C0;
  /* 00C4 */ RoMapTooltipBox* unknown_00C4;
  /* 00C8 */ int* unknown_00C8;
  /* 00CC */ unsigned char unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ unsigned char m_showLabels;
  /* 00D5 */ unsigned char unknown_00D5[0xB];
  /* 00E0 */ RoMapKafraMarkers m_kafraMarkers;
  /* 0148 */ unsigned char unknown_0148[0xA0];
  /* 01E8 */ int m_mouseX;
  /* 01EC */ int m_mouseY;
  /* 01F0 */ unsigned char unknown_01F0[0x18];
  /* 0208 */ int unknown_0208;
  /* 020C */ unsigned char unknown_020C[0xC];
  /* 0218 */ int m_kafraLeft;
  /* 021C */ int m_kafraTop;
  /* 0220 */ int m_kafraRight;
  /* 0224 */ int m_kafraBottom;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RoMapTooltipBox) == 0x14);
static_assert(sizeof(RoMapMarker) == 0x3C);
static_assert(offsetof(RoMapMarker, area) == 0x18);
static_assert(offsetof(RoMapMarker, centerX) == 0x2C);
static_assert(sizeof(RoMapKafraMarkers) == 0x68);
#endif

#endif
