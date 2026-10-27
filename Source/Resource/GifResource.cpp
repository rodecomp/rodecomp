#include "Resource/GifResource.h"

#include "File/File.h"
#include "Resource/GifLzw.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

namespace {
#pragma pack(push, 1)
  struct GifHeader {
    /* 0000 */ char signature[3];
    /* 0003 */ char version[3];
  };

  struct GifScreenDescriptor {
    /* 0000 */ std::uint16_t width;
    /* 0002 */ std::uint16_t height;
    /* 0004 */ std::uint8_t flags;
    /* 0005 */ std::uint8_t background;
    /* 0006 */ std::uint8_t aspect;
  };

  struct GifImageDescriptor {
    /* 0000 */ std::uint16_t left;
    /* 0002 */ std::uint16_t top;
    /* 0004 */ std::uint16_t width;
    /* 0006 */ std::uint16_t height;
    /* 0008 */ std::uint8_t flags;
  };

  struct GifGraphicControl {
    /* 0000 */ std::uint8_t size;
    /* 0001 */ std::uint8_t flags;
    /* 0002 */ std::uint16_t delay;
    /* 0004 */ std::uint8_t transparentIndex;
  };
#pragma pack(pop)

  static_assert(sizeof(GifHeader) == 6);
  static_assert(sizeof(GifScreenDescriptor) == 7);
  static_assert(sizeof(GifImageDescriptor) == 9);
  static_assert(sizeof(GifGraphicControl) == 5);

  // INLINED
  bool IsGifSignature(const void* data) {
    const auto* bytes = static_cast<const unsigned char*>(data);
    return bytes[0] == 'G' && bytes[1] == 'I' && bytes[2] == 'F';
  }
}

// INLINED
CGifRes::CGifRes(BitmapResourceContext& context)
    : CBitmapRes(context), loopCount(0), unknown_0132(0), currentFrame(0), lastFrameTime(0), animated(false), frameChanged(false) {
}

// INLINED
CGifRes::~CGifRes() = default;

// OFFSET: 0x00749910
// FUNCTION: fn_07852
CGifRes* CGifRes::Create() {
  return new CGifRes(*this->m_context);
}

// OFFSET: 0x00749A10
// FUNCTION: fn_07853
bool CGifRes::Load(const char* name) {
  CFile file;
  bool result;
  if (name && file.Open(name, 0, true, false)) {
    result = this->Decode(name, file.GetData(), file.GetSize(), false);
    file.Close();
  } else {
    result = false;
  }

  return result;
}

// OFFSET: 0x00749B60
// FUNCTION: fn_07854
bool CGifRes::Decode(const char* name, const void* data, int bytes, bool iconOnly) {
  if (IsGifSignature(data)) {
    return this->DecodeGif(data, bytes, iconOnly);
  }

  return CBitmapRes::Decode(name, data, bytes, iconOnly);
}

// OFFSET: 0x00749C60
// FUNCTION: fn_07855
void CGifRes::Unload() {
  for (std::vector<Frame>::iterator it = this->frames.begin(); it != this->frames.end(); ++it) {
    if (it->second == this->image.pixels) {
      this->image.pixels = nullptr;
    }

    delete[] it->second;
  }

  this->frames.clear();
  this->loopCount = 0;
  this->unknown_0132 = 0;
  this->currentFrame = 0;

  if (this->image.pixels) {
    delete this->image.pixels;
    this->image.pixels = nullptr;
  }

  this->image.height = 0;
  this->image.width = 0;
  this->image.alpha = 0;
  if (this->image.auxiliary) {
    delete[] static_cast<unsigned char*>(this->image.auxiliary);
  }

  this->image.auxiliary = nullptr;
}

// OFFSET: 0x00749D20
// FUNCTION: fn_07856
void CGifRes::Slot02() {
  const std::size_t count = this->frames.size();
  if (count <= 1 || this->currentFrame >= count) {
    this->frameChanged = false;
    return;
  }

  const DWORD now = timeGetTime();
  if (now < this->lastFrameTime + 10 * this->frames[this->currentFrame].first) {
    this->frameChanged = false;
    return;
  }

  this->lastFrameTime = now;
  this->currentFrame = this->currentFrame + 1 < this->frames.size() ? this->currentFrame + 1 : 0;
  this->image.pixels = this->frames[this->currentFrame].second;
  this->frameChanged = true;
}

// OFFSET: 0x00749DD0
// FUNCTION: fn_07857
bool CGifRes::IsTransient() {
  return this->animated;
}

// OFFSET: 0x0074A5C0
// FUNCTION: fn_07865
void CGifRes::CreateBlank(int width, int height) {
  this->Unload();
  this->image.width = width;
  this->image.height = height;
  this->image.alpha = 0;

  std::uint32_t* pixels = new std::uint32_t[width * height];
  std::memset(pixels, 0xFF, 4 * width * height);
  this->frames.push_back(Frame(0, pixels));
  this->image.pixels = pixels;
}

