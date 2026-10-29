#include "Client/Mode/CGameMode.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeEvents.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DCBitmap.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Text/TextContext.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIListBoxNaviSearch.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarNavi.h"
#include "UI/Text/ColoredText.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Navigation/UIPrivateAirplaneWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Window/WindowRegistration.h"

#include <algorithm>
#include <bit>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <functional>
#include <span>
#include <string>
#include <windows.h>
#include <timeapi.h>

// INLINED
inline WindowGeometry::WindowGeometry()
    : width(0), height(0), x(0), y(0), surface(nullptr), visible(1), id(600), unknown_0030(0), unknown_0034(0), targetAlpha(255), alphaStart(timeGetTime()),
      currentAlpha(255) {
}

// INLINED
inline void WindowGeometry::Show(int state) {
  this->visible = state;
}

// INLINED
inline void WindowGeometry::SetId(int value) {
  this->id = value;
}

// INLINED
inline std::uint32_t WindowGeometry::Color() {
  std::uint32_t value = this->currentAlpha;
  if (value != this->targetAlpha) {
    const std::uint32_t step = (timeGetTime() - this->alphaStart) >> 4;
    const std::uint32_t candidate = value < this->targetAlpha ? value + step : value - step;
    const bool keepCandidate = value < this->targetAlpha ? candidate < this->targetAlpha : this->targetAlpha < candidate;

    value = keepCandidate ? candidate : this->targetAlpha;
    this->currentAlpha = value;
  }

  if (value > 255) {
    value = this->targetAlpha;
    this->currentAlpha = value;
  }

  return (value << 24) + 0xFFFFFF;
}

// STATIC: 0x0145723C
// GLOBAL: gl_24364
std::uint32_t g_windowUid = 0xFFFFFFFFu;

// GLOBAL: gl_24611
int g_navigationTab;

// GLOBAL: gl_24632
CTexture* g_navigationMapTexture;

// INLINED
void UIWindowContext::DestroyDeferredChildren() {
  for (UIWindow* window : this->deferredDeletion) {
    delete window;
  }

  this->deferredDeletion.clear();
}

// OFFSET: 0x00A79A90
// FUNCTION: fn_16914
UIWindow::UIWindow(UIWindowContext& context, unsigned char cached)
    : parent(nullptr), unknown_0044(0), unknown_0048(0), unknown_004C(4), m_dirty(1), m_uid(++g_windowUid), m_cached(cached), m_clip(0), m_context(&context) {
}

// OFFSET: 0x00A79E90
// OFFSET: 0x00A7A050
// FUNCTION: fn_16917
UIWindow::~UIWindow() {
  delete this->geometry.surface;
  this->geometry.surface = nullptr;
  this->DeleteChildren();
  this->m_context->registration.Remove(this);
  this->m_context->windowMgr.ForgetWindowReferences(this);
}

// OFFSET: 0x00A79F80
// FUNCTION: fn_16918
bool RunWindowAction(bool primary, bool secondary, int value, int limit, std::function<void()> primaryAction, std::function<void()> secondaryAction) {
  if (!primary && !secondary) {
    return false;
  }

  (primary && (!secondary || value <= limit) ? primaryAction : secondaryAction)();
  return true;
}

// INLINED
UIWindow* UIWindow::Parent() const {
  return this->parent;
}

// OFFSET: 0x00A82EB0
// FUNCTION: fn_16998
void UIWindow::Resize(int width, int height) {
  delete this->geometry.surface;
  this->geometry.surface = CreateBitmapDC(width, height, true);
  this->geometry.width = width;
  this->geometry.height = height;
  this->Invalidate();
}

// OFFSET: 0x005C4650
// FUNCTION: fn_03571
bool UIWindow::Slot02() {
  return false;
}

// OFFSET: 0x005C4670
// FUNCTION: fn_03573
bool UIWindow::Slot03() {
  return false;
}

// OFFSET: 0x00A81D40
// FUNCTION: fn_16987
void UIWindow::Move(int x, int y) {
  this->geometry.Move(x, y);
}

// OFFSET: 0x005C3220
// FUNCTION: fn_03553
bool UIWindow::Slot05() {
  return false;
}

// OFFSET: 0x005C4510
// FUNCTION: fn_03564
UIWindow* UIWindow::Slot06() {
  return this;
}

// OFFSET: 0x005C45C0
// FUNCTION: fn_03567
bool UIWindow::Slot07(void*) {
  return false;
}

// OFFSET: 0x005C4660
// FUNCTION: fn_03572
bool UIWindow::Slot08() {
  return false;
}

// OFFSET: 0x005CA050
// FUNCTION: fn_03638
bool UIWindow::CanHitTest() {
  return true;
}

// OFFSET: 0x00A7BB00
// FUNCTION: fn_16933
void UIWindow::Slot10(int, int, std::intptr_t) {
  this->m_context->mode->SendModeMessage(18, 0, 0, 0, 0);
}

// OFFSET: 0x005CA0F0
// FUNCTION: fn_03640
void UIWindow::Slot11() {
}

// OFFSET: 0x005C7B40
// FUNCTION: fn_03623
void UIWindow::Slot12() {
}

// OFFSET: 0x005C4680
// FUNCTION: fn_03574
void UIWindow::Slot13(int, int) {
}

// OFFSET: 0x005C9630
// FUNCTION: fn_03632
void UIWindow::Show(int visible) {
  this->geometry.Show(visible);
}

// OFFSET: 0x005C6070
// FUNCTION: fn_03584
void UIWindow::Slot15(int, int) {
}

// OFFSET: 0x005C6F10
// FUNCTION: fn_03592
void UIWindow::Slot16() {
}

// OFFSET: 0x005C7340
// FUNCTION: fn_03613
void UIWindow::Grow(int width, int height) {
  this->Resize((this->geometry.width + width), (this->geometry.height + height));
}

// OFFSET: 0x005C6080
// FUNCTION: fn_03585
void UIWindow::Slot18() {
}

// OFFSET: 0x005C72D0
// FUNCTION: fn_03606
void UIWindow::Slot19() {
}

// OFFSET: 0x006006C0
// FUNCTION: fn_04159
void UIWindow::Paint() {
}

// OFFSET: 0x005C7320
// FUNCTION: fn_03611
void UIWindow::Slot21() {
}

// OFFSET: 0x005C7330
// FUNCTION: fn_03612
void UIWindow::Slot22(int, int) {
}

// OFFSET: 0x005C4C70
// FUNCTION: fn_03578
void UIWindow::Slot23() {
}

