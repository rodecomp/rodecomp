#include "Client/Application/ErrorDialog.h"
#include "Contents/Movies/BinkMovie.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"

// OFFSET: 0x007AF6D0
// FUNCTION: fn_09154
BinkMovie::BinkMovie(BinkMovieContext& context) : m_handle(nullptr), m_state(0), m_loop(0), m_context(&context) {
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
    this->m_context->runtime.close(this->m_handle);
    this->m_handle = nullptr;
  }

  std::vector<unsigned char>().swap(this->m_bytes);
}

// OFFSET: 0x007AF840
// FUNCTION: fn_09158
bool BinkMovie::Open(const char* name, int loop, bool memory) {
  this->Close();
  auto& context = *this->m_context;

  if (memory) {
    if (!context.file.Open(name)) {
      context.file.Close();
      return false;
    }

    const auto* bytes = static_cast<const unsigned char*>(context.file.Data());
    this->m_bytes.assign(bytes, bytes + context.file.Size());
    context.file.Close();

    this->m_handle = context.runtime.open(this->m_bytes.data(), 0x04104000u);
  } else {
    this->m_handle = context.runtime.open(name, 0x00104000u);
  }

  if (!this->m_handle) {
    ShowErrorDialog(context.renderSystem, context.runtime.error(), 0);
    return false;
  }

  this->m_loop = loop;
  this->m_state = 1;
  return true;
}

// OFFSET: 0x007AF990
// FUNCTION: fn_09159
bool BinkMovie::Update() {
  auto& runtime = this->m_context->runtime;

  if (this->m_state != 1 || runtime.wait(this->m_handle) != 0) {
    return false;
  }

  runtime.doFrame(this->m_handle);

  while (runtime.shouldSkip(this->m_handle) != 0) {
    runtime.nextFrame(this->m_handle);
    runtime.doFrame(this->m_handle);
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

  runtime.nextFrame(this->m_handle);
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

  this->m_context->runtime.copyToBuffer(this->m_handle, locked.pixels, locked.pitch, locked.height, 0, 0, 5);
  texture->Unlock();
  const RECT rectangle{0, 0, static_cast<LONG>(locked.width), static_cast<LONG>(locked.height)};
  texture->Update(&rectangle);

  return true;
}
