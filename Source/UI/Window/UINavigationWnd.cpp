#include "UI/Window/UINavigationWnd.h"

#include "Client/Application/Launcher.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
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
#include "UI/Lists/UIListBoxNavi.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIRoMapWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>

// GLOBAL: gl_24609
bool g_navigationMinimized;

// GLOBAL: gl_24610
int g_navigationMode;

// GLOBAL: gl_24769
UIWindow* g_navigationWindow;

// GLOBAL: gl_24608
bool g_navigationHidden;

// GLOBAL: gl_24613
bool g_navigationFilter;

// GLOBAL: gl_25095
bool g_unknown16FC585;

// GLOBAL: gl_25096
bool g_unknown16FC586;

// GLOBAL: gl_24612
int g_navigationSearchType;

// GLOBAL: gl_25130
std::string g_navigationSearchText;

// GLOBAL: gl_24614
int g_navigationActiveLabel;

// GLOBAL: gl_25081
int g_navigationMapCellX;

// GLOBAL: gl_25082
int g_navigationMapCellY;

// GLOBAL: gl_25083
float g_navigationPointScale = 0.0078125f;

// GLOBAL: gl_25084
float g_navigationMarkerScale = 0.453125f;

// GLOBAL: gl_24616
CActRes* g_navigationActions;

// GLOBAL: gl_24617
CSprRes* g_navigationSprites;

// GLOBAL: gl_24615
std::vector<NaviCell> g_navigationPoints;

// GLOBAL: gl_24618
std::vector<NaviRouteLine> g_navigationLabels;

