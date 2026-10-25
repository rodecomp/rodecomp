#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_CLIENTTEXTURESOURCES_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_CLIENTTEXTURESOURCES_H

#include "Render/Textures/TextureCache.h"

#include <ddraw.h>

struct TextureNames;
class BinkMovies;

class ClientTextureSources : public TextureSources {
public:
  ClientTextureSources(TextureNames& names, const int& modeType, BinkMovies* const& movies, CRenderSystemBase*& system, IDirectDraw7* const& directDraw);
  const char* Resolve(const char* name) override;
  bool MovieSize(const char* name, int& width, int& height) override;
  void CleanupTextures() override;

private:
  TextureNames& m_names;
  const int& m_modeType;
  BinkMovies* const& m_movies;
  CRenderSystemBase*& m_system;
  IDirectDraw7* const& m_directDraw;
};

#endif
