#ifndef RAGNAROK_SOURCE_FILE_RESOURCEPATH_H
#define RAGNAROK_SOURCE_FILE_RESOURCEPATH_H

#include <cstdint>
#include <string>

// Reconstructed names for the original resource path and existence helpers.
std::string MakeResourcePath(std::string filename);
bool ResourceExists(const char* filename, int looseOnly);

// Full loose-file reader used after archive lookup. size must be non-null.
// Publishes the low file-size DWORD after opening, even when allocation fails;
// a successful allocation is returned regardless of ReadFile's status/count.
// Caller owns the returned region and releases it with VirtualFree/MEM_RELEASE.
void* ReadLooseResource(const char* filename, std::uint32_t* size);

#endif
