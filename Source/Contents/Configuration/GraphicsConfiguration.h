#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_GRAPHICSCONFIGURATION_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_GRAPHICSCONFIGURATION_H

#include <windows.h>

class LuaScript;
struct ScriptOptions;
struct AdapterConfiguration;
struct OptionWriter;

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

  void Load(LuaScript& script, ScriptOptions& options, int& screenWidth, int& screenHeight);
  AdapterConfiguration Adapter() const;

  void Save(OptionWriter& output) const;
};

#endif
