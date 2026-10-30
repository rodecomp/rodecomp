#include "Resource/ImfResource.h"

// OFFSET: 0x00750540
// FUNCTION: fn_07937
void CImfRes::Reset() {
}

// OFFSET: 0x00750550
// FUNCTION: fn_07938
int CImfRes::GetLayer(int priority, int action, int motion) {
  if (priority >= 15 || action >= this->m_maxAction[priority] ||
      motion >= this->m_maxMotion[priority][action]) {
    return -1;
  }

  int numLayer = this->m_imfData.size();
  for (int layer = 0; layer < numLayer; ++layer) {
    if (this->m_imfData[layer][action].priority[motion] == priority) {
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
