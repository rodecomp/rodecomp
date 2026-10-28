#include "Contents/NavigationManager.h"
#include "Resource/ResourceMgr.h"

#include <cstring>

// OFFSET: 0x00B96980
// FUNCTION: fn_19588
NavigationTarget CNavigationManager::GetCurrentTarget() const {
  return this->currentTarget;
}

// OFFSET: 0x00B96A10
// FUNCTION: fn_19589
std::string CNavigationManager::GetCurrentTargetName() const {
  std::string name;
  void* object = this->currentTarget.object;
  if (object != nullptr) {
    const int type = this->currentTarget.type;
    if (type == 0 || type == 1 || ((type == 2 || type == 3 || type == 6) && (object = static_cast<CNavi_Object*>(object)->GetInfo()) != nullptr)) {
      name.assign(static_cast<CNaviInfo*>(object)->GetName());
    }
  }

  return name;
}

// OFFSET: 0x00B96AE0
// FUNCTION: fn_19590
NavigationTarget CNavigationManager::GetSavedTarget() const {
  return this->savedTarget;
}

// OFFSET: 0x00B96B70
// FUNCTION: fn_19591
CNaviInfo* CNavigationManager::GetDestinationInfo() {
  return this->data.GetInfo();
}

// OFFSET: 0x00B96B80
// FUNCTION: fn_19592
NaviCell* CNavigationManager::GetDestinationCell(NaviCell* out) {
  this->data.GetCell(out);
  return out;
}

// OFFSET: 0x00B96F10
// FUNCTION: fn_19594
void CNavigationManager::ClearData() {
  this->unknown_12B4 = false;
  this->data.Clear();
}

// OFFSET: 0x00B97090
// FUNCTION: fn_19596
bool CNavigationManager::IsCurrentTarget(const char* name) const {
  if (name == nullptr) {
    return false;
  }

  void* object = this->currentTarget.object;
  if (object == nullptr) {
    return false;
  }

  const int type = this->currentTarget.type;
  if (type != 0 && type != 1) {
    if (type != 2 && type != 3 && type != 6) {
      return false;
    }

    object = static_cast<CNavi_Object*>(object)->GetInfo();
    if (object == nullptr) {
      return false;
    }
  }

  const CNaviInfo* info = static_cast<CNaviInfo*>(object);
  if (std::strcmp(info->GetName(), name) == 0) {
    return true;
  }

  const std::string resolved = ResourceMgr::Instance().aliases.ResolveMapName(name);
  return std::strcmp(info->GetName(), resolved.c_str()) == 0;
}

// OFFSET: 0x00B971D0
// FUNCTION: fn_19597
int CNavigationManager::GetCurrentTargetType() const {
  return this->currentTarget.type;
}

// OFFSET: 0x00BA0AE0
// FUNCTION: fn_19635
int CNavigationManager::SaveCurrentTarget() {
  const int type = this->currentTarget.type;
  if (type == 6) {
    return -92;
  }
  if ((type != 1 && type != 2 && type != 3 && type != 0) || !this->currentTarget.object) {
    return -97;
  }
  this->savedTarget = this->currentTarget;
  return 0;
}
