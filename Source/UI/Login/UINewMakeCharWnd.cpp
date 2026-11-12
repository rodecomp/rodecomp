#include "Render/DeviceContext/DCSurface.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Login/UINewMakeCharWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Sprites/ImfResource.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Login/UINewSelectCharWnd.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <cstring>
#include <new>
#include <string>
#include <timeapi.h>

// GLOBAL: gl_25613
static const std::string s_makeCharItemLinkOpen = "<ITEML>";

// GLOBAL: gl_25614
static const std::string s_makeCharItemLinkClose = "</ITEML>";

// OFFSET: 0x007D9960
// FUNCTION: fn_09607
UIHairBitmap::UIHairBitmap() : UIBitmapButton() {
}

// OFFSET: 0x007D9A50
// FUNCTION: fn_09609
UINewMakeCharWnd::UINewMakeCharWnd()
    : UIFrameWnd(0),
      nameEditStorage(0),
      m_race(0),
      m_raceViews() {
  this->geometry.animationFrame = 0;

  const int* preset = reinterpret_cast<const int*>(g_modeMgr.gameMode->SendMsg(LMM_GET_CHAR_PARAM, 0, 0, 0, 0));
  this->m_stats[0] = static_cast<unsigned char>(preset[0]);
  this->m_stats[1] = static_cast<unsigned char>(preset[1]);
  this->m_stats[2] = static_cast<unsigned char>(preset[2]);
  this->m_stats[3] = static_cast<unsigned char>(preset[3]);
  this->m_stats[4] = static_cast<unsigned char>(preset[4]);
  this->m_stats[5] = static_cast<unsigned char>(preset[5]);
  this->m_headPalette = static_cast<short>(preset[6]);
  this->m_head = static_cast<short>(preset[7]);

  this->m_name[0] = 0;
  this->m_job = 0;
  this->m_level = 1;
  this->m_skillPoint = 1;
  this->m_status.jobPoint = 0;
  this->m_status.hp = 100;
  this->m_status.maxHp = 100;
  this->m_status.sp = 100;
  this->m_status.maxSp = 100;
  this->m_status.exp = 0;
  this->m_slot = g_selectedSlotIndex;
  this->m_headPalette = 0;
  this->m_bodyPalette = 0;
  this->m_shield = 0;
  this->accessory2 = 0;
  this->accessory3 = 0;
  this->m_costumeJob = 0;
  this->m_sex = static_cast<unsigned char>(g_session.m_sex);
  this->m_viewX = 500;
  this->m_viewY = 228;
  this->actionBase = 0;
  this->m_action = 0;
  this->m_frame = 0;

  this->m_raceActions[0].assign(static_cast<const std::string&>(g_session.GetBodyActionResourceName(this->m_job, this->m_sex, 0, 0)));
  this->m_raceSprites[0].assign(static_cast<const std::string&>(g_session.GetBodySprName(this->m_job, this->m_sex, 0, 0)));

  if (!g_session.IsDoramJob(this->m_job) && this->m_sex == 1 && 13 == this->m_head) {
    this->m_head = 14;
  }

  int head = this->m_head;
  this->m_raceActions[1].assign(static_cast<const std::string&>(g_session.GetHeadActName(this->m_job, this->m_sex, &head)));
  this->m_raceSprites[1].assign(static_cast<const std::string&>(g_session.GetHeadSprName(this->m_job, this->m_sex, &head)));

  const int bodyPalette = this->m_bodyPalette;
  this->m_head = static_cast<short>(head);
  this->m_racePaletteEnabled[1] = bodyPalette;
  if (bodyPalette > 0) {
    this->m_racePalettes[1].assign(static_cast<const std::string&>(g_session.GetBodyPalName(this->m_job, this->m_sex, bodyPalette, 0, 0, 0)));
  }

  const int headPalette = this->m_headPalette;
  this->m_racePaletteEnabled[0] = headPalette;
  if (headPalette > 0) {
    this->m_racePalettes[0].assign(static_cast<const std::string&>(g_session.GetHeadPalName(head, this->m_job, this->m_sex, headPalette)));
  }

  this->m_raceOffsets = g_session.GetBodyImfName(this->m_job, this->m_sex, this->m_head, this->m_costumeJob);

  this->previewState = 0;
  this->m_startTime = timeGetTime();
  this->previewCounter = 0;
  this->previewPhase = 0;
  this->hairStyleButtonStorage[0] = 0;
  this->hairStyleButtonStorage[1] = 0;
  this->hairStyleButtonStorage[2] = 0;
  this->hairStyleButtonStorage[3] = 0;
  this->hairStyleButtonStorage[4] = 0;
  this->hairStyleButtonStorage[5] = 0;
  this->hairColorButtonStorage[0] = 0;
  this->hairColorButtonStorage[1] = 0;
  this->hairColorButtonByte = 0;
  this->genderButtonStorage = 0;
}

