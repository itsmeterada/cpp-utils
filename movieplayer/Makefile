PROGS	= opencv_movieplay sdl2_movieplay
ALL:	$(PROGS)

CC = gcc
CXX = g++

ifeq ($(shell uname),Darwin)
else
endif
CXXFLAGS += -D_THREAD_SAFE
CXXFLAGS += -O0 -g -Wall
CXXFLAGS += -std=c++11
CXXFLAGS += `pkg-config opencv4 --cflags`
CXXFLAGS2 = `pkg-config sdl2 --cflags`
CXXFLAGS2 += `pkg-config --cflags libavformat libavcodec libswscale`

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

sdl2_movieplay.o: sdl2_movieplay.cpp
	rm -f $*.o
	$(CXX) -c $(CXXFLAGS) $(CXXFLAGS2) $*.cpp

#$(PROGS): $(OBJS)
#	rm -f $@
#	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

opencv_movieplay: opencv_movieplay.o
	rm -f $@
	$(CXX) -o $@ $@.o `pkg-config opencv4 --libs` $(LDFLAGS)

sdl2_movieplay: sdl2_movieplay.o
	rm -f $@
	$(CXX) -o $@ $@.o `pkg-config sdl2 --libs` `pkg-config --libs libavformat libavcodec libswscale libavutil` $(LDFLAGS) 

clean:
	rm -f $(PROGS) *.o core