// OFFSET: 0x005C6F00
// FUNCTION: fn_03591
void UIWindow::Slot24(int) {
}

// OFFSET: 0x005FDA50
// FUNCTION: fn_04116
void UIWindow::Slot25(int, int) {
}

// OFFSET: 0x005C6F30
// FUNCTION: fn_03594
void UIWindow::Slot26(int, int) {
}

// OFFSET: 0x005C7280
// FUNCTION: fn_03602
void UIWindow::Slot27(int, int) {
}

// OFFSET: 0x005C7260
// FUNCTION: fn_03600
void UIWindow::Slot28(int, int) {
}

// OFFSET: 0x005C7290
// FUNCTION: fn_03603
void UIWindow::Slot29(int, int) {
}

// OFFSET: 0x00A81D60
// FUNCTION: fn_16988
void UIWindow::ResetCursor(int, int) {
  this->m_context->cursor.Set(0, this->m_context->cursorTick);
}

// OFFSET: 0x005C7240
// FUNCTION: fn_03598
void UIWindow::Slot31(int, int) {
}

// OFFSET: 0x005C7300
// FUNCTION: fn_03609
void UIWindow::Slot32(int, int) {
}

// OFFSET: 0x005C7310
// FUNCTION: fn_03610
void UIWindow::Slot33(int, int) {
}

// OFFSET: 0x005C72E0
// FUNCTION: fn_03607
void UIWindow::Slot34(int, int) {
}

// OFFSET: 0x005C73A0
// FUNCTION: fn_03615
void UIWindow::Slot35(int) {
}

// OFFSET: 0x005C7AB0
// FUNCTION: fn_03620
void UIWindow::Slot36() {
}

