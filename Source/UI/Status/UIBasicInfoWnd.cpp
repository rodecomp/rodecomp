#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Status/UIBasicInfoWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Jobs/JobId.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Gauges/UIBarGraph.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Lists/UITabControl.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <windows.h>

namespace {

std::int64_t BaseExpCurrent() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.playerExperienceState[0x04]);
}

std::int64_t BaseExpMaximum() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.playerExperienceState[0x0C]);
}

std::int64_t JobExpCurrent() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.playerExperienceState[0x1C]);
}

std::int64_t JobExpMaximum() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.playerExperienceState[0x14]);
}

}

// OFFSET: 0x00997250
// FUNCTION: fn_13484
UIBasicInfoWnd::UIBasicInfoWnd() : UIFrameWnd(0) {
  this->saved = nullptr;
  this->mode = 0;
  this->layoutOffset = 13;
  this->collapsedHeight = 55;
  this->expandedHeight = 134;
  this->extendedOffset = 28;
  this->extendedCollapsedHeight = 71;
  this->extendedExpandedHeight = 149;
  this->normalOffset = 13;
  this->normalCollapsedHeight = 55;
  this->normalExpandedHeight = 134;
  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
    this->layoutOffset = this->extendedOffset;
    this->collapsedHeight = this->extendedCollapsedHeight;
    this->expandedHeight = this->extendedExpandedHeight;
  }
}

// OFFSET: 0x00998190
// FUNCTION: fn_13489
bool UIBasicInfoWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x009981A0
// FUNCTION: fn_13490
void UIBasicInfoWnd::SetHeight(int height) {
  if (height == this->collapsedHeight) {
    this->Resize(this->geometry.width, this->collapsedHeight);
    this->nameText->Resize(this->geometry.width - 30, 15);
    this->nameText->SetTextFitted(g_session.DecodePlayerName());
    this->nameText->Move(17, 3);
    this->hpBar->Move(-100, -100);
    this->spBar->Move(-100, -100);
    this->apBar->Move(-100, -100);
    this->baseExpBar->Move(-100, -100);
    this->jobExpBar->Move(-100, -100);
  } else if (height == this->expandedHeight) {
    this->Resize(this->geometry.width, this->expandedHeight);
    this->hpBar->Move(35, 53);
    this->spBar->Move(35, 68);
    if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
      this->apBar->Move(35, 83);
    }
    this->nameText->Resize(this->geometry.width - 20, 15);
    this->nameText->SetTextFitted(g_session.DecodePlayerName());
    this->nameText->Move(9, 22);

    if (g_session.GetMaxBaseLevel(g_session.GetDisplayJob()) == g_session.m_level) {
      this->baseExpBar->Move(-200, 78);
    } else {
      this->baseExpBar->Move(85, this->layoutOffset + 76);
    }

    if (g_session.m_jobLevel >= g_session.GetMaxJobLevel(g_session.GetDisplayJob())) {
      this->jobExpBar->Move(-200, 88);
    } else {
      this->jobExpBar->Move(85, this->layoutOffset + 88);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);
    window->Move(screenX, this->geometry.height + screenY);
  }
}

// OFFSET: 0x00999490
// FUNCTION: fn_13497
void UIBasicInfoWnd::RefreshLayout() {
  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob())) {
    if (this->geometry.height == this->collapsedHeight) {
      this->layoutOffset = this->extendedOffset;
      this->collapsedHeight = this->extendedCollapsedHeight;
      this->expandedHeight = this->extendedExpandedHeight;
      this->SetHeight(this->extendedCollapsedHeight);
    } else if (this->geometry.height == this->expandedHeight) {
      this->layoutOffset = this->extendedOffset;
      this->collapsedHeight = this->extendedCollapsedHeight;
      this->expandedHeight = this->extendedExpandedHeight;
      this->SetHeight(this->extendedExpandedHeight);
    }
  } else {
    if (this->geometry.height == this->collapsedHeight) {
      this->layoutOffset = this->normalOffset;
      this->collapsedHeight = this->normalCollapsedHeight;
      this->expandedHeight = this->normalExpandedHeight;
      this->SetHeight(this->normalCollapsedHeight);
    } else if (this->geometry.height == this->expandedHeight) {
      this->layoutOffset = this->normalOffset;
      this->collapsedHeight = this->normalCollapsedHeight;
      this->expandedHeight = this->normalExpandedHeight;
      this->SetHeight(this->normalExpandedHeight);
    }
    this->apBar->Move(-100, -100);
  }

  this->Invalidate();
}

