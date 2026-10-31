#include "UI/Window/UINewSkillListWnd.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/UserData/CharacterConfigSave.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIResizer.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace {
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };
}

// INLINED
static int SkillTabOf(int skillId) {
  if (skillId >= NV_BASIC && skillId <= ALL_RESURRECTION) {
    return 0;
  }
  if (skillId >= NV_FIRSTAID && skillId <= MG_ENERGYCOAT) {
    return 0;
  }
  if (skillId >= GS_GLITTERING && skillId <= NJ_ISSEN) {
    return 0;
  }
  if (skillId >= THIRDJOB_BEGIN && skillId <= THIRDJOB_END) {
    return 2;
  }
  if (skillId >= TK_RUN && skillId <= SG_FUSION) {
    return 0;
  }
  return 1;
}

// OFFSET: 0x009B3E70
// FUNCTION: fn_13826
void UINewSkillListWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

// OFFSET: 0x009B2C60
// FUNCTION: fn_13821
void UINewSkillListWnd::Slot35(int delta) {
  UIWindow* window = g_windowMgr.LookupWindow(46);
  if (window && window->geometry.visible) {
    static_cast<UINewSkillListWnd*>(window)->m_scrollBar->Slot35(delta);
    return;
  }

  if (this->m_scrollable) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, 7, step, 0, 0, 0);
  }
}

// OFFSET: 0x009B0370
// FUNCTION: fn_13805
std::string UINewSkillListWnd::ValueText(int id) {
  for (const NewSkillListValue& entry : this->m_values) {
    if (entry.id == id) {
      if (entry.value > 0) {
        char text[12];
        _itoa(entry.value, text, 10);
        return text;
      }

      break;
    }
  }

  return std::string();
}

// OFFSET: 0x009B0460
// FUNCTION: fn_13806
CSkillInfo UINewSkillListWnd::SkillAt(int index, bool live) {
  CSkillInfo result(0);
  int sequentialIndex = 0;
  const int gate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  for (const CSkillInfo& skill : this->SkillList(this->m_tab, live)) {
    int key = skill.unknown_0024;
    if (gate != 0) {
      key = sequentialIndex;
    }
    if (key == index) {
      return skill;
    }
    if (gate != 0) {
      ++sequentialIndex;
    }
  }
  return result;
}

// OFFSET: 0x009B0680
// FUNCTION: fn_13807
CSkillInfo UINewSkillListWnd::VisibleSkill(int row) {
  int index = 0;
  for (const CSkillInfo& skill : this->SkillList(this->m_tab, false)) {
    if (row + this->m_firstRow == index) {
      return skill;
    }
    ++index;
  }

  return CSkillInfo(0);
}

// OFFSET: 0x009B0750
// FUNCTION: fn_13808
std::pair<CSkillInfo*, bool> UINewSkillListWnd::SkillPtr(int index, bool live) {
  std::list<CSkillInfo>& list = this->SkillList(this->m_tab, live);
  if (*reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4) != 0) {
    if (index >= 0 && static_cast<size_t>(index) < list.size()) {
      auto it = list.begin();
      for (int i = 0; i < index; ++i) {
        ++it;
      }
      return {&*it, true};
    }
    return {nullptr, false};
  }

  for (CSkillInfo& skill : list) {
    if (skill.unknown_0024 == index) {
      return {&skill, true};
    }
  }

  return {nullptr, false};
}

// OFFSET: 0x009B07C0
// FUNCTION: fn_13809
std::list<CSkillInfo>& UINewSkillListWnd::SkillList(int tab, bool live) {
  if (tab >= 0 && static_cast<int>(this->m_tabs->labels.size()) - 1 == tab) {
    return g_session.m_skillBundle.extra;
  }

  if (live) {
    return g_session.m_skillBundle.Group(tab);
  }

  return this->m_lists[tab];
}

