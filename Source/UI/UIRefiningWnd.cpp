#include "UI/UIRefiningWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <timeapi.h>

#include <cstddef>
#include <cstring>

// OFFSET: 0x007F5270
// FUNCTION: fn_09902
void UIRefiningWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46 && drag->source == 1) {
    this->SendMsg(this, 23, drag->id, 0, 0, 0);
  }
  g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x007F52D0
// FUNCTION: fn_09903
__declspec(noinline) void UIRefiningWnd::Reset() {
  this->unknown_01F0 = 0;
  this->item = ITEM_INFO(0);
  this->selectedMaterial = -1;
  this->materials.clear();
  this->catalystEnabled = 0;
  this->catalystCount = 0;
  this->notice->SetNotice("", 0, 0);

  this->refiningButton->cost = 0;
  this->refiningButton->Invalidate();
  this->refiningButton->SetDisabled(1);
  this->refiningButton->Invalidate();

  this->state = 0;
  this->result = 7;
  this->stateTick = timeGetTime();
  this->frame = 0;
  if (this->unknown_00C0) {
    this->unknown_00C0->SetDisabled(0);
    this->unknown_00C0->Invalidate();
  }

  this->result = 7;
  this->refiningButton->Enable(1);
  this->refiningButton->SetDisabled(1);
  this->refiningButton->Invalidate();
  this->returnButton->Enable(0);
  this->againButton->Enable(0);
  this->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x007F54B0
// FUNCTION: fn_09904
std::uint32_t UIRefiningWnd::SelectedMaterialValue() {
  if (this->item.unknown_0000[1] != 0 && !this->materials.empty() && this->selectedMaterial >= 0 &&
      static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
    return this->materials[this->selectedMaterial].unknown_0005;
  }
  return 0;
}

// OFFSET: 0x007F5500
// FUNCTION: fn_09905
int UIRefiningWnd::SlotAt(int x, int y) {
  int offsetX = x - 11;
  if (static_cast<unsigned>(offsetX) > 242 || static_cast<unsigned>(y - 55) > 52) {
    return -1;
  }
  int index = offsetX / 49;
  if (x > index * 49 + 57 || index >= 5) {
    return -1;
  }
  return index;
}

// OFFSET: 0x007F55A0
// FUNCTION: fn_09906
void UIRefiningWnd::ApplyResult() {
  this->notice->SetNotice("", 0, 0);
  this->item = this->nextItem;
  this->refiningButton->Enable(0);
  this->returnButton->Enable(1);
  this->againButton->Enable(1);

  switch (this->result) {
  case 3:
    this->returnButton->highlighted = 1;
    this->returnButton->UpdateBitmaps();
    this->againButton->success = 1;
    this->againButton->UpdateBitmaps();
    break;
  case 4:
    this->returnButton->highlighted = 0;
    this->returnButton->UpdateBitmaps();
    this->againButton->success = 0;
    this->againButton->UpdateBitmaps();
    this->againButton->SetDisabled(1);
    this->againButton->Invalidate();
    break;
  case 5:
  case 6:
    this->returnButton->highlighted = 0;
    this->returnButton->UpdateBitmaps();
    this->againButton->success = 0;
    this->againButton->UpdateBitmaps();
    break;
  }

  int nextIndex = 0;
  for (auto it = this->nextMaterials.begin(); it != this->nextMaterials.end(); ++it) {
    if (this->materials[this->selectedMaterial].itemId == it->itemId) {
      break;
    }
    ++nextIndex;
  }

  if (g_session.m_zeny < static_cast<int>(this->SelectedMaterialValue())) {
    this->notice->SetNotice(LookupMessage(3244), 5000, 1);
    this->againButton->SetDisabled(1);
  } else {
    this->againButton->cost = this->SelectedMaterialValue();
  }
  this->againButton->Invalidate();

  this->catalystCount = this->nextCatalystCount;
  if (this->catalystEnabled == 1) {
    if (this->nextCatalystCount == 0) {
      this->catalystEnabled = 0;
      this->notice->SetNotice(LookupMessage(3242), 5000, 1);
      this->againButton->SetDisabled(1);
      this->againButton->Invalidate();
    } else {
      this->notice->SetNotice(LookupMessage(2967), 0, 0);
      const int owned = LookupClientItem(g_session.m_inventoryItems, this->catalystItemId, 0).quantity;
      if (this->catalystCount > owned) {
        this->catalystEnabled = 0;
        this->notice->SetNotice(LookupMessage(3245), 5000, 1);
        this->againButton->SetDisabled(1);
        this->againButton->Invalidate();
      }
    }
  }

  const std::size_t count = this->nextMaterials.size();
  if (count == 0) {
    this->materials.clear();
    this->selectedMaterial = -1;
    this->notice->SetNotice(LookupMessage(2970), 5000, 1);
    this->againButton->SetDisabled(1);
    this->againButton->Invalidate();
  } else {
    bool same = true;
    if (this->materials.size() == count) {
      for (auto it = this->materials.begin(); it != this->materials.end(); ++it) {
        auto found = this->nextMaterials.begin();
        for (; found != this->nextMaterials.end(); ++found) {
          if (it->itemId == found->itemId) {
            break;
          }
        }
        if (found == this->nextMaterials.end()) {
          same = false;
          break;
        }
      }
    } else {
      same = false;
    }

    this->materials.assign(this->nextMaterials.begin(), this->nextMaterials.end());

    unsigned char level = 0;
    if (this->item.unknown_0000[1] != 0 && !this->materials.empty() && this->selectedMaterial >= 0 &&
        static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
      level = this->materials[this->selectedMaterial].unknown_0004;
    }
    this->againButton->unknown_0125 = level;
    this->againButton->Invalidate();

    if (same) {
      if (this->selectedMaterial >= 0 && static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
        this->selectedMaterial = nextIndex;
        const std::uint32_t itemId = this->materials[nextIndex].itemId;

        ITEM_INFO info;
        char name[260];
        std::memset(name, 0, sizeof(name));
        info.SetItemId(itemId);
        int capacity = 260;
        ItemDescriptions descriptions{g_itemDescriptions};
        descriptions.Name(info, name, capacity, 0);
        this->againButton->SetTooltip(name);

        const ITEM_INFO owned = LookupClientItem(g_session.m_inventoryItems, itemId, 0);
        if (owned.quantity == 0) {
          this->notice->SetNotice(LookupMessage(3243), 5000, 1);
          this->againButton->SetDisabled(1);
          this->againButton->Invalidate();
        }
      }
    } else {
      this->selectedMaterial = -1;
      this->notice->SetNotice(LookupMessage(3242), 5000, 1);
      this->againButton->SetDisabled(1);
      this->againButton->Invalidate();
    }
  }

  this->unknown_01F0 = 0;
  this->Invalidate();
}

// OFFSET: 0x007F6BF0
// FUNCTION: fn_09909
void UIRefining_BtnAgain::Paint() {
  UIBitmapButton::Paint();
  auto& controls = *this->m_buttonContext;
  if (this->m_disabled == 1) {
    int width = this->TextWidth(controls.fonts, controls.locale, LookupMessage(3241), 0, 0, 12, 1, 0);
    std::uint32_t color = this->success == 1 ? 0xE1C1F : (this->m_disabled == 1 ? 0x6F4333 : 0x3E1B12);
    this->DrawTextAt(
      controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
      (this->geometry.unknown_0030 == 2) + 14, LookupMessage(3241), 0, 0, 12, color, 1, 0
    );
    return;
  }

  int width = this->TextWidth(controls.fonts, controls.locale, LookupMessage(3241), 0, 0, 12, 1, 0);
  std::uint32_t color = this->success == 1 ? 0xE1C1F : (this->m_disabled == 1 ? 0x6F4333 : 0x3E1B12);
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.unknown_0030 == 2) + 9, LookupMessage(3241), 0, 0, 12, color, 1, 0
  );

  std::string count = FormatOwnedString(LookupMessage(3285), this->unknown_0125);
  width = this->TextWidth(controls.fonts, controls.locale, count.c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.unknown_0030 == 2) + 22, count.c_str(), 0, 0, 11, 0, 0, 0
  );

  width = this->TextWidth(controls.fonts, controls.locale, IntToString(this->cost, 0).c_str(), 0, 0, 11, 1, 0);
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.unknown_0030 == 2) + 38, IntToString(this->cost, 0).c_str(), 0, 0, 11, 0xFFFFFF, 1, 0
  );
}

