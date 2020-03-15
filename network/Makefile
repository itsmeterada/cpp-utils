PROGS	= getifaddrs
ALL:	$(PROGS)

CC = gcc

ifeq ($(shell uname),Darwin)
else
endif
CFLAGS += -D_THREAD_SAFE
CFLAGS += -O0 -g
#CFLAGS += -std=c++11

ifeq ($(shell uname),Darwin)
LIBS =
#LIBS += -framework GLUT -framework OpenGL -framework Cocoa
#LIBS += -L/opt/X11/lib
else
LIBS = 
endif
LDFLAGS = $(LIBS)

.cpp.o:
	rm -f $*.o
	$(CXX) -c $(CXXFLAGS) $*.cpp

#$(PROGS): $(OBJS)
#	rm -f $@
#	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

getifaddrs: getifaddrs.o
	rm -f $@
	$(CC) -o $@ $@.o $(LDFLAGS)

clean:
	rm -f $(PROGS) *.o core
