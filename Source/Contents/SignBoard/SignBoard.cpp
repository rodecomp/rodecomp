#include "Contents/SignBoard/SignBoard.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>
#include <new>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24892
CSignBoardMgr* s_signBoardMgr = nullptr;

// OFFSET: 0x0081FC30
// FUNCTION: fn_10244
bool CreateSignBoardMgr() {
  if (s_signBoardMgr == nullptr) {
    s_signBoardMgr = new (std::nothrow) CSignBoardMgr{};
    if (s_signBoardMgr != nullptr) {
      s_signBoardMgr->unknown_0000 = true;
    }
    if (s_signBoardMgr != nullptr && s_signBoardMgr->unknown_0000) {
      return true;
    }
    if (s_signBoardMgr != nullptr) {
      delete s_signBoardMgr;
      s_signBoardMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x0081FE30
// FUNCTION: fn_10245
void DestroySignBoardMgr() {
  if (s_signBoardMgr != nullptr) {
    delete s_signBoardMgr;
    s_signBoardMgr = nullptr;
  }
}

// OFFSET: 0x0081FE50

// OFFSET: 0x00820BD0
// FUNCTION: fn_10248
void CSignBoardMgr::LoadList() {
  SignBoardInfo defaults;
  g_luaScript->Call(
    "GetSignBoardListTable", "d>sddddsss", 1, &defaults.map, &defaults.column, &defaults.row, &defaults.offset, &defaults.type, &defaults.image,
    &defaults.unknown_0014, &defaults.unknown_001C
  );

  int count = 0;
  g_luaScript->Call("GetTableSizeForC", "s>d", "SignBoardList", &count);
  for (int index = 2; index < count + 1; ++index) {
    SignBoardInfo info;
    g_luaScript->Call(
      "GetSignBoardListTable", "d>sddddsss", index, &info.map, &info.column, &info.row, &info.offset, &info.type, &info.image, &info.unknown_0014,
      &info.unknown_001C
    );

    switch (info.type) {
    case 2:
      info.type = 2;
      break;
    case 3:
      info.type = 3;
      if (info.unknown_0014 == nullptr) {
        info.unknown_0014 = defaults.unknown_0014;
      }
      if (info.unknown_001C == nullptr) {
        info.unknown_001C = defaults.unknown_001C;
      }
      break;
    default:
      info.type = 1;
      break;
    }

    if (info.image == nullptr) {
      info.image = defaults.image;
    }
    info.offset -= 3;
    this->m_entries.push_back(info);
  }
}

// OFFSET: 0x008213D0
// FUNCTION: fn_10258
C3DCooRendering::~C3DCooRendering() = default;

// OFFSET: 0x00821440
// FUNCTION: fn_10259
CBmpObjWnd::~CBmpObjWnd() = default;

// OFFSET: 0x008214C0
// FUNCTION: fn_10260
CIcon::~CIcon() = default;

// OFFSET: 0x00821530
// FUNCTION: fn_10261
CSignBoardWnd::~CSignBoardWnd() = default;

// OFFSET: 0x008215D0
// FUNCTION: fn_10262
void CSignBoardWnd::ApplyColor() {
  int red = 0;
  int green = 0;
  int blue = 0;
  if (this->unknown_00B0.size() != 0) {
    int values[3] = {0, 0, 0};
    int index = 0;
    if (this->unknown_00B0.find(",") != std::string::npos) {
      std::string rest = this->unknown_00B0;
      std::size_t comma = rest.find(",");
      while (comma != std::string::npos) {
        values[index] = std::atoi(rest.substr(0, comma).c_str());
        index += 1;
        rest = rest.substr(comma + 1);
        comma = rest.find(",");
      }
      values[index] = std::atoi(rest.c_str());
    }
    red = values[0];
    green = values[1];
    blue = values[2];
    if (this->unknown_00B0.find("#") != std::string::npos) {
      std::string hex = this->unknown_00B0.substr(this->unknown_00B0.find("#") + 1);
      const unsigned long color = std::strtoul(hex.c_str(), nullptr, 16);
      red = static_cast<int>((color >> 16) & 0xff);
      green = static_cast<int>((color >> 8) & 0xff);
      blue = static_cast<int>(color & 0xff);
    }
  }
  this->text->SetColor(red, green, blue);
}

// OFFSET: 0x00821B40
// FUNCTION: fn_10263
void CBmpObjWnd::Slot15(int width, int height) {
}

// OFFSET: 0x00821B50
// FUNCTION: fn_10264
void CSignBoardWnd::Slot15(int width, int height) {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\signboard\\bg_signboard.bmp")));
  this->Resize(bitmap->image.width, bitmap->image.height);
  UIStaticText* text = new (std::nothrow) UIStaticText(*this->m_context, *this->m_controls);
  this->text = text;
  text->SetSize(0x7e, 0xe);
  text->Move(0x1e, this->geometry.height / 2 - 7);
  text->SetTextFitted("");
  text->fontFamily = 0;
  text->fontHeight = 12;
  text->bold = 0;
  text->italic = 0;
  text->SetColor(0x9e, 0xbd, 0xff);
  this->AddChild(text);
}

// OFFSET: 0x00821C50
// FUNCTION: fn_10265
void CBmpObjWnd::Paint() {
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(this->path.c_str()))), false);
}

