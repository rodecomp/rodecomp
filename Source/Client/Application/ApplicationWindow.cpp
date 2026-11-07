#include "Client/Application/ApplicationWindow.h"
#include "Client/Application/ApplicationActivation.h"
#include "Client/Application/Launcher.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/InputCharset.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Client/Timer.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>
#include <shellapi.h>
#include <string>

// GLOBAL: gl_24399
HWND hWnd;

#pragma optimize("y", on)

// OFFSET: 0x03DE9F37
// FUNCTION: fn_45411
HWND __stdcall CreateApplicationWindow(
  DWORD extendedStyle, const char* className, const char*, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu, HINSTANCE instance,
  void* parameter
) {
  DWORD windowStyle = style | WS_SYSMENU;
  return CreateWindowExA(extendedStyle, className, "Ragnarok 2026", windowStyle, x, y, width, height, parent, menu, instance, parameter);
}

#pragma optimize("y", off)

// OFFSET: 0x00E57DD0
// FUNCTION: fn_24360
BOOL ClipApplicationCursor(HWND window) {
  RECT rectangle;
  GetClientRect(window, &rectangle);
  POINT first{rectangle.left, rectangle.top};
  POINT last{rectangle.right, rectangle.bottom};
  ClientToScreen(window, &first);
  ClientToScreen(window, &last);

  RECT clip{first.x, first.y, last.x, last.y};
  return ClipCursor(&clip);
}

// OFFSET: 0x00E599C0
// FUNCTION: fn_45508
BOOL ReleaseApplicationCursor() {
  return ClipCursor(nullptr);
}

