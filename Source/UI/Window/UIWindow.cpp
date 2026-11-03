#include "UI/Chat/UINewChatWnd.h"
#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CMode.h"
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIRoMapWnd.h"
#include "File/FileEnumeration.h"
#include "Contents/World/World.h"
#include "Contents/Actors/Pc.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Input/Language.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Replay.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
#include "Contents/SpriteRidingOffset.h"
#include "Client/Session.h"
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
#include "Resource/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIListBoxNaviSearch.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarNavi.h"
#include "UI/Text/ColoredText.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/UITextViewer.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Navigation/UIPrivateAirplaneWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UINavigationWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowPolicy.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/PrivateAirplaneMgr.h"
#include "Contents/MapSkyMgr.h"
#include <algorithm>
#include <atlconv.h>
#include <bit>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <functional>
#include <span>
#include <string>
#include <windows.h>
#include <timeapi.h>

namespace {
  struct NavigationTargetLook {
    /* 0000 */ int job;
    /* 0004 */ int sex;
    /* 0008 */ int bodyPalette;
    /* 000C */ int head;
    /* 0010 */ int headPalette;
    /* 0014 */ int accessory0;
    /* 0018 */ int accessory1;
    /* 001C */ int accessory2;
    /* 0020 */ int robe;
  };
}

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

// GLOBAL: gl_25449
int g_navigationSelectedNode;

// GLOBAL: gl_24621
std::string g_navigationText0;

// GLOBAL: gl_24622
std::string g_navigationText1;

// GLOBAL: gl_24623
std::string g_navigationText2;

// GLOBAL: gl_24624
std::string g_navigationText3;

// GLOBAL: gl_24625
std::string g_navigationText4;

// OFFSET: 0x005C2260
// FUNCTION: fn_03528
int __fastcall sub_5C2260(const int* object) {
  return object[0];
}

// OFFSET: 0x005C22D0
// FUNCTION: fn_03529
int __fastcall sub_5C22D0(const int* object) {
  return object[8];
}

// OFFSET: 0x005C22E0
// FUNCTION: fn_03530
int __fastcall sub_5C22E0(const int* object) {
  return object[7];
}

// OFFSET: 0x005C22F0
// FUNCTION: fn_03531
int __fastcall sub_5C22F0(const int* object) {
  return object[9];
}

// GLOBAL: gl_24626
int g_navigationRoadIcon;

// GLOBAL: gl_24632
CTexture* g_navigationMapTexture;

// INLINED
static const char* FindColorCode(const char* text, const char* end) {
  if (!text) {
    return nullptr;
  }

  while (text < end) {
    if (*text == '^' && end - text >= 6) {
      int index = 1;
      while (index <= 6) {
        const unsigned char digit = static_cast<unsigned char>(text[index]);
        if (!((digit >= '0' && digit <= '9') || (digit >= 'a' && digit <= 'f') || (digit >= 'A' && digit <= 'F'))) {
          break;
        }

        ++index;
      }

      if (index > 6) {
        return text;
      }
    }

    text = CharNextExA(static_cast<WORD>(g_textCodePage), text, 0);
  }

  return text;
}

// OFFSET: 0x00A79A90
// FUNCTION: fn_16914
UIWindow::UIWindow(unsigned char cached)
    : parent(nullptr), unknown_0044(0), unknown_0048(0), unknown_004C(4), m_dirty(1), m_uid(++g_windowUid), m_cached(cached), m_clip(0) {
}

