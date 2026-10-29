#ifndef RAGNAROK_SOURCE_SCRIPT_NLUASTATE_H
#define RAGNAROK_SOURCE_SCRIPT_NLUASTATE_H

struct lua_State;

// VTABLE: vt_01824
class NLuaState {
public:
  NLuaState();
  virtual ~NLuaState();

  // INLINED
  lua_State* GetState() const {
    return this->m_state;
  }
  // INLINED
  bool IsValid() const {
    return this->m_isValid;
  }

  /* 0004 */ lua_State* m_state;
  /* 0008 */ bool m_isValid;
};

// VTABLE: vt_01825
class NLuaThread {
public:
  explicit NLuaThread(NLuaState& parent);
  virtual ~NLuaThread();

  /* 0004 */ lua_State* m_state;
  /* 0008 */ bool m_isValid;
};

#endif