// OFFSET: 0x009B0820
// FUNCTION: fn_13810
void UINewSkillListWnd::InitTabs() {
  const int initialTabCount = this->m_tabs->labels.size();
  const std::uint8_t source[23] = {
    0xEF, 0xF2, 0x0F, 0xAA, 0x08, 0x4E, 0x1B, 0x6F, 0xF1, 0xFC, 0x0F, 0xA6,
    0x24, 0x42, 0x03, 0x57, 0xC4, 0xFF, 0x23, 0x98, 0x0E, 0x42, 0x77
  };
  thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> funcName(source);
  const char* func = funcName.Text();

  const char* defaultTabs[4] = {};
  const char* jobTabs[4] = {};
  g_luaScript->Call(func, "d>ssss", 0, &defaultTabs[0], &defaultTabs[1], &defaultTabs[2], &defaultTabs[3]);

  const unsigned baseJob = JobNames::BaseJob(g_session.GetDisplayJob());
  int jobKey = baseJob;
  switch (baseJob) {
  case 0:
  case 4001:
  case 4023:
    jobKey = 0;
    break;
  case 1:
  case 4002:
  case 4024:
    jobKey = 1;
    break;
  case 2:
  case 4003:
  case 4025:
    jobKey = 2;
    break;
  case 3:
  case 4004:
  case 4026:
    jobKey = 3;
    break;
  case 4:
  case 4005:
  case 4027:
    jobKey = 4;
    break;
  case 5:
  case 4006:
  case 4028:
    jobKey = 5;
    break;
  case 6:
  case 4007:
  case 4029:
    jobKey = 6;
    break;
  case 7:
  case 4008:
  case 4030:
  case 4054:
  case 4060:
  case 4096:
  case 4252:
    jobKey = 7;
    break;
  case 8:
  case 4009:
  case 4031:
  case 4057:
  case 4063:
  case 4099:
  case 4256:
    jobKey = 8;
    break;
  case 9:
  case 4010:
  case 4032:
  case 4055:
  case 4061:
  case 4097:
  case 4255:
    jobKey = 9;
    break;
  case 10:
  case 4011:
  case 4033:
  case 4058:
  case 4064:
  case 4100:
  case 4253:
    jobKey = 10;
    break;
  case 11:
  case 4012:
  case 4034:
  case 4056:
  case 4062:
  case 4098:
  case 4257:
    jobKey = 11;
    break;
  case 12:
  case 4013:
  case 4035:
  case 4059:
  case 4065:
  case 4101:
  case 4254:
    jobKey = 12;
    break;
  case 14:
  case 4015:
  case 4037:
  case 4066:
  case 4073:
  case 4102:
  case 4258:
    jobKey = 14;
    break;
  case 15:
  case 4016:
  case 4038:
  case 4070:
  case 4077:
  case 4106:
  case 4262:
    jobKey = 15;
    break;
  case 16:
  case 4017:
  case 4039:
  case 4067:
  case 4074:
  case 4103:
  case 4261:
    jobKey = 16;
    break;
  case 17:
  case 4018:
  case 4040:
  case 4072:
  case 4079:
  case 4108:
  case 4260:
    jobKey = 17;
    break;
  case 18:
  case 4019:
  case 4041:
  case 4071:
  case 4078:
  case 4107:
  case 4259:
    jobKey = 18;
    break;
  case 19:
  case 4020:
  case 4042:
  case 4068:
  case 4075:
  case 4104:
  case 4263:
    jobKey = 19;
    break;
  case 20:
  case 4021:
  case 4043:
  case 4069:
  case 4076:
  case 4105:
  case 4264:
    jobKey = 20;
    break;
  case 23:
  case 4045:
  case 4190:
  case 4191:
  case 4307:
    jobKey = 23;
    break;
  case 24:
  case 4215:
  case 4228:
  case 4229:
  case 4306:
    jobKey = 24;
    break;
  case 25:
  case 4211:
  case 4212:
  case 4222:
  case 4223:
  case 4224:
  case 4304:
  case 4305:
    jobKey = 25;
    break;
  case 4046:
  case 4225:
    jobKey = 4046;
    break;
  case 4047:
  case 4226:
  case 4239:
  case 4241:
  case 4302:
    jobKey = 4047;
    break;
  case 4049:
  case 4227:
  case 4240:
  case 4242:
  case 4303:
    jobKey = 4049;
    break;
  case 4218:
  case 4220:
  case 4308:
    jobKey = 4218;
    break;
  case 4351:
  case 4352:
  case 4353:
  case 4354:
  case 4355:
    jobKey = 4351;
    break;
  default:
    break;
  }

  g_luaScript->Call(func, "d>ssss", jobKey, &jobTabs[0], &jobTabs[1], &jobTabs[2], &jobTabs[3]);

  for (size_t i = this->m_tabs->labels.size(); i < 4; ++i) {
    if (g_session.m_skillBundle.GroupSize(i) > 0) {
      const char* name = jobTabs[i];
      if (!name) {
        name = defaultTabs[i];
      }
      this->m_tabs->AddTab(name, "");
      this->m_tab = i;
    }
  }

  if (initialTabCount == 0) {
    this->m_tabs->AddTab(LookupMessage(1093), "");
  }
  this->m_tabs->unknown_007C = this->m_tab;
}

// OFFSET: 0x009B0E40
// FUNCTION: fn_13811
void UINewSkillListWnd::ResetLists() {
  this->m_locked = 0;
  this->m_skillPoints = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0x165C);
  for (int i = 0; i < 4; ++i) {
    this->m_lists[i] = g_session.m_skillBundle.Group(i);
  }
  this->m_unknown_02A0.clear();
  g_session.m_skillBundle.MarkLevelChanges();
}

