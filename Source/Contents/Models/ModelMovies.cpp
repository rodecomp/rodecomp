#include "Contents/Movies/BinkMovies.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Resource/Models/ModelResource.h"

// OFFSET: 0x0075CDF0
// FUNCTION: fn_08028
void C3dModelRes::RegisterMovies(MoviePosition position) const {
  for (auto name = this->movieNames.begin(); name != this->movieNames.end(); ++name) {
    if (NamedBinkMovie* movie = g_modeMgr.GetGameMode()->scene->movies->Get(*name, 1)) {
      movie->AddPosition(position);
    }
  }
}
