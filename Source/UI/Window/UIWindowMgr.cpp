#include "Render/Renderer.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "Client/Browser/WebBrowser.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
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
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UINavigationWnd.h"
#include "UI/Window/UIRoMapWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "Client/Mode/CLoginMode.h"
#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
#include "Contents/WorldView/WorldViewData.h"
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
#include "Contents/Messenger/ClientMessenger.h"
#include "UI/Text/TextMarkup.h"
namespace {
#pragma pack(push, 1)
struct AddFriendPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
};
#pragma pack(pop)

static_assert(sizeof(AddFriendPacket) == 26);
static_assert(offsetof(AddFriendPacket, name) == 2);
}

struct SessionCommandTarget {
  int sub_DE1010(const char* name, int* optionId, std::string* strings);
};

// GLOBAL: gl_24525
CMiniPartyWndAreaMgr* g_miniPartyWndAreaMgr = nullptr;

// GLOBAL: gl_24366
UIWindowMgr g_windowMgr;

// GLOBAL: gl_25090
unsigned char g_windowCloseByte;

// GLOBAL: gl_25159
bool g_windowToggleFlag;

// GLOBAL: gl_24686
int g_closeWindow137State;

// GLOBAL: gl_24687
int g_closeWindow164State;

// GLOBAL: gl_24683
int g_windowCloseValue;

// GLOBAL: gl_24684
int g_savedWindowCloseValue;