// OFFSET: 0x007DB320
// FUNCTION: fn_09617
void UIHairBitmap::OnLBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDown(x, y);
  }
}

// OFFSET: 0x007DB300
// FUNCTION: fn_09616
void UIHairBitmap::OnLBtnDblClk(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDblClk(x, y);
  }
}

// OFFSET: 0x007DB380
// FUNCTION: fn_09620
void UIHairBitmap::OnMouseMove(int x, int y) {
  if (this->parent) {
    this->parent->OnMouseMove(x, y);
  }
}

// OFFSET: 0x007DB360
// FUNCTION: fn_09619
void UIHairBitmap::OnMouseHover(int x, int y) {
  if (this->parent) {
    this->parent->OnMouseHover(x, y);
  }
}

// OFFSET: 0x007DB3A0
// FUNCTION: fn_09621
void UIHairBitmap::OnMouseCheck(int x, int y) {
  if (this->parent) {
    this->parent->OnMouseCheck(x, y);
  }
}

// OFFSET: 0x007DB3C0
// FUNCTION: fn_09622
void UIHairBitmap::ResetCursor(int x, int y) {
  if (this->parent) {
    this->parent->ResetCursor(x, y);
  }
}

// OFFSET: 0x007DB340
// FUNCTION: fn_09618
void UIHairBitmap::OnLBtnUp(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnUp(x, y);
  }
}

// OFFSET: 0x007DB030
// FUNCTION: fn_09615
void UINewMakeCharWnd::Paint() {
  CBitmapRes* background =
    static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\make_character_ver2\\bg_makeBg.bmp")));
  this->DrawBitmap(0, 0, background, false);
  this->DrawTextAt(0x2C, 0x11, LookupMessage(MSI_STOP_RECRUITING), 0, 0, 0xC, 0x5F5F5F, 1, 0);

  for (int i = 0; i < 2; ++i) {
    CImfRes* offsets = static_cast<CImfRes*>(ResourceMgr::Instance().Load(this->m_raceOffsets.c_str()));
    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_raceActions[i].c_str()));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->m_raceSprites[i].c_str()));
    const ActionFrame& frame = action->Frame(this->m_action, this->m_frame);
    POINT offset = offsets->GetLongPoint(i, this->m_action, this->m_frame);

    if (i == 1) {
      CActRes* first = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_raceActions[0].c_str()));
      const ActionFrame& firstFrame = first->Frame(this->m_action, this->m_frame);
      if (frame.anchorCount != 0 && firstFrame.anchorCount != 0 && frame.anchors[0].attachmentId == firstFrame.anchors[0].attachmentId) {
        offset.x += firstFrame.anchors[0].x - frame.anchors[0].x;
        offset.y += firstFrame.anchors[0].y - frame.anchors[0].y;
      }
    }

    const std::uint32_t* palette = sprite->palette;
    switch (i) {
    case 0:
      if (this->m_racePaletteEnabled[1] > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_racePalettes[1].c_str()))->palette;
      }
      break;
    case 1:
      if (this->m_racePaletteEnabled[0] > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_racePalettes[0].c_str()))->palette;
      }
      break;
    }

    const SpriteFrameCell& cell = frame.Cell(i);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    SpriteAtlasRecord record{};
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    }

    this->geometry.surface->DrawSurface(
      this->m_viewX + offset.x + cell.x, this->m_viewY + offset.y + cell.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->doubleWidth + 1, image->doubleHeight + 1, 0
    );
  }
}

