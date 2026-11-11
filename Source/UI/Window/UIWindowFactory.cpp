#include "UI/Window/UIWindowFactory.h"
#include "UI/Window/UIEnums.h"

#include "Client/Session/Session.h"
#include "Render/Renderer.h"
#include "Script/LuaScript.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Shortcuts/UIQuickSlotWnd.h"
#include "UI/GameSettings/UIHotKeyWnd.h"
#include "UI/Shop/UISerachStoreInfoWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowId.h"

#include <string>

#include "Utility/StdWindowListEmplace.h"

// GLOBAL: gl_25184
bool g_searchStoreInfoCreated;

// OFFSET: 0x00AA1350
// FUNCTION: fn_17208
UIWindow* UIWindowMgr::CreateLayoutWindow(int id) {
  int width = 0;
  int height = 0;
  int x = 0;
  int y = 0;
  if (!g_session.m_hotkeyScript->Call("GetWindowInfo", "d>dddd", id, &width, &height, &x, &y)) {
    return nullptr;
  }

  x = g_renderer->viewport.width * x / 1024;
  y = g_renderer->viewport.height * y / 768;

  UIWindow* window = this->LookupWindow(id);
  std::string yKey;
  std::string xKey;
  if (window == nullptr) {
    switch (id) {
    case WID_HOTKEYWND:
      window = new UIHotKeyWnd();
      this->hotkeyWnd = window;
      break;
    case WID_JOINTO_BATTLEFIELDWND:
      window = new UIEntryQueueWnd();
      break;
    case WID_QUIKSLOTWND:
      window = new UIQuickSlotWnd();
      break;
    case WID_SEARCH_STORE_INFO:
      window = new UISerachStoreInfoWnd();
      g_searchStoreInfoCreated = true;
      break;
    case WID_ENTRYQUEUESTANDBYWND:
      window = new UIEntryQueueStandByWnd();
      break;
    }

    int savedX;
    int savedY;
    auto saved = this->savedRectangles.find(id);
    if (saved != this->savedRectangles.end() && saved->second != nullptr) {
      savedX = saved->second->x;
      savedY = saved->second->y;
    } else {
      savedX = g_session.GetOptionInteger(xKey.c_str(), x);
      savedY = g_session.GetOptionInteger(yKey.c_str(), y);
    }

    window->SetId(id);
    window->SetSize(width, height);
    if (savedX >= 0 && savedX <= g_renderer->viewport.width - window->geometry.width && savedY >= 0 &&
        savedY <= g_renderer->viewport.height - window->geometry.height) {
      x = savedX;
      y = savedY;
    }

    window->Move(x, y);
    this->rendered.push_back(window);
    if (id == 0x9F) {
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->hotkeyPlacement), 0, 0, 0);
    } else if (id == 0x9D) {
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->battlefieldPlacement), 0, 0, 0);
    } else if (id == 0xD4) {
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->entryQueueStandbyPlacement), 0, 0, 0);
    }
  }

  return window;
}
