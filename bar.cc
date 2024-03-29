#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "bar.h"
using namespace std;

static const string BAT_CAPACITY_FILE = "/sys/class/power_supply/BAT0/capacity";
static const string BAT_STATUS_FILE = "/sys/class/power_supply/BAT0/status";

static const string WLAN_COMMAND =
  "iwconfig 2>/dev/null | sed -n 's/^.*ESSID:\"\\(.*\\)\".*$/\\1/p'";

static const string TEMP1_FILE =
  "/proc/acpi/ibm/thermal";

static const string VOLUME_COMMAND =
  "/home/thomas/bin/get_volume.sh";

static const int CAP_WARNING = 20;

int last_capacity = 100;

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

string GetBattCapacity(bool isLoop) {
   ifstream infile(BAT_CAPACITY_FILE);
   string bat_capacity;
   infile >> bat_capacity;
   int capacity_as_number = stoi(bat_capacity);
   if (last_capacity >= CAP_WARNING && capacity_as_number < CAP_WARNING) {
       if (isLoop) {
           ShowNotification("Battery Low", "");
       }
   }
   last_capacity = capacity_as_number;
   return bat_capacity;
}

void GetEssid(ostringstream &oss) {
  string result = GetStdoutFromCommand(WLAN_COMMAND);
  if (result == "") return;
  oss << "  " << result.substr(0, result.size() - 1);
}

void GetTemp(ostringstream &oss) {
  ifstream infile(TEMP1_FILE);
  string temp;
  getline(infile, temp);
  if (temp.length() >= 17) {
	  oss  << "  " << temp.substr(14,2) <<"°";
  }
}

void GetVolume(ostringstream &oss) {
  string result = GetStdoutFromCommand(VOLUME_COMMAND);
  if (result == "") return;
  oss << "  " << result.substr(0, result.size() - 1) << "%";
}

void setBar(bool isLoop) {
  ostringstream oss;

  oss.clear();
  oss.str("");

  GetVolume(oss);
  GetTemp(oss);
  GetEssid(oss);

  oss  << "  " << GetTime()
    << "  " << GetBattCapacity(isLoop) << "% "
    << GetBattStatus().substr(0,1);

  XSetRoot(oss.str());
}

int main(int argc, char* argv[]) {
  bool isLoop = true;
  if ( argc == 2 && strcmp(argv[1], "noloop") == 0 ) {
    isLoop = false;
  }

  if (isLoop) {
    while(1) {
      setBar(isLoop);
      sleep(2);
    }
  } else {
    setBar(isLoop);
  }
}
