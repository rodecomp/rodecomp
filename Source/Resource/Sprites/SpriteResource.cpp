#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourcePreload.h"
#include "Resource/ResourceMgr.h"

#include <cstdlib>
#include <cstring>
#include <string>

// INLINED
CSprRes::CSprRes() : indexedCount(0) {
}

// OFFSET: 0x00760F50
// FUNCTION: fn_08060
CSprRes::~CSprRes() = default;

// OFFSET: 0x0075FE90
// FUNCTION: fn_08053
CSprRes* CSprRes::Create() {
  return new CSprRes;
}

// OFFSET: 0x0075FF40
// FUNCTION: fn_08054
bool CSprRes::Load(const char* name) {
  CFile file;
  if (!file.Open(name, 0, true, false)) {
    ShowErrorDialog((std::string("Spr :: Cannot find File : ") + std::string(name)).c_str(), 0);
    return false;
  }

  struct Header {
    std::uint16_t magic;
    std::uint16_t version;
    std::uint16_t indexed;
  } header;
  file.Read(&header, sizeof(header));

  if (header.magic != 0x5053) {
    ShowErrorDialog("Illegal File Format", 0);
    ShowErrorDialog(name, 0);
    return false;
  }

  if (header.version > 0x0201) {
    ShowErrorDialog("Unsupported Version. Content may be corrupted", 0);
    return false;
  }

  this->indexedCount = header.indexed;

  if (header.version >= 0x0101) {
    unsigned char colors[1024];
    file.Seek(-1024, FILE_END);
    file.Read(colors, sizeof(colors));
    file.Seek(sizeof(header), FILE_BEGIN);
    ConvertSpritePalette(this->palette, colors, 256);
  }

  std::uint16_t trueColorCount;

  if (header.version >= 0x0200) {
    file.Read(&trueColorCount, sizeof(trueColorCount));
  }

  for (int index = 0; index < this->indexedCount; ++index) {
    std::uint16_t dimensions[2];
    file.Read(dimensions, sizeof(dimensions));
    auto* image = new SpriteRleImage{};
    image->surface = nullptr;
    image->width = static_cast<std::int16_t>(dimensions[0]);
    image->height = static_cast<std::int16_t>(dimensions[1]);
    image->doubleWidth = 0;
    image->doubleHeight = 0;
    std::uint16_t encodedBytes;

    if (g_spriteHalfWidth == 0 && g_spriteHalfHeight == 0) {
      if (header.version >= 0x0201) {
        file.Read(&encodedBytes, sizeof(encodedBytes));
        auto* runs = new std::uint8_t[encodedBytes];
        image->runs = runs;
        file.Read(runs, encodedBytes);
      } else {
        const int count = image->width * image->height;
        auto* pixels = new std::uint8_t[count];
        file.Read(pixels, count);
        image->runs = this->EncodeSpriteRuns(pixels, image->width, image->height, encodedBytes);
        delete[] pixels;
      }
    } else {
      if (header.version >= 0x0201) {
        file.Read(&encodedBytes, sizeof(encodedBytes));
        auto* runs = new std::uint8_t[encodedBytes];
        file.Read(runs, encodedBytes);
        auto* pixels = new std::uint8_t[dimensions[0] * dimensions[1]];
        const std::uint8_t* current = runs;
        std::uint16_t zeroCount = 0;
        std::uint8_t value;

        for (unsigned y = 0; y < dimensions[1]; ++y) {
          for (unsigned x = 0; x < dimensions[0]; ++x) {
            if (zeroCount == 0) {
              value = *current++;

              if (value == 0) {
                zeroCount = *current++;
              }
            }

            const std::uint16_t before = zeroCount;
            zeroCount = zeroCount != 0 ? zeroCount - 1 : 0;
            pixels[y * dimensions[0] + x] = before == 0 ? value : 0;
          }
        }

        delete[] runs;
        auto* reduced = this->ReduceSprite(pixels, dimensions[0], dimensions[1], g_spriteHalfWidth, g_spriteHalfHeight);
        delete[] pixels;
        image->runs = this->EncodeSpriteRuns(reduced, dimensions[0] / (g_spriteHalfWidth + 1), dimensions[1] / (g_spriteHalfHeight + 1), encodedBytes);
        delete[] reduced;

        if (g_spriteHalfWidth != 0) {
          image->width /= 2;
          image->doubleWidth = 1;
        }

        if (g_spriteHalfHeight != 0) {
          image->height /= 2;
          image->doubleHeight = 1;
        }
      } else {
        const int count = image->width * image->height;
        auto* pixels = new std::uint8_t[count];
        file.Read(pixels, count);
        auto* reduced = this->ReduceSprite(pixels, dimensions[0], dimensions[1], g_spriteHalfWidth, g_spriteHalfHeight);
        delete[] pixels;
        image->runs = this->EncodeSpriteRuns(reduced, dimensions[0] / (g_spriteHalfWidth + 1), dimensions[1] / (g_spriteHalfHeight + 1), encodedBytes);
        delete[] reduced;

        if (g_spriteHalfWidth != 0) {
          image->width /= 2;
          image->doubleWidth = 1;
        }

        if (g_spriteHalfHeight != 0) {
          image->height /= 2;
          image->doubleHeight = 1;
        }
      }
    }

    this->images[0].push_back(image);
  }

  if (header.version >= 0x0200) {
    for (unsigned index = 0; index < trueColorCount; ++index) {
      std::uint16_t dimensions[2];
      file.Read(dimensions, sizeof(dimensions));
      auto* image = new SpriteRleImage{};
      image->width = static_cast<std::int16_t>(dimensions[0]);
      image->height = static_cast<std::int16_t>(dimensions[1]);
      auto* pixels = new std::uint32_t[image->width * image->height];

      for (int y = image->height - 1; y >= 0; --y) {
        for (int x = 0; x < image->width; ++x) {
          std::uint8_t pixel[4];
          file.Read(pixel, sizeof(pixel));
          pixels[y * image->width + x] = pixel[1] | (pixel[2] << 8) | (pixel[3] << 16) | (static_cast<std::uint32_t>(pixel[0]) << 24);
        }
      }

      const int width = g_spriteHalfWidth != 0 ? image->width / 2 : image->width;
      const int height = g_spriteHalfHeight != 0 ? image->height / 2 : image->height;
      CTexture* texture = CreateTexture(width, height, TextureFormat::A4R4G4B4, false, false);
      image->surface = texture;
      texture->Upload(0, 0, image->width, image->height, pixels, 0, 0);
      delete[] pixels;

      if (g_spriteHalfWidth != 0) {
        image->width /= 2;
        image->doubleWidth = 1;
      }

      if (g_spriteHalfHeight != 0) {
        image->height /= 2;
        image->doubleHeight = 1;
      }

      this->images[1].push_back(image);
    }
  }

  file.Close();
  return true;
}

// OFFSET: 0x00760B10
// FUNCTION: fn_08055
void CSprRes::Unload() {
  for (int type = 0; type < 2; ++type) {
    for (auto image = this->images[type].begin(); image != this->images[type].end(); ++image) {
      if (type == 0) {
        delete[] (*image)->runs;
      } else if ((*image)->surface) {
        delete static_cast<const CTexture*>((*image)->surface);
        (*image)->surface = nullptr;
      }

      delete *image;
    }

    this->images[type].erase(this->images[type].begin(), this->images[type].end());
  }
}

// INLINED
CSprRes::CSprRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25563
CSprRes g_sprPrototype("spr", "sprite\\");
