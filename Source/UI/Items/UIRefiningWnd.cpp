#include "UI/Login/UINewSelectCharWnd.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Items/UIRefiningWnd.h"

#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemId.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <windows.h>
#include <shellapi.h>
#include <timeapi.h>

#include <cstddef>
#include <cstring>
#include <list>
#include <string>

#pragma pack(push, 1)
struct RefiningSelectPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t index;
};

struct RefiningRequestPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t index;
  /* 0004 */ std::uint32_t materialId;
  /* 0008 */ unsigned char useCatalyst;
};

struct RefiningClosePacket {
  /* 0000 */ std::int16_t id;
};
#pragma pack(pop)

// INLINED
inline void UIRefining_Notice::SetNoticeInline(const char* value, int duration, unsigned char highlight) {
  this->highlight = highlight;
  this->text = value;
  if (duration == 0) {
    this->saved.text = value;
    this->saved.highlight = highlight;
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

// OFFSET: 0x007F4920
// FUNCTION: fn_09890
UIRefiningWnd::UIRefiningWnd()
    : UIFrameWnd(0), refiningButton(nullptr), returnButton(nullptr),
      cancelButton(nullptr), state(REFINING_STATE_WAIT), result(7), frameInterval(100), stateTick(0), frame(0), selectedMaterial(-1), materials(), catalystEnabled(0),
      catalystItemId(0), catalystCount(0), item(0), requestPending(0), nextMaterials(), nextCatalystCount(0), nextItem(0) {
  this->catalystItemId = 6635;
}

// OFFSET: 0x007F4D20
// FUNCTION: fn_46430
__declspec(noinline) UIRefiningWnd::~UIRefiningWnd() = default;

// OFFSET: 0x007F4A70
// FUNCTION: fn_09891
UIRefining_BtnAgain::UIRefining_BtnAgain() : UIBitmapButton(), cost(0) {
}

// OFFSET: 0x007F4AD0
// FUNCTION: fn_09892
UIRefining_BtnRefining::UIRefining_BtnRefining() : UIBitmapButton(), cost(0) {
}

// OFFSET: 0x007F4B30
// FUNCTION: fn_09893
UIRefining_BtnReturn::UIRefining_BtnReturn() : UIBitmapButton(), highlighted(1) {
}

// OFFSET: 0x007F5200
// FUNCTION: fn_46579
inline void UIRefining_BtnRefining::InitializeBitmaps() {
  this->SetBitmap("유저인터페이스\\refining_renewal\\bt_refining_normal.bmp", 0, 0);
  this->SetBitmap("유저인터페이스\\refining_renewal\\bt_refining_normal.bmp", 1, 0);
  this->SetBitmap("유저인터페이스\\refining_renewal\\bt_refining_press.bmp", 2, 0);
  this->SetBitmap("유저인터페이스\\refining_renewal\\bt_refining_disable.bmp", 3, 0);
  this->Resize(this->BitmapWidth(), this->BitmapHeight());
}

// OFFSET: 0x007F5E30
// FUNCTION: fn_09907
void UIRefiningWnd::OnCreate(int width, int height) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  UIBasicButton* basic = new UIBasicButton();
  basic->Configure(56, LookupMessage(MSI_CANCEL));
  basic->Move(204, 322);
  basic->SetId(ID_CANCEL);
  this->AddChild(basic);
  this->cancelButton = basic;

  UIRefining_BtnRefining* refining = new UIRefining_BtnRefining();
  this->refiningButton = refining;
  refining->InitializeBitmaps();
  this->refiningButton->Move(84, 262);
  this->refiningButton->SetId(ID_OK);
  {
    auto* refiningButton = this->refiningButton;
    refiningButton->SetDisabled(1);
    refiningButton->Invalidate();
  }
  this->AddChild(this->refiningButton);

  this->returnButton = new UIRefining_BtnReturn();
  this->returnButton->UpdateBitmaps();
  this->returnButton->Move(20, 267);
  this->returnButton->SetId(REFINING_BUTTON_RETURN);
  this->returnButton->Enable(0);
  this->AddChild(this->returnButton);

  this->againButton = new UIRefining_BtnAgain();
  this->againButton->UpdateBitmaps();
  this->againButton->Move(142, 267);
  this->againButton->SetId(REFINING_BUTTON_AGAIN);
  this->againButton->Enable(0);
  this->AddChild(this->againButton);

  this->notice = new UIRefining_Notice();
  this->notice->SetSize(194, 22);
  this->notice->Move(6, 322);
  this->AddChild(this->notice);

  UIBitmapButton* help = new UIBitmapButton();
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_normal.bmp", 0, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_over.bmp", 1, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_pressed.bmp", 2, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_disabled.bmp", 3, 0);
  help->SetSize(help->BitmapWidth(), help->BitmapHeight());
  help->Move(230, 20);
  help->SetTooltip(LookupMessage(MSI_CHECK_PROBABILITY));
  help->SetId(REFINING_BUTTON_PROBABILITY);
  this->AddChild(help);
}

// OFFSET: 0x007F5270
// FUNCTION: fn_09902
void UIRefiningWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46 && drag->source == DRAG_ITEM_INVENTORY) {
    this->SendMsg(this, UM_REFRESHITEM, drag->id, 0, 0, 0);
  }
  g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x007F52D0
