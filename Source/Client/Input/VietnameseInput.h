#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_VIETNAMESEINPUT_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_VIETNAMESEINPUT_H

extern unsigned char g_telexToneKeys[6];
extern unsigned short g_vietLowerChars[74];
extern unsigned short g_vietUpperChars[74];

class CInputMode {
public:
  // INLINED
  CInputMode() : m_lastKey(127), m_toneIndex(-1), m_markKey(-1) {
  }

  virtual int IsModifierKey(char key, int count, char vowel) = 0;
  virtual int Convert(char key, char vowel, int index) = 0;
  virtual void Reset();

  /* 0004 */ char m_lastKey;
  /* 0008 */ int m_toneIndex;
  /* 000C */ int m_markKey;
};

// VTABLE: vt_01911
class TELEXInputMode : public CInputMode {
public:
  TELEXInputMode();
  int IsModifierKey(char key, int count, char vowel) override;
  int Convert(char key, char vowel, int index) override;
};

// VTABLE: vt_01622
class VNIInputMode : public CInputMode {
public:
  VNIInputMode();
  int IsModifierKey(char key, int count, char vowel) override;
  int Convert(char key, char vowel, int index) override;
};

class CVietnameseInput {
public:
  CVietnameseInput();
  int Release();
  int Compose(char key, unsigned short* output, int* replaced);
  bool SetMode(int type);
  int IsComposeKey(char key);
  void Reset(int resetMode);

  /* 0000 */ CInputMode* m_mode;
  /* 0004 */ int m_type;
  /* 0008 */ unsigned int m_count;
  /* 000C */ char m_keys[10];
  /* 0016 */ unsigned short m_chars[10];
  /* 002A */ char m_vowel;
  /* 002C */ int m_state;
  /* 0030 */ int m_index;
};

#endif
