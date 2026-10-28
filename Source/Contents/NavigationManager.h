#ifndef RAGNAROK_SOURCE_CONTENTS_NAVIGATIONMANAGER_H
#define RAGNAROK_SOURCE_CONTENTS_NAVIGATIONMANAGER_H

#include "Contents/MovementRouteInterpolation.h"
#include "Contents/TerrainPathSearch.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class CActRes;
class CNaviInfo;
class CNavi_Link;
class CNavi_Object;
class CSprRes;
class CTexture;
class GroundGeometry;

struct NaviCell {
  /* 0000 */ int x;
  /* 0004 */ int y;
};

struct NaviLink {
  /* 0000 */ CNavi_Link* link;
  /* 0004 */ int cost;
};

struct NaviDataEntry {
  /* 0000 */ int previous;
  /* 0004 */ CNavi_Link* link;
  /* 0008 */ int cost;
};

struct NaviRouteLine {
  /* 0000 */ int kind;
  /* 0004 */ int x;
  /* 0008 */ int y;
  /* 000C */ std::string text;
  /* 0024 */ int color;
  /* 0028 */ int shadowColor;
};

// VTABLE: vt_01777
class CNavi_Object {
public:
  /* 0004 */ int id;
  /* 0008 */ int type;
  union {
    /* 000C */ int index;
    struct {
      /* 000C */ std::uint16_t shortIndex;
      /* 000E */ std::uint16_t flag;
    };
  };
  /* 0010 */ std::string name;
  /* 0028 */ std::string title;
  /* 0040 */ CNaviInfo* info;
  /* 0044 */ int cellX;
  /* 0048 */ int cellY;
  /* 004C */ std::vector<NaviLink> links;
  /* 0058 */ int unknown_0058;
  /* 005C */ int unknown_005C;
  /* 0060 */ int unknown_0060;
  /* 0064 */ int unknown_0064;
  /* 0068 */ int unknown_0068;
  /* 006C */ int unknown_006C;
  /* 0070 */ int unknown_0070;
  /* 0074 */ int unknown_0074;
  /* 0078 */ int unknown_0078;

  virtual ~CNavi_Object();
  virtual int GetId() const;
  virtual int GetType() const;
  virtual int GetIndex() const;
  virtual std::uint16_t GetFlag() const;
  virtual const char* GetName() const;
  virtual const char* GetTitle() const;
  virtual NaviCell* GetCell(NaviCell* out) const;
  virtual CNaviInfo* GetInfo() const;
};

// VTABLE: vt_01778
class CNavi_Link : public CNavi_Object {
public:
  /* 007C */ CNaviInfo* linkedInfo;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int depth;
  /* 008C */ std::vector<NaviLink> linkedLinks;

  CNaviInfo* GetLinkedInfo() const;
  void SetDepth(int depth);
  std::vector<NaviLink>::iterator LinkedBegin();
  std::vector<NaviLink>::iterator LinkedEnd();

  ~CNavi_Link() override;
  int GetId() const override;
  int GetType() const override;
  int GetIndex() const override;
  const char* GetName() const override;
  const char* GetTitle() const override;
  NaviCell* GetCell(NaviCell* out) const override;
  CNaviInfo* GetInfo() const override;
};

class CNaviInfo {
public:
  ~CNaviInfo();

  bool HasName(const char* name) const;
  const char* GetName() const;
  const char* GetTitle() const;
  bool IsVisited() const;
  void SetVisited(bool visited);
  std::vector<CNavi_Link*>::iterator LinksBegin();
  std::vector<CNavi_Link*>::iterator LinksEnd();

  /* 0000 */ int index;
  /* 0004 */ int type;
  /* 0008 */ std::string name;
  /* 0020 */ std::string title;
  /* 0038 */ int cellX;
  /* 003C */ int cellY;
  /* 0040 */ bool visited;
  /* 0044 */ std::vector<CNavi_Object*> npcs;
  /* 0050 */ std::vector<CNavi_Object*> mobs;
  /* 005C */ std::vector<CNavi_Link*> links;
};

struct NavigationTarget {
  NavigationTarget() : type(-1), object(nullptr), x(-1), y(-1) {}
  ~NavigationTarget();

  /* 0000 */ int type;
  /* 0004 */ void* object;
  /* 0008 */ int x;
  /* 000C */ int y;
  /* 0010 */ std::string name;
};

class CNaviData {
public:
  CNaviData();
  ~CNaviData();