// OFFSET: 0x0074B300
// FUNCTION: fn_07870
bool CGifRes::DecodeGif(const void* data, int bytes, bool iconOnly) {
  if (!IsGifSignature(data)) {
    return false;
  }

  const auto* start = static_cast<const unsigned char*>(data);
  GifHeader header;
  memcpy_s(&header, sizeof(header), start, sizeof(header));
  const unsigned char* cursor = start + sizeof(header);

  GifScreenDescriptor screen;
  memcpy_s(&screen, sizeof(screen), cursor, sizeof(screen));
  cursor += sizeof(screen);
  if (iconOnly == true && (screen.width != 24 || screen.height != 24)) {
    return false;
  }

  const int screenWidth = screen.width;
  const int screenHeight = screen.height;
  this->image.height = screenHeight;
  this->image.width = screenWidth;

  std::uint16_t globalCount = 256;
  std::uint32_t* globalPalette = nullptr;
  if (screen.flags & 0x80) {
    globalCount = static_cast<std::uint16_t>(1 << ((screen.flags & 7) + 1));
    globalPalette = new std::uint32_t[globalCount];
    for (int i = 0; i < globalCount; ++i) {
      globalPalette[i] = 0xFF000000u | (cursor[0] << 16) | (cursor[1] << 8) | cursor[2];
      cursor += 3;
    }
  }

  std::uint8_t transparent = 0;
  std::uint8_t transparentIndex = 0;
  std::uint16_t delay = 0;
  while (bytes > cursor - start) {
    std::uint8_t blockType;
    memcpy_s(&blockType, sizeof(blockType), cursor, sizeof(blockType));
    ++cursor;

    if (blockType == '!') {
      std::uint8_t label;
      memcpy_s(&label, sizeof(label), cursor, sizeof(label));
      ++cursor;

      if (label == 0xF9) {
        GifGraphicControl control;
        memcpy_s(&control, sizeof(control), cursor, sizeof(control));
        cursor += sizeof(control);
        transparentIndex = control.transparentIndex;
        transparent = control.flags & 1;
        delay = std::max<std::uint16_t>(10, control.delay);
      } else if (label == 0xFF) {
        std::uint8_t blockSize;
        memcpy_s(&blockSize, sizeof(blockSize), cursor, sizeof(blockSize));
        ++cursor;

        std::string identifier;
        identifier = std::string(cursor, cursor + blockSize);
        cursor += blockSize;

        constexpr char netscape[] = {'N', 'E', 'T', 'S', 'C', 'A', 'P', 'E'};
        if (identifier.find(netscape, 0, sizeof(netscape)) != std::string::npos) {
          std::uint16_t subBlock;
          memcpy_s(&subBlock, sizeof(subBlock), cursor, sizeof(subBlock));
          cursor += sizeof(subBlock);

          std::uint16_t loops;
          memcpy_s(&loops, sizeof(loops), cursor, sizeof(loops));
          cursor += sizeof(loops);
          this->loopCount = loops > 0 ? loops + 1 : 0;
        }
      }

      while (*cursor != 0) {
        ++cursor;
      }
    } else if (blockType == ',') {
      GifImageDescriptor descriptor;
      memcpy_s(&descriptor, sizeof(descriptor), cursor, sizeof(descriptor));
      cursor += sizeof(descriptor);

      std::uint16_t localCount = 0;
      std::uint32_t* localPalette = nullptr;
      const std::uint8_t localTable = descriptor.flags & 0x80;
      if (localTable) {
        localCount = static_cast<std::uint16_t>(1 << ((descriptor.flags & 7) + 1));
        localPalette = new std::uint32_t[localCount];
        for (int i = 0; i < localCount; ++i) {
          localPalette[i] = 0xFF000000u | (cursor[0] << 16) | (cursor[1] << 8) | cursor[2];
          cursor += 3;
        }
      }

      std::uint8_t minimumCodeSize;
      memcpy_s(&minimumCodeSize, sizeof(minimumCodeSize), cursor, sizeof(minimumCodeSize));
      ++cursor;

      int compressedBytes = 0;
      for (const unsigned char* block = cursor; *block != 0; block += *block + 1) {
        compressedBytes += *block;
      }

      unsigned char* compressed = new unsigned char[compressedBytes];
      unsigned char* output = compressed;
      while (*cursor != 0) {
        const std::uint8_t length = *cursor;
        ++cursor;
        memcpy_s(output, compressedBytes - (output - compressed), cursor, length);
        cursor += length;
        output += length;
      }

      ++cursor;

      unsigned char* indices = new unsigned char[descriptor.width * descriptor.height];
      if (DecodeGifLzw(minimumCodeSize, compressed, compressedBytes, indices) > 0) {
        const unsigned int pixelCount = screenHeight * screenWidth;
        std::uint32_t* pixels = new std::uint32_t[pixelCount];

        std::uint32_t* palette = globalPalette;
        std::uint16_t paletteCount = globalCount;
        if (localTable) {
          palette = localPalette;
          paletteCount = localCount;
        }

        if (this->frames.empty()) {
          std::memset(pixels, palette[screen.background], 4 * pixelCount);
        } else {
          memcpy_s(pixels, 4 * pixelCount, this->frames.back().second, 4 * pixelCount);
        }

        for (std::uint16_t y = 0; y < descriptor.height; ++y) {
          for (std::uint16_t x = 0; x < descriptor.width; ++x) {
            const std::uint32_t color = palette[std::min<std::uint16_t>(indices[y * descriptor.width + x], paletteCount)];
            if (this->frames.empty() || transparent != 1 || color != palette[std::min<std::uint16_t>(transparentIndex, paletteCount)]) {
              const unsigned int position = (descriptor.top + y) * screenWidth + x + descriptor.left;
              if (position < pixelCount) {
                pixels[position] = color;
              }
            }
          }
        }

        this->frames.push_back(Frame(delay, pixels));
      }

      if (localPalette) {
        delete[] localPalette;
      }

      delete[] compressed;
      delete[] indices;
    } else if (blockType == ';') {
      delete[] globalPalette;
      if (this->frames.size() != 0) {
        this->image.pixels = this->frames[0].second;
      }

      if (!this->image.pixels) {
        return false;
      }

      this->lastFrameTime = timeGetTime();
      if (this->frames.size() != 0) {
        this->animated = true;
      }

      return true;
    }
  }

  delete[] globalPalette;
  return false;
}