// OFFSET: 0x009B0EE0
// FUNCTION: fn_13812
bool UINewSkillListWnd::IsExtraTab(int tab) {
  return tab >= 0 && static_cast<int>(this->m_tabs->labels.size()) - 1 == tab;
}

// OFFSET: 0x009B0FA0
// FUNCTION: fn_13813
bool UINewSkillListWnd::IsSkillArea(int x, int y) {
  int minX = 76;
  int rowHeight = 56;
  int yOffset = 40;
  const int gate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  if (gate != 0) {
    rowHeight = 32;
    minX = 40;
    yOffset = 30;
  }
  if (y < this->m_topMargin || y >= this->m_visibleRows * rowHeight + 4 + yOffset) {
    return false;
  }
  if (x < minX) {
    return false;
  }
  if (gate == 0 && x > 504) {
    return false;
  }
  return true;
}

// OFFSET: 0x009B1050
// FUNCTION: fn_13814
void UINewSkillListWnd::Slot15(int width, int height) {
  this->m_tabs = new UITabControl(*this->m_context, *this->m_controls);
  reinterpret_cast<void(__thiscall*)(UITabControl*, int, int, int, int, int, int, int, int, int, int, int, int)>(0x0087C3F0)(
    this->m_tabs, 0, 17, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->m_tabs->Move(0, 6);
  this->AddChild(this->m_tabs);

  this->m_btnHelp = this->AddBaseButton(*this->m_controls);
  this->m_btnMinimize = this->AddMinimizeButton(*this->m_controls, "", 0);
  this->m_btnClose = this->AddCloseButton(*this->m_controls, "Alt+S", 0);

  this->m_btnHelp->Move(22, 5);
  this->m_btnMinimize->Move(this->geometry.width - 30, 5);
  this->m_btnClose->Move(this->geometry.width - 16, 5);

  this->m_scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar->Configure(this->geometry.width - 15, 20, 1, this->geometry.height - this->m_topMargin - this->m_bottomMargin);
  this->m_scrollBar->Move(this->geometry.width - 15, 20);
  this->m_scrollBar->SetSize(this->m_scrollBar->geometry.width, this->geometry.height - this->m_topMargin - this->m_bottomMargin);
  this->AddChild(this->m_scrollBar);

  for (int i = 0; i < 10; ++i) {
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap("유저인터페이스\\basic_interface\\skill_up_a.bmp", 0, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\skill_up_b.bmp", 1, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\skill_up_c.bmp", 2, 0);
    button->SetSize(button->geometry.width, button->geometry.height);
    button->Move(78, 21 + i * 32);
    button->SetId(i + 239);
    this->AddChild(button);
    this->m_btnLevelUp[i] = button;
  }

  for (int i = 0; i < 42; ++i) {
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 0, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 1, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 2, 0);
    button->SetSize(button->geometry.width, button->geometry.height);
    button->Move(106, 34 + i * 32);
    button->SetId(i);
    this->AddChild(button);
    this->m_btnSkills[i] = button;
  }

  for (int i = 0; i < 42; ++i) {
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 0, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 1, 0);
    button->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 2, 0);
    button->SetSize(button->geometry.width, button->geometry.height);
    button->Move(198, 34 + i * 32);
    button->SetId(i + 42);
    this->AddChild(button);
    this->m_btnSkillUps[i] = button;
  }

  const char* baseDir = "유저인터페이스\\";
  const char* applyNames[3] = {"btn_apply", "btn_apply_a", "btn_apply_b"};
  this->m_btnApply = new UIBitmapButton(*this->m_context, *this->m_controls);
  for (int state = 0; state < 3; ++state) {
    std::string path = std::string(baseDir) + applyNames[state] + ".bmp";
    this->m_btnApply->SetBitmap(Resolve(path.c_str()), state, 0);
  }
  this->m_btnApply->SetSize(this->m_btnApply->geometry.width, this->m_btnApply->geometry.height);
  this->m_btnApply->Move(this->geometry.width - 100, this->geometry.height - 27);
  this->m_btnApply->SetId(271);
  this->m_btnApply->SetDisabled(1);
  this->m_btnApply->Invalidate();
  this->AddChild(this->m_btnApply);

  const char* resetNames[3] = {"btn_reset", "btn_reset_a", "btn_reset_b"};
  this->m_btnCancel = new UIBitmapButton(*this->m_context, *this->m_controls);
  for (int state = 0; state < 3; ++state) {
    std::string path = std::string(baseDir) + resetNames[state] + ".bmp";
    this->m_btnCancel->SetBitmap(Resolve(path.c_str()), state, 0);
  }
  this->m_btnCancel->SetSize(this->m_btnCancel->geometry.width, this->m_btnCancel->geometry.height);
  this->m_btnCancel->Move(this->geometry.width - 50, this->geometry.height - 27);
  this->m_btnCancel->SetId(363);
  this->m_btnCancel->SetDisabled(1);
  this->m_btnCancel->Invalidate();
  this->AddChild(this->m_btnCancel);

  this->m_btnDetail = new UIResizer(*this->m_context);
  this->m_btnDetail->SetSize(16, 16);
  this->m_btnDetail->SetDirection(3);
  this->m_btnDetail->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->m_btnDetail->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->m_btnDetail->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->m_btnDetail);

  this->InitTabs();
  if (!g_session.m_skillBundle.extraIds.empty()) {
    this->m_tabs->SetFlag(static_cast<int>(this->m_tabs->labels.size()) - 1, true);
  }
  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x009B2160
