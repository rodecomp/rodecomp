#include "Contents/Actors/ActorPickNode.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Movement/TerrainPathSearch.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/SkyBox.h"
#include "Contents/World/World.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Resource/World/TerrainResource.h"

#include <map>
#include <string>

// OFFSET: 0x00AD9530
// FUNCTION: fn_17878
std::uint32_t MovementRouteInterpolation::Duration() const {
  return this->points[this->points.size() - 1].tick - this->points[0].tick;
}

// OFFSET: 0x00AD95F0
// FUNCTION: fn_17880
bool MovementRouteInterpolation::Empty() const {
  return this->points.empty();
}

// OFFSET: 0x00AD9600
// FUNCTION: fn_17881
bool CPathFinder::Walkable(int column, int row) const {
  return this->attr->Walkable(column, row);
}

// OFFSET: 0x00ADA4F0
// FUNCTION: fn_45894
CActorPickNode::CActorPickNode() {
  for (int index = 0; index < 4; ++index) {
    this->children[index] = nullptr;
  }
}

// OFFSET: 0x00ADA690
// FUNCTION: fn_17906
__declspec(noinline) CActorPickNode::~CActorPickNode() {
  this->Clear();
}

// OFFSET: 0x00AD9980
// FUNCTION: fn_17889
bool CPathFinder::LoadAttr(std::string name) {
  C3dAttr* attr;
  const auto found = this->attrCache.find(name);
  if (found != this->attrCache.end()) {
    attr = found->second;
  } else {
    attr = new C3dAttr;
    if (attr == nullptr) {
      return false;
    }

    this->attrCache[name] = attr;
    name += ".gat";
    attr->Load(name.c_str());
  }
  this->attr = attr;
  return true;
}

// OFFSET: 0x00ADA750
// FUNCTION: fn_17908
void CActorPickNode::AddPickInfo(ActorPickInfo* info) {
  CActorPickNode* node = this;
  while (node->children[0] != nullptr) {
    const int x = static_cast<int>(info->corners[0].x);
    const int y = static_cast<int>(info->corners[0].y);
    const int centerX = node->bounds.right / 2;
    const int centerY = node->bounds.bottom / 2;
    if (x <= centerX) {
      node = node->children[y <= centerY ? 0 : 3];
    } else {
      node = node->children[(y > centerY) + 1];
    }
  }

  node->pickInfos.push_back(info);
}

// OFFSET: 0x00ADA850
// FUNCTION: fn_17909
void CActorPickNode::Clear() {
  for (auto info = this->pickInfos.begin(); info != this->pickInfos.end(); ++info) {
    if (*info) {
      delete *info;
    }
  }
  this->pickInfos.erase(this->pickInfos.begin(), this->pickInfos.end());

  for (int index = 0; index < 4; ++index) {
    if (this->children[index]) {
      delete this->children[index];
      this->children[index] = nullptr;
    }
  }
}

// OFFSET: 0x00ADA8F0
// FUNCTION: fn_17910
ActorPickInfo* CActorPickNode::Pick(vector2d point) {
  if (this->children[0] == nullptr) {
    ActorPickInfo* result = nullptr;
    float bestDepth = 0.0f;
    for (ActorPickInfo* info : this->pickInfos) {
      if (info->Contains(point.x, point.y)) {
        const float depth = info->corners[1].z + info->corners[0].z;
        if (depth > bestDepth) {
          bestDepth = depth;
          result = info;
        }
      }
    }
    return result;
  }

  const int quadrant = this->QuadrantAt(static_cast<int>(point.x), static_cast<int>(point.y));

  ActorPickInfo* result = this->children[quadrant]->Pick(point);
  if (result) {
    return result;
  }
  for (int index = 0; index < 4; ++index) {
    if (quadrant != index) {
      result = this->children[index]->Pick(point);
      if (result) {
        return result;
      }
    }
  }
  return nullptr;
}

// OFFSET: 0x00ADA9E0
// FUNCTION: fn_45895
int CActorPickNode::QuadrantAt(int x, int y) const {
  const int centerX = this->bounds.right / 2;
  const int centerY = this->bounds.bottom / 2;
  if (x <= centerX && y <= centerY) {
    return ACTOR_PICK_TOP_LEFT;
  }
  if (x > centerX && y <= centerY) {
    return ACTOR_PICK_TOP_RIGHT;
  }
  if (x > centerX && y > centerY) {
    return ACTOR_PICK_BOTTOM_RIGHT;
  }
  return ACTOR_PICK_BOTTOM_LEFT;
}

// OFFSET: 0x00ADAA20
// FUNCTION: fn_45896
bool ActorPickInfo::Contains(float x, float y) const {
  return x > this->corners[0].x && x < this->corners[1].x && y > this->corners[0].y && y < this->corners[1].y;
}

// OFFSET: 0x00ADAA60
// FUNCTION: fn_17911
void CActorPickNode::MakeQuardTree(int depth, ActorPickBounds bounds) {
  this->Clear();
  this->bounds = bounds;
  if (depth != 0) {
    for (int index = 0; index < 4; ++index) {
      CActorPickNode* child = new CActorPickNode;
      this->children[index] = child;
      child->MakeQuardTree(depth - 1, this->ChildBounds(this->bounds, index));
    }
  }
}

// OFFSET: 0x00ADABD0
// FUNCTION: fn_45897
ActorPickBounds CActorPickNode::ChildBounds(ActorPickBounds bounds, int quadrant) const {
  switch (quadrant) {
  case ACTOR_PICK_TOP_LEFT:
    bounds.right /= 2;
    bounds.bottom /= 2;
    break;
  case ACTOR_PICK_TOP_RIGHT:
    bounds.left = bounds.right / 2 + 1;
    bounds.bottom /= 2;
    break;
  case ACTOR_PICK_BOTTOM_RIGHT:
    bounds.left = bounds.right / 2 + 1;
    bounds.top = bounds.bottom / 2 + 1;
    break;
  case ACTOR_PICK_BOTTOM_LEFT:
    bounds.right /= 2;
    bounds.top = bounds.bottom / 2 + 1;
    break;
  }
  return bounds;
}

// GLOBAL: gl_24536
float g_projectionScale = 0.16f;

// GLOBAL: gl_24775
int g_worldViewDrawn;

// OFFSET: 0x00ADAEA0
// FUNCTION: fn_45884
__declspec(noinline) CView::~CView() {
  if (this->skyBox) {
    delete this->skyBox;
  }
}

// OFFSET: 0x00ADC150
// FUNCTION: fn_17928
void CView::Reset() {
  this->camera.Reset();
  this->frame.up.x = 0;
  this->frame.up.y = 1;
  this->frame.up.z = 0;
}

// OFFSET: 0x00ADC1D0
// FUNCTION: fn_17929
void CView::OnExitFrame() {
}
