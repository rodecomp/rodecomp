#include "UI/Window/UINewSelectCharWnd.h"
#include "Client/Mode/ModeTooltip.h"
#include "UI/Window/UIBasicInfoWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/UIBarGraph.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/MessageTable.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowOrdering.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <windows.h>

namespace {

std::int64_t BaseExpCurrent() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.unknown_162C[0x04]);
}

std::int64_t BaseExpMaximum() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.unknown_162C[0x0C]);
}

std::int64_t JobExpCurrent() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.unknown_162C[0x1C]);
}

std::int64_t JobExpMaximum() {
  return *reinterpret_cast<const std::int64_t*>(&g_session.unknown_162C[0x14]);
}

}

// OFFSET: 0x00997250
// FUNCTION: fn_13484
UIBasicInfoWnd::UIBasicInfoWnd(UIWindowContext& windowContext) : UIFrameWnd(windowContext, 0) {
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
bool UIBasicInfoWnd::Slot03() {
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

    if (g_session.m_jobLevel < g_session.GetMaxJobLevel(g_session.GetDisplayJob())) {
      this->jobExpBar->Move(85, this->layoutOffset + 88);
    } else {
      this->jobExpBar->Move(-200, 88);
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);
    window->Move(screenX, this->geometry.height + screenY);
  }
}

// OFFSET: 0x00999490
// FUNCTION: fn_13497
void UIBasicInfoWnd::RefreshLayout() {
  const int height = this->geometry.height;
  if (g_session.IsTreeGroupThree(g_session.GetDisplayJob())) {
    if (height == this->collapsedHeight) {
      this->layoutOffset = this->extendedOffset;
      this->collapsedHeight = this->extendedCollapsedHeight;
      this->expandedHeight = this->extendedExpandedHeight;
      this->SetHeight(this->extendedCollapsedHeight);
    } else if (height == this->expandedHeight) {
      this->layoutOffset = this->extendedOffset;
      this->collapsedHeight = this->extendedCollapsedHeight;
      this->expandedHeight = this->extendedExpandedHeight;
      this->SetHeight(this->extendedExpandedHeight);
    }
  } else {
    if (height == this->collapsedHeight) {
      this->layoutOffset = this->normalOffset;
      this->collapsedHeight = this->normalCollapsedHeight;
      this->expandedHeight = this->normalExpandedHeight;
      this->SetHeight(this->normalCollapsedHeight);
    } else if (height == this->expandedHeight) {
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
void UIBasicInfoWnd::Slot11() {
  if (this->saved) {
    this->saved->height = this->geometry.height;
    this->saved->width = this->geometry.width;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->unknown_0010 = this->mode;
  }
}

// OFFSET: 0x00999360
// FUNCTION: fn_13494
void UIBasicInfoWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x00999330
// FUNCTION: fn_13493
void UIBasicInfoWnd::Slot26(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, 6, 202, 0, 0, 0);
  }
}

// OFFSET: 0x00999430
// FUNCTION: fn_13496
void UIBasicInfoWnd::Slot27(int x, int y) {
  UIFrameWnd::Slot27(x, y);
  if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);
    window->Move(screenX, this->geometry.height + screenY);
  }
}

// OFFSET: 0x00999380
// FUNCTION: fn_13495
void UIBasicInfoWnd::Slot28(int x, int y) {
  if (this->geometry.height != this->expandedHeight) {
    return;
  }

  int top = this->geometry.height - 20;
  if (x < 0 || x >= 100 || y < top || y >= this->geometry.height) {
    return;
  }

  char text[64];
  std::sprintf(text, "Weight %d%%", g_session.m_maxWeight ? g_session.m_weight * 100 / g_session.m_maxWeight : 0);

  int screenX = 0;
  int screenY = top - 19;
  this->ToScreen(screenX, screenY);
  static_cast<ModeTooltip*>(g_charViewMode)->Show(text, screenX, screenY, -1, false, true);
}

