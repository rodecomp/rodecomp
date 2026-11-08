#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
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
  case WID_BASICINFOWND: case WID_CHATWND: case WID_SELECTSERVERWND: case WID_LOGINWND: case WID_MAKECHARWND: case WID_WAITWND: case WID_LOADINGWND: case WID_TOOLTIPWND: case WID_MINIMAPZOOMWND: case WID_ITEMDROPCNTWND: case WID_SAYDIALOGWND: case WID_CHOOSEWND: case WID_MENUWND: case WID_NOTICECONFIRMWND: case WID_NOTIFYLEVELUPWND: case WID_ITEMSHOPWND: case WID_ITEMPURCHASEWND:
  case WID_ITEMSELLWND: case WID_COMBOBOXWND: case WID_CHATROOMWND: case WID_PASSWORDWND: case 31: case WID_EXCHANGEACCEPTWND: case WID_ITEMSTOREWND: case WID_MESSENGERGROUPWND: case WID_JOINPARTYACCEPTWND: case WID_SHORTCUTWND: case WID_CHOOSEWARPWND: case WID_MERCHANTMIRRORITEMWND: case WID_MERCHANTITEMMYSHOPWND: case WID_NOTIFYJOBLEVELUPWND: case WID_ITEMPARAMCHANGEDISPLAYWND: case WID_NPCEDITDIALOGWND:
  case WID_NOTIFYITEMOBTAINWND: case WID_GUILDINFOMANAGEWND: case WID_GUILDMEMBERMANAGEWND: case WID_GUILDPOSITIONMANAGEWND: case WID_GUILDSKILLWND: case WID_GUILDBANISHEDMEMBERWND: case WID_GUILDNOTICEWND: case WID_GUILDTOTALINFOWND: case 67: case WID_JOINGUILDACCEPTWND: case WID_ITEMIDENTIFYWND: case WID_ITEMCOMPOSITIONWND: case WID_ILLUSTWND: case WID_MAKETARGETLISTWND: case WID_MAKETARGETPROCESSWND: case WID_MAKETARGETRESULTWND:
  case WID_PETEGGLISTWND: case WID_PETTAMINGDECEIVEWND: case WID_MAKINGARROWLISTWND: case WID_SELECTCARTWND: case WID_SPELLLISTWND: case WID_NPCTEXTEDITDIALOGWND: case WID_GRAFFISTRBOXWND: case WID_COUPLEACCEPTWND: case WID_BABYACCEPTWND: case WID_ITEMREPAIRWND: case WID_JOINFRIENDACCEPTWND: case WID_WEAPONREFINEWND: case 115: case WID_AUTOMESSAGEWND: case 123:
  case 124: case WID_SELCHARFORUSERVERWND: case WID_CHANGENAMEWND: case WID_SUBCHATHISWND_UNKNOWN_03E4: case WID_SUBCHATHISWND_UNKNOWN_03E8: case WID_SUBCHATMINIWND: case 136: case WID_MEMORIALDUNWND: case WID_YOURITEMWND: case 144: case 154: case WID_HOTKEYWND: case WID_RELOADLUASCRIPTWND: case WID_QUIKSLOTWND:
  case WID_ITEMSHOPWND2: case WID_ITEMSELLWND2: case WID_SEEKPARTYWND: case WID_MERCHANTMIRRORITEMWND_MODE_1: case WID_MERCHANTITEMMYSHOPWND_MODE_1: case WID_MERCHANTITEMSHOPWND_MODE_1: case WID_MERCHANTMIRRORITEMWND_MODE_2: case WID_LOGINOTPWND: case WID_SHOWINFO_TESTLAYER: case WID_SELECTREPLAYDATAWND: case WID_REPLAYCONTROLWND: case WID_MERCHANTSHOPTITLE: case WID_NCASH_CALLWND: case WID_COLLECTIONSYSTEMWND:
  case WID_REPLAYRECCONTROLWND: case WID_SECONDPASSWORDINPUTWND: case WID_SECONDPASSWORDEDITWND: case WID_QUESTDISPLAY: case WID_MULTIILLUSTWND_SLOT_0: case WID_MULTIILLUSTWND_SLOT_1: case WID_MULTIILLUSTWND_SLOT_2: case WID_MULTIILLUSTWND_SLOT_3: case WID_MULTIILLUSTWND_SLOT_4: case WID_MULTIILLUSTWND_SLOT_5: case WID_MULTIILLUSTWND_SLOT_6: case WID_MULTIILLUSTWND_SLOT_7: case WID_MULTIILLUSTWND_SLOT_8: case WID_MULTIILLUSTWND_SLOT_9:
  case WID_SAYDIALOGWND_OVERLAY: case WID_MERGEITEMWND: case WID_GUILD_STORAGE_LOG: case WID_MERCHANTITEMLOGWND_MODE_0: case WID_MERCHANTITEMLOGWND_MODE_1: case WID_OPENMAILBOXWND: case WID_MILEAGEWND: case WID_OPENROULLETTEWND: case WID_ROULLETTEWND: case WID_ACHTRACINGWND: case WID_NEWSELECTCHARWND: case WID_NEWMAKECHARWND: case WID_CAPTCHAREGISTERWND: case WID_MACRODETECTORWND:
  case WID_MACROREPORTERWND: case WID_VIEWCAMERAINFOWND: case WID_REFININGWND: case WID_MINIPARTYWND: case WID_MENUICONWND: case WID_RPIMAGEWND2: case WID_DISCONNECTEDSERVERMSGWND: case WID_GRADEENCHANTWND: case WID_BALLOONOPENBTN: case WID_MACROBLACKLISTCHECKWND: case 600: case 10011: case WID_DEBUFFREMOVEWND:
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
int UIWindowMgr::IsFriend(const char* name) {
  return g_session.IsFriendName(name) != 0;
}

