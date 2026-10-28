#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Audio/MilesRuntime.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Audio/WaveResource.h"

#include <cstdlib>
#include <new>
#include <string>

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
    ShowErrorDialog(GetRenderSystemHolder()->system, (std::string("Wave :: Cannot find File : ") + std::string(name)).c_str(), 0);

    return false;
  }

  const std::uint32_t size = file.GetSize();
  this->Unload();

  void* raw = ::operator new(size);
  file.Read(raw, size);
  file.Close();

  auto& miles = MilesRuntime::Get();
  const int type = miles.fileType(raw, size);

  if (type == 2) {
    MilesSoundInfo info;
    this->milesAllocated = 1;
    miles.wavInfo(raw, &info);
    miles.decompressADPCM(&info, &this->data, nullptr);
    std::free(raw);

    return true;
  }

  if (type == 13) {
    this->milesAllocated = 1;
    miles.decompressASI(raw, size, name, &this->data, nullptr, nullptr);
    std::free(raw);

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
      MilesRuntime::Get().freeLocked(this->data);
      this->milesAllocated = 0;
      return;
    }

    std::free(this->data);
  }

  this->milesAllocated = 0;
}

// INLINED
void RegisterWaveResource(ResourceMgr& resourceMgr, CWave& prototype) {
  resourceMgr.Register("wav", "wav\\", &prototype);
}
