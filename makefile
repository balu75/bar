LDLIBS=-lX11 $(shell pkg-config --libs gdk-pixbuf-2.0 glib-2.0 libnotify)
CPPFLAGS=$(shell pkg-config --cflags gdk-pixbuf-2.0 glib-2.0 libnotify)

bar: bar.o helper.o
	g++ -O2 -s $(LDLIBS)-o bar bar.o helper.o

bar.o: bar.cc bar.h
	g++ -c bar.cc

helper.o: helper.cc bar.h
	g++ $(CPPFLAGS) -c helper.cc

clean:
	rm -f *.o bar
