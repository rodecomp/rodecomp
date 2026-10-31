#include "UI/Window/UINavigationWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIListBoxNavi.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIRoMapWnd.h"

#include <cstdio>
#include <cstring>

// GLOBAL: gl_24609
bool g_navigationMinimized;

// GLOBAL: gl_24610
int g_navigationMode;

// GLOBAL: gl_24769
UIWindow* g_navigationWindow;

// GLOBAL: gl_24608
bool g_navigationHidden;

// OFFSET: 0x00931390
// FUNCTION: fn_12702
void UINavigationWnd::Slot31(int x, int y) {
  if (static_cast<unsigned int>(x - 8) <= 126 && static_cast<unsigned int>(y - 39) <= 126) {
    this->SendMsg(this, 6, 429, 0, 0, 0);
  }
  this->UIFrameWnd::Slot31(x, y);
}

// OFFSET: 0x0093DAB0
// FUNCTION: fn_12770
void UINavigationWnd::LayoutTabs() {
  this->m_routePanel->Move(-1000, -1000);
  this->m_searchPanel->Move(-1000, -1000);
  this->m_routeFooter->Move(-1000, -1000);
  this->m_searchFooter->Move(-1000, -1000);

  const int x = this->geometry.width - 222;
  if (!g_navigationMinimized) {
    UIWindow* footer;
    if (g_navigationTab == 1) {
      this->m_searchPanel->Move(x, 37);
      this->m_searchPanel->Invalidate();
      footer = this->m_searchFooter;
    } else {
      this->m_routePanel->Move(x, 37);
      this->m_routePanel->Invalidate();
      footer = this->m_routeFooter;
    }
    footer->Move(x, this->geometry.height - 41);
  }

  this->Invalidate();
}

// OFFSET: 0x0093DB80
// FUNCTION: fn_12771
void UINavigationWnd::RefreshHeaderTexts(int mode) {
  if (!g_navigationMinimized) {
    this->m_title = LookupMessage(0x8AD);
    this->m_subtitle = LookupMessage(0x8AC);
  } else if (mode == 2) {
    char text[260];
    char saved[260];
    std::memset(text, 0, sizeof(text));
    std::memset(saved, 0, sizeof(saved));
    if (g_navigationManager.SavedTargetText(saved, sizeof(saved))) {
      std::sprintf(text, ">>%s", saved);
    } else {
      std::sprintf(text, LookupMessage(0x8A9));
    }
    this->m_title = LookupMessage(0x8AA);
    this->m_subtitle = text;
  } else {
    this->m_title = LookupMessage(0x8AB);
    this->m_subtitle = LookupMessage(0x8AC);
  }

  this->Invalidate();
}

// OFFSET: 0x0093DD20
// FUNCTION: fn_12772
void UINavigationWnd::RefreshWorldMapMarks() {
  g_worldViewMgr.ClearMarks();
  if (g_navigationMode == 2) {
    std::string name;
    for (int index = 0; index < g_navigationManager.NodeCount(); ++index) {
      CNavi_Link* node = g_navigationManager.GetNode(index);
      if (node) {
        name = node->GetInfo()->GetName();
        name.append(".rsw", 4);
        g_worldViewMgr.AddMark(name.c_str());
      }
    }

    name = g_navigationManager.GetDestinationInfo()->GetName();
    name.append(".rsw", 4);
    g_worldViewMgr.AddMark(name.c_str());
    this->Invalidate();
  }
}

// OFFSET: 0x0093DED0
// FUNCTION: fn_12773
void UINavigationWnd::RefreshWorldMapSelection() {
  this->Invalidate();
  std::string name = g_navigationManager.GetCurrentTargetName();
  if (!name.empty()) {
    name.append(".rsw", 4);
    g_worldViewMgr.SelectField(name.c_str());
  } else {
    g_worldViewMgr.ResetSelection();
  }
}

// OFFSET: 0x0093DF80
// FUNCTION: fn_12774
void UINavigationWnd::RefreshWorldMapLevels() {
  g_worldViewMgr.ClearLevels();
  const int count = g_navigationManager.StateCount();
  if (count > 0) {
    std::string name;
    for (int index = 0; index < count; ++index) {
      int distance = 0;
      name = g_navigationManager.StateMapName(index, &distance);
      if (!name.empty()) {
        name.append(".rsw", 4);
        g_worldViewMgr.AddLevel(name.c_str(), distance);
      }
    }
    this->Invalidate();
  }
}

