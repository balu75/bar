CXX=g++
RM=rm -f
CPPFLAGS=$(shell pkg-config --cflags gdk-pixbuf-2.0 glib-2.0 libnotify)
LDLIBS=-lX11 $(shell pkg-config --libs gdk-pixbuf-2.0 glib-2.0 libnotify)
EXEC=bar
SRCS=bar.cc helper.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) -O2 -s $(LDLIBS) -o $(EXEC) $(OBJS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
