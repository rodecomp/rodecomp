#include "File/ResourcePath.h"
#include "Resource/ResourceFailures.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"
#include "Utility/Threading/NSyncObject.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include <optional>

// GLOBAL: gl_24581
unsigned g_resourceReuseDelay = 15000;

// GLOBAL: gl_24718
unsigned char g_lowercaseTable[256] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
  0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
  0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
  0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
  0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

// OFFSET: 0x00AECEE0
bool ResourceNameLess::operator()(const ResourceName* left, const ResourceName* right) const {
  if (left->hash != right->hash) {
    return left->hash < right->hash;
  }

  return std::strcmp(left->text, right->text) < 0;
}

// INLINED
ResourceMgr::ResourceMgr() : unknown_004C{} {
  InitializeCriticalSection(&this->m_lock);
}

// OFFSET: 0x00AF1620
// FUNCTION: fn_18209
ResourceMgr& ResourceMgr::Instance() {
  // GLOBAL: gl_24488
  static ResourceMgr g_resourceMgr;
  return g_resourceMgr;
}

// OFFSET: 0x00AEE070
// FUNCTION: fn_18169
ResourceMgr::~ResourceMgr() {
  this->m_transient.clear();

  for (auto& resources : this->m_resources) {
    for (auto& [name, resource] : resources) {
      resource->Unload();
      delete resource;
    }

    resources.clear();
  }

  DeleteCriticalSection(&this->m_lock);
}

// OFFSET: 0x00AF04B0
// FUNCTION: fn_18188
void ResourceMgr::Register(const char* extension, const char* prefix, CRes* prototype) {
  const int type = static_cast<int>(this->m_extensions.size());
  this->m_extensions.emplace(extension, type);
  this->m_prototypes.push_back(prototype);
  this->m_prefixes.push_back(prefix);
  this->m_resources.resize(static_cast<std::size_t>(type) + 1);
}

// OFFSET: 0x00AEE5E0
// FUNCTION: fn_18173
int ResourceMgr::FindType(const char* extension) const {
  if (!extension) {
    return -1;
  }

  const auto entry = this->m_extensions.find(extension);
  if (entry != this->m_extensions.end()) {
    return entry->second;
  }

  char text[256];
  std::sprintf(text, "ResourceError : %s is Undefined Ext \n", extension);
  return -1;
}

// OFFSET: 0x00AEE6B0
// FUNCTION: fn_18174
CRes* ResourceMgr::Find(int type, const char* name) const {
  ResourceName key;
  key.Assign(name);
  const auto& resources = this->m_resources[type];
  const auto entry = resources.find(&key);

  return entry == resources.end() ? nullptr : entry->second;
}

// OFFSET: 0x00AEF7D0
// FUNCTION: fn_18181
bool ResourceMgr::Exists(const char* name) {
  if (!name) {
    return false;
  }

  char normalized[260] = {};
  strncpy_s(normalized, sizeof(normalized), name, _TRUNCATE);

  for (unsigned char* byte = reinterpret_cast<unsigned char*>(normalized); *byte; ++byte) {
    if (*byte >= 'A' && *byte <= 'Z') {
      *byte += 'a' - 'A';
    }
  }

  const char* dot = std::strrchr(normalized, '.');
  const int type = this->FindType(dot ? dot + 1 : nullptr);
  if (type < 0) {
    return false;
  }

  const std::string resolved = AddResourcePrefix(this->aliases.Resolve(normalized), this->m_prefixes[type]);
  if (CRes* existing = this->Find(type, resolved.c_str())) {
    existing->lastUse = timeGetTime();
    return true;
  }

  return ResourceExists(resolved.c_str(), 0);
}

