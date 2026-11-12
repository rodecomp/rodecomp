#include "UI/Items/UIPreviewEquipWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/CharacterRender.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/CombatTypes.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/World/World.h"
#include "UI/Login/FuncDrawWNDChar.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include "Resource/MSGSTRINGID.h"
// OFFSET: 0x007F2F90
// FUNCTION: fn_09879
UIPreviewEquipWnd::UIPreviewEquipWnd()
    : UIFrameWnd(1), saved(nullptr), m_render(nullptr), m_leftButton(nullptr), m_rightButton(nullptr), m_actionButton(0) {
}

// OFFSET: 0x007F3090
// FUNCTION: fn_09880
UIPreviewEquipWnd::~UIPreviewEquipWnd() {
  delete this->m_render;
}

// OFFSET: 0x007F3590
// FUNCTION: fn_09881
void UIPreviewEquipWnd::DrawCharacter() {
  matrix view;
  view.MakeXRotation(this->m_render->cameraPitch / 180.0f * 3.141592f);
  view.AppendYRotation(this->m_render->cameraYaw / 180.0f * 3.141592f);

  vector3d offset;
  offset.x = 0.0f;
  offset.y = 0.0f;
  offset.z = -this->m_render->cameraDistance;
  float eye[3];
  eye[0] = offset.x * view.values[0] + offset.y * view.values[3] + offset.z * view.values[6] + view.values[9];
  eye[1] = offset.x * view.values[1] + offset.y * view.values[4] + offset.z * view.values[7] + view.values[10];
  eye[2] = offset.x * view.values[2] + offset.y * view.values[5] + offset.z * view.values[8] + view.values[11];

  float up[3];
  up[0] = this->m_render->cameraUpX;
  up[1] = this->m_render->cameraUpY;
  up[2] = *reinterpret_cast<float*>(&this->m_render->cameraUpZ);
  view.LookAt(eye, reinterpret_cast<float*>(&this->m_render->positionX), up);

  float screenX = 0.0f;
  float screenY = 0.0f;
  float depth = 0.0f;
  g_renderer->ProjectViewportCentered(
    reinterpret_cast<float*>(&this->m_render->positionX), view.values, &screenX, &screenY, &depth,
    static_cast<float>(this->geometry.width) + *reinterpret_cast<float*>(&this->m_render->viewportOffsetX),
    static_cast<float>(this->geometry.height) + this->m_render->viewportOffsetY
  );

  FuncDrawWNDChar(
    this, static_cast<int>(screenX), static_cast<int>(screenY), static_cast<unsigned char>(this->m_render->appearance[0]),
    static_cast<short>(this->m_render->job), this->m_render->headJob, static_cast<short>(this->m_render->bodyCostume),
    static_cast<short>(this->m_render->appearance[1]), static_cast<short>(this->m_render->lowerHeadgear), static_cast<short>(this->m_render->upperHeadgear),
    static_cast<short>(this->m_render->middleHeadgear), this->m_render->robe, this->m_render->riding, this->m_render->madogearType, this->m_render->appearance[2],
    this->m_render->appearance[3], this->m_render->actionIndex, this->m_render->frameIndex, 0
  ).Draw(1);
}

