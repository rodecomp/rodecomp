#ifndef RAGNAROK_SOURCE_RESOURCE_GIFRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_GIFRESOURCE_H

#include "Resource/Bitmap/BitmapResource.h"

#include <cstdint>
#include <utility>
#include <vector>
#include <windows.h>

class CGifRes : public CBitmapRes {
public:
  using Frame = std::pair<std::uint16_t, std::uint32_t*>;

  explicit CGifRes(BitmapResourceContext& context);
  ~CGifRes() override;
  void Slot02() override;
  bool IsTransient() override;
  CGifRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  bool Decode(const char* name, const void* data, int bytes, bool iconOnly) override;

  void CreateBlank(int width, int height);
  bool DecodeGif(const void* data, int bytes, bool iconOnly);

  /* 0124 */ std::vector<Frame> frames;
  /* 0130 */ std::uint16_t loopCount;
  /* 0132 */ std::uint16_t unknown_0132;
  /* 0134 */ std::uint16_t currentFrame;
  /* 0138 */ DWORD lastFrameTime;
  /* 013C */ bool animated;
  /* 013D */ bool frameChanged;
};

#endif
