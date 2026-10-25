#ifndef RAGNAROK_SOURCE_RENDER_FOGCONFIGURATION_H
#define RAGNAROK_SOURCE_RENDER_FOGCONFIGURATION_H

struct FogConfiguration {
  unsigned color;
  unsigned vertexMode;
  float density;
  float start;
  float end;
  unsigned range;
};

#endif
