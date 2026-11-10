#include "Client/Session.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/Movies/BinkMovies.h"
#include "Contents/World/World.h"
#include "Contents/World/WorldSpatialTree.h"
#include "File/File.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00AC64F0
// FUNCTION: fn_17651
CWorld::CWorld() {
  this->partyRestricted = false;
  this->serializedSpatial = nullptr;
  this->pending = 0;
  this->ground = nullptr;
  this->player = nullptr;
  this->isPKZone = 0;
  this->isSiegeMode = 0;
  this->isBattleFieldMode = 0;
  g_session.m_score1 = -1;
  g_session.m_score2 = -1;
  this->isEventPVPMode = 0;
  this->guildMasterNearby = false;
  this->nearbyRange = 2;
  this->backgroundFog = false;
}

// OFFSET: 0x00AC6FB0
// FUNCTION: fn_17654
CWorld::~CWorld() {
  delete this->ground;

  if (this->movies) {
    delete this->movies;
    this->movies = nullptr;
  }
}

// OFFSET: 0x00AC66F0
// FUNCTION: fn_17652
WorldSpatialTree::WorldSpatialTree() : minimum{0.0f, 0.0f, 0.0f}, maximum{0.0f, 0.0f, 0.0f}, center{0.0f, 0.0f, 0.0f}, halfSize{0.0f, 0.0f, 0.0f} {
  this->parent = nullptr;
  for (unsigned index = 0; index < 4; ++index) {
    this->children[index] = nullptr;
  }
  this->dirty = 1;
}

// OFFSET: 0x00AC71E0
// FUNCTION: fn_17655
WorldSpatialTree::~WorldSpatialTree() {
  for (int index = 0; index < 4; ++index) {
    if (this->children[index]) {
      delete this->children[index];
      this->children[index] = nullptr;
    }
  }
}

// OFFSET: 0x00AC80C0
// FUNCTION: fn_17664
void WorldSpatialTree::ClearChildren() {
  for (WorldSpatialTree*& child : this->children) {
    delete child;
    child = nullptr;
  }
}

// OFFSET: 0x00AC77D0
// FUNCTION: fn_17659
void WorldSpatialTree::Subdivide(int depth) {
  const float sumX = this->minimum[0] + this->maximum[0];
  this->center[1] = 0;
  this->minimum[1] = -999999.0f;
  this->maximum[1] = 999999.0f;
  this->center[0] = sumX * 0.5f;
  this->center[2] = (this->minimum[2] + this->maximum[2]) * 0.5f;

  if (depth != 5) {
    this->children[0] = new WorldSpatialTree;
    this->children[0]->parent = this;
    this->children[0]->minimum[0] = this->minimum[0];
    this->children[0]->maximum[0] = this->center[0];
    this->children[0]->minimum[2] = this->minimum[2];
    this->children[0]->maximum[2] = this->center[2];

    this->children[1] = new WorldSpatialTree;
    this->children[1]->parent = this;
    this->children[1]->minimum[0] = this->center[0];
    this->children[1]->maximum[0] = this->maximum[0];
    this->children[1]->minimum[2] = this->minimum[2];
    this->children[1]->maximum[2] = this->center[2];

    this->children[2] = new WorldSpatialTree;
    this->children[2]->parent = this;
    this->children[2]->minimum[0] = this->minimum[0];
    this->children[2]->maximum[0] = this->center[0];
    this->children[2]->minimum[2] = this->center[2];
    this->children[2]->maximum[2] = this->maximum[2];

    this->children[3] = new WorldSpatialTree;
    this->children[3]->parent = this;
    this->children[3]->minimum[0] = this->center[0];
    this->children[3]->maximum[0] = this->maximum[0];
    this->children[3]->minimum[2] = this->center[2];
    this->children[3]->maximum[2] = this->maximum[2];

    for (int index = 0; index < 4; ++index) {
      this->children[index]->Subdivide(depth + 1);
    }
  }
}

