#include "Contents/Movies/BinkMovies.h"
#include "Resource/Models/ModelResource.h"

// OFFSET: 0x0075CDF0
// FUNCTION: fn_08028
void C3dModelRes::RegisterMovies(BinkMovies& movies, const float* position) const {
  for (const std::string& name : this->movieNames) {
    if (NamedBinkMovie* movie = movies.Get(name, 1)) {
      movies.AddPosition(*movie, position[0], position[1], position[2]);
    }
  }
}
