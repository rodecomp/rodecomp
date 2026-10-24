#include "Contents/WhisperBlockMgr.h"
#include "Network/TextEncoding.h"

#include <new>

// STATIC: 0x01389940
// GLOBAL: gl_24357
CWhisperBlockMgr* g_whisperBlockMgr;

// OFFSET: 0x00610E60
// FUNCTION: fn_04322
CWhisperBlockMgr::CWhisperBlockMgr() : m_enabled(true), m_tracking(false), m_changes(0) {
}

// OFFSET: 0x00610F70
// FUNCTION: fn_04323
CWhisperBlockMgr::~CWhisperBlockMgr() = default;

// OFFSET: 0x00611040
// FUNCTION: fn_04325
bool CWhisperBlockMgr::Initialize() {
  if (g_whisperBlockMgr) {
    return false;
  }
  g_whisperBlockMgr = new (std::nothrow) CWhisperBlockMgr;
  if (!g_whisperBlockMgr) {
    return false;
  }
  if (!g_whisperBlockMgr->m_enabled) {
    delete g_whisperBlockMgr;
    g_whisperBlockMgr = nullptr;
    return false;
  }
  return true;
}

// OFFSET: 0x00611140
// FUNCTION: fn_04326
void CWhisperBlockMgr::Shutdown() {
  if (g_whisperBlockMgr) {
    delete g_whisperBlockMgr;
    g_whisperBlockMgr = nullptr;
  }
}

// OFFSET: 0x00611000
// FUNCTION: fn_04324
void CWhisperBlockMgr::Clear() {
  if (this->m_tracking) {
    ++this->m_changes;
  }
  this->m_names.clear();
}

// OFFSET: 0x00611160
// FUNCTION: fn_04327
void CWhisperBlockMgr::Add(std::string name) {
  if (this->m_tracking) {
    ++this->m_changes;
  }
  this->m_names.emplace(name.c_str());
}

// OFFSET: 0x00611370
// FUNCTION: fn_04328
bool CWhisperBlockMgr::Contains(std::string name) const {
  return this->m_names.find(std::string(name.c_str())) != this->m_names.end();
}

// OFFSET: 0x006114D0
// FUNCTION: fn_04329
void CWhisperBlockMgr::Remove(std::string name) {
  if (this->m_tracking) {
    ++this->m_changes;
  }
  this->m_names.erase(std::string(name.c_str()));
}

// OFFSET: 0x006115F0
// FUNCTION: fn_04330
void CWhisperBlockMgr::Reset() {
  this->m_names.clear();
  this->m_tracking = false;
  this->m_changes = 0;
}

// OFFSET: 0x005BB190
// FUNCTION: fn_03454
void CWhisperBlockMgr::Export(Json::Value& values) const {
  for (const std::string& name : this->m_names) {
    const std::string copy = name;
    values.append(ToUtf8(copy, g_textCodePage));
  }
}
