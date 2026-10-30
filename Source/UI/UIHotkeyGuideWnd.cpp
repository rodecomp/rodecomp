#include "UI/UIHotkeyGuideWnd.h"

#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

// OFFSET: 0x009663D0
// FUNCTION: fn_13154
UIHotkeyGuideWnd::UIHotkeyGuideWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
  this->m_current = 4698;
}

// OFFSET: 0x009664F0
// FUNCTION: fn_13156
HotkeyGuideLabel::HotkeyGuideLabel(std::string text, HotkeyGuideKey key, HotkeyGuideKey altKey) {
  this->text = text;
  this->key = key;
  this->altKey = altKey;
}

// OFFSET: 0x00966600
// FUNCTION: fn_13157
HotkeyGuideKey::HotkeyGuideKey(int x, int y, unsigned char align, unsigned char width, unsigned char padding, unsigned char height) {
  this->align = align;
  this->width = width;
  this->padding = padding;
  this->height = height;
  this->x = x;
  this->y = y;
  this->valid = true;
}

// OFFSET: 0x00966630
// FUNCTION: fn_13158
HotkeyGuideKey::HotkeyGuideKey() : valid(false), x(0), y(0) {
}

// OFFSET: 0x009669D0
// FUNCTION: fn_13160
HotkeyGuideLabel::~HotkeyGuideLabel() {
}