// OFFSET: 0x00921A70
// FUNCTION: fn_12643
void UINavigationWnd::Slot15(int width, int height) {
  this->m_routePanel = new UIListBoxNavi(*this->m_context, *this->m_listContext);
  this->m_routePanel->SetSize(215, 96);
  this->m_routePanel->backgroundRed = 210;
  this->m_routePanel->backgroundGreen = 210;
  this->m_routePanel->backgroundBlue = 210;
  this->m_routePanel->SetId(336);
  this->m_routePanel->Move(-1000, -1000);
  this->AddChild(this->m_routePanel);

  this->m_searchPanel = new UIListBoxNavi(*this->m_context, *this->m_listContext);
  this->m_searchPanel->SetSize(215, 96);
  this->m_searchPanel->backgroundRed = 210;
  this->m_searchPanel->backgroundGreen = 210;
  this->m_searchPanel->backgroundBlue = 210;
  this->m_searchPanel->SetId(441);
  this->m_searchPanel->Move(-1000, -1000);
  this->m_searchPanel->SetDefaultLineColor(0x1414F0);
  this->AddChild(this->m_searchPanel);

  std::string base = "유저인터페이스\\";

  this->unknown_0128 = new UIResizer(*this->m_context);
  this->unknown_0128->SetSize(14, 14);
  this->unknown_0128->SetDirection(7);
  this->unknown_0128->SetBitmapName((base + "btn_resize.bmp").c_str());
  this->unknown_0128->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->unknown_0128->Move(-1000, -1000);
  this->AddChild(this->unknown_0128);

  std::string path = base;
  path += "navigation_interface\\";

  const char* tooltips[6] = {
    LookupMessage(0x894), LookupMessage(0x895), LookupMessage(0x896), LookupMessage(0x897), LookupMessage(0x898), LookupMessage(0x899),
  };
  const char* names[6] = {"NaviSearch", "NaviStart", "NaviRoute", "NaviList", "Navisyschange2", "navifolder"};
  const int ids[6] = {321, 184, 189, 190, 284, 191};

  for (int index = 0; index < 6; ++index) {
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_buttons[index] = button;
    button->SetBitmap((path + names[index] + "_A.bmp").c_str(), 0, 0);
    button->SetBitmap((path + names[index] + "_B.bmp").c_str(), 1, 0);
    button->SetBitmap((path + names[index] + "_C.bmp").c_str(), 2, 0);
    this->m_buttons[index]->SetSize(this->m_buttons[index]->BitmapWidth(), this->m_buttons[index]->BitmapHeight());
    this->m_buttons[index]->Move(-1000, -1000);
    this->m_buttons[index]->SetId(ids[index]);
    this->m_buttons[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->m_buttons[index]);
  }

  this->m_closeButton = this->AddCloseButton(*this->m_controls, LookupMessage(0x89A), 0);
  this->m_minimizeButton = this->AddMinimizeButton(*this->m_controls, "", 0);

  this->unknown_0120 = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->unknown_0120->SetBitmap((path + "NaviHelp_off.bmp").c_str(), 0, 0);
  this->unknown_0120->SetBitmap((path + "NaviHelp_on.bmp").c_str(), 1, 0);
  this->unknown_0120->SetBitmap((path + "NaviHelp_on.bmp").c_str(), 2, 0);
  this->unknown_0120->SetSize(this->unknown_0120->BitmapWidth(), this->unknown_0120->BitmapHeight());
  this->unknown_0120->Move(this->geometry.width - 42, 3);
  this->unknown_0120->SetId(343);
  this->unknown_0120->SetTooltip(LookupMessage(0x89D));
  this->AddChild(this->unknown_0120);

  this->unknown_00EC = new UIComboBoxDefWnd(*this->m_context, *this->m_comboContext);
  this->unknown_00EC->dropdownHeight = 67;
  this->unknown_00EC->SetSize(56, 18);
  this->unknown_00EC->Move(-1000, -1000);
  this->unknown_00EC->borderRed = 100;
  this->unknown_00EC->borderGreen = 100;
  this->unknown_00EC->borderBlue = 100;
  this->unknown_00EC->red = 232;
  this->unknown_00EC->green = 232;
  this->unknown_00EC->blue = 232;
  this->AddChild(this->unknown_00EC);
  this->unknown_00EC->AddOption(LookupMessage(0x89E), 0);
  this->unknown_00EC->AddOption(LookupMessage(0x89F), 0);
  this->unknown_00EC->AddOption(LookupMessage(0x8A0), 0);
  this->unknown_00EC->AddOption(LookupMessage(0x8A1), 0);

  this->m_titleBar = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_titleBar->SetSize(226, 18);
  this->m_titleBar->Move(-1000, -1000);
  this->m_titleBar->maximum = 60;
  this->m_titleBar->Background(232, 232, 232);
  this->m_titleBar->Border(200, 100, 100);
  this->m_titleBar->flat = 0;
  UIEditCtrl* edit = this->m_titleBar;
  const char* tooltip = LookupMessage(0x8A2);
  if (tooltip) {
    edit->tooltip = tooltip;
  }
  this->AddChild(this->m_titleBar);
  if (g_replayAnimationScaling) {
    this->m_titleBar->disabled = 1;
    this->m_titleBar->Invalidate();
  }

  UICheckBox* box = new UICheckBox(*this->m_context, *this->m_controls);
  this->unknown_0118 = box;
  box->SetSize(this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0x8A3), 10, 0, 12, 0, 0), 12);
  path = base;
  this->unknown_0118->SetImages((path + "checkbox_1.bmp").c_str(), (path + "checkbox_0.bmp").c_str(), LookupMessage(0x8A3));
  this->unknown_0118->Move(-1000, -1000);
  this->unknown_0118->SetChecked(g_unknown16FC585);
  this->unknown_0118->SetTooltip(LookupMessage(0x8A4));
  this->AddChild(this->unknown_0118);

  box = new UICheckBox(*this->m_context, *this->m_controls);
  this->unknown_0114 = box;
  box->SetSize(this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0x8A5), 10, 0, 12, 0, 0), 12);
  path = base;
  this->unknown_0114->SetImages((path + "checkbox_1.bmp").c_str(), (path + "checkbox_0.bmp").c_str(), LookupMessage(0x8A5));
  this->unknown_0114->Move(-1000, -1000);
  this->unknown_0114->SetChecked(g_navigationFilter);
  this->unknown_0114->SetTooltip(LookupMessage(0x8A6));
  this->AddChild(this->unknown_0114);

  box = new UICheckBox(*this->m_context, *this->m_controls);
  this->unknown_011C = box;
  box->SetSize(this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0x8A7), 10, 0, 12, 0, 0), 12);
  path = base;
  this->unknown_011C->SetImages((path + "checkbox_1.bmp").c_str(), (path + "checkbox_0.bmp").c_str(), LookupMessage(0x8A7));
  this->unknown_011C->Move(-1000, -1000);
  this->unknown_011C->SetChecked(g_unknown16FC586);
  this->unknown_011C->SetTooltip(LookupMessage(0x8A8));
  this->AddChild(this->unknown_011C);
}

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

