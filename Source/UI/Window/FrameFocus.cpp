#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include <algorithm>
#include "Client/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include <typeinfo>

// STATIC: 0x014576A0
// OFFSET: 0x00A96A60
// FUNCTION: fn_17192
bool UIWindowMgr::IsOrderFixed(int id) {
  switch (id) {
  case 0: case 1: case 2: case 3: case 4: case 6: case 7: case 9: case 14: case 15: case 16: case 17: case 18: case 20: case 21: case 22: case 23:
  case 24: case 26: case 28: case 29: case 31: case 32: case 33: case 34: case 35: case 36: case 39: case 42: case 45: case 49: case 50: case 56:
  case 58: case 60: case 61: case 62: case 63: case 64: case 65: case 66: case 67: case 70: case 73: case 74: case 78: case 79: case 80: case 81:
  case 90: case 91: case 94: case 95: case 99: case 100: case 101: case 104: case 105: case 107: case 109: case 111: case 115: case 116: case 123:
  case 124: case 127: case 128: case 130: case 131: case 133: case 136: case 137: case 138: case 144: case 154: case 156: case 158: case 159:
  case 160: case 161: case 164: case 175: case 176: case 177: case 179: case 180: case 183: case 186: case 187: case 189: case 190: case 196:
  case 198: case 199: case 200: case 204: case 216: case 217: case 218: case 219: case 220: case 221: case 222: case 223: case 224: case 225:
  case 226: case 227: case 253: case 257: case 258: case 262: case 267: case 268: case 269: case 271: case 277: case 278: case 284: case 285:
  case 286: case 289: case 295: case 301: case 307: case 325: case 332: case 343: case 358: case 359: case 600: case 10011: case 10032:
    return true;
  case 145: {
    UIWindow* window = g_windowMgr.LookupWindow(145);
    if (window && window->geometry.visible) {
      return false;
    }
    return true;
  }
  default:
    return false;
  }
}

