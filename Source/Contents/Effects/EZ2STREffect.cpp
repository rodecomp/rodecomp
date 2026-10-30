#include "Contents/Effects/EZ2STREffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Render/Renderer.h"
#include "Render/ScreenRectangle.h"
#include "Render/TextureQuad.h"

// OFFSET: 0x00B661B0
// FUNCTION: fn_19045
void CEZ2STRScreenEffect::Slot04() {
  CEZ2STRParticle* particle = this->particle;
  if (!particle) {
    return;
  }

  switch (this->id) {
  case 0x647:
  case 0x907:
  case 0x908:
  case 0x909:
  case 0x90A:
  case 0x90B:
  case 0x90D:
  case 0x90E:
  case 0x90F:
    this->overlayColor += 0x2000000;
    break;
  case 0x95B:
  case 0x95C:
  case 0x95F:
    this->overlayColor += 0x10000000;
    break;
  default:
    goto draw;
  }

  if (this->overlayColor >= 0xAA000000u) {
    this->overlayColor = 0xAA000000u;
  }

  DrawBlendedRectangle(
    g_renderer->primitives, g_renderer->queues, 0, 0, g_renderer->viewport.width, g_renderer->viewport.height, this->overlayColor,
    g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );

draw:
  particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
}

// OFFSET: 0x00B66460

// OFFSET: 0x00B66480

// OFFSET: 0x00B664A0

// OFFSET: 0x00B66540

// OFFSET: 0x00B665B0

// OFFSET: 0x00B665D0

// OFFSET: 0x00B665F0

// OFFSET: 0x00B66610

// OFFSET: 0x00B66630

// OFFSET: 0x00B66650

// OFFSET: 0x00B66680

// OFFSET: 0x00B666A0
