#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/World/TerrainResource.h"

#include <cstring>

// OFFSET: 0x00748A70
// FUNCTION: fn_07833
C3dAttr::C3dAttr() : width(0), height(0), cellSize(5) {
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
  char magic[5]{};
  file.Read(magic, 4);

  if (std::strcmp(magic, "GRAT") != 0) {
    ShowErrorDialog(GetRenderSystemHolder()->system, "Block Attr ", 0);
    file.Close();
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
    const auto count = static_cast<std::uint32_t>(columns) * static_cast<std::uint32_t>(rows);
    this->m_cells.resize(count);
    this->width = columns;
    this->height = rows;
    file.Read(this->m_cells.data(), count * sizeof(TerrainHeightCell));

    if (minor >= 3) {
      for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
          this->m_cells[static_cast<unsigned>(row) * static_cast<unsigned>(columns) + static_cast<unsigned>(column)].type &= 0x7FFFFFFFu;
        }
      }
    }

    file.Close();
  } else {
    ShowErrorDialog(GetRenderSystemHolder()->system, "C3dAttr::Unsupported Version", 0);
  }

  file.Close();
  return true;
}

// OFFSET: 0x00748840
// FUNCTION: fn_07830
void C3dAttr::Unload() {
  this->width = 0;
  this->height = 0;
}

// INLINED
TerrainHeight C3dAttr::Grid() const {
  return {this->width, this->height, this->cellSize, this->m_cells.data()};
}
