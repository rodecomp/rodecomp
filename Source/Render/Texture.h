#ifndef RAGNAROK_SOURCE_RENDER_TEXTURE_H
#define RAGNAROK_SOURCE_RENDER_TEXTURE_H

#include "Render/Surface.h"

class RenderPrimitivePool;
struct RenderQueues;

// STATIC: 0x01312DE0 / 0x01312DE4
extern float g_textureOffsetX;
extern float g_textureOffsetY;

// Actual CTexture base. Device methods remain abstract.
class CTexture : public CSurface {
public:
  CTexture();
  ~CTexture() override;
  std::uint32_t Width() override;
  std::uint32_t Height() override;
  virtual void Update(int option) = 0;
  virtual void UploadSprite(int x, int y, int width, int height, const void* image, int palette) = 0;
  virtual bool CopyTexture(
    CTexture* source,
    int sourceX,
    int sourceY,
    int sourceWidth,
    int sourceHeight,
    int x,
    int y,
    int width,
    int height
  ) = 0;
  virtual bool Create(int width, int height, int format) = 0;

  // Original Draw/Stretch algorithms with renderer dependencies explicit.
  // They do not replace unresolved virtual backend dispatch/global ownership.
  void DrawTo(
    RenderPrimitivePool& pool,
    RenderQueues& queues,
    int x,
    int y,
    int width,
    int height,
    std::uint32_t color,
    bool depthAlpha,
    float depthStart,
    float depthEnd
  );
  void StretchTo(
    RenderPrimitivePool& pool,
    RenderQueues& queues,
    int x,
    int y,
    int width,
    int height,
    bool depthAlpha,
    float depthStart,
    float depthEnd
  );
  std::uint32_t SetImageSize(std::uint32_t width, std::uint32_t height);
  std::uint32_t Retain();
  std::uint32_t Release();
  int SetName(const char* name);

  // Source accessors for original direct texture-allocation dimensions.
  std::uint32_t TextureWidth() const {
    return this->m_textureWidth;
  }
  std::uint32_t TextureHeight() const {
    return this->m_textureHeight;
  }
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
