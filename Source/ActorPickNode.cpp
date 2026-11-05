#include "ActorPickNode.h"
#include "Contents/Camera/Camera.h"
#include "Contents/TerrainPathSearch.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/World/SkyBox.h"
#include "Contents/World/World.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
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

// OFFSET: 0x00ADA690
// FUNCTION: fn_17906
CActorPickNode::~CActorPickNode() {
  this->Clear();
}

// OFFSET: 0x00AD9980
// FUNCTION: fn_17889
bool CPathFinder::LoadAttr(std::string name) {
  const auto found = this->attrCache.find(name);
  if (found != this->attrCache.end()) {
    this->attr = found->second;
    return true;
  }

  C3dAttr* attr = new C3dAttr;
  if (attr == nullptr) {
    return false;
  }

  this->attrCache[name] = attr;
  name += ".gat";
  attr->Load(name.c_str());
  this->attr = attr;
  return true;
}

// OFFSET: 0x00ADA750
// FUNCTION: fn_17908
void CActorPickNode::AddPickInfo(ActorPickInfo* info) {
  CActorPickNode* node = this;
  while (node->children[0] != nullptr) {
    const int x = static_cast<int>(info->left);
    const int y = static_cast<int>(info->top);
    const int centerX = node->right / 2;
    const int centerY = node->bottom / 2;
    int quadrant;
    if (x > centerX) {
      quadrant = (y > centerY) + 1;
    } else {
      quadrant = y <= centerY ? 0 : 3;
    }

    node = node->children[quadrant];
  }

  node->pickInfos.push_back(info);
}

// OFFSET: 0x00ADA850
// FUNCTION: fn_17909
void CActorPickNode::Clear() {
  for (auto info = this->pickInfos.begin(); info != this->pickInfos.end(); ++info) {
    delete *info;
  }
  this->pickInfos.erase(this->pickInfos.begin(), this->pickInfos.end());

  for (CActorPickNode*& child : this->children) {
    if (child) {
      delete child;
      child = nullptr;
    }
  }
}

// OFFSET: 0x00ADA8F0
// FUNCTION: fn_17910
ActorPickInfo* CActorPickNode::Pick(float x, float y) {
  if (this->children[0] == nullptr) {
    ActorPickInfo* result = nullptr;
    float bestDepth = 0.0f;
    for (ActorPickInfo* info : this->pickInfos) {
      if (x > info->left && x < info->right && y > info->top && y < info->bottom && info->depthOffset + info->depth > bestDepth) {
        result = info;
        bestDepth = info->depthOffset + info->depth;
      }
    }
    return result;
  }

  int quadrant;
  if (static_cast<int>(x) > this->right / 2) {
    quadrant = (static_cast<int>(y) > this->bottom / 2) + 1;
  } else {
    quadrant = static_cast<int>(y) <= this->bottom / 2 ? 0 : 3;
  }

  ActorPickInfo* result = this->children[quadrant]->Pick(x, y);
  if (result) {
    return result;
  }
  for (int index = 0; index < 4; ++index) {
    if (index != quadrant) {
      result = this->children[index]->Pick(x, y);
      if (result) {
        return result;
      }
    }
  }
  return nullptr;
}

// OFFSET: 0x00ADAA60
// FUNCTION: fn_17911
void CActorPickNode::MakeQuardTree(int depth, ActorPickBounds bounds) {
  this->Clear();
  this->left = bounds.left;
  this->top = bounds.top;
  this->right = bounds.right;
  this->bottom = bounds.bottom;
  if (depth != 0) {
    for (int index = 0; index < 4; ++index) {
      this->children[index] = new CActorPickNode;
      ActorPickBounds childBounds{this->left, this->top, this->right, this->bottom};
      switch (index) {
      case 0:
        childBounds.right /= 2;
        childBounds.bottom /= 2;
        break;
      case 1:
        childBounds.left = childBounds.right / 2 + 1;
        childBounds.bottom /= 2;
        break;
      case 2:
        childBounds.left = childBounds.right / 2 + 1;
        childBounds.top = childBounds.bottom / 2 + 1;
        break;
      case 3:
        childBounds.right /= 2;
        childBounds.top = childBounds.bottom / 2 + 1;
        break;
      }
      this->children[index]->MakeQuardTree(depth - 1, childBounds);
    }
  }
}

// GLOBAL: gl_24536
float g_projectionScale = 0.16f;

// GLOBAL: gl_24775
int g_worldViewDrawn;

// OFFSET: 0x00ADAC70
// FUNCTION: fn_17912
CView::CView() {
  this->camera.target[0] = 0.0f;
  this->camera.target[1] = 0.0f;
  this->camera.target[2] = 0.0f;
  this->camera.desiredTarget[0] = 0.0f;
  this->camera.desiredTarget[1] = 0.0f;
  this->camera.desiredTarget[2] = 0.0f;
  this->unknown_0050[0] = 0.0f;
  this->unknown_0050[1] = 0.0f;
  this->unknown_0050[2] = 0.0f;
  this->unknown_0068[0] = 0.0f;
  this->unknown_0068[1] = 0.0f;
  this->unknown_0068[2] = 0.0f;
  this->frame.eye[0] = 0.0f;
  this->frame.eye[1] = 0.0f;
  this->frame.eye[2] = 0.0f;
  this->frame.up[0] = 0.0f;
  this->frame.up[1] = 0.0f;
  this->frame.up[2] = 0.0f;
  this->camera.yaw = 0.0f;
  this->shake.enabled = 0;
  this->unknown_01B4 = nullptr;
  this->shake.start = 0;
  *reinterpret_cast<unsigned char*>(&this->shake.unknown_0010) = 0;
}

CView::~CView() {
  if (this->unknown_01B4) {
    delete this->unknown_01B4;
  }
}

// OFFSET: 0x00ADC150
// FUNCTION: fn_17928
void CView::Reset() {
  this->camera.Reset();
  this->frame.up[0] = 0;
  this->frame.up[1] = 1;
  this->frame.up[2] = 0;
}

// OFFSET: 0x00ADC1D0
// FUNCTION: fn_17929
void CView::Slot2() {
}
