#ifndef RAGNAROK_SOURCE_RENDER_FOGCONFIGURATION_H
#define RAGNAROK_SOURCE_RENDER_FOGCONFIGURATION_H

// Original shared renderer fog configuration globals, borrowed by both backends.
// Mode defaults are reset by Set, not guessed during application.
struct FogConfiguration {
  unsigned color;
  unsigned vertexMode;
  float density;
  float start;
  float end;
  unsigned range;
};

#endif