// OFFSET: 0x00A79E90
// OFFSET: 0x00A7A050
// FUNCTION: fn_16917
UIWindow::~UIWindow() {
  this->Slot18();
  delete this->geometry.surface;
  this->geometry.surface = nullptr;
  this->DeleteChildren();
  g_windowMgr.UnregisterWindow(this);
  g_windowMgr.ForgetWindowReferences(this);
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
  static_cast<CMode*>(g_charViewMode)->SendMsg(18, 0, 0, 0, 0);
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
  static_cast<CMode*>(g_charViewMode)->SetCursorAction(0);
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

    g_renderer->RenderToTexture(this->geometry.width, this->geometry.height, primitives, this->m_texture);
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
std::vector<unsigned char> UIWindow::Slot44() {
  std::vector<unsigned char> result;
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
    static_cast<CDCBitmap*>(this->geometry.surface)->UploadTexture(*GetRenderSystemHolder()->system, this->m_texture);
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
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

// OFFSET: 0x00A7AA90
// FUNCTION: fn_16925
void UIWindow::PlaceBeside(UIWindow* anchor) {
  if (!anchor) {
    return;
  }

  int anchorX = anchor->geometry.x;
  int anchorY = anchor->geometry.y;
  for (UIWindow* node = anchor->parent; node; node = node->parent) {
    anchorX += node->geometry.x;
    anchorY += node->geometry.y;
  }

  const int anchorWidth = anchor->geometry.width;
  const int width = this->geometry.width;
  const int screenWidth = g_renderer->viewport.width;
  int x;
  if (screenWidth < width + anchorWidth + anchorX) {
    if (anchorX < width) {
      x = 0;
      if (anchorX <= screenWidth - anchorWidth - anchorX) {
        x = screenWidth - width;
      }
    } else {
      x = anchorX - width;
    }
  } else {
    x = anchorX + anchorWidth;
  }

  int y = g_renderer->viewport.height - this->geometry.height;
  if (this->geometry.height + anchorY <= g_renderer->viewport.height) {
    y = anchorY;
  }

  int parentX = 0;
  int parentY = 0;
  for (UIWindow* node = this->parent; node; node = node->parent) {
    parentX += node->geometry.x;
    parentY += node->geometry.y;
  }
  this->Move(x - parentX, y - parentY);
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
void UIWindowMgr::DeferDeletion(UIWindow* window) {
  if (!window) {
    return;
  }

  this->ForgetWindowReferences(window);
  this->deferredDeletion.insert(window);
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
        g_windowMgr.DeferDeletion(child);
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

// OFFSET: 0x00A7B3F0
// FUNCTION: fn_16926
void UIWindow::ResetStates() {
  this->geometry.unknown_0030 = 0;
  this->Invalidate();

  for (auto it = this->m_children.begin(); it != this->m_children.end(); ++it) {
    (*it)->ResetStates();
  }
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
int UIWindow::TextWidth(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
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

// OFFSET: 0x00A7D760
// FUNCTION: fn_16955
int UIWindow::ColoredTextWidth(const char* text, unsigned bytes, int family, int height, unsigned char bold) {
  if (!text) {
    return 0;
  }

  if (!bytes) {
    bytes = static_cast<unsigned>(std::strlen(text));
    if (!bytes) {
      return 0;
    }
  }

  int width = this->TextWidth(text, bytes, family, height, bold, 0);
  std::uint32_t color = 0;
  if (ParseColorCode(FindColorCode(text, text + bytes), color) && color) {
    TextContext context(*this->geometry.surface);
    context.SetFont(0, 12, 0, 0);
    const int marker = context.Measure("^000000", 7).cx;
    width += (3 - marker) * 2;
  }

  return width;
}

// OFFSET: 0x00A84360
// FUNCTION: fn_17009
void UIWindow::DrawTextAt(int x, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold,
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

// OFFSET: 0x00A860C0
// FUNCTION: fn_17018
void UIWindow::DrawCenteredOutlinedText(
  int x, int y, const char* text, int width, int bytes, std::uint32_t color, std::uint32_t outline, int family, int height, unsigned char bold
) {
  if (bytes == 0) {
    bytes = static_cast<int>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  const SIZE size = context.Measure(text, static_cast<int>(std::strlen(text)));
  const int left = static_cast<int>((width - size.cx) * 0.5f + x);
  context.SetColor(outline);
  context.Draw(left - 1, y, text, bytes);
  context.Draw(left + 1, y, text, bytes);
  context.Draw(left, y - 1, text, bytes);
  context.Draw(left, y + 1, text, bytes);
  context.SetColor(color);
  context.Draw(left, y, text, bytes);
}

// OFFSET: 0x00A86320
// FUNCTION: fn_17020
void UIWindow::DrawVerticalOutlinedText(
  int x, int y, const char* text, int bytes, std::uint32_t color, std::uint32_t outline, int family, int height, unsigned char bold
) {
  if (bytes == 0) {
    bytes = static_cast<int>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  const SIZE size = context.Measure(text, bytes);
  context.SetColor(outline);
  context.Draw(x + size.cy - 1, y, text, bytes);
  context.Draw(x + size.cy + 1, y, text, bytes);
  context.Draw(x + size.cy, y - 1, text, bytes);
  context.Draw(x + size.cy, y + 1, text, bytes);
  context.SetColor(color);
  context.Draw(x + size.cy, y, text, bytes);
}

// OFFSET: 0x00A86440
// FUNCTION: fn_17021
void UIWindow::DrawRightAlignedText(int right, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold) {
  if (bytes == 0) {
    bytes = static_cast<int>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  const SIZE size = context.Measure(text, static_cast<int>(std::strlen(text)));
  context.SetColor(color);
  context.Draw(right - size.cx, y, text, bytes);
}

// OFFSET: 0x00A86510
// FUNCTION: fn_17022
void UIWindow::DrawShadowedText(
  int x, int y, const char* text, int bytes, std::uint32_t color, std::uint32_t shadow, int family, int height, unsigned char bold
) {
  if (bytes == 0) {
    bytes = static_cast<int>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  context.SetColor(shadow);
  context.Draw(x + 1, y + 1, text, bytes);
  context.SetColor(color);
  context.Draw(x, y, text, bytes);
}

// OFFSET: 0x00A865E0
// FUNCTION: fn_17023
void UIWindow::DrawFormattedText(int x, int y, const std::string& text, int width, int align, int family, int height, std::uint32_t color, unsigned char bold) {
  const int textWidth = this->FormattedTextWidth(text, family, height, bold);
  if (!width) {
    width = textWidth;
  }

  switch (align) {
  case 2:
    x -= textWidth;
    break;
  case 6:
    x = static_cast<int>((width - textWidth) * 0.5f + x);
    break;
  }

  int offset = 0;
  unsigned char currentBold = bold;
  const char* current = text.c_str();
  const char* const end = current + std::strlen(current);

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

      char code[7] = {};
      strncpy_s(code, sizeof(code), current + 1, 6);
      char* parsed = nullptr;
      const DWORD value = std::strtol(code, &parsed, 16);
      if (parsed - code == 6) {
        color = RGB(GetBValue(value), GetGValue(value), GetRValue(value));
        current += 7;
        continue;
      }
    }

    const char* const next = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    int advance = 0;
    {
      const std::string character(current, next);
      const char* const measured = character.c_str();
      if (measured) {
        const unsigned bytes = static_cast<unsigned>(std::strlen(measured));
        if (bytes) {
          TextContext context(*this->geometry.surface);
          context.SetFont(family, height, currentBold, 0);
          advance = context.Measure(measured, bytes).cx;
        }
      }
    }

    {
      const std::string character(current, next);
      const char* const drawn = character.c_str();
      const int bytes = static_cast<int>(std::strlen(drawn));
      TextContext context(*this->geometry.surface);
      context.SetFont(family, height, currentBold, 0);
      context.SetColor(color);
      context.Draw(x + offset, y, drawn, bytes);
    }

    offset += advance;
    if (offset > width) {
      break;
    }

    current = next;
  }
}

// OFFSET: 0x00A869C0
// FUNCTION: fn_17024
void UIWindow::DrawEmboldenedText(int x, int y, const char* text, std::uint32_t color, std::uint32_t secondColor, int bytes, int family, int height) {
  if (bytes == 0) {
    bytes = static_cast<int>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, 0, 0);
  context.SetColor(color);
  context.Draw(x, y, text, bytes);
  context.SetColor(secondColor);
  context.Draw(x - 1, y, text, bytes);
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
void UIWindow::DrawAmount(int amount, std::string text, int x, int y) {
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
int UIWindow::TextHeight(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
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
int UIWindow::VerticalTextHeight(const char* text, unsigned bytes, int family, int height, unsigned char bold) {
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
SIZE UIWindow::TextSize(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
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
SIZE UIWindow::VerticalTextSize(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
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
int UIWindow::FormattedTextWidth(std::string text, int family, int height, unsigned char bold) {
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
    const char* const measured = character.c_str();
    int advance = 0;
    if (measured) {
      const unsigned bytes = static_cast<unsigned>(std::strlen(measured));
      if (bytes) {
        TextContext context(*this->geometry.surface);
        context.SetFont(family, height, currentBold, 0);
        advance = context.Measure(measured, bytes).cx;
      }
    }

    total += advance;
    current = next;
  }

  return total;
}

// OFFSET: 0x00A80880
// FUNCTION: fn_16977
std::string UIWindow::FitDisplayText(int width, const char* text, int family, int height, unsigned char bold) {
  std::string display = DisplayTextTag(std::string(text), "<URL>", "</URL>");
  display = DisplayTextTag(std::move(display), "<NAVI>", "</NAVI>");
  display = DisplayTextTag(std::move(display), "<TIPBOX>", "</TIPBOX>");
  display = DisplayTextTag(std::move(display), "<ITEM>", "</ITEM>");
  display = RemoveTextLinkInfo(std::move(display));

  if (g_windowMgr.window_01C8) {
    display = static_cast<UINewChatWnd*>(g_windowMgr.window_01C8)->ConvertChat(std::move(display));
  }

  std::string navigation;
  DisplayNavigationLink(navigation, display);
  display = std::move(navigation);
  display = DisplayTextTag(std::move(display), "<QUEST>", "</QUEST>");

  const auto textWidth = [this, family, height, bold](const char* value) {
    if (!value) {
      return 0;
    }

    const unsigned bytes = static_cast<unsigned>(std::strlen(value));
    if (!bytes) {
      return 0;
    }

    TextContext context(*this->geometry.surface);
    context.SetFont(family, height, bold, 0);
    return static_cast<int>(context.Measure(value, bytes).cx);
  };

  const int extent = textWidth(display.c_str());
  {
    TextContext context(*this->geometry.surface);
    context.SetFont(family, height, bold, 0);
    context.Measure("...", 3);
  }

  if (extent <= width) {
    return display;
  }

  std::string fitted(display);
  ATL::CA2WEX<128> wide(display.c_str(), CP_THREAD_ACP);
  std::wstring remaining(wide);

  while (remaining.size() > 1) {
    std::size_t length = remaining.size() - 1;

    if ((remaining[remaining.size() - 2] & 0x80) != 0 && (remaining[remaining.size() - 1] & 0x80) != 0) {
      --length;
    }

    remaining.resize(length);

    ATL::CW2AEX<128> converted(remaining.c_str(), CP_THREAD_ACP);
    fitted = std::string(converted) + "...";

    if (textWidth(fitted.c_str()) <= width) {
      return fitted;
    }
  }

  ATL::CW2AEX<128> converted(remaining.c_str(), CP_THREAD_ACP);
  fitted = std::string(converted) + "...";
  return fitted;
}

// OFFSET: 0x00A81400
// FUNCTION: fn_16978
std::string UIWindow::FitVerticalDisplayText(int height, const char* text, int family, int fontHeight, unsigned char bold) {
  std::string display = DisplayTextTag(std::string(text), "<URL>", "</URL>");
  display = DisplayTextTag(std::move(display), "<NAVI>", "</NAVI>");
  display = DisplayTextTag(std::move(display), "<TIPBOX>", "</TIPBOX>");
  display = DisplayTextTag(std::move(display), "<ITEM>", "</ITEM>");
  display = RemoveTextLinkInfo(std::move(display));

  if (g_windowMgr.window_01C8) {
    display = static_cast<UINewChatWnd*>(g_windowMgr.window_01C8)->ConvertChat(std::move(display));
  }

  std::string navigation;
  DisplayNavigationLink(navigation, display);
  display = std::move(navigation);
  display = DisplayTextTag(std::move(display), "<QUEST>", "</QUEST>");

  if (this->VerticalTextHeight(display.c_str(), display.size(), family, fontHeight, bold) <= height) {
    return display;
  }

  const char* begin = display.c_str();
  std::size_t length = display.size();
  std::string fitted(display);
  int extent;

  do {
    fitted.resize(length);
    fitted += ".";

    if (CountCharacters(static_cast<WORD>(g_textCodePage), begin, begin + length) <= 1) {
      break;
    }

    extent = this->VerticalTextHeight(fitted.c_str(), fitted.size(), family, fontHeight, bold);
    length = CharPrevExA(static_cast<WORD>(g_textCodePage), begin, begin + length, 0) - begin;
  } while (height < extent);

  return fitted;
}

// OFFSET: 0x00A81E10
// FUNCTION: fn_16991
const char* UIWindow::TruncateText(const char* text, int maxBytes) {
  if (!text) {
    return nullptr;
  }

  if (static_cast<int>(std::strlen(text)) > maxBytes) {
    int bytes = 0;
    const char* current = text;
    const char* end = text + std::strlen(text);
    while (current < end) {
      const int previous = bytes;
      const char* next = g_serverLanguage->NextChar(current, static_cast<int>(end - current));
      bytes += static_cast<int>(next - current);
      if (bytes > maxBytes) {
        bytes = previous;
        break;
      }
      current = next;
    }

    if (bytes > 0) {
      char* buffer = static_cast<char*>(std::malloc(bytes + 1));
      std::memcpy(buffer, text, bytes);
      buffer[bytes] = '\0';
      // GLOBAL: gl_25070
      static std::string truncated;
      truncated = buffer;
      std::free(buffer);
      return truncated.c_str();
    }
  }
  return text;
}

// OFFSET: 0x00A81FD0
// FUNCTION: fn_16992
void UIWindow::WrapTextWithBreaks(const char* text, std::vector<std::string>* lines, std::vector<bool>* breaks, int width) {
  if (!text) {
    return;
  }

  const int totalWidth = this->TextWidth(text, 0, 0, 12, 0, 0);
  const int length = static_cast<int>(std::strlen(text));
  if (totalWidth <= 0) {
    return;
  }

  for (int offset = 0; offset < length;) {
    int bytes = 0;
    const char* start = text + offset;
    int lineWidth = 0;
    const char* end = start + std::strlen(start);
    if (start >= end) {
      continue;
    }

    bool newline = false;
    const char* current = start;
    do {
      const int previous = bytes;
      const char* next = g_serverLanguage->NextChar(current, static_cast<int>(end - current));
      const int characterBytes = static_cast<int>(next - current);
      bytes += characterBytes;
      lineWidth += this->TextWidth(current, characterBytes, 0, 12, 0, 0);
      if (characterBytes == 1 && *current == '\r') {
        newline = true;
        ++bytes;
        break;
      }
      if (lineWidth > width) {
        bytes = previous;
        break;
      }
      current = next;
    } while (current < end);

    if (bytes > 0) {
      char* buffer = static_cast<char*>(std::malloc(bytes + 1));
      std::memcpy(buffer, start, bytes);
      buffer[bytes] = '\0';
      lines->push_back(std::string(buffer));
      std::free(buffer);
      breaks->push_back(newline);
      offset += bytes;
    }
  }
}

// OFFSET: 0x00A7E120
// FUNCTION: fn_16959
std::list<int> UIWindow::WrapFormattedText(std::string text, std::list<std::string>* lines, int maxWidth, int family, int height, unsigned char bold) {
  lines->clear();
  if (maxWidth <= 0) {
    maxWidth = this->FormattedTextWidth(text.c_str(), family, height, bold);
  }

  std::list<int> widths;
  int width = 0;
  std::string line;
  unsigned char currentBold = bold;
  std::string colorCode;
  const char* current = text.c_str();
  const char* end = current + std::strlen(current);

  while (current != end) {
    if (*current == '\n') {
      lines->push_back(line);
      if (colorCode.empty()) {
        line.clear();
      } else {
        line = '^' + colorCode;
      }
      widths.push_back(width);
      width = 0;
      ++current;
      continue;
    }

    if (current[1] == '\\' && current[2] == 'n') {
      lines->push_back(line);
      line.clear();
      widths.push_back(width);
      width = 0;
      current += 2;
      continue;
    }

    if (*current == '^') {
      if (current[1] == 'b') {
        currentBold = 1;
        line.append(current, 2);
        current += 2;
        if (current == end) {
          lines->push_back(line);
        }
        continue;
      }

      if (current[1] == '/' && current[2] == 'b') {
        currentBold = bold;
        line.append(current, 3);
        current += 3;
        if (current == end) {
          lines->push_back(line);
        }
        continue;
      }

      char color[7] = {};
      strncpy_s(color, sizeof(color), current + 1, 6);
      char* parsed = nullptr;
      std::strtol(color, &parsed, 16);
      if (parsed - color == 6) {
        colorCode = color;
        line.append(current, 7);
        current += 7;
        if (current == end) {
          lines->push_back(line);
        }
        continue;
      }
    }

    const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    const std::string character(current, next);
    const char* const measured = character.c_str();
    int characterWidth = 0;
    if (measured) {
      const unsigned bytes = static_cast<unsigned>(std::strlen(measured));
      if (bytes) {
        TextContext context(*this->geometry.surface);
        context.SetFont(family, height, currentBold, 0);
        characterWidth = context.Measure(measured, bytes).cx;
      }
    }

    if (maxWidth < characterWidth + width) {
      lines->push_back(line);
      if (colorCode.empty()) {
        line.clear();
      } else {
        line = '^' + colorCode;
      }
      widths.push_back(width);
      width = 0;
    }

    width += characterWidth;
    line.append(current, next - current);
    current = next;
    if (current == end) {
      lines->push_back(line);
    }
  }

  return widths;
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

// OFFSET: 0x005C27E0
// FUNCTION: fn_03535
UIListBox_NaviSearch::UIListBox_NaviSearch() : UIListBox() {
}

// OFFSET: 0x005C3340
// FUNCTION: fn_03554
void UIListBox_NaviSearch::Clear() {
  UIListBox::Clear();
  this->icons.clear();
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

  this->unknown_0104.erase(this->unknown_0104.begin(), this->unknown_0104.end());
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
      0
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
  const int maximumX = g_renderer->viewport.width - margin;
  if (maximumX < this->geometry.x) {
    this->geometry.x = maximumX;
  }

  if (this->geometry.x < 0) {
    this->geometry.x = 0;
  }

  this->info->x = this->geometry.x;
  const int maximumY = g_renderer->viewport.height - 338;
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

// OFFSET: 0x005C6CC0
// FUNCTION: fn_03590
void UIPrivateAirplaneWnd::Paint() {
  this->DrawBitmap(
    0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\private_airplane\\bg_pay.bmp")))->image, false
  );
  this->DrawTextAt(19, 4, LookupMessage(0xd02), 0, 0, 12, 0xffffff, 0, 0);
  this->DrawTextAt(18, 3, LookupMessage(0xd02), 0, 0, 12, 0, 0, 0);

  if (this->m_text.size() == 0) {
    return;
  }

  this->DrawBitmap(
    7, 86, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\private_airplane\\ico_notic.bmp")))->image, true
  );

  std::list<std::string> lines;
  this->WrapText(this->m_text, &lines, 0xa4, 0, 12, 0);

  int offset = 0;
  for (auto it = lines.begin(); it != lines.end(); ++it, offset += 12) {
    const int y = static_cast<int>(static_cast<unsigned>(32 - lines.size() * 12) * 0.5 + 78.0 + offset);
    this->DrawTextAt(26, y, it->c_str(), 0, 0, 12, 0x3512b5, 0, 0);
  }
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
int UIWindow::RecordEditTexts() {
  if (Replay::Instance().state != 2 || !WindowPolicy(g_windowMgr.policy[0].actions, g_replayAnimationScaling != 0, 6, this->geometry.id)) {
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
  Replay::Instance().Record(record.bytes, &record, 41, 0);
  return offset;
}

// OFFSET: 0x005C4C80
// FUNCTION: fn_03579
void UIListBox_NaviSearch::Slot15(int width, int height) {
  this->verticalBar = new UIScrollBar_Navi();
  this->verticalBar->Configure(width - 7, 0, 1, height);
  this->AddChild(this->verticalBar);

  this->horizontalBar = new UIScrollBar();
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

  for (int index = first, row = 0; index < end; ++index, ++row) {
    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    int x = 3 - this->horizontalOffset;
    const int y = this->lineHeight * row;
    const int icon = this->icons[index];
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
    DrawColoredText(*this, x, y, line.c_str(), static_cast<unsigned>(line.size()), color, 0, 12, false);
  }
}

// OFFSET: 0x005C4EA0
// FUNCTION: fn_03581
void UINavigationV4Wnd::Slot15(int, int) {
  UIBitmapButton* closeButton = new UIBitmapButton();
  this->unknown_00B8 = closeButton;
  closeButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_close_normal.bmp", 0, 0);
  closeButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_close_over.bmp", 1, 0);
  closeButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_close_press.bmp", 2, 0);
  closeButton->SetSize(closeButton->geometry.width, closeButton->geometry.height);
  closeButton->SetId(201);
  closeButton->SetTooltip(LookupMessage(0x0BE5));
  this->AddChild(closeButton);

  UIBitmapButton* maximizeButton = new UIBitmapButton();
  this->unknown_00C4 = maximizeButton;
  maximizeButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_maximize_normal.bmp", 0, 0);
  maximizeButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_maximize_over.bmp", 1, 0);
  maximizeButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_maximize_press.bmp", 2, 0);
  maximizeButton->SetSize(maximizeButton->geometry.width, maximizeButton->geometry.height);
  maximizeButton->SetId(200);
  maximizeButton->SetTooltip(LookupMessage(0x089B));
  this->AddChild(maximizeButton);

  UICheckBox* minimizeCheck = new UICheckBox();
  this->unknown_00D4 = minimizeCheck;
  minimizeCheck->SetSize(110, 12);
  minimizeCheck->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", "Minimize");
  minimizeCheck->SetChecked(0);
  minimizeCheck->SetTooltip(LookupMessage(0x089C));
  this->AddChild(minimizeCheck);

  UIBitmapButton* roadIconButton = new UIBitmapButton();
  this->unknown_00C8 = roadIconButton;
  this->SetRoadIcon(g_navigationRoadIcon);
  roadIconButton->SetSize(roadIconButton->geometry.width, roadIconButton->geometry.height);
  roadIconButton->SetId(189);
  roadIconButton->SetTooltip(LookupMessage(0x0C8B));
  this->AddChild(roadIconButton);

  UIBitmapButton* roadListButton = new UIBitmapButton();
  this->unknown_00D8 = roadListButton;
  roadListButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_roadlistwnd_normal.bmp", 0, 0);
  roadListButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_roadlistwnd_over.bmp", 1, 0);
  roadListButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_roadlistwnd_press.bmp", 2, 0);
  roadListButton->SetSize(roadListButton->geometry.width, roadListButton->geometry.height);
  roadListButton->SetId(442);
  roadListButton->SetTooltip(LookupMessage(0x0C85));
  this->AddChild(roadListButton);

  UIListBox_NaviSearch* roadList = new UIListBox_NaviSearch();
  this->unknown_00EC = roadList;
  roadList->SetSize(138, 244);
  roadList->backgroundRed = 255;
  roadList->backgroundGreen = 255;
  roadList->backgroundBlue = 255;
  roadList->SetId(336);
  this->AddChild(roadList);

  UIListBox_NaviSearch* searchList = new UIListBox_NaviSearch();
  this->unknown_00F0 = searchList;
  searchList->SetSize(132, 80);
  searchList->backgroundRed = 240;
  searchList->backgroundGreen = 244;
  searchList->backgroundBlue = 253;
  searchList->SetId(441);
  this->AddChild(searchList);

  UICheckBox* filterCheck = new UICheckBox();
  this->unknown_00F4 = filterCheck;
  filterCheck->SetSize(110, 12);
  filterCheck->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", "service");
  filterCheck->SetId(213);
  filterCheck->SetChecked(g_navigationFilter);
  filterCheck->SetTooltip(LookupMessage(0x0C8C));
  this->AddChild(filterCheck);

  UIBitmapButton* shareButton = new UIBitmapButton();
  this->unknown_00F8 = shareButton;
  shareButton->SetBitmap("유저인터페이스\\private_airplane\\btn_share_normal.bmp", 0, 0);
  shareButton->SetBitmap("유저인터페이스\\private_airplane\\btn_share_over.bmp", 1, 0);
  shareButton->SetBitmap("유저인터페이스\\private_airplane\\btn_share_press.bmp", 2, 0);
  shareButton->SetSize(shareButton->geometry.width, shareButton->geometry.height);
  shareButton->SetId(354);
  shareButton->SetTooltip(LookupMessage(0x0C86));
  this->AddChild(shareButton);

  UIBitmapButton* findButton = new UIBitmapButton();
  this->unknown_00FC = findButton;
  findButton->SetBitmap("유저인터페이스\\private_airplane\\btn_find_normal.bmp", 0, 0);
  findButton->SetBitmap("유저인터페이스\\private_airplane\\btn_find_over.bmp", 1, 0);
  findButton->SetBitmap("유저인터페이스\\private_airplane\\btn_find_press.bmp", 2, 0);
  findButton->SetSize(findButton->geometry.width, findButton->geometry.height);
  findButton->SetId(430);
  findButton->SetTooltip(LookupMessage(0x0895));
  this->AddChild(findButton);

  UIBitmapButton* moveButton = new UIBitmapButton();
  this->unknown_0100 = moveButton;
  moveButton->SetBitmap("유저인터페이스\\private_airplane\\btn_move_normal.bmp", 0, 0);
  moveButton->SetBitmap("유저인터페이스\\private_airplane\\btn_move_over.bmp", 1, 0);
  moveButton->SetBitmap("유저인터페이스\\private_airplane\\btn_move_press.bmp", 2, 0);
  moveButton->SetSize(moveButton->geometry.width, moveButton->geometry.height);
  moveButton->SetId(499);
  this->AddChild(moveButton);

  UIStaticText* label = new UIStaticText();
  this->unknown_00E8 = label;
  label->SetSize(100, 20);
  this->AddChild(label);
  label->SetText(LookupMessage(0x0C87), 0);
  label->SetColor(0, 0, 0);

  this->unknown_00B4 = this->AddCloseButton(LookupMessage(0x0BE5), 0);
  this->unknown_00BC = this->AddMinimizeButton(LookupMessage(0x089C), 0);

  UIBitmapButton* helpButton = new UIBitmapButton();
  this->unknown_00C0 = helpButton;
  helpButton->SetBitmap("유저인터페이스\\navigation_interface\\NaviHelp_off.bmp", 0, 0);
  helpButton->SetBitmap("유저인터페이스\\navigation_interface\\NaviHelp_on.bmp", 1, 0);
  helpButton->SetBitmap("유저인터페이스\\navigation_interface\\NaviHelp_on.bmp", 2, 0);
  helpButton->SetSize(helpButton->geometry.width, helpButton->geometry.height);
  helpButton->SetId(343);
  helpButton->SetTooltip(LookupMessage(0x089D));
  this->AddChild(helpButton);

  UIBitmapButton* dataButton = new UIBitmapButton();
  this->unknown_00CC = dataButton;
  dataButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_Navidata_Open_normal.bmp", 0, 0);
  dataButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_Navidata_Open_over.bmp", 1, 0);
  dataButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_Navidata_Open_press.bmp", 2, 0);
  dataButton->SetSize(dataButton->geometry.width, dataButton->geometry.height);
  dataButton->SetId(191);
  dataButton->SetTooltip(LookupMessage(0x0899));
  this->AddChild(dataButton);

  UIBitmapEditCtrl* searchEdit = new UIBitmapEditCtrl();
  this->unknown_00E0 = searchEdit;
  searchEdit->SetSize(113, 18);
  searchEdit->maximum = 30;
  searchEdit->SetBitmapName("유저인터페이스\\navigation_interface3\\bg_search.bmp");
  searchEdit->SetId(516);
  this->AddChild(searchEdit);

  UIBitmapButton* searchButton = new UIBitmapButton();
  this->unknown_00E4 = searchButton;
  searchButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_searchbar_normal.bmp", 3, 0);
  searchButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_searchbar_normal.bmp", 0, 0);
  searchButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_searchbar_over.bmp", 1, 0);
  searchButton->SetBitmap("유저인터페이스\\navigation_interface3\\btn_searchbar_press.bmp", 2, 0);
  searchButton->SetSize(searchButton->geometry.width, searchButton->geometry.height);
  searchButton->SetId(321);
  searchButton->SetTooltip(LookupMessage(0x0894));
  this->AddChild(searchButton);

  UIBitmapButton* backButton = new UIBitmapButton();
  this->unknown_00D0 = backButton;
  backButton->SetBitmap("유저인터페이스\\navigation_interface3\\bt_back.bmp", 3, 0);
  backButton->SetBitmap("유저인터페이스\\navigation_interface3\\bt_back.bmp", 0, 0);
  backButton->SetBitmap("유저인터페이스\\navigation_interface3\\bt_back_on.bmp", 1, 0);
  backButton->SetBitmap("유저인터페이스\\navigation_interface3\\bt_back_on.bmp", 2, 0);
  backButton->SetSize(backButton->geometry.width, backButton->geometry.height);
  backButton->SetId(284);
  backButton->SetTooltip(LookupMessage(0x0C88));
  this->AddChild(backButton);

  UIComboBox_BmpWnd* searchType = new UIComboBox_BmpWnd();
  this->unknown_00DC = searchType;
  searchType->dropdownHeight = 67;
  searchType->Create(61, 18, "유저인터페이스\\navigation_interface3\\bg_combo.bmp", "유저인터페이스\\navigation_interface3\\dropdown_on");
  searchType->borderRed = 100;
  searchType->borderGreen = 100;
  searchType->borderBlue = 100;
  searchType->red = 232;
  searchType->green = 232;
  searchType->blue = 232;
  this->AddChild(searchType);
  searchType->AddOption(LookupMessage(0x089E), 0);
  searchType->AddOption(LookupMessage(0x089F), 0);
  searchType->AddOption(LookupMessage(0x08A0), 0);
  searchType->AddOption(LookupMessage(0x08A1), 0);
  searchType->Select(g_navigationSearchType);

  this->RefreshLayout();
}
// OFFSET: 0x005C5D40
// FUNCTION: fn_03582
void UINavigationroadiconWnd::Slot15(int, int) {
  this->AddCloseButton(LookupMessage(3045), 0);

  this->icon = new UIStaticBitmap();
  this->icon->SetBitmap("유저인터페이스\\navigation_interface3\\img_roadIocn_bg_select.bmp");
  this->icon->SetSize(this->icon->geometry.width, this->icon->geometry.height);

  const int slot = g_navigationManager.viewIcon;
  const int column = slot <= 4 ? slot : slot - 4;
  this->icon->Move(slot >= 1 && slot <= 8 ? column * 36 - 26 : 0, slot < 5 ? 24 : 60);
  this->icon->Show(1);
  this->AddChild(this->icon);
}

// OFFSET: 0x005C62F0
// FUNCTION: fn_03588
void UINavigationV4Wnd::Paint() {
  if (g_navigationMinimized) {
    DrawFilledRoundedBorder(*this, 0, 0, 0x86, 0x3d, 0xffffff);
    DrawFilledRoundedBorder(*this, 1, 0x11, 0x84, 0x2b, 0);
    this->DrawTextAt(3, 3, LookupMessage(0x8ad), 0, 0, 12, 0, 0, 0);
    if (g_navigationText1.size() != 0) {
      this->DrawBitmap(
        2, 0x12, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\ico_destination.bmp")))->image, true
      );
      this->DrawTextAt(5, 0x15, g_navigationText3.c_str(), 0, 0, 12, 0xffffff, 1, 0);
      this->DrawTextAt(5, 0x28, g_navigationText4.c_str(), 0, 0, 12, 0xffffff, 1, 0);
    }
    return;
  }

  this->DrawPanelBackground(this->DrawTitleBar(true, LookupMessage(0x8ad), 0), 0, "");
  this->DrawButtonBar(0, 0, 1);
  this->FillRegion(0, 0x2b, 0x110, 1, 0xcccccc);
  this->DrawBitmap(
    0, this->geometry.height - 0x18,
    &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\bg_down.bmp")))->image, false
  );

  if (g_navigationMode == 2 && g_navigationTab == 1) {
    this->FillRegion(1, 0x2c, 0x10e, 0x10e, 0xcccccc);
    this->DrawMapTexture(3, 0x2e, 0x10a);
    this->DrawTextAt(10, 0x30, g_navigationText0.c_str(), 0, 0, 12, 0xffffff, 1, 0);
    this->DrawTextAt(10, 0x44, g_navigationText1.c_str(), 0, 0, 12, 0xffffff, 1, 0);
    this->DrawTextAt(10, this->geometry.height - 0x2a, g_navigationText2.c_str(), 0, 0, 12, 0xffffff, 1, 0
    );

    if (g_navigationPoints.begin() != g_navigationPoints.end()) {
      for (const NaviCell& point : g_navigationPoints) {
        this->FillRegion(
          static_cast<int>(point.x * 2.078125f + 3.0f), static_cast<int>(point.y * 2.078125f + 46.0f), 2, 2, 0xf0f00a
        );
      }

      const NaviCell& last = g_navigationPoints.back();
      this->DrawBitmap(
        static_cast<int>(last.x * 2.078125f + 3.0f - 8.0f), static_cast<int>(last.y * 2.078125f + 46.0f - 21.0f),
        &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\ico_destination.bmp")))->image, true
      );
    }

    int column = 0;
    int row = 0;
    if (g_navigationManager.PlayerCell(&column, &row)) {
      NaviCell view;
      g_navigationManager.MapCellToView(&view, g_session.m_navigationPlayerMap, column, row, 266.0f);
      this->DrawBitmap(
        view.x - 6, view.y + 0x1d,
        &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\ico_location.bmp")))->image, true
      );
    }
    return;
  }

  if (g_navigationTab == 2) {
    this->FillRegion(1, 0x2c, 0x10e, 0x10e, 0xcccccc);
    this->DrawMapTexture(3, 0x2e, 0x10a);
    CNaviInfo* info = static_cast<CNaviInfo*>(g_navigationManager.GetCurrentTarget().object);
    if (info) {
      DrawOutlinedText(*this, 9, 0x32, info->GetTitle(), 0, 0xffffff, 0, 0, 0xb, 1, 0);
      DrawOutlinedText(*this, 9, 0x41, info->GetName(), 0, 0xffffff, 0, 0, 0xb, 1, 0);
    }

    if (timeGetTime() < static_cast<DWORD>(this->unknown_0118)) {
      CTexture* background = g_texMgr.Load("유저인터페이스\\navigation_interface3\\back.bmp", 0, false, true, false);
      if (background) {
        this->geometry.surface->DrawScaledSurface(
          0xc, 0x110, this->geometry.width - 0x18, 0x1e, background, background->Width(), background->Height(), 1.0f, 1.0f, 0x66ffffff, false, 0, 0,
          0, 0
        );
      }

      CTexture* mouse = g_texMgr.Load("유저인터페이스\\navigation_interface3\\icon_mouse.bmp", 0, false, true, false);
      if (mouse) {
        this->geometry.surface->DrawScaledSurface(
          0x12, 0x115, mouse->Width(), mouse->Height(), mouse, mouse->Width(), mouse->Height(), 1.0f, 1.0f, 0xffffffff, false, 0, 0, 0, 0
        );
      }

      std::string message = LookupMessage(0xc8f);
      std::vector<std::string> lines;
      WrapPlainText(message.c_str(), lines, 0x26);
      int y = (0x29 - static_cast<int>(lines.size())) * 7;
      for (unsigned int index = 0; index < lines.size(); ++index) {
        DrawOutlinedText(*this, 0x2a, y, lines[index].c_str(), 0, 0xffffff, 0, 0, 0xb, 1, 0);
        y += 0xe;
      }
    }

    const int x = g_screenWidth - this->geometry.x;
    if (x > 3 && x < 0x10d) {
      this->FillRegion(x, 0x2e, 2, 0x10a, 0xffaa2200);
    }

    const int y = g_screenHeight - this->geometry.y;
    if (y > 0x2e && y < 0x138) {
      this->FillRegion(3, y, 0x10a, 2, 0xffaa2200);
    }
    return;
  }

  this->FillRegion(0x8a, 0x2c, 1, 0x10e, 0xcccccc);
  this->FillRegion(0x8b, 0xf2, 0x84, 1, 0xcccccc);
  this->DrawBitmap(
    5, 0x2e, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\bg_dot.bmp")))->image, false
  );

  int mapX = 0x9b;
  int mapY = 0x3c;
  int mapSize = 0x64;
  const NavigationTarget target = g_navigationManager.GetCurrentTarget();
  if (target.type == 2) {
    mapX = 0xe3;
    mapSize = 0x28;
    this->DrawTargetSprite(
      0x93, 0x2d, 0x74, 0x74, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\Anibackground.bmp")))
    );
    mapY = 0x78;
  } else if (target.type == 3) {
    mapX = 0xe3;
    mapSize = 0x28;
    this->DrawActionSprite(
      g_navigationSprites, g_navigationActions, 0x93, 0x2d, 0x74, 0x74, 0x5dc, true,
      static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\Anibackground.bmp")))
    );
    mapY = 0x78;
  }
  this->DrawMapTexture(mapX, mapY, mapSize);

  const int active = g_navigationActiveLabel;
  const int count = static_cast<int>(g_navigationLabels.size());
  for (int index = 0; index < count; ++index) {
    if (g_navigationLabels[index].kind == active) {
      const int labelX = g_navigationLabels[index].x;
      const int labelY = g_navigationLabels[index].y;
      this->DrawTextAt(labelX + 1, labelY + 1, g_navigationLabels[index].text.c_str(), 0, 0, 12,
        g_navigationLabels[index].color, 0, 0
      );
      this->DrawTextAt(labelX, labelY, g_navigationLabels[index].text.c_str(), 0, 0, 12,
        g_navigationLabels[index].shadowColor, 0, 0
      );
    }
  }
}

// OFFSET: 0x005C6C70
// FUNCTION: fn_03589
void UINavigationroadiconWnd::Paint() {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\navigation_interface3\\bg_roadIocn_select.bmp")));
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(3203), 0);
  this->DrawBitmap(0, titleHeight, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x005C7CC0

// OFFSET: 0x005C7CC0
// FUNCTION: fn_03626
int UINavigationV4Wnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  const auto recordResponse = [this](int response, std::intptr_t first, std::intptr_t second, std::intptr_t third) {
    Replay& replay = Replay::Instance();
    if (replay.state == 2) {
      std::vector<unsigned char> packet(26, 0);
      const std::uint32_t marker = 0x038408AE;
      const std::uint16_t bytes = 26;
      const int windowId = this->geometry.id;
      const std::uint32_t values[3] = {
        static_cast<std::uint32_t>(first), static_cast<std::uint32_t>(second), static_cast<std::uint32_t>(third)
      };
      std::memcpy(packet.data(), &marker, sizeof(marker));
      std::memcpy(packet.data() + 4, &bytes, sizeof(bytes));
      std::memcpy(packet.data() + 6, &windowId, sizeof(windowId));
      std::memcpy(packet.data() + 10, &response, sizeof(response));
      std::memcpy(packet.data() + 14, values, sizeof(values));
      replay.RecordPacket(packet, 42, 0);
    }
  };

  switch (message) {
  case 0:
    if (sender == this->unknown_00E0 && static_cast<UIEditCtrl*>(this->unknown_00E0)->Length() > 0) {
      this->SendMsg(this->unknown_00E0, 6, 0x141, 0, 0, 0);
    }
    break;
  case 6:
    if (argument1 != 0x150 && argument1 != 0x1B9) {
      recordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
      if (argument1 > 499) {
        break;
      }
      if (argument1 == 499) {
        if (!g_windowMgr.LookupWindow(6)) {
          UIPrivateAirplaneWnd* window = new UIPrivateAirplaneWnd();
          window->SetSize(0xD0, 0x6E);
          window->Move(static_cast<int>((g_renderer->viewport.width - 0xD0) * 0.5), static_cast<int>((g_renderer->viewport.height - 0x6E) * 0.5));
          g_windowMgr.AddRendered(window);
          if (g_navigationManager.GetCurrentTargetType() == 2) {
            window->SendMsg(nullptr, 0x9A, reinterpret_cast<std::intptr_t>(LookupMessage(0xD3B)), 0, 0, 0);
          }

          std::string mapName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->MapName();
          mapName = mapName.substr(0, mapName.find(".rsw")) + ".gat";
          if (!g_privateAirplaneItems->IsStartableMap(mapName)) {
            window->SendMsg(nullptr, 0x9A, reinterpret_cast<std::intptr_t>(LookupMessage(0xD03)), 0, 0, 0);
            window->SendMsg(nullptr, 0x23, 0, 0, 0, 0);
          }

          std::string targetName = g_navigationManager.GetCurrentTargetName() + ".gat";
          if (!g_privateAirplaneItems->IsEndableMap(targetName)) {
            window->SendMsg(nullptr, 0x9A, reinterpret_cast<std::intptr_t>(LookupMessage(0xD04)), 0, 0, 0);
            window->SendMsg(nullptr, 0x23, 0, 0, 0, 0);
          }

          if (mapName.compare(targetName) == 0) {
            window->SendMsg(nullptr, 0x9A, reinterpret_cast<std::intptr_t>(LookupMessage(0xD19)), 0, 0, 0);
            window->SendMsg(nullptr, 0x23, 0, 0, 0, 0);
          }

          CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
          if (mode->scene && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player &&
              static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.unknown_0070 == 3) {
            window->SendMsg(nullptr, 0x9A, reinterpret_cast<std::intptr_t>(LookupMessage(0xD1A)), 0, 0, 0);
            window->SendMsg(nullptr, 0x23, 0, 0, 0, 0);
          }

          g_windowMgr.window_4F258 = window;
          if (g_windowMgr.RunModal(window) == 0xBC) {
            g_windowMgr.window_4F258 = nullptr;
          }
        }
        break;
      }
    }

    switch (argument1) {
    case 0xBD:
      if (!g_windowMgr.CloseWindow(0x132)) {
        g_windowMgr.EnsureWindow(0x132);
      }
      break;
    case 0xBF: {
      if (g_closeChatAfterGroups != 0) {
        g_windowMgr.CloseWindow(0x1A);
      }
      for (std::string& text : this->unknown_0104) {
        text.clear();
      }
      this->unknown_0104.erase(this->unknown_0104.begin(), this->unknown_0104.end());

      const std::string directory = FormatOwnedString("%s\\Navigationdata\\", g_applicationDirectory);
      EnumerateFileNames(this->unknown_0104, directory.c_str(), "*.txt");
      const int count = static_cast<int>(this->unknown_0104.size());
      if (count != 0) {
        UIWindow* history = g_windowMgr.EnsureWindow(0x1A);
        for (const std::string& name : this->unknown_0104) {
          history->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(name.c_str()), 0, 0, 0);
        }
        const int height = (std::min)(count * 0x10, 0xA0);
        history->SendMsg(nullptr, 0x28, 0, 0, 0, 0);
        history->Resize(0xA0, height);
        history->SendMsg(nullptr, 0x53, 0xCB, 0, 0, 0);
        int x = this->geometry.width - 0xA5;
        int y = 0x24;
        this->ToScreen(x, y);
        history->Move(x, y);
      }
      break;
    }
    case 0xC8:
      g_navigationMinimized = false;
      this->geometry.targetAlpha = 0xFF;
      this->geometry.alphaStart = timeGetTime();
      g_navigationHidden = false;
      this->RefreshLayout();
      break;
    case 0xC9:
      if (g_navigationMode == 2) {
        if (g_windowMgr.Confirm(LookupMessage(0x8BF), 2, true, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
          g_navigationManager.SetViewMode(1);
          g_navigationTab = 0;
          g_navigationManager.ClearSavedTarget();
          g_windowMgr.CloseWindow(0x13A);
          this->RefreshLayout();
          g_worldViewMgr.ClearMarks();
        }
      } else {
        if (g_windowMgr.window_0428) {
          this->SendMsg(nullptr, 6, 0xCA, 0, 0, 0);
        }
        g_navigationTab = 0;
        g_navigationMinimized = false;
        g_navigationManager.Reset(true);
        g_worldViewMgr.ClearMarks();
        g_worldViewMgr.ClearLevels();
        g_worldViewMgr.ResetSelection();
        g_windowMgr.CloseWindow(0x13A);
        g_windowMgr.CloseWindow(0xCB);
      }
      break;
    case 0xCA:
      g_navigationMinimized = true;
      this->geometry.targetAlpha = 0xAA;
      this->geometry.alphaStart = timeGetTime();
      if (g_windowMgr.window_0428) {
        static_cast<UIRoMapWnd*>(g_windowMgr.window_0428)->UpdateNavigationButtons();
      }
      this->RefreshLayout();
      break;
    case 0xD5:
      g_unknown16FC585 = false;
      g_navigationFilter = argument2 != 0;
      g_unknown16FC586 = g_navigationFilter;
      break;
    case 0x11C:
      if (!g_navigationManager.GetSavedTarget().object) {
        if (g_navigationTab == 2) {
          if (g_navigationMode == 2) {
            this->LayoutWithoutDetails();
            g_navigationTab = 1;
            g_navigationManager.SelectTarget(g_navigationManager.GetSelectedNode());
          } else {
            this->LayoutWithDetails();
            g_navigationTab = 0;
            g_navigationManager.SelectNodeState();
          }
        }
        break;
      }
      if (g_navigationTab == 1 || (g_navigationTab == 2 && g_navigationMode != 2)) {
        g_navigationTab = 0;
        g_navigationManager.SelectNodeState();
        this->RefreshLayout();
        break;
      }
      g_navigationTab = 1;
      g_navigationManager.SelectTarget(g_navigationManager.GetSelectedNode());
      this->RefreshLayout();
      break;
    case 0x141:
      if (static_cast<UIEditCtrl*>(this->unknown_00E0)->Length() != 0 && !g_navigationMinimized) {
        g_navigationSearchText = static_cast<UIEditCtrl*>(this->unknown_00E0)->Text();
        g_navigationManager.UpdateSearchResults();
        this->unknown_00F0->Clear();
        this->unknown_00F0->Invalidate();
        g_navigationManager.ClearData();
        g_navigationManager.LoadTargetPreview();
        this->FillRouteGroups();
        g_navigationTab = 0;
        if (g_windowMgr.window_0428) {
          static_cast<UINavigationWnd*>(g_windowMgr.window_0428)->RefreshWorldMapLevels();
        }
        this->LayoutWithDetails();
        g_serverLanguage->ResetInput();
      }
      break;
    case 0x150:
      if (g_replayAnimationScaling == 0) {
        argument2 = this->unknown_00EC->selected;
      }
      this->SelectRoute(static_cast<int>(argument2));
      if (argument2 != -1) {
        g_navigationSelectedNode = 0;
        if (!g_navigationManager.SelectNodeState()) {
          g_navigationSelectedNode = 0;
          g_navigationManager.SelectNodeState();
        }
        this->unknown_00F0->selected = g_navigationSelectedNode;
        this->unknown_00F0->Invalidate();
        if (g_windowMgr.window_0428) {
          static_cast<UINavigationWnd*>(g_windowMgr.window_0428)->RefreshWorldMapSelection();
        }
        this->Invalidate();
        recordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
      }
      this->Invalidate();
      break;
    case 0x157:
      if (!g_windowMgr.CloseWindow(0xE5)) {
        g_windowMgr.EnsureWindow(0xE5);
      }
      break;
    case 0x162:
      if (g_navigationManager.GetCurrentTargetName().size() != 0) {
        const int type = g_navigationManager.GetCurrentTarget().type;
        if (type < 2) {
          this->LayoutWithoutDetails();
          g_navigationTab = 2;
          this->unknown_0118 = timeGetTime() + 5000;
        } else {
          const NavigationTarget target = g_navigationManager.GetCurrentTarget();
          CNavi_Object* object = static_cast<CNavi_Object*>(target.object);
          int x = 1000;
          int y = 1000;
          if (type == 2) {
            NaviCell cell;
            x = object->GetCell(&cell)->x;
            NaviCell other;
            y = object->GetCell(&other)->y;
          }
          this->InsertNavigationLink(object->GetInfo()->GetName(), x, y);
        }
      }
      break;
    case 0x1AE: {
      this->Invalidate();
      int result = g_navigationManager.SaveCurrentTarget();
      if (result == 0) {
        const NavigationTarget target = g_navigationManager.GetSavedTarget();
        result = g_navigationManager.Navigate(target.x, target.y);
        if (result >= 0) {
          g_navigationTab = 1;
          g_navigationManager.SelectTarget(g_navigationManager.GetSelectedNode());
          if (g_windowMgr.window_0428) {
            static_cast<UINavigationWnd*>(g_windowMgr.window_0428)->RefreshWorldMapMarks();
          }
        }
      }
      g_navigationManager.ShowRouteResult(result, 1);
      this->RefreshLayout();
      if (UIWindow* window = g_windowMgr.LookupWindow(0x13A)) {
        window->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
      }
      break;
    }
    case 0x1B9:
      if (g_replayAnimationScaling == 0) {
        argument2 = this->unknown_00F0->selected;
      }
      if (argument2 != -1) {
        this->SelectRouteNode(static_cast<int>(argument2));
        recordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
      }
      this->Invalidate();
      break;
    case 0x1BA:
      if (!g_windowMgr.CloseWindow(0x13A) && g_navigationManager.NodeCount() != 0) {
        g_windowMgr.EnsureWindow(0x13A);
      }
      break;
    }
    break;
  case 0x22:
    this->info = reinterpret_cast<NavigationWindowInfo*>(argument1);
    this->info->Clamp();
    this->Move(this->info->x, this->info->y);
    this->Invalidate();
    break;
  case 0x27:
    if (sender == this->unknown_00DC) {
      g_navigationSearchType = static_cast<int>(argument1);
      g_navigationManager.BuildNodePaths();
      this->FillRouteGroups();
      this->Invalidate();
      if (g_replayAnimationScaling != 0) {
        this->unknown_00DC->SendMsg(nullptr, 0x39, argument1, 0, 0, 0);
      }
    } else if (g_replayAnimationScaling == 0) {
      this->LoadRoute(static_cast<int>(argument1));
    }
    break;
  case 0x28:
    this->SetRoadIcon(static_cast<int>(argument1));
    break;
  }
  return 0;
}

// OFFSET: 0x005C8D70
// FUNCTION: fn_03627
int UINavigationroadiconWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && a == 201) {
    g_windowMgr.CloseWindow(306);
  }

  return 0;
}

// OFFSET: 0x005C4D70
// FUNCTION: fn_03580
void UINavigationRuideWnd::Slot15(int width, int) {
  this->AddCloseButton(LookupMessage(3045), 0);
  this->routeList = new UIListBox_NaviSearch();
  this->routeList->SetSize(width - 4, 293);
  this->routeList->backgroundRed = 255;
  this->routeList->backgroundGreen = 255;
  this->routeList->backgroundBlue = 255;
  this->routeList->SetId(336);
  this->routeList->Move(2, 20);
  this->AddChild(this->routeList);
  this->RefreshRoute();
}

// OFFSET: 0x005C6290
// FUNCTION: fn_03587
void UINavigationRuideWnd::Paint() {
  this->FillRegion(0, 0, 272, 338, 0xCCCCCC);
  const int titleHeight = this->DrawTitleBar(true, LookupMessage(3204), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(0, 0, 1);
}

// OFFSET: 0x005C7A00
// FUNCTION: fn_03619
void UIListBox_NaviSearch::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int count = static_cast<int>(this->lines.size());
  const int overflow = std::max(count - visible, 0);
  int maximum;
  if (overflow != 0) {
    this->verticalOverflow = 1;
    maximum = this->bottomMargin + overflow;
  } else {
    this->verticalOverflow = 0;
    maximum = 0;
  }

  this->verticalBar->SetRange(maximum, visible, static_cast<int>(this->lines.size()));
  this->verticalBar->Invalidate();
}

// OFFSET: 0x005C7B50
// FUNCTION: fn_03624
int UIListBox_NaviSearch::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message != 7 && message != 9 && message != 10) {
    return this->UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  const int previous = this->topLine;
  int lineHeight;
  int top;
  if (message == 9) {
    lineHeight = this->lineHeight;
    top = previous - this->geometry.height / lineHeight + 1;
  } else if (message == 10) {
    lineHeight = this->lineHeight;
    top = previous - 1 + this->geometry.height / lineHeight;
  } else {
    top = static_cast<int>(argument1) + previous;
    lineHeight = this->lineHeight;
  }

  this->topLine = top;
  if (top < 0) {
    this->topLine = 0;
    top = 0;
  }

  const int maximum = this->bottomMargin + static_cast<int>(this->lines.size()) - this->geometry.height / lineHeight;
  if (static_cast<unsigned int>(top) > static_cast<unsigned int>(maximum)) {
    this->topLine = maximum;
    top = maximum;
  }

  this->verticalBar->SetPosition(top);
  this->verticalBar->Invalidate();
  this->Invalidate();
  if (this->transparent != 0 && this->parent) {
    this->parent->Invalidate();
  }

  return this->topLine - previous;
}

// OFFSET: 0x005C7C70
// FUNCTION: fn_03625
int UINavigationRuideWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6) {
    if (a == 201) {
      g_windowMgr.CloseWindow(314);
    }
  } else if (message != 60) {
    return 0;
  }

  this->RefreshRoute();
  this->Invalidate();
  return 0;
}

// OFFSET: 0x005C8F70
// FUNCTION: fn_03630
void UINavigationRuideWnd::RefreshRoute() {
  char text[50];
  std::memset(text, 0, sizeof(text));
  this->routeList->Clear();

  for (int index = 0; index < g_navigationManager.NodeCount(); ++index) {
    CNavi_Link* node = g_navigationManager.GetNode(index);
    NaviCell first;
    const int y = node->GetCell(&first)->y;
    NaviCell second;
    const int x = node->GetCell(&second)->x;
    std::sprintf(text, "%s (%d %d)", node->GetInfo()->GetName(), x, y);

    UIListBox_NaviSearch* list = this->routeList;
    list->icons.push_back(index == 0 ? 100 : 200);
    list->AddLine(text);
    list->Invalidate();
  }

  NaviCell cell;
  if (g_navigationManager.GetDestinationCell(&cell)->x < 1) {
    std::sprintf(text, "%s", g_navigationManager.GetDestinationInfo()->GetName());
  } else {
    NaviCell first;
    const int y = g_navigationManager.GetDestinationCell(&first)->y;
    NaviCell second;
    const int x = g_navigationManager.GetDestinationCell(&second)->x;
    std::sprintf(text, "%s (%d %d)", g_navigationManager.GetDestinationInfo()->GetName(), x, y);
  }

  UIListBox_NaviSearch* list = this->routeList;
  list->icons.push_back(300);
  list->AddLine(text);
  list->Invalidate();
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

// OFFSET: 0x005C9640
// FUNCTION: fn_03633
void UINavigationV4Wnd::SelectRoute(int route) {
  this->unknown_00F0->Invalidate();
  this->unknown_00F0->Clear();
  if (g_navigationManager.nodePaths.empty()) {
    return;
  }

  g_navigationManager.unknown_12C4 = route;
  if (route == -1) {
    return;
  }

  const int count = static_cast<int>(g_navigationManager.nodePaths[route].size());
  std::string text;
  for (int index = 0; index < count; ++index) {
    NavigationTarget target = g_navigationManager.GetState(g_navigationManager.nodePaths[route][index]);
    const bool unknownCell = target.x == -1000 && target.y == -1000;
    if (unknownCell) {
      text = this->MobName(static_cast<CNavi_Object*>(target.object));
    } else {
      text = this->ObjectLabel(target.type, target.object);
      if (target.type == 0) {
        char coords[30] = {};
        std::sprintf(coords, " [%d,%d]", target.x, target.y);
        text += coords;
      }
    }

    const int icon = target.type;
    const char* line = text.c_str();
    UIListBox_NaviSearch* list = this->unknown_00F0;
    list->icons.push_back(icon);
    list->AddLine(line);
    list->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x005C9180
// FUNCTION: fn_03631
bool UINavigationV4Wnd::LoadRoute(int index) {
  if (this->unknown_0104.empty() || static_cast<unsigned int>(index) > this->unknown_0104.size() || index < 0) {
    return false;
  }

  std::string name = this->unknown_0104[index].c_str();
  if (name.size() == 0) {
    return false;
  }

  std::string path = FormatOwnedString("%s\\Navigationdata\\%s", g_applicationDirectory, name.c_str());
  FILE* file = fopen(path.c_str(), "rb");
  if (!file) {
    return false;
  }

  g_navigationManager.ClearStates();
  char delimiters[] = "\t ,\t\n";
  char line[1024];
  std::memset(line, 0, sizeof(line));
  std::vector<std::string> tokens;
  while (!feof(file)) {
    fgets(line, 1024, file);
    for (char* token = strtok(line, delimiters); token; token = strtok(nullptr, delimiters)) {
      tokens.emplace_back(std::string(token));
    }
    g_navigationManager.AddState(tokens);
    tokens.erase(tokens.begin(), tokens.end());
  }
  fclose(file);
  g_navigationManager.BuildSingleNodePaths();

  if (g_navigationManager.StateCount() > 0) {
    this->RecordData(0x38F, g_navigationManager.WriteStates());
  }

  this->FillRouteGroups();
  this->SendMsg(this, 6, 190, 0, 0, 0);
  return true;
}

// OFFSET: 0x005C9A20
// FUNCTION: fn_03634
void UINavigationV4Wnd::FillRouteGroups() {
  this->unknown_00EC->Clear();
  this->unknown_00EC->Invalidate();
  for (unsigned int index = 0; index < g_navigationManager.nodePaths.size(); ++index) {
    NavigationTarget target = g_navigationManager.GetState(g_navigationManager.nodePaths[index][0]);
    const int count = static_cast<int>(g_navigationManager.nodePaths[index].size());
    std::string name = this->ObjectName(target.type, target.object);
    if (target.type == 0 && !target.name.empty()) {
      name = target.name;
    }

    char text[50];
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, "[%d]%s", count, name.c_str());
    const int icon = target.type;
    UIListBox_NaviSearch* list = this->unknown_00EC;
    list->icons.push_back(icon);
    list->AddLine(text);
    list->Invalidate();
  }

  char summary[50];
  std::memset(summary, 0, sizeof(summary));
  std::sprintf(summary, LookupMessage(3210), static_cast<int>(g_navigationManager.nodePaths.size()));
  this->unknown_00E8->SetText(summary, 0);
  this->Invalidate();
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
      g_renderer->atlas, *GetRenderSystemHolder()->system, g_serviceType, g_resourceReuseDelay
    );
  }
}

// OFFSET: 0x005C6F90
// FUNCTION: fn_03596
void UINavigationV4Wnd::Slot31(int x, int y) {
  UIFrameWnd::Slot31(x, y);

  if (!g_navigationMinimized) {
    const int mapY = y - 46;
    const int mapX = x - 3;
    if (mapY >= 0 && mapY <= 266 && mapX >= 0 && mapX <= 266 && g_navigationTab == 2) {
      NavigationTarget target = g_navigationManager.GetCurrentTarget();
      CNaviInfo* info = static_cast<CNaviInfo*>(target.object);
      const int width = info->GetCellX();
      const int height = info->GetCellY();
      const int size = width > height ? width : height;
      const float scale = static_cast<float>(size) / 265.0f;
      const int difference = width - height;

      double offsetX = 0.0;
      double offsetY = 0.0;
      if (difference > 0) {
        offsetY = difference * 0.5;
      } else {
        offsetX = difference * 0.5;
      }

      const int cellX = static_cast<int>(static_cast<float>(mapX) * scale - static_cast<float>(static_cast<int>(std::fabs(offsetX))));
      const int cellY = static_cast<int>(static_cast<float>(size) - static_cast<float>(mapY) * scale - static_cast<float>(static_cast<int>(std::fabs(offsetY))));

      if (cellX >= 0 && cellY >= 0 && cellX <= width && cellY <= height) {
        this->InsertNavigationLink(info->GetName(), cellX, cellY);
      }
    }
  } else {
    this->geometry.targetAlpha = 170;
    this->geometry.alphaStart = timeGetTime();
  }
}

// GLOBAL: gl_25052
static const std::string g_windowNaviLinkTag = "<NAVIL>";
// GLOBAL: gl_25053
static const std::string g_windowNaviLinkCloseTag = "</NAVIL>";

// OFFSET: 0x005C9E00
// FUNCTION: fn_03636
void UINavigationV4Wnd::InsertNavigationLink(std::string name, int x, int y) {
  const std::string xText = EncodeNavigationNumber(x < 0 ? 1000 : x);
  const std::string yText = EncodeNavigationNumber(y < 0 ? 1000 : y);
  std::string link = g_windowNaviLinkTag;
  link += name;
  link += xText;
  link += yText;
  link += g_windowNaviLinkCloseTag;

  if (g_windowMgr.GetHoverWindow() && g_windowMgr.GetHoverWindow()->geometry.id != 516) {
    if (g_windowMgr.GetHoverWindow() == ChatWindow()->m_suggestions && !g_windowMgr.unknown_0024) {
      ChatWindow()->ToggleInput();
    }
  } else if (!g_windowMgr.unknown_0024) {
    ChatWindow()->ToggleInput();
  } else {
    g_windowMgr.SetFocus(ChatWindow()->m_suggestions);
  }

  g_windowMgr.GetHoverWindow()->SendMsg(nullptr, 153, reinterpret_cast<std::intptr_t>(link.c_str()), 0, 0, 0);
}

// OFFSET: 0x005C4830
// FUNCTION: fn_03576
void UINavigationV4Wnd::RefreshLayout() {
  if (g_navigationManager.unknown_1254) {
    this->Resize(134, 61);
    this->unknown_00B8->Move((this->geometry.width - 15), 2);
    this->unknown_00C4->Move((this->geometry.width - 32), 2);
    this->unknown_00C0->Move(-1000, -1000);
    this->unknown_00B4->Move(-1000, -1000);
    this->unknown_00BC->Move(-1000, -1000);
    this->unknown_00DC->Move(-1000, -1000);
    this->unknown_00E0->Move(-1000, -1000);
    this->unknown_00E4->Move(-1000, -1000);
    this->unknown_00CC->Move(-1000, -1000);
    this->unknown_00D0->Move(-1000, -1000);
    this->unknown_00C8->Move(-1000, -1000);
    this->unknown_00D4->Move(-1000, -1000);
    this->unknown_00E8->Move(-1000, -1000);
    this->unknown_00EC->Move(-1000, -1000);
    this->unknown_00F0->Move(-1000, -1000);
    this->unknown_00F4->Move(-1000, -1000);
    this->unknown_00F8->Move(-1000, -1000);
    this->unknown_00FC->Move(-1000, -1000);
    this->unknown_00D8->Move(-1000, -1000);
    this->unknown_0100->Move(-1000, -1000);
  } else {
    this->Resize(272, 338);
    if (g_navigationManager.unknown_125C == 0) {
      this->LayoutWithDetails();
    } else {
      this->LayoutWithoutDetails();
    }
  }

  this->Show(!g_navigationManager.unknown_1248);
  this->FillRouteGroups();
  this->SelectRoute(g_navigationManager.unknown_12C4);
  g_navigationManager.LoadTargetPreview();
  g_navigationManager.DestinationTitle();
  g_navigationManager.DestinationText();
  g_navigationManager.RouteText();
  g_navigationManager.SelectedNodeText();
  g_navigationManager.SubDestinationText();
  this->Invalidate();
}

// OFFSET: 0x005C9FE0
// FUNCTION: fn_03637
void UINavigationV4Wnd::SelectRouteNode(int node) {
  if (node == -1) {
    return;
  }

  g_navigationManager.unknown_12C8 = node;
  if (!g_navigationManager.SelectNodeState()) {
    g_navigationManager.unknown_12C8 = 0;
    g_navigationManager.SelectNodeState();
  }

  this->unknown_00F0->selected = g_navigationManager.unknown_12C8;
  this->unknown_00F0->Invalidate();
  if (g_windowMgr.modal) {
    reinterpret_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapSelection();
  }
  this->Invalidate();
}

// OFFSET: 0x00A85850
// FUNCTION: fn_17017
void UIWindow::sub_A85850(
  int x, int y, std::string text, int width, int align, std::uint32_t color, int outlineColor, int family, int height, int bold, unsigned char outlined
) {
  int textWidth = 0;
  if (const char* const string = text.c_str()) {
    unsigned bytes = static_cast<unsigned>(text.size());
    if (!bytes) {
      bytes = static_cast<unsigned>(std::strlen(string));
    }

    if (bytes) {
      TextContext context(*this->geometry.surface);
      context.SetFont(family, height, static_cast<unsigned char>(bold), 0);
      textWidth = context.Measure(string, bytes).cx;
    }
  }

  if (align == 2) {
    x -= textWidth;
  } else if (align == 6) {
    x = static_cast<int>((width - textWidth) * g_depthAverageHalf + x);
  }

  int offset = 0;
  const char* current = text.c_str();
  const char* const end = current + std::strlen(current);
  while (current != end) {
    const char* const next = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    int glyphWidth = 0;
    {
      const std::string glyph(current, next);
      if (const char* const string = glyph.c_str()) {
        if (const unsigned bytes = static_cast<unsigned>(std::strlen(string))) {
          TextContext context(*this->geometry.surface);
          context.SetFont(family, height, static_cast<unsigned char>(bold), 0);
          glyphWidth = context.Measure(string, bytes).cx;
        }
      }
    }

    if (outlined == 1) {
      {
        const std::string glyph(current, next);
        const char* const string = glyph.c_str();
        const int bytes = static_cast<int>(std::strlen(string));
        TextContext context(*this->geometry.surface);
        context.SetFont(family, height, static_cast<unsigned char>(bold), 0);
        context.SetColor(outlineColor);
        context.Draw(x + offset + 1, y, string, bytes);
      }
      {
        const std::string glyph(current, next);
        const char* const string = glyph.c_str();
        const int bytes = static_cast<int>(std::strlen(string));
        TextContext context(*this->geometry.surface);
        context.SetFont(family, height, static_cast<unsigned char>(bold), 0);
        context.SetColor(outlineColor);
        context.Draw(x + offset - 1, y, string, bytes);
      }
      {
        const std::string glyph(current, next);
        const char* const string = glyph.c_str();
        const int bytes = static_cast<int>(std::strlen(string));
        TextContext context(*this->geometry.surface);
        context.SetFont(family, height, static_cast<unsigned char>(bold), 0);
        context.SetColor(outlineColor);
        context.Draw(x + offset, y + 1, string, bytes);
      }
      {
        const std::string glyph(current, next);
        const char* const string = glyph.c_str();
        const int bytes = static_cast<int>(std::strlen(string));
        TextContext context(*this->geometry.surface);
        context.SetFont(family, height, static_cast<unsigned char>(bold), 0);
        context.SetColor(outlineColor);
        context.Draw(x + offset, y - 1, string, bytes);
      }
    }

    {
      const std::string glyph(current, next);
      const char* const string = glyph.c_str();
      const int bytes = static_cast<int>(std::strlen(string));
      TextContext context(*this->geometry.surface);
      context.SetFont(family, height, static_cast<unsigned char>(bold), 0);
      context.SetColor(color);
      context.Draw(x + offset, y, string, bytes);
    }

    offset += glyphWidth;
    if (offset > width) {
      break;
    }

    current = next;
  }
}

// OFFSET: 0x005C9D00
// FUNCTION: fn_03635
void UINavigationV4Wnd::SetRoadIcon(int slot) {
  if (static_cast<unsigned>(slot - 1) > 7) {
    return;
  }

  g_navigationManager.SetViewIcon(slot);

  char normal[100];
  char over[100];
  char press[100];
  std::memset(normal, 0, sizeof(normal));
  std::memset(over, 0, sizeof(over));
  std::memset(press, 0, sizeof(press));
  std::sprintf(normal, "유저인터페이스\\navigation_interface3\\btn_roadIocn_select%d_normal.bmp", slot);
  std::sprintf(over, "유저인터페이스\\navigation_interface3\\btn_roadIocn_select%d_over.bmp", slot);
  std::sprintf(press, "유저인터페이스\\navigation_interface3\\btn_roadIocn_select%d_press.bmp", slot);

  UIBitmapButton* button = static_cast<UIBitmapButton*>(this->unknown_00C8);
  button->SetBitmap(normal, 0, 0);
  button->SetBitmap(over, 1, 0);
  button->SetBitmap(press, 2, 0);
  button->Invalidate();
  g_navigationManager.BuildGridLabels();
}

// OFFSET: 0x005C5E50
// FUNCTION: fn_03583
void UIPrivateAirplaneWnd::Slot15(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  const int count = static_cast<int>(g_privateAirplaneItems->m_needItems.size());
  for (int index = 0, y = 26; index < count; ++index, y += 26) {
    const int itemId = g_privateAirplaneItems->GetNeedItem(index);
    ITEM_INFO item;
    item.SetItemId(itemId);

    char name[260];
    std::memset(name, 0, sizeof(name));
    int capacity = 0x104;
    item.sub_6D5810(name, capacity, 0);
    if (std::strlen(name) == 0) {
      continue;
    }

    UIBasicButton* button = new UIBasicButton();
    const std::string text = FormatOwnedString(LookupMessage(0xd01), name);
    button->Configure(0xc0, text.c_str());
    button->Move(8, y);
    button->SetId(500);
    button->m_argument = itemId;
    this->AddChild(button);
    this->m_buttons.push_back(button);
  }
}

// OFFSET: 0x005C8DA0
// FUNCTION: fn_03628
int UIPrivateAirplaneWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 6:
    if (argument1 == 201) {
      if (!g_windowMgr.modalWindows.empty() && g_windowMgr.modalWindows.back()) {
        g_windowMgr.unknown_0474 = 0xBC;
      }
    } else if (argument1 == 500) {
      const std::string mapName = g_navigationManager.GetCurrentTargetName() + ".gat";
      g_navigationManager.RequestPrivateAirship(mapName, static_cast<int>(argument2));
    }
    break;
  case 35:
    for (UIBasicButton* button : this->m_buttons) {
      button->SetDisabled(1);
      button->Invalidate();
    }

    this->Invalidate();
    break;
  case 154:
    this->m_text = reinterpret_cast<const char*>(argument1);
    this->Invalidate();
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x00A7DA40

// OFFSET: 0x00A7B4D0
// FUNCTION: fn_16930
UIItemTagButton* UIWindow::CreateItemTagButton(const std::string& title, const std::string& label, int tagId, UIWindow* parent, std::uint32_t color) {
  const std::string text = "<" + label + ">";
  UIItemTagButton* button = new (std::nothrow) UIItemTagButton();
  if (button) {
    button->Configure(text.c_str(), color, 0, 12, 0, 0);
    if (title != label) {
      button->SetTooltip(title.c_str());
    }

    button->m_hoverColor = color;
    button->SetId(0x1D0);
    button->m_value = 0x205;
    button->Move(0, 0);
    button->unknown_01F8 = tagId;
    if (parent) {
      parent->AddChild(button);
    }
  }

  return button;
}

// OFFSET: 0x005C34B0
// FUNCTION: fn_03558
void UINavigationV4Wnd::DrawTargetSprite(int x, int y, int width, int height, const CBitmapRes* background) {
  if (!g_navigationManager.GetCurrentTarget().object) {
    return;
  }

  CNavi_Object* target = static_cast<CNavi_Object*>(g_navigationManager.GetCurrentTarget().object);
  NavigationTargetLook look = {
    target->unknown_0058, target->unknown_005C, target->unknown_0060, target->unknown_0064, target->unknown_0068,
    target->unknown_006C, target->unknown_0070, target->unknown_0074, target->unknown_0078,
  };
  if (!look.job) {
    this->DrawActionSprite(g_navigationSprites, g_navigationActions, x, y, width, height, 1500, true, background);
    return;
  }

  std::vector<CActRes*> actions(9);
  std::vector<CSprRes*> sprites(9);
  int offsetX = 0;
  int offsetY = 0;
  float scale = 1.0f;
  actions[0] = static_cast<CActRes*>(ResourceMgr::Instance().Load(GetBodyActionResourceName(look.job, look.sex, look.job, 0)));
  sprites[0] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetBodySprName(look.job, look.sex, look.job, 0)));
  if (!sprites[0] || !actions[0]) {
    return;
  }

  const std::vector<ActionFrame>* fitFrames = &actions[0]->Animation(0);
  if (!fitFrames || fitFrames->empty()) {
    return;
  }

  if (!this->FitSprite(&(*fitFrames)[0], sprites[0], width, height, &offsetX, &offsetY, &scale)) {
    return;
  }

  offsetY += 10;
  scale = 1.0f;
  if (background) {
    const BitmapImage& image = background->image;
    this->geometry.surface->Copy(offsetX - image.width / 2 + x, offsetY - image.height / 2 + y, image.width, image.height, image.pixels, true);
  }

  const std::vector<ActionFrame>* bodyFrames = &actions[0]->Animation(0);
  if (!bodyFrames) {
    return;
  }

  const DWORD now = timeGetTime();
  // GLOBAL: gl_25176
  static DWORD bodyTime = now;
  // GLOBAL: gl_25177
  static DWORD accessoryTime0 = now;
  // GLOBAL: gl_25178
  static DWORD accessoryTime1 = now;
  // GLOBAL: gl_25179
  static DWORD accessoryTime2 = now;
  // GLOBAL: gl_25180
  static int bodyFrame = 0;
  // GLOBAL: gl_25181
  static int accessoryFrame0 = 0;
  // GLOBAL: gl_25182
  static int accessoryFrame1 = 0;
  // GLOBAL: gl_25183
  static int accessoryFrame2 = 0;

  bodyFrame = 0;
  const int bodyCount = static_cast<int>(bodyFrames->size());
  if (static_cast<int>(now - bodyTime) / (this->unknown_011C / bodyCount) >= bodyCount) {
    bodyTime = now;
  }

  if (look.robe) {
    actions[8] = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetRobeActName(look.job, look.job, look.sex, look.robe, 0, look.job)));
    sprites[8] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetRobeSprName(look.job, look.job, look.sex, look.robe, 0, look.job)));
    if (sprites[8] && actions[8]) {
      const std::vector<ActionFrame>* robeFrames = &actions[8]->Animation(0);
      if (robeFrames && !robeFrames->empty()) {
        const ActionFrame* robeFrame = &(*robeFrames)[bodyFrame];
        if (robeFrame) {
          const SpriteRleImage* const* images[2] = {sprites[8]->images[0].data(), sprites[8]->images[1].data()};
          this->geometry.surface->DrawScaledSprite(
            x, y, width, height, std::span<const SpriteFrameCell>(robeFrame->cells), images, robeFrame->cellCount, offsetX, offsetY, scale, nullptr,
            g_renderer->atlas, *GetRenderSystemHolder()->system, g_serviceType, g_resourceReuseDelay
          );
        }
      }
    }
  }

  if (bodyFrames->empty()) {
    return;
  }

  const std::uint32_t* bodyPalette = nullptr;
  if (look.bodyPalette > 0) {
    std::string paletteName = g_session.GetBodyPalName(look.job, look.sex, look.bodyPalette, look.job, 0, 0);
    bodyPalette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(paletteName.c_str()))->palette;
  }

  const ActionFrame* body = &(*bodyFrames)[bodyFrame];
  {
    const SpriteRleImage* const* images[2] = {sprites[0]->images[0].data(), sprites[0]->images[1].data()};
    this->geometry.surface->DrawScaledSprite(
      x, y, width, height, std::span<const SpriteFrameCell>(body->cells), images, body->cellCount, offsetX, offsetY, 1.0f, bodyPalette,
      g_renderer->atlas, *GetRenderSystemHolder()->system, g_serviceType, g_resourceReuseDelay
    );
  }

  actions[1] = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetHeadActName(look.job, look.sex, &look.head)));
  sprites[1] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetHeadSprName(look.job, look.sex, &look.head)));
  if (sprites[1] && actions[1]) {
    const std::vector<ActionFrame>* headFrames = &actions[1]->Animation(0);
    if (headFrames) {
      const std::uint32_t* headPalette = nullptr;
      const ActionFrame* headFrame = &(*headFrames)[bodyFrame];
      const int headX = body->anchors[0].x - headFrame->anchors[0].x;
      const int headY = body->anchors[0].y - headFrame->anchors[0].y;
      if (look.headPalette > 0) {
        std::string paletteName = g_session.GetHeadPalName(look.head, look.job, look.sex, look.headPalette);
        headPalette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(paletteName.c_str()))->palette;
      }

      const SpriteRleImage* const* images[2] = {sprites[1]->images[0].data(), sprites[1]->images[1].data()};
      this->geometry.surface->DrawScaledSprite(
        x + headX, y + headY, width, height, std::span<const SpriteFrameCell>(headFrame->cells), images, headFrame->cellCount, offsetX, offsetY, scale,
        headPalette, g_renderer->atlas, *GetRenderSystemHolder()->system, g_serviceType, g_resourceReuseDelay
      );
    }
  }

  if (look.accessory0) {
    actions[2] = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetAccessoryActName(look.job, look.sex, look.head, look.accessory0)));
    sprites[2] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetAccessorySprName(look.job, look.sex, look.head, look.accessory0)));
    if (sprites[2] && actions[2]) {
      const std::vector<ActionFrame>* accessoryFrames = &actions[2]->Animation(0);
      if (accessoryFrames) {
        const int count = static_cast<int>(accessoryFrames->size());
        if (static_cast<int>(now - accessoryTime0) / (this->unknown_011C / count) >= count) {
          accessoryTime0 = now;
        }

        accessoryFrame0 = 0;
        const ActionFrame* frame = &(*accessoryFrames)[accessoryFrame0];
        const int accessoryX = body->anchors[0].x - frame->anchors[0].x;
        const int accessoryY = body->anchors[0].y - frame->anchors[0].y;
        const SpriteRleImage* const* images[2] = {sprites[2]->images[0].data(), sprites[2]->images[1].data()};
        this->geometry.surface->DrawScaledSprite(
          x + accessoryX, y + accessoryY, width, height, std::span<const SpriteFrameCell>(frame->cells), images, frame->cellCount, offsetX, offsetY,
          scale, nullptr, g_renderer->atlas, *GetRenderSystemHolder()->system, g_serviceType, g_resourceReuseDelay
        );
      }
    }
  }

  if (look.accessory1) {
    actions[3] = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetAccessoryActName(look.job, look.sex, look.head, look.accessory1)));
    sprites[3] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetAccessorySprName(look.job, look.sex, look.head, look.accessory1)));
    if (sprites[3] && actions[3]) {
      const std::vector<ActionFrame>* accessoryFrames = &actions[3]->Animation(0);
      if (accessoryFrames) {
        const int count = static_cast<int>(accessoryFrames->size());
        if (static_cast<int>(now - accessoryTime1) / (this->unknown_011C / count) >= count) {
          accessoryTime1 = now;
        }

        accessoryFrame1 = 0;
        const ActionFrame* frame = &(*accessoryFrames)[accessoryFrame1];
        const int accessoryX = body->anchors[0].x - frame->anchors[0].x;
        const int accessoryY = body->anchors[0].y - frame->anchors[0].y;
        const SpriteRleImage* const* images[2] = {sprites[3]->images[0].data(), sprites[3]->images[1].data()};
        this->geometry.surface->DrawScaledSprite(
          x + accessoryX, y + accessoryY, width, height, std::span<const SpriteFrameCell>(frame->cells), images, frame->cellCount, offsetX, offsetY,
          scale, nullptr, g_renderer->atlas, *GetRenderSystemHolder()->system, g_serviceType, g_resourceReuseDelay
        );
      }
    }
  }

  if (look.accessory2) {
    actions[4] = static_cast<CActRes*>(ResourceMgr::Instance().Load(g_session.GetAccessoryActName(look.job, look.sex, look.head, look.accessory2)));
    sprites[4] = static_cast<CSprRes*>(ResourceMgr::Instance().Load(g_session.GetAccessorySprName(look.job, look.sex, look.head, look.accessory2)));
    if (sprites[4] && actions[4]) {
      const std::vector<ActionFrame>* accessoryFrames = &actions[4]->Animation(0);
      if (accessoryFrames) {
        const int count = static_cast<int>(accessoryFrames->size());
        if (static_cast<int>(now - accessoryTime2) / (this->unknown_011C / count) >= count) {
          accessoryTime2 = now;
        }

        accessoryFrame2 = 0;
        const ActionFrame* frame = &(*accessoryFrames)[accessoryFrame2];
        const int accessoryX = body->anchors[0].x - frame->anchors[0].x;
        const int accessoryY = body->anchors[0].y - frame->anchors[0].y;
        const SpriteRleImage* const* images[2] = {sprites[4]->images[0].data(), sprites[4]->images[1].data()};
        this->geometry.surface->DrawScaledSprite(
          x + accessoryX, y + accessoryY, width, height, std::span<const SpriteFrameCell>(frame->cells), images, frame->cellCount, offsetX, offsetY,
          scale, nullptr, g_renderer->atlas, *GetRenderSystemHolder()->system, g_serviceType, g_resourceReuseDelay
        );
      }
    }
  }
}