// OFFSET: 0x007F6EA0
// FUNCTION: fn_09910
void UIRefining_BtnRefining::Paint() {
  UIBitmapButton::Paint();
  auto& controls = *this->m_buttonContext;
  int width = this->TextWidth(controls.fonts, controls.locale, LookupMessage(2966), 0, 0, 13, 1, 0);
  std::uint32_t color = this->m_disabled == 1 ? 0x94644A : 0x3E1B12;
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.unknown_0030 == 2) + 11, LookupMessage(2966), 0, 0, 12, color, 1, 0
  );

  if (this->m_disabled == 0) {
    width = this->TextWidth(controls.fonts, controls.locale, IntToString(this->cost, 0).c_str(), 0, 0, 12, 1, 0);
    this->DrawTextAt(
      controls.fonts, controls.locale, 83 - width, (this->geometry.unknown_0030 == 2) + 28, IntToString(this->cost, 0).c_str(), 0, 0, 12, 0xFFFFFF, 1, 0
    );
  }
}

// OFFSET: 0x007F7030
// FUNCTION: fn_09911
void UIRefining_BtnReturn::Paint() {
  UIBitmapButton::Paint();
  auto& controls = *this->m_buttonContext;
  int width = this->TextWidth(controls.fonts, controls.locale, LookupMessage(2973), 0, 0, 12, 1, 0);
  std::uint32_t color = this->highlighted == 1 ? 0xE1C1F : 0x3E1B12;
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.unknown_0030 == 2) + 14, LookupMessage(2973), 0, 0, 12, color, 1, 0
  );
}

