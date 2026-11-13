#include "UI/Navigation/UINavigationWnd.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Application/Launcher.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Navigation/NavigationManager.h"
#include "File/FileEnumeration.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/Texture.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Navigation/UIListBoxNavi.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIResizer.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>

// GLOBAL: gl_25083
float g_navigationPointScale = 0.0078125f;

// GLOBAL: gl_25084
float g_navigationMarkerScale = 0.453125f;

// OFFSET: 0x00921A70
// FUNCTION: fn_12643
void UINavigationWnd::OnCreate(int width, int height) {
  this->m_routePanel = new UIListBoxNavi();
  this->m_routePanel->SetSize(215, 96);
  this->m_routePanel->backgroundRed = 210;
  this->m_routePanel->backgroundGreen = 210;
  this->m_routePanel->backgroundBlue = 210;
  this->m_routePanel->SetId(336);
  this->m_routePanel->Move(-1000, -1000);
  this->AddChild(this->m_routePanel);

  this->m_searchPanel = new UIListBoxNavi();
  this->m_searchPanel->SetSize(215, 96);
  this->m_searchPanel->backgroundRed = 210;
  this->m_searchPanel->backgroundGreen = 210;
  this->m_searchPanel->backgroundBlue = 210;
  this->m_searchPanel->SetId(441);
  this->m_searchPanel->Move(-1000, -1000);
  this->m_searchPanel->SetDefaultLineColor(0x1414F0);
  this->AddChild(this->m_searchPanel);

  std::string base = "유저인터페이스\\";

  this->m_resizer = new UIResizer();
  this->m_resizer->SetSize(14, 14);
  this->m_resizer->SetDirection(7);
  this->m_resizer->SetBitmapName((base + "btn_resize.bmp").c_str());
  this->m_resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->m_resizer->Move(-1000, -1000);
  this->AddChild(this->m_resizer);

  std::string path = base;
  path += "navigation_interface\\";

  const char* tooltips[6] = {
    LookupMessage(MSI_NAVIGATION_SEARCHINFO), LookupMessage(MSI_NAVIGATION_VIEWCHANGE), LookupMessage(MSI_NAVIGATION_LIGHTCHANGE), LookupMessage(MSI_NAVIGATION_END), LookupMessage(MSI_NAVIGATION_BASE), LookupMessage(MSI_NAVIGATION_MINI),
  };
  const char* names[6] = {"NaviSearch", "NaviStart", "NaviRoute", "NaviList", "Navisyschange2", "navifolder"};
  const int ids[6] = {321, 184, 189, 190, 284, 191};

  for (int index = 0; index < 6; ++index) {
    this->m_buttons[index] = new UIBitmapButton();
    this->m_buttons[index]->SetBitmap((path + names[index] + "_A.bmp").c_str(), 0, 0);
    this->m_buttons[index]->SetBitmap((path + names[index] + "_B.bmp").c_str(), 1, 0);
    this->m_buttons[index]->SetBitmap((path + names[index] + "_C.bmp").c_str(), 2, 0);
    this->m_buttons[index]->SetSize(this->m_buttons[index]->BitmapWidth(), this->m_buttons[index]->BitmapHeight());
    this->m_buttons[index]->Move(-1000, -1000);
    this->m_buttons[index]->SetId(ids[index]);
    this->m_buttons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->m_buttons[index]);
  }

  this->m_closeButton = this->AddCloseButton(LookupMessage(MSI_NAVIGATION_HELP), 0);
  this->m_minimizeButton = this->AddMinimizeButton("", 0);

  this->m_helpButton = new UIBitmapButton();
  this->m_helpButton->SetBitmap((path + "NaviHelp_off.bmp").c_str(), 0, 0);
  this->m_helpButton->SetBitmap((path + "NaviHelp_on.bmp").c_str(), 1, 0);
  this->m_helpButton->SetBitmap((path + "NaviHelp_on.bmp").c_str(), 2, 0);
  this->m_helpButton->SetSize(this->m_helpButton->BitmapWidth(), this->m_helpButton->BitmapHeight());
  this->m_helpButton->Move(this->geometry.width - 42, 3);
  this->m_helpButton->SetId(343);
  this->m_helpButton->SetTooltip(LookupMessage(MSI_NAVIGATION_NPC));
  this->AddChild(this->m_helpButton);

  this->m_searchType = new UIComboBoxDefWnd();
  this->m_searchType->dropdownHeight = 67;
  this->m_searchType->SetSize(56, 18);
  this->m_searchType->Move(-1000, -1000);
  this->m_searchType->SetBorderColor(100, 100, 100);
  this->m_searchType->SetColor(232, 232, 232);
  this->AddChild(this->m_searchType);
  this->m_searchType->AddOption(LookupMessage(MSI_NAVIGATION_MOB), 0);
  this->m_searchType->AddOption(LookupMessage(MSI_NAVIGATION_SETSTRING), 0);
  this->m_searchType->AddOption(LookupMessage(MSI_NAVIGATION_SCROLL), 0);
  this->m_searchType->AddOption(LookupMessage(MSI_NAVIGATION_USESCROLL), 0);

  this->m_titleBar = new UIEditCtrl();
  this->m_titleBar->SetSize(226, 18);
  this->m_titleBar->Move(-1000, -1000);
  this->m_titleBar->maximum = 60;
  this->m_titleBar->Background(232, 232, 232);
  this->m_titleBar->Border(200, 100, 100);
  this->m_titleBar->flat = 0;
  UIEditCtrl* edit = this->m_titleBar;
  const char* tooltip = LookupMessage(MSI_NAVIGATION_SERVICE);
  if (tooltip) {
    edit->tooltip = tooltip;
  }
  this->AddChild(this->m_titleBar);
  if (g_replayAnimationScaling) {
    UIEditCtrl* titleBar = this->m_titleBar;
    titleBar->disabled = 1;
    titleBar->Invalidate();
  }

  this->m_includeObjects = new UICheckBox();
  this->m_includeObjects->SetSize(this->TextWidth(LookupMessage(MSI_NAVIGATION_USESERVICE), 10, 0, 12, 0, 0), 12);
  path = base;
  this->m_includeObjects->SetImages((path + "checkbox_1.bmp").c_str(), (path + "checkbox_0.bmp").c_str(), LookupMessage(MSI_NAVIGATION_USESERVICE));
  this->m_includeObjects->Move(-1000, -1000);
  this->m_includeObjects->SetChecked(g_navigationManager.includeObjects);
  this->m_includeObjects->SetTooltip(LookupMessage(MSI_NAVIGATION_PLANE));
  this->AddChild(this->m_includeObjects);

  this->m_allowGates = new UICheckBox();
  this->m_allowGates->SetSize(this->TextWidth(LookupMessage(MSI_NAVIGATION_USEPLANE), 10, 0, 12, 0, 0), 12);
  path = base;
  this->m_allowGates->SetImages((path + "checkbox_1.bmp").c_str(), (path + "checkbox_0.bmp").c_str(), LookupMessage(MSI_NAVIGATION_USEPLANE));
  this->m_allowGates->Move(-1000, -1000);
  this->m_allowGates->SetChecked(g_navigationManager.allowGates);
  this->m_allowGates->SetTooltip(LookupMessage(MSI_NAVIGATION_TARGETUNKNOWN));
  this->AddChild(this->m_allowGates);

  this->m_allowWarps = new UICheckBox();
  this->m_allowWarps->SetSize(this->TextWidth(LookupMessage(MSI_NAVIGATION_GOING), 10, 0, 12, 0, 0), 12);
  path = base;
  this->m_allowWarps->SetImages((path + "checkbox_1.bmp").c_str(), (path + "checkbox_0.bmp").c_str(), LookupMessage(MSI_NAVIGATION_GOING));
  this->m_allowWarps->Move(-1000, -1000);
  this->m_allowWarps->SetChecked(g_navigationManager.allowWarps);
  this->m_allowWarps->SetTooltip(LookupMessage(MSI_NAVIGATION_SEARCHING));
  this->AddChild(this->m_allowWarps);
}

