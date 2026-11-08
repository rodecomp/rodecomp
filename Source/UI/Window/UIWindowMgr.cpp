#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Hotkeys/InterfaceHotkey.h"
#include "UI/Window/WindowId.h"
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
#include "Resource/MSGSTRINGID.h"
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
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;

  AiCommand() {
    std::memset(this, 0, sizeof(*this));
  }
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
      if (window->GetTransBoxInfo(&rectangle) == true) {
        DrawAlphaRectangle(rectangle.x, rectangle.y, rectangle.width, rectangle.height, rectangle.color);
        g_renderDiagnosticLine = 3823;
        g_renderDiagnosticFile = g_windowManagerDiagnosticFile;
      } else if (window->GetCustomTransBoxInfo(&rectangle) == true) {
        window->DrawTransBox();
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
      if (window == this->roulletteWnd) {
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
  case WID_LOGINWND:
    return this->window_01CC;
  case WID_LOGINOTPWND:
    return this->window_01D0;
  case WID_CHATWND:
    return this->window_01C8;
  case WID_CHATROOMMAKEWND:
    return this->window_0274;
  case WID_CHATROOMCHANGEWND:
    return this->window_0278;
  case WID_CHATROOMWND:
    return this->window_027C;
  case WID_MESSENGERGROUPWND:
    return this->window_02C8;
  case WID_SKILLLISTWND:
    return this->window_02C4;
  case WID_GUILDMEMBERMANAGEWND:
    return this->window_0324;
  case WID_GUILDINFOMANAGEWND:
    return this->window_0320;
  case WID_PETINFOWND:
    return this->window_038C;
  case WID_GUILDSKILLWND:
    return this->window_032C;
  case WID_PROHIBITLISTWND:
    return this->window_0388;
  case WID_HOMUNINFOWND:
    return this->window_03C0;
  case WID_SKILLLISTWND_SKILLTAB_0:
    return this->window_03C4;
  case WID_MERINFOWND:
    return this->window_03C8;
  case WID_SKILLLISTWND_SKILLTAB_1:
    return this->window_03CC;
  case WID_ITEMSTOREWND:
    return this->window_0288;
  case WID_COLLECTIONSYSTEMWND:
    return this->window_4F18C;
  case WID_QUESTDISPLAY:
    return this->window_0428;
  case WID_NAVIGATIONV4WND:
    return this->window_4F174;
  case WID_CLANINFOMANAGEWND:
    return this->window_0444;
  case WID_GUILD_STORAGE_LOG:
    return this->window_044C;
  case WID_MINIMAPWND:
    return this->window_4F1CC;
  case WID_NEWSELECTCHARWND:
    return this->window_03D4;
  case WID_BANK_NEWWND:
    return this->window_0448;
  case WID_EDITCTRLWND:
    return this->window_4F1D0;
  case WID_CAPTCHAREGISTERWND:
    return this->window_4F1E8;
  case WID_MACRODETECTORWND:
    return this->window_4F1EC;
  case WID_MACROREPORTERWND:
    return this->window_4F1F0;
  case WID_MACROUSERINFOWND:
    return this->window_4F1F4;
  case WID_CAPTCHAPREVIEWWND:
    return this->window_4F1F8;
  case WID_MINIPARTYWND:
    if (!this->snapExcluded.empty()) {
      return this->snapExcluded.front();
    }
    [[fallthrough]];
  case WID_CASHSHOP_WND:
    return this->window_4F254;
  case WID_TIPBOXWND:
    return this->window_0468;
  case WID_RPIMAGEWND2:
    return this->window_4F25C;
  case WID_MACROBLACKLISTCHECKWND:
    return this->window_4F288;
  case WID_WAITDBSAVEWND:
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
  this->loadingWnd = 0;
  this->basicInfoWnd = 0;
  this->itemWnd = 0;
  this->equipWnd = 0;
  this->nCashWnd = 0;
  this->statusWnd = 0;
  this->minimapZoomWnd = 0;
  this->window_01EC = nullptr;
  this->sayDialogWnd = 0;
  this->sayDialogOverlayWnd = 0;
  this->choose3Wnd = 0;
  this->itemIdentifyWnd = 0;
  this->itemCompositionWnd = 0;
  this->chooseWarpWnd = 0;
  this->menuWnd = 0;
  this->comboBoxWnd = 0;
  this->itemCollectionWnd = 0;
  this->comboBoxIDWnd = 0;
  this->itemCollectionComparisonWnd = 0;
  this->combinedCardItemCollectionWnd = 0;
  this->itemParamChangeDisplayWnd = 0;
  this->window_0230 = nullptr;
  this->quitWnd = 0;
  this->restartWnd = 0;
  this->cardItemIllustWnd = 0;
  this->notifyLevelUpWnd = 0;
  this->notifyJobLevelUpWnd = 0;
  this->notifyQuestWnd = 0;
  this->nCashCallWnd = 0;
  this->itemShopWnd = 0;
  this->itemShopWnd2 = 0;
  this->itemPurchaseWnd = 0;
  this->itemSellWnd = 0;
  this->itemSellWnd2 = 0;
  this->chooseSellBuyWnd = 0;
  this->paraItemShopWnd = 0;
  this->paraItemPurchaseWnd = 0;
  this->window_0274 = nullptr;
  this->window_0278 = nullptr;
  this->window_027C = nullptr;
  this->passwordWnd = 0;
  this->exchangeAcceptWnd = 0;
  this->window_0288 = nullptr;
  this->itemStoreTabWnds[0] = 0;
  this->itemStoreTabWnds[1] = 0;
  this->itemStoreTabWnds[2] = 0;
  this->itemStoreTabWnds[3] = 0;
  this->itemStoreTabWnds[4] = 0;
  this->itemStoreTabWnds[5] = 0;
  this->itemStoreTabWnds[6] = 0;
  this->itemStoreLastTabWnd = 0;
  this->itemStoreFindWnd = 0;
  this->window_02C4 = nullptr;
  this->window_02C8 = nullptr;
  this->joinPartyAcceptWnd = 0;
  this->coupleAcceptWnd = 0;
  this->starPlaceAcceptWnd = 0;
  this->mixAcceptWnd = 0;
  this->yourItemWnd = 0;
  this->babyAcceptWnd = 0;
  this->babyAcceptWnd2 = 0;
  this->joinGuildAcceptWnd = 0;
  this->allyGuildAcceptWnd = 0;
  this->window_01E8 = nullptr;
  this->tipOfTheDayWnd = 0;
  this->merchantItemWnd = 0;
  this->merchantMirrorItemWnd = 0;
  this->merchantShopMakeWnd = 0;
  this->merchantItemShopWnd = 0;
  this->merchantItemPurchaseWnd = 0;
  this->merchantItemMyShopWnd = 0;
  this->window_03A4 = 0;
  this->compositionWnd = 0;
  this->window_03AC = 0;
  this->partySettingWnd = 0;
  this->skillNameChangeWnd = 0;
  this->npcEditDialogWnd = 0;
  this->npcTextEditDialogWnd = 0;
  this->notifyItemObtainWnd = 0;
  this->window_0320 = nullptr;
  this->window_0324 = nullptr;
  this->guildPositionManageWnd = 0;
  this->window_032C = nullptr;
  this->guildBanishedMemberWnd = 0;
  this->guildNoticeWnd = 0;
  this->guildTotalInfoWnd = 0;
  this->friendOptionWnd = 0;
  this->guildLeaveReasonDescWnd = 0;
  this->guildBanReasonDescWnd = 0;
  this->illustWnd = 0;
  this->multiIllustWnds[0] = 0;
  this->multiIllustWnds[1] = 0;
  this->multiIllustWnds[2] = 0;
  this->multiIllustWnds[3] = 0;
  this->multiIllustWnds[4] = 0;
  this->multiIllustWnds[5] = 0;
  this->multiIllustWnds[6] = 0;
  this->multiIllustWnds[7] = 0;
  this->multiIllustWnds[8] = 0;
  this->multiIllustWnds[9] = 0;
  this->window_038C = nullptr;
  this->petEggListWnd = 0;
  this->petTamingDeceiveWnd = 0;
  this->itemRepairWnd = 0;
  this->makingArrowListWnd = 0;
  this->selectCartWnd = 0;
  this->emotionWnd = 0;
  this->cashEmotionListWnd = 0;
  this->bookWnd = 0;
  this->joinFriendAcceptWnd = 0;
  this->window_03C0 = nullptr;
  this->window_03C4 = nullptr;
  this->window_03C8 = nullptr;
  this->window_03CC = nullptr;
  this->autoMessageWnd = 0;
  this->secondPasswordInputWnd = 0;
  this->secondPasswordEditWnd = 0;
  this->window_03D4 = nullptr;
  this->window_03D8 = nullptr;
  this->rebirthWnd = 0;
  this->subChatHistoryWnd = 0;
  this->secondSubChatHistoryWnd = 0;
  this->subChatMiniWnd = 0;
  this->battleMsgOptionWnd = 0;
  this->memorialDunWnd = 0;
  this->otherEquipWnd = 0;
  this->modal = 0;
  this->hotkeyWnd = 0;
  this->escOptionWnd = 0;
  this->buyingShopMakeWnd = 0;
  this->buyingShopMirrorItemWnd = 0;
  this->buyingShopMyItemsWnd = 0;
  this->buyingShopItemsWnd = 0;
  this->buyingShopPurchaseWnd = 0;
  this->monsterDisplayWnd = 0;
  this->retainedWindows.clear();
  this->seekPartyWnd = 0;
  this->seekPartyMessageWnd = 0;
  this->jobListWnd = 0;
  this->seekPartyListWnd = 0;
  this->partyBookingHelpWnd = 0;
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
  this->weaponRefineWnd = 0;
  this->makeTargetProcessWnd = 0;
  this->makeTargetListWnd = 0;
  this->window_4F170 = nullptr;
  this->openMailBoxWnd = 0;
  this->rodexInboxWnd = 0;
  this->rodexComposeWnd = 0;
  this->rodexContentsWnd = 0;
  this->guildHelperWnd = 0;
  this->guildTipWnd = 0;
  this->createGuildWnd = 0;
  this->openRoulletteWnd = 0;
  this->roulletteWnd = 0;
  this->hiddenWindows.clear();
  const bool hadWindow0270 = this->window_0270 != nullptr;
  this->windowVisibilityState = 0;
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
  this->partyInvitationToWnd = 0;
  this->window_4F260 = nullptr;
  this->window_4F264 = nullptr;
  this->window_4F268 = nullptr;
  this->window_4F26C = nullptr;
  this->window_4F270 = nullptr;
  this->window_4F274 = nullptr;
  this->window_4F278 = nullptr;
  this->window_4F27C = nullptr;
  this->purchaseMirrorItemWnd = 0;
  this->createGuildAlternateWnd = 0;
  this->window_01D0 = nullptr;
  this->combinedCardItemCollectionComparisonWnd = 0;
  if (hadWindow0270) {
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(MM_NPC_MARKET_CLOSE, 0, 0, 0, 0);
    }
  }
  const bool hadWindow276 = this->window_4F1E4 != nullptr;
  this->window_0270 = nullptr;
  this->makeTargetResultWnd = 0;
  this->window_0388 = nullptr;
  this->spellListWnd = 0;
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
    : activeWindowId(0), retainedPersistentWindow(false), window_03DC(nullptr), previousMouseX(-1), previousMouseY(-1), unknown_0526(true),
      window_0A30(nullptr), macroReporterVisible(false), unknown_4F211(false), window_4F2AC(nullptr) {
  this->ResetWindows();
  this->backgroundSurface = nullptr;
  this->screenWidth = 0;
  this->screenHeight = 0;
  this->chatInputText.assign("", 0);
  this->chatWndStatus = 1;
  this->chatWndHeight = 98;
  this->chatWndX = 0;
  this->chatWndY = 0;
  this->chatWndVisible = 1;
  this->minimapZoom = 1.0f;
  this->minimapMarkersVisible = 1;
  this->minimapColor = 0xAAFFFFFFu;
  this->drawCompass = 1;
  this->unknown_04F8 = 0;
  this->unknown_04F4 = 0;
  this->framesInvalidated = 0;
  this->chatWndStickOn = 1;
  this->battleSubChatEnabled = 0;
  this->compactCompositionLayout = g_language == 2;
  this->skillDescriptionVisible = 0;
  this->loginGridX = 0;
  if (g_serviceType == 0) {
    this->loginGridY = -25;
  } else {
    this->loginGridY = 0;
  }
  this->loginSidePanelOffset = {};
  this->loginBottomPanelOffset = {};
  this->loginButtonPanelOffset = {};
  this->loginExtraPanelOffset = {};
  this->loginDragRegion = 0;
  this->gradeImageWidth = 203;
  this->gradeImageHeight = 115;
  this->gradeImageX = 0;
  this->gradeImageY = 0;
  this->windowHistory.clear();
  this->lastWindowId = -1;
  this->inputSuggestionsEnabled = 0;
  this->chatModeChanged = 1;
  this->windowVisibilityState = 0;
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
  this->itemStoreWindowCount = 0;
  this->miniPartyWidth = 151;
  this->miniPartyHeight = 37;
}

// OFFSET: 0x007375E0
// FUNCTION: fn_07649
__declspec(noinline) CMiniPartyWndAreaMgr::CMiniPartyWndAreaMgr() {
  SetRect(&this->area, 0, 0, g_windowMgr.screenWidth, g_windowMgr.screenHeight);
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
      window->StoreInfo();
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

    if (window == reinterpret_cast<UIWindow*>(this->memorialDunWnd) && g_session.memorialWindowOpen != 0) {
      g_session.memorialWindowX = window->geometry.x;
      g_session.memorialWindowY = window->geometry.y;
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
    window->StoreInfo();
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
  if (this->subChatHistoryWnd) {
    saved03E4 = this->subChatHistoryWnd;
  }
  std::uint32_t saved03E8 = 0;
  if (this->secondSubChatHistoryWnd) {
    saved03E8 = this->secondSubChatHistoryWnd;
  }
  std::uint32_t saved03F0 = 0;
  if (this->subChatMiniWnd) {
    saved03F0 = this->subChatMiniWnd;
  }
  UIWindow* saved01C8 = nullptr;
  if (this->window_01C8) {
    saved01C8 = this->window_01C8;
  }

  std::map<int, UIWindow*> savedWindows(this->retainedWindows);
  this->ResetWindows();
  this->window_01C8 = saved01C8;
  this->subChatHistoryWnd = saved03E4;
  this->subChatMiniWnd = saved03F0;
  this->secondSubChatHistoryWnd = saved03E8;
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
  if (this->backgroundSurface) {
    delete this->backgroundSurface;
    this->backgroundSurface = nullptr;
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
  this->window_01C8->SendMsg(field, UM_COMBOBOX_SELECTED, index, 0, 0, 0);
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
      label = LookupMessage(MSI_FRIEND);
    } else if (g_session.IsGuildMemberCharacter(accountId)) {
      label = "GuildMember";
    } else if (g_session.IsFriendNameDifferent(name)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ID_WARNING)), 0xFFFF, 0, 0, 0);
    } else if (g_session.IsGuildMemberNameDifferent(name)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ID_WARNING_GUILD)), 0xFFFF, 0, 0, 0);
    }

    shown = this->SendNotice(UIM_MAKE_WHISPER_WINDOW, reinterpret_cast<std::intptr_t>(name), reinterpret_cast<std::intptr_t>(label), reinterpret_cast<std::intptr_t>(ShortcutFilename(accountId).c_str()), 0, 0);
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
  window->SendMsg(nullptr, UM_ADDCHAT, reinterpret_cast<std::intptr_t>(text), color, 2, 0);
  if ((this->modal == nullptr || this->window_4F174 == window) && !this->LookupWindow(WID_NEWMAKECHARWND) && !this->LookupWindow(WID_MAKECHARWND) && !this->LookupWindow(WID_SECONDPASSWORDINPUTWND) &&
      !this->LookupWindow(WID_SECONDPASSWORDEDITWND)) {
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
    mode->SendMsg(MM_NPC_MARKET_CLOSE, 0, 0, 0, 0);
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
      if (g_serviceOptions.Get(250)) {
        (*it)->Show(1);
      } else {
        (*it)->Show(0);
      }
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
    this->namedWindows[name]->SendMsg(nullptr, UM_MENU_SELECTED, 0, 0, 0, 0);
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
  this->framesInvalidated = 0;
  if (this->backgroundSurface) {
    int x = static_cast<int>(GetRenderSystemHolder()->Width() - this->screenWidth) / 2;
    int height = this->screenHeight;
    int y = static_cast<int>(GetRenderSystemHolder()->Height() - height) / 2;
    reinterpret_cast<CSurface*>(this->backgroundSurface)->Stretch(x, y, this->screenWidth, height);
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
    *reinterpret_cast<int*>(&chunk[6]) = this->dragAllItems;
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
  this->equipWndVisible = 0;
  this->statusWndVisible = 0;
  this->questWndVisible = 0;
  this->itemWndVisible = 0;
  this->messengerGroupWndVisible = 0;
  this->skillListWndVisible = 0;
  this->optionWndVisible = 0;
  this->shortenItemWndVisible = 0;
  this->m_normalItemSlotWindow.show = options.showItemSlots;
  this->chatWndVisible = options.recordRestricted;
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
        std::memcpy(&this->dragAllItems, data.data() + offset, header.size);
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
  this->imeInputActive = 1;
  if (g_modeMgr.GetGameMode() == nullptr) {
    return this->ProcessHotkeyAction(key, pressed);
  }

  if (this->LookupWindow(WID_HOTKEYWND) != nullptr) {
    this->imeInputActive = 0;
  } else if (!this->inputSuggestionsEnabled) {
    UIWindow* hover = this->hoverWindow;
    UIWindow* parent = hover ? hover->parent : nullptr;
    if (!hover || !parent || parent == this->window_01C8 || !parent->geometry.visible) {
      this->imeInputActive = 0;
    }
  }

  int action = this->TranslateHotkey(key, keyData);
  if (this->hotkeyWnd != nullptr) {
    static_cast<UIHotKeyWnd*>(this->hotkeyWnd)->AssignSelectedHotkey(action);
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
      case VK_TAB:
      case VK_RETURN:
      case VK_SHIFT:
      case VK_CONTROL:
      case VK_MENU:
      case VK_CAPITAL:
      case VK_INSERT:
      case VK_F1:
      case VK_F2:
      case VK_F3:
      case VK_F4:
      case VK_F5:
      case VK_F6:
      case VK_F7:
      case VK_F8:
      case VK_F9:
      case VK_F10:
      case VK_F11:
      case VK_F12:
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
    this->imeInputActive = 1;
  }
  return true;
}

// OFFSET: 0x00AA57E0
// FUNCTION: fn_17224
void UIWindowMgr::SendEmotionHotkey(int index) {
  char text[256];
  const EmotionHotkeys* emotions = &g_session.m_emotionHotkeys;
  std::strcpy(text, emotions->current[index].c_str());

  if (g_windowMgr.basicInfoWnd != nullptr) {
    const bool linked = HasTextLinks(std::string(text));
    if (linked) {
      g_windowMgr.Confirm(LookupMessage(MSI_UNDEAD_MONSTERINFO), 0, true, false, 0, "Message", 280, 120, 0);
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
        g_windowMgr.chatInputText = text + offset;
      }
      g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, talkType, reinterpret_cast<int>(parts), 0, 0);
    }
    return;
  }

  g_windowMgr.chatInputText = text;
  const int& chatTarget = *reinterpret_cast<const int*>(&g_session.itemSendState[0x8]);
  if (chatTarget == 2) {
    if (g_guildInfo.m_guildId != 0) {
      mode->SendMsg(MM_CHAT_GUILD, 0, 0, 0, 0);
      return;
    }
  } else {
    if (chatTarget == 1 && g_session.PartyCount() != 0) {
      mode->SendMsg(MM_CHAT_PARTY, 0, 0, 0, 0);
      return;
    }
    if (chatTarget == 3 && g_clanInfo->m_hasClan) {
      mode->SendMsg(MM_CHAT_CLAN, 0, 0, 0, 0);
      return;
    }
  }
  mode->SendMsg(MM_CHATMSG, 0, 0, 0, 0);
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

  const int state = this->windowVisibilityState;
  if (state == 0) {
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, 222, 0, 0, 0);
    }
    if (this->hiddenWindows.size() == 0) {
      for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
        if ((*it)->geometry.visible) {
          this->hiddenWindows.push_back(*it);
        }
      }
    }
    this->windowVisibilityState = 1;
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
      if (this->basicInfoWnd) {
        this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, 222, 0, 0, 0);
      }
      return;
    }

    for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
      UIWindow* window = *it;
      if (window != reinterpret_cast<UIWindow*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->EnsureBroadcastBalloon())) {
        window->Show(0);
      }
    }
    this->CloseWindow(WID_MINIMAPZOOMWND);
    CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    if (player) {
      player->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      player->skillEffect = 0;
    }
    this->windowVisibilityState = 2;
  } else if (state == 2) {
    bool closed = false;
    for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
      if (*it != reinterpret_cast<UIWindow*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->EnsureBroadcastBalloon()) &&
          *it != static_cast<CGameMode*>(g_modeMgr.GetGameMode())->achievementBalloon && (*it)->geometry.visible) {
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
      if (this->basicInfoWnd) {
        this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, 222, 0, 0, 0);
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
    if (mode && mode->canRotateView == 1) {
      this->EnsureWindow(WID_MINIMAPZOOMWND);
    }
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->RefreshEffect164();
    this->windowVisibilityState = 1;
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
          window->SendMsg(nullptr, UM_REPLAY_FRAMED_DATA, reinterpret_cast<std::intptr_t>(payload.data()), payload.size(), 0, 0);
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
  case UIM_RESOLVE_PLAYER_TAGS:
    for (auto it = this->namedWindows.begin(); it != this->namedWindows.end(); ++it) {
      it->second->SendMsg(nullptr, UM_RESOLVE_PLAYER_TAGS, a, 0, 0, 0);
    }
    break;
  case UIM_MAKE_WHISPER_WINDOW: {
    std::string name(text);
    if (this->namedWindows.find(name) == this->namedWindows.end()) {
      UIWhisperWnd* window = new UIWhisperWnd();
      window->SetSize(280, 120);
      this->rendered.push_back(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&this->m_whisperPlacement), 0, 0, 0);
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
  case UIM_SAVE_CHAT_TO_FILE_FROM_CHATROOM:
    if (this->window_027C != nullptr) {
      static_cast<UIChatRoomWnd*>(this->window_027C)->SaveChatLog();
    }
    break;
  case UIM_SAVE_CHAT_TO_FILE:
    if (this->window_01C8 != nullptr) {
      static_cast<UINewChatWnd*>(this->window_01C8)->SaveChatLogs();
    }
    break;
  case UIM_PUSH_INTO_CHATROOM:
    if (this->window_027C != nullptr) {
      this->window_027C->SendMsg(nullptr, UM_ADDCHAT, a, b, 0, 0);
    }
    break;
  case UIM_PUSHINTOCHATHISTORY:
  case UIM_PUSHINTOCHATHISTORY2: {
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
        this->pendingNoticeLines.push_back({line, color});
      }
    } else {
      if (color == 0) {
        color = 0xFFFFFF;
      }
      if (std::strcmp(text, "No Msg") != 0 && std::strcmp(text, "NO MSG") != 0) {
        this->window_01C8->SendMsg(nullptr, selector == UIM_PUSHINTOCHATHISTORY2 ? UM_ADDCHAT2 : UM_ADDCHAT, a, color, c, d);
      }
    }
    break;
  }
  case UIM_LOADINGPERCENT:
    if (this->loadingWnd != nullptr) {
      this->loadingWnd->SendMsg(nullptr, UM_LOADINGPERCENT, a, 0, 0, 0);
    }
    break;
  case UIM_OPENCHATWND:
    this->ShowWindowById(WID_CHATWND, 1);
    if (this->window_01C8 != nullptr) {
      this->window_01C8->SendMsg(nullptr, UM_OPENCHATWND, 0, 0, 0, 0);
    }
    break;
  case UIM_CHANGECHATWNDSTATUS:
    if (this->window_01C8 != nullptr) {
      this->window_01C8->SendMsg(nullptr, UM_CHANGESTATUS, a, 0, 0, 0);
    }
    break;
  case UIM_IME_OPENCANDIDATE:
  case UIM_IME_CHANGECANDIDATE:
    this->EnsureWindow(WID_CANDIDATEWND)->SendMsg(nullptr, UM_IME_SET_CANDIDATE, a, b, 0, 0);
    break;
  case UIM_IME_CLOSECANDIDATE:
    this->CloseWindow(WID_CANDIDATEWND);
    break;
  }
  return 0;
}

