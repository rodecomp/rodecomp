#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UISelCharForUServerWnd.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "UI/Text/UICharInfoBalloonText.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <timeapi.h>

extern const char* LookupMessage(int id);


#pragma pack(push, 1)
struct PACKET_CZ_SELECT_CHARACTERS {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int accountId;
  /* 0006 */ int count;
  /* 000A */ int characterIds[9];
};
#pragma pack(pop)

// OFFSET: 0x009120E0
// FUNCTION: fn_12526
UISelCharForUServerWnd::UISelCharForUServerWnd()
    : UIFrameWnd(0), m_columns(6) {
  if (g_session.characterCount > 12) {
    this->m_columns = 9;
  }

  this->characterInfoBalloon = 0;
  this->m_checkedCount = 0;
  this->geometry.animationFrame = 0;
  this->confirmButton = 0;
  for (int index = 0; index < g_session.characterCount; ++index) {
    this->m_slotEnabled[index] = 1;
  }

  this->m_startTime = timeGetTime();
}

// OFFSET: 0x00928A60
// FUNCTION: fn_12652
void UISelCharForUServerWnd::OnCreate(int, int) {
  {
    std::string path = "유저인터페이스\\";
    this->confirmButton = new UIBitmapButton();
    this->confirmButton->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
    this->confirmButton->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    this->confirmButton->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    this->confirmButton->SetSize(this->confirmButton->BitmapWidth(), this->confirmButton->BitmapHeight());
    this->confirmButton->Move(this->geometry.width - 46, this->geometry.height - 24);
    this->confirmButton->SetId(184);
    this->AddChild(this->confirmButton);
  }

  this->m_message0 = 184;

  for (int index = 0; index < g_session.characterCount; ++index) {
    this->m_checkBoxes[index] = new UICheckBox();
    this->m_checkBoxes[index]->SetSize(10, 10);
    static_cast<UICheckBox*>(this->m_checkBoxes[index])->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", "");
    this->m_checkBoxes[index]->Move(-100, -100);
    this->AddChild(this->m_checkBoxes[index]);
  }

  for (int index = 0; index < g_session.characterCount; ++index) {
    CharacterInfo* info = reinterpret_cast<CharacterInfo*>(g_modeMgr.gameMode->SendMsg(MM_QUERYCHARICTORINFO, index, 0, 0, 0));
    if (!info) {
      continue;
    }

    this->m_checkBoxes[index]->Move(index % this->m_columns * 81 + 63, index / this->m_columns * 149 + 77);

    const int slotIndex = info->slot;
    this->m_slots[slotIndex].job = info->job;
    this->m_slots[slotIndex].robe = info->robe;
    this->m_slotEnabled[slotIndex] = 0;
    this->m_slots[slotIndex].x = slotIndex % this->m_columns * 81 + 68;
    this->m_slots[slotIndex].y = slotIndex / this->m_columns * 149 + 191;
    this->m_slots[slotIndex].actionBase = 0;
    this->m_slots[slotIndex].action = 0;
    this->m_slots[slotIndex].frame = 0;

    this->m_slots[slotIndex].actionNames[0] = static_cast<const std::string&>(GetBodyActionResourceName(info->job, g_session.GetCurrentSex(), info->bodyCostume, 0));
    this->m_slots[slotIndex].spriteNames[0] = static_cast<const std::string&>(g_session.GetBodySprName(info->job, g_session.GetCurrentSex(), info->bodyCostume, 0));

    int head = info->head;
    this->m_slots[slotIndex].actionNames[1] = static_cast<const std::string&>(g_session.GetHeadActName(info->job, g_session.GetCurrentSex(), &head));
    this->m_slots[slotIndex].spriteNames[1] = static_cast<const std::string&>(g_session.GetHeadSprName(info->job, g_session.GetCurrentSex(), &head));
    this->m_slots[slotIndex].actionNames[2] = static_cast<const std::string&>(g_session.GetAccessoryActName(info->job, g_session.GetCurrentSex(), head, info->accessory3));
    this->m_slots[slotIndex].spriteNames[2] = static_cast<const std::string&>(g_session.GetAccessorySprName(info->job, g_session.GetCurrentSex(), head, info->accessory3));
    info->head = static_cast<std::int16_t>(head);
    this->m_slots[slotIndex].actionNames[3] = static_cast<const std::string&>(g_session.GetAccessoryActName(info->job, g_session.GetCurrentSex(), head, info->accessory2));
    this->m_slots[slotIndex].spriteNames[3] = static_cast<const std::string&>(g_session.GetAccessorySprName(info->job, g_session.GetCurrentSex(), head, info->accessory2));
    this->accessory2[slotIndex] = info->accessory2;
    this->m_slots[slotIndex].actionNames[4] = static_cast<const std::string&>(g_session.GetAccessoryActName(info->job, g_session.GetCurrentSex(), head, info->accessory));
    this->m_slots[slotIndex].spriteNames[4] = static_cast<const std::string&>(g_session.GetAccessorySprName(info->job, g_session.GetCurrentSex(), head, info->accessory));

    int bodyPalette = info->bodyStyle;
    if (info->bodyCostume <= 0) {
      bodyPalette = g_session.GetBodyStyle(info->job, bodyPalette, info->bodyCostume);
    }

    this->m_slots[slotIndex].bodyPalette = bodyPalette;
    if (bodyPalette != 0) {
      this->m_slots[slotIndex].bodyPaletteName = static_cast<const std::string&>(
        g_session.GetBodyPalName(info->job, g_session.GetCurrentSex(), bodyPalette, info->bodyCostume, 0, 0));
    }

    this->m_slots[slotIndex].headPalette = info->headPalette;
    if (this->m_slots[slotIndex].headPalette != 0) {
      this->m_slots[slotIndex].headPaletteName = static_cast<const std::string&>(g_session.GetHeadPalName(head, info->job, g_session.GetCurrentSex(), this->m_slots[slotIndex].headPalette));
    }

    this->m_slots[slotIndex].imfName = GetBodyImfName(info->job, g_session.GetCurrentSex(), info->head, info->bodyCostume);
  }
}