// FUNCTION: fn_13815
void UINewSkillListWnd::Paint() {
  auto& controls = *this->m_controls;
  const int left = this->m_tabs->geometry.width;
  this->ClearSurface(0xFFFF00FF);
  this->FillRegion(left, this->m_topMargin, this->geometry.width - left, this->geometry.height - this->m_bottomMargin - this->m_topMargin, 0xFFFFFF);
  DrawRoundedBorder(*this, left, 0, this->geometry.width - left, this->geometry.height, 0xC3C3C3);
  DrawRoundedBorder(*this, left + 2, 2, this->geometry.width - left - 4, this->geometry.height - 4, 0xC3C3C3);

  CBitmapRes* bitmap =
    static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_upbar_left.bmp")));
  this->DrawBitmap(left, 0, &bitmap->image, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_upbar_right.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, 0, &bitmap->image, false);

  int span = this->geometry.width - left - 24;
  int count = span / 5 + (span % 5 != 0);
  for (int i = 0, x = left + 12; i < count; ++i, x += 5) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_upbar_mid.bmp")));
    this->DrawBitmap(x, 0, &bitmap->image, false);
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_botbar_left.bmp")));
  this->DrawBitmap(left, this->geometry.height - bitmap->image.height, &bitmap->image, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_botbar_right.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, this->geometry.height - bitmap->image.height, &bitmap->image, false);

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\skill_botbar_mid.bmp")));
  span = this->geometry.width - left - 24;
  count = span / bitmap->image.width + (span % bitmap->image.width != 0);
  for (int i = 0, x = left + 12; i < count; ++i, x += 5) {
    this->DrawBitmap(x, this->geometry.height - bitmap->image.height, &bitmap->image, false);
  }

  this->DrawTextAt(controls.fonts, controls.locale, 41, 6, LookupMessage(283), 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 40, 5, LookupMessage(283), 0, 0, 12, 0, 0, 0);

  const int points = this->m_skillPoints;
  char text[80];
  std::memset(text, 0, sizeof(text));
  if (*reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4) == 0) {
    const int total = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0x165C);
    if (points < total) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%s^0000ff%d^000000(^ff0000%d^000000) / %d", LookupMessage(2343), points, total - points,
                   total);
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%s%d", LookupMessage(2343), total);
    }
    this->sub_A84400(80, this->geometry.height - 23, std::string(text), 0, 0, 0, 0, 12, 0, 0);
    this->sub_9AF960();
    this->sub_9AE930();
  } else {
    std::sprintf(text, LookupMessage(284), points);
    this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0);
    this->DrawTextAt(controls.fonts, controls.locale, 30, this->geometry.height - 23, text, 0, 0, 12, 0, 0, 0);
    this->sub_9AF300();
  }

  this->m_needsRedraw = 0;
}

// OFFSET: 0x009B2590

// OFFSET: 0x009B26D0
// FUNCTION: fn_13817
void UINewSkillListWnd::Slot25(int x, int y) {
  if (y < this->m_topMargin) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (this->m_showDetail) {
    this->m_showDetail = false;
    this->sub_9B2CD0();
  }

  int oldSelected = this->m_selectedSkill;
  this->m_selectedSkill = this->SkillIndexAt(x, y);
  if (oldSelected != this->m_selectedSkill) {
    this->Invalidate();
  }

  if (this->IsSkillArea(x, y) && !this->m_locked && this->m_selectedSkill != -1 && g_replayAnimationScaling == 0) {
    CSkillInfo skill = this->SkillAt(this->m_selectedSkill, false);
    if (skill.available != 0 && skill.level != 0 && skill.unknown_000C != 0) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (gameMode) {
        int level = reinterpret_cast<CharacterConfigSave*>(&g_session)->UseLevel(skill.id);
        if (level == 0) {
          level = skill.level;
        }
        gameMode->drag.BeginSkill(reinterpret_cast<WindowManagerWindows&>(g_windowMgr), ResourceMgr::Instance(), 8, skill.id, skill.name, level, -1);
      }
    }
  }
}

