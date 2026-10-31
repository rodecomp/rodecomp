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
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstdint>
#include <vector>

// OFFSET: 0x008D0D80
// FUNCTION: fn_12090
void UISkillDescribeWnd::Slot43() {
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
}

// OFFSET: 0x008D0DB0
// FUNCTION: fn_12091
void UIWhisperWnd::Slot43() {
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
  DrawAlphaRectangle(
    g_renderer->primitives, g_renderer->queues, x + 1, y + 1, this->geometry.width - 2, this->geometry.height, 0x66000000,
    g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
  DrawAlphaRectangle(
    g_renderer->primitives, g_renderer->queues, x + 1, y + 1, this->geometry.width - 2, 17, 0x449EB6E4,
    g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
}

// OFFSET: 0x008D0F30
// FUNCTION: fn_12092
void UIEquipWnd::DrawDamageSkinPage() {
  this->DrawBitmap(
    0, this->m_contentTop + 14,
    &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ShowDamage\\bg_damage.bmp")))->image,
    false
  );
  int slot = 0;
  for (int x = 6; x < 0x116; x += 0x44) {
    int index = this->m_firstSlot + slot;
    if (index >= g_session.unknown_80C8) {
      break;
    }
    if (g_session.m_damageSkinIndex == index) {
      this->DrawBitmap(
        x, 0x3A,
        &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\showdamage\\btn_damage_pick.bmp").c_str())
        ))->image,
        true
      );
    } else if (this->m_hoverSlot == index) {
      this->DrawBitmap(
        x, 0x3A,
        &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\showdamage\\btn_damage_press.bmp").c_str())
        ))->image,
        true
      );
    } else {
      this->DrawBitmap(
        x, 0x3A,
        &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
          Resolve(FormatOwnedString("유저인터페이스\\showdamage\\btn_damage.bmp").c_str())
        ))->image,
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
      &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
        Resolve(FormatOwnedString("%s.bmp", this->m_damageSkinIcons[index].c_str()).c_str())
      ))->image,
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

  using BuildItemNameFn = int(__thiscall*)(UIFrameWnd*, const ITEM_INFO*, int*, std::vector<char*>*, char**, std::size_t*, char**, char, char);
  using DrawItemNameTextFn = void(__thiscall*)(
    UIFrameWnd*, const ItemNameTextInfo*, const char*, const char*, std::uint32_t, std::uint32_t, int, const std::vector<char*>*
  );
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
  reinterpret_cast<BuildItemNameFn>(0x008DA690)(
    this, item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0
  );

  std::uint32_t color = ItemCardColor(item);
  if (color == 0x00ADFF2F) {
    color = 0x00669900;
  }
  color = (color >> 16 & 0xFF) | (color & 0x0000FF00) | ((color & 0xFF) << 16);

  const ItemNameTextInfo info = {x, y, height, family};
  reinterpret_cast<DrawItemNameTextFn>(0x0090A2E0)(
    this, &info, buffer, highlight, color, static_cast<std::uint32_t>(highlightColor), width, &breaks
  );
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
  reinterpret_cast<BuildItemNameFn>(0x008DA690)(
    this, item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0
  );

  std::uint32_t color = ItemCardColor(item);
  if (color == 0x00ADFF2F) {
    color = 0x00669900;
  }
  return this->DrawItemNameSegments(
    x, y, width, breaks, buffer, highlight, static_cast<std::uint32_t>(highlightColor),
    (color >> 16 & 0xFF) | ((color & 0xFF) << 16) | (color & 0x0000FF00), family, lineHeight
  );
}
