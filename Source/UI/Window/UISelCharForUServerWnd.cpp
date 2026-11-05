#include "UI/Window/UISelCharForUServerWnd.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/SpriteRidingOffset.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "UI/Text/UICharInfoBalloonText.h"

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
  if (g_session.unknown_0E94 > 12) {
    this->m_columns = 9;
  }

  this->unknown_19E8 = 0;
  this->m_checkedCount = 0;
  this->geometry.unknown_0034 = 0;
  this->unknown_19E4 = 0;
  for (int index = 0; index < g_session.unknown_0E94; ++index) {
    this->m_slotEnabled[index] = 1;
  }

  this->m_startTime = timeGetTime();
}

// OFFSET: 0x00928A60
// FUNCTION: fn_12652
void UISelCharForUServerWnd::Slot15(int, int) {
  {
    std::string path = "유저인터페이스\\";
    this->unknown_19E4 = new UIBitmapButton();
    this->unknown_19E4->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
    this->unknown_19E4->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    this->unknown_19E4->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    this->unknown_19E4->SetSize(this->unknown_19E4->BitmapWidth(), this->unknown_19E4->BitmapHeight());
    this->unknown_19E4->Move(this->geometry.width - 46, this->geometry.height - 24);
    this->unknown_19E4->SetId(184);
    this->AddChild(this->unknown_19E4);
  }

  this->m_message0 = 184;

  for (int index = 0; index < g_session.unknown_0E94; ++index) {
    UICheckBox* checkBox = new UICheckBox();
    this->m_checkBoxes[index] = checkBox;
    checkBox->SetSize(10, 10);
    checkBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", "");
    this->m_checkBoxes[index]->Move(-100, -100);
    this->AddChild(this->m_checkBoxes[index]);
  }

  for (int index = 0; index < g_session.unknown_0E94; ++index) {
    CharacterInfo* info = reinterpret_cast<CharacterInfo*>(g_modeMgr.gameMode->SendMsg(8, index, 0, 0, 0));
    if (!info) {
      continue;
    }

    this->m_checkBoxes[index]->Move(index % this->m_columns * 81 + 63, index / this->m_columns * 149 + 77);

    const int slotIndex = info->slot;
    CharSlotInfo& slot = this->m_slots[slotIndex];
    slot.job = info->job;
    slot.robe = info->robe;
    this->m_slotEnabled[slotIndex] = 0;
    slot.x = slotIndex % this->m_columns * 81 + 68;
    slot.y = slotIndex / this->m_columns * 149 + 191;
    slot.unknown_0148 = 0;
    slot.action = 0;
    slot.frame = 0;

    slot.actionNames[0] = static_cast<const std::string&>(GetBodyActionResourceName(info->job, g_session.SyncUnknown0E98(), info->unknown_0058, 0));
    slot.spriteNames[0] = static_cast<const std::string&>(g_session.GetBodySprName(info->job, g_session.SyncUnknown0E98(), info->unknown_0058, 0));

    int head = info->head;
    slot.actionNames[1] = static_cast<const std::string&>(g_session.GetHeadActName(info->job, g_session.SyncUnknown0E98(), &head));
    slot.spriteNames[1] = static_cast<const std::string&>(g_session.GetHeadSprName(info->job, g_session.SyncUnknown0E98(), &head));
    slot.actionNames[2] = static_cast<const std::string&>(g_session.GetAccessoryActName(info->job, g_session.SyncUnknown0E98(), head, info->accessory3));
    slot.spriteNames[2] = static_cast<const std::string&>(g_session.GetAccessorySprName(info->job, g_session.SyncUnknown0E98(), head, info->accessory3));
    info->head = static_cast<std::int16_t>(head);
    slot.actionNames[3] = static_cast<const std::string&>(g_session.GetAccessoryActName(info->job, g_session.SyncUnknown0E98(), head, info->accessory2));
    slot.spriteNames[3] = static_cast<const std::string&>(g_session.GetAccessorySprName(info->job, g_session.SyncUnknown0E98(), head, info->accessory2));
    this->unknown_1974[slotIndex] = info->accessory2;
    slot.actionNames[4] = static_cast<const std::string&>(g_session.GetAccessoryActName(info->job, g_session.SyncUnknown0E98(), head, info->accessory));
    slot.spriteNames[4] = static_cast<const std::string&>(g_session.GetAccessorySprName(info->job, g_session.SyncUnknown0E98(), head, info->accessory));

    int bodyPalette = info->bodyStyle;
    if (info->unknown_0058 < 1) {
      bodyPalette = g_session.GetBodyStyle(info->job, bodyPalette, info->unknown_0058);
    }

    slot.bodyPalette = bodyPalette;
    if (bodyPalette != 0) {
      slot.bodyPaletteName = static_cast<const std::string&>(
        g_session.GetBodyPalName(info->job, g_session.SyncUnknown0E98(), bodyPalette, info->unknown_0058, 0, 0));
    }

    slot.headPalette = info->headPalette;
    if (slot.headPalette != 0) {
      slot.headPaletteName = static_cast<const std::string&>(g_session.GetHeadPalName(head, info->job, g_session.SyncUnknown0E98(), slot.headPalette));
    }

    slot.imfName = GetBodyImfName(info->job, g_session.SyncUnknown0E98(), info->head, info->unknown_0058);
  }
}