// OFFSET: 0x00A83660
// FUNCTION: fn_17005
int UIWindow::SendMsg(UIWindow*, int message, std::intptr_t, std::intptr_t, std::intptr_t, std::intptr_t) {
  if ((message == 0 || message == 1) && this->parent) {
    this->Parent()->SendMsg(this, message, 0, 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x00A81C30
// FUNCTION: fn_16983
void UIWindow::Invalidate() {
  this->m_dirty = 1;
  if (this->parent && this->Parent()->m_cached == 1) {
    this->Parent()->Invalidate();
  }
}

// OFFSET: 0x00A81C50
// FUNCTION: fn_16984
void UIWindow::InvalidateChildren() {
  this->m_dirty = 1;
  this->Slot39();

  for (UIWindow* child : this->m_children) {
    if (child->geometry.visible != 0) {
      child->InvalidateChildren();
    }
  }
}

// OFFSET: 0x005C4630
// FUNCTION: fn_03569
void UIWindow::Slot39() {
}

// OFFSET: 0x00A7D6F0
// FUNCTION: fn_16954
std::uint32_t UIWindow::Color() {
  return this->geometry.Color();
}

// OFFSET: 0x00A7B710
// FUNCTION: fn_16931
bool UIWindow::Update(int redraw) {
  if (this->m_dirty == 1) {
    this->Paint();
    this->m_dirty = 0;
    redraw = 1;
  }

  bool changed = false;

  for (UIWindow* child : this->m_children) {
    if (child->geometry.visible == 1) {
      changed = child->Update(redraw) || changed;
    }
  }

  if (this->m_cached == 1) {
    if (redraw != 1 && !changed) {
      return false;
    }

    std::list<RenderPrimitive*> primitives(this->m_primitives);

    for (UIWindow* child : this->m_children) {
      if (child->geometry.visible == 1) {
        primitives.push_back(child->DrawPrimitive());
      }
    }

    this->m_context->renderer.RenderToTexture(this->geometry.width, this->geometry.height, primitives, this->m_texture);
    return this->parent != nullptr;
  }

  if (!this->parent || (redraw != 1 && !changed)) {
    return false;
  }

  UIWindow* parentWindow = this->Parent();
  if (parentWindow->m_cached != 0) {
    return true;
  }

  CDCBitmap& source = *static_cast<CDCBitmap*>(this->geometry.surface);
  if (this->m_clip == 0) {
    parentWindow->geometry.surface->BlendFrom(this->geometry.x, this->geometry.y, this->geometry.width, this->geometry.height, source);
    return true;
  }

  int x = this->geometry.x >= 0 ? this->geometry.x : 0;
  int y = this->geometry.y >= 0 ? this->geometry.y : 0;
  int sourceX = this->geometry.x >= 0 ? 0 : (0 - this->geometry.x);
  int sourceY = this->geometry.y >= 0 ? 0 : (0 - this->geometry.y);
  int width = this->geometry.x >= 0 ? this->geometry.width : (this->geometry.width + this->geometry.x);
  int height = this->geometry.y >= 0 ? this->geometry.height : (this->geometry.height + this->geometry.y);

  if (parentWindow->geometry.width < (x + width)) {
    width = (parentWindow->geometry.width - x);
  }

  if (parentWindow->geometry.height < (y + height)) {
    height = (parentWindow->geometry.height - y);
  }

  if (width > 0 && height > 0) {
    parentWindow->geometry.surface->CopyFrom(x, y, sourceX, sourceY, width, height, source);
  }

  return true;
}

// OFFSET: 0x005C45B0
// FUNCTION: fn_03566
bool UIWindow::Slot42(void*) {
  return false;
}

// OFFSET: 0x005C33D0
// FUNCTION: fn_03556
void UIWindow::Slot43() {
}

// OFFSET: 0x005C4480
// FUNCTION: fn_03560
std::vector<std::uint32_t> UIWindow::Slot44() {
  std::vector<std::uint32_t> result;
  return result;
}

// OFFSET: 0x005C8F60
// FUNCTION: fn_03629
void UIWindow::SetId(int value) {
  this->geometry.SetId(value);
}

// OFFSET: 0x005C4500
// FUNCTION: fn_03563
int UIWindow::Slot46() {
  return 0;
}

// OFFSET: 0x005C7250
// FUNCTION: fn_03599
void UIWindow::Slot47() {
}

// OFFSET: 0x005C7270
// FUNCTION: fn_03601
void UIWindow::Slot48() {
}

// OFFSET: 0x00A7FFB0
// FUNCTION: fn_16970
RenderPrimitive* UIWindow::DrawPrimitive() {
  if (this->m_cached == 0) {
    static_cast<CDCBitmap*>(this->geometry.surface)->UploadTexture(this->m_context->renderSystem, this->m_texture);
  }

  RenderPrimitive* primitive = this->m_context->primitives.Acquire();
  primitive->texture = this->m_texture;
  const float width = static_cast<float>(std::bit_cast<std::int32_t>(this->m_texture->TextureWidth()));
  const float height = static_cast<float>(std::bit_cast<std::int32_t>(this->m_texture->TextureHeight()));
  const float x = static_cast<float>(this->geometry.x);
  const float y = static_cast<float>(this->geometry.y);
  const float right = static_cast<float>((this->geometry.x + this->geometry.width));
  const float bottom = static_cast<float>((this->geometry.y + this->geometry.height));
  const float u0 = g_textureOffsetX / width;
  const float v0 = g_textureOffsetY / height;
  const float u1 = (static_cast<float>(this->geometry.width) + g_textureOffsetX) / width;
  const float v1 = (static_cast<float>(this->geometry.height) + g_textureOffsetY) / height;

  primitive->quad[0].x = x;
  primitive->quad[0].y = y;
  primitive->quad[0].u = u0;
  primitive->quad[0].v = v0;
  primitive->quad[1].x = right;
  primitive->quad[1].y = y;
  primitive->quad[1].u = u1;
  primitive->quad[1].v = v0;
  primitive->quad[2].x = x;
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
    vertex.diffuse = this->Color();
  }

  return primitive;
}

// OFFSET: 0x00A81B10
// FUNCTION: fn_16981
UIWindow* UIWindow::HitTest(int x, int y) {
  if (!this->CanHitTest() || this->geometry.visible == 0) {
    return nullptr;
  }

  if (x < this->geometry.x || x >= (this->geometry.x + this->geometry.width) || y < this->geometry.y || y >= (this->geometry.y + this->geometry.height)) {
    return nullptr;
  }

  for (auto child = this->m_children.rbegin(); child != this->m_children.rend(); ++child) {
    UIWindow* hit = (*child)->HitTest((x - this->geometry.x), (y - this->geometry.y));
    if (hit) {
      return hit;
    }
  }

  return this;
}

// OFFSET: 0x00A7C4B0
// FUNCTION: fn_16945
void UIWindow::VisitChildren() {
  for (UIWindow* child : this->m_children) {
    if (child) {
      child->VisitChildren();
    }
  }
}

// OFFSET: 0x005C7AC0
// FUNCTION: fn_03621
void UIWindow::Slot52(int) {
}

// OFFSET: 0x00A7A080
// FUNCTION: fn_16920
void UIWindow::AddChild(UIWindow* child) {
  child->parent = this;
  this->m_children.push_back(child);
}

// OFFSET: 0x00A7A0A0
// FUNCTION: fn_16921
void UIWindow::AddChildFront(UIWindow* child) {
  child->parent = this;
  this->m_children.push_front(child);
}

// OFFSET: 0x00A81A30
// FUNCTION: fn_16979
UIWindow* UIWindow::FindUid(std::uint32_t uid) {
  if (this->m_uid == uid) {
    return this;
  }

  for (UIWindow* child : this->m_children) {
    if (UIWindow* found = child->FindUid(uid)) {
      return found;
    }
  }

  return nullptr;
}

// OFFSET: 0x00A7D6D0
// FUNCTION: fn_16953
UIWindow* UIWindow::Root() {
  UIWindow* root = this;

  while (root->parent) {
    root = root->parent;
  }

  return root;
}

// OFFSET: 0x00A7ED50
// FUNCTION: fn_16960
UIWindow* UIWindow::NextFocusable(UIWindow* child) {
  auto it = this->m_children.begin();

  for (; it != this->m_children.end(); ++it) {
    if (*it == child && child->Slot05()) {
      break;
    }
  }

  if (it == this->m_children.end()) {
    return nullptr;
  }

  for (++it; it != this->m_children.end(); ++it) {
    if ((*it)->Slot05()) {
      return *it;
    }
  }

  return nullptr;
}

// OFFSET: 0x00A7FF30
// FUNCTION: fn_16969
UIWindow* UIWindow::PreviousFocusable(UIWindow* child) {
  auto it = this->m_children.end();

  for (; it != this->m_children.begin(); --it) {
    UIWindow* window = *std::prev(it);
    if (window == child && window->Slot05()) {
      break;
    }
  }

  if (it == this->m_children.begin()) {
    return nullptr;
  }

  for (--it; it != this->m_children.begin(); --it) {
    UIWindow* window = *std::prev(it);
    if (window->Slot05()) {
      return window;
    }
  }

  return nullptr;
}

// OFFSET: 0x00A82DD0
// FUNCTION: fn_16995
void UIWindow::DeleteChildren() {
  for (UIWindow* child : this->m_children) {
    delete child;
  }

  this->m_children.clear();
}

// OFFSET: 0x00A82E30
// FUNCTION: fn_16996
void UIWindow::ClearChildren(bool destroy) {
  if (destroy) {
    this->DeleteChildren();
  } else {
    this->m_children.clear();
  }
}

// OFFSET: 0x00AA5E90
// FUNCTION: fn_17226
void UIWindow::DeferDeletion(UIWindow* window) {
  if (!window) {
    return;
  }

  this->m_context->windowMgr.ForgetWindowReferences(window);
  this->m_context->deferredDeletion.insert(window);
  window->Show(0);
}

// OFFSET: 0x00A82E50
// FUNCTION: fn_16997
void UIWindow::RemoveChild(UIWindow* child, unsigned char destroy) {
  if (!child) {
    return;
  }

  for (auto entry = this->m_children.begin(); entry != this->m_children.end(); ++entry) {
    if (*entry == child) {
      if (destroy == 1) {
        this->DeferDeletion(child);
      }

      this->m_children.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x00A81CC0
// FUNCTION: fn_16986
bool UIWindow::Contains(const UIWindow* window) const {
  if (!window) {
    return false;
  }

  if (window == this) {
    return true;
  }

  for (UIWindow* child : this->m_children) {
    if (child->Contains(window)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00A7B430
// FUNCTION: fn_16927
void UIWindow::ClearSurface(std::uint32_t color) {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, color);
  }
}

// OFFSET: 0x00A7B470
// FUNCTION: fn_16929
void UIWindow::SetSize(int width, int height) {
  delete this->geometry.surface;
  this->geometry.surface = CreateBitmapDC(width, height, true);
  this->geometry.width = width;
  this->geometry.height = height;
  this->Invalidate();
  this->Slot15(width, height);
  this->Slot22(width, height);
  this->Slot16();
}

// OFFSET: 0x00A7BB60
// FUNCTION: fn_16935
void UIWindow::DrawBitmap(int x, int y, const BitmapImage* image, bool transparent) {
  if (!this->geometry.surface || !image) {
    return;
  }

  static_cast<CDCBitmap*>(this->geometry.surface)->SetAlpha(image->alpha != 0);
  this->geometry.surface->Copy(x, y, image->width, image->height, image->pixels, transparent);
}

// OFFSET: 0x00A7BD60
// FUNCTION: fn_16938
void UIWindow::FillRegion(int x, int y, int width, int height, std::uint32_t color) {
  const int right = (x + width);
  const int bottom = (y + height);
  RECT rectangle = {
    x < 0 ? 0 : x, y < 0 ? 0 : y, this->geometry.width < right ? this->geometry.width : right, this->geometry.height < bottom ? this->geometry.height : bottom
  };
  this->geometry.surface->Fill(&rectangle, color);
}

// OFFSET: 0x00A80580
// FUNCTION: fn_16975
int UIWindow::TextWidth(
  FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
) {
  if (!text) {
    return 0;
  }

  if (!bytes) {
    bytes = static_cast<unsigned>(std::strlen(text));
    if (!bytes) {
      return 0;
    }
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, italic);
  return context.Measure(text, bytes).cx;
}

// OFFSET: 0x00A84360
// FUNCTION: fn_17009
void UIWindow::DrawTextAt(
  FontCache& fonts, const TextLocale& locale, int x, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold,
  unsigned char italic
) {
  if (!bytes) {
    bytes = static_cast<int>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, italic);
  context.SetColor(color);
  context.Draw(x, y, text, bytes);
}

// OFFSET: 0x00A84590
// FUNCTION: fn_17011
void UIWindow::DrawVerticalText(int x, int y, const char* text, unsigned bytes, int family, int height, std::uint32_t color, unsigned char bold) {
  if (!bytes) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  const char* const end = text + bytes;
  const char* const marker = text + TextContext::Marker(text, bytes);
  int widest = 0;
  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  context.SetColor(color);
  for (const char* current = text; current < end;) {
    const char* const next = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    const SIZE size = context.Measure(current, static_cast<int>(next - current));
    current = next;
    if (widest < size.cx) {
      widest = size.cx;
    }
  }

  if (marker && *marker) {
    text = marker + 3;
  }

  for (const char* current = text; current < end;) {
    const char* const next = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    const SIZE size = context.Measure(text, static_cast<int>(next - text));
    context.Draw(x + (widest - size.cx) / 2, y, text, static_cast<int>(next - text));
    current = next;
    text = next;
    y += size.cy;
  }
}

// OFFSET: 0x00A847C0
// FUNCTION: fn_17012
void UIWindow::DrawCenteredText(int x, int y, const char* text, int width, int family, int height, std::uint32_t color, unsigned char bold) {
  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  context.SetColor(color);
  const int bytes = static_cast<int>(std::strlen(text));
  if (width) {
    const int extent = context.Measure(text, bytes).cx;
    context.Draw(static_cast<int>((width - extent) * 0.5 + x), y, text, static_cast<int>(std::strlen(text)));
  } else {
    context.Draw(x, y, text, bytes);
  }
}

// OFFSET: 0x00A7C470
// FUNCTION: fn_16944
void UIWindow::FlushPrimitives(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd) {
  for (RenderPrimitive* primitive : this->m_overlayPrimitives) {
    queues.Submit(primitive, 513, depthAlpha, depthStart, depthEnd);
  }

  this->m_overlayPrimitives.clear();
}

// OFFSET: 0x00A7B450
// FUNCTION: fn_16928
void UIWindow::FillSurface(std::uint32_t color, const RECT* rectangle) {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(rectangle, color);
  }
}

// OFFSET: 0x00A7BB20
// FUNCTION: fn_16934
void UIWindow::DrawBitmapScaled(int x, int y, int width, int height, const BitmapImage* image, int option) {
  if (!this->geometry.surface || !image) {
    return;
  }

  this->geometry.surface->ScaleCopy(x, y, image->width, image->height, width, height, image->pixels, option);
}

// OFFSET: 0x00A7BBB0
// FUNCTION: fn_16936
void UIWindow::DrawBitmapRegion(int x, int y, const BitmapImage* image, bool transparent, bool whole, int left, int top, int right, int bottom) {
  if (!this->geometry.surface || !image) {
    return;
  }

  static_cast<CDCBitmap*>(this->geometry.surface)->SetAlpha(image->alpha != 0);
  if (whole) {
    this->geometry.surface->CopyRegion(x, y, 0, 0, image->width, image->height, image->pixels, image->width, image->height, transparent, false);
  } else {
    this->geometry.surface->CopyRegion(x, y, left, top, right, bottom, image->pixels, image->width, image->height, transparent, false);
  }
}

// OFFSET: 0x00A7BDD0
// FUNCTION: fn_16939
void UIWindow::DrawAuxiliaryScaled(int mode, int x, int y, int width, int height, BitmapImage* image, int option, bool rebuild) {
  if (!this->geometry.surface || !image) {
    return;
  }

  if (rebuild) {
    image->ClearAuxiliary();
  }

  image->CreateAuxiliary(mode);
  if (image->auxiliary) {
    this->geometry.surface->ScaleCopy(x, y, image->width, image->height, width, height, static_cast<const std::uint32_t*>(image->auxiliary), option);
  }
}

// OFFSET: 0x00A7BE30
// FUNCTION: fn_16940
void UIWindow::DrawAuxiliary(int mode, int x, int y, BitmapImage* image, bool transparent, bool rebuild) {
  if (!this->geometry.surface || !image) {
    return;
  }

  if (rebuild) {
    image->ClearAuxiliary();
  }

  image->CreateAuxiliary(mode);
  if (image->auxiliary) {
    this->geometry.surface->Copy(x, y, image->width, image->height, static_cast<const std::uint32_t*>(image->auxiliary), transparent);
  }
}

// OFFSET: 0x00A7BE90
// FUNCTION: fn_16941
void UIWindow::DrawAmount(FontCache& fonts, const TextLocale& locale, int amount, std::string text, int x, int y) {
  COLORREF color = 0;
  COLORREF shadow = 0;
  bool outlined = true;

  if (amount >= 1000000000) {
    color = 0x7F00FF;
    shadow = 0xFF;
  } else if (amount >= 100000000) {
    color = 0x64C8C8;
  } else if (amount >= 10000000) {
    color = 0xFF;
    outlined = false;
  } else if (amount >= 1000000) {
    color = 0xFF00;
  } else if (amount >= 100000) {
    color = 0xFF0000;
    outlined = false;
  } else if (amount >= 10000) {
    color = 0xFF1EFF;
    outlined = false;
  } else if (amount >= 1000) {
    color = 0xFFFF;
    shadow = 0xFF;
  } else if (amount >= 100) {
    color = 0xFFFF00;
    shadow = 0xFF0000;
  } else if (amount >= 10) {
    color = 0xC800C8;
    shadow = 0xFF0000;
  } else {
    color = 0xFFFF00;
  }

  const char* characters = text.c_str();
  const int bytes = static_cast<int>(std::strlen(characters));
  TextContext context(*this->geometry.surface);
  context.SetFont(0, 12, 0, 0);
  context.SetColor(color);
  if (outlined) {
    context.Draw(x, y, characters, bytes);
    context.SetColor(shadow);
    context.Draw(x - 1, y, characters, bytes);
  } else {
    context.Draw(x, y, characters, bytes);
  }
}

// OFFSET: 0x00A80170
// FUNCTION: fn_16971
int UIWindow::TextHeight(
  FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
) {
  if (!text) {
    return 0;
  }

  if (!bytes) {
    bytes = static_cast<unsigned>(std::strlen(text));
    if (!bytes) {
      return 0;
    }
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, italic);
  return context.Measure(text, bytes).cy;
}

// OFFSET: 0x00A80230
// FUNCTION: fn_16972
int UIWindow::VerticalTextHeight(FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold) {
  if (!text) {
    return 0;
  }

  if (!bytes) {
    bytes = static_cast<unsigned>(std::strlen(text));
    if (!bytes) {
      return 0;
    }
  }

  const char* end = text + bytes;
  int total = 0;
  const char* marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));
  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  if (marker && *marker) {
    text = marker + 3;
  }

  while (text < end) {
    const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), text, 0);
    total += context.Measure(text, static_cast<int>(next - text)).cy;
    text = next;
  }

  return total;
}

// OFFSET: 0x00A80340
// FUNCTION: fn_16973
SIZE UIWindow::TextSize(
  FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
) {
  SIZE size = {};
  if (!text) {
    return size;
  }

  if (!bytes) {
    bytes = static_cast<unsigned>(std::strlen(text));
    if (!bytes) {
      return size;
    }
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, italic);
  size = context.Measure(text, bytes);
  return size;
}

// OFFSET: 0x00A80430
// FUNCTION: fn_16974
SIZE UIWindow::VerticalTextSize(
  FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
) {
  SIZE size = {};
  if (!text) {
    return size;
  }

  if (!bytes) {
    bytes = static_cast<unsigned>(std::strlen(text));
    if (!bytes) {
      return size;
    }
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, italic);
  const char* end = text + bytes;
  const char* marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));
  if (marker && *marker) {
    text = marker + 3;
  }

  while (text < end) {
    const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), text, 0);
    const SIZE part = context.Measure(text, static_cast<int>(next - text));
    if (size.cx < part.cx) {
      size.cx = part.cx;
    }

    size.cy += part.cy;
    text = next;
  }

  return size;
}