// OFFSET: 0x00AEE770
// INLINED
__forceinline CRes* ResourceMgr::LoadNamed(const char* name) {
  std::optional<NSyncObject> lock(std::in_place, &this->m_lock);
  char normalized[260] = {};
  StringFormat(normalized, sizeof(normalized), _TRUNCATE, "%s", name);

  for (char* c = normalized; *c; ++c) {
    *c = g_lowercaseTable[static_cast<unsigned char>(*c)];
  }

  const char* dot = ReverseFindChar(normalized, '.');
  const int type = this->FindType(dot ? dot + 1 : nullptr);
  if (type < 0) {
    return nullptr;
  }

  std::string resolved = AddResourcePrefix(this->aliases.Resolve(normalized), this->m_prefixes[type]);
  if (CRes* existing = this->Find(type, resolved.c_str())) {
    existing->lastUse = timeGetTime();
    return existing;
  }

  CRes* resource = this->m_prototypes[type]->Create();
  if (!resource) {
    return nullptr;
  }

  if (resource->Load(resolved.c_str()) != 1) {
    delete resource;

    lock.reset();
    if (resolved.find("\\map\\") == std::string::npos) {
      g_resourceFailures.Report(name, resolved);
    }

    return nullptr;
  }

  resource->name.Assign(resolved.c_str());
  resource->type = type;
  resource->lastUse = timeGetTime();
  this->m_resources[type].emplace(&resource->name, resource);

  if (resource->IsTransient() == 1) {
    this->m_transient.push_back(resource);
  }

  return resource;
}

// OFFSET: 0x00AEE770
// FUNCTION: fn_18175
CRes* ResourceMgr::Load(const char* name) {
  if (!name) {
    return nullptr;
  }

  return this->LoadNamed(name);
}

// OFFSET: 0x00AEECC0
// FUNCTION: fn_18176
CRes* ResourceMgr::Load(std::string name) {
  if (name.empty()) {
    return nullptr;
  }

  return this->LoadNamed(name.c_str());
}

// OFFSET: 0x00AF0780
// FUNCTION: fn_18190
void ResourceMgr::Remove(CRes* resource) {
  this->EraseTransient(resource);
  auto& resources = this->m_resources[resource->type];
  const auto entry = resources.find(&resource->name);
  entry->second->Unload();
  delete entry->second;
  resources.erase(entry);
}

// INLINED
void ResourceMgr::EraseTransient(CRes* resource) {
  auto entry = std::find(this->m_transient.begin(), this->m_transient.end(), resource);
  if (entry != this->m_transient.end()) {
    this->m_transient.erase(entry);
  }
}

// OFFSET: 0x00AF0680
// FUNCTION: fn_18189
void ResourceMgr::UnloadAll() {
  this->m_transient.clear();

  for (std::size_t type = 0; type < this->m_resources.size(); ++type) {
    auto& resources = this->m_resources[type];
    for (auto entry = resources.begin(); entry != resources.end(); ++entry) {
      entry->second->Unload();
      delete entry->second;
    }

    resources.clear();
  }
}

// OFFSET: 0x00AF0A10
// FUNCTION: fn_18192
void ResourceMgr::ClearUnused(const char* extension) {
  const int type = this->FindType(extension);
  if (type < 0) {
    return;
  }

  auto& resources = this->m_resources[type];

  for (auto entry = resources.begin(); entry != resources.end();) {
    CRes* resource = entry->second;
    if (resource->references != 0) {
      ++entry;
      continue;
    }

    this->EraseTransient(resource);
    resource->Unload();
    delete resource;
    entry = resources.erase(entry);
  }
}

// OFFSET: 0x00AF0AE0
// FUNCTION: fn_18193
void ResourceMgr::ClearUnused() {
  timeGetTime();

  for (auto& resources : this->m_resources) {
    for (auto entry = resources.begin(); entry != resources.end();) {
      CRes* resource = entry->second;
      if (resource->references != 0) {
        ++entry;
        continue;
      }

      this->EraseTransient(resource);
      resource->Unload();
      delete resource;
      entry = resources.erase(entry);
    }
  }
}

// OFFSET: 0x00AF0910
// FUNCTION: fn_18191
void ResourceMgr::ClearExpired() {
  const unsigned threshold = timeGetTime() - g_resourceReuseDelay;

  for (auto& resources : this->m_resources) {
    for (auto entry = resources.begin(); entry != resources.end();) {
      CRes* resource = entry->second;

      if (resource->references != 0 || resource->lastUse >= threshold) {
        ++entry;
        continue;
      }

      this->EraseTransient(resource);
      resource->Unload();
      delete resource;
      entry = resources.erase(entry);
    }
  }
}