// OFFSET: 0x00999BC0
// FUNCTION: fn_13499
void UIBasicInfoWnd::StoreInfo() {
  if (this->saved) {
    this->saved->height = this->geometry.height;
    this->saved->width = this->geometry.width;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->mode = this->mode;
  }
}

// OFFSET: 0x00999360
// FUNCTION: fn_13494
void UIBasicInfoWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x00999330
// FUNCTION: fn_13493
void UIBasicInfoWnd::OnLBtnDblClk(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, UM_COMMAND, ID_MINI, 0, 0, 0);
  }
}

// OFFSET: 0x00999430
// FUNCTION: fn_13496
void UIBasicInfoWnd::OnMouseMove(int x, int y) {
  UIFrameWnd::OnMouseMove(x, y);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);
    window->Move(screenX, this->geometry.height + screenY);
  }
}

// OFFSET: 0x00999380
// FUNCTION: fn_13495
void UIBasicInfoWnd::OnMouseHover(int x, int y) {
  if (this->geometry.height != this->expandedHeight) {
    return;
  }

  int top = this->geometry.height - 20;
  if (x >= 0 && x < 100 && y >= top && y < this->geometry.height) {
    char text[64];
    std::sprintf(text, "Weight %d%%", g_session.m_maxWeight == 0 ? 0 : g_session.m_weight * 100 / g_session.m_maxWeight);

    int screenX = 0;
    int screenY = top - 19;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(text, screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x009995A0
// FUNCTION: fn_13498
int UIBasicInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case 373:
      if (!g_windowMgr.CloseWindow(WID_GUILDWND)) {
        g_windowMgr.EnsureWindow(WID_GUILDWND);
      }
      break;
    case ID_STATUS:
      if (!g_windowMgr.CloseWindow(WID_STATUSWND)) {
        g_windowMgr.EnsureWindow(WID_STATUSWND);
      }
      break;
    case ID_OPTION:
      if (!g_windowMgr.CloseWindow(WID_ESCOPTIONWND)) {
        g_windowMgr.EnsureWindow(WID_ESCOPTIONWND);
      }
      break;
    case 247:
      if (!g_windowMgr.LookupWindow(WID_MERGEITEMWND) && !g_windowMgr.CloseWindow(WID_MERCHANTITEMWND) && g_session.replayPacketMode > 0) {
        g_windowMgr.EnsureWindow(WID_MERCHANTITEMWND);
      }
      break;
    case ID_ITEMS:
      if (!g_windowMgr.CloseWindow(WID_ITEMWND)) {
        g_windowMgr.EnsureWindow(WID_ITEMWND);
      }
      break;
    case ID_QUEST:
      if (!g_windowMgr.CloseWindow(WID_RENEWQUESTUI)) {
        g_windowMgr.EnsureWindow(WID_RENEWQUESTUI);
      }
      break;
    case ID_SKILL:
      if (!g_windowMgr.CloseWindow(WID_SKILLLISTWND)) {
        g_windowMgr.EnsureWindow(WID_SKILLLISTWND);
      }
      break;
    case ID_EQUIP: {
      UIEquipWnd* equip = dynamic_cast<UIEquipWnd*>(g_windowMgr.LookupWindow(WID_EQUIPWND));
      if (equip) {
        if (equip->m_page == 0) {
          g_windowMgr.CloseWindow(WID_EQUIPWND);
          return 0;
        }
      } else {
        equip = dynamic_cast<UIEquipWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_EQUIPWND)));
        if (!equip) {
          return 0;
        }
      }

      equip->m_tabs->SelectTab(0);
      break;
    }
    case ID_MAP:
      if (!g_windowMgr.CloseWindow(WID_ROMAPWND)) {
        g_windowMgr.EnsureWindow(WID_ROMAPWND);
      }
      break;
    case ID_FRIEND:
      if (!g_windowMgr.CloseWindow(WID_MESSENGERWND)) {
        g_windowMgr.EnsureWindow(WID_MESSENGERWND);
      }
      break;
    case 214:
      if (!g_windowMgr.CloseWindow(WID_CHATROOMMAKEWND) && !g_windowMgr.chatRoomWnd) {
        if (!g_windowMgr.merchantShopMakeWnd && !g_windowMgr.merchantItemMyShopWnd) {
          g_windowMgr.EnsureWindow(WID_CHATROOMMAKEWND);
        } else {
          g_windowMgr.Confirm(LookupMessage(MSI_CANT_OPEN_STORE_WHILE_CHAT), 0, true, false, 0, "Message", 280, 120, 0);
        }
      }
      break;
    case 222:
      g_windowMgr.CloseWindow(WID_STATUSWND);
      g_windowMgr.CloseWindow(13);
      g_windowMgr.CloseWindow(WID_ITEMWND);
      g_windowMgr.CloseWindow(WID_RENEWQUESTUI);
      g_windowMgr.CloseWindow(WID_EQUIPWND);
      g_windowMgr.CloseWindow(WID_MESSENGERWND);
      g_windowMgr.CloseWindow(WID_CHATROOMMAKEWND);
      g_windowMgr.CloseWindow(WID_CHATROOMCHANGEWND);
      g_windowMgr.CloseWindow(WID_SKILLLISTWND);
      g_windowMgr.CloseWindow(WID_TIPOFTHEDAYWND);
      g_windowMgr.CloseWindow(WID_PETINFOWND);
      g_windowMgr.CloseWindow(WID_EMOTIONWND);
      g_windowMgr.CloseWindow(WID_CASHEMOTIONLISTWND);
      g_windowMgr.CloseWindow(WID_MERCHANTITEMWND);
      g_windowMgr.CloseWindow(WID_GUILDWND);
      g_windowMgr.ShowWindowById(WID_SHORTCUTWND, 0);
      this->SetHeight(this->collapsedHeight);
      g_windowMgr.StoreSnapRectangle(this);
      break;
    case ID_MINI: {
      const int height = this->geometry.height;
      this->SetHeight(height == this->collapsedHeight ? this->expandedHeight : this->collapsedHeight);
      g_windowMgr.StoreSnapRectangle(this);
      g_windowMgr.Promote(this);
      const int delta = this->geometry.height - height;
      if (delta != 0) {
        g_snapMgr.MoveEdge(this, 0, delta, 3);
      }
      break;
    }
    case 578: {
      UIEquipWnd* equip = dynamic_cast<UIEquipWnd*>(g_windowMgr.LookupWindow(WID_EQUIPWND));
      if (equip) {
        if (equip->m_page == 1) {
          g_windowMgr.CloseWindow(WID_EQUIPWND);
          return 0;
        }
      } else {
        equip = dynamic_cast<UIEquipWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_EQUIPWND)));
        if (!equip) {
          return 0;
        }
      }

      equip->m_tabs->SelectTab(1);
      break;
    }
    }
    break;
  case UM_SETINFO:
    this->saved = reinterpret_cast<BasicInfoWndInfo*>(argument1);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->mode = this->saved->mode != 0;
    this->SetHeight(this->saved->height);
    this->Move(this->saved->x, this->saved->y);
    break;
  case UM_REFRESH:
    if (this->geometry.height != this->expandedHeight) {
      this->Invalidate();
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

// OFFSET: 0x009974F0
// FUNCTION: fn_13487
void UIBasicInfoWnd::DrawExpanded() {
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\w_basewin_bg2.bmp"))
  ), false);

  this->DrawTextAt(18, 3, LookupMessage(MSI_BASICINFOWND), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(17, 2, LookupMessage(MSI_BASICINFOWND), 0, 0, 12, 0, 0, 0);

  const bool compact = g_serviceType == 0 || g_serviceType == 14;
  const int family = compact ? 0 : 1;
  const int fontHeight = compact ? 12 : 14;
  const int job = g_session.GetDisplayJob();
  const char* jobName = g_session.GetJobName(job, -1);
  this->DrawTextAt(9, 36, jobName, 0, family, fontHeight, 0, 0, 0);
  this->DrawTextAt(9, 51, LookupMessage(MSI_BASICINFO_JOBLEVEL), 0, 1, 14, 0, 0, 0);
  this->DrawTextAt(9, 65, LookupMessage(MSI_BASICINFO_EXP), 0, 1, 14, 0, 0, 0);
  this->DrawTextAt(9, 80, LookupMessage(MSI_GUILD_AGIT_ARE_YOU_MOVE_AGIT), 0, 1, 14, 0, 0, 0);

  if (g_session.maxHp > 0) {
    const std::int64_t percent = static_cast<std::int64_t>(g_session.hp) * 100 / g_session.maxHp;
    this->DrawTextAt(173, 51, FormatOwnedString("%d%%", percent).c_str(), 0, 1, 14, 0, 0, 0);
  }

  const int sp = *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[0]);
  const int spMax = *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[4]);
  if (spMax > 0) {
    const std::int64_t percent = static_cast<std::int64_t>(sp) * 100 / spMax;
    this->DrawTextAt(173, 65, FormatOwnedString("%d%%", percent).c_str(), 0, 1, 14, 0, 0, 0);
  }

  if (g_session.GetTraitStat(233) > 0) {
    this->DrawTextAt(
      173, 80, FormatOwnedString("%d%%", g_session.GetTraitStat(232) * 100 / g_session.GetTraitStat(233)).c_str(), 0, 1, 14, 0, 0, 0
    );
  }

  {
    std::string baseLevel = FormatOwnedString(LookupMessage(MSI__BASIC_MSG_BASE), g_session.m_level);
    this->DrawTextAt(17, this->layoutOffset + 71, baseLevel.c_str(), 0, 1, 14, 0, 0, 0);
  }

  {
    std::string jobLevel = FormatOwnedString(LookupMessage(MSI__BASIC_MSG_JOB), g_session.m_jobLevel);
    if (g_serviceType == 12) {
      jobLevel = static_cast<const std::string&>(FormatOwnedString("Classe. %d", g_session.m_jobLevel));
    }
    this->DrawTextAt(17, this->layoutOffset + 83, jobLevel.c_str(), 0, 1, 14, 0, 0, 0);
  }

  char zenyBuffer[64];
  std::memset(zenyBuffer, 0, sizeof(zenyBuffer));
  FormatGroupedInteger(g_session.m_zeny, zenyBuffer, 64);
  std::string zenyText = FormatOwnedString(LookupMessage(MSI_BASIC_MSG_ZENY), zenyBuffer);
  zenyText = " | " + zenyText;
  this->DrawRightAlignedText(this->geometry.width - 5, this->layoutOffset + 103, zenyText.c_str(), 0, 1, 14, 0, 0);
  const int zenyWidth = this->TextWidth(zenyText.c_str(), 0, 1, 14, 0, 0
  );

  std::string weightText;
  if (g_serviceType == 12) {
    weightText = static_cast<const std::string&>(FormatOwnedString("Peso : %3d / %3d", g_session.m_weight, g_session.m_maxWeight));
  } else {
    weightText = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_WEIGHT), g_session.m_weight, g_session.m_maxWeight));
  }

  const int denominator = g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight;
  const std::uint32_t weightColor = 100 * g_session.m_weight / denominator >= g_overweightThresholdPercent ? 255 : 0;
  this->DrawRightAlignedText(
    this->geometry.width - zenyWidth - 5, this->layoutOffset + 103, weightText.c_str(), 0, 1, 14, weightColor, 0
  );
}

