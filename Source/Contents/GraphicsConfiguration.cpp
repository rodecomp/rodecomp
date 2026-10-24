#include "Contents/GraphicsConfiguration.h"
#include "Contents/OptionWriter.h"
#include "Contents/ScriptOptions.h"
#include "Render/RenderAdapter.h"
#include "Script/LuaScript.h"

#include <algorithm>
#include <cstring>

// INTERNAL
void GraphicsConfiguration::Load(LuaScript& script, ScriptOptions& options, int& screenWidth, int& screenHeight) {
  const int resetPolicy = std::max(script.GlobalInteger("RenderSystemReset"), 0);
  const int backendPolicy = std::max(script.GlobalInteger("RenderSystemDefault"), 0);
  this->reset = options.Integer("RenderSystemReset", 1);

  if (this->reset != resetPolicy) {
    this->backend = backendPolicy;
    this->reset = resetPolicy;
  } else {
    this->backend = options.Integer("RENDERSYSTEM", backendPolicy);
  }

  this->deviceGuid = options.Guid("DX9DEVICEID", GUID{});
  const std::string name = options.String("DX9DEVICENAME", "");
  strncpy_s(this->deviceName, sizeof(this->deviceName), name.c_str(), _TRUNCATE);
  this->fullscreen = options.Integer("ISFULLSCREENMODE", 0);
  this->width = options.Integer("WIDTH", 1024);
  this->height = options.Integer("HEIGHT", 768);
  this->oldWidth = options.Integer("OLD_WIDTH", 0);
  this->oldHeight = options.Integer("OLD_HEIGHT", 0);

  if (static_cast<unsigned>(this->height) < 664) {
    this->width = 1024;
    this->height = 768;
  }

  screenWidth = this->width;
  screenHeight = this->height;
  this->bitsPerPixel = options.Integer("BITPERPIXEL", 16);
  OSVERSIONINFOA version{};
  version.dwOSVersionInfoSize = sizeof(version);
  GetVersionExA(&version);

  if (version.dwMajorVersion >= 6 && version.dwMinorVersion >= 2) {
    this->bitsPerPixel = 32;
  }
}

// INTERNAL
AdapterConfiguration GraphicsConfiguration::Adapter() const {
  return {this->backend, this->drawGuid, this->renderGuid, this->deviceGuid, this->deviceName};
}

// INTERNAL
void GraphicsConfiguration::Save(OptionWriter& output) const {
  output.Integer("RenderSystemReset", this->reset);
  output.Integer("RENDERSYSTEM", this->backend);
  output.Guid("DX9DEVICEID", this->deviceGuid);
  output.String("DX9DEVICENAME", this->deviceName);
  output.Integer("ISFULLSCREENMODE", this->fullscreen);
  output.Integer("WIDTH", this->width);
  output.Integer("HEIGHT", this->height);
  // Original saves current dimensions under OLD_* rather than retained old values.
  output.Integer("OLD_WIDTH", this->width);
  output.Integer("OLD_HEIGHT", this->height);
  output.Integer("BITPERPIXEL", this->bitsPerPixel);
}