// OFFSET: 0x0092CEF0
// FUNCTION: fn_12668
void UINavigationWnd::Paint() {
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, this->m_title.c_str(), 0);
  this->DrawPanelBackground(0x11, 0, "");
  if (g_navigationMinimized) {
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 6, 20, this->m_subtitle.c_str(), 0, 0, 12, 0, 0, 0);
    return;
  }

  if (!this->m_saved->minimized) {
    return;
  }

  const NavigationTarget target = g_navigationManager.GetCurrentTarget();
  int mapCellX = g_navigationMapCellX;
  int mapCellY = g_navigationMapCellY;
  int size = 0x80;
  int markerSize = 0x80;
  int markerWidth = 0x80;
  this->FillRegion(6, 0x25, 0x82, 0x82, 0x646464);

  if (g_navigationActiveLabel == 2) {
    if (target.type == 2) {
      markerSize = 0x43;
      markerWidth = 0x3A;
      if (mapCellX >= 0 && mapCellY >= 0) {
        mapCellX = static_cast<int>(static_cast<float>(mapCellX) * g_navigationMarkerScale);
        mapCellY = static_cast<int>(static_cast<float>(mapCellY) * g_navigationMarkerScale);
      }
    } else if (target.type == 3) {
      size = 0x20;
      markerWidth = size;
    } else {
      markerWidth = size;
    }
    size = markerWidth;
    this->DrawActionSprite(g_navigationSprites, g_navigationActions, 7, 0x26, markerSize, 0x80, 0x5dc, true, nullptr);
  }

  const int mapX = 0x87 - size;
  const int mapY = 0xA6 - size;
  this->FillRegion(mapX, mapY, size, size, 0);
  if (g_navigationMapTexture != nullptr) {
    const float scale = static_cast<float>(size) / static_cast<float>(g_navigationMapTexture->Width());
    this->geometry.surface->DrawScaledSurface(
      mapX, mapY, size, size, g_navigationMapTexture, g_navigationMapTexture->Width(), g_navigationMapTexture->Height(), scale, scale, 0xFFFFFFFFu, false, 0, 0, 0,
      0, 0
    );
  }

  bool drawCrosshair = true;
  if (g_navigationMode == 2) {
    if (g_navigationManager.IsCurrentTarget(g_session.m_navigationPlayerMap)) {
      if (g_navigationTab == 1) {
        drawCrosshair = false;
      }
      const float scale = static_cast<float>(size) * g_navigationPointScale;
      for (const NaviCell& point : g_navigationPoints) {
        this->FillRegion(static_cast<int>(point.x * scale + mapX), static_cast<int>(point.y * scale + mapY), 2, 2, 0x00F0F00A);
      }

      int playerCellX = 0;
      int playerCellY = 0;
      if (g_navigationManager.PlayerCell(&playerCellX, &playerCellY)) {
        NaviCell playerView;
        g_navigationManager.MapCellToView(
          &playerView, g_session.m_navigationPlayerMap, playerCellX, playerCellY, static_cast<float>(markerWidth)
        );
        this->FillRegion(playerView.x + mapX - 1, playerView.y + mapY - 1, 3, 3, 0xFF);
        this->FillRegion(playerView.x + mapX, playerView.y + mapY, 1, 1, 0xFFFF);
      }
    }
  }

  if (mapCellX >= 0 && mapCellY >= 0) {
    if (drawCrosshair) {
      this->FillRegion(mapX, mapY + mapCellY - 1, markerWidth, 2, 0x1414C8);
      this->FillRegion(mapX + mapCellX - 1, mapY, 2, markerWidth, 0x1414C8);
    }
    this->FillRegion(mapX + mapCellX - 2, mapY + mapCellY - 2, 4, 4, 0xFF);
    this->FillRegion(mapX + mapCellX - 1, mapY + mapCellY - 1, 2, 2, 0xFFFF);
  }

  for (const NaviRouteLine& label : g_navigationLabels) {
    if (label.kind == g_navigationActiveLabel) {
      this->DrawTextAt(this->m_context->fonts, this->m_context->locale, label.x + 1, label.y + 1, label.text.c_str(), 0, 0, 12, 0x1E1E1E, 0, 0);
      this->DrawTextAt(this->m_context->fonts, this->m_context->locale, label.x, label.y, label.text.c_str(), 0, 0, 12, 0xE6E6E6, 0, 0);
    }
  }
}