// FUNCTION: fn_09903
__declspec(noinline) void UIRefiningWnd::Reset() {
  this->requestPending = 0;
  this->item = ITEM_INFO(0);
  this->selectedMaterial = -1;
  this->materials.clear();
  this->catalystEnabled = 0;
  this->catalystCount = 0;
  this->notice->SetNoticeInline("", 0, 0);

  {
    UIRefining_BtnRefining* button = this->refiningButton;
    button->cost = 0;
    button->Invalidate();
  }
  {
    UIRefining_BtnRefining* button = this->refiningButton;
    button->SetDisabled(1);
    button->Invalidate();
  }

  this->state = REFINING_STATE_WAIT;
  this->result = 7;
  this->stateTick = timeGetTime();
  this->frame = 0;
  if (UIButton* button = this->cancelButton) {
    button->SetDisabled(0);
    button->Invalidate();
  }

  this->result = 7;
  this->refiningButton->Enable(1);
  {
    UIRefining_BtnRefining* button = this->refiningButton;
    button->SetDisabled(1);
    button->Invalidate();
  }
  this->returnButton->Enable(0);
  this->againButton->Enable(0);
  this->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x007F54B0
// FUNCTION: fn_09904
std::uint32_t UIRefiningWnd::SelectedMaterialValue() {
  if (this->item.identity[1] > 0 && this->materials.size() > 0 && this->selectedMaterial >= 0 &&
      this->materials.size() > static_cast<unsigned>(this->selectedMaterial)) {
    return this->materials[this->selectedMaterial].cost;
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

// INLINED
static bool LacksItems(int required, const ITEM_INFO& item) {
  return required > item.quantity;
}

// INLINED
static __forceinline int SelectedSuccessRate(const UIRefiningWnd* window) {
  if (window->item.identity[1] > 0 && window->materials.size() != 0 && window->selectedMaterial >= 0 &&
      window->materials.size() > static_cast<unsigned>(window->selectedMaterial)) {
    return window->materials[window->selectedMaterial].successRate;
  }
  return 0;
}

// OFFSET: 0x007F8E60
// FUNCTION: fn_46574
inline void UIRefining_BtnAgain::SetSuccess(unsigned char value) {
  this->success = value;
  this->UpdateBitmaps();
}

// OFFSET: 0x007F8E80
// FUNCTION: fn_46575
inline void UIRefining_BtnReturn::SetHighlighted(unsigned char value) {
  this->highlighted = value;
  this->UpdateBitmaps();
}

// OFFSET: 0x007F55A0
// FUNCTION: fn_09906
void UIRefiningWnd::ApplyResult() {
  this->notice->SetNoticeInline("", 0, 0);
  this->item = this->nextItem;
  this->refiningButton->Enable(0);
  this->returnButton->Enable(1);
  this->againButton->Enable(1);

  switch (this->result) {
  case REFINING_STATE_SUCCESS:
    this->returnButton->SetHighlighted(1);
    this->againButton->SetSuccess(1);
    break;
  case REFINING_STATE_FAIL:
    this->returnButton->SetHighlighted(0);
    this->againButton->SetSuccess(0);
    {
      UIRefining_BtnAgain* button = this->againButton;
      button->SetDisabled(1);
      button->Invalidate();
    }
    break;
  case REFINING_STATE_DOWNGRADE:
  case REFINING_STATE_FAIL2:
    this->returnButton->SetHighlighted(0);
    this->againButton->SetSuccess(0);
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
    this->notice->SetNoticeInline(LookupMessage(MSI_REFINING_AGAIN_FAILED_LACK_OF_MONEY), 5000, 1);
    UIRefining_BtnAgain* button = this->againButton;
    button->SetDisabled(1);
    button->Invalidate();
  } else {
    UIRefining_BtnAgain* button = this->againButton;
    button->cost = this->SelectedMaterialValue();
    button->Invalidate();
  }

  this->catalystCount = this->nextCatalystCount;
  if (this->catalystEnabled == 1) {
    if (this->nextCatalystCount == 0) {
      this->catalystEnabled = 0;
      this->notice->SetNoticeInline(LookupMessage(MSI_REFINING_AGAIN_FAILED_INGREDIENT_CHANGED), 5000, 1);
      {
        UIRefining_BtnAgain* button = this->againButton;
        button->SetDisabled(1);
        button->Invalidate();
      }
    } else {
      this->notice->SetNoticeInline(LookupMessage(MSI_USING_BLACKSMITH_BLESSING), 0, 0);
      if (LacksItems(this->catalystCount, g_session.LookupClientItem(this->catalystItemId, 0))) {
        this->catalystEnabled = 0;
        this->notice->SetNoticeInline(LookupMessage(MSI_REFINING_AGAIN_FAILED_LACK_OF_BLACKSMITHBLESSING), 5000, 1);
        {
          UIRefining_BtnAgain* button = this->againButton;
          button->SetDisabled(1);
          button->Invalidate();
        }
      }
    }
  }

  const std::size_t count = this->nextMaterials.size();
  if (count == 0) {
    this->materials.clear();
    this->selectedMaterial = -1;
    this->notice->SetNoticeInline(LookupMessage(MSI_CANNOT_REFINE_ITEM), 5000, 1);
    {
      UIRefining_BtnAgain* button = this->againButton;
      button->SetDisabled(1);
      button->Invalidate();
    }
  } else {
    bool same = true;
    if (this->materials.size() == count) {
      for (const RefiningMaterial& material : this->materials) {
        auto found = std::find_if(this->nextMaterials.begin(), this->nextMaterials.end(), [&material](const RefiningMaterial& next) {
          return material.itemId == next.itemId;
        });
        if (this->nextMaterials.end() == found) {
          same = false;
          break;
        }
      }
    } else {
      same = false;
    }

    this->materials.assign(this->nextMaterials.begin(), this->nextMaterials.end());

    {
      UIRefining_BtnAgain* button = this->againButton;
      button->remainingCount = this->item.identity[1] > 0 && this->materials.size() > 0 && this->selectedMaterial >= 0 &&
          this->materials.size() > static_cast<unsigned>(this->selectedMaterial)
        ? this->materials[this->selectedMaterial].successRate
        : 0;
      button->Invalidate();
    }

    if (!same) {
      this->selectedMaterial = -1;
      this->notice->SetNoticeInline(LookupMessage(MSI_REFINING_AGAIN_FAILED_INGREDIENT_CHANGED), 5000, 1);
      {
        UIRefining_BtnAgain* button = this->againButton;
        button->SetDisabled(1);
        button->Invalidate();
      }
    } else if (this->selectedMaterial >= 0 && static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
      this->selectedMaterial = nextIndex;
      const std::uint32_t itemId = this->materials[nextIndex].itemId;

      ITEM_INFO info;
      char name[260];
      std::memset(name, 0, sizeof(name));
      info.SetItemId(itemId);
      int capacity = 260;
      info.GetIdDisplayName(name, capacity, 0);
      this->againButton->SetTooltip(name);

      const ITEM_INFO owned = g_session.LookupClientItem(itemId, 0);
      if (owned.quantity == 0) {
        this->notice->SetNoticeInline(LookupMessage(MSI_REFINING_AGAIN_FAILED_LACK_OF_INGREDIENT), 5000, 1);
        {
          UIRefining_BtnAgain* button = this->againButton;
          button->SetDisabled(1);
          button->Invalidate();
        }
      }
    }
  }

  this->requestPending = 0;
  this->Invalidate();
}

// OFFSET: 0x007F6BF0
// FUNCTION: fn_09909
void UIRefining_BtnAgain::Paint() {
  UIBitmapButton::Paint();
  if (this->m_disabled == 1) {
    int width = this->TextWidth(LookupMessage(MSI_AGAIN), 0, 0, 12, 1, 0);
    std::uint32_t color = this->success == 1 ? 0xE1C1F : (this->m_disabled == 1 ? 0x6F4333 : 0x3E1B12);
    this->DrawTextAt(static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
      (this->geometry.state == 2) + 14, LookupMessage(MSI_AGAIN), 0, 0, 12, color, 1, 0
    );
    return;
  }

  int width = this->TextWidth(LookupMessage(MSI_AGAIN), 0, 0, 12, 1, 0);
  std::uint32_t color = this->success == 1 ? 0xE1C1F : (this->m_disabled == 1 ? 0x6F4333 : 0x3E1B12);
  this->DrawTextAt(static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.state == 2) + 9, LookupMessage(MSI_AGAIN), 0, 0, 12, color, 1, 0
  );

  std::string count = FormatOwnedString(LookupMessage(MSI_REFINING_RATE), this->remainingCount);
  width = this->TextWidth(count.c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.state == 2) + 22, count.c_str(), 0, 0, 11, 0, 0, 0
  );

  width = this->TextWidth(IntToString(this->cost, 0).c_str(), 0, 0, 11, 1, 0);
  this->DrawTextAt(static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.state == 2) + 38, IntToString(this->cost, 0).c_str(), 0, 0, 11, 0xFFFFFF, 1, 0
  );
}