// OFFSET: 0x00A80640
// FUNCTION: fn_16976
int UIWindow::FormattedTextWidth(FontCache& fonts, const TextLocale& locale, std::string text, int family, int height, unsigned char bold) {
  int total = 0;
  unsigned char currentBold = bold;
  const char* current = text.c_str();
  const char* end = current + std::strlen(current);

  while (current != end) {
    if (*current == '^') {
      if (current[1] == 'b') {
        currentBold = 1;
        current += 2;
        continue;
      }

      if (current[1] == '/' && current[2] == 'b') {
        currentBold = bold;
        current += 3;
        continue;
      }

      char color[7] = {};
      strncpy_s(color, sizeof(color), current + 1, 6);
      char* parsed = nullptr;
      std::strtol(color, &parsed, 16);
      if (parsed - color == 6) {
        current += 7;
        continue;
      }
    } else if (current[1] == '\\' && current[2] == 'n') {
      current += 2;
      continue;
    }

    const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    const std::string character(current, next);
    total += this->TextWidth(fonts, locale, character.c_str(), 0, family, height, currentBold, 0);
    current = next;
  }

  return total;
}

// OFFSET: 0x00A836C0
// FUNCTION: fn_17006
int UIWindow::RestoreEditTexts(const unsigned char* data) {
  int offset = 4;
  int count;
  std::memcpy(&count, data, sizeof(count));
  if (!count) {
    return 0;
  }

  char text[260];
  std::memset(text, 0, sizeof(text));

  for (UIWindow* child : this->m_children) {
    if (child->Slot46() != 2) {
      continue;
    }

    int length;
    std::memcpy(&length, data + offset, sizeof(length));
    offset += 4;
    if (!length) {
      continue;
    }

    std::memset(text, 0, sizeof(text));
    std::memcpy(text, data + offset, length);
    offset += length;
    static_cast<UIEditCtrl*>(child)->SetText(text);
    child->Invalidate();
  }

  return offset;
}

