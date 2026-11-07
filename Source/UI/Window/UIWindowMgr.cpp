#include "UI/Window/WindowPolicy.h"
#include "Render/Renderer.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Dialogs/UIMemorialDunWnd.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UINavigationWnd.h"
#include "UI/Window/UIRoMapWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "Client/Mode/CLoginMode.h"
#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
#include "Contents/WorldView/WorldViewData.h"
#include "Resource/ResourceMgr.h"
#include "File/ShortcutFilename.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "UI/Window/UIHotKeyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Contents/RouletteMgr.h"
#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Item/UIItemWnd.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "Render/DeviceContext/DCSurface.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <timeapi.h>
#include "Render/Textures/Surface.h"
#include "UI/FrameSkin.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Contents/ClanInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Dialogs/UIWaitDBSaveWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIMessageBox.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Chat/UIWhisperWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Window/UIChatRoomWnd.h"
#include "Contents/GuildInfo.h"

#include "Contents/Configuration/GameSettingsMgr.h"
#include "Contents/Sound/SoundMaker.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Client/ReplayRecordOption.h"
#include "Contents/Items/RuneSystem.h"
#include "Contents/MapPropertyMgr.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Messenger/MessengerWindows.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Acceptance/UIAutoMessageWnd.h"
#include "UI/Acceptance/UIJoinPartyAcceptWnd.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/CashEmotion/UICashEmotionListWnd.h"
#include "UI/CashShop/UICashShopWnd.h"
#include "UI/Chat/UIChatRoomMakeWnd.h"
#include "UI/Chat/UISubChatMiniWnd.h"
#include "UI/Clan/UIClanWnd.h"
#include "UI/Dialogs/UIBookWnd.h"
#include "UI/Dialogs/UIChooseWarpWnd.h"
#include "UI/Dialogs/UIChooseWnd.h"
#include "UI/Dialogs/UIDisconnectedServerMsgWnd.h"
#include "UI/Dialogs/UIEditctrlWnd.h"
#include "UI/Dialogs/UIGraffiStrboxWnd.h"
#include "UI/Dialogs/UILoadingWnd.h"
#include "UI/Dialogs/UIMenuWnd.h"
#include "UI/Dialogs/UIMiniMapWnd.h"
#include "UI/Dialogs/UIMinimapZoomWnd.h"
#include "UI/Dialogs/UINoticeConfirmWnd.h"
#include "UI/Dialogs/UINotifyJobLevelUpWnd.h"
#include "UI/Dialogs/UINotifyLevelUpWnd.h"
#include "UI/Dialogs/UINotifyQuestWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "UI/Dialogs/UIPetTamingDeceiveWnd.h"
#include "UI/Dialogs/UIProhibitListWnd.h"
#include "UI/Dialogs/UIQuitWnd.h"
#include "UI/Dialogs/UIRestartWnd.h"
#include "UI/Dialogs/UISayDialogWnd.h"
#include "UI/Dialogs/UISecondPasswordEditWnd.h"
#include "UI/Dialogs/UISelectCartWnd.h"
#include "UI/Dialogs/UITipOfTheDayWnd.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Dialogs/UIToolTipWnd.h"
#include "UI/Dialogs/UIWaitWnd.h"
#include "UI/Edit/UICandidateWnd.h"
#include "UI/Emotion/UIEmotionWnd.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Guild/UIGuildHelperWnd.h"
#include "UI/Guild/UIGuildWnd.h"
#include "UI/Guild/UIGuild_Storage_Log.h"
#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemSellWnd.h"
#include "UI/Item/UIItemSellWnd2.h"
#include "UI/Item/UIItemShopWnd.h"
#include "UI/Item/UIItemShopWnd2.h"
#include "UI/Item/UIItemStoreFindWnd.h"
#include "UI/Item/UIItemStoreSubWnd.h"
#include "UI/Item/UIMerchantItemLogWnd.h"
#include "UI/Item/UIParaItemShopWnd.h"
#include "UI/Items/MergeItemWnd.h"
#include "UI/Items/UICardItemIllustWnd.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Items/UIIllustWnd.h"
#include "UI/Items/UIItemCollectionWnd.h"
#include "UI/Items/UIItemIdentifyWnd.h"
#include "UI/Items/UIItemParamChangeDisplayWnd.h"
#include "UI/Items/UIItemRepairWnd.h"
#include "UI/Items/UIMakeTargetListWnd.h"
#include "UI/Items/UIMakeTargetProcessWnd.h"
#include "UI/Items/UIMakeTargetResultWnd.h"
#include "UI/Items/UINotifyItemObtainWnd.h"
#include "UI/Items/UISpellListWnd.h"
#include "UI/Items/UIWeaponRefineWnd.h"
#include "UI/Lists/UIComboBoxWnd.h"
#include "UI/Menu/UIMenuIconWnd.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Party/UIPartySettingWnd.h"
#include "UI/Party/UIRegisterPartyWnd.h"
#include "UI/Party/UIRequestJoinPartyWnd.h"
#include "UI/Quest/UIQuestDisplay.h"
#include "UI/Quest/UIRenewQuestWnd.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Rodex/UIRodexInboxWnd.h"
#include "UI/RuneSystem/UIRuneSystemWindows.h"
#include "UI/Shop/UIMerchantItemMyShopWnd.h"
#include "UI/Shop/UIMerchantItemPurchaseMSGWnd.h"
#include "UI/Shop/UIMerchantItemPurchaseWnd.h"
#include "UI/Shop/UIMerchantItemShopWnd.h"
#include "UI/Shop/UIMerchantItemWnd.h"
#include "UI/Shop/UIMerchantMirrorItemWnd.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Shop/UInCash_CallWnd.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Skill/UISkillListWnd.h"
#include "UI/Skill/UISkillNameChangeWnd.h"
#include "UI/Status/UIStatusWnd.h"
#include "UI/StylingShop/UIStylingShopWnd.h"
#include "UI/Text/UIAchWnd.h"
#include "UI/UIBartermarketWnd.h"
#include "UI/UICaptchaPreviewWnd.h"
#include "UI/UICaptchaRegisterWnd.h"
#include "UI/UIChangeMaterialWnd.h"
#include "UI/UICheckAttendanceWnd.h"
#include "UI/UIGradeEnchantWnd.h"
#include "UI/UIHotkeyGuideWnd.h"
#include "UI/UIItemReformWnd.h"
#include "UI/UILapineScrollBar.h"
#include "UI/UILapineUpgradeBoxWnd.h"
#include "UI/UIMacroBlackListCheckWnd.h"
#include "UI/UIMacroDetectorWnd.h"
#include "UI/UIMacroUserInfoWnd.h"
#include "UI/UIPreviewEquipWnd.h"
#include "UI/UIRefiningWnd.h"
#include "UI/UIReputeWnd.h"
#include "UI/UIRoundBoxTooltipWnd.h"
#include "UI/Window/UIAccountLimitedToolWnd.h"
#include "UI/Window/UIBalloonOpenBtn.h"
#include "UI/Window/UIBank_NewWnd.h"
#include "UI/Window/UIBasicInfoWnd.h"
#include "UI/Window/UIBattleMsgOptionWnd.h"
#include "UI/Window/UIChangeNameWnd.h"
#include "UI/Window/UICollectionSystemWnd.h"
#include "UI/Window/UIDebuffRemoveWnd.h"
#include "UI/Window/UIEscOptionWnd.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIHomunInfoWnd.h"
#include "UI/Window/UIJobListWnd.h"
#include "UI/Window/UILoginOTPWnd.h"
#include "UI/Window/UILoginWnd.h"
#include "UI/Window/UIMakeCharWnd.h"
#include "UI/Window/UIMerInfoWnd.h"
#include "UI/Window/UIMileageWnd.h"
#include "UI/Window/UIMonsterDisplayWnd.h"
#include "UI/Window/UIMonsterInfoWnd.h"
#include "UI/Window/UINavigationHelpWnd.h"
#include "UI/Window/UINewMakeCharWnd.h"
#include "UI/Window/UINewSkillListWnd.h"
#include "UI/Window/UINoteWnd.h"
#include "UI/Window/UINoticeWnd.h"
#include "UI/Window/UIOpenMailBoxWnd.h"
#include "UI/Window/UIOpenRoulletteWnd.h"
#include "UI/Window/UIPartyBookingHelpWnd.h"
#include "UI/Window/UIPetEvolutionWnd.h"
#include "UI/Window/UIPetInfoWnd.h"
#include "UI/Window/UIRPImageWnd.h"
#include "UI/Window/UIRPImageWnd2.h"
#include "UI/Window/UIRebirthWnd.h"
#include "UI/Window/UIReplayControlWnd.h"
#include "UI/Window/UIReplayRECControlWnd.h"
#include "UI/Window/UIRodexContentsWnd.h"
#include "UI/Window/UISecondCostumeWnd.h"
#include "UI/Window/UISeekPartyListWnd.h"
#include "UI/Window/UISeekPartyMBWnd.h"
#include "UI/Window/UISeekPartyWnd.h"
#include "UI/Window/UISelCharForUServerWnd.h"
#include "UI/Window/UISelectReplayDataWnd.h"
#include "UI/Window/UISelectServerWnd.h"
#include "UI/Window/UIShowWarpWnd.h"
#include "UI/Window/UITalkboxTrapInputWnd.h"
#include "UI/Window/UITransMenuWnd.h"
#include "UI/Window/UIVideoWnd.h"
#include "UI/Window/UIViewCameraInfoWnd.h"
#include "Utility/JsonCppLoader.h"
#include <cstdlib>

namespace {
#pragma pack(push, 1)
struct AddFriendPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
};

struct BankingCheckPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint32_t aid;
};

struct ShortcutRotatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t option;
  /* 0004 */ unsigned char state;
};
#pragma pack(pop)

struct AiCommand {
  /* 0000 */ int type;
  /* 0004 */ int args[5];
};

static_assert(sizeof(BankingCheckPacket) == 6);
static_assert(sizeof(ShortcutRotatePacket) == 5);
static_assert(sizeof(AiCommand) == 0x18);
static_assert(sizeof(AddFriendPacket) == 26);
static_assert(offsetof(AddFriendPacket, name) == 2);
}


// GLOBAL: gl_24525
CMiniPartyWndAreaMgr* g_miniPartyWndAreaMgr = nullptr;

// GLOBAL: gl_24366
UIWindowMgr g_windowMgr;

// GLOBAL: gl_25159
bool g_windowToggleFlag;


// GLOBAL: gl_24687
int g_closeWindow164State;

// GLOBAL: gl_24683
int g_windowCloseValue;

// GLOBAL: gl_24684
int g_savedWindowCloseValue;

// STRING: st_04179
static const char g_windowManagerDiagnosticFile[] = "C:\\repositories\\RagnarokClient\\RagnarokClient\\UIWindowMgr.cpp";

// GLOBAL: gl_24696
int g_renderDiagnosticLine = 0;

// GLOBAL: gl_24697
const char* g_renderDiagnosticFile = "";