// OFFSET: 0x007F6EA0
// FUNCTION: fn_09910
void UIRefining_BtnRefining::Paint() {
  UIBitmapButton::Paint();
  int width = this->TextWidth(LookupMessage(MSI_REFINING), 0, 0, 13, 1, 0);
  std::uint32_t color = this->m_disabled == 1 ? 0x94644A : 0x3E1B12;
  this->DrawTextAt(static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.state == 2) + 11, LookupMessage(MSI_REFINING), 0, 0, 12, color, 1, 0
  );

  if (this->m_disabled == 0) {
    width = this->TextWidth(IntToString(this->cost, 0).c_str(), 0, 0, 12, 1, 0);
    this->DrawTextAt(83 - width, (this->geometry.state == 2) + 28, IntToString(this->cost, 0).c_str(), 0, 0, 12, 0xFFFFFF, 1, 0
    );
  }
}

// OFFSET: 0x007F7030
// FUNCTION: fn_09911
void UIRefining_BtnReturn::Paint() {
  UIBitmapButton::Paint();
  int width = this->TextWidth(LookupMessage(MSI_RETURN), 0, 0, 12, 1, 0);
  std::uint32_t color = this->highlighted == 1 ? 0xE1C1F : 0x3E1B12;
  this->DrawTextAt(static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.state == 2) + 14, LookupMessage(MSI_RETURN), 0, 0, 12, color, 1, 0
  );
}

