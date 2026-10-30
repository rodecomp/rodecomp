#include "Contents/SignBoard/SignBoard.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

// OFFSET: 0x008213D0
// FUNCTION: fn_10258
C3DCooRendering::~C3DCooRendering() = default;

// OFFSET: 0x00821440
// FUNCTION: fn_10259
CBmpObjWnd::~CBmpObjWnd() = default;

// OFFSET: 0x008214C0
// FUNCTION: fn_10260
CIcon::~CIcon() = default;

// OFFSET: 0x00821530
// FUNCTION: fn_10261
CSignBoardWnd::~CSignBoardWnd() = default;

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
// FUNCTION: fn_10271
void C3DCooRendering::Render(int argument) {
  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, reinterpret_cast<const float*>(argument), &x, &y, &reciprocalDepth);
  this->render.unknown_005C = g_renderer->ScaleDistance(reciprocalDepth) / g_heightScale;
  this->SetTlvert(x, y);
}

// OFFSET: 0x00822040
// FUNCTION: fn_10272
void CIcon::Render(int argument) {
  this->DrawSprite(argument, false, 0.0f);
}

// OFFSET: 0x008220A0
// FUNCTION: fn_10273
void CIcon::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) {
  g_modeMgr.GetGameMode();
  this->CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x008220F0
// FUNCTION: fn_10274
void CIcon::SetRenderInfo(const float* bounds, float x, float y) {
  this->SetTlvert(x, y);
}

// OFFSET: 0x00822390
