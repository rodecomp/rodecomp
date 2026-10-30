#include "Client/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/GuildMgr.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/EmblemResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIEmblemWnd.h"

#include <string>

// OFFSET: 0x0085CE40
// FUNCTION: fn_10809
void UIEmblemWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);

  const bool framed = g_serviceOptions.Get(243) != 0;
  if (framed && g_session.GetSceneUnknown004C() && !g_session.WorldUnknown0050()) {
    CBitmapRes* frame = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\emblem_frame.bmp")));
    if (frame) {
      this->DrawBitmap(0, 0, &frame->image, true);
    }
  }

  const std::string name = g_guildMgr->GetEmblemFilename(this->guildId);
  CEmblemRes* emblem = static_cast<CEmblemRes*>(ResourceMgr::Instance().Load(name));
  if (emblem && g_session.GetSceneUnknown004C() && !g_session.WorldUnknown0050()) {
    this->DrawBitmap(framed ? 2 : 0, framed ? 2 : 0, &emblem->gif.image, true);
  }
}

// OFFSET: 0x0085FAF0
// FUNCTION: fn_10883
void UIEmblemWnd::Slot19() {
  if (g_serviceOptions.Get(243)) {
    if (this->geometry.width == 24) {
      this->Resize(28, 28);
    }
  } else if (this->geometry.width == 28) {
    this->Resize(24, 24);
  }

  CRenderObject* actor = g_session.FindGameActor(this->aid);
  if (actor) {
    const int screenX = actor->render.screenX;
    const int screenY = actor->render.screenY;
    if (this->screenX != screenX || this->screenX != screenY) {
      this->screenY = screenY;
      this->screenX = screenX;
      const float scale = actor->render.unknown_005C;
      const int x = static_cast<int>(screenX - static_cast<float>(g_renderer->viewport.width * 10 / 640) * scale);
      const int y = static_cast<int>(screenY - static_cast<float>(g_renderer->viewport.height * 100 / 480) * scale);
      this->Move(x, y);
    }
  } else {
    this->Move(-100, -100);
  }

  const std::string name = g_guildMgr->GetEmblemFilename(this->guildId);
  CEmblemRes* emblem = static_cast<CEmblemRes*>(ResourceMgr::Instance().Load(name));
  if (emblem && emblem->gif.frameChanged == 1) {
    this->Invalidate();
  }
}