// OFFSET: 0x007F70C0
// FUNCTION: fn_09912
void UIRefining_Notice::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->text.size() > 0) {
    this->DrawBitmap(0, 3, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\refining_renewal\\ico_notice.bmp"))), false);

    std::uint32_t color = this->highlight != 1 ? 0xC4281C : 0x2E15CA;
    std::list<std::string> lines;
    this->WrapFormattedText(this->text, &lines, this->geometry.width - 18, 0, 11, 0);

    int row = 0;
    for (auto line = lines.begin(); line != lines.end(); ++line) {
      this->DrawTextAt(18, static_cast<int>((this->geometry.height - lines.size() * 11) * 0.5 + row * 11), line->c_str(), 0, 0, 11, color, 0, 0
      );
      ++row;
    }
  }
}

// OFFSET: 0x007F72A0
// FUNCTION: fn_09913
void UIRefiningWnd::OnLBtnDblClk(int x, int y) {
  if (this->item.identity[1] > 0 && static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46) {
    this->Reset();
  }
}

// OFFSET: 0x007F72D0
// FUNCTION: fn_09914
void UIRefiningWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (this->state == REFINING_STATE_READY && this->item.identity[1] > 0 && static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46) {
    g_modeMgr.GetGameMode()->BeginDragItem(DRAG_REFINING, this->item, 1, -1);
  }
}

// OFFSET: 0x007F7340
// FUNCTION: fn_09915
void UIRefiningWnd::OnLBtnUp(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnUp(x, y);
    return;
  }

  if (this->state != REFINING_STATE_READY) {
    return;
  }

  const int slot = this->SlotAt(x, y);
  if (slot < 0) {
    return;
  }

  if (static_cast<unsigned>(slot) < this->materials.size()) {
    const int owned = g_session.LookupClientItem(this->materials[slot].itemId, 0).quantity;
    if (owned <= 0) {
      return;
    }

    auto* const button = this->refiningButton;
    this->selectedMaterial = slot;
    button->cost = this->SelectedMaterialValue();
    button->Invalidate();

    if (this->materials[slot].successRate < 100 && this->catalystEnabled == 0) {
      int messageId = 0;
      switch (this->materials[slot].itemId) {
      case ITEMID_CARNIUM:
      case ITEMID_BRADIUM:
        messageId = MSI_REFINING_FAIL_ITEM_DISAPPEAR_OR_ITEM_DOWNGRADE;
        break;
      case ITEMID_HD_CARNIUM:
      case ITEMID_HD_BRADIUM:
      case ITEMID_HD_ORIDECON:
      case ITEMID_HD_ELUNIUM:
      case ITEMID_LI_HD_BRADIUM:
      case ITEMID_LI_HD_CARNIUM:
      case ITEMID_LI_HD_ORIDECON:
      case ITEMID_LI_HD_ELUNIUM:
      case ITEMID_ETHERNIUM:
      case ITEMID_ETHERDEOCON:
      case ITEMID_ENRICHED_ETHERNIUM:
      case ITEMID_ENRICHED_ETHERDEOCON:
        messageId = MSI_REFINING_FAIL_ITEM_DOWMGRADE;
        break;
      case ITEMID_ORIDECON:
      case ITEMID_ELUNIUM:
      case ITEMID_PHRACON:
      case ITEMID_EMVERETARCON:
      case ITEMID_ENRICHED_ELUNIUM:
      case ITEMID_ENRICHED_ORIDECON:
      case ITEMID_HD_ETHERNIUM:
      case ITEMID_HD_ETHERDEOCON:
      case ITEMID_ETEL_BRADIUM:
      case ITEMID_HD_ETEL_BRADIUM:
      case ITEMID_ETEL_CARNIUM:
      case ITEMID_HD_ETEL_CARNIUM:
        messageId = MSI_REFINING_FAIL_ITEM_DISAPPEAR;
        break;
      }
      if (messageId != 0) {
        this->notice->SetNotice(LookupMessage(messageId), 0, 0);
      }
    }
  } else {
    if (slot != 4 || this->catalystCount <= 0) {
      return;
    }

    const bool enough = this->catalystCount <= g_session.LookupClientItem(this->catalystItemId, 0).quantity;
    UIRefining_Notice* const notice = this->notice;
    const char* text;
    int duration;
    unsigned char highlight;
    if (enough) {
      if (this->catalystEnabled == 1) {
        this->catalystEnabled = 0;
        text = "";
      } else {
        this->catalystEnabled = 1;
        text = LookupMessage(MSI_USING_BLACKSMITH_BLESSING);
      }
      duration = 0;
      highlight = 0;
    } else {
      text = LookupMessage(MSI_NOTENOUGH_BLACKSMITH_BLESSING);
      duration = 3000;
      highlight = 1;
    }
    notice->SetNotice(text, duration, highlight);
  }

  this->Invalidate();
}