// OFFSET: 0x00931390
// FUNCTION: fn_12702
void UINavigationWnd::OnLBtnUp(int x, int y) {
  if (static_cast<unsigned int>(x - 8) <= 126 && static_cast<unsigned int>(y - 39) <= 126) {
    this->SendMsg(this, UM_COMMAND, 429, 0, 0, 0);
  }
  this->UIFrameWnd::OnLBtnUp(x, y);
}

// OFFSET: 0x0093DAB0
// FUNCTION: fn_12770
void UINavigationWnd::LayoutTabs() {
  this->m_routePanel->Move(-1000, -1000);
  this->m_searchPanel->Move(-1000, -1000);
  this->m_routeFooter->Move(-1000, -1000);
  this->m_searchFooter->Move(-1000, -1000);

  const int x = this->geometry.width - 222;
  if (!g_navigationManager.expandedMap) {
    UIWindow* footer;
    if (g_navigationManager.displayMode == 1) {
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
  if (g_navigationManager.expandedMap) {
    if (mode == 2) {
      char text[260];
      char saved[260];
      std::memset(text, 0, sizeof(text));
      std::memset(saved, 0, sizeof(saved));
      if (g_navigationManager.SavedTargetText(saved, sizeof(saved))) {
        std::sprintf(text, ">>%s", saved);
      } else {
        std::sprintf(text, LookupMessage(MSI_NAVIGATION_UN));
      }
      this->m_title = LookupMessage(MSI_NAVIGATION_NAVI);
      this->m_subtitle = text;
    } else {
      this->m_title = LookupMessage(MSI_NAVIGATION_SEARCHRESULT_D);
      this->m_subtitle = LookupMessage(MSI_NAVIGATION_NPC_S_S);
    }
  } else {
    this->m_title = LookupMessage(MSI_NAVIGATION_MOB_S_S);
    this->m_subtitle = LookupMessage(MSI_NAVIGATION_NPC_S_S);
  }

  this->Invalidate();
}

// OFFSET: 0x0093DD20
// FUNCTION: fn_12772
void UINavigationWnd::RefreshWorldMapMarks() {
  g_worldViewMgr.ClearMarks();
  if (g_navigationManager.viewMode == 2) {
    std::string name;
    for (int index = 0; index < g_navigationManager.NodeCount(); ++index) {
      CNavi_Link* node = g_navigationManager.GetNode(index);
      if (node) {
        name = node->GetInfo()->GetName();
        name += ".rsw";
        g_worldViewMgr.AddMark(name.c_str());
      }
    }

    name = g_navigationManager.GetDestinationInfo()->GetName();
    name += ".rsw";
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
        name += ".rsw";
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
  if (g_navigationManager.viewMode != 2) {
    return;
  }

  NavigationTarget target = g_navigationManager.GetSavedTarget();
  if (!target.object) {
    return;
  }

  this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_TARGET_S_D_D)), 0, 0);
  char text[100];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, LookupMessage(MSI_NAVIGATION_BOSS), g_navigationManager.GetRouteResult(), nodeCount);
  this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

  int lineOffset;
  if (target.type == 2) {
    CNavi_Object* npc = static_cast<CNavi_Object*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_NORMAL), npc->GetInfo()->GetTitle(), npc->GetInfo()->GetName());
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

    std::memset(text, 0, sizeof(text));
    NaviCell cellX;
    NaviCell cellY;
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_TARGET_S_S), npc->GetName(), npc->GetCell(&cellX)->x, npc->GetCell(&cellY)->y);
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 5;
  } else if (target.type == 3) {
    CNavi_Object* mob = static_cast<CNavi_Object*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_NORMAL), mob->GetInfo()->GetTitle(), mob->GetInfo()->GetName());
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

    std::string kind;
    kind = LookupMessage(mob->GetType() == 301 ? 0x8B6 : 0x8B7);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_ITEM_S_S), mob->GetName(), kind.c_str());
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 5;
  } else if (target.type == 1) {
    CNaviInfo* map = static_cast<CNaviInfo*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_NORMAL), map->GetTitle(), map->GetName());
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 4;
  } else if (target.type == 0) {
    CNaviInfo* map = static_cast<CNaviInfo*>(target.object);
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_NORMAL), map->GetTitle(), map->GetName());
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

    std::memset(text, 0, sizeof(text));
    NaviCell cellX;
    NaviCell cellY;
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_D_S_D_D_D_D), g_navigationManager.GetDestinationCell(&cellX)->x, g_navigationManager.GetDestinationCell(&cellY)->y);
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    lineOffset = 5;
  } else {
    return;
  }

  this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_E_D_S_D_D_D_D)), 0, 0);
  this->m_searchPanel->SetLineOffset(lineOffset);

  CNavi_Link* previous = nullptr;
  for (int index = 0; index < nodeCount; ++index) {
    std::memset(text, 0, 64);
    CNavi_Link* node = g_navigationManager.GetNode(index);
    NaviCell cellX;
    NaviCell cellY;
    if (index == 0) {
      if (node->GetType() == 400) {
        std::sprintf(text, LookupMessage(MSI_NAVIGATION_E_D_S), index, node->GetName(), node->GetLinkedName());
      } else {
        int column = 0;
        int row = 0;
        g_navigationManager.PlayerCell(&column, &row);
        std::sprintf(
          text, LookupMessage(MSI_NAVIGATION_GOEXIT), index, node->GetInfo()->GetName(), column, row, node->GetCell(&cellX)->x, node->GetCell(&cellY)->y
        );
      }
    } else {
      std::sprintf(
        text,
        LookupMessage(MSI_NAVIGATION_GOEXIT),
        index,
        node->GetInfo()->GetName(),
        previous->GetLinkedCell().x,
        previous->GetLinkedCell().y,
        node->GetCell(&cellX)->x,
        node->GetCell(&cellY)->y
      );
    }
    this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);
    previous = node;
  }

  const int savedType = g_navigationManager.GetSavedTarget().type;
  std::string destination = g_navigationManager.GetDestinationInfo()->GetName();
  int startX;
  int startY;
  NaviCell cell;
  int destinationX;
  if (nodeCount == 0) {
    g_navigationManager.PlayerCell(&startX, &startY);
    destinationX = g_navigationManager.GetDestinationCell(&cell)->x;
  } else {
    startX = previous->GetLinkedCell().x;
    startY = previous->GetLinkedCell().y;
    destinationX = g_navigationManager.GetDestinationCell(&cell)->x;
  }
  const int destinationY = g_navigationManager.GetDestinationCell(&cell)->y;

  if (savedType == 2 || savedType == 0) {
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_HELPTITLE), nodeCount, destination.c_str(), startX, startY, destinationX, destinationY);
  } else {
    std::sprintf(text, LookupMessage(MSI_NAVIGATION_HELP1), nodeCount, destination.c_str());
  }
  this->SendMsg(nullptr, UM_ADDITEM, 441, reinterpret_cast<std::intptr_t>(text), 0, 0);

  if (g_navigationManager.GetSelectedNode() == -1) {
    if (g_navigationManager.GetRouteFlag()) {
      this->m_searchPanel->SetLineColor(nodeCount, 1);
      this->m_searchPanel->Invalidate();
    }
  } else {
    this->m_searchPanel->SetLineColor(g_navigationManager.GetSelectedNode(), 1);
    this->m_searchPanel->Invalidate();
  }
}