// OFFSET: 0x0093A7F0
// FUNCTION: fn_12758
int UISelCharForUServerWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (argument1 == 0xB8) {
    if (g_windowMgr.Confirm(LookupMessage(0x4BB), 2, false, false, 0, "Message", 280, 120, 0) != 0xBB) {
      return 0;
    }

    int characterIds[9] = {};
    int count = 0;
    for (int index = 0; index < g_session.unknown_0E94; ++index) {
      if (this->m_checkBoxes[index]->geometry.unknown_0030 != 0) {
        CMode* mode = g_modeMgr.gameMode;
        const int* character = reinterpret_cast<const int*>(mode->SendMsg(8, index, 0, 0, 0));
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
      for (int index = 0; index < 9; ++index) {
        packet.characterIds[index] = characterIds[index];
      }
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      g_windowMgr.Schedule(this);
      return 0;
    }

    g_windowMgr.Confirm(LookupMessage(0x4C1), 0, true, true, 0, "Message", 280, 120, 0);
  } else if (argument1 == 0xD5) {
    this->m_checkedCount += sender->geometry.unknown_0030 != 0 ? 1 : -1;
    if (this->m_checkedCount > 9) {
      g_windowMgr.Confirm(LookupMessage(0x4BC), 0, true, true, 0, "Message", 280, 120, 0);
      static_cast<UICheckBox*>(sender)->SetChecked(0);
    }
  }

  return 0;
}

// OFFSET: 0x00932B40
// FUNCTION: fn_12714
void UISelCharForUServerWnd::Slot27(int x, int y) {
  this->sub_93F1F0(x, y, -1);
  UIFrameWnd::Slot27(x, y);
}

// OFFSET: 0x0093F1F0
// FUNCTION: fn_12777
void UISelCharForUServerWnd::UpdateCharInfoBalloon(int x, int y, std::uint32_t color) {
  const unsigned char red = (color >> 16) & 0xFF;
  if (!this->unknown_19E8) {
    this->unknown_19E8 = new UICharInfoBalloonText();
    this->unknown_19E8->SetSize(140, 200);
    g_windowMgr.AddRendered(this->unknown_19E8);
  }

  this->unknown_19E8->Move(-400, -400);
  this->unknown_19EC.clear();
  if (x > 26 && x < this->geometry.width - 26 && static_cast<unsigned>(y - 95) < 270 && (y > 249 || y < 211)) {
    const int index = (y - 95) / 135 * this->m_columns + (x - 27) / 81;
    CharacterInfo* info = reinterpret_cast<CharacterInfo*>(g_modeMgr.gameMode->SendMsg(8, index, 0, 0, 0));
    if (info) {
      char name[25];
      std::memcpy(name, info->name, sizeof(info->name));
      name[24] = '\0';
      int width = this->TextWidth(name, 0, 0, 12, 0, 0) + 60;

      char text[100];
      this->unknown_19EC.push_back("Name");
      this->unknown_19EC.push_back(name);
      this->unknown_19EC.push_back("Job");
      this->unknown_19EC.push_back(g_session.GetJobName(info->job, -1));
      this->unknown_19EC.push_back("Lv. ");
      this->unknown_19EC.push_back(_itoa(info->level, text, 10));
      this->unknown_19EC.push_back("EXP");
      this->unknown_19EC.push_back(_itoa(static_cast<int>(info->exp), text, 10));
      this->unknown_19EC.push_back("HP");
      this->unknown_19EC.push_back(_i64toa(info->hp, text, 10));
      this->unknown_19EC.push_back("SP");
      this->unknown_19EC.push_back(_i64toa(info->sp, text, 10));
      this->unknown_19EC.push_back("STR");
      this->unknown_19EC.push_back(_itoa(info->stats[0], text, 10));
      this->unknown_19EC.push_back("AGI");
      this->unknown_19EC.push_back(_itoa(info->stats[1], text, 10));
      this->unknown_19EC.push_back("VIT");
      this->unknown_19EC.push_back(_itoa(info->stats[2], text, 10));
      this->unknown_19EC.push_back("INT");
      this->unknown_19EC.push_back(_itoa(info->stats[3], text, 10));
      this->unknown_19EC.push_back("DEX");
      this->unknown_19EC.push_back(_itoa(info->stats[4], text, 10));
      this->unknown_19EC.push_back("LUK");
      this->unknown_19EC.push_back(_itoa(info->stats[5], text, 10));

      if (width < 140) {
        width = 140;
      }

      this->unknown_19E8->Resize(width, 200);
      this->unknown_19E8->m_outlineColor = 0;
      this->unknown_19E8->m_textColor = ((color & 0xFF) << 16) | (color & 0xFF00) | red;
      this->unknown_19E8->SetBackground(0x88000000);
      this->unknown_19E8->m_lines = this->unknown_19EC;

      int screenX = x + 20;
      int screenY = y;
      this->ToScreen(screenX, screenY);
      if (screenX < -3) {
        screenX = -3;
      }
      const int maxX = g_renderer->viewport.width - this->unknown_19E8->geometry.width + 3;
      if (screenX > maxX) {
        screenX = maxX;
      }
      if (screenY < -3) {
        screenY = -3;
      }
      const int maxY = g_renderer->viewport.height - this->unknown_19E8->geometry.height + 3;
      if (screenY > maxY) {
        screenY = maxY;
      }

      this->unknown_19E8->Move(screenX, screenY);
      return;
    }
  }

  g_windowMgr.Schedule(this->unknown_19E8);
  this->unknown_19E8 = nullptr;
}
