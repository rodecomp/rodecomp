#include "Contents/SignBoard/SignBoard.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

// OFFSET: 0x008213D0

// OFFSET: 0x00821440

// OFFSET: 0x008214C0

// OFFSET: 0x00821530

// OFFSET: 0x00821B40
// FUNCTION: fn_10263
void CBmpObjWnd::Slot15(int width, int height) {
}

// OFFSET: 0x00821C50
// FUNCTION: fn_10265
void CBmpObjWnd::Paint() {
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(this->path.c_str()))), false);
}

// OFFSET: 0x00821C90
// FUNCTION: fn_10266
void CSignBoardWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\signboard\\bg_signboard.bmp"))), false);
  this->DrawBitmap(5, 5, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(this->iconPath.c_str()))), true);
}

// OFFSET: 0x00821ED0
// FUNCTION: fn_10269
bool C3DCooRendering::OnProcess() {
  if (UIWindow* window = this->window) {
    const int lift = g_renderer->viewport.height * 81 / 480;
    window->Move(
      this->render.screenX - window->geometry.width / 2,
      static_cast<int>(static_cast<float>(this->render.screenY - window->geometry.height / 2) - static_cast<float>(lift) * this->render.unknown_005C)
    );
  }
  return true;
}

// OFFSET: 0x00821FC0

// OFFSET: 0x00822040

// OFFSET: 0x008220A0

// OFFSET: 0x008220F0

// OFFSET: 0x00822390