// OFFSET: 0x007F76C0
// FUNCTION: fn_09916
void UIRefiningWnd::OnMouseHover(int x, int y) {
  if (this->state != REFINING_STATE_READY) {
    return;
  }

  char name[260];
  std::memset(name, 0, sizeof(name));

  const int& px = x;
  const int& py = y;
  if (this->selectedMaterial != -1 && px >= 81 && px <= 105 && py >= 150 && py <= 174) {
    ITEM_INFO info;
    info.SetItemId(this->materials[this->selectedMaterial].itemId);
    int capacity = 260;
    info.GetIdDisplayName(name, capacity, 0);
    int tipX = 81;
    int tipY = 150;
    this->ToScreen(tipX, tipY);
    g_modeMgr.currentMode->ShowTooltip(name, tipX, tipY - 20, -1, false, true);
    return;
  }

  if (this->catalystEnabled == 1 && px >= 161 && px <= 185 && py >= 150 && py <= 174) {
    ITEM_INFO info;
    info.SetItemId(this->catalystItemId);
    int capacity = 260;
    info.GetIdDisplayName(name, capacity, 0);
    int tipX = 161;
    int tipY = 150;
    this->ToScreen(tipX, tipY);
    g_modeMgr.currentMode->ShowTooltip(name, tipX, tipY - 20, -1, false, true);
    return;
  }

  const int slot = this->SlotAt(x, y);
  if (slot < 0) {
    return;
  }

  if (static_cast<unsigned>(slot) < this->materials.size()) {
    ITEM_INFO info;
    info.SetItemId(this->materials[slot].itemId);
    int capacity = 260;
    info.GetIdDisplayName(name, capacity, 0);
  } else if (slot == 4 && this->catalystCount > 0) {
    ITEM_INFO info;
    info.SetItemId(this->catalystItemId);
    int capacity = 260;
    info.GetIdDisplayName(name, capacity, 0);
  }

  if (std::strlen(name) != 0) {
    int tipX = slot * 49 + 11;
    int tipY = 35;
    this->ToScreen(tipX, tipY);
    g_modeMgr.currentMode->ShowTooltip(name, tipX, tipY, -1, false, true);
  }
}

