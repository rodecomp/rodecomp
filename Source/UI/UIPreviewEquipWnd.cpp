#include "UI/UIPreviewEquipWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/CharacterRender.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/SpriteDrawingPolicy.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/World/World.h"
#include "UI/Window/FuncDrawWNDChar.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x007F2F90
// FUNCTION: fn_09879
UIPreviewEquipWnd::UIPreviewEquipWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 1), saved(nullptr), m_render(nullptr), m_leftButton(nullptr), m_rightButton(nullptr), unknown_00C4(0) {
}

// OFFSET: 0x007F3090
// FUNCTION: fn_09880
UIPreviewEquipWnd::~UIPreviewEquipWnd() {
  delete this->m_render;
}

// OFFSET: 0x007F3590
// FUNCTION: fn_09881
void UIPreviewEquipWnd::DrawCharacter() {
  CCharacterRender* render = this->m_render;
  matrix view;
  view.MakeXRotation(render->unknown_0074 / 180.0f * 3.141592f);
  view.AppendYRotation(render->unknown_0078 / 180.0f * 3.141592f);

  const float distance = -render->unknown_0070;
  float eye[3];
  eye[0] = distance * view.values[6] + view.values[9];
  eye[1] = distance * view.values[7] + view.values[10];
  eye[2] = distance * view.values[8] + view.values[11];

  float up[3];
  up[0] = render->unknown_0088;
  up[1] = render->unknown_008C;
  up[2] = *reinterpret_cast<float*>(&render->unknown_0090);
  view.LookAt(eye, reinterpret_cast<float*>(&render->unknown_0004), up);

  float screenX = 0.0f;
  float screenY = 0.0f;
  float depth = 0.0f;
  g_renderer->ProjectViewportCentered(
    reinterpret_cast<float*>(&render->unknown_0004), view.values, &screenX, &screenY, &depth,
    static_cast<float>(this->geometry.width) + *reinterpret_cast<float*>(&render->unknown_0054),
    static_cast<float>(this->geometry.height) + render->unknown_0058
  );

  FuncDrawWNDChar drawer(
    this, static_cast<int>(screenX), static_cast<int>(screenY), static_cast<unsigned char>(render->unknown_0018[0]),
    static_cast<short>(render->unknown_0010), render->unknown_0014, static_cast<short>(render->unknown_0050),
    static_cast<short>(render->unknown_0018[1]), static_cast<short>(render->unknown_0028), static_cast<short>(render->unknown_002C),
    static_cast<short>(render->unknown_0030), render->unknown_00BC, render->unknown_004C, render->unknown_0048, render->unknown_0018[2],
    render->unknown_0018[3], render->unknown_0040, render->unknown_0044, 0
  );
  drawer.Draw(1);
}

// OFFSET: 0x007F38A0

// OFFSET: 0x007F3CD0
// FUNCTION: fn_09883
void UIPreviewEquipWnd::Paint() {
  this->SetSolid(this->m_context->primitives, this->geometry.width, this->geometry.height, 0);
  this->DrawNamed(this->m_context->primitives, 0, 0, "유저인터페이스\\equip_preview\\bg_mounting2.bmp", 0xffffffff, 0, 0);
  void (UIPreviewEquipWnd::*draw)() = &UIPreviewEquipWnd::DrawCharacter;
  (this->*draw)();
}

