#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWNOTIFICATIONS_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWNOTIFICATIONS_H

class WindowNotifications {
public:
  virtual void Notify(int type, const char* text, unsigned color) = 0;

protected:
  ~WindowNotifications() = default;
};

#endif
