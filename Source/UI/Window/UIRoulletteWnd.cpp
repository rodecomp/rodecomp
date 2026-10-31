#include "UI/Window/UIRoulletteWnd.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RouletteMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UISpriteButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdio>

// GLOBAL: gl_24881
int g_effectDetailLevel;

namespace {
#pragma pack(push, 1)
struct RouletteGeneratePacket {
  /* 0000 */ std::int16_t id;
};

struct RouletteReceiveItemPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ unsigned char condition;
};
#pragma pack(pop)
}

// OFFSET: 0x0080B930
// FUNCTION: fn_10083
UIRoulletteWnd::UIRoulletteWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0),
      m_prizeButton(nullptr),
      m_prizeSprite(nullptr),
      m_stageArrow(nullptr),
      m_enabled(1),
      m_startButton(nullptr),
      m_column(0),
      m_row(0),
      m_goldText(nullptr),
      m_silverText(nullptr),
      m_bronzeText(nullptr),
      m_startSprite(nullptr) {
  this->m_items.clear();
}

// OFFSET: 0x0080BAF0
// FUNCTION: fn_10084
UIRoulletteWnd::~UIRoulletteWnd() = default;

// OFFSET: 0x0080BBB0
// FUNCTION: fn_10085
ITEM_INFO UIRoulletteWnd::ItemAt(int x, int y) {
  this->m_row = 0;
  this->m_column = 0;

  int row = 0;
  for (int top = 344; top > 64; top -= 40, ++row) {
    if (top + 26 < y || top > y) {
      continue;
    }

    this->m_row = row;
    const int count = 9 - row;
    int right = row * 36 + 185;
    for (int column = 0; column < count; ++column, right += 36) {
      if (right - 27 > x || right < x) {
        continue;
      }

      this->m_column = column;
      for (auto it = g_rouletteMgr->m_items.begin(); it != g_rouletteMgr->m_items.end(); ++it) {
        if (row == it->first.row && column == it->first.column) {
          return it->second;
        }
      }
      return ITEM_INFO(0);
    }
    break;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x0080BD10
// FUNCTION: fn_10086
void UIRoulletteWnd::Slot15(int, int) {
  this->AddCloseButton(*reinterpret_cast<UIButtonContext*>(nullptr), "", 0);

  auto* startButton = new UIBitmapButton(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
  this->m_startButton = startButton;
  startButton->SetBitmap("유저인터페이스\\basic_interface\\roullette\\StartRoullette_a.bmp", 0, 0);
  startButton->SetBitmap("유저인터페이스\\basic_interface\\roullette\\StartRoullette_a.bmp", 1, 0);
  startButton->SetBitmap("유저인터페이스\\basic_interface\\roullette\\StartRoullette_b.bmp", 2, 0);
  startButton->SetSize(startButton->BitmapWidth(), startButton->BitmapHeight());
  startButton->Move(-100, -100);
  startButton->SetId(409);
  this->AddChild(startButton);

  if (g_effectDetailLevel == 2) {
    auto* startSprite = new UISpriteButton(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
    this->m_startSprite = startSprite;
    startSprite->SetSprite("roullette\\Start.spr");
    startSprite->SetActName("roullette\\Start.act");
    startSprite->SetSize(99, 99);
    startSprite->Move(-100, -100);
    startSprite->unknown_00BC = 0;
    startSprite->unknown_00C0 = 0;
    startSprite->unknown_00C4 = 1;
    this->AddChild(startSprite);

    auto* prizeSprite = new UISpriteButton(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
    this->m_prizeSprite = prizeSprite;
    prizeSprite->SetSprite("roullette\\GetWinPrize.spr");
    prizeSprite->SetActName("roullette\\GetWinPrize.act");
    prizeSprite->SetSize(99, 99);
    prizeSprite->Move(-100, -100);
    prizeSprite->unknown_00BC = 0;
    prizeSprite->unknown_00C0 = 0;
    prizeSprite->unknown_00C4 = 1;
    this->AddChild(prizeSprite);

    auto* stageArrow = new UISpriteButton(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
    this->m_stageArrow = stageArrow;
    stageArrow->SetSprite("roullette\\stageArrow.spr");
    stageArrow->SetActName("roullette\\stageArrow.act");
    stageArrow->SetSize(35, 40);
    stageArrow->Move(-100, -100);
    stageArrow->unknown_00BC = 0;
    stageArrow->unknown_00C0 = 0;
    stageArrow->unknown_00C4 = 1;
    this->AddChild(stageArrow);
  }

  auto* prizeButton = new UIBitmapButton(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
  this->m_prizeButton = prizeButton;
  prizeButton->SetBitmap("유저인터페이스\\basic_interface\\roullette\\GetWinPrize_a.bmp", 0, 0);
  prizeButton->SetBitmap("유저인터페이스\\basic_interface\\roullette\\GetWinPrize_a.bmp", 1, 0);
  prizeButton->SetBitmap("유저인터페이스\\basic_interface\\roullette\\GetWinPrize_b.bmp", 2, 0);
  prizeButton->SetSize(prizeButton->BitmapWidth(), prizeButton->BitmapHeight());
  prizeButton->Move(-100, -100);
  prizeButton->SetId(410);
  this->AddChild(prizeButton);

  auto* goldText = new UIStaticText(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
  this->m_goldText = goldText;
  goldText->SetSize(25, 12);
  goldText->Move(575, 134);
  goldText->SetColor(0, 0, 0);
  this->AddChild(goldText);

  auto* silverText = new UIStaticText(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
  this->m_silverText = silverText;
  silverText->SetSize(25, 12);
  silverText->Move(575, 218);
  silverText->SetColor(0, 0, 0);
  this->AddChild(silverText);

  auto* bronzeText = new UIStaticText(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr));
  this->m_bronzeText = bronzeText;
  bronzeText->SetSize(25, 12);
  bronzeText->Move(575, 302);
  bronzeText->SetColor(0, 0, 0);
  this->AddChild(bronzeText);
}

// OFFSET: 0x0080C220
// FUNCTION: fn_10087
void UIRoulletteWnd::Paint() {
  const TextLocale locale = {};
  this->DrawTitleBar(g_fontCache, locale, true, LookupMessage(2641), 0);
  this->DrawPanelBackground(17, 0, "유저인터페이스\\basic_interface\\roullette\\roulletteBG.bmp");

  char buffer[40] = {};
  std::sprintf(buffer, "%d", g_rouletteMgr->m_goldPoint);
  this->m_goldText->SetTextFitted(buffer);
  std::sprintf(buffer, "%d", g_rouletteMgr->m_silverPoint);
  this->m_silverText->SetTextFitted(buffer);
  std::sprintf(buffer, "%d", g_rouletteMgr->m_bronzePoint);
  this->m_bronzeText->SetTextFitted(buffer);

  if (g_rouletteMgr->m_spinning != 0 || (g_rouletteMgr->m_resultShown == 1 && g_rouletteMgr->m_bonus == 0)) {
    CBitmapRes* arrow =
      static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\roullette\\stageArrow.bmp")));
    if (arrow != nullptr) {
      this->DrawBitmap(482, 344 - g_rouletteMgr->m_stage * 40, &arrow->image, true);
    }
  }

  this->UpdateButtons();

  ItemDescriptions descriptions{g_itemDescriptions};
  for (auto it = g_rouletteMgr->m_items.begin(); it != g_rouletteMgr->m_items.end(); ++it) {
    const int row = it->first.row;
    const int column = it->first.column;
    const int x = (row + column) * 36 + 159;
    const int y = 344 - row * 40;
    if (g_rouletteMgr->m_stage == row && (g_rouletteMgr->m_resultShown != 0 || g_rouletteMgr->m_spinning != 0)) {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, g_fontCache, locale, x, y, it->second, 0, 0);
    } else {
      DrawItemWithQuantityAuxiliary(*this, descriptions, g_fontCache, locale, 1, x, y, it->second, 0, 0);
    }
  }

  if (g_rouletteMgr->m_resultShown == 1 && g_rouletteMgr->m_resultItem.quantity != 0) {
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, g_fontCache, locale, 189, 145, g_rouletteMgr->m_resultItem, 0, 0);
  }

  if (g_rouletteMgr->m_additionItem.quantity != 0) {
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, g_fontCache, locale, 60, 135, g_rouletteMgr->m_additionItem, 0, 0);
  }
}

// OFFSET: 0x0080C490
// FUNCTION: fn_10088
void UIRoulletteWnd::Slot25(int, int) {
}

// OFFSET: 0x0080C4A0
// FUNCTION: fn_10089
void UIRoulletteWnd::Slot28(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  int tooltipX = (this->m_column + this->m_row) * 36 + 155;
  int tooltipY = 330 - this->m_row * 40;
  if (y >= 143 && y <= 173 && x >= 186 && x <= 216) {
    tooltipX = 186;
    tooltipY = 130;
    if (g_rouletteMgr->m_resultShown == 1) {
      item = g_rouletteMgr->m_resultItem;
    }
  }
  if (y >= 133 && y <= 163 && x >= 60 && x <= 88) {
    tooltipX = 57;
    tooltipY = 121;
    item = g_rouletteMgr->m_additionItem;
  }

  if (item.quantity != 0) {
    this->ToScreen(tooltipX, tooltipY);
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
    if (item.identified == 1 && item.unknown_0098 != 0) {
      tooltip->Show(FormatOwnedString(LookupMessage(2600), naming.DisplayName(item, 0).c_str(), item.unknown_0098).c_str(), tooltipX, tooltipY,
                    g_session.GetItemNameColor(item), 0, 1);
    } else {
      tooltip->Show(naming.DisplayName(item, 0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x0080C730
// FUNCTION: fn_10090
void UIRoulletteWnd::Slot19() {
  if (g_effectDetailLevel != 2) {
    if (this->m_startButton->geometry.unknown_0030 == 1 && this->m_enabled == 1) {
      if (g_rouletteMgr->m_spinning == 0) {
        CBitmapRes* arrow =
          static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\roullette\\stageArrow.bmp")));
        this->DrawBitmap(480, 337 - (this->CoinType() + g_rouletteMgr->m_stage) * 40, &arrow->image, true);
      }
      if (g_rouletteMgr->m_resultShown == 1 && g_rouletteMgr->m_bonus == 0) {
        CBitmapRes* arrow =
          static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\roullette\\stageArrow.bmp")));
        this->DrawBitmap(480, 297 - g_rouletteMgr->m_stage * 40, &arrow->image, true);
      }
      this->m_startButton->SetState(0);
    }
    return;
  }

  if (this->m_startButton->geometry.unknown_0030 == 1 && this->m_enabled == 1) {
    if (g_rouletteMgr->m_spinning == 0) {
      this->m_startSprite->Move(339, 398);
      this->m_startSprite->SetTooltip(LookupMessage(2684));
      this->m_stageArrow->Move(480, 337 - (this->CoinType() + g_rouletteMgr->m_stage) * 40);
    }
    if (g_rouletteMgr->m_resultShown == 1 && g_rouletteMgr->m_bonus == 0) {
      this->m_stageArrow->Move(480, 297 - g_rouletteMgr->m_stage * 40);
    }
    this->m_startButton->SetState(0);
  }

  if (this->m_startSprite->geometry.unknown_0030 == 2) {
    this->m_startSprite->SetState(3);
    this->m_startSprite->Move(-100, -100);
    this->m_startSprite->SetTooltip("");
    this->SendMsg(nullptr, 6, 409, 0, 0, 0);
    this->m_startButton->SetState(2);
  }

  if (this->m_prizeButton->geometry.unknown_0030 == 1) {
    this->m_prizeSprite->Move(203, 398);
    this->m_prizeSprite->SetTooltip(LookupMessage(2683));
    this->m_prizeButton->SetState(0);
  }

  if (this->m_prizeSprite->geometry.unknown_0030 == 2) {
    this->m_prizeSprite->SetState(3);
    this->m_prizeSprite->Move(-100, -100);
    this->m_prizeSprite->SetTooltip("");
    this->SendMsg(nullptr, 6, 410, 0, 0, 0);
    this->m_prizeButton->SetState(2);
  }

  if (g_modeMgr.gameMode->cursorAction != 2 || g_rouletteMgr->m_spinning == 1) {
    this->m_startSprite->Move(-100, -100);
    this->m_prizeSprite->Move(-100, -100);
    this->m_stageArrow->Move(-100, -100);
    this->Invalidate();
  }

  if (g_rouletteMgr->m_resultShown == 1) {
    this->m_prizeSprite->Invalidate();
  }

  if (g_rouletteMgr->m_spinning == 0) {
    this->m_startSprite->Invalidate();
    this->m_stageArrow->Invalidate();
  }
}

// OFFSET: 0x0080CA70
// FUNCTION: fn_10091
void UIRoulletteWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);

  if (static_cast<unsigned>(y - 143) < 31 && static_cast<unsigned>(x - 186) < 31 && g_rouletteMgr->m_resultShown == 1) {
    item = g_rouletteMgr->m_resultItem;
  }

  if (static_cast<unsigned>(y - 133) < 31 && static_cast<unsigned>(x - 60) < 29) {
    item = g_rouletteMgr->m_additionItem;
  }

  if (item.quantity != 0) {
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0080CB90
// FUNCTION: fn_10092
int UIRoulletteWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 201) {
      if (g_rouletteMgr->m_spinning == 0) {
        if (g_windowMgr.unknown_0464 != nullptr) {
          this->m_frameContext->windows.CloseWindow(269);
          return 0;
        }
      } else {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2640)), 0x6464FF, 0, 0, 0);
      }
    } else if (argument1 == 409) {
      if (this->m_enabled == 1) {
        RouletteGeneratePacket packet;
        packet.id = ID_CZ_REQ_GENERATE_ROULETTE;
        GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
        this->m_enabled = 0;
        return 0;
      }
    } else if (argument1 == 410 && g_rouletteMgr->m_resultShown == 1) {
      RouletteReceiveItemPacket packet;
      packet.id = ID_CZ_RECV_ROULETTE_ITEM;
      packet.condition = static_cast<unsigned char>(argument2);
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
      return 0;
    }
    return 0;
  }

  if (message == 60) {
    this->Invalidate();
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0080CCE0
// FUNCTION: fn_10093
void UIRoulletteWnd::UpdateButtons() {
  if (g_effectDetailLevel == 2) {
    if (g_rouletteMgr->m_stage != 6 &&
        (g_rouletteMgr->m_bronzePoint > 0 || g_rouletteMgr->m_silverPoint >= 10 || g_rouletteMgr->m_goldPoint >= 10)) {
      this->m_startButton->Move(339, 398);
    }
    if (g_rouletteMgr->m_spinning != 0) {
      this->m_startButton->Move(-100, -100);
      this->m_stageArrow->Move(-100, -100);
    }
    if (g_rouletteMgr->m_resultShown != 1) {
      this->m_prizeSprite->Move(-100, -100);
      this->m_prizeButton->Move(-100, -100);
      return;
    }
    this->m_prizeButton->Move(203, 398);
  } else {
    if (g_rouletteMgr->m_stage != 6 &&
        (g_rouletteMgr->m_bronzePoint > 0 || g_rouletteMgr->m_silverPoint >= 10 || g_rouletteMgr->m_goldPoint >= 10)) {
      this->m_startButton->Move(339, 398);
    }
    if (g_rouletteMgr->m_spinning != 0) {
      this->m_startButton->Move(-100, -100);
    }
    if (g_rouletteMgr->m_resultShown != 1) {
      this->m_prizeButton->Move(-100, -100);
      return;
    }
    this->m_prizeButton->Move(203, 398);
  }

  if (g_rouletteMgr->m_stage != 6) {
    this->m_startButton->Move(339, 398);
  }
}

// OFFSET: 0x0080CE40
// FUNCTION: fn_10094
int UIRoulletteWnd::CoinType() const {
  const RouletteMgr* roulette = g_rouletteMgr;

  if (roulette->m_resultShown == 1) {
    if (roulette->m_stage + 1 < 7 && roulette->m_bonus == 0) {
      return 1;
    }
  } else if (roulette->m_bronzePoint <= 0) {
    if (roulette->m_silverPoint >= 10) {
      return 2;
    }
    if (roulette->m_goldPoint >= 10) {
      return 4;
    }
  }

  return 0;
}
