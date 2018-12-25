#!/bin/bash

g++ *.cc -O2 -s -lX11 `pkg-config --cflags --libs gdk-pixbuf-2.0 glib-2.0 libnotify` -o bar