// OFFSET: 0x00821C90
// FUNCTION: fn_10266
void CSignBoardWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\signboard\\bg_signboard.bmp"))), false);
  this->DrawBitmap(5, 5, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(this->iconPath.c_str()))), true);
}

// OFFSET: 0x00821D10
// FUNCTION: fn_10267
void C3DCooRendering::Create(UIWindowContext& windows, UIButtonContext& controls) {
  UIWindow* window = nullptr;
  int width = 0;
  int height = 0;
  if (this->type == 1) {
    window = new (std::nothrow) CBmpObjWnd(windows);
    width = 0xaa;
    height = 0xaa;
  } else if (this->type == 3) {
    window = new (std::nothrow) CSignBoardWnd(windows, controls);
    width = 0xa2;
    height = 0x24;
  } else {
    return;
  }
  this->window = window;
  window->SetSize(width, height);
  g_windowMgr.AddRenderedFront(this->window);
}

// OFFSET: 0x00821EC0
// FUNCTION: fn_10268
void C3DCooRendering::SetOverrideColor() {
  this->render.overrideColor = 0xFFFFFF00;
}

// OFFSET: 0x00821ED0
// FUNCTION: fn_10269
bool C3DCooRendering::OnProcess() {
  if (UIWindow* window = this->window) {
    const int lift = g_renderer->viewport.height * 81 / 480;
    window->Move(
      this->render.screenX - window->geometry.width / 2,
      static_cast<int>(static_cast<float>(this->render.screenY - window->geometry.height / 2) - static_cast<float>(lift) * this->render.unknown_005C)
    );
  }
  return true;
}

// OFFSET: 0x00821F50
// FUNCTION: fn_10270
bool CIcon::OnProcess() {
  const float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  this->render.previousAnimation = this->render.animation;
  this->render.curMotion = static_cast<int>(frame / this->render.speed) % static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation));
  return true;
}

// OFFSET: 0x00821FC0
// FUNCTION: fn_10271
void C3DCooRendering::Render(int argument) {
  float x;
  float y;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&this->render.position.x, reinterpret_cast<const float*>(argument), &x, &y, &reciprocalDepth);
  this->render.unknown_005C = g_renderer->ScaleDistance(reciprocalDepth) / g_heightScale;
  this->SetTlvert(x, y);
}

// OFFSET: 0x00822040
// FUNCTION: fn_10272
void CIcon::Render(int argument) {
  this->DrawSprite(argument, false, 0.0f);
}

// OFFSET: 0x008220A0
// FUNCTION: fn_10273
void CIcon::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) {
  g_modeMgr.GetGameMode();
  this->CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}

// OFFSET: 0x008220F0
// FUNCTION: fn_10274
void CIcon::SetRenderInfo(const float* bounds, float x, float y) {
  this->SetTlvert(x, y);
}

// OFFSET: 0x00822140
// FUNCTION: fn_10275
void CIcon::Load(const std::string& name) {
  std::string stem;
  if (name.find(".") != std::string::npos) {
    stem = name.substr(0, name.find("."));
  }
  this->SetSpriteName(ResourceMgr::Instance(), (stem + ".spr").c_str());
  this->SetActionName(ResourceMgr::Instance(), (stem + ".act").c_str());
  this->SetAction(0, 10, 0);
}

// OFFSET: 0x00822310
// FUNCTION: fn_10276
void C3DCooRendering::SetBoard(const std::string& text, const std::string& color) {
  CSignBoardWnd* board = static_cast<CSignBoardWnd*>(this->window);
  board->unknown_0080 = text;
  board->unknown_00B0 = color;
  board->ApplyColor();
  board->text->SetTextFitted(board->unknown_0080.c_str());
}

// OFFSET: 0x00822390
// FUNCTION: fn_10277
void C3DCooRendering::SetWindowImage(const std::string& name) {
  if (this->window == nullptr) {
    return;
  }

  if (this->type == 1) {
    CBmpObjWnd* window = static_cast<CBmpObjWnd*>(this->window);
    window->path = name;
    const CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(window->path.c_str())));
    window->Resize(bitmap->image.width, bitmap->image.height);
  } else if (this->type == 3) {
    static_cast<CSignBoardWnd*>(this->window)->iconPath = name;
  }
}