// OFFSET: 0x00997C20
// FUNCTION: fn_13488
void UIBasicInfoWnd::DrawCollapsed() {
  {
    unsigned red;
    unsigned green;
    unsigned blue;
    SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    this->FillRegion(1, 17, this->geometry.width - 2, this->geometry.height - 18, RGB(red, green, blue));
    SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
    DrawRoundedBorder(*this, 0, 14, this->geometry.width, this->geometry.height - 14, RGB(red, green, blue));
  }
  this->DrawTitleBar(false, "", 0);

  int family = 1;
  int fontHeight = 14;
  if (g_serviceType == 0) {
    switch (g_session.GetDisplayJob()) {
    case JT_TAEKWON:
    case JT_STAR:
    case JT_STAR2:
    case JT_LINKER:
    case JT_STAR_EMPEROR:
    case JT_SOUL_REAPER:
    case JT_STAR_EMPEROR_B:
    case JT_SOUL_REAPER_B:
    case JT_STAR2_EMPEROR:
    case JT_STAR2_EMPEROR_B:
    case JT_SKY_EMPEROR:
    case JT_SOUL_ASCETIC:
    case JT_SKY_EMPEROR2:
      family = 0;
      fontHeight = 11;
      break;
    }
  }

  this->DrawTextAt(
    8, 20,
    FormatOwnedString(
      "%s %d / %s / %s %d", LookupMessage(MSI_ALL_ON), g_session.m_level, g_session.GetJobName(g_session.GetDisplayJob(), -1),
      LookupMessage(MSI_GUILD_AGIT_INVESTMENT_INFO), g_session.m_jobLevel
    ).c_str(),
    0, family, fontHeight, 0, 0, 0
  );

  const int hp = g_session.hp;
  const int maxHp = g_session.maxHp;
  std::uint32_t hpColor = 0;
  if (maxHp > 0) {
    hpColor = 0.25f > static_cast<float>(hp) / static_cast<float>(maxHp) ? 255 : 0;
  }
  this->DrawTextAt(
    8, 36, FormatOwnedString("%s %d / %d", LookupMessage(MSI_BASICINFO_JOBLEVEL), hp, maxHp).c_str(), 0, family, fontHeight, hpColor, 0, 0
  );

  std::int64_t current = BaseExpCurrent();
  if (current < 0) {
    current = 0;
  }
  std::int64_t maximum = BaseExpMaximum();
  if (maximum <= 0) {
    maximum = current + 1;
  }
  double percent;
  if (g_session.m_level < g_session.GetMaxBaseLevel(g_session.GetDisplayJob())) {
    percent = static_cast<double>(
                static_cast<std::int64_t>(static_cast<double>(current) / static_cast<double>(maximum) * 1000.0)
              ) *
              0.1;
  } else {
    percent = 100.0;
  }
  this->DrawTextAt(
    136, 36, FormatOwnedString("| %s %.1lf%%", LookupMessage(MSI_SIMPLECASH_P), percent).c_str(), 0, family, fontHeight, 0, 0, 0
  );

  const int sp = *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[0]);
  const int spMax = *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[4]);
  std::uint32_t spColor = 0;
  if (spMax > 0) {
    spColor = 0.25f > static_cast<float>(sp) / static_cast<float>(spMax) ? 255 : 0;
  }
  this->DrawTextAt(
    8, 52, FormatOwnedString("%s %d / %d", LookupMessage(MSI_BASICINFO_EXP), sp, spMax).c_str(), 0, family, fontHeight, spColor, 0, 0
  );

  this->DrawTextAt(
    136, 52,
    FormatOwnedString("| %s %d / %d", LookupMessage(MSI_GUILD_AGIT_ARE_YOU_MOVE_AGIT), g_session.GetTraitStat(232), g_session.GetTraitStat(233))
      .c_str(),
    0, family, fontHeight, 0, 0, 0
  );
}