// OFFSET: 0x00AC8F60
// FUNCTION: fn_17681
void WorldSpatialTree::ReadBounds(CFile& input, int depth) {
  input.Read(&this->maximum[0], sizeof(float));
  input.Read(&this->maximum[1], sizeof(float));
  input.Read(&this->maximum[2], sizeof(float));
  input.Read(&this->minimum[0], sizeof(float));
  input.Read(&this->minimum[1], sizeof(float));
  input.Read(&this->minimum[2], sizeof(float));
  input.Read(&this->halfSize[0], sizeof(float));
  input.Read(&this->halfSize[1], sizeof(float));
  input.Read(&this->halfSize[2], sizeof(float));
  input.Read(&this->center[0], sizeof(float));
  input.Read(&this->center[1], sizeof(float));
  input.Read(&this->center[2], sizeof(float));

  if (depth != 5) {
    for (unsigned index = 0; index < 4; ++index) {
      this->children[index]->ReadBounds(input, depth + 1);
    }
  }
}

// OFFSET: 0x00AC8590
// FUNCTION: fn_17674
void WorldSpatialTree::BindTerrain(C3dAttr& resource, int depth) {
  this->dirty = 1;
  this->terrain = &resource;
  resource.SpatialCell(this->minimum[0], this->minimum[2], this->terrainCells[0], this->terrainCells[1]);
  this->terrain->SpatialCell(this->maximum[0], this->maximum[2], this->terrainCells[2], this->terrainCells[3]);

  if (depth != 5) {
    for (unsigned index = 0; index < 4; ++index) {
      this->children[index]->BindTerrain(resource, depth + 1);
    }
  }
}

// OFFSET: 0x00AC8620
// FUNCTION: fn_17675
void WorldSpatialTree::BindGround(GroundGeometry& resource, int depth) {
  this->dirty = 1;
  this->ground = &resource;
  resource.SpatialCell(this->minimum[0], this->minimum[2], this->groundCells[0], this->groundCells[1]);
  this->ground->SpatialCell(this->maximum[0], this->maximum[2], this->groundCells[2], this->groundCells[3]);

  if (depth != 5) {
    for (unsigned index = 0; index < 4; ++index) {
      this->children[index]->BindGround(resource, depth + 1);
    }
  }
}

// OFFSET: 0x00AC7EE0
// FUNCTION: fn_17662
void WorldSpatialTree::CopyBounds(int depth, const WorldSpatialTree& source) {
  this->dirty = 0;

  for (unsigned index = 0; index < 3; ++index) {
    this->maximum[index] = source.maximum[index];
  }

  for (unsigned index = 0; index < 3; ++index) {
    this->minimum[index] = source.minimum[index];
  }

  for (unsigned index = 0; index < 3; ++index) {
    this->halfSize[index] = source.halfSize[index];
  }

  for (unsigned index = 0; index < 3; ++index) {
    this->center[index] = source.center[index];
  }

  if (depth != 5) {
    for (unsigned index = 0; index < 4; ++index) {
      this->children[index]->CopyBounds(depth + 1, *source.children[index]);
    }
  }
}

// OFFSET: 0x00ACB400
// FUNCTION: fn_17703
void WorldSpatialTree::RefreshBounds(int depth) {
  this->dirty = 0;

  if (depth == 5) {
    float groundMinimum;
    float groundMaximum;
    float terrainMinimum;
    float terrainMaximum;
    this->ground->HeightBounds(this->groundCells, groundMinimum, groundMaximum);
    this->terrain->HeightBounds(this->terrainCells, terrainMinimum, terrainMaximum);
    this->maximum[1] = terrainMaximum > groundMaximum ? terrainMaximum : groundMaximum;
    this->minimum[1] = terrainMinimum < groundMinimum ? terrainMinimum : groundMinimum;

    auto object = this->objects.begin();

    while (object != this->objects.end()) {
      if (*object) {
        const float low = (*object)->minimum[1];

        if (this->minimum[1] > low) {
          this->minimum[1] = low;
        }

        const float high = (*object)->maximum[1];

        if (high > this->maximum[1]) {
          this->maximum[1] = high;
        }

        ++object;
      }
    }

    WorldSpatialTree* node = this;
    const float averageHalf = g_depthAverageHalf;
    do {
      if (node->minimum[1] > this->minimum[1]) {
        node->minimum[1] = this->minimum[1];
      }

      if (node->maximum[1] < this->maximum[1]) {
        node->maximum[1] = this->maximum[1];
      }

      node->halfSize[0] = node->maximum[0] - node->center[0];
      const float halfZ = node->maximum[2] - node->center[2];
      const float middleY = (node->maximum[1] + node->minimum[1]) * averageHalf;
      node->center[1] = middleY;
      node->halfSize[2] = halfZ;
      node->halfSize[1] = node->maximum[1] - middleY;
      node = node->parent;
    } while (node);
  } else {
    for (int index = 0; index < 4; ++index) {
      if (this->children[index]->dirty) {
        this->children[index]->RefreshBounds(depth + 1);
      }
    }
  }
}