// OFFSET: 0x0093EB60
// FUNCTION: fn_12776
void UINavigationWnd::ApplyMinimized() {
  if (g_navigationManager.expandedMap) {
    this->Resize(134, 61);
    int x = this->geometry.x;
    int y = this->geometry.y;
    if (g_renderer->viewport.width - 272 <= x) {
      x = g_renderer->viewport.width - this->geometry.width;
    }
    if (g_renderer->viewport.height - 338 <= y) {
      y = g_renderer->viewport.height - this->geometry.height;
    }
    this->Move(std::ranges::max(x, 0), std::ranges::max(y, 0));

    this->m_resizer->Move(-1000, -1000);
    this->m_helpButton->Move(this->geometry.width - 42, 3);
    this->m_closeButton->Move(this->geometry.width - 14, 3);
    this->m_minimizeButton->Move(this->geometry.width - 28, 3);
    this->m_minimizeButton->SetTooltip(LookupMessage(MSI_NAVIGATION_ALL));

    for (int index = 0; index < 6; ++index) {
      this->m_buttons[index]->Move(-1000, -1000);
    }
    this->LayoutTabs();

    this->m_searchType->Move(-1000, -1000);
    this->m_allowGates->Move(-1000, -1000);
    this->m_includeObjects->Move(-1000, -1000);
    this->m_allowWarps->Move(-1000, -1000);
    this->m_titleBar->Move(-1000, -1000);
  } else {
    if (this->m_saved->expanded) {
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
    this->Move(std::ranges::max(x, 0), std::ranges::max(y, 0));

    this->m_routePanel->Resize(215, this->geometry.height - 78);
    this->m_searchPanel->Resize(215, this->geometry.height - 78);
    this->m_resizer->Move(this->geometry.width - 14, this->geometry.height - 14);
    this->m_helpButton->Move(this->geometry.width - 42, 3);
    this->m_closeButton->Move(this->geometry.width - 14, 3);
    this->m_minimizeButton->Move(this->geometry.width - 28, 3);
    this->m_minimizeButton->SetTooltip(LookupMessage(MSI_NAVIGATION_MAP));

    int buttonX[6] = {this->geometry.width - 66, this->geometry.width - 77, -1000, -1000, this->geometry.width - 224,
                      this->geometry.width - 31};
    int buttonY[6] = {18, this->geometry.height - 37, -1000, -1000, this->geometry.height - 22, 18};
    for (int index = 0; index < 6; ++index) {
      this->m_buttons[index]->Move(buttonX[index], buttonY[index]);
    }
    this->LayoutTabs();

    this->m_searchType->Move(6, 18);
    this->m_allowGates->Move(this->geometry.width - 136, this->geometry.height - 37);
    this->m_includeObjects->Move(this->geometry.width - 186, this->geometry.height - 37);
    this->m_allowWarps->Move(this->geometry.width - 136, this->geometry.height - 21);
    this->m_titleBar->Move(65, 18);
  }
}

// OFFSET: 0x00940E20
// FUNCTION: fn_12781
void UINavigationWnd::StoreInfo() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    const int maxX = g_renderer->viewport.width - (!this->m_saved->expanded ? 228 : 272);
    if (maxX < this->geometry.x) {
      this->geometry.x = maxX;
    }
    if (this->geometry.x < 0) {
      this->geometry.x = 0;
    }
    this->m_saved->x = this->geometry.x;
    const int maxY = g_renderer->viewport.height - 338;
    if (maxY < this->geometry.y) {
      this->geometry.y = maxY;
    }
    if (this->geometry.y < 0) {
      this->geometry.y = 0;
    }
    this->m_saved->y = this->geometry.y;
  }
}