// OFFSET: 0x007F3DC0
// FUNCTION: fn_09885
void UIPreviewEquipWnd::SyncFromPlayer() {
  this->m_render->unknown_00B0 = 0;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CPlayer* player = mode->scene->player;
  if (player == nullptr) {
    return;
  }

  this->m_render->unknown_0010 = player->GetEffectiveJob(-1);
  this->m_render->unknown_0014 = player->unknown_04D4;
  this->m_render->unknown_0018[0] = player->unknown_0260;
  this->m_render->unknown_0018[1] = player->unknown_0440;
  this->m_render->unknown_0018[2] = static_cast<int>(player->render.previousAnimation);
  this->m_render->unknown_0018[3] = player->unknown_0444;
  this->m_render->unknown_0028 = player->unknown_0450;
  this->m_render->unknown_002C = player->unknown_0454;
  this->m_render->unknown_0030 = player->unknown_0458;
  reinterpret_cast<SpriteDrawingPolicy*>(this->m_render)->SetRobe(player->unknown_049C);
  this->m_render->unknown_0048 = player->Slot56();
  this->m_render->unknown_004C = player->unknown_02CC;
  this->m_render->unknown_0050 = player->Slot46();
  *reinterpret_cast<float*>(&this->m_render->unknown_0054) = -5.0f;
  this->m_render->unknown_0058 = 136.0f;
  this->RefreshHeadgearLocations();
  this->Invalidate();
}

