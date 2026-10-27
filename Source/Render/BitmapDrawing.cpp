#include "Render/BitmapDrawing.h"
#include "Render/DeviceContext/DCBitmap.h"
#include "Resource/Bitmap/BitmapResource.h"

// OFFSET: 0x00A8FC30
// FUNCTION: fn_17148
void DrawBitmapResource(CRenderSystemBase& system, int x, int y, const CBitmapRes* bitmap) {
  if (!bitmap) {
    return;
  }

  CDCBitmap* surface = CreateBitmapDC(bitmap->image.width, bitmap->image.height, true);
  surface->Copy(0, 0, bitmap->image.width, bitmap->image.height, bitmap->image.pixels, true);
  surface->Draw(system, x, y, bitmap->image.width, bitmap->image.height, 0xFFFFFFFF);
  delete surface;
}
