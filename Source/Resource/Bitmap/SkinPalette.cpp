#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"

// INLINED
std::uint32_t SkinPaletteColor(ResourceMgr& resources, const SkinPaletteCell& cell, bool resolveSkin) {
  auto* bitmap = static_cast<CBitmapRes*>(
      resources.Load(resolveSkin ? Resolve("유저인터페이스\\colorchip.bmp") : "유저인터페이스\\colorchip.bmp"));
  if (bitmap) {
    const std::uint32_t color = bitmap->Pixel(4 * cell.column + 2, 4 * cell.row + 2);
    return RGB((color >> 16) & 255, (color >> 8) & 255, color & 255);
  }
  return 0;
}

// OFFSET: 0x007E0F40
// FUNCTION: fn_09684
std::uint32_t __stdcall SkinPaletteColor(const SkinPaletteCell& cell, bool resolveSkin) {
  return SkinPaletteColor(ResourceMgr::Instance(), cell, resolveSkin);
}

// OFFSET: 0x007E0FC0
// FUNCTION: fn_09685
void SkinPaletteChannels(ResourceMgr& resources, int x, int y, unsigned& red, unsigned& green, unsigned& blue) {
  auto* bitmap = static_cast<CBitmapRes*>(resources.Load(Resolve("유저인터페이스\\colorchip.bmp")));
  const std::uint32_t color = bitmap->Pixel(x, y);
  red = (color >> 16) & 255;
  green = (color >> 8) & 255;
  blue = color & 255;
}