// OFFSET: 0x00A83FD0
// FUNCTION: fn_17008
bool UIWindow::FitSprite(const ActionFrame* frame, const CSprRes* sprite, int width, int height, int* x, int* y, float* scale) {
  if (!width || !height || !frame || !sprite || !x || !y || !scale) {
    return false;
  }

  const SpriteFrameCell& first = frame->Cell(0);
  if (first.image == -1) {
    return false;
  }

  const SpriteRleImage* image = sprite->images[first.imageType][first.image];
  if (!image) {
    return false;
  }

  int left = static_cast<int>(static_cast<float>(first.x) * first.scaleX);
  int top = static_cast<int>(static_cast<float>(first.y) * first.scaleY);
  int right = static_cast<int>(static_cast<float>(image->width) * first.scaleX) + left;
  int bottom = static_cast<int>(static_cast<float>(image->height) * first.scaleY) + top;

  for (int index = 1; index < frame->cellCount; ++index) {
    const SpriteFrameCell& cell = frame->Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* part = sprite->images[cell.imageType][cell.image];
    if (!part) {
      continue;
    }

    const int cellX = static_cast<int>(static_cast<float>(cell.x) * cell.scaleX);
    const int cellWidth = static_cast<int>(static_cast<float>(part->width) * cell.scaleX);
    const int cellY = static_cast<int>(static_cast<float>(cell.y) * cell.scaleY);
    const int cellHeight = static_cast<int>(static_cast<float>(part->height) * cell.scaleY);
    if (left > cellX) {
      left = cellX;
    }

    if (right < cellX + cellWidth) {
      right = cellX + cellWidth;
    }

    if (top > cellY) {
      top = cellY;
    }

    if (bottom < cellY + cellHeight) {
      bottom = cellY + cellHeight;
    }
  }

  const int spanX = std::abs(left - right);
  const int spanY = std::abs(top - bottom);
  bool horizontal = true;
  float fit;
  if (spanX < spanY) {
    horizontal = false;
    fit = height < spanY ? static_cast<float>(height) / static_cast<float>(spanY) : 1.0f;
  } else {
    fit = width < spanX ? static_cast<float>(width) / static_cast<float>(spanX) : 1.0f;
  }

  *scale = fit;
  if (fit == 1.0) {
    *x = -left - static_cast<int>(static_cast<float>(width - spanX) * -0.5f);
    *y = -top - static_cast<int>(static_cast<float>(height - spanY) * -0.5f);
    return true;
  }

  *x = static_cast<int>(static_cast<float>(-left) * fit);
  *y = static_cast<int>(static_cast<float>(-top) * *scale);
  if (horizontal) {
    *y -= static_cast<int>((static_cast<float>(height) - static_cast<float>(spanY) * *scale) * -0.5f);
  } else {
    *x += static_cast<int>((static_cast<float>(width) - static_cast<float>(spanX) * *scale) * 0.5f);
  }

  return true;
}

