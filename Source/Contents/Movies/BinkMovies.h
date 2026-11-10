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
  NamedBinkMovie();
  ~NamedBinkMovie();
  std::string Name() const;
  bool Open(std::string fileName, int loop);
  void AddPosition(MoviePosition position);
  /* 0000 */ std::string name;
  /* 0018 */ BinkMovie* decoder;
  /* 001C */ float radius;
  /* 0020 */ std::list<MoviePosition> positions;
};

class BinkMovies {
public:
  BinkMovies();
  ~BinkMovies();
  NamedBinkMovie* Get(std::string name, int loop);
  void Update();

private:
  /* 0000 */ std::list<NamedBinkMovie*> m_movies;
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