// OFFSET: 0x009995A0
// FUNCTION: fn_13498
int UIBasicInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 192:
      if (!g_windowMgr.CloseWindow(11)) {
        g_windowMgr.EnsureWindow(11);
      }
      break;
    case 193:
      if (!g_windowMgr.CloseWindow(155)) {
        g_windowMgr.EnsureWindow(155);
      }
      break;
    case 194:
      if (!g_windowMgr.CloseWindow(8)) {
        g_windowMgr.EnsureWindow(8);
      }
      break;
    case 195: {
      UIEquipWnd* equip = dynamic_cast<UIEquipWnd*>(g_windowMgr.LookupWindow(10));
      if (equip) {
        if (equip->m_page == 0) {
          g_windowMgr.CloseWindow(10);
          return 0;
        }
      } else {
        equip = dynamic_cast<UIEquipWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(10)));
        if (!equip) {
          return 0;
        }
      }

      equip->m_tabs->SelectTab(0);
      break;
    }
    case 196:
      if (!g_windowMgr.CloseWindow(37)) {
        g_windowMgr.EnsureWindow(37);
      }
      break;
    case 199:
      if (!g_windowMgr.CloseWindow(69)) {
        g_windowMgr.EnsureWindow(69);
      }
      break;
    case 202: {
      const int height = this->geometry.height;
      this->SetHeight(height == this->collapsedHeight ? this->expandedHeight : this->collapsedHeight);
      g_windowMgr.StoreSnapRectangle(this);
      reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this);
      const int delta = this->geometry.height - height;
      if (delta != 0) {
        g_snapMgr.MoveEdge(this, 0, delta, 3);
      }
      break;
    }
    case 214:
      if (!g_windowMgr.CloseWindow(27) && !g_windowMgr.window_027C) {
        if (!g_windowMgr.unknown_02FC && !g_windowMgr.unknown_0304) {
          g_windowMgr.EnsureWindow(27);
        } else {
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(587), 0, true, false, 0, "Message", 280, 120, 0);
        }
      }
      break;
    case 219:
      if (!g_windowMgr.CloseWindow(140)) {
        g_windowMgr.EnsureWindow(140);
      }
      break;
    case 222:
      g_windowMgr.CloseWindow(11);
      g_windowMgr.CloseWindow(13);
      g_windowMgr.CloseWindow(8);
      g_windowMgr.CloseWindow(10008);
      g_windowMgr.CloseWindow(10);
      g_windowMgr.CloseWindow(69);
      g_windowMgr.CloseWindow(27);
      g_windowMgr.CloseWindow(30);
      g_windowMgr.CloseWindow(37);
      g_windowMgr.CloseWindow(38);
      g_windowMgr.CloseWindow(88);
      g_windowMgr.CloseWindow(86);
      g_windowMgr.CloseWindow(87);
      g_windowMgr.CloseWindow(40);
      g_windowMgr.CloseWindow(59);
      g_windowMgr.ShowWindowById(36, 0);
      this->SetHeight(this->collapsedHeight);
      g_windowMgr.StoreSnapRectangle(this);
      break;
    case 247:
      if (!g_windowMgr.LookupWindow(227) && !g_windowMgr.CloseWindow(40) && g_session.unknown_5C1C > 0) {
        g_windowMgr.EnsureWindow(40);
      }
      break;
    case 361:
      if (!g_windowMgr.CloseWindow(10008)) {
        g_windowMgr.EnsureWindow(10008);
      }
      break;
    case 373:
      if (!g_windowMgr.CloseWindow(59)) {
        g_windowMgr.EnsureWindow(59);
      }
      break;
    case 578: {
      UIEquipWnd* equip = dynamic_cast<UIEquipWnd*>(g_windowMgr.LookupWindow(10));
      if (equip) {
        if (equip->m_page == 1) {
          g_windowMgr.CloseWindow(10);
          return 0;
        }
      } else {
        equip = dynamic_cast<UIEquipWnd*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(10)));
        if (!equip) {
          return 0;
        }
      }

      equip->m_tabs->SelectTab(1);
      break;
    }
    }
    break;
  case 34:
    this->saved = reinterpret_cast<BasicInfoWndInfo*>(argument1);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->mode = this->saved->unknown_0010 != 0;
    this->SetHeight(this->saved->height);
    this->Move(this->saved->x, this->saved->y);
    break;
  case 60:
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
  const int width = this->geometry.width;
  const int job = g_session.GetDisplayJob();
  const bool compact = g_serviceType == 0 || g_serviceType == 14;
  const int family = compact ? 0 : 1;
  const int fontHeight = compact ? 12 : 14;

  CBitmapRes* background = static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\w_basewin_bg2.bmp"))
  );
  this->DrawBitmap(0, 0, &background->image, false);

  this->DrawTextAt(18, 3, LookupMessage(238), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(17, 2, LookupMessage(238), 0, 0, 12, 0, 0, 0);

  this->DrawTextAt(9, 36, g_session.GetJobName(job, -1), 0, family, fontHeight, 0, 0, 0
  );
  this->DrawTextAt(9, 51, LookupMessage(3068), 0, 1, 14, 0, 0, 0);
  this->DrawTextAt(9, 65, LookupMessage(3069), 0, 1, 14, 0, 0, 0);
  this->DrawTextAt(9, 80, LookupMessage(3628), 0, 1, 14, 0, 0, 0);

  if (g_session.unknown_556C > 0) {
    const std::int64_t percent = static_cast<std::int64_t>(g_session.unknown_5568) * 100 / g_session.unknown_556C;
    std::string text = FormatOwnedString("%d%%", percent);
    this->DrawTextAt(173, 51, text.c_str(), 0, 1, 14, 0, 0, 0);
  }

  const int sp = *reinterpret_cast<const int*>(&g_session.unknown_5570[0]);
  const int spMax = *reinterpret_cast<const int*>(&g_session.unknown_5570[4]);
  if (spMax > 0) {
    const std::int64_t percent = static_cast<std::int64_t>(sp) * 100 / spMax;
    std::string text = FormatOwnedString("%d%%", percent);
    this->DrawTextAt(173, 65, text.c_str(), 0, 1, 14, 0, 0, 0);
  }

  const int apMax = g_session.GetTraitStat(233);
  if (apMax > 0) {
    std::string text = FormatOwnedString("%d%%", 100 * g_session.GetTraitStat(232) / apMax);
    this->DrawTextAt(173, 80, text.c_str(), 0, 1, 14, 0, 0, 0);
  }

  std::string baseLevel = FormatOwnedString(LookupMessage(1995), g_session.m_level);
  this->DrawTextAt(17, this->layoutOffset + 71, baseLevel.c_str(), 0, 1, 14, 0, 0, 0
  );

  std::string jobLevel = FormatOwnedString(LookupMessage(1996), g_session.m_jobLevel);
  if (g_serviceType == 12) {
    jobLevel = FormatOwnedString("Classe. %d", g_session.m_jobLevel);
  }
  this->DrawTextAt(17, this->layoutOffset + 83, jobLevel.c_str(), 0, 1, 14, 0, 0, 0
  );

  char zenyBuffer[64];
  std::memset(zenyBuffer, 0, sizeof(zenyBuffer));
  FormatGroupedInteger(g_session.m_zeny, zenyBuffer, 64);
  std::string zenyText = FormatOwnedString(LookupMessage(1997), zenyBuffer);
  zenyText = " | " + zenyText;
  this->DrawRightAlignedText(width - 5, this->layoutOffset + 103, zenyText.c_str(), 0, 1, 14, 0, 0);
  const int zenyWidth = this->TextWidth(zenyText.c_str(), 0, 1, 14, 0, 0
  );

  const int denominator = g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight;
  const std::uint32_t weightColor = 100 * g_session.m_weight / denominator >= g_unknown173A5BC ? 255 : 0;
  std::string weightText = g_serviceType == 12
    ? FormatOwnedString("Peso : %3d / %3d", g_session.m_weight, g_session.m_maxWeight)
    : FormatOwnedString(LookupMessage(1992), g_session.m_weight, g_session.m_maxWeight);
  this->DrawRightAlignedText(
    width - zenyWidth - 5, this->layoutOffset + 103, weightText.c_str(), 0, 1, 14, weightColor, 0
  );
}

