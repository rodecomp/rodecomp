#include "Contents/Movies/BinkMovies.h"
#include "Render/Textures/TextureCache.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Sound/SoundMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include "Utility/Math/ScalarConversion.h"
#include <algorithm>
#include <cmath>
#include <new>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

// INLINED
NamedBinkMovie::NamedBinkMovie() : radius(150.0f) {
}

// INLINED
NamedBinkMovie::~NamedBinkMovie() {
  this->decoder.reset();
}

// INLINED
std::string NamedBinkMovie::Name() const {
  return this->name;
}

// INLINED
__forceinline bool NamedBinkMovie::Open(std::string fileName, int loop) {
  if (fileName.empty()) {
    return false;
  }

  this->decoder.reset();
  this->decoder.reset(new (std::nothrow) BinkMovie);
  if (!this->decoder) {
    return false;
  }

  if (this->decoder->Open(("video\\" + fileName).c_str(), loop, true) != true) {
    return false;
  }

  this->name = fileName;
  return true;
}

// OFFSET: 0x007AFB50
// FUNCTION: fn_09161
BinkMovies::BinkMovies() = default;

// OFFSET: 0x007AFED0
// FUNCTION: fn_09162
BinkMovies::~BinkMovies() = default;

// OFFSET: 0x007B00F0
// FUNCTION: fn_09165
NamedBinkMovie* BinkMovies::Get(std::string name, int loop) {
  auto found = std::find_if(this->m_movies.begin(), this->m_movies.end(), [name](const std::unique_ptr<NamedBinkMovie>& movie) {
    return name.compare(movie->Name()) == 0;
  });
  if (found != this->m_movies.end()) {
    return found->get();
  }

  std::unique_ptr<NamedBinkMovie> movie(new (std::nothrow) NamedBinkMovie);
  if (!movie || !movie->Open(name, loop)) {
    return nullptr;
  }

  NamedBinkMovie* result = movie.get();
  this->m_movies.push_back(std::move(movie));
  return result;
}

// OFFSET: 0x007B0040
// FUNCTION: fn_09164
void BinkMovies::AddPosition(NamedBinkMovie& movie, float x, float y, float z) {
  movie.positions.push_back({x, y, z});
}

// OFFSET: 0x007B04F0
// FUNCTION: fn_09166
void BinkMovies::Update() {
  for (const auto& entry : this->m_movies) {
    NamedBinkMovie* movie = entry.get();
    if (!movie || !movie->decoder || movie->name.empty()) {
      continue;
    }

    if (movie->decoder->Update() == true) {
      CTexture* texture = g_texMgr.Load(movie->name.c_str(), 0, false, true, false);

      if (texture) {
        movie->decoder->CopyTo(texture);
      }
    }

    if (movie->positions.empty()) {
      continue;
    }

    CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    if (player) {
      const float* playerPosition = &player->render.position.x;
      float nearest = 250.0f;

      for (std::list<MoviePosition>::const_iterator position = movie->positions.begin(); position != movie->positions.end(); ++position) {
        const float x = position->x - playerPosition[0];
        const float y = position->y - playerPosition[1];
        const float z = position->z - playerPosition[2];
        const float distance = SquareRoot((y * y + x * x) + z * z);
        nearest = distance < nearest ? distance : nearest;
      }

      const int attenuation = movie->radius > nearest
        ? static_cast<int>((static_cast<float>(g_soundMgr->sampleVolume) / movie->radius) * (movie->radius - nearest)) * 256
        : 0;
      BinkSetVolume(movie->decoder->Handle(), 0, attenuation);
    }
  }
}
