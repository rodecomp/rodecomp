#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Session.h"
#include "Render/Renderer.h"

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
bool NormalItemSlotWindow::IsValid(int screenWidth, int screenHeight, bool active) const {
  if (this->width != 290 || this->height < 43 || this->height > 175 || (this->height - 10) % 33 != 0 || (this->height == 175 && !active)) {
    return false;
  }

  const int maxX = screenWidth - 290;
  const int maxY = screenHeight - this->height;
  return this->x >= 0 && this->x <= maxX && this->y >= 0 && this->y <= maxY;
}

// OFFSET: 0x005E75A0
// FUNCTION: fn_03935
void ExportNormalItemSlots(Json::Value& data, const NormalItemSlot* slots, const NormalItemSlotWindow& window, bool active) {
  Json::Value values;

  for (int index = 0; index < 9; ++index) {
    Json::Value value;
    value["ItemID"] = Json::UInt(slots[index].itemId);
    value["SlotNum"] = index;
    values.append(value);
  }

  Json::Value info;
  info["SlotX"] = window.x;
  info["SlotY"] = window.y;
  info["SlotWidth"] = window.width;
  info["SlotHeight"] = window.height;
  info["SlotShow"] = window.show;
  data["SlotInfo"] = info;
  data["NormalItemSlot"] = values;
  data["ItemSlotActive"] = active;
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
    if (this->unknown_0012 == 1) {
      this->height = 149;
    } else {
      this->height = 71;
    }
    this->unknown_0010 = 0;
  } else {
    this->height = this->unknown_0012 == 1 ? 134 : 55;
    this->unknown_0010 = 0;
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
  this->unknown_0010 = 167;
  this->unknown_0014 = 0;
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
  this->unknown_0010 = 0;
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
  this->unknown_0010 = 0;
}

// OFFSET: 0x009A6780
// FUNCTION: fn_13701
void ItemStoreFindSlotInfo::Reset(int index) {
  const Position positions[] = {{210, 50}, {60, 50}, {210, 205}, {60, 205}, {210, 360}, {60, 360}, {210, 515}, {60, 515}};

  this->width = 140;
  this->height = 145;
  this->x = positions[index].x;
  this->y = positions[index].y;
}

// OFFSET: 0x009A6800
// FUNCTION: fn_13702
void ItemWndInfo::Reset() {
  this->x = 0;
  this->y = 120;
  this->width = 280;
  this->height = 120;
  this->unknown_0010 = 120;
  this->unknown_0014 = 0;
  this->unknown_0018 = 3;
  this->unknown_001C = 3;
  this->unknown_0020 = 4;
  this->unknown_0024 = 0;
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
  this->unknown_0010 = 516;
  this->unknown_0014 = 559;
  this->unknown_0020 = 1;
  this->unknown_0018 = 1;
  this->unknown_001C = 1;
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
  this->width = wide ? 350 : 280;
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
  this->unknown_0018 = 0;
  this->x = 300;
  this->y = 200;
  this->unknown_0010 = 0;
  this->unknown_0014 = 1;
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
  this->unknown_0010 = 60;
  this->unknown_0014 = 0;
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
  this->unknown_0000 = 1;
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