// OFFSET: 0x00AA73B0
// FUNCTION: fn_17238
void UIWindowMgr::DrawWindows() {
  BalloonRectangle rectangle;

  for (auto overlay = this->overlays.begin(); overlay != this->overlays.end(); ++overlay) {
    overlay->second->Update(0);
    CDC* surface = overlay->second->geometry.surface;
    const std::uint32_t color = overlay->second->Color();
    surface->Draw(
      *GetRenderSystemHolder()->system, overlay->second->geometry.x, overlay->second->geometry.y, overlay->second->geometry.width,
      overlay->second->geometry.height, color
    );
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
    UIWindow* window = *entry;
    if (window->geometry.visible != 0 && (this->window_4F288 == nullptr || window->geometry.id == 359)) {
      g_renderDiagnosticLine = 3783;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      if (window->geometry.id == 189) {
        window->Invalidate();
      }

      g_renderDiagnosticLine = 3811;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      window->Update(0);
      g_renderDiagnosticLine = 3815;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      if (window->Slot07(&rectangle) == true) {
        DrawAlphaRectangle(rectangle.x, rectangle.y, rectangle.width, rectangle.height, rectangle.color);
        g_renderDiagnosticLine = 3823;
        g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      } else if (window->Slot42(&rectangle) == true) {
        window->Slot43();
        g_renderDiagnosticLine = 3829;
        g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      }

      if (window == this->modal) {
        static_cast<UIRoMapWnd*>(this->modal)->DrawWorld();
      }

      g_renderDiagnosticLine = 3835;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      if (window->m_cached == 1) {
        CTexture* texture = window->m_texture;
        if (texture != nullptr) {
          const std::uint32_t color = window->Color();
          texture->Draw(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height, color);
        }
        g_renderDiagnosticLine = 3844;
      } else {
        CDC* surface = window->geometry.surface;
        const std::uint32_t color = window->Color();
        surface->Draw(
          *GetRenderSystemHolder()->system, window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height, color
        );
        g_renderDiagnosticLine = 3850;
      }
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      window->FlushPrimitives(g_renderer->queues, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      g_renderDiagnosticLine = 3860;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      g_cooldownOwner->Drawing().SubmitNormal(g_renderer->queues, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
      g_renderDiagnosticLine = 3866;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      if (window == this->unknown_0464) {
        g_rouletteMgr->Draw();
      }

      g_renderDiagnosticLine = 3873;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      window->VisitChildren();
      g_renderDiagnosticLine = 3877;
      g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
    }
  }

  g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
  g_renderDiagnosticLine = 3886;
}

// OFFSET: 0x00A94FD0
// FUNCTION: fn_17179
void UIWindowMgr::InitializeWindowPolicy() {
  for (int i = 0; i < 10041; ++i) {
    this->policy[i] = {};
    this->policy[i].actions[3] = 1;
  }

  this->policy[15].actions[3] = 0;
  this->policy[16].actions[3] = 0;
  this->policy[17].actions[3] = 0;
  this->policy[22].actions[3] = 0;
  this->policy[23].actions[3] = 0;
  this->policy[24].actions[3] = 0;
  this->policy[25].actions[3] = 0;
  this->policy[27].actions[3] = 0;
  this->policy[28].actions[3] = 0;
  this->policy[31].actions[3] = 0;
  this->policy[32].actions[3] = 0;
  this->policy[35].actions[3] = 0;
  this->policy[39].actions[3] = 0;
  this->policy[41].actions[3] = 0;
  this->policy[43].actions[3] = 1;
  this->policy[44].actions[3] = 0;
  this->policy[45].actions[3] = 0;
  this->policy[56].actions[3] = 0;
  this->policy[70].actions[3] = 0;
  this->policy[73].actions[3] = 0;
  this->policy[74].actions[3] = 0;
  this->policy[79].actions[3] = 0;
  this->policy[80].actions[3] = 0;
  this->policy[88].actions[3] = 0;
  this->policy[90].actions[3] = 0;
  this->policy[94].actions[3] = 0;
  this->policy[99].actions[3] = 0;
  this->policy[100].actions[3] = 0;
  this->policy[105].actions[3] = 0;
  this->policy[107].actions[3] = 0;
  this->policy[109].actions[3] = 0;
  this->policy[110].actions[3] = 0;
  this->policy[111].actions[3] = 0;
  this->policy[113].actions[3] = 0;
  this->policy[119].actions[3] = 0;
  this->policy[125].actions[3] = 0;
  this->policy[160].actions[3] = 1;
  this->policy[161].actions[3] = 1;
  this->policy[174].actions[3] = 0;
  this->policy[176].actions[3] = 0;
  this->policy[178].actions[3] = 0;
  this->policy[177].actions[3] = 0;
  this->policy[179].actions[3] = 0;
  this->policy[203].actions[3] = 0;
  this->policy[157].actions[3] = 0;
  this->policy[209].actions[3] = 0;
  this->policy[210].actions[3] = 0;
  this->policy[275].actions[3] = 0;
  this->policy[315].actions[3] = 0;
  this->policy[8].actions[5] = 1;
  this->policy[22].actions[5] = 1;
  this->policy[23].actions[5] = 1;
  this->policy[24].actions[5] = 1;
  this->policy[31].actions[5] = 1;
  this->policy[10011].actions[5] = 1;
  this->policy[33].actions[5] = 1;
  this->policy[40].actions[5] = 1;
  this->policy[41].actions[5] = 1;
  this->policy[42].actions[5] = 1;
  this->policy[43].actions[5] = 1;
  this->policy[44].actions[5] = 1;
  this->policy[160].actions[5] = 1;
  this->policy[161].actions[5] = 1;
  this->policy[174].actions[5] = 1;
  this->policy[175].actions[5] = 1;
  this->policy[178].actions[5] = 1;
  this->policy[36].actions[5] = 1;
  this->policy[159].actions[5] = 1;
  this->policy[80].actions[5] = 1;
  this->policy[1].actions[6] = 1;
  this->policy[4].actions[6] = 1;
  this->policy[15].actions[6] = 1;
  this->policy[27].actions[6] = 1;
  this->policy[28].actions[6] = 1;
  this->policy[31].actions[6] = 1;
  this->policy[10011].actions[6] = 1;
  this->policy[41].actions[6] = 1;
  this->policy[55].actions[6] = 1;
  this->policy[56].actions[6] = 1;
  this->policy[84].actions[6] = 1;
  this->policy[88].actions[6] = 1;
  this->policy[100].actions[6] = 1;
  this->policy[101].actions[6] = 1;
  this->policy[113].actions[6] = 1;
  this->policy[116].actions[6] = 1;
  this->policy[120].actions[6] = 1;
  this->policy[121].actions[6] = 1;
  this->policy[128].actions[6] = 1;
  this->policy[125].actions[6] = 1;
  this->policy[130].actions[6] = 1;
  this->policy[145].actions[6] = 1;
  this->policy[168].actions[6] = 1;
  this->policy[169].actions[6] = 1;
  this->policy[174].actions[6] = 1;
  this->policy[181].actions[6] = 1;
  this->policy[191].actions[6] = 1;
  this->policy[203].actions[6] = 1;
  this->policy[16].actions[7] = 1;
  this->policy[17].actions[7] = 1;
  this->policy[23].actions[7] = 1;
  this->policy[24].actions[7] = 1;
  this->policy[25].actions[7] = 1;
  this->policy[28].actions[7] = 1;
  this->policy[31].actions[7] = 1;
  this->policy[10011].actions[7] = 1;
  this->policy[32].actions[7] = 1;
  this->policy[33].actions[7] = 1;
  this->policy[35].actions[7] = 1;
  this->policy[39].actions[7] = 1;
  this->policy[41].actions[7] = 1;
  this->policy[43].actions[7] = 1;
  this->policy[44].actions[7] = 1;
  this->policy[45].actions[7] = 1;
  this->policy[70].actions[7] = 1;
  this->policy[73].actions[7] = 1;
  this->policy[74].actions[7] = 1;
  this->policy[79].actions[7] = 1;
  this->policy[80].actions[7] = 1;
  this->policy[94].actions[7] = 1;
  this->policy[105].actions[7] = 1;
  this->policy[107].actions[7] = 1;
  this->policy[109].actions[7] = 1;
  this->policy[110].actions[7] = 1;
  this->policy[111].actions[7] = 1;
  this->policy[119].actions[7] = 1;
  this->policy[161].actions[7] = 1;
  this->policy[174].actions[7] = 1;
  this->policy[176].actions[7] = 1;
  this->policy[178].actions[7] = 1;
  this->policy[177].actions[7] = 1;
  this->policy[179].actions[7] = 1;
  this->policy[203].actions[7] = 1;
  this->policy[157].actions[7] = 1;
  this->policy[209].actions[7] = 1;
  this->policy[210].actions[7] = 1;
  this->policy[321].actions[7] = 1;
  this->policy[322].actions[7] = 1;
  this->policy[307].actions[7] = 1;
  this->policy[247].actions[7] = 1;
  this->policy[316].actions[7] = 1;
}

// OFFSET: 0x00AA5F70
// FUNCTION: fn_17227
UIWindow* UIWindowMgr::LookupWindow(int id) {
  if (g_replayAnimationScaling != 0 && id == 600) {
    return this->window_4F16C;
  }

  switch (id) {
  case 3:
    return this->window_01CC;
  case 180:
    return this->window_01D0;
  case 1:
    return this->window_01C8;
  case 27:
    return this->window_0274;
  case 30:
    return this->window_0278;
  case 28:
    return this->window_027C;
  case 34:
    return this->window_02C8;
  case 37:
    return this->window_02C4;
  case 61:
    return this->window_0324;
  case 60:
    return this->window_0320;
  case 88:
    return this->window_038C;
  case 63:
    return this->window_032C;
  case 103:
    return this->window_0388;
  case 113:
    return this->window_03C0;
  case 114:
    return this->window_03C4;
  case 125:
    return this->window_03C8;
  case 126:
    return this->window_03CC;
  case 33:
    return this->window_0288;
  case 196:
    return this->window_4F18C;
  case 204:
    return this->window_0428;
  case 203:
    return this->window_4F174;
  case 237:
    return this->window_0444;
  case 253:
    return this->window_044C;
  case 273:
    return this->window_4F1CC;
  case 277:
    return this->window_03D4;
  case 275:
    return this->window_0448;
  case 274:
    return this->window_4F1D0;
  case 284:
    return this->window_4F1E8;
  case 285:
    return this->window_4F1EC;
  case 286:
    return this->window_4F1F0;
  case 287:
    return this->window_4F1F4;
  case 288:
    return this->window_4F1F8;
  case 301:
    if (!this->snapExcluded.empty()) {
      return this->snapExcluded.front();
    }
    [[fallthrough]];
  case 318:
    return this->window_4F254;
  case 315:
    return this->window_0468;
  case 325:
    return this->window_4F25C;
  case 359:
    return this->window_4F288;
  case 10024:
    return this->window_4F2AC;
  default: {
    const auto found = this->windows.find(id);
    return found != this->windows.end() ? found->second : nullptr;
  }
  }
}


// OFFSET: 0x00AA1800
// FUNCTION: fn_17209
void UIWindowMgr::ResetWindows() {
  this->windows.clear();
  this->window_01B8 = nullptr;
  this->inputWindow = nullptr;
  this->hoverWindow = nullptr;
  this->window_01C8 = nullptr;
  this->unknown_01BC = 0;
  this->unknown_01DC = 0;
  this->unknown_01D4 = 0;
  this->unknown_01E0 = 0;
  this->unknown_01E4 = 0;
  this->unknown_01C4 = 0;
  this->unknown_01C0 = 0;
  this->window_01EC = nullptr;
  this->unknown_01F0 = 0;
  this->unknown_01F4 = 0;
  this->unknown_01F8 = 0;
  this->unknown_01FC = 0;
  this->unknown_0208 = 0;
  this->unknown_020C = 0;
  this->unknown_0210 = 0;
  this->unknown_0214 = 0;
  this->unknown_0218 = 0;
  this->unknown_021C = 0;
  this->unknown_0220 = 0;
  this->unknown_0228 = 0;
  this->unknown_022C = 0;
  this->window_0230 = nullptr;
  this->unknown_0234 = 0;
  this->unknown_0238 = 0;
  this->unknown_023C = 0;
  this->unknown_0240 = 0;
  this->unknown_0244 = 0;
  this->unknown_0248 = 0;
  this->unknown_024C = 0;
  this->unknown_0250 = 0;
  this->unknown_0254 = 0;
  this->unknown_0258 = 0;
  this->unknown_025C = 0;
  this->unknown_0260 = 0;
  this->unknown_0264 = 0;
  this->unknown_0268 = 0;
  this->unknown_026C = 0;
  this->window_0274 = nullptr;
  this->window_0278 = nullptr;
  this->window_027C = nullptr;
  this->unknown_0280 = 0;
  this->unknown_0284 = 0;
  this->window_0288 = nullptr;
  this->unknown_028C[0] = 0;
  this->unknown_028C[1] = 0;
  this->unknown_028C[2] = 0;
  this->unknown_028C[3] = 0;
  this->unknown_028C[4] = 0;
  this->unknown_028C[5] = 0;
  this->unknown_028C[6] = 0;
  this->unknown_02A8 = 0;
  this->unknown_02AC = 0;
  this->window_02C4 = nullptr;
  this->window_02C8 = nullptr;
  this->unknown_02CC = 0;
  this->unknown_02D0 = 0;
  this->unknown_02D4 = 0;
  this->unknown_02D8 = 0;
  this->unknown_02DC = 0;
  this->unknown_02E0 = 0;
  this->unknown_02E4 = 0;
  this->unknown_02E8 = 0;
  this->unknown_02EC = 0;
  this->window_01E8 = nullptr;
  this->unknown_02F0 = 0;
  this->unknown_02F4 = 0;
  this->unknown_02F8 = 0;
  this->unknown_02FC = 0;
  this->unknown_0300 = 0;
  this->unknown_0308 = 0;
  this->unknown_0304 = 0;
  this->window_03A4 = 0;
  this->unknown_03A8 = 0;
  this->window_03AC = 0;
  this->unknown_030C = 0;
  this->unknown_0310 = 0;
  this->unknown_0314 = 0;
  this->unknown_0318 = 0;
  this->unknown_031C = 0;
  this->window_0320 = nullptr;
  this->window_0324 = nullptr;
  this->unknown_0328 = 0;
  this->window_032C = nullptr;
  this->unknown_0330 = 0;
  this->unknown_0334 = 0;
  this->unknown_0338 = 0;
  this->unknown_033C = 0;
  this->unknown_0340 = 0;
  this->unknown_0344 = 0;
  this->unknown_0348 = 0;
  this->unknown_034C[0] = 0;
  this->unknown_034C[1] = 0;
  this->unknown_034C[2] = 0;
  this->unknown_034C[3] = 0;
  this->unknown_034C[4] = 0;
  this->unknown_034C[5] = 0;
  this->unknown_034C[6] = 0;
  this->unknown_034C[7] = 0;
  this->unknown_034C[8] = 0;
  this->unknown_034C[9] = 0;
  this->window_038C = nullptr;
  this->unknown_0390 = 0;
  this->unknown_0394 = 0;
  this->unknown_0200 = 0;
  this->unknown_0398 = 0;
  this->unknown_039C = 0;
  this->unknown_0380 = 0;
  this->unknown_0384 = 0;
  this->unknown_03B0 = 0;
  this->unknown_03B4 = 0;
  this->window_03C0 = nullptr;
  this->window_03C4 = nullptr;
  this->window_03C8 = nullptr;
  this->window_03CC = nullptr;
  this->unknown_03D0 = 0;
  this->unknown_03B8 = 0;
  this->unknown_03BC = 0;
  this->window_03D4 = nullptr;
  this->window_03D8 = nullptr;
  this->unknown_03E0 = 0;
  this->unknown_03E4 = 0;
  this->unknown_03E8 = 0;
  this->unknown_03F0 = 0;
  this->unknown_03EC = 0;
  this->unknown_03F8 = 0;
  this->unknown_03FC = 0;
  this->modal = 0;
  this->unknown_0404 = 0;
  this->unknown_0408 = 0;
  this->unknown_040C = 0;
  this->unknown_0410 = 0;
  this->unknown_0414 = 0;
  this->unknown_0418 = 0;
  this->unknown_041C = 0;
  this->unknown_0424 = 0;
  this->retainedWindows.clear();
  this->unknown_02B0 = 0;
  this->unknown_02B4 = 0;
  this->unknown_02B8 = 0;
  this->unknown_02BC = 0;
  this->unknown_02C0 = 0;
  this->window_4F18C = nullptr;
  this->window_4F164 = nullptr;
  this->unknown_01D8 = 0;
  this->window_0428 = nullptr;
  this->window_4F174 = nullptr;
  this->unknown_0440 = 0;
  this->window_0444 = nullptr;
  this->window_0448 = nullptr;
  this->messageBoxes.clear();
  this->window_044C = nullptr;
  this->unknown_0204 = 0;
  this->unknown_0378 = 0;
  this->unknown_0374 = 0;
  this->window_4F170 = nullptr;
  this->unknown_0450 = 0;
  this->unknown_0454 = 0;
  this->unknown_0458 = 0;
  this->unknown_045C = 0;
  this->unknown_0430 = 0;
  this->unknown_0434 = 0;
  this->unknown_0438 = 0;
  this->unknown_0460 = 0;
  this->unknown_0464 = 0;
  this->hiddenWindows.clear();
  const bool hadWindow0270 = this->window_0270 != nullptr;
  this->unknown_0508 = 0;
  this->window_4F1CC = nullptr;
  this->window_4F1D0 = nullptr;
  this->window_4F1D4 = nullptr;
  this->window_4F1E8 = nullptr;
  this->window_4F1F0 = nullptr;
  this->window_4F1F4 = nullptr;
  this->window_4F1FC = nullptr;
  this->window_4F250 = nullptr;
  this->window_4F254 = nullptr;
  this->window_4F258 = nullptr;
  this->window_01CC = nullptr;
  this->window_0468 = nullptr;
  this->window_4F25C = nullptr;
  this->unknown_042C = 0;
  this->window_4F260 = nullptr;
  this->window_4F264 = nullptr;
  this->window_4F268 = nullptr;
  this->window_4F26C = nullptr;
  this->window_4F270 = nullptr;
  this->window_4F274 = nullptr;
  this->window_4F278 = nullptr;
  this->window_4F27C = nullptr;
  this->unknown_0420 = 0;
  this->unknown_043C = 0;
  this->window_01D0 = nullptr;
  this->unknown_0224 = 0;
  if (hadWindow0270) {
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(292, 0, 0, 0, 0);
    }
  }
  const bool hadWindow276 = this->window_4F1E4 != nullptr;
  this->window_0270 = nullptr;
  this->unknown_037C = 0;
  this->window_0388 = nullptr;
  this->unknown_03A0 = 0;
  if (hadWindow276) {
    this->visible_276 = 0;
    this->window_4F1E4 = nullptr;
  }
  this->window_4F1F8 = nullptr;
  this->window_0A30 = nullptr;
  this->window_4F2AC = nullptr;
}

// OFFSET: 0x00A88490
// FUNCTION: fn_17086
UIWindowMgr::UIWindowMgr()
    : activeWindowId(0), retainedPersistentWindow(false), window_03DC(nullptr), unknown_046C(-1), unknown_0470(-1), unknown_0526(true),
      window_0A30(nullptr), unknown_4F210(false), unknown_4F211(false), window_4F2AC(nullptr) {
  this->ResetWindows();
  this->unknown_0478 = nullptr;
  this->unknown_047C = 0;
  this->unknown_0480 = 0;
  this->unknown_04DC.assign("", 0);
  this->unknown_004C = 1;
  this->unknown_0038 = 98;
  this->unknown_0030 = 0;
  this->unknown_0034 = 0;
  this->unknown_003C = 1;
  this->unknown_0050 = 1.0f;
  this->unknown_0054 = 1;
  this->unknown_005C = 0xAAFFFFFFu;
  this->unknown_0060 = 1;
  this->unknown_04F8 = 0;
  this->unknown_04F4 = 0;
  this->unknown_04FC = 0;
  this->unknown_03F4 = 1;
  this->unknown_03F6 = 0;
  this->unknown_0068 = g_language == 2;
  this->unknown_0048 = 0;
  this->unknown_049C = 0;
  this->unknown_049E = g_serviceType != 0 ? 0 : -25;
  this->unknown_04A0 = {};
  this->unknown_04A4 = {};
  this->unknown_04A8 = {};
  this->unknown_04AC = {};
  this->unknown_04B0 = 0;
  this->unknown_04B4 = 203;
  this->unknown_04B8 = 115;
  this->unknown_04BC = 0;
  this->unknown_04C0 = 0;
  this->windowHistory.clear();
  this->lastWindowId = -1;
  this->inputSuggestionsEnabled = 0;
  this->chatModeChanged = 1;
  this->unknown_0508 = 0;
  this->window_4F168 = nullptr;
  this->window_4F164 = nullptr;
  this->window_4F16C = nullptr;
  this->InitializeWindowPolicy();
  this->window_4F174 = nullptr;
  this->activeBrowser = nullptr;
  this->specialBrowser = nullptr;
  this->currentHitUid = 0;
  this->previousHitUid = 0;
  this->visible_276 = 0;
  this->window_4F1EC = nullptr;
  this->window_4F250 = nullptr;
  this->unknown_0058 = 0;
  this->unknown_4F238 = 151;
  this->unknown_4F23C = 37;
}

// OFFSET: 0x007375E0
// FUNCTION: fn_07649
__declspec(noinline) CMiniPartyWndAreaMgr::CMiniPartyWndAreaMgr() {
  SetRect(&this->area, 0, 0, g_windowMgr.unknown_047C, g_windowMgr.unknown_0480);
}

// OFFSET: 0x00AA66D0
// FUNCTION: fn_17233
void UIWindowMgr::Shutdown() {
  if (!this->modalWindows.empty() && this->modalWindows.back()) {
    this->activeWindowId = 185;
  }

  if (this->window_0A30 && this->window_0A30->geometry.visible) {
    static_cast<CUIDropDownBox*>(this->window_0A30)->Detach();
    this->window_0A30->Show(0);
  }

  for (UIWindow*& window : this->rendered) {
    if (this->HasModalWindow() && this->persistentWindows.find(window) != this->persistentWindows.end()) {
      this->retainedPersistentWindow = true;
    } else if (!this->HasModalWindow() || std::find(this->modalWindows.begin(), this->modalWindows.end(), window) == this->modalWindows.end()) {
      if (this->persistentWindows.find(window) != this->persistentWindows.end()) {
        this->persistentWindows.erase(window);
      }
      window->Slot11();
      delete window;
      window = nullptr;
    }
  }

  for (auto& entry : this->browsers) {
    delete entry.second;
    entry.second = nullptr;
  }
  this->browsers.clear();
  if (this->specialBrowser) {
    delete this->specialBrowser;
    this->specialBrowser = nullptr;
  }

  for (auto entry = this->overlays.begin(); entry != this->overlays.end();) {
    delete entry->second;
    entry = this->overlays.erase(entry);
  }
  this->rendered.clear();
  this->retired.clear();
  this->snapRectangles.erase(this->snapRectangles.begin(), this->snapRectangles.end());
  this->messageBoxes.clear();
  this->ResetWindows();

  for (UIWindow*& window : this->snapExcluded) {
    this->Destroy(window);
    window = nullptr;
  }
  this->snapExcluded.clear();
}

// OFFSET: 0x00AA6B80
// FUNCTION: fn_17234
void UIWindowMgr::ClearMapWindows() {
  if (!this->modalWindows.empty() && this->modalWindows.back()) {
    this->activeWindowId = 185;
  }

  this->SaveVisibleWindowIds();
  if (!this->window_01C8) {
    this->Shutdown();
    return;
  }

  if (this->window_0A30 && this->window_0A30->geometry.visible) {
    static_cast<CUIDropDownBox*>(this->window_0A30)->Detach();
    this->window_0A30->Show(0);
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end();) {
    bool unregistered = true;
    for (unsigned index = 0; index < this->retainedWindows.size(); ++index) {
      if (*entry == this->retainedWindows[static_cast<int>(index)]) {
        unregistered = false;
      }
    }
    if (!unregistered) {
      ++entry;
      continue;
    }

    UIWindow* window = *entry;
    if (this->window_01C8 == window || window == this->window_4F1EC || window->geometry.id == 67) {
      ++entry;
      continue;
    }

    if (window == reinterpret_cast<UIWindow*>(this->unknown_03F8) && g_session.unknown_593C != 0) {
      g_session.unknown_5940 = window->geometry.x;
      g_session.unknown_5944 = window->geometry.y;
    }

    if (!this->modalWindows.empty() && this->modalWindows.back()) {
      if (this->persistentWindows.find(window) != this->persistentWindows.end()) {
        this->retainedPersistentWindow = true;
        ++entry;
        continue;
      }
      if (std::find(this->modalWindows.begin(), this->modalWindows.end(), window) != this->modalWindows.end()) {
        ++entry;
        continue;
      }
    }

    this->persistentWindows.erase(window);
    window->Slot11();
    delete window;
    entry = this->rendered.erase(entry);
  }

  for (auto entry = this->retired.begin(); entry != this->retired.end();) {
    bool unregistered = true;
    for (unsigned index = 0; index < this->retainedWindows.size(); ++index) {
      if (*entry == this->retainedWindows[static_cast<int>(index)]) {
        unregistered = false;
      }
    }
    if (!unregistered || this->window_01C8 == *entry) {
      ++entry;
      continue;
    }

    entry = this->retired.erase(entry);
  }

  for (auto entry = this->snapRectangles.begin(); entry != this->snapRectangles.end();) {
    if (this->window_01C8 == entry->first) {
      ++entry;
    } else {
      entry = this->snapRectangles.erase(entry);
    }
  }

  std::uint32_t saved03E4 = 0;
  if (this->unknown_03E4) {
    saved03E4 = this->unknown_03E4;
  }
  std::uint32_t saved03E8 = 0;
  if (this->unknown_03E8) {
    saved03E8 = this->unknown_03E8;
  }
  std::uint32_t saved03F0 = 0;
  if (this->unknown_03F0) {
    saved03F0 = this->unknown_03F0;
  }
  UIWindow* saved01C8 = nullptr;
  if (this->window_01C8) {
    saved01C8 = this->window_01C8;
  }

  std::map<int, UIWindow*> savedWindows(this->retainedWindows);
  this->ResetWindows();
  this->window_01C8 = saved01C8;
  this->unknown_03E4 = saved03E4;
  this->unknown_03F0 = saved03F0;
  this->unknown_03E8 = saved03E8;
  this->retainedWindows = savedWindows;

  for (UIWindow*& window : this->snapExcluded) {
    this->Destroy(window);
    window = nullptr;
  }
  this->snapExcluded.clear();
}

// OFFSET: 0x00A8A1C0
// FUNCTION: fn_17099
UIWindowMgr::~UIWindowMgr() {
  if (!g_miniPartyWndAreaMgr) {
    g_miniPartyWndAreaMgr = new (std::nothrow) CMiniPartyWndAreaMgr;
  }
  if (g_miniPartyWndAreaMgr) {
    delete g_miniPartyWndAreaMgr;
    g_miniPartyWndAreaMgr = nullptr;
  }

  this->Shutdown();
  if (this->unknown_0478) {
    delete this->unknown_0478;
    this->unknown_0478 = nullptr;
  }
  for (auto& entry : this->savedRectangles) {
    delete entry.second;
  }
  this->savedRectangles.clear();
}

// OFFSET: 0x00A8B0A0
// FUNCTION: fn_17121
void UIWindowMgr::SelectWhisperName(const char* name) {
  if (!name || !this->window_01C8) {
    return;
  }

  char* list = *reinterpret_cast<char**>(reinterpret_cast<char*>(this->window_01C8) + 0xC0);
  std::vector<std::string>& names = *reinterpret_cast<std::vector<std::string>*>(list + 0x11C);
  int index = 0;
  bool found = false;
  for (const std::string& entry : names) {
    if (entry == name) {
      found = true;
      break;
    }
    ++index;
  }

  if (!found) {
    names.push_back(std::string(name));
  }

  UIWindow* field = *reinterpret_cast<UIWindow**>(reinterpret_cast<char*>(this->window_01C8) + 0xC0);
  *reinterpret_cast<int*>(reinterpret_cast<char*>(field) + 0x128) = index;
  this->window_01C8->SendMsg(field, 0x27, index, 0, 0, 0);
}

// GLOBAL: gl_25163
const char* g_newWhisperWave = "..\\..\\BGM\\Msg_NewMsg.wav";

// OFFSET: 0x00A8B3D0
// FUNCTION: fn_17123
int UIWindowMgr::ShowWhisperFrom(const char* name, const char* text, int color, std::uint32_t accountId) {
  int shown = 0;
  const bool isFriend = g_session.IsFriendName(name) != 0;
  if ((g_session.m_autoOpenWhisperWindow != 0 && !isFriend) || (g_session.m_autoOpenFriendWhisperWindow != 0 && isFriend)) {
    const char* label = nullptr;
    if (isFriend) {
      label = LookupMessage(102);
    } else if (g_session.IsGuildMemberCharacter(accountId)) {
      label = "GuildMember";
    } else if (g_session.IsFriendNameDifferent(name)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(917)), 0xFFFF, 0, 0, 0);
    } else if (g_session.IsGuildMemberNameDifferent(name)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(919)), 0xFFFF, 0, 0, 0);
    }

    shown = this->SendNotice(14, reinterpret_cast<std::intptr_t>(name), reinterpret_cast<std::intptr_t>(label), reinterpret_cast<std::intptr_t>(ShortcutFilename(accountId).c_str()), 0, 0);
    if (shown) {
      if (ResourceMgr::Instance().Exists(g_newWhisperWave)) {
        g_soundMgr->PlayWave(g_newWhisperWave, 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      } else if (g_session.m_whisperOpenSound != 0) {
        g_soundMgr->PlayWave("버튼소리.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
    }
  }

  if (std::strcmp(name, g_session.DecodePlayerName()) == 0) {
    return 0;
  }

  if (g_session.m_autoOpenWhisperWindow != 0 && shown) {
    this->AddWhisperName(name);
  }

  const auto entry = this->namedWindows.find(std::string(name));
  if (entry == this->namedWindows.end()) {
    return 0;
  }

  UIWindow* window = entry->second;
  window->SendMsg(nullptr, 37, reinterpret_cast<std::intptr_t>(text), color, 2, 0);
  if ((this->modal == nullptr || this->window_4F174 == window) && !this->LookupWindow(278) && !this->LookupWindow(4) && !this->LookupWindow(199) &&
      !this->LookupWindow(200)) {
    int companion = -1;
    if (window->geometry.id == 12) {
      companion = 234;
    } else if (window->geometry.id == 82) {
      companion = 250;
    }

    if (companion != -1) {
      const auto found = std::find_if(this->rendered.begin(), this->rendered.end(), [&](const UIWindow* value) { return value->geometry.id == companion; });
      if (found != this->rendered.end()) {
        UIWindow* value = *found;
        this->rendered.erase(found);
        this->rendered.push_back(value);
      }
    }

    const auto found = std::find(this->rendered.begin(), this->rendered.end(), window);
    if (found != this->rendered.end()) {
      this->rendered.erase(found);
      this->rendered.push_back(window);
    }
  }
  return 1;
}

// OFFSET: 0x00A8B240
// FUNCTION: fn_17122
void UIWindowMgr::AddWhisperName(const char* name) {
  if (!this->window_01C8) {
    return;
  }

  char* list = *reinterpret_cast<char**>(reinterpret_cast<char*>(this->window_01C8) + 0xC0);
  std::vector<std::string>& names = *reinterpret_cast<std::vector<std::string>*>(list + 0x11C);
  for (const std::string& entry : names) {
    if (entry == name) {
      return;
    }
  }

  names.push_back(std::string(name));
  const unsigned count = static_cast<unsigned>(names.size());
  unsigned& current = *reinterpret_cast<unsigned*>(*reinterpret_cast<char**>(reinterpret_cast<char*>(this->window_01C8) + 0xC0) + 0x128);
  if (count - 1 <= current) {
    current = count;
  }
}

// OFFSET: 0x00A8BB20
// FUNCTION: fn_17127
void UIWindowMgr::RefreshFrameSkins() {
  for (UIWindow* window : this->rendered) {
    if (CUIRegisterMgr::s_instance == nullptr) {
      CUIRegisterMgr::s_instance = new (std::nothrow) CUIRegisterMgr;
    }
    auto& entries = CUIRegisterMgr::s_instance->entries;
    if (entries.find(window->geometry.id) != entries.end()) {
      static_cast<CUIFrameWnd*>(window)->RefreshSkinColors();
    }
  }
}

// OFFSET: 0x00A8B840
// FUNCTION: fn_17124
__declspec(noinline) void UIWindowMgr::RegisterNamedWindow(std::string name, UIWindow* window) {
  if (this->namedWindows.find(name) == this->namedWindows.end()) {
    this->namedWindows[name] = window;
  }
}

// OFFSET: 0x00AA9B00
// FUNCTION: fn_17254
void UIWindowMgr::NotifyMode() {
  if (CMode* mode = g_modeMgr.GetGameMode()) {
    mode->SendMsg(292, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00A8AD60
// FUNCTION: fn_17116
void UIWindowMgr::ShowTransient(UIWindow* window) {
  if (this->window_4F250) {
    this->Schedule(this->window_4F250);
  }

  this->window_4F250 = window;
  window->geometry.targetAlpha = 128;
  window->geometry.alphaStart = timeGetTime();
  window = this->window_4F250;
  this->rendered.push_back(window);
}

// OFFSET: 0x00A8ADB0
// FUNCTION: fn_17117
void UIWindowMgr::RequestAddFriend(const char* name) {
  AddFriendPacket packet;
  packet.id = ID_CZ_ADD_FRIENDS;
  std::memcpy(packet.name, name, sizeof(packet.name));
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
}

// OFFSET: 0x00A8AE20
// FUNCTION: fn_17118
void UIWindowMgr::AddOverlay(int id, UIWindow* window) {
  this->overlays.emplace(id, window);
}

// OFFSET: 0x00A8B9F0
// FUNCTION: fn_17126
void UIWindowMgr::AddRenderedFront(UIWindow* window) {
  this->rendered.push_front(window);
}

// OFFSET: 0x00A8C150
// FUNCTION: fn_17129
__declspec(noinline) void UIWindowMgr::ShowWindows189(unsigned char show) {
  for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    if ((*it)->geometry.id != 189) {
      continue;
    }

    if (show == 1) {
      (*it)->Show(g_serviceOptions.Get(250) ? 1 : 0);
    } else {
      (*it)->Show(0);
    }
  }
}

// OFFSET: 0x00A8C310
// FUNCTION: fn_17131
void UIWindowMgr::DestroyPersistentWindows() {
  while (!this->modalWindows.empty()) {
    UIWindow* window = this->modalWindows.back();
    this->persistentWindows.erase(window);
    delete window;
    this->modalWindows.pop_back();
  }

  for (UIWindow* window : this->persistentWindows) {
    delete window;
  }
  this->persistentWindows.clear();
}

// OFFSET: 0x00A8CD70
// FUNCTION: fn_17140
void UIWindowMgr::RemoveSnapRectangle(UIWindow* window) {
  if (g_serviceOptions.Get(141)) {
    this->snapRectangles.erase(window);
  }
}

// OFFSET: 0x00A8CDE0
// FUNCTION: fn_17141
void UIWindowMgr::ReleaseRetainedWindow(int id) {
  const auto entry = this->retainedWindows.find(id);
  if (entry != this->retainedWindows.end()) {
    this->Schedule(entry->second);
    this->retainedWindows.erase(entry);
  }
}

// OFFSET: 0x00A8CE50
// FUNCTION: fn_17142
void UIWindowMgr::UnregisterNamedWindow(std::string name) {
  const auto entry = this->namedWindows.find(name);
  if (entry != this->namedWindows.end()) {
    this->namedWindows.erase(entry);
  }
}

// OFFSET: 0x00AAD8E0
// FUNCTION: fn_17285
void UIWindowMgr::SendMsgToNamedWindow(const char* name) {
  if (this->namedWindows.find(name) != this->namedWindows.end()) {
    this->namedWindows[name]->SendMsg(nullptr, 48, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00AAA280
// FUNCTION: fn_17267
void UIWindowMgr::SetWindows189Visible(unsigned char show) {
  for (auto it = this->rendered.begin(); this->rendered.end() != it; ++it) {
    if ((*it)->geometry.id == 189) {
      (*it)->Show(show);
    }
  }
}

// OFFSET: 0x00AAA2D0
// FUNCTION: fn_17268
void UIWindowMgr::ShowWindowsWithId(int id, unsigned char show) {
  for (UIWindow* window : this->rendered) {
    if (id == window->geometry.id) {
      window->Show(show);
    }
  }
}

namespace {

#pragma pack(push, 1)
struct ReplaySettingHeader {
  /* 0000 */ std::int16_t tag;
  /* 0002 */ int size;

  ReplaySettingHeader() : tag(0), size(0) {
  }

  ~ReplaySettingHeader() {
    this->tag = 0;
    this->size = 0;
  }

  void Read(const unsigned char* source) {
    this->size = 0;
    this->tag = 0;
    std::memcpy(this, source, sizeof(*this));
  }
};
#pragma pack(pop)

static_assert(sizeof(ReplaySettingHeader) == 6);

}

// OFFSET: 0x00AA80A0
// FUNCTION: fn_17240
void UIWindowMgr::CenterSurface0478() {
  this->unknown_04FC = 0;
  if (this->unknown_0478) {
    int x = static_cast<int>(GetRenderSystemHolder()->Width() - this->unknown_047C) / 2;
    int height = this->unknown_0480;
    int y = static_cast<int>(GetRenderSystemHolder()->Height() - height) / 2;
    reinterpret_cast<CSurface*>(this->unknown_0478)->Stretch(x, y, this->unknown_047C, height);
  }
}

// OFFSET: 0x00AA8130
// FUNCTION: fn_17242
std::vector<unsigned char> UIWindowMgr::SaveReplaySettings() {
  std::vector<unsigned char> data;
  {
    std::vector<unsigned char> chunk(6);
    *reinterpret_cast<std::int16_t*>(&chunk[0]) = 25000;
    *reinterpret_cast<int*>(&chunk[2]) = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  {
    std::vector<unsigned char> chunk(10);
    *reinterpret_cast<std::int16_t*>(&chunk[0]) = 25010;
    *reinterpret_cast<int*>(&chunk[2]) = 4;
    *reinterpret_cast<int*>(&chunk[6]) = this->unknown_0064;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  {
    std::vector<unsigned char> chunk(6);
    *reinterpret_cast<std::int16_t*>(&chunk[0]) = 25001;
    *reinterpret_cast<int*>(&chunk[2]) = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  return data;
}

// OFFSET: 0x00AA8440
// FUNCTION: fn_17243
std::vector<unsigned char> UIWindowMgr::SaveReplayReserved() {
  std::vector<unsigned char> data;
  {
    std::vector<unsigned char> chunk(6);
    *reinterpret_cast<std::int16_t*>(&chunk[0]) = 26000;
    *reinterpret_cast<int*>(&chunk[2]) = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  {
    std::vector<unsigned char> chunk(6);
    *reinterpret_cast<std::int16_t*>(&chunk[0]) = 26001;
    *reinterpret_cast<int*>(&chunk[2]) = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  return data;
}

// OFFSET: 0x00AA8980
// FUNCTION: fn_17245
void UIWindowMgr::ApplyReplayOptions(const ReplayOptions& options) {
  this->unknown_05A4 = 0;
  this->unknown_0588 = 0;
  this->unknown_0568 = 0;
  this->unknown_054C = 0;
  this->unknown_073C = 0;
  this->unknown_077C = 0;
  this->unknown_05BC = 0;
  this->unknown_05D0 = 0;
  this->m_normalItemSlotWindow.show = options.unknown_0007;
  this->unknown_003C = options.recordRestricted;
}

// OFFSET: 0x00AA8B20
// FUNCTION: fn_17247
int UIWindowMgr::LoadReplaySettings(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return -1;
  }

  ReplaySettingHeader header;
  unsigned offset = 6;
  header.Read(data.data());
  if (header.tag != 25000) {
    offset = static_cast<unsigned>(-1);
  } else {
    while (offset < data.size()) {
      header.Read(data.data() + offset);
      offset += 6;
      if (header.tag == 25001) {
        break;
      }
      if (header.tag == 25010) {
        std::memcpy(&this->unknown_0064, data.data() + offset, header.size);
      }
      offset += header.size;
    }
  }
  return static_cast<int>(offset);
}

// OFFSET: 0x00AA8C30
// FUNCTION: fn_17248
int UIWindowMgr::LoadReplayReserved(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return -1;
  }

  ReplaySettingHeader header;
  unsigned offset = 6;
  header.Read(data.data());
  if (header.tag != 26000) {
    offset = static_cast<unsigned>(-1);
  } else {
    while (offset < data.size()) {
      header.Read(data.data() + offset);
      offset += 6;
      if (header.tag == 26001) {
        break;
      }
      offset += header.size;
    }
  }
  return static_cast<int>(offset);
}

// OFFSET: 0x00AA55C0
// FUNCTION: fn_17223
bool UIWindowMgr::ProcessKeyDown(int key, int keyData, int pressed) {
  this->unknown_0025 = 1;
  if (g_modeMgr.GetGameMode() == nullptr) {
    return this->ProcessHotkeyAction(key, pressed);
  }

  if (this->LookupWindow(0x9C) != nullptr) {
    this->unknown_0025 = 0;
  } else if (!this->inputSuggestionsEnabled) {
    UIWindow* hover = this->hoverWindow;
    UIWindow* parent = hover ? hover->parent : nullptr;
    if (!hover || !parent || parent == this->window_01C8 || !parent->geometry.visible) {
      this->unknown_0025 = 0;
    }
  }

  int action = this->TranslateHotkey(key, keyData);
  if (this->unknown_0404 != nullptr) {
    static_cast<UIHotKeyWnd*>(this->unknown_0404)->AssignSelectedHotkey(action);
    return false;
  }

  bool hoverOnButton = false;
  if (this->inputSuggestionsEnabled) {
    const UIWindow* window = this->window_01C8;
    if (window) {
      UIWindow* const* buttons = reinterpret_cast<UIWindow* const*>(reinterpret_cast<const char*>(window) + 0xBC);
      if (this->hoverWindow == buttons[0] || this->hoverWindow == buttons[1]) {
        hoverOnButton = true;
      }
    }
  }
  if (!hoverOnButton) {
    UIWindow* hover = this->hoverWindow;
    if (hover) {
      UIWindow* parent = hover->parent;
      if (parent && parent != this->window_01C8 && parent->geometry.visible) {
        hoverOnButton = true;
      }
    }
  }

  if (hoverOnButton) {
    if (this->modifierKey == 0) {
      switch (this->keyCode) {
      case 0x09:
      case 0x0D:
      case 0x10:
      case 0x11:
      case 0x12:
      case 0x14:
      case 0x2D:
      case 0x70:
      case 0x71:
      case 0x72:
      case 0x73:
      case 0x74:
      case 0x75:
      case 0x76:
      case 0x77:
      case 0x78:
      case 0x79:
      case 0x7A:
      case 0x7B:
        break;
      default:
        return this->ProcessHotkeyAction(key, pressed);
      }
    } else if (this->modifierKey == 0x10) {
      return this->ProcessHotkeyAction(key, pressed);
    } else if (this->modifierKey == 0x11) {
      switch (this->keyCode) {
      case 0x41:
      case 0x43:
      case 0x56:
      case 0x58:
        return this->ProcessHotkeyAction(key, pressed);
      }
    }
  }

  int& lastKey = g_session.m_currentHotkey;
  if (lastKey != key) {
    if (pressed == 0) {
      return this->ProcessHotkeyAction(key, pressed);
    }
    g_session.m_automaticSkills.DisableAll();
  }
  lastKey = key;

  if ((key == 0x0D || key == 0x20) && this->ProcessEnterKey()) {
    return true;
  }
  if (!this->ProcessShortcutKey(action)) {
    return this->ProcessHotkeyAction(key, pressed);
  }
  if (this->IsHoverParentVisible() || this->IsHoverOnWindow01C8Button()) {
    this->unknown_0025 = 1;
  }
  return true;
}

// OFFSET: 0x00AA57E0
// FUNCTION: fn_17224
void UIWindowMgr::SendEmotionHotkey(int index) {
  char text[256];
  const EmotionHotkeys* emotions = &g_session.m_emotionHotkeys;
  std::strcpy(text, emotions->current[index].c_str());

  if (g_windowMgr.unknown_01DC != nullptr) {
    const bool linked = HasTextLinks(std::string(text));
    if (linked) {
      g_windowMgr.Confirm(LookupMessage(0xE53), 0, true, false, 0, "Message", 280, 120, 0);
      return;
    }
  }

  CMode* mode = g_modeMgr.gameMode;
  if (text[0] == '/') {
    if (g_session.ProcessEmotionShortcut(text) == 0) {
      std::string parts[3];
      int talkType;
      const int offset = g_session.GetTalkType(text, &talkType, parts);
      if (offset != -1) {
        g_windowMgr.unknown_04DC = text + offset;
      }
      g_modeMgr.gameMode->SendMsg(0x2A, talkType, reinterpret_cast<int>(parts), 0, 0);
    }
    return;
  }

  g_windowMgr.unknown_04DC = text;
  const int& chatTarget = *reinterpret_cast<const int*>(&g_session.unknown_5490[0x8]);
  if (chatTarget == 2) {
    if (g_guildInfo.m_guildId != 0) {
      mode->SendMsg(0x81, 0, 0, 0, 0);
      return;
    }
  } else {
    if (chatTarget == 1 && g_session.PartyCount() != 0) {
      mode->SendMsg(0x42, 0, 0, 0, 0);
      return;
    }
    if (chatTarget == 3 && g_clanInfo->m_hasClan) {
      mode->SendMsg(0x121, 0, 0, 0, 0);
      return;
    }
  }
  mode->SendMsg(6, 0, 0, 0, 0);
}

// OFFSET: 0x00AA5B00
// FUNCTION: fn_17225
void UIWindowMgr::CycleWindowVisibility(int) {
  for (std::list<UIWindow*>::iterator it = this->hiddenWindows.begin(); it != this->hiddenWindows.end();) {
    std::list<UIWindow*>::iterator next = it;
    ++next;
    if (std::find(this->rendered.begin(), this->rendered.end(), *it) == this->rendered.end()) {
      this->hiddenWindows.erase(it);
    }
    it = next;
  }

  const int state = this->unknown_0508;
  if (state == 0) {
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 222, 0, 0, 0);
    }
    if (this->hiddenWindows.size() == 0) {
      for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
        if ((*it)->geometry.visible) {
          this->hiddenWindows.push_back(*it);
        }
      }
    }
    this->unknown_0508 = 1;
  } else if (state == 1) {
    bool closed = false;
    for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
      UIWindow* window = *it;
      if (window->geometry.visible) {
        const int id = window->geometry.id;
        if (!this->IsOrderFixed(id)) {
          this->CloseWindow(id);
          closed = true;
        } else if (std::find(this->hiddenWindows.begin(), this->hiddenWindows.end(), window) == this->hiddenWindows.end()) {
          this->hiddenWindows.push_back(*it);
        }
      }
    }
    if (closed) {
      if (this->unknown_01DC) {
        this->unknown_01DC->SendMsg(nullptr, 6, 222, 0, 0, 0);
      }
      return;
    }

    for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
      UIWindow* window = *it;
      if (window != reinterpret_cast<UIWindow*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->EnsureBroadcastBalloon())) {
        window->Show(0);
      }
    }
    this->CloseWindow(14);
    CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    if (player) {
      player->SendMsg(nullptr, 109, 0, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      player->unknown_0544 = 0;
    }
    this->unknown_0508 = 2;
  } else if (state == 2) {
    bool closed = false;
    for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
      if (*it != reinterpret_cast<UIWindow*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->EnsureBroadcastBalloon()) &&
          *it != static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_05FC && (*it)->geometry.visible) {
        UIWindow* window = *it;
        if (std::find(this->hiddenWindows.begin(), this->hiddenWindows.end(), window) != this->hiddenWindows.end()) {
          window->Show(0);
        } else {
          this->CloseWindow(window->geometry.id);
        }
        closed = true;
      }
    }
    if (closed) {
      this->ShowWindows189(0);
      if (this->unknown_01DC) {
        this->unknown_01DC->SendMsg(nullptr, 6, 222, 0, 0, 0);
      }
      return;
    }

    for (std::list<UIWindow*>::iterator it = this->hiddenWindows.begin(); it != this->hiddenWindows.end(); ++it) {
      (*it)->Show(1);
    }
    for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
      if ((*it)->geometry.id == 189) {
        (*it)->Show(g_serviceOptions.Get(250) ? 1 : 0);
      }
    }
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode && mode->unknown_0254 == 1) {
      this->EnsureWindow(14);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->RefreshEffect164();
    this->unknown_0508 = 1;
  }
}

// OFFSET: 0x00AA8D10
// FUNCTION: fn_17249
int UIWindowMgr::LoadReplayWindows(const std::vector<unsigned char>& data) {
  if (data.empty()) {
    return -1;
  }

  ReplaySettingHeader header;
  std::vector<unsigned char> payload;
  unsigned offset = 6;
  header.Read(data.data());
  if (header.tag != 20000) {
    offset = static_cast<unsigned>(-1);
  } else {
    while (offset < data.size()) {
      header.Read(data.data() + offset);
      offset += 6;
      if (header.tag == 20001) {
        break;
      }
      if (static_cast<unsigned short>(header.tag - 20100) <= 399) {
        UIWindow* window = this->EnsureWindow(header.tag - 20100);
        if (window && header.size > 0) {
          payload.resize(header.size);
          std::memcpy(payload.data(), data.data() + offset, header.size);
          offset += header.size;
          window->SendMsg(nullptr, 123, reinterpret_cast<std::intptr_t>(payload.data()), payload.size(), 0, 0);
        }
      }
    }
  }
  return static_cast<int>(offset);
}

// OFFSET: 0x00AA9100
// FUNCTION: fn_17253
int UIWindowMgr::SendNotice(int selector, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d, std::intptr_t e) {
  const char* text = reinterpret_cast<const char*>(a);
  switch (selector) {
  case 154:
    for (auto it = this->namedWindows.begin(); it != this->namedWindows.end(); ++it) {
      it->second->SendMsg(nullptr, 154, a, 0, 0, 0);
    }
    break;
  case 14: {
    std::string name(text);
    if (this->namedWindows.find(name) == this->namedWindows.end()) {
      UIWhisperWnd* window = new UIWhisperWnd();
      window->SetSize(280, 120);
      this->rendered.push_back(window);
      window->SendMsg(nullptr, 34, reinterpret_cast<std::intptr_t>(&this->m_whisperPlacement), 0, 0, 0);
      window->Show(1);
      window->m_target = name;
      window->SetId(67);
      if (b == 0) {
        window->m_name = name;
      } else {
        window->m_name = name + "  (" + reinterpret_cast<const char*>(b) + ")  *^_^*";
      }
      if (c != 0) {
        window->m_code = std::string(reinterpret_cast<const char*>(c));
      }
      const int count = static_cast<int>(this->namedWindows.size());
      if (count != 0) {
        const int offset = count * 17;
        const int x = this->m_whisperPlacement.x + offset;
        const int y = this->m_whisperPlacement.y + offset;
        if (x > 0 && this->m_whisperPlacement.width + x < g_renderer->viewport.width - 17 && y > 0 &&
            this->m_whisperPlacement.height + y < g_renderer->viewport.height - 17) {
          window->Move(x, y);
        } else if (offset > 0 && this->m_whisperPlacement.width + offset < g_renderer->viewport.width - 17 &&
                   this->m_whisperPlacement.height + offset < g_renderer->viewport.height - 17) {
          window->Move(offset, offset);
        }
      }
      this->RegisterNamedWindow(name, window);
      this->AddWhisperName(name.c_str());
      return 1;
    }
    break;
  }
  case 7:
    if (this->window_027C != nullptr) {
      static_cast<UIChatRoomWnd*>(this->window_027C)->SaveChatLog();
    }
    break;
  case 6:
    if (this->window_01C8 != nullptr) {
      static_cast<UINewChatWnd*>(this->window_01C8)->SaveChatLogs();
    }
    break;
  case 5:
    if (this->window_027C != nullptr) {
      this->window_027C->SendMsg(nullptr, 37, a, b, 0, 0);
    }
    break;
  case 1:
  case 19: {
    if (g_replayAnimationScaling != 0 && !Replay::Instance().header.options.recordRestricted) {
      break;
    }
    std::uint32_t color = static_cast<std::uint32_t>(b);
    if (this->window_01C8 == nullptr) {
      if (color == 0) {
        color = 0xFFFFFF;
      }
      std::vector<std::string> lines;
      WrapPlainText(text, lines, 94);
      for (unsigned int index = 0; index < lines.size(); ++index) {
        std::string line;
        line = lines[index];
        this->unknown_04C4.push_back({line, color});
      }
    } else {
      if (color == 0) {
        color = 0xFFFFFF;
      }
      if (std::strcmp(text, "No Msg") != 0 && std::strcmp(text, "NO MSG") != 0) {
        this->window_01C8->SendMsg(nullptr, selector == 19 ? 115 : 37, a, color, c, d);
      }
    }
    break;
  }
  case 4:
    if (this->unknown_01BC != nullptr) {
      this->unknown_01BC->SendMsg(nullptr, 17, a, 0, 0, 0);
    }
    break;
  case 3:
    this->ShowWindowById(1, 1);
    if (this->window_01C8 != nullptr) {
      this->window_01C8->SendMsg(nullptr, 16, 0, 0, 0, 0);
    }
    break;
  case 2:
    if (this->window_01C8 != nullptr) {
      this->window_01C8->SendMsg(nullptr, 15, a, 0, 0, 0);
    }
    break;
  case 8:
  case 9:
    this->EnsureWindow(51)->SendMsg(nullptr, 65, a, b, 0, 0);
    break;
  case 10:
    this->CloseWindow(51);
    break;
  }
  return 0;
}

// OFFSET: 0x00AA8F50
// FUNCTION: fn_17252
__declspec(noinline) void UIWindowMgr::BroadcastMessage21() {
  UIWindow* window = this->window_03DC;
  if (window || (window = this->unknown_03B0) != nullptr) {
    window->SendMsg(nullptr, 21, 0, 0, 0, 0);
    return;
  }

  for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    if ((*it)->Slot02()) {
      (*it)->SendMsg(nullptr, 21, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00AB1120
// FUNCTION: fn_17332
__declspec(noinline) bool UIWindowMgr::IsHoverOnWindow01C8Button() {
  const UIWindow* window = this->window_01C8;
  if (window) {
    UIWindow* const* buttons = reinterpret_cast<UIWindow* const*>(reinterpret_cast<const char*>(window) + 0xBC);
    if (this->hoverWindow == buttons[0] || this->hoverWindow == buttons[1]) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x007D5250
// FUNCTION: fn_09589
__declspec(noinline) bool UIWindowMgr::HasModalWindow() {
  UIWindow* top = this->modalWindows.empty() ? nullptr : this->modalWindows.back();
  return top != nullptr;
}

// OFFSET: 0x00A8BC00
// FUNCTION: fn_17128
int UIWindowMgr::TranslateHotkey(int key, int keyData) {
  if (!this->modalWindows.empty() && this->modalWindows.back() && this->unknown_0404) {
    return 1;
  }

  if (key == VK_PROCESSKEY && keyData == 0x100001) {
    this->keyCode = 'Q';
  } else if (key == VK_PROCESSKEY && keyData == 0x110001) {
    this->keyCode = 'W';
  } else if (key == VK_PROCESSKEY && keyData == 0x120001) {
    this->keyCode = 'E';
  } else if (key == VK_PROCESSKEY && keyData == 0x130001) {
    this->keyCode = 'R';
  } else if (key == VK_PROCESSKEY && keyData == 0x140001) {
    this->keyCode = 'T';
  } else if (key == VK_PROCESSKEY && keyData == 0x150001) {
    this->keyCode = 'Y';
  } else if (key == VK_PROCESSKEY && keyData == 0x160001) {
    this->keyCode = 'U';
  } else if (key == VK_PROCESSKEY && keyData == 0x170001) {
    this->keyCode = 'I';
  } else if (key == VK_PROCESSKEY && keyData == 0x180001) {
    this->keyCode = 'O';
  } else if (key == VK_PROCESSKEY && keyData == 0x190001) {
    this->keyCode = 'P';
  } else if (key == VK_PROCESSKEY && keyData == 0x1E0001) {
    this->keyCode = 'A';
  } else if (key == VK_PROCESSKEY && keyData == 0x1F0001) {
    this->keyCode = 'S';
  } else if (key == VK_PROCESSKEY && keyData == 0x200001) {
    this->keyCode = 'D';
  } else if (key == VK_PROCESSKEY && keyData == 0x210001) {
    this->keyCode = 'F';
  } else if (key == VK_PROCESSKEY && keyData == 0x220001) {
    this->keyCode = 'G';
  } else if (key == VK_PROCESSKEY && keyData == 0x230001) {
    this->keyCode = 'H';
  } else if (key == VK_PROCESSKEY && keyData == 0x240001) {
    this->keyCode = 'J';
  } else if (key == VK_PROCESSKEY && keyData == 0x250001) {
    this->keyCode = 'K';
  } else if (key == VK_PROCESSKEY && keyData == 0x260001) {
    this->keyCode = 'L';
  } else if (key == VK_PROCESSKEY && keyData == 0x2C0001) {
    this->keyCode = 'Z';
  } else if (key == VK_PROCESSKEY && keyData == 0x2D0001) {
    this->keyCode = 'X';
  } else if (key == VK_PROCESSKEY && keyData == 0x2E0001) {
    this->keyCode = 'C';
  } else if (key == VK_PROCESSKEY && keyData == 0x2F0001) {
    this->keyCode = 'V';
  } else if (key == VK_PROCESSKEY && keyData == 0x300001) {
    this->keyCode = 'B';
  } else if (key == VK_PROCESSKEY && keyData == 0x310001) {
    this->keyCode = 'N';
  } else if (key == VK_PROCESSKEY && keyData == 0x320001) {
    this->keyCode = 'M';
  } else if (key == VK_PROCESSKEY && keyData == 0x330001) {
    this->keyCode = VK_OEM_COMMA;
  } else if (key == VK_PROCESSKEY && keyData == 0x340001) {
    this->keyCode = VK_OEM_PERIOD;
  } else {
    this->keyCode = key;
    this->modifierKey = 0;
    if (key == VK_ESCAPE) {
      return 0;
    }
  }
  this->modifierKey = 0;

  int& primary = this->keyCode;
  if (static_cast<char>(GetAsyncKeyState(VK_MENU) >> 8) && primary != VK_MENU) {
    this->modifierKey = VK_MENU;
  } else if (static_cast<char>(GetAsyncKeyState(VK_SHIFT) >> 8) && primary != VK_SHIFT) {
    this->modifierKey = VK_SHIFT;
  } else if (static_cast<char>(GetAsyncKeyState(VK_CONTROL) >> 8) && primary != VK_CONTROL) {
    this->modifierKey = VK_CONTROL;
  } else if (static_cast<char>(GetAsyncKeyState(VK_TAB) >> 8) && primary != VK_TAB) {
    this->modifierKey = VK_TAB;
  }

  const int modifier = this->modifierKey;
  const int pressed = primary;
  if (modifier == 0) {
    if (pressed == VK_TAB || pressed == VK_MENU || pressed == VK_CONTROL || pressed == VK_SHIFT) {
      return 2;
    }
    return 4;
  }

  if (pressed == VK_TAB) {
    if (modifier == VK_CONTROL) {
      return 4;
    }
    return 3;
  }
  if (pressed == VK_MENU && modifier == VK_TAB) {
    return 3;
  }
  if (pressed == VK_SHIFT) {
    if (modifier == VK_TAB) {
      return 3;
    }
    return 4;
  }
  if (pressed == VK_MENU) {
    if (modifier == VK_CONTROL || modifier == VK_SHIFT) {
      primary = modifier;
      this->modifierKey = pressed;
    }
    return 4;
  }
  if (pressed == VK_CONTROL && (modifier == VK_TAB || modifier == VK_SHIFT)) {
    primary = modifier;
    this->modifierKey = pressed;
  }
  return 4;
}

// INLINED
static __forceinline std::vector<unsigned char> MakeReplayMarker(std::int16_t tag) {
  std::vector<unsigned char> chunk(6);
  *reinterpret_cast<std::int16_t*>(&chunk[0]) = tag;
  *reinterpret_cast<int*>(&chunk[2]) = 0;
  return chunk;
}

// INLINED
static __forceinline std::vector<unsigned char> MakeReplayChunk(int tag, const std::vector<unsigned char>& state) {
  std::vector<unsigned char> chunk(state.size() + 6, 0);
  *reinterpret_cast<std::int16_t*>(&chunk[0]) = static_cast<std::int16_t>(tag);
  *reinterpret_cast<int*>(&chunk[2]) = static_cast<int>(state.size());
  if (state.size()) {
    std::memcpy(&chunk[6], state.data(), state.size());
  }
  return chunk;
}

// OFFSET: 0x00AA8660
// FUNCTION: fn_17244
std::vector<unsigned char> UIWindowMgr::SaveReplayWindows() {
  std::vector<unsigned char> data;
  {
    std::vector<unsigned char> chunk = MakeReplayMarker(20000);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  for (UIWindow* window : this->rendered) {
    const unsigned id = static_cast<unsigned>(window->geometry.id);
    if (id <= 10040 && this->policy[id].actions[7]) {
      std::vector<unsigned char> state = window->Slot44();
      std::vector<unsigned char> chunk = MakeReplayChunk(window->geometry.id + 20100, state);
      data.insert(data.end(), chunk.begin(), chunk.end());
    }
  }
  {
    std::vector<unsigned char> chunk = MakeReplayMarker(20001);
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  return data;
}

void __fastcall RefreshPartyWindows(CSession* session);

// OFFSET: 0x00A8C4B0
// FUNCTION: fn_17132
void UIWindowMgr::CloseMiniPartyWindow(int id) {
  g_session.EraseRecord0734(id);
  this->unknown_4F248.push_back(id);
  RefreshPartyWindows(&g_session);
}

// OFFSET: 0x00A8CA20
// FUNCTION: fn_17139
bool UIWindowMgr::ProcessEnterKey() {
  if (g_replayAnimationScaling != 0) {
    return false;
  }

  bool handled = true;
  if (!this->modalWindows.empty() && this->modalWindows.back() != nullptr) {
    this->Top()->SendMsg(nullptr, 0, 0, 0, 0, 0);
    return true;
  }

  UIWindow* window = this->unknown_0234;
  if (window == nullptr) {
    window = this->unknown_0314;
    if (window != nullptr) {
      if (Replay::Instance().state == 2) {
        window->RecordEditTexts();
      }
      window->SendMsg(nullptr, 0, 0, 0, 0, 0);
      return true;
    }
    window = this->unknown_0318;
    if (window != nullptr) {
      if (Replay::Instance().state == 2) {
        window->RecordEditTexts();
      }
      window->SendMsg(nullptr, 0, 0, 0, 0, 0);
      return true;
    }
    window = this->unknown_01F8;
    if (window == nullptr) {
      window = this->unknown_01F0;
    }
    if (window == nullptr) {
      window = this->unknown_01F4;
    }
    if (window == nullptr) {
      window = this->unknown_0214;
    }
    if (window == nullptr) {
      window = this->unknown_021C;
      if (window != nullptr) {
        window->SendMsg(nullptr, 0x13, 0, 0, 0, 0);
        return true;
      }
      window = this->unknown_020C;
      if (window == nullptr) {
        window = this->unknown_0374;
      }
      if (window == nullptr) {
        window = this->unknown_0378;
      }
      if (window == nullptr) {
        window = this->unknown_030C;
      }
      if (window == nullptr) {
        if (this->hoverWindow != nullptr) {
          if (this->window_01C8 != nullptr && this->hoverWindow->parent == this->window_01C8) {
            if (static_cast<const UINewChatWnd*>(this->window_01C8)->SuggestionsVisible()) {
              UIWindow* child = static_cast<const UINewChatWnd*>(this->window_01C8)->m_suggestions;
              if (child != nullptr) {
                child->SendMsg(nullptr, 0xB0, 0xD, 0, 0, 0);
                return true;
              }
            }
            handled = false;
          }
          if (Replay::Instance().state == 2 && handled) {
            this->hoverWindow->Root()->RecordEditTexts();
          }
          if (g_windowMgr.chatModeChanged == 0) {
            handled = true;
          } else if (!handled) {
            return false;
          }
          this->hoverWindow->SendMsg(nullptr, 0, 0, 0, 0, 0);
          return handled;
        }
        window = this->window_03D4;
        if (window == nullptr) {
          if (this->window_01C8 != nullptr && g_windowMgr.inputSuggestionsEnabled == 1) {
            handled = false;
            this->window_01C8->RecordEditTexts();
            window = this->window_01C8;
          } else {
            for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
              if (!(*it)->Slot02()) {
                continue;
              }
              if (g_replayAnimationScaling == 0) {
                (*it)->Root()->RecordEditTexts();
              }
              (*it)->SendMsg(nullptr, 0, 0, 0, 0, 0);
            }
            return false;
          }
        }
      }
    }
  }

  window->SendMsg(nullptr, 0, 0, 0, 0, 0);
  return handled;
}

// OFFSET: 0x00A97580
// FUNCTION: fn_17205
UIWindow* UIWindowMgr::OpenSubChatWindow(int id, int x, int y, int width, int height) {
  if (x == -1 && y == -1) {
    x = (id + 10) * 10;
    y = (id + 5) * 20;
  }
  if (width == 0 || width > g_renderer->viewport.width) {
    width = 300;
  }
  if (height == 0 || height > g_renderer->viewport.height) {
    height = 0x5F;
  }
  int posX = g_renderer->viewport.width - width;
  if (x <= g_renderer->viewport.width) {
    posX = x;
  }
  int posY = y;
  if (y > g_renderer->viewport.height) {
    posY = 100;
  }

  UIWindow* window = this->retainedWindows[id];
  if (window == nullptr) {
    UISubChatHisWnd* created = new UISubChatHisWnd();
    created->m_index = id;
    created->SetSize(width, height);
    created->Move(posX, posY);
    this->rendered.push_back(created);
    this->retainedWindows[id] = created;
    window = created;
  }
  window->Show(1);
  return window;
}

// FUNCTION: fn_45514
void CloseWindow10005A() {
  g_windowMgr.CloseWindow(10005);
}

// FUNCTION: fn_45513
void CloseWindow10005B() {
  g_windowMgr.CloseWindow(10005);
}

// OFFSET: 0x00A05280
// FUNCTION: fn_45510
void CloseWindow10006() {
  g_windowMgr.CloseWindow(10006);
}

// OFFSET: 0x00AA76F0
// FUNCTION: fn_17239
void UIWindowMgr::DrawLoginOverlay() {
  const short quarter = static_cast<short>(g_renderer->viewport.width / 4);
  const short third = static_cast<short>(g_renderer->viewport.height / 3);
  char name[256];

  if (CTexture* grade = g_texMgr.Load("유저인터페이스\\\0_GameGrade.tga", 0, false, true, false)) {
    this->unknown_04B4 = grade->Width();
    this->unknown_04B8 = grade->Height();
  }

  int gradeLeft = this->unknown_04BC;
  if (gradeLeft == 0) {
    gradeLeft = g_renderer->viewport.width - this->unknown_04B4 - 10;
    this->unknown_04BC = gradeLeft;
  }

  int gradeTop = this->unknown_04C0;
  if (gradeTop == 0) {
    this->unknown_04C0 = 10;
    gradeTop = 10;
  }

  if (g_mouse.buttons[0] == 1) {
    if (!this->inputWindow) {
      this->unknown_04B0 = 0;
      if (g_mouse.x > gradeLeft && g_mouse.x < this->unknown_04B4 + gradeLeft && g_mouse.y > gradeTop && g_mouse.y < this->unknown_04B8 + gradeTop) {
        this->unknown_04B0 = 3;
      }

      const float columns = static_cast<float>(quarter);
      const float mouseX = static_cast<float>(g_mouse.x);
      if (mouseX > columns * 3.3f + this->unknown_04A8.x && mouseX < columns * 3.9f + this->unknown_04A8.x &&
          static_cast<float>(g_mouse.y) > static_cast<float>(third) * 2.4f + this->unknown_04A8.y &&
          static_cast<float>(g_mouse.y) < static_cast<float>(third) * 2.8f + this->unknown_04A8.y) {
        this->unknown_04B0 = 2;
      }

      if (static_cast<float>(g_mouse.x) > columns * 0.1f + this->unknown_049C && static_cast<float>(g_mouse.x) < columns * 1.8f + this->unknown_049C &&
          static_cast<float>(g_mouse.y) > static_cast<float>(third) * 0.3f + this->unknown_049E &&
          static_cast<float>(g_mouse.y) < static_cast<float>(third) * 1.2f + this->unknown_049E) {
        this->unknown_04B0 = 1;
      }

      if (g_serviceType != 0 && static_cast<signed char>(g_startupSecondPhase) > 0 && static_cast<float>(g_mouse.x) > columns * 2.2f + this->unknown_04A0.x &&
          static_cast<float>(g_mouse.x) < columns * 3.5f + this->unknown_04A0.x && g_mouse.y > this->unknown_04A0.y &&
          g_mouse.y < this->unknown_04A0.y + third * 3) {
        this->unknown_04B0 = 5;
      }

      if (static_cast<float>(g_mouse.y) > static_cast<float>(third) * 2.7f + this->unknown_04A4.x &&
          static_cast<float>(g_mouse.y) < static_cast<float>(third) * 3.0f + this->unknown_04A4.x) {
        this->unknown_04B0 = 4;
      }
    }
  } else if (g_mouse.buttons[0] == 2 && !this->inputWindow) {
    if (this->unknown_04B0 == 1) {
      this->unknown_049C += static_cast<short>(g_mouse.deltaX);
      this->unknown_049E += static_cast<short>(g_mouse.deltaY);
    } else if (this->unknown_04B0 == 2) {
      this->unknown_04A8.x += static_cast<short>(g_mouse.deltaX);
      this->unknown_04A8.y += static_cast<short>(g_mouse.deltaY);
    } else if (this->unknown_04B0 == 3) {
      this->unknown_04BC += g_mouse.deltaX;
      this->unknown_04C0 += g_mouse.deltaY;
    } else if (this->unknown_04B0 == 6) {
      this->unknown_04AC.x += static_cast<short>(g_mouse.deltaX);
      this->unknown_04AC.y += static_cast<short>(g_mouse.deltaY);
    } else if (this->unknown_04B0 == 4) {
      if (this->unknown_04A4.x + g_mouse.deltaY >= 0) {
        this->unknown_04A4.x += static_cast<short>(g_mouse.deltaY);
      }
    } else if (this->unknown_04B0 == 5) {
      if (g_serviceType != 0 || this->unknown_04A0.x + g_mouse.deltaX >= 0) {
        this->unknown_04A0.x += static_cast<short>(g_mouse.deltaX);
      }
    }
  }

  RECT rect = {0, 0, g_renderer->viewport.width, g_renderer->viewport.height};
  const TextureRegion region = {0.0f, 0.0f, 1.0f, 1.0f};
  if (CTexture* login = g_texMgr.Load("유저인터페이스\\t_login.jpg", 0, false, true, false)) {
    g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, login, region);
  }

  const int step = quarter;
  const float rows = static_cast<float>(third);
  char tape = 0;
  for (int x = 0; x < step * 4;) {
    ++tape;
    rect.left = x;
    x += step;
    rect.right = x;
    rect.bottom = static_cast<int>(this->unknown_04A4.x + rows * 3.0f);
    rect.top = static_cast<int>(this->unknown_04A4.x + rows * 2.5f);
    this->unknown_04A4.y = static_cast<short>(rect.top);
    std::sprintf(name, "유저인터페이스\\\0_테입%d.tga", tape);
    g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load(name, 0, false, true, false), region);
  }

  rect.left = this->unknown_04BC;
  rect.top = this->unknown_04C0;
  rect.right = this->unknown_04B4 + this->unknown_04BC;
  rect.bottom = this->unknown_04B8 + this->unknown_04C0;
  g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load("유저인터페이스\\\0_GameGrade.tga", 0, false, true, false), region);

  rect.left = static_cast<int>(this->unknown_04A8.x + static_cast<float>(step) * 3.1f);
  rect.right = step + rect.left;
  rect.top = static_cast<int>(this->unknown_04A8.y + rows * 2.4f);
  const float half = rows * 0.5f;
  rect.bottom = static_cast<int>(static_cast<float>(rect.top) + half);
  g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load("유저인터페이스\\\0_중력성인.tga", 0, false, true, false), region);

  char row = -2;
  for (int column = this->unknown_049C; column < this->unknown_049C + step * 2; column += step) {
    ++row;
    int y = static_cast<int>(this->unknown_049E + rows * 0.1f);
    if (this->unknown_049E + (rows + rows) > static_cast<float>(y)) {
      const int left = static_cast<int>(static_cast<float>(step) * 0.05f + column);
      char index = row;
      do {
        index += 2;
        rect.left = left;
        rect.top = y;
        rect.right = step + left;
        if (index <= 2) {
          y += third;
          rect.bottom = y;
        } else {
          rect.bottom = static_cast<int>(static_cast<float>(y) + half);
          y += third;
        }
        std::sprintf(name, "유저인터페이스\\\0_R%d.tga", index);
        g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load(name, 0, false, true, false), region);
      } while (this->unknown_049E + (rows + rows) > static_cast<float>(y));
    }
  }
}

// OFFSET: 0x00AA2DB0
// FUNCTION: fn_17220
bool UIWindowMgr::ProcessHotkeyAction(int action, int pressed) {
  switch (action) {
  case 18:
    if (this->modifierKey == 0) {
      if (UIWindow* window = this->LookupWindow(140)) {
        window->SendMsg(nullptr, 6, 219, 0, 0, 0);
        return false;
      }
    }
    break;

  case 27: {
    if (!pressed) {
      break;
    }

    if (this->unknown_03D0 != 0) {
      g_windowToggleFlag = false;
    }

    if ((this->IsHoverOnWindow01C8Button() || (this->windowHistory.begin() != this->windowHistory.end() && *this->windowHistory.begin() == 1)) && this->inputSuggestionsEnabled != 0 &&
        this->chatModeChanged != 0) {
      UINewChatWnd* suggestions = static_cast<UINewChatWnd*>(this->window_01C8);
      if (suggestions && suggestions->SuggestionsVisible()) {
        suggestions->ShowSuggestions(0);
        return false;
      }

      this->inputSuggestionsEnabled = 0;
      static_cast<UINewChatWnd*>(this->window_01C8)->ApplyInputLayout();
      return false;
    }

    if (this->window_01C8 && this->hoverWindow) {
      UIWindow* parent = this->hoverWindow->parent;
      if (parent && this->hoverWindow->geometry.x != -100) {
        const auto isRetained = [this](const UIWindow* window) {
          for (auto it = this->retainedWindows.begin(); it != this->retainedWindows.end(); ++it) {
            if (it->second == window) {
              return true;
            }
          }
          return false;
        };
        if (parent == static_cast<UINewChatWnd*>(this->window_01C8)->m_tabControl || isRetained(parent)) {
          parent->SendMsg(this->hoverWindow, 0, 1, 0, 0, 0);
          return false;
        }
      }
    }

    if (this->HasModalWindow()) {
      this->unknown_0474 = 188;
      return false;
    }

    const int id = this->windowHistory.begin() != this->windowHistory.end() ? *this->windowHistory.begin() : -1;
    if (g_replayAnimationScaling != 0 && (static_cast<unsigned int>(id) > 10040 || this->policy[id].actions[3] == 0)) {
      return false;
    }

    if (g_windowMgr.CloseWindow(78)) {
      g_modeMgr.GetGameMode()->SendMsg(100, reinterpret_cast<int>(""), -1, 1, 0);
    } else if (UIWindow* window = g_windowMgr.LookupWindow(id)) {
      switch (id) {
      case 22:
      case 23:
      case 24:
      case 25:
      case 41:
      case 44:
      case 161:
      case 174:
      case 178:
        if (g_replayAnimationScaling != 0) {
          return false;
        }
        window->SendMsg(nullptr, 6, 185, 0, 0, 0);
        return false;
      case 132:
      case 181:
      case 184:
      case 203:
      case 281:
      case 326:
        window->SendMsg(nullptr, 6, 201, 0, 0, 0);
        return false;
      case 358:
      case 10024:
        return false;
      }
    }

    if (id != -1) {
      if (this->CloseWindow(id) == true) {
        return false;
      }
    } else if (g_navigationMode == 2) {
      if (g_windowMgr.Confirm(LookupMessage(0x8BF), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        g_navigationTab = 0;
        g_navigationMinimized = false;
        g_navigationManager.Reset(true);
        g_worldViewMgr.ClearMarks();
        g_worldViewMgr.ClearLevels();
        g_worldViewMgr.ResetSelection();
        g_windowMgr.CloseWindow(314);
      }
      return false;
    }

    if (g_modalImmediate != 0 || g_modeMgr.running == 0) {
      return false;
    }

    if (this->LookupWindow(16) || this->LookupWindow(226)) {
      return false;
    }

    if (g_modeMgr.GetGameMode()) {
      if (g_replayAnimationScaling != 0) {
        return false;
      }

      UIWindow* window = this->LookupWindow(10024);
      if (this->CloseWindow(155) || window) {
        return false;
      }

      this->EnsureWindow(155);
      return false;
    }

    if (this->LookupWindow(277) && g_loginModeFlag == 1) {
      if (g_windowMgr.Confirm(LookupMessage(0xD68), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        g_modeMgr.gameMode->SendMsg(10011, 0, 0, 0, 0);
      }
      return false;
    }

    if (this->Confirm(LookupMessage(0x11), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
      GetRagConnection().Close();
      g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
    }
    return false;
  }

  case 9:
    if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
      break;
    }

    if (this->modifierKey == 0) {
      if (UIWindow* window = this->LookupWindow(140)) {
        window->SendMsg(nullptr, 6, 192, 0, 0, 0);
        return false;
      }

      if (UIWindow* window = this->LookupWindow(37)) {
        window->SendMsg(nullptr, 6, 216, 0, 0, 0);
        return false;
      }
    }

    if (!this->hoverWindow) {
      this->RestoreInput();
      return false;
    }

    if (GetAsyncKeyState(VK_SHIFT)) {
      if (this->hoverWindow && this->hoverWindow->parent->PreviousFocusable(this->hoverWindow) == nullptr) {
        this->FocusLast();
      } else {
        this->FocusPrevious();
      }
    } else if (this->hoverWindow && this->hoverWindow->parent->NextFocusable(this->hoverWindow) == nullptr) {
      this->FocusFirst();
    } else {
      this->FocusNext();
    }
    return false;

  case 13:
    if (pressed) {
      this->ProcessEnterKey();
    }
    break;

  case 33:
    if (this->window_4F1D4) {
      this->window_4F1D4->BroadcastMsg3();
      return false;
    }
    break;

  case 34:
    if (this->window_4F1D4) {
      this->window_4F1D4->BroadcastMsg4();
      return false;
    }
    break;

  case 38:
    this->ConfirmActiveDialog();
    return false;

  case 40:
    this->SubmitActiveDialog();
    return false;

  case 37:
    this->CancelActiveDialog();
    return false;

  case 39:
    this->BroadcastMessage21();
    return false;

  case 44:
  case 145:
    if (pressed) {
      g_modeMgr.gameMode->SendMsg(68, 0, 0, 0, 0);
      return false;
    }
    break;

  case 70:
    if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) && this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, 177, 0, 0, 0, 0);
      return false;
    }
    break;

  case 36:
    if (this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, 179, 0, 0, 0, 0);
      return false;
    }
    break;

  case 35:
    if (this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, 114, 0, 0, 0, 0);
    }
    break;
  }

  return false;
}

