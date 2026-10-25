#include "File/ResourcePath.h"
#include "Resource/ResourceFailures.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Threading/NSyncObject.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include <optional>

// OFFSET: 0x00AECEE0
// FUNCTION: fn_18148
bool ResourceNameLess::operator()(const ResourceName* left, const ResourceName* right) const {
  if (left->hash != right->hash) {
    return left->hash < right->hash;
  }

  return std::strcmp(left->text, right->text) < 0;
}

// OFFSET: 0x00AF1620
// FUNCTION: fn_18209
ResourceMgr::ResourceMgr(ResourceFailures& failures) : m_failures(&failures) {
  InitializeCriticalSection(&this->m_lock);
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
CRes* ResourceMgr::Load(const char* name) {
  if (!name) {
    return nullptr;
  }

  std::optional<NSyncObject> lock(std::in_place, &this->m_lock);
  char normalized[260] = {};
  _snprintf_s(normalized, sizeof(normalized), _TRUNCATE, "%s", name);

  for (unsigned char* byte = reinterpret_cast<unsigned char*>(normalized); *byte; ++byte) {
    if (*byte >= 'A' && *byte <= 'Z') {
      *byte += 'a' - 'A';
    }
  }

  const char* dot = std::strrchr(normalized, '.');
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
      this->m_failures->Report(name, resolved);
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

void ResourceMgr::EraseTransient(CRes* resource) {
  auto entry = std::find(this->m_transient.begin(), this->m_transient.end(), resource);
  if (entry != this->m_transient.end()) {
    this->m_transient.erase(entry);
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
void ResourceMgr::ClearExpired(const unsigned& reuseDelay) {
  const unsigned threshold = timeGetTime() - reuseDelay;

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
