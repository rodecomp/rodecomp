#ifndef RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIES_H
#define RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIES_H

#include "Contents/Movies/BinkMovie.h"

#include <list>
#include <memory>
#include <string>

class TextureCache;

struct MoviePosition {
  float x;
  float y;
  float z;
};

struct NamedBinkMovie {
  explicit NamedBinkMovie(BinkMovieContext& context);
  ~NamedBinkMovie();
  std::string name;
  std::unique_ptr<BinkMovie> decoder;
  float radius;
  std::list<MoviePosition> positions;
};

class BinkMovies {
public:
  explicit BinkMovies(BinkMovieContext& context);
  ~BinkMovies();
  NamedBinkMovie* Get(std::string name, int loop);
  void AddPosition(NamedBinkMovie& movie, float x, float y, float z);
  void Update(TextureCache& textures, const float* playerPosition, int volume);

private:
  std::list<std::unique_ptr<NamedBinkMovie>> m_movies;
  BinkMovieContext* m_context;
};

#endif