// OFFSET: 0x0092CEF0
// FUNCTION: fn_12668
void UINavigationWnd::Paint() {
  this->DrawTitleBar(true, this->m_title.c_str(), 0);
  this->DrawPanelBackground(0x11, 0, "");
  if (g_navigationManager.expandedMap) {
    this->DrawTextAt(6, 20, this->m_subtitle.c_str(), 0, 0, 12, 0, 0, 0);
    return;
  }

  if (!this->m_saved->expanded) {
    return;
  }

  int mapCellY = g_navigationManager.previewY;
  int mapCellX = g_navigationManager.previewX;
  const int targetType = g_navigationManager.GetCurrentTarget().type;
  int size = 0x80;
  this->FillRegion(6, 0x25, 0x82, 0x82, 0x646464);

  if (g_navigationManager.previewType == 2) {
    int markerSize = size;
    if (targetType == 2) {
      markerSize = size - 0x3D;
      size = 0x3A;
      if (mapCellX > -1 && mapCellY > -1) {
        mapCellX = static_cast<int>(static_cast<float>(mapCellX) * g_navigationMarkerScale);
        mapCellY = static_cast<int>(static_cast<float>(mapCellY) * g_navigationMarkerScale);
      }
    } else if (targetType == 3) {
      size = 0x20;
    }
    this->DrawActionSprite(g_navigationManager.previewSprite, g_navigationManager.previewAction, 7, 0x26, markerSize, 0x80, 0x5dc, true, nullptr);
  }

  const int mapX = 0x87 - size;
  const int mapY = 0xA6 - size;
  this->FillRegion(mapX, mapY, size, size, 0);
  if (g_navigationManager.previewTexture != nullptr) {
    const float scale = static_cast<float>(size) / static_cast<float>(g_navigationManager.previewTexture->Width());
    this->geometry.surface->DrawScaledSurface(
      mapX, mapY, size, size, g_navigationManager.previewTexture, g_navigationManager.previewTexture->Width(), g_navigationManager.previewTexture->Height(), scale, scale, 0xFFFFFFFFu, false, 0, 0, 0,
      0
    );
  }

  bool drawCrosshair = true;
  if (g_navigationManager.viewMode == 2) {
    if (g_navigationManager.IsCurrentTarget(g_session.m_navigationPlayerMap)) {
      if (g_navigationManager.displayMode == 1) {
        drawCrosshair = false;
      }
      const float scale = static_cast<float>(size) * g_navigationPointScale;
      for (const NaviCell& point : g_navigationManager.cells) {
        this->FillRegion(static_cast<int>(point.x * scale + mapX), static_cast<int>(point.y * scale + mapY), 2, 2, 0x00F0F00A);
      }

      int playerCellX = 0;
      int playerCellY = 0;
      if (g_navigationManager.PlayerCell(&playerCellX, &playerCellY)) {
        NaviCell playerView;
        g_navigationManager.MapCellToView(
          &playerView, g_session.m_navigationPlayerMap, playerCellX, playerCellY, static_cast<float>(size)
        );
        this->FillRegion(playerView.x + mapX - 1, playerView.y + mapY - 1, 3, 3, 0xFF);
        this->FillRegion(playerView.x + mapX, playerView.y + mapY, 1, 1, 0xFFFF);
      }
    }
  }

  if (mapCellX >= 0 && mapCellY >= 0) {
    if (drawCrosshair) {
      this->FillRegion(mapX, mapY + mapCellY - 1, size, 2, 0x1414C8);
      this->FillRegion(mapX + mapCellX - 1, mapY, 2, size, 0x1414C8);
    }
    this->FillRegion(mapX + mapCellX - 2, mapY + mapCellY - 2, 4, 4, 0xFF);
    this->FillRegion(mapX + mapCellX - 1, mapY + mapCellY - 1, 2, 2, 0xFFFF);
  }

  const int activeLabel = g_navigationManager.previewType;
  const int labelCount = static_cast<int>(g_navigationManager.routeLines.size());
  for (int index = 0; index < labelCount; ++index) {
    if (g_navigationManager.routeLines[index].kind == activeLabel) {
      const int x = g_navigationManager.routeLines[index].x;
      const int y = g_navigationManager.routeLines[index].y;
      this->DrawTextAt(x + 1, y + 1, g_navigationManager.routeLines[index].text.c_str(), 0, 0, 12, 0x1E1E1E, 0, 0);
      this->DrawTextAt(x, y, g_navigationManager.routeLines[index].text.c_str(), 0, 0, 12, 0xE6E6E6, 0, 0);
    }
  }
}