// OFFSET: 0x00A96EB0
// FUNCTION: fn_17196
bool UIWindowMgr::IsSharedWindowId(int id) {
  switch (id) {
  case WID_TOOLTIPWND:
  case 13:
  case WID_SAYDIALOGWND:
  case WID_CHOOSEWND:
  case WID_MENUWND:
  case WID_NOTICECONFIRMWND:
  case WID_MERCHANTITEMMYSHOPWND:
  case 112:
  case WID_HOMUNINFOWND:
  case WID_SKILLLISTWND_SKILLTAB_0:
  case 115:
  case WID_AUTOMESSAGEWND:
  case 117:
  case 118:
  case WID_STARPLACEACCEPTWND:
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
int UIWindowMgr::Window4F250Ready() {
  UIWindow* window = this->window_4F250;
  if (window == nullptr) {
    return false;
  }
  if (*reinterpret_cast<void**>(reinterpret_cast<char*>(window) + 0xB4) == nullptr) {
    return false;
  }
  if (window->geometry.state != 0) {
    return false;
  }
  return true;
}

// OFFSET: 0x00A97410
// FUNCTION: fn_17203
bool UIWindowMgr::HasPendingDialog() {
  return this->exchangeAcceptWnd || this->joinPartyAcceptWnd || this->coupleAcceptWnd || this->starPlaceAcceptWnd || this->mixAcceptWnd || this->yourItemWnd ||
         this->babyAcceptWnd || this->babyAcceptWnd2 || this->joinGuildAcceptWnd || this->allyGuildAcceptWnd || this->joinFriendAcceptWnd || this->sayDialogWnd ||
         this->sayDialogOverlayWnd;
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
  for (UIWindow*& window : this->rendered) {
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
  UIWindow* window = this->window_03DC ? this->window_03DC : this->bookWnd;
  if (window) {
    window->SendMsg(nullptr, UM_LEFTPUSH, 0, 0, 0, 0);
    return;
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
    if ((*entry)->IsFrameWnd()) {
      (*entry)->SendMsg(nullptr, UM_LEFTPUSH, 0, 0, 0, 0);
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