// OFFSET: 0x0093A7F0
// FUNCTION: fn_12758
int UISelCharForUServerWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_OK: {
    if (g_windowMgr.Confirm(LookupMessage(MSI_CONFIRM_SELECT_CHARACTERS), 2, false, false, 0, "Message", 280, 120, 0) != 0xBB) {
      return 0;
    }

    int characterIds[9] = {};
    int count = 0;
    for (int index = 0; index < g_session.characterCount; ++index) {
      if (this->m_checkBoxes[index]->geometry.state != 0) {
        CMode* mode = g_modeMgr.gameMode;
        const int* character = reinterpret_cast<const int*>(mode->SendMsg(MM_QUERYCHARICTORINFO, index, 0, 0, 0));
        if (character) {
          characterIds[count++] = *character;
        }
      }
    }

    if (count != 0) {
      PACKET_CZ_SELECT_CHARACTERS packet;
      packet.id = 0x28C;
      packet.accountId = g_session.m_aid;
      packet.count = count;
      std::memcpy(packet.characterIds, characterIds, sizeof(packet.characterIds));
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      g_windowMgr.Schedule(this);
      return 0;
    }

    g_windowMgr.Confirm(LookupMessage(MSI_NO_SELECT_CHARACTERS), 0, true, true, 0, "Message", 280, 120, 0);
    break;
  }
  case ID_CHECK:
    this->m_checkedCount += sender->geometry.state != 0 ? 1 : -1;
    if (this->m_checkedCount > 9) {
      g_windowMgr.Confirm(LookupMessage(MSI_ERROR_SELECT_CHARACTERS), 0, true, true, 0, "Message", 280, 120, 0);
      static_cast<UICheckBox*>(sender)->SetChecked(0);
    }
    break;
  }

  return 0;
}

// OFFSET: 0x00932B40
// FUNCTION: fn_12714
void UISelCharForUServerWnd::OnMouseMove(int x, int y) {
  this->UpdateCharInfoBalloon(x, y, static_cast<std::uint32_t>(-1));
  UIFrameWnd::OnMouseMove(x, y);
}