// OFFSET: 0x007F3EF0
// FUNCTION: fn_09886
int UIPreviewEquipWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    switch (a) {
    case 0xC9:
      g_windowMgr.CloseWindow(0x126);
      break;
    case 0x16B:
      this->SyncFromPlayer();
      break;
    case 0x180:
      this->m_render->unknown_0040 = this->m_render->unknown_0040 + 1;
      if (this->m_render->unknown_0040 > 7) {
        this->m_render->unknown_0040 = 0;
      }
      this->Invalidate();
      break;
    case 0x181:
      this->m_render->unknown_0040 = this->m_render->unknown_0040 - 1;
      if (this->m_render->unknown_0040 < 0) {
        this->m_render->unknown_0040 = 7;
      }
      this->Invalidate();
      break;
    }
  } else if (message == 0x22) {
    this->saved = reinterpret_cast<PreviewEquipWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->Move(this->saved->x, this->saved->y);
  } else if (message == 0x17 && a != 0) {
    const ITEM_INFO* item = reinterpret_cast<const ITEM_INFO*>(a);
    CCharacterRender* render = this->m_render;
    const int value = static_cast<int>(item->unknown_0070);
    switch (item->unknown_0000[2]) {
    case 0x100:
    case 0x400:
      render->unknown_002C = value;
      render->unknown_0038 = 0x100;
      if ((render->unknown_0034 & 0x100) != 0) {
        render->unknown_0034 = 0;
        render->unknown_0028 = 0;
      }
      if ((render->unknown_003C & 0x100) != 0) {
        render->unknown_003C = 0;
        render->unknown_0030 = 0;
      }
      break;
    case 0x200:
    case 0x800:
      render->unknown_0030 = value;
      render->unknown_003C = 0x200;
      if ((render->unknown_0034 & 0x200) != 0) {
        render->unknown_0034 = 0;
        render->unknown_0028 = 0;
      }
      if ((render->unknown_0038 & 0x200) != 0) {
        render->unknown_0038 = 0;
        render->unknown_002C = 0;
      }
      break;
    case 0x300:
    case 0xC00:
      render->unknown_002C = value;
      render->unknown_0030 = 0;
      render->unknown_0038 = 0x300;
      render->unknown_003C = 0x300;
      if (render->unknown_0034 != 1) {
        render->unknown_0034 = 0;
        render->unknown_0028 = 0;
      }
      break;
    case 0x301:
    case 0x1C00:
      render->unknown_0028 = value;
      render->unknown_002C = 0;
      render->unknown_0030 = 0;
      render->unknown_0034 = 0x301;
      render->unknown_0038 = 0x301;
      render->unknown_003C = 0x301;
      break;
    case 0x201:
    case 0x1800:
      render->unknown_0030 = value;
      render->unknown_0028 = 0;
      render->unknown_003C = 0x201;
      render->unknown_0034 = 0x201;
      if (render->unknown_0038 != 0x100) {
        render->unknown_0038 = 0;
        render->unknown_002C = 0;
      }
      break;
    case 0x101:
    case 0x1400:
      render->unknown_002C = value;
      render->unknown_0028 = 0;
      render->unknown_0038 = 0x101;
      render->unknown_0034 = 0x101;
      if (render->unknown_003C != 0x200) {
        render->unknown_003C = 0;
        render->unknown_0030 = 0;
      }
      break;
    case 1:
    case 0x1000:
      render->unknown_0028 = value;
      render->unknown_0034 = 1;
      if ((render->unknown_0038 & 1) != 0) {
        render->unknown_0038 = 0;
        render->unknown_002C = 0;
      }
      if ((render->unknown_003C & 1) != 0) {
        render->unknown_003C = 0;
        render->unknown_0030 = 0;
      }
      break;
    case 4:
    case 0x2000:
      reinterpret_cast<SpriteDrawingPolicy*>(render)->SetRobe(value);
      break;
    }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x007F43E0
// FUNCTION: fn_09887
void UIPreviewEquipWnd::RefreshHeadgearLocations() {
  if (this->m_render->unknown_0028) {
    ITEM_INFO equipped = g_session.GetItem17F0(0);
    if (this->m_render->unknown_0028 == equipped.unknown_0070) {
      this->m_render->unknown_0034 = 1;
      if (equipped.unknown_0000[2] == 0x301) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }

    ITEM_INFO costume = g_session.GetItem2B50(0);
    if (this->m_render->unknown_0028 == costume.unknown_0070) {
      this->m_render->unknown_0034 = 1;
      if (costume.unknown_0000[2] == 0x1C00) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }
  }

  if (this->m_render->unknown_002C) {
    ITEM_INFO equipped = g_session.GetItem17F0(8);
    if (this->m_render->unknown_002C == equipped.unknown_0070) {
      if (equipped.unknown_0000[2] == 0x100) {
        this->m_render->unknown_0038 = 0x100;
      }

      if (equipped.unknown_0000[2] == 0x101) {
        this->m_render->unknown_0038 = 0x101;
        this->m_render->unknown_0034 = 0x101;
      }

      if (equipped.unknown_0000[2] == 0x300) {
        this->m_render->unknown_0038 = 0x300;
        this->m_render->unknown_003C = 0x300;
      }

      if (equipped.unknown_0000[2] == 0x301) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }

    ITEM_INFO costume = g_session.GetItem2B50(8);
    if (this->m_render->unknown_002C == costume.unknown_0070) {
      if (costume.unknown_0000[2] == 0x400) {
        this->m_render->unknown_0038 = 0x100;
      }

      if (costume.unknown_0000[2] == 0x1400) {
        this->m_render->unknown_0038 = 0x101;
        this->m_render->unknown_0034 = 0x101;
      }

      if (costume.unknown_0000[2] == 0xC00) {
        this->m_render->unknown_0038 = 0x300;
        this->m_render->unknown_003C = 0x300;
      }

      if (costume.unknown_0000[2] == 0x1C00) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }
  }

  if (this->m_render->unknown_0030) {
    ITEM_INFO equipped = g_session.GetItem17F0(9);
    if (this->m_render->unknown_0030 == equipped.unknown_0070) {
      if (equipped.unknown_0000[2] == 0x200) {
        this->m_render->unknown_003C = 0x200;
      }

      if (equipped.unknown_0000[2] == 0x201) {
        this->m_render->unknown_003C = 0x201;
        this->m_render->unknown_0034 = 0x201;
      }
    }

    ITEM_INFO costume = g_session.GetItem2B50(9);
    if (this->m_render->unknown_0030 == costume.unknown_0070) {
      if (costume.unknown_0000[2] == 0x800) {
        this->m_render->unknown_003C = 0x200;
      }

      if (costume.unknown_0000[2] == 0x1800) {
        this->m_render->unknown_003C = 0x201;
        this->m_render->unknown_0034 = 0x201;
      }
    }
  }
}

// OFFSET: 0x007F4830
// FUNCTION: fn_09888
void UIPreviewEquipWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007F3DB0
// FUNCTION: fn_09884
void UIPreviewEquipWnd::Slot19() {
  this->Invalidate();
}
