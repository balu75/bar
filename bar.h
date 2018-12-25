#ifndef BAR_H
#define BAR_H

#include <string>

using namespace std;

string GetStdoutFromCommand(string cmd);

void XSetRoot(string name);

void ShowNotification(string heading, string msg);

#endif