// OFFSET: 0x005C3340
// FUNCTION: fn_03554
void UIListBox_NaviSearch::Clear() {
  UIListBox::Clear();
  this->unknown_00D8 = this->unknown_00D4;
}

// OFFSET: 0x005C6F20
// FUNCTION: fn_03593
void UIListBox_NaviSearch::Slot26(int, int) {
}

// OFFSET: 0x005C6F40
// FUNCTION: fn_03595
void UIListBox_NaviSearch::Slot25(int x, int y) {
  this->selected = this->LineAt(x, y);
  if (this->selected != -1) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->selected, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x005C7360
// FUNCTION: fn_03614
void UIListBox_NaviSearch::Slot35(int value) {
  if (this->verticalOverflow != 0) {
    this->SendMsg(this->verticalBar, 7, value >= 0 ? -6 : 6, 0, 0, 0);
  }
}

// OFFSET: 0x005C7AD0
// FUNCTION: fn_03622
void UIListBox_NaviSearch::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->verticalBar->Resize(12, height);
  this->verticalBar->Move((width - 13), 0);
  this->horizontalBar->Resize((width - 13), 12);
  this->horizontalBar->Move(0, (height - 13));
  this->Refresh();
}

// OFFSET: 0x005C3360
// FUNCTION: fn_03555
bool UINavigationV4Wnd::ClearRouteTexts() {
  for (std::string& text : this->unknown_0104) {
    text.clear();
  }

  this->unknown_0104.clear();
  return true;
}

// OFFSET: 0x005C33E0
// FUNCTION: fn_03557
void UINavigationV4Wnd::DrawMapTexture(int x, int y, int size) {
  if (g_navigationMapTexture) {
    this->FillRegion(x, y, size, size, 0);
    const float scale = static_cast<float>(size) / static_cast<float>(g_navigationMapTexture->Width());
    this->geometry.surface->DrawScaledSurface(
      x, y, size, size, g_navigationMapTexture, g_navigationMapTexture->Width(), g_navigationMapTexture->Height(), scale, scale, 0xFFFFFFFFu, false, 0, 0, 0,
      0, 0
    );
  }
}

// OFFSET: 0x005C4520
// FUNCTION: fn_03565
std::string CGameMode::MapName() {
  return this->mapName;
}

// OFFSET: 0x005C45D0
// FUNCTION: fn_03568
bool UINavigationV4Wnd::IsListShown() const {
  return this->unknown_00D4->geometry.unknown_0030 != 0;
}

// OFFSET: 0x005C4690
// FUNCTION: fn_03575
void UINavigationV4Wnd::LayoutWithoutDetails() {
  this->unknown_00B8->Move(-1000, -1000);
  this->unknown_00C4->Move(-1000, -1000);
  this->unknown_00C0->Move((this->geometry.width - 42), 3);
  this->unknown_00B4->Move((this->geometry.width - 14), 3);
  this->unknown_00BC->Move((this->geometry.width - 28), 3);
  this->unknown_00DC->Move(29, 21);
  this->unknown_00E0->Move(94, 21);
  this->unknown_00E4->Move(207, 21);
  this->unknown_00CC->Move(236, 21);
  this->unknown_00D0->Move(4, 21);
  this->unknown_00C8->Move(237, 317);
  this->unknown_00D4->Move(9, 321);
  this->unknown_00D8->Move(203, 317);
  this->unknown_00E8->Move(-1000, -1000);
  this->unknown_00EC->Move(-1000, -1000);
  this->unknown_00F0->Move(-1000, -1000);
  this->unknown_00F4->Move(-1000, -1000);
  this->unknown_00F8->Move(-1000, -1000);
  this->unknown_00FC->Move(-1000, -1000);
  this->unknown_0100->Move(-1000, -1000);
  this->Invalidate();
}

// OFFSET: 0x005C4AE0
// FUNCTION: fn_03577
void UINavigationV4Wnd::LayoutWithDetails() {
  this->unknown_00B8->Move(-1000, -1000);
  this->unknown_00C4->Move(-1000, -1000);
  this->unknown_00C0->Move((this->geometry.width - 42), 3);
  this->unknown_00B4->Move((this->geometry.width - 14), 3);
  this->unknown_00BC->Move((this->geometry.width - 28), 3);
  this->unknown_00DC->Move(29, 21);
  this->unknown_00E0->Move(94, 21);
  this->unknown_00E4->Move(207, 21);
  this->unknown_00CC->Move(236, 21);
  this->unknown_00D0->Move(4, 21);
  this->unknown_00C8->Move(237, 317);
  this->unknown_00D4->Move(9, 321);
  this->unknown_00D8->Move(203, 317);
  this->unknown_00E8->Move(20, 48);
  this->unknown_00EC->Move(1, 70);
  this->unknown_00F0->Move(139, 166);
  this->unknown_00F4->Move(146, 247);
  this->unknown_00F8->Move(145, 263);
  this->unknown_00FC->Move(207, 263);
  this->unknown_0100->Move(145, 288);
  this->Invalidate();
}