// OFFSET: 0x0093F1F0
// FUNCTION: fn_12777
void UISelCharForUServerWnd::UpdateCharInfoBalloon(int x, int y, std::uint32_t color) {
  const unsigned char red = (color >> 16) & 0xFF;
  if (!this->characterInfoBalloon) {
    this->characterInfoBalloon = new UICharInfoBalloonText();
    this->characterInfoBalloon->SetSize(140, 200);
    g_windowMgr.AddRendered(this->characterInfoBalloon);
  }

  this->characterInfoBalloon->Move(-400, -400);
  this->characterInfoLines.clear();
  if (x > 26 && x < this->geometry.width - 26 && static_cast<unsigned>(y - 95) < 270 && (y > 249 || y < 211)) {
    const int index = (y - 95) / 135 * this->m_columns + (x - 27) / 81;
    CharacterInfo* info = reinterpret_cast<CharacterInfo*>(g_modeMgr.gameMode->SendMsg(MM_QUERYCHARICTORINFO, index, 0, 0, 0));
    if (info) {
      char name[25];
      std::memcpy(name, info->name, sizeof(info->name));
      name[24] = '\0';
      int width = this->TextWidth(name, 0, 0, 12, 0, 0) + 60;

      char text[100];
      this->characterInfoLines.push_back("Name");
      this->characterInfoLines.push_back(name);
      this->characterInfoLines.push_back("Job");
      this->characterInfoLines.push_back(g_session.GetJobName(info->job, -1));
      this->characterInfoLines.push_back("Lv. ");
      this->characterInfoLines.push_back(_itoa(info->level, text, 10));
      this->characterInfoLines.push_back("EXP");
      this->characterInfoLines.push_back(_itoa(static_cast<int>(info->exp), text, 10));
      this->characterInfoLines.push_back("HP");
      this->characterInfoLines.push_back(_i64toa(info->hp, text, 10));
      this->characterInfoLines.push_back("SP");
      this->characterInfoLines.push_back(_i64toa(info->sp, text, 10));
      this->characterInfoLines.push_back("STR");
      this->characterInfoLines.push_back(_itoa(info->stats[0], text, 10));
      this->characterInfoLines.push_back("AGI");
      this->characterInfoLines.push_back(_itoa(info->stats[1], text, 10));
      this->characterInfoLines.push_back("VIT");
      this->characterInfoLines.push_back(_itoa(info->stats[2], text, 10));
      this->characterInfoLines.push_back("INT");
      this->characterInfoLines.push_back(_itoa(info->stats[3], text, 10));
      this->characterInfoLines.push_back("DEX");
      this->characterInfoLines.push_back(_itoa(info->stats[4], text, 10));
      this->characterInfoLines.push_back("LUK");
      this->characterInfoLines.push_back(_itoa(info->stats[5], text, 10));

      if (width < 140) {
        width = 140;
      }

      this->characterInfoBalloon->Resize(width, 200);
      this->characterInfoBalloon->m_outlineColor = 0;
      this->characterInfoBalloon->m_textColor = ((color & 0xFF) << 16) | (color & 0xFF00) | red;
      this->characterInfoBalloon->SetBackground(0x88000000);
      this->characterInfoBalloon->m_lines = this->characterInfoLines;

      int screenX = x + 20;
      int screenY = y;
      this->ToScreen(screenX, screenY);
      if (screenX < -3) {
        screenX = -3;
      }
      const int maxX = g_renderer->viewport.width - this->characterInfoBalloon->geometry.width + 3;
      if (screenX > maxX) {
        screenX = maxX;
      }
      if (screenY < -3) {
        screenY = -3;
      }
      const int maxY = g_renderer->viewport.height - this->characterInfoBalloon->geometry.height + 3;
      if (screenY > maxY) {
        screenY = maxY;
      }

      this->characterInfoBalloon->Move(screenX, screenY);
      return;
    }
  }

  g_windowMgr.Schedule(this->characterInfoBalloon);
  this->characterInfoBalloon = nullptr;
}
