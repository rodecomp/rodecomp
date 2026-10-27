#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_CLIENTTEXTURESOURCES_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_CLIENTTEXTURESOURCES_H

#include "Contents/Movies/BinkMovies.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/TextureNames.h"

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

// INLINED
inline ClientTextureSources::ClientTextureSources(
  TextureNames& names, const int& modeType, BinkMovies* const& movies, CRenderSystemBase*& system, IDirectDraw7* const& directDraw
)
    : m_names(names), m_modeType(modeType), m_movies(movies), m_system(system), m_directDraw(directDraw) {
}

// INLINED
inline const char* ClientTextureSources::Resolve(const char* name) {
  return this->m_names.Resolve(name);
}

// INLINED
inline bool ClientTextureSources::MovieSize(const char* name, int& width, int& height) {
  if (this->m_modeType != 1 || !this->m_movies) {
    return false;
  }

  NamedBinkMovie* movie = this->m_movies->Get(name, 1);

  if (!movie) {
    return false;
  }

  width = movie->decoder->Handle()->width;
  height = movie->decoder->Handle()->height;
  return true;
}

// INLINED
inline void ClientTextureSources::CleanupTextures() {
  if (this->m_system && this->m_system->Backend() == 1 && this->m_directDraw) {
    this->m_directDraw->Compact();
  }
}

#endif
