#include "Resource/SkinPalette.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

// OFFSET: 0x007E0F40
// FUNCTION: fn_09684
std::uint32_t SkinPaletteColor(ResourceMgr& resources, TextureNames& names, const SkinPaletteCell& cell, bool resolveSkin) {
  const char* filename = "유저인터페이스\\colorchip.bmp";
  if (resolveSkin) {
    filename = names.Resolve(filename);
  }
  auto* bitmap = static_cast<CBitmapRes*>(resources.Load(filename));
  if (!bitmap) {
    return 0;
  }
  const std::uint32_t color = bitmap->image.Pixel(4 * cell.column + 2, 4 * cell.row + 2);
  return ((color & 255) << 16) | (color & 0xFF00) | ((color >> 16) & 255);
}

// OFFSET: 0x007E0FC0
// FUNCTION: fn_09685
void SkinPaletteChannels(ResourceMgr& resources, TextureNames& names, int x, int y, unsigned& red, unsigned& green, unsigned& blue) {
  auto* bitmap = static_cast<CBitmapRes*>(resources.Load(names.Resolve("유저인터페이스\\colorchip.bmp")));
  const std::uint32_t color = bitmap->image.Pixel(x, y);
  red = (color >> 16) & 255;
  green = (color >> 8) & 255;
  blue = color & 255;
}
