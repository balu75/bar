#include <iostream>
#include <string>
#include <X11/Xlib.h>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

static const string BAT_CAPACITY_FILE = "/sys/class/power_supply/BAT0/capacity";
static const string BAT_STATUS_FILE = "/sys/class/power_supply/BAT0/status";
static const string WLAN_COMMAND = "iwconfig 2>/dev/null | sed -n 's/^.*ESSID:\"\\(.*\\)\".*$/\\1/p'";

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

string GetTime() {
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);

   ostringstream oss;
   oss << std::put_time(&tm, "%d.%m %H%M");
   return oss.str();
}

string GetBattStatus() {
   ifstream infile(BAT_STATUS_FILE);
   string bat_status;
   infile >> bat_status;
   return bat_status;
}

string GetBattCapacity() {
   ifstream infile(BAT_CAPACITY_FILE);
   string bat_capacity;
   infile >> bat_capacity;
   return bat_capacity;
}

string GetEssid() {
  string result = GetStdoutFromCommand(WLAN_COMMAND);
  return result.substr(0, result.size()-1);
}

int main() {
  while(1) {
    ostringstream oss;
    oss << GetEssid() << " " << GetTime() << " "
      << GetBattCapacity() << GetBattStatus().substr(0,1);

    XSetRoot(oss.str().c_str());
    sleep(2);
  }
}
