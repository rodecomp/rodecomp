#include "UI/UIScrollWnd.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Textures/Texture.h"
#include "UI/Lists/UIScrollBar.h"

#include <bit>
#include <cstdint>

// OFFSET: 0x009AC2B0
// FUNCTION: fn_13772
UIScrollWnd::~UIScrollWnd() {
  if (this->horizontalBar) {
    this->RemoveChild(this->horizontalBar, 0);
    delete this->horizontalBar;
    this->horizontalBar = nullptr;
  }
  if (this->scrollBar) {
    this->RemoveChild(this->scrollBar, 0);
    delete this->scrollBar;
    this->scrollBar = nullptr;
  }
}

// OFFSET: 0x009AD0C0
// FUNCTION: fn_13781
RenderPrimitive* UIScrollWnd::DrawPrimitive() {
  if (this->m_cached == 0) {
    return UIWindow::DrawPrimitive();
  }

  return this->BuildPrimitive(
    this->geometry.x, this->geometry.y, this->unknown_009C, this->scrollPosition, this->geometry.width, this->geometry.height, this->m_texture, this->Color()
  );
}

// OFFSET: 0x009AD100
// FUNCTION: fn_13782
RenderPrimitive* UIScrollWnd::BuildPrimitive(int x, int y, int sourceX, int sourceY, int width, int height, CTexture* texture, std::uint32_t color) {
  RenderPrimitive* primitive = this->m_context->primitives.Acquire();
  primitive->texture = texture;
  const float textureWidth = static_cast<float>(std::bit_cast<std::int32_t>(texture->TextureWidth()));
  const float textureHeight = static_cast<float>(std::bit_cast<std::int32_t>(texture->TextureHeight()));
  const float left = static_cast<float>(x);
  const float top = static_cast<float>(y);
  const float right = static_cast<float>(x + width);
  const float bottom = static_cast<float>(y + height);
  const float u0 = (static_cast<float>(sourceX) + g_textureOffsetX) / textureWidth;
  const float v0 = (static_cast<float>(sourceY) + g_textureOffsetY) / textureHeight;
  const float u1 = (static_cast<float>(sourceX + width) + g_textureOffsetX) / textureWidth;
  const float v1 = (static_cast<float>(sourceY + height) + g_textureOffsetY) / textureHeight;

  primitive->quad[0].x = left;
  primitive->quad[0].y = top;
  primitive->quad[0].u = u0;
  primitive->quad[0].v = v0;
  primitive->quad[1].x = right;
  primitive->quad[1].y = top;
  primitive->quad[1].u = u1;
  primitive->quad[1].v = v0;
  primitive->quad[2].x = left;
  primitive->quad[2].y = bottom;
  primitive->quad[2].u = u0;
  primitive->quad[2].v = v1;
  primitive->quad[3].x = right;
  primitive->quad[3].y = bottom;
  primitive->quad[3].u = u1;
  primitive->quad[3].v = v1;

  for (RenderVertex& vertex : primitive->quad) {
    vertex.z = 0.000001f;
    vertex.rhw = 0.999999f;
    vertex.diffuse = color;
  }

  return primitive;
}

// OFFSET: 0x009AD3A0
// FUNCTION: fn_13783
void UIScrollWnd::Slot15(int width, int height) {
  this->horizontalBar = new UIScrollBar(*this->m_context);
  this->horizontalBar->Configure(0, height - this->scrollBarHeight, 0, width - this->scrollBarWidth);
  this->AddChild(this->horizontalBar);
  this->scrollBar = new UIScrollBar(*this->m_context);
  this->scrollBar->Configure(width - this->scrollBarWidth, 0, 1, height - this->scrollBarHeight);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x009AD4A0
// FUNCTION: fn_13784
void UIScrollWnd::Slot35(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, 7, 3, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, 7, -3, 0, 0, 0);
  }
}

// OFFSET: 0x009AD790
// FUNCTION: fn_13786
void UIScrollWnd::Clear() {
  this->unknown_009C = 0;
  this->scrollPosition = 0;
  this->childOrigins.clear();
  this->DeleteChildren();
}

// OFFSET: 0x009AD7C0
// FUNCTION: fn_13787
void UIScrollWnd::RemoveChild(UIWindow* child, unsigned char destroy) {
  if (this->horizontalBar && this->scrollBar) {
    this->childOrigins.erase(child);
  }
  UIWindow::RemoveChild(child, destroy);
}