// OFFSET: 0x007DB3E0
// FUNCTION: fn_09623
void UINewMakeCharWnd::OnProcess() {
  this->m_raceViews[this->m_race]->Invalidate();
}

// OFFSET: 0x007DB400
// FUNCTION: fn_09624
int UIHairStyleButton::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_SETSTATE: {
    const bool previous = this->m_selected;
    this->m_selected = argument1 != 0;
    if (previous != this->m_selected) {
      if (argument1) {
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 0, 0);
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 1, 0);
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 2, 0);
      } else {
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_normal.bmp", 0, 0);
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_over.bmp", 1, 0);
      }
      this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 2, 0);
      this->Invalidate();
    }
    break;
  }
  case UM_SETCURITEM: {
    char name[128];
    memset(name, 0, sizeof(name));
    if (this->m_race == 0) {
      if (this->m_style > 22) {
        if (this->m_preview) {
          this->m_preview->Enable(0);
          this->InvalidateChildren();
        }
        return 0;
      }
      this->m_preview->Enable(1);
      sprintf_s(
        name, this->m_sex != 0 ? "유저인터페이스\\make_character_ver2\\img_hairStyle%02d.bmp" : "유저인터페이스\\make_character_ver2\\img_hairStyle_girl%02d.bmp",
        this->m_style + 1
      );
    } else if (this->m_race == 1) {
      if (this->m_style > 5) {
        if (this->m_preview) {
          this->m_preview->Enable(0);
          this->InvalidateChildren();
        }
        return 0;
      }
      this->m_preview->Enable(1);
      if (this->m_sex == 0) {
        sprintf_s(name, "유저인터페이스\\make_character_ver2\\img_hairStyle_doramGirl%02d.bmp", this->m_style + 1);
      } else {
        sprintf_s(name, "유저인터페이스\\make_character_ver2\\img_hairStyle_doramBoy%02d.bmp", this->m_style + 1);
      }
    }
    if (this->m_preview) {
      this->m_preview->SetBitmap(name, 0, 0);
      this->m_preview->SetBitmap(name, 1, 0);
      this->m_preview->SetBitmap(name, 2, 0);
      this->m_preview->SetBitmap("유저인터페이스\\make_character_ver2\\img_hairStyle_none.bmp", 3, 0);
    }
    this->InvalidateChildren();
    break;
  }
  default:
    UIBitmapButton::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    break;
  }
  return 0;
}

// OFFSET: 0x007DA290
// FUNCTION: fn_09613
void UIHairStyleButton::OnCreate(int width, int height) {
  this->m_preview = new (std::nothrow) UIHairBitmap();
  this->m_preview->SetSize(36, 37);
  this->AddChild(this->m_preview);
  this->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
}