// OFFSET: 0x00AA8F50
// FUNCTION: fn_17252
__declspec(noinline) void UIWindowMgr::BroadcastMessage21() {
  UIWindow* window = this->window_03DC;
  if (window || (window = this->bookWnd) != nullptr) {
    window->SendMsg(nullptr, UM_RIGHTPUSH, 0, 0, 0, 0);
    return;
  }

  for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    if ((*it)->IsFrameWnd()) {
      (*it)->SendMsg(nullptr, UM_RIGHTPUSH, 0, 0, 0, 0);
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
  if (!this->modalWindows.empty() && this->modalWindows.back() && this->hotkeyWnd) {
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
  if (HIBYTE(GetAsyncKeyState(VK_MENU)) && primary != VK_MENU) {
    this->modifierKey = VK_MENU;
  } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT)) && primary != VK_SHIFT) {
    this->modifierKey = VK_SHIFT;
  } else if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) && primary != VK_CONTROL) {
    this->modifierKey = VK_CONTROL;
  } else if (HIBYTE(GetAsyncKeyState(VK_TAB)) && primary != VK_TAB) {
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
      std::vector<unsigned char> state = window->GetReplayData();
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
  this->closedMiniPartyIds.push_back(id);
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
    this->Top()->SendMsg(nullptr, UM_DEFPUSH, 0, 0, 0, 0);
    return true;
  }

  UIWindow* window = this->quitWnd;
  if (window == nullptr) {
    window = this->npcEditDialogWnd;
    if (window != nullptr) {
      if (Replay::Instance().state == 2) {
        window->RecordEditTexts();
      }
      window->SendMsg(nullptr, UM_DEFPUSH, 0, 0, 0, 0);
      return true;
    }
    window = this->npcTextEditDialogWnd;
    if (window != nullptr) {
      if (Replay::Instance().state == 2) {
        window->RecordEditTexts();
      }
      window->SendMsg(nullptr, UM_DEFPUSH, 0, 0, 0, 0);
      return true;
    }
    window = this->choose3Wnd;
    if (window == nullptr) {
      window = this->sayDialogWnd;
    }
    if (window == nullptr) {
      window = this->sayDialogOverlayWnd;
    }
    if (window == nullptr) {
      window = this->comboBoxWnd;
    }
    if (window == nullptr) {
      window = this->comboBoxIDWnd;
      if (window != nullptr) {
        window->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
        return true;
      }
      window = this->chooseWarpWnd;
      if (window == nullptr) {
        window = this->makeTargetListWnd;
      }
      if (window == nullptr) {
        window = this->makeTargetProcessWnd;
      }
      if (window == nullptr) {
        window = this->partySettingWnd;
      }
      if (window == nullptr) {
        if (this->hoverWindow != nullptr) {
          if (this->window_01C8 != nullptr && this->hoverWindow->parent == this->window_01C8) {
            if (static_cast<const UINewChatWnd*>(this->window_01C8)->SuggestionsVisible()) {
              UIWindow* child = static_cast<const UINewChatWnd*>(this->window_01C8)->m_suggestions;
              if (child != nullptr) {
                child->SendMsg(nullptr, UM_CASH_EMOTION_KEY, 0xD, 0, 0, 0);
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
          this->hoverWindow->SendMsg(nullptr, UM_DEFPUSH, 0, 0, 0, 0);
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
              if (!(*it)->IsFrameWnd()) {
                continue;
              }
              if (g_replayAnimationScaling == 0) {
                (*it)->Root()->RecordEditTexts();
              }
              (*it)->SendMsg(nullptr, UM_DEFPUSH, 0, 0, 0, 0);
            }
            return false;
          }
        }
      }
    }
  }

  window->SendMsg(nullptr, UM_DEFPUSH, 0, 0, 0, 0);
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
  g_windowMgr.CloseWindow(WID_MACROREPORT);
}

// FUNCTION: fn_45513
void CloseWindow10005B() {
  g_windowMgr.CloseWindow(WID_MACROREPORT);
}

// OFFSET: 0x00A05280
// FUNCTION: fn_45510
void CloseWindow10006() {
  g_windowMgr.CloseWindow(WID_ENCHANTUI);
}

// OFFSET: 0x00AA76F0
// FUNCTION: fn_17239
void UIWindowMgr::DrawLoginOverlay() {
  const short quarter = static_cast<short>(g_renderer->viewport.width / 4);
  const short third = static_cast<short>(g_renderer->viewport.height / 3);
  char name[256];

  if (CTexture* grade = g_texMgr.Load("유저인터페이스\\\0_GameGrade.tga", 0, false, true, false)) {
    this->gradeImageWidth = grade->Width();
    this->gradeImageHeight = grade->Height();
  }

  int gradeLeft = this->gradeImageX;
  if (gradeLeft == 0) {
    gradeLeft = g_renderer->viewport.width - this->gradeImageWidth - 10;
    this->gradeImageX = gradeLeft;
  }

  int gradeTop = this->gradeImageY;
  if (gradeTop == 0) {
    this->gradeImageY = 10;
    gradeTop = 10;
  }

  if (g_mouse.buttons[0] == 1) {
    if (!this->inputWindow) {
      this->loginDragRegion = 0;
      if (g_mouse.x > gradeLeft && g_mouse.x < this->gradeImageWidth + gradeLeft && g_mouse.y > gradeTop && g_mouse.y < this->gradeImageHeight + gradeTop) {
        this->loginDragRegion = 3;
      }

      const float columns = static_cast<float>(quarter);
      const float mouseX = static_cast<float>(g_mouse.x);
      if (mouseX > columns * 3.3f + this->loginButtonPanelOffset.x && mouseX < columns * 3.9f + this->loginButtonPanelOffset.x &&
          static_cast<float>(g_mouse.y) > static_cast<float>(third) * 2.4f + this->loginButtonPanelOffset.y &&
          static_cast<float>(g_mouse.y) < static_cast<float>(third) * 2.8f + this->loginButtonPanelOffset.y) {
        this->loginDragRegion = 2;
      }

      if (static_cast<float>(g_mouse.x) > columns * 0.1f + this->loginGridX && static_cast<float>(g_mouse.x) < columns * 1.8f + this->loginGridX &&
          static_cast<float>(g_mouse.y) > static_cast<float>(third) * 0.3f + this->loginGridY &&
          static_cast<float>(g_mouse.y) < static_cast<float>(third) * 1.2f + this->loginGridY) {
        this->loginDragRegion = 1;
      }

      if (g_serviceType != 0 && static_cast<signed char>(g_startupSecondPhase) > 0 && static_cast<float>(g_mouse.x) > columns * 2.2f + this->loginSidePanelOffset.x &&
          static_cast<float>(g_mouse.x) < columns * 3.5f + this->loginSidePanelOffset.x && g_mouse.y > this->loginSidePanelOffset.y &&
          g_mouse.y < this->loginSidePanelOffset.y + third * 3) {
        this->loginDragRegion = 5;
      }

      if (static_cast<float>(g_mouse.y) > static_cast<float>(third) * 2.7f + this->loginBottomPanelOffset.x &&
          static_cast<float>(g_mouse.y) < static_cast<float>(third) * 3.0f + this->loginBottomPanelOffset.x) {
        this->loginDragRegion = 4;
      }
    }
  }

  if (g_mouse.buttons[0] == 2 && !this->inputWindow) {
    if (this->loginDragRegion == 1) {
      this->loginGridX += static_cast<short>(g_mouse.deltaX);
      this->loginGridY += static_cast<short>(g_mouse.deltaY);
    } else if (this->loginDragRegion == 2) {
      this->loginButtonPanelOffset.x += static_cast<short>(g_mouse.deltaX);
      this->loginButtonPanelOffset.y += static_cast<short>(g_mouse.deltaY);
    } else if (this->loginDragRegion == 3) {
      this->gradeImageX += g_mouse.deltaX;
      this->gradeImageY += g_mouse.deltaY;
    } else if (this->loginDragRegion == 6) {
      this->loginExtraPanelOffset.x += static_cast<short>(g_mouse.deltaX);
      this->loginExtraPanelOffset.y += static_cast<short>(g_mouse.deltaY);
    } else if (this->loginDragRegion == 4) {
      if (this->loginBottomPanelOffset.x + g_mouse.deltaY >= 0) {
        this->loginBottomPanelOffset.x += static_cast<short>(g_mouse.deltaY);
      }
    } else if (this->loginDragRegion == 5) {
      if (g_serviceType == 0) {
        if (this->loginSidePanelOffset.x + g_mouse.deltaX >= 0) {
          this->loginSidePanelOffset.x += static_cast<short>(g_mouse.deltaX);
        }
      } else {
        this->loginSidePanelOffset.x += static_cast<short>(g_mouse.deltaX);
      }
    }
  }

  RECT rect = {0, 0, g_renderer->viewport.width, g_renderer->viewport.height};
  const TextureRegion region = {0.0f, 0.0f, 1.0f, 1.0f};
  if (CTexture* login = g_texMgr.Load("유저인터페이스\\t_login.jpg", 0, false, true, false)) {
    g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, login, region);
  }

  const int step = quarter;
  char tape = 0;
  for (int x = 0; x < step * 4;) {
    ++tape;
    rect.left = x;
    x += step;
    rect.right = x;
    rect.bottom = static_cast<int>(this->loginBottomPanelOffset.x + static_cast<float>(third) * 3.0f);
    rect.top = static_cast<int>(this->loginBottomPanelOffset.x + static_cast<float>(third) * 2.5f);
    this->loginBottomPanelOffset.y = static_cast<short>(rect.top);
    std::sprintf(name, "유저인터페이스\\\0_테입%d.tga", tape);
    g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load(name, 0, false, true, false), region);
  }

  rect.left = this->gradeImageX;
  rect.top = this->gradeImageY;
  rect.right = this->gradeImageWidth + this->gradeImageX;
  rect.bottom = this->gradeImageHeight + this->gradeImageY;
  g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load("유저인터페이스\\\0_GameGrade.tga", 0, false, true, false), region);

  rect.left = static_cast<int>(this->loginButtonPanelOffset.x + static_cast<float>(step) * 3.1f);
  rect.right = step + rect.left;
  rect.top = static_cast<int>(this->loginButtonPanelOffset.y + static_cast<float>(third) * 2.4f);
  const float half = static_cast<float>(third) * 0.5f;
  rect.bottom = static_cast<int>(static_cast<float>(rect.top) + half);
  g_modeMgr.GetGameMode()->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load("유저인터페이스\\\0_중력성인.tga", 0, false, true, false), region);

  char row = -2;
  for (int column = this->loginGridX; column < this->loginGridX + step * 2; column += step) {
    ++row;
    int y = static_cast<int>(this->loginGridY + static_cast<float>(third) * 0.1f);
    if (this->loginGridY + (static_cast<float>(third) + static_cast<float>(third)) > static_cast<float>(y)) {
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
      } while (this->loginGridY + (static_cast<float>(third) + static_cast<float>(third)) > static_cast<float>(y));
    }
  }
}

