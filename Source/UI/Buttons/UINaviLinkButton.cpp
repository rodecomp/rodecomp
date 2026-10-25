#include "UI/Text/NavigationLink.h"
#include "UI/Buttons/UINaviLinkButton.h"

// OFFSET: 0x0086DF60
UINaviLinkButton::UINaviLinkButton(UIWindowContext& windows, UIButtonContext& buttons, NavigationDestination& navigation)
    : UITextButton(windows, buttons), m_navigation(&navigation) {
}

// OFFSET: 0x00873A90
// FUNCTION: fn_11163
UINaviLinkButton::~UINaviLinkButton() = default;

// OFFSET: 0x00893050
// FUNCTION: fn_11503
void UINaviLinkButton::Slot25(int, int) {
  const auto end = this->link.find("</NAVIL>");

  if (end == std::string::npos) {
    return;
  }

  const auto start = end - 4;
  const std::string coordinates = this->link.substr(start);
  const int x = DecodeNavigationNumber(coordinates.substr(0, 2));
  const int y = DecodeNavigationNumber(coordinates.substr(2, 2));
  std::string map = this->link.substr(0, start);

  if (map.find("<NAVIL>") != std::string::npos) {
    map = map.substr(7);
  }

  if (!map.empty()) {
    this->m_navigation->Navigate(map, x, y);
  }
}
