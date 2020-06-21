ifndef OSTYPE
#OSTYPE = Windows
#OSTYPE = Mac
OSTYPE = Linux
endif

PROGS	= SimdFaceDetection
ALL:	$(PROGS)

CC = gcc
CXX = g++

SRCS	= SimdFaceDetection.cpp
OBJS	= $(SRCS:.cpp=.o)

HOMEDIR = /home/axengr

#AILIA_SDK = $(AILIADIR)
AILIA_SDK = $(HOMEDIR)/trees/ax/ailia_1_21_0
#POSE_SDK = ../../ailia_1_1_1_pose_estimator/library/

SIMD_SDK = $(HOMEDIR)/trees/Simd

#CXXFLAGS = `pkg-config --cflags freeglut`
#CXXFLAGS += `pkg-config opencv --cflags`
CXXFLAGS += `pkg-config opencv4 --cflags`

ifeq ($(OSTYPE),Mac)
CXXFLAGS += -std=c++11 -stdlib=libc++
endif

CXXFLAGS += -D_THREAD_SAFE
#CXXFLAGS += -O3 -g
CXXFLAGS += -O0 -g
CXXFLAGS += -mavx -mavx2 -msse4.1 -msse4.2 -maes -mfma
ifeq ($(OSTYPE),Linux)
CXXFLAGS += -DLINUX
CXXFLAGS += -I$(SIMD_SDK)/src
endif

#CXXFLAGS += -I./ailia_include -I./pose_include -Wl,-rpath,./ailia_lib
CXXFLAGS += -I$(AILIA_SDK)/export
CXXFLAGS += -I$(AILIA_SDK)/library/include
#CXXFLAGS += -I./$(POSE_SDK)/include
#CXXFLAGS =  -Ofast
#CXXFLAGS +=  -g # -DDEBUG
#LIBS = `sdl2-config --libs`
#LIBS = `pkg-config SDL2_ttf --libs`
LIBS = -lglut -lGLU -lGL
#LIBS += `pkg-config opencv --libs`
LIBS += `pkg-config opencv4 --libs`

ifeq ($(OSTYPE),Windows)
#LIBS += ./$(AILIA_SDK)/windows/x64/ailia.lib
LIBS += $(AILIA_SDK)/library/windows/x64/ailia.lib
#LIBS += ./$(POSE_SDK)/windows/x64/ailia_pose_estimate.lib
else ifeq ($(OSTYPE),Mac)
LIBS += -rpath $(AILIA_SDK)/mac/
LIBS += ./$(AILIA_SDK)/mac/libailia.dylib
#LIBS += ./$(POSE_SDK)/mac/libailia_pose_estimate.dylib
else
#LIBS += -L$(AILIA_SDK)/bin -lailia
#LIBS += -L$(AILIA_SDK)/library/linux -lailia
LIBS += -L$(SIMD_SDK)/linux-build -lSimd -lpthread
#LIBS += $(AILIA_SDK)/bin/libailia.so
#LIBS += ./$(POSE_SDK)/mac/libailia_pose_estimate.dylib
endif

LDFLAGS = $(LIBS)

.cpp.o:
	rm -f $*.o
	$(CXX) -c $(CXXFLAGS) $*.cpp

$(PROGS): $(OBJS)
	rm -f $@
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(PROGS) $(OBJS) core