// OFFSET: 0x005CA060
// FUNCTION: fn_03639
void UINavigationV4Wnd::Slot11() {
  NavigationWindowInfo* info = this->info;
  if (!info) {
    return;
  }

  info->width = this->geometry.width;
  this->info->height = this->geometry.height;
  const int margin = this->info->expanded ? 272 : 228;
  const int maximumX = this->m_context->renderer.viewport.width - margin;
  if (maximumX < this->geometry.x) {
    this->geometry.x = maximumX;
  }

  if (this->geometry.x < 0) {
    this->geometry.x = 0;
  }

  this->info->x = this->geometry.x;
  const int maximumY = this->m_context->renderer.viewport.height - 338;
  if (maximumY < this->geometry.y) {
    this->geometry.y = maximumY;
  }

  if (this->geometry.y < 0) {
    this->geometry.y = 0;
  }

  this->info->y = this->geometry.y;
}

// OFFSET: 0x005C7140
// FUNCTION: fn_03597
void UINavigationroadiconWnd::Slot31(int x, int y) {
  int slot = 0;
  if (y >= 25 && y <= 55) {
    if (x >= 11 && x <= 41) {
      slot = 1;
    } else if (x >= 47 && x <= 77) {
      slot = 2;
    } else if (x >= 83 && x <= 113) {
      slot = 3;
    } else if (x >= 119 && x <= 149) {
      slot = 4;
    }
  }

  if (slot == 0 && y >= 63 && y <= 91) {
    if (x >= 11 && x <= 41) {
      slot = 5;
    } else if (x >= 47 && x <= 77) {
      slot = 6;
    } else if (x >= 83 && x <= 113) {
      slot = 7;
    } else if (x >= 119 && x <= 149) {
      slot = 8;
    }
  }

  if (slot != 0) {
    if (g_windowMgr.window_4F174) {
      g_windowMgr.window_4F174->SendMsg(nullptr, 40, slot, 0, 0, 0);
    }

    const int column = slot <= 4 ? slot : slot - 4;
    this->icon->Move(column * 36 - 26, slot < 5 ? 24 : 60);
  }

  this->Invalidate();
  UIFrameWnd::Slot31(x, y);
}

// OFFSET: 0x005C72A0
// FUNCTION: fn_03604
void UINavigationV4Wnd::Slot19() {
  if (g_navigationTab == 2) {
    this->Invalidate();
  }
}

// OFFSET: 0x005C72C0
// FUNCTION: fn_03605
void UIPrivateAirplaneWnd::Slot19() {
}

#pragma pack(push, 1)
struct EditTextsRecord {
  /* 0000 */ std::uint32_t marker;
  /* 0004 */ std::uint16_t bytes;
  /* 0006 */ int windowId;
  /* 000A */ int count;
  /* 000E */ unsigned char data[0xFF2];
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EditTextsRecord) == 0x1000);
#endif

// OFFSET: 0x00A834E0
// FUNCTION: fn_17004
int UIWindow::RecordEditTexts(WindowReplayRecorder& recorder) {
  if (Replay::Instance().state != 2 || !WindowPolicy(recorder.policy, recorder.restricted, 6, this->geometry.id)) {
    return 0;
  }

  EditTextsRecord record;
  std::memset(&record, 0, sizeof(record));

  int count = 0;
  for (UIWindow* child : this->m_children) {
    if (child->Slot46() == 2) {
      ++count;
    }
  }

  record.count = count;
  int offset = 14;
  if (count == 0) {
    return offset;
  }

  for (UIWindow* child : this->m_children) {
    if (child->Slot46() != 2) {
      continue;
    }

    UIEditCtrl* edit = static_cast<UIEditCtrl*>(child);
    const int length = edit->Length();
    std::memcpy(reinterpret_cast<unsigned char*>(&record) + offset, &length, sizeof(length));
    offset += 4;
    if (length) {
      std::memcpy(reinterpret_cast<unsigned char*>(&record) + offset, edit->Text(), length);
      offset += length;
    }
  }

  record.bytes = static_cast<std::uint16_t>(offset);
  record.marker = 0x038608AEu;
  record.windowId = this->geometry.id;
  recorder.packets.Record(record.bytes, &record, 41, 0);
  return offset;
}

// OFFSET: 0x005C4C80
// FUNCTION: fn_03579
void UIListBox_NaviSearch::Slot15(int width, int height) {
  this->verticalBar = new UIScrollBar_Navi(*this->m_context);
  this->verticalBar->Configure(width - 7, 0, 1, height);
  this->AddChild(this->verticalBar);

  this->horizontalBar = new UIScrollBar(*this->m_context);
  this->horizontalBar->Configure(0, height - 13, 0, width - 13);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x005C6090
// FUNCTION: fn_03586
void UIListBox_NaviSearch::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FFu);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface((this->backgroundRed & 255) | ((this->backgroundGreen & 255) << 8) | ((this->backgroundBlue & 255) << 16));
  }

  this->FillRegion(this->geometry.width - 1, 0, 1, this->geometry.height, 0xEBDEDE);

  const int first = this->topLine;
  const int limit = this->geometry.height / this->lineHeight - this->bottomMargin + first;
  const int end = static_cast<int>(std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(limit)));
  std::uint32_t color = 0;
  const int* icons = reinterpret_cast<const int*>(this->unknown_00D4);

  for (int index = first, row = 0; index < end; ++index, ++row) {
    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    int x = 3 - this->horizontalOffset;
    const int y = this->lineHeight * row;
    const int icon = icons[index];
    if (icon > 0) {
      const char* name = nullptr;
      if (icon == 1) {
        name = "유저인터페이스\\navigation_interface3\\icon_list_map.bmp";
      } else if (icon == 2) {
        name = "유저인터페이스\\navigation_interface3\\icon_list_npc.bmp";
      } else if (icon == 3) {
        name = "유저인터페이스\\navigation_interface3\\icon_list_mob.bmp";
      } else if (icon == 100) {
        name = "유저인터페이스\\navigation_interface3\\icon_list_Start.bmp";
      } else if (icon == 200) {
        name = "유저인터페이스\\navigation_interface3\\icon_list_Riute.bmp";
      } else if (icon == 300) {
        name = "유저인터페이스\\navigation_interface3\\icon_list_Target.bmp";
      }

      if (name) {
        CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
        this->DrawBitmap(x, y, bitmap ? &bitmap->image : nullptr, false);
      }

      x += 11;
    }

    const std::string& line = this->lines[index];
    DrawColoredText(*this, this->m_listContext->codePage, x, y, line.c_str(), static_cast<unsigned>(line.size()), color, 0, 12, false);
  }
}