// OFFSET: 0x007DBFD0
// FUNCTION: fn_09626
void UINewMakeCharWnd::UpdatePreviewResources() {
  this->m_raceActions[0] = static_cast<const std::string&>(g_session.GetBodyActionResourceName(this->m_job, this->m_sex, this->m_job, 0));
  this->m_raceSprites[0] = static_cast<const std::string&>(g_session.GetBodySprName(this->m_job, this->m_sex, this->m_job, 0));
  int head = this->m_head;
  this->m_raceActions[1] = static_cast<const std::string&>(g_session.GetHeadActName(this->m_job, this->m_sex, &head));
  this->m_raceSprites[1] = static_cast<const std::string&>(g_session.GetHeadSprName(this->m_job, this->m_sex, &head));

  const int bodyPalette = this->m_bodyPalette;
  this->m_head = static_cast<short>(head);
  this->m_racePaletteEnabled[1] = bodyPalette;
  if (bodyPalette > 0) {
    this->m_racePalettes[1] = static_cast<const std::string&>(g_session.GetBodyPalName(this->m_job, this->m_sex, bodyPalette, this->m_job, 0, 0));
  }

  const int headPalette = this->m_headPalette;
  this->m_racePaletteEnabled[0] = headPalette;
  if (headPalette > 0) {
    this->m_racePalettes[0] = static_cast<const std::string&>(g_session.GetHeadPalName(head, this->m_job, this->m_sex, headPalette));
  }

  this->m_raceOffsets = g_session.GetBodyImfName(this->m_job, this->m_sex, this->m_head, this->m_costumeJob);
}