// OFFSET: 0x00966C70
// FUNCTION: fn_13164
void UIHotkeyGuideWnd::Slot15(int, int) {
  UIButtonContext& controls = *this->m_controls;
  UIBitmapButton* base = new UIBitmapButton(*this->m_context, controls);
  base->SetBitmap("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0, 0);
  base->SetBitmap("유저인터페이스\\basic_interface\\sys_base_on.bmp", 1, 0);
  base->SetBitmap("유저인터페이스\\basic_interface\\sys_base_on.bmp", 2, 0);
  base->SetSize(base->BitmapWidth(), base->BitmapHeight());
  base->Move(3, 3);
  this->AddChild(base);
  this->AddCloseButton(controls, "", 0);

  this->m_labels[8] = HotkeyGuideLabel("←", HotkeyGuideKey(528, 93, 0, 75, 15, 36), HotkeyGuideKey());
  this->m_labels[9] = HotkeyGuideLabel("Tab", HotkeyGuideKey(21, 132, 0, 55, 15, 36), HotkeyGuideKey());
  this->m_labels[13] = HotkeyGuideLabel("Enter", HotkeyGuideKey(528, 171, 2, 75, 20, 36), HotkeyGuideKey(876, 210, 6, 36, 0, 75));
  this->m_labels[16] = HotkeyGuideLabel("Shift", HotkeyGuideKey(21, 210, 0, 94, 15, 36), HotkeyGuideKey(508, 210, 2, 95, 20, 36));
  this->m_labels[17] = HotkeyGuideLabel("Ctrl", HotkeyGuideKey(21, 249, 0, 55, 15, 36), HotkeyGuideKey());
  this->m_labels[18] = HotkeyGuideLabel("Alt", HotkeyGuideKey(118, 249, 6, 49, 0, 36), HotkeyGuideKey());
  this->m_labels[19] = HotkeyGuideLabel("Pause", HotkeyGuideKey(702, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[20] = HotkeyGuideLabel("Caps", HotkeyGuideKey(21, 171, 0, 75, 15, 36), HotkeyGuideKey());
  this->m_labels[25] = HotkeyGuideLabel(LookupMessage(3147), HotkeyGuideKey(547, 249, 6, 56, 0, 36), HotkeyGuideKey());
  this->m_labels[27] = HotkeyGuideLabel("Esc", HotkeyGuideKey(21, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[32] = HotkeyGuideLabel("Space", HotkeyGuideKey(170, 249, 6, 246, 0, 36), HotkeyGuideKey());
  this->m_labels[33] = HotkeyGuideLabel("PgUp", HotkeyGuideKey(702, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[34] = HotkeyGuideLabel("PgDn", HotkeyGuideKey(702, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[35] = HotkeyGuideLabel("End", HotkeyGuideKey(663, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[36] = HotkeyGuideLabel("Home", HotkeyGuideKey(663, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[37] = HotkeyGuideLabel("←", HotkeyGuideKey(624, 249, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[38] = HotkeyGuideLabel("↑", HotkeyGuideKey(663, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[39] = HotkeyGuideLabel("→", HotkeyGuideKey(702, 249, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[40] = HotkeyGuideLabel("↓", HotkeyGuideKey(663, 249, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[44] = HotkeyGuideLabel("PrtSc", HotkeyGuideKey(624, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[45] = HotkeyGuideLabel("Insert", HotkeyGuideKey(624, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[46] = HotkeyGuideLabel("Delete", HotkeyGuideKey(624, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[48] = HotkeyGuideLabel("0", HotkeyGuideKey(411, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[49] = HotkeyGuideLabel("1", HotkeyGuideKey(60, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[50] = HotkeyGuideLabel("2", HotkeyGuideKey(99, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[51] = HotkeyGuideLabel("3", HotkeyGuideKey(138, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[52] = HotkeyGuideLabel("4", HotkeyGuideKey(177, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[53] = HotkeyGuideLabel("5", HotkeyGuideKey(216, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[54] = HotkeyGuideLabel("6", HotkeyGuideKey(255, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[55] = HotkeyGuideLabel("7", HotkeyGuideKey(294, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[56] = HotkeyGuideLabel("8", HotkeyGuideKey(333, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[57] = HotkeyGuideLabel("9", HotkeyGuideKey(372, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[65] = HotkeyGuideLabel("A", HotkeyGuideKey(99, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[66] = HotkeyGuideLabel("B", HotkeyGuideKey(274, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[67] = HotkeyGuideLabel("C", HotkeyGuideKey(196, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[68] = HotkeyGuideLabel("D", HotkeyGuideKey(177, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[69] = HotkeyGuideLabel("E", HotkeyGuideKey(157, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[70] = HotkeyGuideLabel("F", HotkeyGuideKey(216, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[71] = HotkeyGuideLabel("G", HotkeyGuideKey(255, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[72] = HotkeyGuideLabel("H", HotkeyGuideKey(294, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[73] = HotkeyGuideLabel("I", HotkeyGuideKey(352, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[74] = HotkeyGuideLabel("J", HotkeyGuideKey(333, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[75] = HotkeyGuideLabel("K", HotkeyGuideKey(372, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[76] = HotkeyGuideLabel("L", HotkeyGuideKey(411, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[77] = HotkeyGuideLabel("M", HotkeyGuideKey(352, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[78] = HotkeyGuideLabel("N", HotkeyGuideKey(313, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[79] = HotkeyGuideLabel("O", HotkeyGuideKey(391, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[80] = HotkeyGuideLabel("P", HotkeyGuideKey(430, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[81] = HotkeyGuideLabel("Q", HotkeyGuideKey(79, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[82] = HotkeyGuideLabel("R", HotkeyGuideKey(196, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[83] = HotkeyGuideLabel("S", HotkeyGuideKey(138, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[84] = HotkeyGuideLabel("T", HotkeyGuideKey(235, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[85] = HotkeyGuideLabel("U", HotkeyGuideKey(313, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[86] = HotkeyGuideLabel("V", HotkeyGuideKey(235, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[87] = HotkeyGuideLabel("W", HotkeyGuideKey(118, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[88] = HotkeyGuideLabel("X", HotkeyGuideKey(157, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[89] = HotkeyGuideLabel("Y", HotkeyGuideKey(274, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[90] = HotkeyGuideLabel("Z", HotkeyGuideKey(118, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[93] = HotkeyGuideLabel("▤", HotkeyGuideKey(508, 249, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[96] = HotkeyGuideLabel("0", HotkeyGuideKey(759, 249, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[97] = HotkeyGuideLabel("1", HotkeyGuideKey(759, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[98] = HotkeyGuideLabel("2", HotkeyGuideKey(798, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[99] = HotkeyGuideLabel("3", HotkeyGuideKey(837, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[100] = HotkeyGuideLabel("4", HotkeyGuideKey(759, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[101] = HotkeyGuideLabel("5", HotkeyGuideKey(798, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[102] = HotkeyGuideLabel("6", HotkeyGuideKey(837, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[103] = HotkeyGuideLabel("7", HotkeyGuideKey(759, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[104] = HotkeyGuideLabel("8", HotkeyGuideKey(798, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[105] = HotkeyGuideLabel("9", HotkeyGuideKey(837, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[106] = HotkeyGuideLabel("*", HotkeyGuideKey(837, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[107] = HotkeyGuideLabel("+", HotkeyGuideKey(876, 132, 6, 36, 0, 75), HotkeyGuideKey());
  this->m_labels[109] = HotkeyGuideLabel("-", HotkeyGuideKey(876, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[110] = HotkeyGuideLabel("Del", HotkeyGuideKey(837, 249, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[111] = HotkeyGuideLabel("/", HotkeyGuideKey(798, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[112] = HotkeyGuideLabel("F1", HotkeyGuideKey(83, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[113] = HotkeyGuideLabel("F2", HotkeyGuideKey(122, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[114] = HotkeyGuideLabel("F3", HotkeyGuideKey(161, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[115] = HotkeyGuideLabel("F4", HotkeyGuideKey(200, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[116] = HotkeyGuideLabel("F5", HotkeyGuideKey(267, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[117] = HotkeyGuideLabel("F6", HotkeyGuideKey(306, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[118] = HotkeyGuideLabel("F7", HotkeyGuideKey(345, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[119] = HotkeyGuideLabel("F8", HotkeyGuideKey(384, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[120] = HotkeyGuideLabel("F9", HotkeyGuideKey(450, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[121] = HotkeyGuideLabel("F10", HotkeyGuideKey(489, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[122] = HotkeyGuideLabel("F11", HotkeyGuideKey(528, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[123] = HotkeyGuideLabel("F12", HotkeyGuideKey(567, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[144] = HotkeyGuideLabel("Num", HotkeyGuideKey(759, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[145] = HotkeyGuideLabel("Scroll", HotkeyGuideKey(663, 46, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[186] = HotkeyGuideLabel(";", HotkeyGuideKey(450, 171, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[187] = HotkeyGuideLabel("=", HotkeyGuideKey(489, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[188] = HotkeyGuideLabel(",", HotkeyGuideKey(391, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[189] = HotkeyGuideLabel("-", HotkeyGuideKey(450, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[190] = HotkeyGuideLabel(".", HotkeyGuideKey(430, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[191] = HotkeyGuideLabel("/", HotkeyGuideKey(469, 210, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[192] = HotkeyGuideLabel("`", HotkeyGuideKey(21, 93, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[219] = HotkeyGuideLabel("[", HotkeyGuideKey(469, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[220] = HotkeyGuideLabel("\\", HotkeyGuideKey(547, 132, 0, 56, 11, 36), HotkeyGuideKey());
  this->m_labels[221] = HotkeyGuideLabel("]", HotkeyGuideKey(508, 132, 6, 36, 0, 36), HotkeyGuideKey());
  this->m_labels[222] = HotkeyGuideLabel("'", HotkeyGuideKey(489, 171, 6, 36, 0, 36), HotkeyGuideKey());

  int messages[18] = {105, 104, 283, 106, 103, 3134, 1317, 2859, 2353, 265, 3135, 2350, 3136, 2646, 3225, 3137, 574, 3138};
  int hotkeys[18] = {47, 1, 2, 9, 5, 10, 3, 8, 42, -1, 46, 38, 51, 49, 6, 37, 17, 12};
  for (int i = 0; i < 18; ++i) {
    HotkeyInfo hotkey = GetHotkeyInfo(controls.behavior, 1, hotkeys[i]);
    if (messages[i] == 265) {
      this->m_entries.push_back(HotkeyGuideEntry(messages[i], 27, 0));
    } else {
      this->m_entries.push_back(HotkeyGuideEntry(messages[i], hotkey.key1, hotkey.key2));
    }
  }

  int index = 0;
  for (auto entry = this->m_entries.begin(); entry != this->m_entries.end(); ++entry) {
    UIBasicButton* tab = new UIBasicButton(*this->m_context, controls);
    tab->Configure(118, LookupMessage(entry->id));
    tab->Move(index / 6 * 246 + 26, index % 6 * 30 + 310);
    tab->SetId(entry->id);
    this->AddChild(tab);
    this->m_tabs.push_back(tab);
    ++index;
  }

  UIBmpTextButton* settings = new UIBmpTextButton(*this->m_context, controls);
  settings->SetBitmap("유저인터페이스\\hotkey_guide\\bt_keysettings_normal.bmp", 0, 0);
  settings->SetBitmap("유저인터페이스\\hotkey_guide\\bt_keysettings_over.bmp", 1, 0);
  settings->SetBitmap("유저인터페이스\\hotkey_guide\\bt_keysettings_press.bmp", 2, 0);
  settings->SetSize(settings->BitmapWidth(), settings->BitmapHeight());
  settings->SetText(LookupMessage(3148));
  settings->Move(763, 462);
  settings->SetId(342);
  this->AddChild(settings);
}

// OFFSET: 0x0096B730
// FUNCTION: fn_13165
void UIHotkeyGuideWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bg_shortcuts.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
  this->DrawTitleBar(controls.fonts, controls.locale, false, LookupMessage(3128), 0);

  for (auto key = this->m_keys.begin(); key != this->m_keys.end(); ++key) {
    HotkeyGuideLabel& label = this->m_labels[*key];
    if (label.key.valid == true) {
      this->DrawKey(&label.key);
    }
    if (label.altKey.valid == true) {
      this->DrawKey(&label.altKey);
    }
  }

  for (auto label = this->m_labels.begin(); label != this->m_labels.end(); ++label) {
    if (label->second.key.valid == true) {
      this->DrawKeyLabel(label->second.text, &label->second.key);
    }
    if (label->second.altKey.valid == true) {
      this->DrawKeyLabel(label->second.text, &label->second.altKey);
    }
  }

  int index = 0;
  for (auto entry = this->m_entries.begin(); entry != this->m_entries.end(); ++entry) {
    std::string text;
    if (entry->firstKey != 0) {
      text = this->m_labels[entry->firstKey].text;
      if (entry->secondKey != 0) {
        std::string prefix = FormatOwnedString("%s + ", this->m_labels[entry->secondKey].text.c_str());
        text = prefix + text;
      }
    } else {
      text = "Non";
    }
    int width = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 12, 1, 0);
    int x = static_cast<int>((100 - width) * 0.5 + (index / 6 * 246 + 156));
    this->DrawTextAt(controls.fonts, controls.locale, x, index % 6 * 30 + 315, text.c_str(), 0, 0, 12, 0xA55139, 1, 0);
    ++index;
  }

  this->DrawCenteredText(4, 32, FormatOwnedString("%s / %s", LookupMessage(3129), LookupMessage(3045)).c_str(), 75, 0, 12, 0x2B2625, 0);
  this->DrawCenteredText(207, 34, LookupMessage(3130), 159, 0, 11, 0x5B2C24, 0);
  this->DrawCenteredText(604, 32, LookupMessage(3131), 75, 0, 12, 0x2B2625, 0);
  this->DrawCenteredText(791, 313, FormatOwnedString("%s / %s", LookupMessage(3132), LookupMessage(3133)).c_str(), 102, 0, 12, 0x2B2625, 0);
  this->DrawCenteredText(770, 346, LookupMessage(3139), 55, 0, 12, 0x2B2625, 0);
  this->DrawCenteredText(859, 346, LookupMessage(3140), 55, 0, 12, 0x2B2625, 0);
}

// OFFSET: 0x0096C140
// FUNCTION: fn_13166
void UIHotkeyGuideWnd::Slot19() {
  auto tab = this->m_tabs.begin();
  for (; tab != this->m_tabs.end(); ++tab) {
    if ((*tab)->geometry.unknown_0030 == 2) {
      int id = (*tab)->geometry.id;
      if (id != this->m_current) {
        this->m_keys.clear();
        for (auto entry = this->m_entries.begin(); entry != this->m_entries.end(); ++entry) {
          if (id == entry->id) {
            if (entry->firstKey != 0) {
              this->m_keys.push_back(entry->firstKey);
              if (entry->secondKey != 0) {
                this->m_keys.push_back(entry->secondKey);
              }
            }
            break;
          }
        }
        this->m_current = id;
        this->Invalidate();
      }
      break;
    }
  }
  if (tab == this->m_tabs.end() && this->m_current != 4698) {
    this->m_keys.clear();
    this->m_current = 4698;
    this->Invalidate();
  }
}

// OFFSET: 0x0096C260
// FUNCTION: fn_13167
int UIHotkeyGuideWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    if (a == 342) {
      UIWindow* window = this->m_frameContext->windows.EnsureWindow(156);
      if (window) {
        window->SendMsg(this, 22, 1, 0, 0, 0);
      }
    }

    if (a == 201 || a == 342) {
      this->m_frameContext->windows.CloseWindow(303);
    }
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x0096C5A0
// FUNCTION: fn_13173
void UIHotkeyGuideWnd::DrawKey(const HotkeyGuideKey* key) {
  CBitmapRes* bitmap;
  int x;
  if (key->width == 36) {
    const char* name = key->height != 75 ? "유저인터페이스\\hotkey_guide\\bt_keyboard.bmp" : "유저인터페이스\\hotkey_guide\\bt_enter2.bmp";
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
    x = key->x;
  } else {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_left.bmp")));
    this->DrawBitmap(key->x, key->y, &bitmap->image, true);
    int offset = 6;
    for (int i = 1; i <= (key->width - 12) / 6; ++i) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_mid.bmp")));
      this->DrawBitmap(key->x + offset, key->y, &bitmap->image, true);
      offset += 6;
    }
    if ((key->width - 12) % 6 != 0) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_mid.bmp")));
      this->DrawBitmap(key->x + (key->width - 12), key->y, &bitmap->image, true);
    }
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\hotkey_guide\\bt_right.bmp")));
    x = key->x + (key->width - 6);
  }
  this->DrawBitmap(x, key->y, &bitmap->image, true);
}

// OFFSET: 0x0096C730
// FUNCTION: fn_13174
void UIHotkeyGuideWnd::DrawKeyLabel(std::string text, const HotkeyGuideKey* key) {
  UIButtonContext& controls = *this->m_controls;
  int x = key->x;
  if (key->align == 6) {
    int textWidth = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 12, 0, 0);
    x = static_cast<int>(static_cast<float>(key->width - textWidth) * g_depthAverageHalf + static_cast<float>(x));
  } else if (key->align == 0) {
    x += key->padding;
  } else if (key->align == 2) {
    int textWidth = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 12, 0, 0);
    x += key->width - key->padding - textWidth;
  }
  int y = static_cast<int>(static_cast<double>(key->height - 12) * 0.5 + static_cast<double>(key->y));
  this->DrawTextAt(controls.fonts, controls.locale, x, y, text.c_str(), 0, 0, 12, 0x2B2625, 0, 0);
}