  bool SetDestination(CNaviInfo* info, int cellX, int cellY);
  CNaviInfo* GetInfo();
  NaviCell* GetCell(NaviCell* out);
  void Clear();
  int FindRoute(std::vector<CNavi_Link*>* route, bool direct, bool allowGates, bool includeObjects, bool allowWarps, int side);
  void ResetLinkDepths();

  /* 0000 */ std::vector<CNaviInfo*> destinations;
  /* 000C */ std::vector<CNavi_Link*> objects;
  /* 0018 */ CNaviInfo* info;
  /* 001C */ int cellX;
  /* 0020 */ int cellY;
  /* 0024 */ int range;
  /* 0028 */ std::vector<NaviDataEntry> entries;
  /* 0034 */ std::vector<NaviLink> startLinks;
  /* 0040 */ std::vector<NaviLink> endLinks;
};

class CNavigationManager {
public:
  CNavigationManager();
  ~CNavigationManager();

  NavigationTarget GetCurrentTarget() const;
  std::string GetCurrentTargetName() const;
  NavigationTarget GetSavedTarget() const;
  CNaviInfo* GetDestinationInfo();
  NaviCell* GetDestinationCell(NaviCell* out);
  void ClearData();
  bool IsCurrentTarget(const char* name) const;
  int GetCurrentTargetType() const;
  int SaveCurrentTarget();
  CNaviInfo* FindObject(const char* name) const;
  std::string SelectedNodeText();
  std::string SubDestinationText();
  std::string DestinationText();
  std::string DestinationTitle();
  float FitToSquare(int* offsetX, int* offsetY, int width, int height, float size) const;
  void LoadSettings();
  void Reset(bool clearAll);
  void ClearRouteNodes();
  void ClearNodePaths();
  CNaviInfo* GetLinkedPlayerInfo();
  bool UpdateGround();
  bool PlayerCell(int* column, int* row);
  bool LoadStates(std::vector<char> packet);

  /* 0000 */ CNaviData data;
  /* 004C */ CPathFinder pathFinder;
  /* 10F0 */ std::vector<NavigationTarget> states;
  /* 10FC */ NavigationTarget currentTarget;
  /* 1124 */ NavigationTarget savedTarget;
  /* 114C */ int routeResult;
  /* 1150 */ std::vector<CNavi_Object*> nodes;
  /* 115C */ bool unknown_115C;
  /* 1160 */ int selectedNode;
  /* 1164 */ MovementRouteInterpolation movementRoute;
  /* 117C */ bool unknown_117C;
  /* 1180 */ std::string labels[8];
  /* 1240 */ GroundGeometry* ground;
  /* 1244 */ CNaviInfo* unknown_1244;
  /* 1248 */ bool unknown_1248;
  /* 124C */ int unknown_124C;
  /* 1250 */ int unknown_1250;
  /* 1254 */ bool unknown_1254;
  /* 1258 */ int unknown_1258;
  /* 125C */ int unknown_125C;
  /* 1260 */ int unknown_1260;
  /* 1264 */ std::string unknown_1264;
  /* 127C */ bool unknown_127C;
  /* 127D */ bool unknown_127D;
  /* 127E */ bool unknown_127E;
  /* 1280 */ int unknown_1280;
  /* 1284 */ int unknown_1284;
  /* 1288 */ CTexture* unknown_1288;
  /* 128C */ int unknown_128C;
  /* 1290 */ int unknown_1290;
  /* 1294 */ std::vector<NaviCell> cells;
  /* 12A0 */ CActRes* unknown_12A0;
  /* 12A4 */ CSprRes* unknown_12A4;
  /* 12A8 */ std::vector<NaviRouteLine> routeLines;
  /* 12B4 */ bool unknown_12B4;
  /* 12B8 */ std::vector<std::vector<int>> nodePaths;
  /* 12C4 */ int unknown_12C4;
  /* 12C8 */ int unknown_12C8;
  /* 12CC */ std::string destinationTitle;
  /* 12E4 */ std::string destinationText;
  /* 12FC */ std::string routeText;
  /* 1314 */ std::string selectedNodeText;
  /* 132C */ std::string subDestinationText;
  /* 1344 */ int viewIcon;
};

