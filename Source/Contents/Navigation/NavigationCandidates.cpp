#include "Contents/Navigation/NavigationManager.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <string>
#include <vector>

// OFFSET: 0x00BA0870
// FUNCTION: fn_19634
bool CNavigationManager::CollectStartLinks() {
  CNaviInfo* info = this->FindObject(g_session.m_navigationPlayerMap);
  if (info == nullptr) {
    const std::string resolved = ResourceMgr::Instance().ResolveMapName(g_session.m_navigationPlayerMap);
    info = this->FindObject(resolved.c_str());
    if (info == nullptr) {
      g_windowMgr.Confirm(LookupMessage(MSI_NAVIGATION_RS_FAILSTARTDATA), 0, true, false, 0, "Message", 280, 120, 0);
      return false;
    }
  }

  if (info->LinkCount() == 0) {
    return false;
  }

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode == nullptr) {
    return false;
  }

  int column;
  int row;
  mode->PlayerCell(&column, &row);
  this->UpdateGround();

  MovementRouteInterpolation route;
  this->data.startLinks.clear();
  for (std::vector<CNavi_Link*>::iterator it = info->LinksBegin(); it != info->LinksEnd(); ++it) {
    NaviCell linkCell;
    NaviCell linkRowCell;
    if ((*it)->GetCell(&linkCell)->x == column && (*it)->GetCell(&linkRowCell)->y == row) {
      this->data.startLinks.push_back(NaviLink{*it, 1});
    } else {
      NaviCell targetRowCell;
      NaviCell targetCell;
      if (this->pathFinder.FindPath(1, column, row, 8, 8, (*it)->GetCell(&targetCell)->x, (*it)->GetCell(&targetRowCell)->y, 10, route, 15)) {
        this->data.startLinks.push_back(NaviLink{*it, static_cast<int>(route.points.size())});
      }
      this->pathFinder.Reset();
    }
  }

  return !this->data.startLinks.empty();
}
