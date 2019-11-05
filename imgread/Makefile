PROGS	= imgread
ALL:	$(PROGS)

CC = gcc
CXX = g++

ifeq ($(shell uname),Darwin)
else
endif
CXXFLAGS += -D_THREAD_SAFE
CXXFLAGS += -O3 -g
CXXFLAGS += -std=c++11

ifeq ($(shell uname),Darwin)
LIBS =
LIBS += -framework GLUT -framework OpenGL -framework Cocoa
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

imgread: imgread.o
	rm -f $@
	$(CXX) -o $@ $@.o $(LDFLAGS)

clean:
	rm -f $(PROGS) *.o core
