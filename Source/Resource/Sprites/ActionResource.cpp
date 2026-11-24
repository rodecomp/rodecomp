#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
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

    return static_cast<int>(coordinate / scale);
  }

  // INLINED
  __forceinline bool SpriteImageSize(const char* spriteName, const SpriteFrameCell& cell, int& width, int& height) {
    auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));

    if (!sprite) {
      return false;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];

    if (!image) {
      return false;
    }

    width = image->doubleWidth != 0 ? image->width * 2 : image->width;
    height = image->doubleHeight != 0 ? image->height * 2 : image->height;
    return true;
  }
}

// INLINED
CActRes::~CActRes() = default;

// OFFSET: 0x00745FB0
// FUNCTION: fn_07767
CActRes::CActRes() {
  this->maximumCells = 0;
}

// OFFSET: 0x007436B0
// FUNCTION: fn_07749
CActRes* CActRes::Create() {
  return new CActRes;
}

// OFFSET: 0x007474A0
// FUNCTION: fn_45820
__forceinline void CActRes::AdjustCellPosition(SpriteFrameCell* cell, int width, int height, int doubleWidth,
                                               int doubleHeight) {
  if (!cell) {
    this->Unload();
    ShowErrorDialog("Error While Load Act", 0);
    return;
  }

  const int imageWidth = doubleWidth ? width * 2 : width;
  const int imageHeight = doubleHeight ? height * 2 : height;
  cell->x = RelativeCoordinate(cell->x, imageWidth, cell->scaleX);
  cell->y = RelativeCoordinate(cell->y, imageHeight, cell->scaleY);
}

