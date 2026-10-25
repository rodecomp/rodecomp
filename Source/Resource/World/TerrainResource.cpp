#include "Client/Application/ErrorDialog.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/World/TerrainResource.h"

#include <cstring>

// OFFSET: 0x00748A70
// FUNCTION: fn_07833
C3dAttr::C3dAttr(TerrainResourceContext& context) : width(0), height(0), cellSize(5), m_context(&context) {
}

// INLINED
C3dAttr::~C3dAttr() = default;

// OFFSET: 0x00748410
// FUNCTION: fn_07828
C3dAttr* C3dAttr::Create() {
  return new C3dAttr(*this->m_context);
}

// OFFSET: 0x007484E0
// FUNCTION: fn_07829
bool C3dAttr::Load(const char* name) {
  auto& file = this->m_context->file;
  file.Open(name);
  ResourceByteReader input{static_cast<const unsigned char*>(file.Data()), static_cast<unsigned>(file.Size())};
  char magic[5]{};
  input.Read(magic, 4);

  if (std::strcmp(magic, "GRAT") != 0) {
    ShowErrorDialog(this->m_context->renderSystem, "Block Attr ", 0);
    file.Close();
    return false;
  }

  unsigned char major;
  unsigned char minor;
  input.Read(&major, 1);
  input.Read(&minor, 1);

  if (major == 1 && minor <= 3) {
    int columns;
    int rows;
    input.Read(&columns, sizeof(columns));
    input.Read(&rows, sizeof(rows));
    this->Unload();
    const auto count = static_cast<std::uint32_t>(columns) * static_cast<std::uint32_t>(rows);
    this->m_cells.resize(count);
    this->width = columns;
    this->height = rows;
    input.Read(this->m_cells.data(), count * sizeof(TerrainHeightCell));

    if (minor >= 3) {
      for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
          this->m_cells[static_cast<unsigned>(row) * static_cast<unsigned>(columns) + static_cast<unsigned>(column)].type &= 0x7FFFFFFFu;
        }
      }
    }

    file.Close();
  } else {
    ShowErrorDialog(this->m_context->renderSystem, "C3dAttr::Unsupported Version", 0);
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

TerrainHeight C3dAttr::Grid() const {
  return {this->width, this->height, this->cellSize, this->m_cells.data()};
}
