#include "Contents/SignBoard/SignBoard.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Resource/World/TerrainResource.h"
#include "Contents/World/World.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>
#include <mbstring.h>
#include <new>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24892
CSignBoardMgr* s_signBoardMgr = nullptr;

// OFFSET: 0x0081FC30
// FUNCTION: fn_10244
bool CreateSignBoardMgr() {
  if (s_signBoardMgr == nullptr) {
    s_signBoardMgr = new (std::nothrow) CSignBoardMgr();
    if (s_signBoardMgr != nullptr) {
      if (!s_signBoardMgr->initialized) {
        delete s_signBoardMgr;
        s_signBoardMgr = nullptr;
      } else {
        return true;
      }
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
// FUNCTION: fn_10246
void __fastcall InitializeSignBoardManager(CSignBoardMgr* manager) {
  manager->LoadList();
  manager->Initialize();
}

// OFFSET: 0x0081FE60
// FUNCTION: fn_10247
void CSignBoardMgr::Initialize() {
  std::string mapName;
  if (g_modeMgr.GetGameMode() != nullptr) {
    mapName = std::string(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->mapName);
  }
  mapName = mapName.substr(0, mapName.find(".rsw"));

  for (std::vector<SignBoardInfo>::iterator it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
    std::string entryMap(it->map);
    LowercaseText(entryMap);
    LowercaseText(mapName);
    if (entryMap != mapName) {
      continue;
    }

    switch (it->type) {
    case 1: {
      C3DCooRendering* object = new C3DCooRendering();
      object->type = it->type;
      object->Create();
      std::string image;
      if (_mbsstr(reinterpret_cast<const unsigned char*>(it->image), reinterpret_cast<const unsigned char*>("유저인터페이스")) == nullptr) {
        image = "유저인터페이스\\";
      }
      image += it->image;
      object->SetWindowImage(image);

      vector3d position = {0.0f, 0.0f, 0.0f};
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(it->column, it->row, position.x, position.z);
      position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(position.x, position.z) -
        static_cast<float>(it->offset);
      object->render.position = position;
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(object);
      break;
    }
    case 2: {
      CIcon* icon = new CIcon();
      vector3d position = {0.0f, 0.0f, 0.0f};
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(it->column, it->row, position.x, position.z);
      position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(position.x, position.z) -
        static_cast<float>(it->offset);
      icon->render.position = position;
      icon->Load(std::string(it->image));
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(icon);
      break;
    }
    case 3: {
      C3DCooRendering* object = new C3DCooRendering();
      object->type = it->type;
      object->Create();
      std::string image;
      if (_mbsstr(reinterpret_cast<const unsigned char*>(it->image), reinterpret_cast<const unsigned char*>("유저인터페이스")) == nullptr) {
        image = "유저인터페이스\\";
      }
      image += it->image;
      object->SetWindowImage(image);
      object->SetBoard(std::string(it->text), std::string(it->color));

      vector3d position = {0.0f, 0.0f, 0.0f};
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CellCenter(it->column, it->row, position.x, position.z);
      position.y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(position.x, position.z) -
        static_cast<float>(it->offset);
      object->render.position = position;
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(object);
      break;
    }
    }
  }
}

// OFFSET: 0x00820BD0
// FUNCTION: fn_10248
void CSignBoardMgr::LoadList() {
  SignBoardInfo defaults;
  g_session.m_hotkeyScript->Call(
    "GetSignBoardListTable", "d>sddddsss", 1, &defaults.map, &defaults.column, &defaults.row, &defaults.offset, &defaults.type, &defaults.image,
    &defaults.text, &defaults.color
  );

  int count = 0;
  g_session.m_hotkeyScript->Call("GetTableSizeForC", "s>d", "SignBoardList", &count);
  for (int index = 2; index < count + 1; ++index) {
    SignBoardInfo info;
    g_session.m_hotkeyScript->Call(
      "GetSignBoardListTable", "d>sddddsss", index, &info.map, &info.column, &info.row, &info.offset, &info.type, &info.image, &info.text,
      &info.color
    );
    switch (info.type) {
    case 2:
      info.type = 2;
      break;
    case 3:
      info.type = 3;
      if (info.text == nullptr) {
        info.text = defaults.text;
      }
      if (info.color == nullptr) {
        info.color = defaults.color;
      }
      break;
    case 1:
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

// OFFSET: 0x00821040
// FUNCTION: fn_10254
C3DCooRendering::C3DCooRendering() : window(nullptr) {
}

// OFFSET: 0x008210A0
// FUNCTION: fn_10255
CBmpObjWnd::CBmpObjWnd() : UIWindow(0) {
}

// OFFSET: 0x00821120
// FUNCTION: fn_10256
CIcon::CIcon() {
}

// OFFSET: 0x00821180
// FUNCTION: fn_10257
CSignBoardWnd::CSignBoardWnd() : UIWindow(0) {
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
  if (this->colorText.size() != 0) {
    int values[3] = {0, 0, 0};
    int index = 0;
    std::string rest = this->colorText;
    std::string token = this->colorText;
    std::size_t comma = this->colorText.find(",");
    if (comma != std::string::npos) {
      while (comma != std::string::npos) {
        token = rest.substr(0, comma);
        values[index] = std::atoi(token.c_str());
        index += 1;
        rest = rest.substr(comma + 1);
        comma = rest.find(",");
      }
      values[index] = std::atoi(rest.c_str());
    }
    red = values[0];
    green = values[1];
    blue = values[2];
    if (this->colorText.find("#") != std::string::npos) {
      std::string hex = this->colorText;
      hex = hex.substr(hex.find("#") + 1);
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
void CBmpObjWnd::OnCreate(int width, int height) {
}

// OFFSET: 0x00821B50
// FUNCTION: fn_10264
void CSignBoardWnd::OnCreate(int width, int height) {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\signboard\\bg_signboard.bmp")));
  this->Resize(bitmap->image.width, bitmap->image.height);
  this->text = new (std::nothrow) UIStaticText();

  this->text->SetSize(0x7e, 0xe);
  this->text->Move(0x1e, this->geometry.height / 2 - 7);
  this->text->SetTextFitted("");
  this->text->SetFont(0, 12, 0, 0);
  this->text->SetColor(0x9e, 0xbd, 0xff);
  this->AddChild(this->text);
}

// OFFSET: 0x00821C50
// FUNCTION: fn_10265
void CBmpObjWnd::Paint() {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->path.c_str())));
  this->DrawBitmap(0, 0, bitmap, false);
}

// OFFSET: 0x00821C90
// FUNCTION: fn_10266
void CSignBoardWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\signboard\\bg_signboard.bmp")));
  this->DrawBitmap(0, 0, background, false);
  CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->iconPath.c_str())));
  this->DrawBitmap(5, 5, icon, true);
}

// OFFSET: 0x00821D10
// FUNCTION: fn_10267
void C3DCooRendering::Create() {
  UIWindow* window;
  int width;
  int height;
  switch (this->type) {
  case 1:
    window = new (std::nothrow) CBmpObjWnd();
    width = 0xaa;
    height = 0xaa;
    break;
  case 3:
    window = new (std::nothrow) CSignBoardWnd();
    width = 0xa2;
    height = 0x24;
    break;
  default:
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
      static_cast<int>(static_cast<float>(this->render.screenY - window->geometry.height / 2) - static_cast<float>(lift) * this->render.distanceScale)
    );
  }
  return true;
}

// OFFSET: 0x00821F50
// FUNCTION: fn_10270
bool CIcon::OnProcess() {
  const float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.motionStartTime) / 24.0);
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
  const float distance = g_renderer->ScaleDistance(reciprocalDepth);
  this->render.distanceScale = distance / g_heightScale;
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
  const std::string spriteName = stem + ".spr";
  const std::string actionName = stem + ".act";
  this->SetSpriteName(spriteName.c_str());
  this->SetActionName(actionName.c_str());
  this->SetAction(0, 10, 0);
}

// OFFSET: 0x00822310
// FUNCTION: fn_10276
void C3DCooRendering::SetBoard(const std::string& text, const std::string& color) {
  static_cast<CSignBoardWnd*>(this->window)->boardText = text;
  static_cast<CSignBoardWnd*>(this->window)->colorText = color;
  CSignBoardWnd* board = static_cast<CSignBoardWnd*>(this->window);
  board->ApplyColor();
  board->text->SetTextFitted(board->boardText.c_str());
}

// OFFSET: 0x00822390
// FUNCTION: fn_10277
void C3DCooRendering::SetWindowImage(const std::string& name) {
  if (this->window == nullptr) {
    return;
  }

  switch (this->type) {
  case 1: {
    CBmpObjWnd* window = static_cast<CBmpObjWnd*>(this->window);
    window->path = name;
    const CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(window->path.c_str())));
    window->Resize(bitmap->image.width, bitmap->image.height);
    break;
  }
  case 3:
    static_cast<CSignBoardWnd*>(this->window)->iconPath = name;
    break;
  }
}