// OFFSET: 0x007437B0
// FUNCTION: fn_07750
bool CActRes::Load(const char* name) {
  CFile file;
  bool loaded;

  if (!file.Open(name, 0, true, false)) {
    loaded = false;
    ShowErrorDialog((std::string("Cannot find File : ") + std::string(name)).c_str(), 0);
  } else {

  struct Header {
    std::uint16_t magic;
    std::uint16_t version;
    std::uint16_t count;
    unsigned char reserved[10];
  } header;
  file.Read(&header, sizeof(header));

  if (header.magic != 0x4341) {
    ShowErrorDialog("Illegal file format", 0);
    ShowErrorDialog(name, 0);
    file.Close();

    return false;
  }

  std::string spriteName(name);
  spriteName.resize(spriteName.size() - 4);
  spriteName.append(".spr", 4);

  if (header.version > 0x0205) {
    ShowErrorDialog("Unsupported Version. Content may be corrupted", 0);
    file.Close();
    return false;
  }

  this->InitializeAnimations(header.count);

  for (unsigned animationIndex = 0; animationIndex < this->animations.size(); ++animationIndex) {
    std::vector<ActionFrame>& animation = this->animations[animationIndex];
    int frameCount;
    file.Read(&frameCount, sizeof(frameCount));
    animation.resize(frameCount);

    for (int frameIndex = 0; frameIndex < frameCount; ++frameIndex) {
      std::int32_t range1[4];
      std::int32_t range2[4];
      file.Read(range1, sizeof(range1));
      file.Read(range2, sizeof(range2));
      int cellCount;
      file.Read(&cellCount, sizeof(cellCount));
      ActionFrame& frame = animation[frameIndex];
      frame.sound = -1;
      frame.cellCount = cellCount;
      frame.cells.resize(std::min(static_cast<unsigned>(cellCount), 1000u));
      frame.anchors.clear();
      frame.anchorCount = 0;
      std::memcpy(frame.rangeBounds, range1, sizeof(range1));
      std::memcpy(frame.rangeBounds + 4, range2, sizeof(range2));
      if (this->maximumCells < cellCount) {
        this->maximumCells = cellCount;
      }

      for (int index = 0; index < cellCount; ++index) {
        SpriteFrameCell& cell = frame.cells[index];
        int width;
        int height;

        if (header.version < 0x0200) {
          file.Read(&cell, 16);
          cell.color = 0xFFFFFFFFu;
          cell.scaleX = 1.0f;
          cell.scaleY = 1.0f;
          cell.rotation = 0;
          cell.imageType = 0;
          continue;
        }

        if (header.version < 0x0204) {
          struct LegacyCell {
            std::int32_t x;
            std::int32_t y;
            std::int32_t image;
            std::int32_t mirrored;
            SpriteCellColor color;
            float scale;
            std::int32_t rotation;
            std::int32_t imageType;
          } legacy;
          file.Read(&legacy, sizeof(legacy));
          cell.colorChannels.alpha = legacy.color.alpha;
          cell.colorChannels.red = legacy.color.red;
          cell.colorChannels.green = legacy.color.green;
          cell.colorChannels.blue = legacy.color.blue;
          cell.x = legacy.x;
          cell.y = legacy.y;
          cell.rotation = legacy.rotation;
          cell.mirrored = legacy.mirrored;
          cell.imageType = legacy.imageType;
          cell.image = legacy.image;
          cell.scaleX = legacy.scale;
          cell.scaleY = legacy.scale;

          if (cell.image == -1 || !SpriteImageSize(spriteName.c_str(), cell, width, height)) {
            continue;
          }
        } else {
          file.Read(&cell, sizeof(cell));

          if (header.version >= 0x0205) {
            int imageWidth;
            int imageHeight;
            file.Read(&imageWidth, sizeof(imageWidth));
            file.Read(&imageHeight, sizeof(imageHeight));

            if (cell.image == -1) {
              continue;
            }

            width = imageWidth;
            height = imageHeight;
          } else if (cell.image == -1 || !SpriteImageSize(spriteName.c_str(), cell, width, height)) {
            continue;
          }
        }

        this->AdjustCellPosition(&cell, width, height, 0, 0);
      }

      if (header.version >= 0x0200) {
        file.Read(&frame.sound, sizeof(frame.sound));

        if (header.version == 0x0200) {
          frame.sound = -1;
        }
      }

      if (header.version >= 0x0203) {
        int count;
        file.Read(&count, sizeof(count));
        frame.anchorCount = count;
        frame.anchors.resize(static_cast<unsigned>(count));

        for (int index = 0; index < count; ++index) {
          struct AnchorRecord {
            int unused;
            ActionAnchor anchor;
          } anchor;
          file.Read(&anchor, sizeof(anchor));
          frame.anchors[index] = anchor.anchor;
        }
      }
    }
  }

  if (header.version >= 0x0201) {
    int count;
    file.Read(&count, sizeof(count));
    this->sounds.resize(static_cast<unsigned>(count));

    for (int index = 0; index < count; ++index) {
      char sound[40];
      file.Read(sound, sizeof(sound));
      this->sounds[index] = sound;
    }
  }

  if (header.version >= 0x0202) {
    for (int index = 0; index < header.count; ++index) {
      file.Read(&this->speeds[index], sizeof(float));
    }
  }

  file.Close();
  loaded = true;
  }

  return loaded;
}

// OFFSET: 0x007445A0
// FUNCTION: fn_07751
void CActRes::Unload() {
  this->animations.clear();
  this->sounds.clear();
}

// OFFSET: 0x00746CF0
// FUNCTION: fn_46343
void CActRes::InitializeAnimations(unsigned animationCount) {
  this->Unload();
  this->animations.resize(animationCount);
  this->speeds.resize(animationCount, 4.0f);
}

// OFFSET: 0x00746ED0
// FUNCTION: fn_46342
void ActionFrame::InitializeCells(unsigned cellCount) {
  this->cellCount = cellCount;
  this->sound = -1;
  this->cells.resize((std::min)(1000u, cellCount), SpriteFrameCell{});
  this->anchors.clear();
  this->anchorCount = 0;
}

// OFFSET: 0x00746F80
// FUNCTION: fn_07783
const std::vector<ActionFrame>& CActRes::Animation(unsigned index) const {
  if (index < this->animations.size()) {
    return this->animations[index];
  }

  // STATIC: 0x0138D700
  // GLOBAL: gl_25519
  static const std::vector<ActionFrame> missingAnimation;
  return missingAnimation;
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
  if (index < this->speeds.size()) {
    return this->speeds[index];
  }

  return 4.0f;
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
  return (std::max)(static_cast<unsigned>(frames.size()), 1u);
}

// INLINED
CActRes::CActRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25546
CActRes g_actPrototype("act", "sprite\\");
