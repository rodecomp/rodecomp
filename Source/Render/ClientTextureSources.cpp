#include "Contents/BinkMovies.h"
#include "Render/ClientTextureSources.h"
#include "Render/RenderSystemBase.h"
#include "Resource/TextureNames.h"

ClientTextureSources::ClientTextureSources(
  TextureNames& names,
  const int& modeType,
  BinkMovies* const& movies,
  CRenderSystemBase*& system,
  IDirectDraw7* const& directDraw
)
    : m_names(names), m_modeType(modeType), m_movies(movies), m_system(system), m_directDraw(directDraw) {
}

const char* ClientTextureSources::Resolve(const char* name) {
  return this->m_names.Resolve(name);
}

bool ClientTextureSources::MovieSize(const char* name, int& width, int& height) {
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

void ClientTextureSources::CleanupTextures() {
  // DX7 slot compacts IDirectDraw; DX9 slot is empty.
  if (this->m_system && this->m_system->Backend() == 1 && this->m_directDraw) {
    this->m_directDraw->Compact();
  }
}
