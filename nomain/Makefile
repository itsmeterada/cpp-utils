PROGS	= nomain
ALL:	$(PROGS)

SRCS = nomain.c
OBJS = $(SRCS:.c=.o)

CC = gcc

ifeq ($(shell uname),Darwin)
else
endif
CFLAGS += -D_THREAD_SAFE
CFLAGS += -O3 -g
#CFLAGS += -std=c++11

ifeq ($(shell uname),Darwin)
LIBS =
#LIBS += -framework GLUT -framework OpenGL -framework Cocoa
#LIBS += -L/opt/X11/lib
else
LIBS = 
endif
LDFLAGS = $(LIBS) -nostartfiles

.c.o:
	rm -f $*.o
	$(CC) -c $(CFLAGS) $*.c

.cpp.o:
	rm -f $*.o
	$(CXX) -c $(CXXFLAGS) $*.cpp

$(PROGS): $(OBJS)
	rm -f $@
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(PROGS) *.o core