// OFFSET: 0x009B2810
// FUNCTION: fn_13818
void UINewSkillListWnd::Slot31(int x, int y) {
  if (y < this->m_topMargin) {
    UIFrameWnd::Slot31(x, y);
    this->Slot11();
    return;
  }

  const int gate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  if (gate == 0 && this->IsSkillArea(x, y)) {
    CSkillInfo skill = this->SkillAt(this->m_selectedSkill, false);
    if (skill.unknown_002C != 0 && skill.available != 0) {
      if (this->sub_9B3EB0(skill.id)) {
        this->m_needsRedraw = 1;
        this->m_locked = 1;
        this->LayoutButtons();
        this->Invalidate();
      }
    }
  }
}

// OFFSET: 0x009B28F0
// FUNCTION: fn_13819
void UINewSkillListWnd::Slot28(int x, int y) {
  const int gate = *reinterpret_cast<const int*>(0x017326EC);
  if (!this->IsSkillArea(x, y) || (gate != 0 && x > 96)) {
    return;
  }

  const int previous = this->m_selectedSkill;
  this->m_selectedSkill = this->SkillIndexAt(x, y);
  if (this->m_selectedSkill != -1) {
    std::pair<CSkillInfo*, bool> skill = this->SkillPtr(this->m_selectedSkill, false);
    if (skill.second && skill.first->available != 0) {
      int tooltipX = -10;
      int tooltipY = -20;
      this->ToScreen(tooltipX, tooltipY);
      const char* name = skill.first->DisplayName(*g_luaScript);
      (*reinterpret_cast<ModeTooltip**>(0x013495BC))->Show(name, tooltipX + x, tooltipY + y, -1, 0, 1);
    }
  }

  if (previous != this->m_selectedSkill) {
    if (!this->m_showDetail) {
      this->sub_9B2CD0();
    }
    this->m_needsRedraw = 1;
    this->Invalidate();
  }
}

// OFFSET: 0x009B29E0
// FUNCTION: fn_13820
void UINewSkillListWnd::Slot33(int x, int y) {
  this->m_selectedSkill = this->SkillIndexAt(x, y);
  if (this->m_selectedSkill == -1) {
    this->m_showDetail = false;
    this->m_values.clear();
    for (int i = 0; i < static_cast<int>(this->m_tabs->labels.size()); ++i) {
      this->m_tabs->colors[i] = 0xFFFFFFFF;
      this->m_tabs->Invalidate();
    }
  } else if (this->m_detailId == this->m_selectedSkill) {
    this->m_showDetail = !this->m_showDetail;
  } else {
    this->m_showDetail = true;
    this->sub_9B2CD0();
  }

  if (this->m_showDetail) {
    this->m_detailId = this->m_selectedSkill;
  }
  this->m_needsRedraw = 1;
  this->Invalidate();

  std::pair<CSkillInfo*, bool> found = this->SkillPtr(this->m_selectedSkill, false);
  if (!found.second) {
    return;
  }
  CSkillInfo* skill = found.first;

  UISkillDescribeWnd* current = static_cast<UISkillDescribeWnd*>(g_windowMgr.LookupWindow(46));
  if (current && (current->m_skillId == static_cast<int>(skill->id) || skill->available == 0)) {
    g_windowMgr.CloseWindow(46);
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(46);
  if (window) {
    window->SendMsg(nullptr, 61, skill->id, skill->unknown_0018, skill->level, 1);
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);
    x += screenX + 24;
    y += screenY;
    if (g_renderer->viewport.width < window->geometry.width + x) {
      x += -48 - window->geometry.width;
    }
    int top = y - window->geometry.height;
    if (window->geometry.height + y <= g_renderer->viewport.height) {
      top = y;
    }
    if (top < 0) {
      top = 0;
    }
    window->Move(x, top);
    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(46, 1);
  }
}

// OFFSET: 0x009B2CD0
// FUNCTION: fn_13822
void UINewSkillListWnd::sub_9B2CD0() {
  this->m_values.clear();
  for (int i = 0; i < static_cast<int>(this->m_tabs->labels.size()); ++i) {
    this->m_tabs->colors[i] = 0xFFFFFFFF;
    this->m_tabs->Invalidate();
  }

  for (CSkillInfo& skill : this->SkillList(this->m_tab, false)) {
    const int key = static_cast<int>(skill.unknown_0024);
    if (key != -1 && this->m_firstRow * this->m_columns <= key && key == this->m_selectedSkill) {
      this->m_values.push_back({static_cast<int>(skill.id), 0});
      for (SkillInfoEntry& entry : skill.entries) {
        NewSkillListValue required{static_cast<int>(entry.unknown_0000), static_cast<int>(entry.unknown_0004)};
        const int tab = SkillTabOf(required.id);
        if (tab != this->m_tab && tab < static_cast<int>(this->m_tabs->labels.size())) {
          this->m_tabs->colors[tab] = 0xE1E7FC;
          this->m_tabs->Invalidate();
        }

        for (auto it = this->m_values.begin(); it != this->m_values.end(); ++it) {
          if (it->id == required.id) {
            if (required.value < it->value) {
              required.value = it->value;
            }
            this->m_values.erase(it);
            break;
          }
        }
        entry.unknown_0004 = required.value;
        this->m_values.push_back(required);
      }
      return;
    }
  }
}