// OFFSET: 0x00A8F720
// FUNCTION: fn_17144
void UIWindowMgr::RemoveOrder(int id) {
  if (this->IsOrderFixed(id)) {
    return;
  }

  for (auto entry = this->windowHistory.begin(); entry != this->windowHistory.end(); ++entry) {
    if (id == *entry) {
      this->windowHistory.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x00A95540
// FUNCTION: fn_17180
void UIWindowMgr::BringToFront(int id) {
  if (this->IsOrderFixed(id)) {
    return;
  }

  this->RemoveOrder(id);
  if (id == 203) {
    this->windowHistory.push_back(id);
  } else {
    this->windowHistory.push_front(id);
  }
}

// OFFSET: 0x00AA9B30
// FUNCTION: fn_17255
void UIWindowMgr::SetCapture(UIWindow* window) {
  this->inputWindow = window;
}

// OFFSET: 0x00AA66C0
// FUNCTION: fn_17232
void UIWindowMgr::ReleaseCapture() {
  this->inputWindow = nullptr;
}

// OFFSET: 0x00A913F0
// FUNCTION: fn_17158
UIWindow* UIWindowMgr::Capture() const {
  return this->inputWindow;
}

// OFFSET: 0x00A96A40
// FUNCTION: fn_17191
bool UIWindowMgr::IsDropDownChild(const UIWindow* window) {
  if (!this->window_0A30 || !this->window_0A30->geometry.visible) {
    return false;
  }

  return static_cast<CUIDropDownBox*>(this->window_0A30)->HasChild(window);
}

// OFFSET: 0x00A96E60
// FUNCTION: fn_17194
bool UIWindowMgr::IsHoverParentVisible() {
  if (this->hoverWindow) {
    UIWindow* parent = this->hoverWindow->parent;
    if (parent && parent != this->window_01C8 && parent->geometry.visible) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00A96E90
// FUNCTION: fn_17195
bool UIWindowMgr::IsFriend(const char* name) {
  return g_session.IsFriendName(name) != 0;
}

// OFFSET: 0x00A96EB0
// FUNCTION: fn_17196
bool UIWindowMgr::IsSharedWindowId(int id) {
  switch (id) {
  case 9:
  case 13:
  case 16:
  case 17:
  case 18:
  case 20:
  case 45:
  case 112:
  case 113:
  case 114:
  case 115:
  case 116:
  case 117:
  case 118:
  case 119:
  case 120:
  case 121:
  case 122:
  case 123:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00A97070
// FUNCTION: fn_17198
bool UIWindowMgr::HasNavigationBlocker() {
  return this->window_4F260 || this->window_4F264 || this->window_4F268 || this->window_4F26C;
}

// OFFSET: 0x00A97380
// FUNCTION: fn_17202
bool UIWindowMgr::Window4F250Ready() {
  UIWindow* window = this->window_4F250;
  if (window == nullptr) {
    return false;
  }
  if (*reinterpret_cast<void**>(reinterpret_cast<char*>(window) + 0xB4) == nullptr) {
    return false;
  }
  if (window->geometry.unknown_0030 != 0) {
    return false;
  }
  return true;
}

// OFFSET: 0x00A97410
// FUNCTION: fn_17203
bool UIWindowMgr::HasPendingDialog() {
  return this->unknown_0284 || this->unknown_02CC || this->unknown_02D0 || this->unknown_02D4 || this->unknown_02D8 || this->unknown_02DC ||
         this->unknown_02E0 || this->unknown_02E4 || this->unknown_02E8 || this->unknown_02EC || this->unknown_03B4 || this->unknown_01F0 ||
         this->unknown_01F4;
}

// OFFSET: 0x00A97200
// FUNCTION: fn_17201
bool UIWindowMgr::HasTitleAtActorCell(std::uint32_t aid) {
  CRenderObject* actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAidIncludeMe(aid);
  if (actor == nullptr) {
    return true;
  }

  int titleColumn = 0;
  int titleRow = 0;
  int actorColumn = 0;
  int actorRow = 0;
  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  world->terrain->SpatialCell(actor->render.position.x, actor->render.position.z, actorColumn, actorRow);
  for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    UIWindow* window = *it;
    CRenderObject* owner;
    if (typeid(*window) == typeid(UIMerchantShopTitle)) {
      owner = g_session.FindGameActor(static_cast<UIMerchantShopTitle*>(window)->ownerAid);
      if (owner == nullptr) {
        return true;
      }
    } else if (typeid(*window) == typeid(UIChatRoomTitle)) {
      const int roomId = static_cast<UIChatRoomTitle*>(window)->roomId;
      if (roomId == 0) {
        return true;
      }

      const std::uint32_t ownerAid = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->FindUnknown01AC(roomId);
      if (ownerAid == 0) {
        return true;
      }

      owner = g_session.FindGameActor(ownerAid);
      if (owner == nullptr) {
        return true;
      }
    } else {
      continue;
    }

    world->terrain->SpatialCell(owner->render.position.x, owner->render.position.z, titleColumn, titleRow);
    if (titleColumn == actorColumn && titleRow == actorRow) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00A97490
// FUNCTION: fn_17204
void UIWindowMgr::CancelActiveDialog() {
  UIWindow* window = this->window_03DC ? this->window_03DC : this->unknown_03B0;
  if (window) {
    window->SendMsg(nullptr, 20, 0, 0, 0, 0);
    return;
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
    if ((*entry)->Slot02()) {
      (*entry)->SendMsg(nullptr, 20, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00A96C60
// FUNCTION: fn_17193
bool UIWindowMgr::HasMiniPartyMember(std::uint32_t accountId) {
  for (auto it = this->snapExcluded.begin(); it != this->snapExcluded.end(); ++it) {
    bool matches;
    {
      const MessengerMember member = static_cast<UIMiniPartyWnd*>(*it)->m_member;
      matches = member.accountId == accountId;
    }
    if (matches) {
      return true;
    }
  }
  return false;
}