// OFFSET: 0x007DA340
// FUNCTION: fn_09614
void UINewMakeCharWnd::OnCreate(int width, int height) {
  UIBitmapButton* closeButton = new UIBitmapButton();
  closeButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_close_normal.bmp", 0, 0);
  closeButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_close_over.bmp", 1, 0);
  closeButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_close_press.bmp", 2, 0);
  closeButton->SetSize(closeButton->BitmapWidth(), closeButton->BitmapHeight());
  closeButton->Move(0x300, 9);
  closeButton->SetId(0xB9);
  this->AddChild(closeButton);

  UIBmpTextButton* makeButton = new UIBmpTextButton();
  makeButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_make_normal.bmp", 0, 0);
  makeButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_make_over.bmp", 1, 0);
  makeButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_make_press.bmp", 2, 0);
  makeButton->SetSize(makeButton->BitmapWidth(), makeButton->BitmapHeight());
  makeButton->Move(0x268, 0x17B);
  {
    std::string text = LookupMessage(MSI_MAKECHARWND_DOUBLECHECK_NAME);
    makeButton->text = text;
    makeButton->Invalidate();
  }
  makeButton->AddTextStyle(0, 0xE, 0x313131, 1, 1, -1000, -1000);
  makeButton->AddTextStyle(1, 0xE, 0x313131, 1, 1, -1000, -1000);
  makeButton->AddTextStyle(2, 0xE, 0x313131, 1, 1, -1000, -1000);
  makeButton->SetId(0xB8);
  this->AddChild(makeButton);

  UIBmpTextButton* cancelButton = new UIBmpTextButton();
  cancelButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_make_normal.bmp", 0, 0);
  cancelButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_make_over.bmp", 1, 0);
  cancelButton->SetBitmap("유저인터페이스\\make_character_ver2\\bt_make_press.bmp", 2, 0);
  cancelButton->SetSize(cancelButton->BitmapWidth(), cancelButton->BitmapHeight());
  cancelButton->Move(0x19, 0x17B);
  {
    std::string text = LookupMessage(MSI_PRIVATE_AIRPLANE_FAILED_DEAD_STATE);
    cancelButton->text = text;
    cancelButton->Invalidate();
  }
  cancelButton->AddTextStyle(0, 0xE, 0x313131, 1, 1, -1000, -1000);
  cancelButton->AddTextStyle(1, 0xE, 0x313131, 1, 1, -1000, -1000);
  cancelButton->AddTextStyle(2, 0xE, 0x313131, 1, 1, -1000, -1000);
  cancelButton->SetId(0xB9);
  this->AddChild(cancelButton);

  this->m_nameEdit = new UIEditCtrl2();
  this->m_nameEdit->SetSize(0x82, 0x14);
  this->m_nameEdit->Move(0x1B3, 0x10C);
  this->m_nameEdit->maximum = 0x17;
  this->AddChild(this->m_nameEdit);

  UIDescriptRace* human = new UIDescriptRace(0);
  human->SetBitmap("유저인터페이스\\make_character_ver2\\img_human_off.bmp", 0, 0);
  human->SetBitmap("유저인터페이스\\make_character_ver2\\img_human_on.bmp", 1, 0);
  human->SetBitmap("유저인터페이스\\make_character_ver2\\img_human_on.bmp", 2, 0);
  human->SetSize(human->BitmapWidth(), human->BitmapHeight());
  human->Move(0x15, 0x24);
  this->m_raceViews[0] = human;
  this->AddChild(human);

  UIDescriptRace* doram = new UIDescriptRace(1);
  const char* doramNormal;
  const char* doramActive;
  if (!g_doramCreationEnabled) {
    doramActive = "유저인터페이스\\make_character_ver2\\img_doram_comingSoon.bmp";
    doramNormal = doramActive;
  } else {
    doramActive = "유저인터페이스\\make_character_ver2\\img_doram_on.bmp";
    doramNormal = "유저인터페이스\\make_character_ver2\\img_doram_off.bmp";
  }
  doram->SetBitmap(doramNormal, 0, 0);
  doram->SetBitmap(doramActive, 1, 0);
  doram->SetBitmap(doramActive, 2, 0);
  doram->SetSize(doram->BitmapWidth(), doram->BitmapHeight());
  doram->Move(0x15, 0xCA);
  this->m_raceViews[1] = doram;
  this->AddChild(doram);

  this->m_genderButtons[1] = new UIBitmapButton();
  this->m_genderButtons[1]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_male_off.bmp", 0, 0);
  this->m_genderButtons[1]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_male_over.bmp", 1, 0);
  this->m_genderButtons[1]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_male_press.bmp", 2, 0);
  this->m_genderButtons[1]->SetSize(this->m_genderButtons[1]->BitmapWidth(), this->m_genderButtons[1]->BitmapHeight());
  this->m_genderButtons[1]->Move(0x1B5, 0x3B);
  this->m_genderButtons[1]->SetId(0x20D);
  this->AddChild(this->m_genderButtons[1]);

  this->m_genderButtons[0] = new UIBitmapButton();
  this->m_genderButtons[0]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_female_off.bmp", 0, 0);
  this->m_genderButtons[0]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_female_over.bmp", 1, 0);
  this->m_genderButtons[0]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_female_press.bmp", 2, 0);
  this->m_genderButtons[0]->SetSize(this->m_genderButtons[0]->BitmapWidth(), this->m_genderButtons[0]->BitmapHeight());
  this->m_genderButtons[0]->Move(500, 0x3B);
  this->m_genderButtons[0]->SetId(0x20D);
  this->AddChild(this->m_genderButtons[0]);

  if (this->m_sex == 1) {
    this->m_genderButtons[1]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_male_on.bmp", 0, 0);
  } else if (this->m_sex == 0) {
    this->m_genderButtons[0]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_female_on.bmp", 0, 0);
  }

  UIBitmapButton* leftTurn = new UIBitmapButton();
  leftTurn->SetBitmap("유저인터페이스\\make_character_ver2\\bt_leftTurn_normal.bmp", 0, 0);
  leftTurn->SetBitmap("유저인터페이스\\make_character_ver2\\bt_leftTurn_over.bmp", 1, 0);
  leftTurn->SetBitmap("유저인터페이스\\make_character_ver2\\bt_leftTurn_press.bmp", 2, 0);
  leftTurn->SetSize(leftTurn->BitmapWidth(), leftTurn->BitmapHeight());
  leftTurn->Move(0x1AA, 0xB3);
  leftTurn->SetId(0xE3);
  leftTurn->m_argument = 0;
  leftTurn->SetTooltip(LookupMessage(MSI_VIEW_BEFORE_WORLDINFO_TITLE));
  this->AddChild(leftTurn);

  UIBitmapButton* rightTurn = new UIBitmapButton();
  rightTurn->SetBitmap("유저인터페이스\\make_character_ver2\\bt_rightTurn_normal.bmp", 0, 0);
  rightTurn->SetBitmap("유저인터페이스\\make_character_ver2\\bt_rightTurn_over.bmp", 1, 0);
  rightTurn->SetBitmap("유저인터페이스\\make_character_ver2\\bt_rightTurn_press.bmp", 2, 0);
  rightTurn->SetSize(rightTurn->BitmapWidth(), rightTurn->BitmapHeight());
  rightTurn->Move(0x227, 0xB3);
  rightTurn->SetId(0xE2);
  rightTurn->m_argument = 0;
  rightTurn->SetTooltip(LookupMessage(MSI_VIEW_BEFORE_SERVERINFO));
  this->AddChild(rightTurn);

  UIStaticText* hairStyleTitle = new UIStaticText();
  hairStyleTitle->SetSize(100, 0xF);
  hairStyleTitle->SetText(LookupMessage(MSI_DATE_HH_MM_SS), 0);
  hairStyleTitle->SetColor(0x40, 0x40, 0x40);
  hairStyleTitle->SetFont(0, 0xB, 1, 0);
  hairStyleTitle->Move(0x261, 0x2E);
  this->AddChild(hairStyleTitle);

  for (int index = 0; index < 24; ++index) {
    this->m_hairStyleButtons[index] = new UIHairStyleButton(index, this->m_sex);
    this->m_hairStyleButtons[index]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_normal.bmp", 0, 0);
    this->m_hairStyleButtons[index]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_over.bmp", 1, 0);
    this->m_hairStyleButtons[index]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 2, 0);
    this->m_hairStyleButtons[index]->SetSize(0x24, 0x25);
    this->m_hairStyleButtons[index]->SetId(0x20C);
    this->m_hairStyleButtons[index]->Move((index % 4) * 0x26 + 0x25E, (index / 4) * 0x25 + 0x3B);
    this->AddChild(this->m_hairStyleButtons[index]);
    if (index == 0) {
      this->m_hairStyleButtons[0]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
    }
  }

  UIStaticText* hairColorTitle = new UIStaticText();
  hairColorTitle->SetSize(100, 0xF);
  hairColorTitle->SetText(LookupMessage(MSI_INPUT_TIME_EXCEEDED_SIGN_IN_AGAIN), 0);
  hairColorTitle->SetColor(0x40, 0x40, 0x40);
  hairColorTitle->SetFont(0, 0xB, 1, 0);
  hairColorTitle->Move(0x261, 0x11F);
  this->AddChild(hairColorTitle);

  char name[128];
  memset(name, 0, sizeof(name));
  for (int index = 0; index < 9; ++index) {
    UIBmpRadioBtn* button = new UIBmpRadioBtn();
    memset(name, 0, 0x40);
    sprintf(name, "유저인터페이스\\make_character_ver2\\color%02d_off.bmp", index + 1);
    button->SetBitmap(name, 0);
    memset(name, 0, 0x40);
    sprintf(name, "유저인터페이스\\make_character_ver2\\color%02d_over.bmp", index + 1);
    button->SetBitmap(name, 1);
    memset(name, 0, 0x40);
    sprintf(name, "유저인터페이스\\make_character_ver2\\color%02d_on.bmp", index + 1);
    button->SetBitmap(name, 2);
    if (index == 0) {
      button->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
    }
    button->m_value = 0x1EF;
    button->Move((index % 5) * 0x1B + 0x26B, (index / 5) * 0x18 + 0x132);
    this->m_hairColorButtons[index] = button;
    this->AddChild(button);
  }

  g_windowMgr.SetFocus(this->m_nameEdit);
  this->m_message0 = 0xB8;
}