// OFFSET: 0x009983C0
// FUNCTION: fn_13491
void UIBasicInfoWnd::OnCreate(int width, int height) {
  this->AddBaseButton();

  UIBitmapButton* minimizeButton = this->AddMinimizeButton("Alt+V, Ctrl+V", 0);
  minimizeButton->Move(this->geometry.width - 14, 3);

  this->nameText = new UIStaticText();
  this->nameText->SetSize(this->geometry.width - 20, 15);
  this->nameText->Move(9, 23);
  this->nameText->SetTextFitted(g_session.DecodePlayerName());
  this->nameText->SetFont(0, 12, 0, 0);
  this->AddChild(this->nameText);

  this->hpBar = new UIBarGraphPlayer();
  static_cast<UIBarGraphPlayer*>(this->hpBar)->SetValues(
    []() -> std::int64_t { return g_session.hp; },
    []() -> std::int64_t { return g_session.maxHp; }
  );
  static_cast<UIBarGraphPlayer*>(this->hpBar)->SetSize(135, 9);
  static_cast<UIBarGraphPlayer*>(this->hpBar)->Move(-100, -100);
  this->AddChild(this->hpBar);

  this->spBar = new UIBarGraphPlayer();
  static_cast<UIBarGraphPlayer*>(this->spBar)->SetValues(
    []() -> std::int64_t { return *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[0]); },
    []() -> std::int64_t { return *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[4]); }
  );
  static_cast<UIBarGraphPlayer*>(this->spBar)->SetSize(135, 9);
  static_cast<UIBarGraphPlayer*>(this->spBar)->Move(-100, -100);
  this->AddChild(this->spBar);

  this->apBar = new UIBarGraphPlayerAP();
  static_cast<UIBarGraphPlayerAP*>(this->apBar)->SetValues(
    []() -> std::int64_t { return g_session.GetTraitStat(232); },
    []() -> std::int64_t { return g_session.GetTraitStat(233); }
  );
  static_cast<UIBarGraphPlayerAP*>(this->apBar)->SetSize(135, 9);
  static_cast<UIBarGraphPlayerAP*>(this->apBar)->Move(-100, -100);
  this->AddChild(this->apBar);

  this->baseExpBar = new UIINT64BarGraph();
  static_cast<UIINT64BarGraph*>(this->baseExpBar)->m_plainColor = 1;
  static_cast<UIINT64BarGraph*>(this->baseExpBar)->SetSize(112, 6);
  static_cast<UIINT64BarGraph*>(this->baseExpBar)->Move(84, this->layoutOffset + 76);
  static_cast<UIINT64BarGraph*>(this->baseExpBar)->SetValues(BaseExpCurrent(), BaseExpMaximum());
  this->AddChild(this->baseExpBar);

  this->jobExpBar = new UIINT64BarGraph();
  static_cast<UIINT64BarGraph*>(this->jobExpBar)->m_plainColor = 1;
  static_cast<UIINT64BarGraph*>(this->jobExpBar)->SetSize(112, 6);
  static_cast<UIINT64BarGraph*>(this->jobExpBar)->Move(84, this->layoutOffset + 86);
  static_cast<UIINT64BarGraph*>(this->jobExpBar)->SetValues(JobExpCurrent(), JobExpMaximum());
  this->AddChild(this->jobExpBar);

  this->SetHeight(height);
}

