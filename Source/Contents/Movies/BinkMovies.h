#ifndef RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIES_H
#define RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIES_H

#include "Contents/Movies/BinkMovie.h"

#include <cstddef>
#include <list>
#include <memory>
#include <string>

class TextureCache;

struct MoviePosition {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
};

struct NamedBinkMovie {
  explicit NamedBinkMovie(BinkMovieContext& context);
  ~NamedBinkMovie();
  /* 0000 */ std::string name;
  /* 0018 */ std::unique_ptr<BinkMovie> decoder;
  /* 001C */ float radius;
  /* 0020 */ std::list<MoviePosition> positions;
};

class BinkMovies {
public:
  explicit BinkMovies(BinkMovieContext& context);
  ~BinkMovies();
  NamedBinkMovie* Get(std::string name, int loop);
  void AddPosition(NamedBinkMovie& movie, float x, float y, float z);
  void Update(TextureCache& textures, const float* playerPosition, int volume);

private:
  /* 0000 */ std::list<std::unique_ptr<NamedBinkMovie>> m_movies;
  BinkMovieContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MoviePosition) == 0x000C);
static_assert(offsetof(MoviePosition, x) == 0x0000);
static_assert(offsetof(MoviePosition, y) == 0x0004);
static_assert(offsetof(MoviePosition, z) == 0x0008);
static_assert(sizeof(NamedBinkMovie) == 0x0028);
static_assert(offsetof(NamedBinkMovie, name) == 0x0000);
static_assert(offsetof(NamedBinkMovie, decoder) == 0x0018);
static_assert(offsetof(NamedBinkMovie, radius) == 0x001C);
static_assert(offsetof(NamedBinkMovie, positions) == 0x0020);
#endif

#endif
