CC 					= g++
CFLAGS 				= -Wall
LD					= g++

EXE					= Motorcross
SRCS 				= Motorcross.cpp imageloader.cpp views.cpp vec3f.cpp
LIBS 				= -lglut -lGL -lGLU 

all: $(EXE)

$(EXE):	 $(SRCS)
	$(CC) $(CFLAGS) -o $(EXE) $(SRCS) $(LIBS)

clean:
	rm -f $(EXE)