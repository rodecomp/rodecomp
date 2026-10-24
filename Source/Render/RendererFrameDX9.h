#ifndef RAGNAROK_SOURCE_RENDER_RENDERERFRAMEDX9_H
#define RAGNAROK_SOURCE_RENDER_RENDERERFRAMEDX9_H

struct RendererDX9Operations;
struct DeviceOwnerDX9;
class RenderCommandPool;
class RenderPrimitivePool;
class LightmappedPrimitivePool;
class RenderVertexPool;
struct EffectRetirement;
struct RenderQueues;
struct RendererFogDX9;
struct FogConfiguration;
struct ServiceOptions;
class CRenderSystemBase;

// Borrowed fields of the original renderer plus its shared effect-retirement
// owner. This view neither owns the device nor replaces the renderer vtable.
struct RendererFrameDX9 {
  DeviceOwnerDX9& device;
  RendererDX9Operations& operations;
  RenderCommandPool& commands;
  RenderPrimitivePool& primitives;
  LightmappedPrimitivePool& lightmapped;
  RenderVertexPool& vertices;
  RenderQueues& queues;
  EffectRetirement& retirement;

  void Clear();
  bool Draw(
    CRenderSystemBase* renderSystem,
    const ServiceOptions& options,
    unsigned char multitexture,
    unsigned char twoPass,
    RendererFogDX9& fog,
    const float& aspect,
    const FogConfiguration& configuration
  );
};

#endif
