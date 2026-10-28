#include "Contents/NavigationManager.h"

#include "Client/Registry.h"
#include "Utility/RegistryKey.h"

#include <cstdio>
#include <cstring>

// OFFSET: 0x00B88800
// FUNCTION: fn_19376
CNaviInfo* CNavigationManager::FindObject(const char* name) const {
  for (CNaviInfo* info : this->data.destinations) {
    if (info->HasName(name)) {
      return info;
    }
  }
  return nullptr;
}

// OFFSET: 0x00B8D270
// FUNCTION: fn_19434
bool CNaviInfo::HasName(const char* name) const {
  return std::strcmp(this->name.c_str(), name) == 0;
}

// OFFSET: 0x00B97FC0
// FUNCTION: fn_19604
std::string CNavigationManager::SelectedNodeText() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  if (!this->nodes.empty() && this->selectedNode >= 0) {
    if (this->nodes.size() <= static_cast<unsigned>(this->selectedNode)) {
      this->selectedNodeText.assign(buffer, std::strlen(buffer));
      return this->selectedNodeText;
    }

    CNavi_Object* node = this->nodes[this->selectedNode];
    NaviCell cellY;
    NaviCell cellX;
    const int y = node->GetCell(&cellY)->y;
    const int x = node->GetCell(&cellX)->x;
    std::sprintf(buffer, "%s %d %d...", node->GetInfo()->GetName(), x, y);
  } else if (this->unknown_1258 == 2) {
    CNaviInfo* info = this->data.GetInfo();
    if (info != nullptr) {
      NaviCell cellY;
      NaviCell cellX;
      const int y = this->data.GetCell(&cellY)->y;
      const int x = this->data.GetCell(&cellX)->x;
      std::sprintf(buffer, "%s %d %d...", info->GetName(), x, y);
    }
  }

  this->selectedNodeText.assign(buffer, std::strlen(buffer));
  return this->selectedNodeText;
}

// OFFSET: 0x00B981A0
// FUNCTION: fn_19605
std::string CNavigationManager::SubDestinationText() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  CNaviInfo* info = this->data.GetInfo();
  if (info != nullptr) {
    NaviCell cell;
    if (this->data.GetCell(&cell)->x > 0) {
      NaviCell cellY;
      NaviCell cellX;
      const int y = this->data.GetCell(&cellY)->y;
      const int x = this->data.GetCell(&cellX)->x;
      std::sprintf(buffer, ">.%s %d %d", info->GetName(), x, y);
    } else {
      std::sprintf(buffer, ">.%s", info->GetName());
    }
  }

  this->subDestinationText.assign(buffer, std::strlen(buffer));
  return this->subDestinationText;
}

// OFFSET: 0x00B982D0
// FUNCTION: fn_19606
std::string CNavigationManager::DestinationText() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  if (!this->nodes.empty() && this->selectedNode >= 0) {
    if (this->nodes.size() <= static_cast<unsigned>(this->selectedNode)) {
      this->destinationText.assign(buffer, std::strlen(buffer));
      return this->destinationText;
    }

    CNavi_Object* node = this->nodes[this->selectedNode];
    NaviCell cellY;
    NaviCell cellX;
    const int y = node->GetCell(&cellY)->y;
    const int x = node->GetCell(&cellX)->x;
    std::sprintf(buffer, "%s %d %d", node->GetInfo()->GetName(), x, y);
  } else if (this->unknown_1258 == 2) {
    CNaviInfo* info = this->data.GetInfo();
    if (info != nullptr) {
      NaviCell cellY;
      NaviCell cellX;
      const int y = this->data.GetCell(&cellY)->y;
      const int x = this->data.GetCell(&cellX)->x;
      std::sprintf(buffer, "%s %d %d", info->GetName(), x, y);
    }
  }

  this->destinationText.assign(buffer, std::strlen(buffer));
  return this->destinationText;
}

// OFFSET: 0x00B984C0
// FUNCTION: fn_19607
std::string CNavigationManager::DestinationTitle() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  if (!this->nodes.empty() && this->selectedNode >= 0) {
    if (this->nodes.size() <= static_cast<unsigned>(this->selectedNode)) {
      this->destinationTitle.assign(buffer, std::strlen(buffer));
      return this->destinationTitle;
    }

    this->destinationTitle.assign(this->nodes[this->selectedNode]->GetInfo()->GetTitle());
    return this->destinationTitle;
  }

  if (this->unknown_1258 == 2) {
    NaviCell cell;
    this->data.GetCell(&cell);
    CNaviInfo* info = this->data.GetInfo();
    if (info != nullptr) {
      this->destinationTitle.assign(info->GetTitle());
      return this->destinationTitle;
    }
  }

  this->destinationTitle.assign(buffer, std::strlen(buffer));
  return this->destinationTitle;
}

// OFFSET: 0x00B996D0
// FUNCTION: fn_19613
void CNavigationManager::LoadSettings() {
  RegistryKey key;
  std::string subKey(g_subKey);
  subKey += "\Navigation";
  if (key.Open(HKEY_LOCAL_MACHINE, subKey.c_str())) {
    DWORD viewIcon = 0;
    if (key.Query("ViewIcon", REG_DWORD, reinterpret_cast<BYTE*>(&viewIcon), sizeof(viewIcon))) {
      this->viewIcon = viewIcon;
    }
  }
}

// OFFSET: 0x00B9C020
// FUNCTION: fn_19618
float CNavigationManager::FitToSquare(int* offsetX, int* offsetY, int width, int height, float size) const {
  float scale;
  if (width > height) {
    scale = size / width;
    *offsetY = static_cast<int>((width - height) * 0.5f * scale);
    *offsetX = 0;
  } else if (width < height) {
    scale = size / height;
    *offsetX = static_cast<int>((height - width) * 0.5f * scale);
    *offsetY = 0;
  } else {
    *offsetY = 0;
    scale = size / width;
    *offsetX = 0;
  }
  return scale;
}

// OFFSET: 0x00B888F0
// FUNCTION: fn_19378
NaviCell* CNaviData::GetCell(NaviCell* out) {
  out->x = this->cellX;
  out->y = this->cellY;
  return out;
}

// OFFSET: 0x00B88920
// FUNCTION: fn_19380
CNaviInfo* CNaviData::GetInfo() {
  return this->info;
}

// OFFSET: 0x00B88A00
// FUNCTION: fn_45449
void CNaviData::Clear() {
  this->unknown_0034.clear();
  this->info = nullptr;
  this->cellX = 0;
  this->cellY = 0;
  this->range = 50000;
  this->unknown_0040.clear();

  for (NaviDataEntry& entry : this->unknown_0028) {
    entry.unknown_0000 = -1;
    entry.unknown_0004 = 0;
    entry.unknown_0008 = 0;
  }
  this->unknown_0028.clear();
}

// OFFSET: 0x00B8CD60
// FUNCTION: fn_19430
CNavi_Object::~CNavi_Object() {
  this->id = -1;
  this->type = 101;
  this->index = -1;
  this->name.clear();
  this->title.clear();
  this->cellX = 0;
  this->cellY = 0;
  this->info = nullptr;
  this->links.clear();
}