// OFFSET: 0x00A8CF20
// FUNCTION: fn_17143
bool UIWindowMgr::CloseWindow(int id) {
  UIWindow* window = this->LookupWindow(id);
  if (window) {
    if (this->savedRectangles.find(id) != this->savedRectangles.end()) {
      this->savedRectangles[id]->x = window->geometry.x;
      this->savedRectangles[id]->y = window->geometry.y;
      this->savedRectangles[id]->width = window->geometry.width;
      this->savedRectangles[id]->height = window->geometry.height;
    } else {
      this->savedRectangles[id] = new SavedWindowRectangle{window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height};
    }
  }

  bool closed = false;
  g_modeMgr.GetGameMode();
  if (!this->modalWindows.empty() && this->modalWindows.back()) {
    return false;
  }

  if (this->LookupWindow(id) == this->window_4F1D4) {
    this->window_4F1D4 = nullptr;
  }

  if (Replay::Instance().state == 2 && Replay::Instance().header.options.Get(ReplayRecordOption::Item) &&
      Replay::Instance().header.options.Get(ReplayRecordOption::BasicInfo) && Replay::Instance().header.options.Get(ReplayRecordOption::Status) &&
      (id == 315 || id == 321 || id == 322)) {
#pragma pack(push, 1)
    struct WindowClosePacket {
      std::uint32_t marker;
      std::uint16_t bytes;
      std::int32_t windowId;
    };
#pragma pack(pop)
    static_assert(sizeof(WindowClosePacket) == 10);
    WindowClosePacket packet{0x039208AEu, 10, id};

    Replay::Instance().Record(packet.bytes, &packet, 42, 0);
  }

  switch (id) {
  case 1:
    this->CloseWindow(26);
    if (this->window_01C8) {
      this->Schedule(this->window_01C8);
      this->window_01C8 = nullptr;
      closed = true;
      this->unknown_003C = 0;
    }
    break;
  case 0:
    if (this->unknown_01DC) {
      this->Schedule(this->unknown_01DC);
      this->unknown_01DC = nullptr;
      closed = true;
    }
    break;
  case 8:
    if (this->unknown_01D4) {
      static_cast<UIItemWnd*>(this->unknown_01D4)->ReleaseSelection(0);
      this->Schedule(this->unknown_01D4);
      this->unknown_01D4 = nullptr;
      closed = true;
      this->unknown_054C = 0;
    }
    break;
  case 361:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->CloseWindow(362);
      this->Schedule(found);
      closed = true;
    }
    break;
  case 204:
    if (this->window_0428) {
      this->Schedule(this->window_0428);
      this->window_0428 = nullptr;
      closed = true;
    }
    break;
  case 10:
    if (this->unknown_01E0) {
      this->Schedule(this->unknown_01E0);
      this->unknown_01E0 = nullptr;
      closed = true;
      this->unknown_05A4 = 0;
      g_savedWindowCloseValue = g_windowCloseValue;
    }
    break;
  case 184:
    if (this->unknown_01E4) {
      this->Schedule(this->unknown_01E4);
      this->unknown_01E4 = nullptr;
      closed = true;
    }
    break;
  case 11:
    if (this->unknown_01C4) {
      if (reinterpret_cast<std::uintptr_t>(this->unknown_01C4) > 8) {
        this->Schedule(this->unknown_01C4);
      }
      this->unknown_01C4 = nullptr;
      closed = true;
      this->unknown_0588 = 0;
    }
    break;
  case 15:
    if (this->window_01EC) {
      this->Schedule(this->window_01EC);
      this->window_01EC = nullptr;
      closed = true;
    }
    break;
  case 226:
    if (this->unknown_01F4) {
      this->Schedule(this->unknown_01F4);
      this->unknown_01F4 = nullptr;
      closed = true;
    }
    break;
  case 16:
    if (this->unknown_01F0) {
      this->Schedule(this->unknown_01F0);
      this->unknown_01F0 = nullptr;
      closed = true;
    }
    break;
  case 28:
    if (this->window_027C) {
      this->Schedule(this->window_027C);
      this->window_027C = nullptr;
      closed = true;
    }
    break;
  case 39:
    if (this->unknown_020C) {
      this->Schedule(this->unknown_020C);
      this->unknown_020C = nullptr;
      closed = true;
    }
    break;
  case 17:
    if (this->unknown_01F8) {
      this->Schedule(this->unknown_01F8);
      this->unknown_01F8 = nullptr;
      closed = true;
    }
    break;
  case 74:
    if (this->unknown_0208) {
      this->Schedule(this->unknown_0208);
      this->unknown_0208 = nullptr;
      closed = true;
    }
    break;
  case 73:
    if (this->unknown_01FC) {
      this->Schedule(this->unknown_01FC);
      this->unknown_01FC = nullptr;
      closed = true;
    }
    break;
  case 26:
    if (this->unknown_0214) {
      this->Schedule(this->unknown_0214);
      this->unknown_0214 = nullptr;
      closed = true;
    }
    break;
  case 260:
    if (this->unknown_021C) {
      this->Schedule(this->unknown_021C);
      this->unknown_021C = nullptr;
      closed = true;
    }
    break;
  case 18:
    if (this->unknown_0210) {
      this->Schedule(this->unknown_0210);
      this->unknown_0210 = nullptr;
      closed = true;
    }
    break;
  case 30:
    if (this->window_0278) {
      this->Schedule(this->window_0278);
      this->window_0278 = nullptr;
      closed = true;
    }
    break;
  case 27:
    if (this->window_0274) {
      this->Schedule(this->window_0274);
      this->window_0274 = nullptr;
      closed = true;
    }
    break;
  case 25:
    if (this->unknown_0264) {
      g_modeMgr.gameMode->SendMsg(291, 0, 0, 0, 0);
      this->Schedule(this->unknown_0264);
      this->unknown_0264 = nullptr;
      closed = true;
    }
    break;
  case 24:
    if (this->unknown_025C) {
      g_modeMgr.gameMode->SendMsg(291, 0, 0, 0, 0);
      this->Schedule(this->unknown_025C);
      this->unknown_025C = nullptr;
      closed = true;
    }
    break;
  case 161:
    if (this->unknown_0260) {
      this->Schedule(this->unknown_0260);
      this->unknown_0260 = nullptr;
      closed = true;
    }
    break;
  case 44:
    if (this->unknown_0308) {
      this->Schedule(this->unknown_0308);
      this->unknown_0308 = nullptr;
      closed = true;
    }
    break;
  case 23:
    if (this->unknown_0258) {
      g_modeMgr.gameMode->SendMsg(291, 0, 0, 0, 0);
      this->Schedule(this->unknown_0258);
      this->unknown_0258 = nullptr;
      closed = true;
    }
    break;
  case 45:
    if (this->unknown_0304) {
      this->Schedule(this->unknown_0304);
      this->unknown_0304 = nullptr;
      closed = true;
    }
    break;
  case 43:
    if (this->unknown_0300) {
      this->Schedule(this->unknown_0300);
      this->unknown_0300 = nullptr;
      closed = true;
    }
    break;
  case 22:
    if (this->unknown_0250) {
      this->Schedule(this->unknown_0250);
      this->unknown_0250 = nullptr;
      closed = true;
    }
    break;
  case 160:
    if (this->unknown_0254) {
      this->Schedule(this->unknown_0254);
      this->unknown_0254 = nullptr;
      closed = true;
    }
    break;
  case 254:
    if (this->unknown_0268) {
      this->Schedule(this->unknown_0268);
      this->unknown_0268 = nullptr;
      closed = true;
    }
    break;
  case 255:
    if (this->unknown_026C) {
      this->NotifyMode();
      this->Schedule(this->unknown_026C);
      this->unknown_026C = nullptr;
      closed = true;
    }
    break;
  case 256:
    if (this->window_0270) {
      this->NotifyMode();
      this->Schedule(this->window_0270);
      this->window_0270 = nullptr;
      closed = true;
    }
    break;
  case 334:
    if (this->window_4F260) {
      this->Schedule(this->window_4F260);
      this->window_4F260 = nullptr;
      closed = true;
    }
    break;
  case 335:
    if (this->window_4F264) {
      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->SendMsg(309, 0, 0, 0, 0);
      }
      this->Schedule(this->window_4F264);
      this->window_4F264 = nullptr;
      closed = true;
    }
    break;
  case 341:
    if (this->window_4F268) {
      this->Schedule(this->window_4F268);
      closed = true;
      this->window_4F268 = nullptr;
      if (this->window_4F26C) {
        this->CloseWindow(342);
      }
    }
    break;
  case 342:
    if (this->window_4F26C) {
      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->SendMsg(322, 0, 0, 0, 0);
      }
      this->Schedule(this->window_4F26C);
      closed = true;
      this->window_4F26C = nullptr;
      if (this->window_4F268) {
        this->CloseWindow(341);
      }
    }
    break;
  case 49:
    if (this->unknown_0244) {
      this->Schedule(this->unknown_0244);
      this->unknown_0244 = nullptr;
      closed = true;
    }
    break;
  case 21:
    if (this->unknown_0240) {
      this->Schedule(this->unknown_0240);
      this->unknown_0240 = nullptr;
      closed = true;
    }
    break;
  case 143:
    if (this->unknown_0248) {
      this->Schedule(this->unknown_0248);
      this->unknown_0248 = nullptr;
      closed = true;
    }
    break;
  case 190:
    if (this->unknown_024C) {
      this->Schedule(this->unknown_024C);
      this->unknown_024C = nullptr;
      closed = true;
    }
    break;
  case 47:
    if (this->unknown_023C) {
      this->Schedule(this->unknown_023C);
      this->unknown_023C = nullptr;
      closed = true;
    }
    break;
  case 201:
    if (this->unknown_0424) {
      this->Schedule(this->unknown_0424);
      this->unknown_0424 = nullptr;
      closed = true;
    }
    break;
  case 19:
    if (this->unknown_0238) {
      this->Schedule(this->unknown_0238);
      this->unknown_0238 = nullptr;
      closed = true;
    }
    break;
  case 48:
    if (this->unknown_0234) {
      this->Schedule(this->unknown_0234);
      this->unknown_0234 = nullptr;
      closed = true;
    }
    break;
  case 46:
    if (this->window_0230) {
      this->Schedule(this->window_0230);
      this->window_0230 = nullptr;
      closed = true;
      this->unknown_0048 = 0;
    }
    break;
  case 50:
    if (this->unknown_022C) {
      this->Schedule(this->unknown_022C);
      this->unknown_022C = nullptr;
      closed = true;
    }
    break;
  case 82:
    if (this->unknown_0228) {
      this->Schedule(this->unknown_0228);
      this->unknown_0228 = nullptr;
      closed = true;
    }
    break;
  case 12:
    if (this->unknown_0218) {
      this->Schedule(this->unknown_0218);
      closed = true;
      this->unknown_0218 = nullptr;
      if (this->unknown_0220) {
        this->CloseWindow(234);
      }
    }
    break;
  case 234:
    if (this->unknown_0220) {
      this->Schedule(this->unknown_0220);
      closed = true;
      this->unknown_0220 = nullptr;
      if (this->unknown_0218) {
        this->CloseWindow(12);
      }
    }
    break;
  case 250:
    if (this->unknown_0224) {
      this->Schedule(this->unknown_0224);
      this->unknown_0224 = nullptr;
      closed = true;
    }
    break;
  case 14:
    if (this->unknown_01C0) {
      this->Schedule(this->unknown_01C0);
      this->unknown_01C0 = nullptr;
      closed = true;
    }
    break;
  case 37:
    if (g_windowMgr.LookupWindow(10030)) {
      this->CloseWindow(10030);
    }
    this->CloseWindow(46);
    if (this->window_02C4) {
      this->Schedule(this->window_02C4);
      this->window_02C4 = nullptr;
      closed = true;
      this->unknown_077C = 0;
    }
    break;
  case 33:
    if (this->window_0288) {
      this->Schedule(this->window_0288);
      this->window_0288 = nullptr;
      closed = true;
    }
    break;
  case 69:
    if (this->window_02C8) {
      this->window_02C8->Slot11();
    }
    closed = this->CloseWindow(34);
    g_windowMgr.DismissTransient();
    break;
  case 34:
    if (this->window_02C8) {
      this->Schedule(this->window_02C8);
      this->window_02C8 = nullptr;
      this->unknown_073C = 0;
      g_windowMgr.CloseWindow(53);
      g_windowMgr.CloseWindow(68);
      g_windowMgr.DismissTransient();
      closed = true;
    }
    break;
  case 41:
    this->CloseWindow(42);
    if (this->unknown_02FC) {
      this->Schedule(this->unknown_02FC);
      this->unknown_02FC = nullptr;
      closed = true;
    }
    break;
  case 42:
    if (this->unknown_02F8) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->unknown_02F8));
      this->unknown_02F8 = 0;
      closed = true;
    }
    break;
  case 40:
    if (this->unknown_02F4) {
      this->Schedule(this->unknown_02F4);
      this->unknown_02F4 = nullptr;
      closed = true;
      this->unknown_0790 = 0;
    }
    break;
  case 51:
    if (this->window_03A4) {
      this->Schedule(this->window_03A4);
      this->window_03A4 = nullptr;
      closed = true;
    }
    break;
  case 52:
    if (this->unknown_03A8) {
      this->Schedule(this->unknown_03A8);
      this->unknown_03A8 = nullptr;
      closed = true;
    }
    break;
  case 58:
    if (this->unknown_031C) {
      this->Schedule(this->unknown_031C);
      this->unknown_031C = nullptr;
      closed = true;
    }
    break;
  case 56:
    if (this->unknown_0314) {
      this->Schedule(this->unknown_0314);
      this->unknown_0314 = nullptr;
      closed = true;
    }
    break;
  case 100:
    if (this->unknown_0318) {
      this->Schedule(this->unknown_0318);
      this->unknown_0318 = nullptr;
      closed = true;
    }
    break;
  case 55:
    if (this->unknown_0310) {
      this->Schedule(this->unknown_0310);
      this->unknown_0310 = nullptr;
      closed = true;
    }
    break;
  case 212:
    if (this->unknown_0430) {
      this->unknown_0430->Slot11();
      this->Schedule(this->unknown_0430);
      this->unknown_0430 = nullptr;
      closed = true;
    }
    break;
  case 214:
    if (this->unknown_0434) {
      this->Schedule(this->unknown_0434);
      this->unknown_0434 = nullptr;
      closed = true;
    }
    break;
  case 213:
    if (this->unknown_0438) {
      this->Schedule(this->unknown_0438);
      this->unknown_0438 = nullptr;
      closed = true;
    }
    break;
  case 215:
    if (this->unknown_043C) {
      this->Schedule(this->unknown_043C);
      this->unknown_043C = nullptr;
      closed = true;
    }
    break;
  case 59:
  {
    const bool closed60 = g_windowMgr.CloseWindow(60);
    const bool closed61 = g_windowMgr.CloseWindow(61);
    const bool closed62 = g_windowMgr.CloseWindow(62);
    const bool closed63 = g_windowMgr.CloseWindow(63);
    const bool closed64 = g_windowMgr.CloseWindow(64);
    const bool closed65 = g_windowMgr.CloseWindow(65);
    const bool closed66 = g_windowMgr.CloseWindow(66);
    closed = closed60 || closed61 || closed62 || closed63 || closed64 || closed65 || closed66;
    if (g_closeChatAfterGroups) {
      g_windowMgr.CloseWindow(26);
    }
    break;
  }
  case 61:
    if (this->window_0324) {
      this->window_0324->Slot11();
      this->Schedule(this->window_0324);
      this->window_0324 = nullptr;
      closed = true;
    }
    break;
  case 62:
    if (this->unknown_0328) {
      this->unknown_0328->Slot11();
      this->Schedule(this->unknown_0328);
      this->unknown_0328 = nullptr;
      closed = true;
    }
    break;
  case 63:
    if (this->window_032C) {
      this->window_032C->Slot11();
      this->Schedule(this->window_032C);
      this->window_032C = nullptr;
      closed = true;
    }
    break;
  case 64:
    if (this->unknown_0330) {
      this->unknown_0330->Slot11();
      this->Schedule(this->unknown_0330);
      this->unknown_0330 = nullptr;
      closed = true;
    }
    break;
  case 65:
    if (this->unknown_0334) {
      this->unknown_0334->Slot11();
      this->Schedule(this->unknown_0334);
      this->unknown_0334 = nullptr;
      closed = true;
    }
    break;
  case 66:
    if (this->unknown_0338) {
      this->unknown_0338->Slot11();
      this->Schedule(this->unknown_0338);
      this->unknown_0338 = nullptr;
      closed = true;
    }
    break;
  case 60:
    if (this->window_0320) {
      this->window_0320->Slot11();
      this->Schedule(this->window_0320);
      this->window_0320 = nullptr;
      closed = true;
    }
    break;
  case 78:
    if (this->unknown_0348) {
      this->Schedule(this->unknown_0348);
      this->unknown_0348 = nullptr;
      closed = true;
    }
    break;
  case 216: case 217: case 218: case 219: case 220: case 221: case 222: case 223: case 224: case 225:
    if (this->unknown_034C[id - 216]) {
      this->Schedule(this->unknown_034C[id - 216]);
      this->unknown_034C[id - 216] = nullptr;
      closed = true;
    }
    break;
  case 76:
    if (this->unknown_0344) {
      this->Schedule(this->unknown_0344);
      this->unknown_0344 = nullptr;
      closed = true;
    }
    break;
  case 75:
    if (this->unknown_0340) {
      this->Schedule(this->unknown_0340);
      this->unknown_0340 = nullptr;
      closed = true;
    }
    break;
  case 68:
    if (this->unknown_033C) {
      this->Schedule(this->unknown_033C);
      this->unknown_033C = nullptr;
      closed = true;
    }
    break;
  case 53:
    if (this->unknown_030C) {
      this->Schedule(this->unknown_030C);
      this->unknown_030C = nullptr;
      closed = true;
    }
    break;
  case 208:
    if (this->unknown_042C) {
      this->Schedule(this->unknown_042C);
      this->unknown_042C = nullptr;
      closed = true;
    }
    break;
  case 207:
    this->CloseWindow(53);
    closed = true;
    break;
  case 38:
    if (this->unknown_02F0) {
      this->Schedule(this->unknown_02F0);
      this->unknown_02F0 = nullptr;
      closed = true;
    }
    break;
  case 70:
    if (this->unknown_02E8) {
      this->Schedule(this->unknown_02E8);
      this->unknown_02E8 = nullptr;
      closed = true;
    }
    break;
  case 72:
    if (this->unknown_02EC) {
      this->Schedule(this->unknown_02EC);
      this->unknown_02EC = nullptr;
      closed = true;
    }
    break;
  case 35:
    if (this->unknown_02CC) {
      this->Schedule(this->unknown_02CC);
      this->unknown_02CC = nullptr;
      closed = true;
    }
    break;
  case 104:
    if (this->unknown_02D0) {
      this->Schedule(this->unknown_02D0);
      this->unknown_02D0 = nullptr;
      closed = true;
    }
    break;
  case 119:
    if (this->unknown_02D4) {
      this->Schedule(this->unknown_02D4);
      this->unknown_02D4 = nullptr;
      closed = true;
    }
    break;
  case 163:
    if (this->unknown_02D8) {
      this->Schedule(this->unknown_02D8);
      this->unknown_02D8 = nullptr;
      closed = true;
    }
    break;
  case 138:
    if (this->unknown_02DC) {
      this->Schedule(this->unknown_02DC);
      this->unknown_02DC = nullptr;
      closed = true;
    }
    break;
  case 105:
    if (this->unknown_02E0) {
      this->Schedule(this->unknown_02E0);
      this->unknown_02E0 = nullptr;
      closed = true;
    }
    break;
  case 110:
    if (this->unknown_02E4) {
      this->Schedule(this->unknown_02E4);
      this->unknown_02E4 = nullptr;
      closed = true;
    }
    break;
  case 32:
    if (this->unknown_0284) {
      this->Schedule(this->unknown_0284);
      this->unknown_0284 = nullptr;
      closed = true;
    }
    break;
  case 29:
    if (this->unknown_0280) {
      this->Schedule(this->unknown_0280);
      this->unknown_0280 = nullptr;
      closed = true;
    }
    break;
  case 79:
    if (this->unknown_0374) {
      this->Schedule(this->unknown_0374);
      this->unknown_0374 = nullptr;
    }
    break;
  case 107:
    if (this->unknown_0200) {
      this->Schedule(this->unknown_0200);
      this->unknown_0200 = nullptr;
    }
    break;
  case 111:
    if (this->unknown_0204) {
      this->Schedule(this->unknown_0204);
      this->unknown_0204 = nullptr;
    }
    break;
  case 80:
    if (this->unknown_0378) {
      this->Schedule(this->unknown_0378);
      this->unknown_0378 = nullptr;
    }
    break;
  case 81:
    if (this->unknown_037C) {
      this->Schedule(this->unknown_037C);
      this->unknown_037C = nullptr;
    }
    break;
  case 85:
    if (this->window_03AC) {
      this->Schedule(this->window_03AC);
      this->window_03AC = nullptr;
    }
    break;
  case 86:
    if (this->unknown_0380) {
      this->Schedule(this->unknown_0380);
      this->unknown_0380 = nullptr;
      closed = true;
    }
    break;
  case 87:
    if (this->unknown_0384) {
      this->Schedule(this->unknown_0384);
      this->unknown_0384 = nullptr;
      closed = true;
    }
    break;
  case 88:
    if (this->window_038C) {
      this->Schedule(this->window_038C);
      this->window_038C = nullptr;
      closed = true;
    }
    break;
  case 90:
    if (this->unknown_0390) {
      this->Schedule(this->unknown_0390);
      this->unknown_0390 = nullptr;
      closed = true;
      g_modeMgr.gameMode->SendMsg(146, -1, 0, 0, 0);
    }
    break;
  case 91:
    if (this->unknown_0394) {
      this->Schedule(this->unknown_0394);
      this->unknown_0394 = nullptr;
      closed = true;
    }
    break;
  case 94:
    if (this->unknown_0398) {
      this->Schedule(this->unknown_0398);
      this->unknown_0398 = nullptr;
      closed = true;
    }
    break;
  case 95:
    if (this->unknown_039C) {
      this->Schedule(this->unknown_039C);
      this->unknown_039C = nullptr;
      closed = true;
    }
    break;
  case 99:
    if (this->unknown_03A0) {
      this->Schedule(this->unknown_03A0);
      this->unknown_03A0 = nullptr;
      closed = true;
    }
    break;
  case 103:
    if (this->window_0388) {
      this->Schedule(this->window_0388);
      this->window_0388 = nullptr;
      closed = true;
    }
    break;
  case 106:
    if (this->unknown_03B0) {
      this->Schedule(this->unknown_03B0);
      this->unknown_03B0 = nullptr;
      closed = true;
    }
    break;
  case 109:
    if (this->unknown_03B4) {
      this->Schedule(this->unknown_03B4);
      this->unknown_03B4 = nullptr;
      closed = true;
    }
    break;
  case 199:
    if (this->unknown_03B8) {
      this->Schedule(this->unknown_03B8);
      this->unknown_03B8 = nullptr;
      closed = true;
    }
    break;
  case 200:
    if (this->unknown_03BC) {
      this->Schedule(this->unknown_03BC);
      this->unknown_03BC = nullptr;
      closed = true;
    }
    break;
  case 113:
    if (this->window_03C0) {
      this->Schedule(this->window_03C0);
      this->window_03C0 = nullptr;
      closed = true;
    }
    break;
  case 114:
    if (this->window_03C4) {
      this->Schedule(this->window_03C4);
      this->window_03C4 = nullptr;
      closed = true;
      this->unknown_08EC = 0;
    }
    break;
  case 125:
    if (this->window_03C8) {
      this->Schedule(this->window_03C8);
      this->window_03C8 = nullptr;
      closed = true;
    }
    break;
  case 187:
    this->Schedule(this->window_4F168);
    this->window_4F168 = nullptr;
    closed = true;
    break;
  case 198:
    if (this->window_4F164) {
      this->Schedule(this->window_4F164);
      this->window_4F164 = nullptr;
      closed = true;
    }
    break;
  case 203:
    if (this->window_4F174) {
      this->Schedule(this->window_4F174);
      this->window_4F174 = nullptr;
      closed = true;
    }
    break;
  case 229:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->Schedule(found);
      closed = true;
    }
    break;
  case 126:
    if (this->window_03CC) {
      this->Schedule(this->window_03CC);
      this->window_03CC = nullptr;
      closed = true;
      this->unknown_0910 = 0;
    }
    break;
  case 116:
    if (this->unknown_03D0) {
      this->Schedule(this->unknown_03D0);
      this->unknown_03D0 = nullptr;
      closed = true;
    }
    break;
  case 128:
    if (this->window_03DC) {
      this->Schedule(this->window_03DC);
      this->window_03DC = nullptr;
      closed = true;
    }
    break;
  case 129:
    if (this->unknown_03E0) {
      this->Schedule(this->unknown_03E0);
      this->unknown_03E0 = nullptr;
      closed = true;
    }
    break;
  case 130:
    if (this->unknown_03E4) {
      reinterpret_cast<UIWindow*>(this->unknown_03E4)->Slot11();
      this->Schedule(reinterpret_cast<UIWindow*>(this->unknown_03E4));
      this->unknown_03E4 = 0;
      closed = true;
    }
    break;
  case 131:
    if (this->unknown_03E8) {
      reinterpret_cast<UIWindow*>(this->unknown_03E8)->Slot11();
      this->Schedule(reinterpret_cast<UIWindow*>(this->unknown_03E8));
      this->unknown_03E8 = 0;
      closed = true;
    }
    break;
  case 132:
    if (this->unknown_03EC) {
      this->Schedule(this->unknown_03EC);
      this->unknown_03EC = nullptr;
      closed = true;
    }
    break;
  case 133:
    if (this->unknown_03F0) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->unknown_03F0));
      this->unknown_03F0 = 0;
      closed = true;
    }
    break;
  case 137:
    if (this->unknown_03F8) {
      static_cast<UIMemorialDunWnd*>(this->unknown_03F8)->ResetDungeonInfo();
      this->Schedule(this->unknown_03F8);
      this->unknown_03F8 = nullptr;
      closed = true;
      g_session.unknown_593C = 0;
    }
    break;
  case 139:
    if (this->unknown_03FC) {
      this->Schedule(this->unknown_03FC);
      this->unknown_03FC = nullptr;
      closed = true;
    }
    break;
  case 140:
    if (this->modal) {
      this->Schedule(this->modal);
      this->modal = nullptr;
      closed = true;
      if (this->window_4F174) {
        g_navigationHidden = false;
        static_cast<UINavigationV4Wnd*>(this->window_4F174)->RefreshLayout();
      }
    }
    break;
  case 146: case 147: case 148: case 149: case 150: case 151: case 152:
    if (this->unknown_028C[id - 146]) {
      this->Schedule(this->unknown_028C[id - 146]);
      this->unknown_028C[id - 146] = nullptr;
      closed = true;
      if (this->window_0288) {
        static_cast<UIItemStoreWnd*>(this->window_0288)->SetTabChecked(id - 146, false);
      }
    }
    break;
  case 309:
    if (this->unknown_02A8) {
      this->Schedule(this->unknown_02A8);
      this->unknown_02A8 = nullptr;
      closed = true;
      if (this->window_0288) {
        static_cast<UIItemStoreWnd*>(this->window_0288)->SetTabChecked(7, false);
      }
    }
    break;
  case 153:
    if (this->unknown_02AC) {
      this->Schedule(this->unknown_02AC);
      this->unknown_02AC = nullptr;
      closed = true;
    }
    break;
  case 173:
    if (this->unknown_02C0) {
      this->Schedule(this->unknown_02C0);
      this->unknown_02C0 = nullptr;
      closed = true;
    }
    break;
  case 170:
    if (this->unknown_02B8) {
      this->Schedule(this->unknown_02B8);
      this->unknown_02B8 = nullptr;
      closed = true;
    }
    break;
  case 164:
    if (this->unknown_02B0) {
      this->Schedule(this->unknown_02B0);
      this->unknown_02B0 = nullptr;
      closed = true;
      g_closeWindow164State = 0;
    }
    break;
  case 168:
    if (this->unknown_02B4) {
      this->Schedule(this->unknown_02B4);
      this->unknown_02B4 = nullptr;
      closed = true;
    }
    break;
  case 169:
    if (this->unknown_02BC) {
      this->Schedule(this->unknown_02BC);
      this->unknown_02BC = nullptr;
      closed = true;
    }
    break;
  case 155:
    if (this->unknown_0408) {
      this->Schedule(this->unknown_0408);
      this->unknown_0408 = nullptr;
      closed = true;
    }
    break;
  case 156: case 158: case 159: case 165: case 166: case 167: case 171: case 181: case 183: case 191:
    if (this->LookupWindow(id)) {
      if (UIWindow* found = this->LookupWindow(id)) {
        switch (id) {
        case 156:
          this->Schedule(this->unknown_0404);
          this->unknown_0404 = nullptr;
          break;
        case 157:
        case 158:
        case 159:
        case 165:
        case 166:
        case 167:
        case 171:
        case 183:
        case 191:
          this->Schedule(found);
          break;
        case 181:
          this->Schedule(found);
          g_session.ClearEntryList5BD4();
          break;
        }
      }
      closed = true;
    }
    break;
  default:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->Schedule(found);
      closed = true;
    }
    break;
  case 174:
    this->CloseWindow(175);
    if (this->unknown_040C) {
      this->Schedule(this->unknown_040C);
      this->unknown_040C = nullptr;
      closed = true;
    }
    break;
  case 175:
    if (this->unknown_0410) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->unknown_0410));
      this->unknown_0410 = 0;
      closed = true;
    }
    break;
  case 176:
    if (this->unknown_0414) {
      this->Schedule(this->unknown_0414);
      this->unknown_0414 = nullptr;
      closed = true;
    }
    break;
  case 177:
    if (this->unknown_0418) {
      this->Schedule(this->unknown_0418);
      this->unknown_0418 = nullptr;
      closed = true;
    }
    break;
  case 178:
    if (this->unknown_041C) {
      this->Schedule(this->unknown_041C);
      this->unknown_041C = nullptr;
      closed = true;
    }
    break;
  case 179:
    if (this->unknown_0420) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->unknown_0420));
      this->unknown_0420 = 0;
      closed = true;
    }
    break;
  case 196:
    if (this->window_4F18C) {
      this->Schedule(this->window_4F18C);
      this->window_4F18C = nullptr;
      closed = true;
    }
    break;
  case 237:
    if (this->window_0444) {
      this->window_0444->Slot11();
      this->Schedule(this->window_0444);
      this->window_0444 = nullptr;
      closed = true;
    }
    break;
  case 275:
    if (this->window_0448) {
      this->Schedule(this->window_0448);
      this->window_0448 = nullptr;
      closed = true;
    }
    break;
  case 253:
    if (this->window_044C) {
      this->Schedule(this->window_044C);
      this->window_044C = nullptr;
      closed = true;
    }
    break;
  case 262:
    if (this->unknown_0450) {
      this->Schedule(this->unknown_0450);
      this->unknown_0450 = nullptr;
      closed = true;
    }
    break;
  case 263:
    if (this->unknown_0454) {
      std::int16_t packet = 0x9E9;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
      this->Schedule(this->unknown_0454);
      this->unknown_0454 = nullptr;
      closed = true;
      if (this->unknown_0450) {
        this->CloseWindow(262);
      }
    }
    break;
  case 264:
    if (this->unknown_0458) {
      std::int16_t packet = 0xA03;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
      g_session.RestoreItems5C68();
      g_session.ClearItemQuantities();
      this->Schedule(this->unknown_0458);
      this->unknown_0458 = nullptr;
      closed = true;
    }
    break;
  case 265:
    if (this->unknown_045C) {
      g_session.ClearItemQuantities();
      this->Schedule(this->unknown_045C);
      this->unknown_045C = nullptr;
      closed = true;
    }
    break;
  case 259:
    if (this->window_4F170) {
      this->Schedule(this->window_4F170);
      this->window_4F170 = nullptr;
      closed = true;
    }
    break;
  case 268:
    if (this->unknown_0460) {
      this->Schedule(this->unknown_0460);
      this->unknown_0460 = nullptr;
      closed = true;
    }
    break;
  case 269:
    if (this->unknown_0464) {
      std::int16_t packet = 0xA1D;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
      g_rouletteMgr->Reset();
      this->Schedule(this->unknown_0464);
      this->unknown_0464 = nullptr;
      closed = true;
    }
    break;
  case 36:
    if (this->window_01E8) {
      this->Schedule(this->window_01E8);
      this->window_01E8 = nullptr;
      closed = true;
    }
    break;
  case 273:
    if (this->window_4F1CC) {
      this->Schedule(this->window_4F1CC);
      this->window_4F1CC = nullptr;
      closed = true;
    }
    break;
  case 274:
    if (this->window_4F1D0) {
      this->Schedule(this->window_4F1D0);
      this->window_4F1D0 = nullptr;
      closed = true;
    }
    break;
  case 276:
    if (this->window_4F1E4) {
      this->Schedule(this->window_4F1E4);
      this->window_4F1E4 = nullptr;
      closed = true;
      this->visible_276 = 0;
    }
    break;
  case 284:
    if (this->window_4F1E8) {
      this->Schedule(this->window_4F1E8);
      this->window_4F1E8 = nullptr;
      closed = true;
    }
    break;
  case 285:
    if (this->window_4F1EC) {
      this->Schedule(this->window_4F1EC);
      this->window_4F1EC = nullptr;
      closed = true;
      this->window_4F1FC = nullptr;
    }
    break;
  case 286:
    if (this->window_4F1F0) {
      this->Schedule(this->window_4F1F0);
      this->window_4F1F0 = nullptr;
      closed = true;
      this->unknown_4F210 = false;
    }
    break;
  case 287:
    if (this->window_4F1F4) {
      this->Schedule(this->window_4F1F4);
      this->window_4F1F4 = nullptr;
      closed = true;
    }
    break;
  case 288:
    if (this->window_4F1F8) {
      this->Schedule(this->window_4F1F8);
      this->window_4F1F8 = nullptr;
      closed = true;
    }
    break;
  case 180:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->Schedule(found);
      this->window_01D0 = nullptr;
      closed = true;
    }
    break;
  case 318:
    if (this->window_4F254) {
      this->Schedule(this->window_4F254);
      this->window_4F254 = nullptr;
      closed = true;
      std::int16_t packet = 0x84A;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
    }
    break;
  case 315:
    if (this->window_0468) {
      this->Schedule(this->window_0468);
      this->window_0468 = nullptr;
      closed = true;
    }
    break;
  case 321:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->Schedule(found);
      this->unknown_0568 = 0;
      closed = true;
    }
    break;
  case 324:
    if (this->window_4F274) {
      this->window_4F274->Slot11();
      this->Schedule(this->window_4F274);
      this->window_4F274 = nullptr;
      closed = true;
    }
    break;
  case 323:
    if (this->window_4F270) {
      this->window_4F270->Slot11();
      this->Schedule(this->window_4F270);
      this->window_4F270 = nullptr;
      closed = true;
    }
    break;
  case 326:
    if (this->window_4F278) {
      this->Schedule(this->window_4F278);
      this->window_4F278 = nullptr;
      closed = true;
    }
    break;
  case 345:
    if (this->window_4F27C) {
      this->window_4F27C->Slot11();
      this->Schedule(this->window_4F27C);
      this->window_4F27C = nullptr;
      closed = true;
    }
    break;
  case 359:
    if (this->window_4F288) {
      this->Schedule(this->window_4F288);
      this->window_4F288 = nullptr;
    }
    break;
  case 10024:
    if (this->window_4F2AC) {
      this->Schedule(this->window_4F2AC);
      this->window_4F2AC = nullptr;
      closed = true;
    }
    break;
  }

  auto entry = this->windows.find(id);
  if (entry != this->windows.end()) {
    this->windows.erase(entry);
  }

  if (closed) {
    this->RemoveOrder(id);
  }

  return closed;
}