// OFFSET: 0x009B3040
// FUNCTION: fn_13823
void UINewSkillListWnd::UpdateScrollBar() {
  const int initialGate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  if (initialGate == 0) {
    this->m_columns = 7;
    this->m_visibleRows = 6;
  } else {
    this->m_columns = 1;
    this->m_visibleRows = (this->geometry.height - this->m_bottomMargin - 30) / 32;
  }

  if (!this->m_scrollBar) {
    return;
  }

  this->m_scrollBar->Move(this->geometry.width - 15, 20);

  const int gate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  const std::list<CSkillInfo>& list = this->SkillList(this->m_tab, false);
  int total = 0;
  if (gate == 0) {
    int maxKey = 0;
    for (const CSkillInfo& skill : list) {
      const int key = static_cast<int>(skill.unknown_0024);
      if (key > maxKey) {
        maxKey = key;
      }
    }

    const int count = maxKey + 1;
    total = count / this->m_columns + (count % this->m_columns != 0);
  } else {
    const int count = static_cast<int>(list.size());
    total = count / this->m_columns + (count % this->m_columns != 0);
  }

  int maxScroll = total - this->m_visibleRows;
  if (maxScroll < 0) {
    maxScroll = 0;
  }
  this->m_scrollable = maxScroll != 0;
  if (maxScroll == 0) {
    this->m_firstRow = 0;
    this->m_scrollBar->SetRange(0, this->m_visibleRows, total);
  } else {
    this->m_scrollBar->SetRange(maxScroll, this->m_visibleRows, total);
    if (this->m_firstRow < 0) {
      this->m_firstRow = 0;
    }
    if (this->m_firstRow > total - this->m_visibleRows) {
      this->m_firstRow = total - this->m_visibleRows;
    }
    this->m_scrollBar->SetPosition(this->m_firstRow);
  }

  this->m_scrollBar->Invalidate();
  this->m_needsRedraw = 1;
}

// OFFSET: 0x009B31C0
// FUNCTION: fn_13824
void UINewSkillListWnd::LayoutButtons() {
  int inset = 0;
  const int gate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  if (gate == 0) {
    this->m_btnDetail->Move(-100, -100);
  } else {
    this->m_selectedSkill = -1;
    inset = 15;
    this->m_btnDetail->Move(this->geometry.width - 15, this->geometry.height - 15);
  }

  this->m_btnMinimize->Move(this->geometry.width - 30, 5);
  this->m_btnClose->Move(this->geometry.width - 16, 5);
  this->m_btnApply->Move(this->geometry.width - inset - 100, this->geometry.height - 27);
  this->m_btnCancel->Move(this->geometry.width - inset - 50, this->geometry.height - 27);
  this->m_btnApply->SetDisabled(!this->m_locked);
  this->m_btnApply->Invalidate();
  this->m_btnCancel->SetDisabled(!this->m_locked);
  this->m_btnCancel->Invalidate();
  if (!this->m_locked) {
    this->m_btnApply->SetState(0);
    this->m_btnCancel->SetState(0);
  }

  for (int i = 0; i < 10; ++i) {
    this->m_btnLevelUp[i]->Move(-200, -200);
  }
  for (int i = 0; i < 42; ++i) {
    this->m_btnSkills[i]->Move(-200, -200);
    this->m_btnSkillUps[i]->Move(-200, -200);
  }

  std::list<CSkillInfo>& list = this->SkillList(this->m_tab, false);
  if (*reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4) == 0) {
    if (!this->m_locked) {
      for (const CSkillInfo& skill : list) {
        const unsigned int slot = skill.unknown_0024 - this->m_columns * this->m_firstRow;
        if (slot < 42 && skill.available != 0 && skill.unknown_0024 != 0xFFFFFFFF && skill.level != 0 &&
            CSkillInfo::UsesLevel(*g_luaScript, skill.id)) {
          const int y = static_cast<int>(slot) / this->m_columns * 56 + 70;
          const int x = static_cast<int>(slot) % this->m_columns * 68;
          this->m_btnSkillUps[slot]->Move(x + 111, y);
          this->m_btnSkills[slot]->Move(x + 56, y);
        }
      }
    }
    return;
  }

  for (int row = 0; row < 10; ++row) {
    const int y = row * 32 + 48;
    CSkillInfo skill = this->VisibleSkill(row);
    if (skill.level < static_cast<int>(skill.unknown_0028) && row < this->m_visibleRows &&
        static_cast<unsigned int>(this->m_firstRow + row) < list.size() && this->m_skillPoints != 0 && skill.available != 0 &&
        skill.unknown_0018 != 0) {
      this->m_btnLevelUp[row]->Move(78, y - 14);
    }
    if (!this->m_locked && row < this->m_visibleRows && static_cast<unsigned int>(this->m_firstRow + row) < list.size() &&
        skill.available != 0 && skill.level != 0 && CSkillInfo::UsesLevel(*g_luaScript, skill.id)) {
      this->m_btnSkills[row]->Move(112, y);
      this->m_btnSkillUps[row]->Move(192, y);
    }
  }
}

