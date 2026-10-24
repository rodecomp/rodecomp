#ifndef RAGNAROK_SOURCE_RENDER_RENDERERDX9OPERATIONS_H
#define RAGNAROK_SOURCE_RENDER_RENDERERDX9OPERATIONS_H

#include <list>
#include <vector>

class CTexture;
struct RenderPrimitive;
struct RenderCommand;
struct DeviceStateDX9;
struct TextureDX9Context;
struct DepthPrimitive;
struct RenderQueues;
struct LightmappedPrimitive;
struct ServiceOptions;
struct RendererFogDX9;
struct FogConfiguration;
class DeviceOwnerDX9;
class CRenderSystemBase;
struct tagRECT;

// Recovered operations over the renderer's borrowed device-owner state.
// This is not a substitute for the full CRendererDX9 virtual interface.
struct RendererDX9Operations {
  TextureDX9Context& context;
  DeviceStateDX9& state;

  void SetCamera(const float* eye, const float* target, const float* up);
  void SetLighting(const float* direction, const float* diffuse, const float* ambient);
  void ClearDepth(int clearColor);
  void ClearWorld(unsigned color, int mode, unsigned char worldFlag, unsigned overlay);
  void SetTextureMode(int mode);
  void DrawPrimitive(RenderCommand* primitive);
  void DrawLightmappedPrimitive(LightmappedPrimitive* primitive);
  void DrawDepthQueue(std::vector<DepthPrimitive>& queue, bool sort);
  void DrawOpaqueQueues(RenderQueues& queues);
  void DrawGroundQueues(RenderQueues& queues, const ServiceOptions& options, unsigned char multitexture);
  void DrawAlphaQueues(RenderQueues& queues, unsigned char twoPass);
  void DrawAuxiliaryQueue(RenderQueues& queues);
  void DrawQueues(
    RenderQueues& queues,
    const ServiceOptions& options,
    unsigned char multitexture,
    unsigned char twoPass,
    RendererFogDX9& fog,
    const float& aspect,
    const FogConfiguration& configuration
  );
  bool RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture);
  bool SaveScreenshot(
    DeviceOwnerDX9& owner,
    CRenderSystemBase* renderSystem,
    const char* filename,
    int format,
    CTexture* overlay,
    const tagRECT* rectangle
  );
};

#endif
