#include "Render/DeviceContext.h"
#include "Resource/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/UIScrollBarNavi.h"

#include <bit>

namespace {
  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }
  int Multiply(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) * static_cast<unsigned>(right));
  }
}

// OFFSET: 0x00885EE0
// FUNCTION: fn_11320
UIScrollBar_Navi::UIScrollBar_Navi(UIWindowContext& windows, FrameFocus& focus, ResourceMgr& resources)
    : UIScrollBar(windows, focus, resources), m_resources(&resources) {
  this->arrowLength = 7;
  this->thickness = 7;
}

// OFFSET: 0x00874080
// FUNCTION: fn_11176
UIScrollBar_Navi::~UIScrollBar_Navi() = default;

// OFFSET: 0x0088E930
// FUNCTION: fn_11383
void UIScrollBar_Navi::Paint() {
  this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);

  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    constexpr char prefix[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\navigation_interface3\\";
    const auto load = [&](const char* name) {
      return static_cast<CBitmapRes*>(this->m_resources->Load((std::string(prefix) + name).c_str()));
    };
    const auto draw = [&](CBitmapRes* image, int y, bool transparent) {
      this->DrawBitmap(0, y, image ? &image->image : nullptr, transparent);
    };
    auto* up = load("Navi_scroll_up.bmp");
    auto* middle = load("Navi_scroll_mid.bmp");
    auto* down = load("Navi_scroll_down.bmp");
    const int interior = Add(this->geometry.height, -Multiply(2, this->arrowLength));
    const int count = interior / this->arrowLength + (interior % this->arrowLength > 0);

    for (int index = 1; index <= count; ++index) {
      draw(middle, Multiply(index, this->arrowLength), false);
    }

    draw(up, 0, false);
    draw(down, Add(this->geometry.height, -this->arrowLength), false);
    auto* thumbUp = load("Navi_bar_up.bmp");
    auto* thumbMiddle = load("Navi_bar_mid.bmp");
    auto* thumbDown = load("Navi_bar_down.bmp");
    const int thumb =
      Add(this->arrowLength, Multiply(Add(interior, -this->ThumbLength()), this->position) / this->maximum);
    draw(thumbUp, thumb, true);
    const int end = Add(Add(thumb, thumbUp->image.height), this->ThumbLength());

    for (int y = Add(thumb, thumbUp->image.height); y < end; y = Add(y, thumbDown->image.height)) {
      draw(thumbDown, y, true);
    }

    draw(thumbMiddle, end, true);
  }

  this->minimumThumb = 0;
  static_cast<UIWindow*>(this->parent)->Invalidate();
}
