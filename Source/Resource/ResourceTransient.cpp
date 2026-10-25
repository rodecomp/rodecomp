#include "Resource/ResourceMgr.h"

// OFFSET: 0x00AEFDB0
// FUNCTION: fn_18186
void ResourceMgr::UpdateTransient() {
  const auto end = this->m_transient.end();

  for (auto it = this->m_transient.begin(); it != end; ++it) {
    (*it)->Slot02();
  }
}