// OFFSET: 0x007F38A0
// FUNCTION: fn_09882
void UIPreviewEquipWnd::OnCreate(int width, int height) {

  this->AddBaseButton();
  this->AddCloseButton("", 0);

  UIStaticText* shadow = new UIStaticText();
  shadow->SetSize(
    this->TextWidth(LookupMessage(MSI_IMPOSSIBLE_CHANGE_GUILD_MASTER_IN_SIEGE_TIME), 0, 0, 12, 0, 0),
    this->TextHeight(LookupMessage(MSI_IMPOSSIBLE_CHANGE_GUILD_MASTER_IN_SIEGE_TIME), 0, 0, 12, 0, 0)
  );
  shadow->SetText(LookupMessage(MSI_IMPOSSIBLE_CHANGE_GUILD_MASTER_IN_SIEGE_TIME), 0);
  shadow->SetColor(255, 255, 255);
  shadow->Move(19, 4);
  this->AddChild(shadow);

  UIStaticText* title = new UIStaticText();
  title->SetSize(
    this->TextWidth(LookupMessage(MSI_IMPOSSIBLE_CHANGE_GUILD_MASTER_IN_SIEGE_TIME), 0, 0, 12, 0, 0),
    this->TextHeight(LookupMessage(MSI_IMPOSSIBLE_CHANGE_GUILD_MASTER_IN_SIEGE_TIME), 0, 0, 12, 0, 0)
  );
  title->SetText(LookupMessage(MSI_IMPOSSIBLE_CHANGE_GUILD_MASTER_IN_SIEGE_TIME), 0);
  title->Move(18, 3);
  this->AddChild(title);

  if (!g_modeMgr.GetGameMode() || !static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene) {
    return;
  }

  this->m_render = new CCharacterRender(width, height);
  this->SyncFromPlayer();

  this->m_leftButton = new UIBitmapButton();
  this->m_leftButton->SetBitmap("유저인터페이스\\equip_preview\\chr_arrow_rotate_l_out.bmp", 0, 0);
  this->m_leftButton->SetBitmap("유저인터페이스\\equip_preview\\chr_arrow_rotate_l_over.bmp", 1, 0);
  this->m_leftButton->SetBitmap("유저인터페이스\\equip_preview\\chr_arrow_rotate_l_press.bmp", 2, 0);
  this->m_leftButton->SetSize(this->m_leftButton->BitmapWidth(), this->m_leftButton->BitmapHeight());
  this->m_leftButton->Move(13, 151);
  this->m_leftButton->SetId(0x180);
  this->AddChild(this->m_leftButton);

  this->m_rightButton = new UIBitmapButton();
  this->m_rightButton->SetBitmap("유저인터페이스\\equip_preview\\chr_arrow_rotate_r_out.bmp", 0, 0);
  this->m_rightButton->SetBitmap("유저인터페이스\\equip_preview\\chr_arrow_rotate_r_over.bmp", 1, 0);
  this->m_rightButton->SetBitmap("유저인터페이스\\equip_preview\\chr_arrow_rotate_r_press.bmp", 2, 0);
  this->m_rightButton->SetSize(this->m_rightButton->BitmapWidth(), this->m_rightButton->BitmapHeight());
  this->m_rightButton->Move(this->geometry.width - this->m_rightButton->BitmapWidth() - 13, 151);
  this->m_rightButton->SetId(0x181);
  this->AddChild(this->m_rightButton);

  this->m_actionButton = new UIBasicButton();
  this->m_actionButton->Configure(80, LookupMessage(MSI_BLOCK_CALL_PERMIT));
  this->m_actionButton->Move(static_cast<int>((this->geometry.width - this->m_actionButton->geometry.width) * 0.5), this->geometry.height - 30);
  this->m_actionButton->SetId(0x16B);
  this->m_actionButton->invalidateParent = 1;
  this->AddChild(this->m_actionButton);
}

// OFFSET: 0x007F3CD0
// FUNCTION: fn_09883
void UIPreviewEquipWnd::Paint() {
  this->SetSolid(g_renderer->primitives, this->geometry.width, this->geometry.height, 0);
  this->DrawNamed(g_renderer->primitives, 0, 0, "유저인터페이스\\equip_preview\\bg_mounting2.bmp", 0xffffffff, 0, 0);
  void (UIPreviewEquipWnd::*draw)() = &UIPreviewEquipWnd::DrawCharacter;
  (this->*draw)();
}

// OFFSET: 0x007F3DC0
// FUNCTION: fn_09885
void UIPreviewEquipWnd::SyncFromPlayer() {
  this->m_render->drawShadow = 0;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CPlayer* player = mode->scene->player;
  if (player == nullptr) {
    return;
  }

  this->m_render->job = player->GetEffectiveJob(-1);
  this->m_render->headJob = player->appearanceJob;
  this->m_render->appearance[0] = player->sex;
  this->m_render->appearance[1] = player->head;
  this->m_render->appearance[2] = static_cast<int>(player->render.bodyPalette);
  this->m_render->appearance[3] = player->headPalette;
  this->m_render->lowerHeadgear = player->accessory;
  this->m_render->upperHeadgear = player->accessory2;
  this->m_render->middleHeadgear = player->accessory3;
  this->m_render->SetRobe(player->robe);
  this->m_render->madogearType = player->GetMadogearType();
  this->m_render->riding = player->isRiding;
  this->m_render->bodyCostume = player->GetBodyCostume();
  *reinterpret_cast<float*>(&this->m_render->viewportOffsetX) = -5.0f;
  this->m_render->viewportOffsetY = 136.0f;
  this->RefreshHeadgearLocations();
  this->Invalidate();
}

