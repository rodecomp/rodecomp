#include "Client/Application/ErrorDialog.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"

#include "Utility/Math/ScalarConversion.h"
#include <algorithm>
#include <cstring>

namespace {
  // INLINED
  int RelativeCoordinate(int position, int extent, float scale) {
    const int center = extent / 2 + extent % 2;
    const float offset = static_cast<float>(center) * scale;
    const float coordinate = static_cast<float>(position) - offset;

    return TruncateToInt(coordinate / scale);
  }
}

// OFFSET: 0x007436B0
// FUNCTION: fn_07767
CActRes::CActRes(ActionResourceContext& context) : maximumCells(0), m_context(&context) {
}

// INLINED
CActRes::~CActRes() = default;

// OFFSET: 0x007436B0
// FUNCTION: fn_07749
CActRes* CActRes::Create() {
  return new CActRes(*this->m_context);
}

// OFFSET: 0x007437B0
// FUNCTION: fn_07750
bool CActRes::Load(const char* name) {
  auto& context = *this->m_context;
  auto& file = context.file;
  struct CloseFile {
    ResourceBytes& file;

    ~CloseFile() {
      file.Close();
    }
  } close{file};

  if (!file.Open(name)) {
    const std::string message = std::string("Cannot find File : ") + name;
    ShowErrorDialog(context.renderSystem, message.c_str(), 0);
    return false;
  }

  ResourceByteReader input{static_cast<const unsigned char*>(file.Data()), static_cast<unsigned>(file.Size())};
  struct Header {
    std::uint16_t magic;
    std::uint16_t version;
    std::uint16_t count;
    unsigned char reserved[10];
  } header;
  input.Read(&header, sizeof(header));

  if (header.magic != 0x4341) {
    ShowErrorDialog(context.renderSystem, "Illegal File Format", 0);
    ShowErrorDialog(context.renderSystem, name, 0);
    file.Close();

    return false;
  }

  std::string spriteName(name);
  spriteName.resize(spriteName.size() - 4);
  spriteName.append(".spr", 4);

  if (header.version > 0x0205) {
    ShowErrorDialog(context.renderSystem, "Unsupported Version. Content may be corrupted", 0);
    file.Close();
    return false;
  }

  this->Unload();
  this->animations.resize(header.count);
  this->speeds.resize(header.count, 4.0f);

  for (auto& animation : this->animations) {
    std::uint32_t frameCount;
    input.Read(&frameCount, sizeof(frameCount));
    animation.resize(frameCount);

    for (ActionFrame& frame : animation) {
      input.Read(frame.unknown_0000, sizeof(frame.unknown_0000));
      int cellCount;
      input.Read(&cellCount, sizeof(cellCount));
      frame.sound = -1;
      frame.cellCount = cellCount;
      frame.cells.resize(std::min(static_cast<unsigned>(cellCount), 1000u));
      frame.anchors.clear();
      frame.anchorCount = 0;
      this->maximumCells = std::max(this->maximumCells, cellCount);

      for (int index = 0; index < cellCount; ++index) {
        SpriteFrameCell& cell = frame.cells[index];

        if (header.version < 0x0200) {
          input.Read(&cell, 16);
          cell.color = 0xFFFFFFFFu;
          cell.scaleX = 1.0f;
          cell.scaleY = 1.0f;
          cell.unknown_001C = 0;
          cell.imageType = 0;
          continue;
        }

        if (header.version < 0x0204) {
          struct LegacyCell {
            std::int32_t x;
            std::int32_t y;
            std::int32_t image;
            std::int32_t mirrored;
            std::uint32_t color;
            float scale;
            std::int32_t rotation;
            std::int32_t imageType;
          } legacy;
          input.Read(&legacy, sizeof(legacy));
          cell = {legacy.x, legacy.y, legacy.image, legacy.mirrored, legacy.color, legacy.scale, legacy.scale, legacy.rotation, legacy.imageType};
        } else {
          input.Read(&cell, sizeof(cell));
        }

        if (header.version >= 0x0205) {
          int width;
          int height;
          input.Read(&width, sizeof(width));
          input.Read(&height, sizeof(height));

          if (cell.image != -1) {
            cell.x = RelativeCoordinate(cell.x, width, cell.scaleX);
            cell.y = RelativeCoordinate(cell.y, height, cell.scaleY);
          }
        } else if (cell.image != -1) {
          auto* sprite = static_cast<CSprRes*>(context.resources.Load(spriteName.c_str()));

          if (sprite) {
            const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];

            if (image) {
              const int width = image->width * ((image->unknown_0004 & 0xFFFFu) != 0 ? 2 : 1);
              const int height = image->height * ((image->unknown_0004 >> 16) != 0 ? 2 : 1);
              cell.x = RelativeCoordinate(cell.x, width, cell.scaleX);
              cell.y = RelativeCoordinate(cell.y, height, cell.scaleY);
            }
          }
        }
      }

      if (header.version >= 0x0200) {
        input.Read(&frame.sound, sizeof(frame.sound));

        if (header.version == 0x0200) {
          frame.sound = -1;
        }
      }

      if (header.version >= 0x0203) {
        int count;
        input.Read(&count, sizeof(count));
        frame.anchorCount = count;
        frame.anchors.resize(static_cast<unsigned>(count));

        for (int index = 0; index < count; ++index) {
          struct AnchorRecord {
            int unused;
            ActionAnchor anchor;
          } anchor;
          input.Read(&anchor, sizeof(anchor));
          frame.anchors[index] = anchor.anchor;
        }
      }
    }
  }

  if (header.version >= 0x0201) {
    int count;
    input.Read(&count, sizeof(count));
    this->sounds.resize(static_cast<unsigned>(count));

    for (int index = 0; index < count; ++index) {
      char sound[40];
      input.Read(sound, sizeof(sound));
      this->sounds[index].assign(sound, std::strlen(sound));
    }
  }

  if (header.version >= 0x0202) {
    for (float& speed : this->speeds) {
      input.Read(&speed, sizeof(speed));
    }
  }

  file.Close();
  return true;
}

