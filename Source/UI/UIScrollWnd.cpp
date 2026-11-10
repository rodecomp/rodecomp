#include "Render/DeviceContext/DCSurface.h"
#include "UI/Window/UIEnums.h"
#include "UI/UIScrollWnd.h"
#include "Render/DeviceContext/DCBitmap.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Renderer.h"
#include "Render/Textures/Texture.h"
#include "UI/Lists/UIScrollBar.h"

#include <bit>
#include <cstdint>
#include <cstdlib>
#include <list>
#include <utility>

// OFFSET: 0x009AC040
// FUNCTION: fn_13770
UIScrollWnd::UIScrollWnd(int width, int height, unsigned char cached)
    : UIWindow(cached), viewWidth(0), viewHeight(0), contentWidth(0), contentHeight(0), horizontalBar(nullptr), scrollBar(nullptr),
      scrollBarWidth(13), scrollBarHeight(13), horizontalScrollPosition(0), scrollPosition(0), horizontalScrollable(0), wheelScroll(0) {
  this->UIWindow::Resize(width, height);
  this->OnCreate(width, height);
}

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

  this->UpdateScrollRange();
}

// OFFSET: 0x009AC7B0
// FUNCTION: fn_13777
void UIScrollWnd::MoveScrollChild(UIWindow* child, int x, int y) {
  child->Move(x, y);
  this->childOrigins[child].first = x;
  this->childOrigins[child].second = y;
  this->UpdateScrollRange();
}

// OFFSET: 0x009AC890
// FUNCTION: fn_13778
bool UIScrollWnd::Update(int redraw) {
  if (this->m_cached == 1) {
    return this->UpdateCached(redraw);
  }

  for (auto it = this->childOrigins.begin(); it != this->childOrigins.end(); ++it) {
    it->first->Move(it->second.first - this->horizontalScrollPosition, it->second.second - this->scrollPosition);
  }

  if (this->m_dirty == 1) {
    this->Paint();
    redraw = 1;
    this->m_dirty = 0;
  }

  bool changed = false;
  for (UIWindow* child : this->m_children) {
    if (child != this->horizontalBar && child != this->scrollBar) {
      if (child->geometry.x + child->geometry.width > 0 && child->geometry.x < this->geometry.width && child->geometry.y + child->geometry.height > 0 &&
          child->geometry.y < this->geometry.height && child->geometry.visible == 1) {
        changed |= this->UpdateChild(child, redraw);
      }
    } else if (child->geometry.visible == 1) {
      changed |= child->Update(redraw);
    }
  }

  if (this->parent && this->parent->m_cached == 0 && (redraw == 1 || changed == 1)) {
    this->parent->geometry.surface->BlendFrom(
      this->geometry.x, this->geometry.y, this->geometry.width, this->geometry.height, *static_cast<CDCBitmap*>(this->geometry.surface)
    );
    return true;
  }
  return false;
}

// OFFSET: 0x009ACB50
// FUNCTION: fn_13779
bool UIScrollWnd::UpdateCached(int redraw) {
  for (auto it = this->childOrigins.begin(); it != this->childOrigins.end(); ++it) {
    it->first->Move(it->second.first, it->second.second);
  }

  if (this->m_dirty == 1) {
    this->Paint();
    this->m_dirty = 0;
    redraw = 1;
  }

  bool changed = false;
  for (UIWindow* child : this->m_children) {
    if (child->geometry.visible == 1) {
      changed |= child->Update(redraw);
    }
  }

  if (redraw == 1 || changed == 1) {
    std::list<RenderPrimitive*> primitives(this->m_primitives);
    for (UIWindow* child : this->m_children) {
      if (child != this->horizontalBar && child != this->scrollBar) {
        primitives.push_back(child->DrawPrimitive());
      } else {
        static_cast<CDCBitmap*>(child->geometry.surface)->UploadTexture(child->m_texture);
        primitives.push_back(this->BuildPrimitive(
          child->geometry.x + this->horizontalScrollPosition, child->geometry.y + this->scrollPosition, 0, 0, child->geometry.width, child->geometry.height,
          child->m_texture, child->Color()
        ));
      }
    }
    g_renderer->RenderToTexture(
      this->contentWidth + this->scrollBarWidth, this->contentHeight + this->scrollBarHeight, primitives, this->m_texture
    );
    changed = true;
  }

  for (auto it = this->childOrigins.begin(); it != this->childOrigins.end(); ++it) {
    it->first->Move(it->second.first - this->horizontalScrollPosition, it->second.second - this->scrollPosition);
  }
  return changed;
}

