EXE=main
CAMERA=./Camera/
SHADERS=./Shaders/
# Main target
all: $(EXE)

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lglfw3 -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DUSEGLEW -DRES=$(RES)
LIBS=-lglew -lglfw -framework Cocoa -framework OpenGL -framework IOKit
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglfw3 -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
main.o: main.cpp CSCIx229.h ./Shaders/compileshaders.h ./Camera/camera.h

#  Create archive
CSCIx229.a:main.o ./Shaders/compileshaders.o ./Camera/camera.o
	ar -rcs $@ $^

./Shaders/compileshaders.a: main.o CSCIx229.o camera.o
# Compile rules./Camera/
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
main:main.o CSCIx229.a 
	g++ $(CFLG) -o $@ $^  $(LIBS)

compileshaders:compileshaders.o main.o CSCIx229.a camera.a
	g++ $(CFLG) -o $@ $^  $(LIBS)./Camera/

./Camera/camera:compileshaders.o main.o CSCIx229.a camera.a
	g++ $(CFLG) -o $@ $^  $(LIBS)./Camera/
#  Clean
clean:
	$(CLEAN)