// OFFSET: 0x007F72A0
// FUNCTION: fn_09913
void UIRefiningWnd::Slot26(int x, int y) {
  if (this->item.unknown_0000[1] != 0 && static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46) {
    this->Reset();
  }
}

// OFFSET: 0x007F72D0
// FUNCTION: fn_09914
void UIRefiningWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (this->state == 1 && this->item.unknown_0000[1] > 0 && static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 28, this->item, 1, -1);
  }
}

// OFFSET: 0x007F7340
// FUNCTION: fn_09915
void UIRefiningWnd::Slot31(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot31(x, y);
    return;
  }

  if (this->state != 1) {
    return;
  }

  const int slot = this->SlotAt(x, y);
  if (slot < 0) {
    return;
  }

  if (static_cast<unsigned>(slot) < this->materials.size()) {
    const int owned = LookupClientItem(g_session.m_inventoryItems, this->materials[slot].itemId, 0).quantity;
    if (owned < 1) {
      return;
    }

    this->selectedMaterial = slot;
    this->refiningButton->cost = this->SelectedMaterialValue();
    this->refiningButton->Invalidate();

    if (this->materials[slot].unknown_0004 <= 99 && this->catalystEnabled == 0) {
      int message;
      switch (this->materials[slot].itemId) {
      case 6223:
      case 6224:
        message = 2990;
        break;
      case 6225:
      case 6226:
      case 6240:
      case 6241:
      case 6327:
      case 6906:
      case 6910:
      case 6911:
      case 1000331:
      case 1000332:
      case 1000333:
      case 1000334:
        message = 2989;
        break;
      case 984:
      case 985:
      case 1010:
      case 1011:
      case 7619:
      case 7620:
      case 1000335:
      case 1000336:
      case 1000368:
      case 1000369:
      case 1000370:
      case 1000371:
        message = 2988;
        break;
      default:
        this->Invalidate();
        return;
      }
      this->notice->SetNotice(LookupMessage(message), 0, 0);
    }
  } else {
    if (slot != 4 || this->catalystCount < 1) {
      return;
    }

    const int owned = LookupClientItem(g_session.m_inventoryItems, this->catalystItemId, 0).quantity;
    if (owned < this->catalystCount) {
      this->notice->SetNotice(LookupMessage(2969), 3000, 1);
    } else if (this->catalystEnabled == 1) {
      this->catalystEnabled = 0;
      this->notice->SetNotice("", 0, 0);
    } else {
      this->catalystEnabled = 1;
      this->notice->SetNotice(LookupMessage(2967), 0, 0);
    }
  }

  this->Invalidate();
}

