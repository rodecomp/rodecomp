#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Session/Session.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/Renderer.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIWindowMgr.h"
#include <json/value.h>

// GLOBAL: gl_24700
int g_defaultPlacementWidth = 400;

// GLOBAL: gl_24701
int g_defaultPlacementHeight = 317;

// OFFSET: 0x009A6C20
// FUNCTION: fn_13725
void NormalItemSlotWindow::Reset() {
  this->x = 230;
  this->y = 0;
  this->width = 290;
  this->height = 76;
  this->show = 1;
}

// OFFSET: 0x009A6000
// FUNCTION: fn_13667
int NormalItemSlotWindow::IsValid() const {
  if (this->width != 290 || this->height > 175) {
    return 0;
  }
  if (this->height < 43 || (this->height - 10) % 33 != 0 || (this->height == 175 && !g_session.ItemSlotActive())) {
    return 0;
  }

  const int maxX = g_renderer->viewport.width - 290;
  const int maxY = g_renderer->viewport.height - this->height;
  return this->x >= 0 && this->x <= maxX && this->y >= 0 && this->y <= maxY;
}

// OFFSET: 0x005E75A0
// FUNCTION: fn_03935
void CCharacterLinkedUserDataMgr::ExportNormalItemSlots(Json::Value& data) {
  Json::Value values;

  for (int index = 0; index < 9; ++index) {
    Json::Value value;
    value["ItemID"] = Json::UInt(g_session.m_normalItemSlots[index].itemId);
    value["SlotNum"] = index;
    values.append(value);
  }

  Json::Value info;
  info["SlotX"] = g_windowMgr.m_normalItemSlotWindow.x;
  info["SlotY"] = g_windowMgr.m_normalItemSlotWindow.y;
  info["SlotWidth"] = g_windowMgr.m_normalItemSlotWindow.width;
  info["SlotHeight"] = g_windowMgr.m_normalItemSlotWindow.height;
  info["SlotShow"] = g_windowMgr.m_normalItemSlotWindow.show;
  data["SlotInfo"] = info;
  data["NormalItemSlot"] = values;
  data["ItemSlotActive"] = g_session.ItemSlotActive();
}

// OFFSET: 0x009A50E0
// FUNCTION: fn_13620
int AchTracingWndInfo::IsValid() const {
  return this->x > 0 && this->x <= g_renderer->viewport.width - 300 && this->y > 0 && this->y <= g_renderer->viewport.height - 150;
}

// OFFSET: 0x009A5120
// FUNCTION: fn_13621
int BasicInfoWndInfo::IsValid() {
  if (this->width != 220) {
    return 0;
  }

  if (this->height != 55 && this->height != 134 && this->height != 71 && this->height != 149) {
    return 0;
  }

  this->expanded = this->height == 134 || this->height == 149;

  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
    if (this->height != 71 && this->height != 149) {
      return 0;
    }
  } else if (this->height != 55 && this->height != 134) {
    return 0;
  }

  return this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 && this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A51D0
// FUNCTION: fn_13622
int CashEmotionListWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5210
// FUNCTION: fn_13623
int ChatRoomWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5260
// FUNCTION: fn_13624
int ChooseWndInfo::IsValid() const {
  return this->width == 280 && this->height == 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 && this->y <= g_renderer->viewport.height - 120;
}

// OFFSET: 0x009A52B0
// FUNCTION: fn_13625
int ClanWndInfo::IsValid() const {
  return this->width == 400 && this->height == 317 && this->x >= 0 && this->x <= g_renderer->viewport.width - 400 && this->y >= 0 && this->y <= g_renderer->viewport.height - 317;
}

