#ifndef RAGNAROK_SOURCE_FILE_MAPNAME_H
#define RAGNAROK_SOURCE_FILE_MAPNAME_H

#include <string>

// Removes three bytes starting four bytes before the final '@', if that '@'
// has at least four preceding bytes, then formats at most 15 bytes plus NUL.
// Returned storage belongs to the original GetCurrentThread pseudo-handle map.
// The pseudo-handle is shared across threads: later calls overwrite the same
// buffer. This is deliberately not thread-local storage and is not synchronized.
const char* ScriptMapName(std::string name);

#endif