// OFFSET: 0x00935A00
// FUNCTION: fn_12749
int UINavigationWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3,
                             std::intptr_t argument4) {
  switch (message) {
  case UM_SETINFO:
    this->m_saved = reinterpret_cast<NavigationWindowInfo*>(argument1);
    this->m_saved->Clamp();
    this->Move(this->m_saved->x, this->m_saved->y);
    this->Invalidate();
    return 0;
  case UM_COMMAND:
    if (argument1 != ID_SELECT_ITEM && argument1 != 0x1B9) {
      this->RecordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
    }

    switch (argument1) {
    case ID_OK: {
      this->Invalidate();
      int result = g_navigationManager.SaveCurrentTarget();
      if (result == 0) {
        {
          NavigationTarget target = g_navigationManager.GetSavedTarget();
          result = g_navigationManager.Navigate(target.x, target.y);
        }
        if (result >= 0) {
          this->SendMsg(this, UM_COMMAND, ID_BUTTON1, 0, 0, 0);
          this->RefreshRouteList();
          if (g_windowMgr.modal) {
            static_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapMarks();
          }
        }
      }
      g_navigationManager.ShowRouteResult(result, 1);
      return 0;
    }
    case ID_SEARCH:
      if (this->m_titleBar->Length() != 0) {
        g_navigationManager.searchText = this->m_titleBar->Text();
        g_navigationManager.RefreshSearch();
        this->SendMsg(this, UM_COMMAND, ID_BUTTON2, 0, 0, 0);
        if (g_windowMgr.modal) {
          static_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapLevels();
          return 0;
        }
      }
      break;
    case ID_SELECT_ITEM:
      if (g_replayAnimationScaling == 0) {
        argument2 = this->m_routePanel->selected;
      }
      if (argument2 != -1) {
        this->RecordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
        const int index = static_cast<int>(argument2) - this->m_routePanel->LineOffset();
        if (index >= 0 && g_navigationManager.SelectState(index)) {
          if (g_windowMgr.modal) {
            static_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapSelection();
          }
          this->Invalidate();
          return 0;
        }
      }
      break;
    case 0x1AD:
      if (!g_navigationManager.expandedMap && this->m_saved->expanded) {
        const int targetType = g_navigationManager.GetCurrentTarget().type;
        if (targetType != 1 && targetType != 6 && targetType != 0) {
          if (g_navigationManager.previewType == 1) {
            g_navigationManager.previewType = 2;
            this->Invalidate();
            return 0;
          }
          if (g_navigationManager.previewType == 2) {
            g_navigationManager.previewType = 1;
          }
          this->Invalidate();
          return 0;
        }
      }
      break;
    case 0x11C:
      this->m_saved->expanded = !this->m_saved->expanded;
      this->ApplyMinimized();
      this->Invalidate();
      return 0;
    case ID_MINI:
      g_navigationManager.expandedMap = !g_navigationManager.expandedMap;
      this->ApplyMinimized();
      this->RefreshHeaderTexts(g_navigationManager.viewMode);
      if (g_navigationManager.expandedMap && g_windowMgr.modal) {
        static_cast<UIRoMapWnd*>(g_windowMgr.modal)->UpdateNavigationButtons();
        return 0;
      }
      break;
    case ID_CLOSE:
      if (g_navigationManager.viewMode == 2) {
        const int response = g_windowMgr.Confirm(LookupMessage(MSI_NAVIGATION_HELP2), 2, true, false, 0, "Message", 280, 120, 0);
        if (response != 0xBB) {
          break;
        }
        g_navigationManager.SetViewMode(0);
      }
      g_worldViewMgr.ClearMarks();
      g_worldViewMgr.ClearLevels();
      g_worldViewMgr.ResetSelection();
      g_navigationManager.Reset(true);
      g_navigationManager.expandedMap = false;
      g_navigationManager.hideWindow = false;
      if (g_windowMgr.modal) {
        g_navigationManager.expandedMap = true;
        static_cast<UIRoMapWnd*>(g_windowMgr.modal)->UpdateNavigationButtons();
        return 0;
      }
      g_windowMgr.Schedule(this);
      g_windowMgr.navigationWnd = nullptr;
      return 0;
    case ID_BUTTON1:
      g_navigationManager.displayMode = 1;
      this->LayoutTabs();
      return 0;
    case ID_BUTTON2:
      g_navigationManager.displayMode = 0;
      this->LayoutTabs();
      return 0;
    case 0xBF: {
      if (g_windowMgr.comboBoxWnd) {
        g_windowMgr.CloseWindow(WID_COMBOBOXWND);
      }

      this->ClearHistory();

      const std::string directory = FormatOwnedString("%s\\Navigationdata\\", g_applicationDirectory);
      EnumerateFileNames(this->m_history, directory.c_str(), "txt");
      const int count = static_cast<int>(this->m_history.size());
      if (count != 0) {
        UIWindow* historyWindow = g_windowMgr.EnsureWindow(WID_COMBOBOXWND);
        for (std::vector<std::string>::iterator it = this->m_history.begin(); it != this->m_history.end(); ++it) {
          historyWindow->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(it->c_str()), 0, 0, 0);
        }

        const int height = (std::min)(0xA0, count * 0x10);
        historyWindow->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
        historyWindow->Resize(0xA0, height);
        historyWindow->SendMsg(nullptr, UM_SET_CALLER_WID, 0xCB, 0, 0, 0);

        int x = this->geometry.width - 0xA5;
        int y = 0x24;
        this->ToScreen(x, y);
        historyWindow->Move(x, y);
      }
      return 0;
    }
    case ID_CHECK:
      if (sender == this->m_allowGates) {
        g_navigationManager.allowGates = argument2 != 0;
        return 0;
      }
      if (sender == this->m_includeObjects) {
        g_navigationManager.includeObjects = argument2 != 0;
        return 0;
      }
      if (sender == this->m_allowWarps) {
        g_navigationManager.allowWarps = argument2 != 0;
        return 0;
      }
      break;
    case ID_HELP:
      if (!g_windowMgr.CloseWindow(WID_NAVIGATIONHELPWND)) {
        g_windowMgr.EnsureWindow(WID_NAVIGATIONHELPWND);
        return 0;
      }
      break;
    case 0x1B9:
      if (g_replayAnimationScaling == 0) {
        argument2 = this->m_searchPanel->selected;
      }
      if (argument2 != -1) {
        this->RecordResponse(0x1B9, argument2, argument3, argument4);
        if (g_navigationManager.SelectTarget(static_cast<int>(argument2) - this->m_searchPanel->LineOffset())) {
          if (g_windowMgr.modal) {
            static_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapSelection();
          }
          this->Invalidate();
          return 0;
        }
      }
      break;
    default:
      break;
    }
    break;
  case UM_COMBOBOX_SELECTED:
    if (this->m_searchType == sender) {
      g_navigationManager.searchType = static_cast<int>(argument1);
      this->Invalidate();
      if (g_replayAnimationScaling != 0) {
        this->m_searchType->SendMsg(nullptr, UM_CHANGEMSG, argument1, 0, 0, 0);
        return 0;
      }
      break;
    }
    if (g_replayAnimationScaling == 0) {
      this->LoadRoute(static_cast<int>(argument1));
      return 0;
    }
    break;
  case UM_ADDITEM:
    switch (argument1) {
    case ID_SELECT_ITEM:
      this->m_routePanel->AddLine(reinterpret_cast<const char*>(argument2));
      break;
    case 0x1B9:
      this->m_searchPanel->AddLine(reinterpret_cast<const char*>(argument2));
      break;
    }
  case UM_DEFPUSH:
    if (this->m_titleBar == sender && this->m_titleBar->Length() > 0) {
      this->SendMsg(this->m_titleBar, UM_COMMAND, ID_SEARCH, 0, 0, 0);
      return 0;
    }
  case UM_RESIZE:
    if (argument1 == 7) {
      const int height = (std::min)(std::ranges::max(static_cast<int>(argument3), 0x152), 0x152);
      this->m_saved->height = height;
      this->ApplyMinimized();
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x0093D620
// FUNCTION: fn_12769
void UINavigationWnd::FillRouteList() {
  this->m_routePanel->Invalidate();
  this->m_routePanel->Clear();
  if (g_navigationManager.StateCount() == 0) {
    return;
  }

  char text[64];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, LookupMessage(MSI_NAVIGATION_MAP_S), g_navigationManager.StateCount());
  this->SendMsg(nullptr, UM_ADDITEM, 0x150, reinterpret_cast<std::intptr_t>(text), 0, 0);
  this->m_routePanel->SetLineOffset(1);

  for (int index = 0; index < g_navigationManager.StateCount(); ++index) {
    std::memset(text, 0, sizeof(text));
    NavigationTarget state = g_navigationManager.GetState(index);
    if (state.type == 2) {
      CNavi_Object* object = static_cast<CNavi_Object*>(state.object);
      std::sprintf(text, LookupMessage(MSI_NAVIGATION_RESULTINFO), object->GetInfo()->GetName(), object->GetName());
    } else if (state.type == 3) {
      CNavi_Object* object = static_cast<CNavi_Object*>(state.object);
      const int flag = object->GetFlag();
      if (flag > 0) {
        std::string kind = "없음";
        if (flag > 99) {
          kind = LookupMessage(MSI_NAVIGATION_NUM4);
        } else if (flag > 49) {
          kind = LookupMessage(MSI_NAVIGATION_NUM5);
        } else if (flag >= 20 && flag <= 49) {
          kind = LookupMessage(MSI_BANK_SYSTEM_ERROR);
        } else if (flag >= 5 && flag <= 19) {
          kind = LookupMessage(MSI_BANK_WITHDRAW_NO_MONEY);
        } else if (flag >= 1 && flag <= 4) {
          kind = LookupMessage(MSI_BANK_DEPOSIT_NO_MONEY);
        }
        std::sprintf(text, LookupMessage(MSI_NAVIGATION_NUM2), object->GetInfo()->GetName(), object->GetName(), kind.c_str());
      } else {
        std::sprintf(text, LookupMessage(MSI_NAVIGATION_LENGTH), object->GetInfo()->GetName(), object->GetName());
      }
    } else if (state.type == 1) {
      std::sprintf(text, LookupMessage(MSI_NAVIGATION_TARGETMAP_S_S), static_cast<CNaviInfo*>(state.object)->GetName());
    } else if (state.type == 0) {
      CNaviInfo* info = static_cast<CNaviInfo*>(state.object);
      if (state.name.empty()) {
        std::sprintf(text, LookupMessage(MSI_REFUSE_ERRORCODE), info->GetName(), state.x, state.y);
      } else {
        std::sprintf(text, LookupMessage(MSI_REFUSE_DISCONNECT_KEEP_TIME), state.name.c_str(), info->GetName(), state.x, state.y);
      }
    }
    this->SendMsg(nullptr, UM_ADDITEM, 0x150, reinterpret_cast<std::intptr_t>(text), 0, 0);
  }
}

// OFFSET: 0x00917000
// FUNCTION: fn_12587
bool UINavigationWnd::ClearHistory() {
  for (std::vector<std::string>::iterator it = this->m_history.begin(); it != this->m_history.end(); ++it) {
    it->clear();
  }
  this->m_history.clear();
  return true;
}
