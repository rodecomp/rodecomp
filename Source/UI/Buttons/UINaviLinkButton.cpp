#include "Contents/Navigation/NavigationManager.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Buttons/UINaviLinkButton.h"

extern template std::string::basic_string(const std::string&);
extern template std::size_t std::_Traits_find<std::char_traits<char>>(const char*, std::size_t, std::size_t, const char*, std::size_t) noexcept;

// OFFSET: 0x0086DF60
// FUNCTION: fn_11062
UINaviLinkButton::UINaviLinkButton() : UITextButton() {
}

UINaviLinkButton::~UINaviLinkButton() = default;

// OFFSET: 0x00893050
// FUNCTION: fn_11503
void UINaviLinkButton::OnLBtnDown(int, int) {
  const auto end = this->link.find(g_naviLinkTagEnd);

  if (end == std::string::npos) {
    return;
  }

  const auto start = end - 4;
  const std::string coordinates = this->link.substr(start);
  const int x = DecodeNavigationNumber(coordinates.substr(0, 2));
  const int y = DecodeNavigationNumber(coordinates.substr(2, 2));
  std::string map = this->link.substr(0, start);

  if (map.find(g_naviLinkTagBegin) != std::string::npos) {
    map = map.substr(std::strlen(g_naviLinkTagBegin.c_str()));
  }

  if (!map.empty()) {
    g_navigationManager.SetDestinationMap(map, x, y);
  }
}