// OFFSET: 0x007445A0
// FUNCTION: fn_07751
void CActRes::Unload() {
  this->animations.clear();
  this->sounds.clear();
}

// OFFSET: 0x00746F80
// FUNCTION: fn_07783
const std::vector<ActionFrame>& CActRes::Animation(unsigned index) const {
  if (index < this->animations.size()) {
    return this->animations[index];
  }

  // STATIC: 0x0138D700
  static const std::vector<ActionFrame> missing;
  return missing;
}

// OFFSET: 0x00747050
// FUNCTION: fn_07784
const SpriteFrameCell& ActionFrame::Cell(unsigned index) const {
  if (index < this->cells.size()) {
    return this->cells[index];
  }

  // STATIC: 0x0132A1A8
  static const SpriteFrameCell missing{0, 0, -1, 0, 0xFFFFFFFFu, 1.0f, 1.0f, 0, 0};
  return missing;
}

// OFFSET: 0x00747090
// FUNCTION: fn_07785
float CActRes::Speed(unsigned index) const {
  return index < this->speeds.size() ? this->speeds[index] : 4.0f;
}

// OFFSET: 0x007470E0
// FUNCTION: fn_07786
const char* CActRes::Sound(unsigned index) const {
  return index < this->sounds.size() ? this->sounds[index].c_str() : "";
}

// OFFSET: 0x00747170
// FUNCTION: fn_07787
const ActionFrame& CActRes::Frame(unsigned animation, unsigned frame) const {
  const auto& frames = this->Animation(animation);
  if (frame < frames.size()) {
    return frames[frame];
  }
  if (!frames.empty()) {
    return frames.front();
  }

  // GLOBAL: gl_24507
  static const ActionFrame missing;
  return missing;
}

// OFFSET: 0x00747370
// FUNCTION: fn_07788
unsigned CActRes::FrameCount(unsigned animation) const {
  const auto& frames = this->Animation(animation);
  return frames.empty() ? 1u : static_cast<unsigned>(frames.size());
}
