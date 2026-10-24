#ifndef RAGNAROK_SOURCE_RESOURCE_TEXTURENAMES_H
#define RAGNAROK_SOURCE_RESOURCE_TEXTURENAMES_H

#include <string>

class ResourceMgr;
struct SkinInfo;

struct TextureNames {
  ResourceMgr& resources;
  SkinInfo& skin;
  const bool& activeMode;
  const int& service;
  const std::string& servicePrefix;

  const char* Resolve(const char* name);
  const char* ServiceName(const char* name);
};

#endif
