#include "UI/Window/UIPetEvolutionWnd.h"

#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/PetEvolutionMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

// OFFSET: 0x009AAF30
// FUNCTION: fn_13761
UIPetEvolutionWnd::UIPetEvolutionWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->m_petEggId = -1;
  this->m_evolvedEggId = -1;
}

// OFFSET: 0x009AB0E0
// FUNCTION: fn_13762
UIPetEvolutionWnd::~UIPetEvolutionWnd() {
}

// OFFSET: 0x009AB170
// FUNCTION: fn_13763
void UIPetEvolutionWnd::Slot15(int width, int height) {
  this->AddCloseButton(*this->m_controls, "", 0);

  UIBitmapButton* okButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  okButton->SetBitmap("유저인터페이스\\btn_big_ok.bmp", 0, 0);
  okButton->SetBitmap("유저인터페이스\\btn_big_ok_a.bmp", 1, 0);
  okButton->SetBitmap("유저인터페이스\\btn_big_ok_b.bmp", 2, 0);
  okButton->SetSize(okButton->BitmapWidth(), okButton->BitmapHeight());
  okButton->Move(this->geometry.width - (okButton->BitmapWidth() * 2 + 20), this->geometry.height - okButton->BitmapHeight() - 10);
  okButton->SetId(184);
  this->AddChild(okButton);

  UIBitmapButton* cancelButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  cancelButton->SetBitmap("유저인터페이스\\btn_big_cancel.bmp", 0, 0);
  cancelButton->SetBitmap("유저인터페이스\\btn_big_cancel_a.bmp", 1, 0);
  cancelButton->SetBitmap("유저인터페이스\\btn_big_cancel_b.bmp", 2, 0);
  cancelButton->SetSize(cancelButton->BitmapWidth(), cancelButton->BitmapHeight());
  cancelButton->Move(this->geometry.width - cancelButton->BitmapWidth() - 10, this->geometry.height - cancelButton->BitmapHeight() - 10);
  cancelButton->SetId(185);
  this->AddChild(cancelButton);
}

// OFFSET: 0x009AB320
// FUNCTION: fn_13764
void UIPetEvolutionWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2567), 0);

  char text[260];
  std::memset(text, 0, sizeof(text));
  {
    std::string unused;

    int job = g_session.GetPetJobByEggItem(this->m_petEggId);
    if (job != -1) {
      g_session.GetPetIllustName(job, g_session.unknown_1038, text);
      CBitmapRes* illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      if (!illust) {
        illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("userinterface\\illust\\pet_noimage.bmp")));
      }
      if (illust) {
        this->FillRegion(12, 29, illust->image.width, illust->image.height, 0x9B9B9B);
        this->DrawBitmap(10, 27, &illust->image, false);
        DrawRectangleBorder(*this, 10, 27, illust->image.width, illust->image.height, 0);
      }
    }

    CBitmapRes* arrow = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\img_next.bmp")));
    if (arrow) {
      this->DrawBitmap(
        static_cast<int>((this->geometry.width - arrow->image.width) * 0.5), static_cast<int>((134 - arrow->image.height) * 0.5 + 27.0),
        &arrow->image, true
      );
    }

    job = g_session.GetPetJobByEggItem(this->m_evolvedEggId);
    if (job != -1) {
      g_session.GetPetIllustName(job, g_session.unknown_1038, text);
      CBitmapRes* illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      if (!illust) {
        illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("userinterface\\illust\\pet_noimage.bmp")));
      }
      if (illust) {
        this->FillRegion(this->geometry.width - illust->image.width - 8, 29, illust->image.width, illust->image.height, 0x9B9B9B);
        this->DrawBitmap(this->geometry.width - illust->image.width - 10, 27, &illust->image, false);
        DrawRectangleBorder(*this, this->geometry.width - illust->image.width - 10, 27, illust->image.width, illust->image.height, 0);
      }
    }
  }

  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 10, 200, LookupMessage(2569), 0, 0, 12, 0, 0, 0);

  int index = 0;
  for (auto it = this->m_materials.begin(); it != this->m_materials.end(); ++it, ++index) {
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, "%dea", it->second);
    UITextButton* button = this->m_materialButtons[index];
    this->DrawTextAt(
      this->m_controls->fonts, this->m_controls->locale, button->geometry.width + button->geometry.x + 10, button->geometry.y + 1, text, 0, 0, 12, 0, 1, 0
    );
  }

  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 10, this->geometry.height - 52, LookupMessage(2570), 0, 0, 12, 0, 0, 0);
}

