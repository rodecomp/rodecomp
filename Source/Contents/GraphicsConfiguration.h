#ifndef RAGNAROK_SOURCE_CONTENTS_GRAPHICSCONFIGURATION_H
#define RAGNAROK_SOURCE_CONTENTS_GRAPHICSCONFIGURATION_H

#include <windows.h>

class LuaScript;
struct ScriptOptions;
struct AdapterConfiguration;
struct OptionWriter;

// Recovered shared graphics configuration globals, grouped by their owner.
// Legacy GUID selection is populated by separate setup paths and retained by Load.
struct GraphicsConfiguration {
  GUID drawGuid;
  GUID renderGuid;
  int fullscreen;
  int width;
  int height;
  int bitsPerPixel;
  int oldWidth;
  int oldHeight;
  int reset;
  int backend;
  GUID deviceGuid;
  char deviceName[32];

  // Source helper for graphics publication within the larger startup loader.
  // Not a separate original entry point.
  void Load(LuaScript& script, ScriptOptions& options, int& screenWidth, int& screenHeight);
  AdapterConfiguration Adapter() const;
  // Graphics portion of the larger option save routine.
  void Save(OptionWriter& output) const;
};

#endif
