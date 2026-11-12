#include "Client/Browser/WebBrowser.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<HWND const, CWebBrowser*>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<HWND const, CWebBrowser*>>>, std::_Iterator_base0>::operator++() noexcept;
template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, UIWindow*>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, UIWindow*>>>, std::_Iterator_base0>::operator++() noexcept;

// OFFSET: 0x00AA26C0
// FUNCTION: fn_17215
void UIWindowMgr::UpdateFrameWindows() {
  for (auto window = this->rendered.begin(); window != this->rendered.end(); ++window) {
    (*window)->OnProcess();
  }

  for (auto browser = this->browsers.begin(); browser != this->browsers.end();) {
    if (browser->second) {
      browser->second->Poll();
    }

    if (this->browsers.empty()) {
      break;
    }

    ++browser;
  }

  for (auto window = this->overlays.begin(); window != this->overlays.end(); ++window) {
    window->second->OnProcess();
  }
}