namespace {
#pragma pack(push, 1)
  struct PACKET_CZ_PET_EVOLUTION {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t packetLength;
    /* 0004 */ int evolvedEggId;
    /* 0008 */ unsigned char materials[252];
  };
#pragma pack(pop)
}

// OFFSET: 0x009AB760
// FUNCTION: fn_13765
int UIPetEvolutionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    if (argument1 == 184) {
      std::vector<std::uint16_t> materials;
      for (auto it = this->m_materials.begin(); it != this->m_materials.end(); ++it) {
        ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, it->first, 1);
        if (item.quantity < it->second) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2574)), 255, 0, 0, 0);
          return 0;
        }
        materials.push_back(static_cast<std::uint16_t>(item.unknown_0000[1]));
        materials.push_back(static_cast<std::uint16_t>(it->second));
      }

      if (g_session.unknown_1038 != 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2607)), 255, 0, 0, 0);
        return 0;
      }

      int count = static_cast<int>(materials.size());
      std::uint16_t length = static_cast<std::uint16_t>(count * 2 + 8);
      PACKET_CZ_PET_EVOLUTION packet;
      std::memset(packet.materials, 0, sizeof(packet.materials));
      packet.packetType = ID_CZ_PET_EVOLUTION;
      packet.packetLength = length;
      packet.evolvedEggId = this->m_evolvedEggId;
      std::memcpy(packet.materials, materials.data(), count * 2);
      GetRagConnection().Send(static_cast<short>(length), &packet);
      g_windowMgr.CloseWindow(261);
    } else if (argument1 == 185 || argument1 == 201) {
      g_windowMgr.CloseWindow(261);
    }
    break;

  case 98: {
    ITEM_INFO item;
    item.SetItemId(argument1);
    item.identified = 1;
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    break;
  }

  case 142: {
    this->m_materials.clear();
    this->m_petEggId = argument1;
    this->m_evolvedEggId = argument2;
    std::pair<bool, std::vector<std::pair<int, int>>> result = g_petEvolutionMgr->GetMaterials(argument1, argument2);
    if (result.first) {
      this->m_materials = result.second;
    }
    this->RebuildButtons();
    break;
  }

  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x009ABC20
// FUNCTION: fn_13766
void UIPetEvolutionWnd::RebuildButtons() {
  const char* name = g_session.GetPetString(g_session.GetPetJobByEggItem(this->m_petEggId));
  if (name) {
    UITextButton* button = new UITextButton(*this->m_context, *this->m_controls);
    button->Configure(name, 0xFF6464, 0, 12, 1, 1);
    button->m_hoverColor = 0x9B9B9B;
    button->ResizeToTextClipped(button->geometry.width - 10, button->geometry.height);
    button->SetId(this->m_petEggId);
    button->Move(10, 171);
    button->CenterIn(90, button->geometry.height);
    button->SetTooltip(name);
    this->AddChild(button);
  }

  name = g_session.GetPetString(g_session.GetPetJobByEggItem(this->m_evolvedEggId));
  if (name) {
    UITextButton* button = new UITextButton(*this->m_context, *this->m_controls);
    button->Configure(name, 0xFF6464, 0, 12, 1, 1);
    button->m_hoverColor = 0x9B9B9B;
    button->ResizeToTextClipped(button->geometry.width - 10, button->geometry.height);
    button->SetId(this->m_evolvedEggId);
    button->Move(this->geometry.width - 100, 171);
    button->CenterIn(90, button->geometry.height);
    button->SetTooltip(name);
    this->AddChild(button);
  }

  this->m_materialButtons.clear();
  int y = 230;
  for (auto it = this->m_materials.begin(); it != this->m_materials.end(); ++it) {
    ITEM_INFO item;
    item.SetItemId(it->first);
    char text[260];
    std::memset(text, 0, sizeof(text));
    int capacity = sizeof(text);
    item.sub_6D5810(text, capacity, 0);
    UITextButton* button = new UITextButton(*this->m_context, *this->m_controls);
    button->Configure(text, 0xFF6464, 0, 12, 1, 1);
    button->m_hoverColor = 0x9B9B9B;
    button->ResizeToTextClipped(button->geometry.width, button->geometry.height);
    button->SetId(it->first);
    button->Move(10, y);
    this->AddChild(button);
    this->m_materialButtons.push_back(button);
    y += 20;
  }
}
