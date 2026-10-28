#include "Resource/ImfResource.h"

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
