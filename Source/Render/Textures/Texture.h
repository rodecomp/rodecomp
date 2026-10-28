#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_TEXTURE_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_TEXTURE_H

#include "Render/Textures/Surface.h"

class RenderPrimitivePool;
struct RenderQueues;
struct SpriteRleImage;

enum TextureFormat : std::int32_t { UnknownTextureFormat = -1, A1R5G5B5 = 0, A4R4G4B4 = 1, R5G6B5 = 2, X1R5G5B5 = 3, A8R8G8B8 = 4 };

extern float g_textureOffsetX;
extern float g_textureOffsetY;

class CTexture : public CSurface {
public:
  CTexture();
  ~CTexture() override;
  std::uint32_t Width() override;
  std::uint32_t Height() override;
  virtual void Update(const RECT* rectangle) = 0;
  virtual void UploadSprite(int x, int y, int width, int height, const SpriteRleImage* image, const std::uint32_t* palette) = 0;
  virtual bool CopyTexture(CTexture* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int x, int y, int width, int height) = 0;
  virtual bool Create(int width, int height, int format) = 0;

  void DrawTo(
    RenderPrimitivePool& pool, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, bool depthAlpha, float depthStart, float depthEnd
  );
  void StretchTo(RenderPrimitivePool& pool, RenderQueues& queues, int x, int y, int width, int height, bool depthAlpha, float depthStart, float depthEnd);
  void StretchTintedTo(
    RenderPrimitivePool& pool, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, bool depthAlpha, float depthStart, float depthEnd
  );
  std::uint32_t SetImageSize(std::uint32_t width, std::uint32_t height);
  std::uint32_t Retain();
  std::uint32_t Touch();
  std::uint32_t Release();
  int SetName(const char* name);

  // INLINED
  std::uint32_t TextureWidth() const {
    return this->m_textureWidth;
  }
  // INLINED
  std::uint32_t TextureHeight() const {
    return this->m_textureHeight;
  }
  // INLINED
  std::uint32_t ImageWidth() const {
    return this->m_width;
  }
  // INLINED
  std::uint32_t ImageHeight() const {
    return this->m_height;
  }
  // INLINED
  std::int32_t Format() const {
    return this->m_format;
  }

protected:
  friend class SpriteAtlas;
  friend class TextureCache;
  /* 0004 */ std::int32_t m_format;
  /* 0008 */ std::uint16_t unknown_0008;
  /* 000A */ std::uint8_t unknown_000A[2];
  /* 000C */ std::uint32_t m_textureWidth;
  /* 0010 */ std::uint32_t m_textureHeight;
  /* 0014 */ std::uint32_t m_width;
  /* 0018 */ std::uint32_t m_height;
  /* 001C */ std::uint32_t unknown_001C;
  /* 0020 */ std::uint32_t unknown_0020;
  /* 0024 */ char m_name[256];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CTexture) == 292);
#endif

#endif