// OFFSET: 0x005C5D40
// FUNCTION: fn_03582
void UINavigationroadiconWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, LookupMessage(3045), 0);

  this->icon = new UIStaticBitmap(*this->m_context, *this->m_controls);
  this->icon->SetBitmap("유저인터페이스\\navigation_interface3\\img_roadIocn_bg_select.bmp");
  this->icon->SetSize(this->icon->geometry.width, this->icon->geometry.height);

  const int slot = g_navigationManager.viewIcon;
  const int column = slot <= 4 ? slot : slot - 4;
  this->icon->Move(slot >= 1 && slot <= 8 ? column * 36 - 26 : 0, slot < 5 ? 24 : 60);
  this->icon->Show(1);
  this->AddChild(this->icon);
}

// OFFSET: 0x005C6C70
// FUNCTION: fn_03589
void UINavigationroadiconWnd::Paint() {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\bg_roadIocn_select.bmp")));
  const int titleHeight = this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(3203), 0);
  this->DrawBitmap(0, titleHeight, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x005C8D70
// FUNCTION: fn_03627
int UINavigationroadiconWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && a == 201) {
    this->m_frameContext->windows.CloseWindow(306);
  }

  return 0;
}

// OFFSET: 0x005C6290
// FUNCTION: fn_03587
void UINavigationRuideWnd::Paint() {
  this->FillRegion(0, 0, 272, 338, 0xCCCCCC);
  const int titleHeight = this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(3204), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(0, 0, 1);
}

// OFFSET: 0x005C73B0
// FUNCTION: fn_03616
std::string UINavigationV4Wnd::ObjectName(int type, void* object) {
  std::string name;
  if (type == 2) {
    name.assign(static_cast<CNavi_Object*>(object)->GetName());
  } else if (type == 3) {
    name.assign(static_cast<CNavi_Object*>(object)->GetName());
  } else if (type == 1) {
    name.assign(static_cast<CNaviInfo*>(object)->GetName());
  } else if (type == 0) {
    name.assign(static_cast<CNaviInfo*>(object)->GetName());
  }

  return name;
}

// OFFSET: 0x005C77B0
// FUNCTION: fn_03618
std::string UINavigationV4Wnd::MobName(CNavi_Object* mob) {
  char grade[50];
  std::memset(grade, 0, sizeof(grade));
  std::string name;
  name.assign(mob->GetName());

  const int level = mob->GetFlag();
  if (level > 0) {
    std::string text;
    text.assign(LookupMessage(2453));
    if (level > 99) {
      text.assign(LookupMessage(2449));
    } else if (level > 49) {
      text.assign(LookupMessage(2450));
    } else if (level >= 20 && level <= 49) {
      text.assign(LookupMessage(2451));
    } else if (level >= 5 && level <= 19) {
      text = LookupMessage(2452);
    }

    std::sprintf(grade, " (%s)", text.c_str());
  }

  name.append(grade);
  return name;
}

// OFFSET: 0x005C74B0
// FUNCTION: fn_03617
std::string UINavigationV4Wnd::ObjectLabel(int type, void* object) {
  char suffix[50];
  std::memset(suffix, 0, sizeof(suffix));
  std::string label;

  if (type == 2) {
    label.assign(static_cast<CNavi_Object*>(object)->GetInfo()->GetName());
    NaviCell first;
    NaviCell second;
    std::sprintf(suffix, " [%d,%d]", static_cast<CNavi_Object*>(object)->GetCell(&second)->x, static_cast<CNavi_Object*>(object)->GetCell(&first)->y);
    label.append(suffix);
  } else if (type == 3) {
    label.assign(static_cast<CNavi_Object*>(object)->GetInfo()->GetName());
    const int level = static_cast<CNavi_Object*>(object)->GetFlag();
    if (level > 0) {
      std::string text;
      text.assign(LookupMessage(2453));
      if (level > 99) {
        text.assign(LookupMessage(2449));
      } else if (level > 49) {
        text = LookupMessage(2450);
      } else if (level >= 20 && level <= 49) {
        text = LookupMessage(2451);
      } else if (level >= 5 && level <= 19) {
        text = LookupMessage(2452);
      }

      std::sprintf(suffix, " (%s)", text.c_str());
    }

    label.append(suffix);
  } else if (type == 1) {
    label.assign(static_cast<CNaviInfo*>(object)->GetTitle());
  } else if (type == 0) {
    label.assign(static_cast<CNaviInfo*>(object)->GetTitle());
  }

  return label;
}

// OFFSET: 0x00A7C580
// FUNCTION: fn_16947
void UIWindow::DrawActionSprite(
  const CSprRes* sprite, const CActRes* action, int x, int y, int width, int height, int interval, bool scaled, const CBitmapRes* background
) {
  if (!sprite || !action) {
    return;
  }

  const std::vector<ActionFrame>& frames = action->Animation(0);
  if (frames.empty()) {
    return;
  }

  int offsetX;
  int offsetY;
  float scale;
  if (!this->FitSprite(&frames[0], sprite, width, height, &offsetX, &offsetY, &scale)) {
    return;
  }

  if (!scaled) {
    scale = 1.0f;
  }

  if (background) {
    const BitmapImage& image = background->image;
    this->geometry.surface->Copy(offsetX - image.width / 2 + x, offsetY - image.height / 2 + y, image.width, image.height, image.pixels, true);
  }

  const int count = static_cast<int>(frames.size());
  int index = 0;
  if (interval > 0) {
    const float elapsed = static_cast<float>(timeGetTime() % static_cast<unsigned>(interval));
    const int frame = std::max(static_cast<int>(elapsed * (static_cast<float>(count) / static_cast<float>(interval))), 0);
    index = count > frame ? frame : count - 1;
  }

  if (frames.size() > static_cast<unsigned>(index)) {
    const ActionFrame& frame = frames[index];
    const SpriteRleImage* const* images[2] = {sprite->images[0].data(), sprite->images[1].data()};
    this->geometry.surface->DrawScaledSprite(
      x, y, width, height, std::span<const SpriteFrameCell>(frame.cells), images, frame.cellCount, offsetX, offsetY, scale, nullptr,
      this->m_context->renderer.atlas, this->m_context->renderSystem, g_serviceType, g_resourceReuseDelay
    );
  }
}
