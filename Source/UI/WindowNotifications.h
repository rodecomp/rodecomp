#ifndef RAGNAROK_SOURCE_UI_WINDOWNOTIFICATIONS_H
#define RAGNAROK_SOURCE_UI_WINDOWNOTIFICATIONS_H

// Source boundary for UIWindowMgr notification routing.
// Type 1 displays text; type 3 restores/shows the message window and sends
// its reset message. Implementations must retain those real owner operations.
class WindowNotifications {
public:
  virtual void Notify(int type, const char* text, unsigned color) = 0;

protected:
  ~WindowNotifications() = default;
};

#endif