// OFFSET: 0x00998810
// FUNCTION: fn_13492
void UIBasicInfoWnd::Paint() {
  char zenyText[64];
  char text[128];

  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob()) == true) {
    if (this->geometry.height == this->collapsedHeight) {
      this->DrawCollapsed();
    } else if (this->geometry.height == this->expandedHeight) {
      this->DrawExpanded();
    }
    return;
  }

  if (this->geometry.height == this->collapsedHeight) {
    {
      unsigned red;
      unsigned green;
      unsigned blue;
      SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
      DrawRoundedBorder(*this, 0, 17, this->geometry.width, this->geometry.height - 17, RGB(red, green, blue));
      this->FillRegion(0, 17, this->geometry.width, 5, RGB(red, green, blue));
      SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
      this->FillRegion(1, 17, this->geometry.width - 2, this->geometry.height - 19, RGB(red, green, blue));
      this->FillRegion(2, this->geometry.height - 2, this->geometry.width - 4, 1, RGB(red, green, blue));
    }
    this->DrawTitleBar(false, "", 0);

    std::int64_t current = BaseExpCurrent();
    std::int64_t maximum = BaseExpMaximum();
    if (current < 0) {
      current = 0;
    }
    if (maximum <= 0) {
      maximum = current + 1;
    }

    int maxLevel;
    if (g_session.IsTreeGroupTwo(g_session.GetDisplayJob())) {
      maxLevel = g_session.m_maxLevel;
    } else if (g_session.IsSuperNovice2Job(g_session.GetDisplayJob())) {
      maxLevel = *reinterpret_cast<const int*>(&g_session.jobLevelLimits[0]);
    } else if (g_session.IsExpandedJob(g_session.GetDisplayJob())) {
      maxLevel = *reinterpret_cast<const int*>(&g_session.jobLevelLimits[4]);
    } else {
      maxLevel = g_session.IsDoramJob(g_session.GetDisplayJob()) == true ? *reinterpret_cast<const int*>(&g_session.jobLevelLimits[0x28])
                                                                         : *reinterpret_cast<const int*>(g_session.maxBaseLevel);
    }

    double percent;
    if (maxLevel == g_session.m_level) {
      percent = 100.0;
    } else {
      percent = static_cast<float>(static_cast<int>(static_cast<double>(current) / static_cast<double>(maximum) * 1000.0)) * 0.1f;
    }

    int x = 9;
    int family = 1;
    int fontHeight = 14;
    int length;
    if (g_serviceType == 12) {
      length = std::sprintf(text, "Nv. %2d / %s / Nv. %2d / Exp. %lld %%", g_session.m_level, g_session.GetJobName(g_session.GetDisplayJob(), -1),
                            g_session.m_jobLevel, static_cast<long long>(percent));
    } else {
      if (g_serviceType == 0) {
        switch (g_session.GetDisplayJob()) {
        case JT_SUPERNOVICE:
        case JT_TAEKWON:
        case JT_STAR:
        case JT_STAR2:
        case JT_LINKER:
        case JT_STAR_EMPEROR:
        case JT_SOUL_REAPER:
        case JT_STAR_EMPEROR_B:
        case JT_SOUL_REAPER_B:
        case JT_STAR2_EMPEROR:
        case JT_STAR2_EMPEROR_B:
        case JT_SKY_EMPEROR:
        case JT_SOUL_ASCETIC:
        case JT_SKY_EMPEROR2:
          family = 0;
          fontHeight = 11;
          break;
        }
      }

      length = std::sprintf(text, "Lv.%2d / %s / Lv.%2d / Exp. %.1lf %%", g_session.m_level, g_session.GetJobName(g_session.GetDisplayJob(), -1),
                            g_session.m_jobLevel, percent);
      if (this->TextWidth(text, length, family, fontHeight, 0, 0) + 9 > this->geometry.width) {
        length = std::sprintf(text, "Lv%2d / %s / Lv%2d / Exp%.1lf%%", g_session.m_level, g_session.GetJobName(g_session.GetDisplayJob(), -1),
                              g_session.m_jobLevel, percent);
        const int remaining = this->geometry.width - this->TextWidth(text, length, family, fontHeight, 0, 0);
        if (remaining < 0) {
          x = 3;
        } else {
          x = remaining / 2;
        }
      }
    }
    this->DrawTextAt(x, 20, text, length, family, fontHeight, 0, 0, 0);

    FormatGroupedInteger(g_session.m_zeny, zenyText, 64);

    int offset = 0;
    const int maxHp = g_session.maxHp;
    if (maxHp != 0) {
      const int hp = g_session.hp;
      const std::uint32_t color = static_cast<float>(hp) * 100.0f / static_cast<float>(maxHp) < 25.0f ? 255 : 0;
      const int hpLength = std::sprintf(text, x == 9 ? "HP. %d / %d" : "HP %d / %d", hp, maxHp);
      this->DrawTextAt(x, 36, text, hpLength, 1, 14, color, 0, 0);
      offset = this->TextWidth(text, hpLength, 1, 14, 0, 0);
      length = std::sprintf(text, " | ");
      this->DrawTextAt(offset + x, 36, text, length, 1, 14, 0, 0, 0);
    }

    const int& sp = *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[0]);
    const int& spMax = *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[4]);
    if (spMax < 0) {
      return;
    }

    const std::uint32_t color = static_cast<float>(sp) * 100.0f / static_cast<float>(spMax) < 25.0f ? 255 : 0;
    offset += this->TextWidth(text, length, 1, 14, 0, 0);
    const int spLength = std::sprintf(text, x == 9 ? "SP. %d / %d" : "SP %d / %d", sp, spMax);
    this->DrawTextAt(offset + x, 36, text, spLength, 1, 14, color, 0, 0);
  } else if (this->geometry.height == this->expandedHeight) {
    CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\basewin_bg2.bmp")));
    this->DrawTitleBar(false, "", 0);
    this->DrawBitmap(0, 0, background, false);

    int family;
    int fontHeight;
    if (g_serviceType == 0 || g_serviceType == 14) {
      family = 0;
      fontHeight = 12;
    } else {
      family = 1;
      fontHeight = 14;
    }
    const int job = g_session.GetDisplayJob();
    const char* jobName = g_session.GetJobName(job, -1);
    this->DrawTextAt(9, 36, jobName, 0, family, fontHeight, 0, 0, 0);

    std::memset(text, 0, sizeof(text));
    if (g_serviceType == 1) {
      this->DrawTextAt(18, 3, "Basic Info", 0, 0, 12, 0xFFFFFF, 0, 0);
      this->DrawTextAt(17, 2, "Basic Info", 0, 0, 12, 0, 0, 0);
    } else {
      this->DrawTextAt(18, 3, LookupMessage(MSI_BASICINFOWND), 0, 0, 12, 0xFFFFFF, 0, 0);
      this->DrawTextAt(17, 2, LookupMessage(MSI_BASICINFOWND), 0, 0, 12, 0, 0, 0);
    }

    this->DrawTextAt(17, 51, "HP", 0, 1, 14, 0, 0, 0);
    this->DrawTextAt(17, 65, "SP", 0, 1, 14, 0, 0, 0);

    const int maxHp = g_session.maxHp;
    if (maxHp != 0) {
      std::sprintf(text, "%d%%", g_session.hp * 100 / maxHp);
      this->DrawTextAt(173, 51, text, 0, 1, 14, 0, 0, 0);
    }

    const int spMax = *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[4]);
    if (spMax != 0) {
      std::sprintf(text, "%d%%", *reinterpret_cast<const int*>(&g_session.spAndHomunculusStats[0]) * 100 / spMax);
      this->DrawTextAt(173, 65, text, 0, 1, 14, 0, 0, 0);
    }

    if (g_serviceType == 12) {
      std::sprintf(text, "Base. %d", g_session.m_level);
    } else {
      std::sprintf(text, "Base Lv. %d", g_session.m_level);
    }
    this->DrawTextAt(17, this->layoutOffset + 71, text, 0, 1, 14, 0, 0, 0);
    if (g_serviceType == 12) {
      std::sprintf(text, "Classe. %d", g_session.m_jobLevel);
    } else {
      std::sprintf(text, "Job Lv. %d", g_session.m_jobLevel);
    }
    this->DrawTextAt(17, this->layoutOffset + 83, text, 0, 1, 14, 0, 0, 0);

    std::memset(zenyText, 0, sizeof(zenyText));
    FormatGroupedInteger(g_session.m_zeny, zenyText, 64);
    std::sprintf(text, "Zeny : %s", zenyText);
    const int zenyWidth = this->TextWidth(text, std::strlen(text), 1, 14, 0, 0);
    this->DrawRightAlignedText(this->geometry.width - 5, this->layoutOffset + 103, text, std::strlen(text), 1, 14, 0, 0);

    if (g_serviceType == 12) {
      std::sprintf(text, "Peso : %3d / %3d", g_session.m_weight, g_session.m_maxWeight);
    } else {
      std::sprintf(text, "Weight : %3d / %3d", g_session.m_weight, g_session.m_maxWeight);
    }
    int capacity = g_session.m_maxWeight;
    if (capacity < 1) {
      capacity = 1;
    }
    const std::uint32_t color = g_session.m_weight * 100 / capacity >= g_overweightThresholdPercent ? 255 : 0;
    this->DrawRightAlignedText(this->geometry.width - zenyWidth - 10, this->layoutOffset + 103, text, std::strlen(text), 1, 14, color, 0);
  }
}
