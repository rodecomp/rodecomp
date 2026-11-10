#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Chat/UIWhisperWnd.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "UI/Text/TextFitting.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

// OFFSET: 0x008D0D80
// FUNCTION: fn_12090
void UISkillDescribeWnd::DrawTransBox() {
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
}

// OFFSET: 0x008D0DB0
// FUNCTION: fn_12091
void UIWhisperWnd::DrawTransBox() {
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
  DrawAlphaRectangle(x + 1, y + 1, this->geometry.width - 2, this->geometry.height, 0x66000000);
  DrawAlphaRectangle(x + 1, y + 1, this->geometry.width - 2, 17, 0x449EB6E4);
}

// OFFSET: 0x008D0F30
// FUNCTION: fn_12092
void UIEquipWnd::DrawDamageSkinPage() {
  this->DrawBitmap(
    0, this->m_contentTop + 14,
    static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ShowDamage\\bg_damage.bmp"))),
    false
  );
  int slot = 0;
  for (int x = 6; x < 0x116; x += 0x44) {
    int index = this->m_firstSlot + slot;
    if (index >= g_session.damageSkinCount) {
      break;
    }
    if (g_session.m_damageSkinIndex == index) {
      this->DrawBitmap(
        x, 0x3A,
        static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\showdamage\\btn_damage_pick.bmp").c_str())
        )),
        true
      );
    } else if (this->m_hoverSlot == index) {
      this->DrawBitmap(
        x, 0x3A,
        static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\showdamage\\btn_damage_press.bmp").c_str())
        )),
        true
      );
    } else {
      this->DrawBitmap(
        x, 0x3A,
        static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\showdamage\\btn_damage.bmp").c_str())
        )),
        true
      );
    }
    int height;
    unsigned char bold;
    if (g_session.m_damageSkinIndex == index) {
      this->DrawCenteredText(x + 1, 0x45, this->m_damageSkinNames[index].c_str(), 0x40, 0, 14, 0xBABABA, 1);
      height = 14;
      bold = 1;
    } else {
      this->DrawCenteredText(x + 1, 0x45, this->m_damageSkinNames[index].c_str(), 0x40, 0, 12, 0xBABABA, 0);
      height = 12;
      bold = 0;
    }
    this->DrawCenteredText(x, 0x44, this->m_damageSkinNames[index].c_str(), 0x40, 0, height, 0x323232, bold);
    this->DrawBitmap(
      x + 14, 0x5A,
      static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
        Resolve(FormatOwnedString("%s.bmp", this->m_damageSkinIcons[index].c_str()).c_str())
      )),
      true
    );
    ++slot;
  }
}

namespace {
  struct ItemNameTextInfo {
    int x;
    int y;
    int height;
    int family;
  };
}

// OFFSET: 0x008D13E0
// FUNCTION: fn_12093
int UIFrameWnd::DrawFittedItemName(int x, int y, const ITEM_INFO* item, std::uint32_t color, int, int, char option) {
  std::vector<char*> breaks;
  char buffer[256];
  std::memset(buffer, 0, sizeof(buffer));
  char* highlight = nullptr;
  std::size_t remaining = sizeof(buffer);
  char* cursor = buffer;
  int highlightColor;
  this->BuildItemName(item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 0, option);

  char* text = buffer;
  int offset = 0;
  for (auto it = breaks.begin(); it != breaks.end(); ++it) {
    std::string fitted = FitText(*this->geometry.surface, std::string(text), this->geometry.width - x - 10);
    bool truncated;
    char* end;
    if (std::strcmp(fitted.c_str(), text) != 0) {
      truncated = true;
      strcpy_s(buffer, sizeof(buffer), fitted.c_str());
      end = text + std::strlen(text) + 1;
    } else {
      end = *it;
      truncated = false;
    }

    int bytes = static_cast<int>(end - text);
    if (bytes == 0) {
      continue;
    }

    if (text == highlight) {
      this->DrawShadowedText(offset + x, y, text, bytes, static_cast<std::uint32_t>(highlightColor), 0xE0E0E0, 0, 12, 0);
    } else if (item->damaged) {
      this->DrawShadowedText(offset + x, y, text, bytes, color, 0x5050FA, 0, 12, 0);
    } else {
      std::uint32_t card = ItemCardColor(item);
      if (card == 0x00ADFF2F) {
        card = 0x00669900;
      }
      color = (card >> 16 & 0xFF) | ((card & 0xFF) << 16) | (card & 0x0000FF00);
      this->DrawShadowedText(offset + x, y, text, bytes, color, 0xFFFFFF, 0, 12, 0);
    }
    offset += this->TextWidth(text, bytes, 0, 12, 0, 0);
    text = end;
    if (truncated) {
      break;
    }
  }
  return offset;
}

// OFFSET: 0x008D17C0
// FUNCTION: fn_12094
int UIFrameWnd::DrawItemName(int x, int y, int width, const ITEM_INFO* item, int, int family, int height) {
  int highlightColor = 0;
  std::vector<char*> breaks;
  char buffer[256] = {};
  char* cursor = buffer;
  std::size_t remaining = sizeof(buffer);
  char* highlight = nullptr;
  this->BuildItemName(item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);

  std::uint32_t color = ItemCardColor(item);
  if (color == 0x00ADFF2F) {
    color = 0x00669900;
  }
  color = (color >> 16 & 0xFF) | (color & 0x0000FF00) | ((color & 0xFF) << 16);

  const ItemNameTextInfo info = {x, y, height, family};
  this->DrawItemNameText(&info, buffer, highlight, color, static_cast<std::uint32_t>(highlightColor), width, &breaks);
  return x;
}

// OFFSET: 0x008D1980
// FUNCTION: fn_12095
int UIFrameWnd::DrawWrappedItemName(int x, int y, int width, const ITEM_INFO* item, int, int family, int lineHeight) {
  char buffer[256] = {};
  char* highlight = nullptr;
  char* cursor = buffer;
  std::vector<char*> breaks;
  std::size_t remaining = sizeof(buffer);
  int highlightColor;
  this->BuildItemName(item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);

  std::uint32_t color = ItemCardColor(item);
  if (color == 0x00ADFF2F) {
    color = 0x00669900;
  }
  color = (color >> 16 & 0xFF) | ((color & 0xFF) << 16) | (color & 0x0000FF00);
  return this->DrawItemNameSegments(
    x, y, width, breaks, buffer, highlight, static_cast<std::uint32_t>(highlightColor), color, family, lineHeight
  );
}
