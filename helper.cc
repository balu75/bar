#include "bar.h"

#include <X11/Xlib.h>
#include <iostream>

using namespace std;

string GetStdoutFromCommand(string cmd) {
  string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");
  if (stream) {
    while (!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
}

void XSetRoot(string name){
   Display *display;

   if (( display = XOpenDisplay(0x0)) == NULL ) {
      cerr << "[bar] cannot open display!";
      exit(1);
   }

   XStoreName(display, DefaultRootWindow(display), name.c_str());
   XSync(display, 0);

   XCloseDisplay(display);
}

