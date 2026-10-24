#include "Client/ErrorDialog.h"
#include "Resource/BitmapResource.h"
#include "Resource/MilesRuntime.h"
#include "Resource/ResourceMgr.h"
#include "Resource/WaveResource.h"

#include <cstdlib>
#include <cstring>
#include <string>

// OFFSET: 0x007618D0
CWave::CWave(WaveResourceContext& context) : data(nullptr), milesAllocated(0), m_context(&context) {
}

// OFFSET: 0x00761870
// FUNCTION: fn_08069
CWave::~CWave() = default;

// OFFSET: 0x007613D0
// FUNCTION: fn_08066
CWave* CWave::Create() {
  return new CWave(*this->m_context);
}

// OFFSET: 0x00761470
// FUNCTION: fn_08067
bool CWave::Load(const char* name) {
  auto& context = *this->m_context;
  auto& file = context.file;

  if (!file.Open(name)) {
    const std::string message = std::string("Wave :: Cannot find File : ") + name;
    ShowErrorDialog(context.renderSystem, message.c_str(), 0);
    file.Close();
    return false;
  }

  const std::size_t size = static_cast<std::size_t>(file.Size());
  this->Unload();

  void* raw = std::malloc(size);
  std::memcpy(raw, file.Data(), size);
  file.Close();

  auto& miles = MilesRuntime::Get();
  const int type = miles.fileType(raw, static_cast<unsigned>(size));

  if (type == 2) {
    MilesSoundInfo info{};
    this->milesAllocated = 1;
    miles.wavInfo(raw, &info);
    miles.decompressADPCM(&info, &this->data, nullptr);
    std::free(raw);
    return true;
  }

  if (type == 13) {
    this->milesAllocated = 1;
    miles.decompressASI(raw, static_cast<unsigned>(size), name, &this->data, nullptr, nullptr);
    std::free(raw);
    return true;
  }

  this->data = raw;
  return true;
}

// OFFSET: 0x00761830
// FUNCTION: fn_08068
void CWave::Unload() {
  // In the original binary, this->data is freed but never reset to nullptr.
  // Repeatedly calling Unload without an intervening Load retains a dangling
  // pointer, preserving original memory management semantics.
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

// OFFSET: 0x0044E060
// FUNCTION: fn_45419
void RegisterWaveResource(ResourceMgr& resourceMgr, CWave& prototype) {
  resourceMgr.Register("wav", "wav\\", &prototype);
}