// OFFSET: 0x00A84400
// OFFSET: 0x00A84400
// FUNCTION: fn_17010
int UIWindow::sub_A84400(
  int x, int y, const std::string& text, int width, int lineSpacing, int align, int family, int height, std::uint32_t color, unsigned char bold
) {
  std::list<std::string> lines;
  this->WrapFormattedText(text, &lines, width, family, height, bold);

  if (width == 0) {
    width = this->FormattedTextWidth(text, family, height, bold);
  }

  int totalHeight = 0;
  for (auto line = lines.begin(); line != lines.end(); ++line) {
    this->DrawFormattedText(x, y + totalHeight, *line, width, align, family, height, color, bold);
    totalHeight += height;
    auto nextLine = line;
    ++nextLine;
    if (nextLine != lines.end()) {
      totalHeight += lineSpacing;
    }
  }
  return totalHeight;
}

// OFFSET: 0x00A7DA40
// FUNCTION: fn_16958
std::list<int> UIWindow::WrapText(std::string text, std::list<std::string>* lines, int maxWidth, int family, int height, unsigned char bold) {
  lines->erase(lines->begin(), lines->end());
  std::list<int> widths;
  int width = 0;
  std::string line;
  const char* current = text.c_str();
  const char* end = current + std::strlen(current);

  while (current != end) {
    if (*current == '\n') {
      lines->push_back(line);
      line.clear();
      widths.push_back(width);
      width = 0;
      ++current;
      continue;
    }

    if (current[1] == '\\' && current[2] == 'n') {
      lines->push_back(line);
      line.clear();
      widths.push_back(width);
      width = 0;
      current += 2;
      continue;
    }

    const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    const std::string character(current, next);
    const char* const measured = character.c_str();
    int characterWidth = 0;
    if (measured) {
      const unsigned bytes = static_cast<unsigned>(std::strlen(measured));
      if (bytes) {
        TextContext context(*this->geometry.surface);
        context.SetFont(family, height, bold, 0);
        characterWidth = context.Measure(measured, bytes).cx;
      }
    }

    if (width + characterWidth > maxWidth) {
      lines->push_back(line);
      line.clear();
      widths.push_back(width);
      width = 0;
    }

    width += characterWidth;
    line.append(current, next - current);
    current = next;
    if (current == end) {
      lines->push_back(line);
    }
  }

  return widths;
}
