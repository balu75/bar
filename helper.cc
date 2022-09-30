#include "bar.h"

#include <X11/Xlib.h>
#include <iostream>
#include <fstream>
#include <libnotify/notify.h>

const int TEN_SECONDS=10000;

using namespace std;

string GetStdoutFromCommand(string cmd) {
  string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];

  ifstream f(cmd.c_str());
  if (!f.good()) return "";

  cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");
  if (stream) {
    while (!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
}

void XSetRoot(string name) {
   Display *display;

   if (( display = XOpenDisplay(0x0)) == NULL ) {
      cerr << "[bar] cannot open display!";
      exit(1);
   }

   XStoreName(display, DefaultRootWindow(display), name.c_str());
   XSync(display, 0);

   XCloseDisplay(display);
}

void ShowNotification(string heading, string msg) {
  notify_init("bar");

  NotifyNotification* n = notify_notification_new (heading.c_str(),
      msg.c_str(), "dialog-warning");

  notify_notification_set_timeout(n, TEN_SECONDS);

  if (!notify_notification_show(n, 0))
  {
    std::cerr << "show has failed" << std::endl;
  }

  g_object_unref(G_OBJECT(n));

  notify_uninit();
}

