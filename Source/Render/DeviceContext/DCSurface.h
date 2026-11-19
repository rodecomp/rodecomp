#ifndef RAGNAROK_SOURCE_RENDER_DEVICECONTEXT_DCSURFACE_H
#define RAGNAROK_SOURCE_RENDER_DEVICECONTEXT_DCSURFACE_H

#include "Client/Session/Session.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/RenderSystemBase.h"

#include <memory>

class CTexture;

struct ScreenDimensions {
  int height;
  int width;
};

ScreenDimensions* DefaultResolution(ScreenDimensions* output);

struct RenderSystemHolder {
  /* 0000 */ CRenderSystemBase* system;

  RenderSystemHolder();
  ~RenderSystemHolder() noexcept;
  CRenderer* CreateRenderer();
  CSurface* CreateSurface(std::uint32_t width, std::uint32_t height);
  CTexture* CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool dynamic, bool renderTarget);
  CSurface* CreateSurfaceOrTexture(std::uint32_t width, std::uint32_t height);
  HRESULT Close();
  void DestroyRenderer(CRenderer* renderer);
  void UpdateOption();
  void RestoreDisplayMode();
  void PrepareErrorDialog();
  std::int32_t Backend() const;
  const char* BackendName() const;
  static const char* BackendNameFor(std::int32_t backend);

  // OFFSET: 0x0056A260
  // FUNCTION: fn_46361
  static int PreferredBackend() {
    switch (g_session.DefaultRenderBackend()) {
    case 1:
      return 1;
    case 2:
      return 2;
    default:
      return 2;
    }
  }

  CTexture* BackBufferTexture() const;
  std::uint32_t Width() const;
  std::uint32_t Height() const;
  bool IsWindowed() const;
  HRESULT TestDevice() const;
  HRESULT Present(bool option) const;
  HRESULT Initialize(int backend, HWND window);
  std::shared_ptr<CSurface> BackBufferSurface() const;
};


// OFFSET: 0x0055D340
// FUNCTION: fn_02102
inline RenderSystemHolder* GetRenderSystemHolder() {
  // GLOBAL: gl_24491
  static RenderSystemHolder holder;
  return &holder;
}

class CDCSurface : public CDC {
public:
  CDCSurface(std::uint32_t width, std::uint32_t height);
  explicit CDCSurface(std::shared_ptr<CSurface> surface);
  ~CDCSurface() override;
  void Reset();
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

private:
  /* 0004 */ std::shared_ptr<CSurface> m_surface;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDCSurface) == 12);
#endif

#endif