// OFFSET: 0x00A94FD0
// FUNCTION: fn_17179
void UIWindowMgr::InitializeWindowPolicy() {
  for (WindowPolicyEntry& entry : this->policy) {
    entry = {};
    entry.actions[3] = 1;
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
  case 1:
    return this->window_01C8;
  case 3:
    return this->window_01CC;
  case 27:
    return this->window_0274;
  case 28:
    return this->window_027C;
  case 30:
    return this->window_0278;
  case 33:
    return this->window_0288;
  case 34:
    return this->window_02C8;
  case 37:
    return this->window_02C4;
  case 60:
    return this->window_0320;
  case 61:
    return this->window_0324;
  case 63:
    return this->window_032C;
  case 88:
    return this->window_038C;
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
  case 180:
    return this->window_01D0;
  case 196:
    return this->window_4F18C;
  case 203:
    return this->window_4F174;
  case 204:
    return this->window_0428;
  case 237:
    return this->window_0444;
  case 253:
    return this->window_044C;
  case 273:
    return this->window_4F1CC;
  case 274:
    return this->window_4F1D0;
  case 275:
    return this->window_0448;
  case 277:
    return this->window_03D4;
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
  case 315:
    return this->window_0468;
  case 325:
    return this->window_4F25C;
  case 359:
    return this->window_4F288;
  case 10024:
    return this->window_4F2AC;
  case 301:
    if (!this->snapExcluded.empty()) {
      return this->snapExcluded.front();
    }
    [[fallthrough]];
  case 318:
    return this->window_4F254;
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
  this->unknown_0024 = 0;
  this->unknown_0026 = 1;
  this->unknown_0508 = 0;
  this->window_4F168 = nullptr;
  this->window_4F164 = nullptr;
  this->window_4F16C = nullptr;
  this->InitializeWindowPolicy();
  this->window_4F174 = nullptr;
  this->unknown_09CC = 0;
  this->browser_09D0 = nullptr;
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
CMiniPartyWndAreaMgr::CMiniPartyWndAreaMgr() {
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
    if (!this->modalWindows.empty() && this->modalWindows.back() && this->persistentWindows.find(window) != this->persistentWindows.end()) {
      this->retainedPersistentWindow = true;
    } else if (this->modalWindows.empty() || !this->modalWindows.back() ||
               std::find(this->modalWindows.begin(), this->modalWindows.end(), window) == this->modalWindows.end()) {
      this->persistentWindows.erase(window);
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
  if (this->browser_09D0) {
    delete this->browser_09D0;
    this->browser_09D0 = nullptr;
  }

  for (auto entry = this->overlays.begin(); entry != this->overlays.end();) {
    delete entry->second;
    entry = this->overlays.erase(entry);
  }
  this->rendered.clear();
  this->retired.clear();
  this->snapRectangles.clear();
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
void UIWindowMgr::sub_AA6B80() {
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

    if (window == reinterpret_cast<UIWindow*>(this->unknown_03F8) && g_closeWindow137State != 0) {
      g_memorialDunWndX = window->geometry.x;
      g_memorialDunWndY = window->geometry.y;
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
    std::map<int, int>& entries = CUIRegisterMgr::s_instance->entries;
    if (entries.find(window->geometry.id) != entries.end()) {
      static_cast<CUIFrameWnd*>(window)->RefreshSkinColors();
    }
  }
}

// OFFSET: 0x00A8B840
// FUNCTION: fn_17124
void UIWindowMgr::RegisterNamedWindow(std::string name, UIWindow* window) {
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
  this->rendered.push_back(this->window_4F250);
}

// OFFSET: 0x00A8ADB0
// FUNCTION: fn_17117
void UIWindowMgr::RequestAddFriend(const char* name) {
  AddFriendPacket packet;
  packet.id = ID_CZ_ADD_FRIENDS;
  std::memcpy(packet.name, name, sizeof(packet.name));
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
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
void UIWindowMgr::ShowWindows189(unsigned char show) {
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
    const auto entry = this->snapRectangles.find(window);
    if (entry != this->snapRectangles.end()) {
      this->snapRectangles.erase(entry);
    }
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
  for (auto it = this->rendered.begin(); it != this->rendered.end(); ++it) {
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
  this->unknown_0758 = options.unknown_0007;
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

// OFFSET: 0x00AA57E0
// FUNCTION: fn_17224
void UIWindowMgr::SendEmotionHotkey(int index) {
  char text[256];
  const EmotionHotkeys* emotions = reinterpret_cast<const EmotionHotkeys*>(&g_session.unknown_0FF8);
  std::strcpy(text, emotions->current[index].c_str());

  if (g_windowMgr.unknown_01DC != nullptr) {
    const bool linked = HasTextLinks(std::string(text));
    if (linked) {
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xE53), 0, true, false, 0, "Message", 280, 120, 0);
      return;
    }
  }

  CMode* mode = static_cast<CMode*>(g_charViewMode);
  if (text[0] == '/') {
    if (g_session.ProcessEmotionShortcut(text) == 0) {
      std::string parts[3];
      int talkType;
      const int offset = reinterpret_cast<SessionCommandTarget*>(&g_session)->sub_DE1010(text, &talkType, parts);
      if (offset != -1) {
        g_windowMgr.unknown_04DC = text + offset;
      }
      static_cast<CMode*>(g_charViewMode)->SendMsg(0x2A, talkType, reinterpret_cast<int>(parts), 0, 0);
    }
    return;
  }

  g_windowMgr.unknown_04DC = text;
  const int& chatTarget = *reinterpret_cast<const int*>(&g_session.unknown_5490[0x8]);
  if (chatTarget == 2) {
    if (g_guildId != 0) {
      mode->SendMsg(0x81, 0, 0, 0, 0);
      return;
    }
  } else {
    if (chatTarget == 1 && ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount() != 0) {
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

// OFFSET: 0x00AA8F50
// FUNCTION: fn_17252
void UIWindowMgr::BroadcastMessage21() {
  UIWindow* window = this->window_03DC;
  if (window || (window = this->unknown_03B0) != nullptr) {
    window->SendMsg(nullptr, 21, 0, 0, 0, 0);
    return;
  }

  for (UIWindow* rendered : this->rendered) {
    if (rendered->Slot02()) {
      rendered->SendMsg(nullptr, 21, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00AB1120
// FUNCTION: fn_17332
bool UIWindowMgr::IsHoverOnWindow01C8Button() {
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
bool UIWindowMgr::HasModalWindow() {
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

// OFFSET: 0x00AA8660
// FUNCTION: fn_17244
std::vector<unsigned char> UIWindowMgr::SaveReplayWindows() {
  std::vector<unsigned char> data;
  {
    std::vector<unsigned char> chunk(6);
    *reinterpret_cast<std::int16_t*>(&chunk[0]) = 20000;
    *reinterpret_cast<int*>(&chunk[2]) = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  for (UIWindow* window : this->rendered) {
    const unsigned id = static_cast<unsigned>(window->geometry.id);
    if (id <= 10040 && this->policy[id].actions[7]) {
      std::vector<std::uint32_t> state = window->Slot44();
      const int tag = window->geometry.id + 20100;
      std::vector<unsigned char> chunk(state.size() * sizeof(std::uint32_t) + 6, 0);
      *reinterpret_cast<std::int16_t*>(&chunk[0]) = static_cast<std::int16_t>(tag);
      *reinterpret_cast<int*>(&chunk[2]) = static_cast<int>(state.size() * sizeof(std::uint32_t));
      if (state.size() * sizeof(std::uint32_t)) {
        std::memcpy(&chunk[6], state.data(), state.size() * sizeof(std::uint32_t));
      }
      data.insert(data.end(), chunk.begin(), chunk.end());
    }
  }
  {
    std::vector<unsigned char> chunk(6);
    *reinterpret_cast<std::int16_t*>(&chunk[0]) = 20001;
    *reinterpret_cast<int*>(&chunk[2]) = 0;
    data.insert(data.end(), chunk.begin(), chunk.end());
  }
  return data;
}

void __fastcall sub_DFF150(CSession* session);

// OFFSET: 0x00A8C4B0
// FUNCTION: fn_17132
void UIWindowMgr::sub_A8C4B0(int id) {
  g_session.EraseRecord0734(id);
  this->unknown_4F248.push_back(id);
  sub_DFF150(&g_session);
}

__forceinline void RecordEditTextsNoArg(UIWindow* window) {
  union {
    int (UIWindow::*method)(WindowReplayRecorder&);
    void (__thiscall* call)(UIWindow*);
  } convert;
  convert.method = &UIWindow::RecordEditTexts;
  convert.call(window);
}

// OFFSET: 0x00A8CA20
// FUNCTION: fn_17139
bool UIWindowMgr::sub_A8CA20() {
  if (g_replayAnimationScaling != 0) {
    return false;
  }

  bool handled = true;
  if (!this->modalWindows.empty() && this->modalWindows.back() != nullptr) {
    reinterpret_cast<const WindowDialogs*>(this)->Top()->SendMsg(nullptr, 0, 0, 0, 0, 0);
    return true;
  }

  UIWindow* window = this->unknown_0234;
  if (window == nullptr) {
    window = this->unknown_0314;
    if (window != nullptr) {
      if (Replay::Instance().state == 2) {
        RecordEditTextsNoArg(window);
      }
      window->SendMsg(nullptr, 0, 0, 0, 0, 0);
      return true;
    }
    window = this->unknown_0318;
    if (window != nullptr) {
      if (Replay::Instance().state == 2) {
        RecordEditTextsNoArg(window);
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
            if (reinterpret_cast<const ChatSuggestions*>(this->window_01C8)->Visible()) {
              UIWindow* child = reinterpret_cast<const ChatSuggestions*>(this->window_01C8)->unknown_013C;
              if (child != nullptr) {
                child->SendMsg(nullptr, 0xB0, 0xD, 0, 0, 0);
                return true;
              }
            }
            handled = false;
          }
          if (Replay::Instance().state == 2 && handled) {
            RecordEditTextsNoArg(this->hoverWindow->Root());
          }
          if (g_windowCloseByte == 0) {
            handled = true;
          } else if (!handled) {
            return false;
          }
          this->hoverWindow->SendMsg(nullptr, 0, 0, 0, 0, 0);
          return handled;
        }
        window = this->window_03D4;
        if (window == nullptr) {
          if (this->window_01C8 != nullptr && g_inputSuggestionsEnabled == 1) {
            handled = false;
            RecordEditTextsNoArg(this->window_01C8);
            window = this->window_01C8;
          } else {
            for (UIWindow* item : this->rendered) {
              if (!item->Slot02()) {
                continue;
              }
              if (g_replayAnimationScaling == 0) {
                RecordEditTextsNoArg(item->Root());
              }
              item->SendMsg(nullptr, 0, 0, 0, 0, 0);
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
UIWindow* UIWindowMgr::sub_A97580(int id, int x, int y, int width, int height) {
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
      reinterpret_cast<WindowDialogs*>(this)->RestoreInput();
      return false;
    }

    if (GetAsyncKeyState(VK_SHIFT)) {
      if (this->hoverWindow && this->hoverWindow->parent->PreviousFocusable(this->hoverWindow) == nullptr) {
        InputFocus{this->hoverWindow}.Last();
      } else {
        InputFocus{this->hoverWindow}.Previous();
      }
    } else if (this->hoverWindow && this->hoverWindow->parent->NextFocusable(this->hoverWindow) == nullptr) {
      InputFocus{this->hoverWindow}.First();
    } else {
      InputFocus{this->hoverWindow}.Next();
    }
    return false;

  case 13:
    if (pressed) {
      this->sub_A8CA20();
    }
    break;

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

    if ((this->IsHoverOnWindow01C8Button() || (!this->windowHistory.empty() && this->windowHistory.front() == 1)) && this->unknown_0024 != 0 &&
        this->unknown_0026 != 0) {
      ChatSuggestions* suggestions = reinterpret_cast<ChatSuggestions*>(this->window_01C8);
      if (suggestions && suggestions->Visible()) {
        suggestions->Show(0);
        return false;
      }

      this->unknown_0024 = 0;
      reinterpret_cast<ChatInputLayout*>(this->window_01C8)->Apply();
      return false;
    }

    if (this->window_01C8 && this->hoverWindow) {
      UIWindow* parent = this->hoverWindow->parent;
      if (parent && this->hoverWindow->geometry.x != -100) {
        if (parent == reinterpret_cast<ChatSuggestions*>(this->window_01C8)->unknown_0138) {
          parent->SendMsg(this->hoverWindow, 0, 1, 0, 0, 0);
          return false;
        }

        for (const auto& entry : this->retainedWindows) {
          if (entry.second == parent) {
            parent->SendMsg(this->hoverWindow, 0, 1, 0, 0, 0);
            return false;
          }
        }
      }
    }

    if (this->HasModalWindow()) {
      this->unknown_0474 = 188;
      return false;
    }

    const int id = this->windowHistory.empty() ? -1 : this->windowHistory.front();
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
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x8BF), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
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
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xD68), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
        g_modeMgr.gameMode->SendMsg(10011, 0, 0, 0, 0);
      }
      return false;
    }

    if (reinterpret_cast<WindowDialogs*>(this)->Confirm(LookupMessage(0x11), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
      GetRagConnection().Close();
      g_modeMgr.gameMode->SendMsg(2, 0, 0, 0, 0);
    }
    return false;
  }

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

  case 35:
    if (this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, 114, 0, 0, 0, 0);
    }
    break;

  case 36:
    if (this->window_4F1D4) {
      this->window_4F1D4->SendMsg(nullptr, 179, 0, 0, 0, 0);
      return false;
    }
    break;

  case 37:
    this->CancelActiveDialog();
    return false;

  case 38:
    this->ConfirmActiveDialog();
    return false;

  case 39:
    this->BroadcastMessage21();
    return false;

  case 40:
    this->SubmitActiveDialog();
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

    Replay& replay = Replay::Instance();
    ReplayPacketRecorder recorder = {
      g_packetLengths,
      replay.baseFrame,
      replay.started,
      replay.frame,
      replay.header.key0,
      replay.header.key1,
      replay.header.options.recordRestricted,
      replay.file,
      replay.fileOffset,
      replay.records,
      replay.cipherIndex,
      replay.cipherBase,
      replay.cipherKey,
    };
    recorder.Record(packet.bytes, &packet, 42, 0);
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
      static_cast<CMode*>(g_charViewMode)->SendMsg(291, 0, 0, 0, 0);
      this->Schedule(this->unknown_0264);
      this->unknown_0264 = nullptr;
      closed = true;
    }
    break;
  case 24:
    if (this->unknown_025C) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(291, 0, 0, 0, 0);
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
      static_cast<CMode*>(g_charViewMode)->SendMsg(291, 0, 0, 0, 0);
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
      if (g_charViewMode) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(309, 0, 0, 0, 0);
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
      if (g_charViewMode) {
        static_cast<CMode*>(g_charViewMode)->SendMsg(322, 0, 0, 0, 0);
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
      static_cast<CMode*>(g_charViewMode)->SendMsg(146, -1, 0, 0, 0);
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
      g_closeWindow137State = 0;
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