// OFFSET: 0x00997C20
// FUNCTION: fn_13488
void UIBasicInfoWnd::DrawCollapsed() {
  const int width = this->geometry.width;
  const int height = this->geometry.height;

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 17, width - 2, height - 18, RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  DrawRoundedBorder(*this, 0, 14, width, height - 14, RGB(red, green, blue));
  this->DrawTitleBar(false, "", 0);

  int family = 1;
  int fontHeight = 14;
  if (g_serviceType == 0) {
    switch (g_session.GetDisplayJob()) {
    case 4046:
    case 4047:
    case 4048:
    case 4049:
    case 4239:
    case 4240:
    case 4241:
    case 4242:
    case 4243:
    case 4244:
    case 4302:
    case 4303:
    case 4316:
      family = 0;
      fontHeight = 11;
      break;
    }
  }

  std::string title = FormatOwnedString(
    "%s %d / %s / %s %d", LookupMessage(3070), g_session.m_level, g_session.GetJobName(g_session.GetDisplayJob(), -1), LookupMessage(3627),
    g_session.m_jobLevel
  );
  this->DrawTextAt(8, 20, title.c_str(), 0, family, fontHeight, 0, 0, 0);

  std::uint32_t hpColor = 0;
  if (g_session.unknown_556C > 0) {
    hpColor = 0.25f > static_cast<float>(g_session.unknown_5568) / static_cast<float>(g_session.unknown_556C) ? 255 : 0;
  }
  std::string hpText = FormatOwnedString("%s %d / %d", LookupMessage(3068), g_session.unknown_5568, g_session.unknown_556C);
  this->DrawTextAt(8, 36, hpText.c_str(), 0, family, fontHeight, hpColor, 0, 0);

  std::int64_t current = BaseExpCurrent();
  std::int64_t maximum = BaseExpMaximum();
  if (current < 0) {
    current = 0;
  }
  if (maximum <= 0) {
    maximum = current + 1;
  }
  double percent;
  if (g_session.m_level >= g_session.GetMaxBaseLevel(g_session.GetDisplayJob())) {
    percent = 100.0;
  } else {
    percent = static_cast<double>(
                static_cast<std::int64_t>(static_cast<double>(current) / static_cast<double>(maximum) * 1000.0)
              ) *
              0.1;
  }
  std::string expText = FormatOwnedString("| %s %.1lf%%", LookupMessage(3072), percent);
  this->DrawTextAt(136, 36, expText.c_str(), 0, family, fontHeight, 0, 0, 0);

  const int sp = *reinterpret_cast<const int*>(&g_session.unknown_5570[0]);
  const int spMax = *reinterpret_cast<const int*>(&g_session.unknown_5570[4]);
  std::uint32_t spColor = 0;
  if (spMax > 0) {
    spColor = 0.25f > static_cast<float>(sp) / static_cast<float>(spMax) ? 255 : 0;
  }
  std::string spText = FormatOwnedString("%s %d / %d", LookupMessage(3069), sp, spMax);
  this->DrawTextAt(8, 52, spText.c_str(), 0, family, fontHeight, spColor, 0, 0);

  std::string apText = FormatOwnedString("| %s %d / %d", LookupMessage(3628), g_session.GetTraitStat(232), g_session.GetTraitStat(233));
  this->DrawTextAt(136, 52, apText.c_str(), 0, family, fontHeight, 0, 0, 0);
}