// OFFSET: 0x00AC84F0
// FUNCTION: fn_17673
// OFFSET: 0x00AC86B0
// FUNCTION: fn_17676
void WorldSpatialTree::InsertModel(C3dActor& model, int depth) {
  if (depth == 5) {
    this->objects.push_back(&model);

    for (WorldSpatialTree* node = this; node; node = node->parent) {
      node->dirty = 1;
    }

    return;
  }

  for (int index = 0; index < 4; ++index) {
    if (model.oriented.Intersects(this->children[index]->minimum)) {
      this->children[index]->InsertModel(model, depth + 1);
    }
  }
}

// OFFSET: 0x00ACA5D0
// FUNCTION: fn_17694
void WorldSpatialTree::RemoveModel(C3dActor* model, int depth) {
  if (depth == 5) {
    bool removed = false;
    auto current = this->objects.begin();

    while (current != this->objects.end()) {
      if (*current == model) {
        current = this->objects.erase(current);
        removed = true;
      } else {
        ++current;
      }
    }

    if (removed) {
      for (WorldSpatialTree* node = this; node; node = node->parent) {
        node->dirty = 1;
      }
    }
  } else {
    for (int index = 0; index < 4; ++index) {
      if (this->children[index]) {
        this->children[index]->RemoveModel(model, depth + 1);
      }
    }
  }
}

// OFFSET: 0x00AC8100
// FUNCTION: fn_17665
std::vector<C3dActor*>& WorldSpatialTree::ModelsAt(float x, float z, int depth) {
  if (depth == 5) {
    return this->objects;
  }

  WorldSpatialTree* child;

  if (this->center[0] > x) {
    if (this->center[2] > z) {
      child = this->children[0];
    } else {
      child = this->children[2];
    }
  } else if (this->center[2] > z) {
    child = this->children[1];
  } else {
    child = this->children[3];
  }

  return child->ModelsAt(x, z, depth + 1);
}

// OFFSET: 0x00ACB560
// FUNCTION: fn_17704
void WorldSpatialTree::RefreshModelBounds(int depth) {
  this->dirty = 0;

  if (depth == 5) {
    float groundMinimum;
    float groundMaximum;
    float terrainMinimum;
    float terrainMaximum;
    this->ground->HeightBounds(this->groundCells, groundMinimum, groundMaximum);
    this->terrain->HeightBounds(this->terrainCells, terrainMinimum, terrainMaximum);
    this->maximum[1] = terrainMaximum > groundMaximum ? terrainMaximum : groundMaximum;
    this->minimum[1] = terrainMinimum < groundMinimum ? terrainMinimum : groundMinimum;

    for (auto object = this->objects.begin(); object != this->objects.end(); ++object) {
      if (this->minimum[1] > (*object)->minimum[1]) {
        this->minimum[1] = (*object)->minimum[1];
      }

      if ((*object)->maximum[1] > this->maximum[1]) {
        this->maximum[1] = (*object)->maximum[1];
      }
    }

    WorldSpatialTree* node = this;
    const float averageHalf = g_depthAverageHalf;
    do {
      if (node->minimum[1] > this->minimum[1]) {
        node->minimum[1] = this->minimum[1];
      }

      if (this->maximum[1] > node->maximum[1]) {
        node->maximum[1] = this->maximum[1];
      }

      node->halfSize[0] = node->maximum[0] - node->center[0];
      const float halfZ = node->maximum[2] - node->center[2];
      const float middleY = (node->maximum[1] + node->minimum[1]) * averageHalf;
      node->center[1] = middleY;
      node->halfSize[2] = halfZ;
      node->halfSize[1] = node->maximum[1] - middleY;
      node = node->parent;
    } while (node);
  } else {
    for (int index = 0; index < 4; ++index) {
      if (this->children[index]->dirty) {
        this->children[index]->RefreshModelBounds(depth + 1);
      }
    }
  }
}
