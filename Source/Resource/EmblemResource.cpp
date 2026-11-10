#include "Resource/EmblemResource.h"

#include "Contents/Guild/GuildMgr.h"
#include "File/File.h"
#include "Resource/ResourceMgr.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <zlib.h>

// INLINED
CEmblemRes::~CEmblemRes() = default;

// OFFSET: 0x0074C9F0
// FUNCTION: fn_07880
CEmblemRes* CEmblemRes::Create() {
  return new CEmblemRes;
}

// OFFSET: 0x0074CA90
// FUNCTION: fn_07881
bool CEmblemRes::Load(const char* name) {
  CGuildMgr* guildMgr = g_guildMgr;
  if (guildMgr->IsInvalidEmblem(name) == true) {
    this->gif.CreateBlank(24, 24);
    return true;
  }

  CFile file;
  if (!file.Open(name, 0, true, false)) {
    return false;
  }

  uLongf size = static_cast<uLongf>(static_cast<std::uint32_t>(g_guildMgr->m_timeout) * 1.2);
  Bytef* buffer = new Bytef[size];
  if (uncompress(buffer, &size, static_cast<const Bytef*>(file.GetData()), file.GetSize()) != Z_OK
      || this->gif.Decode("_emblem.bmp", buffer, size, true) != true) {
    guildMgr = g_guildMgr;
    guildMgr->MarkInvalidEmblem(name);
    this->gif.CreateBlank(24, 24);
  }

  file.Close();
  delete[] buffer;
  return true;
}

// OFFSET: 0x0074CD00
// FUNCTION: fn_07882
void CEmblemRes::Unload() {
  this->gif.Unload();
}

// OFFSET: 0x0074CD10
// FUNCTION: fn_07883
void CEmblemRes::OnProcess() {
  this->gif.OnProcess();
}

// OFFSET: 0x0074CD20
// FUNCTION: fn_07884
bool CEmblemRes::IsTransient() {
  return this->gif.IsTransient();
}

// OFFSET: 0x0074CD40
// FUNCTION: fn_07885
void CEmblemRes::MissingFile(const char*) {
}

// OFFSET: 0x0074CDC0
// FUNCTION: fn_07887
CEmblemRes::CEmblemRes() : CRes(), gif() {
}

// INLINED
CEmblemRes::CEmblemRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25554
CEmblemRes g_ebmPrototype("ebm", "..\\_tmpEmblem\\");