// OFFSET: 0x009983C0
// FUNCTION: fn_13491
void UIBasicInfoWnd::Slot15(int width, int height) {
  this->AddBaseButton();

  UIBitmapButton* minimizeButton = this->AddMinimizeButton("Alt+V, Ctrl+V", 0);
  minimizeButton->Move(this->geometry.width - 14, 3);

  this->nameText = new UIStaticText(*this->m_context);
  this->nameText->SetSize(this->geometry.width - 20, 15);
  this->nameText->Move(9, 23);
  this->nameText->SetTextFitted(g_session.DecodePlayerName());
  this->nameText->SetFont(0, 12, 0, 0);
  this->AddChild(this->nameText);

  UIBarGraphPlayer* hpBar = new UIBarGraphPlayer(*this->m_context);
  hpBar->SetValues(
    []() -> std::int64_t { return g_session.unknown_5568; },
    []() -> std::int64_t { return g_session.unknown_556C; }
  );
  hpBar->SetSize(135, 9);
  hpBar->Move(-100, -100);
  this->AddChild(hpBar);
  this->hpBar = hpBar;

  UIBarGraphPlayer* spBar = new UIBarGraphPlayer(*this->m_context);
  spBar->SetValues(
    []() -> std::int64_t { return *reinterpret_cast<const int*>(&g_session.unknown_5570[0]); },
    []() -> std::int64_t { return *reinterpret_cast<const int*>(&g_session.unknown_5570[4]); }
  );
  spBar->SetSize(135, 9);
  spBar->Move(-100, -100);
  this->AddChild(spBar);
  this->spBar = spBar;

  UIBarGraphPlayerAP* apBar = new UIBarGraphPlayerAP(*this->m_context);
  apBar->SetValues(
    []() -> std::int64_t { return g_session.GetTraitStat(232); },
    []() -> std::int64_t { return g_session.GetTraitStat(233); }
  );
  apBar->SetSize(135, 9);
  apBar->Move(-100, -100);
  this->AddChild(apBar);
  this->apBar = apBar;

  UIINT64BarGraph* baseExpBar = new UIINT64BarGraph(*this->m_context);
  baseExpBar->m_plainColor = 1;
  baseExpBar->SetSize(112, 6);
  baseExpBar->Move(84, this->layoutOffset + 76);
  baseExpBar->SetValues(BaseExpCurrent(), BaseExpMaximum());
  this->AddChild(baseExpBar);
  this->baseExpBar = baseExpBar;

  UIINT64BarGraph* jobExpBar = new UIINT64BarGraph(*this->m_context);
  jobExpBar->m_plainColor = 1;
  jobExpBar->SetSize(112, 6);
  jobExpBar->Move(84, this->layoutOffset + 86);
  jobExpBar->SetValues(JobExpCurrent(), JobExpMaximum());
  this->AddChild(jobExpBar);
  this->jobExpBar = jobExpBar;

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
    unsigned red;
    unsigned green;
    unsigned blue;
    SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
    DrawRoundedBorder(*this, 0, 17, this->geometry.width, this->geometry.height - 17, RGB(red, green, blue));
    this->FillRegion(0, 17, this->geometry.width, 5, RGB(red, green, blue));
    SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    this->FillRegion(1, 17, this->geometry.width - 2, this->geometry.height - 19, RGB(red, green, blue));
    this->FillRegion(2, this->geometry.height - 2, this->geometry.width - 4, 1, RGB(red, green, blue));
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
      maxLevel = *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0]);
    } else if (g_session.IsExpandedJob(g_session.GetDisplayJob())) {
      maxLevel = *reinterpret_cast<const int*>(&g_session.unknown_7EF0[4]);
    } else {
      maxLevel = g_session.IsDoramJob(g_session.GetDisplayJob()) == true ? *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x28])
                                                                         : *reinterpret_cast<const int*>(g_session.unknown_7EE8);
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
        case 23:
        case 4046:
        case 4047:
        case 4048:
        case 4049:
        case 4239:
        case 4240:
        case 4241:
        case 4242:
        case 4243:
        case 4244:
        case 4302:
        case 4303:
        case 4316:
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
    if (g_session.unknown_556C != 0) {
      const std::uint32_t color = static_cast<float>(g_session.unknown_5568) * 100.0f / static_cast<float>(g_session.unknown_556C) < 25.0f ? 255 : 0;
      const int hpLength = std::sprintf(text, x == 9 ? "HP. %d / %d" : "HP %d / %d", g_session.unknown_5568, g_session.unknown_556C);
      this->DrawTextAt(x, 36, text, hpLength, 1, 14, color, 0, 0);
      offset = this->TextWidth(text, hpLength, 1, 14, 0, 0);
      length = std::sprintf(text, " | ");
      this->DrawTextAt(offset + x, 36, text, length, 1, 14, 0, 0, 0);
    }

    const int sp = *reinterpret_cast<const int*>(&g_session.unknown_5570[0]);
    const int spMax = *reinterpret_cast<const int*>(&g_session.unknown_5570[4]);
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
    this->DrawBitmap(0, 0, background ? &background->image : nullptr, false);

    int family;
    int fontHeight;
    if (g_serviceType == 0 || g_serviceType == 14) {
      family = 0;
      fontHeight = 12;
    } else {
      family = 1;
      fontHeight = 14;
    }
    this->DrawTextAt(9, 36, g_session.GetJobName(g_session.GetDisplayJob(), -1), 0, family, fontHeight, 0, 0, 0);

    std::memset(text, 0, sizeof(text));
    if (g_serviceType == 1) {
      this->DrawTextAt(18, 3, "Basic Info", 0, 0, 12, 0xFFFFFF, 0, 0);
      this->DrawTextAt(17, 2, "Basic Info", 0, 0, 12, 0, 0, 0);
    } else {
      this->DrawTextAt(18, 3, LookupMessage(238), 0, 0, 12, 0xFFFFFF, 0, 0);
      this->DrawTextAt(17, 2, LookupMessage(238), 0, 0, 12, 0, 0, 0);
    }

    this->DrawTextAt(17, 51, "HP", 0, 1, 14, 0, 0, 0);
    this->DrawTextAt(17, 65, "SP", 0, 1, 14, 0, 0, 0);

    if (g_session.unknown_556C != 0) {
      std::sprintf(text, "%d%%", g_session.unknown_5568 * 100 / g_session.unknown_556C);
      this->DrawTextAt(173, 51, text, 0, 1, 14, 0, 0, 0);
    }

    const int spMax = *reinterpret_cast<const int*>(&g_session.unknown_5570[4]);
    if (spMax != 0) {
      std::sprintf(text, "%d%%", *reinterpret_cast<const int*>(&g_session.unknown_5570[0]) * 100 / spMax);
      this->DrawTextAt(173, 65, text, 0, 1, 14, 0, 0, 0);
    }

    std::sprintf(text, g_serviceType == 12 ? "Base. %d" : "Base Lv. %d", g_session.m_level);
    this->DrawTextAt(17, this->layoutOffset + 71, text, 0, 1, 14, 0, 0, 0);
    std::sprintf(text, g_serviceType == 12 ? "Classe. %d" : "Job Lv. %d", g_session.m_jobLevel);
    this->DrawTextAt(17, this->layoutOffset + 83, text, 0, 1, 14, 0, 0, 0);

    std::memset(zenyText, 0, sizeof(zenyText));
    FormatGroupedInteger(g_session.m_zeny, zenyText, 64);
    std::sprintf(text, "Zeny : %s", zenyText);
    const int zenyWidth = this->TextWidth(text, std::strlen(text), 1, 14, 0, 0);
    this->DrawRightAlignedText(this->geometry.width - 5, this->layoutOffset + 103, text, std::strlen(text), 1, 14, 0, 0);

    std::sprintf(text, g_serviceType == 12 ? "Peso : %3d / %3d" : "Weight : %3d / %3d", g_session.m_weight, g_session.m_maxWeight);
    const int capacity = g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight;
    const std::uint32_t color = g_session.m_weight * 100 / capacity >= g_unknown173A5BC ? 255 : 0;
    this->DrawRightAlignedText(this->geometry.width - zenyWidth - 10, this->layoutOffset + 103, text, std::strlen(text), 1, 14, color, 0);
  }
}