// OFFSET: 0x009ACF80
// FUNCTION: fn_13780
__declspec(noinline) bool UIScrollWnd::UpdateChild(UIWindow* child, int redraw) {
  if (child->m_dirty == 1) {
    child->Paint();
    redraw = 1;
    child->m_dirty = 0;
  }

  bool changed = false;
  for (UIWindow* window : child->m_children) {
    if (window->geometry.visible == 1) {
      changed |= window->Update(redraw);
    }
  }

  if (redraw != 1 && changed != 1) {
    return false;
  }

  int x = child->geometry.x;
  int width = child->geometry.width;
  int y = child->geometry.y;
  int height = child->geometry.height;
  int sourceX = 0;
  int sourceY = 0;

  const int right = width + x;
  if (x < 0) {
    x = 0;
    if (this->geometry.width < right) {
      sourceX = std::abs(child->geometry.x);
      width = this->geometry.width;
    } else {
      sourceX = std::abs(child->geometry.x);
      width -= sourceX;
    }
  } else if (this->geometry.width < right) {
    width = this->geometry.width - std::abs(x);
  }

  const int bottom = y + height;
  if (y < 0) {
    sourceY = std::abs(y);
    y = 0;
    if (this->geometry.height < bottom) {
      height = this->geometry.height;
    } else {
      height -= sourceY;
    }
  } else if (this->geometry.height < bottom) {
    height = this->geometry.height - std::abs(y);
  }

  this->geometry.surface->CopyFrom(x, y, sourceX, sourceY, width, height, *static_cast<CDCBitmap*>(child->geometry.surface));
  return true;
}

// OFFSET: 0x009AD0C0
// FUNCTION: fn_13781
RenderPrimitive* UIScrollWnd::DrawPrimitive() {
  if (this->m_cached == 0) {
    return UIWindow::DrawPrimitive();
  }

  return this->BuildPrimitive(
    this->geometry.x, this->geometry.y, this->horizontalScrollPosition, this->scrollPosition, this->geometry.width, this->geometry.height, this->m_texture, this->Color()
  );
}

// OFFSET: 0x009AD100
// FUNCTION: fn_13782
RenderPrimitive* UIScrollWnd::BuildPrimitive(int x, int y, int sourceX, int sourceY, int width, int height, CTexture* texture, std::uint32_t color) {
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  primitive->texture = texture;
  const std::int32_t textureWidth = std::bit_cast<std::int32_t>(texture->TextureWidth());
  const std::int32_t textureHeight = std::bit_cast<std::int32_t>(texture->TextureHeight());
  primitive->quad[0].x = static_cast<float>(x);
  primitive->quad[0].y = static_cast<float>(y);
  primitive->quad[0].u = (static_cast<float>(sourceX) + g_textureOffsetX) / static_cast<float>(textureWidth);
  primitive->quad[0].v = (static_cast<float>(sourceY) + g_textureOffsetY) / static_cast<float>(textureHeight);
  primitive->quad[1].x = static_cast<float>(x + width);
  primitive->quad[1].y = static_cast<float>(y);
  primitive->quad[1].u = (static_cast<float>(sourceX + width) + g_textureOffsetX) / static_cast<float>(textureWidth);
  primitive->quad[1].v = (static_cast<float>(sourceY) + g_textureOffsetY) / static_cast<float>(textureHeight);
  primitive->quad[2].x = static_cast<float>(x);
  primitive->quad[2].y = static_cast<float>(y + height);
  primitive->quad[2].u = (static_cast<float>(sourceX) + g_textureOffsetX) / static_cast<float>(textureWidth);
  primitive->quad[2].v = (static_cast<float>(sourceY + height) + g_textureOffsetY) / static_cast<float>(textureHeight);
  primitive->quad[3].x = static_cast<float>(x + width);
  primitive->quad[3].y = static_cast<float>(y + height);
  primitive->quad[3].u = (static_cast<float>(sourceX + width) + g_textureOffsetX) / static_cast<float>(textureWidth);
  primitive->quad[3].v = (static_cast<float>(sourceY + height) + g_textureOffsetY) / static_cast<float>(textureHeight);

  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].z = 0.000001f;
    primitive->quad[i].rhw = 0.999999f;
    primitive->quad[i].diffuse = color;
  }

  return primitive;
}

