#ifndef RAGNAROK_SOURCE_RENDER_RENDERERDX7OPERATIONS_H
#define RAGNAROK_SOURCE_RENDER_RENDERERDX7OPERATIONS_H

#include <d3d.h>
#include <list>
#include <string>

class DeviceOwnerDX7;
struct RenderCommand;
struct LightmappedPrimitive;
struct RenderPrimitive;
class CTexture;
class CTextureDX7;
struct TextureDX7Context;
struct TextureBindingDX7;
struct RendererBackBuffer;
struct BitmapImage;
class IJL15Runtime;
class ResourceMgr;
struct TextureNames;

// Original renderer snapshots its device pointer separately from the shared
// device owner used by clear/capture/presentation operations.
struct RendererDX7Operations {
  DeviceOwnerDX7& owner;
  IDirect3DDevice7* device;

  void SetLighting(const float* direction, const float* diffuse, const float* ambient);
  void ClearDepth(int clearColor);
  void ClearWorld(unsigned color, int mode, unsigned char worldFlag, unsigned overlay);
  void DrawPrimitive(RenderCommand* primitive);
  void DrawLightmappedPrimitive(LightmappedPrimitive* primitive);
  void SetTextureMode(int mode);
  bool RenderToTexture(
    int width,
    int height,
    const std::list<RenderPrimitive*>& primitives,
    CTexture*& texture,
    TextureDX7Context& context,
    TextureBindingDX7& binding
  );
  bool
  SaveBitmap(const char* filename, const BitmapImage& overlay, int width, int height, RendererBackBuffer& backBuffer);
  bool SaveJpeg(
    const char* filename,
    const BitmapImage& overlay,
    int width,
    int height,
    RendererBackBuffer& backBuffer,
    IJL15Runtime& runtime
  );
  bool SaveLogo(std::string filename, ResourceMgr& resources, TextureNames& names, IJL15Runtime& runtime);
};

#endif
