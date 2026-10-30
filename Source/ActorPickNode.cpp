#include "ActorPickNode.h"
#include "Contents/TerrainPathSearch.h"

#include <map>
#include <string>

// OFFSET: 0x00ADA690
// FUNCTION: fn_17906
CActorPickNode::~CActorPickNode() {
  for (ActorPickInfo* info : this->pickInfos) {
    delete info;
  }
  this->pickInfos.clear();

  for (CActorPickNode* child : this->children) {
    delete child;
  }
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
