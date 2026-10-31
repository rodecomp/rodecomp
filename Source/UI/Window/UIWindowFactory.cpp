#include "UI/Window/UIWindowFactory.h"

#include "Client/Session.h"
#include "Contents/Configuration/ScriptOptions.h"
#include "Render/Renderer.h"
#include "Script/LuaScript.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Shortcuts/UIQuickSlotWnd.h"
#include "UI/Window/UIHotKeyWnd.h"
#include "UI/Window/UISerachStoreInfoWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <string>

struct LayoutWindowContexts {
  UIWindowContext& windows;
  UIFrameContext& frames;
  UIEntryQueueContext& entryQueue;
  UIListBoxContext& listBox;
  QuickSlotPainting& quickSlots;
};

// GLOBAL: gl_25184
bool g_searchStoreInfoCreated;

// OFFSET: 0x00AA1350
// FUNCTION: fn_17208
UIWindow* UIWindowMgr::CreateLayoutWindow(int id) {
  int width = 0;
  int height = 0;
  int x = 0;
  int y = 0;
  if (!g_luaScript->Call("GetWindowInfo", "d>dddd", id, &width, &height, &x, &y)) {
    return nullptr;
  }

  x = g_renderer->viewport.width * x / 1024;
  y = g_renderer->viewport.height * y / 768;

  UIWindow* window = this->LookupWindow(id);
  std::string yKey;
  std::string xKey;
  if (window == nullptr) {
    LayoutWindowContexts& contexts = *reinterpret_cast<LayoutWindowContexts*>(this);
    switch (id) {
    case 0x9C:
      window = new UIHotKeyWnd(contexts.windows, contexts.frames);
      this->unknown_0404 = window;
      break;
    case 0x9D:
      window = new UIEntryQueueWnd(contexts.windows, contexts.frames, contexts.entryQueue, contexts.listBox);
      break;
    case 0x9F:
      window = new UIQuickSlotWnd(contexts.windows, contexts.frames, contexts.quickSlots);
      break;
    case 0xB5:
      window = new UISerachStoreInfoWnd();
      g_searchStoreInfoCreated = true;
      break;
    case 0xD1:
      window = new UIEntryQueueStandByWnd(contexts.windows, contexts.frames, contexts.entryQueue);
      break;
    }

    int savedX;
    int savedY;
    auto saved = this->savedRectangles.find(id);
    if (saved != this->savedRectangles.end() && saved->second != nullptr) {
      savedX = saved->second->x;
      savedY = saved->second->y;
    } else {
      savedX = reinterpret_cast<ScriptOptions&>(g_session).Integer(xKey.c_str(), x);
      savedY = reinterpret_cast<ScriptOptions&>(g_session).Integer(yKey.c_str(), y);
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
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_0934), 0, 0, 0);
    } else if (id == 0x9D) {
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_0944), 0, 0, 0);
    } else if (id == 0xD4) {
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_09D4), 0, 0, 0);
    }
  }

  return window;
}