// OFFSET: 0x00AA2A00
// FUNCTION: fn_17218
void UIWindowMgr::ShowWaitDBSaveWindow() {
  if (this->window_4F2AC != nullptr) {
    return;
  }

  this->window_4F2AC = new UIWaitDBSaveWnd();
  this->window_4F2AC->SetSize(g_renderer->viewport.width, g_renderer->viewport.height);
  this->window_4F2AC->Move(0, 0);
  this->window_4F2AC->SetId(10024);
  this->rendered.push_back(this->window_4F2AC);

  UIWindow* window = this->window_4F2AC;
  if (window && (this->modal == nullptr || this->window_4F174 == window) && !this->LookupWindow(278) && !this->LookupWindow(4) &&
      !this->LookupWindow(199) && !this->LookupWindow(200)) {
    int companion = -1;
    if (window->geometry.id == 12) {
      companion = 234;
    } else if (window->geometry.id == 82) {
      companion = 250;
    }

    if (companion != -1) {
      const auto entry = std::find_if(this->rendered.begin(), this->rendered.end(), [&](const UIWindow* value) { return value->geometry.id == companion; });
      if (entry != this->rendered.end()) {
        UIWindow* value = *entry;
        this->rendered.erase(entry);
        this->rendered.push_back(value);
      }
    }

    const auto entry = std::find(this->rendered.begin(), this->rendered.end(), window);
    if (entry != this->rendered.end()) {
      this->rendered.erase(entry);
      this->rendered.push_back(window);
    }
  }

  this->window_4F2AC->Show(1);
}


