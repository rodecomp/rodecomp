#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Lists/UIScrollBarNavi.h"

// OFFSET: 0x0086E6E0
// FUNCTION: fn_11075
UIScrollBar_Navi::UIScrollBar_Navi() : UIScrollBar() {
  this->arrowLength = 7;
  this->thickness = 7;
}

UIScrollBar_Navi::~UIScrollBar_Navi() = default;

// OFFSET: 0x0088E930
// FUNCTION: fn_11383
void UIScrollBar_Navi::Paint() {
  this->ClearSurface(0xFFFF00FFu);

  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    CBitmapRes* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\Navi_scroll_up.bmp")));
    CBitmapRes* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\Navi_scroll_mid.bmp")));
    CBitmapRes* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\Navi_scroll_down.bmp")));
    const int interior = (this->geometry.height - (2 * this->arrowLength));
    const int count = interior / this->arrowLength + (interior % this->arrowLength > 0);

    for (int index = 1; index <= count; ++index) {
      this->DrawBitmap(0, (index * this->arrowLength), middle, false);
    }

    this->DrawBitmap(0, 0, up, false);
    this->DrawBitmap(0, (this->geometry.height - this->arrowLength), down, false);
    CBitmapRes* thumbUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\Navi_bar_up.bmp")));
    CBitmapRes* thumbMiddle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\Navi_bar_mid.bmp")));
    CBitmapRes* thumbDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\Navi_bar_down.bmp")));
    const int thumb = (this->arrowLength + ((interior - this->ThumbLength()) * this->position) / this->maximum);
    this->DrawBitmap(0, thumb, thumbUp, true);
    const int length = this->ThumbLength();

    for (int y = (thumb + thumbUp->image.height); y < ((thumb + thumbUp->image.height) + length); y = (y + thumbDown->image.height)) {
      this->DrawBitmap(0, y, thumbDown, true);
    }

    this->DrawBitmap(0, ((thumb + thumbUp->image.height) + length), thumbMiddle, true);
  }

  this->minimumThumb = 0;
  this->parent->Invalidate();
}