// OFFSET: 0x007F3EF0
// FUNCTION: fn_09886
int UIPreviewEquipWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case 0x180:
      this->m_render->actionIndex = this->m_render->actionIndex + 1;
      if (this->m_render->actionIndex > 7) {
        this->m_render->actionIndex = 0;
      }
      this->Invalidate();
      break;
    case 0x181:
      this->m_render->actionIndex = this->m_render->actionIndex - 1;
      if (this->m_render->actionIndex < 0) {
        this->m_render->actionIndex = 7;
      }
      this->Invalidate();
      break;
    case ID_RESET:
      this->SyncFromPlayer();
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_PREVIEWEQUIPWND);
      break;
    }
    break;
  case UM_REFRESHITEM:
    if (a != 0) {
      const ITEM_INFO* item = reinterpret_cast<const ITEM_INFO*>(a);
      switch (item->identity[2]) {
      case EQP_HEAD_TOP:
      case EQP_COSTUME_HEAD_TOP:
        this->m_render->upperHeadgear = item->sprite;
        this->m_render->upperHeadgearLocation = 0x100;
        if ((this->m_render->lowerHeadgearLocation & 0x100) != 0) {
          this->m_render->lowerHeadgearLocation = 0;
          this->m_render->lowerHeadgear = 0;
        }
        if ((this->m_render->middleHeadgearLocation & 0x100) != 0) {
          this->m_render->middleHeadgearLocation = 0;
          this->m_render->middleHeadgear = 0;
        }
        break;
      case EQP_HEAD_MID:
      case EQP_COSTUME_HEAD_MID:
        this->m_render->middleHeadgear = item->sprite;
        this->m_render->middleHeadgearLocation = 0x200;
        if ((this->m_render->lowerHeadgearLocation & 0x200) != 0) {
          this->m_render->lowerHeadgearLocation = 0;
          this->m_render->lowerHeadgear = 0;
        }
        if ((this->m_render->upperHeadgearLocation & 0x200) != 0) {
          this->m_render->upperHeadgearLocation = 0;
          this->m_render->upperHeadgear = 0;
        }
        break;
      case 0x300:
      case 0xC00:
        this->m_render->upperHeadgear = item->sprite;
        this->m_render->middleHeadgear = 0;
        this->m_render->upperHeadgearLocation = 0x300;
        this->m_render->middleHeadgearLocation = 0x300;
        if (this->m_render->lowerHeadgearLocation != EQP_HEAD_LOW) {
          this->m_render->lowerHeadgearLocation = 0;
          this->m_render->lowerHeadgear = 0;
        }
        break;
      case 0x301:
      case 0x1C00:
        this->m_render->lowerHeadgear = item->sprite;
        this->m_render->upperHeadgear = 0;
        this->m_render->middleHeadgear = 0;
        this->m_render->lowerHeadgearLocation = 0x301;
        this->m_render->upperHeadgearLocation = 0x301;
        this->m_render->middleHeadgearLocation = 0x301;
        break;
      case 0x201:
      case 0x1800:
        this->m_render->middleHeadgear = item->sprite;
        this->m_render->lowerHeadgear = 0;
        this->m_render->middleHeadgearLocation = 0x201;
        this->m_render->lowerHeadgearLocation = 0x201;
        if (this->m_render->upperHeadgearLocation != EQP_HEAD_TOP) {
          this->m_render->upperHeadgearLocation = 0;
          this->m_render->upperHeadgear = 0;
        }
        break;
      case 0x101:
      case 0x1400:
        this->m_render->upperHeadgear = item->sprite;
        this->m_render->lowerHeadgear = 0;
        this->m_render->upperHeadgearLocation = 0x101;
        this->m_render->lowerHeadgearLocation = 0x101;
        if (this->m_render->middleHeadgearLocation != EQP_HEAD_MID) {
          this->m_render->middleHeadgearLocation = 0;
          this->m_render->middleHeadgear = 0;
        }
        break;
      case EQP_HEAD_LOW:
      case EQP_COSTUME_HEAD_LOW:
        this->m_render->lowerHeadgear = item->sprite;
        this->m_render->lowerHeadgearLocation = 1;
        if ((this->m_render->upperHeadgearLocation & 1) != 0) {
          this->m_render->upperHeadgearLocation = 0;
          this->m_render->upperHeadgear = 0;
        }
        if ((this->m_render->middleHeadgearLocation & 1) != 0) {
          this->m_render->middleHeadgearLocation = 0;
          this->m_render->middleHeadgear = 0;
        }
        break;
      case EQP_GARMENT:
      case EQP_COSTUME_GARMENT:
        this->m_render->SetRobe(item->sprite);
        break;
      }
    }
    break;
  case UM_SETINFO:
    this->saved = reinterpret_cast<PreviewEquipWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->Move(this->saved->x, this->saved->y);
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x007F43E0
// FUNCTION: fn_09887
void UIPreviewEquipWnd::RefreshHeadgearLocations() {
  if (this->m_render->lowerHeadgear) {
    ITEM_INFO equipped = g_session.GetEquippedItemAt(0);
    if (this->m_render->lowerHeadgear == equipped.sprite) {
      this->m_render->lowerHeadgearLocation = 1;
      if (equipped.identity[2] == 0x301) {
        this->m_render->lowerHeadgearLocation = 0x301;
        this->m_render->upperHeadgearLocation = 0x301;
        this->m_render->middleHeadgearLocation = 0x301;
      }
    }

    ITEM_INFO costume = g_session.GetCostumeItemAt(0);
    if (this->m_render->lowerHeadgear == costume.sprite) {
      this->m_render->lowerHeadgearLocation = 1;
      if (costume.identity[2] == 0x1C00) {
        this->m_render->lowerHeadgearLocation = 0x301;
        this->m_render->upperHeadgearLocation = 0x301;
        this->m_render->middleHeadgearLocation = 0x301;
      }
    }
  }

  if (this->m_render->upperHeadgear) {
    ITEM_INFO equipped = g_session.GetEquippedItemAt(8);
    if (this->m_render->upperHeadgear == equipped.sprite) {
      if (equipped.identity[2] == 0x100) {
        this->m_render->upperHeadgearLocation = 0x100;
      }

      if (equipped.identity[2] == 0x101) {
        this->m_render->upperHeadgearLocation = 0x101;
        this->m_render->lowerHeadgearLocation = 0x101;
      }

      if (equipped.identity[2] == 0x300) {
        this->m_render->upperHeadgearLocation = 0x300;
        this->m_render->middleHeadgearLocation = 0x300;
      }

      if (equipped.identity[2] == 0x301) {
        this->m_render->lowerHeadgearLocation = 0x301;
        this->m_render->upperHeadgearLocation = 0x301;
        this->m_render->middleHeadgearLocation = 0x301;
      }
    }

    ITEM_INFO costume = g_session.GetCostumeItemAt(8);
    if (this->m_render->upperHeadgear == costume.sprite) {
      if (costume.identity[2] == 0x400) {
        this->m_render->upperHeadgearLocation = 0x100;
      }

      if (costume.identity[2] == 0x1400) {
        this->m_render->upperHeadgearLocation = 0x101;
        this->m_render->lowerHeadgearLocation = 0x101;
      }

      if (costume.identity[2] == 0xC00) {
        this->m_render->upperHeadgearLocation = 0x300;
        this->m_render->middleHeadgearLocation = 0x300;
      }

      if (costume.identity[2] == 0x1C00) {
        this->m_render->lowerHeadgearLocation = 0x301;
        this->m_render->upperHeadgearLocation = 0x301;
        this->m_render->middleHeadgearLocation = 0x301;
      }
    }
  }

  if (this->m_render->middleHeadgear) {
    ITEM_INFO equipped = g_session.GetEquippedItemAt(9);
    if (this->m_render->middleHeadgear == equipped.sprite) {
      if (equipped.identity[2] == 0x200) {
        this->m_render->middleHeadgearLocation = 0x200;
      }

      if (equipped.identity[2] == 0x201) {
        this->m_render->middleHeadgearLocation = 0x201;
        this->m_render->lowerHeadgearLocation = 0x201;
      }
    }

    ITEM_INFO costume = g_session.GetCostumeItemAt(9);
    if (this->m_render->middleHeadgear == costume.sprite) {
      if (costume.identity[2] == 0x800) {
        this->m_render->middleHeadgearLocation = 0x200;
      }

      if (costume.identity[2] == 0x1800) {
        this->m_render->middleHeadgearLocation = 0x201;
        this->m_render->lowerHeadgearLocation = 0x201;
      }
    }
  }
}

// OFFSET: 0x007F4830
// FUNCTION: fn_09888
void UIPreviewEquipWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007F3DB0
// FUNCTION: fn_09884
void UIPreviewEquipWnd::OnProcess() {
  this->Invalidate();
}
