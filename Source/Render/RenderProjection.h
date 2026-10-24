#ifndef RAGNAROK_SOURCE_RENDER_RENDERPROJECTION_H
#define RAGNAROK_SOURCE_RENDER_RENDERPROJECTION_H

struct RenderVertex;

// Borrowed renderer projection fields +0x04/+0x08 and +0x30/+0x34.
// The camera transform is the original 12-float affine matrix.
struct RenderProjection {
  const float& scaleX;
  const float& scaleY;
  const int& centerX;
  const int& centerY;

  void Project(const float* position, const float* matrix, RenderVertex& output) const;
  void Ray(int x, int y, const float* origin, const float* matrix, float* output) const;
};

#endif