// OFFSET: 0x009B3580
// FUNCTION: fn_13825
int UINewSkillListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  int& gate = *reinterpret_cast<int*>(reinterpret_cast<char*>(&g_session) + 0xB4);
  CMode* gameMode = g_modeMgr.gameMode;

  switch (message) {
  case 6:
    switch (a) {
    case 201:
      g_windowMgr.CloseWindow(37);
      return 0;
    case 202:
      this->Slot11();
      this->m_needsRedraw = 1;
      gate = gate == 0;
      if (gate != 0) {
        this->SendMsg(nullptr, 14, 3, 330, this->m_saved->height, 0);
      } else {
        this->SendMsg(nullptr, 14, 3, 560, 400, 0);
      }
      return 0;
    case 213: {
      int& flag = *reinterpret_cast<int*>(reinterpret_cast<char*>(&g_session) + 0xB8);
      flag = flag == 0;
      return 0;
    }
    case 216: {
      const int tab = this->m_tab;
      int next = 0;
      if (tab + 1 != this->m_tabs->TabCount()) {
        next = tab + 1;
      }
      this->m_tabs->SelectTab(next);
      return 0;
    }
    case 239:
    case 240:
    case 241:
    case 242:
    case 243:
    case 244:
    case 245:
    case 246:
    case 247:
    case 248:
      if (this->m_skillPoints != 0) {
        CSkillInfo skill = this->SkillAt(this->m_columns * this->m_firstRow + a - 239, false);
        if (skill.available != 0 && skill.unknown_0018 != 0 && this->sub_9B3EB0(skill.id)) {
          this->m_needsRedraw = 1;
          this->m_locked = 1;
          this->LayoutButtons();
          this->Invalidate();
        }
        return 0;
      }
      break;
    case 271:
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1377), 2, false, false, 0, "Message", 280, 120, 0) == 187) {
        this->sub_9AE730();
        this->SendMsg(nullptr, 23, 0, 0, 0, 0);
        if (g_chatSuggestions) {
          InputFocus focus{g_windowMgr.hoverWindow};
          focus.Set(g_chatSuggestions->window);
          return 0;
        }
      }
      break;
    case 363:
      this->ResetLists();
      this->SendMsg(nullptr, 23, 0, 0, 0, 0);
      return 0;
    default: {
      g_session.SetOption(7, 1, 0);
      const unsigned int offset = static_cast<unsigned int>(a) - 42;
      const unsigned int slot = offset > 41 ? static_cast<unsigned int>(a) : offset;
      CSkillInfo skill = this->SkillAt(this->m_columns * this->m_firstRow + slot, false);
      if (skill.available != 0) {
        const int current = reinterpret_cast<CharacterConfigSave*>(&g_session)->UseLevel(skill.id);
        int level = offset > 41 ? current - 1 : current + 1;
        if (level <= 0) {
          level = 1;
        }
        if (level > skill.level) {
          level = skill.level;
        }
        g_session.m_skillBundle.SetUnknownValue(skill.id, level);
        this->m_needsRedraw = 1;
        this->Invalidate();
      }
      return 0;
    }
    }
    break;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_firstRow;
    int next;
    if (message == 9) {
      next = previous - this->m_visibleRows + 1;
    } else {
      if (message == 10) {
        a = this->m_visibleRows - 1;
      }
      next = a + previous;
    }
    this->m_firstRow = next;
    if (previous != next) {
      this->UpdateScrollBar();
      this->LayoutButtons();
      this->Invalidate();
      next = this->m_firstRow;
    }
    return next - previous;
  }
  case 14:
    if (a == 3) {
      if (gate != 0) {
        const int minimum = this->m_tabs->geometry.height + 12;
        if (c < minimum) {
          c = minimum;
        }
        if (c > 400) {
          c = 400;
        }
        c = (c - 400) / 32 * 32 + 400;
      }
      if (this->geometry.width != b || c != this->geometry.height) {
        this->Resize(b, c);
        this->m_scrollBar->Resize(this->m_scrollBar->geometry.width, this->geometry.height - this->m_bottomMargin - this->m_topMargin);
        this->UpdateScrollBar();
        this->LayoutButtons();
        this->Invalidate();
      }
      this->Move(this->m_saved->x, this->m_saved->y);
      this->m_frameContext->snapping.Store(this);
      return 0;
    }
    break;
  case 22: {
    std::list<CSkillInfo>& list = this->SkillList(a, false);
    if (!this->IsExtraTab(a) && list.size() == 0) {
      this->m_tabs->unknown_007C = this->m_tab;
      return 0;
    }
    if (this->m_tab != a) {
      this->m_selectedSkill = -1;
      this->m_tab = a;
      this->m_needsRedraw = 1;
      if (this->IsExtraTab(a) == true) {
        this->m_tabs->SetFlag(a, false);
        g_session.m_skillBundle.extraIds.clear();
      }
    }
    this->UpdateScrollBar();
    this->LayoutButtons();
    this->Invalidate();
    return 0;
  }
  case 23:
    this->ResetLists();
    g_session.m_skillBundle.NumberExtra();
    this->UpdateScrollBar();
    this->LayoutButtons();
    this->Invalidate();
    return 0;
  case 34:
    this->m_saved = reinterpret_cast<SkillListWndInfo*>(a);
    if (this->m_saved->IsValid() == 0) {
      this->m_saved->Reset();
    }
    if (gate != 0) {
      this->SendMsg(nullptr, 14, 3, 330, this->m_saved->height, 0);
    } else {
      this->SendMsg(nullptr, 14, 3, 560, 400, 0);
    }
    return 0;
  case 58: {
    const CSkillInfo* skill = reinterpret_cast<const CSkillInfo*>(a);
    if (skill->level != 0) {
      gameMode->SendMsg(113, a, reinterpret_cast<CharacterConfigSave*>(&g_session)->UseLevel(skill->id), 0, 0);
      return 0;
    }
    break;
  }
  case 60:
    this->InitTabs();
    return 0;
  case 161:
    if (!this->IsExtraTab(this->m_tab)) {
      this->m_tabs->SetFlag(this->m_tabs->TabCount() - 1, !g_session.m_skillBundle.extraIds.empty());
      this->m_tabs->Invalidate();
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x009B3EB0
// FUNCTION: fn_13827
bool UINewSkillListWnd::sub_9B3EB0(int skillId) {
  if (this->m_skillPoints <= 0 || this->m_showDetail) {
    return false;
  }

  const int gate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  if (gate != 0) {
    std::list<CSkillInfo>& list = this->SkillList(this->m_tab, false);
    for (CSkillInfo& skill : list) {
      if (static_cast<int>(skill.id) == skillId) {
        if (skill.level >= static_cast<int>(skill.unknown_0028)) {
          return false;
        }
        ++skill.level;
        --this->m_skillPoints;
        return true;
      }
    }
    return false;
  }

  for (auto it = this->m_values.begin(); it != this->m_values.end(); ++it) {
    if (this->m_skillPoints <= 0) {
      break;
    }

    const int id = it->id;
    CSkillInfo* info = g_session.m_skillBundle.FindGrouped(id);
    if (info == nullptr || info->available == 0) {
      continue;
    }

    int target;
    if (id == skillId) {
      int current = 0;
      for (const NewSkillListValue& value : this->m_unknown_02A0) {
        if (value.id == id) {
          current = value.value;
          break;
        }
      }
      target = (std::max)(static_cast<int>(info->unknown_0030), current) + 1;
      if (static_cast<int>(info->unknown_0030) >= static_cast<int>(info->unknown_0028)) {
        return false;
      }
    } else {
      target = it->value;
    }

    if (target > static_cast<int>(info->unknown_0028)) {
      continue;
    }

    int current = 0;
    for (const NewSkillListValue& value : this->m_unknown_02A0) {
      if (value.id == id) {
        current = value.value;
        break;
      }
    }
    current = (std::max)(static_cast<int>(info->unknown_0030), current);
    if (current >= target) {
      continue;
    }

    if (static_cast<int>(info->unknown_002C) <= 0) {
      const char* name = info->DisplayName(*g_luaScript);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatString(LookupMessage(0xDCB), name)), 0xFF, 0, 0, 0);
      return false;
    }

    const int added = (std::min)(target - current, this->m_skillPoints);
    this->m_skillPoints -= added;
    const int level = current + added;

    bool found = false;
    for (NewSkillListValue& value : this->m_unknown_02A0) {
      if (value.id == id) {
        value.value = level;
        found = true;
        break;
      }
    }
    if (!found) {
      this->m_unknown_02A0.push_back(NewSkillListValue{id, level});
    }
  }

  return true;
}
