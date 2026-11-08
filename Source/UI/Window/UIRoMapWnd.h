#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIROMAPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIROMAPWND_H

#include "Contents/FlyingShipRouteInfo.h"
#include "Contents/WorldView/WorldViewData.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdint>
#include <list>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

class CTexture;
class UIBitmapButton;
class UICheckBox;
class UIComboBoxDefWnd;
class UIEditCtrl;

struct RoMapMarker {
  /* 0000 */ std::string name;
  /* 0018 */ WorldViewArea area = {};
  /* 002C */ float centerX;
  /* 0030 */ float centerY;
  /* 0034 */ float width;
  /* 0038 */ float height;
};

struct RoMapTooltipBox {
  // INLINED
  RoMapTooltipBox(int x, int y, int width, int height) : x(x), y(y), width(width), height(height), unknown_0010(1) {
  }

  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ int unknown_0010;
};

void SetRoMapMarker(std::string name, const WorldViewArea& area, RoMapMarker& marker);

extern CWorldViewMgr g_worldViewMgr;

class UIRoMapWnd;

struct CKafraMoveMapServiceInfo {
  virtual ~CKafraMoveMapServiceInfo() = default;

  void Build(std::string mapName);
  __declspec(noinline) void Update(UIRoMapWnd* wnd, float elapsed);

  /* 0004 */ std::vector<RoMapMarker> markers;
  /* 0010 */ RoMapMarker current;
  /* 004C */ std::string mapName;
  /* 0064 */ float blend;
};

class UIRoMapWnd : public UIFrameWnd {
  friend struct CKafraMoveMapServiceInfo;
  friend class CFlyingShipRouteInfo;

public:
  UIRoMapWnd();
  ~UIRoMapWnd() override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void ClearRecords();
  void ResetKafraBounds();
  __declspec(noinline) void UpdateNavigationButtons();
  void DrawHoverTooltip();
  void DrawMapNames();
  void DrawFieldLabels();
  void DrawRecordBorders();
  __declspec(noinline) void DrawDungeonLinks();
  __declspec(noinline) void DrawMarks();
  __declspec(noinline) void DrawRecordFaces(int unused);
  void DrawRecordAreas();
  void DrawSelectedMapPreview();
  void DrawKafraIcon(float x, float y);
  void DrawArrow(
    float x0, float y0, float x1, float y1, float startLeft, float startTop, float startRight, float startBottom, float endLeft, float endTop, float endRight,
    float endBottom, int bodyWidth, CTexture* body, int headWidth, CTexture* head, float progress, std::uint32_t color
  );
  void DrawDashedArrow(
    float startX, float startY, float endX, float endY, int stepWidth, int thickness, CTexture* body, int headSize, CTexture* head, float progress,
    std::uint32_t color, int offset
  );
  __declspec(noinline) void DrawKafraMarkers(float elapsed);
  __declspec(noinline) void DrawMapMarker(const RoMapMarker& marker, unsigned char skipSelected);
  void DrawWorld();
  __declspec(noinline) void RefreshPartyRecords();
  __declspec(noinline) bool LocateRecord(WorldViewRecord& record);

private:
  /* 00B4 */ std::list<WorldViewRecord> m_records;
  /* 00BC */ WorldViewRecord* hoveredRecord;
  /* 00C0 */ WorldViewRecord* selectedRecord;
  /* 00C4 */ RoMapTooltipBox* hoverTooltip;
  /* 00C8 */ int* previewArea;
  /* 00CC */ unsigned char unknown_00CC;
  /* 00D0 */ int previewTexture;
  /* 00D4 */ unsigned char m_showLabels;
  /* 00D8 */ std::uint32_t m_lastTime;
  /* 00DC */ unsigned char m_resetTime;
  /* 00E0 */ CKafraMoveMapServiceInfo m_kafraMarkers;
  /* 0148 */ CFlyingShipRouteInfo m_flyingShipRoute;
  /* 0198 */ CFlyingShipRouteInfo m_flyingShipRoute2;
  /* 01E8 */ int m_mouseX;
  /* 01EC */ int m_mouseY;
  /* 01F0 */ int m_comboHeight;
  /* 01F4 */ UIComboBoxDefWnd* m_worldCombo;
  /* 01F8 */ UICheckBox* m_labelCheck;
  /* 01FC */ std::vector<std::string> fieldNames;
  /* 0208 */ int selectedMapIndex;
  /* 020C */ UIBitmapButton* m_searchButton;
  /* 0210 */ UIBitmapButton* m_searchListButton;
  /* 0214 */ UIEditCtrl* m_searchEdit;
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
static_assert(sizeof(CKafraMoveMapServiceInfo) == 0x68);
static_assert(offsetof(CKafraMoveMapServiceInfo, markers) == 0x04);
#endif

#endif
