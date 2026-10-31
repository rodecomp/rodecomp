#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWordDisplayWnd.h"

// OFFSET: 0x0085B000
// FUNCTION: fn_10791
void UIWordDisplayWnd::Slot15(int, int) {
}

// OFFSET: 0x0085E370
// FUNCTION: fn_10825
void UIWordDisplayWnd::Paint() {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\전장적표시.bmp")));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, true);
}

// OFFSET: 0x0085FD30
// FUNCTION: fn_10885
void UIWordDisplayWnd::Slot19() {
  CRenderObject* actor = g_session.FindGameActor(this->aid);
  if (this->aid == g_session.m_aid) {
    actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
  }

  if (!actor) {
    return;
  }

  const int screenX = actor->render.screenX;
  const int screenY = actor->render.screenY;
  if (this->screenX == screenX && this->screenX == screenY) {
    return;
  }

  this->screenY = screenY;
  this->screenX = screenX;
  const float scale = actor->render.unknown_005C;
  const int y = static_cast<int>(screenY - static_cast<float>(g_renderer->viewport.height * 100 / 480) * scale);
  const int x = static_cast<int>(screenX - static_cast<float>(g_renderer->viewport.width * 5 / 640) * scale);
  this->Move(x, y);
}

// OFFSET: 0x00865360
// FUNCTION: fn_10923
int UIWordDisplayWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 34) {
    this->aid = argument1;
  }

  return 0;
}

// OFFSET: 0x00850910
// FUNCTION: fn_10682
UIWordDisplayWnd::UIWordDisplayWnd(UIWindowContext& windowContext, UIFrameContext& frameContext) : UIFrameWnd(windowContext, frameContext, 0), aid(0) {
  this->text.clear();
  this->SetSize(24, 24);
}
