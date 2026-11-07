#include "Client/Input/Mouse.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/World.h"
#include "Contents/World/WorldRayQuery.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UITextButton.h"
#include "Contents/World/WorldTerrainPicking.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00CD4060
// FUNCTION: fn_21274
bool CGameMode::PickScreenTerrain(int& column, int& row) {
  WorldRayQuery query;
  float ray[6]{};
  g_renderer->viewport.Ray(g_mouse.x, g_mouse.y, this->camera->frame.eye, this->camera->frame.inverse.values, ray);
  query.Collect(ray, &this->scene->spatial);
  float nearest = 999999.0f;
  float result = nearest;

  for (WorldSpatialTree* node : query.nodes) {
    C3dAttr* terrain = node->terrain;
    int candidateRow = node->terrainCells[1];

    while (candidateRow <= node->terrainCells[3]) {
      int candidateColumn = node->terrainCells[0];

      while (candidateColumn <= node->terrainCells[2]) {
        const float distance = terrain->RayDistance(ray, candidateColumn, candidateRow);

        if (nearest > distance) {
          nearest = distance;
          column = candidateColumn;
          row = candidateRow;
        }

        result = nearest;
        candidateColumn = candidateColumn + 1;
      }

      candidateRow = candidateRow + 1;
    }
  }

  return result < 999999.0f;
}