extern CNavigationManager g_navigationManager;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CNavi_Link) == 0x98);
static_assert(offsetof(CNavi_Link, linkedInfo) == 0x7C);
static_assert(offsetof(CNavi_Link, depth) == 0x88);
static_assert(offsetof(CNaviInfo, visited) == 0x40);
static_assert(offsetof(NaviDataEntry, cost) == 0x08);
static_assert(offsetof(CNavi_Link, linkedLinks) == 0x8C);
static_assert(sizeof(NaviCell) == 0x08);
static_assert(sizeof(NaviDataEntry) == 0x0C);
static_assert(sizeof(NaviRouteLine) == 0x2C);
static_assert(offsetof(NaviRouteLine, text) == 0x0C);
static_assert(sizeof(NavigationTarget) == 0x28);
static_assert(offsetof(NavigationTarget, object) == 0x04);
static_assert(offsetof(NavigationTarget, name) == 0x10);
static_assert(sizeof(CNavi_Object) == 0x7C);
static_assert(offsetof(CNavi_Object, id) == 0x04);
static_assert(offsetof(CNavi_Object, name) == 0x10);
static_assert(offsetof(CNavi_Object, title) == 0x28);
static_assert(offsetof(CNavi_Object, info) == 0x40);
static_assert(offsetof(CNavi_Object, cellX) == 0x44);
static_assert(offsetof(CNavi_Object, links) == 0x4C);
static_assert(sizeof(CNaviInfo) == 0x68);
static_assert(offsetof(CNaviInfo, name) == 0x08);
static_assert(offsetof(CNaviInfo, title) == 0x20);
static_assert(offsetof(CNaviInfo, cellX) == 0x38);
static_assert(offsetof(CNaviInfo, npcs) == 0x44);
static_assert(offsetof(CNaviInfo, links) == 0x5C);
static_assert(sizeof(CNaviData) == 0x4C);
static_assert(offsetof(CNaviData, info) == 0x18);
static_assert(offsetof(CNaviData, cellX) == 0x1C);
static_assert(offsetof(CNaviData, range) == 0x24);
static_assert(offsetof(CNaviData, entries) == 0x28);
static_assert(offsetof(CNaviData, startLinks) == 0x34);
static_assert(offsetof(CNaviData, endLinks) == 0x40);
static_assert(offsetof(CNavigationManager, pathFinder) == 0x004C);
static_assert(offsetof(CNavigationManager, states) == 0x10F0);
static_assert(offsetof(CNavigationManager, currentTarget) == 0x10FC);
static_assert(offsetof(CNavigationManager, savedTarget) == 0x1124);
static_assert(offsetof(CNavigationManager, routeResult) == 0x114C);
static_assert(offsetof(CNavigationManager, nodes) == 0x1150);
static_assert(offsetof(CNavigationManager, unknown_115C) == 0x115C);
static_assert(offsetof(CNavigationManager, selectedNode) == 0x1160);
static_assert(offsetof(CNavigationManager, movementRoute) == 0x1164);
static_assert(offsetof(CNavigationManager, unknown_117C) == 0x117C);
static_assert(offsetof(CNavigationManager, labels) == 0x1180);
static_assert(offsetof(CNavigationManager, ground) == 0x1240);
static_assert(offsetof(CNavigationManager, unknown_1244) == 0x1244);
static_assert(offsetof(CNavigationManager, unknown_1248) == 0x1248);
static_assert(offsetof(CNavigationManager, unknown_1258) == 0x1258);
static_assert(offsetof(CNavigationManager, unknown_1260) == 0x1260);
static_assert(offsetof(CNavigationManager, unknown_1264) == 0x1264);
static_assert(offsetof(CNavigationManager, unknown_127C) == 0x127C);
static_assert(offsetof(CNavigationManager, unknown_1280) == 0x1280);
static_assert(offsetof(CNavigationManager, unknown_1288) == 0x1288);
static_assert(offsetof(CNavigationManager, cells) == 0x1294);
static_assert(offsetof(CNavigationManager, unknown_12A0) == 0x12A0);
static_assert(offsetof(CNavigationManager, routeLines) == 0x12A8);
static_assert(offsetof(CNavigationManager, unknown_12B4) == 0x12B4);
static_assert(offsetof(CNavigationManager, nodePaths) == 0x12B8);
static_assert(offsetof(CNavigationManager, unknown_12C4) == 0x12C4);
static_assert(offsetof(CNavigationManager, destinationTitle) == 0x12CC);
static_assert(offsetof(CNavigationManager, destinationText) == 0x12E4);
static_assert(offsetof(CNavigationManager, routeText) == 0x12FC);
static_assert(offsetof(CNavigationManager, selectedNodeText) == 0x1314);
static_assert(offsetof(CNavigationManager, subDestinationText) == 0x132C);
static_assert(offsetof(CNavigationManager, viewIcon) == 0x1344);
static_assert(sizeof(CNavigationManager) == 0x1348);
#endif

#endif
