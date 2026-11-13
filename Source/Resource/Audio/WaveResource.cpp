#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Audio/MilesRuntime.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Audio/WaveResource.h"

#include <cstdlib>
#include <new>
#include <string>

extern "C" __declspec(dllimport) void __stdcall AIL_mem_free_lock(void* ptr);
extern "C" __declspec(dllimport) int __stdcall AIL_file_type(const void* data, unsigned int size);
extern "C" __declspec(dllimport) int __stdcall AIL_WAV_info(const void* data, MilesSoundInfo* info);
extern "C" __declspec(dllimport) int __stdcall AIL_decompress_ADPCM(const MilesSoundInfo* info, void** out, unsigned int* outSize);
extern "C" __declspec(dllimport) int __stdcall AIL_decompress_ASI(const void* data, unsigned int size, const char* name, void** out, unsigned int* outSize, void* fn);

// OFFSET: 0x007618D0
// FUNCTION: fn_08070
CWave::CWave() : data(nullptr), milesAllocated(0) {
}

// INLINED
CWave::~CWave() = default;

// OFFSET: 0x007613D0
// FUNCTION: fn_08066
CWave* CWave::Create() {
  return new CWave;
}

// OFFSET: 0x00761470
// FUNCTION: fn_08067
bool CWave::Load(const char* name) {
  CFile file;

  if (!file.Open(name, 0, true, false)) {
    ShowErrorDialog((std::string("Wave :: Cannot find File : ") + std::string(name)).c_str(), 0);

    return false;
  }

  const std::uint32_t size = file.GetSize();
  this->Unload();

  char* raw = new char[size];
  file.Read(raw, size);
  file.Close();

  const int type = AIL_file_type(raw, size);

  if (type == 2) {
    MilesSoundInfo info;
    this->milesAllocated = 1;
    AIL_WAV_info(raw, &info);
    AIL_decompress_ADPCM(&info, &this->data, nullptr);
    delete[] raw;

    return true;
  }

  if (type == 13) {
    this->milesAllocated = 1;
    AIL_decompress_ASI(raw, size, name, &this->data, nullptr, nullptr);
    delete[] raw;

    return true;
  }

  this->data = raw;
  return true;
}

// OFFSET: 0x00761830
// FUNCTION: fn_08068
void CWave::Unload() {
  if (this->data != nullptr) {
    if (this->milesAllocated != 0) {
      AIL_mem_free_lock(this->data);
      this->milesAllocated = 0;
      return;
    }

    delete[] static_cast<char*>(this->data);
  }

  this->milesAllocated = 0;
}

// INLINED
void RegisterWaveResource(ResourceMgr& resourceMgr, CWave& prototype) {
  resourceMgr.Register("wav", "wav\\", &prototype);
}

// INLINED
CWave::CWave(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_24585
CWave g_wavePrototype("wav", "wav\\");
