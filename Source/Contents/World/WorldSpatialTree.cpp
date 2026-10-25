#include "Contents/Ground/GroundSpatial.h"
#include "Contents/Models/ModelActor.h"
#include "Contents/World/WorldSpatialTree.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/World/TerrainResource.h"

// OFFSET: 0x00AC66F0
// FUNCTION: fn_17652
WorldSpatialTree::WorldSpatialTree() : parent(nullptr), children{}, minimum{}, maximum{}, center{}, halfSize{}, dirty(1) {
}

// OFFSET: 0x00AC71E0
// FUNCTION: fn_17655
WorldSpatialTree::~WorldSpatialTree() {
  this->ClearChildren();
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

    for (WorldSpatialTree* child : this->children) {
      child->Subdivide(depth + 1);
    }
  }
}

// OFFSET: 0x00AC8F60
// FUNCTION: fn_17681
void WorldSpatialTree::ReadBounds(ResourceByteReader& input, int depth) {
  for (float& value : this->maximum) {
    input.Read(&value, sizeof(value));
  }

  for (float& value : this->minimum) {
    input.Read(&value, sizeof(value));
  }

  for (float& value : this->halfSize) {
    input.Read(&value, sizeof(value));
  }

  for (float& value : this->center) {
    input.Read(&value, sizeof(value));
  }

  if (depth != 5) {
    for (WorldSpatialTree* child : this->children) {
      child->ReadBounds(input, depth + 1);
    }
  }
}

// OFFSET: 0x00AC8590
// FUNCTION: fn_17674
void WorldSpatialTree::BindTerrain(C3dAttr& resource, int depth) {
  this->dirty = 1;
  this->terrain = &resource;
  resource.Grid().SpatialCell(this->minimum[0], this->minimum[2], this->terrainCells[0], this->terrainCells[1]);
  this->terrain->Grid().SpatialCell(this->maximum[0], this->maximum[2], this->terrainCells[2], this->terrainCells[3]);

  if (depth != 5) {
    for (WorldSpatialTree* child : this->children) {
      child->BindTerrain(resource, depth + 1);
    }
  }
}

// OFFSET: 0x00AC8620
// FUNCTION: fn_17675
void WorldSpatialTree::BindGround(GroundSpatial& resource, int depth) {
  this->dirty = 1;
  this->ground = &resource;
  resource.SpatialCell(this->minimum[0], this->minimum[2], this->groundCells[0], this->groundCells[1]);
  this->ground->SpatialCell(this->maximum[0], this->maximum[2], this->groundCells[2], this->groundCells[3]);

  if (depth != 5) {
    for (WorldSpatialTree* child : this->children) {
      child->BindGround(resource, depth + 1);
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

  if (depth != 5) {
    for (WorldSpatialTree* child : this->children) {
      if (child->dirty) {
        child->RefreshBounds(depth + 1);
      }
    }

    return;
  }

  float groundMinimum;
  float groundMaximum;
  float terrainMinimum;
  float terrainMaximum;
  this->ground->HeightBounds(this->groundCells, groundMinimum, groundMaximum);
  this->terrain->Grid().HeightBounds(this->terrainCells, terrainMinimum, terrainMaximum);
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

  do {
    if (node->minimum[1] > this->minimum[1]) {
      node->minimum[1] = this->minimum[1];
    }

    if (this->maximum[1] > node->maximum[1]) {
      node->maximum[1] = this->maximum[1];
    }

    node->halfSize[0] = node->maximum[0] - node->center[0];
    const float halfZ = node->maximum[2] - node->center[2];
    const float middleY = (node->maximum[1] + node->minimum[1]) * 0.5f;
    node->center[1] = middleY;
    node->halfSize[2] = halfZ;
    node->halfSize[1] = node->maximum[1] - middleY;
    node = node->parent;
  } while (node);
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

  for (WorldSpatialTree* child : this->children) {
    if (model.oriented.Intersects(child->minimum)) {
      child->InsertModel(model, depth + 1);
    }
  }
}

// OFFSET: 0x00ACA5D0
// FUNCTION: fn_17694
void WorldSpatialTree::RemoveModel(C3dActor* model, int depth) {
  if (depth != 5) {
    for (WorldSpatialTree* child : this->children) {
      if (child) {
        child->RemoveModel(model, depth + 1);
      }
    }

    return;
  }

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
}

// OFFSET: 0x00AC8100
// FUNCTION: fn_17665
std::vector<C3dActor*>& WorldSpatialTree::ModelsAt(float x, float z, int depth) {
  WorldSpatialTree* node = this;

  while (depth != 5) {
    const unsigned sideX = node->center[0] > x ? 0u : 1u;
    const unsigned sideZ = node->center[2] > z ? 0u : 2u;
    node = node->children[sideX + sideZ];
    ++depth;
  }

  return node->objects;
}