// OFFSET: 0x0093E150
// FUNCTION: fn_12775
void UINavigationWnd::RefreshRouteList() {
  this->m_searchPanel->Clear();
  this->m_searchPanel->ResetLineColors();
  this->m_searchPanel->Invalidate();

  const int nodeCount = g_navigationManager.NodeCount();
  if (g_navigationMode != 2) {
    return;
  }

  NavigationTarget target = g_navigationManager.GetSavedTarget();
  if (!target.object) {
    return;
  }

  this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(LookupMessage(0x8B2)), 0, 0);
  char text[100];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, LookupMessage(0x8B3), g_navigationManager.GetRouteResult(), nodeCount);
  this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

  int lineOffset;
  if (target.type == 2) {
    CNavi_Object* npc = static_cast<CNavi_Object*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(0x8B4), npc->GetInfo()->GetTitle(), npc->GetInfo()->GetName());
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

    std::memset(text, 0, sizeof(text));
    NaviCell cellX;
    NaviCell cellY;
    std::sprintf(text, LookupMessage(0x8B5), npc->GetName(), npc->GetCell(&cellX)->x, npc->GetCell(&cellY)->y);
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 5;
  } else if (target.type == 3) {
    CNavi_Object* mob = static_cast<CNavi_Object*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(0x8B4), mob->GetInfo()->GetTitle(), mob->GetInfo()->GetName());
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

    std::string kind;
    kind = LookupMessage(mob->GetType() == 301 ? 0x8B6 : 0x8B7);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(0x8B8), mob->GetName(), kind.c_str());
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 5;
  } else if (target.type == 1) {
    CNaviInfo* map = static_cast<CNaviInfo*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(0x8B4), map->GetTitle(), map->GetName());
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 4;
  } else if (target.type == 0) {
    CNaviInfo* map = static_cast<CNaviInfo*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(0x8B4), map->GetTitle(), map->GetName());
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

    std::memset(text, 0, sizeof(text));
    NaviCell cellX;
    NaviCell cellY;
    std::sprintf(text, LookupMessage(0x8B9), g_navigationManager.GetDestinationCell(&cellX)->x, g_navigationManager.GetDestinationCell(&cellY)->y);
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 5;
  } else {
    return;
  }

  this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(LookupMessage(0x8BA)), 0, 0);
  this->m_searchPanel->lineOffset = lineOffset;

  CNavi_Link* previous = nullptr;
  for (int index = 0; index < nodeCount; ++index) {
    std::memset(text, 0, 64);
    CNavi_Link* node = g_navigationManager.GetNode(index);
    NaviCell cellX;
    NaviCell cellY;
    if (index == 0) {
      if (node->GetType() == 400) {
        std::sprintf(text, LookupMessage(0x8BB), index, node->GetName(), node->GetLinkedName());
      } else {
        int column = 0;
        int row = 0;
        g_navigationManager.PlayerCell(&column, &row);
        std::sprintf(
          text, LookupMessage(0x8BC), index, node->GetInfo()->GetName(), column, row, node->GetCell(&cellX)->x, node->GetCell(&cellY)->y
        );
      }
    } else {
      std::sprintf(
        text,
        LookupMessage(0x8BC),
        index,
        node->GetInfo()->GetName(),
        previous->GetLinkedCell().x,
        previous->GetLinkedCell().y,
        node->GetCell(&cellX)->x,
        node->GetCell(&cellY)->y
      );
    }
    this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    previous = node;
  }

  const int savedType = g_navigationManager.GetSavedTarget().type;
  std::string destination = g_navigationManager.GetDestinationInfo()->GetName();
  int startX;
  int startY;
  NaviCell cell;
  if (nodeCount == 0) {
    g_navigationManager.PlayerCell(&startX, &startY);
  } else {
    startX = previous->GetLinkedCell().x;
    startY = previous->GetLinkedCell().y;
  }
  const int destinationX = g_navigationManager.GetDestinationCell(&cell)->x;
  const int destinationY = g_navigationManager.GetDestinationCell(&cell)->y;

  if (savedType == 2 || savedType == 0) {
    std::sprintf(text, LookupMessage(0x8BD), nodeCount, destination.c_str(), startX, startY, destinationX, destinationY);
  } else {
    std::sprintf(text, LookupMessage(0x8BE), nodeCount, destination.c_str());
  }
  this->SendMsg(nullptr, 31, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

  if (g_navigationManager.GetSelectedNode() != -1) {
    this->m_searchPanel->SetLineColor(g_navigationManager.GetSelectedNode(), 1);
    this->m_searchPanel->Invalidate();
  } else if (g_navigationManager.GetRouteFlag()) {
    this->m_searchPanel->SetLineColor(nodeCount, 1);
    this->m_searchPanel->Invalidate();
  }
}

