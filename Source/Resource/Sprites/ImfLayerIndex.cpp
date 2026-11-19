#include "Resource/Sprites/ImfResource.h"

// OFFSET: 0x00750540
// FUNCTION: fn_07937
void CImfRes::Reset() {
}

// OFFSET: 0x00750690
// FUNCTION: fn_45831
int CImfRes::LayerCount() const {
  return static_cast<int>(this->m_imfData.size());
}

// OFFSET: 0x00750660
// FUNCTION: fn_45830
int CImfRes::ActionCount(int layer) const {
  return static_cast<int>(this->m_imfData[layer].size());
}

// OFFSET: 0x007506B0
// FUNCTION: fn_45832
int CImfRes::MotionCount(int layer, int action) const {
  return static_cast<int>(this->m_imfData[layer][action].cx.size());
}

// OFFSET: 0x007505E0
// FUNCTION: fn_45829
int CImfRes::MaximumMotionCount(int layer) const {
  int maximum = 0;
  for (int action = 0; action < this->ActionCount(layer); ++action) {
    const int count = this->MotionCount(layer, action);
    if (maximum < count) {
      maximum = count;
    }
  }
  return maximum;
}

// OFFSET: 0x00750720
// FUNCTION: fn_45833
int CImfRes::LayerPriority(int layer, int action, int motion) const {
  return this->m_imfData[layer][action].priority[motion];
}

// OFFSET: 0x00750550
// FUNCTION: fn_07938
int CImfRes::GetLayer(int priority, int action, int motion) {
  if (priority >= 15 || action >= this->m_maxAction[priority] ||
      motion >= this->m_maxMotion[priority][action]) {
    return -1;
  }

  for (int layer = 0; layer < this->LayerCount(); ++layer) {
    if (this->LayerPriority(layer, action, motion) == priority) {
      return layer;
    }
  }

  return -1;
}

// OFFSET: 0x007506E0
// FUNCTION: fn_07939
POINT CImfRes::GetLongPoint(int layer, int action, int motion) {
  POINT point;
  point.x = this->m_imfData[layer][action].cx[motion];
  point.y = this->m_imfData[layer][action].cy[motion];
  return point;
}
