#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Models/ModelActor.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/WorldRayQuery.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Render/Renderer.h"

#include <cstring>

// OFFSET: 0x00CD5AD0
// FUNCTION: fn_21286
void CGameMode::MarkWorldOcclusion() {
  if (!this->scene || !this->scene->player || !g_renderer) {
    return;
  }

  const int screenX = this->scene->player->render.screenX;
  const int screenY = static_cast<int>(static_cast<float>(this->scene->player->render.screenY) - g_renderer->viewport.uiScaleY * 23.0f);
  WorldRayQuery query;
  float ray[6]{};
  g_renderer->viewport.Ray(screenX, screenY, this->camera->frame.eye, this->camera->frame.inverse.values, ray);

  float actorRay[7];
  actorRay[0] = ray[0];
  actorRay[1] = ray[1];
  actorRay[2] = ray[2];
  actorRay[3] = ray[3];
  actorRay[4] = ray[4];
  actorRay[5] = ray[5];
  actorRay[6] = (this->scene->player->render.position.y + this->camera->camera.distance) - 16.0f;
  query.Collect(ray, &this->scene->spatial);

  for (auto node = query.nodes.begin(); node != query.nodes.end(); ++node) {
    for (auto model = (*node)->objects.begin(); model != (*node)->objects.end(); ++model) {
      (*model)->MarkOccluded(actorRay);
    }
  }

  if (this->checkGroundAlpha) {
    for (auto it = query.nodes.begin(); it != query.nodes.end(); ++it) {
      WorldSpatialTree* node = *it;
      int row = node->groundCells[1];

      while (row <= node->groundCells[3]) {
        int column = node->groundCells[0];

        while (column <= node->groundCells[2]) {
          node->ground->MarkRay(ray, column, row, actorRay[6]);
          ++column;
        }

        ++row;
      }
    }
  }
}
