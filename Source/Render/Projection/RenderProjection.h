#ifndef RAGNAROK_SOURCE_RENDER_PROJECTION_RENDERPROJECTION_H
#define RAGNAROK_SOURCE_RENDER_PROJECTION_RENDERPROJECTION_H

struct RenderVertex;

struct RenderProjection {
  const float& scaleX;
  const float& scaleY;
  const int& centerX;
  const int& centerY;

  void Project(const float* position, const float* matrix, RenderVertex& output) const;
  void Ray(int x, int y, const float* origin, const float* matrix, float* output) const;
};

#endif
