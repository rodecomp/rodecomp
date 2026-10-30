#include "UI/Window/UIChangeNameWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

// OFFSET: 0x0092BCA0
// FUNCTION: fn_12660
void UIChangeNameWnd::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\login_interface\\change_name.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
}