// OFFSET: 0x00935A00
// FUNCTION: fn_12749
int UINavigationWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3,
                             std::intptr_t argument4) {
  const auto recordResponse = [this](int response, std::intptr_t first, std::intptr_t second, std::intptr_t third) {
    Replay& replay = Replay::Instance();
    if (replay.state == 2) {
      std::vector<unsigned char> packet(26, 0);
      const std::uint32_t marker = 0x038408AE;
      const std::uint16_t bytes = 26;
      const int windowId = this->geometry.id;
      const std::uint32_t values[3] = {
        static_cast<std::uint32_t>(first), static_cast<std::uint32_t>(second), static_cast<std::uint32_t>(third)
      };
      std::memcpy(packet.data(), &marker, sizeof(marker));
      std::memcpy(packet.data() + 4, &bytes, sizeof(bytes));
      std::memcpy(packet.data() + 6, &windowId, sizeof(windowId));
      std::memcpy(packet.data() + 10, &response, sizeof(response));
      std::memcpy(packet.data() + 14, values, sizeof(values));
      replay.RecordPacket(packet, 42, 0);
    }
  };

  switch (message) {
  case 0:
    if (sender == this->m_titleBar && this->m_titleBar->Length() > 0) {
      this->SendMsg(this->m_titleBar, 6, 0x141, 0, 0, 0);
      return 0;
    }
    if (argument1 == 7) {
      this->m_saved->height = 0x152;
      this->ApplyMinimized();
    }
    break;
  case 6:
    if (argument1 != 0x150) {
      if (argument1 == 0x1B9) {
        if (g_replayAnimationScaling == 0) {
          argument2 = this->m_searchPanel->selected;
        }
        if (argument2 != -1) {
          recordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
          g_navigationManager.SelectTarget(static_cast<int>(argument2) - this->m_searchPanel->LineOffset());
          if (g_navigationManager.GetCurrentTargetType() == 6) {
            if (g_windowMgr.window_0428) {
              static_cast<UIRoMapWnd*>(g_windowMgr.window_0428)->UpdateNavigationButtons();
            }
            this->Invalidate();
            return 0;
          }
        }
      } else {
        recordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
        if (argument1 >= 0x1BA) {
          return 0;
        }
        break;
      }
    }
    break;
  case 0xE:
    if (argument1 == 7) {
      this->m_saved->height = 0x152;
      this->ApplyMinimized();
    }
    break;
  case 0x1F:
    if (argument1 == 0x150) {
      this->m_routePanel->AddLine(reinterpret_cast<const char*>(argument2));
    } else if (argument1 == 0x1B9) {
      this->m_searchPanel->AddLine(reinterpret_cast<const char*>(argument2));
    }
    if (sender == this->m_titleBar && this->m_titleBar->Length() > 0) {
      this->SendMsg(this->m_titleBar, 6, 0x141, 0, 0, 0);
      return 0;
    }
    if (argument1 == 7) {
      const int height = (std::max)(0x152, (std::min)(static_cast<int>(argument3), 0x152));
      this->m_saved->height = height;
      this->ApplyMinimized();
    }
    break;
  case 0x22:
    this->m_saved = reinterpret_cast<NavigationWndInfo*>(argument1);
    reinterpret_cast<NavigationWindowInfo*>(this->m_saved)->Clamp();
    this->Resize(this->m_saved->width, this->m_saved->height);
    this->Invalidate();
    return 0;
  case 0x27:
    if (sender == this->unknown_00EC) {
      g_navigationSearchType = static_cast<int>(argument1);
      this->Invalidate();
      if (g_replayAnimationScaling != 0) {
        this->unknown_00EC->SendMsg(nullptr, 0x39, argument1, 0, 0, 0);
        return 0;
      }
    } else if (g_replayAnimationScaling == 0) {
      this->LoadRoute(static_cast<int>(argument1));
      return 0;
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

dispatch:
  switch (argument1) {
  case 0xB8: {
    this->Invalidate();
    int result = g_navigationManager.SaveCurrentTarget();
    if (result == 0) {
      NavigationTarget target = g_navigationManager.GetSavedTarget();
      result = g_navigationManager.Navigate(target.x, target.y);
      if (result >= 0) {
        this->SendMsg(this, 6, 0xBD, 0, 0, 0);
        this->RefreshRouteList();
        if (g_windowMgr.window_0428) {
          this->RefreshWorldMapMarks();
        }
      }
    }
    g_navigationManager.ShowRouteResult(result, 1, *reinterpret_cast<WindowDialogs*>(&g_windowMgr));
    return 0;
  }
  case 0xBD:
    g_navigationTab = 1;
    this->LayoutTabs();
    return 0;
  case 0xBE:
    g_navigationTab = 0;
    this->LayoutTabs();
    return 0;
  case 0xBF: {
    if (g_closeChatAfterGroups != 0) {
      g_windowMgr.CloseWindow(0x1A);
    }
    struct VectorStorage {
      std::string* first;
      std::string* last;
      std::string* end;
    };
    VectorStorage& history = *reinterpret_cast<VectorStorage*>(&this->m_history);
    for (std::string* item = history.first; item != history.last; ++item) {
      item->clear();
    }
    history.last = history.first;
    const std::string directory = FormatOwnedString("%s\\Navigationdata\\", g_applicationDirectory);
    EnumerateFileNames(this->m_history, directory.c_str(), "txt");
    const int count = static_cast<int>(this->m_history.size());
    if (count != 0) {
      UIWindow* historyWindow = g_windowMgr.EnsureWindow(0x1A);
      for (const std::string& name : this->m_history) {
        historyWindow->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(name.c_str()), 0, 0, 0);
      }
      const int height = (std::min)(count * 0x10, 0xA0);
      historyWindow->SendMsg(nullptr, 0x28, 0, 0, 0, 0);
      historyWindow->Resize(0xA0, height);
      historyWindow->SendMsg(nullptr, 0x53, 0xCB, 0, 0, 0);
      int x = this->geometry.x - 0xA5;
      int y = 0x24;
      for (UIWindow* ancestor = this; ancestor; ancestor = ancestor->parent) {
        x += ancestor->geometry.width;
        y += ancestor->geometry.height;
      }
      historyWindow->Move(x, y);
    }
    return 0;
  }
  case 0xC9:
    if (g_navigationMode == 2) {
      const int response = reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x8BF), 2, true, false, 0, "Message", 280, 120, 0);
      if (response != 0xBB) {
        return 0;
      }
      g_navigationManager.SetViewMode(0);
    }
    g_worldViewMgr.ClearMarks();
    g_worldViewMgr.ClearLevels();
    g_worldViewMgr.ResetSelection();
    g_navigationManager.Reset(true);
    g_navigationMinimized = false;
    g_navigationHidden = false;
    if (!g_windowMgr.window_0428) {
      g_windowMgr.Schedule(this);
      g_navigationWindow = nullptr;
    } else {
      g_navigationMinimized = true;
      static_cast<UIRoMapWnd*>(g_windowMgr.window_0428)->UpdateNavigationButtons();
    }
    return 0;
  case 0xCA:
    g_navigationMinimized = !g_navigationMinimized;
    this->ApplyMinimized();
    this->RefreshHeaderTexts(g_navigationMode);
    if (g_navigationMinimized && g_windowMgr.window_0428) {
      static_cast<UIRoMapWnd*>(g_windowMgr.window_0428)->UpdateNavigationButtons();
      return 0;
    }
    break;
  case 0xD5:
    if (sender == this->unknown_0114) {
      g_navigationFilter = argument2 != 0;
      return 0;
    }
    if (sender == this->unknown_0118) {
      g_unknown16FC585 = argument2 != 0;
      return 0;
    }
    if (sender == this->unknown_011C) {
      g_unknown16FC586 = argument2 != 0;
      return 0;
    }
    break;
  case 0x11C:
    this->m_saved->minimized = !this->m_saved->minimized;
    this->ApplyMinimized();
    this->Invalidate();
    return 0;
  case 0x141:
    if (this->m_titleBar->Length() != 0) {
      g_navigationSearchText = this->m_titleBar->Text();
      g_navigationManager.RefreshSearch();
      this->SendMsg(this, 6, 0xBE, 0, 0, 0);
      if (g_windowMgr.window_0428) {
        this->RefreshWorldMapLevels();
        return 0;
      }
    }
    break;
  case 0x150:
    if (g_replayAnimationScaling == 0) {
      argument2 = this->m_routePanel->selected;
    }
    if (argument2 != -1) {
      recordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
      const int index = static_cast<int>(argument2) - this->m_routePanel->LineOffset();
      if (index >= 0 && g_navigationManager.SelectState(index)) {
        if (g_windowMgr.window_0428) {
          static_cast<UIRoMapWnd*>(g_windowMgr.window_0428)->UpdateNavigationButtons();
        }
        this->Invalidate();
        return 0;
      }
    }
    break;
  case 0x157:
    if (!g_windowMgr.CloseWindow(0xE5)) {
      g_windowMgr.EnsureWindow(0xE5);
    }
    return 0;
  case 0x1AD: {
    if (!g_navigationMinimized && this->m_saved->minimized) {
      const NavigationTarget current = g_navigationManager.GetCurrentTarget();
      if (current.type != 1 && current.type != 6 && current.type != 0) {
        if (g_navigationActiveLabel == 1) {
          g_navigationActiveLabel = 2;
        } else if (g_navigationActiveLabel == 2) {
          g_navigationActiveLabel = 1;
        }
        this->Invalidate();
        return 0;
      }
    }
    break;
  }
  }

  return 0;
}

