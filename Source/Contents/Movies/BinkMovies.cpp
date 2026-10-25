#include "Contents/Movies/BinkMovies.h"
#include "Render/Textures/TextureCache.h"

#include "Utility/Math/ScalarConversion.h"
#include <algorithm>
#include <cmath>

// OFFSET: 0x007B00F0
// FUNCTION: fn_09165
NamedBinkMovie::NamedBinkMovie(BinkMovieContext& context) : decoder(std::make_unique<BinkMovie>(context)), radius(150.0f) {
}

// OFFSET: 0x007AFF80
// FUNCTION: fn_09163
NamedBinkMovie::~NamedBinkMovie() = default;

// OFFSET: 0x007AFB50
// FUNCTION: fn_09161
BinkMovies::BinkMovies(BinkMovieContext& context) : m_context(&context) {
}

// OFFSET: 0x007AFED0
// FUNCTION: fn_09162
BinkMovies::~BinkMovies() = default;

// OFFSET: 0x007B00F0
// FUNCTION: fn_09165
NamedBinkMovie* BinkMovies::Get(std::string name, int loop) {
  for (const auto& movie : this->m_movies) {
    if (movie->name == name) {
      return movie.get();
    }
  }

  auto movie = std::make_unique<NamedBinkMovie>(*this->m_context);

  if (name.empty() || !movie->decoder->Open((std::string("video\\") + name).c_str(), loop, true)) {
    return nullptr;
  }

  movie->name = name;
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
void BinkMovies::Update(TextureCache& textures, const float* playerPosition, int volume) {
  for (const auto& movie : this->m_movies) {
    if (!movie || !movie->decoder || movie->name.empty()) {
      continue;
    }

    if (movie->decoder->Update()) {
      CTexture* texture = textures.Load(movie->name.c_str(), 0, false, true, false);

      if (texture) {
        movie->decoder->CopyTo(texture);
      }
    }

    if (!movie->positions.empty() && playerPosition) {
      float nearest = 250.0f;

      for (const MoviePosition& position : movie->positions) {
        const float x = position.x - playerPosition[0];
        const float y = position.y - playerPosition[1];
        const float z = position.z - playerPosition[2];
        const float distance = std::sqrt((y * y + x * x) + z * z);
        nearest = distance < nearest ? distance : nearest;
      }

      int attenuation = 0;

      if (movie->radius > nearest) {
        const float scaled = (static_cast<float>(volume) / movie->radius) * (movie->radius - nearest);
        const unsigned level = static_cast<unsigned>(TruncateToInt(scaled));
        attenuation = static_cast<int>(level * 256u);
      }

      this->m_context->runtime.setVolume(movie->decoder->Handle(), 0, attenuation);
    }
  }
}