// OFFSET: 0x007F7AA0
// FUNCTION: fn_09917
void UIRefiningWnd::OnProcess() {
  if (this->notice) {
    this->notice->OnProcess();
  }

  if (this->state == REFINING_STATE_READY && this->refiningButton->Disabled() == 1 && this->item.identity[1] > 0 && this->selectedMaterial >= 0 &&
      static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
    {
      auto* refiningButton = this->refiningButton;
      refiningButton->SetDisabled(0);
      refiningButton->Invalidate();
    }
    {
      auto* refiningButton = this->refiningButton;
      refiningButton->cost = this->SelectedMaterialValue();
      refiningButton->Invalidate();
    }
    this->Invalidate();
  }

  const int frameCounts[7] = {4, 3, 14, 17, 20, 20, 20};
  if (frameCounts[this->state] <= this->frame) {
    return;
  }
  if (timeGetTime() <= this->stateTick + this->frameInterval) {
    return;
  }

  ++this->frame;
  if (this->state == REFINING_STATE_PROCESS) {
    if (this->frame == 7) {
      g_soundMgr->PlayWave("effect\\refine\\bs_refine_1.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    if (this->frame == 12) {
      g_soundMgr->PlayWave("effect\\refine\\bs_refine_2.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
  }

  if (frameCounts[this->state] <= this->frame) {
    switch (this->state) {
    case REFINING_STATE_WAIT:
      this->frame = 0;
      break;
    case REFINING_STATE_SUCCESS:
      this->frame = 13;
      break;
    case REFINING_STATE_FAIL:
    case REFINING_STATE_DOWNGRADE:
    case REFINING_STATE_FAIL2:
      this->frame = 18;
      break;
    default:
      this->frame = frameCounts[this->state] - 1;
      break;
    }

    if (this->state == REFINING_STATE_PROCESS) {
      if (this->result == REFINING_STATE_SUCCESS) {
        g_soundMgr->PlayWave("effect\\refine\\bs_refine_success.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      } else if (this->result == REFINING_STATE_FAIL || this->result == REFINING_STATE_DOWNGRADE || this->result == REFINING_STATE_FAIL2) {
        g_soundMgr->PlayWave("effect\\refine\\bs_refine_failed.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      }
      this->ApplyResult();
      this->SetState(this->result, 7);
      return;
    }
  }

  this->stateTick = timeGetTime();
  this->Invalidate();
}

// OFFSET: 0x007F7D80
// FUNCTION: fn_09918
void UIRefining_Notice::OnProcess() {
  if (this->expireTick != 0 && timeGetTime() > this->expireTick) {
    this->expireTick = 0;
    if (this->autoShown == 1) {
      this->autoShown = 0;
      this->geometry.visible = 0;
    }
    this->text = this->saved.text;
    this->highlight = this->saved.highlight;
    this->Invalidate();
    if (this->parent) {
      this->parent->Invalidate();
    }
  }
}

// OFFSET: 0x007F7E10
// FUNCTION: fn_09919
void UIRefiningWnd::OnRBtnDown(int x, int y) {
}

// OFFSET: 0x007F7E20
// FUNCTION: fn_09920
void UIRefiningWnd::OnRBtnUp(int x, int y) {
  if (y < 17) {
    return;
  }

  auto inRect = [&](int left, int top, int right, int bottom) {
    return x >= left && x <= right && y >= top && y <= bottom;
  };

  if (this->state == REFINING_STATE_READY) {
    if (this->selectedMaterial != -1 && inRect(81, 150, 105, 174)) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      if (!window) {
        return;
      }
      ITEM_INFO info = g_session.LookupClientItem(this->materials[this->selectedMaterial].itemId, 0);
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      return;
    }

    if (this->catalystEnabled == 1 && inRect(161, 150, 185, 174)) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      if (!window) {
        return;
      }
      ITEM_INFO info = g_session.LookupClientItem(this->catalystItemId, 0);
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
      return;
    }

    const int slot = this->SlotAt(x, y);
    if (slot >= 0) {
      if (static_cast<unsigned>(slot) < this->materials.size()) {
        ITEM_INFO info = g_session.LookupClientItem(this->materials[slot].itemId, 0);
        if (info.identity[1] > 0) {
          UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
          if (window) {
            window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
          }
        }
        return;
      }

      if (slot == 4) {
        if (this->catalystCount <= 0) {
          return;
        }
        ITEM_INFO info = g_session.LookupClientItem(this->catalystItemId, 0);
        if (info.identity[1] > 0) {
          UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
          if (window) {
            window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
          }
        }
        return;
      }
    }
  }

  if (this->item.identity[1] > 0 && static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    if (window) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
    }
  }
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

// OFFSET: 0x007F8360
// FUNCTION: fn_09923
int UIRefiningWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case REFINING_BUTTON_PROBABILITY:
      ShellExecuteA(hWnd, "open", LookupMessage(MSI_REFINING_PROBABILITY_URL), nullptr, nullptr, SW_SHOWNORMAL);
      break;
    case ID_OK:
      if (this->requestPending != 1) {
        const std::uint32_t index = this->item.identity[1];
        if (index != 0) {
          if (g_session.m_zeny < static_cast<int>(this->SelectedMaterialValue())) {
            this->notice->SetNotice(LookupMessage(MSI_NOTENOUGH_ZENY), 3000, 1);
          } else {
            this->requestPending = 1;
            RefiningRequestPacket packet;
            packet.id = ID_CZ_REQ_REFINING;
            packet.index = static_cast<std::uint16_t>(index);
            packet.materialId = this->materials[this->selectedMaterial].itemId;
            packet.useCatalyst = this->catalystEnabled;
            GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
          }
        }
      }
      break;
    case ID_CANCEL:
    case ID_CLOSE:
      if (this->state != REFINING_STATE_PROCESS) {
        RefiningClosePacket packet;
        packet.id = ID_CZ_CLOSE_REFINING_UI;
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
        g_windowMgr.CloseWindow(WID_REFININGWND);
      }
      break;
    case REFINING_BUTTON_RETURN:
      this->notice->SetNoticeInline("", 0, 0);
      this->selectedMaterial = -1;
      this->catalystEnabled = 0;
      if (this->state == REFINING_STATE_FAIL) {
        this->Reset();
      } else {
        this->SetState(REFINING_STATE_READY, 7);
      }
      break;
    case REFINING_BUTTON_AGAIN:
      this->SendMsg(this, UM_COMMAND, ID_OK, 0, 0, 0);
      break;
    }
    break;
  case UM_REFRESHITEM:
    if (this->state == REFINING_STATE_WAIT || this->state == REFINING_STATE_READY) {
      const ITEM_INFO info = g_session.LookupClientItemByIdentity(argument1);
      if (g_session.HasSwapEquipmentItem(info.identity[1]) == 1) {
        g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return -1;
      }

      if (info.identity[1] > 0) {
        if (info.identified == 1) {
          RefiningSelectPacket packet;
          packet.id = ID_CZ_REFINING_SELECT_ITEM;
          packet.index = static_cast<std::uint16_t>(argument1);
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
        } else {
          this->notice->SetNotice(LookupMessage(MSI_CANNOT_REFINE_ITEM), 3000, 1);
        }
      }
    }
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007F87B0
// FUNCTION: fn_09924
void UIRefiningWnd::ApplyItem(int id, int count, const RefiningMaterial* materials, int catalystCount) {
  switch (this->state) {
  case REFINING_STATE_WAIT:
  case REFINING_STATE_READY: {
    if (id == -1 || count == 0) {
      this->notice->SetNotice(LookupMessage(MSI_CANNOT_REFINE_ITEM), 3000, 1);
      return;
    }

    ITEM_INFO found = g_session.LookupClientItemByIdentity(id);
    if (found.identity[1] <= 0) {
      return;
    }

    this->item = found;
    int highlightColor;
    std::vector<const char*> breaks;
    char text[0x104];
    std::memset(text, 0, sizeof(text));
    char* highlight = nullptr;
    int remaining = sizeof(text);
    char* cursor = text;
    this->BuildItemName(&this->item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);
    if (this->item.identified == 1 && this->item.optionCount != 0) {
      sprintf_s(text, sizeof(text), LookupMessage(MSI_EA6), text, this->item.optionCount);
    }

    this->item.SetName(text);
    this->materials.assign(materials, materials + count);
    this->selectedMaterial = -1;
    this->catalystCount = catalystCount;
    this->catalystEnabled = 0;
    this->notice->SetNotice("", 0, 0);
    this->state = REFINING_STATE_READY;
    this->result = 7;
    this->stateTick = timeGetTime();
    this->frame = 0;
    if (this->cancelButton) {
      {
        auto* cancelButton = this->cancelButton;
        cancelButton->SetDisabled(0);
        cancelButton->Invalidate();
      }
    }

    this->result = 7;
    this->refiningButton->Enable(1);
    {
      auto* refiningButton = this->refiningButton;
      refiningButton->SetDisabled(1);
      refiningButton->Invalidate();
    }
    this->returnButton->Enable(0);
    this->againButton->Enable(0);
    this->Invalidate();
    this->requestPending = 0;
    break;
  }
  case REFINING_STATE_PROCESS:
  case REFINING_STATE_SUCCESS:
  case REFINING_STATE_FAIL:
  case REFINING_STATE_DOWNGRADE:
  case REFINING_STATE_FAIL2: {
    if (id == -1 || this->item.identity[1] != static_cast<std::uint32_t>(id)) {
      return;
    }

    ITEM_INFO found = g_session.LookupClientItemByIdentity(id);
    if (found.identity[1] <= 0) {
      return;
    }

    this->nextItem = found;
    int highlightColor;
    std::vector<const char*> breaks;
    char text[0x104];
    std::memset(text, 0, sizeof(text));
    char* cursor = nullptr;
    int remaining = sizeof(text);
    char* name = text;
    this->BuildItemName(&this->nextItem, &highlightColor, &breaks, &name, &remaining, &cursor, 1, 0);
    if (this->nextItem.identified == 1 && this->nextItem.optionCount != 0) {
      sprintf_s(text, sizeof(text), LookupMessage(MSI_EA6), text, this->nextItem.optionCount);
    }

    this->nextItem.SetName(text);
    this->nextMaterials.assign(materials, materials + count);
    this->nextCatalystCount = catalystCount;
    break;
  }
  default:
    break;
  }
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
  if (UIButton* button = this->cancelButton) {
    button->SetDisabled(0);
    button->Invalidate();
  }

  switch (state) {
  case REFINING_STATE_WAIT:
  case REFINING_STATE_READY:
    this->result = 7;
    this->refiningButton->Enable(1);
    {
      UIButton* button = this->refiningButton;
      button->SetDisabled(1);
      button->Invalidate();
    }
    this->returnButton->Enable(0);
    this->againButton->Enable(0);
    this->Invalidate();
    return;
  case REFINING_STATE_PROCESS:
    this->refiningButton->Enable(1);
    {
      UIButton* button = this->refiningButton;
      button->SetDisabled(1);
      button->Invalidate();
    }
    this->returnButton->Enable(0);
    this->againButton->Enable(0);
    if (UIButton* button = this->cancelButton) {
      button->SetDisabled(1);
      button->Invalidate();
    }
    break;
  case REFINING_STATE_SUCCESS:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFINING_SUCCEESS)), 0xFFFF00, 0, 0, 0);
    break;
  case REFINING_STATE_DOWNGRADE:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFINING_DOWNGRADE)), 0xFFFF00, 0, 0, 0);
    break;
  case REFINING_STATE_FAIL:
  case REFINING_STATE_FAIL2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_REFINING_FAIL)), 0xFFFF00, 0, 0, 0);
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
    this->saved.text = value;
    this->saved.highlight = highlight;
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

