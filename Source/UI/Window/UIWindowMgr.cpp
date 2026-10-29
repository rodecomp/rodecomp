#include "Client/Browser/WebBrowser.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Replay.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Lists/UIDropDownBox.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <new>

// GLOBAL: gl_24525
CMiniPartyWndAreaMgr* g_miniPartyWndAreaMgr = nullptr;

// GLOBAL: gl_24366
UIWindowMgr g_windowMgr;

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
  this->unknown_028C = 0;
  this->unknown_0290 = 0;
  this->unknown_0294 = 0;
  this->unknown_0298 = 0;
  this->unknown_029C = 0;
  this->unknown_02A0 = 0;
  this->unknown_02A4 = 0;
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
  this->unknown_034C = 0;
  this->unknown_0350 = 0;
  this->unknown_0354 = 0;
  this->unknown_0358 = 0;
  this->unknown_035C = 0;
  this->unknown_0360 = 0;
  this->unknown_0364 = 0;
  this->unknown_0368 = 0;
  this->unknown_036C = 0;
  this->unknown_0370 = 0;
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
    if (CMode* mode = g_world.GetGameMode()) {
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
  this->unknown_04A0 = 0;
  this->unknown_04A4 = 0;
  this->unknown_04A8 = 0;
  this->unknown_04AC = 0;
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
  if (CMode* mode = g_world.GetGameMode()) {
    mode->SendMsg(292, 0, 0, 0, 0);
  }
}
