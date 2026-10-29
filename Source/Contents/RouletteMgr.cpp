#include "Contents/RouletteMgr.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>
#include <new>
#include <utility>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24688
RouletteMgr* g_rouletteMgr = nullptr;

// OFFSET: 0x0080A310

// OFFSET: 0x0080A980

// OFFSET: 0x0080ABA0
// FUNCTION: fn_10073
void RouletteMgr::Reset() {
  std::memset(&this->m_quads, 0, sizeof(this->m_quads));
  this->m_frame = 0;
  this->m_cycle = 0;
  this->m_stage = 0;
  this->unknown_0018 = 0;
  this->m_lastTick = 0;
  this->m_interval = 0;
  this->m_spinning = 0;
}

// OFFSET: 0x0080ABF0

// OFFSET: 0x0080ACA0

// OFFSET: 0x0080ACC0

// OFFSET: 0x0080ADC0

// OFFSET: 0x0080B1D0

// OFFSET: 0x0080B270

// OFFSET: 0x0080B490

// OFFSET: 0x0080B680
