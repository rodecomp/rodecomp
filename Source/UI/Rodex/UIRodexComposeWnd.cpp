#include "UI/Rodex/UIRodexComposeWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

// OFFSET: 0x00800DD0
// FUNCTION: fn_10008
void UIRodexScrollBar::Paint() {
  this->ClearSurface(0xFF00FF);

  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    this->FillRegion(0, 0, 5, this->geometry.height, 0xFFFFFF);
    const auto* upper = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\scrollbar_arrow_upper.bmp"))
    );
    this->DrawBitmap(0, 0, &upper->image, false);

    int offset = this->ThumbOffset() + this->arrowLength;
    if (offset < 5) {
      offset = 5;
    }

    int length = this->ThumbLength();
    if (this->geometry.height - 10 < length) {
      length = 1;
    }

    this->FillRegion(0, offset, 5, length, 0xE9E9E9);
    const auto* lower = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\scrollbar_arrow_down.bmp"))
    );
    this->DrawBitmap(0, this->geometry.height - this->arrowLength, &lower->image, false);
  }

  this->minimumThumb = 0;
}