// OFFSET: 0x007F7AA0
// FUNCTION: fn_09917
void UIRefiningWnd::Slot19() {
  if (this->notice) {
    this->notice->Slot19();
  }

  if (this->state == 1 && this->refiningButton->Disabled() == 1 && this->item.unknown_0000[1] != 0 && this->selectedMaterial >= 0 &&
      static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
    this->refiningButton->SetDisabled(0);
    this->refiningButton->Invalidate();
    this->refiningButton->cost = this->SelectedMaterialValue();
    this->refiningButton->Invalidate();
    this->Invalidate();
  }

  const int frameCounts[7] = {4, 3, 14, 17, 20, 20, 20};
  if (this->frame >= frameCounts[this->state]) {
    return;
  }
  if (timeGetTime() <= this->stateTick + this->frameInterval) {
    return;
  }

  ++this->frame;
  if (this->state == 2) {
    if (this->frame == 7) {
      g_soundMgr->PlayWave("effect\\refine\\bs_refine_1.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    if (this->frame == 12) {
      g_soundMgr->PlayWave("effect\\refine\\bs_refine_2.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
  }

  if (this->frame >= frameCounts[this->state]) {
    switch (this->state) {
    case 0:
      this->frame = 0;
      break;
    case 3:
      this->frame = 13;
      break;
    case 4:
    case 5:
    case 6:
      this->frame = 18;
      break;
    default:
      this->frame = frameCounts[this->state] - 1;
      if (this->state == 2) {
        if (this->result == 3) {
          g_soundMgr->PlayWave("effect\\refine\\bs_refine_success.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
        } else if (this->result == 4 || this->result == 5 || this->result == 6) {
          g_soundMgr->PlayWave("effect\\refine\\bs_refine_failed.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
        }
        this->ApplyResult();
        this->SetState(this->result, 7);
        return;
      }
      break;
    }
  }

  this->stateTick = timeGetTime();
  this->Invalidate();
}

// OFFSET: 0x007F7D80
// FUNCTION: fn_09918
void UIRefining_Notice::Slot19() {
  if (this->expireTick != 0 && this->expireTick < timeGetTime()) {
    this->expireTick = 0;
    if (this->autoShown == 1) {
      this->autoShown = 0;
      this->geometry.visible = 0;
    }
    this->text = this->savedText;
    this->highlight = this->savedHighlight;
    this->Invalidate();
    if (this->parent) {
      this->parent->Invalidate();
    }
  }
}

// OFFSET: 0x007F7E10
// FUNCTION: fn_09919
void UIRefiningWnd::Slot32(int x, int y) {
}

// OFFSET: 0x007F80C0
// FUNCTION: fn_09921
void UIRefining_BtnAgain::UpdateBitmaps() {
  this->SetBitmap(
    FormatOwnedString("유저인터페이스\\refining_renewal\\bt_refining_again_%s_normal.bmp", this->success == 1 ? "success" : "fail").c_str(), 0, 0
  );
  this->SetBitmap(
    FormatOwnedString("유저인터페이스\\refining_renewal\\bt_refining_again_%s_normal.bmp", this->success == 1 ? "success" : "fail").c_str(), 1, 0
  );
  this->SetBitmap(
    FormatOwnedString("유저인터페이스\\refining_renewal\\bt_refining_again_%s_press.bmp", this->success == 1 ? "success" : "fail").c_str(), 2, 0
  );
  this->Resize(this->m_bitmapWidth, this->m_bitmapHeight);
  this->Invalidate();
}

// OFFSET: 0x007F8210
// FUNCTION: fn_09922
void UIRefining_BtnReturn::UpdateBitmaps() {
  this->SetBitmap(
    FormatOwnedString("유저인터페이스\\refining_renewal\\bt_refining_return_%s_normal.bmp", this->highlighted == 1 ? "success" : "fail").c_str(), 0, 0
  );
  this->SetBitmap(
    FormatOwnedString("유저인터페이스\\refining_renewal\\bt_refining_return_%s_normal.bmp", this->highlighted == 1 ? "success" : "fail").c_str(), 1, 0
  );
  this->SetBitmap(
    FormatOwnedString("유저인터페이스\\refining_renewal\\bt_refining_return_%s_press.bmp", this->highlighted == 1 ? "success" : "fail").c_str(), 2, 0
  );
  this->Resize(this->m_bitmapWidth, this->m_bitmapHeight);
  this->Invalidate();
}

// OFFSET: 0x007F8C40
// FUNCTION: fn_09925
void UIRefining_BtnAgain::Enable(unsigned char value) {
  if (value == 0) {
    g_windowMgr.ReleaseCapture();
  }

  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x007F8C80
// FUNCTION: fn_09926
void UIRefining_BtnRefining::Enable(unsigned char value) {
  if (value == 0) {
    g_windowMgr.ReleaseCapture();
  }

  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x007F8CC0
// FUNCTION: fn_09927
void UIRefiningWnd::SetState(int state, int result) {
  this->state = state;
  this->result = result;
  this->stateTick = timeGetTime();
  this->frame = 0;
  if (this->unknown_00C0) {
    this->unknown_00C0->SetDisabled(0);
    this->unknown_00C0->Invalidate();
  }

  switch (state) {
  case 0:
  case 1:
    this->result = 7;
    this->refiningButton->Enable(1);
    this->refiningButton->SetDisabled(1);
    this->refiningButton->Invalidate();
    this->returnButton->Enable(0);
    this->againButton->Enable(0);
    this->Invalidate();
    return;
  case 2:
    this->refiningButton->Enable(1);
    this->refiningButton->SetDisabled(1);
    this->refiningButton->Invalidate();
    this->returnButton->Enable(0);
    this->againButton->Enable(0);
    if (this->unknown_00C0) {
      this->unknown_00C0->SetDisabled(1);
      this->unknown_00C0->Invalidate();
    }
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(498)), 0xFFFF00, 0, 0, 0);
    break;
  case 4:
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(499)), 0xFFFF00, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(1537)), 0xFFFF00, 0, 0, 0);
    break;
  }

  this->Invalidate();
}

// OFFSET: 0x007F8EA0
// FUNCTION: fn_09928
void UIRefining_Notice::SetNotice(const char* value, int duration, unsigned char highlight) {
  this->highlight = highlight;
  this->text = value;
  if (duration == 0) {
    this->savedText = value;
    this->savedHighlight = highlight;
  } else {
    this->expireTick = timeGetTime() + duration;
    if (this->geometry.visible == 0) {
      this->autoShown = 1;
      this->geometry.visible = 1;
    }
  }
  this->Invalidate();
  if (this->parent) {
    this->parent->Invalidate();
  }
}