// OFFSET: 0x007F6280
// FUNCTION: fn_09908
void UIRefiningWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawRectOutline(0, 17, 264, 302, 0xC2C2C2);

  std::string background;
  switch (this->state) {
  case REFINING_STATE_WAIT:
    background = static_cast<const std::string&>(FormatOwnedString("유저인터페이스\\refining_renewal\\bg_refining_wait_%02d.bmp", this->frame));
    break;
  case REFINING_STATE_READY:
    background = static_cast<const std::string&>(FormatOwnedString("유저인터페이스\\refining_renewal\\bg_refining%s_ready_%02d.bmp", this->catalystCount > 0 ? "A" : "B", this->frame));
    break;
  case REFINING_STATE_PROCESS:
    if (this->frame <= 8) {
      background = static_cast<const std::string&>(FormatOwnedString("유저인터페이스\\refining_renewal\\bg_refining%s_process_%02d.bmp", this->catalystEnabled == 1 ? "A" : "B", this->frame));
    } else {
      background = static_cast<const std::string&>(FormatOwnedString("유저인터페이스\\refining_renewal\\bg_refining_process_%02d.bmp", this->frame));
    }
    break;
  case REFINING_STATE_SUCCESS:
    background = static_cast<const std::string&>(FormatOwnedString("유저인터페이스\\refining_renewal\\bg_refining_success_%02d.bmp", this->frame));
    break;
  case REFINING_STATE_FAIL:
  case REFINING_STATE_DOWNGRADE:
  case REFINING_STATE_FAIL2:
    background = static_cast<const std::string&>(FormatOwnedString("유저인터페이스\\refining_renewal\\bg_refining_fail_%02d.bmp", this->frame));
    break;
  }

  this->DrawBitmap(1, 17, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background.c_str()))), false);

  if (this->state == REFINING_STATE_READY) {
    if (this->selectedMaterial >= 0) {
      this->DrawBitmap(this->selectedMaterial * 49 + 11, 55,
        static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\refining_renewal\\slot_select_blue%d.bmp", this->selectedMaterial).c_str()))),
        false);
    }

    if (this->catalystEnabled == 1) {
      this->DrawBitmap(207, 55, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\refining_renewal\\slot_select_red.bmp"))), false);
    }

    int index = 0;
    for (auto material = this->materials.begin(); material != this->materials.end(); ++index, ++material) {
      const int x = index * 49 + 10;
      ITEM_INFO icon;
      icon.SetItemId(material->itemId);
      icon.quantity = g_session.LookupClientItem(material->itemId, 0).quantity;
      this->DrawItemIcon(x + 12, 69, icon, 0);
      const std::string count = FormatOwnedString("%s%d^000000/1", icon.quantity == 0 ? "^CA152E" : "", icon.quantity);
      this->DrawFormattedText(x, 107, count, 48, 6, 0, 12, 0, 0);
      if (index == this->selectedMaterial && (this->state == REFINING_STATE_READY || (this->state == REFINING_STATE_PROCESS && this->frame == 0))) {
        this->DrawItemIcon(81, 150, icon, 0);
      }
    }

    if (this->catalystItemId > 0 && this->catalystCount > 0) {
      ITEM_INFO icon;
      icon.SetItemId(this->catalystItemId);
      icon.quantity = g_session.LookupClientItem(this->catalystItemId, 0).quantity;
      this->DrawItemIcon(218, 69, icon, 0);
      const std::string count = FormatOwnedString("%s%d^000000/%d", this->catalystCount > icon.quantity ? "^CA152E" : "", icon.quantity, this->catalystCount);
      this->DrawFormattedText(206, 107, count, 48, 6, 0, 12, 0, 0);
      if (this->catalystEnabled == 1 && (this->state == REFINING_STATE_READY || (this->state == REFINING_STATE_PROCESS && this->frame < 13))) {
        this->DrawItemIcon(161, 150, icon, 0);
      }
    }
  }

  if (this->item.identity[1] > 0) {
    this->DrawItemIcon(120, 212, this->item, 0);
    if (0 < this->item.grade) {
      this->DrawBitmap(120, 224,
        static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", this->item.grade).c_str()))),
        true);
    }
  }

  switch (this->state) {
  case REFINING_STATE_READY:
    this->DrawCenteredText(0, 246, this->item.displayName.c_str(), 264, 0, 12, 0x462117, 1);
    break;
  case REFINING_STATE_FAIL:
    this->DrawCenteredText(0, 249, LookupMessage(MSI_REFINING_FAIL_BROKEN_ITEM), 264, 0, 12, 0x462117, 1);
    break;
  case REFINING_STATE_SUCCESS:
  case REFINING_STATE_DOWNGRADE:
  case REFINING_STATE_FAIL2:
    this->DrawCenteredText(0, 249, this->item.displayName.c_str(), 264, 0, 12, 0x462117, 1);
    break;
  }

  switch (this->state) {
  case REFINING_STATE_WAIT:
  case REFINING_STATE_READY:
  case REFINING_STATE_PROCESS: {
    if (this->state == REFINING_STATE_PROCESS && this->frame > 3) {
      break;
    }

    const std::string label = FormatOwnedString("%s %d%%", LookupMessage(MSI_SUCCESS), SelectedSuccessRate(this));
    const int labelWidth = this->TextWidth(label.c_str(), 0, 0, 12, 1, 0);

    const std::string percent = FormatOwnedString("%d%%", SelectedSuccessRate(this));
    const int percentWidth = this->TextWidth(percent.c_str(), 0, 0, 12, 1, 0);

    const int x = static_cast<int>((this->geometry.width - labelWidth) * 0.5);
    const int percentX = x - percentWidth + labelWidth;
    this->DrawTextAt(x, 31, LookupMessage(MSI_SUCCESS), 0, 0, 12, 0x422B1F, 1, 0);
    this->DrawTextAt(percentX, 31, percent.c_str(), 0, 0, 12, 0xDE612A, 1, 0);
    break;
  }
  case REFINING_STATE_SUCCESS: {
    const int width = this->TextWidth(LookupMessage(MSI_REFINING_SUCCESS), 0, 0, 12, 1, 0);
    this->DrawTextAt(static_cast<int>((this->geometry.width - width) * 0.5), 31, LookupMessage(MSI_REFINING_SUCCESS), 0, 0, 12, 0x422B1F, 1, 0
    );
    break;
  }
  case REFINING_STATE_FAIL:
  case REFINING_STATE_DOWNGRADE:
  case REFINING_STATE_FAIL2: {
    const int width = this->TextWidth(LookupMessage(MSI_REFINING_FAIL), 0, 0, 12, 1, 0);
    this->DrawTextAt(static_cast<int>((this->geometry.width - width) * 0.5), 31, LookupMessage(MSI_REFINING_FAIL), 0, 0, 12, 0x422B1F, 1, 0
    );
    break;
  }
  }

  this->DrawButtonBar(2, 0, 1);
  this->DrawTitleBar(false, LookupMessage(MSI_REFINING), 0);
}