// OFFSET: 0x009AD3A0
// FUNCTION: fn_13783
__declspec(noinline) void UIScrollWnd::OnCreate(int width, int height) {
  this->horizontalBar = new UIScrollBar();
  this->horizontalBar->Configure(0, height - this->scrollBarHeight, 0, width - this->scrollBarWidth);
  this->AddChild(this->horizontalBar);
  this->scrollBar = new UIScrollBar();
  this->scrollBar->Configure(width - this->scrollBarWidth, 0, 1, height - this->scrollBarHeight);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x009AD4A0
// FUNCTION: fn_13784
void UIScrollWnd::OnWheel(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, 3, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, UM_SCROLLVERT, -3, 0, 0, 0);
  }
}

// OFFSET: 0x009AD4E0
// FUNCTION: fn_13785
void UIScrollWnd::UpdateScrollRange() {
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

  this->horizontalScrollable = 0;
  this->wheelScroll = 0;
  this->viewWidth = this->geometry.width;
  this->viewHeight = this->geometry.height;

  int visibleHeight;
  if (right > this->geometry.width) {
    visibleHeight = this->geometry.height - this->scrollBarHeight;
    this->horizontalScrollable = 1;
    this->viewHeight = visibleHeight;
  } else {
    visibleHeight = this->geometry.height;
  }

  int visibleWidth;
  if (bottom > this->geometry.height) {
    visibleWidth = this->geometry.width - this->scrollBarWidth;
    this->wheelScroll = 1;
    this->viewWidth = visibleWidth;
  } else {
    visibleWidth = this->geometry.width;
  }

  if (this->horizontalScrollable == 0 && right > visibleWidth) {
    this->horizontalScrollable = 1;
    visibleHeight = this->geometry.height - this->scrollBarHeight;
    this->viewHeight = visibleHeight;
  }

  if (this->wheelScroll == 0 && bottom > visibleHeight) {
    visibleWidth = this->geometry.width - this->scrollBarWidth;
    this->wheelScroll = 1;
    this->viewWidth = visibleWidth;
  }

  if (right < visibleWidth) {
    right = visibleWidth;
  }
  this->contentWidth = right;
  if (bottom < visibleHeight) {
    bottom = visibleHeight;
  }
  this->contentHeight = bottom;

  if (this->horizontalScrollable == 1) {
    this->horizontalBar->Resize(visibleWidth, this->scrollBarHeight);
    this->horizontalBar->SetRange(this->contentWidth - this->viewWidth, this->viewWidth, this->contentWidth);
    const int maximum = this->contentWidth - this->viewWidth;
    int position = this->horizontalScrollPosition;
    if (position > maximum) {
      this->horizontalScrollPosition = maximum;
      position = maximum;
    }
    if (position < 0) {
      this->horizontalScrollPosition = 0;
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

  if (this->horizontalScrollable == 0) {
    this->horizontalScrollPosition = 0;
  }
  if (this->wheelScroll == 0) {
    this->scrollPosition = 0;
  }
  this->horizontalBar->Show(this->horizontalScrollable);
  this->scrollBar->Show(this->wheelScroll);
}

// OFFSET: 0x009AD790
// FUNCTION: fn_13786
void UIScrollWnd::Clear() {
  this->horizontalScrollPosition = 0;
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
  this->UpdateScrollRange();
}

// OFFSET: 0x009AD940
// FUNCTION: fn_13789
int UIScrollWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  int delta = argument1;
  switch (message) {
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->scrollPosition;
    if (message == UM_SCROLLVERTPAGEUP) {
      delta = -1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
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
  case UM_SCROLLHORZ:
  case UM_SCROLLHORZPAGEUP:
  case UM_SCROLLHORZPAGEDOWN: {
    const int previous = this->horizontalScrollPosition;
    if (message == UM_SCROLLHORZPAGEUP) {
      delta = -1;
    } else if (message == UM_SCROLLHORZPAGEDOWN) {
      delta = 1;
    }
    this->horizontalScrollPosition = previous + delta;
    if (this->horizontalScrollPosition < 0) {
      this->horizontalScrollPosition = 0;
    }
    if (this->horizontalScrollPosition > this->contentWidth - this->viewWidth) {
      this->horizontalScrollPosition = this->contentWidth - this->viewWidth;
    }
    this->horizontalBar->SetPosition(this->horizontalScrollPosition);
    this->horizontalBar->Invalidate();
    this->Invalidate();
    return this->horizontalScrollPosition - previous;
  }
  }
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