// OFFSET: 0x00E5C430
// FUNCTION: fn_24372
LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
  std::uint8_t handled = 0;
  const LRESULT result = g_serverLanguage->Message(window, message, wParam, lParam, handled);
  if (handled) {
    if (g_serverLanguage->candidateCount == 0) {
      g_windowMgr.CloseWindow(0x33);
    } else {
      std::string candidates[9];
      int info[4];
      info[0] = g_serverLanguage->selection % 9;
      const int first = g_serverLanguage->selection - info[0];
      int last = first + 9;
      if (g_serverLanguage->candidateCount < last) {
        last = g_serverLanguage->candidateCount;
      }
      info[1] = (g_serverLanguage->selection + 8) / 9;
      info[2] = (g_serverLanguage->candidateCount + 8) / 9;
      const int count = last - first;
      info[3] = count;
      for (int i = 0; i < count; ++i) {
        candidates[i] = g_serverLanguage->Candidate(first + i);
      }
      g_windowMgr.EnsureWindow(0x33)->SendMsg(nullptr, 0x41, reinterpret_cast<std::intptr_t>(candidates), reinterpret_cast<std::intptr_t>(info), 0, 0);
    }

    if (g_serverLanguage->DisplayLength()) {
      g_windowMgr.EnsureWindow(0x55)->Invalidate();
    } else {
      g_windowMgr.CloseWindow(0x55);
    }

    if (message == WM_IME_COMPOSITION) {
      if (CUIEditCtrlEx* edit = dynamic_cast<CUIEditCtrlEx*>(g_windowMgr.GetHoverWindow())) {
        edit->SendMsg(nullptr, WM_IME_COMPOSITION, wParam, lParam, 0, 0);
      }
    }
    return result;
  }

  switch (message) {
  case WM_CREATE:
    return 0;
  case WM_DESTROY:
    if (std::strlen(g_homepageUrl) != 0) {
      ShellExecuteA(hWnd, "open", g_homepageUrl, nullptr, nullptr, SW_SHOWNORMAL);
    }
    return 0;
  case WM_MOVE:
    if (g_isAppActive != 1 && !GetRenderSystemHolder()->IsWindowed()) {
      GetRenderSystemHolder()->Present(true);
    }
    return 0;
  case WM_ACTIVATE: {
    const short state = static_cast<short>(wParam);
    if (state == WA_INACTIVE) {
      if (g_mouse.active) {
        g_mouse.Reset();
        g_mouse.active = 0;
      }
    } else if ((state == WA_ACTIVE || state == WA_CLICKACTIVE) && g_mouse.active != 1) {
      g_mouse.Reset();
      g_mouse.active = 1;
    }

    if (!GetRenderSystemHolder()->IsWindowed()) {
      if (g_isAppActive == 1) {
        g_restoreSurfaces = 1;
      }
      return 1;
    }
    if (g_isAppActive == 1) {
      g_restoreSurfaces = 1;
      ShowWindow(window, SW_MAXIMIZE);
      return 1;
    }
    CWebBrowser* browser = g_windowMgr.FindBrowser(reinterpret_cast<HWND>(lParam));
    if (browser && browser->MatchesWindow(reinterpret_cast<HWND>(lParam))) {
      SetApplicationActive(1);
    } else {
      ShowWindow(window, SW_MINIMIZE);
    }
    return 1;
  }
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    ValidateRect(window, nullptr);
    return 0;
  case WM_CLOSE:
    g_modalImmediate = 1;
    g_accountLinkedUserDataMgr->Save();
    if (g_modeMgr.GetGameMode()) {
      g_characterLinkedUserDataMgr->Save();
    }
    if (g_modeMgr.GetGameMode()) {
      g_adventurerAgencyMgr->RequestStop();
    }
    if (g_serviceType == 11) {
      ShellExecuteA(hWnd, "open", "http://www.ragnarok.co.in/index.php", nullptr, nullptr, SW_SHOWNORMAL);
    }
    return 0;
  case WM_SETCURSOR:
    if (!GetRenderSystemHolder()->IsWindowed()) {
      if (!g_serviceOptions.Get(ServiceOption::ConfineCursor) && static_cast<short>(lParam) != HTCLIENT) {
        SetCursor(LoadCursorA(nullptr, IDC_ARROW));
        g_mouse.SetX(-100);
        g_mouse.SetY(-100);
      } else {
        SetCursor(nullptr);
      }
    }
    return 0;
  case WM_GETMINMAXINFO:
    if (hWnd) {
      MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
      const RECT bounds = Rect;
      info->ptMinTrackSize.x = bounds.right - bounds.left;
      info->ptMaxTrackSize.x = bounds.right - bounds.left;
      info->ptMinTrackSize.y = bounds.bottom - bounds.top;
      info->ptMaxTrackSize.y = bounds.bottom - bounds.top;
    }
    return 0;
  case WM_NCACTIVATE:
    SetApplicationActive(LOWORD(wParam));
    if (wParam) {
      ResetTimer();
    }
    break;
  case WM_KEYDOWN:
    if (UIWindow* target = g_windowMgr.LookupWindow(0x130)) {
      target->SendMsg(nullptr, 0, wParam, 0, 0, 0);
      return 0;
    }
    if (g_windowMgr.ProcessKeyDown(wParam, lParam, ~(static_cast<unsigned>(lParam) >> 30) & 1)) {
      return 0;
    }
    if (!g_windowMgr.unknown_0025) {
      g_windowMgr.RefreshWindow01C8Edits();
      return 0;
    }
    if (g_windowMgr.IsHoverOnWindow01C8Button() && g_serviceType != 15 && g_windowMgr.ProcessDialogKey(wParam, lParam)) {
      return 0;
    }
    if (!g_windowMgr.GetHoverWindow() || g_windowMgr.GetHoverWindow()->SendMsg(nullptr, WM_KEYDOWN, wParam, lParam, 0, 0) != 3) {
      g_serverLanguage->KeyDown(wParam, lParam);
    }
    if (g_session.unknown_5480 && !g_session.unknown_5488) {
      g_windowMgr.RefreshWindow01C8Edits();
    }
    return 0;
  case WM_KEYUP:
    g_session.m_automaticSkills.DisableAll();
    if (wParam == VK_SNAPSHOT) {
      g_windowMgr.ProcessKeyDown(VK_SNAPSHOT, lParam, 1);
    }
    return 0;
  case WM_CHAR:
    if (!g_windowMgr.LookupWindow(0x130) && g_windowMgr.unknown_0025) {
      if (g_windowMgr.GetHoverWindow() && g_windowMgr.GetHoverWindow()->SendMsg(nullptr, WM_CHAR, wParam, lParam, 0, 0) == 3) {
        return 0;
      }
      g_serverLanguage->Character(wParam, lParam);
    }
    return 0;
  case WM_SYSKEYDOWN:
    if (g_windowMgr.ProcessKeyDown(wParam, lParam, ~(static_cast<unsigned>(lParam) >> 30) & 1)) {
      return 0;
    }
    break;
  case WM_SYSCOMMAND:
    switch (wParam & 0xFFF0) {
    case SC_MOVE:
      if (!GetRenderSystemHolder()->IsWindowed()) {
        GetCursorPos(&g_windowDragStart);
        GetWindowRect(window, &g_windowDragRect);
        g_windowDragging = true;
        if (g_serviceOptions.Get(ServiceOption::ConfineCursor)) {
          ClipCursor(nullptr);
        }
      }
      return 0;
    case SC_MINIMIZE:
    case SC_MOUSEMENU:
    case SC_RESTORE:
    case SC_MONITORPOWER:
      if (GetRenderSystemHolder()->IsWindowed()) {
        return 0;
      }
      break;
    case SC_CLOSE:
      break;
    case SC_KEYMENU:
      if (lParam != ' ') {
        return 0;
      }
      break;
    default:
      return 0;
    }
    break;
  case WM_MOUSEMOVE:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_LBUTTONDBLCLK:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_RBUTTONDBLCLK:
  case WM_MOUSEWHEEL:
    g_mouse.Message(window, message, wParam, lParam);
    break;
  case WM_ENTERMENULOOP:
  case WM_ENTERSIZEMOVE:
    g_isAppActive = 0;
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
    ClipCursor(nullptr);
    g_session.m_automaticSkills.DisableAll();
    break;
  case WM_EXITMENULOOP:
  case WM_EXITSIZEMOVE:
    if (GetActiveWindow() == window || !IsIconic(window)) {
      SetApplicationActive(1);
    } else {
      SetApplicationActive(0);
    }
    break;
  case WM_IME_SETCONTEXT:
    wParam = 1;
    lParam = 0;
    break;
  }
  return DefWindowProcA(window, message, wParam, lParam);
}
