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
// FUNCTION: fn_19046
void CEZ2STRScreenEffect::Slot00(int argument) {
  this->particle->submit = reinterpret_cast<ParticleSubmit>(argument);
}

// OFFSET: 0x00B66480
// FUNCTION: fn_19047
void CEZ2STRScreenEffect::Slot23(float argument) {
  if (this->particle) {
    this->particle->m_alpha = argument;
  }
}

// OFFSET: 0x00B664A0
// FUNCTION: fn_19048
void CEZ2STRScreenEffect::Slot29(int id) {
  CEffect::Slot29(id);
  switch (id) {
  case 0x3F4:
  case 0x40A:
  case 0x40B:
  case 0x40C:
  case 0x433:
  case 0x437:
  case 0x43B:
  case 0x43C:
  case 0x43D:
  case 0x43E:
  case 0x43F:
  case 0x441:
  case 0x443:
  case 0x444:
  case 0x445:
  case 0x447:
    this->unknown_00C8 = 1;
    break;
  default:
    if (id >= 0x9B5) {
      this->unknown_00C8 = 1;
    }
    break;
  }
}

// OFFSET: 0x00B66540
// FUNCTION: fn_19049
void CEZ2STRScreenEffect::Slot24(int argument1, int argument2, int argument3, int argument4, int argument5) {
  if (!this->particle) {
    return;
  }

  const vector3d offset = *reinterpret_cast<const vector3d*>(&argument1);
  this->particle->m_windowOffset = offset;
  if (static_cast<unsigned char>(argument4) == 1) {
    this->particle->m_attachToWindow = true;
    this->particle->m_windowId = argument5;
  }
}

// OFFSET: 0x00B665B0
// FUNCTION: fn_19050
void CEZ2STRScreenEffect::Slot21(int argument) {
  this->particle->flags = argument;
}

// OFFSET: 0x00B665D0
// FUNCTION: fn_19051
void CEZ2STRScreenEffect::Slot15(int argument) {
  if (this->particle) {
    this->particle->m_repeat = argument - 1;
  }
}

// OFFSET: 0x00B665F0
// FUNCTION: fn_19052
void CEZ2STRScreenEffect::Slot14(int argument) {
  if (this->particle) {
    this->particle->m_lifeTime = argument;
  }
}

// OFFSET: 0x00B66610
// FUNCTION: fn_19053
void CEZ2STRScreenEffect::Slot17(float argument) {
  if (this->particle) {
    this->particle->m_angle = argument;
  }
}

// OFFSET: 0x00B66630
// FUNCTION: fn_19054
void CEZ2STRScreenEffect::Slot22(float argument) {
  if (this->particle) {
    this->particle->m_scale = argument;
  }
}

// OFFSET: 0x00B66650
// FUNCTION: fn_19055
void CEZ2STRScreenEffect::Slot13(int argument) {
  if (this->particle) {
    this->particle->Advance(*reinterpret_cast<float*>(&argument));
  }
}

// OFFSET: 0x00B66680
// FUNCTION: fn_19056
void CEZ2STRScreenEffect::Slot16(int argument) {
  if (this->particle) {
    this->particle->m_delay = argument;
  }
}

// OFFSET: 0x00B666A0
// FUNCTION: fn_19057
void CEZ2STRScreenEffect::Slot20(int argument) {
  this->name = reinterpret_cast<const char*>(argument);
  FreeParticle(this->particle);
  this->Slot01();
}
