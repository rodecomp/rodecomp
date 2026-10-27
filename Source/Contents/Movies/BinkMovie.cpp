#include "Client/Application/ErrorDialog.h"
#include "Contents/Movies/BinkMovie.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/Texture.h"

#include <new>

// OFFSET: 0x007AF6D0
// FUNCTION: fn_09154
BinkMovie::BinkMovie() : m_handle(nullptr), m_state(0), m_loop(0), m_file(nullptr) {
}

// OFFSET: 0x007AF6F0
// FUNCTION: fn_09155
BinkMovie::~BinkMovie() {
  this->Close();
}

// OFFSET: 0x007AF760
// FUNCTION: fn_09156
void BinkMovie::Close() {
  if (this->m_handle) {
    BinkClose(this->m_handle);
    this->m_handle = nullptr;
  }

  if (this->m_file) {
    this->m_file->Close();
    delete this->m_file;
    this->m_file = nullptr;
  }
}

// OFFSET: 0x007AF840
// FUNCTION: fn_09158
bool BinkMovie::Open(const char* name, int loop, bool memory) {
  this->Close();

  if (memory) {
    this->m_file = new (std::nothrow) CFile;
    if (!this->m_file || !this->m_file->Open(name, 0, true, false)) {
      return false;
    }

    const char* data = static_cast<const char*>(this->m_file->GetData());
    if (!data) {
      return false;
    }

    this->m_handle = BinkOpen(data, 0x04104000u);
  } else {
    this->m_handle = BinkOpen(name, 0x00104000u);
  }

  if (!this->m_handle) {
    ShowErrorDialog(GetRenderSystemHolder()->system, BinkGetError(), 0);
    return false;
  }

  this->m_loop = loop;
  this->m_state = 1;
  return true;
}

// OFFSET: 0x007AF990
// FUNCTION: fn_09159
bool BinkMovie::Update() {
  if (this->m_state != 1 || BinkWait(this->m_handle) != 0) {
    return false;
  }

  BinkDoFrame(this->m_handle);

  while (BinkShouldSkip(this->m_handle) != 0) {
    BinkNextFrame(this->m_handle);
    BinkDoFrame(this->m_handle);
  }

  if (this->m_handle->frame == this->m_handle->frames) {
    if (this->m_loop == 0) {
      this->Close();
      this->m_state = 2;
      return false;
    }

    if (this->m_loop == 1) {
      this->m_handle->frame = 0;
    }
  }

  BinkNextFrame(this->m_handle);
  return true;
}

// OFFSET: 0x007AF7A0
// FUNCTION: fn_09157
bool BinkMovie::CopyTo(CTexture* texture) {
  if (!texture) {
    return false;
  }

  const SurfaceLock locked = texture->Lock();

  if (!locked.success) {
    return false;
  }

  BinkCopyToBuffer(this->m_handle, locked.pixels, locked.pitch, locked.height, 0, 0, 5);
  texture->Unlock();
  const RECT rectangle{0, 0, static_cast<LONG>(locked.width), static_cast<LONG>(locked.height)};
  texture->Update(&rectangle);

  return true;
}
