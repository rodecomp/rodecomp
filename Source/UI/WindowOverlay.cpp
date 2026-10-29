#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "UI/Chat/UIWhisperWnd.h"

// OFFSET: 0x008D0DB0
// FUNCTION: fn_12091
void UIWhisperWnd::Slot43() {
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
  DrawAlphaRectangle(
    g_renderer->primitives, this->m_context->textures, g_renderer->queues, x + 1, y + 1, this->geometry.width - 2, this->geometry.height, 0x66000000,
    g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
  DrawAlphaRectangle(
    g_renderer->primitives, this->m_context->textures, g_renderer->queues, x + 1, y + 1, this->geometry.width - 2, 17, 0x449EB6E4,
    g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
}