// OFFSET: 0x009A5300
// FUNCTION: fn_13626
int CombinedCardItemCollectionWndInfo::IsValid() const {
  return this->width == 280 && this->height >= 120 && this->height <= 450 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A53A0
// FUNCTION: fn_13627
int EmotionWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A53E0
// FUNCTION: fn_13628
int EquipWndInfo::IsValid() const {
  if (this->width != 280) {
    return 0;
  }

  int extraHeight = 0;
  if (g_session.m_swapEquipmentMode == 1) {
    extraHeight = 158;
  }
  if (this->height != 167 && this->height != 17 && this->height != 187 && this->height != 187 + extraHeight) {
    return 0;
  }
  if (this->contentHeight != 167 && this->contentHeight != 187) {
    return 0;
  }

  if (this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 && this->y <= g_renderer->viewport.height - this->height) {
    return 1;
  }

  return 0;
}

// OFFSET: 0x009A5460
// FUNCTION: fn_13629
int ExchangeAcceptWndInfo::IsValid() const {
  return this->width == 280 && this->height == 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 && this->y <= g_renderer->viewport.height - 120;
}

// OFFSET: 0x009A5500
// FUNCTION: fn_13631
int GuildHelperWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5540
// FUNCTION: fn_13632
int GuildWndInfo::IsValid() const {
  return this->width == g_defaultPlacementWidth && this->height == g_defaultPlacementHeight && this->x >= 0 &&
         this->x <= g_renderer->viewport.width - this->width && this->y >= 0 && this->y <= g_renderer->viewport.height - this->height && this->tab >= 0 &&
         this->tab <= 6;
}

// OFFSET: 0x009A5590
// FUNCTION: fn_13633
int HomunInfoWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A55D0
// FUNCTION: fn_13634
int ItemCollectionWndInfo::IsValid() const {
  return this->width == 280 && this->height >= 120 && this->height <= 450 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5620
// FUNCTION: fn_13635
int ItemCompositionWndInfo::IsValid() const {
  return this->width == 200 && this->height == 200 && this->x >= 0 && this->x <= g_renderer->viewport.width - 200 && this->y >= 0 && this->y <= g_renderer->viewport.height - 200;
}

// OFFSET: 0x009A5670
// FUNCTION: fn_13636
int ItemStoreFindWndInfo::IsValid() const {
  return this->width >= 140 && this->height >= 145 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A56C0
// FUNCTION: fn_13637
int ItemIdentifyWndInfo::IsValid() const {
  return this->width == 200 && this->height == 200 && this->x >= 0 && this->x <= g_renderer->viewport.width - 200 && this->y >= 0 && this->y <= g_renderer->viewport.height - 200;
}

// OFFSET: 0x009A5710
// FUNCTION: fn_13638
int ItemParamChangeDisplayWndInfo::IsValid() const {
  return this->width == 280 && this->height == 34 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 && this->y <= g_renderer->viewport.height - 34;
}

// OFFSET: 0x009A5760
// FUNCTION: fn_13639
int ItemPurchaseWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A57B0
// FUNCTION: fn_13640
int ItemSellWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5800
// FUNCTION: fn_13641
int ItemShopWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5850
// FUNCTION: fn_13642
int ItemStoreFindListInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A58A0
// FUNCTION: fn_13643
int ItemStoreFindSlotInfo::IsValid() const {
  return this->width >= 140 && this->height >= 145 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A58F0
// FUNCTION: fn_13644
int ItemWndInfo::IsValid() const {
  return this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 && this->y <= g_renderer->viewport.height - this->height &&
         this->selectedTab >= 0 && this->selectedTab <= 3 && this->usableRows >= 0 && this->usableRows <= 3 &&
         this->equipmentRows >= 0 && this->equipmentRows <= 3 && this->etcRows >= 0 && this->etcRows <= 4;
}

// OFFSET: 0x009A5940
// FUNCTION: fn_13645
int EntryQueueWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5980
// FUNCTION: fn_13646
int MiniMapWndInfo::IsValid() const {
  return this->expandedWidth > 0 && this->expandedHeight > 0 && this->width >= 0 && this->width <= g_renderer->viewport.width - this->expandedWidth &&
         this->height >= 0 && this->height <= g_renderer->viewport.height - this->expandedHeight;
}

// OFFSET: 0x009A59C0
// FUNCTION: fn_13647
int MerInfoWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5A00
// FUNCTION: fn_13648
int MerchantItemMyShopWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5A50
// FUNCTION: fn_13649
int MerchantItemPurchaseWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5AA0
// FUNCTION: fn_13650
int MerchantItemShopWndInfo::IsValid(bool wide) const {
  if (wide) {
    if (this->width < 350) {
      return 0;
    }
  } else if (this->width < 280) {
    return 0;
  }

  if (this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 && this->y <= g_renderer->viewport.height - this->height) {
    return 1;
  }

  return 0;
}

// OFFSET: 0x009A5B00
// FUNCTION: fn_13651
int MerchantItemWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5B50
// FUNCTION: fn_13652
int MerchantMirrorItemWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5BA0
// FUNCTION: fn_13653
int MerchantShopMakeWndInfo::IsValid() const {
  return this->width == 280 && this->height == 220 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 && this->y <= g_renderer->viewport.height - 220;
}

// OFFSET: 0x009A5BF0
// FUNCTION: fn_13654
int MessengerGroupWndInfo::IsValid() const {
  return this->iconIndex <= 2 && this->width >= 243 && this->height >= 166 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width &&
         this->y >= 0 && this->y <= g_renderer->viewport.height - this->height && (this->mode == 0 || this->mode == 1);
}

// OFFSET: 0x009A5C50
// FUNCTION: fn_13655
int NCashWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5D00
// FUNCTION: fn_13656
int UnusedWndInfoB::IsValid() const {
  return 0;
}

// OFFSET: 0x009A5D10
// FUNCTION: fn_13657
int SeekPartyWndInfo::IsValid() const {
  return this->width >= 152 && this->height >= 199 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5D60
// FUNCTION: fn_13658
int SeekPartyListWndInfo::IsValid() const {
  return this->width >= 550 && this->height >= 350 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5DB0
// FUNCTION: fn_13659
int SeekPartyMBWndInfo::IsValid() const {
  return this->width >= 360 && this->height >= 350 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5E00
// FUNCTION: fn_13660
int JobListWndInfo::IsValid() const {
  return this->width >= 152 && this->height >= 199 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5E50
// FUNCTION: fn_13661
int PartySettingWndInfo::IsValid() const {
  return this->width == 280 && this->height == 180 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 && this->y <= g_renderer->viewport.height - 180;
}

// OFFSET: 0x009A5EA0
// FUNCTION: fn_13662
int PetInfoWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5EE0
// FUNCTION: fn_13663
int QuestDisplayInfo::IsValid() const {
  return 1;
}

// OFFSET: 0x009A5F30
// FUNCTION: fn_13664
int QuickSlotWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5F70
// FUNCTION: fn_13665
int ReplayRECControlWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A5FB0
// FUNCTION: fn_13666
int SayDialogWndInfo::IsValid() const {
  return this->width == 280 && this->height == 180 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - 180;
}

// OFFSET: 0x009A6110
// FUNCTION: fn_13668
int SkillListWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A6160
// FUNCTION: fn_13669
int StatusWndInfo::IsValid() const {
  return this->width == 280 && this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A61A0
// FUNCTION: fn_13670
int SubChatHisWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A61E0
// FUNCTION: fn_13671
int AdvenPartyBoardWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A6220
// FUNCTION: fn_13672
int ApplyForPartyWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A6260
// FUNCTION: fn_13673
int RegisterPartyWndInfo::IsValid() const {
  return this->width > 0 && this->height > 0 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A62A0
// FUNCTION: fn_13674
int WhisperWndInfo::IsValid() const {
  return this->width >= 280 && this->height >= 120 && this->x >= 0 && this->x <= g_renderer->viewport.width - this->width && this->y >= 0 &&
         this->y <= g_renderer->viewport.height - this->height;
}

// OFFSET: 0x009A62F0
// FUNCTION: fn_13675
bool MacroReporterWndInfo::IsValid() const {
  return this->width > 0 && g_renderer->viewport.width - this->x >= this->width && this->height > 0 && g_renderer->viewport.height - this->y >= this->height;
}

// OFFSET: 0x009A6320
// FUNCTION: fn_13676
bool PreviewEquipWndInfo::IsValid() const {
  return this->x >= 0 && this->x <= g_renderer->viewport.width - 280 && this->y >= 0 && this->y <= g_renderer->viewport.height - 260;
}

// OFFSET: 0x009A6390
// FUNCTION: fn_13677
void AchTracingWndInfo::Reset() {
  this->x = g_renderer->viewport.width - 350;
  this->y = (g_renderer->viewport.height - 150) / 2;
}

// OFFSET: 0x009A63C0
// FUNCTION: fn_13678
void BasicInfoWndInfo::Reset() {
  this->x = 0;
  this->y = 0;
  this->width = 220;
  this->height = 134;
  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
    if (this->expanded == 1) {
      this->height = 149;
    } else {
      this->height = 71;
    }
    this->mode = 0;
  } else {
    this->height = this->expanded == 1 ? 134 : 55;
    this->mode = 0;
  }
}

// OFFSET: 0x009A6430
// FUNCTION: fn_13679
void CashEmotionListWndInfo::Reset() {
  this->x = 200;
  this->y = 150;
  this->width = 298;
  this->height = 380;
}

// OFFSET: 0x009A6450
// FUNCTION: fn_13680
void ChatRoomWndInfo::Reset() {
  this->x = 500;
  this->y = 180;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6470
// FUNCTION: fn_13681
void ChooseWndInfo::Reset() {
  this->x = 200;
  this->y = 300;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6490
// FUNCTION: fn_13682
void ClanWndInfo::Reset() {
  this->x = 0;
  this->y = 34;
  this->width = 400;
  this->height = 317;
}

// OFFSET: 0x009A64B0
// FUNCTION: fn_13683
void CombinedCardItemCollectionWndInfo::Reset() {
  this->x = 280;
  this->y = 278;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A64D0
// FUNCTION: fn_13684
void UnusedWndInfoA::Reset() {
  this->x = 200;
  this->y = 200;
  this->width = 260;
  this->height = 175;
}

// OFFSET: 0x009A64F0
// FUNCTION: fn_13685
void EmotionWndInfo::Reset() {
  this->x = 200;
  this->y = 150;
  this->width = 375;
  this->height = 280;
}

// OFFSET: 0x009A6510
// FUNCTION: fn_13686
void EquipWndInfo::Reset() {
  this->x = 185;
  this->y = 300;
  this->width = 280;
  this->height = 167;
  this->contentHeight = 167;
  this->visible = 0;
}

// OFFSET: 0x009A6540
// FUNCTION: fn_13687
void ExchangeAcceptWndInfo::Reset() {
  this->x = 200;
  this->y = 200;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6560
// FUNCTION: fn_13688
void FriendOptionWndInfo::Reset() {
  this->x = 200;
  this->y = 120;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6580
// FUNCTION: fn_13689
void GuildHelperWndInfo::Reset() {
  this->x = 230;
  this->y = 0;
  this->width = 200;
  this->height = 150;
}

// OFFSET: 0x009A65A0
// FUNCTION: fn_13690
void GuildWndInfo::Reset() {
  this->x = 0;
  this->y = 34;
  this->width = g_defaultPlacementWidth;
  this->height = g_defaultPlacementHeight;
  this->tab = 0;
}

// OFFSET: 0x009A65D0
// FUNCTION: fn_13691
void HomunInfoWndInfo::Reset() {
  this->x = 150;
  this->y = 100;
  this->width = 280;
  this->height = 180;
}

// OFFSET: 0x009A65F0
// FUNCTION: fn_13692
void ItemCollectionWndInfo::Reset() {
  this->x = 280;
  this->y = 126;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6610
// FUNCTION: fn_13693
void ItemCompositionWndInfo::Reset() {
  this->x = (g_renderer->viewport.width - 640) / 2 + 200;
  this->y = g_renderer->viewport.height * 200 / 480;
  this->width = 200;
  this->height = 200;
}

// OFFSET: 0x009A6660
// FUNCTION: fn_13694
void ItemStoreFindWndInfo::Reset() {
  this->x = 510;
  this->y = 360;
  this->width = 280;
  this->height = 312;
}

// OFFSET: 0x009A6680
// FUNCTION: fn_13695
void ItemIdentifyWndInfo::Reset() {
  this->x = (g_renderer->viewport.width - 640) / 2 + 200;
  this->y = g_renderer->viewport.height * 200 / 480;
  this->width = 200;
  this->height = 200;
}

// OFFSET: 0x009A66D0
// FUNCTION: fn_13696
void ItemParamChangeDisplayWndInfo::Reset() {
  this->x = 280;
  this->y = 246;
  this->width = 280;
  this->height = 34;
}

// OFFSET: 0x009A66F0
// FUNCTION: fn_13697
void ItemPurchaseWndInfo::Reset() {
  this->x = 280;
  this->y = 280;
  this->width = 280;
  this->height = 152;
}

// OFFSET: 0x009A6710
// FUNCTION: fn_13698
void ItemSellWndInfo::Reset() {
  this->x = 280;
  this->y = 280;
  this->width = 280;
  this->height = 152;
}

// OFFSET: 0x009A6730
// FUNCTION: fn_13699
void ItemShopWndInfo::Reset() {
  this->x = 0;
  this->y = 120;
  this->width = 280;
  this->height = 312;
}

// OFFSET: 0x009A6750
// FUNCTION: fn_13700
void ItemStoreFindListInfo::Reset() {
  this->x = 360;
  this->y = 200;
  this->width = 280;
  this->height = 458;
  this->openTabs = 0;
}

// OFFSET: 0x009A6780
// FUNCTION: fn_13701
void ItemStoreFindSlotInfo::Reset(int index) {
  const Position positions[] = {{210, 50}, {60, 50}, {210, 205}, {60, 205}, {210, 360}, {60, 360}, {210, 515}, {60, 515}};

  this->x = positions[index].x;
  this->y = positions[index].y;
  this->width = 140;
  this->height = 145;
}

// OFFSET: 0x009A6800
// FUNCTION: fn_13702
void ItemWndInfo::Reset() {
  this->x = 0;
  this->y = 120;
  this->width = 280;
  this->height = 120;
  this->expandedHeight = 120;
  this->selectedTab = 0;
  this->usableRows = 3;
  this->equipmentRows = 3;
  this->etcRows = 4;
  this->visible = 0;
}

// OFFSET: 0x009A6850
// FUNCTION: fn_13703
void EntryQueueWndInfo::Reset() {
  this->width = 280;
  this->height = 360;
  this->x = 350;
  this->y = 150;
}

// OFFSET: 0x009A6870
// FUNCTION: fn_13704
void MiniMapWndInfo::Reset() {
  const int x = g_renderer->viewport.width - 536;

  this->height = 190;
  this->width = x;
  this->x = x;
  this->y = 190;
  this->expandedWidth = 516;
  this->expandedHeight = 559;
  this->showMembers = 1;
  this->showQuests = 1;
  this->showTowns = 1;
}

// OFFSET: 0x009A68C0
// FUNCTION: fn_13705
void MerInfoWndInfo::Reset() {
  this->x = 150;
  this->y = 100;
  this->width = 280;
  this->height = 180;
}

// OFFSET: 0x009A68E0
// FUNCTION: fn_13706
void MerchantItemMyShopWndInfo::Reset() {
  this->x = 200;
  this->y = 120;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6900
// FUNCTION: fn_13707
void MerchantItemPurchaseWndInfo::Reset() {
  this->x = 280;
  this->y = 280;
  this->width = 280;
  this->height = 152;
}

// OFFSET: 0x009A6920
// FUNCTION: fn_13708
void MerchantItemShopWndInfo::Reset(bool wide) {
  this->x = 0;
  this->y = 120;
  this->width = !wide ? 280 : 350;
  this->height = 312;
}

// OFFSET: 0x009A6950
// FUNCTION: fn_13709
void MerchantItemWndInfo::Reset() {
  this->x = 200;
  this->y = 120;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6970
// FUNCTION: fn_13710
void MerchantMirrorItemWndInfo::Reset() {
  this->x = 0;
  this->y = 120;
  this->width = 280;
  this->height = 120;
}

// OFFSET: 0x009A6990
// FUNCTION: fn_13711
void MerchantShopMakeWndInfo::Reset() {
  this->x = 0;
  this->y = 240;
  this->width = 280;
  this->height = 220;
}

// OFFSET: 0x009A69B0
// FUNCTION: fn_13712
void MessengerGroupWndInfo::Reset() {
  this->width = 273;
  this->height = 326;
  this->iconIndex = 0;
  this->x = 300;
  this->y = 200;
  this->visible = 0;
  this->mode = 1;
}

// OFFSET: 0x009A69F0
// FUNCTION: fn_13713
void NCashWndInfo::Reset() {
  this->x = 0;
  this->y = 30;
  this->width = 640;
  this->height = 508;
}

// OFFSET: 0x009A6A70
// FUNCTION: fn_13714
void UnusedWndInfoB::Reset() {
  this->x = 185;
  this->y = 300;
  this->width = 250;
  this->height = 60;
  this->defaultHeight = 60;
  this->visible = 0;
}

// OFFSET: 0x009A6AA0
// FUNCTION: fn_13715
void SeekPartyWndInfo::Reset() {
  this->x = 510;
  this->y = 360;
  this->width = 152;
  this->height = 199;
}

// OFFSET: 0x009A6AC0
// FUNCTION: fn_13716
void SeekPartyListWndInfo::Reset() {
  this->x = 510;
  this->y = 360;
  this->width = 550;
  this->height = 350;
}

// OFFSET: 0x009A6AE0
// FUNCTION: fn_13717
void SeekPartyMBWndInfo::Reset() {
  this->x = 510;
  this->y = 360;
  this->width = 360;
  this->height = 350;
}

// OFFSET: 0x009A6B00
// FUNCTION: fn_13718
void JobListWndInfo::Reset() {
  this->x = 510;
  this->y = 360;
  this->width = 152;
  this->height = 199;
}

// OFFSET: 0x009A6B20
// FUNCTION: fn_13719
void PartySettingWndInfo::Reset() {
  this->x = 280;
  this->y = 246;
  this->width = 280;
  this->height = 180;
}

// OFFSET: 0x009A6B40
// FUNCTION: fn_13720
void PetInfoWndInfo::Reset() {
  this->x = 150;
  this->y = 100;
  this->width = 280;
  this->height = 200;
}

// OFFSET: 0x009A6B60
// FUNCTION: fn_13721
void QuestDisplayInfo::Reset() {
  this->visible = 1;
}

// OFFSET: 0x009A6BB0
// FUNCTION: fn_13722
void QuickSlotWndInfo::Reset() {
  this->x = g_renderer->viewport.width - 124;
  this->y = 0;
  this->width = 77;
  this->height = 34;
}

// OFFSET: 0x009A6BE0
// FUNCTION: fn_13723
void ReplayRECControlWndInfo::Reset() {
  this->x = 259;
  this->y = 230;
  this->width = 194;
  this->height = 34;
}

// OFFSET: 0x009A6C00
// FUNCTION: fn_13724
void SayDialogWndInfo::Reset() {
  this->x = 200;
  this->y = 100;
  this->width = 280;
  this->height = 180;
}

// OFFSET: 0x009A6C50
// FUNCTION: fn_13726
void UnusedWndInfoC::Reset() {
  this->x = 185;
  this->y = 300;
  this->width = 140;
  this->height = 30;
  this->visible = 0;
}

// OFFSET: 0x009A6C80
// FUNCTION: fn_13727
void UnusedWndInfoD::Reset() {
  this->x = 185;
  this->y = 300;
  this->width = 140;
  this->height = 30;
}

// OFFSET: 0x009A6CA0
// FUNCTION: fn_13728
void SkillListWndInfo::Reset() {
  this->x = 360;
  this->y = 200;
  this->width = 280;
  this->height = 240;
}

// OFFSET: 0x009A6CC0
// FUNCTION: fn_13729
void StatusWndInfo::Reset() {
  this->x = 185;
  this->y = 300;
  this->width = 280;
  this->visible = 0;
  this->height = 141;
  this->expandedHeight = 141;
  this->extended = 1;
}

// OFFSET: 0x009A6D00
// FUNCTION: fn_13730
void SubChatHisWndInfo::Reset(int side) {
  switch (side) {
  case 0:
    this->x = 0;
    break;
  case 1:
    this->x = 300;
    break;
  }

  this->y = g_renderer->viewport.height - 119;
  this->width = 300;
  this->height = 95;
}

// OFFSET: 0x009A6D40
// FUNCTION: fn_13731
void AdvenPartyBoardWndInfo::Reset() {
  this->x = 200;
  this->y = 200;
  this->width = 480;
  this->height = 340;
}

// OFFSET: 0x009A6D60
// FUNCTION: fn_13732
void ApplyForPartyWndInfo::Reset() {
  this->x = 200;
  this->y = 200;
  this->width = 220;
  this->height = 190;
}

// OFFSET: 0x009A6D80
// FUNCTION: fn_13733
void RegisterPartyWndInfo::Reset() {
  this->x = 200;
  this->y = 200;
  this->width = 220;
  this->height = 197;
}

// OFFSET: 0x009A6DA0
// FUNCTION: fn_13734
void WhisperWndInfo::Reset() {
  this->width = 280;
  this->height = 120;
  this->x = 0;
  this->y = 347;
}

// OFFSET: 0x009A6DC0
// FUNCTION: fn_13735
void MacroReporterWndInfo::Reset() {
  this->x = 249;
  this->y = 308;
  this->width = g_renderer->viewport.width - 299;
  this->height = (g_renderer->viewport.height - 308) / 2;
}

// OFFSET: 0x009A6E00
// FUNCTION: fn_13736
void PreviewEquipWndInfo::Reset() {
  this->x = static_cast<int>(static_cast<float>(g_renderer->viewport.width - 280) * g_depthAverageHalf);
  this->y = static_cast<int>(static_cast<float>(g_renderer->viewport.height - 260) * g_depthAverageHalf);
}

// OFFSET: 0x009A6E90
// FUNCTION: fn_13737
void UIWindowMgr::ResetUIRectInfo() {
  // GLOBAL: gl_25071
  static bool s_resetPending = true;
  if (!s_resetPending) {
    return;
  }
  s_resetPending = false;

  this->basicInfoWndInfo.Reset();
  this->itemWndInfo.Reset();

  int* const questWnd = this->questWndPlacement;
  questWnd[0] = 0;
  questWnd[1] = 120;
  questWnd[2] = 280;
  questWnd[3] = 200;
  questWnd[4] = 200;
  questWnd[5] = 0;
  this->questWndVisible = 0;

  this->statusWndInfo.Reset();
  this->equipWndInfo.Reset();
  this->optionWndInfo.Reset();
  this->shortenItemWndInfo.Reset();
  this->unusedWndInfoD.Reset();
  this->sayDialogWndInfo.Reset();
  this->chooseWndInfo.Reset();
  this->chatRoomWndInfo.Reset();
  this->itemStoreFindListInfo.Reset();

  ItemStoreFindSlotInfo* const storeSlots = this->itemStoreFindSlots;
  for (int index = 0; index < 8; ++index) {
    storeSlots[index].Reset(index);
  }
  this->itemStoreWindowCount = 8;

  this->itemStoreFindWndInfo.Reset();
  this->seekPartyListWndInfo.Reset();
  this->seekPartyMBWndInfo.Reset();
  this->seekPartyWndInfo.Reset();
  this->jobListWndInfo.Reset();
  this->itemShopWndInfo.Reset();
  this->itemSellWndInfo.Reset();
  this->itemPurchaseWndInfo.Reset();
  this->messengerGroupWndInfo.Reset();
  this->m_normalItemSlotWindow.Reset();
  this->exchangeAcceptWndInfo.Reset();
  this->skillListWndInfo.Reset();
  this->merchantItemWndInfo.Reset();
  this->merchantMirrorItemWndInfo.Reset();
  this->merchantShopMakeWndInfo.Reset();
  this->merchantItemShopWndInfo.Reset(false);
  this->merchantItemMyShopWndInfo.Reset();
  this->merchantItemPurchaseWndInfo.Reset();
  this->itemCollectionWndInfo.Reset();
  this->combinedCardItemCollectionWndInfo.Reset();
  this->itemParamChangeDisplayWndInfo.Reset();
  this->partySettingWndInfo.Reset();
  this->unusedWndInfoA.Reset();
  this->guildWndInfo.Reset();
  this->m_whisperPlacement.Reset();
  this->itemCompositionWndInfo.Reset();
  this->itemIdentifyWndInfo.Reset();
  this->emotionWndInfo.Reset();
  this->cashEmotionListWndInfo.Reset();
  this->petInfoWndInfo.Reset();
  this->homunInfoWndInfo.Reset();
  this->merInfoWndInfo.Reset();
  this->subChatHisWndInfo[0].Reset(0);
  this->subChatHisWndInfo[1].Reset(1);
  this->quickSlotWndInfo.Reset();
  this->entryQueueWndInfo.Reset();
  this->buyingShopMakeWndInfo.Reset();
  this->buyingMirrorItemWndInfo.Reset();
  this->buyingItemMyShopWndInfo.Reset();
  this->buyingItemShopWndInfo.Reset(true);
  this->buyingItemPurchaseWndInfo.Reset();
  this->buyingMirrorItemWndInfo2.Reset();
  this->nCashWndInfo.Reset();
  this->replayRecControlWndInfo.Reset();

  NavigationWindowInfo* const navigation = &this->navigationWindowInfo;
  navigation->x = static_cast<int>(g_renderer->viewport.width * 0.5 - 136.0);
  navigation->y = static_cast<int>(g_renderer->viewport.height * 0.5 - 169.0);
  navigation->width = 272;
  navigation->height = 338;
  navigation->expanded = 1;

  this->miniMapWndInfo.Reset();
  reinterpret_cast<AchTracingWndInfo*>(&this->achievementTracingX)->Reset();
  this->macroReporterWndInfo.Reset();
  reinterpret_cast<PreviewEquipWndInfo*>(&this->previewEquipX)->Reset();

  int* const swapEquipment = this->swapEquipmentPlacement;
  swapEquipment[0] = (g_renderer->viewport.width - 280) / 2;
  swapEquipment[1] = (g_renderer->viewport.height - 177) / 2;
  swapEquipment[2] = 280;
  swapEquipment[3] = 167;

  this->questDisplayInfo.Reset();
  this->registerPartyWndInfo.Reset();
  this->applyForPartyWndInfo.Reset();
  this->advenPartyBoardWndInfo.Reset();
}

// OFFSET: 0x009A7CA0
// FUNCTION: fn_13738
void NavigationWindowInfo::Clamp() {
  if (this->height < 338 || this->height > 338) {
    this->height = 338;
  }

  if (this->expanded) {
    if (this->width != 272) {
      this->width = 272;
    }
  } else if (this->width != 228) {
    this->width = 228;
  }

  if (this->x < 0) {
    this->x = 0;
  }
  if (this->x > g_renderer->viewport.width - this->width) {
    this->x = g_renderer->viewport.width - this->width;
  }

  if (this->y < 0) {
    this->y = 0;
  }
  if (this->y > g_renderer->viewport.height - this->height) {
    this->y = g_renderer->viewport.height - this->height;
  }
}