// OFFSET: 0x00AA2DB0
// FUNCTION: fn_17220
bool UIWindowMgr::ProcessHotkeyAction(int action, int pressed) {
  switch (action) {
  case VK_MENU:
    if (this->modifierKey == 0) {
      if (UIWindow* window = this->LookupWindow(WID_ROMAPWND)) {
        window->SendMsg(nullptr, UM_COMMAND, ID_MAP, 0, 0, 0);
        return false;
      }
    }
    break;

  case VK_ESCAPE: {
    if (!pressed) {
      break;
    }

    if (this->autoMessageWnd != 0) {
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
          parent->SendMsg(this->hoverWindow, UM_DEFPUSH, 1, 0, 0, 0);
          return false;
        }
      }
    }

    if (this->HasModalWindow()) {
      this->modalResponse = 188;
      return false;
    }

    const int id = this->windowHistory.begin() != this->windowHistory.end() ? *this->windowHistory.begin() : -1;
    const int replaying = g_replayAnimationScaling;
    if (replaying != 0 && (static_cast<unsigned int>(id) > 10040 || this->policy[id].actions[3] == 0)) {
      return false;
    }

    if (g_windowMgr.CloseWindow(WID_ILLUSTWND)) {
      g_modeMgr.GetGameMode()->SendMsg(MM_SHOW_IMAGE, reinterpret_cast<int>(""), -1, 1, 0);
    } else if (UIWindow* window = g_windowMgr.LookupWindow(id)) {
      switch (id) {
      case WID_ITEMSHOPWND:
      case WID_ITEMPURCHASEWND:
      case WID_ITEMSELLWND:
      case WID_CHOOSESELLBUYWND:
      case WID_MERCHANTSHOPMAKEWND:
      case WID_MERCHANTITEMPURCHASEWND:
      case WID_ITEMSELLWND2:
      case WID_MERCHANTSHOPMAKEWND_MODE_1:
      case WID_MERCHANTITEMPURCHASEWND_MODE_1:
        if (replaying != 0) {
          return false;
        }
        window->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
        return false;
      case WID_BATTLEMSGOPTIONWND:
      case WID_SEARCH_STORE_INFO:
      case WID_NCASHWND:
      case WID_NAVIGATIONV4WND:
      case WID_STYLINGSHOPWND:
      case WID_REQUESTJOINPARTYWND:
        window->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
        return false;
      case WID_BALLOONOPENBTN:
      case WID_WAITDBSAVEWND:
        return false;
      }
    }

    if (id != -1) {
      if (this->CloseWindow(id) == true) {
        return false;
      }
    } else if (g_navigationManager.viewMode == 2) {
      if (g_windowMgr.Confirm(LookupMessage(MSI_NAVIGATION_HELP2), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        g_navigationManager.displayMode = 0;
        g_navigationManager.expandedMap = false;
        g_navigationManager.Reset(true);
        g_worldViewMgr.ClearMarks();
        g_worldViewMgr.ClearLevels();
        g_worldViewMgr.ResetSelection();
        g_windowMgr.CloseWindow(WID_NAVIGATIONRUIDEWND);
      }
      return false;
    }

    if (g_modalImmediate != 0 || g_modeMgr.running == 0) {
      return false;
    }

    if (this->LookupWindow(WID_SAYDIALOGWND) || this->LookupWindow(WID_SAYDIALOGWND_OVERLAY)) {
      return false;
    }

    if (g_modeMgr.GetGameMode()) {
      if (g_replayAnimationScaling != 0) {
        return false;
      }

      UIWindow* window = this->LookupWindow(WID_WAITDBSAVEWND);
      if (this->CloseWindow(WID_ESCOPTIONWND) || window) {
        return false;
      }

      this->EnsureWindow(WID_ESCOPTIONWND);
      return false;
    }

    if (this->LookupWindow(WID_NEWSELECTCHARWND) && g_session.m_loginModeFlag == 1) {
      if (g_windowMgr.Confirm(LookupMessage(MSI_ENST_ANGELUS2), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        g_modeMgr.gameMode->SendMsg(LMM_GOTO_LOGIN, 0, 0, 0, 0);
      }
      return false;
    }

    if (this->Confirm(LookupMessage(MSI_DO_YOU_REALLY_WANT_TO_QUIT), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
      GetRagConnection().Close();
      g_modeMgr.gameMode->SendMsg(MM_QUIT, 0, 0, 0, 0);
    }
    return false;
  }

  case VK_TAB:
    if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
      break;
    }

    if (this->modifierKey == 0) {
      if (UIWindow* window = this->LookupWindow(WID_ROMAPWND)) {
        window->SendMsg(nullptr, UM_COMMAND, ID_STATUS, 0, 0, 0);
        return false;
      }

      if (UIWindow* window = this->LookupWindow(WID_SKILLLISTWND)) {
        window->SendMsg(nullptr, UM_COMMAND, ID_PLUS, 0, 0, 0);
        return false;
      }
    }

    if (this->hoverWindow) {
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
    }

    this->RestoreInput();
    return false;

  case VK_RETURN:
    if (pressed) {
      this->ProcessEnterKey();
    }
    break;

  case VK_PRIOR:
    if (this->window_4F1D4) {
      this->window_4F1D4->BroadcastMsg3();
      return false;
    }
    break;

  case VK_NEXT:
    if (this->window_4F1D4) {
      this->window_4F1D4->BroadcastMsg4();
      return false;
    }
    break;

  case VK_UP:
    this->ConfirmActiveDialog();
    return false;

  case VK_DOWN:
    this->SubmitActiveDialog();
    return false;

  case VK_LEFT:
    this->CancelActiveDialog();
    return false;

  case VK_RIGHT:
    this->BroadcastMessage21();
    return false;

  case VK_SNAPSHOT:
  case VK_SCROLL:
    if (pressed) {
      g_modeMgr.gameMode->SendMsg(MM_SCREENSHOT, 0, 0, 0, 0);
      return false;
    }
    break;

  case 70:
    if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) && this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, 177, 0, 0, 0, 0);
      return false;
    }
    break;

  case VK_HOME:
    if (this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, 179, 0, 0, 0, 0);
      return false;
    }
    break;

  case VK_END:
    if (this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, UM_ENDPUSH, 0, 0, 0, 0);
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
      this->savedRectangles[id] = new SavedWindowRectangle(window->geometry.x, window->geometry.y, window->geometry.width, window->geometry.height);
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
      (id == WID_TIPBOXWND || id == WID_RENEWQUESTWND || id == WID_RENEWDETAILQUESTINFOWND)) {
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
  case WID_CHATWND:
    this->CloseWindow(WID_COMBOBOXWND);
    if (this->window_01C8) {
      this->Schedule(this->window_01C8);
      this->window_01C8 = nullptr;
      closed = true;
      this->chatWndVisible = 0;
    }
    break;
  case WID_BASICINFOWND:
    if (this->basicInfoWnd) {
      this->Schedule(this->basicInfoWnd);
      this->basicInfoWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMWND:
    if (this->itemWnd) {
      static_cast<UIItemWnd*>(this->itemWnd)->ReleaseSelection(0);
      this->Schedule(this->itemWnd);
      this->itemWnd = nullptr;
      closed = true;
      this->itemWndVisible = 0;
    }
    break;
  case WID_RUNESYSTEMWND:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->CloseWindow(WID_RUNESYSTEM_DECOMRESULTWND);
      this->Schedule(found);
      closed = true;
    }
    break;
  case WID_QUESTDISPLAY:
    if (this->window_0428) {
      this->Schedule(this->window_0428);
      this->window_0428 = nullptr;
      closed = true;
    }
    break;
  case WID_EQUIPWND:
    if (this->equipWnd) {
      this->Schedule(this->equipWnd);
      this->equipWnd = nullptr;
      closed = true;
      this->equipWndVisible = 0;
      g_savedWindowCloseValue = g_windowCloseValue;
    }
    break;
  case WID_NCASHWND:
    if (this->nCashWnd) {
      this->Schedule(this->nCashWnd);
      this->nCashWnd = nullptr;
      closed = true;
    }
    break;
  case WID_STATUSWND:
    if (this->statusWnd) {
      if (reinterpret_cast<std::uintptr_t>(this->statusWnd) > 8) {
        this->Schedule(this->statusWnd);
      }
      this->statusWnd = nullptr;
      closed = true;
      this->statusWndVisible = 0;
    }
    break;
  case WID_ITEMDROPCNTWND:
    if (this->window_01EC) {
      this->Schedule(this->window_01EC);
      this->window_01EC = nullptr;
      closed = true;
    }
    break;
  case WID_SAYDIALOGWND_OVERLAY:
    if (this->sayDialogOverlayWnd) {
      this->Schedule(this->sayDialogOverlayWnd);
      this->sayDialogOverlayWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SAYDIALOGWND:
    if (this->sayDialogWnd) {
      this->Schedule(this->sayDialogWnd);
      this->sayDialogWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CHATROOMWND:
    if (this->window_027C) {
      this->Schedule(this->window_027C);
      this->window_027C = nullptr;
      closed = true;
    }
    break;
  case WID_CHOOSEWARPWND:
    if (this->chooseWarpWnd) {
      this->Schedule(this->chooseWarpWnd);
      this->chooseWarpWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CHOOSEWND:
    if (this->choose3Wnd) {
      this->Schedule(this->choose3Wnd);
      this->choose3Wnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMCOMPOSITIONWND:
    if (this->itemCompositionWnd) {
      this->Schedule(this->itemCompositionWnd);
      this->itemCompositionWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMIDENTIFYWND:
    if (this->itemIdentifyWnd) {
      this->Schedule(this->itemIdentifyWnd);
      this->itemIdentifyWnd = nullptr;
      closed = true;
    }
    break;
  case WID_COMBOBOXWND:
    if (this->comboBoxWnd) {
      this->Schedule(this->comboBoxWnd);
      this->comboBoxWnd = nullptr;
      closed = true;
    }
    break;
  case WID_COMBOBOXIDWND:
    if (this->comboBoxIDWnd) {
      this->Schedule(this->comboBoxIDWnd);
      this->comboBoxIDWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MENUWND:
    if (this->menuWnd) {
      this->Schedule(this->menuWnd);
      this->menuWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CHATROOMCHANGEWND:
    if (this->window_0278) {
      this->Schedule(this->window_0278);
      this->window_0278 = nullptr;
      closed = true;
    }
    break;
  case WID_CHATROOMMAKEWND:
    if (this->window_0274) {
      this->Schedule(this->window_0274);
      this->window_0274 = nullptr;
      closed = true;
    }
    break;
  case WID_CHOOSESELLBUYWND:
    if (this->chooseSellBuyWnd) {
      g_modeMgr.gameMode->SendMsg(MM_NPC_TRADE_QUIT, 0, 0, 0, 0);
      this->Schedule(this->chooseSellBuyWnd);
      this->chooseSellBuyWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMSELLWND:
    if (this->itemSellWnd) {
      g_modeMgr.gameMode->SendMsg(MM_NPC_TRADE_QUIT, 0, 0, 0, 0);
      this->Schedule(this->itemSellWnd);
      this->itemSellWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMSELLWND2:
    if (this->itemSellWnd2) {
      this->Schedule(this->itemSellWnd2);
      this->itemSellWnd2 = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTITEMPURCHASEWND:
    if (this->merchantItemPurchaseWnd) {
      this->Schedule(this->merchantItemPurchaseWnd);
      this->merchantItemPurchaseWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMPURCHASEWND:
    if (this->itemPurchaseWnd) {
      g_modeMgr.gameMode->SendMsg(MM_NPC_TRADE_QUIT, 0, 0, 0, 0);
      this->Schedule(this->itemPurchaseWnd);
      this->itemPurchaseWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTITEMMYSHOPWND:
    if (this->merchantItemMyShopWnd) {
      this->Schedule(this->merchantItemMyShopWnd);
      this->merchantItemMyShopWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTITEMSHOPWND:
    if (this->merchantItemShopWnd) {
      this->Schedule(this->merchantItemShopWnd);
      this->merchantItemShopWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMSHOPWND:
    if (this->itemShopWnd) {
      this->Schedule(this->itemShopWnd);
      this->itemShopWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMSHOPWND2:
    if (this->itemShopWnd2) {
      this->Schedule(this->itemShopWnd2);
      this->itemShopWnd2 = nullptr;
      closed = true;
    }
    break;
  case WID_PARAITEMSHOPWND:
    if (this->paraItemShopWnd) {
      this->Schedule(this->paraItemShopWnd);
      this->paraItemShopWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PARAITEMPURCHASEWND:
    if (this->paraItemPurchaseWnd) {
      this->NotifyMode();
      this->Schedule(this->paraItemPurchaseWnd);
      this->paraItemPurchaseWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PARARESULTWND:
    if (this->window_0270) {
      this->NotifyMode();
      this->Schedule(this->window_0270);
      this->window_0270 = nullptr;
      closed = true;
    }
    break;
  case WID_BARTERMARKETWND:
    if (this->window_4F260) {
      this->Schedule(this->window_4F260);
      this->window_4F260 = nullptr;
      closed = true;
    }
    break;
  case WID_BARTERITEMPURCHASEWND:
    if (this->window_4F264) {
      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->SendMsg(MM_NPC_BARTER_MARKET_CLOSE, 0, 0, 0, 0);
      }
      this->Schedule(this->window_4F264);
      this->window_4F264 = nullptr;
      closed = true;
    }
    break;
  case WID_EXPANDEDBARTERMARKETWND:
    if (this->window_4F268) {
      this->Schedule(this->window_4F268);
      closed = true;
      this->window_4F268 = nullptr;
      if (this->window_4F26C) {
        this->CloseWindow(WID_EXPANDEDBARTERITEMPURCHASEWND);
      }
    }
    break;
  case WID_EXPANDEDBARTERITEMPURCHASEWND:
    if (this->window_4F26C) {
      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->SendMsg(MM_NPC_EXPANDED_BARTER_MARKET_CLOSE, 0, 0, 0, 0);
      }
      this->Schedule(this->window_4F26C);
      closed = true;
      this->window_4F26C = nullptr;
      if (this->window_4F268) {
        this->CloseWindow(WID_EXPANDEDBARTERMARKETWND);
      }
    }
    break;
  case WID_NOTIFYJOBLEVELUPWND:
    if (this->notifyJobLevelUpWnd) {
      this->Schedule(this->notifyJobLevelUpWnd);
      this->notifyJobLevelUpWnd = nullptr;
      closed = true;
    }
    break;
  case WID_NOTIFYLEVELUPWND:
    if (this->notifyLevelUpWnd) {
      this->Schedule(this->notifyLevelUpWnd);
      this->notifyLevelUpWnd = nullptr;
      closed = true;
    }
    break;
  case WID_NOTIFYQUESTWND:
    if (this->notifyQuestWnd) {
      this->Schedule(this->notifyQuestWnd);
      this->notifyQuestWnd = nullptr;
      closed = true;
    }
    break;
  case WID_NCASH_CALLWND:
    if (this->nCashCallWnd) {
      this->Schedule(this->nCashCallWnd);
      this->nCashCallWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CARDITEMILLUSTWND:
    if (this->cardItemIllustWnd) {
      this->Schedule(this->cardItemIllustWnd);
      this->cardItemIllustWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MONSTERDISPLAYWND:
    if (this->monsterDisplayWnd) {
      this->Schedule(this->monsterDisplayWnd);
      this->monsterDisplayWnd = nullptr;
      closed = true;
    }
    break;
  case WID_RESTARTWND:
    if (this->restartWnd) {
      this->Schedule(this->restartWnd);
      this->restartWnd = nullptr;
      closed = true;
    }
    break;
  case WID_QUITWND:
    if (this->quitWnd) {
      this->Schedule(this->quitWnd);
      this->quitWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SKILLDESCRIBEWND:
    if (this->window_0230) {
      this->Schedule(this->window_0230);
      this->window_0230 = nullptr;
      closed = true;
      this->skillDescriptionVisible = 0;
    }
    break;
  case WID_ITEMPARAMCHANGEDISPLAYWND:
    if (this->itemParamChangeDisplayWnd) {
      this->Schedule(this->itemParamChangeDisplayWnd);
      this->itemParamChangeDisplayWnd = nullptr;
      closed = true;
    }
    break;
  case WID_COMBINEDCARDITEMCOLLECTIONWND:
    if (this->combinedCardItemCollectionWnd) {
      this->Schedule(this->combinedCardItemCollectionWnd);
      this->combinedCardItemCollectionWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ITEMCOLLECTIONWND:
    if (this->itemCollectionWnd) {
      this->Schedule(this->itemCollectionWnd);
      closed = true;
      this->itemCollectionWnd = nullptr;
      if (this->itemCollectionComparisonWnd) {
        this->CloseWindow(WID_ITEMCOLLECTION_COMPARISONWND);
      }
    }
    break;
  case WID_ITEMCOLLECTION_COMPARISONWND:
    if (this->itemCollectionComparisonWnd) {
      this->Schedule(this->itemCollectionComparisonWnd);
      closed = true;
      this->itemCollectionComparisonWnd = nullptr;
      if (this->itemCollectionWnd) {
        this->CloseWindow(WID_ITEMCOLLECTIONWND);
      }
    }
    break;
  case WID_COMBINEDCARDITEMCOLLECTION_COMPARISONWND:
    if (this->combinedCardItemCollectionComparisonWnd) {
      this->Schedule(this->combinedCardItemCollectionComparisonWnd);
      this->combinedCardItemCollectionComparisonWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MINIMAPZOOMWND:
    if (this->minimapZoomWnd) {
      this->Schedule(this->minimapZoomWnd);
      this->minimapZoomWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SKILLLISTWND:
    if (g_windowMgr.LookupWindow(WID_SKILLDELAYINFO)) {
      this->CloseWindow(WID_SKILLDELAYINFO);
    }
    this->CloseWindow(WID_SKILLDESCRIBEWND);
    if (this->window_02C4) {
      this->Schedule(this->window_02C4);
      this->window_02C4 = nullptr;
      closed = true;
      this->skillListWndVisible = 0;
    }
    break;
  case WID_ITEMSTOREWND:
    if (this->window_0288) {
      this->Schedule(this->window_0288);
      this->window_0288 = nullptr;
      closed = true;
    }
    break;
  case WID_MESSENGERWND:
    if (this->window_02C8) {
      this->window_02C8->StoreInfo();
    }
    closed = this->CloseWindow(WID_MESSENGERGROUPWND);
    g_windowMgr.DismissTransient();
    break;
  case WID_MESSENGERGROUPWND:
    if (this->window_02C8) {
      this->Schedule(this->window_02C8);
      this->window_02C8 = nullptr;
      this->messengerGroupWndVisible = 0;
      g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
      g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND);
      g_windowMgr.DismissTransient();
      closed = true;
    }
    break;
  case WID_MERCHANTSHOPMAKEWND:
    this->CloseWindow(WID_MERCHANTMIRRORITEMWND);
    if (this->merchantShopMakeWnd) {
      this->Schedule(this->merchantShopMakeWnd);
      this->merchantShopMakeWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTMIRRORITEMWND:
    if (this->merchantMirrorItemWnd) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->merchantMirrorItemWnd));
      this->merchantMirrorItemWnd = 0;
      closed = true;
    }
    break;
  case WID_MERCHANTITEMWND:
    if (this->merchantItemWnd) {
      this->Schedule(this->merchantItemWnd);
      this->merchantItemWnd = nullptr;
      closed = true;
      this->merchantItemWndVisible = 0;
    }
    break;
  case WID_CANDIDATEWND:
    if (this->window_03A4) {
      this->Schedule(this->window_03A4);
      this->window_03A4 = nullptr;
      closed = true;
    }
    break;
  case WID_COMPOSITIONWND:
    if (this->compositionWnd) {
      this->Schedule(this->compositionWnd);
      this->compositionWnd = nullptr;
      closed = true;
    }
    break;
  case WID_NOTIFYITEMOBTAINWND:
    if (this->notifyItemObtainWnd) {
      this->Schedule(this->notifyItemObtainWnd);
      this->notifyItemObtainWnd = nullptr;
      closed = true;
    }
    break;
  case WID_NPCEDITDIALOGWND:
    if (this->npcEditDialogWnd) {
      this->Schedule(this->npcEditDialogWnd);
      this->npcEditDialogWnd = nullptr;
      closed = true;
    }
    break;
  case WID_NPCTEXTEDITDIALOGWND:
    if (this->npcTextEditDialogWnd) {
      this->Schedule(this->npcTextEditDialogWnd);
      this->npcTextEditDialogWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SKILLNAMECHANGEWND:
    if (this->skillNameChangeWnd) {
      this->Schedule(this->skillNameChangeWnd);
      this->skillNameChangeWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDHELPERWND:
    if (this->guildHelperWnd) {
      this->guildHelperWnd->StoreInfo();
      this->Schedule(this->guildHelperWnd);
      this->guildHelperWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDTIPWND:
    if (this->guildTipWnd) {
      this->Schedule(this->guildTipWnd);
      this->guildTipWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CREATEGUILDWND_MODE_0:
    if (this->createGuildWnd) {
      this->Schedule(this->createGuildWnd);
      this->createGuildWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CREATEGUILDWND_MODE_1:
    if (this->createGuildAlternateWnd) {
      this->Schedule(this->createGuildAlternateWnd);
      this->createGuildAlternateWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDWND:
  {
    const bool closed60 = g_windowMgr.CloseWindow(WID_GUILDINFOMANAGEWND);
    const bool closed61 = g_windowMgr.CloseWindow(WID_GUILDMEMBERMANAGEWND);
    const bool closed62 = g_windowMgr.CloseWindow(WID_GUILDPOSITIONMANAGEWND);
    const bool closed63 = g_windowMgr.CloseWindow(WID_GUILDSKILLWND);
    const bool closed64 = g_windowMgr.CloseWindow(WID_GUILDBANISHEDMEMBERWND);
    const bool closed65 = g_windowMgr.CloseWindow(WID_GUILDNOTICEWND);
    const bool closed66 = g_windowMgr.CloseWindow(WID_GUILDTOTALINFOWND);
    closed = closed60 || closed61 || closed62 || closed63 || closed64 || closed65 || closed66;
    if (g_windowMgr.comboBoxWnd) {
      g_windowMgr.CloseWindow(WID_COMBOBOXWND);
    }
    break;
  }
  case WID_GUILDMEMBERMANAGEWND:
    if (this->window_0324) {
      this->window_0324->StoreInfo();
      this->Schedule(this->window_0324);
      this->window_0324 = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDPOSITIONMANAGEWND:
    if (this->guildPositionManageWnd) {
      this->guildPositionManageWnd->StoreInfo();
      this->Schedule(this->guildPositionManageWnd);
      this->guildPositionManageWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDSKILLWND:
    if (this->window_032C) {
      this->window_032C->StoreInfo();
      this->Schedule(this->window_032C);
      this->window_032C = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDBANISHEDMEMBERWND:
    if (this->guildBanishedMemberWnd) {
      this->guildBanishedMemberWnd->StoreInfo();
      this->Schedule(this->guildBanishedMemberWnd);
      this->guildBanishedMemberWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDNOTICEWND:
    if (this->guildNoticeWnd) {
      this->guildNoticeWnd->StoreInfo();
      this->Schedule(this->guildNoticeWnd);
      this->guildNoticeWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDTOTALINFOWND:
    if (this->guildTotalInfoWnd) {
      this->guildTotalInfoWnd->StoreInfo();
      this->Schedule(this->guildTotalInfoWnd);
      this->guildTotalInfoWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDINFOMANAGEWND:
    if (this->window_0320) {
      this->window_0320->StoreInfo();
      this->Schedule(this->window_0320);
      this->window_0320 = nullptr;
      closed = true;
    }
    break;
  case WID_ILLUSTWND:
    if (this->illustWnd) {
      this->Schedule(this->illustWnd);
      this->illustWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MULTIILLUSTWND_SLOT_0: case WID_MULTIILLUSTWND_SLOT_1: case WID_MULTIILLUSTWND_SLOT_2: case WID_MULTIILLUSTWND_SLOT_3: case WID_MULTIILLUSTWND_SLOT_4: case WID_MULTIILLUSTWND_SLOT_5: case WID_MULTIILLUSTWND_SLOT_6: case WID_MULTIILLUSTWND_SLOT_7: case WID_MULTIILLUSTWND_SLOT_8: case WID_MULTIILLUSTWND_SLOT_9:
    if (this->multiIllustWnds[id - 216]) {
      this->Schedule(this->multiIllustWnds[id - 216]);
      this->multiIllustWnds[id - 216] = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDBANREASONDESCWND:
    if (this->guildBanReasonDescWnd) {
      this->Schedule(this->guildBanReasonDescWnd);
      this->guildBanReasonDescWnd = nullptr;
      closed = true;
    }
    break;
  case WID_GUILDLEAVEREASONDESCWND:
    if (this->guildLeaveReasonDescWnd) {
      this->Schedule(this->guildLeaveReasonDescWnd);
      this->guildLeaveReasonDescWnd = nullptr;
      closed = true;
    }
    break;
  case WID_FRIENDOPTIONWND:
    if (this->friendOptionWnd) {
      this->Schedule(this->friendOptionWnd);
      this->friendOptionWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PARTYSETTINGWND_PARTYCOUNT:
    if (this->partySettingWnd) {
      this->Schedule(this->partySettingWnd);
      this->partySettingWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PARTYINVITATIONTOWND:
    if (this->partyInvitationToWnd) {
      this->Schedule(this->partyInvitationToWnd);
      this->partyInvitationToWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PARTYSETTINGWND_HEIGHT_220:
    this->CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
    closed = true;
    break;
  case WID_TIPOFTHEDAYWND:
    if (this->tipOfTheDayWnd) {
      this->Schedule(this->tipOfTheDayWnd);
      this->tipOfTheDayWnd = nullptr;
      closed = true;
    }
    break;
  case WID_JOINGUILDACCEPTWND:
    if (this->joinGuildAcceptWnd) {
      this->Schedule(this->joinGuildAcceptWnd);
      this->joinGuildAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ALLYGUILDACCEPTWND:
    if (this->allyGuildAcceptWnd) {
      this->Schedule(this->allyGuildAcceptWnd);
      this->allyGuildAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_JOINPARTYACCEPTWND:
    if (this->joinPartyAcceptWnd) {
      this->Schedule(this->joinPartyAcceptWnd);
      this->joinPartyAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_COUPLEACCEPTWND:
    if (this->coupleAcceptWnd) {
      this->Schedule(this->coupleAcceptWnd);
      this->coupleAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_STARPLACEACCEPTWND:
    if (this->starPlaceAcceptWnd) {
      this->Schedule(this->starPlaceAcceptWnd);
      this->starPlaceAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MIXACCEPTWND:
    if (this->mixAcceptWnd) {
      this->Schedule(this->mixAcceptWnd);
      this->mixAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_YOURITEMWND:
    if (this->yourItemWnd) {
      this->Schedule(this->yourItemWnd);
      this->yourItemWnd = nullptr;
      closed = true;
    }
    break;
  case WID_BABYACCEPTWND:
    if (this->babyAcceptWnd) {
      this->Schedule(this->babyAcceptWnd);
      this->babyAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_BABYACCEPTWND2:
    if (this->babyAcceptWnd2) {
      this->Schedule(this->babyAcceptWnd2);
      this->babyAcceptWnd2 = nullptr;
      closed = true;
    }
    break;
  case WID_EXCHANGEACCEPTWND:
    if (this->exchangeAcceptWnd) {
      this->Schedule(this->exchangeAcceptWnd);
      this->exchangeAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PASSWORDWND:
    if (this->passwordWnd) {
      this->Schedule(this->passwordWnd);
      this->passwordWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MAKETARGETLISTWND:
    if (this->makeTargetListWnd) {
      this->Schedule(this->makeTargetListWnd);
      this->makeTargetListWnd = nullptr;
    }
    break;
  case WID_ITEMREPAIRWND:
    if (this->itemRepairWnd) {
      this->Schedule(this->itemRepairWnd);
      this->itemRepairWnd = nullptr;
    }
    break;
  case WID_WEAPONREFINEWND:
    if (this->weaponRefineWnd) {
      this->Schedule(this->weaponRefineWnd);
      this->weaponRefineWnd = nullptr;
    }
    break;
  case WID_MAKETARGETPROCESSWND:
    if (this->makeTargetProcessWnd) {
      this->Schedule(this->makeTargetProcessWnd);
      this->makeTargetProcessWnd = nullptr;
    }
    break;
  case WID_MAKETARGETRESULTWND:
    if (this->makeTargetResultWnd) {
      this->Schedule(this->makeTargetResultWnd);
      this->makeTargetResultWnd = nullptr;
    }
    break;
  case WID_KEYSTROKEWND:
    if (this->window_03AC) {
      this->Schedule(this->window_03AC);
      this->window_03AC = nullptr;
    }
    break;
  case WID_EMOTIONWND:
    if (this->emotionWnd) {
      this->Schedule(this->emotionWnd);
      this->emotionWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CASHEMOTIONLISTWND:
    if (this->cashEmotionListWnd) {
      this->Schedule(this->cashEmotionListWnd);
      this->cashEmotionListWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PETINFOWND:
    if (this->window_038C) {
      this->Schedule(this->window_038C);
      this->window_038C = nullptr;
      closed = true;
    }
    break;
  case WID_PETEGGLISTWND:
    if (this->petEggListWnd) {
      this->Schedule(this->petEggListWnd);
      this->petEggListWnd = nullptr;
      closed = true;
      g_modeMgr.gameMode->SendMsg(MM_PETEGG_SELECT_FROM_LIST, -1, 0, 0, 0);
    }
    break;
  case WID_PETTAMINGDECEIVEWND:
    if (this->petTamingDeceiveWnd) {
      this->Schedule(this->petTamingDeceiveWnd);
      this->petTamingDeceiveWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MAKINGARROWLISTWND:
    if (this->makingArrowListWnd) {
      this->Schedule(this->makingArrowListWnd);
      this->makingArrowListWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SELECTCARTWND:
    if (this->selectCartWnd) {
      this->Schedule(this->selectCartWnd);
      this->selectCartWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SPELLLISTWND:
    if (this->spellListWnd) {
      this->Schedule(this->spellListWnd);
      this->spellListWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PROHIBITLISTWND:
    if (this->window_0388) {
      this->Schedule(this->window_0388);
      this->window_0388 = nullptr;
      closed = true;
    }
    break;
  case WID_BOOKWND:
    if (this->bookWnd) {
      this->Schedule(this->bookWnd);
      this->bookWnd = nullptr;
      closed = true;
    }
    break;
  case WID_JOINFRIENDACCEPTWND:
    if (this->joinFriendAcceptWnd) {
      this->Schedule(this->joinFriendAcceptWnd);
      this->joinFriendAcceptWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SECONDPASSWORDINPUTWND:
    if (this->secondPasswordInputWnd) {
      this->Schedule(this->secondPasswordInputWnd);
      this->secondPasswordInputWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SECONDPASSWORDEDITWND:
    if (this->secondPasswordEditWnd) {
      this->Schedule(this->secondPasswordEditWnd);
      this->secondPasswordEditWnd = nullptr;
      closed = true;
    }
    break;
  case WID_HOMUNINFOWND:
    if (this->window_03C0) {
      this->Schedule(this->window_03C0);
      this->window_03C0 = nullptr;
      closed = true;
    }
    break;
  case WID_SKILLLISTWND_SKILLTAB_0:
    if (this->window_03C4) {
      this->Schedule(this->window_03C4);
      this->window_03C4 = nullptr;
      closed = true;
      this->homunculusSkillWndVisible = 0;
    }
    break;
  case WID_MERINFOWND:
    if (this->window_03C8) {
      this->Schedule(this->window_03C8);
      this->window_03C8 = nullptr;
      closed = true;
    }
    break;
  case WID_REPLAYCONTROLWND:
    this->Schedule(this->window_4F168);
    this->window_4F168 = nullptr;
    closed = true;
    break;
  case WID_REPLAYRECCONTROLWND:
    if (this->window_4F164) {
      this->Schedule(this->window_4F164);
      this->window_4F164 = nullptr;
      closed = true;
    }
    break;
  case WID_NAVIGATIONV4WND:
    if (this->window_4F174) {
      this->Schedule(this->window_4F174);
      this->window_4F174 = nullptr;
      closed = true;
    }
    break;
  case WID_NAVIGATIONHELPWND:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->Schedule(found);
      closed = true;
    }
    break;
  case WID_SKILLLISTWND_SKILLTAB_1:
    if (this->window_03CC) {
      this->Schedule(this->window_03CC);
      this->window_03CC = nullptr;
      closed = true;
      this->mercenarySkillWndVisible = 0;
    }
    break;
  case WID_AUTOMESSAGEWND:
    if (this->autoMessageWnd) {
      this->Schedule(this->autoMessageWnd);
      this->autoMessageWnd = nullptr;
      closed = true;
    }
    break;
  case WID_CHANGENAMEWND:
    if (this->window_03DC) {
      this->Schedule(this->window_03DC);
      this->window_03DC = nullptr;
      closed = true;
    }
    break;
  case WID_REBIRTHWND:
    if (this->rebirthWnd) {
      this->Schedule(this->rebirthWnd);
      this->rebirthWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SUBCHATHISWND_UNKNOWN_03E4:
    if (this->subChatHistoryWnd) {
      reinterpret_cast<UIWindow*>(this->subChatHistoryWnd)->StoreInfo();
      this->Schedule(reinterpret_cast<UIWindow*>(this->subChatHistoryWnd));
      this->subChatHistoryWnd = 0;
      closed = true;
    }
    break;
  case WID_SUBCHATHISWND_UNKNOWN_03E8:
    if (this->secondSubChatHistoryWnd) {
      reinterpret_cast<UIWindow*>(this->secondSubChatHistoryWnd)->StoreInfo();
      this->Schedule(reinterpret_cast<UIWindow*>(this->secondSubChatHistoryWnd));
      this->secondSubChatHistoryWnd = 0;
      closed = true;
    }
    break;
  case WID_BATTLEMSGOPTIONWND:
    if (this->battleMsgOptionWnd) {
      this->Schedule(this->battleMsgOptionWnd);
      this->battleMsgOptionWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SUBCHATMINIWND:
    if (this->subChatMiniWnd) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->subChatMiniWnd));
      this->subChatMiniWnd = 0;
      closed = true;
    }
    break;
  case WID_MEMORIALDUNWND:
    if (this->memorialDunWnd) {
      static_cast<UIMemorialDunWnd*>(this->memorialDunWnd)->ResetDungeonInfo();
      this->Schedule(this->memorialDunWnd);
      this->memorialDunWnd = nullptr;
      closed = true;
      g_session.memorialWindowOpen = 0;
    }
    break;
  case WID_EQUIPWND_VIEWOTHER_1:
    if (this->otherEquipWnd) {
      this->Schedule(this->otherEquipWnd);
      this->otherEquipWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ROMAPWND:
    if (this->modal) {
      this->Schedule(this->modal);
      this->modal = nullptr;
      closed = true;
      if (this->window_4F174) {
        g_navigationManager.hideWindow = false;
        static_cast<UINavigationV4Wnd*>(this->window_4F174)->RefreshLayout();
      }
    }
    break;
  case WID_ITEMSTORESUBWND_TAB_0: case WID_ITEMSTORESUBWND_TAB_1: case WID_ITEMSTORESUBWND_TAB_2: case WID_ITEMSTORESUBWND_TAB_3: case WID_ITEMSTORESUBWND_TAB_4: case WID_ITEMSTORESUBWND_TAB_5: case WID_ITEMSTORESUBWND_TAB_6:
    if (this->itemStoreTabWnds[id - 146]) {
      this->Schedule(this->itemStoreTabWnds[id - 146]);
      this->itemStoreTabWnds[id - 146] = nullptr;
      closed = true;
      if (this->window_0288) {
        static_cast<UIItemStoreWnd*>(this->window_0288)->SetTabChecked(id - 146, false);
      }
    }
    break;
  case WID_ITEMSTORESUBWND_TAB_7:
    if (this->itemStoreLastTabWnd) {
      this->Schedule(this->itemStoreLastTabWnd);
      this->itemStoreLastTabWnd = nullptr;
      closed = true;
      if (this->window_0288) {
        static_cast<UIItemStoreWnd*>(this->window_0288)->SetTabChecked(7, false);
      }
    }
    break;
  case WID_ITEMSTOREFINDWND:
    if (this->itemStoreFindWnd) {
      this->Schedule(this->itemStoreFindWnd);
      this->itemStoreFindWnd = nullptr;
      closed = true;
    }
    break;
  case WID_PARTYBOOKINGHELPWND:
    if (this->partyBookingHelpWnd) {
      this->Schedule(this->partyBookingHelpWnd);
      this->partyBookingHelpWnd = nullptr;
      closed = true;
    }
    break;
  case WID_JOBLISTWND:
    if (this->jobListWnd) {
      this->Schedule(this->jobListWnd);
      this->jobListWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SEEKPARTYWND:
    if (this->seekPartyWnd) {
      this->Schedule(this->seekPartyWnd);
      this->seekPartyWnd = nullptr;
      closed = true;
      g_session.m_partyBooking.unknown_0000 = 0;
    }
    break;
  case WID_SEEKPARTYMBWND:
    if (this->seekPartyMessageWnd) {
      this->Schedule(this->seekPartyMessageWnd);
      this->seekPartyMessageWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SEEKPARTYLISTWND:
    if (this->seekPartyListWnd) {
      this->Schedule(this->seekPartyListWnd);
      this->seekPartyListWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ESCOPTIONWND:
    if (this->escOptionWnd) {
      this->Schedule(this->escOptionWnd);
      this->escOptionWnd = nullptr;
      closed = true;
    }
    break;
  case WID_HOTKEYWND: case WID_RELOADLUASCRIPTWND: case WID_QUIKSLOTWND: case WID_BATTLEFIELD_DESCRIBEWND: case WID_BATTLEFIELD_STATUSWND: case WID_BATTLEFIELD_HELPWND: case 171: case WID_SEARCH_STORE_INFO: case WID_SHOWINFO_TESTLAYER: case WID_EXMACRO_SETUPWND:
    if (this->LookupWindow(id)) {
      if (UIWindow* found = this->LookupWindow(id)) {
        switch (id) {
        case WID_HOTKEYWND:
          this->Schedule(this->hotkeyWnd);
          this->hotkeyWnd = nullptr;
          break;
        case WID_JOINTO_BATTLEFIELDWND:
        case WID_RELOADLUASCRIPTWND:
        case WID_QUIKSLOTWND:
        case WID_BATTLEFIELD_DESCRIBEWND:
        case WID_BATTLEFIELD_STATUSWND:
        case WID_BATTLEFIELD_HELPWND:
        case 171:
        case WID_SHOWINFO_TESTLAYER:
        case WID_EXMACRO_SETUPWND:
          this->Schedule(found);
          break;
        case WID_SEARCH_STORE_INFO:
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
  case WID_MERCHANTSHOPMAKEWND_MODE_1:
    this->CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_1);
    if (this->buyingShopMakeWnd) {
      this->Schedule(this->buyingShopMakeWnd);
      this->buyingShopMakeWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTMIRRORITEMWND_MODE_1:
    if (this->buyingShopMirrorItemWnd) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->buyingShopMirrorItemWnd));
      this->buyingShopMirrorItemWnd = 0;
      closed = true;
    }
    break;
  case WID_MERCHANTITEMMYSHOPWND_MODE_1:
    if (this->buyingShopMyItemsWnd) {
      this->Schedule(this->buyingShopMyItemsWnd);
      this->buyingShopMyItemsWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTITEMSHOPWND_MODE_1:
    if (this->buyingShopItemsWnd) {
      this->Schedule(this->buyingShopItemsWnd);
      this->buyingShopItemsWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTITEMPURCHASEWND_MODE_1:
    if (this->buyingShopPurchaseWnd) {
      this->Schedule(this->buyingShopPurchaseWnd);
      this->buyingShopPurchaseWnd = nullptr;
      closed = true;
    }
    break;
  case WID_MERCHANTMIRRORITEMWND_MODE_2:
    if (this->purchaseMirrorItemWnd) {
      this->Schedule(reinterpret_cast<UIWindow*>(this->purchaseMirrorItemWnd));
      this->purchaseMirrorItemWnd = 0;
      closed = true;
    }
    break;
  case WID_COLLECTIONSYSTEMWND:
    if (this->window_4F18C) {
      this->Schedule(this->window_4F18C);
      this->window_4F18C = nullptr;
      closed = true;
    }
    break;
  case WID_CLANINFOMANAGEWND:
    if (this->window_0444) {
      this->window_0444->StoreInfo();
      this->Schedule(this->window_0444);
      this->window_0444 = nullptr;
      closed = true;
    }
    break;
  case WID_BANK_NEWWND:
    if (this->window_0448) {
      this->Schedule(this->window_0448);
      this->window_0448 = nullptr;
      closed = true;
    }
    break;
  case WID_GUILD_STORAGE_LOG:
    if (this->window_044C) {
      this->Schedule(this->window_044C);
      this->window_044C = nullptr;
      closed = true;
    }
    break;
  case WID_OPENMAILBOXWND:
    if (this->openMailBoxWnd) {
      this->Schedule(this->openMailBoxWnd);
      this->openMailBoxWnd = nullptr;
      closed = true;
    }
    break;
  case WID_RODEXINBOXWND:
    if (this->rodexInboxWnd) {
      std::int16_t packet = 0x9E9;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
      this->Schedule(this->rodexInboxWnd);
      this->rodexInboxWnd = nullptr;
      closed = true;
      if (this->openMailBoxWnd) {
        this->CloseWindow(WID_OPENMAILBOXWND);
      }
    }
    break;
  case WID_RODEXCOMPOSEWND:
    if (this->rodexComposeWnd) {
      std::int16_t packet = 0xA03;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
      g_session.RestoreItems5C68();
      g_session.ClearItemQuantities();
      this->Schedule(this->rodexComposeWnd);
      this->rodexComposeWnd = nullptr;
      closed = true;
    }
    break;
  case WID_RODEXCONTENTSWND:
    if (this->rodexContentsWnd) {
      g_session.ClearItemQuantities();
      this->Schedule(this->rodexContentsWnd);
      this->rodexContentsWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SECONDCOSTUMEWND:
    if (this->window_4F170) {
      this->Schedule(this->window_4F170);
      this->window_4F170 = nullptr;
      closed = true;
    }
    break;
  case WID_OPENROULLETTEWND:
    if (this->openRoulletteWnd) {
      this->Schedule(this->openRoulletteWnd);
      this->openRoulletteWnd = nullptr;
      closed = true;
    }
    break;
  case WID_ROULLETTEWND:
    if (this->roulletteWnd) {
      std::int16_t packet = 0xA1D;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
      g_rouletteMgr->Reset();
      this->Schedule(this->roulletteWnd);
      this->roulletteWnd = nullptr;
      closed = true;
    }
    break;
  case WID_SHORTCUTWND:
    if (this->window_01E8) {
      this->Schedule(this->window_01E8);
      this->window_01E8 = nullptr;
      closed = true;
    }
    break;
  case WID_MINIMAPWND:
    if (this->window_4F1CC) {
      this->Schedule(this->window_4F1CC);
      this->window_4F1CC = nullptr;
      closed = true;
    }
    break;
  case WID_EDITCTRLWND:
    if (this->window_4F1D0) {
      this->Schedule(this->window_4F1D0);
      this->window_4F1D0 = nullptr;
      closed = true;
    }
    break;
  case WID_ROUNDBOXTOOLTIPWND:
    if (this->window_4F1E4) {
      this->Schedule(this->window_4F1E4);
      this->window_4F1E4 = nullptr;
      closed = true;
      this->visible_276 = 0;
    }
    break;
  case WID_CAPTCHAREGISTERWND:
    if (this->window_4F1E8) {
      this->Schedule(this->window_4F1E8);
      this->window_4F1E8 = nullptr;
      closed = true;
    }
    break;
  case WID_MACRODETECTORWND:
    if (this->window_4F1EC) {
      this->Schedule(this->window_4F1EC);
      this->window_4F1EC = nullptr;
      closed = true;
      this->window_4F1FC = nullptr;
    }
    break;
  case WID_MACROREPORTERWND:
    if (this->window_4F1F0) {
      this->Schedule(this->window_4F1F0);
      this->window_4F1F0 = nullptr;
      closed = true;
      this->macroReporterVisible = false;
    }
    break;
  case WID_MACROUSERINFOWND:
    if (this->window_4F1F4) {
      this->Schedule(this->window_4F1F4);
      this->window_4F1F4 = nullptr;
      closed = true;
    }
    break;
  case WID_CAPTCHAPREVIEWWND:
    if (this->window_4F1F8) {
      this->Schedule(this->window_4F1F8);
      this->window_4F1F8 = nullptr;
      closed = true;
    }
    break;
  case WID_LOGINOTPWND:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->Schedule(found);
      this->window_01D0 = nullptr;
      closed = true;
    }
    break;
  case WID_CASHSHOP_WND:
    if (this->window_4F254) {
      this->Schedule(this->window_4F254);
      this->window_4F254 = nullptr;
      closed = true;
      std::int16_t packet = 0x84A;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet), &packet);
    }
    break;
  case WID_TIPBOXWND:
    if (this->window_0468) {
      this->Schedule(this->window_0468);
      this->window_0468 = nullptr;
      closed = true;
    }
    break;
  case WID_RENEWQUESTWND:
    if (UIWindow* found = this->LookupWindow(id)) {
      this->Schedule(found);
      this->questWndVisible = 0;
      closed = true;
    }
    break;
  case WID_ADVENPARTYBOARDWND:
    if (this->window_4F274) {
      this->window_4F274->StoreInfo();
      this->Schedule(this->window_4F274);
      this->window_4F274 = nullptr;
      closed = true;
    }
    break;
  case WID_REGISTERPARTYWND:
    if (this->window_4F270) {
      this->window_4F270->StoreInfo();
      this->Schedule(this->window_4F270);
      this->window_4F270 = nullptr;
      closed = true;
    }
    break;
  case WID_REQUESTJOINPARTYWND:
    if (this->window_4F278) {
      this->Schedule(this->window_4F278);
      this->window_4F278 = nullptr;
      closed = true;
    }
    break;
  case WID_APPLYFORPARTYWND:
    if (this->window_4F27C) {
      this->window_4F27C->StoreInfo();
      this->Schedule(this->window_4F27C);
      this->window_4F27C = nullptr;
      closed = true;
    }
    break;
  case WID_MACROBLACKLISTCHECKWND:
    if (this->window_4F288) {
      this->Schedule(this->window_4F288);
      this->window_4F288 = nullptr;
    }
    break;
  case WID_WAITDBSAVEWND:
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
  if (window && (this->modal == nullptr || this->window_4F174 == window) && !this->LookupWindow(WID_NEWMAKECHARWND) && !this->LookupWindow(WID_MAKECHARWND) &&
      !this->LookupWindow(WID_SECONDPASSWORDINPUTWND) && !this->LookupWindow(WID_SECONDPASSWORDEDITWND)) {
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
      this->window_01E8->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, hotkey % 9, hotkey / 9, 0, 0);
    }
    return true;
  }

  if (this->modalResponse == 0) {
    if (this->LookupWindow(WID_SELECTPACKAGEITEMBOX) || this->LookupWindow(WID_WAITDBSAVEWND)) {
      return false;
    }
  }

  switch (hotkey) {
  case HK_BASICINFOWND_MINI + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, ID_MINI, 0, 0, 0);
    }
    break;
  case HK_EQUIPWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, ID_EQUIP, 0, 0, 0);
    }
    break;
  case HK_SKILLWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, ID_SKILL, 0, 0, 0);
    }
    break;
  case HK_QUESTWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, ID_QUEST, 0, 0, 0);
    }
    break;
  case HK_FRIENDWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      int previous;
      std::memcpy(&previous, this->messengerPartyTabState, sizeof(previous));
      int partyTab = 0;
      std::memcpy(this->messengerPartyTabState, &partyTab, sizeof(partyTab));
      const bool closed = g_windowMgr.CloseWindow(WID_MESSENGERWND);
      std::memcpy(&partyTab, this->messengerPartyTabState, sizeof(partyTab));
      if (!closed || previous != partyTab) {
        g_windowMgr.EnsureWindow(WID_MESSENGERWND);
      }
    }
    break;
  case HK_PARTYWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      int previous;
      std::memcpy(&previous, this->messengerPartyTabState, sizeof(previous));
      int partyTab = 1;
      std::memcpy(this->messengerPartyTabState, &partyTab, sizeof(partyTab));
      const bool closed = g_windowMgr.CloseWindow(WID_MESSENGERWND);
      std::memcpy(&partyTab, this->messengerPartyTabState, sizeof(partyTab));
      if (!closed || previous != partyTab) {
        g_windowMgr.EnsureWindow(WID_MESSENGERWND);
      }
    }
    break;
  case HK_CHATWND_MAKE + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, 0xD6, 0, 0, 0);
    }
    break;
  case HK_CARTWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, 0xF7, 0, 0, 0);
    }
    break;
  case HK_MAPWND_ONOFF + 100:
    if (this->basicInfoWnd && !g_windowMgr.CloseWindow(WID_ROMAPWND)) {
      g_windowMgr.EnsureWindow(WID_ROMAPWND);
    }
    break;
  case HK_ITEMWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, ID_ITEMS, 0, 0, 0);
    }
    break;
  case HK_GUILDWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      if (g_guildInfo.m_guildId != 0) {
        if (!g_windowMgr.CloseWindow(WID_GUILDWND)) {
          g_windowMgr.EnsureWindow(WID_GUILDWND);
        }
      } else if (!g_windowMgr.CloseWindow(WID_GUILDHELPERWND)) {
        g_windowMgr.EnsureWindow(WID_GUILDHELPERWND);
      }
    }
    break;
  case HK_CLANWND_ONOFF + 100:
    if (this->basicInfoWnd && !g_windowMgr.CloseWindow(WID_CLANINFOMANAGEWND)) {
      g_windowMgr.EnsureWindow(WID_CLANINFOMANAGEWND);
    }
    break;
  case HK_EMOTIONWND_ONOFF + 100:
    if (this->basicInfoWnd && !g_windowMgr.CloseWindow(WID_CASHEMOTIONLISTWND)) {
      g_windowMgr.EnsureWindow(WID_CASHEMOTIONLISTWND);
    }
    break;
  case HK_SITORSTAND + 100:
    if (g_modeMgr.gameMode) {
      g_modeMgr.gameMode->SendMsg(MM_PROCESS_TALK_TYPE, 0x16, 0, 0, 0);
    }
    break;
  case HK_MINIMAP_ONOFF + 100:
    if (g_windowMgr.modal == nullptr && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->canRotateView != 0) {
      if (this->minimapColor == 0xFFFFFFFF) {
        this->minimapColor = 0xAAFFFFFF;
        return true;
      }
      if (!this->CloseWindow(WID_MINIMAPZOOMWND)) {
        this->EnsureWindow(WID_MINIMAPZOOMWND);
        this->minimapColor = 0xFFFFFFFF;
      }
    }
    break;
  case HK_MACROWND_ONOFF + 100:
    if (this->basicInfoWnd && !g_windowMgr.CloseWindow(WID_EMOTIONWND)) {
      g_windowMgr.EnsureWindow(WID_EMOTIONWND);
    }
    break;
  case HK_EXEWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      ToggleGameSettingsTab4();
    }
    break;
  case HK_ALLWND_OFF + 100:
    this->CycleWindowVisibility(4);
    break;
  case HK_SKILLBAR_ONOFF + 100:
    if (UIWindow* window = this->LookupWindow(WID_SHORTCUTWND)) {
      window->SendMsg(nullptr, UM_COMMAND, ID_PLUS, 0, 0, 0);
    }
    break;
  case HK_CHATWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->ShowWindowById(WID_CHATWND, 1 - this->chatWndVisible);
    }
    break;
  case HK_CHATWND_ZOOM + 100:
    if (UIWindow* window = this->window_01C8) {
      if (window->geometry.visible != 0) {
        window->SendMsg(nullptr, UM_TOGGLEPUSH, 0, 0, 0, 0);
        if (this->window_01C8->geometry.state == 0) {
          this->ShowWindowById(WID_CHATWND, 0);
        }
      } else {
        this->ShowWindowById(WID_CHATWND, 1);
        static_cast<UINewChatWnd*>(this->window_01C8)->ApplyInputLayout();
      }
    }
    break;
  case HK_GAGE_ONOFF + 100:
    if (g_modeMgr.gameMode) {
      g_modeMgr.gameMode->SendMsg(MM_TOGGLE_PLAYERGAGE, 0, 0, 0, 0);
    }
    break;
  case HK_SNAPSHOT + 100:
    g_modeMgr.gameMode->SendMsg(MM_SCREENSHOT, 0, 0, 0, 0);
    break;
  case HK_MEMORIALWND_ONOFF + 100:
    if (UIWindow* window = this->memorialDunWnd) {
      window->Show(window->geometry.visible == 0);
    } else {
      g_windowMgr.EnsureWindow(WID_MEMORIALDUNWND);
    }
    break;
  case HK_PETWND_ONOFF + 100:
    if (this->basicInfoWnd && !g_windowMgr.CloseWindow(WID_PETINFOWND)) {
      g_windowMgr.EnsureWindow(WID_PETINFOWND);
    }
    break;
  case HK_HOMUNWND_ONOFF + 100:
    if (this->basicInfoWnd && g_session.homunAlive != 0 && !g_windowMgr.CloseWindow(WID_HOMUNINFOWND)) {
      g_windowMgr.EnsureWindow(WID_HOMUNINFOWND);
    }
    break;
  case HK_MERWND_ONOFF + 100:
    if (this->basicInfoWnd && g_session.mercenaryAlive != 0 && !g_windowMgr.CloseWindow(WID_MERINFOWND)) {
      g_windowMgr.EnsureWindow(WID_MERINFOWND);
    }
    break;
  case HK_HOMUN_FOLLOW + 100:
    if (g_session.homunAlive != 0) {
      g_session.ClearHomunStatus();
      if (CMode* mode = g_modeMgr.GetGameMode()) {
        std::uint32_t aid;
        std::memcpy(&aid, &g_session.spAndHomunculusStats[8], sizeof(aid));
        CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(mode)->scene->GetGameActorByAID(aid));
        if (actor) {
          if (actor->HasUnknown030CEntry()) {
            AiCommand command;
            command.type = 9;
            actor->SetUnknown0318(reinterpret_cast<const float*>(&command), 0);
          }
        }
      }
    }
    break;
  case HK_MER_FOLLOW + 100:
    if (g_session.mercenaryAlive != 0) {
      g_session.ResetMercenaryStats();
      if (CMode* mode = g_modeMgr.GetGameMode()) {
        std::uint32_t aid;
        std::memcpy(&aid, &g_session.companionTargetState[8], sizeof(aid));
        CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(mode)->scene->GetGameActorByAID(aid));
        if (actor && actor->HasUnknown0310Entry()) {
          AiCommand command;
          command.type = 9;
          actor->SetUnknown0318(reinterpret_cast<const float*>(&command), 1);
        }
      }
    }
    break;
  case HK_FRIEND_OPTIONWND + 100:
    if (this->basicInfoWnd && !g_windowMgr.CloseWindow(WID_FRIENDOPTIONWND)) {
      g_windowMgr.EnsureWindow(WID_FRIENDOPTIONWND);
    }
    break;
  case HK_PARTY_OPTIONWND + 100:
    if (this->basicInfoWnd && !g_windowMgr.CloseWindow(WID_PARTYSETTINGWND_PARTYCOUNT) && g_session.PartyCount()) {
      g_windowMgr.EnsureWindow(WID_PARTYSETTINGWND_PARTYCOUNT);
    }
    break;
  case HK_GROUND_CURSOR_ONOFF + 100: {
    int flag;
    std::memcpy(&flag, g_session.showEffects, sizeof(flag));
    flag = 1 - flag;
    std::memcpy(g_session.showEffects, &flag, sizeof(flag));
    break;
  }
  case HK_CHATMODE_ONOFF + 100:
    if (this->basicInfoWnd && this->chatModeChanged && this->window_01C8 && (this->IsHoverOnWindow01C8Button() || this->hoverWindow == nullptr)) {
      static_cast<UINewChatWnd*>(this->window_01C8)->ToggleInput();
      if (this->hoverWindow) {
        this->hoverWindow->SendMsg(nullptr, UM_DEFPUSH, 0, 0, 0, 0);
      }
    }
    break;
  case HK_BASICINFOWND_BTN_ONOFF + 100:
    if (UIWindow* window = this->LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
    }
    break;
  case HK_CASHSHOPWND + 100:
    if (this->basicInfoWnd) {
      if (g_windowMgr.LookupWindow(WID_CASHSHOP_WND) == nullptr) {
        g_modeMgr.GetGameMode()->SendMsg(MM_REQUEST_CASH_SHOP, 0, 0, 0, 0);
      } else {
        g_windowMgr.CloseWindow(WID_CASHSHOP_WND);
      }
    }
    break;
  case HK_REPLAY_ONOFF + 100:
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_COMMAND, 0x18F, 0, 0, 0);
    }
    break;
  case HK_REPLAY_STARTSTOP + 100:
    if (g_replayAnimationScaling != 0 && this->window_4F168) {
      const int state = Replay::Instance().state;
      this->window_4F168->SendMsg(nullptr, UM_COMMAND, state == 6 ? 0x191 : 0x192, 0, 0, 0);
      this->window_4F168->SendMsg(nullptr, UM_REFRESHBUTTON, 0, 0, 0, 0);
    }
    break;
  case HK_REPLAY_SPEEDP + 100:
    if (g_replayAnimationScaling != 0 && this->window_4F168) {
      this->window_4F168->SendMsg(nullptr, UM_COMMAND, ID_INCREASE_REPLAY_SPEED, 0, 0, 0);
    }
    break;
  case HK_REPLAY_SPEEDN + 100:
    if (g_replayAnimationScaling != 0 && this->window_4F168) {
      this->window_4F168->SendMsg(nullptr, UM_COMMAND, ID_DECREASE_REPLAY_SPEED, 0, 0, 0);
    }
    break;
  case HK_NAVIGATION_ONOFF + 100:
    if (g_windowMgr.window_4F174) {
      g_windowMgr.window_4F174->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
    } else if (g_windowMgr.modal) {
      g_windowMgr.modal->SendMsg(nullptr, UM_COMMAND, ID_BASE, 0, 0, 0);
    } else {
      g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
    }
    break;

  case HK_BANK + 100:
    if (g_windowMgr.window_0448) {
      g_windowMgr.CloseWindow(WID_BANK_NEWWND);
    } else {
      BankingCheckPacket packet;
      packet.id = 0x9AB;
      packet.aid = g_session.m_aid;
      CRagConnection& connection = GetRagConnection();
      GetRagConnection();
      connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
    }
    break;
  case HK_STATUSWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, ID_STATUS, 0, 0, 0);
    }
    break;
  case HK_SHORTCUTWND_ROTATE + 100: {
    if (UIWindow* window = this->window_01E8) {
      window->SendMsg(nullptr, UM_ROTATE_SHORTCUT_ROWS, 1, g_session.GetOption(10), 0, 0);
    }
    ShortcutRotatePacket packet;
    packet.id = 0xB22;
    packet.option = static_cast<std::uint16_t>(g_session.GetOption(10));
    if (packet.option == 0) {
      packet.state = static_cast<unsigned char>(g_session.mainShortcutRow);
    } else if (packet.option == 1) {
      packet.state = static_cast<unsigned char>(g_session.alternateShortcutRow);
    }
    CRagConnection& connection = GetRagConnection();
    GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
    break;
  }
  case HK_ACHIEVEMENT + 100:
    if (!g_windowMgr.CloseWindow(WID_ACHIEVEMENTWND)) {
      g_windowMgr.EnsureWindow(WID_ACHIEVEMENTWND);
    }
    break;
  case HK_SINGLEMAP_ONOFF + 100:
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->canRotateView != 0) {
      if (!g_session.IsNavigationAllowed()) {
        g_windowMgr.Confirm(LookupMessage(MSI_BANK_0_MONEY), 0, true, false, 0, "Message", 280, 120, 0);
      } else if (!g_windowMgr.CloseWindow(WID_MINIMAPWND)) {
        g_windowMgr.EnsureWindow(WID_MINIMAPWND);
      }
    }
    break;
  case HK_MAIL_ONOFF + 100:
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      window->SendMsg(nullptr, UM_COMMAND, 0x1DC, 0, 0, 0);
    }
    break;
  case HK_SWAP_EQUIPMENT + 100:
    g_modeMgr.GetGameMode()->SendMsg(MM_SWAP_EQUIPMENT, 0, 0, 0, 0);
    break;
  case HK_TIPBOXWND_ONOFF + 100:
    if (!g_windowMgr.CloseWindow(WID_TIPBOXWND)) {
      g_windowMgr.EnsureWindow(WID_TIPBOXWND);
    }
    break;
  case HK_CHANGE_SKILLBAR + 100:
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_SHORTCUTWND)) {
      window->SendMsg(nullptr, UM_COMMAND, 0x225, 0, 0, 0);
    }
    break;
  case HK_COSEQUIPWND_ONOFF + 100:
    if (this->basicInfoWnd) {
      this->basicInfoWnd->SendMsg(nullptr, UM_COMMAND, 0x242, 0, 0, 0);
    }
    break;
  case HK_PARTYBOARDWND_ONOFF + 100:
    if (!g_windowMgr.CloseWindow(WID_ADVENPARTYBOARDWND)) {
      g_windowMgr.EnsureWindow(WID_ADVENPARTYBOARDWND);
    }
    break;
  case 0x9F:
    Singleton<CGameSettingsMgr>::GetInstance()->ToggleOption(0x109);
    break;
  case 0xA0:
    if (!g_windowMgr.CloseWindow(WID_REPUTEWND)) {
      g_windowMgr.EnsureWindow(WID_REPUTEWND);
    }
    break;
  case 0xA1:
    if (!g_windowMgr.CloseWindow(WID_EQUIPMENTPROPERTIESWND)) {
      g_windowMgr.EnsureWindow(WID_EQUIPMENTPROPERTIESWND);
    }
    break;
  case 0xA3:
    Singleton<CGameSettingsMgr>::GetInstance()->ToggleOption(0x10A);
    break;
  case HK_MACRO_1 + 200:
  case HK_MACRO_2 + 200:
  case HK_MACRO_3 + 200:
  case HK_MACRO_4 + 200:
  case HK_MACRO_5 + 200:
  case HK_MACRO_6 + 200:
  case HK_MACRO_7 + 200:
  case HK_MACRO_8 + 200:
  case HK_MACRO_9 + 200:
  case HK_MACRO_10 + 200:
    if (g_modeMgr.gameMode) {
      this->SendEmotionHotkey(hotkey - 200);
    }
    break;
  case HK_FLAG_1 + 200:
  case HK_FLAG_2 + 200:
  case HK_FLAG_3 + 200:
  case HK_FLAG_4 + 200:
  case HK_FLAG_5 + 200:
  case HK_FLAG_6 + 200:
  case HK_FLAG_7 + 200:
  case HK_FLAG_8 + 200:
  case HK_FLAG_9 + 200:
  case HK_FLAG_10 + 200:
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
  if (transient && static_cast<UIDragMiniPartyWnd*>(transient)->member.valid && transient->geometry.state == 0) {
    bool behavior = false;
    g_session.m_hotkeyScript->Call("IsBeHaviorOfWindow", "d>b", this->GetHotkeyBehavior(), &behavior);
    if (behavior && this->window_4F250 && static_cast<UIDragMiniPartyWnd*>(this->window_4F250)->member.valid) {
      this->window_4F250->geometry.state = 1;
      this->Schedule(this->window_4F250);
      this->window_4F250 = nullptr;
    }
  }

  if (this->window_4F288) {
    switch (id) {
    case WID_ITEMWND:
    case WID_EQUIPWND:
    case WID_MENUWND:
    case WID_MERCHANTITEMWND:
    case WID_ESCOPTIONWND:
    case WID_NAVIGATIONV4WND:
    case 251:
    case WID_RODEXINBOXWND:
    case WID_BANK_NEWWND:
      return nullptr;
    }
  }

  if (!this->IsOrderFixed(id)) {
    this->RemoveOrder(id);
    if (id == WID_NAVIGATIONV4WND) {
      this->windowHistory.push_back(int(id));
    } else {
      this->windowHistory.push_front(int(id));
    }
  }

  unsigned char* const base = reinterpret_cast<unsigned char*>(this);
  const auto layerGreater = [](const UIWindow* left, const UIWindow* right) {
    return static_cast<int>(left->renderLayer) > static_cast<int>(right->renderLayer);
  };
  UIWindow* window = nullptr;
  switch (id) {
  case WID_BASICINFOWND:
    window = this->basicInfoWnd;
    if (window == nullptr) {
      window = new UIBasicInfoWnd();
      this->basicInfoWnd = window;
      window->SetSize(220, g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true ? 71 : 55);
      window->Move(0, 0);
      this->AddRendered(window);
      this->basicInfoWnd->SendMsg(this->basicInfoWnd, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->basicInfoPlacement), 0, 0, 0);
      g_snapMgr.SetRoot(window);
      if (UIWindow* navigation = g_windowMgr.EnsureWindow(WID_MENUICONWND)) {
        int x = 0;
        int y = 0;
        window->ToScreen(x, y);
        navigation->Move(x, window->geometry.height + y);
      }
    }
    window->Show(g_replayAnimationScaling ? Replay::Instance().header.options.Get(ReplayRecordOption::BasicInfo) : 1);
    if (this->savedWindowIds.size() != 0) {
      for (int savedId : this->savedWindowIds) {
        if (savedId == WID_MINIPARTYWND) {
          this->RebuildMiniPartyWindows();
        } else {
          this->EnsureWindow(savedId);
        }
      }
    } else {
      if (this->itemWndVisible) {
        this->EnsureWindow(WID_ITEMWND);
      }
      if (this->equipWndVisible) {
        this->EnsureWindow(WID_EQUIPWND);
      }
      if (this->statusWndVisible) {
        this->EnsureWindow(WID_STATUSWND);
      }
      if (this->optionWndVisible) {
        this->EnsureWindow(13);
      }
      if (this->skillListWndVisible) {
        this->EnsureWindow(WID_SKILLLISTWND);
      }
      if (this->questWndVisible) {
        this->EnsureWindow(WID_RENEWQUESTWND);
      }
      if (this->messengerGroupWndVisible) {
        this->EnsureWindow(WID_MESSENGERWND);
      }
      if (g_session.lowMemory == 1 && g_session.performanceWarningEnabled == 1 && g_effectDetailLevel == 2 && g_textureDetailLevel == 2) {
        this->EnsureWindow(WID_GAMESETTINGSUI);
      }
      if (g_session.HasUnknown6178Records() == true) {
        this->EnsureWindow(WID_ACHTRACINGWND);
      }
      if (this->macroReporterVisible == true) {
        this->EnsureWindow(WID_MACROREPORTERWND);
      }
      this->RebuildMiniPartyWindows();
    }
    if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->canRotateView) {
      this->EnsureWindow(WID_MINIMAPZOOMWND);
    }
    this->ShowWindowById(WID_CHATWND, this->chatWndVisible);
    this->ShowWindowById(WID_SHORTCUTWND, this->m_normalItemSlotWindow.show);
    this->ShowWindowById(WID_ITEMDROPCNTWND, 0);
    break;
  case WID_CHATWND:
    window = this->window_01C8;
    if (window == nullptr) {
      window = new UINewChatWnd();
      this->window_01C8 = window;
      window->SetSize(600, this->chatWndHeight);
      this->AddRendered(window);
    }
    this->window_01C8->SendMsg(nullptr, UM_SETINFO, 0, 0, 0, 0);
    break;
  case WID_SELECTSERVERWND:
    window = new UISelectServerWnd();
    window->SetSize(280, 200);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 80);
    this->AddRendered(window);
    break;
  case WID_RPIMAGEWND2:
    if (this->LookupWindow(WID_RPIMAGEWND2) == nullptr) {
      window = new UIRPImageWnd2();
      this->window_4F25C = window;
      window->SetSize(301, 132);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_RPIMAGEWND2);
    }
    break;
  case WID_LOGINWND:
    window = new UILoginWnd();
    this->window_01CC = window;
    window->SetSize(301, 132);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case WID_LOGINOTPWND:
    window = new UILoginOTPWnd();
    this->window_01D0 = window;
    window->SetSize(280, 142);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case WID_NOTICEWND:
    window = new UINoticeWnd();
    window->SetSize(600, 400);
    window->Move(CenterOnViewportX(33), CenterOnViewportY(40));
    this->AddRendered(window);
    break;
  case WID_MAKECHARWND:
    window = new UIMakeCharWnd();
    window->SetSize(150, 286);
    window->Move((g_renderer->viewport.width - 150) / 2, (g_renderer->viewport.height - 286) / 2);
    this->AddRendered(window);
    break;
  case WID_NEWSELECTCHARWND: {
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
  case WID_NEWMAKECHARWND: {
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
  case WID_ITEMWND:
    window = this->itemWnd;
    if (window == nullptr) {
      this->RecordWindowPacket(8);
      window = new UIItemWnd();
      this->itemWnd = window;
      window->SetSize(280, 120);
      window->Move(0, 120);
      this->AddRendered(window);
      this->itemWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x528), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->itemWndVisible = 1;
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      basicInfo->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC2, 0, 0);
    }
    break;
  case WID_QUESTDISPLAY:
    if (this->window_0428 == nullptr) {
      UIQuestDisplay* display = new UIQuestDisplay();
      this->window_0428 = display;
      display->Create(250, 400);
      this->window_0428->Move(this->screenWidth - 320, 150);
      this->window_0428->renderLayer = 5;
      this->AddRendered(this->window_0428);
      this->rendered.sort(layerGreater);
      this->window_0428->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x570), 0, 0, 0);
    }
    break;
  case WID_WAITWND:
    window = new UIWaitWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case WID_LOADINGWND:
    window = this->loadingWnd;
    if (window == nullptr) {
      window = new UILoadingWnd();
      this->loadingWnd = window;
      window->SetSize(240, 15);
      window->Move(g_renderer->viewport.width / 2 - 100, g_renderer->viewport.height / 2 + 160);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_TOOLTIPWND:
    window = new UIToolTipWnd();
    window->SetSize(200, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case WID_SHORTCUTWND:
    window = this->window_01E8;
    if (window == nullptr) {
      window = new UIShortCutWnd();
      this->window_01E8 = window;
      window->SetSize(290, 34);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_01E8->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&this->m_normalItemSlotWindow), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    break;
  case WID_EQUIPWND:
    window = this->equipWnd;
    if (window == nullptr) {
      window = new UIEquipWnd();
      this->equipWnd = window;
      static_cast<UIEquipWnd*>(window)->m_viewOther = 0;
      window->SetSize(280, 167);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->equipWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x590), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->equipWndVisible = 1;
    break;
  case WID_NCASHWND:
    window = this->nCashWnd;
    if (window == nullptr) {
      window = new UInCashWnd();
      this->nCashWnd = window;
      window->SetSize(640, 540);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(30));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x9B4), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    break;
  case WID_STATUSWND:
    window = this->statusWnd;
    if (window == nullptr) {
      window = new UIStatusWnd();
      this->statusWnd = window;
      window->SetSize(280, 141);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->statusWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x574), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->statusWndVisible = 1;
    this->CloseWindow(WID_NOTIFYLEVELUPWND);
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      basicInfo->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC0, 0, 0);
    }
    break;
  case WID_SKILLLISTWND:
    window = this->window_02C4;
    if (window == nullptr) {
      window = new UINewSkillListWnd();
      this->window_02C4 = window;
      window->SetSize(560, 400);
      window->Move(CenterOnViewportX(50), ScaleByScreenHeight(50));
      this->AddRendered(window);
      this->window_02C4->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x76C), 0, 0, 0);
    }
    window->Show(1);
    this->skillListWndVisible = 1;
    this->CloseWindow(WID_NOTIFYJOBLEVELUPWND);
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      basicInfo->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0xC4, 0, 0);
    }
    break;
  case WID_ITEMSTOREWND:
    window = this->window_0288;
    if (window == nullptr) {
      window = new UIItemStoreWnd();
      this->window_0288 = window;
      window->SetSize(280, 458);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_0288->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x614), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMSTORESUBWND_TAB_0:
  case WID_ITEMSTORESUBWND_TAB_1:
  case WID_ITEMSTORESUBWND_TAB_2:
  case WID_ITEMSTORESUBWND_TAB_3:
  case WID_ITEMSTORESUBWND_TAB_4:
  case WID_ITEMSTORESUBWND_TAB_5:
  case WID_ITEMSTORESUBWND_TAB_6: {
    const int tab = id - 146;
    window = this->itemStoreTabWnds[tab];
    if (window == nullptr) {
      UIItemStoreSubWnd* store = new UIItemStoreSubWnd();
      window = store;
      this->itemStoreTabWnds[tab] = window;
      store->SetTabIndex(tab);
      int message = 0;
      switch (id) {
      case WID_ITEMSTORESUBWND_TAB_0:
      case WID_ITEMSTORESUBWND_TAB_1:
      case WID_ITEMSTORESUBWND_TAB_2:
      case WID_ITEMSTORESUBWND_TAB_3:
        message = tab + 0x5B9;
        break;
      case WID_ITEMSTORESUBWND_TAB_4:
        message = 0xC4F;
        break;
      case WID_ITEMSTORESUBWND_TAB_5:
      case WID_ITEMSTORESUBWND_TAB_6:
        message = tab + 0x5B8;
        break;
      }
      window->SetSize(140, 145);
      static_cast<UIItemStoreSubWnd*>(this->itemStoreTabWnds[tab])->SetTitle(LookupMessage(message));
      this->AddRendered(window);
      this->itemStoreTabWnds[tab]->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(reinterpret_cast<ItemStoreFindSlotInfo*>(base + 0x62C) + tab), 0, 0, 0);
    }
    window->Show(1);
    break;
  }
  case WID_ITEMSTORESUBWND_TAB_7:
    window = this->itemStoreLastTabWnd;
    if (window == nullptr) {
      UIItemStoreSubWnd* store = new UIItemStoreSubWnd();
      window = store;
      this->itemStoreLastTabWnd = window;
      store->SetTabIndex(7);
      window->SetSize(140, 145);
      static_cast<UIItemStoreSubWnd*>(this->itemStoreLastTabWnd)->SetTitle(LookupMessage(id == WID_ITEMSTORESUBWND_TAB_7 ? 0x5BF : 0));
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->itemStoreLastTabWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x69C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMSTOREFINDWND:
    window = this->itemStoreFindWnd;
    if (window == nullptr) {
      window = new UIItemStoreFindWnd();
      this->itemStoreFindWnd = window;
      window->SetSize(140, 145);
      static_cast<UIItemStoreFindWnd*>(this->itemStoreFindWnd)->SetTitle(LookupMessage(MSI_AUCTION_SEARCH));
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->itemStoreFindWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6AC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PARTYBOOKINGHELPWND:
    if (this->partyBookingHelpWnd == nullptr) {
      window = new UIPartyBookingHelpWnd();
      this->partyBookingHelpWnd = window;
      window->SetSize(320, 199);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    break;
  case WID_JOBLISTWND:
    if (this->jobListWnd == nullptr) {
      window = new UIJobListWnd();
      this->jobListWnd = window;
      window->SetSize(152, 199);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->jobListWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6EC), 0, 0, 0);
    }
    break;
  case WID_SEEKPARTYWND:
    window = this->seekPartyWnd;
    if (window == nullptr) {
      window = new UISeekPartyWnd();
      this->seekPartyWnd = window;
      window->SetSize(152, 199);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->seekPartyWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6DC), 0, 0, 0);
    }
    window->Show(1);
    g_session.m_partyBooking.unknown_0000 = 1;
    break;
  case WID_SEEKPARTYMBWND:
    if (this->seekPartyWnd == nullptr) {
      if (this->seekPartyMessageWnd == nullptr) {
        window = new UISeekPartyMBWnd();
        this->seekPartyMessageWnd = window;
        window->SetSize(360, 350);
        window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
        this->AddRendered(window);
        this->seekPartyMessageWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6CC), 0, 0, 0);
      } else {
        g_windowMgr.CloseWindow(WID_SEEKPARTYMBWND);
      }
    }
    break;
  case WID_SEEKPARTYLISTWND:
    if (this->seekPartyListWnd == nullptr) {
      window = new UISeekPartyListWnd();
      this->seekPartyListWnd = window;
      window->SetSize(550, 350);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->seekPartyListWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6BC), 0, 0, 0);
    } else {
      g_windowMgr.CloseWindow(WID_SEEKPARTYLISTWND);
    }
    break;
  case WID_MESSENGERWND:
    if (*reinterpret_cast<const int*>(this->messengerPartyTabState)) {
      this->EnsureWindow(WID_MESSENGERGROUPWND)->SendMsg(nullptr, UM_COMMAND, ID_RADIOBTN, 2, 0, 0);
    } else {
      this->EnsureWindow(WID_MESSENGERGROUPWND)->SendMsg(nullptr, UM_COMMAND, ID_RADIOBTN, 1, 0, 0);
    }
    break;
  case WID_MESSENGERGROUPWND:
    if (g_replayAnimationScaling && !Replay::Instance().header.options.Get(ReplayRecordOption::Party)) {
      break;
    }
    this->CloseWindow(WID_MESSENGERWND);
    window = this->window_02C8;
    if (window == nullptr) {
      window = new UIMessengerGroupWnd();
      this->window_02C8 = window;
      window->SetSize(273, 326);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_02C8->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x72C), 0, 0, 0);
    }
    window->Show(1);
    this->messengerGroupWndVisible = 1;
    break;
  case WID_MERCHANTSHOPMAKEWND:
    window = this->merchantShopMakeWnd;
    if (window == nullptr) {
      window = new UIMerchantShopMakeWnd(0);
      this->merchantShopMakeWnd = window;
      window->SetSize(400, 260);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(200));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x7A4), 0, 0, 0);
    }
    window->Show(1);
    this->EnsureWindow(WID_MERCHANTMIRRORITEMWND);
    break;
  case WID_MERCHANTMIRRORITEMWND:
    window = reinterpret_cast<UIWindow*>(this->merchantMirrorItemWnd);
    if (window == nullptr) {
      window = new UIMerchantMirrorItemWnd(0);
      this->merchantMirrorItemWnd = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->merchantMirrorItemPlacement), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MERCHANTITEMWND:
    if (!g_mapPropertyMgr->GetValue(g_session.mapPropertyId, 40)) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RANKING_NUMBER)), 255, 0, 0, 0);
      break;
    }
    window = this->merchantItemWnd;
    if (window == nullptr) {
      window = new UIMerchantItemWnd();
      this->merchantItemWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->merchantItemPlacement), 0, 0, 0);
    }
    window->Show(1);
    this->merchantItemWndVisible = 1;
    break;
  case WID_NOTIFYITEMOBTAINWND:
    window = this->notifyItemObtainWnd;
    if (window == nullptr) {
      window = new UINotifyItemObtainWnd();
      this->notifyItemObtainWnd = window;
      window->SetSize(180, 32);
      window->Move(CenterOnViewportX(220), 75);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_NPCEDITDIALOGWND:
    window = this->npcEditDialogWnd;
    if (window == nullptr) {
      window = new UINpcEditDialogWnd();
      this->npcEditDialogWnd = window;
      window->SetSize(176, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_NPCTEXTEDITDIALOGWND:
    window = this->npcTextEditDialogWnd;
    if (window == nullptr) {
      window = new UINpcTextEditDialogWnd();
      this->npcTextEditDialogWnd = window;
      window->SetSize(346, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SKILLNAMECHANGEWND:
    window = this->skillNameChangeWnd;
    if (window == nullptr) {
      window = new UISkillNameChangeWnd();
      this->skillNameChangeWnd = window;
      window->SetSize(280, 48);
      window->Move(CenterOnViewportX(185), CenterOnViewportY(180));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_GUILDHELPERWND:
    window = this->guildHelperWnd;
    if (window == nullptr) {
      window = new UIGuildHelperWnd();
      this->guildHelperWnd = window;
      window->SetSize(200, 100);
      window->Move(CenterOnViewportX(220), CenterOnViewportY(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x9D4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDTIPWND:
    window = this->guildTipWnd;
    if (window == nullptr) {
      window = new UIGuildTipWnd();
      this->guildTipWnd = window;
      window->SetSize(330, 230);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_CREATEGUILDWND_MODE_0:
    window = this->createGuildWnd;
    if (window == nullptr) {
      window = new UICreateGuildWnd(0);
      this->createGuildWnd = window;
      window->SetSize(150, 100);
      window->Move(CenterOnViewportX(200), CenterOnViewportY(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_CREATEGUILDWND_MODE_1:
    window = this->createGuildAlternateWnd;
    if (window == nullptr) {
      window = new UICreateGuildWnd(1);
      this->createGuildAlternateWnd = window;
      window->SetSize(150, 100);
      window->Move(CenterOnViewportX(200), CenterOnViewportY(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_GUILDWND: {
    GuildWndInfo* guildInfo = reinterpret_cast<GuildWndInfo*>(base + 0x834);
    if (!guildInfo->IsValid()) {
      guildInfo->Reset();
    }
    switch (guildInfo->tab) {
    case 0:
      window = g_windowMgr.EnsureWindow(WID_GUILDINFOMANAGEWND);
      break;
    case 1:
      window = g_windowMgr.EnsureWindow(WID_GUILDMEMBERMANAGEWND);
      break;
    case 2:
      window = g_windowMgr.EnsureWindow(WID_GUILDPOSITIONMANAGEWND);
      break;
    case 3:
      window = g_windowMgr.EnsureWindow(WID_GUILDSKILLWND);
      break;
    case 4:
      window = g_windowMgr.EnsureWindow(WID_GUILDBANISHEDMEMBERWND);
      break;
    case 5:
      window = g_windowMgr.EnsureWindow(WID_GUILDNOTICEWND);
      break;
    case 6:
      window = g_windowMgr.EnsureWindow(WID_GUILDTOTALINFOWND);
      break;
    }
    break;
  }
  case WID_GUILDMEMBERMANAGEWND:
    window = this->window_0324;
    if (window == nullptr) {
      window = new UIGuildMemberManageWnd();
      this->window_0324 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDPOSITIONMANAGEWND:
    window = this->guildPositionManageWnd;
    if (window == nullptr) {
      window = new UIGuildPositionManageWnd();
      this->guildPositionManageWnd = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDSKILLWND:
    window = this->window_032C;
    if (window == nullptr) {
      window = new UIGuildSkillWnd();
      this->window_032C = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDBANISHEDMEMBERWND:
    window = this->guildBanishedMemberWnd;
    if (window == nullptr) {
      window = new UIGuildBanishedMemberWnd();
      this->guildBanishedMemberWnd = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDNOTICEWND:
    window = this->guildNoticeWnd;
    if (window == nullptr) {
      window = new UIGuildNoticeWnd();
      this->guildNoticeWnd = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDTOTALINFOWND:
    window = this->guildTotalInfoWnd;
    if (window == nullptr) {
      window = new UIGuildTotalInfoWnd();
      this->guildTotalInfoWnd = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDINFOMANAGEWND:
    window = this->window_0320;
    if (window == nullptr) {
      window = new UIGuildInfoManageWnd();
      this->window_0320 = window;
      window->SetSize(g_defaultPlacementWidth, g_defaultPlacementHeight);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x834), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_GUILDLEAVEREASONDESCWND:
    window = this->guildLeaveReasonDescWnd;
    if (window == nullptr) {
      window = new UIGuildLeaveReasonDescWnd();
      this->guildLeaveReasonDescWnd = window;
      window->SetSize(280, 80);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ILLUSTWND:
    window = this->illustWnd;
    if (window == nullptr) {
      window = new UIIllustWnd();
      this->illustWnd = window;
      window->SetSize(4, 4);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->rendered.insert(this->rendered.end(), window);
      this->illustWnd->renderLayer = 2;
      this->rendered.sort(layerGreater);
    } else {
      this->illustWnd->SendMsg(nullptr, UM_SETINFO, -1, 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MULTIILLUSTWND_SLOT_0:
  case WID_MULTIILLUSTWND_SLOT_1:
  case WID_MULTIILLUSTWND_SLOT_2:
  case WID_MULTIILLUSTWND_SLOT_3:
  case WID_MULTIILLUSTWND_SLOT_4:
  case WID_MULTIILLUSTWND_SLOT_5:
  case WID_MULTIILLUSTWND_SLOT_6:
  case WID_MULTIILLUSTWND_SLOT_7:
  case WID_MULTIILLUSTWND_SLOT_8:
  case WID_MULTIILLUSTWND_SLOT_9:
    window = this->multiIllustWnds[id - 216];
    if (window == nullptr) {
      window = new UIMultiIllustWnd();
      this->multiIllustWnds[id - 216] = window;
      window->SetSize(4, 4);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddOverlay(id - 216, window);
    }
    window->Show(1);
    break;
  case WID_GUILDBANREASONDESCWND:
    window = this->guildBanReasonDescWnd;
    if (window == nullptr) {
      window = new UIGuildBanReasonDescWnd();
      this->guildBanReasonDescWnd = window;
      window->SetSize(280, 80);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_FRIENDOPTIONWND:
    window = this->friendOptionWnd;
    if (window == nullptr) {
      window = new UIFriendOptionWnd();
      this->friendOptionWnd = window;
      window->SetSize(213, 90);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x86C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PARTYSETTINGWND_PARTYCOUNT:
    window = this->partySettingWnd;
    if (window == nullptr) {
      window = new UIPartySettingWnd();
      this->partySettingWnd = window;
      window->SetSize(130, g_session.PartyCount() ? 245 : 180);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x814), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PARTYSETTINGWND_HEIGHT_220:
    window = this->partySettingWnd;
    if (window == nullptr) {
      window = new UIPartySettingWnd();
      this->partySettingWnd = window;
      window->SetSize(130, 220);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x814), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PARTYINVITATIONTOWND:
    window = this->partyInvitationToWnd;
    if (window == nullptr) {
      window = new UIPartyInvitationToWnd();
      this->partyInvitationToWnd = window;
      window->SetSize(150, 100);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_COMPOSITIONWND:
    window = this->compositionWnd;
    if (window == nullptr) {
      window = new UICompositionWnd();
      this->compositionWnd = window;
      window->SetSize(34, 34);
      window->Move(0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_CANDIDATEWND:
    window = this->window_03A4;
    if (window == nullptr) {
      window = new UICandidateWnd();
      this->window_03A4 = window;
      const int width = this->compactCompositionLayout ? 120 : 292;
      const int height = this->compactCompositionLayout ? 212 : 34;
      window->SetSize(width, height);
      window->Move(g_renderer->viewport.width - width, g_renderer->viewport.height - height);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_TIPOFTHEDAYWND:
    window = this->tipOfTheDayWnd;
    if (window == nullptr) {
      window = new UITipOfTheDayWnd();
      this->tipOfTheDayWnd = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_JOINGUILDACCEPTWND:
    window = this->joinGuildAcceptWnd;
    if (window == nullptr) {
      window = new UIJoinGuildAcceptWnd();
      this->joinGuildAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ALLYGUILDACCEPTWND:
    window = this->allyGuildAcceptWnd;
    if (window == nullptr) {
      window = new UIAllyGuildAcceptWnd();
      this->allyGuildAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_JOINPARTYACCEPTWND:
    window = this->joinPartyAcceptWnd;
    if (window == nullptr) {
      window = new UIJoinPartyAcceptWnd();
      this->joinPartyAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_COUPLEACCEPTWND:
    window = this->coupleAcceptWnd;
    if (window == nullptr) {
      window = new UICoupleAcceptWnd();
      this->coupleAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_STARPLACEACCEPTWND:
    window = this->starPlaceAcceptWnd;
    if (window == nullptr) {
      window = new UIStarPlaceAcceptWnd();
      this->starPlaceAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MIXACCEPTWND:
    window = this->mixAcceptWnd;
    if (window == nullptr) {
      window = new UIMixAcceptWnd();
      this->mixAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_YOURITEMWND:
    window = this->yourItemWnd;
    if (window == nullptr) {
      window = new UIYourItemWnd();
      this->yourItemWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_BABYACCEPTWND:
    window = this->babyAcceptWnd;
    if (window == nullptr) {
      window = new UIBabyAcceptWnd();
      this->babyAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_BABYACCEPTWND2:
    window = this->babyAcceptWnd2;
    if (window == nullptr) {
      window = new UIBabyAcceptWnd2();
      this->babyAcceptWnd2 = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_EXCHANGEACCEPTWND:
    window = this->exchangeAcceptWnd;
    if (window == nullptr) {
      window = new UIExchangeAcceptWnd();
      this->exchangeAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->exchangeAcceptWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x75C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PASSWORDWND:
    window = this->passwordWnd;
    if (window == nullptr) {
      window = new UIPasswordWnd();
      this->passwordWnd = window;
      window->SetSize(176, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ITEMDROPCNTWND:
    window = this->window_01EC;
    if (window == nullptr) {
      window = new UIItemDropCntWnd();
      this->window_01EC = window;
      window->SetSize(182, 46);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    break;
  case WID_SAYDIALOGWND:
    window = this->sayDialogWnd;
    if (window == nullptr) {
      window = new UISayDialogWnd();
      this->sayDialogWnd = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(100));
      this->AddRendered(window);
      this->sayDialogWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x5E4), 0, 0, 0);
    }
    window->Show(1);
    this->sayDialogWnd->renderLayer = 1;
    this->rendered.sort(layerGreater);
    break;
  case WID_SAYDIALOGWND_OVERLAY:
    window = this->sayDialogOverlayWnd;
    if (window == nullptr) {
      window = new UISayDialogWnd();
      this->sayDialogOverlayWnd = window;
      window->SetSize(g_renderer->viewport.width, 100);
      window->Move(0, g_renderer->viewport.height - 100);
      this->AddOverlay(WID_COMBINEDCARDITEMCOLLECTION_COMPARISONWND, window);
    }
    window->Show(1);
    break;
  case WID_CHATROOMWND:
    window = this->window_027C;
    if (window == nullptr) {
      window = new UIChatRoomWnd();
      this->window_027C = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->window_027C->SendMsg(nullptr, UM_SETINFO2, reinterpret_cast<std::intptr_t>(base + 0x604), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_CHOOSEWARPWND:
    window = this->chooseWarpWnd;
    if (window == nullptr) {
      window = new UIChooseWarpWnd();
      this->chooseWarpWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_CHOOSEWND:
    window = this->choose3Wnd;
    if (window == nullptr) {
      window = new UIChoose3Wnd();
      this->choose3Wnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
      this->choose3Wnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x5F4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMCOMPOSITIONWND:
    window = this->itemCompositionWnd;
    if (window == nullptr) {
      window = new UIItemCompositionWnd();
      this->itemCompositionWnd = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->itemCompositionWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x87C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMIDENTIFYWND:
    window = this->itemIdentifyWnd;
    if (window == nullptr) {
      window = new UIItemIdentifyWnd();
      this->itemIdentifyWnd = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->itemIdentifyWnd->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x88C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_COMBOBOXWND:
    window = this->comboBoxWnd;
    if (window == nullptr) {
      window = new UIComboBoxWnd();
      this->comboBoxWnd = window;
      window->SetSize(16, 16);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_COMBOBOXIDWND:
    window = this->comboBoxIDWnd;
    if (window == nullptr) {
      window = new UIComboBoxIDWnd();
      this->comboBoxIDWnd = window;
      window->SetSize(16, 16);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MENUWND:
    window = this->menuWnd;
    if (window == nullptr) {
      window = new UIMenuWnd();
      this->menuWnd = window;
      window->SetSize(4, 4);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MINIMAPZOOMWND:
    window = this->minimapZoomWnd;
    if (window == nullptr) {
      window = new UIMinimapZoomWnd();
      this->minimapZoomWnd = window;
      window->SetSize(128, 140);
      window->Move(g_renderer->viewport.width - 145, 17);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_CHATROOMCHANGEWND:
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
  case WID_CHATROOMMAKEWND:
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
  case WID_CHOOSESELLBUYWND:
    window = this->chooseSellBuyWnd;
    if (window == nullptr) {
      window = new UIChooseSellBuyWnd();
      this->chooseSellBuyWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ITEMSELLWND:
    window = this->itemSellWnd;
    if (window == nullptr) {
      window = new UIItemSellWnd();
      this->itemSellWnd = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x70C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMSELLWND2:
    window = this->itemSellWnd2;
    if (window == nullptr) {
      window = new UIItemSellWnd2();
      this->itemSellWnd2 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x70C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MERCHANTITEMPURCHASEWND:
    window = this->merchantItemPurchaseWnd;
    if (window == nullptr) {
      window = new UIMerchantItemPurchaseWnd(0);
      this->merchantItemPurchaseWnd = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x7D4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMPURCHASEWND:
    window = this->itemPurchaseWnd;
    if (window == nullptr) {
      window = new UIItemPurchaseWnd();
      this->itemPurchaseWnd = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x71C), 0, 0, 0);
    }
    window->Show(1);
    this->EnsureWindow(WID_ITEMPARAMCHANGEDISPLAYWND);
    break;
  case WID_MERCHANTITEMMYSHOPWND:
    window = this->merchantItemMyShopWnd;
    if (window == nullptr) {
      window = new UIMerchantItemMyShopWnd(0);
      this->merchantItemMyShopWnd = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x7C4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MERCHANTITEMSHOPWND:
    window = this->merchantItemShopWnd;
    if (window == nullptr) {
      window = new UIMerchantItemShopWnd(0);
      this->merchantItemShopWnd = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x7B4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMSHOPWND:
    window = this->itemShopWnd;
    if (window == nullptr) {
      window = new UIItemShopWnd();
      this->itemShopWnd = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMSHOPWND2:
    window = this->itemShopWnd2;
    if (window == nullptr) {
      window = new UIItemShopWnd2();
      this->itemShopWnd2 = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PARAITEMSHOPWND:
    window = this->paraItemShopWnd;
    if (window == nullptr) {
      window = new UIParaItemShopWnd();
      this->paraItemShopWnd = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PARAITEMPURCHASEWND:
    window = this->paraItemPurchaseWnd;
    if (window == nullptr) {
      window = new UIParaItemPurchaseWnd();
      this->paraItemPurchaseWnd = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x71C), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_PARARESULTWND:
    window = this->window_0270;
    if (window == nullptr) {
      window = new UIParaResultWnd();
      this->window_0270 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, 0, 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_NOTIFYJOBLEVELUPWND:
    window = this->notifyJobLevelUpWnd;
    if (window == nullptr) {
      window = new UINotifyJobLevelUpWnd();
      this->notifyJobLevelUpWnd = window;
      window->SetSize(43, 43);
      window->Move(0, g_renderer->viewport.height - 43);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_NOTIFYLEVELUPWND:
    window = this->notifyLevelUpWnd;
    if (window == nullptr) {
      window = new UINotifyLevelUpWnd();
      this->notifyLevelUpWnd = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 43, g_renderer->viewport.height - 43);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_NOTIFYQUESTWND:
    window = this->notifyQuestWnd;
    if (window == nullptr) {
      window = new UINotifyQuestWnd();
      this->notifyQuestWnd = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 43, g_renderer->viewport.height - 43);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_NCASH_CALLWND:
    window = this->nCashCallWnd;
    if (window == nullptr) {
      window = new UInCash_CallWnd();
      this->nCashCallWnd = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 187, 16);
      this->AddRendered(window);
    }
    window->Show(g_serviceOptions.Get(218));
    break;
  case WID_CARDITEMILLUSTWND:
    window = this->cardItemIllustWnd;
    if (window == nullptr) {
      window = new UICardItemIllustWnd();
      this->cardItemIllustWnd = window;
      window->SetSize(300, 417);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MONSTERDISPLAYWND:
    window = this->monsterDisplayWnd;
    if (window == nullptr) {
      window = new UIMonsterDisplayWnd();
      this->monsterDisplayWnd = window;
      window->SetSize(100, 150);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_RESTARTWND:
    window = this->restartWnd;
    if (window == nullptr) {
      window = new UIRestartWnd();
      this->restartWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_QUITWND:
    window = this->quitWnd;
    if (window == nullptr) {
      window = new UIQuitWnd();
      this->quitWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SKILLDESCRIBEWND:
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
  case WID_ITEMPARAMCHANGEDISPLAYWND:
    window = this->itemParamChangeDisplayWnd;
    if (window == nullptr) {
      window = new UIItemParamChangeDisplayWnd();
      this->itemParamChangeDisplayWnd = window;
      window->SetSize(280, 34);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 34);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x804), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_COMBINEDCARDITEMCOLLECTIONWND:
    window = this->combinedCardItemCollectionWnd;
    if (window == nullptr) {
      window = new UICombinedCardItemCollectionWnd();
      this->combinedCardItemCollectionWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x7F4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMCOLLECTIONWND:
    window = this->itemCollectionWnd;
    if (window == nullptr) {
      window = new UIItemCollectionWnd();
      this->itemCollectionWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x7E4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ITEMCOLLECTION_COMPARISONWND:
    window = this->itemCollectionComparisonWnd;
    if (window == nullptr) {
      window = new UIItemCollection_ComparisonWnd();
      this->itemCollectionComparisonWnd = window;
      window->SetSize(280, 120);
      this->AddRendered(window);
    }
    window->SendMsg(nullptr, UM_SETINFO, 0, 0, 0, 0);
    window->Show(1);
    break;
  case WID_COMBINEDCARDITEMCOLLECTION_COMPARISONWND:
    window = this->combinedCardItemCollectionComparisonWnd;
    if (window == nullptr) {
      window = new UICombinedCardItemCollection_ComparisonWnd();
      this->combinedCardItemCollectionComparisonWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x7F4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_NOTICECONFIRMWND:
    window = new UINoticeConfirmWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case WID_DISCONNECTEDSERVERMSGWND:
    window = new UIDisconnectedServerMsgWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
    this->AddRendered(window);
    break;
  case WID_MONSTERINFOWND:
    window = new UIMonsterInfoWnd();
    window->SetSize(400, 200);
    window->Move(0, 0);
    this->AddRendered(window);
    break;
  case WID_MAKETARGETLISTWND:
    window = this->makeTargetListWnd;
    if (window == nullptr) {
      window = new UIMakeTargetListWnd();
      this->makeTargetListWnd = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ITEMREPAIRWND:
    window = this->itemRepairWnd;
    if (window == nullptr) {
      window = new UIItemRepairWnd();
      this->itemRepairWnd = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_WEAPONREFINEWND:
    window = this->weaponRefineWnd;
    if (window == nullptr) {
      window = new UIWeaponRefineWnd();
      this->weaponRefineWnd = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MAKETARGETPROCESSWND:
    window = this->makeTargetProcessWnd;
    if (window == nullptr) {
      window = new UIMakeTargetProcessWnd();
      this->makeTargetProcessWnd = window;
      window->SetSize(280, 150);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MAKETARGETRESULTWND:
    window = this->makeTargetResultWnd;
    if (window == nullptr) {
      window = new UIMakeTargetResultWnd();
      this->makeTargetResultWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_TALKBOXTRAPINPUTWND:
    window = new UITalkboxTrapInputWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
    this->AddRendered(window);
    break;
  case WID_KEYSTROKEWND:
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
  case WID_EMOTIONWND:
    window = this->emotionWnd;
    if (window == nullptr) {
      window = new UIEmotionWnd();
      this->emotionWnd = window;
      window->SetSize(375, 260);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x89C), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_CASHEMOTIONLISTWND:
    window = this->cashEmotionListWnd;
    if (window == nullptr) {
      window = new (std::nothrow) UICashEmotionListWnd();
      this->cashEmotionListWnd = window;
      if (window == nullptr) {
        return nullptr;
      }
      window->SetSize(298, 380);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x8AC), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_PETINFOWND:
    if (g_session.petJob == -1) {
      break;
    }
    window = this->window_038C;
    if (window == nullptr) {
      window = new UIPetInfoWnd();
      this->window_038C = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x8BC), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_PETEGGLISTWND:
    window = this->petEggListWnd;
    if (window == nullptr) {
      window = new UIPetEggListWnd();
      this->petEggListWnd = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_PETTAMINGDECEIVEWND:
    window = this->petTamingDeceiveWnd;
    if (window == nullptr) {
      window = new UIPetTamingDeceiveWnd();
      this->petTamingDeceiveWnd = window;
      window->SetSize(196, 256);
      window->Move(g_renderer->viewport.width / 2 - 98, g_renderer->viewport.height / 2 - 128);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MAKINGARROWLISTWND:
    window = this->makingArrowListWnd;
    if (window == nullptr) {
      window = new UIMakingArrowListWnd();
      this->makingArrowListWnd = window;
      window->SetSize(200, 200);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SELECTCARTWND:
    window = this->selectCartWnd;
    if (window == nullptr) {
      window = new UISelectCartWnd();
      this->selectCartWnd = window;
      window->SetSize(385, 98);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SPELLLISTWND:
    if (this->spellListWnd) {
      this->Schedule(this->spellListWnd);
      this->spellListWnd = nullptr;
    }
    window = new UISpellListWnd();
    this->spellListWnd = window;
    window->SetSize(200, 200);
    window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
    this->AddRendered(window);
    window->Show(1);
    break;
  case WID_GRAFFISTRBOXWND:
    window = new UIGraffiStrboxWnd();
    window->SetSize(280, 120);
    window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300) - 60);
    this->AddRendered(window);
    break;
  case WID_PROHIBITLISTWND:
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
  case WID_BOOKWND:
    window = this->bookWnd;
    if (window == nullptr) {
      window = new UIBookWnd();
      this->bookWnd = window;
      window->SetSize(600, 420);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_JOINFRIENDACCEPTWND:
    window = this->joinFriendAcceptWnd;
    if (window == nullptr) {
      window = new UIJoinFriendAcceptWnd();
      this->joinFriendAcceptWnd = window;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_HOMUNINFOWND:
    if (g_session.m_homunJob == -1) {
      break;
    }
    window = this->window_03C0;
    if (window == nullptr) {
      window = new UIHomunInfoWnd();
      this->window_03C0 = window;
      window->SetSize(280, 208);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x8CC), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SKILLLISTWND_SKILLTAB_0:
    window = this->window_03C4;
    if (window == nullptr) {
      window = new UISkillListWnd();
      this->window_03C4 = window;
      static_cast<UISkillListWnd*>(window)->skillTab = 0;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_03C4->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x8DC), 0, 0, 0);
    }
    static_cast<UISkillListWnd*>(this->window_03C4)->skillTab = 0;
    this->window_03C4->Show(1);
    this->homunculusSkillWndVisible = 1;
    break;
  case WID_MERINFOWND:
    if (g_session.m_mercenaryClass == -1) {
      break;
    }
    window = this->window_03C8;
    if (window == nullptr) {
      window = new UIMerInfoWnd();
      this->window_03C8 = window;
      window->SetSize(280, 180);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(400) - 60);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x8F0), 0, 0, 0);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SELECTREPLAYDATAWND:
    window = new UISelectReplayDataWnd();
    window->SetSize(280, 311);
    window->Move(CenterOnViewportX(320) - 140, CenterOnViewportY(240) - 160);
    this->AddRendered(window);
    break;
  case WID_REPLAYCONTROLWND:
    if (this->window_4F168 == nullptr) {
      window = new UIReplayControlWnd();
      this->window_4F168 = window;
      window->SetSize(197, 138);
      window->Move(ScaleToViewportWidth(640) - 197, ScaleByScreenHeight(480) - 138);
      window->renderLayer = 0;
      this->rendered.sort(layerGreater);
      this->AddRendered(window);
    }
    break;
  case WID_REPLAYRECCONTROLWND:
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
      this->window_4F164->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->navigationPlacement), 0, 0, 0);
    }
    break;
  case WID_NAVIGATIONV4WND:
    if (this->window_4F174 == nullptr) {
      this->RecordWindowPacket(203);
      g_navigationManager.hideWindow = false;
      window = new UINavigationV4Wnd();
      this->window_4F174 = window;
      window->SetSize(272, 338);
      this->window_4F174->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x4F178), 0, 0, 0);
      this->AddRendered(window);
    }
    break;
  case WID_NAVIGATIONHELPWND:
    if (this->LookupWindow(WID_NAVIGATIONHELPWND) == nullptr) {
      window = new UINavigationHelpWnd();
      window->SetSize(310, 200);
      window->Move(100, 100);
      this->AddRendered(window);
    }
    break;
  case WID_NAVIGATIONROADICONWND:
    if (this->LookupWindow(WID_NAVIGATIONROADICONWND) == nullptr) {
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
  case WID_NAVIGATIONRUIDEWND:
    if (this->LookupWindow(WID_NAVIGATIONRUIDEWND) == nullptr) {
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
  case WID_SKILLLISTWND_SKILLTAB_1:
    window = this->window_03CC;
    if (window == nullptr) {
      window = new UISkillListWnd();
      this->window_03CC = window;
      static_cast<UISkillListWnd*>(window)->skillTab = 1;
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200));
      this->AddRendered(window);
      this->window_03CC->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x900), 0, 0, 0);
    }
    static_cast<UISkillListWnd*>(this->window_03CC)->skillTab = 1;
    this->window_03CC->Show(1);
    this->mercenarySkillWndVisible = 1;
    break;
  case WID_AUTOMESSAGEWND:
    window = this->autoMessageWnd;
    if (window == nullptr) {
      window = new UIAutoMessageWnd();
      this->autoMessageWnd = window;
      window->SetSize(280, 48);
      window->Move(CenterOnViewportX(185), CenterOnViewportY(180));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SELCHARFORUSERVERWND: {
    const int width = g_session.characterCount > 12 ? 783 : 540;
    window = new UISelCharForUServerWnd();
    this->window_03D8 = window;
    window->SetSize(width, 400);
    window->Move((g_renderer->viewport.width - width) / 2, (g_renderer->viewport.height - 400) / 2);
    this->AddRendered(window);
    break;
  }
  case WID_CHANGENAMEWND:
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
  case WID_REBIRTHWND:
    window = this->rebirthWnd;
    if (window == nullptr) {
      window = new UIRebirthWnd();
      this->rebirthWnd = window;
      window->SetSize(280, 143);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SUBCHATHISWND_UNKNOWN_03E4:
    window = reinterpret_cast<UIWindow*>(this->subChatHistoryWnd);
    if (window == nullptr) {
      window = new UISubChatHisWnd();
      this->subChatHistoryWnd = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(300, 95);
      this->AddRendered(window);
      reinterpret_cast<UIWindow*>(this->subChatHistoryWnd)->SetId(130);
      reinterpret_cast<UIWindow*>(this->subChatHistoryWnd)->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x914), 0, 0, 0);
    }
    window->Show(0);
    break;
  case WID_SUBCHATHISWND_UNKNOWN_03E8:
    window = reinterpret_cast<UIWindow*>(this->secondSubChatHistoryWnd);
    if (window == nullptr) {
      window = new UISubChatHisWnd();
      this->secondSubChatHistoryWnd = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(300, 95);
      this->AddRendered(window);
      reinterpret_cast<UIWindow*>(this->secondSubChatHistoryWnd)->SetId(131);
      reinterpret_cast<UIWindow*>(this->secondSubChatHistoryWnd)->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x924), 0, 0, 0);
    }
    window->Show(0);
    break;
  case WID_BATTLEMSGOPTIONWND:
    window = this->battleMsgOptionWnd;
    if (window == nullptr) {
      window = new UIBattleMsgOptionWnd();
      this->battleMsgOptionWnd = window;
      window->SetSize(280, 200);
      window->Move(CenterOnViewportX(400), ScaleByScreenHeight(200));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SUBCHATMINIWND:
    window = reinterpret_cast<UIWindow*>(this->subChatMiniWnd);
    if (window == nullptr) {
      window = new UISubChatMiniWnd();
      this->subChatMiniWnd = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(5, 5);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MEMORIALDUNWND:
    if (g_session.memorialDungeonName == "") {
      return nullptr;
    }
    window = this->memorialDunWnd;
    if (window == nullptr) {
      window = new UIMemorialDunWnd();
      this->memorialDunWnd = window;
      window->SetSize(200, 150);
      if (g_session.memorialWindowX >= 0 && g_session.memorialWindowY >= 0) {
        window->Move(g_session.memorialWindowX, g_session.memorialWindowY);
      } else {
        window->Move(0, 310);
      }
      this->AddRendered(window);
    }
    window->Show(1);
    g_session.memorialWindowOpen = 1;
    break;
  case WID_EQUIPWND_VIEWOTHER_1:
    window = this->otherEquipWnd;
    if (window == nullptr) {
      window = new UIEquipWnd();
      this->otherEquipWnd = window;
      static_cast<UIEquipWnd*>(window)->m_viewOther = 1;
      window->SetSize(280, 167);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    break;
  case WID_ROMAPWND:
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
  case WID_ESCOPTIONWND:
    if (this->window_4F1EC || this->window_4F288) {
      break;
    }
    window = this->escOptionWnd;
    if (window == nullptr) {
      window = new UIEscOptionWnd();
      this->escOptionWnd = window;
      window->SetSize(280, 143);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(300));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_HOTKEYWND:
  case WID_RELOADLUASCRIPTWND:
  case WID_QUIKSLOTWND:
  case WID_BATTLEFIELD_DESCRIBEWND:
  case WID_BATTLEFIELD_STATUSWND:
  case WID_BATTLEFIELD_HELPWND:
  case WID_SEARCH_STORE_INFO:
  case WID_SHOWINFO_TESTLAYER:
  case WID_EXMACRO_SETUPWND:
    window = this->CreateLayoutWindow(id);
    if (window == nullptr) {
      break;
    }
    window->Show(1);
    break;
  case WID_JOINTO_BATTLEFIELDWND:
    window = this->LookupWindow(WID_JOINTO_BATTLEFIELDWND);
    if (window == nullptr) {
      window = new UIEntryQueueWnd();
      window->SetSize(350, 350);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->battlefieldPlacement), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_ENTRYQUEUESTANDBYWND:
    window = this->LookupWindow(WID_ENTRYQUEUESTANDBYWND);
    if (window == nullptr) {
      UIEntryQueueStandByWnd* standBy = new UIEntryQueueStandByWnd();
      window = standBy;
      standBy->Create();
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ENTRYQUEUEREQUESTWND:
    window = this->LookupWindow(WID_ENTRYQUEUEREQUESTWND);
    if (window == nullptr) {
      UIEntryQueueRequestWnd* request = new UIEntryQueueRequestWnd();
      window = request;
      request->Create();
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ENTRYQUEUEHELPWND:
    window = this->LookupWindow(WID_ENTRYQUEUEHELPWND);
    if (window == nullptr) {
      window = new UIEntryQueueHelpWnd();
      window->SetSize(100, 100);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_MERCHANTSHOPMAKEWND_MODE_1:
    window = this->buyingShopMakeWnd;
    if (window == nullptr) {
      window = new UIMerchantShopMakeWnd(1);
      this->buyingShopMakeWnd = window;
      window->SetSize(360, 289);
      window->Move(CenterOnViewportX(185), ScaleByScreenHeight(200));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x954), 0, 0, 0);
    }
    window->Show(1);
    this->EnsureWindow(WID_MERCHANTMIRRORITEMWND_MODE_1);
    break;
  case WID_MERCHANTMIRRORITEMWND_MODE_1:
    window = reinterpret_cast<UIWindow*>(this->buyingShopMirrorItemWnd);
    if (window == nullptr) {
      window = new UIMerchantMirrorItemWnd(1);
      this->buyingShopMirrorItemWnd = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(0), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x964), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MERCHANTITEMMYSHOPWND_MODE_1:
    window = this->buyingShopMyItemsWnd;
    if (window == nullptr) {
      window = new UIMerchantItemMyShopWnd(1);
      this->buyingShopMyItemsWnd = window;
      window->SetSize(280, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x974), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MERCHANTITEMSHOPWND_MODE_1:
    window = this->buyingShopItemsWnd;
    if (window == nullptr) {
      window = new UIMerchantItemShopWnd(1);
      this->buyingShopItemsWnd = window;
      window->SetSize(350, 120);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x984), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MERCHANTITEMPURCHASEWND_MODE_1:
    window = this->buyingShopPurchaseWnd;
    if (window == nullptr) {
      window = new UIMerchantItemPurchaseWnd(1);
      this->buyingShopPurchaseWnd = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x994), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_MERCHANTMIRRORITEMWND_MODE_2:
    window = reinterpret_cast<UIWindow*>(this->purchaseMirrorItemWnd);
    if (window == nullptr) {
      window = new UIMerchantMirrorItemWnd(2);
      this->purchaseMirrorItemWnd = reinterpret_cast<std::uint32_t>(window);
      window->SetSize(280, 120);
      window->Move(300, 0);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x9A4), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_COLLECTIONSYSTEMWND:
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
  case WID_SECONDPASSWORDINPUTWND: {
    const int rangeX = g_renderer->viewport.width - 540;
    const int minX = rangeX / 2;
    const int rangeY = g_renderer->viewport.height - 365;
    const int minY = rangeY / 2;
    window = this->secondPasswordInputWnd;
    if (window == nullptr) {
      window = new UISecondPasswordInputWnd();
      this->secondPasswordInputWnd = window;
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
    static_cast<UISecondPasswordInputWnd*>(this->secondPasswordInputWnd)->CreateKeypad(g_session.m_loginAccountId);
    static_cast<UISecondPasswordInputWnd*>(this->secondPasswordInputWnd)->SetInputMode(0);
    break;
  }
  case WID_SECONDPASSWORDEDITWND: {
    const int rangeX = g_renderer->viewport.width - 540;
    const int minX = rangeX / 2;
    const int rangeY = g_renderer->viewport.height - 365;
    const int minY = rangeY / 2;
    window = this->secondPasswordEditWnd;
    if (window == nullptr) {
      window = new UISecondPasswordEditWnd();
      this->secondPasswordEditWnd = window;
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
    static_cast<UISecondPasswordInputWnd*>(this->secondPasswordEditWnd)->CreateKeypad(g_session.m_loginAccountId);
    static_cast<UISecondPasswordInputWnd*>(this->secondPasswordEditWnd)->SetInputMode(1);
    break;
  }
  case WID_MERGEITEMWND:
    window = this->LookupWindow(WID_MERGEITEMWND);
    if (window == nullptr) {
      window = new CMergeItemWnd();
      window->SetSize(1, 1);
      window->Show(1);
      window->Move(static_cast<int>(g_renderer->viewport.width * 0.5), static_cast<int>(g_renderer->viewport.height * 0.5));
      this->AddRendered(window);
    }
    break;
  case WID_MERCHANTITEMPURCHASEMSGWND:
    if (this->LookupWindow(WID_MERCHANTITEMPURCHASEMSGWND) == nullptr) {
      this->CloseWindow(WID_MERCHANTITEMPURCHASEMSGWND);
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
  case WID_CLANINFOMANAGEWND:
    window = this->window_0444;
    if (window == nullptr) {
      window = new UIClanInfoManageWnd();
      this->window_0444 = window;
      window->SetSize(400, 317);
      window->Move(ScaleToViewportWidth(0), 147);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x9F0), 0, 0, 0);
    }
    window->Show(1);
    break;
  case WID_BANK_NEWWND:
    if (this->LookupWindow(WID_REFININGWND)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NEWMAKECHAR_HUMANJOB)), 255, 0, 0, 0);
      break;
    }
    if (this->LookupWindow(WID_GRADEENCHANTWND)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_GRADE_ENCHANT_ITEM)), 255, 0, 0, 0);
      break;
    }
    if (this->LookupWindow(WID_ENCHANTUI)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFORM_WARNNING_NOT_ENOUGH_MATERIAL)), 255, 0, 0, 0);
      break;
    }
    if (this->LookupWindow(WID_EXPANDEDBARTERMARKETWND)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PACKAGEITEM_SLEEPSTONECURSEFAIL)), 255, 0, 0, 0);
      break;
    }
    if (g_windowMgr.LookupWindow(WID_RUNESYSTEMWND)) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTIFY_JOIN_GUILD_ACCEPT_WND)), 255, 0, 0, 0);
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
  case WID_GUILD_STORAGE_LOG:
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
  case WID_MERCHANTITEMLOGWND_MODE_0:
  case WID_MERCHANTITEMLOGWND_MODE_1:
    window = this->LookupWindow(id);
    if (window == nullptr) {
      if (id == WID_MERCHANTITEMLOGWND_MODE_1) {
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
  case WID_OPENMAILBOXWND:
    window = this->openMailBoxWnd;
    if (window == nullptr) {
      window = new UIOpenMailBoxWnd();
      this->openMailBoxWnd = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 234, 16);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_RODEXINBOXWND:
    window = this->rodexInboxWnd;
    if (window == nullptr) {
      window = new UIRodexInboxWnd();
      this->rodexInboxWnd = window;
      window->SetSize(309, 416);
      window->Move(CenterOnViewportX(100), ScaleByScreenHeight(100));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_RODEXCOMPOSEWND:
    window = this->rodexComposeWnd;
    if (window == nullptr) {
      window = new UIRodexComposeWnd();
      this->rodexComposeWnd = window;
      window->SetSize(300, 400);
      window->Move(CenterOnViewportX(120), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_RODEXCONTENTSWND:
    window = this->rodexContentsWnd;
    if (window == nullptr) {
      window = new UIRodexContentsWnd();
      this->rodexContentsWnd = window;
      window->SetSize(300, 400);
      window->Move(CenterOnViewportX(120), ScaleByScreenHeight(120));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_PETEVOLUTIONWND:
    window = this->LookupWindow(WID_PETEVOLUTIONWND);
    if (window == nullptr) {
      window = new UIPetEvolutionWnd();
      window->SetSize(290, 400);
      window->Move(static_cast<int>((g_renderer->viewport.width - 290) * 0.5), static_cast<int>((g_renderer->viewport.height - 400) * 0.5));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_SECONDCOSTUMEWND:
    window = dynamic_cast<UIFrameWnd*>(this->LookupWindow(WID_SECONDCOSTUMEWND));
    if (window == nullptr) {
      window = new UISecondCostumeWnd();
      this->window_4F170 = window;
      window->SetSize(395, 205);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(200) + 46);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case WID_MILEAGEWND:
    window = this->LookupWindow(WID_MILEAGEWND);
    if (window == nullptr) {
      window = new UIMileageWnd();
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\mileage_bg1.bmp")));
      if (bitmap == nullptr) {
        window->SetSize(150, 70);
        window->Move(g_renderer->viewport.width - 480, 12);
      } else {
        window->SetSize(bitmap->image.width, bitmap->image.height);
        if (this->nCashCallWnd) {
          window->Move(g_renderer->viewport.width - 292, 11);
        } else {
          window->Move(g_renderer->viewport.width - bitmap->image.width - 340, 12);
        }
      }
      this->AddRendered(window);
    }
    window->Show(g_serviceOptions.Get(219));
    break;
  case WID_OPENROULLETTEWND:
    window = this->openRoulletteWnd;
    if (window == nullptr) {
      window = new UIOpenRoulletteWnd();
      this->openRoulletteWnd = window;
      window->SetSize(43, 43);
      window->Move(g_renderer->viewport.width - 187, 71);
      this->AddRendered(window);
    }
    window->Show(g_serviceOptions.Get(220));
    break;
  case WID_ROULLETTEWND:
    window = this->roulletteWnd;
    if (window == nullptr) {
      UIRoulletteWnd* roulette = new UIRoulletteWnd();
      window = roulette;
      this->roulletteWnd = roulette;
      window->SetSize(640, 497);
      window->Move(static_cast<int>(g_renderer->viewport.width * 0.5 - 320.0), static_cast<int>(g_renderer->viewport.height * 0.5 - 240.0));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_ACHIEVEMENTWND:
    window = dynamic_cast<UIFrameWnd*>(this->LookupWindow(WID_ACHIEVEMENTWND));
    if (window == nullptr) {
      window = new UIAchievementWnd();
      window->SetSize(750, 468);
      window->Move((g_renderer->viewport.width - 750) / 2, (g_renderer->viewport.height - 468) / 2);
      window->Show(1);
      this->AddRendered(window);
    }
    if (UIWindow* basicInfo = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
      basicInfo->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 0x1D9, 0, 0);
    }
    break;
  case WID_ACHTRACINGWND:
    window = dynamic_cast<UIFrameWnd*>(this->LookupWindow(WID_ACHTRACINGWND));
    if (window == nullptr) {
      window = new UIAchTracingWnd();
      window->SetSize(300, 150);
      window->Move(g_renderer->viewport.width - 350, (g_renderer->viewport.height - 150) / 2);
      window->Show(1);
      window->renderLayer = 5;
      this->AddRendered(window);
      this->rendered.sort(layerGreater);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x4F1D8), 0, 0, 0);
    }
    break;
  case WID_MINIMAPWND:
    if (this->window_4F1CC == nullptr) {
      window = new UIMiniMapWnd();
      this->window_4F1CC = window;
      MiniMapWndInfo* miniMapInfo = reinterpret_cast<MiniMapWndInfo*>(base + 0x4F1A8);
      window->SetSize(miniMapInfo->expandedWidth, miniMapInfo->expandedHeight);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(miniMapInfo), 0, 0, 0);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case WID_EDITCTRLWND:
    if (this->window_4F1D0 == nullptr) {
      window = new UIEditctrlWnd();
      this->window_4F1D0 = window;
      window->SetSize(250, 42);
      window->Move(g_mouse.x, g_mouse.y);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case WID_ROUNDBOXTOOLTIPWND:
    if (this->LookupWindow(WID_ROUNDBOXTOOLTIPWND) == nullptr) {
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
  case WID_STYLINGSHOPWND:
    window = this->LookupWindow(WID_STYLINGSHOPWND);
    if (window == nullptr) {
      window = new UIStylingShopWnd();
      window->SetSize(480, 330);
      window->Move((g_renderer->viewport.width - 480) / 2, (g_renderer->viewport.height - 330) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_CAPTCHAREGISTERWND:
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
  case WID_MACRODETECTORWND:
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
  case WID_MACROREPORTERWND:
    window = this->window_4F1F0;
    if (window == nullptr) {
      window = new UIMacroReporterWnd();
      this->window_4F1F0 = window;
      window->SetSize(249, 308);
      window->Move(g_renderer->viewport.width - 299, (g_renderer->viewport.height - 308) / 2);
      this->AddRendered(window);
    }
    window->Show(1);
    window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x4F200), 0, 0, 0);
    this->macroReporterVisible = true;
    break;
  case WID_MACROUSERINFOWND:
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
  case WID_CAPTCHAPREVIEWWND:
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
  case WID_VIEWCAMERAINFOWND:
    window = this->LookupWindow(WID_VIEWCAMERAINFOWND);
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
      window->renderLayer = 1;
      this->AddRendered(window);
      this->rendered.sort(layerGreater);
    }
    window->Show(g_session.showCameraInfo);
    break;
  case WID_LAPINEBOXWND:
    if (this->LookupWindow(WID_LAPINEBOXWND) == nullptr) {
      window = new UILapineBoxWnd();
      window->SetSize(400, 250);
      window->Move((g_renderer->viewport.width - 400) / 2, (g_renderer->viewport.height - 250) / 2);
      this->AddRendered(window);
    }
    break;
  case WID_TRANSMENUWND:
    window = this->LookupWindow(WID_TRANSMENUWND);
    if (window == nullptr) {
      window = new UITransMenuWnd();
      window->SetSize(30, 40);
      this->AddRendered(window);
    } else {
      window->Show(1);
      this->Promote(window);
    }
    break;
  case WID_PREVIEWEQUIPWND:
    if (this->LookupWindow(WID_PREVIEWEQUIPWND) == nullptr) {
      window = new UIPreviewEquipWnd();
      window->SetSize(280, 260);
      window->Move(static_cast<int>((g_renderer->viewport.width - 280) * g_depthAverageHalf), static_cast<int>((g_renderer->viewport.height - 260) * g_depthAverageHalf));
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(this->previewEquipPlacement), 0, 0, 0);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_PREVIEWEQUIPWND);
    }
    break;
  case WID_REFININGWND: {
    const int x = static_cast<int>((g_renderer->viewport.width - 264) * g_depthAverageHalf);
    const int y = static_cast<int>((g_renderer->viewport.height - 346) * g_depthAverageHalf);
    this->EnsureWindow(WID_ITEMWND);
    if (this->itemWnd) {
      this->itemWnd->Move(x + 264, y - this->itemWnd->geometry.height + 346);
    }
    if (this->rodexInboxWnd) {
      this->CloseWindow(WID_RODEXCOMPOSEWND);
      this->CloseWindow(WID_RODEXCONTENTSWND);
      this->rodexInboxWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_AURA_EFFECT_ON)), 255, 0, 0, 0);
    }
    if (this->window_0448) {
      g_windowMgr.CloseWindow(WID_BANK_NEWWND);
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NEWMAKECHAR_HUMAN)), 255, 0, 0, 0);
    }
    if (this->LookupWindow(WID_REFININGWND) == nullptr) {
      window = new UIRefiningWnd();
      window->SetSize(264, 346);
      window->Move(x, y);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_REFININGWND);
    }
    break;
  }
  case WID_LAPINEUPGRADEBOXWND:
    if (this->LookupWindow(WID_LAPINEUPGRADEBOXWND) == nullptr) {
      window = new UILapineUpgradeBoxWnd();
      window->SetSize(400, 250);
      window->Move((g_renderer->viewport.width - 400) / 2, (g_renderer->viewport.height - 250) / 2);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_LAPINEUPGRADEBOXWND);
    }
    break;
  case WID_HOTKEYGUIDEWND:
    if (this->LookupWindow(WID_HOTKEYGUIDEWND) == nullptr) {
      window = new UIHotkeyGuideWnd();
      window->SetSize(934, 498);
      window->Move((g_renderer->viewport.width - 934) / 2, (g_renderer->viewport.height - 498) / 2);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_HOTKEYGUIDEWND);
    }
    break;
  case WID_VIDEOWND:
    window = this->LookupWindow(WID_VIDEOWND);
    if (window == nullptr) {
      window = new UIVideoWnd();
      window->SetSize(g_renderer->viewport.width, g_renderer->viewport.height);
      window->Move(0, 0);
      this->AddRendered(window);
    }
    break;
  case WID_MENUICONWND:
    if (this->LookupWindow(WID_MENUICONWND) == nullptr) {
      window = new UIMenuIconWnd();
      window->SetSize(220, 197);
      window->Move(0, 225);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_MENUICONWND);
    }
    break;
  case WID_SHOWWARPWND:
    if (this->LookupWindow(WID_SHOWWARPWND) == nullptr) {
      window = new UIShowWarpWnd();
      window->SetSize(280, 120);
      window->Move(CenterOnViewportX(200), ScaleByScreenHeight(300));
      this->AddRendered(window);
      window->Show(1);
    } else {
      window = this->LookupWindow(WID_SHOWWARPWND);
      window->Show(1);
    }
    break;
  case WID_TIPBOXWND:
    if (this->LookupWindow(WID_TIPBOXWND) == nullptr) {
      window = new UITipboxWnd();
      this->window_0468 = window;
      window->SetSize(480, 365);
      window->Move(CenterOnViewportX(120), ScaleByScreenHeight(120));
      this->AddRendered(window);
      window->Show(1);
    } else {
      window = this->LookupWindow(WID_TIPBOXWND);
      window->Show(1);
    }
    break;
  case WID_CASHSHOP_WND:
    if (this->LookupWindow(WID_CASHSHOP_WND) == nullptr) {
      window = new UICashShopWnd();
      this->window_4F254 = window;
      window->SetSize(723, 540);
      window->Move(static_cast<int>((g_renderer->viewport.width - 723) * 0.5), static_cast<int>((g_renderer->viewport.height - 540) * 0.5));
      window->SendMsg(nullptr, UM_SETINFO, 0, 0, 0, 0);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_CASHSHOP_WND);
    }
    break;
  case WID_RENEWQUESTWND:
    this->RecordWindowPacket(321);
    window = this->LookupWindow(id);
    if (window == nullptr) {
      window = new UIRenewQuestWnd();
      window->SetSize(381, 466);
      window->Move((g_renderer->viewport.width - 381) / 2, (g_renderer->viewport.height - 466) / 2);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x550), 0, 0, 0);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    this->CloseWindow(WID_NOTIFYQUESTWND);
    this->questWndVisible = 1;
    break;
  case WID_RENEWDETAILQUESTINFOWND:
    this->RecordWindowPacket(321);
    window = this->LookupWindow(id);
    if (window == nullptr) {
      window = new UIRenewDetailQuestInfoWnd();
      window->SetSize(342, 412);
      if (UIWindow* quest = this->LookupWindow(WID_RENEWQUESTWND)) {
        window->Move(quest->geometry.x + quest->geometry.width, quest->geometry.y);
      } else {
        window->Move((g_renderer->viewport.width - 342) / 2, (g_renderer->viewport.height - 412) / 2);
      }
      this->AddRendered(window);
      g_snapMgr.Add(window);
    }
    window->Show(1);
    break;
  case WID_CHECKATTENDANCEWND:
    if (this->LookupWindow(WID_CHECKATTENDANCEWND) == nullptr) {
      window = new UICheckAttendanceWnd();
      window->SetSize(488, 413);
      window->Move(static_cast<int>((g_renderer->viewport.width - 488) * 0.5), static_cast<int>((g_renderer->viewport.height - 413) * 0.5));
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_CHECKATTENDANCEWND);
    }
    break;
  case WID_BARTERMARKETWND:
    if (this->LookupWindow(WID_BARTERMARKETWND) == nullptr) {
      window = new UIBartermarketWnd();
      this->window_4F260 = window;
      window->SetSize(280, 400);
      window->Move(0, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x6FC), 0, 0, 0);
    } else {
      window = this->LookupWindow(WID_BARTERMARKETWND);
    }
    break;
  case WID_BARTERITEMPURCHASEWND:
    if (this->LookupWindow(WID_BARTERITEMPURCHASEWND) == nullptr) {
      window = new UIBarterItemPurchaseWnd();
      this->window_4F264 = window;
      window->SetSize(280, 120);
      window->Move(300, 200);
      this->AddRendered(window);
      window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0x71C), 0, 0, 0);
    } else {
      window = this->LookupWindow(WID_BARTERITEMPURCHASEWND);
    }
    break;
  case WID_EXPANDEDBARTERMARKETWND:
    if (this->LookupWindow(WID_BANK_NEWWND)) {
      g_windowMgr.CloseWindow(WID_BANK_NEWWND);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_USESKILL_FAIL_NEED_COMMUNION_WITH_SPIRIT)), 255, 0, 0, 0);
    }
    if (this->LookupWindow(WID_EXPANDEDBARTERMARKETWND) == nullptr) {
      window = new UIExpandedBartermarketWnd();
      this->window_4F268 = window;
      window->SetSize(366, 366);
      window->Move(200, 200);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_EXPANDEDBARTERMARKETWND);
    }
    break;
  case WID_EXPANDEDBARTERITEMPURCHASEWND:
    if (this->LookupWindow(WID_BANK_NEWWND)) {
      break;
    }
    if (this->LookupWindow(WID_EXPANDEDBARTERITEMPURCHASEWND) == nullptr) {
      window = new UIExpandedBarterItemPurchaseWnd();
      this->window_4F26C = window;
      window->SetSize(366, 366);
      window->Move(586, 200);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_EXPANDEDBARTERMARKETWND);
    }
    break;
  case WID_NOTEWND:
    if (this->LookupWindow(WID_NOTEWND) == nullptr) {
      window = new UINoteWnd();
      window->SetSize(358, 275);
      if (UIWindow* shop = this->merchantShopMakeWnd) {
        window->Move(shop->geometry.width + shop->geometry.x + 5, shop->geometry.y);
      } else {
        window->Move(static_cast<int>((g_renderer->viewport.width - 358) * 0.5), static_cast<int>((g_renderer->viewport.height - 275) * 0.5));
      }
      window->SendMsg(nullptr, UM_SETINFO, 0xC95, 0, 0, 0);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_NOTEWND);
    }
    break;
  case WID_ACCOUNTLIMITEDTOOLWND:
    window = this->LookupWindow(WID_ACCOUNTLIMITEDTOOLWND);
    if (window == nullptr) {
      window = new UIAccountLimitedToolWnd();
      window->SetSize(390, 350);
      window->Move(g_renderer->viewport.centerX - window->geometry.width / 2, g_renderer->viewport.centerY - window->geometry.height / 2);
      window->Show(1);
      this->AddRendered(window);
    }
    break;
  case WID_GRADEENCHANTWND: {
    const int x = static_cast<int>((g_renderer->viewport.width - 400) * g_depthAverageHalf - 100.0f);
    const int y = static_cast<int>((g_renderer->viewport.height - 330) * g_depthAverageHalf);
    this->EnsureWindow(WID_ITEMWND);
    if (this->itemWnd) {
      this->itemWnd->Move(x + 400, y - this->itemWnd->geometry.height + 330);
    }
    if (this->rodexInboxWnd) {
      this->CloseWindow(WID_RODEXCOMPOSEWND);
      this->CloseWindow(WID_RODEXCONTENTSWND);
      this->rodexInboxWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_OPEN_BANKING_DURING_GRADE_ENCHANT)), 255, 0, 0, 0);
    }
    if (this->window_0448) {
      g_windowMgr.CloseWindow(WID_BANK_NEWWND);
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BROADCASTING_GRADE_ENCHANT_FAIL)), 255, 0, 0, 0);
    }
    if (this->LookupWindow(WID_GRADEENCHANTWND) == nullptr) {
      window = new UIGradeEnchantWnd();
      window->SetSize(400, 330);
      window->Move(x, y);
      this->AddRendered(window);
    } else {
      window = this->LookupWindow(WID_GRADEENCHANTWND);
    }
    break;
  }
  case WID_CHANGEMATERIALWND:
    window = this->LookupWindow(WID_CHANGEMATERIALWND);
    if (window == nullptr) {
      window = new UIChangeMaterialWnd();
      window->SetSize(500, 314);
      window->Move(static_cast<int>((g_renderer->viewport.width - 500) * 0.5), static_cast<int>((g_renderer->viewport.height - 314) * 0.5));
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      this->AddRendered(window);
      this->SetFocus(nullptr);
    }
    break;
  case WID_ADVENPARTYBOARDWND:
    window = this->window_4F274;
    if (window == nullptr) {
      window = new UIAdvenPartyBoardWnd();
      this->window_4F274 = window;
      window->SetSize(480, 340);
      window->Move(g_renderer->viewport.centerX - window->geometry.width / 2, g_renderer->viewport.centerY - window->geometry.height / 2);
      this->AddRendered(window);
      this->window_4F274->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0xA20), 0, 0, 0);
    }
    break;
  case WID_REGISTERPARTYWND:
    window = this->window_4F270;
    if (window == nullptr) {
      window = new UIRegisterPartyWnd();
      this->window_4F270 = window;
      window->SetSize(220, 215);
      window->Move(g_renderer->viewport.centerX + 240, g_renderer->viewport.centerY + 170);
      this->AddRendered(window);
      this->window_4F270->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0xA00), 0, 0, 0);
    }
    break;
  case WID_REQUESTJOINPARTYWND:
    window = this->window_4F278;
    if (window == nullptr) {
      window = new UIRequestJoinPartyWnd();
      this->window_4F278 = window;
      window->SetSize(220, 160);
      window->Move(g_renderer->viewport.centerX - window->geometry.width / 2, g_renderer->viewport.centerY - window->geometry.height / 2);
      this->AddRendered(window);
    }
    break;
  case WID_APPLYFORPARTYWND:
    window = this->window_4F27C;
    if (window == nullptr) {
      window = new UIApplyForPartyWnd();
      this->window_4F27C = window;
      window->SetSize(220, 190);
      window->Move(g_renderer->viewport.centerX + 240, g_renderer->viewport.centerY - 170);
      this->AddRendered(window);
      this->window_4F27C->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(base + 0xA10), 0, 0, 0);
    }
    break;
  case WID_REPUTEWND:
    window = this->LookupWindow(WID_REPUTEWND);
    if (window == nullptr) {
      window = new UIReputeWnd();
      window->SetSize(640, 450);
      window->Move(static_cast<int>((g_renderer->viewport.width - 640u) * 0.5), static_cast<int>((g_renderer->viewport.height - 450u) * 0.5));
      this->AddRendered(window);
    }
    break;
  case WID_ITEMREFORMWND:
    window = this->LookupWindow(WID_ITEMREFORMWND);
    if (window == nullptr) {
      window = new UIItemReformWnd();
      window->SetSize(440, 260);
      window->Move(static_cast<int>((g_renderer->viewport.width - 440u) * 0.5), static_cast<int>((g_renderer->viewport.height - 260u) * 0.5));
      this->AddRendered(window);
    }
    break;
  case WID_BALLOONOPENBTN:
    window = this->LookupWindow(WID_BALLOONOPENBTN);
    if (window == nullptr) {
      window = new UIBalloonOpenBtn();
      window->SetSize(18, 18);
      window->Move(g_renderer->viewport.width - 200, static_cast<int>(g_renderer->viewport.height * 0.5));
      this->AddRendered(window);
    }
    break;
  case WID_MACROBLACKLISTCHECKWND:
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
  case WID_RUNESYSTEMWND:
    window = this->LookupWindow(WID_RUNESYSTEMWND);
    if (window == nullptr) {
      g_runeSystemMgr->SendRequest(0xBCB, nullptr, -1);
      if (this->rodexInboxWnd) {
        this->CloseWindow(WID_RODEXCOMPOSEWND);
        this->CloseWindow(WID_RODEXCONTENTSWND);
        this->rodexInboxWnd->SendMsg(nullptr, UM_COMMAND, ID_CLOSE, 0, 0, 0);
        this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTIFY_ALLY_GUILD_ACCEPT_WND)), 255, 0, 0, 0);
      }
      if (this->window_0448) {
        g_windowMgr.CloseWindow(WID_BANK_NEWWND);
        this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTIFY_JOIN_FRIEND_ACCEPT_WND)), 255, 0, 0, 0);
      }
      window = new UIRuneSystemWnd();
      window->SetSize(910, 539);
      window->Move(static_cast<int>((g_renderer->viewport.width - window->geometry.width) * 0.5), static_cast<int>((g_renderer->viewport.height - window->geometry.height) * 0.5));
      this->AddRendered(window);
    }
    window->Show(1);
    break;
  case WID_RUNESYSTEM_DECOMRESULTWND:
    window = this->LookupWindow(WID_RUNESYSTEM_DECOMRESULTWND);
    if (window == nullptr) {
      window = new UIRuneSystem_DecomResultWnd();
      window->SetSize(240, 180);
      if (UIWindow* runeSystem = this->LookupWindow(WID_RUNESYSTEMWND)) {
        window->Move(runeSystem->geometry.x + 478, runeSystem->geometry.y + 129);
      }
      this->AddRendered(window);
    }
    window->Show(1);
    this->Promote(window);
    break;
  case WID_DEBUFFREMOVEWND:
    if (this->LookupWindow(WID_DEBUFFREMOVEWND) == nullptr) {
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
    case WID_JOINGUILDACCEPTWND:
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTIFY_JOIN_PARTY_ACCEPT_WND)), 0xFFFF00, 0, 0, 0);
      break;
    case WID_ALLYGUILDACCEPTWND:
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NOTIFY_EXCHANGE_ACCEPT_WND)), 0xFFFF00, 0, 0, 0);
      break;
    case WID_JOINFRIENDACCEPTWND:
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_WARNING)), 0xFFFF00, 0, 0, 0);
      break;
    case WID_JOINPARTYACCEPTWND:
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_FAILED_ITEMCREATE)), 0xFFFF00, 0, 0, 0);
      break;
    case WID_EXCHANGEACCEPTWND:
      this->SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_TITLE_QUEST_BOARD_LIST)), 0xFFFF00, 0, 0, 0);
      break;
    }
  }
  return window;
}
