#ifndef RAGNAROK_SOURCE_RENDER_DEVICECONTEXT_DCBITMAP_H
#define RAGNAROK_SOURCE_RENDER_DEVICECONTEXT_DCBITMAP_H

#include "Render/Textures/BitmapPixels.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Textures/Texture.h"

#include <list>

class CDCBitmap : public CDC {
public:
  __declspec(noinline) CDCBitmap(int width, int height);
  ~CDCBitmap() override;
  void Grow(std::uint32_t width, std::uint32_t height) override;
  bool GetDC(HDC* output) override;
  void ReleaseDC(HDC dc) override;
  void Copy(int x, int y, int width, int height, const std::uint32_t* source, int transparent) override;
  void ScaleCopy(int x, int y, int sourceWidth, int sourceHeight, int width, int height, const std::uint32_t* source, int option) override;
  void CopyRegion(
    int x, int y, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, const std::uint32_t* source, int sourceWidth, int sourceHeight,
    bool transparent, bool alpha
  ) override;
  void CopyFrom(int x, int y, int sourceX, int sourceY, int width, int height, const CDCBitmap& source) override;
  void BlendFrom(std::uint32_t x, std::uint32_t y, int width, int height, const CDCBitmap& source) override;
  void DrawRleSprite(
    int x, int y, const CSprRes& sprite, const ActionFrame& frame, const std::uint32_t* palette
  ) override;
  void DrawSurface(
    int x, int y, CSurface* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int mirrored, int scaleX, int scaleY, int filter
  ) override;
  void Fill(const RECT* rectangle, std::uint32_t color) override;
  void Draw(int x, int y, int width, int height, std::uint32_t color) override;
  void DrawScaledSprite(
    int x, int y, int width, int height, const CSprRes* sprite, const ActionFrame* frame, int offsetX, int offsetY, float scale,
    const std::uint32_t* palette
  ) override;
  void DrawScaledSurface(
    int x, int y, int width, int height, CSurface* source, int sourceWidth, int sourceHeight, float scaleX, float scaleY, std::uint32_t color, bool mirrored,
    int offsetX, int offsetY, int sourceX, int sourceY
  ) override;
  void Brighten(int x, int y, int width, int height, int amount) override;
  void Dim(int x, int y, int width, int height, int percent) override;
  void UploadTexture(CTexture*& texture);
  void UpdateCache();
  HGDIOBJ Create(int width, int height);
  void ApplyPixelFilter(int filter, unsigned int* red, unsigned int* green, unsigned int* blue);
  void FilterPixel1555(int filter, int pixel, unsigned int* red, unsigned int* green, unsigned int* blue);
  void FilterPixel4444(int filter, int pixel, unsigned int* red, unsigned int* green, unsigned int* blue);
  void SetAlpha(unsigned char maxLuminance);
  // INLINED
  void SetAlpha(bool alpha) {
    this->m_alpha = alpha;
  }
  // INLINED
  void ClearTextures() {
    for (std::list<CTexture*>::iterator it = this->m_textures.begin(); it != this->m_textures.end(); ++it) {
      delete *it;
    }

    this->m_textures.clear();
  }

protected:
  /* 0004 */ BitmapPixels m_bitmap;
  /* 0020 */ std::list<CTexture*> m_textures;
  /* 0028 */ bool m_alpha;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDCBitmap) == 44);
#endif

CDCBitmap* CreateBitmapDC(int width, int height, bool fill);

#endif
