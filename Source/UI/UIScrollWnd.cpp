#include "UI/UIScrollWnd.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Textures/Texture.h"
#include "UI/Lists/UIScrollBar.h"

#include <bit>
#include <cstdint>
#include <utility>

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

// OFFSET: 0x009AC5D0
// FUNCTION: fn_13776
void UIScrollWnd::AddScrollChild(UIWindow* child) {
  if (this->horizontalBar) {
    this->RemoveChild(this->horizontalBar, 0);
  }
  if (this->scrollBar) {
    this->RemoveChild(this->scrollBar, 0);
  }
  this->UIWindow::AddChild(child);
  if (this->horizontalBar) {
    this->UIWindow::AddChild(this->horizontalBar);
  }
  if (this->scrollBar) {
    this->UIWindow::AddChild(this->scrollBar);
  }

  for (UIWindow* window : this->m_children) {
    if (window != this->horizontalBar && window != this->scrollBar) {
      this->childOrigins[window] = std::make_pair(window->geometry.x, window->geometry.y);
    }
  }

  this->Slot53();
}

// OFFSET: 0x009AC7B0
// FUNCTION: fn_13777
void UIScrollWnd::MoveScrollChild(UIWindow* child, int x, int y) {
  child->Move(x, y);
  this->childOrigins[child].first = x;
  this->childOrigins[child].second = y;
  this->Slot53();
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

// OFFSET: 0x009AD4E0
// FUNCTION: fn_13785
void UIScrollWnd::Slot53() {
  if (!this->horizontalBar || !this->scrollBar) {
    return;
  }

  int right = 0;
  int bottom = 0;
  for (auto it = this->childOrigins.begin(); it != this->childOrigins.end(); ++it) {
    UIWindow* child = it->first;
    if (child->geometry.visible != 0) {
      const int childRight = child->geometry.width + it->second.first;
      if (right < childRight) {
        right = childRight;
      }
      const int childBottom = it->second.second + child->geometry.height;
      if (bottom < childBottom) {
        bottom = childBottom;
      }
    }
  }

  this->unknown_00A4 = 0;
  this->wheelScroll = 0;
  this->ViewWidth() = this->geometry.width;
  this->viewHeight = this->geometry.height;

  int visibleHeight;
  if (right > this->geometry.width) {
    visibleHeight = this->geometry.height - this->scrollBarHeight;
    this->unknown_00A4 = 1;
    this->viewHeight = visibleHeight;
  } else {
    visibleHeight = this->geometry.height;
  }

  int visibleWidth;
  if (bottom > this->geometry.height) {
    visibleWidth = this->geometry.width - this->scrollBarWidth;
    this->wheelScroll = 1;
    this->ViewWidth() = visibleWidth;
  } else {
    visibleWidth = this->geometry.width;
  }

  if (this->unknown_00A4 == 0 && right > visibleWidth) {
    this->unknown_00A4 = 1;
    visibleHeight = this->geometry.height - this->scrollBarHeight;
    this->viewHeight = visibleHeight;
  }

  if (this->wheelScroll == 0 && bottom > visibleHeight) {
    visibleWidth = this->geometry.width - this->scrollBarWidth;
    this->wheelScroll = 1;
    this->ViewWidth() = visibleWidth;
  }

  if (right < visibleWidth) {
    right = visibleWidth;
  }
  this->contentWidth = right;
  if (bottom < visibleHeight) {
    bottom = visibleHeight;
  }
  this->contentHeight = bottom;

  if (this->unknown_00A4 == 1) {
    this->horizontalBar->Resize(visibleWidth, this->scrollBarHeight);
    this->horizontalBar->SetRange(this->contentWidth - this->ViewWidth(), this->ViewWidth(), this->contentWidth);
    const int maximum = this->contentWidth - this->ViewWidth();
    int position = this->unknown_009C;
    if (position > maximum) {
      this->unknown_009C = maximum;
      position = maximum;
    }
    if (position < 0) {
      this->unknown_009C = 0;
      position = 0;
    }
    this->horizontalBar->SetPosition(position);
    this->horizontalBar->Invalidate();
  }

  if (this->wheelScroll == 1) {
    this->scrollBar->Resize(this->scrollBarWidth, this->viewHeight);
    this->scrollBar->SetRange(this->contentHeight - this->viewHeight, this->viewHeight, this->contentHeight);
    const int maximum = this->contentHeight - this->viewHeight;
    int position = this->scrollPosition;
    if (position > maximum) {
      this->scrollPosition = maximum;
      position = maximum;
    }
    if (position < 0) {
      this->scrollPosition = 0;
      position = 0;
    }
    this->scrollBar->SetPosition(position);
    this->scrollBar->Invalidate();
  }

  if (this->unknown_00A4 == 0) {
    this->unknown_009C = 0;
  }
  if (this->wheelScroll == 0) {
    this->scrollPosition = 0;
  }
  this->horizontalBar->Show(this->unknown_00A4);
  this->scrollBar->Show(this->wheelScroll);
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

// OFFSET: 0x009AD8B0
// FUNCTION: fn_13788
void UIScrollWnd::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  if (this->scrollBar) {
    this->scrollBar->Resize(this->scrollBarWidth, height - this->scrollBarHeight);
    this->scrollBar->Move(width - this->scrollBarWidth, 0);
  }
  if (this->horizontalBar) {
    this->horizontalBar->Resize(width - this->scrollBarWidth, this->scrollBarHeight);
    this->horizontalBar->Move(0, height - this->scrollBarHeight);
  }
  this->Slot53();
}

// OFFSET: 0x009AD940
// FUNCTION: fn_13789
int UIScrollWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 7:
  case 9:
  case 10: {
    const int previous = this->scrollPosition;
    int delta = argument1;
    if (message == 9) {
      delta = -1;
    } else if (message == 10) {
      delta = 1;
    }
    this->scrollPosition = previous + delta;
    if (this->scrollPosition < 0) {
      this->scrollPosition = 0;
    }
    if (this->scrollPosition > this->contentHeight - this->viewHeight) {
      this->scrollPosition = this->contentHeight - this->viewHeight;
    }
    this->scrollBar->SetPosition(this->scrollPosition);
    this->scrollBar->Invalidate();
    this->Invalidate();
    return this->scrollPosition - previous;
  }
  case 8:
  case 11:
  case 12: {
    const int previous = this->unknown_009C;
    int delta = argument1;
    if (message == 11) {
      delta = -1;
    } else if (message == 12) {
      delta = 1;
    }
    this->unknown_009C = previous + delta;
    if (this->unknown_009C < 0) {
      this->unknown_009C = 0;
    }
    if (this->unknown_009C > this->contentWidth - this->ViewWidth()) {
      this->unknown_009C = this->contentWidth - this->ViewWidth();
    }
    this->horizontalBar->SetPosition(this->unknown_009C);
    this->horizontalBar->Invalidate();
    this->Invalidate();
    return this->unknown_009C - previous;
  }
  }
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
