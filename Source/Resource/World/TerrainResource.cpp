#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/World/TerrainResource.h"
#include "Resource/ResourceMgr.h"

#include <cstring>

// OFFSET: 0x00748A70
// FUNCTION: fn_07833
C3dAttr::C3dAttr() {
  this->width = 0;
  this->height = 0;
  this->cellSize = 5;
}

// INLINED
C3dAttr::~C3dAttr() = default;

// OFFSET: 0x00748410
// FUNCTION: fn_07828
C3dAttr* C3dAttr::Create() {
  return new C3dAttr;
}

// OFFSET: 0x007484E0
// FUNCTION: fn_07829
bool C3dAttr::Load(const char* name) {
  CFile file;
  file.Open(name, 0, true, false);
  char signature[] = "GRAT";
  char magic[5]{};
  file.Read(magic, 4);
  magic[4] = 0;

  if (std::strcmp(magic, signature) != 0) {
    ShowErrorDialog("Block Attr ", 0);
    return false;
  }

  unsigned char major;
  unsigned char minor;
  file.Read(&major, 1);
  file.Read(&minor, 1);

  if (major == 1 && minor <= 3) {
    int columns;
    int rows;
    file.Read(&columns, sizeof(columns));
    file.Read(&rows, sizeof(rows));
    this->Unload();
    this->m_cells.resize(static_cast<std::uint32_t>(columns) * static_cast<std::uint32_t>(rows));
    this->width = columns;
    this->height = rows;
    file.Read(this->m_cells.data(), rows * columns * sizeof(TerrainHeightCell));

    if (major > 1 || (major == 1 && minor >= 3)) {
      for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
          this->m_cells[row * columns + column].type &= 0x7FFFFFFFu;
        }
      }
    }

    file.Close();
  } else {
    ShowErrorDialog("C3dAttr::Unsupported Version", 0);
  }

  return true;
}

// OFFSET: 0x00748840
// FUNCTION: fn_07830
void C3dAttr::Unload() {
  this->width = 0;
  this->height = 0;
}

// INLINED
C3dAttr::C3dAttr(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25548
C3dAttr g_gatPrototype("gat", "");