// OFFSET: 0x00AA35F0
// FUNCTION: fn_17221
bool UIWindowMgr::ProcessShortcutKey(int action) {
  if (action != 4) {
    return false;
  }

  const int hotkey = this->GetHotkeyBehavior();
  if (hotkey == -1) {
    return false;
  }

  if (g_replayAnimationScaling != 0 && hotkey != 0x8B && hotkey != 0x8C && hotkey != 0x8D) {
    const ReplayOptions& options = Replay::Instance().header.options;
    if (hotkey != 0x71 && hotkey != 0x6C && hotkey != 0x78 && hotkey != 0x67 && hotkey != 0x73) {
      bool restricted;
      if (hotkey == 0x64) {
        restricted = options.Get(ReplayRecordOption::BasicInfo) == 0;
      } else if (hotkey == 0x65 || hotkey == 0x7A || hotkey == 0x7B || hotkey == 0x7C) {
        restricted = options.Get(ReplayRecordOption::Equip) == 0;
      } else if (hotkey == 0x6D || hotkey == 0x6B) {
        restricted = options.Get(ReplayRecordOption::Item) == 0;
      } else if (hotkey == 0x66) {
        restricted = options.Get(ReplayRecordOption::Skill) == 0;
      } else if (hotkey == 0x68 || hotkey == 0x69) {
        restricted = options.Get(ReplayRecordOption::Party) == 0;
      } else if (hotkey == 0x75) {
        restricted = options.Get(ReplayRecordOption::Shortcut) == 0;
      } else if (hotkey == 0x76 || hotkey == 0x77) {
        restricted = options.Get(ReplayRecordOption::Chatting) == 0;
      } else if (hotkey == 0x93) {
        restricted = options.Get(ReplayRecordOption::Status) == 0;
      } else {
        return false;
      }

      if (restricted) {
        return false;
      }
    }
  }

  if (static_cast<int>(g_session.m_options.Get(5)) > 0) {
    return false;
  }

  if (this->window_4F258 != nullptr) {
    return false;
  }

  if (static_cast<unsigned int>(hotkey) < 45) {
    if (this->window_01E8) {
      this->window_01E8->SendMsg(nullptr, 41, hotkey % 9, hotkey / 9, 0, 0);
    }
    return true;
  }

  if (this->unknown_0474 == 0) {
    if (this->LookupWindow(0x2731) || this->LookupWindow(0x2728)) {
      return false;
    }
  }

  switch (hotkey) {
  case 0x64:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0xCA, 0, 0, 0);
    }
    break;
  case 0x65:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0xC3, 0, 0, 0);
    }
    break;
  case 0x66:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0xC4, 0, 0, 0);
    }
    break;
  case 0x67:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0x169, 0, 0, 0);
    }
    break;
  case 0x68:
  case 0x69:
    if (this->unknown_01DC) {
      int previous;
      std::memcpy(&previous, this->unknown_0740, sizeof(previous));
      int partyTab = hotkey == 0x69 ? 1 : 0;
      std::memcpy(this->unknown_0740, &partyTab, sizeof(partyTab));
      const bool closed = g_windowMgr.CloseWindow(0x45);
      std::memcpy(&partyTab, this->unknown_0740, sizeof(partyTab));
      if (!closed || previous != partyTab) {
        g_windowMgr.EnsureWindow(0x45);
      }
    }
    break;
  case 0x6A:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0xD6, 0, 0, 0);
    }
    break;
  case 0x6B:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0xF7, 0, 0, 0);
    }
    break;
  case 0x6C:
    if (this->unknown_01DC && !g_windowMgr.CloseWindow(0x8C)) {
      g_windowMgr.EnsureWindow(0x8C);
    }
    break;
  case 0x6D:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0xC2, 0, 0, 0);
    }
    break;
  case 0x6E:
    if (this->unknown_01DC) {
      if (g_guildInfo.m_guildId != 0) {
        if (!g_windowMgr.CloseWindow(0x3B)) {
          g_windowMgr.EnsureWindow(0x3B);
        }
      } else if (!g_windowMgr.CloseWindow(0xD4)) {
        g_windowMgr.EnsureWindow(0xD4);
      }
    }
    break;
  case 0x90:
    if (this->unknown_01DC && !g_windowMgr.CloseWindow(0xED)) {
      g_windowMgr.EnsureWindow(0xED);
    }
    break;
  case 0x6F:
    if (this->unknown_01DC && !g_windowMgr.CloseWindow(0x57)) {
      g_windowMgr.EnsureWindow(0x57);
    }
    break;
  case 0x70:
    if (g_modeMgr.gameMode) {
      g_modeMgr.gameMode->SendMsg(0x2A, 0x16, 0, 0, 0);
    }
    break;
  case 0x71:
    if (g_windowMgr.modal == nullptr && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0254 != 0) {
      if (this->unknown_005C == 0xFFFFFFFF) {
        this->unknown_005C = 0xAAFFFFFF;
        return true;
      }
      if (!this->CloseWindow(14)) {
        this->EnsureWindow(14);
        this->unknown_005C = 0xFFFFFFFF;
      }
    }
    break;
  case 0x72:
    if (this->unknown_01DC && !g_windowMgr.CloseWindow(0x56)) {
      g_windowMgr.EnsureWindow(0x56);
    }
    break;
  case 0x73:
    if (this->unknown_01DC) {
      ToggleGameSettingsTab4();
    }
    break;
  case 0x74:
    this->CycleWindowVisibility(4);
    break;
  case 0x75:
    if (UIWindow* window = this->LookupWindow(0x24)) {
      window->SendMsg(nullptr, 6, 0xD8, 0, 0, 0);
    }
    break;
  case 0x76:
    if (this->unknown_01DC) {
      this->ShowWindowById(1, 1 - this->unknown_003C);
    }
    break;
  case 0x77:
    if (UIWindow* window = this->window_01C8) {
      if (window->geometry.visible == 0) {
        this->ShowWindowById(1, 1);
        static_cast<UINewChatWnd*>(this->window_01C8)->ApplyInputLayout();
        return true;
      }
      window->SendMsg(nullptr, 2, 0, 0, 0, 0);
      if (this->window_01C8->geometry.unknown_0030 == 0) {
        this->ShowWindowById(1, 0);
      }
    }
    break;
  case 0x78:
    if (g_modeMgr.gameMode) {
      g_modeMgr.gameMode->SendMsg(0x29, 0, 0, 0, 0);
    }
    break;
  case 0x79:
    g_modeMgr.gameMode->SendMsg(0x44, 0, 0, 0, 0);
    break;
  case 0x7A:
    if (UIWindow* window = this->unknown_03F8) {
      window->Show(window->geometry.visible == 0);
    } else {
      g_windowMgr.EnsureWindow(0x89);
    }
    break;
  case 0x7B:
    if (this->unknown_01DC && !g_windowMgr.CloseWindow(0x58)) {
      g_windowMgr.EnsureWindow(0x58);
    }
    break;
  case 0x7C:
    if (this->unknown_01DC && g_session.unknown_5614 != 0 && !g_windowMgr.CloseWindow(0x71)) {
      g_windowMgr.EnsureWindow(0x71);
    }
    break;
  case 0x7D:
    if (this->unknown_01DC && g_session.unknown_56B0 != 0 && !g_windowMgr.CloseWindow(0x7D)) {
      g_windowMgr.EnsureWindow(0x7D);
    }
    break;
  case 0x7E:
    if (g_session.unknown_5614 != 0) {
      g_session.ClearHomunStatus();
      if (CMode* mode = g_modeMgr.GetGameMode()) {
        std::uint32_t aid;
        std::memcpy(&aid, &g_session.unknown_5570[8], sizeof(aid));
        CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(mode)->scene->GetGameActorByAID(aid));
        if (actor) {
          if (actor->HasUnknown030CEntry()) {
            AiCommand command = {};
            command.type = 9;
            actor->SetUnknown0318(reinterpret_cast<const float*>(&command), 0);
          }
        }
      }
    }
    break;
  case 0x7F:
    if (g_session.unknown_56B0 != 0) {
      g_session.ResetMercenaryStats();
      if (CMode* mode = g_modeMgr.GetGameMode()) {
        std::uint32_t aid;
        std::memcpy(&aid, &g_session.unknown_5620[8], sizeof(aid));
        CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(mode)->scene->GetGameActorByAID(aid));
        if (actor && actor->HasUnknown0310Entry()) {
          AiCommand command = {};
          command.type = 9;
          actor->SetUnknown0318(reinterpret_cast<const float*>(&command), 1);
        }
      }
    }
    break;
  case 0x80:
    if (this->unknown_01DC && !g_windowMgr.CloseWindow(0x44)) {
      g_windowMgr.EnsureWindow(0x44);
    }
    break;
  case 0x81:
    if (this->unknown_01DC && !g_windowMgr.CloseWindow(0x35) && g_session.PartyCount()) {
      g_windowMgr.EnsureWindow(0x35);
    }
    break;
  case 0x87: {
    int flag;
    std::memcpy(&flag, g_session.unknown_546C, sizeof(flag));
    flag = 1 - flag;
    std::memcpy(g_session.unknown_546C, &flag, sizeof(flag));
    break;
  }
  case 0x83:
    if (this->unknown_01DC && this->chatModeChanged && this->window_01C8 && (this->IsHoverOnWindow01C8Button() || this->hoverWindow == nullptr)) {
      static_cast<UINewChatWnd*>(this->window_01C8)->ToggleInput();
      if (this->hoverWindow) {
        this->hoverWindow->SendMsg(nullptr, 0, 0, 0, 0, 0);
      }
    }
    break;
  case 0x84:
    if (UIWindow* window = this->LookupWindow(0x133)) {
      window->SendMsg(nullptr, 60, 0, 0, 0, 0);
    }
    break;
  case 0x89:
    if (this->unknown_01DC) {
      if (g_windowMgr.LookupWindow(0x13E) == nullptr) {
        g_modeMgr.GetGameMode()->SendMsg(0x143, 0, 0, 0, 0);
      } else {
        g_windowMgr.CloseWindow(0x13E);
      }
    }
    break;
  case 0x8A:
    if (UIWindow* window = g_windowMgr.LookupWindow(0x133)) {
      window->SendMsg(nullptr, 6, 0x18F, 0, 0, 0);
    }
    break;
  case 0x8B:
    if (g_replayAnimationScaling != 0 && this->window_4F168) {
      this->window_4F168->SendMsg(nullptr, 6, Replay::Instance().state == 6 ? 0x191 : 0x192, 0, 0, 0);
      this->window_4F168->SendMsg(nullptr, 35, 0, 0, 0, 0);
    }
    break;
  case 0x8C:
    if (g_replayAnimationScaling != 0 && this->window_4F168) {
      this->window_4F168->SendMsg(nullptr, 6, 0x195, 0, 0, 0);
    }
    break;
  case 0x8D:
    if (g_replayAnimationScaling != 0 && this->window_4F168) {
      this->window_4F168->SendMsg(nullptr, 6, 0x196, 0, 0, 0);
    }
    break;
  case 0x8E:
    if (g_windowMgr.window_4F174) {
      g_windowMgr.window_4F174->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
    } else if (g_windowMgr.modal) {
      g_windowMgr.modal->SendMsg(nullptr, 6, 0xC8, 0, 0, 0);
    } else {
      g_windowMgr.EnsureWindow(0xCB);
    }
    break;

  case 0x92:
    if (g_windowMgr.window_0448) {
      g_windowMgr.CloseWindow(0x113);
    } else {
      BankingCheckPacket packet;
      packet.id = 0x9AB;
      packet.aid = g_session.m_aid;
      CRagConnection& connection = GetRagConnection();
      GetRagConnection();
      connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
    }
    break;
  case 0x93:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0xC0, 0, 0, 0);
    }
    break;
  case 0x94: {
    if (UIWindow* window = this->window_01E8) {
      window->SendMsg(nullptr, 143, 1, g_session.GetOption(10), 0, 0);
    }
    ShortcutRotatePacket packet;
    packet.id = 0xB22;
    packet.option = static_cast<std::uint16_t>(g_session.GetOption(10));
    if (packet.option == 0) {
      packet.state = static_cast<unsigned char>(g_session.unknown_06FC);
    } else if (packet.option == 1) {
      packet.state = static_cast<unsigned char>(g_session.unknown_06A8);
    }
    CRagConnection& connection = GetRagConnection();
    GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
    break;
  }
  case 0x95:
    if (!g_windowMgr.CloseWindow(0x10E)) {
      g_windowMgr.EnsureWindow(0x10E);
    }
    break;
  case 0x96:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0254 != 0) {
      if (!g_session.IsNavigationAllowed()) {
        g_windowMgr.Confirm(LookupMessage(0xACF), 0, true, false, 0, "Message", 280, 120, 0);
      } else if (!g_windowMgr.CloseWindow(0x111)) {
        g_windowMgr.EnsureWindow(0x111);
      }
    }
    break;
  case 0x97:
    if (UIWindow* window = g_windowMgr.LookupWindow(0x133)) {
      window->SendMsg(nullptr, 6, 0x1DC, 0, 0, 0);
    }
    break;
  case 0x98:
    g_modeMgr.GetGameMode()->SendMsg(0x130, 0, 0, 0, 0);
    break;
  case 0x9A:
    if (!g_windowMgr.CloseWindow(0x13B)) {
      g_windowMgr.EnsureWindow(0x13B);
    }
    break;
  case 0x9B:
    if (UIWindow* window = g_windowMgr.LookupWindow(0x24)) {
      window->SendMsg(nullptr, 6, 0x225, 0, 0, 0);
    }
    break;
  case 0x9C:
    if (this->unknown_01DC) {
      this->unknown_01DC->SendMsg(nullptr, 6, 0x242, 0, 0, 0);
    }
    break;
  case 0x9D:
    if (!g_windowMgr.CloseWindow(0x144)) {
      g_windowMgr.EnsureWindow(0x144);
    }
    break;
  case 0x9F:
    Singleton<CGameSettingsMgr>::GetInstance()->ToggleOption(0x109);
    break;
  case 0xA0:
    if (!g_windowMgr.CloseWindow(0x15A)) {
      g_windowMgr.EnsureWindow(0x15A);
    }
    break;
  case 0xA1:
    if (!g_windowMgr.CloseWindow(0x2711)) {
      g_windowMgr.EnsureWindow(0x2711);
    }
    break;
  case 0xA3:
    Singleton<CGameSettingsMgr>::GetInstance()->ToggleOption(0x10A);
    break;
  case 0xC8:
  case 0xC9:
  case 0xCA:
  case 0xCB:
  case 0xCC:
  case 0xCD:
  case 0xCE:
  case 0xCF:
  case 0xD0:
  case 0xD1:
    if (g_modeMgr.gameMode) {
      this->SendEmotionHotkey(hotkey - 200);
    }
    break;
  case 0xD2:
  case 0xD3:
  case 0xD4:
  case 0xD5:
  case 0xD6:
  case 0xD7:
  case 0xD8:
  case 0xD9:
  case 0xDA:
  case 0xDB:
    this->SendFlagEffect(hotkey);
    break;
  default:
    return false;
  }

  return true;
}
// OFFSET: 0x00A978D0
// FUNCTION: fn_17207
UIWindow* UIWindowMgr::EnsureWindow(int id) {
  UIWindow* transient = this->window_4F250;
  if (transient && static_cast<UIDragMiniPartyWnd*>(transient)->member.valid && transient->geometry.unknown_0030 == 0) {
    bool behavior = false;
    g_session.m_hotkeyScript->Call("IsBeHaviorOfWindow", "d>b", this->GetHotkeyBehavior(), &behavior);
    if (behavior && this->window_4F250 && static_cast<UIDragMiniPartyWnd*>(this->window_4F250)->member.valid) {
      this->window_4F250->geometry.unknown_0030 = 1;
      this->Schedule(this->window_4F250);
      this->window_4F250 = nullptr;
    }
  }

  if (this->window_4F288) {
    switch (id) {
    case 8:
    case 10:
    case 18:
    case 40:
    case 155:
    case 203:
    case 251:
    case 263:
    case 275:
      return nullptr;
    }
  }

  if (!this->IsOrderFixed(id)) {
    this->RemoveOrder(id);
    if (id == 203) {
      this->windowHistory.push_back(int(id));
    } else {
      this->windowHistory.push_front(int(id));
    }
  }

  unsigned char* const base = reinterpret_cast<unsigned char*>(this);
  const auto layerGreater = [](const UIWindow* left, const UIWindow* right) {
    return static_cast<int>(left->unknown_004C) > static_cast<int>(right->unknown_004C);
  };
  UIWindow* window = nullptr;
  switch (id) {
  case 0:
    window = this->unknown_01DC;
    if (window == nullptr) {
      window = new UIBasicInfoWnd();
      this->unknown_01DC = window;
      window->SetSize(220, g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true ? 71 : 55);
      window->Move(0, 0);
      this->AddRendered(window);
      this->unknown_01DC->SendMsg(this->unknown_01DC, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_0514), 0, 0, 0);
      g_snapMgr.SetRoot(window);
      if (UIWindow* navigation = g_windowMgr.EnsureWindow(307)) {
        int x = 0;
        int y = 0;
        window->ToScreen(x, y);
        navigation->Move(x, window->geometry.height + y);
      }
    }
    window->Show(g_replayAnimationScaling ? Replay::Instance().header.options.Get(ReplayRecordOption::BasicInfo) : 1);
    if (this->savedWindowIds.size() != 0) {
      for (int savedId : this->savedWindowIds) {
        if (savedId == 301) {
          this->RebuildMiniPartyWindows();
        } else {
          this->EnsureWindow(savedId);
        }
      }
    } else {
      if (this->unknown_054C) {
        this->EnsureWindow(8);
      }
      if (this->unknown_05A4) {
        this->EnsureWindow(10);
      }
      if (this->unknown_0588) {
        this->EnsureWindow(11);
      }
      if (this->unknown_05BC) {
        this->EnsureWindow(13);
      }
      if (this->unknown_077C) {
        this->EnsureWindow(37);
      }
      if (this->unknown_0568) {
        this->EnsureWindow(321);
      }
      if (this->unknown_073C) {
        this->EnsureWindow(69);
      }
      if (g_session.unknown_5470 == 1 && g_session.unknown_5474 == 1 && g_effectDetailLevel == 2 && g_textureDetailLevel == 2) {
        this->EnsureWindow(10014);
      }
      if (g_session.HasUnknown6178Records() == true) {
        this->EnsureWindow(271);
      }
      if (this->unknown_4F210 == true) {
        this->EnsureWindow(286);
      }
      this->RebuildMiniPartyWindows();
    }
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0254) {
      this->EnsureWindow(14);
    }
    this->ShowWindowById(1, this->unknown_003C);
    this->ShowWindowById(36, this->m_normalItemSlotWindow.show);
    this->ShowWindowById(15, 0);
    break;
  case 1:
    window = this->window_01C8;
    if (window == nullptr) {
      window = new UINewChatWnd();
      this->window_01C8 = window;
      window->SetSize(600, this->unknown_0038);
      this->AddRendered(window);
    }
    this->window_01C8->SendMsg(nullptr, 0x22, 0, 0, 0, 0);
    break;
  case 2:
    window = new UISelectServerWnd();
    window->SetSize(280, 200);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 80);
    this->AddRendered(window);
    break;
  case 325:
    if (this->LookupWindow(325) == nullptr) {
      window = new UIRPImageWnd2();
      this->window_4F25C = window;
      window->SetSize(301, 132);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(325);
    }
    break;
  case 3:
    window = new UILoginWnd();
    this->window_01CC = window;
    window->SetSize(301, 132);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case 180:
    window = new UILoginOTPWnd();
    this->window_01D0 = window;
    window->SetSize(280, 142);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case 93:
    window = new UINoticeWnd();
    window->SetSize(600, 400);
    window->Move(CenterOnViewportX(33), CenterOnViewportY(40));
    this->AddRendered(window);
    break;
  case 4:
    window = new UIMakeCharWnd();
    window->SetSize(150, 286);
    window->Move((g_renderer->viewport.width - 150) / 2, (g_renderer->viewport.height - 286) / 2);
    this->AddRendered(window);
    break;
  case 277: {
    window = new UINewSelectCharWnd();
    this->window_03D4 = window;
    UIWindow* background = new UIRPImageWnd("유저인터페이스\\select_character_ver3\\bg_back1.tga", 5);
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\select_character_ver3\\bg_back1.tga")));
    if (background && bitmap) {
      background->SetSize(bitmap->image.width, bitmap->image.height);
      background->Move((g_renderer->viewport.width - background->geometry.width) / 2, (g_renderer->viewport.height - background->geometry.height) / 2);
      this->AddRendered(background);
      window->SetSize(background->geometry.width, background->geometry.height);
      window->Move((g_renderer->viewport.width - background->geometry.width) / 2, (g_renderer->viewport.height - background->geometry.height) / 2);
      this->AddRendered(window);
    }
    g_activeSkillEffects.erase(g_activeSkillEffects.begin(), g_activeSkillEffects.end());
    g_session.m_requestTimers.clear();
    if (g_session.m_passwordWndMode == 1) {
      static_cast<UINewSelectCharWnd*>(this->window_03D4)->EnableButtonById(418);
    } else {
      static_cast<UINewSelectCharWnd*>(this->window_03D4)->DisableButtonById(418);
      static_cast<UINewSelectCharWnd*>(this->window_03D4)->DisableButtonById(416);
      static_cast<UINewSelectCharWnd*>(this->window_03D4)->DisableButtonById(417);
    }
    break;
  }
  case 278: {
    window = new UINewMakeCharWnd();
    UIWindow* background = new UIRPImageWnd("", 1);
    background->SetSize(1028, 768);
    background->Move((g_renderer->viewport.width - 1028) / 2 - 2, (g_renderer->viewport.height - 768) / 2);
    this->AddRendered(background);
    window->SetSize(794, 422);
    window->Move((g_renderer->viewport.width - 794) / 2, (g_renderer->viewport.height - 422) / 2 - 70);
    this->AddRendered(window);
    break;
  }
  case 8:
    window = this->unknown_01D4;
    if (window == nullptr) {
      this->RecordWindowPacket(8);
      window = new UIItemWnd();
      this->unknown_01D4 = window;
      window->SetSize(280, 120);
      window->Move(0, 120);
      this->AddRendered(window);
      this->unknown_01D4->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x528), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->unknown_054C = 1;
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(307)) {
      basicInfo->SendMsg(nullptr, 6, 0x11E, 0xC2, 0, 0);
    }
    break;
  case 204:
    if (this->window_0428 == nullptr) {
      UIQuestDisplay* display = new UIQuestDisplay();
      this->window_0428 = display;
      display->Create(250, 400);
      this->window_0428->Move(this->unknown_047C - 320, 150);
      this->window_0428->unknown_004C = 5;
      this->AddRendered(this->window_0428);
      this->rendered.sort(layerGreater);
      this->window_0428->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x570), 0, 0, 0);
    }
    break;
  case 6:
    window = new UIWaitWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case 7:
    window = this->unknown_01BC;
    if (window == nullptr) {
      window = new UILoadingWnd();
      this->unknown_01BC = window;
      window->SetSize(240, 15);
      window->Move(g_renderer->viewport.width / 2 - 100, g_renderer->viewport.height / 2 + 160);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 9:
    window = new UIToolTipWnd();
    window->SetSize(200, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case 36:
    window = this->window_01E8;
    if (window == nullptr) {
      window = new UIShortCutWnd();
      this->window_01E8 = window;
      window->SetSize(290, 34);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_01E8->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(&this->m_normalItemSlotWindow), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    break;
  case 10:
    window = this->unknown_01E0;
    if (window == nullptr) {
      window = new UIEquipWnd();
      this->unknown_01E0 = window;
      static_cast<UIEquipWnd*>(window)->m_viewOther = 0;
      window->SetSize(280, 167);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->unknown_01E0->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x590), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->unknown_05A4 = 1;
    break;
  case 184:
    window = this->unknown_01E4;
    if (window == nullptr) {
      window = new UInCashWnd();
      this->unknown_01E4 = window;
      window->SetSize(640, 540);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(30));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x9B4), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    break;
  case 11:
    window = this->unknown_01C4;
    if (window == nullptr) {
      window = new UIStatusWnd();
      this->unknown_01C4 = window;
      window->SetSize(280, 141);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->unknown_01C4->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x574), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->unknown_0588 = 1;
    this->CloseWindow(21);
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(307)) {
      basicInfo->SendMsg(nullptr, 6, 0x11E, 0xC0, 0, 0);
    }
    break;
  case 37:
    window = this->window_02C4;
    if (window == nullptr) {
      window = new UINewSkillListWnd();
      this->window_02C4 = window;
      window->SetSize(560, 400);
      window->Move(CenterOnViewportX(50), ScaleByScreenHeight(50));
      this->AddRendered(window);
      this->window_02C4->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x76C), 0, 0, 0);
    }
    window->Show(1);
    this->unknown_077C = 1;
    this->CloseWindow(49);
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(307)) {
      basicInfo->SendMsg(nullptr, 6, 0x11E, 0xC4, 0, 0);
    }
    break;
  case 33:
    window = this->window_0288;
    if (window == nullptr) {
      window = new UIItemStoreWnd();
      this->window_0288 = window;
      window->SetSize(280, 458);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_0288->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x614), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 146:
  case 147:
  case 148:
  case 149:
  case 150:
  case 151:
  case 152: {
    const int tab = id - 146;
    window = this->unknown_028C[tab];
    if (window == nullptr) {
      UIItemStoreSubWnd* store = new UIItemStoreSubWnd();
      window = store;
      this->unknown_028C[tab] = window;
      store->SetTabIndex(tab);
      int message = 0;
      switch (id) {
      case 146:
      case 147:
      case 148:
      case 149:
        message = tab + 0x5B9;
        break;
      case 150:
        message = 0xC4F;
        break;
      case 151:
      case 152:
        message = tab + 0x5B8;
        break;
      }
      window->SetSize(140, 145);
      static_cast<UIItemStoreSubWnd*>(this->unknown_028C[tab])->SetTitle(LookupMessage(message));
      this->AddRendered(window);
      this->unknown_028C[tab]->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(reinterpret_cast<ItemStoreFindSlotInfo*>(base + 0x62C) + tab), 0, 0, 0);
    }
    window->Show(1);
    break;
  }
  case 309:
    window = this->unknown_02A8;
    if (window == nullptr) {
      UIItemStoreSubWnd* store = new UIItemStoreSubWnd();
      window = store;
      this->unknown_02A8 = window;
      store->SetTabIndex(7);
      window->SetSize(140, 145);
      static_cast<UIItemStoreSubWnd*>(this->unknown_02A8)->SetTitle(LookupMessage(id == 309 ? 0x5BF : 0));
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_02A8->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x69C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 153:
    window = this->unknown_02AC;
    if (window == nullptr) {
      window = new UIItemStoreFindWnd();
      this->unknown_02AC = window;
      window->SetSize(140, 145);
      static_cast<UIItemStoreFindWnd*>(this->unknown_02AC)->SetTitle(LookupMessage(0x447));
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_02AC->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6AC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 173:
    if (this->unknown_02C0 == nullptr) {
      window = new UIPartyBookingHelpWnd();
      this->unknown_02C0 = window;
      window->SetSize(320, 199);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    break;
  case 170:
    if (this->unknown_02B8 == nullptr) {
      window = new UIJobListWnd();
      this->unknown_02B8 = window;
      window->SetSize(152, 199);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_02B8->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6EC), 0, 0, 0);
    }
    break;
  case 164:
    window = this->unknown_02B0;
    if (window == nullptr) {
      window = new UISeekPartyWnd();
      this->unknown_02B0 = window;
      window->SetSize(152, 199);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_02B0->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6DC), 0, 0, 0);
    }
    window->Show(1);
    g_session.m_partyBooking.unknown_0000 = 1;
    break;
  case 168:
    if (this->unknown_02B0 == nullptr) {
      if (this->unknown_02B4 == nullptr) {
        window = new UISeekPartyMBWnd();
        this->unknown_02B4 = window;
        window->SetSize(360, 350);
        window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
        this->AddRendered(window);
        this->unknown_02B4->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6CC), 0, 0, 0);
      } else {
        g_windowMgr.CloseWindow(168);
      }
    }
    break;
  case 169:
    if (this->unknown_02BC == nullptr) {
      window = new UISeekPartyListWnd();
      this->unknown_02BC = window;
      window->SetSize(550, 350);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_02BC->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6BC), 0, 0, 0);
    } else {
      g_windowMgr.CloseWindow(169);
    }
    break;
  case 69:
    if (*reinterpret_cast<const int*>(this->unknown_0740)) {
      this->EnsureWindow(34)->SendMsg(nullptr, 6, 0xD7, 2, 0, 0);
    } else {
      this->EnsureWindow(34)->SendMsg(nullptr, 6, 0xD7, 1, 0, 0);
    }
    break;
  case 34:
    if (g_replayAnimationScaling && !Replay::Instance().header.options.Get(ReplayRecordOption::Party)) {
      break;
    }
    this->CloseWindow(69);
    window = this->window_02C8;
    if (window == nullptr) {
      window = new UIMessengerGroupWnd();
      this->window_02C8 = window;
      window->SetSize(273, 326);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_02C8->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x72C), 0, 0, 0);
    }
    window->Show(1);
    this->unknown_073C = 1;
    break;
  case 41:
    window = this->unknown_02FC;
    if (window == nullptr) {
      window = new UIMerchantShopMakeWnd(0);
      this->unknown_02FC = window;
      window->SetSize(400, 260);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(200));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x7A4), 0, 0, 0);
    }
    window->Show(1);
    this->EnsureWindow(42);
    break;
  case 42:
    window = reinterpret_cast<UIWindow*>(this->unknown_02F8);
    if (window == nullptr) {
      window = new UIMerchantMirrorItemWnd(0);
      this->unknown_02F8 = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_0794), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 40:
    if (!g_mapPropertyMgr->GetValue(g_session.unknown_0000, 40)) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x94C)), 255, 0, 0, 0);
      break;
    }
    window = this->unknown_02F4;
    if (window == nullptr) {
      window = new UIMerchantItemWnd();
      this->unknown_02F4 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_0780), 0, 0, 0);
    }
    window->Show(1);
    this->unknown_0790 = 1;
    break;
  case 58:
    window = this->unknown_031C;
    if (window == nullptr) {
      window = new UINotifyItemObtainWnd();
      this->unknown_031C = window;
      window->SetSize(180, 32);
      window->Move(CenterOnViewportX(220), 75);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 56:
    window = this->unknown_0314;
    if (window == nullptr) {
      window = new UINpcEditDialogWnd();
      this->unknown_0314 = window;
      window->SetSize(176, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 100:
    window = this->unknown_0318;
    if (window == nullptr) {
      window = new UINpcTextEditDialogWnd();
      this->unknown_0318 = window;
      window->SetSize(346, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 55:
    window = this->unknown_0310;
    if (window == nullptr) {
      window = new UISkillNameChangeWnd();
      this->unknown_0310 = window;
      window->SetSize(280, 48);
      window->Move(CenterOnViewportX(185), CenterOnViewportY(180));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 212:
    window = this->unknown_0430;
    if (window == nullptr) {
      window = new UIGuildHelperWnd();
      this->unknown_0430 = window;
      window->SetSize(200, 100);
      window->Move(CenterOnViewportX(220), CenterOnViewportY(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x9D4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 214:
    window = this->unknown_0434;
    if (window == nullptr) {
      window = new UIGuildTipWnd();
      this->unknown_0434 = window;
      window->SetSize(330, 230);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 213:
    window = this->unknown_0438;
    if (window == nullptr) {
      window = new UICreateGuildWnd(0);
      this->unknown_0438 = window;
      window->SetSize(150, 100);
      window->Move(CenterOnViewportX(200), CenterOnViewportY(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 215:
    window = this->unknown_043C;
    if (window == nullptr) {
      window = new UICreateGuildWnd(1);
      this->unknown_043C = window;
      window->SetSize(150, 100);
      window->Move(CenterOnViewportX(200), CenterOnViewportY(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 59: {
    GuildWndInfo* guildInfo = reinterpret_cast<GuildWndInfo*>(base + 0x834);
    if (!guildInfo->IsValid()) {
      guildInfo->Reset();
    }
    switch (guildInfo->unknown_0010) {
    case 0:
      window = g_windowMgr.EnsureWindow(60);
      break;
    case 1:
      window = g_windowMgr.EnsureWindow(61);
      break;
    case 2:
      window = g_windowMgr.EnsureWindow(62);
      break;
    case 3:
      window = g_windowMgr.EnsureWindow(63);
      break;
    case 4:
      window = g_windowMgr.EnsureWindow(64);
      break;
    case 5:
      window = g_windowMgr.EnsureWindow(65);
      break;
    case 6:
      window = g_windowMgr.EnsureWindow(66);
      break;
    }
    break;
  }
  case 61:
    window = this->window_0324;
    if (window == nullptr) {
      window = new UIGuildMemberManageWnd();
      this->window_0324 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 62:
    window = this->unknown_0328;
    if (window == nullptr) {
      window = new UIGuildPositionManageWnd();
      this->unknown_0328 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 63:
    window = this->window_032C;
    if (window == nullptr) {
      window = new UIGuildSkillWnd();
      this->window_032C = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 64:
    window = this->unknown_0330;
    if (window == nullptr) {
      window = new UIGuildBanishedMemberWnd();
      this->unknown_0330 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 65:
    window = this->unknown_0334;
    if (window == nullptr) {
      window = new UIGuildNoticeWnd();
      this->unknown_0334 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 66:
    window = this->unknown_0338;
    if (window == nullptr) {
      window = new UIGuildTotalInfoWnd();
      this->unknown_0338 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 60:
    window = this->window_0320;
    if (window == nullptr) {
      window = new UIGuildInfoManageWnd();
      this->window_0320 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 75:
    window = this->unknown_0340;
    if (window == nullptr) {
      window = new UIGuildLeaveReasonDescWnd();
      this->unknown_0340 = window;
      window->SetSize(280, 80);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 78:
    window = this->unknown_0348;
    if (window == nullptr) {
      window = new UIIllustWnd();
      this->unknown_0348 = window;
      window->SetSize(4, 4);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->rendered.push_front(window);
      this->unknown_0348->unknown_004C = 2;
      this->rendered.sort(layerGreater);
    } else {
      this->unknown_0348->SendMsg(nullptr, 0x22, -1, 0, 0, 0);
    }
    window->Show(1);
    break;
  case 216:
  case 217:
  case 218:
  case 219:
  case 220:
  case 221:
  case 222:
  case 223:
  case 224:
  case 225:
    window = this->unknown_034C[id - 216];
    if (window == nullptr) {
      window = new UIMultiIllustWnd();
      this->unknown_034C[id - 216] = window;
      window->SetSize(4, 4);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddOverlay(id - 216, window);
    }
    window->Show(1);
    break;
  case 76:
    window = this->unknown_0344;
    if (window == nullptr) {
      window = new UIGuildBanReasonDescWnd();
      this->unknown_0344 = window;
      window->SetSize(280, 80);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 68:
    window = this->unknown_033C;
    if (window == nullptr) {
      window = new UIFriendOptionWnd();
      this->unknown_033C = window;
      window->SetSize(213, 90);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x86C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 53:
    window = this->unknown_030C;
    if (window == nullptr) {
      window = new UIPartySettingWnd();
      this->unknown_030C = window;
      window->SetSize(130, g_session.PartyCount() ? 245 : 180);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x814), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 207:
    window = this->unknown_030C;
    if (window == nullptr) {
      window = new UIPartySettingWnd();
      this->unknown_030C = window;
      window->SetSize(130, 220);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x814), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 208:
    window = this->unknown_042C;
    if (window == nullptr) {
      window = new UIPartyInvitationToWnd();
      this->unknown_042C = window;
      window->SetSize(150, 100);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 52:
    window = this->unknown_03A8;
    if (window == nullptr) {
      window = new UICompositionWnd();
      this->unknown_03A8 = window;
      window->SetSize(34, 34);
      window->Move(0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 51:
    window = this->window_03A4;
    if (window == nullptr) {
      window = new UICandidateWnd();
      this->window_03A4 = window;
      const int width = this->unknown_0068 ? 120 : 292;
      const int height = this->unknown_0068 ? 212 : 34;
      window->SetSize(width, height);
      window->Move(g_renderer->viewport.width - width, g_renderer->viewport.height - height);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 38:
    window = this->unknown_02F0;
    if (window == nullptr) {
      window = new UITipOfTheDayWnd();
      this->unknown_02F0 = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 70:
    window = this->unknown_02E8;
    if (window == nullptr) {
      window = new UIJoinGuildAcceptWnd();
      this->unknown_02E8 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 72:
    window = this->unknown_02EC;
    if (window == nullptr) {
      window = new UIAllyGuildAcceptWnd();
      this->unknown_02EC = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 35:
    window = this->unknown_02CC;
    if (window == nullptr) {
      window = new UIJoinPartyAcceptWnd();
      this->unknown_02CC = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 104:
    window = this->unknown_02D0;
    if (window == nullptr) {
      window = new UICoupleAcceptWnd();
      this->unknown_02D0 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 119:
    window = this->unknown_02D4;
    if (window == nullptr) {
      window = new UIStarPlaceAcceptWnd();
      this->unknown_02D4 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 163:
    window = this->unknown_02D8;
    if (window == nullptr) {
      window = new UIMixAcceptWnd();
      this->unknown_02D8 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 138:
    window = this->unknown_02DC;
    if (window == nullptr) {
      window = new UIYourItemWnd();
      this->unknown_02DC = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 105:
    window = this->unknown_02E0;
    if (window == nullptr) {
      window = new UIBabyAcceptWnd();
      this->unknown_02E0 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 110:
    window = this->unknown_02E4;
    if (window == nullptr) {
      window = new UIBabyAcceptWnd2();
      this->unknown_02E4 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 32:
    window = this->unknown_0284;
    if (window == nullptr) {
      window = new UIExchangeAcceptWnd();
      this->unknown_0284 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_0284->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x75C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 29:
    window = this->unknown_0280;
    if (window == nullptr) {
      window = new UIPasswordWnd();
      this->unknown_0280 = window;
      window->SetSize(176, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 15:
    window = this->window_01EC;
    if (window == nullptr) {
      window = new UIItemDropCntWnd();
      this->window_01EC = window;
      window->SetSize(182, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    break;
  case 16:
    window = this->unknown_01F0;
    if (window == nullptr) {
      window = new UISayDialogWnd();
      this->unknown_01F0 = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(100));
      this->AddRendered(window);
      this->unknown_01F0->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x5E4), 0, 0, 0);
    }
    window->Show(1);
    this->unknown_01F0->unknown_004C = 1;
    this->rendered.sort(layerGreater);
    break;
  case 226:
    window = this->unknown_01F4;
    if (window == nullptr) {
      window = new UISayDialogWnd();
      this->unknown_01F4 = window;
      window->SetSize(g_renderer->viewport.width, 100);
      window->Move(0, g_renderer->viewport.height - 100);
      this->AddOverlay(250, window);
    }
    window->Show(1);
    break;
  case 28:
    window = this->window_027C;
    if (window == nullptr) {
      window = new UIChatRoomWnd();
      this->window_027C = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->window_027C->SendMsg(nullptr, 0x35, reinterpret_cast<std::intptr_t>(base + 0x604), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 39:
    window = this->unknown_020C;
    if (window == nullptr) {
      window = new UIChooseWarpWnd();
      this->unknown_020C = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 17:
    window = this->unknown_01F8;
    if (window == nullptr) {
      window = new UIChoose3Wnd();
      this->unknown_01F8 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->unknown_01F8->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x5F4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 74:
    window = this->unknown_0208;
    if (window == nullptr) {
      window = new UIItemCompositionWnd();
      this->unknown_0208 = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_0208->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x87C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 73:
    window = this->unknown_01FC;
    if (window == nullptr) {
      window = new UIItemIdentifyWnd();
      this->unknown_01FC = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->unknown_01FC->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x88C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 26:
    window = this->unknown_0214;
    if (window == nullptr) {
      window = new UIComboBoxWnd();
      this->unknown_0214 = window;
      window->SetSize(16, 16);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 260:
    window = this->unknown_021C;
    if (window == nullptr) {
      window = new UIComboBoxIDWnd();
      this->unknown_021C = window;
      window->SetSize(16, 16);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 18:
    window = this->unknown_0210;
    if (window == nullptr) {
      window = new UIMenuWnd();
      this->unknown_0210 = window;
      window->SetSize(4, 4);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 14:
    window = this->unknown_01C0;
    if (window == nullptr) {
      window = new UIMinimapZoomWnd();
      this->unknown_01C0 = window;
      window->SetSize(128, 140);
      window->Move(g_renderer->viewport.width - 145, 17);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 30:
    window = this->window_0278;
    if (window == nullptr) {
      window = new UIChatRoomChangeWnd();
      this->window_0278 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 27:
    window = this->window_0274;
    if (window == nullptr) {
      window = new UIChatRoomMakeWnd();
      this->window_0274 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 25:
    window = this->unknown_0264;
    if (window == nullptr) {
      window = new UIChooseSellBuyWnd();
      this->unknown_0264 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 24:
    window = this->unknown_025C;
    if (window == nullptr) {
      window = new UIItemSellWnd();
      this->unknown_025C = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x70C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 161:
    window = this->unknown_0260;
    if (window == nullptr) {
      window = new UIItemSellWnd2();
      this->unknown_0260 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x70C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 44:
    window = this->unknown_0308;
    if (window == nullptr) {
      window = new UIMerchantItemPurchaseWnd(0);
      this->unknown_0308 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x7D4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 23:
    window = this->unknown_0258;
    if (window == nullptr) {
      window = new UIItemPurchaseWnd();
      this->unknown_0258 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x71C), 0, 0, 0);
    }
    window->Show(1);
    this->EnsureWindow(50);
    break;
  case 45:
    window = this->unknown_0304;
    if (window == nullptr) {
      window = new UIMerchantItemMyShopWnd(0);
      this->unknown_0304 = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x7C4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 43:
    window = this->unknown_0300;
    if (window == nullptr) {
      window = new UIMerchantItemShopWnd(0);
      this->unknown_0300 = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x7B4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 22:
    window = this->unknown_0250;
    if (window == nullptr) {
      window = new UIItemShopWnd();
      this->unknown_0250 = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 160:
    window = this->unknown_0254;
    if (window == nullptr) {
      window = new UIItemShopWnd2();
      this->unknown_0254 = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 254:
    window = this->unknown_0268;
    if (window == nullptr) {
      window = new UIParaItemShopWnd();
      this->unknown_0268 = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 255:
    window = this->unknown_026C;
    if (window == nullptr) {
      window = new UIParaItemPurchaseWnd();
      this->unknown_026C = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x71C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 256:
    window = this->window_0270;
    if (window == nullptr) {
      window = new UIParaResultWnd();
      this->window_0270 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, 0, 0, 0, 0);
    }
    window->Show(1);
    break;
  case 49:
    window = this->unknown_0244;
    if (window == nullptr) {
      window = new UINotifyJobLevelUpWnd();
      this->unknown_0244 = window;
      window->SetSize(43, 43);
      window->Move(0, g_renderer->viewport.height - 43);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 21:
    window = this->unknown_0240;
    if (window == nullptr) {
      window = new UINotifyLevelUpWnd();
      this->unknown_0240 = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 43, g_renderer->viewport.height - 43);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 143:
    window = this->unknown_0248;
    if (window == nullptr) {
      window = new UINotifyQuestWnd();
      this->unknown_0248 = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 43, g_renderer->viewport.height - 43);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 190:
    window = this->unknown_024C;
    if (window == nullptr) {
      window = new UInCash_CallWnd();
      this->unknown_024C = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 187, 16);
      this->AddRendered(window);
    }
    window->Show(g_serviceOptions.Get(218));
    break;
  case 47:
    window = this->unknown_023C;
    if (window == nullptr) {
      window = new UICardItemIllustWnd();
      this->unknown_023C = window;
      window->SetSize(300, 417);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 201:
    window = this->unknown_0424;
    if (window == nullptr) {
      window = new UIMonsterDisplayWnd();
      this->unknown_0424 = window;
      window->SetSize(100, 150);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 19:
    window = this->unknown_0238;
    if (window == nullptr) {
      window = new UIRestartWnd();
      this->unknown_0238 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 48:
    window = this->unknown_0234;
    if (window == nullptr) {
      window = new UIQuitWnd();
      this->unknown_0234 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 46:
    window = this->window_0230;
    if (window == nullptr) {
      window = new UISkillDescribeWnd();
      this->window_0230 = window;
      window->SetSize(304, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 120);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 50:
    window = this->unknown_022C;
    if (window == nullptr) {
      window = new UIItemParamChangeDisplayWnd();
      this->unknown_022C = window;
      window->SetSize(280, 34);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 34);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x804), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 82:
    window = this->unknown_0228;
    if (window == nullptr) {
      window = new UICombinedCardItemCollectionWnd();
      this->unknown_0228 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x7F4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 12:
    window = this->unknown_0218;
    if (window == nullptr) {
      window = new UIItemCollectionWnd();
      this->unknown_0218 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x7E4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 234:
    window = this->unknown_0220;
    if (window == nullptr) {
      window = new UIItemCollection_ComparisonWnd();
      this->unknown_0220 = window;
      window->SetSize(280, 120);
      this->AddRendered(window);
    }
    window->SendMsg(nullptr, 0x22, 0, 0, 0, 0);
    window->Show(1);
    break;
  case 250:
    window = this->unknown_0224;
    if (window == nullptr) {
      window = new UICombinedCardItemCollection_ComparisonWnd();
      this->unknown_0224 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x7F4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 20:
    window = new UINoticeConfirmWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case 332:
    window = new UIDisconnectedServerMsgWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case 77:
    window = new UIMonsterInfoWnd();
    window->SetSize(400, 200);
    window->Move(0, 0);
    this->AddRendered(window);
    break;
  case 79:
    window = this->unknown_0374;
    if (window == nullptr) {
      window = new UIMakeTargetListWnd();
      this->unknown_0374 = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 107:
    window = this->unknown_0200;
    if (window == nullptr) {
      window = new UIItemRepairWnd();
      this->unknown_0200 = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 111:
    window = this->unknown_0204;
    if (window == nullptr) {
      window = new UIWeaponRefineWnd();
      this->unknown_0204 = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 80:
    window = this->unknown_0378;
    if (window == nullptr) {
      window = new UIMakeTargetProcessWnd();
      this->unknown_0378 = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 81:
    window = this->unknown_037C;
    if (window == nullptr) {
      window = new UIMakeTargetResultWnd();
      this->unknown_037C = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 84:
    window = new UITalkboxTrapInputWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
    this->AddRendered(window);
    break;
  case 85:
    window = this->window_03AC;
    if (window == nullptr) {
      window = new UIKeyStrokeWnd();
      this->window_03AC = window;
      window->SetSize(60, 18);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 86:
    window = this->unknown_0380;
    if (window == nullptr) {
      window = new UIEmotionWnd();
      this->unknown_0380 = window;
      window->SetSize(375, 260);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x89C), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 87:
    window = this->unknown_0384;
    if (window == nullptr) {
      window = new (std::nothrow) UICashEmotionListWnd();
      this->unknown_0384 = window;
      if (window == nullptr) {
        return nullptr;
      }
      window->SetSize(298, 380);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x8AC), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 88:
    if (g_session.unknown_103C == -1) {
      break;
    }
    window = this->window_038C;
    if (window == nullptr) {
      window = new UIPetInfoWnd();
      this->window_038C = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x8BC), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 90:
    window = this->unknown_0390;
    if (window == nullptr) {
      window = new UIPetEggListWnd();
      this->unknown_0390 = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 91:
    window = this->unknown_0394;
    if (window == nullptr) {
      window = new UIPetTamingDeceiveWnd();
      this->unknown_0394 = window;
      window->SetSize(196, 256);
      window->Move(g_renderer->viewport.width / 2 - 98, g_renderer->viewport.height / 2 - 128);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 94:
    window = this->unknown_0398;
    if (window == nullptr) {
      window = new UIMakingArrowListWnd();
      this->unknown_0398 = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 95:
    window = this->unknown_039C;
    if (window == nullptr) {
      window = new UISelectCartWnd();
      this->unknown_039C = window;
      window->SetSize(385, 98);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 99:
    if (this->unknown_03A0) {
      this->Schedule(this->unknown_03A0);
      this->unknown_03A0 = nullptr;
    }
    window = new UISpellListWnd();
    this->unknown_03A0 = window;
    window->SetSize(200, 200);
    window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
    this->AddRendered(window);
    window->Show(1);
    break;
  case 101:
    window = new UIGraffiStrboxWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
    this->AddRendered(window);
    break;
  case 103:
    window = this->window_0388;
    if (window == nullptr) {
      window = new UIProhibitListWnd();
      this->window_0388 = window;
      window->SetSize(500, 400);
      window->Move(CenterOnViewportX(100), ScaleByScreenHeight(100));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 106:
    window = this->unknown_03B0;
    if (window == nullptr) {
      window = new UIBookWnd();
      this->unknown_03B0 = window;
      window->SetSize(600, 420);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 109:
    window = this->unknown_03B4;
    if (window == nullptr) {
      window = new UIJoinFriendAcceptWnd();
      this->unknown_03B4 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 113:
    if (g_session.m_homunJob == -1) {
      break;
    }
    window = this->window_03C0;
    if (window == nullptr) {
      window = new UIHomunInfoWnd();
      this->window_03C0 = window;
      window->SetSize(280, 208);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x8CC), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 114:
    window = this->window_03C4;
    if (window == nullptr) {
      window = new UISkillListWnd();
      this->window_03C4 = window;
      static_cast<UISkillListWnd*>(window)->skillTab = 0;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_03C4->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x8DC), 0, 0, 0);
    }
    static_cast<UISkillListWnd*>(this->window_03C4)->skillTab = 0;
    this->window_03C4->Show(1);
    this->unknown_08EC = 1;
    break;
  case 125:
    if (g_session.m_mercenaryClass == -1) {
      break;
    }
    window = this->window_03C8;
    if (window == nullptr) {
      window = new UIMerInfoWnd();
      this->window_03C8 = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x8F0), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 186:
    window = new UISelectReplayDataWnd();
    window->SetSize(280, 311);
    window->Move(CenterOnViewportX(320) - 140, CenterOnViewportY(240) - 160);
    this->AddRendered(window);
    break;
  case 187:
    if (this->window_4F168 == nullptr) {
      window = new UIReplayControlWnd();
      this->window_4F168 = window;
      window->SetSize(197, 138);
      window->Move(ScaleToViewportWidth(640) - 197, ScaleByScreenHeight(480) - 138);
      window->unknown_004C = 0;
      this->rendered.sort(layerGreater);
      this->AddRendered(window);
    }
    break;
  case 198:
    if (this->window_4F164 == nullptr) {
      if (Replay::Instance().state == 2) {
        window = new UIReplayRECControlWnd(1);
      } else {
        window = new UIReplayRECControlWnd(0);
      }
      this->window_4F164 = window;
      window->SetSize(194, 34);
      window->Move(CenterOnViewportX(259), CenterOnViewportY(230));
      this->AddRendered(window);
      this->window_4F164->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_4F154), 0, 0, 0);
    }
    break;
  case 203:
    if (this->window_4F174 == nullptr) {
      this->RecordWindowPacket(203);
      g_navigationHidden = false;
      window = new UINavigationV4Wnd();
      this->window_4F174 = window;
      window->SetSize(272, 338);
      this->window_4F174->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x4F178), 0, 0, 0);
      this->AddRendered(window);
    }
    break;
  case 229:
    if (this->LookupWindow(229) == nullptr) {
      window = new UINavigationHelpWnd();
      window->SetSize(310, 200);
      window->Move(100, 100);
      this->AddRendered(window);
    }
    break;
  case 306:
    if (this->LookupWindow(306) == nullptr) {
      int x = 200;
      int y = 200;
      if (this->window_4F174) {
        x = this->window_4F174->geometry.x;
        y = this->window_4F174->geometry.y;
      }
      window = new UINavigationroadiconWnd();
      window->SetSize(158, 102);
      window->Move(x + 113, y + 211);
      this->AddRendered(window);
    }
    break;
  case 314:
    if (this->LookupWindow(314) == nullptr) {
      int x = 200;
      int y = 200;
      if (this->window_4F174) {
        x = this->window_4F174->geometry.x;
        y = this->window_4F174->geometry.y;
      }
      window = new UINavigationRuideWnd();
      window->SetSize(150, 338);
      window->Move(x + 272, y);
      this->AddRendered(window);
    }
    break;
  case 126:
    window = this->window_03CC;
    if (window == nullptr) {
      window = new UISkillListWnd();
      this->window_03CC = window;
      static_cast<UISkillListWnd*>(window)->skillTab = 1;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_03CC->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x900), 0, 0, 0);
    }
    static_cast<UISkillListWnd*>(this->window_03CC)->skillTab = 1;
    this->window_03CC->Show(1);
    this->unknown_0910 = 1;
    break;
  case 116:
    window = this->unknown_03D0;
    if (window == nullptr) {
      window = new UIAutoMessageWnd();
      this->unknown_03D0 = window;
      window->SetSize(280, 48);
      window->Move(CenterOnViewportX(185), CenterOnViewportY(180));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 127: {
    const int width = g_session.unknown_0E94 > 12 ? 783 : 540;
    window = new UISelCharForUServerWnd();
    this->window_03D8 = window;
    window->SetSize(width, 400);
    window->Move((g_renderer->viewport.width - width) / 2, (g_renderer->viewport.height - 400) / 2);
    this->AddRendered(window);
    break;
  }
  case 128:
    window = this->window_03DC;
    if (window == nullptr) {
      window = new UIChangeNameWnd();
      this->window_03DC = window;
      window->SetSize(184, 105);
      window->Move(CenterOnViewportX(260), CenterOnViewportY(180));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 129:
    window = this->unknown_03E0;
    if (window == nullptr) {
      window = new UIRebirthWnd();
      this->unknown_03E0 = window;
      window->SetSize(280, 143);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 130:
    window = reinterpret_cast<UIWindow*>(this->unknown_03E4);
    if (window == nullptr) {
      window = new UISubChatHisWnd();
      this->unknown_03E4 = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(300, 95);
      this->AddRendered(window);
      reinterpret_cast<UIWindow*>(this->unknown_03E4)->SetId(130);
      reinterpret_cast<UIWindow*>(this->unknown_03E4)->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x914), 0, 0, 0);
    }
    window->Show(0);
    break;
  case 131:
    window = reinterpret_cast<UIWindow*>(this->unknown_03E8);
    if (window == nullptr) {
      window = new UISubChatHisWnd();
      this->unknown_03E8 = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(300, 95);
      this->AddRendered(window);
      reinterpret_cast<UIWindow*>(this->unknown_03E8)->SetId(131);
      reinterpret_cast<UIWindow*>(this->unknown_03E8)->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x924), 0, 0, 0);
    }
    window->Show(0);
    break;
  case 132:
    window = this->unknown_03EC;
    if (window == nullptr) {
      window = new UIBattleMsgOptionWnd();
      this->unknown_03EC = window;
      window->SetSize(280, 200);
      window->Move(CenterOnViewportX(400), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 133:
    window = reinterpret_cast<UIWindow*>(this->unknown_03F0);
    if (window == nullptr) {
      window = new UISubChatMiniWnd();
      this->unknown_03F0 = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(5, 5);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 137:
    if (g_session.unknown_5950 == "") {
      return nullptr;
    }
    window = this->unknown_03F8;
    if (window == nullptr) {
      window = new UIMemorialDunWnd();
      this->unknown_03F8 = window;
      window->SetSize(200, 150);
      if (g_session.unknown_5940 >= 0 && g_session.unknown_5944 >= 0) {
        window->Move(g_session.unknown_5940, g_session.unknown_5944);
      } else {
        window->Move(0, 310);
      }
      this->AddRendered(window);
    }
    window->Show(1);
    g_session.unknown_593C = 1;
    break;
  case 139:
    window = this->unknown_03FC;
    if (window == nullptr) {
      window = new UIEquipWnd();
      this->unknown_03FC = window;
      static_cast<UIEquipWnd*>(window)->m_viewOther = 1;
      window->SetSize(280, 167);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    break;
  case 140:
    window = this->modal;
    if (window == nullptr) {
      window = new UIRoMapWnd();
      this->modal = window;
      window->SetSize(g_renderer->viewport.width, g_renderer->viewport.height);
      window->Move(0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    g_windowMgr.Promote(this->modal);
    g_windowMgr.Promote(this->window_4F174);
    break;
  case 155:
    if (this->window_4F1EC || this->window_4F288) {
      break;
    }
    window = this->unknown_0408;
    if (window == nullptr) {
      window = new UIEscOptionWnd();
      this->unknown_0408 = window;
      window->SetSize(280, 143);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 156:
  case 158:
  case 159:
  case 165:
  case 166:
  case 167:
  case 181:
  case 183:
  case 191:
    window = this->CreateLayoutWindow(id);
    if (window == nullptr) {
      break;
    }
    window->Show(1);
    break;
  case 157:
    window = this->LookupWindow(157);
    if (window == nullptr) {
      window = new UIEntryQueueWnd();
      window->SetSize(350, 350);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_0944), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 209:
    window = this->LookupWindow(209);
    if (window == nullptr) {
      UIEntryQueueStandByWnd* standBy = new UIEntryQueueStandByWnd();
      window = standBy;
      standBy->Create();
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 210:
    window = this->LookupWindow(210);
    if (window == nullptr) {
      UIEntryQueueRequestWnd* request = new UIEntryQueueRequestWnd();
      window = request;
      request->Create();
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 211:
    window = this->LookupWindow(211);
    if (window == nullptr) {
      window = new UIEntryQueueHelpWnd();
      window->SetSize(100, 100);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 174:
    window = this->unknown_040C;
    if (window == nullptr) {
      window = new UIMerchantShopMakeWnd(1);
      this->unknown_040C = window;
      window->SetSize(360, 289);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(200));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x954), 0, 0, 0);
    }
    window->Show(1);
    this->EnsureWindow(175);
    break;
  case 175:
    window = reinterpret_cast<UIWindow*>(this->unknown_0410);
    if (window == nullptr) {
      window = new UIMerchantMirrorItemWnd(1);
      this->unknown_0410 = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x964), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 176:
    window = this->unknown_0414;
    if (window == nullptr) {
      window = new UIMerchantItemMyShopWnd(1);
      this->unknown_0414 = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x974), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 177:
    window = this->unknown_0418;
    if (window == nullptr) {
      window = new UIMerchantItemShopWnd(1);
      this->unknown_0418 = window;
      window->SetSize(350, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x984), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 178:
    window = this->unknown_041C;
    if (window == nullptr) {
      window = new UIMerchantItemPurchaseWnd(1);
      this->unknown_041C = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x994), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 179:
    window = reinterpret_cast<UIWindow*>(this->unknown_0420);
    if (window == nullptr) {
      window = new UIMerchantMirrorItemWnd(2);
      this->unknown_0420 = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(280, 120);
      window->Move(300, 0);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x9A4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 196:
    window = this->window_4F18C;
    if (window == nullptr) {
      window = new CUICollectionSystemWnd();
      this->window_4F18C = window;
      window->SetSize(196, 256);
      window->Move(g_renderer->viewport.width / 2 - 98, g_renderer->viewport.height / 2 - 128);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 199: {
    const int rangeX = g_renderer->viewport.width - 540;
    const int minX = rangeX / 2;
    const int rangeY = g_renderer->viewport.height - 365;
    const int minY = rangeY / 2;
    window = this->unknown_03B8;
    if (window == nullptr) {
      window = new UISecondPasswordInputWnd();
      this->unknown_03B8 = window;
      window->SetSize(540, 365);
      int x;
      int y;
      for (;;) {
        x = std::rand() % rangeX;
        y = std::rand() % rangeY;
        if (x > minX && x < rangeX && y > minY && y < rangeY) {
          break;
        }
        std::rand();
        std::rand();
      }
      window->Move(x, y);
      this->AddRendered(window);
    }
    window->Show(1);
    static_cast<UISecondPasswordInputWnd*>(this->unknown_03B8)->CreateKeypad(g_session.m_loginAccountId);
    static_cast<UISecondPasswordInputWnd*>(this->unknown_03B8)->SetInputMode(0);
    break;
  }
  case 200: {
    const int rangeX = g_renderer->viewport.width - 540;
    const int minX = rangeX / 2;
    const int rangeY = g_renderer->viewport.height - 365;
    const int minY = rangeY / 2;
    window = this->unknown_03BC;
    if (window == nullptr) {
      window = new UISecondPasswordEditWnd();
      this->unknown_03BC = window;
      window->SetSize(540, 365);
      int x;
      int y;
      for (;;) {
        x = std::rand() % rangeX;
        y = std::rand() % rangeY;
        if (x > minX && x < rangeX && y > minY && y < rangeY) {
          break;
        }
        std::rand();
        std::rand();
      }
      window->Move(x, y);
      this->AddRendered(window);
    }
    window->Show(1);
    static_cast<UISecondPasswordInputWnd*>(this->unknown_03BC)->CreateKeypad(g_session.m_loginAccountId);
    static_cast<UISecondPasswordInputWnd*>(this->unknown_03BC)->SetInputMode(1);
    break;
  }
  case 227:
    window = this->LookupWindow(227);
    if (window == nullptr) {
      window = new CMergeItemWnd();
      window->SetSize(1, 1);
      window->Show(1);
      window->Move(static_cast<int>(g_renderer->viewport.width * 0.5), static_cast<int>(g_renderer->viewport.height * 0.5));
      this->AddRendered(window);
    }
    break;
  case 230:
    if (this->LookupWindow(230) == nullptr) {
      this->CloseWindow(230);
    }
    window = new (std::nothrow) UIMerchantItemPurchaseMSGWnd();
    if (window == nullptr) {
      break;
    }
    window->SetSize(300, 150);
    window->Move((g_renderer->viewport.width - window->geometry.width) / 2, (g_renderer->viewport.height - window->geometry.height) / 2);
    window->Show(1);
    this->AddRendered(window);
    break;
  case 237:
    window = this->window_0444;
    if (window == nullptr) {
      window = new UIClanInfoManageWnd();
      this->window_0444 = window;
      window->SetSize(400, 317);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x9F0), 0, 0, 0);
    }
    window->Show(1);
    break;
  case 275:
    if (this->LookupWindow(295)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBC6)), 255, 0, 0, 0);
      break;
    }
    if (this->LookupWindow(343)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xE85)), 255, 0, 0, 0);
      break;
    }
    if (this->LookupWindow(10006)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xF05)), 255, 0, 0, 0);
      break;
    }
    if (this->LookupWindow(341)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xF7F)), 255, 0, 0, 0);
      break;
    }
    if (g_windowMgr.LookupWindow(361)) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFD7)), 255, 0, 0, 0);
      break;
    }
    window = this->window_0448;
    if (window == nullptr) {
      window = new UIBank_NewWnd();
      this->window_0448 = window;
      window->SetSize(280, 153);
      window->Move(static_cast<int>(g_renderer->viewport.width * 0.5 - 140.0), static_cast<int>(g_renderer->viewport.height * 0.5 - 77.0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 253:
    window = this->window_044C;
    if (window == nullptr) {
      window = new UIGuild_Storage_Log();
      this->window_044C = window;
      window->SetSize(400, 250);
      window->Move(static_cast<int>(g_renderer->viewport.width * 0.5 - 300.0), static_cast<int>(g_renderer->viewport.height * 0.5 - 225.0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 257:
  case 258:
    window = this->LookupWindow(id);
    if (window == nullptr) {
      if (id == 258) {
        window = new UIMerchantItemLogWnd(1);
      } else {
        window = new UIMerchantItemLogWnd(0);
      }
      window->SetSize(450, 152);
      window->Move(0, 200);
      window->Show(0);
      this->AddRendered(window);
    }
    break;
  case 262:
    window = this->unknown_0450;
    if (window == nullptr) {
      window = new UIOpenMailBoxWnd();
      this->unknown_0450 = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 234, 16);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 263:
    window = this->unknown_0454;
    if (window == nullptr) {
      window = new UIRodexInboxWnd();
      this->unknown_0454 = window;
      window->SetSize(309, 416);
      window->Move(CenterOnViewportX(100), ScaleByScreenHeight(100));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 264:
    window = this->unknown_0458;
    if (window == nullptr) {
      window = new UIRodexComposeWnd();
      this->unknown_0458 = window;
      window->SetSize(300, 400);
      window->Move(CenterOnViewportX(120), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 265:
    window = this->unknown_045C;
    if (window == nullptr) {
      window = new UIRodexContentsWnd();
      this->unknown_045C = window;
      window->SetSize(300, 400);
      window->Move(CenterOnViewportX(120), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 261:
    window = this->LookupWindow(261);
    if (window == nullptr) {
      window = new UIPetEvolutionWnd();
      window->SetSize(290, 400);
      window->Move(static_cast<int>((g_renderer->viewport.width - 290) * 0.5), static_cast<int>((g_renderer->viewport.height - 400) * 0.5));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 259:
    window = dynamic_cast<UIFrameWnd*>(this->LookupWindow(259));
    if (window == nullptr) {
      window = new UISecondCostumeWnd();
      this->window_4F170 = window;
      window->SetSize(395, 205);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200) + 46);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case 267:
    window = this->LookupWindow(267);
    if (window == nullptr) {
      window = new UIMileageWnd();
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mileage_bg1.bmp")));
      if (bitmap == nullptr) {
        window->SetSize(150, 70);
        window->Move(g_renderer->viewport.width - 480, 12);
      } else {
        window->SetSize(bitmap->image.width, bitmap->image.height);
        if (this->unknown_024C) {
          window->Move(g_renderer->viewport.width - 292, 11);
        } else {
          window->Move(g_renderer->viewport.width - bitmap->image.width - 340, 12);
        }
      }
      this->AddRendered(window);
    }
    window->Show(g_serviceOptions.Get(219));
    break;
  case 268:
    window = this->unknown_0460;
    if (window == nullptr) {
      window = new UIOpenRoulletteWnd();
      this->unknown_0460 = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 187, 71);
      this->AddRendered(window);
    }
    window->Show(g_serviceOptions.Get(220));
    break;
  case 269:
    window = this->unknown_0464;
    if (window == nullptr) {
      UIRoulletteWnd* roulette = new UIRoulletteWnd();
      window = roulette;
      this->unknown_0464 = roulette;
      window->SetSize(640, 497);
      window->Move(static_cast<int>(g_renderer->viewport.width * 0.5 - 320.0), static_cast<int>(g_renderer->viewport.height * 0.5 - 240.0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 270:
    window = dynamic_cast<UIFrameWnd*>(this->LookupWindow(270));
    if (window == nullptr) {
      window = new UIAchievementWnd();
      window->SetSize(750, 468);
      window->Move((g_renderer->viewport.width - 750) / 2, (g_renderer->viewport.height - 468) / 2);
      window->Show(1);
      this->AddRendered(window);
    }
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(307)) {
      basicInfo->SendMsg(nullptr, 6, 0x11E, 0x1D9, 0, 0);
    }
    break;
  case 271:
    window = dynamic_cast<UIFrameWnd*>(this->LookupWindow(271));
    if (window == nullptr) {
      window = new UIAchTracingWnd();
      window->SetSize(300, 150);
      window->Move(g_renderer->viewport.width - 350, (g_renderer->viewport.height - 150) / 2);
      window->Show(1);
      window->unknown_004C = 5;
      this->AddRendered(window);
      this->rendered.sort(layerGreater);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x4F1D8), 0, 0, 0);
    }
    break;
  case 273:
    if (this->window_4F1CC == nullptr) {
      window = new UIMiniMapWnd();
      this->window_4F1CC = window;
      MiniMapWndInfo* miniMapInfo = reinterpret_cast<MiniMapWndInfo*>(base + 0x4F1A8);
      window->SetSize(miniMapInfo->unknown_0010, miniMapInfo->unknown_0014);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(miniMapInfo), 0, 0, 0);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case 274:
    if (this->window_4F1D0 == nullptr) {
      window = new UIEditctrlWnd();
      this->window_4F1D0 = window;
      window->SetSize(250, 42);
      window->Move(g_mouse.x, g_mouse.y);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case 276:
    if (this->LookupWindow(276) == nullptr) {
      window = new UIRoundBoxTooltipWnd();
      this->window_4F1E4 = window;
      window->SetSize(100, 200);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 120);
      this->AddRendered(window);
      window->Show(1);
    } else {
      window = this->window_4F1E4;
      this->window_4F1E4->Show(1);
    }
    break;
  case 281:
    window = this->LookupWindow(281);
    if (window == nullptr) {
      window = new UIStylingShopWnd();
      window->SetSize(480, 330);
      window->Move((g_renderer->viewport.width - 480) / 2, (g_renderer->viewport.height - 330) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 284:
    window = this->window_4F1E8;
    if (window == nullptr) {
      window = new UICaptchaRegisterWnd();
      this->window_4F1E8 = window;
      window->SetSize(290, 211);
      window->Move((g_renderer->viewport.width - 290) / 2, (g_renderer->viewport.height - 211) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 285:
    window = this->window_4F1EC;
    if (window == nullptr) {
      window = new UIMacroDetectorWnd();
      this->window_4F1EC = window;
      window->SetSize(340, 312);
      window->Move((g_renderer->viewport.width - 340) / 2, (g_renderer->viewport.height - 312) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    this->window_4F1FC = static_cast<UIMacroDetectorWnd*>(this->window_4F1EC)->m_editCtrl;
    break;
  case 286:
    window = this->window_4F1F0;
    if (window == nullptr) {
      window = new UIMacroReporterWnd();
      this->window_4F1F0 = window;
      window->SetSize(249, 308);
      window->Move(g_renderer->viewport.width - 299, (g_renderer->viewport.height - 308) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x4F200), 0, 0, 0);
    this->unknown_4F210 = true;
    break;
  case 287:
    window = this->window_4F1F4;
    if (window == nullptr) {
      window = new UIMacroUserInfoWnd();
      this->window_4F1F4 = window;
      window->SetSize(200, 37);
      window->Move((g_renderer->viewport.width - 200) / 2, (g_renderer->viewport.height - 37) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 288:
    window = this->window_4F1F8;
    if (window == nullptr) {
      window = new UICaptchaPreviewWnd();
      this->window_4F1F8 = window;
      window->SetSize(220, 90);
      window->Move((g_renderer->viewport.width - 220) / 2, (g_renderer->viewport.height - 90) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 289:
    window = this->LookupWindow(289);
    if (window == nullptr) {
      int y = 200;
      int x = g_renderer->viewport.width - 200;
      window = new UIViewCameraInfoWnd();
      window->SetSize(150, 100);
      if (this->window_4F1CC) {
        x = this->window_4F1CC->geometry.x;
        y = this->window_4F1CC->geometry.height + 20 + this->window_4F1CC->geometry.y;
      }
      window->Move(x, y);
      window->unknown_004C = 1;
      this->AddRendered(window);
      this->rendered.sort(layerGreater);
    }
    window->Show(g_session.unknown_61B1);
    break;
  case 290:
    if (this->LookupWindow(290) == nullptr) {
      window = new UILapineBoxWnd();
      window->SetSize(400, 250);
      window->Move((g_renderer->viewport.width - 400) / 2, (g_renderer->viewport.height - 250) / 2);
      this->AddRendered(window);
    }
    break;
  case 291:
    window = this->LookupWindow(291);
    if (window == nullptr) {
      window = new UITransMenuWnd();
      window->SetSize(30, 40);
      this->AddRendered(window);
    } else {
      window->Show(1);
      this->Promote(window);
    }
    break;
  case 294:
    if (this->LookupWindow(294) == nullptr) {
      window = new UIPreviewEquipWnd();
      window->SetSize(280, 260);
      window->Move(static_cast<int>((g_renderer->viewport.width - 280) * g_depthAverageHalf), static_cast<int>((g_renderer->viewport.height - 260) * g_depthAverageHalf));
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(this->unknown_4F220), 0, 0, 0);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(294);
    }
    break;
  case 295: {
    const int x = static_cast<int>((g_renderer->viewport.width - 264) * g_depthAverageHalf);
    const int y = static_cast<int>((g_renderer->viewport.height - 346) * g_depthAverageHalf);
    this->EnsureWindow(8);
    if (this->unknown_01D4) {
      this->unknown_01D4->Move(x + 264, y - this->unknown_01D4->geometry.height + 346);
    }
    if (this->unknown_0454) {
      this->CloseWindow(264);
      this->CloseWindow(265);
      this->unknown_0454->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBAF)), 255, 0, 0, 0);
    }
    if (this->window_0448) {
      g_windowMgr.CloseWindow(275);
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBC5)), 255, 0, 0, 0);
    }
    if (this->LookupWindow(295) == nullptr) {
      window = new UIRefiningWnd();
      window->SetSize(264, 346);
      window->Move(x, y);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(295);
    }
    break;
  }
  case 302:
    if (this->LookupWindow(302) == nullptr) {
      window = new UILapineUpgradeBoxWnd();
      window->SetSize(400, 250);
      window->Move((g_renderer->viewport.width - 400) / 2, (g_renderer->viewport.height - 250) / 2);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(302);
    }
    break;
  case 303:
    if (this->LookupWindow(303) == nullptr) {
      window = new UIHotkeyGuideWnd();
      window->SetSize(934, 498);
      window->Move((g_renderer->viewport.width - 934) / 2, (g_renderer->viewport.height - 498) / 2);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(303);
    }
    break;
  case 304:
    window = this->LookupWindow(304);
    if (window == nullptr) {
      window = new UIVideoWnd();
      window->SetSize(g_renderer->viewport.width, g_renderer->viewport.height);
      window->Move(0, 0);
      this->AddRendered(window);
    }
    break;
  case 307:
    if (this->LookupWindow(307) == nullptr) {
      window = new UIMenuIconWnd();
      window->SetSize(220, 197);
      window->Move(0, 225);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(307);
    }
    break;
  case 308:
    if (this->LookupWindow(308) == nullptr) {
      window = new UIShowWarpWnd();
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->Show(1);
    } else {
      window = this->LookupWindow(308);
      window->Show(1);
    }
    break;
  case 315:
    if (this->LookupWindow(315) == nullptr) {
      window = new UITipboxWnd();
      this->window_0468 = window;
      window->SetSize(480, 365);
      window->Move(CenterOnViewportX(120), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->Show(1);
    } else {
      window = this->LookupWindow(315);
      window->Show(1);
    }
    break;
  case 318:
    if (this->LookupWindow(318) == nullptr) {
      window = new UICashShopWnd();
      this->window_4F254 = window;
      window->SetSize(723, 540);
      window->Move(static_cast<int>((g_renderer->viewport.width - 723) * 0.5), static_cast<int>((g_renderer->viewport.height - 540) * 0.5));
      window->SendMsg(nullptr, 0x22, 0, 0, 0, 0);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(318);
    }
    break;
  case 321:
    this->RecordWindowPacket(321);
    window = this->LookupWindow(id);
    if (window == nullptr) {
      window = new UIRenewQuestWnd();
      window->SetSize(381, 466);
      window->Move((g_renderer->viewport.width - 381) / 2, (g_renderer->viewport.height - 466) / 2);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x550), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->CloseWindow(143);
    this->unknown_0568 = 1;
    break;
  case 322:
    this->RecordWindowPacket(321);
    window = this->LookupWindow(id);
    if (window == nullptr) {
      window = new UIRenewDetailQuestInfoWnd();
      window->SetSize(342, 412);
      if (UIWindow* quest = this->LookupWindow(321)) {
        window->Move(quest->geometry.x + quest->geometry.width, quest->geometry.y);
      } else {
        window->Move((g_renderer->viewport.width - 342) / 2, (g_renderer->viewport.height - 412) / 2);
      }
      this->AddRendered(window);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    break;
  case 328:
    if (this->LookupWindow(328) == nullptr) {
      window = new UICheckAttendanceWnd();
      window->SetSize(488, 413);
      window->Move(static_cast<int>((g_renderer->viewport.width - 488) * 0.5), static_cast<int>((g_renderer->viewport.height - 413) * 0.5));
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(328);
    }
    break;
  case 334:
    if (this->LookupWindow(334) == nullptr) {
      window = new UIBartermarketWnd();
      this->window_4F260 = window;
      window->SetSize(280, 400);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    } else {
      window = this->LookupWindow(334);
    }
    break;
  case 335:
    if (this->LookupWindow(335) == nullptr) {
      window = new UIBarterItemPurchaseWnd();
      this->window_4F264 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0x71C), 0, 0, 0);
    } else {
      window = this->LookupWindow(335);
    }
    break;
  case 341:
    if (this->LookupWindow(275)) {
      g_windowMgr.CloseWindow(275);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xF7E)), 255, 0, 0, 0);
    }
    if (this->LookupWindow(341) == nullptr) {
      window = new UIExpandedBartermarketWnd();
      this->window_4F268 = window;
      window->SetSize(366, 366);
      window->Move(200, 200);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(341);
    }
    break;
  case 342:
    if (this->LookupWindow(275)) {
      break;
    }
    if (this->LookupWindow(342) == nullptr) {
      window = new UIExpandedBarterItemPurchaseWnd();
      this->window_4F26C = window;
      window->SetSize(366, 366);
      window->Move(586, 200);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(341);
    }
    break;
  case 333:
    if (this->LookupWindow(333) == nullptr) {
      window = new UINoteWnd();
      window->SetSize(358, 275);
      if (UIWindow* shop = this->unknown_02FC) {
        window->Move(shop->geometry.width + shop->geometry.x + 5, shop->geometry.y);
      } else {
        window->Move(static_cast<int>((g_renderer->viewport.width - 358) * 0.5), static_cast<int>((g_renderer->viewport.height - 275) * 0.5));
      }
      window->SendMsg(nullptr, 0x22, 0xC95, 0, 0, 0);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(333);
    }
    break;
  case 340:
    window = this->LookupWindow(340);
    if (window == nullptr) {
      window = new UIAccountLimitedToolWnd();
      window->SetSize(390, 350);
      window->Move(g_renderer->viewport.centerX - window->geometry.width / 2, g_renderer->viewport.centerY - window->geometry.height / 2);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case 343: {
    const int x = static_cast<int>((g_renderer->viewport.width - 400) * g_depthAverageHalf - 100.0f);
    const int y = static_cast<int>((g_renderer->viewport.height - 330) * g_depthAverageHalf);
    this->EnsureWindow(8);
    if (this->unknown_01D4) {
      this->unknown_01D4->Move(x + 400, y - this->unknown_01D4->geometry.height + 330);
    }
    if (this->unknown_0454) {
      this->CloseWindow(264);
      this->CloseWindow(265);
      this->unknown_0454->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xE82)), 255, 0, 0, 0);
    }
    if (this->window_0448) {
      g_windowMgr.CloseWindow(275);
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xE84)), 255, 0, 0, 0);
    }
    if (this->LookupWindow(343) == nullptr) {
      window = new UIGradeEnchantWnd();
      window->SetSize(400, 330);
      window->Move(x, y);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(343);
    }
    break;
  }
  case 344:
    window = this->LookupWindow(344);
    if (window == nullptr) {
      window = new UIChangeMaterialWnd();
      window->SetSize(500, 314);
      window->Move(static_cast<int>((g_renderer->viewport.width - 500) * 0.5), static_cast<int>((g_renderer->viewport.height - 314) * 0.5));
      window->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
      this->AddRendered(window);
      this->SetFocus(nullptr);
    }
    break;
  case 324:
    window = this->window_4F274;
    if (window == nullptr) {
      window = new UIAdvenPartyBoardWnd();
      this->window_4F274 = window;
      window->SetSize(480, 340);
      window->Move(g_renderer->viewport.centerX - window->geometry.width / 2, g_renderer->viewport.centerY - window->geometry.height / 2);
      this->AddRendered(window);
      this->window_4F274->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0xA20), 0, 0, 0);
    }
    break;
  case 323:
    window = this->window_4F270;
    if (window == nullptr) {
      window = new UIRegisterPartyWnd();
      this->window_4F270 = window;
      window->SetSize(220, 215);
      window->Move(g_renderer->viewport.centerX + 240, g_renderer->viewport.centerY + 170);
      this->AddRendered(window);
      this->window_4F270->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0xA00), 0, 0, 0);
    }
    break;
  case 326:
    window = this->window_4F278;
    if (window == nullptr) {
      window = new UIRequestJoinPartyWnd();
      this->window_4F278 = window;
      window->SetSize(220, 160);
      window->Move(g_renderer->viewport.centerX - window->geometry.width / 2, g_renderer->viewport.centerY - window->geometry.height / 2);
      this->AddRendered(window);
    }
    break;
  case 345:
    window = this->window_4F27C;
    if (window == nullptr) {
      window = new UIApplyForPartyWnd();
      this->window_4F27C = window;
      window->SetSize(220, 190);
      window->Move(g_renderer->viewport.centerX + 240, g_renderer->viewport.centerY - 170);
      this->AddRendered(window);
      this->window_4F27C->SendMsg(nullptr, 0x22, reinterpret_cast<std::intptr_t>(base + 0xA10), 0, 0, 0);
    }
    break;
  case 346:
    window = this->LookupWindow(346);
    if (window == nullptr) {
      window = new UIReputeWnd();
      window->SetSize(640, 450);
      window->Move(static_cast<int>((g_renderer->viewport.width - 640u) * 0.5), static_cast<int>((g_renderer->viewport.height - 450u) * 0.5));
      this->AddRendered(window);
    }
    break;
  case 348:
    window = this->LookupWindow(348);
    if (window == nullptr) {
      window = new UIItemReformWnd();
      window->SetSize(440, 260);
      window->Move(static_cast<int>((g_renderer->viewport.width - 440u) * 0.5), static_cast<int>((g_renderer->viewport.height - 260u) * 0.5));
      this->AddRendered(window);
    }
    break;
  case 358:
    window = this->LookupWindow(358);
    if (window == nullptr) {
      window = new UIBalloonOpenBtn();
      window->SetSize(18, 18);
      window->Move(g_renderer->viewport.width - 200, static_cast<int>(g_renderer->viewport.height * 0.5));
      this->AddRendered(window);
    }
    break;
  case 359:
    window = this->window_4F288;
    if (window == nullptr) {
      window = new UIMacroBlackListCheckWnd();
      this->window_4F288 = window;
      window->SetSize(540, 365);
      window->Move((g_renderer->viewport.width - 540) / 2, (g_renderer->viewport.height - 365) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 361:
    window = this->LookupWindow(361);
    if (window == nullptr) {
      g_runeSystemMgr->SendRequest(0xBCB, nullptr, -1);
      if (this->unknown_0454) {
        this->CloseWindow(264);
        this->CloseWindow(265);
        this->unknown_0454->SendMsg(nullptr, 6, 0xC9, 0, 0, 0);
        this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFD8)), 255, 0, 0, 0);
      }
      if (this->window_0448) {
        g_windowMgr.CloseWindow(275);
        this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFD9)), 255, 0, 0, 0);
      }
      window = new UIRuneSystemWnd();
      window->SetSize(910, 539);
      window->Move(static_cast<int>((g_renderer->viewport.width - window->geometry.width) * 0.5), static_cast<int>((g_renderer->viewport.height - window->geometry.height) * 0.5));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case 362:
    window = this->LookupWindow(362);
    if (window == nullptr) {
      window = new UIRuneSystem_DecomResultWnd();
      window->SetSize(240, 180);
      if (UIWindow* runeSystem = this->LookupWindow(361)) {
        window->Move(runeSystem->geometry.x + 478, runeSystem->geometry.y + 129);
      }
      this->AddRendered(window);
    }
    window->Show(1);
    this->Promote(window);
    break;
  case 10032:
    if (this->LookupWindow(10032) == nullptr) {
      window = new UIDebuffRemoveWnd();
      window->SetSize(180, 47);
      window->Move(static_cast<int>((g_renderer->viewport.width - 180) * 0.5), static_cast<int>((g_renderer->viewport.height - 47) * 0.5));
      this->AddRendered(window);
    }
    break;
  default:
    if (Singleton<CUIRegisterMgr>::GetInstance()->IsRegistered(id)) {
      window = this->LookupWindow(id);
      if (window == nullptr) {
        window = Singleton<CUIRegisterMgr>::GetInstance()->Create(id);
        if (window == nullptr) {
          break;
        }
        this->AddRendered(window);
      }
    }
    break;
  }

  if (window) {
    window->SetId(id);
    this->windows[id] = window;
    UIWindow* root = window;
    while (root->parent) {
      root = root->parent;
    }
    this->window_4F1D4 = root;
  }

  bool notify = false;
  auto transientEntry = std::find(this->rendered.begin(), this->rendered.end(), this->window_4F258);
  if (transientEntry != this->rendered.end()) {
    this->rendered.erase(transientEntry);
    this->rendered.push_back(this->window_4F258);
    notify = true;
  }

  auto topEntry = std::find_if(this->rendered.begin(), this->rendered.end(), [](const UIWindow* entry) {
    return entry->geometry.id == 363;
  });
  if (topEntry != this->rendered.end()) {
    UIWindow* top = *topEntry;
    this->rendered.erase(topEntry);
    this->rendered.push_back(top);
    notify = true;
  }

  if (notify) {
    switch (id) {
    case 70:
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFDA)), 0xFFFF00, 0, 0, 0);
      break;
    case 72:
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFDB)), 0xFFFF00, 0, 0, 0);
      break;
    case 109:
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFDC)), 0xFFFF00, 0, 0, 0);
      break;
    case 35:
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFDD)), 0xFFFF00, 0, 0, 0);
      break;
    case 32:
      this->SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFDE)), 0xFFFF00, 0, 0, 0);
      break;
    }
  }
  return window;
}