// OFFSET: 0x0093EB60
// FUNCTION: fn_12776
void UINavigationWnd::ApplyMinimized() {
  if (g_navigationMinimized) {
    this->Resize(134, 61);
    int x = this->geometry.x;
    int y = this->geometry.y;
    if (x >= g_renderer->viewport.width - 272) {
      x = g_renderer->viewport.width - this->geometry.width;
    }
    if (y >= g_renderer->viewport.height - 338) {
      y = g_renderer->viewport.height - this->geometry.height;
    }
    this->Move(x < 0 ? 0 : x, y < 0 ? 0 : y);

    this->unknown_0128->Move(-1000, -1000);
    this->unknown_0120->Move(this->geometry.width - 42, 3);
    this->m_closeButton->Move(this->geometry.width - 14, 3);
    this->m_minimizeButton->Move(this->geometry.width - 28, 3);
    this->m_minimizeButton->SetTooltip(LookupMessage(0x89B));

    this->unknown_00FC->Move(-1000, -1000);
    this->unknown_0100->Move(-1000, -1000);
    this->m_routeFooter->Move(-1000, -1000);
    this->m_searchFooter->Move(-1000, -1000);
    this->unknown_010C->Move(-1000, -1000);
    this->unknown_0110->Move(-1000, -1000);
    this->LayoutTabs();

    this->unknown_00EC->Move(-1000, -1000);
    this->unknown_0114->Move(-1000, -1000);
    this->unknown_0118->Move(-1000, -1000);
    this->unknown_011C->Move(-1000, -1000);
    this->m_titleBar->Move(-1000, -1000);
  } else {
    if (this->m_saved->minimized) {
      this->m_saved->width = 272;
      this->m_titleBar->Resize(226, 18);
    } else {
      this->m_saved->width = 228;
      this->m_titleBar->Resize(182, 18);
    }
    this->Resize(this->m_saved->width, this->m_saved->height);

    int x = g_renderer->viewport.width - this->m_saved->width;
    if (x > this->geometry.x) {
      x = this->geometry.x;
    }
    int y = g_renderer->viewport.height - this->m_saved->height;
    if (y > this->geometry.y) {
      y = this->geometry.y;
    }
    this->Move(x < 0 ? 0 : x, y < 0 ? 0 : y);

    this->m_routePanel->Resize(215, this->geometry.height - 78);
    this->m_searchPanel->Resize(215, this->geometry.height - 78);
    this->unknown_0128->Move(this->geometry.width - 14, this->geometry.height - 14);
    this->unknown_0120->Move(this->geometry.width - 42, 3);
    this->m_closeButton->Move(this->geometry.width - 14, 3);
    this->m_minimizeButton->Move(this->geometry.width - 28, 3);
    this->m_minimizeButton->SetTooltip(LookupMessage(0x89C));

    this->unknown_00FC->Move(this->geometry.width - 66, 18);
    this->unknown_0100->Move(this->geometry.width - 77, this->geometry.height - 37);
    this->m_routeFooter->Move(-1000, -1000);
    this->m_searchFooter->Move(-1000, -1000);
    this->unknown_010C->Move(this->geometry.width - 224, this->geometry.height - 22);
    this->unknown_0110->Move(this->geometry.width - 31, 18);
    this->LayoutTabs();

    this->unknown_00EC->Move(6, 18);
    this->unknown_0114->Move(this->geometry.width - 136, this->geometry.height - 37);
    this->unknown_0118->Move(this->geometry.width - 186, this->geometry.height - 37);
    this->unknown_011C->Move(this->geometry.width - 136, this->geometry.height - 21);
    this->m_titleBar->Move(65, 18);
  }
}

// OFFSET: 0x00940E20
// FUNCTION: fn_12781
void UINavigationWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    const int maxX = g_renderer->viewport.width - (this->m_saved->minimized ? 272 : 228);
    if (this->geometry.x > maxX) {
      this->geometry.x = maxX;
    }
    if (this->geometry.x < 0) {
      this->geometry.x = 0;
    }
    this->m_saved->x = this->geometry.x;
    const int maxY = g_renderer->viewport.height - 338;
    if (this->geometry.y > maxY) {
      this->geometry.y = maxY;
    }
    if (this->geometry.y < 0) {
      this->geometry.y = 0;
    }
    this->m_saved->y = this->geometry.y;
  }
}