// OFFSET: 0x007DB630
// FUNCTION: fn_09625
int UINewMakeCharWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_DEFPUSH: {
    char name[25] = {};
    strcpy(name, this->m_nameEdit->Text());
    memcpy(this->m_name, name, sizeof(this->m_name));
    this->SendMsg(nullptr, UM_COMMAND, this->m_message0, 0, 0, 0);
    return 0;
  }
  case UM_COMMAND:
    break;
  case UM_SETSTATE:
    this->m_race = static_cast<int>(argument1);
    [[fallthrough]];
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_OK: {
    for (int index = 0; index < 2; ++index) {
      if ((g_doramCreationEnabled || index != 1) && sender == this->m_raceViews[index]) {
        return 0;
      }
    }

    if (this->m_nameEdit->Length() < 4) {
      g_windowMgr.Confirm(LookupMessage(MSI_ENTER_NAME_MORE_THAN_4_CHAR), 0, true, false, 0, "Message", 280, 120, 0);
      return 0;
    }

    if (_mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_makeCharItemLinkOpen.c_str())) != nullptr ||
        _mbsstr(reinterpret_cast<const unsigned char*>(this->m_nameEdit->Text()), reinterpret_cast<const unsigned char*>(s_makeCharItemLinkClose.c_str())) != nullptr) {
      g_windowMgr.Confirm(LookupMessage(0xAFC), 0, true, false, 0, "Message", 280, 120, 0);
      return 0;
    }

    int valid = 1;
    if (g_serviceType == 1 && !ValidateCharacterName(this->m_nameEdit->Text())) {
      valid = 0;
    }
    int allowed = g_forbiddenWords.Allows(this->m_nameEdit->Text());
    bool hasControlCharacter = g_session.HasControlCharacter(this->m_nameEdit->Text());
    int blank = g_session.IsBlankString(this->m_nameEdit->Text());

    if (!hasControlCharacter && allowed == 1 && valid == 1 && blank == 0) {
      if (this->m_nameEdit->Length() >= 24) {
        g_windowMgr.Confirm(LookupMessage(MSI_NAMELENGTH_TOO_LONG), 0, true, false, 0, "Message", 280, 120, 0);
        this->m_nameEdit->SetText("");
        return 0;
      }

      char name[24] = {};
      strcpy(name, this->m_nameEdit->Text());
      memcpy(this->m_name, name, sizeof(this->m_name));
      memcpy_s(&g_charInfo, sizeof(g_charInfo), this->characterData, sizeof(g_charInfo));
      if (strstr(g_charInfo.name, "\xB1\xD7\xB6\xF3\xBA\xF1\xC6\xBC") != nullptr) {
        return 0;
      }
      _strlwr(name);
      if (strstr(name, "gravity") != nullptr) {
        return 0;
      }
      if ((name[0] == '[' && name[1] == 'g' && name[2] == 'm') || (name[1] == '[' && name[2] == 'g' && name[3] == 'm') ||
          (name[2] == '[' && name[3] == 'g' && name[4] == 'm')) {
        return 0;
      }

      g_modeMgr.gameMode->SendMsg(LMM_SENDCHARINFO, 0, 0, 0, 0);
      g_windowMgr.Schedule(this);
    } else {
      if (hasControlCharacter || allowed == 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_BAD_NAME), 0, true, false, 0, "Message", 280, 120, 0);
      }
      if (valid == 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_ENGLISHONLY), 0, true, false, 0, "Message", 280, 120, 0);
      }
      if (blank == 1) {
        g_windowMgr.Confirm(LookupMessage(MSI_NO_SPACE_IN_NAME), 0, true, false, 0, "Message", 280, 120, 0);
      }
    }

    CMode* mode = g_modeMgr.gameMode;
    mode->SendMsg(LMM_SET_MAKING_CHAR_NAME, reinterpret_cast<int>(this->m_nameEdit->Text()), 0, 0, 0);
    int payload[8];
    payload[0] = this->m_stats[0];
    payload[1] = this->m_stats[1];
    payload[2] = this->m_stats[2];
    payload[3] = this->m_stats[3];
    payload[4] = this->m_stats[4];
    payload[5] = this->m_stats[5];
    payload[6] = this->m_headPalette;
    payload[7] = this->m_head;
    mode->SendMsg(LMM_SET_CHAR_PARAM, reinterpret_cast<int>(payload), 0, 0, 0);
    return 0;
  }

  case ID_CANCEL:
    g_windowMgr.Schedule(this);
    g_modeMgr.gameMode->SendMsg(MM_COMMAND, 0x2714, 0, 0, 0);
    return 0;

  case ID_SLOT:
    if (g_doramCreationEnabled || sender != this->m_raceViews[1]) {
      for (int index = 0; index < 2; ++index) {
        if (sender == this->m_raceViews[index]) {
          this->m_race = index;
          this->m_raceViews[index]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
        } else {
          this->m_raceViews[index]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        }
        this->m_raceViews[index]->Invalidate();
      }

      if (sender == this->m_raceViews[this->m_race]) {
        this->m_head = 1;
        for (int index = 0; index < 24; ++index) {
          this->m_hairStyleButtons[index]->SendMsg(this, UM_SETSTATE, index == 0, 0, 0, 0);
          this->m_hairStyleButtons[index]->m_race = this->m_race;
          this->m_hairStyleButtons[index]->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
        }

        if (this->m_race == 0) {
          this->m_job = 0;
        } else if (this->m_race == 1) {
          this->m_job = 0x107A;
        }
        this->UpdatePreviewResources();
      }
    }
    return 0;

  case ID_RADIOBTN:
    if (argument2 == 0x1EF) {
      for (int index = 0; index < 9; ++index) {
        if (sender == this->m_hairColorButtons[index]) {
          this->m_hairColorButtons[index]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
          this->m_headPalette = static_cast<short>(index);
          if (this->m_headPalette < 0) {
            this->m_headPalette = 8;
          }
          this->UpdatePreviewResources();
        } else {
          this->m_hairColorButtons[index]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
        }
      }
    }
    return 0;

  case ID_RIGHT:
    if (argument2 == 0) {
      --this->m_action;
      if (this->m_action < this->actionBase) {
        this->m_action = this->actionBase + 7;
      }
      this->Invalidate();
    }
    return 0;

  case ID_LEFT:
    if (argument2 == 0) {
      ++this->m_action;
      if (this->m_action >= this->actionBase + 8) {
        this->m_action = this->actionBase;
      }
      this->Invalidate();
    }
    return 0;

  case 0x20C:
    for (int index = 0; index < 24; ++index) {
      if (sender == this->m_hairStyleButtons[index]) {
        if (index >= (g_session.IsDoramJob(this->m_job) == true ? 6 : 0x17)) {
          return 0;
        }
        this->m_hairStyleButtons[index]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
        this->m_head = static_cast<short>(index + 1);
        if (this->m_head < 0) {
          this->m_head = 0;
        }
        this->UpdatePreviewResources();
      } else {
        this->m_hairStyleButtons[index]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
      }
    }
    return 0;

  case 0x20D:
    for (int index = 0; index < 2; ++index) {
      if (sender == this->m_genderButtons[index]) {
        if (index == this->m_sex) {
          return 0;
        }
        this->m_sex = static_cast<unsigned char>(index);
        if (index == 1) {
          this->m_genderButtons[1]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_male_on.bmp", 0, 0);
        } else if (index == 0) {
          this->m_genderButtons[0]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_female_on.bmp", 0, 0);
        }
      } else if (index == 1) {
        this->m_genderButtons[index]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_male_off.bmp", 0, 0);
      } else if (this->m_sex == 0) {
        this->m_genderButtons[index]->SetBitmap("유저인터페이스\\make_character_ver2\\bt_female_off.bmp", 0, 0);
      }
      this->m_genderButtons[index]->Invalidate();
    }

    for (int index = 0; index < 24; ++index) {
      this->m_hairStyleButtons[index]->m_sex = this->m_sex;
      this->m_hairStyleButtons[index]->SendMsg(nullptr, UM_SETCURITEM, 0, 0, 0, 0);
    }
    this->UpdatePreviewResources();
    return 0;
  }

  return 0;
}