// OFFSET: 0x0093D620
// FUNCTION: fn_12769
void UINavigationWnd::FillRouteList() {
  this->m_routePanel->Invalidate();
  this->m_routePanel->Clear();
  const int count = g_navigationManager.StateCount();
  if (count == 0) {
    return;
  }

  char text[64];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, LookupMessage(0x8AE), count);
  this->SendMsg(nullptr, 0x1F, 0x150, reinterpret_cast<std::intptr_t>(text), 0, 0);
  this->m_routePanel->lineOffset = 1;

  for (int index = 0; index < g_navigationManager.StateCount(); ++index) {
    std::memset(text, 0, sizeof(text));
    NavigationTarget state = g_navigationManager.GetState(index);
    if (state.type == 2) {
      CNavi_Object* object = static_cast<CNavi_Object*>(state.object);
      std::sprintf(text, LookupMessage(0x8AF), object->GetInfo()->GetName(), object->GetName());
    } else if (state.type == 3) {
      CNavi_Object* object = static_cast<CNavi_Object*>(state.object);
      const int flag = object->GetFlag();
      if (flag < 1) {
        std::sprintf(text, LookupMessage(0x8B0), object->GetInfo()->GetName(), object->GetName());
      } else {
        const char* kind;
        if (flag >= 100) {
          kind = LookupMessage(0x991);
        } else if (flag > 0x31) {
          kind = LookupMessage(0x992);
        } else if (static_cast<unsigned int>(flag - 0x14) < 0x1E) {
          kind = LookupMessage(0x993);
        } else if (static_cast<unsigned int>(flag - 5) < 0xF) {
          kind = LookupMessage(0x994);
        } else {
          kind = LookupMessage(0x995);
        }
        std::sprintf(text, LookupMessage(0x98F), object->GetInfo()->GetName(), object->GetName(), kind);
      }
    } else if (state.type == 1) {
      std::sprintf(text, LookupMessage(0x8B1), static_cast<CNaviInfo*>(state.object)->GetName());
    } else if (state.type == 0) {
      CNaviInfo* info = static_cast<CNaviInfo*>(state.object);
      if (state.name.empty()) {
        std::sprintf(text, LookupMessage(0x975), info->GetName(), state.x, state.y);
      } else {
        std::sprintf(text, LookupMessage(0x976), state.name.c_str(), info->GetName(), state.x, state.y);
      }
    }
    this->SendMsg(nullptr, 0x1F, 0x150, reinterpret_cast<std::intptr_t>(text), 0, 0);
  }
}

// OFFSET: 0x00917000
// FUNCTION: fn_12587
bool UINavigationWnd::ClearHistory() {
  for (std::string& name : this->m_history) {
    name.clear();
  }
  this->m_history.clear();
  return true;
}
